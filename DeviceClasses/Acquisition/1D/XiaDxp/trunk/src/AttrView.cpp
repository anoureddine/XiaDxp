//=============================================================================
//
// file :        AttrView.cpp
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

#include "AttrView.h"
#include <iomanip>      
#include <XiaDxp.h>

namespace XiaDxp_ns
{


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrView::AttrView(Tango::DeviceImpl *dev)
: Tango::LogAdapter(dev),
m_device(dev),
m_dim(dev)
{
    INFO_STREAM << "AttrView::AttrView() - [BEGIN]" << endl;

    INFO_STREAM << "AttrView::AttrView() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
AttrView::~AttrView()
{
    INFO_STREAM << "AttrView::~AttrView() - [BEGIN]" << endl;
    INFO_STREAM << "AttrView::~AttrView() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrView::init_common_attributes(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "AttrView::init_common_attributes() - [BEGIN]" << endl;

    int total_channels = data_store->get_nb_modules()*data_store->get_nb_channels();
    INFO_STREAM<<"total_channels = "<<total_channels<<endl;

    int nb_pixels = data_store->get_nb_pixels();
    INFO_STREAM<<"nb_pixels = "<<nb_pixels<<endl;

	int nb_bins = data_store->get_nb_bins();
    INFO_STREAM<<"nb_bins = "<<nb_bins<<endl;
	
    m_dyn_realtime.resize(total_channels);
    m_dyn_livetime.resize(total_channels);
    m_dyn_deadtime.resize(total_channels);
    m_dyn_icr.resize(total_channels);
    m_dyn_ocr.resize(total_channels);
    m_dyn_trigger_livetime.resize(total_channels);
    m_dyn_events_in_run.resize(total_channels);
    m_dyn_channel.resize(total_channels);

    for(int i = 0; i<total_channels; i++)//@@TODO call init when store is already initialized !!
    {
        //callback_type foo = &AttrView::read_stat_callback;
        ////////////////////////////////////////////////////////////////////////////////////////
        stringstream ss("");
        std::string name("");
        name = "triggerLiveTime";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_trigger_livetime[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "sec",
                         "%f",
                         "The livetime run statistic as measured by the trigger filter, reported in seconds.",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_trigger_livetime[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "realTime";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_realtime[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "sec",
                         "%f",
                         "The total time that the processor was taking data in seconds",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_realtime[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "liveTime";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_livetime[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "sec",
                         "%f",
                         "The total time that the processor was able to acquire new events in seconds.",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_livetime[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "deadTime";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_deadtime[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "%",
                         "%f",
                         "Deadtime as calculated by (ICR - OCR) / ICR",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_deadtime[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "inputCountRate";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_icr[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "cts/sec",
                         "%f",
                         "This is the total number of triggers divided by the runtime in counts per second.",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_icr[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "outputCountRate";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_ocr[i] = new DoubleUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_DOUBLE,
                         Tango::SCALAR,
                         Tango::READ,
                         "cts/sec",
                         "%f",
                         "This is the total number of events in the run divided by the runtime in counts per second.",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_ocr[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "eventsInRun";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_events_in_run[i] = new ULongUserData(ss.str());
        create_attribute(	ss.str(),
                         Tango::DEV_ULONG,
                         Tango::SCALAR,
                         Tango::READ,
                         "cts",
                         "%d",
                         "The total number of events in a run that are written into the MCA spectrum.",
                         &AttrView::read_stat_callback,
                         &AttrView::write_callback_null,
                         m_dyn_events_in_run[i]);

        ////////////////////////////////////////////////////////////////////////////////////////
        ss.str("");
        name = "channel";
        ss<<name<<"_"<<std::setfill('0') << std::setw(2)<<i;
        m_dyn_channel[i] = new ChannelUserData(ss.str(), nb_bins);
        create_attribute(	ss.str(),
                         Tango::DEV_ULONG,
                         Tango::SPECTRUM,
                         Tango::READ,
                         "cts",
                         "%9d",
                         "Fluo spectrum for " + ss.str(),
                         &AttrView::read_channel_callback,
                         &AttrView::write_callback_null,
                         m_dyn_channel[i]);

    }
    INFO_STREAM << "AttrView::init_common_attributes() - [END]" << endl;
}

//----------------------------------------------------------------------------------------
// DYN. ATTRS. READ CALLBACK
//----------------------------------------------------------------------------------------
void AttrView::read_stat_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrView::read_stat_callback() : "<<cbd.dya->get_name()<<" - [BEGIN]" << endl;
    Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
    bool is_device_initialized = dynamic_cast<XiaDxp*>(m_device)->is_device_initialized();
    if ((state == Tango::FAULT && is_device_initialized) ||
        state == Tango::INIT	||
        state == Tango::OFF	||
        state == Tango::DISABLE)
    {
        std::string reason = "It's currently not allowed to read attribute : "+cbd.dya->get_name();
        Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                       reason.c_str(),
                                       "AttrView::read_stat_callback()");
    }

    //- be sure the pointer to the dyn. attr. is valid
    if(!cbd.dya)
    {
        THROW_DEVFAILED("INTERNAL_ERROR",
                        "unexpected NULL pointer to dynamic attribute",
                        "DynamicInterface::read_callback");
    }

    try
    {
        switch(cbd.dya->get_tango_data_type())
        {
            case Tango::DEV_DOUBLE:
            {
                DoubleUserData* user_data = cbd.dya->get_user_data<DoubleUserData>();
                //- set the attribute value
                cbd.tga->set_value((Tango::DevDouble*)&user_data->get_value());
            }
                break;

            case Tango::DEV_ULONG:
            {
                ULongUserData* user_data = cbd.dya->get_user_data<ULongUserData>();
                //- set the attribute value
                cbd.tga->set_value((Tango::DevULong*)&user_data->get_value());
            }
                break;
        }
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrView::read_stat_callback");
    }
    DEBUG_STREAM << "AttrView::read_stat_callback() - [END]" << endl;
}

//----------------------------------------------------------------------------------------
// DYN. ATTRS. READ CALLBACK
//----------------------------------------------------------------------------------------
void AttrView::read_channel_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "AttrView::read_channel_callback() : "<<cbd.dya->get_name()<<" - [BEGIN]" << endl;

    Tango::DevState state = dynamic_cast<XiaDxp*>(m_device)->get_ctrl()->get_state();
    bool is_device_initialized = dynamic_cast<XiaDxp*>(m_device)->is_device_initialized();
    if ((state == Tango::FAULT && is_device_initialized) ||
        state == Tango::INIT	||
        state == Tango::OFF	||
        state == Tango::DISABLE)
    {
        std::string reason = "It's currently not allowed to read attribute : "+cbd.dya->get_name();
        Tango::Except::throw_exception("TANGO_DEVICE_ERROR",
                                       reason.c_str(),
                                       "AttrView::read_channel_callback()");
    }
    
    //- be sure the pointer to the dyn. attr. is valid
    if(!cbd.dya)
    {
        THROW_DEVFAILED("INTERNAL_ERROR",
                        "unexpected NULL pointer to dynamic attribute",
                        "DynamicInterface::read_callback");
    }

    try
    {
        ChannelUserData* channel_data = cbd.dya->get_user_data<ChannelUserData>();
        //- set the attribute value
        cbd.tga->set_value(const_cast<Tango::DevULong*>(&channel_data->get_value()[0]), channel_data->get_value().size());
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
                                          "TANGO_DEVICE_ERROR",
                                          string(df.errors[0].desc).c_str(),
                                          "AttrView::read_channel_callback");
    }
    DEBUG_STREAM << "AttrView::read_channel_callback() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void AttrView::update_common_attributes(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    DEBUG_STREAM << "AttrView::update_common_attributes() - [BEGIN]" << endl;

    //- update stats attributes	
    m_dyn_realtime[ichannel]->set_value(data_store->get_realtime(ichannel));
    m_dyn_livetime[ichannel]->set_value(data_store->get_livetime(ichannel));
    m_dyn_deadtime[ichannel]->set_value(data_store->get_deadtime(ichannel));
    m_dyn_icr[ichannel]->set_value(data_store->get_input_count_rate(ichannel));
    m_dyn_ocr[ichannel]->set_value(data_store->get_output_count_rate(ichannel));
    m_dyn_trigger_livetime[ichannel]->set_value(data_store->get_trigger_livetime(ichannel));
    m_dyn_events_in_run[ichannel]->set_value(data_store->get_events_in_run(ichannel));

    //- update channels attributes	
    m_dyn_channel[ichannel]->set_value(data_store->get_channel_data(ichannel));

    DEBUG_STREAM << "AttrView::update_common_attributes() - [END]" << endl;
}

}// namespace
