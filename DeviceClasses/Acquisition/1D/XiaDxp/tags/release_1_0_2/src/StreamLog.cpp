

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
	INFO_STREAM<<"trigger_livetime("<<ichannel<<")  = "	<<data_store->get_trigger_livetime(ichannel)<<endl;
	INFO_STREAM<<"realtime("<<ichannel<<")          = "	<<data_store->get_realtime(ichannel)<<endl;
	INFO_STREAM<<"livetime("<<ichannel<<")          = "	<<data_store->get_livetime(ichannel)<<endl;
	INFO_STREAM<<"deadtime("<<ichannel<<")          = "	<<data_store->get_deadtime(ichannel)<<endl;
	INFO_STREAM<<"input_count_rate("<<ichannel<<")  = " <<data_store->get_input_count_rate(ichannel)<<endl;
	INFO_STREAM<<"output_count_rate("<<ichannel<<") = "	<<data_store->get_output_count_rate(ichannel)<<endl;
	INFO_STREAM<<"triggers("<<ichannel<<")          = "	<<data_store->get_triggers(ichannel)<<endl;
	INFO_STREAM<<"events_in_run("<<ichannel<<")     = "	<<data_store->get_events_in_run(ichannel)<<endl;
	INFO_STREAM<<"channel_data("<<ichannel<<").size = "	<<data_store->get_channel_data(ichannel).size()<<endl;	
	
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
