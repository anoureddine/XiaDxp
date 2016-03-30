/*************************************************************************
/*! 
 *  \file   AttrView.h
 *  \brief  class AttrView
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef ATTR_VIEW_H
#define ATTR_VIEW_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include <yat/utils/Callback.h>
#include <yat/any/Any.h>
#include <yat4tango/DynamicInterfaceManager.h>
#include "UserData.h"
#include "DataStore.h"

//#define WRITE_CALLBACK_NULL AttrView::write_callback_null
//#define READ_CALLBACK_NULL AttrView::read_callback_null

namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	AttrView
 *	description: base class for the data storage
 /------------------------------------------------------------------*/
class AttrView : public Tango::LogAdapter
{

//	typedef void (AttrView::*callback_type)(yat4tango::DynamicAttributeReadCallbackData& cbd);
	
public:
    AttrView(Tango::DeviceImpl *dev);
    ~AttrView();
    virtual void init(yat::SharedPtr<DataStore> data_store) = 0;
    virtual void update_data(int ichannel, yat::SharedPtr<DataStore> data_store) = 0;
	
protected:
    void init_common_attributes(yat::SharedPtr<DataStore> data_store);				
	void update_common_attributes(int channel, yat::SharedPtr<DataStore> data_store);
	///generic method to create a tango dynamic attribute
    template <class F1, class F2>
    void create_attribute(	std::string name,
							int data_type,
							Tango::AttrDataFormat data_format,
							Tango::AttrWriteType access_type,
							const std::string& unit,
							const std::string& format,
							const std::string& desc,
							F1 read_callback,
							F2 write_callback,
							yat::Any user); //put any user data attached to this attribute
							
    /// callback methods for tango dyn attributes - NULL
    void read_callback_null(yat4tango::DynamicAttributeReadCallbackData& cbd){/*nop*/}
	
    /// callback methods for tango dyn attributes - NULL
    void write_callback_null(yat4tango::DynamicAttributeWriteCallbackData& cbd){/*nop*/}		
	
    /// callback methods for common tango dyn attributes - statistics attributes
    void read_stat_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);
	
    /// callback methods for common tango dyn attributes - channels attributes
    void read_channel_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);
	
    /// Owner Device server object
    Tango::DeviceImpl* m_device;

    //dynamic attributes objects        
    yat4tango::DynamicInterfaceManager m_dim;
    std::vector<DoubleUserData*> 	m_dyn_trigger_livetime;
    std::vector<DoubleUserData*> 	m_dyn_realtime;
    std::vector<DoubleUserData*> 	m_dyn_livetime;
    std::vector<DoubleUserData*> 	m_dyn_deadtime;
    std::vector<DoubleUserData*> 	m_dyn_icr;
    std::vector<DoubleUserData*> 	m_dyn_ocr;
    std::vector<ULongUserData*> 	m_dyn_events_in_run;
    std::vector<ChannelUserData*> 	m_dyn_channel;
};

} // namespace 

///////////////////////////////////////////////////////////////////////////////
//// INCLUDE TEMPLATE IMPLEMENTAION
///////////////////////////////////////////////////////////////////////////////    
#include "AttrView.hpp"

#endif // ATTR_VIEW_H


