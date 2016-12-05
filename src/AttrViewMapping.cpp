//=============================================================================
//
// file :        AttrViewMapping.cpp
//
// description : 
//
// project :	XiaDxp Project
//
// $Author: noureddine $
//
// copyleft :    Synchrotron SOLEIL
//               L'Orme des merisiers - Saint Aubin
//               BP48 - 91192 Gif sur Yvette
//               FRANCE
//=============================================================================

#include "AttrViewMapping.h"
#include <XiaDxp.h>

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrViewMapping::AttrViewMapping(Tango::DeviceImpl *dev)
:AttrView(dev)
{
    INFO_STREAM << "AttrViewMapping::AttrViewMapping() - [BEGIN]" << endl;
    INFO_STREAM << "AttrViewMapping::AttrViewMapping() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrViewMapping::~AttrViewMapping()
{
    INFO_STREAM << "AttrViewMapping::~AttrViewMapping() - [BEGIN]" << endl;
    INFO_STREAM << "AttrViewMapping::~AttrViewMapping() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrViewMapping::init(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "AttrViewMapping::init() - [BEGIN]" << endl;

    //- remove all previous dyn attributes
    m_dim.dynamic_attributes_manager().remove_attributes();
    //- create common attributes (stats + channels)
    AttrView::init_common_attributes(data_store);

    ////////////////////////////////////////////////////////////////////////////////////////	
    {
        std::string name = "nbPixels";
        INFO_STREAM << "\t- Create Mapping dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 
        m_dyn_nb_pixels = new LongUserData(name);
        dai.set_user_data(m_dyn_nb_pixels);
        //- describe the dynamic attr we want...
        dai.tai.data_type = Tango::DEV_LONG;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ_WRITE;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = " ";
        dai.tai.format = "%5d";
        dai.tai.description = "Number of pixels (step) for the Mapping acquisition.";
        // - cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

		//- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMapping::read_nb_pixels_callback);
		
        //- instanciate the write callback (called when the dyn. attr. is writen)    
        dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &AttrViewMapping::write_nb_pixels_callback);

        //- add the dyn. attr. to the device
        m_dim.dynamic_attributes_manager().add_attribute(dai);
    }

    ////////////////////////////////////////////////////////////////////////////////////////	
    {
        std::string name = "currentPixel";
        INFO_STREAM << "\t- Create Mapping dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 	
        m_dyn_current_pixel = new LongUserData(name);
        dai.set_user_data(m_dyn_current_pixel);

        //- describe the dynamic attr we want...
        dai.tai.data_type = Tango::DEV_LONG;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = " ";
        dai.tai.format = "%5d";
        dai.tai.description = "current pixel of the mapping.";
        //- cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

        //- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMapping::read_current_pixel_callback);

        //- add the dyn. attr. to the device
        m_dim.dynamic_attributes_manager().add_attribute(dai);
    }

    ////////////////////////////////////////////////////////////////////////////////////////	
    {
        std::string name = "nbPixelsPerBuffer";
        INFO_STREAM << "\t- Create Mapping dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 
        m_dyn_nb_pixels_per_buffer = new LongUserData(name);
        dai.set_user_data(m_dyn_nb_pixels_per_buffer);
        //- describe the dynamic attr we want...
        dai.tai.data_type = Tango::DEV_LONG;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ_WRITE;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = " ";
        dai.tai.format = "%5d";
        dai.tai.description = "Number of pixels per buffer for the Mapping acquisition.";
        // - cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

		//- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMapping::read_nb_pixels_per_buffer_callback);
		
        //- instanciate the write callback (called when the dyn. attr. is writen)    
        dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &AttrViewMapping::write_nb_pixels_per_buffer_callback);

        //- add the dyn. attr. to the device
        m_dim.dynamic_attributes_manager().add_attribute(dai);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////	
    try
    {
        std::string name = "pixelAdvanceMode";
        INFO_STREAM << "\t- Create Mapping dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 
        m_dyn_pixel_advance_mode = new StringUserData(name);
        dai.set_user_data(m_dyn_pixel_advance_mode);
        //- describe the dyn. attr.
        dai.tai.data_type = Tango::DEV_STRING;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ;////@TODO : Tango::READ_WRITE;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = " ";
        dai.tai.format = "%s";
        dai.tai.description =   "Available values :\n"
        "GATE\n"
        "SYNC\n";
        // - cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

        //- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMapping::read_pixel_advance_mode_callback);

        //- instanciate the write callback (called when the dyn. attr. is writen)
        ////@@TODOD dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &AttrViewMapping::write_pixel_advance_mode_callback);

        //- add the dyn. attr. to the device
        m_dim.dynamic_attributes_manager().add_attribute(dai);
    }
    catch (Tango::DevFailed& df)
    {
        std::string err("failed to instanciate dynamic attributes  - Tango exception caught - see log attribute for details");
        ERROR_STREAM << err << std::endl;
        ERROR_STREAM << df << std::endl;
        return;
    }
	
    INFO_STREAM << "AttrViewMapping::init() - [END]" << endl;
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::read_nb_pixels_callback()
//
// description : Read nbPixels attribute
//
//-----------------------------------------------------------------------------
void AttrViewMapping::read_nb_pixels_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::read_nb_pixels_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = static_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = static_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute nbPixels";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::read_nb_pixels_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "DynamicInterface::read_callback");
        }

        LongUserData* user_data = cbd.dya->get_user_data<LongUserData>();	
		
        //- set the attribute value 
        long current = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_num_map_pixels();
        user_data->set_value(current);
        
        //- set the attribute value
        cbd.tga->set_value((Tango::DevLong*)&user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::read_nb_pixels_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::write_nb_pixels_callback()
//
// description : Write nbPixels attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMapping::write_nb_pixels_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::write_nb_pixels_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = static_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        if (state == Tango::FAULT	||
            state == Tango::INIT	||
            state == Tango::RUNNING	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to write attribute nbPixels";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::write_nb_pixels_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid		
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "DynamicInterface::write_callback");
        }

        Tango::DevLong val;
        cbd.tga->get_write_value( val);

		LongUserData* user_data = cbd.dya->get_user_data<LongUserData>();        
        user_data->set_value(val);

        static_cast<XiaDxp*>(m_device)->get_ctrl()->set_num_map_pixels(val);      
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::write_nb_pixels_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::read_nb_pixels_per_buffer_callback()
//
// description : Read nbPixelsPerBuffer attribute
//
//-----------------------------------------------------------------------------
void AttrViewMapping::read_nb_pixels_per_buffer_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::read_nb_pixels_per_buffer_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = static_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = static_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT  && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF     ||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute nbPixels";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::read_nb_pixels_per_buffer_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "DynamicInterface::read_callback");
        }

        LongUserData* user_data = cbd.dya->get_user_data<LongUserData>();	
		
        //- set the attribute value 
        long current = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_num_map_pixels_per_buffer();
        user_data->set_value(current);
        
        //- set the attribute value
        cbd.tga->set_value((Tango::DevLong*)&user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::read_nb_pixels_per_buffer_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::write_nb_pixels_per_buffer_callback()
//
// description : Write nbPixelsPerBuffer attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMapping::write_nb_pixels_per_buffer_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::write_nb_pixels_per_buffer_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = static_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        if (state == Tango::FAULT	||
            state == Tango::INIT	||
            state == Tango::RUNNING	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to write attribute nbPixelsPerBuffer";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::write_nb_pixels_per_buffer_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid		
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "DynamicInterface::write_callback");
        }

        Tango::DevLong val;
        cbd.tga->get_write_value( val);

		LongUserData* user_data = cbd.dya->get_user_data<LongUserData>();        
        user_data->set_value(val);

        static_cast<XiaDxp*>(m_device)->get_ctrl()->set_num_map_pixels_per_buffer(val);        
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::write_nb_pixels_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::write_gap_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMapping::read_current_pixel_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::read_current_pixel_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = static_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = static_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute currentPixel";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::read_current_pixel_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "DynamicInterface::read_callback");
        }

        LongUserData* user_data = cbd.dya->get_user_data<LongUserData>();
        //- set the attribute value
        cbd.tga->set_value((Tango::DevLong*)&user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::read_current_pixel_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMca::read_preset_type_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMapping::read_pixel_advance_mode_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMca::read_pixel_advance_mode_callback()" << endl; //  << cbd.dya->get_name() << endl;
    //- be sure the pointer to the dyn. attr. is valid
    if(!cbd.dya)
    {
        THROW_DEVFAILED("INTERNAL_ERROR",
                        "unexpected NULL pointer to dynamic attribute",
                        "AttrViewMapping::read_pixel_advance_mode_callback");
    }

    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = dynamic_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute pixelAdvanceMode";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::read_pixel_advance_mode_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMapping::read_pixel_advance_mode_callback()");
        }

        StringUserData* user_data = cbd.dya->get_user_data<StringUserData>();
        //- set the attribute value 
        string current = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_pixel_advance_mode();
        user_data->set_value(current);
        cbd.tga->set_value((Tango::DevString*)user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::read_pixel_advance_mode_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMapping::write_pixel_advance_mode_callback()
//
// description : Write pixelAdvanceMode attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMapping::write_pixel_advance_mode_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMapping::write_pixel_advance_mode_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        if (state == Tango::FAULT	||
            state == Tango::INIT	||
            state == Tango::RUNNING	||
            state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to write attribute pixelAdvanceMode";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMapping::write_pixel_advance_mode_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMapping::write_pixel_advance_mode_callback");
        }

        Tango::DevString val;
        cbd.tga->get_write_value( val);

        string current = val;
        transform(current.begin(), current.end(), current.begin(), ::toupper);
        dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->set_pixel_advance_mode(current);

        StringUserData* user_data = cbd.dya->get_user_data<StringUserData>();
        user_data->set_value(val);
        //memorize in the device properties
        yat4tango::PropertyHelper::set_property(dynamic_cast<XiaDxp*>(m_device), "__MemorizedPixelAdvanceMode", val);
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMapping::write_pixel_advance_mode_callback()");
    }
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrViewMapping::update_data(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    DEBUG_STREAM << "AttrViewMapping::update_data() - [BEGIN]" << endl;

    //- update common attributes (stats + channels)	
    AttrView::update_common_attributes(ichannel, data_store);
	
    //- update currentPixel attribute
    m_dyn_current_pixel->set_value(data_store->get_current_pixel(ichannel)+1);

    DEBUG_STREAM << "AttrViewMapping::update_data() - [END]" << endl;
}

}// namespace
