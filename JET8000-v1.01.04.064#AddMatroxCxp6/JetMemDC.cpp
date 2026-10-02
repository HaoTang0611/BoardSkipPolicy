// JetMemDC.cpp: implementation of the CJetMemDC class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetMemDC.h"
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
CJetMemDC::CJetMemDC()
{
	CJetMemDC::PreInitial();	
}
//-------------------------------------------------------------------------------------//
CJetMemDC::~CJetMemDC()
{	
	CJetMemDC::ReleaseMemDC();
}
//-------------------------------------------------------------------------------------//
void CJetMemDC::PreInitial()
{
	CJetMemDC::m_hBitmap = NULL;
}
//-------------------------------------------------------------------------------------//
void CJetMemDC::ReleaseMemDC()
{
	HDC hMemDC = CDC::m_hDC;
	this->Detach();
	if ( hMemDC != NULL )
	{	::DeleteDC(hMemDC); hMemDC = NULL; }
	if ( this->m_hBitmap!= NULL ) 
	{	::DeleteObject(this->m_hBitmap); this->m_hBitmap = NULL; }
}
//-------------------------------------------------------------------------------------//
bool CJetMemDC::CreateMemDC(CWnd *pWnd, COLORREF bkclr)
{	
	if ( pWnd == NULL ) { return false; }
	CClientDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( hDC == NULL ) { return false; }

	RECT Rect;
	pWnd->GetClientRect(&Rect);
	return CJetMemDC::CreateMemDC(hDC, Rect, TRUE, bkclr);
}
//-------------------------------------------------------------------------------------//
bool CJetMemDC::CreateMemDC(HWND hWnd, COLORREF bkclr)
{
	if ( ::IsWindow(hWnd) == FALSE ) { return false; }
	HDC hDC = ::GetDC(hWnd);
	if ( hDC == NULL ) { return false; }
	RECT Rect;
	::GetClientRect(hWnd, &Rect);
	
	if ( CJetMemDC::CreateMemDC(hDC, Rect, TRUE, bkclr) == false )
	{
		::ReleaseDC(hWnd, hDC);
		return false;
	}
	::ReleaseDC(hWnd, hDC);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemDC::CreateMemDC(HDC hDC, RECT &Rect, bool EraseBK, COLORREF bkclr)
{	
	CJetMemDC::ReleaseMemDC();

	HDC hMemDC = ::CreateCompatibleDC(hDC);	
	if ( hMemDC == NULL ) { return false; }

	CJetMemDC::m_hBitmap = ::CreateCompatibleBitmap(hDC, Rect.right, Rect.bottom);
	if ( CJetMemDC::m_hBitmap == NULL )
	{
		::DeleteDC(hMemDC); 
		hMemDC = NULL; 
		return false;
	}	

	::SelectObject(hMemDC, m_hBitmap);
	CDC::Attach(hMemDC);

	::SetBkColor(hMemDC, bkclr);
	if ( EraseBK == true )	
	{		
		this->FillSolidRect(&Rect, bkclr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//