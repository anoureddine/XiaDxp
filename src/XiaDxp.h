//=============================================================================
//
// file :        XiaDxp.h
//
// description : Include for the XiaDxp class.
//
// project :	
//
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// SVN only:
// $HeadURL: $
//
// CVS only:
// $Source:  $
// $Log:  $
//
// copyleft :    Synchrotron SOLEIL 
//               L'Orme des merisiers - Saint Aubin
//		 BP48 - 91192 Gif sur Yvette
//               FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================
#ifndef _XIADXP_H
#define _XIADXP_H

#include <tango.h>
//using namespace Tango;

//- YAT/YAT4TANGO
#include <yat/utils/Logging.h>
#include <yat4tango/PropertyHelper.h>
#include <yat4tango/InnerAppender.h>
#include <yat4tango/YatLogAdapter.h>
#include <yat4tango/DeviceInfo.h>
#include <yat/memory/UniquePtr.h>
#include <yat/threading/Mutex.h>
#include <yat/utils/StringTokenizer.h>
#include <yat/utils/String.h>
#include <yat/file/FileName.h>



#include "Controller.h"

/**
 * @author	$Author:  $
 * @version	$Revision:  $
 */

//	Add your own constant definitions here.
//-----------------------------------------------


namespace XiaDxp_ns
{

/**
 * Class Description:
 * The XIA Digital X-ray Processor is a family of digital spectrometers using acquistion boards XMap/FalconX
 */

/*
 *	Device States Description:
*  Tango::STANDBY :
*  Tango::FAULT :
*  Tango::INIT :
*  Tango::RUNNING :
*  Tango::DISABLE :
*  Tango::OFF :
 */


class XiaDxp : public Tango::Device_4Impl
{
public:
    //	Add your own data members here
    //-----------------------------------------


    //	Here is the Start of the automatic code generation part
    //-------------------------------------------------------------	
    /**
     *	@name attributes
     *	Attribute member data.
     */
    //@{
		Tango::DevLong	*attr_nbModules_read;
		Tango::DevLong	*attr_nbChannels_read;
		Tango::DevLong	*attr_nbBins_read;
		Tango::DevString	*attr_currentAlias_read;
		Tango::DevString	*attr_currentMode_read;
		Tango::DevString	*attr_currentConfigFile_read;
		Tango::DevDouble	*attr_peakingTime_read;
		Tango::DevDouble	*attr_dynamicRange_read;
		Tango::DevString	attr_streamType_write;
		Tango::DevString	attr_streamTargetPath_write;
		Tango::DevString	attr_streamTargetFile_write;
		Tango::DevLong	attr_streamNbDataPerAcq_write;
		Tango::DevLong	attr_streamNbAcqPerFile_write;
//@}

    /**
     * @name Device properties
     * Device properties member data.
     */
    //@{
/**
 *	
 */
	string	boardType;
/**
 *	Timebase factor of the board. [in seconds]
 */
	Tango::DevDouble	boardTimebase;
/**
 *	Define the list of Configuration "*.INI" files and their associated alias & mode.
 */
	vector<string>	configurationFiles;
/**
 *	Define the list of rois files "*.txt"  and their associated alias.
 */
	vector<string>	roisFiles;
/**
 *	Define the list of Items managed by the Streamer. (Nexus, CSV, ...)<BR>
 *	Availables values are :<BR>
 *	Triggers<BR>
 *	Outputs<BR>
 *	Icr<BR>
 *	Ocr<BR>
 *	RealTime<BR>
 *	LiveTime<BR>
 *	DeadTime<BR>
 *	Channel<BR>
 *	
 */
	vector<string>	streamItems;
/**
 *	
 */
	string	spoolID;
/**
 *	
 */
	string	__MemorizedConfigurationAlias;
/**
 *	
 */
	string	__MemorizedRoisAlias;
/**
 *	
 */
	Tango::DevLong	__MemorizedNumChannel;
/**
 *	Available for MCA Mode Only !
 */
	string	__MemorizedPresetType;
/**
 *	Available for MCA Mode Only !
 */
	Tango::DevDouble	__MemorizedPresetValue;
/**
 *	Available for MCA Mode Only !
 */
	Tango::DevBoolean	__MemorizedAccumulate;
/**
 *	Define the type of Stream.<BR>
 *	Available types are :<BR>
 *	NO_STREAM<BR>
 *	LOG_STREAM<BR>
 *	CSV_STREAM<BR>
 *	NEXUS_STREAM<BR>
 */
	string	__MemorizedStreamType;
/**
 *	
 */
	string	__MemorizedStreamTargetPath;
/**
 *	
 */
	string	__MemorizedStreamTargetFile;
/**
 *	
 */
	Tango::DevLong	__MemorizedStreamNbDataPerAcq;
/**
 *	
 */
	Tango::DevLong	__MemorizedStreamNbAcqPerFile;
/**
 *	Applicable for StreamNexus Only !<BR>
 *	Available Values :<BR>
 *	- IMMEDIATE<BR>
 *	- SYNCHRONOUS<BR>
 */
	string	__ExpertStreamWriteMode;
//@}

