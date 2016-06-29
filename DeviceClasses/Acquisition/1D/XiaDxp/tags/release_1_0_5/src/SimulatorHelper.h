/*************************************************************************
/*! 
 *  \file   SimulatorHelper.h
 *  \brief  class SimulatorHelper 
 *  \author Arafat Noureddine - SOLEIL (consultant MEDIANE SYSTEME)
 */
/*************************************************************************/

#ifndef SIMULATOR_HELPER_H
#define SIMULATOR_HELPER_H

//- TANGO
#include <tango.h>

#include <yat/time/Timer.h>
#include <yat/threading/Thread.h>
#include <math.h>
#include <DriverHelper.h>

////////////////////////////////////////////////////////
// HARDWARE CONSTANTS
////////////////////////////////////////////////////////
#define TAG_HEAD0 0x55aa 
#define TAG_HEAD1 0xaa55
#define TAG_DATA0 0x33cc
#define TAG_DATA1 0xcc33
const unsigned long BUFFER_HEADER_SIZE = 256;
const unsigned long PIXEL_HEADER_SIZE = 256;
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// ACQUISITION values
////////////////////////////////////////////////////////
const int NUMERIC_NB_MODULES = 2;
const int NUMERIC_NB_CHANNELS = 8;
const int NUMERIC_NB_BINS = 2048;
const int NUMERIC_MCA_LENGTH = 2048;
const int NUMERIC_BUFFER_LENGTH = BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 4 * NUMERIC_NB_BINS;
const double NUMERIC_DYNAMIC_RANGE = 3.0;
const double NUMERIC_PEAKING_TIME = 2.0;
const double NUMERIC_PRESET_VALUE = 4.0; //in s
const double NUMERIC_NUMBER_MCA_CHANNELS = 2048; //nb_bins
const double NUMERIC_REALTIME = 2.2;
const double NUMERIC_LIVETIME = 3.3;
const double NUMERIC_TRIGGER_LIVETIME = 1.1;
const double NUMERIC_ICR = 5.5;
const double NUMERIC_OCR = 4.4;
const unsigned long NUMERIC_EVENTS_IN_RUN = 6;
const double NUMERIC_NUM_MAP_PIXELS = 1000;
const double NUMERIC_NUM_MAP_PIXELS_PER_BUFFER = 100;
const std::string NUMERIC_PIXEL_ADVANCE_MODE = "GATE";
const unsigned long NUMERIC_MAPPING_CLOCK_MS = 10; //ms
////////////////////////////////////////////////////////

namespace XiaDxp_ns
{
class SimulatorHelper : public DriverHelper
{
public:

