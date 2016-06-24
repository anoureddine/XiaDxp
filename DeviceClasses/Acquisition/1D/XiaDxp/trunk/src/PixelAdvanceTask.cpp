/*************************************************************************
/*! 
 *  \file   PixelAdvanceTask.h
 *  \brief  class PixelAdvanceTask 
 *  \author 
 */
/*************************************************************************/


#include <tango.h>

#include <map>

//- YAT stuff
#include <yat/utils/XString.h>

#include "handel_constants.h"

#include "PixelAdvanceTask.h"

namespace XiaDxp_ns
{

//----------------------------------------------------------------------------------------------------------------------
//- PixelAdvanceTask Ctor
//----------------------------------------------------------------------------------------------------------------------
PixelAdvanceTask::PixelAdvanceTask(Tango::DeviceImpl *dev, yat::SharedPtr<DriverHelper> handel)
: yat4tango::DeviceTask(dev),
m_device(dev),
m_helper(handel)
{
    INFO_STREAM << "PixelAdvanceTask::PixelAdvanceTask() - [BEGIN]" << endl;
    set_periodic_msg_period(PIXELADVANCE_TASK_PERIODIC_MS);//50Hz
    enable_periodic_msg(false);
    INFO_STREAM << "PixelAdvanceTask::PixelAdvanceTask() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- PixelAdvanceTask Dtor
//----------------------------------------------------------------------------------------------------------------------
PixelAdvanceTask::~PixelAdvanceTask()
{
    INFO_STREAM << "PixelAdvanceTask::~PixelAdvanceTask() - [BEGIN]" << endl;
    INFO_STREAM << "PixelAdvanceTask::~PixelAdvanceTask() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void PixelAdvanceTask::start()
{
    INFO_STREAM << "PixelAdvanceTask::start() - [BEGIN]" << endl;	
	post(PIXELADVANCE_START_MSG);   
    INFO_STREAM << "PixelAdvanceTask::start() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void PixelAdvanceTask::stop()
{
    INFO_STREAM << "PixelAdvanceTask::stop() - [BEGIN]" << endl;
    post(PIXELADVANCE_STOP_MSG);   
    INFO_STREAM << "PixelAdvanceTask::stop() - [END]" << endl;
}

//---------------------------
//- PixelAdvanceTask::process_message()
//---------------------------
void PixelAdvanceTask::process_message(yat::Message& msg) throw (Tango::DevFailed)
{    
    try
    {
        switch (msg.type())
        {
                //-----------------------------------------------------
            case yat::TASK_INIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> PixelAdvanceTask::TASK_INIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_EXIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> PixelAdvanceTask::TASK_EXIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_TIMEOUT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> PixelAdvanceTask::TASK_TIMEOUT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------------------------------------------------
            case PIXELADVANCE_START_MSG:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> PixelAdvanceTask::PIXELADVANCE_START_MSG" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;		
                enable_periodic_msg(true);
            }
                break;
                //-----------------------------------------------------                
            case PIXELADVANCE_STOP_MSG:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM<<"-> PixelAdvanceTask::PIXELADVANCE_STOP_MSG" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;				
                enable_periodic_msg(false);
            }
                break;
                //-----------------------------------------------------                
            case yat::TASK_PERIODIC:
            {
                DEBUG_STREAM<<" "<<std::endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                DEBUG_STREAM<<"-> PixelAdvanceTask::TASK_PERIODIC" << endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;			
                m_helper->mapping_pixel_next();
            }
                break;
                //-----------------------------------------------------
        }
    }
    catch (yat::Exception& ex)
    {
        //- TODO Error Handling
		ex.dump();
		std::stringstream error_msg("");
		error_msg << "Origin\t: " << ex.errors[0].origin << endl;
        error_msg << "Desc\t: " << ex.errors[0].desc << endl;
        error_msg << "Reason\t: " << ex.errors[0].reason << endl;
        ERROR_STREAM << "Exception from - PixelAdvanceTask::process_message() : " << error_msg.str()<<endl;
        throw;
    }
}

} // namespace 

