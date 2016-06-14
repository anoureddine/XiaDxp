/*************************************************************************
/*! 
 *  \file   AttrView.hpp
 *  \brief  class AttrView
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/
#ifndef ATTR_VIEW_HPP
#define ATTR_VIEW_HPP

//- TANGO
#include <tango.h>

///////////////////////////////////////////////////////////////////////////////
//// TEMPLATE IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////    
namespace XiaDxp_ns
{

template <class F1, class F2>
void AttrView::create_attribute(std::string name,
								 int data_type,
								 Tango::AttrDataFormat data_format ,
								 Tango::AttrWriteType access_type,
								 const std::string& unit,
								 const std::string& format,
								 const std::string& desc,
								 F1 read_callback,
								 F2 write_callback,
								 yat::Any user)
{
    DEBUG_STREAM << "AttrView::create_attribute() - [BEGIN]" << endl;
    INFO_STREAM << "\t- Create dynamic attribute [" << name << "]" << endl;
    ////////////////////////////////////////////////////////////////////////////////////////    
    yat4tango::DynamicAttributeInfo dai;
    dai.dev = m_device;
    //- specify the dyn. attr.  name
    dai.tai.name = name;
    //- associate the dyn. attr. with its data
    dai.set_user_data(user);
    //- describe the dynamic attr we want...
    dai.tai.data_type = data_type;
    dai.tai.data_format = data_format;
    dai.tai.writable = access_type;
    dai.tai.disp_level = Tango::OPERATOR;
    dai.tai.unit = unit;
    dai.tai.format = format;
    dai.tai.description = desc;
    //- cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
    //	dai.cdb = true;

    
    //- instanciate the read callback (called when the dyn. attr. is read)    
	if(access_type == Tango::READ ||access_type == Tango::READ_WRITE)
	{
		dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, read_callback);
	}

	//- instanciate the write callback (called when the dyn. attr. is read)    
	if(access_type == Tango::WRITE ||access_type == Tango::READ_WRITE)
	{
	    dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, write_callback);
	}
	
    //- add the dyn. attr. to the device
    DEBUG_STREAM << "\t- add the dyn. attr. to the device [" << m_device << "]" << endl;
    m_dim.dynamic_attributes_manager().add_attribute(dai);
    DEBUG_STREAM << "AttrView::create_stat_attribute() - [END]" << endl;
}

}

#endif // ATTR_VIEW_HPP