/*************************************************************************
/*! 
 *  \file   StreamCsv.h
 *  \brief  class StreamCsv
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef STREAM_CSV_H
#define STREAM_CSV_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include <yat/memory/UniquePtr.h>
#include <yat/threading/Mutex.h>
#include "Stream.h"
#include "DataStore.h"

namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	StreamCsv
 *	description:	specialisation class for the Nexus storage
 /------------------------------------------------------------------*/
class StreamCsv : public Stream
{
public:
    StreamCsv(Tango::DeviceImpl *dev);
    ~StreamCsv();
    void init(yat::SharedPtr<DataStore> data_store);
    void open();
    void close();
    void abort();
    void reset_index();
    void update_data(int ichannel, yat::SharedPtr<DataStore> data_store);
	void set_target_path(const std::string& target_path);
	void set_file_name(const std::string& file_name);
private:
private:
    yat::Mutex m_data_lock;
    //- file stuff	
    std::string m_target_path;
    std::string m_file_name;
    int m_scan_index;
};

} // namespace 

#endif



