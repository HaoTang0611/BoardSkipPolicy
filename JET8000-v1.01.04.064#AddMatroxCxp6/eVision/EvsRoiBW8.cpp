// EvsRoiBW8.cpp: implementation of the CEvsRoiBW8 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsRoiBW8.h"
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
CEvsRoiBW8::CEvsRoiBW8()
{
	CEvsRoiBW8::PreInitRoiBW8();
	CEvsRoiBW8::InitialRoiBW8();
}
//-------------------------------------------------------------------------------------//
CEvsRoiBW8::CEvsRoiBW8(const CEvsRoiBW8 &Roi)
{
	CEvsRoiBW8::PreInitRoiBW8();
	CEvsRoiBW8::CloneRoiBW8(Roi);
}
//-------------------------------------------------------------------------------------//
CEvsRoiBW8::~CEvsRoiBW8()
{
}
//-------------------------------------------------------------------------------------//
CEvsRoiBW8& CEvsRoiBW8::operator=(const CEvsRoiBW8 &Roi)
{
	if ( this == &Roi ) { return *this; }
	CEvsRoiBW8::CloneRoiBW8(Roi);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsRoiBW8::PreInitRoiBW8()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
	m_ErrorCode = 0;	
}
//-------------------------------------------------------------------------------------//
void CEvsRoiBW8::InitialRoiBW8()
{
}
//-------------------------------------------------------------------------------------//
void CEvsRoiBW8::CloneRoiBW8(const CEvsRoiBW8 &Roi)
{	
	this->m_ErrorCode = Roi.m_ErrorCode;
	::strcpy(m_ErrorString, Roi.m_ErrorString);
#ifdef EVISION_USE		
	this->m_RoiBW8 = Roi.m_RoiBW8;	
#endif//EVISION_USE
}
//-------------------------------------------------------------------------------------//
#ifdef EVISION_USE
EVS_ROI_BW8* CEvsRoiBW8::GetRoiBW8Ptr()
{
	return &m_RoiBW8;	
}
#endif//EVISION_USE
//-------------------------------------------------------------------------------------//
const char* CEvsRoiBW8::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsRoiBW8::GetIsEVisionError()
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
bool CEvsRoiBW8::Detach()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiBW8.Detach();
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;						
			this->m_RoiBW8.Detach();
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
bool CEvsRoiBW8::Attach(CEvsImageBW8 *Img)
{
#ifdef EVISION_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiBW8.Attach(Img->GetImageBW8Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_RoiBW8.Attach(Img->GetImageBW8Ptr());
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
bool CEvsRoiBW8::SetPlacement(int x, int y, int w, int h)
{	
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_RoiBW8.SetPlacement(x, y, w, h);
		if ( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_RoiBW8.SetPlacement(x, y, w, h);
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
