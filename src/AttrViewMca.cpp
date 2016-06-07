//=============================================================================
//
// file :        AttrViewMca.cpp
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

#include "AttrViewMca.h"
#include <XiaDxp.h>

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrViewMca::AttrViewMca(Tango::DeviceImpl *dev)
: AttrView(dev)
{
    INFO_STREAM << "AttrViewMca::AttrViewMca() - [BEGIN]" << endl;
    INFO_STREAM << "AttrViewMca::AttrViewMca() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrViewMca::~AttrViewMca()
{
    INFO_STREAM << "AttrViewMca::~AttrViewMca() - [BEGIN]" << endl;
    INFO_STREAM << "AttrViewMca::~AttrViewMca() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrViewMca::init(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "AttrViewMca::init() - [BEGIN]" << endl;
    //- remove all previous dyn attributes
    m_dim.dynamic_attributes_manager().remove_attributes();
    //- create common attributes (stats + channels)
    AttrView::init_common_attributes(data_store);
    
    int nb_total_channels = data_store->get_nb_modules()*data_store->get_nb_channels();

    ////////////////////////////////////////////////////////////////////////////////////////	
    try
    {
        std::string name = "presetValue";
        INFO_STREAM << "\t- Create Mca dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 
        m_dyn_preset_value = new DoubleUserData(name);
        dai.set_user_data(m_dyn_preset_value);
        //- describe the dyn. attr.
        dai.tai.data_type = Tango::DEV_DOUBLE;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ_WRITE;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = "sec or counts";
        dai.tai.format = "%6.2f";
        dai.tai.description =  "This value is either the number of counts or the time (specified in seconds)\n"
        "it depends on preset_type parameter.";
        // - cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

        //- instanciate the write callback (called when the dyn. attr. is writen)    
        dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &AttrViewMca::write_preset_value_callback);

        //- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMca::read_preset_value_callback);

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

    ////////////////////////////////////////////////////////////////////////////////////////	
    try
    {
        std::string name = "presetType";
        INFO_STREAM << "\t- Create Mca dynamic attribute [" << name << "]" << endl;
        yat4tango::DynamicAttributeInfo dai;
        dai.dev = m_device;

        //- specify the dyn. attr.  name
        dai.tai.name = name;
        //- associate the dyn. attr. with its data 
        m_dyn_preset_type = new StringUserData(name);
        dai.set_user_data(m_dyn_preset_type);
        //- describe the dyn. attr.
        dai.tai.data_type = Tango::DEV_STRING;
        dai.tai.data_format = Tango::SCALAR;
        dai.tai.writable = Tango::READ_WRITE;
        dai.tai.disp_level = Tango::OPERATOR;
        dai.tai.unit = " ";
        dai.tai.format = "%s";
        dai.tai.description =   "Sets the preset run type:<br>\n"
        "NONE<br>\n"
        "FIXED_REAL<br>\n"
        "FIXED_LIVE<br>\n"
        "FIXED_EVENTS<br>\n"
        "FIXED_TRIGGERS<br>";
        // - cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
        //dai.cdb = true;

        //- instanciate the write callback (called when the dyn. attr. is writen)    
        dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &AttrViewMca::write_preset_type_callback);

        //- instanciate the read callback (called when the dyn. attr. is read)    
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMca::read_preset_type_callback);

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

    ////////////////////////////////////////////////////////////////////////////////////////        
    for (int i = 0; i < nb_total_channels; i++)
    {
        int nb_rois = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_nb_rois(i);
        DEBUG_STREAM<<"channel_num = "<<i<<" : nb_rois = "<<nb_rois<<endl;
        for (int j = 0; j < nb_rois; j++)
        {
			double low=0, high=0;
			dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_roi_bounds(i, j, low,high);
			DEBUG_STREAM<<"channel_num = "<<i<<", roi_num = "<<j<<", low = "<<low<<", high = "<<high<<endl;

            //- roi name
            const string name = yat::String::str_format("roi%02d_%02d", i, j + 1);
            try
            {
                INFO_STREAM << "\t- Create Mca dynamic attribute [" << name << "]" << endl;
                yat4tango::DynamicAttributeInfo dai;
                dai.dev = m_device;

                //- specify the dyn. attr.  name
                dai.tai.name = name;
                dai.tai.data_format = Tango::SCALAR;
                dai.tai.writable = Tango::READ;
                dai.tai.disp_level = Tango::OPERATOR;
                dai.tai.name = name;
                dai.tai.data_type = Tango::DEV_DOUBLE;
                dai.tai.unit = "cts";
                dai.tai.format = "%d";
                dai.tai.description = "";
                ;
                dai.set_user_data(new RoiUserData(name, i, j));

                //- instanciate the read callback (called when the dyn. attr. is read)    
                dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &AttrViewMca::read_roi_callback);

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
        }
    }



    m_is_write_at_init = true;
    //@@TODO: doesn't work . Why ?
    //    INFO_STREAM << "Write tango hardware at Init - presetType." << endl;
    //    Tango::WAttribute &presetType = m_device->get_device_attr()->get_w_attr_by_name("presetType");
    //    string mem_preset_type = yat4tango::PropertyHelper::get_property<string > (dynamic_cast<XiaDxp*>(m_device), "__MemorizedPresetType");
    //    m_dyn_preset_type->set_value(mem_preset_type)	;
    //    presetType.set_write_value(mem_preset_type);
    //    yat4tango::DynamicAttributeWriteCallbackData wcd_preset_type;
    //    wcd_preset_type.tga = &presetType;
    //    write_preset_type_callback(wcd_preset_type);


    //@@TODO: doesn't work . Why ?
    //    INFO_STREAM << "Write tango hardware at Init - presetValue." << endl;
    //    Tango::WAttribute &presetValue = m_device->get_device_attr()->get_w_attr_by_name("presetValue");
    //    Tango::DevDouble mem_preset_value = yat4tango::PropertyHelper::get_property<Tango::DevDouble> (dynamic_cast<XiaDxp*>(m_device), "__MemorizedPresetValue");
    //    m_dyn_preset_value->set_value(mem_preset_value)	;
    //    presetValue.set_write_value(mem_preset_value);
    //    yat4tango::DynamicAttributeWriteCallbackData wcd_preset_value;
    //    wcd_preset_value.tga = &presetValue;
    //    write_preset_value_callback(wcd_preset_value);
    //    m_is_write_at_init = false;
    INFO_STREAM << "AttrViewMca::init() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrViewMca::update_data(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    DEBUG_STREAM << "AttrViewMca::update_data() - [BEGIN]" << endl;

    //- update common attributes (stats + channels)	
    AttrView::update_common_attributes(ichannel, data_store);

    DEBUG_STREAM << "AttrViewMca::update_data() - [END]" << endl;
}


