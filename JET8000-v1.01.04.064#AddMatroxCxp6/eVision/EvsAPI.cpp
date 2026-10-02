// EvsAPI.cpp: implementation of the CEvsAPI class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsAPI.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
int  CEvsAPI::m_ErrorCode=0;
char CEvsAPI::m_ErrorString[128]="";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CEvsAPI::CEvsAPI()
{
	CEvsAPI::PreInitApi();
//	CEvsAPI::InitialApi();
}
//-------------------------------------------------------------------------------------//
CEvsAPI::CEvsAPI(const CEvsAPI &Api)
{
	CEvsAPI::PreInitApi();
//	CEvsAPI::CloneApi(Api);
}
//-------------------------------------------------------------------------------------//
CEvsAPI::~CEvsAPI()
{
}
//-------------------------------------------------------------------------------------//
CEvsAPI& CEvsAPI::operator=(const CEvsAPI &Api)
{
	if ( this == &Api ) { return *this; }
	CEvsAPI::CloneApi(Api);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsAPI::PreInitApi()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));	
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::InitialApi()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION		
		ESetTraceMode(E_TRACE_SILENT);
		ESetAngleUnit(E_ANGLE_UNIT_DEGREES);
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{
			Easy::SetAngleUnit(EAngleUnit_Degrees);
			return true;
		}
		catch(EException exc)
		{				
			m_ErrorCode = exc.GetError();
			sprintf(CEvsAPI::m_ErrorString, "%s", exc.What().c_str());
			return false;
		}		
	#endif		
#endif //EVISION_USE
	return true;
}
//-------------------------------------------------------------------------------------//
void CEvsAPI::CloneApi(const CEvsAPI &Api)
{
//	this->m_ErrorCode = Api.m_ErrorCode;
//	::strcpy(this->m_ErrorString, Api.m_ErrorString);
}
//-------------------------------------------------------------------------------------//
const char* CEvsAPI::GetErrorString()
{
	return CEvsAPI::m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::GetIsEVisionError()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		if (EGetError( ) != E_OK)
		{
			sprintf(CEvsAPI::m_ErrorString, "%s", EGetErrorText());
			EOk();
			return true;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		if ( CEvsAPI::m_ErrorCode != EError_Ok )
		{	return true;	}		
	#endif	
	return false;
#endif//EVISION_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::CheckLicenses()
{
	//return true;
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{
			CEvsAPI::m_ErrorCode = EError_Ok;			
		#if EVISION_MODE_OPEN_EVISION_VERSION>=EVISION_MODE_OPEN_EVISION_22_12_2_15123//EVISION_MODE_OPEN_EVISION_23_12_0_18439
			Preconfiguration::SelectLicensingModels(ELicensingModel::ELicensingModel_LegacyDongle);
		#endif//EVISION_MODE_OPEN_EVISION_VERSION
			Easy::CheckLicenses();
			return true;
		}
		catch(EException exc)
		{				
			m_ErrorCode = exc.GetError();
			sprintf(CEvsAPI::m_ErrorString, "%s", exc.What().c_str());
			return false;
		}			
	#endif	
	return false;
#endif //EVISION_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::SetTraceMode(int Mode)//for EVision
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION		
		ESetTraceMode((E_TRACE_MODE)Mode);////E_TRACE_SILENT
		if ( CEvsAPI::GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		return true;
	#endif
	return false;
#endif //EVISION_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::SetAngleUnit(int Unit)//設定角度單位
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		ESetAngleUnit((E_ANGLE_UNITS)Unit);//E_ANGLE_UNIT_DEGREES
		if ( CEvsAPI::GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{
			CEvsAPI::m_ErrorCode = EError_Ok;				
			Easy::SetAngleUnit((EAngleUnit)(Unit));//EAngleUnit
			return true;
		}
		catch(EException exc)
		{				
			m_ErrorCode = exc.GetError();
			sprintf(CEvsAPI::m_ErrorString, "%s", exc.What().c_str());
			return false;
		}		
	#endif		
#endif //EVISION_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsAPI::GetVersion(char strVersion[], size_t StrSize)
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		const char *Version = EGetVersion();
		const char Name[] = "eVision";
		size_t len  = ::strlen(Version);
		size_t len2 = ::strlen(Name);
		if ( (len+len2) < StrSize )
		{	::sprintf(strVersion, "%s-v%s", Name, Version);	}
		else if ( len < StrSize )
		{	::strcpy(strVersion, Version);	}
		else
		{	::memset(strVersion, 0x00, sizeof(StrSize)); }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{
			CEvsAPI::m_ErrorCode = EError_Ok;	
			const char Name[] = "Open eVision";			
			std::string str = Easy::GetVersion();//EAngleUnit
			size_t len = str.length();
			size_t len2 = ::strlen(Name);
			if ( (len+len2) < StrSize )
			{	::sprintf(strVersion, "%s-v%s", Name, str.c_str());	}
			else if ( len < StrSize )
			{	::strcpy(strVersion, str.c_str());	}
			else
			{	::memset(strVersion, 0x00, sizeof(StrSize)); }
			return true;
		}
		catch(EException exc)
		{				
			m_ErrorCode = exc.GetError();
			sprintf(CEvsAPI::m_ErrorString, "%s", exc.What().c_str());
			return false;
		}		
	#endif		
#endif //EVISION_USE
	return true;
}
//-------------------------------------------------------------------------------------//