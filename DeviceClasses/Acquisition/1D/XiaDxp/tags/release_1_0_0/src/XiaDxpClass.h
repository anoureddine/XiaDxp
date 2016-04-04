//=============================================================================
//
// file :         XiaDxpClass.h
//
// description :  Include for the XiaDxpClass root class.
//                This class is the singleton class for
//                the XiaDxp device class.
//                It contains all properties and methods which the 
//                XiaDxp requires only once e.g. the commands.
//			
// project :      TANGO Device Server
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
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================

#ifndef _XIADXPCLASS_H
#define _XIADXPCLASS_H

#include <tango.h>
#include <XiaDxp.h>


namespace XiaDxp_ns
{//=====================================
//	Define classes for attributes
//=====================================
class streamNbAcqPerFileAttrib: public Tango::Attr
{
public:
	streamNbAcqPerFileAttrib():Attr("streamNbAcqPerFile", Tango::DEV_LONG, Tango::WRITE) {};
	~streamNbAcqPerFileAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_streamNbAcqPerFile(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
	{(static_cast<XiaDxp *>(dev))->write_streamNbAcqPerFile(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_streamNbAcqPerFile_allowed(ty);}
};

class streamNbDataPerAcqAttrib: public Tango::Attr
{
public:
	streamNbDataPerAcqAttrib():Attr("streamNbDataPerAcq", Tango::DEV_LONG, Tango::WRITE) {};
	~streamNbDataPerAcqAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_streamNbDataPerAcq(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
	{(static_cast<XiaDxp *>(dev))->write_streamNbDataPerAcq(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_streamNbDataPerAcq_allowed(ty);}
};

class streamTargetFileAttrib: public Tango::Attr
{
public:
	streamTargetFileAttrib():Attr("streamTargetFile", Tango::DEV_STRING, Tango::WRITE) {};
	~streamTargetFileAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_streamTargetFile(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
	{(static_cast<XiaDxp *>(dev))->write_streamTargetFile(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_streamTargetFile_allowed(ty);}
};

class streamTargetPathAttrib: public Tango::Attr
{
public:
	streamTargetPathAttrib():Attr("streamTargetPath", Tango::DEV_STRING, Tango::WRITE) {};
	~streamTargetPathAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_streamTargetPath(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
	{(static_cast<XiaDxp *>(dev))->write_streamTargetPath(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_streamTargetPath_allowed(ty);}
};

class streamTypeAttrib: public Tango::Attr
{
public:
	streamTypeAttrib():Attr("streamType", Tango::DEV_STRING, Tango::WRITE) {};
	~streamTypeAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_streamType(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
	{(static_cast<XiaDxp *>(dev))->write_streamType(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_streamType_allowed(ty);}
};

class dynamicRangeAttrib: public Tango::Attr
{
public:
	dynamicRangeAttrib():Attr("dynamicRange", Tango::DEV_DOUBLE, Tango::READ) {};
	~dynamicRangeAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_dynamicRange(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_dynamicRange_allowed(ty);}
};

class peakingTimeAttrib: public Tango::Attr
{
public:
	peakingTimeAttrib():Attr("peakingTime", Tango::DEV_DOUBLE, Tango::READ) {};
	~peakingTimeAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_peakingTime(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_peakingTime_allowed(ty);}
};

class currentConfigFileAttrib: public Tango::Attr
{
public:
	currentConfigFileAttrib():Attr("currentConfigFile", Tango::DEV_STRING, Tango::READ) {};
	~currentConfigFileAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_currentConfigFile(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_currentConfigFile_allowed(ty);}
};

class currentModeAttrib: public Tango::Attr
{
public:
	currentModeAttrib():Attr("currentMode", Tango::DEV_STRING, Tango::READ) {};
	~currentModeAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_currentMode(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_currentMode_allowed(ty);}
};

class currentAliasAttrib: public Tango::Attr
{
public:
	currentAliasAttrib():Attr("currentAlias", Tango::DEV_STRING, Tango::READ) {};
	~currentAliasAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_currentAlias(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_currentAlias_allowed(ty);}
};

class nbBinsAttrib: public Tango::Attr
{
public:
	nbBinsAttrib():Attr("nbBins", Tango::DEV_LONG, Tango::READ) {};
	~nbBinsAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_nbBins(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_nbBins_allowed(ty);}
};

class nbChannelsAttrib: public Tango::Attr
{
public:
	nbChannelsAttrib():Attr("nbChannels", Tango::DEV_LONG, Tango::READ) {};
	~nbChannelsAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_nbChannels(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_nbChannels_allowed(ty);}
};

class nbModulesAttrib: public Tango::Attr
{
public:
	nbModulesAttrib():Attr("nbModules", Tango::DEV_LONG, Tango::READ) {};
	~nbModulesAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<XiaDxp *>(dev))->read_nbModules(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<XiaDxp *>(dev))->is_nbModules_allowed(ty);}
};

//=========================================
//	Define classes for commands
//=========================================
class StreamResetIndexCmd : public Tango::Command
{
public:
	StreamResetIndexCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	StreamResetIndexCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~StreamResetIndexCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_StreamResetIndex_allowed(any);}
};



class RemoveRoisCmd : public Tango::Command
{
public:
	RemoveRoisCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	RemoveRoisCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~RemoveRoisCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_RemoveRois_allowed(any);}
};



class GetRoisCmd : public Tango::Command
{
public:
	GetRoisCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	GetRoisCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~GetRoisCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_GetRois_allowed(any);}
};



class SetRoisFromFileCmd : public Tango::Command
{
public:
	SetRoisFromFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SetRoisFromFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SetRoisFromFileCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_SetRoisFromFile_allowed(any);}
};



class SetRoisFromListCmd : public Tango::Command
{
public:
	SetRoisFromListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SetRoisFromListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SetRoisFromListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_SetRoisFromList_allowed(any);}
};



class SaveConfigFileCmd : public Tango::Command
{
public:
	SaveConfigFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SaveConfigFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SaveConfigFileCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_SaveConfigFile_allowed(any);}
};



class LoadConfigFileCmd : public Tango::Command
{
public:
	LoadConfigFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	LoadConfigFileCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~LoadConfigFileCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_LoadConfigFile_allowed(any);}
};



class StopCmd : public Tango::Command
{
public:
	StopCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	StopCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~StopCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_Stop_allowed(any);}
};



class SnapCmd : public Tango::Command
{
public:
	SnapCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SnapCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SnapCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XiaDxp *>(dev))->is_Snap_allowed(any);}
};



//
// The XiaDxpClass singleton definition
//

class
#ifdef WIN32
	__declspec(dllexport)
#endif
	XiaDxpClass : public Tango::DeviceClass
{
public:
//	properties member data

//	add your own data members here
//------------------------------------

public:
	Tango::DbData	cl_prop;
	Tango::DbData	cl_def_prop;
	Tango::DbData	dev_def_prop;

//	Method prototypes
	static XiaDxpClass *init(const char *);
	static XiaDxpClass *instance();
	~XiaDxpClass();
	Tango::DbDatum	get_class_property(string &);
	Tango::DbDatum	get_default_device_property(string &);
	Tango::DbDatum	get_default_class_property(string &);
	
protected:
	XiaDxpClass(string &);
	static XiaDxpClass *_instance;
	void command_factory();
	void get_class_property();
	void attribute_factory(vector<Tango::Attr *> &);
	void write_class_property();
	void set_default_property();
	string get_cvstag();
	string get_cvsroot();

private:
	void device_factory(const Tango::DevVarStringArray *);
};


}	//	namespace XiaDxp_ns

#endif // _XIADXPCLASS_H