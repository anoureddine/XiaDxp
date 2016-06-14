/*************************************************************************
/*! 
 *  \file   Acquisition.h
 *  \brief  class Acquisition 
 *  \author 
 */
/*************************************************************************/

#ifndef ACQUISITION_H
#define ACQUISITION_H

//TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

#include "DataStore.h"
#include "DriverHelper.h"
#include "HandelHelper.h"
#include "SimulatorHelper.h"

namespace XiaDxp_ns
{
class Acquisition : public yat4tango::DeviceTask
{
public:

    /// ctor
    Acquisition(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store);

    /// dtor
    virtual ~Acquisition(void);

    /// load config ini file
    virtual void load_config_file(const std::string& file_name) = 0;

    /// save modified parameters
    virtual void save_config_file(const std::string& file_name) = 0;

    /// start acquisition
    virtual void start_acquisition(short accumulate = false) = 0;

    /// stop acquisition
    virtual void stop_acquisition() = 0;

    /// get number of total modules
    int get_nb_modules();

    /// get number of total channels
    int get_nb_channels();

    /// get number of bins
    int get_nb_bins();

    /// get number of rois of the channel
    int get_nb_rois(int channel);
    
    /// get number of rois of the channel
    void set_nb_rois(int channel, int nb_rois);
    
    /// get roi low/high limits for a channel & roi number
    void get_roi_bounds(int channel, int roi_num, double& low, double& high);    

    /// set roi low/high limits for a channel & roi number
    void set_roi_bounds(int channel, int roi_num, double low, double high);    
    
    /// get roi data value faor a channel & roi number
    double get_roi_data(int channel, int roi_num);
    
    /// get dynamic range
    double get_dynamic_range();

    /// set dynamic range
    void set_dynamic_range(double dynamic_range);
	
    /// get dynamic range
    double get_peaking_time();

    /// set dynamic range
    void set_peaking_time(double peaking_time);
	
    /// get preset value
    double get_preset_value();

    /// set preset value
    void set_preset_value(double preset_value);

    /// get preset type
    std::string get_preset_type();

    /// set preset type
    void set_preset_type(const std::string& str_preset_type);

    /// get "num_map_pixels" parameter
	long get_num_map_pixels();
    
    /// set "num_map_pixels" parameter
	void set_num_map_pixels(long num_map_pixels);

    /// get "num_map_pixels_per_buffer" parameter
    long get_num_map_pixels_per_buffer();    
    
    /// set "num_map_pixels_per_buffer" parameter
    void set_num_map_pixels_per_buffer(long num_map_pixels_per_buffer);    
	
    /// set "pixel_advance_mode" parameter to XIA_MAPPING_CTL_GATE or XIA_MAPPING_CTL_SYNC
    void set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel = 0.0);    
    
    /// get "pixel_advance_mode" (GATE/SYNC/HOST))
    std::string get_pixel_advance_mode();
    
    ///get board state 
    Tango::DevState get_state();

    ///get board status 
    std::string get_status();


protected:
    /// [yat4tango::DeviceTask implementation]
    virtual void process_message(yat::Message& msg) throw(Tango::DevFailed) = 0;

    ///
    void set_state(Tango::DevState state);

    ///
    void set_status(const std::string& status);

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
    
    /// return true if run_active  || buffer data is not collected yet
    bool is_running();

    /// inform Task to collect the last available buffer in the board, even if the board state is not running
    void enable_collecting_last_data();

    /// inform Task that the last available buffer has already been collected
    void disable_collecting_last_data();

    /// ask if the last buffer must be collected
    bool is_need_collecting_last_data();

    /// state & status stuff
    Tango::DevState m_state;
    std::stringstream m_status;
    yat::Mutex m_state_lock;
    yat::Mutex m_data_lock;
    ///owner device server
    Tango::DeviceImpl* m_device;
    ///DataStore object
    yat::SharedPtr<DataStore> m_store;
    ///xia board type (XMAP/FALCON/..)
    std::string m_board_type;
    ///collect the last spectrum 
    bool m_is_need_collecting_last_data;    
    ///handle library wrapper     
    yat::SharedPtr<DriverHelper> m_helper;
};

} // namespace XiaDxp_ns

#endif

