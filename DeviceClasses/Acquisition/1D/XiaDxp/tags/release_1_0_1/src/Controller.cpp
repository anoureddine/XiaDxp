//=============================================================================
//
// file :        Controller.cpp
//
// description : 
//
// project :	XiaDxp Project
//
// $Author: noureddine $
//
// copyleft :    Synchrotron SOLEIL
//               L'Orme des merisiers - Saint Aubin
//               BP48 - 91192 Gif sur Yvette
//               FRANCE
//=============================================================================

#include <tango.h>
#include <vector>
#include <string>

#include "XiaDxp.h"
#include "Controller.h"


namespace XiaDxp_ns
{

//============================================================================
//- Controller::Controller
//============================================================================
Controller::Controller(Tango::DeviceImpl* dev, ConfigLoader conf)
: yat4tango::DeviceTask(dev),
m_device(dev),
m_conf(conf)
{
    INFO_STREAM << "Controller::Controller() - [BEGIN]" << endl;
    enable_timeout_msg(false);
    enable_periodic_msg(false);
    set_periodic_msg_period(CONTROLLER_TASK_PERIODIC_MS);
    set_status("Use the 'LoadConfigFile' command to load a configuration file (*.ini) on the board !");
    set_state(Tango::OFF);

    INFO_STREAM << "Controller::Controller() - [END]" << endl;
}

//============================================================================
//- Controller::~Controller
//============================================================================
Controller::~Controller()
{
    INFO_STREAM << "Controller::~Controller() - [BEGIN]" << endl;
    /* not necessary with SharedPtr
    m_store.reset();
    m_stream.reset();
    m_acquisition.reset();
     */
    INFO_STREAM << "Controller::~Controller() - [END]" << endl;
}

// ============================================================================
// Controller::set_state
// ============================================================================
void Controller::set_state(Tango::DevState state)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_state = state;
    }
}

// ============================================================================
// Controller::get_state
// ============================================================================
Tango::DevState Controller::get_state(void)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        compute_state_status();
        return m_state;
    }
}

// ============================================================================
// Controller::set_status
// ============================================================================
void Controller::set_status(const std::string& status)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_status.str("");
        m_status << status.c_str();
    }
}

// ============================================================================
// Controller::get_status
// ============================================================================
std::string Controller::get_status(void)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        return (m_status.str());
    }
}

// ============================================================================
// Controller::on_abort
// ============================================================================
void Controller::on_abort(Tango::DevFailed df)
{
    DEBUG_STREAM << "Controller::on_abort()" << endl;
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());

    //- rethrow exception
    Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                   std::string(df.errors[0].desc).c_str(),
                                   "Controller::on_abort()");
}


// ============================================================================
// Controller::on_abort
// ============================================================================
void Controller::on_abort(const std::string& st)
{
    DEBUG_STREAM << "Controller::on_abort()" << endl;
    std::string status("");
    status += st;
    set_status(status);

    //- rethrow exception
    Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                   std::string(st).c_str(),
                                   "Controller::on_abort()");
}

// ============================================================================
// Controller::on_fault
// ============================================================================
void Controller::on_fault(Tango::DevFailed df)
{
    DEBUG_STREAM << "Controller::on_fault()" << endl;
    set_state(Tango::FAULT);
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());
}

// ============================================================================
// Controller::on_fault
// ============================================================================
void Controller::on_fault(const std::string& st)
{
    DEBUG_STREAM << "Controller::on_fault()" << endl;
    set_state(Tango::FAULT);
    std::string status("");
    status += st;
    set_status(status);
}

// ============================================================================
// Controller::on_alarm
// ============================================================================
void Controller::on_alarm(Tango::DevFailed df)
{
    DEBUG_STREAM << "Controller::on_alarm()" << endl;
    set_state(Tango::ALARM);
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());
}

// ============================================================================
// Controller::on_alarm
// ============================================================================
void Controller::on_alarm(const std::string& st)
{
    DEBUG_STREAM << "Controller::on_alarm()" << endl;
    set_state(Tango::ALARM);
    std::string status("");
    status = st;
    set_status(status);
}

