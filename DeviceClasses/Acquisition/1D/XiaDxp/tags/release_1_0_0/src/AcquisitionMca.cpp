/*************************************************************************
/*! 
 *  \file   AcquisitionMca.h
 *  \brief  class AcquisitionMca 
 *  \author 
 */
/*************************************************************************/

#include <tango.h>
#include <map>
#include <yat/utils/XString.h>

#include "AcquisitionMca.h"


namespace XiaDxp_ns
{

//----------------------------------------------------------------------------------------------------------------------
//- AcquisitionMca Ctor
//----------------------------------------------------------------------------------------------------------------------
AcquisitionMca::AcquisitionMca(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store)
: Acquisition(dev, board_type, store)
{
    INFO_STREAM << "Acquisition::AcquisitionMca() - [BEGIN]" << endl;
    enable_timeout_msg(false);
    enable_periodic_msg(false);
    set_periodic_msg_period(MCA_TASK_PERIODIC_MS);
    INFO_STREAM << "Acquisition::AcquisitionMca() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- AcquisitionMca Dtor
//----------------------------------------------------------------------------------------------------------------------
AcquisitionMca::~AcquisitionMca(void)
{
    INFO_STREAM << "Acquisition::~AcquisitionMca() - [BEGIN]" << endl;
    INFO_STREAM << "Acquisition::~AcquisitionMca() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- Init the XIA library with a new INI configuration file
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMca::load_config_file(const std::string& file_name)
{
    INFO_STREAM << "AcquisitionMca::load_config_file() - [BEGIN]" << endl;
    m_helper->load_config_file(file_name);
    //init DataStore with new configurations    
    m_store->init	(get_nb_modules(), //number of modules
                     get_nb_channels()/get_nb_modules(), //number of channels per module (always 4 for xmap))
                     get_nb_channels()/get_nb_modules(), //active_channels == nb_channels !
                     1,	//always 1 pixel in mode MCA
                     get_nb_bins(),
                     1	//no timestamp for now
                     );
    INFO_STREAM << "AcquisitionMca::load_config_file() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMca::save_config_file(const std::string& file_name)
{
    INFO_STREAM << "AcquisitionMca::save_config_file() - [BEGIN]" << endl;
    m_helper->save_config_file(file_name);
    INFO_STREAM << "AcquisitionMca::save_config_file() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMca::start_acquisition(short accumulate)
{
    INFO_STREAM << "AcquisitionMca::start_acquisition() - [BEGIN]" << endl;
    m_helper->stop_acquisition(); //@TODO : check why 

    m_helper->start_acquisition(accumulate);
    enable_periodic_msg(true); //only when start is done    
    INFO_STREAM<<"collecting in progress ..."<<endl;
    INFO_STREAM << "AcquisitionMca::start_acquisition() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMca::stop_acquisition()
{
    INFO_STREAM << "AcquisitionMca::stop_acquisition() - [BEGIN]" << endl;
    //- Post MCA_STOP_MSG in order to stop XIA acquisition
    yat::Message* msg = yat::Message::allocate(MCA_STOP_MSG, DEFAULT_MSG_PRIORITY, true);
    post(msg);
    INFO_STREAM << "AcquisitionMca::stop_acquisition() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- collect_data
//----------------------------------------------------------------------------------------------------------------------
void AcquisitionMca::collect_data(void)
{
    INFO_STREAM << "AcquisitionMca::collect_data() - [BEGIN]" << endl;
    //for each module
    for (int imodule = 0; imodule < get_nb_modules(); imodule++)
    {
        for(int ichannel = 0; ichannel < get_nb_channels()/get_nb_modules(); ichannel++)
        {
            //get statistics values from board via handle library
            INFO_STREAM << ""<< endl;
            INFO_STREAM << "-------------------------------------------"<< endl;
            INFO_STREAM << "channel_" <<to_channel_cluster(imodule, ichannel)<<endl;
            INFO_STREAM << "-------------------------------------------"<< endl;

            INFO_STREAM << "-- get statistics from board" << endl;
            PixelData pix_data;
            pix_data.triggers = 0; //available only for MAPPING mode
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "realtime", &pix_data.realtime);
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "livetime", &pix_data.livetime);
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "trigger_livetime", &pix_data.trigger_livetime);
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "input_count_rate", &pix_data.input_count_rate);
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "output_count_rate", &pix_data.output_count_rate);
            m_helper->get_run_data(to_channel_cluster(imodule, ichannel), "events_in_run", &pix_data.events_in_run);

            //push statistics into DataStore
            INFO_STREAM << "-- push statistics into DataStore" << endl;
            m_store->store_statistics(imodule,              //numero of module
                                      ichannel,             //numero of channel
                                      0,                    //always 0 in MCA , @@TODO nbFrames later
                                      "MCA",
                                      pix_data);

            //get data from from board via handle library
            INFO_STREAM << "-- get buffer data from board" << endl;
            std::vector<DataType> spectrum;
            m_helper->get_mca_data(to_channel_cluster(imodule, ichannel),
                                   m_helper->get_mca_length(to_channel_cluster(imodule, ichannel)),
                                   spectrum);

            //push channel data into DataStore
            INFO_STREAM << "-- push buffer data into DataStore" << endl;
            m_store->store_data(imodule,                   //numero of module
                                ichannel,                  //numero of channel
                                0,                         //always 0 in MCA , @@TODO nbFrames later
                                (DataType*)&spectrum[0],
                                spectrum.size()
                                );
        }
    }

    INFO_STREAM << "AcquisitionMca::collect_data() - [END]" << endl<<endl;
    INFO_STREAM <<" "<< endl;
}

//---------------------------
//- AcquisitionMca::process_message()
//---------------------------
void AcquisitionMca::process_message(yat::Message& msg) throw (Tango::DevFailed)
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
                INFO_STREAM << "-> AcquisitionMca::TASK_INIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                set_state(Tango::STANDBY);
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_EXIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> AcquisitionMca::TASK_EXIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_TIMEOUT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> AcquisitionMca::TASK_TIMEOUT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------

            case yat::TASK_PERIODIC:
            {
                DEBUG_STREAM<<" "<<std::endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                DEBUG_STREAM << "-> AcquisitionMca::TASK_PERIODIC" << endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                ///
                if(is_running())
                {
                    //get statistics/data values from board via handle library and store into dataStore
                    collect_data();

                    if(!is_need_collecting_last_data()) // do it once
                    {
                        DEBUG_STREAM<<"enable collecting the final data at the end of acquisition"<<endl;
                        enable_collecting_last_data();
                    }
                }
                else
                {
                    if(is_need_collecting_last_data()) //do it once                    
                    {
                        INFO_STREAM<<"collecting the final data ..."<<endl;
                        collect_data();
                        m_store->close_data();
                        disable_collecting_last_data();
                    }
                }

            }
                break;

                //-----------------------------------------------------                
            case MCA_STOP_MSG:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> AcquisitionMca::MCA_STOP_MSG" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                // stop xia acquisition
                m_helper->stop_acquisition();
            }
                break;
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
        ERROR_STREAM << "Exception from - AcquisitionMca::process_message() : " << error_msg.str()<<endl;
        on_abort(error_msg.str());
        throw;
    }
}

} // namespace 