    /**
     *	@name Device properties
     *	Device property member data.
     */
    //@{
    //@}

    /**@name Constructors
     * Miscellaneous constructors */
    //@{
    /**
     * Constructs a newly allocated Command object.
     *
     *	@param cl	Class.
     *	@param s 	Device Name
     */
    XiaDxp(Tango::DeviceClass *cl, string &s);
    /**
     * Constructs a newly allocated Command object.
     *
     *	@param cl	Class.
     *	@param s 	Device Name
     */
    XiaDxp(Tango::DeviceClass *cl, const char *s);
    /**
     * Constructs a newly allocated Command object.
     *
     *	@param cl	Class.
     *	@param s 	Device name
     *	@param d	Device description.
     */
    XiaDxp(Tango::DeviceClass *cl, const char *s, const char *d);
    //@}

    /**@name Destructor
     * Only one destructor is defined for this class */
    //@{
        /**
     * The object destructor.
     */
    ~XiaDxp()
    {
        delete_device();
    };
    /**
     *	will be called at device destruction or at init command.
     */
    void delete_device();
    //@}


    /**@name Miscellaneous methods */
    //@{
    /**
     *	Initialize the device
     */
    virtual void init_device();
    /**
     *	Always executed method before execution command method.
     */
    virtual void always_executed_hook();

//@}

/**
 * @name XiaDxp methods prototypes
 */

//@{
/**
 *	Hardware acquisition for attributes.
 */
	virtual void read_attr_hardware(vector<long> &attr_list);
/**
 *	Extract real attribute values for nbModules acquisition result.
 */
	virtual void read_nbModules(Tango::Attribute &attr);
/**
 *	Extract real attribute values for nbChannels acquisition result.
 */
	virtual void read_nbChannels(Tango::Attribute &attr);
/**
 *	Extract real attribute values for nbBins acquisition result.
 */
	virtual void read_nbBins(Tango::Attribute &attr);
/**
 *	Extract real attribute values for currentAlias acquisition result.
 */
	virtual void read_currentAlias(Tango::Attribute &attr);
/**
 *	Extract real attribute values for currentMode acquisition result.
 */
	virtual void read_currentMode(Tango::Attribute &attr);
/**
 *	Extract real attribute values for currentConfigFile acquisition result.
 */
	virtual void read_currentConfigFile(Tango::Attribute &attr);
/**
 *	Extract real attribute values for peakingTime acquisition result.
 */
	virtual void read_peakingTime(Tango::Attribute &attr);
/**
 *	Extract real attribute values for dynamicRange acquisition result.
 */
	virtual void read_dynamicRange(Tango::Attribute &attr);
/**
 *	Extract real attribute values for streamType acquisition result.
 */
	virtual void read_streamType(Tango::Attribute &attr);
/**
 *	Write streamType attribute values to hardware.
 */
	virtual void write_streamType(Tango::WAttribute &attr);
/**
 *	Extract real attribute values for streamTargetPath acquisition result.
 */
	virtual void read_streamTargetPath(Tango::Attribute &attr);
/**
 *	Write streamTargetPath attribute values to hardware.
 */
	virtual void write_streamTargetPath(Tango::WAttribute &attr);
/**
 *	Extract real attribute values for streamTargetFile acquisition result.
 */
	virtual void read_streamTargetFile(Tango::Attribute &attr);
/**
 *	Write streamTargetFile attribute values to hardware.
 */
	virtual void write_streamTargetFile(Tango::WAttribute &attr);
/**
 *	Extract real attribute values for streamNbDataPerAcq acquisition result.
 */
	virtual void read_streamNbDataPerAcq(Tango::Attribute &attr);
/**
 *	Write streamNbDataPerAcq attribute values to hardware.
 */
	virtual void write_streamNbDataPerAcq(Tango::WAttribute &attr);
/**
 *	Extract real attribute values for streamNbAcqPerFile acquisition result.
 */
	virtual void read_streamNbAcqPerFile(Tango::Attribute &attr);
/**
 *	Write streamNbAcqPerFile attribute values to hardware.
 */
	virtual void write_streamNbAcqPerFile(Tango::WAttribute &attr);
/**
 *	Read/Write allowed for nbModules attribute.
 */
	virtual bool is_nbModules_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for nbChannels attribute.
 */
	virtual bool is_nbChannels_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for nbBins attribute.
 */
	virtual bool is_nbBins_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for currentAlias attribute.
 */
	virtual bool is_currentAlias_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for currentMode attribute.
 */
	virtual bool is_currentMode_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for currentConfigFile attribute.
 */
	virtual bool is_currentConfigFile_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for peakingTime attribute.
 */
	virtual bool is_peakingTime_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for dynamicRange attribute.
 */
	virtual bool is_dynamicRange_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for streamType attribute.
 */
	virtual bool is_streamType_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for streamTargetPath attribute.
 */
	virtual bool is_streamTargetPath_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for streamTargetFile attribute.
 */
	virtual bool is_streamTargetFile_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for streamNbDataPerAcq attribute.
 */
	virtual bool is_streamNbDataPerAcq_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for streamNbAcqPerFile attribute.
 */
	virtual bool is_streamNbAcqPerFile_allowed(Tango::AttReqType type);
/**
 *	Execution allowed for Snap command.
 */
	virtual bool is_Snap_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for Stop command.
 */
	virtual bool is_Stop_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for LoadConfigFile command.
 */
	virtual bool is_LoadConfigFile_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for SaveConfigFile command.
 */
	virtual bool is_SaveConfigFile_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for SetRoisFromList command.
 */
	virtual bool is_SetRoisFromList_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for SetRoisFromFile command.
 */
	virtual bool is_SetRoisFromFile_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetRois command.
 */
	virtual bool is_GetRois_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for RemoveRois command.
 */
	virtual bool is_RemoveRois_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for StreamResetIndex command.
 */
	virtual bool is_StreamResetIndex_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetDataStreams command.
 */
	virtual bool is_GetDataStreams_allowed(const CORBA::Any &any);
/**
 * This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *	@return	State Code
 *	@exception DevFailed
 */
	virtual Tango::DevState	dev_state();
/**
 * 
 *	@exception DevFailed
 */
	void	snap();
/**
 * 
 *	@exception DevFailed
 */
	void	stop();
/**
 * Load a new config file (.ini)<br>
 *	This will download firmware and initialize the boards/modules in the system<br>
 *	@param	argin	
 *	@exception DevFailed
 */
	void	load_config_file(Tango::DevString);
/**
 * Config file (.ini) can be updated at any time using this command.<br>
 *	@exception DevFailed
 */
	void	save_config_file();
/**
 * ....<br>
 *	@param	argin	
 *	@exception DevFailed
 */
	void	set_rois_from_list(const Tango::DevVarStringArray *);
/**
 * ....<br>
 *	@param	argin	
 *	@exception DevFailed
 */
	void	set_rois_from_file(Tango::DevString);
/**
 * ....<br>
 *	...<br>
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_rois();
/**
 * Remove all Rois for the selected channel.<br>
 *	@param	argin	
 *	@exception DevFailed
 */
	void	remove_rois(Tango::DevLong);
/**
 * Reset the stream (Nexus) buffer index to 1.
 *	@exception DevFailed
 */
	void	stream_reset_index();
/**
 * Returns the flyscan data streams associated with this device.
 *	@return	
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_data_streams();

/**
 *	Read the device properties from database
 */
	 void get_device_property();
//@}

    //	Here is the end of the automatic code generation part
    //-------------------------------------------------------------	
    //- return the Controller object
    yat::SharedPtr<Controller> get_ctrl();
    
    //- return true if the device is correctly initialized in init_device
    bool is_device_initialized();    
protected:
    //	Add your own data members here
    //-----------------------------------------
    void set_rois_from_string(const std::string& input);

    struct tModeAndFile
    {
        std::string mode;
        std::string file;
    };
    
    ConfigLoader m_conf;    

    //- state & status stuff
    bool m_is_device_initialized;
    std::stringstream m_status_message;

    /* object used to control the acquisition & to get data buffers */
    yat::SharedPtr<Controller> m_controller;

    std::map<std::string, tModeAndFile>  m_map_alias_configuration_files;
    std::map<std::string, std::string>  m_map_alias_rois_files;
    std::vector<std::string>        m_rois_per_channel;
    bool m_is_first_rois_call;
    
};

} // namespace_ns

#endif	// _XIADXP_H
