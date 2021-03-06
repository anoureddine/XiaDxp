
#include <iostream>
#include <iomanip>
#include <fstream>
#include <yat/utils/XString.h>
#include <yat/utils/String.h>
#include "StreamCsv.h"

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamCsv::StreamCsv(Tango::DeviceImpl *dev)
: Stream(dev),
m_target_path("TO_BE_DEFINED"),
m_file_name("TO_BE_DEFINED"),
m_scan_index(-1)
{
    INFO_STREAM << "StreamCsv::StreamCsv() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "StreamCsv::StreamCsv() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamCsv::~StreamCsv()
{
    INFO_STREAM << "StreamCsv::~StreamCsv() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "StreamCsv::~StreamCsv() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::init(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "StreamCsv::init() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_scan_index++;
    INFO_STREAM<<"m_target_path = "<<m_target_path<<endl;
    INFO_STREAM<<"m_file_name = "<<m_file_name<<endl;
    INFO_STREAM<<"m_scan_index = "<<m_scan_index<<endl;
    INFO_STREAM << "StreamCsv::init() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::open()
{
    INFO_STREAM << "StreamCsv::open() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "StreamCsv::open() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::close()
{
    INFO_STREAM << "StreamCsv::close() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "StreamCsv::close() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::abort()
{
    INFO_STREAM << "StreamCsv::abort() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "StreamCsv::abort() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::set_target_path(const std::string& target_path)
{
    INFO_STREAM << "StreamCsv::set_target_path(" << target_path<<")"<<endl;
    m_target_path = target_path;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::set_file_name(const std::string& file_name)
{
    INFO_STREAM << "StreamCsv::set_file_name(" << file_name<<")"<<endl;
    m_file_name = file_name;
}


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::update_data(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    DEBUG_STREAM << "StreamCsv::update_data() - [BEGIN]"<<endl;

    yat::MutexLock scoped_lock(m_data_lock);
    int module = to_module_and_channel(ichannel).first;
    int channel_of_module = to_module_and_channel(ichannel).second;
    int current_pixel = data_store->get_current_pixel(ichannel);
    ///GENERATE 1 CSV FILE per MODULE & per CHANNEL

    char filename[255];
    sprintf(filename, "%s/%s_module%02d_channel%02d_%04d.csv", m_target_path.c_str(), m_file_name.c_str(), module, ichannel, m_scan_index);
    std::ofstream output_file(filename , std::ios::out | std::ofstream::binary| std::ofstream::app);

    //Header only at beginning of file
    output_file.seekp(0, ios::end);
    if((long)0 == output_file.tellp())
    {
        output_file<<"currentPixel;triggers;realTime;liveTime;deadTime;inputCountRate;outputCountRate;outputs;channel_spectrum;"<<std::endl;
    }

    //- num pixel
    output_file<<data_store->get_current_pixel(ichannel)<<";";

    //- triggers
    if ( m_is_triggers_enabled )
    {
        output_file<<data_store->get_triggers(ichannel)<<";";
    }

    //- realtime
    if ( m_is_realtime_enabled )
    {
        output_file<<data_store->get_realtime(ichannel)<<";";
    }

    //- livetime
    if ( m_is_livetime_enabled )
    {
        output_file<<data_store->get_livetime(ichannel)<<";";
    }

    //- dedatime
    if ( m_is_deadtime_enabled )
    {
        output_file<<data_store->get_deadtime(ichannel)<<";";
    }

    //- icr
    if ( m_is_icr_enabled )
    {
        output_file<<data_store->get_input_count_rate(ichannel)<<";";
    }

    //- ocr
    if ( m_is_ocr_enabled )
    {
        output_file<<data_store->get_output_count_rate(ichannel)<<";";
    }

    //- outputs
    if ( m_is_outputs_enabled )
    {
        output_file<<data_store->get_outputs(ichannel)<<";";
    }

    //- channel
    if ( m_is_channel_enabled )
    {
        std::ostream_iterator<DataType> output_iterator(output_file, ";");
        vector<DataType> data_array = data_store->get_channel_data(ichannel);
        std::copy(data_array.begin(), data_array.end(), output_iterator);
    }

    output_file<<std::endl;
    output_file.flush();

    DEBUG_STREAM << "StreamCsv::update_data() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamCsv::reset_index()
{
    INFO_STREAM << "StreamCsv::reset_index() - [BEGIN]" << endl;
    m_scan_index = -1;
    INFO_STREAM << "StreamCsv::reset_index() - [END]" << endl;
}

}// namespace
