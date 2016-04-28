/*************************************************************************
/*! 
 *  \file   StreamLog.h
 *  \brief  class StreamLog
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef STREAM_LOG_H
#define STREAM_LOG_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include "Stream.h"
#include "DataStore.h"


namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	StreamLog
 *	description:	specialisation class for the NO storage
 /------------------------------------------------------------------*/
class StreamLog : public Stream
{
public:
    StreamLog(Tango::DeviceImpl *dev);
    ~StreamLog();
    void init(yat::SharedPtr<DataStore> data_store);
    void open();
    void close();
    void abort();
    void update_data(int ichannel, yat::SharedPtr<DataStore> data_store);
    void reset_index();
private:

};

} // namespace 

#endif



