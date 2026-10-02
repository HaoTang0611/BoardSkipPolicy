// MimRoiC24.cpp: implementation of the CMimRoiC24 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimRoiC24.h"
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
CMimRoiC24::CMimRoiC24()
{
	CMimRoiC24::PreInitRoiC24();
	CMimRoiC24::InitialRoiC24();
}
//-------------------------------------------------------------------------------------//
CMimRoiC24::CMimRoiC24(const CMimRoiC24 &Roi)
{
	CMimRoiC24::PreInitRoiC24();
	CMimRoiC24::CloneRoiC24(Roi);
}
//-------------------------------------------------------------------------------------//
CMimRoiC24::~CMimRoiC24()
{
#ifdef MIM_LIB_USE
	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
CMimRoiC24& CMimRoiC24::operator=(const CMimRoiC24 &Roi)
{
	if ( this == &Roi ) { return *this; }
	CMimRoiC24::CloneRoiC24(Roi);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimRoiC24::PreInitRoiC24()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
#ifdef MIM_LIB_USE
	m_ImageC24Ptr = NULL;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimRoiC24::InitialRoiC24()
{
#ifdef MIM_LIB_USE
	m_ImageC24Ptr = NULL;
	m_RoiBase.OrgX = 0;
	m_RoiBase.OrgY = 0;
	m_RoiBase.Width = 0;
	m_RoiBase.Height = 0;
	
	m_MimErrCode = E__OK;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimRoiC24::CloneRoiC24(const CMimRoiC24 &Roi)
{
	::strcpy(m_ErrorString, Roi.m_ErrorString);
#ifdef MIM_LIB_USE
	m_ImageC24Ptr = Roi.m_ImageC24Ptr;
	m_RoiBase = Roi.m_RoiBase;
	m_MimErrCode = Roi.m_MimErrCode;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
bool CMimRoiC24::GetIsMimError()
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
LONG_PTR CMimRoiC24::GetRoiC24Ptr()
{
	return m_ImageC24Ptr;
}
//-------------------------------------------------------------------------------------//
mRect CMimRoiC24::GetRoiRect()
{
	mRect rect;
	rect.top   = m_RoiBase.OrgY;
	rect.down  = m_RoiBase.OrgY + m_RoiBase.Height;
	rect.left  = m_RoiBase.OrgX;
	rect.right = m_RoiBase.OrgX + m_RoiBase.Width;
	return rect;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiC24::CheckRoiC24Ptr()
{
	if ( NULL == m_ImageC24Ptr )
	{
		::sprintf(this->m_ErrorString, "Error, NULL == m_ImageC24Ptr");
		return false;
	}
	m_MimErrCode = iImageIsNULL(m_ImageC24Ptr);
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
const char* CMimRoiC24::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiC24::Detach()
{
#ifdef MIM_LIB_USE	
	m_ImageC24Ptr = NULL;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiC24::Attach(CMimImageC24 *Img)
{
#ifdef MIM_LIB_USE
	m_ImageC24Ptr = NULL;
	if ( NULL == Img ) { return false; }
	m_ImageC24Ptr = Img->GetImageC24Ptr();
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimRoiC24::SetPlacement(int x, int y, int w, int h)
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