/*************************************************************************
/*! 
 *  \file   Acquisition.h
 *  \brief  class Acquisition 
 *  \author 
 */
/*************************************************************************/


#include <tango.h>

#include <map>

//- YAT stuff
#include <yat/utils/XString.h>

#include "handel_constants.h"

#include "Acquisition.h"

namespace XiaDxp_ns
{

//----------------------------------------------------------------------------------------------------------------------
//- Acquisition Ctor
//----------------------------------------------------------------------------------------------------------------------
Acquisition::Acquisition(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store)
: yat4tango::DeviceTask(dev),
m_device(dev),
m_store(store),
m_board_type(board_type),
m_is_need_collecting_last_data(false)
{
    INFO_STREAM << "Acquisition::Acquisition() - [BEGIN]" << endl;    
    if(board_type == "SIMULATOR")        
        m_helper.reset(new SimulatorHelper(dev, board_type));
    else        
        m_helper.reset(new HandelHelper(dev, board_type));
    enable_periodic_msg(false);
    INFO_STREAM << "Acquisition::Acquisition() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- Acquisition Dtor
//----------------------------------------------------------------------------------------------------------------------
Acquisition::~Acquisition()
{
    INFO_STREAM << "Acquisition::~Acquisition() - [BEGIN]" << endl;
    m_helper.reset();
    INFO_STREAM << "Acquisition::~Acquisition() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Tango::DevState Acquisition::get_state()
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);

        switch(m_helper->get_state())
        {
            case State::INITIALIZATION_SUCCESSFUL:
                ///DEBUG_STREAM<<"------- INITIALIZATION_SUCCESSFUL"<<endl;
				if(m_state!=Tango::ALARM)
					set_state(Tango::STANDBY);
                break;
            case State::INITIALIZATION_UNKNOWN:
                DEBUG_STREAM<<"------- INITIALIZATION_UNKNOWN"<<endl;
                set_state(Tango::FAULT);
                return m_state;
                break;
            case State::INITIALIZATION_FAILED:
                DEBUG_STREAM<<"------- INITIALIZATION_FAILED"<<endl;
                set_state(Tango::FAULT);
                return m_state;
                break;
            case State::INITIALIZATION_PROGRESS:
                DEBUG_STREAM<<"------- INITIALIZATION_PROGRESS"<<endl;
                set_state(Tango::INIT);
                return m_state;
                break;
        }

        //if board is initialized, check if it is running
        if(is_running() || is_need_collecting_last_data())
        {
            DEBUG_STREAM<<"------- IS COLLECTING"<<endl;
            set_state(Tango::RUNNING);
        }
        return m_state;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_state(Tango::DevState state)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_state = state;
    }
}


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_status(const std::string& status)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_status.str("");
        m_status << status.c_str();
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string Acquisition::get_status()
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        return (m_status.str());
    }
}

// ============================================================================
// Acquisition::on_abort
// ============================================================================
void Acquisition::on_abort(Tango::DevFailed df)
{
    DEBUG_STREAM << "Acquisition::on_abort()" << endl;
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());

    //- rethrow exception
    Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                   std::string(df.errors[0].desc).c_str(),
                                   "Acquisition::on_abort()");
}


// ============================================================================
// Acquisition::on_abort
// ============================================================================
void Acquisition::on_abort(const std::string& st)
{
    DEBUG_STREAM << "Acquisition::on_abort()" << endl;
    std::string status("");
    status += st;
    set_status(status);

    //- rethrow exception
    Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                   std::string(st).c_str(),
                                   "Acquisition::on_abort()");
}

// ============================================================================
// Acquisition::on_fault
// ============================================================================
void Acquisition::on_fault(Tango::DevFailed df)
{
    DEBUG_STREAM << "Acquisition::on_fault()" << endl;
    set_state(Tango::FAULT);
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());
}

// ============================================================================
// Acquisition::on_fault
// ============================================================================
void Acquisition::on_fault(const std::string& st)
{
    DEBUG_STREAM << "Acquisition::on_fault()" << endl;
    set_state(Tango::FAULT);
    std::string status("");
    status += st;
    set_status(status);
}

// ============================================================================
// Acquisition::on_alarm
// ============================================================================
void Acquisition::on_alarm(Tango::DevFailed df)
{
    DEBUG_STREAM << "Acquisition::on_alarm()" << endl;
    set_state(Tango::ALARM);
    std::stringstream status;
    status.str("");
    status << "Origin\t: " << df.errors[0].origin << endl;
    status << "Desc\t: " << df.errors[0].desc << endl;
    set_status(status.str());
}

