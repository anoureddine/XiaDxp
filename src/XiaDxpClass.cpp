static const char *ClassId    = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL: $";
static const char *HttpServer = "http://www.esrf.fr/computing/cs/tango/tango_doc/ds_doc/";
//+=============================================================================
//
// file :        XiaDxpClass.cpp
//
// description : C++ source for the XiaDxpClass. A singleton
//               class derived from DeviceClass. It implements the
//               command list and all properties and methods required
//               by the XiaDxp once per process.
//
// project :     TANGO Device Server
//
// $Author:  $
//
// $Revision:  $
//
// $Log:  $
//
// copyleft :   European Synchrotron Radiation Facility
//              BP 220, Grenoble 38043
//              FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


#include <tango.h>

#include <XiaDxp.h>
#include <XiaDxpClass.h>


//+----------------------------------------------------------------------------
/**
 *	Create XiaDxpClass singleton and return it in a C function for Python usage
 */
//+----------------------------------------------------------------------------
extern "C" {
#ifdef WIN32

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_XiaDxp_class(const char *name) {
		return XiaDxp_ns::XiaDxpClass::init(name);
	}
}


namespace XiaDxp_ns
{
//+----------------------------------------------------------------------------
//
// method : 		StreamResetIndexCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *StreamResetIndexCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "StreamResetIndexCmd::execute(): arrived" << endl;

	((static_cast<XiaDxp *>(device))->stream_reset_index());
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		GetRoisCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *GetRoisCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "GetRoisCmd::execute(): arrived" << endl;

	return insert((static_cast<XiaDxp *>(device))->get_rois());
}


//+----------------------------------------------------------------------------
//
// method : 		SetRoisFromListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *SetRoisFromListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "SetRoisFromListCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<XiaDxp *>(device))->set_rois_from_list(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		SetRoisFromFileCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *SetRoisFromFileCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "SetRoisFromFileCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<XiaDxp *>(device))->set_rois_from_file(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		RemoveRoisCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *RemoveRoisCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "RemoveRoisCmd::execute(): arrived" << endl;

	Tango::DevLong	argin;
	extract(in_any, argin);

	((static_cast<XiaDxp *>(device))->remove_rois(argin));
	return new CORBA::Any();
}



//+----------------------------------------------------------------------------
//
// method : 		SnapCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *SnapCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "SnapCmd::execute(): arrived" << endl;

	((static_cast<XiaDxp *>(device))->snap());
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		SaveConfigFileCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *SaveConfigFileCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "SaveConfigFileCmd::execute(): arrived" << endl;

	((static_cast<XiaDxp *>(device))->save_config_file());
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		StopCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *StopCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "StopCmd::execute(): arrived" << endl;

	((static_cast<XiaDxp *>(device))->stop());
	return new CORBA::Any();
}




//+----------------------------------------------------------------------------
//
// method : 		LoadConfigFileCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *LoadConfigFileCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "LoadConfigFileCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<XiaDxp *>(device))->load_config_file(argin));
	return new CORBA::Any();
}




//
//----------------------------------------------------------------
//	Initialize pointer for singleton pattern
//----------------------------------------------------------------
//
XiaDxpClass *XiaDxpClass::_instance = NULL;

//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::XiaDxpClass(string &s)
// 
// description : 	constructor for the XiaDxpClass
//
// in : - s : The class name
//
//-----------------------------------------------------------------------------
XiaDxpClass::XiaDxpClass(string &s):DeviceClass(s)
{

	cout2 << "Entering XiaDxpClass constructor" << endl;
	set_default_property();
	get_class_property();
	write_class_property();
	
	cout2 << "Leaving XiaDxpClass constructor" << endl;

}
//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::~XiaDxpClass()
// 
// description : 	destructor for the XiaDxpClass
//
//-----------------------------------------------------------------------------
XiaDxpClass::~XiaDxpClass()
{
	_instance = NULL;
}

