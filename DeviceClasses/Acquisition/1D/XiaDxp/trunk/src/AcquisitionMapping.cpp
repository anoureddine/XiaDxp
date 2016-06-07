/*************************************************************************
/*! 
 *  \file   AcquisitionMapping.h
 *  \brief  class AcquisitionMapping 
 *  \author 
 */
/*************************************************************************/

#include <tango.h>
#include <map>
#include <sstream>

#include <yat/utils/XString.h>
#include <yat/time/Timer.h>
#include "AcquisitionMapping.h"



namespace XiaDxp_ns
{

//----------------------------------------------------------------------------------------------------------------------
//- AcquisitionMapping Ctor
//----------------------------------------------------------------------------------------------------------------------
AcquisitionMapping::AcquisitionMapping(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store)
: Acquisition(dev, board_type, store)
{
    INFO_STREAM << "Acquisition::AcquisitionMapping() - [BEGIN]" << endl;
    enable_timeout_msg(false);
    enable_periodic_msg(false);
    set_periodic_msg_period(MAPPING_TASK_PERIODIC_MS);
    m_pixel_advance_task.reset(new PixelAdvanceTask(m_device, m_helper), yat4tango::DeviceTaskExiter());
    //@@TODO : enable this task_pixel_advance_task->go();
    m_module_info_array.clear();
    m_module_info_array.resize(1);
    m_num_map_pixels = 0;
    m_current_pixel = 0;
    INFO_STREAM << "Acquisition::AcquisitionMapping() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- AcquisitionMapping Dtor
//----------------------------------------------------------------------------------------------------------------------
AcquisitionMapping::~AcquisitionMapping(void)
{
    INFO_STREAM << "AcquisitionMapping::~AcquisitionMapping() - [BEGIN]" << endl;
    m_pixel_advance_task.reset();
    INFO_STREAM << "AcquisitionMapping::~AcquisitionMapping() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- Init the XIA library with a new INI configuration file
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::load_config_file(const std::string& file_name)
{
    INFO_STREAM << "AcquisitionMapping::load_config_file() - [BEGIN]" << endl;

    set_status("");
    m_helper->load_config_file(file_name);
    m_num_map_pixels = m_helper->get_num_map_pixels(0); //     

    INFO_STREAM << "m_num_map_pixels = " << m_num_map_pixels<<endl;
    //init DataStore with new configurations
    m_store->init	(get_nb_modules(),
                     get_nb_channels()/get_nb_modules(),
                     m_num_map_pixels,	// num map pixels in mode MAPPING
                     get_nb_bins(),
                     "MAPPING"    
                     );

    INFO_STREAM << "AcquisitionMapping::load_config_file() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::save_config_file(const std::string& file_name)
{
    INFO_STREAM << "AcquisitionMapping::save_config_file() - [BEGIN]" << endl;
    m_helper->save_config_file(file_name);
    INFO_STREAM << "AcquisitionMapping::save_config_file() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::start_acquisition(short accumulate)
{
    INFO_STREAM << "AcquisitionMapping::start_acquisition() - [BEGIN]" << endl;
    set_status("");
    m_helper->stop_acquisition(); //@@TODO : check why 

    // fix the advance pixel mode
    if(m_helper->get_pixel_advance_mode() == "HOST")
    {
        INFO_STREAM << "Pixel Advance Mode: HOST" << endl;
        m_pixel_advance_task->start();
    }
    else
    {
        INFO_STREAM << "Pixel Advance Mode: GATE/SYNCH" << endl;
    }

    // initialize the struct ModuleInfo in order to monitor the progress of the run
    INFO_STREAM<<"nb_modules = "<<get_nb_modules()<<std::endl;
    m_module_info_array.clear();
    m_module_info_array.resize(get_nb_modules());
    for(int i = 0; i<get_nb_modules(); i++)
    {
        m_module_info_array[i].m_buffer_length        = m_helper->get_buffer_length(i);
        m_module_info_array[i].m_current_buffer       = BUFFER_A;
        m_module_info_array[i].m_is_pixel_readout     = false;
    }

    //reset m_current_pixel
    m_current_pixel = 0;
    m_num_map_pixels = m_helper->get_num_map_pixels(0);
    INFO_STREAM << "Map Pixels Number = " << m_num_map_pixels<<endl;

    // start the mapping acquisition
    m_helper->start_acquisition(0); //@@TODO : Always no accumulate in mapping mode? 
    INFO_STREAM << "AcquisitionMapping::start_acquisition() - [END]" << endl;

    // enable/disable collecting in TASK_PERIODIC MSG
    enable_periodic_msg(true);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::stop_acquisition()
{
    INFO_STREAM << "AcquisitionMapping::stop_acquisition() - [BEGIN]" << endl;
    // stop the advance pixel task
    m_pixel_advance_task->stop();
    // stop the mapping acquisition
    m_helper->stop_acquisition();
    //- Post MAPPING_STOP_MSG in order to stop pixeladvance/acquisition/store
    yat::Message* msg = yat::Message::allocate(MAPPING_STOP_MSG, DEFAULT_MSG_PRIORITY, true);
    post(msg);
    INFO_STREAM << "AcquisitionMapping::stop_acquisition() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::collect_data()
{
    //INFO_STREAM<<"AcquisitionMapping::collect_data() - [BEGIN]"<<std::endl;
    int current_pixel  = m_helper->get_current_pixel();
    //INFO_STREAM<<"current acquired  pixel = "<<current_pixel<<std::endl;
    //INFO_STREAM<<"current processed pixel = "<<m_current_pixel<<std::endl;
    if(m_current_pixel != current_pixel)
    {
        //m_current_pixel = current_pixel;
        for (int imod = 0; imod < (int)get_nb_modules(); imod++)
        {
            m_module_info_array[imod].m_is_pixel_readout = false;
            //DEBUG_STREAM<<"m_is_pixel_readout = false"<<std::endl;
        }
    }

    //foreach module, check buffer , i.e is_full() -> readout_buffer() -> switch_buffer())
    for(int i = 0; i<get_nb_modules(); i++)
    {
        DEBUG_STREAM << "check_buffer("<<i<<")" << endl;
        check_buffer(i);
    }

    // check if all mapping pixels are readout
    bool is_read_out_done = is_all_pixel_read_out();
    if ((m_current_pixel >= m_num_map_pixels) && is_read_out_done)
    {
        INFO_STREAM << "All pixels are readout" << endl;
        INFO_STREAM << "\t- m_current_pixel = " << m_current_pixel<<endl;
        INFO_STREAM << "\t- is_all_pixel_read_out = " << (is_read_out_done ? "true" : "false")<<endl;
        stop_acquisition();
    }
    //INFO_STREAM<<"AcquisitionMapping::collect_data() - [END]"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::check_buffer(int module)
{
    //DEBUG_STREAM<<"AcquisitionMapping::check_buffer() - module["<<module<<"] - [BEGIN]"<<std::endl;
    // check if buffer is overrun, throw exception in that case !
    //DEBUG_STREAM << "check buffer - module[" << module <<"]"<< std::endl;
    if(m_helper->is_buffer_overrun(module))
    {
        std::stringstream ss_msg;
        ss_msg<<"BUFFER OVERRUN ! - module[" << module << "]"<<std::endl;
        ERROR_STREAM << "AcquisitionMapping::check_buffer() - "<<ss_msg.str()<< std::endl;
        stop_acquisition();
        on_fault(ss_msg.str());
        Tango::Except::throw_exception("XIA_ERROR",
                                       "Buffer overrun !",
                                       "AcquisitionMapping::check_buffer()");
    }

    // check if buffer is full, readout_buffer() -> switch_buffer() in that case
    if(m_helper->is_buffer_full(module, m_module_info_array[module].m_current_buffer))
    {
        //DEBUG_STREAM<<"buffer["<<m_module_info_array[module].m_current_buffer<<"] is full"<<std::endl;
        readout_buffer(module);
        switch_buffer(module);
    }
    //DEBUG_STREAM<<"AcquisitionMapping::check_buffer() - module["<<module<<"] - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::readout_buffer(int module)
{
    //DEBUG_STREAM<<"AcquisitionMapping::readout_buffer() - module["<<module<<"] - [BEGIN]"<<std::endl;
    //prepare a DataBuffer intended to hold the buffer from the board
    yat::Timer t1;
    m_module_info_array[module].m_is_pixel_readout = true;
    DataBufferContainer* map_buffer = new DataBufferContainer(module, m_module_info_array[module].m_buffer_length);

    // readout acquisition buffer from installed modules
    INFO_STREAM << "\t- readout buffer from board - "<<module<< std::endl;
    m_helper->get_buffer(module, m_module_info_array[module].m_current_buffer, map_buffer->base());

    // tell the hardware that the buffer is ready to be reused    
    //DEBUG_STREAM << "\t- inform the hardware that the buffer is ready to be reused" << endl;
    m_helper->buffer_done(module, m_module_info_array[module].m_current_buffer);

    int starting_pixel = WORD_TO_LONG((int)map_buffer->base()[9], (int)map_buffer->base()[10]);
    int nb_pixels = (int)map_buffer->base()[8];

    // send MappingDataBufferContainer over to MappingDataProcessingTask
    INFO_STREAM    << "\t- send Data Buffer of pixels [" << starting_pixel<< " : " << starting_pixel + nb_pixels - 1<< "] to Data Store."<< std::endl;

    //process the buffer    
    m_store->process_data(map_buffer);
    m_current_pixel = starting_pixel+nb_pixels;
    INFO_STREAM<<"\t- duration readout buffer : "<<t1.elapsed_msec()<<" (ms)"<<std::endl;
    INFO_STREAM<<" "<<std::endl;
    //DEBUG_STREAM<<"AcquisitionMapping::readout_buffer() - module["<<module<<"] - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMapping::switch_buffer(int module)
{
    //DEBUG_STREAM<<"AcquisitionMapping::switch_buffer() - module["<<module<<"] - [BEGIN]"<<std::endl;
    if (m_module_info_array[module].m_current_buffer == BUFFER_A)
    {
        //DEBUG_STREAM << "\t- switch to  BUFFER_B\n" << std::endl;
        m_module_info_array[module].m_current_buffer = BUFFER_B;
        return;
    }

    if (m_module_info_array[module].m_current_buffer == BUFFER_B)
    {
        //DEBUG_STREAM << "\t- switch to  BUFFER_A\n" <<std::endl;
        m_module_info_array[module].m_current_buffer = BUFFER_A;
        return;
    }

    // is that possible?
    Tango::Except::throw_exception("INVALID_STATE",
                                   "m_current_buffer has no valid state",
                                   "AcquisitionMapping::switch_current_buffer()");

    //DEBUG_STREAM<<"AcquisitionMapping::switch_buffer() - module["<<module<<"] - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool AcquisitionMapping::is_all_pixel_read_out(void)
{
    //DEBUG_STREAM<<"AcquisitionMapping::is_all_pixel_read_out() - [BEGIN]"<<std::endl;
    //foreach module, check if pixels are readout
    for(int i = 0; i<get_nb_modules(); i++)
    {
        if(!m_module_info_array[i].m_is_pixel_readout)
            return false;
    }

    //DEBUG_STREAM<<"AcquisitionMapping::is_all_pixel_read_out() - [END]"<<std::endl;
    return true;
}

//---------------------------
//- AcquisitionMapping::process_message()
//---------------------------
void AcquisitionMapping::process_message(yat::Message& msg) throw (Tango::DevFailed)
{
    try
    {
        switch (msg.type())
        {
                //-----------------------------------------------------
            case yat::TASK_INIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> AcquisitionMapping::TASK_INIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                set_state(Tango::STANDBY);
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_EXIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> AcquisitionMapping::TASK_EXIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_TIMEOUT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> AcquisitionMapping::TASK_TIMEOUT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------

            case yat::TASK_PERIODIC:
            {
                DEBUG_STREAM<<" "<<std::endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                DEBUG_STREAM<<"-> AcquisitionMapping::TASK_PERIODIC" << endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                collect_data();
            }
                break;
                //-----------------------------------------------------                
            case MAPPING_STOP_MSG:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> AcquisitionMapping::MAPPING_STOP_MSG" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                // enable/disable collecting in TASK_PERIODIC MSG
                enable_periodic_msg(false);
                //stop/abort  storage of data, i.e inform stream to close/abort its current buffer !
                if((m_current_pixel >= m_num_map_pixels) && is_all_pixel_read_out())
                    m_store->close_data(); //close storage if all pixels are acquired
                else
                    m_store->abort_data(); //abort the storage 
            }
                break;
                //-----------------------------------------------------
        }
    }
    catch (yat::Exception& ex)
    {
        //- TODO Error Handling
        ex.dump();
        std::stringstream error_msg("");
        error_msg << "Origin\t: " << ex.errors[0].origin << endl;
        error_msg << "Desc\t: " << ex.errors[0].desc << endl;
        error_msg << "Reason\t: " << ex.errors[0].reason << endl;
        ERROR_STREAM << "Exception from - AcquisitionMapping::process_message() : " << error_msg.str()<<endl;
        stop_acquisition();
        on_fault(error_msg.str());
        throw;
    }
}
} // namespace 

