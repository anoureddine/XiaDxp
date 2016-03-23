/*************************************************************************
/*! 
 *  \file   PixelAdvanceTask.h
 *  \brief  class PixelAdvanceTask 
 *  \author 
 */
/*************************************************************************/

#ifndef PIXEL_ADVANCE_TASK
#define PIXEL_ADVANCE_TASK

//TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat4tango/DeviceTask.h>

#include "DriverHelper.h"
#include "HandelHelper.h"
#include "SimulatorHelper.h"

const size_t PIXELADVANCE_START_MSG = yat::FIRST_USER_MSG + 200;
const size_t PIXELADVANCE_STOP_MSG  = yat::FIRST_USER_MSG + 201;
const size_t PIXELADVANCE_TASK_PERIODIC_MS   = 20;//ms

namespace XiaDxp_ns
{
class PixelAdvanceTask : public yat4tango::DeviceTask
{
public:

    /// ctor
    PixelAdvanceTask(Tango::DeviceImpl *dev, yat::SharedPtr<DriverHelper> handel);
    
    /// dtor
    virtual ~PixelAdvanceTask(void);

    /// start advancing manual pixel
    void start();
    
    /// stop advancing manual pixel
    void stop();
    
protected:
    /// [yat4tango::DeviceTask implementation]
    void process_message(yat::Message& msg) throw(Tango::DevFailed);

    ///owner device server
    Tango::DeviceImpl* m_device;  
    
    ///handle library wrapper 
    yat::SharedPtr<DriverHelper> m_helper;
};

//-------------------------------------------------------------

} // namespace XiaDxp_ns

#endif

