
#include <iostream>
#include <iomanip>
#include <fstream>
#include <yat/utils/XString.h>

#include "StreamNexus.h"

namespace XiaDxp_ns
{
#if defined(USE_NX_FINALIZER)
nxcpp::NexusDataStreamerFinalizer StreamNexus::nxs_DataStreamerFinalizer;
bool StreamNexus::nxs_DataStreamerFinalizerStarted = false;
#endif

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamNexus::StreamNexus(Tango::DeviceImpl *dev)
: Stream(dev),
m_target_path("TO_BE_DEFINED"),
m_file_name("TO_BE_DEFINED"),
m_write_mode("TO_BE_DEFINED"),
m_nb_acq_per_file(0),
m_nb_bins(0),
m_nb_pixels(0),
m_writer(0)
{
    INFO_STREAM << "StreamNexus::StreamNexus() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
#if defined(USE_NX_FINALIZER)
    if ( ! StreamNexus::nxs_DataStreamerFinalizerStarted )
    {
        INFO_STREAM << "starting the underlying NexusDataStreamerFinalizer..." << std::endl;
        StreamNexus::nxs_DataStreamerFinalizer.start();
        StreamNexus::nxs_DataStreamerFinalizerStarted = true;
        INFO_STREAM << "NexusDataStreamerFinalizer successfully started" << std::endl;
    }
#endif
    INFO_STREAM << "StreamNexus::StreamNexus() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StreamNexus::~StreamNexus()
{
    INFO_STREAM << "StreamNexus::~StreamNexus() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    //- this might be required, in case we could not pass the writer to the finalizer	 
    delete m_writer;
    m_writer = 0;
    INFO_STREAM << "StreamNexus::~StreamNexus() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::init(yat::SharedPtr<DataStore> data_store)
{
    INFO_STREAM << "StreamNexus::init() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_channel_names.clear();
    m_triggers_names.clear();
    m_outputs_names.clear();
    m_icr_names.clear();
    m_ocr_names.clear();
    m_realtime_names.clear();
    m_livetime_names.clear();
    m_deadtime_names.clear();
    int total_channels = data_store->get_nb_modules() * data_store->get_nb_channels();
    m_nb_pixels = data_store->get_nb_pixels();

    INFO_STREAM << "- Create DataStreamer :"	<< endl;
    INFO_STREAM << "\t- target path = " 		<< m_target_path << endl;
    INFO_STREAM << "\t- file name = "		<< m_file_name << endl;
    INFO_STREAM << "\t- write mode = " 		<< m_write_mode << endl;
    INFO_STREAM << "\t- nb map pixels = "	<< m_nb_pixels << endl;
    INFO_STREAM << "\t- nb data per acq = "	<< m_nb_bins << endl;
    INFO_STREAM << "\t- nb acq per file = "	<< m_nb_acq_per_file << endl;

    INFO_STREAM << "\t- write data for : ";
    for (int i = 0; i < m_stream_items.size(); i++)
    {
        INFO_STREAM << "\t\t. " << m_stream_items.at(i) << endl;
    }
    INFO_STREAM << endl;

    m_writer = new nxcpp::DataStreamer(m_file_name, m_nb_pixels, m_nb_acq_per_file);

    INFO_STREAM << "- Initialize()" << endl;
    m_writer->Initialize(m_target_path);

    INFO_STREAM << "- Prepare Data Items :" << endl;
    for (int ichan = 0; ichan < total_channels; ichan++)
    {
        //- Channels        
        if ( m_is_channel_enabled )
        {
            m_channel_names.push_back(yat::String::str_format("channel%02d", ichan));
            INFO_STREAM << "\t- AddDataItem1D(" << m_channel_names[ichan] << "," << m_nb_bins << ")" << endl;
            m_writer->AddDataItem1D(m_channel_names[ichan], m_nb_bins);
        }

        //- Triggers
        if ( m_is_triggers_enabled )
        {
            m_triggers_names.push_back(yat::String::str_format("triggers%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_triggers_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_triggers_names[ichan]);
        }

        //- events in run
        if ( m_is_outputs_enabled )
        {
            m_outputs_names.push_back(yat::String::str_format("outputs%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_outputs_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_outputs_names[ichan]);
        }

        //- ICRs
        if ( m_is_icr_enabled )
        {
            m_icr_names.push_back(yat::String::str_format("icr%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_icr_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_icr_names[ichan]);
        }

        //- OCRs
        if ( m_is_ocr_enabled )
        {
            m_ocr_names.push_back(yat::String::str_format("ocr%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_ocr_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_ocr_names[ichan]);
        }

        //- Realtimes
        if ( m_is_realtime_enabled )
        {
            m_realtime_names.push_back(yat::String::str_format("realtime%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_realtime_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_realtime_names[ichan]);
        }

        //- Livetimes
        if ( m_is_livetime_enabled )
        {
            m_livetime_names.push_back(yat::String::str_format("livetime%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_livetime_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_livetime_names[ichan]);
        }

        //- Deadtimes
        if ( m_is_deadtime_enabled )
        {
            m_deadtime_names.push_back(yat::String::str_format("deadtime%02d", ichan));
            INFO_STREAM << "\t- AddDataItem0D(" << m_deadtime_names[ichan] << ")" << endl;
            m_writer->AddDataItem0D(m_deadtime_names[ichan]);
        }
    }

    // configure the Writer mode 
    INFO_STREAM << "- Configure the Writer mode :" << endl;
    //by default is IMMEDIATE			
    if (m_write_mode == "SYNCHRONOUS" )
    {
        INFO_STREAM << "\t- SYNCHRONOUS" << endl;
        m_writer->SetWriteMode(nxcpp::NexusFileWriter::SYNCHRONOUS);
    }
    else if (m_write_mode == "DELAYED" )
    {
        INFO_STREAM << "\t- DELAYED" << endl;
        m_writer->SetWriteMode(nxcpp::NexusFileWriter::DELAYED);
    }
    else
    {
        INFO_STREAM << "\t- IMMEDIATE" << endl;
        m_writer->SetWriteMode(nxcpp::NexusFileWriter::IMMEDIATE);
    }

    INFO_STREAM << "StreamNexus::init() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::open()
{
    INFO_STREAM << "StreamNexus::open() - [BEGIN]" << endl;
    INFO_STREAM << "StreamNexus::open() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::close()
{
    INFO_STREAM << "StreamNexus::close() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    if (m_writer)
    {
#if defined (USE_NX_FINALIZER) 
        //- Use NexusFinalizer to optimize the finalize which was extremly long !!!
        INFO_STREAM << "passing DataStreamer to the NexusDataStreamerFinalizer" << std::endl;
        nxcpp::NexusDataStreamerFinalizer::Entry *e = new nxcpp::NexusDataStreamerFinalizer::Entry();
        e->data_streamer = m_writer;
        m_writer = 0;
        StreamNexus::nxs_DataStreamerFinalizer.push(e);
        INFO_STREAM << "DataStreamer successfully passed to the NexusDataStreamerFinalizer" << std::endl;
#else  
        INFO_STREAM << "- Finalize()..." << endl;
        m_writer->Finalize();
        delete m_writer;
        m_writer = 0;
#endif 
    }
    INFO_STREAM << "StreamNexus::close() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::abort()
{
    INFO_STREAM << "StreamNexus::abort() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    if (m_writer)
    {
        INFO_STREAM << "- Stop()..." << endl;
        m_writer->Stop();
        delete m_writer;
        m_writer = 0;
    }
    INFO_STREAM << "StreamNexus::abort() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::set_target_path(const std::string& target_path)
{
    INFO_STREAM << "StreamNexus::set_target_path(" << target_path << ")" << endl;
    m_target_path = target_path;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::set_file_name(const std::string& file_name)
{
    INFO_STREAM << "StreamNexus::set_file_name(" << file_name << ")" << endl;
    m_file_name = file_name;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::set_write_mode(const std::string& write_mode)
{
    INFO_STREAM << "StreamNexus::set_write_mode(" << write_mode << ")" << endl;
    m_write_mode = write_mode;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::set_nb_data_per_acq(int nb_bins)
{
    INFO_STREAM << "StreamNexus::set_nb_data_per_acq(" << nb_bins << ")" << endl;
    m_nb_bins = nb_bins;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::set_nb_acq_per_file(int nb_acq_per_file)
{
    INFO_STREAM << "StreamNexus::set_nb_acq_per_file(" << nb_acq_per_file << ")" << endl;
    m_nb_acq_per_file = nb_acq_per_file;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::store_statistics(int module,
                                   int channel,
                                   int pixel,
                                   unsigned long triggers,
                                   unsigned long outputs,
                                   double icr,
                                   double ocr,
                                   double realtime,
                                   double livetime,
                                   double deadtime)
{
    //DEBUG_STREAM << "StreamNexus::store_statistics()" << endl;
    if (m_writer)
    {
        //- triggers
        if ( m_is_triggers_enabled )
        {
            m_writer->PushData(m_triggers_names[channel], &triggers);
        }

        //- outputs
        if ( m_is_outputs_enabled )
        {
            m_writer->PushData(m_outputs_names[channel], &outputs);
        }

        //- icr
        if ( m_is_icr_enabled )
        {
            m_writer->PushData(m_icr_names[channel], &icr);
        }

        //- ocr
        if ( m_is_ocr_enabled )
        {
            m_writer->PushData(m_ocr_names[channel], &ocr);
        }

        //- realtime
        if ( m_is_realtime_enabled )
        {
            m_writer->PushData(m_realtime_names[channel], &realtime);
        }

        //- livetime
        if ( m_is_livetime_enabled )
        {
            m_writer->PushData(m_livetime_names[channel], &livetime);
        }

        //- deadtime
        if ( m_is_deadtime_enabled )
        {
            m_writer->PushData(m_deadtime_names[channel], &deadtime);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::store_data(int module, int channel, int pixel, DataType* data, size_t length)
{
    //DEBUG_STREAM << "StreamNexus::store_data()" << endl;
    if (m_writer)
    {
        //- channel
        if ( m_is_channel_enabled )
        {
            m_writer->PushData(m_channel_names[channel], data);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::update_data(int ichannel, yat::SharedPtr<DataStore> data_store)
{
    //DEBUG_STREAM << "StreamNexus::update_data() - [pixel = "<<data_store->get_current_pixel(ichannel)<<"]" << endl;

    //DEBUG_STREAM<<"\t- Store statistics : channel "<<ichannel<<endl;
    yat::MutexLock scoped_lock(m_data_lock);
    store_statistics(data_store->get_nb_modules(),
                     ichannel,
                     data_store->get_nb_pixels(),
                     data_store->get_triggers(ichannel),
                     data_store->get_outputs(ichannel),
                     data_store->get_input_count_rate(ichannel),
                     data_store->get_output_count_rate(ichannel),
                     data_store->get_realtime(ichannel),
                     data_store->get_livetime(ichannel),
                     data_store->get_deadtime(ichannel)
                     );


    //DEBUG_STREAM<<"\t- Store data\t   : channel "<<ichannel<<endl;
    store_data(data_store->get_nb_modules(),
               ichannel,
               data_store->get_nb_pixels(),
               (DataType*) & data_store->get_channel_data(ichannel)[0],
               data_store->get_channel_data(ichannel).size()
               );
    //INFO_STREAM<<" "<<endl;    
    DEBUG_STREAM << "StreamNexus::update_data() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StreamNexus::reset_index()
{
    INFO_STREAM << "StreamNexus::reset_index() - [BEGIN]" << endl;
    INFO_STREAM << "- ResetBufferIndex()" << endl;
    nxcpp::DataStreamer::ResetBufferIndex();
    INFO_STREAM << "StreamNexus::reset_index() - [END]" << endl;
}

}// namespace
