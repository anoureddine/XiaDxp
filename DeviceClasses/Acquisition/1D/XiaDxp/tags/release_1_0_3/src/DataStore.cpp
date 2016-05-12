//=============================================================================
//
// file :        DataStore.cpp
//
// description : 
//
// project :	XiaDxp Project
//
// $Author: noureddine $
//
// copyleft :    Synchrotron SOLEIL
//               L'Orme des merisiers - Saint Aubin
//               BP48 - 91192 Gif sur Yvette
//               FRANCE
//=============================================================================

#include <tango.h>
#include "Controller.h"
#include "DataStore.h"


namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//- ctor
//----------------------------------------------------------------------------------------------------------------------
DataStore::DataStore(Tango::DeviceImpl *dev)
: yat4tango::DeviceTask(dev)
{
    INFO_STREAM << "DataStore::DataStore() - [BEGIN]" << endl;
    enable_timeout_msg(false);
    enable_periodic_msg(false);
    set_periodic_msg_period(CONTROLLER_TASK_PERIODIC_MS);
    m_nb_modules = 0;
    m_nb_channels = 0;
    m_nb_pixels = 0;
    m_nb_bins = 0;
    m_timebase = 1;
    m_is_statistics_enabled = true;
    set_status("");
    set_state(Tango::OFF);
    INFO_STREAM << "DataStore::DataStore() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- dtor
//----------------------------------------------------------------------------------------------------------------------
DataStore::~DataStore()
{
    INFO_STREAM << "DataStore::~DataStore() - [BEGIN]" << endl;
    INFO_STREAM << "DataStore::~DataStore() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::init
//----------------------------------------------------------------------------------------------------------------------
void DataStore::init(int nb_modules, int nb_channels, int nb_pixels, int nb_bins)
{
    INFO_STREAM << "DataStore::init() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_nb_modules = nb_modules;
    m_nb_channels = nb_channels;
    m_nb_pixels = nb_pixels;
    m_nb_bins = nb_bins;
    m_data.module_data.clear();

    // setup mapping data structure
    m_data.module_data.resize(nb_modules);
    for (int idx_mod = 0; idx_mod < nb_modules; idx_mod++)
    {
        m_data.module_data[idx_mod].module = idx_mod;
        m_data.module_data[idx_mod].channel_data.resize(nb_channels);

        for (int idx_ch = 0; idx_ch < nb_channels; idx_ch++)
        {
            m_data.module_data[idx_mod].channel_data[idx_ch].channel = idx_ch;

            // only the last pixel is stored            
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.pixel = -1;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.trigger_livetime = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.triggers = 0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.outputs = 0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.input_count_rate = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.output_count_rate = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.deadtime = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.realtime = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.livetime = 0.0;
            m_data.module_data[idx_mod].channel_data[idx_ch].pixel_data.data.resize(m_nb_bins);
        }
    }
    set_state(Tango::STANDBY);
    INFO_STREAM << "DataStore::init() - [END]" << endl;
}
//----------------------------------------------------------------------------------------------------------------------
//- DataStore::store_statistics
//----------------------------------------------------------------------------------------------------------------------
void DataStore::store_statistics(int module, int channel, int pixel, const std::string& acquisition_type, PixelData pix_data)
{
    DEBUG_STREAM << "DataStore::store_statistics() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    set_state(Tango::RUNNING);
    double computed_realtime = 0.0;
    double computed_livetime = 0.0;
    double computed_icr = 0.0;
    double computed_ocr = 0.0;
    double computed_deadtime = 0.0;
    double computed_trigger_livetime = 0.0;
    if(acquisition_type == "MAPPING")
    {
        computed_deadtime = compute_deadtime(pix_data.outputs, pix_data.triggers, pix_data.livetime, pix_data.realtime); //100*(1 - outputs*livetime/(realtime*triggers))
        computed_realtime = compute_realtime(pix_data.realtime);
        computed_livetime = compute_livetime(pix_data.livetime);
        computed_icr = compute_input_count_rate(pix_data.triggers, computed_livetime);
        computed_ocr = compute_output_count_rate(pix_data.outputs, computed_realtime);
        computed_trigger_livetime = 0.0; //always =0.0
    }
    else if(acquisition_type == "MCA")
    {
        computed_deadtime = compute_deadtime(pix_data.output_count_rate, pix_data.input_count_rate); //100* (icr - ocr)/icr
        computed_realtime = pix_data.realtime;
        computed_livetime = pix_data.livetime;
        computed_icr = pix_data.input_count_rate;
        computed_ocr = pix_data.output_count_rate;
        computed_trigger_livetime = pix_data.trigger_livetime;
    }

    m_data.module_data[module].channel_data[channel].pixel_data.pixel = pixel;
    m_data.module_data[module].channel_data[channel].pixel_data.triggers = pix_data.triggers;
    m_data.module_data[module].channel_data[channel].pixel_data.outputs = pix_data.outputs;
    m_data.module_data[module].channel_data[channel].pixel_data.trigger_livetime = computed_trigger_livetime;
    m_data.module_data[module].channel_data[channel].pixel_data.events_in_run = pix_data.events_in_run;
    m_data.module_data[module].channel_data[channel].pixel_data.input_count_rate = computed_icr;
    m_data.module_data[module].channel_data[channel].pixel_data.output_count_rate = computed_ocr;
    m_data.module_data[module].channel_data[channel].pixel_data.deadtime = computed_deadtime;
    m_data.module_data[module].channel_data[channel].pixel_data.realtime = computed_realtime;
    m_data.module_data[module].channel_data[channel].pixel_data.livetime = computed_livetime;
    DEBUG_STREAM << "DataStore::store_statistics() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::store_data
//----------------------------------------------------------------------------------------------------------------------
void DataStore::store_data(int module, int channel, int pixel, DataType* data, size_t length)
{
    DEBUG_STREAM << "DataStore::store_data() - [BEGIN]" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    set_state(Tango::RUNNING);
    m_data.module_data[module].channel_data[channel].pixel_data.pixel = pixel;
    m_data.module_data[module].channel_data[channel].pixel_data.data.resize(length);
    std::copy(data, data + length, &m_data.module_data[module].channel_data[channel].pixel_data.data[0]);
    //inform observers that data is changed
    int channel_cluster = to_channel_cluster(module, channel);
    notify(channel_cluster); //data+view    
    DEBUG_STREAM << "DataStore::store_data() - [END]" << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::on_close
//----------------------------------------------------------------------------------------------------------------------
void DataStore::on_close()
{
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "DataStore::on_close() - [BEGIN]" << endl;
    if(m_controller!=0)
        m_controller->close();
    set_state(Tango::STANDBY);
    INFO_STREAM << "DataStore::on_close() - [END]" << endl;
    INFO_STREAM<<"--------------------------------------------"<<std::endl;
    INFO_STREAM<<" "<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::on_abort
//----------------------------------------------------------------------------------------------------------------------
void DataStore::on_abort()
{
    yat::MutexLock scoped_lock(m_data_lock);
    INFO_STREAM << "DataStore::on_abort() - [BEGIN]" << endl;
    if(m_controller!=0)
        m_controller->abort();
    set_state(Tango::STANDBY);
    INFO_STREAM << "DataStore::on_abort() - [END]" << endl;
    INFO_STREAM<<"--------------------------------------------"<<std::endl;
    INFO_STREAM<<" "<<std::endl;    
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_current_pixel
//----------------------------------------------------------------------------------------------------------------------
long   DataStore::get_current_pixel(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    long current_pixel;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    current_pixel = m_data.module_data[module].channel_data[channel_of_module].pixel_data.pixel;
    return current_pixel;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_channel_data
//----------------------------------------------------------------------------------------------------------------------
std::vector<DataType> DataStore::get_channel_data(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    std::vector<DataType> pixelData = m_data.module_data[module].channel_data[channel_of_module].pixel_data.data;
    return pixelData;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_trigger_livetime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_trigger_livetime(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double trigger_livetime;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    trigger_livetime = m_data.module_data[module].channel_data[channel_of_module].pixel_data.trigger_livetime;
    return trigger_livetime;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_realtime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_realtime(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double realtime;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    realtime = m_data.module_data[module].channel_data[channel_of_module].pixel_data.realtime;
    return realtime;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_livetime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_livetime(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double livetime;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    livetime = m_data.module_data[module].channel_data[channel_of_module].pixel_data.livetime;
    return livetime;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_deadtime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_deadtime(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double deadtime;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    deadtime = m_data.module_data[module].channel_data[channel_of_module].pixel_data.deadtime;
    return deadtime;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_output_count_rate
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_output_count_rate(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double count_rate;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    count_rate = m_data.module_data[module].channel_data[channel_of_module].pixel_data.output_count_rate;
    return count_rate;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_input_count_rate
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_input_count_rate(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    double count_rate;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    count_rate = m_data.module_data[module].channel_data[channel_of_module].pixel_data.input_count_rate;
    return count_rate;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_events
//----------------------------------------------------------------------------------------------------------------------
unsigned long DataStore::get_events_in_run(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    unsigned long events_in_run;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    events_in_run = m_data.module_data[module].channel_data[channel_of_module].pixel_data.events_in_run;
    return events_in_run;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_triggers
//----------------------------------------------------------------------------------------------------------------------
unsigned long DataStore::get_triggers(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    unsigned long triggers;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    triggers = m_data.module_data[module].channel_data[channel_of_module].pixel_data.triggers;
    return triggers;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_outputs
//----------------------------------------------------------------------------------------------------------------------
unsigned long DataStore::get_outputs(int channel)
{
    yat::MutexLock scoped_lock(m_data_lock);
    unsigned long outputs;

    int module = to_module_and_channel(channel).first;
    int channel_of_module = to_module_and_channel(channel).second;

    outputs = m_data.module_data[module].channel_data[channel_of_module].pixel_data.outputs;
    return outputs;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_nb_modules
//----------------------------------------------------------------------------------------------------------------------
int DataStore::get_nb_modules()
{
    yat::MutexLock scoped_lock(m_data_lock);
    return m_nb_modules;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_nb_channels
//----------------------------------------------------------------------------------------------------------------------
int DataStore::get_nb_channels()
{
    yat::MutexLock scoped_lock(m_data_lock);
    return m_nb_channels;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_nb_pixels
//----------------------------------------------------------------------------------------------------------------------
int DataStore::get_nb_pixels()
{
    yat::MutexLock scoped_lock(m_data_lock);
    return m_nb_pixels;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::set_nb_pixels
//----------------------------------------------------------------------------------------------------------------------
void DataStore::set_nb_pixels(int nb_pixels)
{
    yat::MutexLock scoped_lock(m_data_lock);
    m_nb_pixels = nb_pixels;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_nb_bins
//----------------------------------------------------------------------------------------------------------------------
int DataStore::get_nb_bins()
{
    yat::MutexLock scoped_lock(m_data_lock);
    return m_nb_bins;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
bool DataStore::get_statistics_enabled(void)
{
    return m_is_statistics_enabled;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void DataStore::set_statistics_enabled(bool is_enabled)
{
    m_is_statistics_enabled = is_enabled;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::get_timebase
//----------------------------------------------------------------------------------------------------------------------
double DataStore::get_timebase()
{
    return m_timebase;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::set_timebase
//----------------------------------------------------------------------------------------------------------------------
void DataStore::set_timebase(double timebase)
{
    m_timebase = timebase;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_realtime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_realtime(double realtime)
{
    //DEBUG_STREAM << "DataStore::compute_realtime()" << endl;
    return realtime * m_timebase;
}
//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_livetime
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_livetime(double livetime)
{
    //DEBUG_STREAM << "DataStore::compute_livetime()" << endl;
    return livetime * m_timebase;
}
//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_input_count_rate
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_input_count_rate(unsigned long triggers, double livetime)
{
    //DEBUG_STREAM << "DataStore::compute_input_count_rate()" << endl;
    if (livetime != 0.0)
    {
        return double(triggers) / livetime;
    }
    return 1.0;
}
//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_output_count_rate
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_output_count_rate(unsigned long outputs, double realtime)
{
    //DEBUG_STREAM << "DataStore::compute_output_count_rate()" << endl;
    if (realtime != 0.0)
    {
        return double(outputs) / realtime;
    }
    return 1.0;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_deadtime (MCA))
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_deadtime(double ocr, double icr)
{
    //DEBUG_STREAM << "DataStore::compute_deadtime()" << endl;

    if (icr != 0.0)
    {
        return 100.0*((icr - ocr) / icr);
    }
    return 0.0;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::compute_deadtime (MAPPING))
//----------------------------------------------------------------------------------------------------------------------
double DataStore::compute_deadtime(unsigned long outputs, unsigned long triggers, double livetime, double realtime)
{
    //DEBUG_STREAM << "DataStore::compute_deadtime()" << endl;
    if (realtime != 0.0 && triggers != 0)
    {
        return (100.0 * (1 - (1.0 * double(outputs) * livetime / ( realtime * double(triggers)))));
    }
    return 0.0;
}


//----------------------------------------------------------------------------------------------------------------------
//- DataStore::subscribe
//----------------------------------------------------------------------------------------------------------------------
void DataStore::subscribe(Controller* observer)
{
    INFO_STREAM << "DataStore::subscribe("<<observer<<")" <<endl;
    yat::MutexLock scoped_lock(m_data_lock);
    m_controller = observer;
}

//----------------------------------------------------------------------------------------------------------------------
//- DataStore::notify
//----------------------------------------------------------------------------------------------------------------------
void DataStore::notify(int ichannel)
{
    DEBUG_STREAM << "DataStore::notify()" << endl;
    yat::MutexLock scoped_lock(m_data_lock);
    if(m_controller!=0)
        m_controller->update_data(ichannel);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::close_data(void)
{
    DEBUG_STREAM<<"DataStore::close_data() - [BEGIN]"<<std::endl;
    post(DATASTORE_CLOSE_DATA_MSG);
    DEBUG_STREAM<<"DataStore::close_data() - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::abort_data(void)
{
    DEBUG_STREAM<<"DataStore::abort_data() - [BEGIN]"<<std::endl;
    post(DATASTORE_ABORT_DATA_MSG);
    DEBUG_STREAM<<"DataStore::abort_data() - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::process_data(DataBufferContainer* map_buffer)
{
    DEBUG_STREAM<<"DataStore::process_data() - [BEGIN]"<<std::endl;
    set_state(Tango::RUNNING);
    post(DATASTORE_PROCESS_DATA_MSG, map_buffer, true);
    DEBUG_STREAM<<"DataStore::process_data() - [END]"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::on_process_data(DataBufferContainer& map_buffer)
{
    //DEBUG_STREAM<<"DataStore::on_process_data() - [BEGIN]"<<std::endl;

    try
    {
        if ((map_buffer.base()[0] != TAG_HEAD0) || (map_buffer.base()[1]!=TAG_HEAD1))
        {
            std::stringstream ss_msg;
            ss_msg<<"Missing TAG HEAD in buffer !"<<std::endl;
            ERROR_STREAM << "DataStore::on_process_data() - "<<ss_msg.str()<< std::endl;
            //stop_acquisition();//@@TODO 
            //on_alarm(ss_msg.str());
            set_state(Tango::FAULT);
            Tango::Except::throw_exception("XIA_ERROR",
                                           "Missing TAG HEAD in buffer !",
                                           "DataStore::on_process_data()");
        }

        if (map_buffer.base()[31] != 0x0000)
        {
            std::stringstream ss_msg;
            ss_msg<<"Expected 0 in word 31 of mapping buffer !"<<std::endl;
            ERROR_STREAM << "DataStore::on_process_data() - "<<ss_msg.str()<< std::endl;
            //stop_acquisition();//@@TODO 
            //on_alarm(ss_msg.str());
            set_state(Tango::FAULT);
            Tango::Except::throw_exception("XIA_ERROR",
                                           (ss_msg.str()).c_str(),
                                           "DataStore::on_process_data()");
        }

        // parse pixels
        const int startingPixel = WORD_TO_LONG((int)map_buffer.base()[9], (int)map_buffer.base()[10]);
        const int nbPixels = (int)map_buffer.base()[8];
        //INFO_STREAM << "\t- pixels to process : " << nbPixels <<std::endl;

        for (int pixel = 0; pixel < nbPixels; ++pixel)
        {
            //DEBUG_STREAM<<"------------------------------------"<<std::endl;
            //DEBUG_STREAM<<"---------- pixel = "<<pixel<<std::endl;
            //DEBUG_STREAM<<"------------------------------------"<<std::endl;            
            parse_data(map_buffer.module(), pixel, map_buffer.base());
        }

        //INFO_STREAM << "\t- processed pixels : " << startingPixel << " to "  << startingPixel + nbPixels - 1 << " (" <<nbPixels<<")"<<std::endl;
    }
    catch(const std::exception& ex)
    {
        ERROR_STREAM << "DataStore::process_data() - " << ex.what() << endl;
        //stop_acquisition();//@@TODO 
        //on_alarm(ex.what());
        set_state(Tango::FAULT);
        Tango::Except::throw_exception("XIA_ERROR",
                                       ex.what(),
                                       "DataStore::on_process_data()");
    }

    //DEBUG_STREAM<<"DataStore::process_data() - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::parse_data(int module, int pixel, DataType* map_buffer)
{
    //DEBUG_STREAM<<"DataStore::parse_data() - [BEGIN]"<<std::endl;

    unsigned long* pixel_data = map_buffer + BUFFER_HEADER_SIZE;

    // go to current pixel
    for (int i = 0; i < pixel; ++i)
    {
        const unsigned long pixelBlockSize = WORD_TO_LONG(pixel_data[6], pixel_data[7]);
        pixel_data += pixelBlockSize;
    }

    // check pixel buffer
    if ((pixel_data[0]!=TAG_DATA0) && (pixel_data[1]!=TAG_DATA1))
    {
        std::stringstream ss_msg;
        ss_msg<<"Missing TAG DATA in buffer for pixel : "<<pixel<<std::endl;
        ERROR_STREAM << "DataStore::parse_data() - "<<ss_msg.str()<< endl;
        //stop_acquisition();//@@TODO 
        //on_alarm(ss_msg.str());
        set_state(Tango::FAULT);
        Tango::Except::throw_exception("XIA_ERROR",
                                       (ss_msg.str()).c_str(),
                                       "DataStore::parse_data()");
    }

    const unsigned long the_pixel = WORD_TO_LONG(pixel_data[4], pixel_data[5]);
    //DEBUG_STREAM<<"\t- the_pixel = "<<the_pixel<<std::endl;
    const std::size_t length_of_pixData = WORD_TO_LONG(pixel_data[6], pixel_data[7]);

    // Get spectrum sizes
    // WORD 8/9/10/11 (XMAP_USER_MANUAL.pdf - 5.3.3.3 Mapping Mode 1 - page 70)
    // number of channels = ALWAYS 4 for XMAP board !  @@TODO what is the spec for Falcon ?
    unsigned long size[4];
    for (int channel = 0; channel < 4; channel++)
    {
        size[channel] = pixel_data[8+channel];
        //DEBUG_STREAM<<"\t- size channel["<<channel<<"] = "<<size[channel]<<std::endl;
    }

    //DEBUG_STREAM <<"\t- parse data - module  = "  <<module <<" - pixel   = "  <<the_pixel <<" - length  = "  <<length_of_pixData <<std::endl;

    // statistics for current pixel
    //DEBUG_STREAM << "\t- push statistics for the pixel ["<<the_pixel<<"] into DataStore" <<std::endl;
    for (int channel = 0; channel < 4; channel++)
    {
        //each channels statistics contains 8 WORD, that is why channel*8
        unsigned long realtime = WORD_TO_LONG(pixel_data[32+channel*8], pixel_data[33+channel*8]);
        unsigned long livetime = WORD_TO_LONG(pixel_data[34+channel*8], pixel_data[35+channel*8]);        
        unsigned long triggers  = WORD_TO_LONG(pixel_data[36+channel*8], pixel_data[37+channel*8]);
        unsigned long outputs   = WORD_TO_LONG(pixel_data[38+channel*8], pixel_data[39+channel*8]);

        PixelData pix_data;
        pix_data.triggers = triggers;
        pix_data.outputs = outputs;
        pix_data.realtime = static_cast<double>(realtime);
        pix_data.livetime = static_cast<double>(livetime);

        //push statistics into DataStore
        store_statistics(module,               //nb module
                         channel,              //numero of channel
                         the_pixel,            //numero of pixel
                         "MAPPING",
                         pix_data);
    }

    //datas for current pixel
    //DEBUG_STREAM << "\t- push buffer data for the pixel ["<<the_pixel<<"] into DataStore "<<endl;
    unsigned long* spectrum_data = pixel_data + PIXEL_HEADER_SIZE;
    for (int channel = 0; channel <4; ++channel)
    {
        store_data(module,                   //nb module
                   channel,                  //numero of channel
                   the_pixel,                    //numero of pixel
                   (DataType*)spectrum_data,
                   size[channel]
                   );
        spectrum_data += size[channel];
    }
    //m_current_pixel = the_pixel; 
    ////DEBUG_STREAM << "\t- The pixel ["<<m_current_pixel<<"] is parsed & stored." <<std::endl;
    //DEBUG_STREAM<<"DataStore::parse_data() - [END]"<<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::set_state(Tango::DevState state)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_state = state;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Tango::DevState DataStore::get_state(void)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        return m_state;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DataStore::set_status(const std::string& status)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        m_status.str("");
        m_status << status.c_str();
    }
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
std::string DataStore::get_status(void)
{
    {
        //- AutoLock the following
        yat::MutexLock scoped_lock(m_state_lock);
        return (m_status.str());
    }
}

//---------------------------
//- DataStore::process_message()
//---------------------------
void DataStore::process_message(yat::Message& msg) throw (Tango::DevFailed)
{
    DEBUG_STREAM << "DataStore::process_message(): entering... !" << endl;
    try
    {
        switch (msg.type())
        {
                //-----------------------------------------------------
            case yat::TASK_INIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM << "-> DataStore::TASK_INIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_EXIT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM <<"-> DataStore::TASK_EXIT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case yat::TASK_TIMEOUT:
            {
                INFO_STREAM<<" "<<std::endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
                INFO_STREAM <<"-> DataStore::TASK_TIMEOUT" << endl;
                INFO_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------------------------------------------------

            case yat::TASK_PERIODIC:
            {
                DEBUG_STREAM<<" "<<std::endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
                DEBUG_STREAM<<"-> DataStore::TASK_PERIODIC" << endl;
                DEBUG_STREAM<<" "<<std::endl;
                DEBUG_STREAM<<"--------------------------------------------"<<std::endl;
            }
                break;
                //-----------------------------------------------------
            case DATASTORE_PROCESS_DATA_MSG:
            {
                //INFO_STREAM<<" "<<std::endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                //INFO_STREAM <<"-> DataStore::DATASTORE_PROCESS_DATA_MSG" << endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                try
                {
                    yat::MutexLock scoped_lock(m_data_lock);
                    on_process_data(msg.get_data<DataBufferContainer>());
                }
                catch (Tango::DevFailed &df)
                {
                    ERROR_STREAM << df << endl;
                    set_state(Tango::FAULT);
                }
            }
                break;
                //-----------------------------------------------------
            case DATASTORE_CLOSE_DATA_MSG:
            {
                //INFO_STREAM<<" "<<std::endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                //INFO_STREAM <<"-> DataStore::DATASTORE_CLOSE_DATA_MSG" << endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                try
                {
                    yat::MutexLock scoped_lock(m_data_lock);
                    on_close();
                }
                catch (Tango::DevFailed &df)
                {
                    ERROR_STREAM << df << endl;
                    set_state(Tango::FAULT);
                }
            }
                break;
                //-----------------------------------------------------			
            case DATASTORE_ABORT_DATA_MSG:
            {
                //INFO_STREAM<<" "<<std::endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                //INFO_STREAM <<"-> DataStore::DATASTORE_ABORT_DATA_MSG" << endl;
                //INFO_STREAM<<"--------------------------------------------"<<std::endl;
                try
                {
                    yat::MutexLock scoped_lock(m_data_lock);
                    on_abort();
                }
                catch (Tango::DevFailed &df)
                {
                    ERROR_STREAM << df << endl;
                    set_state(Tango::FAULT);
                }
            }
                break;
                //-----------------------------------------------------		                
        }
    }
    catch (yat::Exception& ex)
    {
        //- TODO Error Handling
        ex.dump();
        std::stringstream error_msg("");
        error_msg << "Origin\t: " << ex.errors[0].origin << endl;
        error_msg << "Desc\t: " << ex.errors[0].desc << endl;
        error_msg << "Reason\t: " << ex.errors[0].reason << endl;
        set_state(Tango::FAULT);
        ERROR_STREAM << "Exception from - DataStore::process_message() : " << error_msg.str()<<endl;
        throw;
    }
}
/*------------------------------------------------------------------
 *	class:	DataBufferContainer
 *	description:	DataStore Factory
/------------------------------------------------------------------*/
DataBufferContainer::DataBufferContainer(int module, std::size_t buffer_length)
: m_module(module)
{
    m_buffer = new DataBuffer(buffer_length, true /*clear buffer*/);
}

//----------------------------------------------------------------------------------------------------------------------
//- DataBufferContainer::base
//----------------------------------------------------------------------------------------------------------------------
DataType* DataBufferContainer::base() const
{
    return m_buffer->base();
}

//----------------------------------------------------------------------------------------------------------------------
//- DataBufferContainer::module
//----------------------------------------------------------------------------------------------------------------------
int DataBufferContainer::module() const
{
    return m_module;
}


} // namespace 
