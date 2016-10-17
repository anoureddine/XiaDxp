/*************************************************************************
/*! 
 *  \file   AttrViewMapping.h
 *  \brief  class AttrViewMapping
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef ATTR_VIEW_MAPPING_H
#define ATTR_VIEW_MAPPING_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include "AttrView.h"
#include "DataStore.h"

namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	AttrViewMapping
 *	description: 
 /------------------------------------------------------------------*/
class AttrViewMapping : public AttrView
{
public:
    AttrViewMapping(Tango::DeviceImpl *dev);
    ~AttrViewMapping();
    void init(yat::SharedPtr<DataStore> data_store);
    void update_data(int ichannel, yat::SharedPtr<DataStore> data_store);

protected:	

	/// callback methods for  tango dyn attributes - nbPixels
    void read_nb_pixels_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);	
	
	/// callback methods for  tango dyn attributes - nbPixels
    void write_nb_pixels_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd);	

	/// callback methods for  tango dyn attributes - nbPixelsPerBuffer
    void read_nb_pixels_per_buffer_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);	
	
	/// callback methods for  tango dyn attributes - nbPixelsPerBuffer
    void write_nb_pixels_per_buffer_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd);	
	
	/// callback methods for  tango dyn attributes - currentPixel
    void read_current_pixel_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);		
	
	/// callback methods for  tango dyn attributes - pixelAdvanceMode
	void read_pixel_advance_mode_callback(yat4tango::DynamicAttributeReadCallbackData& cbd);	
	
	///user data attached to the dynamic attributes
	LongUserData* m_dyn_nb_pixels;	
    LongUserData* m_dyn_nb_pixels_per_buffer;	
    LongUserData* m_dyn_current_pixel;			
	StringUserData* m_dyn_pixel_advance_mode;			
};

} // namespace 

#endif


