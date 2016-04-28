/*************************************************************************
/*! 
 *  \file   AcquisitionMapping.h
 *  \brief  class AcquisitionMapping 
 *  \author 
 */
/*************************************************************************/

#ifndef ACQUISITION_MAPPING_H
#define ACQUISITION_MAPPING_H

//TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

#include "PixelAdvanceTask.h"
#include "DriverHelper.h"
#include "HandelHelper.h"
#include "SimulatorHelper.h"
#include "Acquisition.h"

#define MAPPING_TASK_PERIODIC_MS          5

const size_t MAPPING_STOP_MSG = yat::FIRST_USER_MSG + 400;

namespace XiaDxp_ns
{

class ModuleInfo
{
public : 
    ///ctor
    explicit ModuleInfo()
    { 
        m_is_pixel_readout = false;
        m_current_buffer = BUFFER_A;
        m_buffer_length = 0;
    }    
    ///dtor
    ~ModuleInfo()
    {        
    }
    ///data members
    bool m_is_pixel_readout;
    Buffer m_current_buffer;
    size_t m_buffer_length;
};


class AcquisitionMapping : public Acquisition
{
public:

    /// ctor
    AcquisitionMapping(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store);

    /// dtor
    virtual ~AcquisitionMapping(void);

    /// load config file
    void load_config_file(const std::string& file_name);

    /// save config file
    void save_config_file(const std::string& file_name);

    /// start acquisition
    void start_acquisition(short accumulate = false);

    /// stop acquisition
    void stop_acquisition();

    /// collect data from buffer (process for each module is : check_buffer -> readout_buffer -> switch_buffer)
    void collect_data();

protected:
    /// [yat4tango::DeviceTask implementation]
    virtual void process_message(yat::Message& msg) throw(Tango::DevFailed);
private:
    ///
    void check_buffer(int module);
    ///
    void readout_buffer(int module);
    ///
    void switch_buffer(int module);
    ///
    bool is_all_pixel_read_out(void);

    /// Acquisition object
    yat::SharedPtr<PixelAdvanceTask> m_pixel_advance_task;
    std::vector<ModuleInfo> m_module_info_array;
    int m_num_map_pixels; //nb mapping pixels to acquire
    int m_current_pixel; //current acquired mapping pixel available for all modules (see m_helper->get_current_pixel())
};

//-------------------------------------------------------------
/// Functor object called when reset the object AcquisitionMapping
struct AcquisitionMappingTaskExiter
{
    /// unique operator() of this functor
    void operator()(yat4tango::DeviceTask* t) const
    {
        try
        {
            //- Automatically post the TASK_EXIT msg! U may never delete manually a DeviceTask !!!
            cout << "AcquisitionMapping::TaskExiter ..." << endl;
            t->exit();
        }
        catch(...)
        {
        }
    }
};

} // namespace XiaDxp_ns

#endif

