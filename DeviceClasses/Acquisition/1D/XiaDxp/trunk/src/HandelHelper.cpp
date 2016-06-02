/*************************************************************************
/*! 
 *  \file   HandelHelper.h
 *  \brief  class HandelHelper 
 *  \author 
 */
/*************************************************************************/

#include <tango.h>
#include <map>
#include <yat/utils/XString.h>

#include "HandelHelper.h"


namespace XiaDxp_ns
{

//----------------------------------------------------------------------------------------------------------------------
//- HandelHelper Ctor
//----------------------------------------------------------------------------------------------------------------------
HandelHelper::HandelHelper(Tango::DeviceImpl *dev, const std::string & board_type)
: DriverHelper(dev, board_type),
m_nb_modules(0),
m_nb_channels(0)
{
    INFO_STREAM << "HandelHelper::HandelHelper()- [BEGIN]" << endl;
    INFO_STREAM << "HandelHelper::HandelHelper()- [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- HandelHelper Dtor
//----------------------------------------------------------------------------------------------------------------------
HandelHelper::~HandelHelper(void)
{
    INFO_STREAM << "HandelHelper::~HandelHelper()- [BEGIN]" << endl;
    int state = xiaExit();
    if (state != STATE_OK)
    {
        std::cerr << "Error on xiaExit: " << get_error_txt(state) << std::endl;
    }
    INFO_STREAM << "HandelHelper::~HandelHelper()- [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- Init the XIA library with a new INI configuration file
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::load_config_file(const std::string& config_file)
{
    XIA_BOARD_LOCK
    xiaSetLogLevel(MD_INFO);
    xiaSetLogOutput("handel.log");

    set_state(INITIALIZATION_PROGRESS);
    INFO_STREAM << "=========================================================" << endl;
    INFO_STREAM << "Load config file '" << config_file << "' ..." << endl;
    int state = xiaInit((char*)config_file.c_str());
    if (state != STATE_OK)
    {
        FATAL_STREAM << m_status << endl;
        set_status(get_error_txt(state));
        set_state(INITIALIZATION_FAILED);
        Tango::Except::throw_exception("XIA_ERROR",
                                       get_error_txt(state).c_str(),
                                       "HandelHelper::load_config_file()");
    }

    DEBUG_STREAM << "starting System ..." << endl;
    state = xiaStartSystem();
    if (state != STATE_OK)
    {
        FATAL_STREAM << m_status << endl;
        set_status(get_error_txt(state));
        set_state(INITIALIZATION_FAILED);
        Tango::Except::throw_exception("XIA_ERROR",
                                       get_error_txt(state).c_str(),
                                       "HandelHelper::load_config_file()");
    }

    init_modules_channels();
    set_state(INITIALIZATION_SUCCESSFUL);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::save_config_file(const std::string& config_file)
{
    INFO_STREAM << "=========================================================" << endl;
    INFO_STREAM << "Save config file '" << config_file << "' ..." << endl;
    const int state = xiaSaveSystem("handel_ini", (char*)config_file.c_str());
    CHECK_STATE(state, "HandelHelper::save_ini_file");
}


//----------------------------------------------------------------------------------------------------------------------
//- Get the XIA modules (boards) and channels and Rois on each channels
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::init_modules_channels()
{
    int		nb_channels;
    char	modalias[MAXALIAS_LEN];

    DEBUG_STREAM << "init_modules_channels() ..." << endl;
    //- Modules
    int state = xiaGetNumModules((unsigned int*)(&m_nb_modules));
    INFO_STREAM << "Number of total Modules (boards) :  " << m_nb_modules << endl;
    CHECK_STATE(state, "HandelHelper::HandelHelper::init_modules_channels()");

    if (m_nb_modules == 0)
    {
        Tango::Except::throw_exception ("CONFIG_ERROR",
                                        "No modules defined (please use xManager/Sitoro/... to configure your Board",
                                        "HandelHelper::init_modules_channels()");
    }

    DEBUG_STREAM << "init xiaGetModules_VB/xiaGetModuleItem ..." << endl;
    //- Loop over modules to get channels
    for (int imod = 0; imod < m_nb_modules; imod++)
    {
        state = xiaGetModules_VB(imod, modalias);
        CHECK_STATE(state, "HandelHelper::init_modules_channels()");

        DEBUG_STREAM<<"modalias = "<<modalias<<endl;
        state = xiaGetModuleItem(modalias, "number_of_channels", (void*) &(nb_channels));
        CHECK_STATE(state, "HandelHelper::init_modules_channels()");

        DEBUG_STREAM << "Module #" << imod << ": alias = " << modalias << "; nb channels = " << nb_channels << endl;
        for (int ich = 0; ich < nb_channels; ich++)
        {
            char	chanalias[MAXALIAS_LEN];
            sprintf(chanalias, "channel%d_alias", ich);
            int idet = 0;
            state = xiaGetModuleItem(modalias, chanalias, (void*)&(idet));
            CHECK_STATE(state, "HandelHelper::init_modules_channels()");

            DEBUG_STREAM << "Module #" << imod << ": channel #" << ich << ": idet: " << idet << endl;
            if (idet >= 0)
            {
                m_nb_channels++;
            }
        }

    }

    INFO_STREAM << "Number of total channels : " << m_nb_channels << endl;

    DEBUG_STREAM << "init xiaGetAcquisitionValues ..." << endl;
    //- Loop over Channels to get/display rois
    //num_scas = 0.;
    string acquisition_variable, level;
    for (int ichan = 0; ichan < m_nb_channels; ichan++)
    {
        DEBUG_STREAM << " -- channel " << ichan << endl;

        //- Get number of sca from the loaded file (rois)
        double num_scas = get_number_of_scas(ichan);
        INFO_STREAM << "Number of sca (from the loaded file) for channel " << ichan << ": " << num_scas << endl;

        //- MAX roi: 32 because of max length of attribute roisStartsEnds (could be increased)
        if(num_scas > 32)
        {
            Tango::Except::throw_exception(	"MAX_VALUE_REACHED",
                                           "You are able to only have 32 rois",
                                           "HandelHelper::init_modules_channels");
        }

        //- For Info purpose
        for (int i = 0; i < num_scas * 2; i++)
        {
            //- get roi hi and lo values
            int num_roi;
            if((i % 2) == 0)
            {
                level = "_lo";
            }
            else
            {
                level = "_hi";
            }

            num_roi = i / 2;
            acquisition_variable = "sca" + yat::StringUtil::to_string<int>(num_roi) + level ; //sca{n}_[lo|hi]
            DEBUG_STREAM << "acquisition_variable = " << acquisition_variable;

            double dvalue = 0.0;
            const int state = xiaGetAcquisitionValues(ichan, (char*)acquisition_variable.c_str(), &dvalue);
            CHECK_STATE(state,
                        std::string("HandelHelper::init_modules_channels")
                        +std::string("(channel = ")
                        +yat::StringUtil::to_string<int>(ichan)
                        +std::string(", name ='")
                        +acquisition_variable
                        +std::string("')")
                        );

            DEBUG_STREAM << "channel " << ichan << ": " << acquisition_variable << " = " << dvalue << endl;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
State HandelHelper::get_state()
{
    XIA_STATUS_LOCK
    return m_state;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string HandelHelper::get_status()
{
    XIA_STATUS_LOCK
    return m_status;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
int HandelHelper::get_nb_modules()
{
    return m_nb_modules;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int HandelHelper::get_nb_channels()
{
    return m_nb_channels;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int HandelHelper::get_nb_bins(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "number_mca_channels", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_nb_bins")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("number_mca_channels")
                +std::string("')")
                );
    return (int)dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_nb_bins(int channel, int value)
{
	double dvalue = (double)value;
    const int state = xiaSetAcquisitionValues(channel, "number_mca_channels", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::set_nb_bins")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("number_mca_channels")
                + std::string("')")
                );
}



//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double HandelHelper::get_preset_type(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "preset_type", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_preset_type")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("preset_type")
                +std::string("')")
                );
    return dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_preset_type(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "preset_type", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_preset_type")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("preset_type")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double HandelHelper::get_preset_value(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "preset_value", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_preset_value")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("preset_value")
                +std::string("')")
                );
    return dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_preset_value(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "preset_value", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_preset_value")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("preset_value")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double HandelHelper::get_peaking_time(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "peaking_time", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_peaking_time")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("get_peaking_time")
                +std::string("')")
                );
    return dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_peaking_time(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "peaking_time", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_peaking_time")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("peaking_time")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double HandelHelper::get_dynamic_range(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "dynamic_range", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_dynamic_range")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("dynamic_range")
                +std::string("')")
                );
    return dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_dynamic_range(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "dynamic_range", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_dynamic_range")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("dynamic_range")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int HandelHelper::get_num_map_pixels(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "num_map_pixels", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_num_map_pixels")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("num_map_pixels")
                +std::string("')")
                );
    return (int)dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_num_map_pixels(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "num_map_pixels", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_num_map_pixels")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("num_map_pixels")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
int HandelHelper::get_num_map_pixels_per_buffer(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "num_map_pixels_per_buffer", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_num_map_pixels_per_buffer")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("num_map_pixels_per_buffer")
                +std::string("')")
                );
    return (int)dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_num_map_pixels_per_buffer(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "num_map_pixels_per_buffer", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_num_map_pixels_per_buffer")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("num_map_pixels_per_buffer")
                + std::string("')")
                );
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
double HandelHelper::get_number_of_scas(int channel)
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(channel, "number_of_scas", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_number_of_scas")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(channel)
                +std::string(", name ='")
                +std::string("number_of_scas")
                +std::string("')")
                );
    return dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------    
void HandelHelper::set_number_of_scas(int channel, double value)
{
    const int state = xiaSetAcquisitionValues(channel, "number_of_scas", &value);
    CHECK_STATE(state,
                std::string("HandelHelper::set_number_of_scas")
                + std::string("(channel_number = ")
                + yat::XString<int>::to_string(channel)
                + std::string(", name = '")
                + std::string("number_of_scas")
                + std::string("')")
                );
    apply_all_board_operation("apply");    
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool HandelHelper::is_running()
{
    for (int i = 0; i < m_nb_modules; ++i)
    {
        int is_running = 0;

        get_run_data(i * 4, "run_active", &is_running);
        if (is_running == 0 || is_running == 2) //- Not Running
        {
            ///DEBUG_STREAM << "------- IS NOT RUNNING" << std::endl;
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::start_acquisition(short accumulate)
{
    XIA_BOARD_LOCK
    const int state = xiaStartRun(ALL_CHANNELS, accumulate);
    CHECK_STATE(state, "HandelHelper::start_acquisition");
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::stop_acquisition()
{
    XIA_BOARD_LOCK
    const int state = xiaStopRun(ALL_CHANNELS);
    CHECK_STATE(state, "HandelHelper::stop_acquisition");
}


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Tango::DevLong HandelHelper::get_mca_length(int channel)
{

    XIA_BOARD_LOCK
    Tango::DevLong   result;
    const int state = xiaGetRunData(channel, "mca_length", &result);
    CHECK_STATE(state, "HandelHelper::get_mca_length");
    return result;

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::get_mca_data(int channel, int number_of_bins, std::vector<Tango::DevULong>& result)
{
    XIA_BOARD_LOCK
    if (result.size() != number_of_bins)
    {
        result.resize(number_of_bins);
    }
    const int state = xiaGetRunData(channel, "mca", &result[0]);
    CHECK_STATE(state, "HandelHelper::read_channel");
}


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
int HandelHelper::get_nb_rois(int channel)
{
    double num_scas = get_number_of_scas(channel);
    return (int)num_scas;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::set_nb_rois(int channel, int nb_rois)
{
    set_number_of_scas(channel, (double)nb_rois);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::get_roi_bounds(int channel, int roi_num, double& low, double& high)
{
    int num_scas = (int)get_number_of_scas(channel);
    if(roi_num>=num_scas)
    {
        Tango::Except::throw_exception ("LOGIC_ERROR",
                                        "The roi number is out of limits [1, total number of rois] !",
                                        "get_roi_bounds");
    }

    XIA_BOARD_LOCK
    double dvalue = 0.0;
    std::string acquisition_variable = "sca" + yat::StringUtil::to_string<int>(roi_num) + "_lo";
    int state = xiaGetAcquisitionValues(channel, (char*)acquisition_variable.c_str(), &dvalue);
    CHECK_STATE(state, "HandelHelper::get_roi_bounds " + acquisition_variable);
    DEBUG_STREAM << " Roi " << roi_num + 1 << " -> low value = " << dvalue << endl;
    low = dvalue;

    acquisition_variable = "sca" + yat::StringUtil::to_string<int>(roi_num) + "_hi";
    state = xiaGetAcquisitionValues(channel, (char*)acquisition_variable.c_str(), &dvalue);
    CHECK_STATE(state, "HandelHelper::get_roi_bounds " + acquisition_variable);
    DEBUG_STREAM << " Roi " << roi_num + 1 << " -> high value = " << dvalue << endl;
    high = dvalue;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::set_roi_bounds(int channel, int roi_num, double low, double high)
{
    INFO_STREAM << "-----------------------------" << endl;
    INFO_STREAM << "channel : " << channel << endl;
    string conf_low = "sca" + yat::StringUtil::to_string<int>(roi_num) + "_lo" ; //sca{n}_[lo]
    INFO_STREAM << "conf_low = " << conf_low << " : value = "<<low<<endl;
    string conf_high = "sca" + yat::StringUtil::to_string<int>(roi_num) + "_hi" ; //sca{n}_[hi]
    INFO_STREAM << "conf_high = " << conf_high << " : value = "<<high<<endl;

    const int state_low = xiaSetAcquisitionValues(channel, const_cast<char*>(conf_low.c_str()), &low);
    CHECK_STATE(state_low,
                std::string("HandelHelper::set_roi_bounds")
                + std::string("(channel_number = ")
                + yat::StringUtil::to_string<int>(channel)
                + std::string(", roi_num ='")
                + yat::StringUtil::to_string<int>(roi_num)
                + std::string(", low ='")
                + yat::StringUtil::to_string<double>(low)
                + std::string(", high ='")
                + yat::StringUtil::to_string<double>(high)
                + std::string("')")
                );

    const int state_high = xiaSetAcquisitionValues(channel, const_cast<char*>(conf_high.c_str()), &high);
    CHECK_STATE(state_high,
                std::string("HandelHelper::set_roi_bounds")
                + std::string("(channel_number = ")
                + yat::StringUtil::to_string<int>(channel)
                + std::string(", roi_num ='")
                + yat::StringUtil::to_string<int>(roi_num)
                + std::string(", low ='")
                + yat::StringUtil::to_string<double>(low)
                + std::string(", high ='")
                + yat::StringUtil::to_string<double>(high)
                + std::string("')")
                );

    apply_all_board_operation("apply");
    INFO_STREAM << "-----------------------------" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
double HandelHelper::get_roi_data(int channel, int roi_number)
{
    XIA_BOARD_LOCK
    unsigned short sca_length = 1;

    int state = xiaGetRunData(channel, "sca_length", (void*)&sca_length);
    CHECK_STATE(state, "HandelHelper::get_roi_data(sca_length)");

    double scadata[32];
    memset(scadata, 0, 32 * sizeof(double));

    state = xiaGetRunData(channel, "sca", scadata);
    CHECK_STATE(state, "HandelHelper::get_roi_data(sca_length)");

    return scadata[roi_number];
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel /*= 0.0*/)
{
    XIA_BOARD_LOCK
    double enabled = 1.0;
    double pixelMode = XIA_MAPPING_CTL_GATE;
    int state = -1;
    std::string mode_type = mode;
    std::transform(mode_type.begin(), mode_type.end(), mode_type.begin(), ::toupper);
    if(mode_type == "GATE")
    {
        pixelMode = XIA_MAPPING_CTL_GATE;
        if (m_board_type == XMAP_TYPE)
        {
            state = xiaSetAcquisitionValues(0, "gate_master", &enabled);
            CHECK_STATE(state, "HandelHelper::set_pixel_advance_mode() (gate_master)");
        }
        state = xiaSetAcquisitionValues(0, "pixel_advance_mode", &pixelMode);
        CHECK_STATE(state, "HandelHelper::set_pixel_advance_mode() (pixel_advance_mode)");
    }
    else if(mode_type == "SYNC")
    {
        pixelMode = XIA_MAPPING_CTL_SYNC;
        if (m_board_type == XMAP_TYPE)
        {
            state = xiaSetAcquisitionValues(0, "sync_master", &enabled);
            CHECK_STATE(state, "HandelHelper::set_pixel_advance_mode() (sync_master)");
        }
        state = xiaSetAcquisitionValues(0, "pixel_advance_mode", &pixelMode);
        CHECK_STATE(state, "HandelHelper::set_pixel_advance_mode() (pixel_advance_mode)");
        state = xiaSetAcquisitionValues(0, "sync_count", &ticks_per_pixel);
        CHECK_STATE(state, "HandelHelper::set_pixel_advance_mode() (sync_count)");
    }
    else if(mode_type == "HOST")
    {
        // nothing to do
    }
    else
    {
        Tango::Except::throw_exception ("XIA_ERROR",
                                        "Unknown pixel advance mode : " + mode_type,
                                        "HandelHelper::set_pixel_advance_mode()");
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string HandelHelper::get_pixel_advance_mode()
{
    double dvalue = 0.0;
    const int state = xiaGetAcquisitionValues(0, "pixel_advance_mode", &dvalue);
    CHECK_STATE(state,
                std::string("HandelHelper::get_pixel_advance_mode")
                +std::string("(channel = ")
                +yat::StringUtil::to_string<int>(0)
                +std::string(", name ='")
                +std::string("pixel_advance_mode")
                +std::string("')")
                );

    if (dvalue == XIA_MAPPING_CTL_GATE)
    {
        return "GATE";
    }
    if (dvalue == XIA_MAPPING_CTL_SYNC)
    {
        return "SYNC";
    }
    return "HOST";
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
unsigned long HandelHelper::get_buffer_length(int module)
{
    unsigned long result = 0;
    get_run_data(module * 4, "buffer_len", &result);
    return result;

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
unsigned long HandelHelper::get_current_pixel()
{
    //module is not used !!
    int detChan = 0;
    unsigned long result = 0;
    get_run_data(detChan, "current_pixel", &result);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool HandelHelper::is_buffer_overrun(int module)
{
    unsigned short result = 0;
    get_run_data(module * 4, "buffer_overrun", &result);
    return result != 0;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool HandelHelper::is_buffer_full(int module, Buffer buffer_num)
{
    static char buffers[] = "ab";
    char buffer_name[20];
    sprintf(buffer_name, "buffer_full_%c", buffers[buffer_num]);
    unsigned short is_full = 0;
    get_run_data(module * 4, buffer_name, &is_full);

    if (0 != is_full)
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::get_buffer(int module, Buffer buffer_num, unsigned long* buffer)
{
    static char buffers[] = "ab";
    char buffer_name[20];
    sprintf(buffer_name, "buffer_%c", buffers[buffer_num]);
    get_run_data(module * 4, buffer_name, buffer);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::buffer_done(int module, Buffer buffer_num)
{
    XIA_BOARD_LOCK
    static char buffers[] = "ab";
    const int state = xiaBoardOperation(module * 4, "buffer_done", &buffers[buffer_num]);
    CHECK_STATE(state, "HandelHelper::buffer_done " + buffers[buffer_num]);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::get_run_data(int channel, const char* name, void* value)
{
    XIA_BOARD_LOCK
    const int state = xiaGetRunData(channel, (char*)name, value);

    CHECK_STATE(state,
                "HandelHelper::get_run_data(detChan="
                + yat::StringUtil::to_string<int>(channel)
                + ", name='"
                + name + "'");
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::mapping_pixel_next()
{
    apply_all_board_operation("mapping_pixel_next");

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::set_state(State state)
{
    XIA_STATUS_LOCK
    m_state = state;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::set_status(const std::string& status)
{
    XIA_STATUS_LOCK
    m_status = status;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HandelHelper::apply_all_board_operation(const char* operation)
{
    XIA_BOARD_LOCK
    int ignored = 0;

    for (int i = 0; i < m_nb_channels; i += 4)
    {
        const int state = xiaBoardOperation(i, (char*)operation, (void *)&ignored);
        CHECK_STATE(state,
                    "HandelHelper::apply_on_all_boards "
                    + yat::StringUtil::to_string<int>(i));
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string HandelHelper::get_error_txt(int state)
{
    std::string msg;

    switch (state)
    {
        case XIA_OPEN_FILE:	msg = "Unable to open the specified .ini file";
            break;
        case XIA_FILEERR:	msg = "Error getting firmware from FDD file";
            break;
        case XIA_NOSECTION:	msg = "Section missing in file";
            break;
        case XIA_FORMAT_ERROR:	msg = "File is improperly formatted";
            break;
        case XIA_ILLEGAL_OPERATION:	msg = "Attempted to configure options in an illegal order ";
            break;
        case XIA_FILE_RA:	msg = "File random access unable to find name-value pair ";
            break;
        case XIA_UNKNOWN_DECIMATION:	msg = "The decimation read from the hardware does not match a known value ";
            break;
        case XIA_SLOWLEN_OOR:	msg = "Calculated SLOWLEN value is out-of-range ";
            break;
        case XIA_SLOWGAP_OOR:	msg = "Calculated SLOWGAP value is out-of-range ";
            break;
        case XIA_SLOWFILTER_OOR:	msg = "Attempt to set the Peaking or Gap time s.t. P+G>31 ";
            break;
        case XIA_FASTLEN_OOR:	msg = "Calculated FASTLEN value is out-of-range ";
            break;
        case XIA_FASTGAP_OOR:	msg = "Calculated FASTGAP value is out-of-range ";
            break;
        case XIA_FASTFILTER_OOR:	msg = "Attempt to set the Peaking or Gap time s.t. P+G>31 ";
            break;
        case XIA_BASELINE_OOR:	msg = "Baseline filter length is out-of-range ";
            break;
        case XIA_INITIALIZE:	msg = "XIA_INITIALIZE";
            break;
        case XIA_NO_ALIAS:	msg = "Specified alias does not exist";
            break;
        case XIA_ALIAS_EXISTS:	msg = "Firmware with the specified alias already exists";
            break;
        case XIA_BAD_VALUE:	msg = "Error with value passed in";
            break;
        case XIA_INFINITE_LOOP:	msg = "Problem with detChan and detChan Set definitions such that an infinite loop exists. This prevents against situations where a detChan (or Set) refers to another detChan (or Set) that then refers back to itself";
            break;
        case XIA_BAD_NAME:	msg = "Specified name is not valid ";
            break;
        case XIA_BAD_PTRR:	msg = "Specified PTRR is not valid for this alias ";
            break;
        case XIA_ALIAS_SIZE:	msg = "Alias name has too many characters ";
            break;
        case XIA_NO_MODULE:	msg = "Must define at least one module before ";
            break;
        case XIA_BAD_INTERFACE:	msg = "The specified interface does not exist ";
            break;
        case XIA_NO_INTERFACE:	msg = "An interface must defined before more information is specified ";
            break;
        case XIA_WRONG_INTERFACE:	msg = "Specified information doesn't apply to this interface ";
            break;
        case XIA_NO_CHANNELS:	msg = "Number of channels for this module is set to 0 ";
            break;
        case XIA_BAD_CHANNEL:	msg = "Specified channel index is invalid or out-of-range ";
            break;
        case XIA_NO_MODIFY:	msg = "Specified name cannot be modified once set ";
            break;
        case XIA_INVALID_DETCHAN:	msg = "Specified detChan value is invalid ";
            break;
        case XIA_BAD_TYPE:	msg = "The DetChanElement type specified is invalid ";
            break;
        case XIA_WRONG_TYPE:	msg = "This routine only operates on detChans that are sets ";
            break;
        case XIA_UNKNOWN_BOARD:	msg = "Board type is unknown ";
            break;
        case XIA_NO_DETCHANS:	msg = "No detChans are currently defined ";
            break;
        case XIA_NOT_FOUND:	msg = "Unable to locate the HandelHelper value requested ";
            break;
        case XIA_PTR_CHECK:	msg = "Pointer is out of synch when it should be valid ";
            break;
        case XIA_LOOKING_PTRR:	msg = "FirmwareSet has a FDD file defined and this only works with PTRRs ";
            break;
        case XIA_NO_FILENAME:	msg = "Requested filename information is set to NULL ";
            break;
        case XIA_BAD_INDEX:	msg = "User specified an alias index that doesn't exist ";
            break;
        case XIA_NULL_ALIAS:	msg = "Null alias passed into function ";
            break;
        case XIA_NULL_NAME:	msg = "Null name passed into function ";
            break;
        case XIA_NULL_VALUE:	msg = "Null value passed into function ";
            break;
        case XIA_NEEDS_BOARD_TYPE:	msg = "Module needs board_type ";
            break;
        case XIA_UNKNOWN_ITEM:	msg = "Unknown item ";
            break;
        case XIA_TYPE_REDIRECT:	msg = "Module type can not be redefined once set ";
            break;
        case XIA_FIRM_BOTH:	msg = "A FirmwareSet may not contain both an FDD and seperate Firmware definitions ";
            break;
        case XIA_PTR_OVERLAP:	msg = "Peaking time ranges in the Firmware definitions may not overlap ";
            break;
        case XIA_MISSING_FIRM:	msg = "Either the FiPPI or DSP file is missing from a Firmware element ";
            break;
        case XIA_MISSING_POL:	msg = "A polarity value is missing from a Detector element ";
            break;
        case XIA_MISSING_GAIN:	msg = "A gain value is missing from a Detector element ";
            break;
        case XIA_MISSING_INTERFACE:	msg = "The interface this channel requires is missing ";
            break;
        case XIA_MISSING_ADDRESS:	msg = "The epp_address information is missing for this channel ";
            break;
        case XIA_INVALID_NUMCHANS:	msg = "The wrong number of channels are assigned to this module ";
            break;
        case XIA_INCOMPLETE_DEFAULTS:	msg = "Some of the required defaults are missing ";
            break;
        case XIA_BINS_OOR:	msg = "There are too many or too few bins for this module type ";
            break;
        case XIA_MISSING_TYPE:	msg = "The type for the current detector is not specified properly ";
            break;
        case XIA_NO_MMU:	msg = "No MMU defined and/or required for this module ";
            break;
        case XIA_NULL_FIRMWARE:	msg = "No firmware set defined ";
            break;
        case XIA_NO_FDD:	msg = "No FDD defined in the firmware set ";
            break;
        case XIA_WRONG_DET_TYPE:	msg = "The detector type is wrong for the requested operation ";
            break;
        case XIA_NOMEM:	msg = "Unable to allocate memory ";
            break;
        case XIA_XERXES:	msg = "XerXes returned an error ";
            break;
        case XIA_MD:	msg = "MD layer returned an error ";
            break;
        case XIA_EOF:	msg = "EOF encountered ";
            break;
        case XIA_XERXES_NORMAL_RUN_ACTIVE:	msg = "XerXes says a normal run is still active ";
            break;
        case XIA_XERXES_CONTROL_RUN_ACTIVE:	msg = "XerXes says a control run is still active ";
            break;
            /*    case XIA_HARDWARE_RUN_ACTIVE:	msg = "The hardware says a control run is still active "; break;*/
        case XIA_UNKNOWN:	msg = "Internal error. Contact XIA.";
            break;
        case XIA_LOG_LEVEL:	msg = "Log level invalid ";
            break;
        case XIA_NO_LIST:	msg = "List size is zero ";
            break;
        case XIA_NO_ELEM:	msg = "No data to remove ";
            break;
        case XIA_DATA_DUP:	msg = "Data already in table ";
            break;
        case XIA_REM_ERR:	msg = "Unable to remove entry from hash table ";
            break;
        case XIA_FILE_TYPE:	msg = "Improper file type specified ";
            break;
        case XIA_END:	msg = "There are no more instances of the name specified. Pos set to end ";
            break;
        case XIA_INVALID_STR:	msg = "Invalid string format ";
            break;
        case XIA_UNIMPLEMENTED:	msg = "The routine is unimplemented in this version ";
            break;
        case XIA_NOSUPPORT_FIRM:	msg = "The specified firmware is not supported by this board type ";
            break;
        case XIA_UNKNOWN_FIRM:	msg = "The specified firmware type is unknown ";
            break;
        case XIA_NOSUPPORT_VALUE:	msg = "The specified acquisition value is not supported ";
            break;
        case XIA_UNKNOWN_VALUE:	msg = "The specified acquisition value is unknown ";
            break;
        case XIA_PEAKINGTIME_OOR:	msg = "The specified peaking time is out-of-range for this product ";
            break;
        case XIA_NODEFINE_PTRR:	msg = "The specified peaking time does not have a PTRR associated with it ";
            break;
        case XIA_THRESH_OOR:	msg = "The specified treshold is out-of-range ";
            break;
        case XIA_ERROR_CACHE:	msg = "The data in the values cache is out-of-sync ";
            break;
        case XIA_GAIN_OOR:	msg = "The specified gain is out-of-range for this produce ";
            break;
        case XIA_TIMEOUT:	msg = "Timeout waiting for BUSY ";
            break;
        case XIA_BAD_SPECIAL:	msg = "The specified special run is not supported for this module ";
            break;
        case XIA_TRACE_OOR:	msg = "The specified value of tracewait (in ns) is out-of-range ";
            break;
        case XIA_DEFAULTS:	msg = "The PSL layer encountered an error creating a Defaults element ";
            break;
        case XIA_BAD_FILTER:	msg = "Error loading filter info from either a FDD file or the Firmware configuration ";
            break;
        case XIA_NO_REMOVE:	msg = "Specified acquisition value is required for this product and can't be removed ";
            break;
        case XIA_NO_GAIN_FOUND:	msg = "Handel was unable to converge on a stable gain value ";
            break;
        case XIA_UNDEFINED_RUN_TYPE:	msg = "Handel does not recognize this run type ";
            break;
        case XIA_INTERNAL_BUFFER_OVERRUN:	msg = "Handel attempted to overrun an internal buffer boundry ";
            break;
        case XIA_EVENT_BUFFER_OVERRUN:	msg = "Handel attempted to overrun the event buffer boundry ";
            break;
        case XIA_BAD_DATA_LENGTH:	msg = "Handel was asked to set a Data length to zero for readout ";
            break;
        case XIA_NO_LINEAR_FIT:	msg = "Handel was unable to perform a linear fit to some data ";
            break;
        case XIA_MISSING_PTRR:	msg = "Required PTRR is missing ";
            break;
        case XIA_PARSE_DSP:	msg = "Error parsing DSP ";
            break;
        case XIA_UDXPS:	msg = "XIA_UDXPS";
            break;
        case XIA_BIN_WIDTH:	msg = "Specified bin width is out-of-range ";
            break;
        case XIA_NO_VGA:	msg = "An attempt was made to set the gaindac on a board that doesn't have a VGA ";
            break;
        case XIA_TYPEVAL_OOR:	msg = "Specified detector type value is out-of-range ";
            break;
        case XIA_LOW_LIMIT_OOR:	msg = "Specified low MCA limit is out-of-range ";
            break;
        case XIA_BPB_OOR:	msg = "bytes_per_bin is out-of-range ";
            break;
        case XIA_FIP_OOR:	msg = "Specified FiPPI is out-fo-range ";
            break;
        case XIA_MISSING_PARAM:	msg = "Unable to find DSP parameter in list ";
            break;
        case XIA_OPEN_XW:	msg = "Error opening a handle in the XW library ";
            break;
        case XIA_ADD_XW:	msg = "Error adding to a handle in the XW library ";
            break;
        case XIA_WRITE_XW:	msg = "Error writing out a handle in the XW library ";
            break;
        case XIA_VALUE_VERIFY:	msg = "Returned value inconsistent with sent value ";
            break;
        case XIA_POL_OOR:	msg = "Specifed polarity is out-of-range ";
            break;
        case XIA_SCA_OOR:	msg = "Specified SCA number is out-of-range ";
            break;
        case XIA_BIN_MISMATCH:	msg = "Specified SCA bin is either too high or too low ";
            break;
        case XIA_WIDTH_OOR:	msg = "MCA bin width is out-of-range ";
            break;
        case XIA_UNKNOWN_PRESET:	msg = "Unknown PRESET run type specified ";
            break;
        case XIA_GAIN_TRIM_OOR:	msg = "Gain trim out-of-range ";
            break;
        case XIA_GENSET_MISMATCH:	msg = "Returned GENSET doesn't match the set GENSET ";
            break;
        case XIA_NUM_MCA_OOR:	msg = "The specified number of MCA bins is out of range ";
            break;
        case XIA_PEAKINT_OOR:	msg = "The specified value for PEAKINT is out-of-range. ";
            break;
        case XIA_PEAKSAM_OOR:	msg = "The specified value for PEAKSAM is out-of-range. ";
            break;
        case XIA_MAXWIDTH_OOR:	msg = "The specified value for MAXWIDTH is out-of-range.";
            break;
        case XIA_NULL_TYPE:	msg = "A NULL file type was specified ";
            break;
        case XIA_GAIN_SCALE:	msg = "Gain scale factor is not valid ";
            break;
        case XIA_NULL_INFO:	msg = "The specified info array is NULL ";
            break;
        case XIA_UNKNOWN_PARAM_DATA:	msg = "Unknown parameter data type ";
            break;
        case XIA_MAX_SCAS:  msg = "The specified number of SCAs is more then the maximum allowed ";
            break;
        case XIA_UNKNOWN_BUFFER:    msg = "Requested buffer is unknown ";
            break;
        case XIA_NO_MAPPING:        msg = "Mapping mode is currently not installed/enabled ";
            break;
        case XIA_MAPPING_PT_CTL:    msg = "Wrong mapping point control for operation ";
            break;
        case XIA_UNKNOWN_PT_CTL:    msg = "Unknown mapping point control. ";
            break;
        case XIA_CLOCK_SPEED:       msg = "The hardware is reporting an invalid clock speed. ";
            break;
        case XIA_BAD_DECIMATION:	msg = "Passed in decimation is invalid. ";
            break;
        case XIA_BAD_SYNCH_RUN:	msg = "Specified value for synchronous run is bad. ";
            break;
        case XIA_XUP_VERSION:	msg = "XUP version is not supported ";
            break;
        case XIA_CHKSUM:	msg = "checksum mismatch in the XUP ";
            break;
        case XIA_BAK_MISSING:	msg = "Requested BAK file cannot be opened ";
            break;
        case XIA_SIZE_MISMATCH:	msg = "Size read from file is incorrect ";
            break;
        case XIA_NO_ACCESS:	msg = "Specified access file isn't valid ";
            break;
        case XIA_N_FILTER_BAD:	msg = "The number of filter parameters in the FDD doesn't match the number requires for the hardware ";
            break;
        case XIA_UNIT_TEST:	msg = "XIA_UNIT_TEST";
            break;
        default: msg = "UNKNOWN_ERROR";
    }

    return msg;
}

} // namespace 

