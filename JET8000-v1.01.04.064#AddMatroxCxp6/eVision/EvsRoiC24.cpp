// EvsRoiC24.cpp: implementation of the CEvsRoiC24 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsRoiC24.h"
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CEvsRoiC24::CEvsRoiC24()
{
	CEvsRoiC24::PreInitRoiC24();
	CEvsRoiC24::InitialRoiC24();
}
//-------------------------------------------------------------------------------------//
CEvsRoiC24::CEvsRoiC24(const CEvsRoiC24 &Roi)
{
	CEvsRoiC24::PreInitRoiC24();
	CEvsRoiC24::CloneRoiC24(Roi);
}
//-------------------------------------------------------------------------------------//
CEvsRoiC24::~CEvsRoiC24()
{
}
//-------------------------------------------------------------------------------------//
CEvsRoiC24& CEvsRoiC24::operator=(const CEvsRoiC24 &Roi)
{
	if ( this == &Roi ) { return *this; }
	CEvsRoiC24::CloneRoiC24(Roi);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsRoiC24::PreInitRoiC24()
{
	m_ErrorCode = 0;
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
}
//-------------------------------------------------------------------------------------//
void CEvsRoiC24::InitialRoiC24()
{
}
//-------------------------------------------------------------------------------------//
void CEvsRoiC24::CloneRoiC24(const CEvsRoiC24 &Roi)
{
	this->m_ErrorCode = Roi.m_ErrorCode;
	::strcpy(m_ErrorString, Roi.m_ErrorString);
#ifdef EVISION_USE		
	this->m_RoiC24 = Roi.m_RoiC24;
#endif //EVISION_USE	
}
//-------------------------------------------------------------------------------------//
#ifdef EVISION_USE
EVS_ROI_C24* CEvsRoiC24::GetRoiC24Ptr()
{
	return &m_RoiC24;	
}
#endif//EVISION_USE
//-------------------------------------------------------------------------------------//
const char* CEvsRoiC24::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsRoiC24::GetIsEVisionError()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		if (EGetError( ) != E_OK)
		{
			sprintf(this->m_ErrorString, "%s", EGetErrorText());
			EOk();
			return true;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		if ( this->m_ErrorCode != EError_Ok )
		{	return true;	}		
	#endif	
	return false;
#endif//EVISION_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsRoiC24::Detach()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiC24.Detach();
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;						
			this->m_RoiC24.Detach();
			return true;	
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}	
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEvsRoiC24::Attach(CEvsImageC24 *Img)
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiC24.Attach(Img->GetImageC24Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;						
			this->m_RoiC24.Attach(Img->GetImageC24Ptr());
			return true;	
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}	
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEvsRoiC24::SetPlacement(int x, int y, int w, int h)
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiC24.SetPlacement(x, y, w, h);
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_RoiC24.SetPlacement(x, y, w, h);
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
