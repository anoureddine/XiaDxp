/*************************************************************************
/*! 
 *  \file   StreamNexus.h
 *  \brief  class StreamNexus
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef STREAM_NEXUS_H
#define STREAM_NEXUS_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include "Stream.h"
#include "DataStore.h"
#include <nexuscpp/nexuscpp.h>


namespace XiaDxp_ns
{

/*------------------------------------------------------------------
 *	class:	StreamNexus
 *	description:	specialisation class for the Nexus storage
 /------------------------------------------------------------------*/

class StreamNexus : public Stream, public nxcpp::IExceptionHandler
{
public:
    StreamNexus(Tango::DeviceImpl *dev);
    ~StreamNexus();
    void init(yat::SharedPtr<DataStore> data_store);
    void open();
    void close();
    void abort();
    void reset_index();
    void update_data(int ichannel, yat::SharedPtr<DataStore> data_store);
    void set_target_path(const std::string& target_path);
    void set_file_name(const std::string& file_name);
    void set_write_mode(const std::string& write_mode);
    void set_nb_data_per_acq(int nb_bins);
    void set_nb_acq_per_file(int nb_acq_per_file);
    void OnNexusException(const nxcpp::NexusException &ex)
    {
        DEBUG_STREAM << "StreamNexus::OnNexusException() - [BEGIN]" << endl;
        ostringstream ossMsgErr;
        ossMsgErr << endl;
        ossMsgErr << "===============================================" << endl;
        ossMsgErr << "Origin\t: " << ex.errors[0].origin << endl;
        ossMsgErr << "Desc\t: " << ex.errors[0].desc << endl;
        ossMsgErr << "Reason\t: " << ex.errors[0].reason << endl;
        ossMsgErr << "===============================================" << endl;
        ERROR_STREAM << ossMsgErr.str() << endl;
        yat::MutexLock scoped_lock(m_data_lock);        
        //1 - finalize the DataStreamer , this will set m_writer = 0 in order to avoid any new push data
        if (m_writer)
        {   
            close();
        }
        //2 -  inform m_store in order to stop acquisition & to set its state to FAULT
        m_store->abort(ex.errors[0].desc);
        DEBUG_STREAM << "StreamNexus::OnNexusException() - [END]" << endl;        
    };
private:
    void store_statistics(int module,
            int channel,
            int pixel,
            unsigned long triggers,
            unsigned long outputs,
            double icr,
            double ocr,
            double realtime,
            double livetime,
            double deadtime);

    void store_data(int module,
            int channel,
            int pixel,
            DataType* data,
            size_t length);
private:
    yat::Mutex m_data_lock;
    //- Nexus stuff	
#if defined(USE_NX_FINALIZER)
    static nxcpp::NexusDataStreamerFinalizer m_data_streamer_finalizer;
    static bool m_is_data_streamer_finalizer_started;
#endif
    yat::SharedPtr<DataStore> m_store;
    std::string m_target_path;
    std::string m_file_name;
    std::string m_write_mode;
    int m_nb_acq_per_file;
    int m_nb_bins;
    int m_nb_pixels;
    nxcpp::DataStreamer* m_writer;
    std::vector<string> m_channel_names;
    std::vector<string> m_triggers_names;
    std::vector<string> m_outputs_names;
    std::vector<string> m_icr_names;
    std::vector<string> m_ocr_names;
    std::vector<string> m_realtime_names;
    std::vector<string> m_livetime_names;
    std::vector<string> m_deadtime_names;
} ;

} // namespace 

#endif



