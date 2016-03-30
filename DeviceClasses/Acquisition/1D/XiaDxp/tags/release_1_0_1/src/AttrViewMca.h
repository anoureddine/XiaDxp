/*************************************************************************
/*! 
 *  \file   AttrViewMca.h
 *  \brief  class AttrViewMca
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef ATTR_VIEW_MCA_H
#define ATTR_VIEW_MCA_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include <yat4tango/PropertyHelper.h>
#include "AttrView.h"
#include "DataStore.h"

namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	AttrViewMca
 *	description: 
 /------------------------------------------------------------------*/
class AttrViewMca : public AttrView
{
public:
    AttrViewMca(Tango::DeviceImpl *dev);
    ~AttrViewMca();
    void init(yat::SharedPtr<DataStore> data_store);
    void update_data(int ichannel, yat::SharedPtr<DataStore> data_store);

protected:
	/// callback methods for  tango dyn attributes - mapPixelsNumber
    void write_preset_value_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd);	
	
	/// callback methods for  tango dyn attributes - currentPixel
    void read_preset_value_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);		
	
	/// callback methods for  tango dyn attributes - mapPixelsNumber
    void write_preset_type_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd);	
	
	/// callback methods for  tango dyn attributes - currentPixel
    void read_preset_type_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);		
    
    /// callback methods for  tango dyn attributes - currentPixel
    void read_roi_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);		    
    
	///user data attached to the dynamic attributes
	DoubleUserData* m_dyn_preset_value;	    
	StringUserData* m_dyn_preset_type;	     
	bool is_write_at_init;
};

} // namespace 

#endif