//+----------------------------------------------------------------------------
//
// method :         AttrViewMca::read_preset_value_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMca::read_preset_value_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMca::read_preset_value_callback()" << endl; //  << cbd.dya->get_name() << endl;

    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = dynamic_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute presetValue";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMca::read_preset_value_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMca::read_preset_value_callback");
        }

        DoubleUserData* user_data = cbd.dya->get_user_data<DoubleUserData>();
        //- set the attribute value
        double val = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_preset_value();
        user_data->set_value(val);
        cbd.tga->set_value((Tango::DevDouble*)&user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMca::read_preset_value_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMca::write_preset_value_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMca::write_preset_value_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMca::write_preset_value_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        if (state == Tango::FAULT	||
            state == Tango::INIT	||
            state == Tango::RUNNING	||
			state == Tango::OFF	||
            state == Tango::DISABLE &&
            !m_is_write_at_init)
        {
            std::string reason = "It's currently not allowed to write attribute presetValue";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMca::write_preset_value_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMca::write_preset_value_callback");
        }

        Tango::DevDouble val;
        cbd.tga->get_write_value( val);

        DoubleUserData* user_data = cbd.dya->get_user_data<DoubleUserData>();
        user_data->set_value(val);

        dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->set_preset_value(val);
        //memorize in the device propeeties
        yat4tango::PropertyHelper::set_property(dynamic_cast<XiaDxp*>(m_device), "__MemorizedPresetValue", val);
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMca::write_preset_value_callback()");
    }
}