//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::instance
// 
// description : 	Create the object if not already done. Otherwise, just
//			return a pointer to the object
//
// in : - name : The class name
//
//-----------------------------------------------------------------------------
XiaDxpClass *XiaDxpClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new XiaDxpClass(s);
		}
		catch (bad_alloc)
		{
			throw;
		}		
	}		
	return _instance;
}

XiaDxpClass *XiaDxpClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}

//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::command_factory
// 
// description : 	Create the command object(s) and store them in the 
//			command list
//
//-----------------------------------------------------------------------------
void XiaDxpClass::command_factory()
{
	command_list.push_back(new SnapCmd("Snap",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new StopCmd("Stop",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new LoadConfigFileCmd("LoadConfigFile",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new SaveConfigFileCmd("SaveConfigFile",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new SetRoisFromListCmd("SetRoisFromList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new SetRoisFromFileCmd("SetRoisFromFile",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new GetRoisCmd("GetRois",
		Tango::DEV_VOID, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new RemoveRoisCmd("RemoveRois",
		Tango::DEV_LONG, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new StreamResetIndexCmd("StreamResetIndex",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::EXPERT));

	//	add polling if any
	for (unsigned int i=0 ; i<command_list.size(); i++)
	{
	}
}

//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::get_class_property
// 
// description : 	Get the class property for specified name.
//
// in :		string	name : The property name
//
//+----------------------------------------------------------------------------
Tango::DbDatum XiaDxpClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::get_default_device_property()
// 
// description : 	Return the default value for device property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum XiaDxpClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::get_default_class_property()
// 
// description : 	Return the default value for class property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum XiaDxpClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::device_factory
// 
// description : 	Create the device object(s) and store them in the 
//			device list
//
// in :		Tango::DevVarStringArray *devlist_ptr : The device name list
//
//-----------------------------------------------------------------------------
void XiaDxpClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{

	//	Create all devices.(Automatic code generation)
	//-------------------------------------------------------------
	for (unsigned long i=0 ; i < devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
						
		// Create devices and add it into the device list
		//----------------------------------------------------
		device_list.push_back(new XiaDxp(this, (*devlist_ptr)[i]));							 

		// Export device to the outside world
		// Check before if database used.
		//---------------------------------------------
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(device_list.back());
		else
			export_device(device_list.back(), (*devlist_ptr)[i]);
	}
	//	End of Automatic code generation
	//-------------------------------------------------------------

}
//+----------------------------------------------------------------------------
//	Method: XiaDxpClass::attribute_factory(vector<Tango::Attr *> &att_list)
//-----------------------------------------------------------------------------
void XiaDxpClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	//	Attribute : nbModules
	nbModulesAttrib	*nb_modules = new nbModulesAttrib();
	Tango::UserDefaultAttrProp	nb_modules_prop;
	nb_modules_prop.set_unit(" ");
	nb_modules_prop.set_standard_unit(" ");
	nb_modules_prop.set_display_unit(" ");
	nb_modules_prop.set_format("%d");
	nb_modules_prop.set_description("Number of Xia board");
	nb_modules->set_default_properties(nb_modules_prop);
	att_list.push_back(nb_modules);

	//	Attribute : nbChannels
	nbChannelsAttrib	*nb_channels = new nbChannelsAttrib();
	Tango::UserDefaultAttrProp	nb_channels_prop;
	nb_channels_prop.set_unit(" ");
	nb_channels_prop.set_standard_unit(" ");
	nb_channels_prop.set_display_unit(" ");
	nb_channels_prop.set_format("%d");
	nb_channels_prop.set_description("Number of Channels");
	nb_channels->set_default_properties(nb_channels_prop);
	att_list.push_back(nb_channels);

	//	Attribute : nbBins
	nbBinsAttrib	*nb_bins = new nbBinsAttrib();
	Tango::UserDefaultAttrProp	nb_bins_prop;
	nb_bins_prop.set_unit(" ");
	nb_bins_prop.set_standard_unit(" ");
	nb_bins_prop.set_display_unit(" ");
	nb_bins_prop.set_description("Number of Bins for each inputs.<Br>\naka: mca_length");
	nb_bins->set_default_properties(nb_bins_prop);
	att_list.push_back(nb_bins);

	//	Attribute : currentAlias
	currentAliasAttrib	*current_alias = new currentAliasAttrib();
	Tango::UserDefaultAttrProp	current_alias_prop;
	current_alias_prop.set_unit(" ");
	current_alias_prop.set_standard_unit(" ");
	current_alias_prop.set_display_unit(" ");
	current_alias_prop.set_format("%s");
	current_alias_prop.set_description("Display the current Alias used to load the *.ini file");
	current_alias->set_default_properties(current_alias_prop);
	att_list.push_back(current_alias);

	//	Attribute : currentMode
	currentModeAttrib	*current_mode = new currentModeAttrib();
	Tango::UserDefaultAttrProp	current_mode_prop;
	current_mode_prop.set_unit(" ");
	current_mode_prop.set_standard_unit(" ");
	current_mode_prop.set_display_unit(" ");
	current_mode_prop.set_format("%s");
	current_mode_prop.set_description("Display the current Mode (MCA or MAPPING).");
	current_mode->set_default_properties(current_mode_prop);
	att_list.push_back(current_mode);

	//	Attribute : currentConfigFile
	currentConfigFileAttrib	*current_config_file = new currentConfigFileAttrib();
	Tango::UserDefaultAttrProp	current_config_file_prop;
	current_config_file_prop.set_unit(" ");
	current_config_file_prop.set_standard_unit(" ");
	current_config_file_prop.set_display_unit(" ");
	current_config_file_prop.set_format("%s");
	current_config_file_prop.set_description("Display the path+name of current loaded *.ini file ");
	current_config_file->set_default_properties(current_config_file_prop);
	att_list.push_back(current_config_file);

	//	Attribute : peakingTime
	peakingTimeAttrib	*peaking_time = new peakingTimeAttrib();
	Tango::UserDefaultAttrProp	peaking_time_prop;
	peaking_time_prop.set_unit("ms");
	peaking_time_prop.set_format("%6.2f");
	peaking_time_prop.set_description("Peaking time of the energy filter, specified in ms");
	peaking_time->set_default_properties(peaking_time_prop);
	att_list.push_back(peaking_time);

	//	Attribute : dynamicRange
	dynamicRangeAttrib	*dynamic_range = new dynamicRangeAttrib();
	Tango::UserDefaultAttrProp	dynamic_range_prop;
	dynamic_range_prop.set_unit("eV");
	dynamic_range_prop.set_format("%6.2f");
	dynamic_range_prop.set_description("Energy range corresponding to 40% of the total ADC range, specified in eV");
	dynamic_range->set_default_properties(dynamic_range_prop);
	att_list.push_back(dynamic_range);

	//	Attribute : streamType
	streamTypeAttrib	*stream_type = new streamTypeAttrib();
	Tango::UserDefaultAttrProp	stream_type_prop;
	stream_type_prop.set_description("Avalaible stream types :<br>\nNO_STREAM<br>\nLOG_STREAM	<br>\nCSV_STREAM	<br>\nNEXUS_STREAM<br>\n");
	stream_type->set_default_properties(stream_type_prop);
	stream_type->set_disp_level(Tango::EXPERT);
	att_list.push_back(stream_type);

	//	Attribute : streamTargetPath
	streamTargetPathAttrib	*stream_target_path = new streamTargetPathAttrib();
	Tango::UserDefaultAttrProp	stream_target_path_prop;
	stream_target_path_prop.set_unit(" ");
	stream_target_path_prop.set_standard_unit(" ");
	stream_target_path_prop.set_display_unit(" ");
	stream_target_path_prop.set_format("%s");
	stream_target_path_prop.set_description("Define the root path for generated Stream files.");
	stream_target_path->set_default_properties(stream_target_path_prop);
	stream_target_path->set_disp_level(Tango::EXPERT);
	att_list.push_back(stream_target_path);

	//	Attribute : streamTargetFile
	streamTargetFileAttrib	*stream_target_file = new streamTargetFileAttrib();
	Tango::UserDefaultAttrProp	stream_target_file_prop;
	stream_target_file_prop.set_description("Define the file name for generated Stream files.");
	stream_target_file->set_default_properties(stream_target_file_prop);
	stream_target_file->set_disp_level(Tango::EXPERT);
	att_list.push_back(stream_target_file);

	//	Attribute : streamNbDataPerAcq
	streamNbDataPerAcqAttrib	*stream_nb_data_per_acq = new streamNbDataPerAcqAttrib();
	Tango::UserDefaultAttrProp	stream_nb_data_per_acq_prop;
	stream_nb_data_per_acq_prop.set_unit(" ");
	stream_nb_data_per_acq_prop.set_standard_unit(" ");
	stream_nb_data_per_acq_prop.set_display_unit(" ");
	stream_nb_data_per_acq_prop.set_format("%d");
	stream_nb_data_per_acq_prop.set_description("Define the number of data per acquisition for each Stream file.");
	stream_nb_data_per_acq->set_default_properties(stream_nb_data_per_acq_prop);
	stream_nb_data_per_acq->set_disp_level(Tango::EXPERT);
	att_list.push_back(stream_nb_data_per_acq);

	//	Attribute : streamNbAcqPerFile
	streamNbAcqPerFileAttrib	*stream_nb_acq_per_file = new streamNbAcqPerFileAttrib();
	Tango::UserDefaultAttrProp	stream_nb_acq_per_file_prop;
	stream_nb_acq_per_file_prop.set_unit(" ");
	stream_nb_acq_per_file_prop.set_standard_unit(" ");
	stream_nb_acq_per_file_prop.set_display_unit(" ");
	stream_nb_acq_per_file_prop.set_format("%d");
	stream_nb_acq_per_file_prop.set_description("Define the number of acquisition for each Stream file.");
	stream_nb_acq_per_file->set_default_properties(stream_nb_acq_per_file_prop);
	stream_nb_acq_per_file->set_disp_level(Tango::EXPERT);
	att_list.push_back(stream_nb_acq_per_file);

	//	End of Automatic code generation
	//-------------------------------------------------------------
}






//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::get_class_property()
// 
// description : 	Read the class properties from database.
//
//-----------------------------------------------------------------------------
void XiaDxpClass::get_class_property()
{
	//	Initialize your default values here (if not done with  POGO).
	//------------------------------------------------------------------

	//	Read class properties from database.(Automatic code generation)
	//------------------------------------------------------------------

	//	Call database and extract values
	//--------------------------------------------
	if (Tango::Util::instance()->_UseDb==true)
		get_db_class()->get_property(cl_prop);
	Tango::DbDatum	def_prop;
	int	i = -1;


	//	End of Automatic code generation
	//------------------------------------------------------------------

}

//+----------------------------------------------------------------------------
//
// method : 	XiaDxpClass::set_default_property
// 
// description: Set default property (class and device) for wizard.
//              For each property, add to wizard property name and description
//              If default value has been set, add it to wizard property and
//              store it in a DbDatum.
//
//-----------------------------------------------------------------------------
void XiaDxpClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;

	vector<string>	vect_data;
	//	Set Default Class Properties
	//	Set Default Device Properties
	prop_name = "BoardType";
	prop_desc = "";
	prop_def  = "XMAP";
	vect_data.clear();
	vect_data.push_back("XMAP");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "BoardTimebase";
	prop_desc = "Timebase factor of the board. [in seconds]";
	prop_def  = "0.000000320";
	vect_data.clear();
	vect_data.push_back("0.000000320");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "ConfigurationFiles";
	prop_desc = "Define the list of Configuration \"*.INI\" files and their associated alias & mode.";
	prop_def  = "ALIAS;MODE;FILE_PATH_NAME";
	vect_data.clear();
	vect_data.push_back("ALIAS;MODE;FILE_PATH_NAME");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "RoisFiles";
	prop_desc = "Define the list of rois files \"*.txt\"  and their associated alias.";
	prop_def  = "ALIAS;FILE_PATH_NAME";
	vect_data.clear();
	vect_data.push_back("ALIAS;FILE_PATH_NAME");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "StreamItems";
	prop_desc = "Define the list of Items managed by the Streamer. (Nexus, CSV, ...)<BR>\nAvailables values are :<BR>\nTriggers<BR>\nOutputs<BR>\nIcr<BR>\nOcr<BR>\nRealTime<BR>\nLiveTime<BR>\nDeadTime<BR>\nChannel<BR>\n";
	prop_def  = "Triggers\nOutputs\nIcr\nOcr\nRealTime\nLiveTime\nDeadTime\nChannel";
	vect_data.clear();
	vect_data.push_back("Triggers");
	vect_data.push_back("Outputs");
	vect_data.push_back("Icr");
	vect_data.push_back("Ocr");
	vect_data.push_back("RealTime");
	vect_data.push_back("LiveTime");
	vect_data.push_back("DeadTime");
	vect_data.push_back("Channel");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedConfigurationAlias";
	prop_desc = "";
	prop_def  = "TO_BE_DEFINED";
	vect_data.clear();
	vect_data.push_back("TO_BE_DEFINED");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedRoisAlias";
	prop_desc = "";
	prop_def  = "TO_BE_DEFINED";
	vect_data.clear();
	vect_data.push_back("TO_BE_DEFINED");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedNumChannel";
	prop_desc = "";
	prop_def  = "-1";
	vect_data.clear();
	vect_data.push_back("-1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedPresetType";
	prop_desc = "Available for MCA Mode Only !";
	prop_def  = "FIXED_REAL";
	vect_data.clear();
	vect_data.push_back("FIXED_REAL");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedPresetValue";
	prop_desc = "Available for MCA Mode Only !";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedNbPixels";
	prop_desc = "Available for MAPPING Mode Only !";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedStreamType";
	prop_desc = "Define the type of Stream.<BR>\nAvailable types are :<BR>\nNO_STREAM<BR>\nLOG_STREAM<BR>\nCSV_STREAM<BR>\nNEXUS_STREAM<BR>";
	prop_def  = "LOG_STREAM";
	vect_data.clear();
	vect_data.push_back("LOG_STREAM");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedStreamTargetPath";
	prop_desc = "";
	prop_def  = "TO_BE_DEFINED";
	vect_data.clear();
	vect_data.push_back("TO_BE_DEFINED");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedStreamTargetFile";
	prop_desc = "";
	prop_def  = "TO_BE_DEFINED";
	vect_data.clear();
	vect_data.push_back("TO_BE_DEFINED");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedStreamNbDataPerAcq";
	prop_desc = "";
	prop_def  = "2048";
	vect_data.clear();
	vect_data.push_back("2048");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__MemorizedStreamNbAcqPerFile";
	prop_desc = "";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "__ExpertStreamWriteMode";
	prop_desc = "Applicable for StreamNexus Only !<BR>\nAvailable Values :<BR>\n- IMMEDIATE<BR>\n- SYNCHRONOUS<BR>";
	prop_def  = "IMMEDIATE";
	vect_data.clear();
	vect_data.push_back("IMMEDIATE");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

}
//+----------------------------------------------------------------------------
//
// method : 		XiaDxpClass::write_class_property
// 
// description : 	Set class description as property in database
//
//-----------------------------------------------------------------------------
void XiaDxpClass::write_class_property()
{
	//	First time, check if database used
	//--------------------------------------------
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("The XIA Digital X-ray Processor is a family of digital spectrometers using acquistion boards XMap/FalconX");
	description << str_desc;
	data.push_back(description);
		
	//	put cvs or svn location
	string	filename(classname);
	filename += "Class.cpp";
	
	// Create a string with the class ID to
	// get the string into the binary
	string	class_id(ClassId);
	
	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}
	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("Device_4Impl");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	//--------------------------------------------
	get_db_class()->put_property(data);
}

}	// namespace
