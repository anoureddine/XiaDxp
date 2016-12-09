

#include "StreamLog.h"

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamLog::StreamLog(Tango::DeviceImpl *dev)
: Stream(dev)
{
    INFO_STREAM << "StreamLog::StreamLog() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::StreamLog() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamLog::~StreamLog()
{
    INFO_STREAM << "StreamLog::~StreamLog() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::~StreamLog() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::init(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "StreamLog::init() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::init() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::open()
{
    INFO_STREAM << "StreamLog::open() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::open() - [END]" << endl;
}


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::close()
{
    INFO_STREAM << "StreamLog::close() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::close() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::abort()
{
    INFO_STREAM << "StreamLog::abort() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::abort() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::update_data(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "StreamLog::update_data() - [BEGIN]" << endl;

    INFO_STREAM<<"nb_modules()              = " <<data_store->get_nb_modules()<<endl;
    INFO_STREAM<<"nb_channels()             = " <<data_store->get_nb_channels()<<endl;
    INFO_STREAM<<"nb_pixels()               = "	<<data_store->get_nb_pixels()<<endl<<endl;
    //- realtime
    if ( m_is_realtime_enabled )
    {
        INFO_STREAM<<"realtime("<<ichannel<<")          = "	<<data_store->get_realtime(ichannel)<<endl;
    }

    //- livetime
    if ( m_is_livetime_enabled )
    {
        INFO_STREAM<<"livetime("<<ichannel<<")          = "	<<data_store->get_livetime(ichannel)<<endl;
    }

    //- deadtime
    if ( m_is_deadtime_enabled )
    {
        INFO_STREAM<<"deadtime("<<ichannel<<")          = "	<<data_store->get_deadtime(ichannel)<<endl;
    }

    //- icr
    if ( m_is_icr_enabled )
    {
        INFO_STREAM<<"input_count_rate("<<ichannel<<")  = " <<data_store->get_input_count_rate(ichannel)<<endl;
    }

    //- ocr
    if ( m_is_ocr_enabled )
    {
        INFO_STREAM<<"output_count_rate("<<ichannel<<") = "	<<data_store->get_output_count_rate(ichannel)<<endl;
    }

    //- triggers
    if ( m_is_triggers_enabled )
    {
        INFO_STREAM<<"triggers("<<ichannel<<")          = "	<<data_store->get_triggers(ichannel)<<endl;
    }

    //- outputs
    if ( m_is_outputs_enabled )
    {
        INFO_STREAM<<"outputs("<<ichannel<<")     = "	<<data_store->get_outputs(ichannel)<<endl;
    }

    //- channel
    if ( m_is_channel_enabled )
    {
        INFO_STREAM<<"channel_data("<<ichannel<<").size = "	<<data_store->get_channel_data(ichannel).size()<<endl;
    }

    INFO_STREAM << "StreamLog::update_data() - [END]" << endl;

}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamLog::reset_index()
{
    INFO_STREAM << "StreamLog::reset_index() - [BEGIN]" << endl;
    INFO_STREAM << "StreamLog::reset_index() - [END]" << endl;
}

}// namespace
