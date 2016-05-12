/*************************************************************************
/*! 
 *  \file   StreamNo.h
 *  \brief  class StreamNo
 *  \author Arafat NOUREDDINE
 */
/*************************************************************************/

#ifndef STREAM_NO_H
#define STREAM_NO_H

#include <tango.h>
#include <yat/memory/SharedPtr.h>
#include "Stream.h"
#include "DataStore.h"


namespace XiaDxp_ns
{
/*------------------------------------------------------------------
 *	class:	StreamNo
 *	description:	specialisation class for the NO storage
 /------------------------------------------------------------------*/
class StreamNo : public Stream
{
public:
    StreamNo(Tango::DeviceImpl *dev);
    ~StreamNo();
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