// ============================================================================
// Controller::compute_state_status
// ============================================================================
void Controller::compute_state_status(void)
{
    //DEBUG_STREAM << "Controller::compute_state_status() - [BEGIN]" << endl;        	
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        if(m_state != Tango::FAULT)
        {
            if(m_state != Tango::DISABLE && m_state != Tango::OFF && m_acquisition && m_store)
            {
                if(m_acquisition->get_state()==Tango::RUNNING || m_store->get_state()==Tango::RUNNING)
                {
                    set_state(Tango::RUNNING);
                    set_status("Acquisition is in progress ...");
                }
                else
                {
                    set_state(m_acquisition->get_state());
                    set_status(m_acquisition->get_status());
                }
            }
            else if(m_state == Tango::OFF)
            {
                set_status("Use the 'LoadConfigFile' command to load a configuration file (*.ini) on the board !");
            }
        }
    }
    //DEBUG_STREAM << "Controller::compute_state_status() - [END]" << endl;        
}

// ============================================================================
// Controller::update_parameters
// ============================================================================
void Controller::update_parameters(ConfigLoader conf)
{
    INFO_STREAM << "Controller::update_parameters() - [BEGIN]" << endl;
    INFO_STREAM<<"conf.is_device_initialized = "<<conf.is_device_initialized<<std::endl;
    //@@TODO : manage a strategy to update stream but only if it is necessary  !!
    if( conf.is_device_initialized 	== true)
    {
        std::transform(conf.stream_type.begin(), conf.stream_type.end(), conf.stream_type.begin(), ::toupper);        
        //need to build a new type of stream (NEXUS_STREAM/LOG_STREAM/NO_STREAM/...) 
        if(conf.stream_type != m_conf.stream_type)
        {
            m_conf.stream_type = conf.stream_type;
            m_stream.reset(build_stream(m_conf.stream_type, m_conf.acquisition_mode));
        }

        //need to "init" in order to refresh parameters within the nexus stream
        if(conf.stream_path != m_conf.stream_path)
        {
            m_conf.stream_path = conf.stream_path;
            if(m_conf.stream_type == "NEXUS_STREAM")
                static_cast<StreamNexus*>(m_stream.get())->set_target_path(m_conf.stream_path);
        }

        //need to "init" in order to refresh parameters within the nexus stream
        if(conf.stream_file != m_conf.stream_file)
        {
            m_conf.stream_file = conf.stream_file;
            if(m_conf.stream_type == "NEXUS_STREAM")
                static_cast<StreamNexus*>(m_stream.get())->set_file_name(m_conf.stream_file);
        }

        //need to "init" in order to refresh parameters within the nexus stream
        if(conf.stream_nb_data_per_acq != m_conf.stream_nb_data_per_acq)
        {
            m_conf.stream_nb_data_per_acq = conf.stream_nb_data_per_acq;
            if(m_conf.stream_type == "NEXUS_STREAM")
                static_cast<StreamNexus*>(m_stream.get())->set_nb_data_per_acq(m_conf.stream_nb_data_per_acq);
        }

        //need to "init" in order to refresh parameters within the nexus stream
        if(conf.stream_nb_acq_per_file != m_conf.stream_nb_acq_per_file)
        {
            m_conf.stream_nb_acq_per_file = conf.stream_nb_acq_per_file;
            if(m_conf.stream_type == "NEXUS_STREAM")
                static_cast<StreamNexus*>(m_stream.get())->set_nb_acq_per_file(m_conf.stream_nb_acq_per_file);
        }

        //always in order to instantiate a new writer
        m_stream->init(m_store);
    }

    INFO_STREAM << "Controller::update_parameters() - [END]" << endl;
}

