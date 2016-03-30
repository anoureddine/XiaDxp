/*************************************************************************
/*! 
 *  \file   HandelHelper.h
 *  \brief  class HandelHelper 
 *  \author Arafat Noureddine - SOLEIL (consultant MEDIANE SYSTEME) /V. Kaiser - ANKA / F. Langlois - SOLEIL
 */
/*************************************************************************/

#ifndef HANDEL_HELPER_H
#define HANDEL_HELPER_H

//- TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/utils/String.h>
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

//- XIA stuff
#include "handel.h"
#include "handel_errors.h"
#include "handel_constants.h"
#include <handel_generic.h>
#include "md_generic.h"
#include <handel_errors.h>
#include <DriverHelper.h>

namespace XiaDxp_ns
{

#define TAG_HEAD0 0x55aa 
#define TAG_HEAD1 0xaa55
#define TAG_DATA0 0x33cc
#define TAG_DATA1 0xcc33

const unsigned long BUFFER_HEADER_SIZE = 256;
const unsigned long PIXEL_HEADER_SIZE = 256;

const std::string XMAP_TYPE = "XMAP";
const std::string SATURN_TYPE = "SATURN";
const std::string MERCURY_TYPE = "MERCURY";
const std::string FALCON_TYPE = "FALCON";

const int XMAP_ID = 1;
const int SATURN_ID = 2;
const int MERCURY_ID = 3;
const int FALCON_ID = 4;

const int STATE_OK = 0;
const int STATE_FAIL = -1;
const int ALL_CHANNELS = -1;

const int MCA = 0;
const int MAP = 1;
const int SCA = 2;


#define XIA_BOARD_LOCK  yat::MutexLock synchronize(this->m_lock);
#define XIA_STATUS_LOCK yat::MutexLock synchronize(this->m_status_lock);


#define CHECK_STATE(state, origin)\
if(state != STATE_OK)\
{\
    Tango::Except::throw_exception ("XIA_ERROR",\
                                    get_error_txt(state).c_str(),\
                                    origin);\
}

class HandelHelper : DriverHelper
{
public:



    /// ctor
    HandelHelper(Tango::DeviceImpl *dev, const std::string & board_type);

    /// dtor
    virtual ~HandelHelper(void);

    /// load config ini file
    void load_config_file(const std::string& config_file);
    /// save modified parameters
    void save_config_file(const std::string& config_file);

    /// start acquisition
    void start_acquisition(short accumulate);
    /// stop acquisition
    void stop_acquisition();

    /// get number of total modules
    int get_nb_modules();

    /// get number of total channels
    int get_nb_channels();

    ///get board state 
    State get_state();
    ///get board status 
    std::string get_status();

    int get_nb_bins(int channel);
    void set_nb_bins(int channel, int value);
    double get_preset_type(int channel);
    void set_preset_type(int channel, double value);
    double get_preset_value(int channel);
    void set_preset_value(int channel, double value);
    double get_peaking_time(int channel);
    void set_peaking_time(int channel, double value);
    double get_dynamic_range(int channel);
    void set_dynamic_range(int channel, double value);
    int get_num_map_pixels(int channel);
    void set_num_map_pixels(int channel, double value);
    int get_num_map_pixels_per_buffer(int channel);
    void set_num_map_pixels_per_buffer(int channel, double value);
    double get_number_of_scas(int channel);
    void set_number_of_scas(int channel, double value);
    int get_nb_rois(int channel);
    void set_nb_rois(int channel, int nb_rois);
    void get_roi_bounds(int channel, int roi_num, double& low, double& high);
    void set_roi_bounds(int channel, int roi_num, double low, double high);
    double get_roi_data(int channel, int roi_number);

    Tango::DevLong get_mca_length(int channel);
    void get_mca_data(int channel, int number_of_bins, std::vector<Tango::DevULong>& result);
    void get_run_data(int channel, const char* name, void* value);


    /// return true if run_active 
    bool is_running();
    /// return the type of pixel advance (SYNC/GATE/HOST) (MAPPING)
    std::string get_pixel_advance_mode();
    /// define the  the type of pixel advance (SYNC/GATE/HOST) (MAPPING)
    void set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel = 0.0);
    /// advance pixel manual
    void mapping_pixel_next();
    /// return true if overrun occured 
    bool is_buffer_overrun(int module);
    /// return true if buffer_num is full 
    bool is_buffer_full(int module, Buffer buffer_num);

    unsigned long get_buffer_length(int module);
    unsigned long get_current_pixel();
    void get_buffer(int module, Buffer buffer_num, unsigned long* buffer);
    void buffer_done(int module, Buffer buffer_num);
    void apply_all_board_operation(const char* operation);
private:
    void init_modules_channels();
    void set_state(State state);
    void set_status(const std::string& status);
    std::string get_error_txt(int state);

    int m_nb_modules;
    int m_nb_channels;
    std::string m_board_type;

    State m_state;
    std::string m_status;

    yat::Mutex m_lock;
    yat::Mutex m_status_lock;
};

} // namespace XiaDxp_ns


#endif

