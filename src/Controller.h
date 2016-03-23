/*************************************************************************
/*! 
 *  \file   Controller.h
 *  \brief  class Controller
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

//- TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/memory/UniquePtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

#include "DataStore.h"
#include "Stream.h"
#include "StreamLog.h"
#include "StreamNexus.h"
#include "StreamNo.h"
#include "AttrView.h"
#include "AttrViewMca.h"
#include "AttrViewMapping.h"
#include "DriverHelper.h"
#include "HandelHelper.h"
#include "SimulatorHelper.h"
#include "Acquisition.h"
#include "AcquisitionMca.h"
#include "AcquisitionMapping.h"

const size_t CONTROLLER_TASK_PERIODIC_MS        = 100;//ms
const size_t CONTROLLER_LOAD_MSG                = yat::FIRST_USER_MSG + 100;

///////////////////////////////////////////////////////////


namespace XiaDxp_ns
{
/// Used to pass properties from Main Device to Undulators object
class ConfigLoader
{
public:
	bool is_device_initialized;
    std::string board_type; 
    std::string acquisition_mode;
    std::string acquisition_file;
    std::string stream_type;
    std::string stream_path;
    std::string stream_file;
	std::string stream_write_mode;
    int stream_nb_data_per_acq;
    int stream_nb_acq_per_file;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// 
class Controller : public yat4tango::DeviceTask
{
public:
    /// ctor
    Controller(Tango::DeviceImpl * dev, ConfigLoader conf);

    /// dtor
    virtual ~Controller();

    /// load config file (*.ini) through Acquisition object
    void load_config_file(const std::string& mode, const std::string& file_name);

    /// save modified parameters through Acquisition object
    void save_config_file(const std::string& file_name);
        
    /// reapply a set of parameters (stream_type, stream_path, ...), if necessary !
    void update_parameters(ConfigLoader conf);

    /// start acquisition through Acquisition object
    void start(void);

    /// stop acquisition through Acquisition object
    void stop(void);

    /// get number of total modules
    int get_nb_modules();

    /// get number of total channels
    int get_nb_channels();

    /// get number of bins
    int get_nb_bins();

    /// get number of rois
    int get_nb_rois(int channel);

    /// set number of rois
    void set_nb_rois(int channel, int nb_rois);
    
    /// get roi low/high limits
    void get_roi_bounds(int channel, int roi_num, double& low, double& high);    
    
    /// set roi low/high limits
    void set_roi_bounds(int channel, int roi_num, double low, double high);    
    
    /// get roi data value
    double get_roi_data(int channel, int roi_num);
    
    /// get dynamic_range
    double get_dynamic_range();

    /// set preset value
    void set_dynamic_range(double dynamic_range);
	
    /// set peaking_time
    void set_peaking_time(double peaking_time);
	
    /// get peaking_time
    double get_peaking_time();
	
    /// get preset value
    double get_preset_value();
	
    /// set preset value
    void set_preset_value(double preset_value);

    /// get preset type
    std::string get_preset_type();

    /// set preset type
    void set_preset_type(const std::string& str_preset_type);

	/// set num_map_pixels
	void set_num_map_pixels(long num_map_pixels);
	
    /// set pixel_advance_mode
    void set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel = 0.0);

    /// get pixel_advance_mode
    std::string get_pixel_advance_mode();
    
    /// reset buffer index (Nexus))
    void stream_reset_index();
    
    /// get the last state of the task
    Tango::DevState get_state(void);

    /// get the last status of the task
    std::string get_status(void);

    /// fix the state in a scoped_lock
    void set_state(Tango::DevState state);

    /// fix the status in a scoped_lock
    void set_status(const std::string& status);

    /// notification from DataStore, to inform that datas are refreshed
    void update_data(int ichannel);
    
    /// notification from controller::update_data() or directly from device (rois management). this will refresh tango view.
    void update_view(void);
    
    /// close the streamer. generally done at end of acquisition
    void close();    
    
    /// abort the streamer. generally done when user abort the acquisition
    void abort();    
    
protected:

    /// [yat4tango::DeviceTask implementation]
    virtual void process_message(yat::Message& msg) throw(Tango::DevFailed);

    /// compute internal state/status of task
    void compute_state_status(void);

    /// useful to manage behaviour in catch block : stop_moving(), status = "xxx "...
    void on_abort(Tango::DevFailed df);

    /// useful to manage behaviour in catch block : stop_moving(), status = "xxx "...
    void on_abort(const std::string& st);

    /// useful to manage behaviour in catch block : state = Tango::ALARM, status = "xxx "...
    void on_alarm(Tango::DevFailed df);

    /// useful to manage behaviour in catch block : state = Tango::ALARM, status = "xxx "...
    void on_alarm(const std::string& st);

    /// useful to manage behaviour in catch block : stop_moving(), state = Tango::FAULT, status = "xxx "...
    void on_fault(Tango::DevFailed df);

    /// useful to manage behaviour in catch block : stop_moving(), state = Tango::FAULT, status = "xxx "...
    void on_fault(const std::string& st);

private:
    /// make store
    yat::SharedPtr<DataStore> build_store();    
    /// make acquisition
    Acquisition* build_acquisition(const std::string& board_type, const std::string& mode);
    /// make Atr View
    AttrView* build_attributes(const std::string& mode);
    /// make stream
    Stream* build_stream(const std::string& type, const std::string& mode);
    /// state
    Tango::DevState m_state;
    /// status
    std::stringstream m_status;
    /// lock for general state
    yat::Mutex m_state_lock;
    /// Owner Device server object
    Tango::DeviceImpl* m_device;
    /// device properties/config 
    ConfigLoader m_conf;    
    /// DataStore object
    yat::SharedPtr<DataStore> m_store;    
    /// Acquisition object
    yat::UniquePtr<Acquisition, yat4tango::DeviceTaskExiter> m_acquisition;
    /// Stream object
    yat::UniquePtr<Stream> m_stream;
    /// AttrView
    yat::UniquePtr<AttrView>  m_attr_view;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      


}

#endif // CONTROLLER_H