// ============================================================================
// Controller::load_config_file
// ============================================================================
void Controller::load_config_file(const std::string& mode, const std::string& file_name)
{
    INFO_STREAM << "Controller::load_config_file() - [BEGIN]" << endl;
    try
    {
        set_state(Tango::DISABLE);
        set_status(std::string("Loading Configuration File : [")+ file_name+std::string("] ..."));
        m_conf.acquisition_mode = mode;
        m_conf.acquisition_file = file_name;
        //- create an msg to pass it some data (Conf)
        yat::Message* msg = yat::Message::allocate(CONTROLLER_LOAD_MSG, DEFAULT_MSG_PRIORITY, true);
        msg->attach_data(m_conf);
        post(msg);
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    INFO_STREAM << "Controller::load_config_file() - [END]" << endl;
}

// ============================================================================
// Controller::save_config_file
// ============================================================================
void Controller::save_config_file(const std::string& file_name)
{
    INFO_STREAM << "Controller::save_config_file() - [BEGIN]" << endl;
    try
    {
        m_acquisition->save_config_file(file_name);
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    INFO_STREAM << "Controller::save_config_file() - [END]" << endl;
}

// ============================================================================
// Controller::start
// ============================================================================
void Controller::start(void)
{
    INFO_STREAM<<" "<<std::endl;
    INFO_STREAM << "Controller::start() - [BEGIN]" << endl;
    try
    {
        yat::MutexLock scoped_lock(m_state_lock);
        m_acquisition->start_acquisition();
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    INFO_STREAM << "Controller::start() - [END]" << endl;
}

// ============================================================================
// Controller::stop
// ============================================================================
void Controller::stop(void)
{
    INFO_STREAM<<" "<<std::endl;
    INFO_STREAM << "Controller::stop() - [BEGIN]" << endl;
    try
    {
        yat::MutexLock scoped_lock(m_state_lock);
        m_acquisition->stop_acquisition();
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    INFO_STREAM << "Controller::stop() - [END]" << endl;
}

// ============================================================================
// Controller::get_nb_modules
// ============================================================================
int Controller::get_nb_modules()
{
    return m_acquisition->get_nb_modules();
}

// ============================================================================
// Controller::get_nb_channels
// ============================================================================
int Controller::get_nb_channels()
{
    return m_acquisition->get_nb_channels();
}

// ============================================================================
// Controller::get_nb_bins
// ============================================================================
int Controller::get_nb_bins()
{
    return m_acquisition->get_nb_bins();
}

// ============================================================================
// Controller::get_nb_rois
// ============================================================================
int Controller::get_nb_rois(int channel)
{
    return m_acquisition->get_nb_rois(channel);
}

// ============================================================================
// Controller::set_nb_rois
// ============================================================================
void Controller::set_nb_rois(int channel, int nb_rois)
{
    m_acquisition->set_nb_rois(channel, nb_rois);
}

// ============================================================================
// Controller::get_roi_bounds
// ============================================================================
void Controller::get_roi_bounds(int channel, int roi_num, double& low, double& high)
{
    return m_acquisition->get_roi_bounds(channel, roi_num, low, high);
}

// ============================================================================
// Controller::set_roi_bounds
// ============================================================================
void Controller::set_roi_bounds(int channel, int roi_num, double low, double high)
{
    m_acquisition->set_roi_bounds(channel, roi_num, low, high);
}

// ============================================================================
// Controller::get_roi_data
// ============================================================================
double Controller::get_roi_data(int channel, int roi_num)
{
    return m_acquisition->get_roi_data(channel, roi_num);
}

// ============================================================================
// Controller::get_preset_value
// ============================================================================
double Controller::get_preset_value()
{
    return m_acquisition->get_preset_value();
}

// ============================================================================
// Controller::set_preset_value
// ============================================================================
void Controller::set_preset_value(double preset_value)
{
    m_acquisition->set_preset_value(preset_value);
}

// ============================================================================
// Controller::get_preset_type
// ============================================================================
std::string Controller::get_preset_type()
{
    return m_acquisition->get_preset_type();
}

// ============================================================================
// Controller::set_preset_type
// ============================================================================
void Controller::set_preset_type(const std::string& str_preset_type)
{
    m_acquisition->set_preset_type(str_preset_type);
}

// ============================================================================
// Controller::set_dynamic_range
// ============================================================================
void Controller::set_dynamic_range(double dynamic_range)
{
    m_acquisition->set_dynamic_range(dynamic_range);
}

// ============================================================================
// Controller::get_dynamic_range
// ============================================================================
double Controller::get_dynamic_range()
{
    return m_acquisition->get_dynamic_range();
}

// ============================================================================
// Controller::set_peaking_time
// ============================================================================
void Controller::set_peaking_time(double peaking_time)
{
    m_acquisition->set_peaking_time(peaking_time);
}

// ============================================================================
// Controller::get_peaking_time
// ============================================================================
double Controller::get_peaking_time()
{
    return m_acquisition->get_peaking_time();
}

// ============================================================================
// Controller::set_num_map_pixels
// ============================================================================
void Controller::set_num_map_pixels(long num_map_pixels)
{
    m_acquisition->set_num_map_pixels(num_map_pixels);
    m_store->set_nb_pixels(num_map_pixels);
}


// ============================================================================
// Controller::set_pixel_advance_mode
// ============================================================================
void Controller::set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel)
{

    m_acquisition->set_pixel_advance_mode(mode, ticks_per_pixel);
}

// ============================================================================
// Controller::set_pixel_advance_mode
// ============================================================================
std::string Controller::get_pixel_advance_mode()
{
    return m_acquisition->get_pixel_advance_mode();
}

// ============================================================================
// Controller::stream_reset_index()
// ============================================================================
void Controller::stream_reset_index()
{
    try
    {
        if(m_stream)
        {
            m_stream->reset_index();
        }
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
}

// ============================================================================
// Controller::build_acquisition()
// ============================================================================
Acquisition* Controller::build_acquisition(const std::string& board_type, const std::string& mode)
{
    INFO_STREAM << "Controller::build_acquisition()" << endl;
    //reinit previous acquisition ! 
    //m_acquisition.reset();
    Acquisition* acquisition;
    if(mode == "MCA")
    {
        //m_acquisition.reset(new AcquisitionMca(m_device, board_type, m_store), AcquisitionMcaTaskExiter());
        acquisition = new AcquisitionMca(m_device, board_type, m_store);
    }
    else if(mode == "MAPPING")
    {
        //m_acquisition.reset(new AcquisitionMapping(m_device, board_type, m_store), AcquisitionMappingTaskExiter());
        acquisition = new AcquisitionMapping(m_device, board_type, m_store);
    }
    else
    {
        std::ostringstream ossMsgErr;
        ossMsgErr << "Acquisition Mode value (" <<mode << ") is incorrect !";
        Tango::Except::throw_exception("LOGIC_ERROR",
                                       ossMsgErr.str().c_str(),
                                       "Controller::build_acquisition()");
    }
    acquisition->go();
    return acquisition;
}

// ============================================================================
// Controller::build_store()
// ============================================================================
yat::SharedPtr<DataStore> Controller::build_store()
{
    INFO_STREAM << "Controller::build_store()" << endl;
    //reinit previous acquisition ! 
    try
    {
        m_store.reset();
        m_store.reset(new DataStore(m_device), yat4tango::DeviceTaskExiter());
        m_store->go(); //- post the INIT msg        
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    return m_store;
}

// ============================================================================
// Controller::build_stream()
// ============================================================================
Stream* Controller::build_stream(const std::string& type, const std::string& mode)
{
    INFO_STREAM << "Controller::build_stream() - [BEGIN]" << endl;
    //reinit previous acquisition ! 
    Stream* stream;
    try
    {
        std::string stream_type = type;
        std::transform(stream_type.begin(), stream_type.end(), stream_type.begin(), ::toupper);

        //create the stream
        if(stream_type == "NEXUS_STREAM")//NEXUS_STREAM
        {
            if(mode == "MAPPING") //Nexus is available in MAPPING mode only
            {
                stream = new StreamNexus(m_device);
                static_cast<StreamNexus*>(stream)->set_target_path(m_conf.stream_path);
                static_cast<StreamNexus*>(stream)->set_file_name(m_conf.stream_file);
                static_cast<StreamNexus*>(stream)->set_write_mode(m_conf.stream_write_mode);
                static_cast<StreamNexus*>(stream)->set_nb_data_per_acq(m_conf.stream_nb_data_per_acq);
                static_cast<StreamNexus*>(stream)->set_nb_acq_per_file(m_conf.stream_nb_acq_per_file);
            }
            else
            {
                stream = new StreamNo(m_device);
            }
        }
        else if(stream_type == "LOG_STREAM")//LOG_STREAM
        {
            stream = new StreamLog(m_device);
        }
        else if(stream_type == "NO_STREAM")//NO_STREAM
        {
            stream = new StreamNo(m_device);
        }
        else
        {
            //only NO_STREAM/LOG_STREAM/NEXUS_STREAM are available
            std::ostringstream ossMsgErr;
            ossMsgErr   <<  "Wrong Stream Type:\n"
            "Possibles values are:\n"
            "NO_STREAM\n"
            "LOG_STREAM\n"
            "NEXUS_STREAM"
            <<std::endl;
            Tango::Except::throw_exception("LOGIC_ERROR",
                                           ossMsgErr.str().c_str(),
                                           "Controller::build_stream()");
        }
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        on_abort(df);
    }
    INFO_STREAM << "Controller::build_stream() - [END]" << endl;
    return stream;
}

// ============================================================================
// Controller::build_attributes()
// ============================================================================
AttrView* Controller::build_attributes(const std::string& mode)
{
    INFO_STREAM << "Controller::build_attributes()" << endl;
    //reinit previous acquisition ! 
    AttrView* attr_view;
    if(mode == "MCA")
    {
        attr_view = new AttrViewMca(m_device);
    }
    else if(mode == "MAPPING")
    {
        attr_view = new AttrViewMapping(m_device);
    }
    else
    {
        std::ostringstream ossMsgErr;
        ossMsgErr << "Acquisition Mode value (" <<mode << ") is incorrect !";
        Tango::Except::throw_exception("LOGIC_ERROR",
                                       ossMsgErr.str().c_str(),
                                       "Controller::build_attributes()");
    }
    return attr_view;
}

// ============================================================================
// Controller::update_data()
// ============================================================================
void Controller::update_data(int ichannel)
{
    DEBUG_STREAM << "Controller::update_data() - [BEGIN]" << endl;

    //update streamer view data
    m_stream->update_data(ichannel, m_store);

    //update tango view data
    m_attr_view->update_data(ichannel, m_store);
    DEBUG_STREAM << "Controller::update_data() - [END]" << endl;
}

// ============================================================================
// Controller::update_view()
// ============================================================================
void Controller::update_view(void)
{
    DEBUG_STREAM << "Controller::update_view() - [BEGIN]" << endl;
    set_state(Tango::DISABLE);
    set_status(std::string("Update attributes according to Rois configurations ..."));
    //init the tango attributes according to the new configuration
    m_attr_view->init(m_store);
    set_state(Tango::STANDBY);
    DEBUG_STREAM << "Controller::update_view() - [END]" << endl;
}

// ============================================================================
// Controller::close()
// ============================================================================
void Controller::close()
{
    INFO_STREAM << "Controller::close() - [BEGIN]" << endl;
    //close stream & go out
    m_stream->close();
    INFO_STREAM << "Controller::close() - [END]" << endl;
}

// ============================================================================
// Controller::abort()
// ============================================================================
void Controller::abort()
{
    INFO_STREAM << "Controller::abort() - [BEGIN]" << endl;
    //abort stream & go out
    m_stream->abort();
    INFO_STREAM << "Controller::abort() - [END]" << endl;
}

///////////////////////////////////////////////////////////////////////////////

//---------------------------
//- Controller::process_message()
//---------------------------
void Controller::process_message(yat::Message& msg) throw (Tango::DevFailed)
{
    DEBUG_STREAM << "Controller::process_message(): entering... !" << endl;
    try
    {
        switch (msg.type())
        {
                //-----------------------------------------------------
            case yat::TASK_INIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> Controller::TASK_INIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                set_state(Tango::OFF);
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_EXIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> Controller::TASK_EXIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_TIMEOUT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> Controller::TASK_TIMEOUT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------------------------------------------------

            case yat::TASK_PERIODIC:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> Controller::TASK_PERIODIC" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case CONTROLLER_LOAD_MSG:
            {
                INFO_STREAM<<"-> Controller::CONTROLLER_LOAD_MSG" << endl;
                try
                {
                    //conf = msg.get_data<ConfigLoader>();
                    //build  the data store
                    m_store = build_store();
                    //enable/disable writing statistics (read from Device Property)
                    m_store->set_statistics_enabled(m_conf.stream_statistics_enabled);
                    //build  the tango attributes (statistics+channels)
                    m_attr_view.reset(build_attributes(m_conf.acquisition_mode));
                    //build  the acquisition (MCA/MAPPING)                    
                    m_acquisition.reset(build_acquisition(m_conf.board_type, m_conf.acquisition_mode));
                    // build  the stream (LOG/NEXUS) and enable/disable it!
                    m_stream.reset(build_stream(m_conf.stream_type, m_conf.acquisition_mode));
                    //subscribe Controller to the data store in order to notify controller if data store is changed.
                    m_store->subscribe(this);
                    //load INI file on Xia Board via the HandelHelper
                    m_acquisition->load_config_file(m_conf.acquisition_file);
                    //init the tango attributes according to the new configuration
                    m_attr_view->init(m_store);
                    //init the stream according to the new configuration
                    m_stream->init(m_store);
                    //everything is ok
                    set_state(Tango::STANDBY);
                    set_status("");
                }
                catch (Tango::DevFailed &df)
                {
                    ERROR_STREAM << df << endl;
                    on_fault(df);
                }
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
        ERROR_STREAM << "Exception from - process_message() : " << error_msg.str()<<endl;
        on_abort(error_msg.str());
        throw;
    }
}

}
