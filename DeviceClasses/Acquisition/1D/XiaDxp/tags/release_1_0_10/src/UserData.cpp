//=============================================================================
//
// file :        UserData.cpp
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

#include "UserData.h"
#include <PogoHelper.h>

namespace XiaDxp_ns
{
//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
DoubleUserData::DoubleUserData(const std::string& name)
: m_name(name),
m_value(0)
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
DoubleUserData::~DoubleUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevDouble& DoubleUserData::get_value()
{
    return m_value;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void DoubleUserData::set_value(Tango::DevDouble value)
{
    m_value = value;
}

/////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------	
ULongUserData::ULongUserData(const std::string& name)
: m_name(name),
m_value(0)
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
ULongUserData::~ULongUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevULong& ULongUserData::get_value()
{
    return m_value;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void ULongUserData::set_value(Tango::DevULong value)
{
    m_value = value;
}

/////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------	
LongUserData::LongUserData(const std::string& name)
: m_name(name),
m_value(0)
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
LongUserData::~LongUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevLong& LongUserData::get_value()
{
    return m_value;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void LongUserData::set_value(Tango::DevLong value)
{
    m_value = value;
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
BooleanUserData::BooleanUserData(const std::string& name)
: m_name(name),
m_value(false)
{
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
BooleanUserData::~BooleanUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevBoolean& BooleanUserData::get_value()
{
    return m_value;
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
void BooleanUserData::set_value(Tango::DevBoolean value)
{
    m_value = value;
}
/////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------	
StringUserData::StringUserData(const std::string& name)
: m_name(name)
{
    CREATE_DEVSTRING_ATTRIBUTE(m_value, MAX_ATTRIBUTE_STRING_LENGTH);
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
StringUserData::~StringUserData()
{
    DELETE_DEVSTRING_ATTRIBUTE(m_value);
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevString* StringUserData::get_value()
{
    return m_value;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void StringUserData::set_value(std::string value)
{
    strcpy(*m_value, value.c_str());
}

/////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
ChannelUserData::ChannelUserData(const std::string& name, int nb_bins)
: m_name(name)
{
    m_data.resize(nb_bins);
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
ChannelUserData::~ChannelUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const std::vector<Tango::DevULong>& ChannelUserData::get_value()
{
    return m_data;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void ChannelUserData::set_value(std::vector<Tango::DevULong> value)
{
    m_data = value;
}

/////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
RoiUserData::RoiUserData(const std::string& name, int channel, int roi_num)
: m_name(name),
m_channel(channel),
m_roi_num(roi_num),
m_data(0)
{

}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
RoiUserData::~RoiUserData()
{
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const Tango::DevDouble& RoiUserData::get_value()
{
    return m_data;
}

//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
void RoiUserData::set_value(Tango::DevDouble value)
{
    m_data = value;
}


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const int& RoiUserData::get_channel()
{
    return m_channel;
}


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
const int& RoiUserData::get_roi_num()
{
    return m_roi_num;
}

/////////////////////////////////////////////////////////////////////////////////
} // namespace
