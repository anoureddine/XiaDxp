

#include "Stream.h"

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
Stream::Stream(Tango::DeviceImpl *dev)
: Tango::LogAdapter(dev),
m_is_channel_enabled(false),
m_is_triggers_enabled(false),
m_is_outputs_enabled(false),
m_is_icr_enabled(false),
m_is_ocr_enabled(false),
m_is_realtime_enabled(false),
m_is_livetime_enabled(false),
m_is_deadtime_enabled(false)
{
    DEBUG_STREAM << "Stream::Stream() - [BEGIN]" << endl;

    DEBUG_STREAM << "Stream::Stream() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
Stream::~Stream()
{
    DEBUG_STREAM << "Stream::~Stream() - [BEGIN]" << endl;

    DEBUG_STREAM << "Stream::~Stream() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
std::vector<std::string> Stream::get_stream_items(void)
{
    return m_stream_items;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void Stream::set_stream_items(std::vector<std::string> stream_items)
{
    INFO_STREAM << "Stream::set_stream_items( nb_items = " << stream_items.size()<<" )"<<endl;    
    //- update m_stream_items
	m_stream_items = stream_items;

    //- store which data must be stored
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "channel") != m_stream_items.end() )
        m_is_channel_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "triggers") != m_stream_items.end() )
        m_is_triggers_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "outputs") != m_stream_items.end() )
        m_is_outputs_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "icr") != m_stream_items.end() )
        m_is_icr_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "ocr") != m_stream_items.end() )
        m_is_ocr_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "realtime") != m_stream_items.end() )
        m_is_realtime_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "livetime") != m_stream_items.end() )
        m_is_livetime_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "deadtime") != m_stream_items.end() )
        m_is_deadtime_enabled = true;

}

}// namespace