    ////////////////////////////////////////////////////////////////////////
    SimulatorHelper(Tango::DeviceImpl *dev, const std::string & board_type) : DriverHelper(dev, board_type)
    {
        INFO_STREAM << "SimulatorHelper::SimulatorHelper()- [BEGIN]" << endl;
        m_status = "Board Type : " + board_type;
        m_state = INITIALIZATION_SUCCESSFUL;
        m_is_running = false;
        m_numero_pixel[0] = 0;
        m_numero_pixel[1] = 0;
        m_map_rois.clear();
        //prepare boards parameters values
        m_map_parameters.clear();
        m_map_parameters["nb_modules"] = yat::StringUtil::to_string<int>(NUMERIC_NB_MODULES);
        m_map_parameters["nb_channels"] = yat::StringUtil::to_string<int>(NUMERIC_NB_CHANNELS);
        m_map_parameters["mca_length"] = yat::StringUtil::to_string<double>(NUMERIC_MCA_LENGTH);
        m_map_parameters["buffer_len"] = yat::StringUtil::to_string<double>(NUMERIC_BUFFER_LENGTH);
        m_map_parameters["number_mca_channels"] = yat::StringUtil::to_string<double>(NUMERIC_NUMBER_MCA_CHANNELS);
        m_map_parameters["dynamic_range"] = yat::StringUtil::to_string<double>(NUMERIC_DYNAMIC_RANGE);
        m_map_parameters["peaking_time"] = yat::StringUtil::to_string<double>(NUMERIC_PEAKING_TIME);
        m_map_parameters["preset_value"] = yat::StringUtil::to_string<double>(NUMERIC_PRESET_VALUE);
        m_map_parameters["preset_type"] = yat::StringUtil::to_string<double>(XIA_PRESET_FIXED_REAL);
        m_map_parameters["preset_value"] = yat::StringUtil::to_string<double>(NUMERIC_PRESET_VALUE);
        m_map_parameters["num_map_pixels"] = yat::StringUtil::to_string<double>(NUMERIC_NUM_MAP_PIXELS);
        m_map_parameters["num_map_pixels_per_buffer"] = yat::StringUtil::to_string<double>(NUMERIC_NUM_MAP_PIXELS_PER_BUFFER);
        m_map_parameters["pixel_advance_mode"] = std::string(NUMERIC_PIXEL_ADVANCE_MODE);
        m_map_parameters["realtime"] = yat::StringUtil::to_string<double>(NUMERIC_REALTIME);
        m_map_parameters["livetime"] = yat::StringUtil::to_string<double>(NUMERIC_LIVETIME);
        m_map_parameters["trigger_livetime"] = yat::StringUtil::to_string<double>(NUMERIC_TRIGGER_LIVETIME);
        m_map_parameters["input_count_rate"] = yat::StringUtil::to_string<double>(NUMERIC_ICR);
        m_map_parameters["output_count_rate"] = yat::StringUtil::to_string<double>(NUMERIC_OCR);
        m_map_parameters["events_in_run"] = yat::StringUtil::to_string<unsigned long>(NUMERIC_EVENTS_IN_RUN);
        //
        m_timeout = yat::Timeout(NUMERIC_PRESET_VALUE, yat::Timeout::TMO_UNIT_SEC); //in s
        INFO_STREAM << "SimulatorHelper::SimulatorHelper()- [END]" << endl;
    }

    ////////////////////////////////////////////////////////////////////////
    virtual ~SimulatorHelper()
    {
        INFO_STREAM << "SimulatorHelper::SimulatorHelper()- [BEGIN]" << endl;
        INFO_STREAM << "SimulatorHelper::SimulatorHelper()- [END]" << endl;
    }

