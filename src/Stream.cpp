

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
    //- clear previous one
    m_stream_items.clear();

    //- store it in upper case
    for (int i = 0; i < stream_items.size(); i++)
    {
        m_stream_items.push_back(stream_items.at(i));
        std::transform(m_stream_items.at(i).begin(), m_stream_items.at(i).end(), m_stream_items.at(i).begin(), ::toupper);
    }

    //- fix which data must be stored
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "CHANNEL") != m_stream_items.end() )
        m_is_channel_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "TRIGGERS") != m_stream_items.end() )
        m_is_triggers_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "OUTPUTS") != m_stream_items.end() )
        m_is_outputs_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "ICR") != m_stream_items.end() )
        m_is_icr_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "OCR") != m_stream_items.end() )
        m_is_ocr_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "REALTIME") != m_stream_items.end() )
        m_is_realtime_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "LIVETIME") != m_stream_items.end() )
        m_is_livetime_enabled = true;
    if ( std::find(m_stream_items.begin(), m_stream_items.end(), "DEADTIME") != m_stream_items.end() )
        m_is_deadtime_enabled = true;

}

}// namespace