// ============================================================================
// Acquisition::on_alarm
// ============================================================================
void Acquisition::on_alarm(const std::string& st)
{
    DEBUG_STREAM << "Acquisition::on_alarm()" << endl;
    set_state(Tango::ALARM);
    std::string status("");
    status = st;
    set_status(status);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
int Acquisition::get_nb_modules()
{
    return m_helper->get_nb_modules();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int Acquisition::get_nb_channels()
{
    return m_helper->get_nb_channels();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int Acquisition::get_nb_bins()
{    
    return m_helper->get_nb_bins(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int Acquisition::get_nb_rois(int channel)
{
    return m_helper->get_nb_rois(channel);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void Acquisition::set_nb_rois(int channel, int nb_rois)
{
    m_helper->set_nb_rois(channel, nb_rois);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void Acquisition::get_roi_bounds(int channel, int roi_num, double& low, double& high)
{
    return m_helper->get_roi_bounds(channel, roi_num, low, high);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void Acquisition::set_roi_bounds(int channel, int roi_num, double low, double high)
{
    return m_helper->set_roi_bounds(channel, roi_num, low, high);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double Acquisition::get_roi_data(int channel, int roi_num)
{
    return m_helper->get_roi_data(channel, roi_num);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
double Acquisition::get_dynamic_range()
{
    return m_helper->get_dynamic_range(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_dynamic_range(double dynamic_range)
{
    m_helper->stop_acquisition(); //@TODO : check why 
    m_helper->set_dynamic_range(ALL_CHANNELS, dynamic_range);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
double Acquisition::get_peaking_time()
{
    return m_helper->get_peaking_time(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_peaking_time(double peaking_time)
{
    m_helper->stop_acquisition(); //@TODO : check why 
    m_helper->set_peaking_time(ALL_CHANNELS, peaking_time);
}


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
double Acquisition::get_preset_value()
{
    return m_helper->get_preset_value(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void Acquisition::set_preset_value(double preset_value)
{
    m_helper->stop_acquisition(); //@TODO : check why 
    m_helper->set_preset_value(ALL_CHANNELS, preset_value);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
std::string Acquisition::get_preset_type()
{
    double preset_type = m_helper->get_preset_type(0);
    std::string str_preset_type;
    if(preset_type == XIA_PRESET_NONE)
    {
        str_preset_type = "NONE";
    }
    else if(preset_type == XIA_PRESET_FIXED_REAL)
    {
        str_preset_type = "FIXED_REAL";
    }
    else if(preset_type == XIA_PRESET_FIXED_LIVE)
    {
        str_preset_type = "FIXED_LIVE";
    }
    else if(preset_type == XIA_PRESET_FIXED_EVENTS)
    {
        str_preset_type = "FIXED_EVENTS";
    }
    else if(preset_type == XIA_PRESET_FIXED_TRIGGERS)
    {
        str_preset_type = "FIXED_TRIGGERS";
    }
    else
    {
        THROW_DEVFAILED(_CPTC ("LOGIC_ERROR"),
                        _CPTC ("Unknown preset type !"),
                        _CPTC ("Acquisition::get_preset_type()"));
    }
    return str_preset_type;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void Acquisition::set_preset_type(const std::string& str_preset_type)
{
    double preset_type = -1;
    if(str_preset_type == "NONE")
    {
        preset_type = XIA_PRESET_NONE;
    }
    else if(str_preset_type == "FIXED_REAL")
    {
        preset_type = XIA_PRESET_FIXED_REAL;
    }
    else if(str_preset_type == "FIXED_LIVE")
    {
        preset_type = XIA_PRESET_FIXED_LIVE;
    }
    else if(str_preset_type == "FIXED_EVENTS")
    {
        preset_type = XIA_PRESET_FIXED_EVENTS;
    }
    else if(str_preset_type == "FIXED_TRIGGERS")
    {
        preset_type = XIA_PRESET_FIXED_TRIGGERS;
    }
    else
    {
        THROW_DEVFAILED(_CPTC ("LOGIC_ERROR"),
                        _CPTC ("Unknown preset type !"),
                        _CPTC ("Acquisition::set_preset_type()"));
    }
    m_helper->stop_acquisition();	//@TODO : check why 
    m_helper->set_preset_type(ALL_CHANNELS, (double)preset_type);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
long Acquisition::get_num_map_pixels()
{
    return (long) m_helper->get_num_map_pixels(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_num_map_pixels(long num_map_pixels)
{
    m_helper->stop_acquisition(); //@TODO : check why     
    m_helper->set_num_map_pixels(ALL_CHANNELS, num_map_pixels);
    m_helper->apply_all_board_operation("apply");
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
long Acquisition::get_num_map_pixels_per_buffer()
{
    return m_helper->get_num_map_pixels_per_buffer(0);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_num_map_pixels_per_buffer(long num_map_pixels_per_buffer)
{
    m_helper->stop_acquisition(); //@TODO : check why     
    m_helper->set_num_map_pixels_per_buffer(ALL_CHANNELS, num_map_pixels_per_buffer);
    m_helper->apply_all_board_operation("apply");
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel)
{    
    m_helper->set_pixel_advance_mode(mode, ticks_per_pixel);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string Acquisition::get_pixel_advance_mode()
{    
    return m_helper->get_pixel_advance_mode();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool Acquisition::is_running()
{
    return m_helper->is_running();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::enable_collecting_last_data()
{
    DEBUG_STREAM << "Acquisition::enable_collecting_last_data()" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_is_need_collecting_last_data = true;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Acquisition::disable_collecting_last_data()
{
    DEBUG_STREAM << "Acquisition::disable_collecting_last_data()" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_is_need_collecting_last_data = false;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool Acquisition::is_need_collecting_last_data()
{
    yat::MutexLock scoped_lock(m_data_lock);
    return m_is_need_collecting_last_data;
}

} // namespace 

