// MimAPI.cpp: implementation of the CMimAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimAPI.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
char CMimAPI::m_ErrorString[128]="";
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMimAPI::CMimAPI()
{
	CMimAPI::PreInitApi();
}
//-------------------------------------------------------------------------------------//
CMimAPI::CMimAPI(const CMimAPI &Api)
{
	CMimAPI::PreInitApi();
}
//-------------------------------------------------------------------------------------//
CMimAPI::~CMimAPI()
{

}
//-------------------------------------------------------------------------------------//
CMimAPI& CMimAPI::operator=(const CMimAPI &Api)
{
	if ( this == &Api ) { return *this; }
	CMimAPI::CloneApi(Api);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimAPI::PreInitApi()
{	
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
}
//-------------------------------------------------------------------------------------//
void CMimAPI::CloneApi(const CMimAPI &Api)
{
}
//-------------------------------------------------------------------------------------//
bool CMimAPI::CheckLicenses()
{
#ifdef MIM_LIB_USE
	E_iVision_ERRORS Err = E__OK;
	Err = iGetKeyState();
	if ( E_TRUE != Err )
	{
		::sprintf(m_ErrorString, "Mim Lib Check Key State Fault");
		return false; 
	}
	Err = iVisitingKey();
	if ( E__OK != Err )
	{
		::sprintf(m_ErrorString, "Mim Lib Check Visit Key Fault");
		return false; 
	}	
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimAPI::GetVersion(char strVersion[], size_t StrSize)
{
#ifdef MIM_LIB_USE
	char *pBuf = iGetiMatchVersion();
	::sprintf(strVersion, "Mim Lib-v%s", pBuf);	
	return true;
#endif//MIM_LIB_USE
	::sprintf(strVersion, "No Define MimLib");
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimAPI::InitialApi()
{
#ifdef MIM_LIB_USE
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
const char* CMimAPI::GetErrorString()
{
	return CMimAPI::m_ErrorString;
}
//-------------------------------------------------------------------------------------//