    ////////////////////////////////////////////////////////////////////////
    void load_config_file(const std::string& config_file)
    {
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void save_config_file(const std::string& config_file)
    {
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void start_acquisition(short accumulate)
    {
        m_is_running = true;
        m_numero_pixel[0] = 0;
        m_numero_pixel[1] = 0;
        m_timeout.restart();
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void stop_acquisition()
    {
        m_is_running = false;
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    int get_nb_modules()
    {
        return yat::StringUtil::to_num<int>(m_map_parameters["nb_modules"]);
    }

    ////////////////////////////////////////////////////////////////////////
    int get_nb_channels()
    {
        return yat::StringUtil::to_num<int>(m_map_parameters["nb_channels"]);
    }

    ////////////////////////////////////////////////////////////////////////    
    int get_nb_bins(int channel)
    {
        return yat::StringUtil::to_num<int>(m_map_parameters["number_mca_channels"]);
    }

    ////////////////////////////////////////////////////////////////////////    
    void set_nb_bins(int channel, int value)
    {
        m_map_parameters["number_mca_channels"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    double get_preset_type(int channel)
    {
        return yat::StringUtil::to_num<double>(m_map_parameters["preset_type"]);
    }

    ////////////////////////////////////////////////////////////////////////
    void set_preset_type(int channel, double value)
    {
        m_map_parameters["preset_type"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    double get_preset_value(int channel)
    {
        return yat::StringUtil::to_num<double>(m_map_parameters["preset_value"]);
    }

    ////////////////////////////////////////////////////////////////////////
    void set_preset_value(int channel, double value)
    {
        m_map_parameters["preset_value"] = yat::StringUtil::to_string<double>(value);
        m_timeout.set_value(value);
    }
    ////////////////////////////////////////////////////////////////////////
    double get_peaking_time(int channel)
    {
        return yat::StringUtil::to_num<double>(m_map_parameters["peaking_time"]);
    }

    ////////////////////////////////////////////////////////////////////////
    void set_peaking_time(int channel, double value)
    {
        m_map_parameters["peaking_time"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    double get_dynamic_range(int channel)
    {
        return yat::StringUtil::to_num<double>(m_map_parameters["dynamic_range"]);
    }
    ////////////////////////////////////////////////////////////////////////
    void set_dynamic_range(int channel, double value)
    {
        m_map_parameters["dynamic_range"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////    
    int get_num_map_pixels(int channel)
    {
        return(int) (yat::StringUtil::to_num<double>(m_map_parameters["num_map_pixels"]));
    }
    ////////////////////////////////////////////////////////////////////////
    void set_num_map_pixels(int channel, double value)
    {
        m_map_parameters["num_map_pixels"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    int get_num_map_pixels_per_buffer(int channel)
    {
        return(int) (yat::StringUtil::to_num<double>(m_map_parameters["num_map_pixels_per_buffer"]));
    }

    ////////////////////////////////////////////////////////////////////////
    void set_num_map_pixels_per_buffer(int channel, double value)
    {
        m_map_parameters["num_map_pixels_per_buffer"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    double get_number_of_scas(int channel)
    {
        return yat::StringUtil::to_num<double>(m_map_parameters["number_of_scas"]);
    }

    ////////////////////////////////////////////////////////////////////////
    void set_number_of_scas(int channel, double value)
    {
        m_map_parameters["number_of_scas"] = yat::StringUtil::to_string<double>(value);
    }

    ////////////////////////////////////////////////////////////////////////
    State get_state()
    {
        return m_state;
    }

    ////////////////////////////////////////////////////////////////////////
    std::string get_status()
    {
        return m_status;
    };

    ////////////////////////////////////////////////////////////////////////
    Tango::DevLong get_mca_length(int channel)
    {
        return yat::StringUtil::to_num<long>(m_map_parameters["mca_length"]);
    }

    ////////////////////////////////////////////////////////////////////////
    void get_mca_data(int channel, int number_of_bins, std::vector<Tango::DevULong>& result)
    {
        result.resize(number_of_bins);
        if(channel % 2)
        {
            //simulate f(x)=ABS(sin(x)/x) for channel odd
            for(size_t i = 0; i < result.size(); i++)
                result.at(i) = (Tango::DevULong)(fabs(sin((double) i) / (i + 1))*100000);
        }
        else
        {
            //simulate f(x)=ABS(sin(x)) for channel even
            for(size_t i = 0; i < result.size(); i++)
                result.at(i) = (Tango::DevULong)(fabs(sin((double) i))*100);
        }

        /*if a preset_value (s) is expired, then change state to standby*/
        if(m_timeout.expired())
            m_is_running = false;

        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void get_run_data(int channel, const char* name, void* value)
    {

        if(std::string(name) == "realtime")
            *(static_cast<double*> (value)) = yat::StringUtil::to_num<double>(m_map_parameters["realtime"]);
        else if(std::string(name) == "livetime")
            *(static_cast<double*> (value)) = yat::StringUtil::to_num<double>(m_map_parameters["livetime"]);
        else if(std::string(name) == "trigger_livetime")
            *(static_cast<double*> (value)) = yat::StringUtil::to_num<double>(m_map_parameters["trigger_livetime"]);
        else if(std::string(name) == "input_count_rate")
            *(static_cast<double*> (value)) = yat::StringUtil::to_num<double>(m_map_parameters["input_count_rate"]);
        else if(std::string(name) == "output_count_rate")
            *(static_cast<double*> (value)) = yat::StringUtil::to_num<double>(m_map_parameters["output_count_rate"]);
        else if(std::string(name) == "events_in_run")
            *(static_cast<unsigned long*> (value)) = yat::StringUtil::to_num<unsigned long>(m_map_parameters["events_in_run"]);
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    int get_nb_rois(int channel)
    {
        std::vector<double> bounds = m_map_rois[channel];
        int roi_count = bounds.size() / 2;
        return roi_count;
    }

    ////////////////////////////////////////////////////////////////////////
    void set_nb_rois(int channel, int nb_rois)
    {
        if(nb_rois == 0)
            m_map_rois[channel].clear();
        else
            m_map_rois[channel].resize(nb_rois * 2);
    }

    ////////////////////////////////////////////////////////////////////////
    void get_roi_bounds(int channel, int roi_num, double& low, double& high)
    {
        std::vector<double> bounds = m_map_rois[channel];
        low = bounds.at(2 * roi_num);
        high = bounds.at(2 * roi_num + 1);
    }

    ////////////////////////////////////////////////////////////////////////    
    void set_roi_bounds(int channel, int roi_num, double low, double high)
    {
        INFO_STREAM << "set_roi_bounds (" << channel << "," << roi_num << "," << low << "," << high << ")" << endl;
        std::vector<double> bounds = m_map_rois[channel];
        //if a new roi_num, then resize vector in order to insert a new low/high bounds
        if((size_t) roi_num >= bounds.size() / 2)
        {
            bounds.resize(roi_num * 2);
        }

        bounds[2 * roi_num] = low;
        bounds[2 * roi_num + 1] = high;
        m_map_rois[channel] = bounds;
        for(std::map<int, std::vector<double>>::const_iterator it = m_map_rois.begin(); it != m_map_rois.end(); ++it)
        {
            std::stringstream ss;
            ss << "channel = " << it->first;
            for(size_t i = 0; i < it->second.size() / 2; i++)
            {
                ss << ", roi" << i << "_low  = " << it->second.at(2 * i);
                ss << ", roi" << i << "_high = " << it->second.at(2 * i + 1);
            }
            DEBUG_STREAM << ss.str() << endl;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    double get_roi_data(int channel, int roi_num)
    {
        /*roi_data is  equal to high-low+1*/
        std::vector<double> bounds = m_map_rois[channel];
        return(bounds[2 * roi_num + 1] - bounds[2 * roi_num]);
    }

    ////////////////////////////////////////////////////////////////////////
    bool is_running()
    {
        //MCA mode : managed in get_mca_data() :
        //a timer object is initialized with the preset_value() fixed by user
        //if the timer expired, m_is_running = false
        //--------------------------------------------------------------------
        //MAPPING mode : managed in buffer_done() :
        //if (m_numero_pixel == nbPixels) i.e all pixels are acquired, then m_is_running = false
        return m_is_running;
    }

    ////////////////////////////////////////////////////////////////////////
    std::string get_pixel_advance_mode()
    {
        return m_map_parameters["pixel_advance_mode"];
    }

    ////////////////////////////////////////////////////////////////////////
    void set_pixel_advance_mode(const std::string& mode, double ticks_per_pixel = 0.0)
    {
        m_map_parameters["pixel_advance_mode"] = mode;
    }

    ////////////////////////////////////////////////////////////////////////
    void mapping_pixel_next()
    {
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    bool is_buffer_overrun(int module)
    {
        /*always in order to never throw exception overrun inthe simulator*/
        return false;
    }

    ////////////////////////////////////////////////////////////////////////
    bool is_buffer_full(int module, Buffer buffer_num)
    {
        /*always in order to free run the simulator*/
        return true;
    }

    ////////////////////////////////////////////////////////////////////////
    unsigned long get_buffer_length(int module)
    {
        return yat::StringUtil::to_num<int>(m_map_parameters["buffer_len"])*4;
    }

    ////////////////////////////////////////////////////////////////////////
    unsigned long get_current_pixel()
    {
        return m_numero_pixel[1];
    }

    ////////////////////////////////////////////////////////////////////////
    void get_buffer(int module, Buffer buffer_num, unsigned long* buffer)
    {
        for(int i = 0; i < NUMERIC_BUFFER_LENGTH; i++)
        {
            switch(i)
            {
                    //HEADER
                case 0:buffer[i] = TAG_HEAD0;
                    break;
                case 1:buffer[i] = TAG_HEAD1;
                    break;
                case 2:buffer[i] = BUFFER_HEADER_SIZE; //buffer header size
                    break;
                case 3:buffer[i] = 1; //mapping mode
                    break;
                case 4:buffer[i] = 1; //run number
                    break;
                case 5:buffer[i] = 0; //seq buffer number
                    break;
                case 6:buffer[i] = 0; //seq buffer number
                    break;
                case 7:buffer[i] = 0; //buffer A
                    break;
                case 8:buffer[i] = 1; //nb of pixels in buffer
                    break;
                case 9:buffer[i] = m_numero_pixel[module]; //starting pixel in buffer LSB
                    break;
                case 10:buffer[i] = 0; //starting pixel in buffer MSB
                    break;
                case 31:buffer[i] = 0; //reserved must be set to 0
                    break;
                    //DATA
                case BUFFER_HEADER_SIZE + 0: buffer[i] = TAG_DATA0; //TAG WORD : 0x33CC
                    break;
                case BUFFER_HEADER_SIZE + 1: buffer[i] = TAG_DATA1; //TAG WORD : 0xCC33
                    break;
                case BUFFER_HEADER_SIZE + 4: buffer[i] = m_numero_pixel[module]; //pixel number LSB
                    break;
                case BUFFER_HEADER_SIZE + 5: buffer[i] = 0; //pixel number MSB
                    break;
                case BUFFER_HEADER_SIZE + 8: buffer[i] = NUMERIC_NB_BINS; //channel 0 size
                    break;
                case BUFFER_HEADER_SIZE + 9: buffer[i] = NUMERIC_NB_BINS; //channel 1 size
                    break;
                case BUFFER_HEADER_SIZE + 10: buffer[i] = NUMERIC_NB_BINS; //channel 2 size
                    break;
                case BUFFER_HEADER_SIZE + 11: buffer[i] = NUMERIC_NB_BINS; //channel 3 size
                    break;
                    //channel0 data (simulate sqrt(j))
                case BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 0:
                    for(int j = 0; j < NUMERIC_NB_BINS; j++)
                        buffer[BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 0 * NUMERIC_NB_BINS + j] = (unsigned long) (sqrt((double) j));
                    break;
                    //channel1 data (simulate sqrt(2048)-sqrt(j))
                case BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 1:
                    for(int j = 0; j < NUMERIC_NB_BINS; j++)
                        buffer[BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 1 * NUMERIC_NB_BINS + j] = (unsigned long) (sqrt((double) NUMERIC_NB_BINS) - sqrt((double) j));
                    break;
                    //channel2 data (simulate sqrt(j))
                case BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 2:
                    for(int j = 0; j < NUMERIC_NB_BINS; j++)
                        buffer[BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 2 * NUMERIC_NB_BINS + j] = (unsigned long) (sqrt((double) j));
                    break;
                    //channel3 data (simulate sqrt(2048)-sqrt(j))
                case BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 3:
                    for(int j = 0; j < NUMERIC_NB_BINS; j++)
                        buffer[BUFFER_HEADER_SIZE + PIXEL_HEADER_SIZE + 3 * NUMERIC_NB_BINS + j] = (unsigned long) (sqrt((double) NUMERIC_NB_BINS) - sqrt((double) j));
                    break;
                default:
                    break;
            }
        }
        //in ms in order to simulate  a duration of acquisition
        yat::Thread::sleep(NUMERIC_MAPPING_CLOCK_MS);
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void buffer_done(int module, Buffer buffer_num)
    {
        /*if all pixels are acquired, then change state to standby*/
        if(m_numero_pixel[module] == get_num_map_pixels(0))
            m_is_running = false;
        m_numero_pixel[module]++;
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    void apply_all_board_operation(const char* operation)
    {
        return;
    }
    ////////////////////////////////////////////////////////////////////////    
private:
    bool m_is_running;
    std::map<int, std::vector<double>> m_map_rois;
    yat::Timeout m_timeout;
    int m_numero_pixel[NUMERIC_NB_MODULES];
    std::map<std::string, std::string> m_map_parameters;
};

} // namespace XiaDxp_ns


#endif

