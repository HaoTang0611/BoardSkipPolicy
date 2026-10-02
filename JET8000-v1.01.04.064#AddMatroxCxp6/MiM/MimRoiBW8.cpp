// MimRoiBW8.cpp: implementation of the CMimRoiBW8 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimRoiBW8.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMimRoiBW8::CMimRoiBW8()
{
	CMimRoiBW8::PreInitRoiBW8();
	CMimRoiBW8::InitialRoiBW8();
}
//-------------------------------------------------------------------------------------//
CMimRoiBW8::CMimRoiBW8(const CMimRoiBW8 &Roi)
{
	CMimRoiBW8::PreInitRoiBW8();
	CMimRoiBW8::CloneRoiBW8(Roi);
}
//-------------------------------------------------------------------------------------//
CMimRoiBW8::~CMimRoiBW8()
{
#ifdef MIM_LIB_USE
	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
CMimRoiBW8& CMimRoiBW8::operator=(const CMimRoiBW8 &Roi)
{
	if ( this == &Roi ) { return *this; }
	CMimRoiBW8::CloneRoiBW8(Roi);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimRoiBW8::PreInitRoiBW8()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
#ifdef MIM_LIB_USE
	m_ImageBW8Ptr = NULL;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimRoiBW8::InitialRoiBW8()
{
#ifdef MIM_LIB_USE
	m_ImageBW8Ptr = NULL;
	m_RoiBase.OrgX = 0;
	m_RoiBase.OrgY = 0;
	m_RoiBase.Width = 0;
	m_RoiBase.Height = 0;
	
	m_MimErrCode = E__OK;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimRoiBW8::CloneRoiBW8(const CMimRoiBW8 &Roi)
{
	::strcpy(m_ErrorString, Roi.m_ErrorString);
#ifdef MIM_LIB_USE
	m_ImageBW8Ptr = Roi.m_ImageBW8Ptr;
	m_RoiBase = Roi.m_RoiBase;
	m_MimErrCode = Roi.m_MimErrCode;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
bool CMimRoiBW8::GetIsMimError()
{
#ifdef MIM_LIB_USE
	if ( E__OK != m_MimErrCode ) 
	{
		char *pBuf = iGetErrorText(m_MimErrCode);
		::strcpy(m_ErrorString, pBuf);
		return true;
	}
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
#ifdef MIM_LIB_USE
//-------------------------------------------------------------------------------------//
LONG_PTR CMimRoiBW8::GetRoiBW8Ptr()
{
	return m_ImageBW8Ptr;
}
//-------------------------------------------------------------------------------------//
mRect CMimRoiBW8::GetRoiRect()
{
	mRect rect;
	rect.top   = m_RoiBase.OrgY;
	rect.down  = m_RoiBase.OrgY + m_RoiBase.Height;
	rect.left  = m_RoiBase.OrgX;
	rect.right = m_RoiBase.OrgX + m_RoiBase.Width;
	return rect;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiBW8::CheckRoiBW8Ptr()
{
	if ( NULL == m_ImageBW8Ptr )
	{
		::sprintf(this->m_ErrorString, "Error, NULL == m_ImageBW8Ptr");
		return false;
	}
	m_MimErrCode = iImageIsNULL(m_ImageBW8Ptr);
	if ( E_NULL == m_MimErrCode ) 
	{
		char *pBuf = iGetErrorText(E_NULL);
		::strcpy(m_ErrorString, pBuf);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//MIM_LIB_USE
//-------------------------------------------------------------------------------------//
const char* CMimRoiBW8::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiBW8::Detach()
{
#ifdef MIM_LIB_USE	
	m_ImageBW8Ptr = NULL;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiBW8::Attach(CMimImageBW8 *Img)
{
#ifdef MIM_LIB_USE
	m_ImageBW8Ptr = NULL;
	if ( NULL == Img ) { return false; }
	m_ImageBW8Ptr = Img->GetImageBW8Ptr();
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiBW8::SetPlacement(int x, int y, int w, int h)
{
#ifdef MIM_LIB_USE
	m_RoiBase.OrgX = x;
	m_RoiBase.OrgY = y;
	m_RoiBase.Width = w;
	m_RoiBase.Height = h;	
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//