//+----------------------------------------------------------------------------
//
// method :         AttrViewMca::read_preset_type_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMca::read_preset_type_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMca::read_preset_type_callback()" << endl; //  << cbd.dya->get_name() << endl;
    //- be sure the pointer to the dyn. attr. is valid
    if(!cbd.dya)
    {
        THROW_DEVFAILED("INTERNAL_ERROR",
                        "unexpected NULL pointer to dynamic attribute",
                        "AttrViewMca::read_preset_type_callback");
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
            std::string reason = "It's currently not allowed to read attribute presetType";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMca::read_preset_type_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMca::read_preset_type_callback()");
        }

        StringUserData* user_data = cbd.dya->get_user_data<StringUserData>();
        //- set the attribute value 
        string current = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_preset_type();
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
                                          "AttrViewMca::read_preset_type_callback()");
    }
}

//+----------------------------------------------------------------------------
//
// method :         AttrViewMca::write_preset_type_callback()
//
// description : Write gap attribute values to hardware.
//
//-----------------------------------------------------------------------------
void AttrViewMca::write_preset_type_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    DEBUG_STREAM << "AttrViewMca::write_preset_type_callback()" << endl; //  << cbd.dya->get_name() << endl;
    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        if (state == Tango::FAULT	||
            state == Tango::INIT	||
            state == Tango::RUNNING	||
			state == Tango::OFF	||			
            state == Tango::DISABLE &&
            !m_is_write_at_init)
        {
            std::string reason = "It's currently not allowed to write attribute presetType";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMca::write_preset_type_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMca::write_preset_type_callback");
        }

        Tango::DevString val;
        cbd.tga->get_write_value( val);

        string current = val;
        transform(current.begin(), current.end(), current.begin(), ::toupper);
        dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->set_preset_type(current);

        StringUserData* user_data = cbd.dya->get_user_data<StringUserData>();
        user_data->set_value(val);
        //memorize in the device propeeties
        yat4tango::PropertyHelper::set_property(dynamic_cast<XiaDxp*>(m_device), "__MemorizedPresetType", val);
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMca::write_preset_type_callback()");
    }
}

//+------------------------------------------------------------------
/**
 *	method:	AttrViewMca::read_roi_callback
 *	description: callback fct for the roi attrs
 */
//+------------------------------------------------------------------
void AttrViewMca::read_roi_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    try
    {
        Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
        bool is_device_initialized = dynamic_cast<XiaDxp*>(m_device)->is_device_initialized();
        if ((state == Tango::FAULT && is_device_initialized) ||
            state == Tango::INIT	||
			state == Tango::OFF	||
            state == Tango::DISABLE)
        {
            std::string reason = "It's currently not allowed to read attribute roixx_yy";
            Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                           reason.c_str(),
                                           "AttrViewMca::read_roi_callback()");
        }

        //- be sure the pointer to the dyn. attr. is valid
        if(!cbd.dya)
        {
            THROW_DEVFAILED("INTERNAL_ERROR",
                            "unexpected NULL pointer to dynamic attribute",
                            "AttrViewMca::read_roi_callback");
        }

        RoiUserData* user_data = cbd.dya->get_user_data<RoiUserData>();
        //- set the attribute value
        double val = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_roi_data(user_data->get_channel(), user_data->get_roi_num());
        user_data->set_value(val);
        cbd.tga->set_value((Tango::DevDouble*)&user_data->get_value());
    }
    catch (Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrViewMca::read_roi_callback()");
    }
}
}// namespace
