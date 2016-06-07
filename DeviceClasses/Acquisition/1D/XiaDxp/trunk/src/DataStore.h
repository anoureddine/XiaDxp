/*************************************************************************
/*! 
 *  \file   DataStore.h
 *  \brief  class DataStore
 *  \author Arafat Noureddine - SOLEIL (consultant MEDIANE SYSTEME) /V. Kaiser - ANKA / F. Langlois - SOLEIL
 */
/*************************************************************************/

#ifndef DATA_STORE_H
#define DATA_STORE_H

//TANGO
#include <tango.h>

//- YAT/YAT4TANGO
#include <yat/memory/SharedPtr.h>
#include <yat/threading/Mutex.h>
#include <yat/memory/DataBuffer.h>
#include <yat4tango/DeviceTask.h>
#include <yat/any/Any.h>
#include <vector>
#include <nexuscpp/nexuscpp.h>

const size_t DATASTORE_TASK_PERIODIC_MS      = 100;//ms
const size_t DATASTORE_PROCESS_DATA_MSG      = yat::FIRST_USER_MSG + 600;
const size_t DATASTORE_CLOSE_DATA_MSG        = yat::FIRST_USER_MSG + 601;
const size_t DATASTORE_ABORT_DATA_MSG        = yat::FIRST_USER_MSG + 602;


///return the cluster channel number from module number and channel number
inline int to_channel_cluster(int module, int channel)          {return (module*4+channel);}
///return the std::pair<module number, channel number> from a cluster channel number
inline std::pair<int, int> to_module_and_channel(int channel)   {return std::make_pair(((int)channel/(int)4), ((int)channel%(int)4));}

namespace XiaDxp_ns
{
typedef unsigned long DataType;
typedef yat::Buffer<DataType> DataBuffer;
typedef yat::SharedPtr<DataBuffer> DataBufferPtr;

//------------------------------------------------------------------
//	Data structures
//------------------------------------------------------------------
struct PixelData
{
    int pixel;
	unsigned long triggers;
    unsigned long outputs;
    double trigger_livetime;
    unsigned long events_in_run;
    double input_count_rate; //input count rate
    double output_count_rate; //output count rate
    double deadtime;
    double realtime;
    double livetime;

    std::vector<DataType> data;
};

//------------------------------------------------------------------
struct ChannelData
{
    int channel;
    PixelData pixel_data;
};

//------------------------------------------------------------------
struct ModuleData
{
    int module;
    std::vector<ChannelData> channel_data;
};

//------------------------------------------------------------------
struct Data
{
    std::vector<ModuleData> module_data;
};

//------------------------------------------------------------------
class DataBufferContainer
{
public:
    DataBufferContainer(int module, std::size_t buffer_length);
    DataType* base() const;
    int module() const;

private:
    int m_module;
    DataBufferPtr m_buffer;
};

//------------------------------------------------------------------
//	class:	DataStore
//------------------------------------------------------------------
class DataStore : public yat4tango::DeviceTask
{
public:
    ///ctor
    DataStore(Tango::DeviceImpl *dev);
    ///dtor
    virtual ~DataStore();
    void init(int nb_modules, int nb_channels, int nb_pixels, int nb_bins, const std::string& acquisition_type);
    void store_statistics(int module, int channel, int pixel, PixelData pix_data);
    void store_data(int module, int channel, int pixel, DataType* data, size_t length);    
	void process_data(DataBufferContainer* map_buffer);
	void close_data(void);    
    void abort_data(void);           
    void subscribe(class Controller* observer);
    void set_nb_pixels(int nb_pixels);
    double get_timebase(void);  
	void set_timebase(double timebase);  
    
    ///getters
    int get_nb_modules();
    int get_nb_channels();
    int get_nb_pixels();
	int get_nb_bins();
	long   get_current_pixel(int channel);	
	double get_trigger_livetime(int channel);	
    double get_realtime(int channel);
    double get_livetime(int channel);
    double get_deadtime(int channel);		
    double get_output_count_rate(int channel);
    double get_input_count_rate(int channel);
    unsigned long get_triggers(int channel);
    unsigned long get_outputs(int channel);
    unsigned long get_events_in_run(int channel);
    std::vector<DataType> get_channel_data(int channel);        
    Tango::DevState get_state(void);
    std::string get_status(void);
protected:
    void process_message(yat::Message& msg) throw (Tango::DevFailed);      
private:
    ///notify_data controller (observer) about new datas
    void notify_data(int ichannel);
    ///notify_data controller (observer) at the end of datas
	void on_close_data();
    ///notify_data controller (observer) at the abort of datas
	void on_abort_data();    
    ///process the data's buffer in order to get statistics & datas from the acquired buffer
    void on_process_data(DataBufferContainer& map_buffer);	
    ///parse datas for each acquired pixel
    void parse_data(int module, int pixel, DataType* map_buffer);            
    ///compute statistics
    double compute_realtime(double realtime);
    double compute_livetime(double livetime);
	double compute_deadtime(unsigned long outputs, unsigned long triggers, double livetime, double realtime);	
    double compute_deadtime(double ocr, double icr);	
    double compute_input_count_rate(unsigned long trigger_livetime, double livetime);
    double compute_output_count_rate(unsigned long outputs, double realtime);    
    ///
    void set_state(Tango::DevState state);
    void set_status(const std::string& status);    
    ///

    Tango::DevState m_state;
    std::stringstream m_status;
    yat::Mutex m_state_lock;
    yat::Mutex m_data_lock;
    Data m_data;
    int m_nb_modules;
    int m_nb_channels;
    int m_nb_pixels;
	int m_nb_bins;
    std::string m_acquisition_type;
    double m_timebase;
    class Controller* m_controller;
};

} 

#endif //DATA_STORE_H