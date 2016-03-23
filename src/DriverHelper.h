/*************************************************************************
/*! 
 *  \file   DriverHelper.h
 *  \brief  class DriverHelper 
 *  \author Arafat Noureddine - SOLEIL (consultant MEDIANE SYSTEME)
 */
/*************************************************************************/

#ifndef DRIVER_HELPER_H
#define DRIVER_HELPER_H

//- TANGO
#include <tango.h>


namespace XiaDxp_ns
{

    enum MappingMode
    {
        MAPPING_MODE_DISABLED = 0,
        MAPPING_MODE_MCA,
        MAPPING_MODE_SCA,
        MAPPING_MODE_LIST
    };
    
    enum State
    {
        INITIALIZATION_UNKNOWN = 0,
        INITIALIZATION_PROGRESS,
        INITIALIZATION_SUCCESSFUL,
        INITIALIZATION_FAILED
    };
    
    enum PixelAdvanceMode
    {
        PIXEL_ADVANCE_MODE_GATE = 0,
        PIXEL_ADVANCE_MODE_SYNCH,
        PIXEL_ADVANCE_MODE_HOST
    };
    
    enum Buffer
    {
        BUFFER_A = 0,
        BUFFER_B
    };
    
class DriverHelper : public Tango::LogAdapter
{
public:

    ////////////////////////////////////////////////////////////////////////
    DriverHelper(Tango::DeviceImpl *dev, const std::string & board_type) : Tango::LogAdapter(dev)
    {
        INFO_STREAM << "DriverHelper::DriverHelper()- [BEGIN]" << endl;
        m_device = dev;
        m_board_type = board_type;
        m_status = "WARNING !";
        m_state = INITIALIZATION_SUCCESSFUL;
        INFO_STREAM << "DriverHelper::DriverHelper()- [END]" << endl;
    }

    ////////////////////////////////////////////////////////////////////////
    virtual ~DriverHelper(void)
    {
        INFO_STREAM << "DriverHelper::DriverHelper()- [BEGIN]" << endl;
        INFO_STREAM << "DriverHelper::DriverHelper()- [END]" << endl;
    }
    ////////////////////////////////////////////////////////////////////////
    
    virtual void load_config_file(const std::string& config_file) = 0;
    virtual void save_config_file(const std::string& config_file) = 0;
    virtual void start_acquisition(short accumulate) = 0;
    virtual void stop_acquisition() = 0;
    virtual State get_state() = 0;
    virtual std::string get_status() = 0;        
    virtual int get_nb_modules() = 0;
    virtual int get_nb_channels() = 0;
    virtual int get_nb_bins(int channel) = 0;
    virtual void set_nb_bins(int channel, int value) = 0;
    virtual double get_preset_type(int channel) = 0;
    virtual void set_preset_type(int channel, double value) = 0;
    virtual double get_preset_value(int channel) = 0;
    virtual void set_preset_value(int channel, double value) = 0;
    virtual double get_peaking_time(int channel) = 0;
    virtual void set_peaking_time(int channel, double value) = 0;
    virtual double get_dynamic_range(int channel) = 0;
    virtual void set_dynamic_range(int channel, double value) = 0;
    virtual double get_num_map_pixels(int channel) = 0;
    virtual void set_num_map_pixels(int channel, double value) = 0;
    virtual double get_num_map_pixels_per_buffer(int channel) = 0;
    virtual void set_num_map_pixels_per_buffer(int channel, double value) = 0;
    virtual double get_number_of_scas(int channel) = 0;
    virtual void set_number_of_scas(int channel, double value) = 0;    
    virtual Tango::DevLong get_mca_length(int channel) = 0;
    virtual void get_mca_data(int channel, int number_of_bins, std::vector<Tango::DevULong>& result) = 0;
    virtual void get_run_data(int channel, const char* name, void* value) = 0;
    virtual int get_nb_rois(int channel) = 0;
    virtual void set_nb_rois(int channel, int nb_rois) = 0;
    virtual void get_roi_bounds(int channel, int roi_num, double& low, double& high) = 0;
    virtual void set_roi_bounds(int channel, int roi_num, double low, double high) = 0;
    virtual double get_roi_data(int channel, int roi_num) = 0;
    virtual bool is_running() = 0;
    virtual std::string get_pixel_advance_mode() = 0;
    virtual void set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel = 0.0) = 0;
    virtual void mapping_pixel_next() = 0;
    virtual bool is_buffer_overrun(int module) = 0;
    virtual bool is_buffer_full(int module, Buffer buffer_num) = 0;
    virtual unsigned long get_buffer_length(int module) = 0;
    virtual unsigned long get_current_pixel() = 0;
    virtual void get_buffer(int module, Buffer buffer_num, unsigned long* buffer) = 0;
    virtual void buffer_done(int module, Buffer buffer_num) = 0;
    virtual void apply_all_board_operation(const char* operation) = 0;
protected:
    ///owner device server
    Tango::DeviceImpl* m_device;    
    std::string m_board_type;
    std::string m_status;
    State m_state;
};

} // namespace XiaDxp_ns


#endif

