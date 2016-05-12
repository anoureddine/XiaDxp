/*************************************************************************
/*! 
 *  \file   AcquisitionMca.h
 *  \brief  class AcquisitionMca 
 *  \author 
 */
/*************************************************************************/

#ifndef ACQUISITION_MCA_H
#define ACQUISITION_MCA_H

//TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

#include "Acquisition.h"

#define MCA_TASK_PERIODIC_MS          100

const size_t MCA_STOP_MSG = yat::FIRST_USER_MSG + 300;

namespace XiaDxp_ns
{
class AcquisitionMca : public Acquisition
{
public:

    /// ctor
    AcquisitionMca(Tango::DeviceImpl *dev, const std::string & board_type, yat::SharedPtr<DataStore> store);

    /// dtor
    virtual ~AcquisitionMca(void);

    /// load config file
    void load_config_file(const std::string& file_name);
    
    /// save config file
    void save_config_file(const std::string& file_name);
    
    /// start acquisition
    void start_acquisition(short accumulate = false);

    /// stop acquisition
    void stop_acquisition();
protected:
    /// [yat4tango::DeviceTask implementation]
    void process_message(yat::Message& msg) throw(Tango::DevFailed);

private:
    void collect_data(void);
};

//-------------------------------------------------------------
/// Functor object called when reset the object AcquisitionMca
struct AcquisitionMcaTaskExiter
{
    /// unique operator() of this functor
    void operator()(yat4tango::DeviceTask* t) const
    {
        try
        {
            //- Automatically post the TASK_EXIT msg! U may never delete manually a DeviceTask !!!
            cout << "AcquisitionMca::TaskExiter ..." << endl;
            t->exit();
        }
        catch(...)
        {
        }
    }
};

} // namespace XiaDxp_ns

#endif

