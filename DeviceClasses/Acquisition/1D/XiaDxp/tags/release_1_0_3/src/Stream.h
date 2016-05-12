/*************************************************************************
/*! 
 *  \file   Stream.h
 *  \brief  class Stream
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef STREAM_H
#define STREAM_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include "DataStore.h"

namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	Stream
 *	description: base class for the data storage
 /------------------------------------------------------------------*/
class Stream : public Tango::LogAdapter
{
public:
    Stream(Tango::DeviceImpl *dev);
    ~Stream();
    virtual void init(yat::SharedPtr<DataStore> data_store) = 0;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void abort() = 0;
    virtual void update_data(int ichannel, yat::SharedPtr<DataStore> data_store) = 0;
    virtual void reset_index() = 0;
protected:

};

} // namespace 

#endif


