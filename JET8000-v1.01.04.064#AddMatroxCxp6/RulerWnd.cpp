// RulerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "RulerWnd.h"
#include "InputBoxWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define WM_CHANGE_RULER_SCALE  WM_USER+1
//-------------------------------------------------------------------------------------//
CRulerWnd RulerWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CRulerWnd dialog
//-------------------------------------------------------------------------------------//
CRulerWnd::CRulerWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CRulerWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRulerWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_BkColor = 0xFFFFFF;
	this->m_RulerScaleX = 248;
	this->m_RulerScaleY = 248;
}
//-------------------------------------------------------------------------------------//
void CRulerWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRulerWnd)
	DDX_Control(pDX, RULER_RULER_WND, m_RulerWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CRulerWnd, CDialog)
	//{{AFX_MSG_MAP(CRulerWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CRulerWnd message handlers
//-------------------------------------------------------------------------------------//
void CRulerWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( this->m_RulerWnd.GetSafeHwnd() != NULL )
	{
		int  MarginX=0, MarginY=0;
		RECT WndRect;
		this->m_RulerWnd.GetWindowRect(&WndRect);				
		this->ScreenToClient(&WndRect);
		WndRect.left = 0+MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = 0+MarginY;
		WndRect.bottom = cy-MarginY;
		this->m_RulerWnd.MoveWindow(&WndRect);

		this->m_RulerWnd.GetClientRect(&m_RulerWndRect);
		this->m_RulerWndMemDC.CreateMemDC(&m_RulerWnd, m_BkColor);
		this->m_RulerWndCP.x = (m_RulerWndRect.left+m_RulerWndRect.right)/2;
		this->m_RulerWndCP.y = (m_RulerWndRect.top+m_RulerWndRect.bottom)/2;
		
		HDC hDC = this->m_RulerWndMemDC.GetSafeHdc();
		this->DrawRulerImage(hDC, false);		
		this->RedrawWnd();
	}
}
//-------------------------------------------------------------------------------------//
void CRulerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CRulerWnd::RedrawWnd()
{	
	CClientDC dc(&m_RulerWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC MemDC = this->m_RulerWndMemDC.GetSafeHdc();
	if ( MemDC == NULL ) { return; }

	//::IntersectClipRect(hDC, m_ImageViewRect.left, m_ImageViewRect.top, m_ImageViewRect.right, m_ImageViewRect.bottom);
	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	//this->DrawRulerImage(MemDC);		

	::BitBlt(hDC, 0, 0, m_RulerWndRect.right, m_RulerWndRect.bottom, MemDC, 0, 0, SRCCOPY ); 	
//	this->DrawRulerImage(hDC);
	
}
//-------------------------------------------------------------------------------------//
void CRulerWnd::DrawRulerImage(HDC hDC, bool bEraseBK)
{
	if ( hDC == NULL ) { return; }
	int i=0;
	int strLen = 0;
	int Offset = 0;	

	CString str;
	double  RulerScaleX = this->m_RulerScaleX;//1個pixel為多少個um
	double  RulerScaleY = this->m_RulerScaleY;
	const int WndSizeW = m_RulerWndRect.right-m_RulerWndRect.left;
	const int WndSizeH = m_RulerWndRect.bottom-m_RulerWndRect.top;
	const double WndSizeWum = (WndSizeW*RulerScaleX);
	const double WndSizeHum = (WndSizeH*RulerScaleY);
	const double DrawMarginW = 10000;//10 mm
	const double DrawMarginH = 1000;//1 mm
	int          MaxUint=1;
	double       mmUnit = 0;
	double       PixelUnit = 0;
	double       DrawMinX=0, DrawMinY=0, DrawMaxX=0, DrawMaxY=0;
	int          LongLine = 0;
	int          MidLine  = 0;
	int          ShortLine = 0;
	POINT   LinePt1={0}, LinePt2={0};
	RECT    DrawRect = this->m_RulerWndRect;

	if ( true == bEraseBK )
	{
		HBRUSH hBrush=::CreateSolidBrush(m_BkColor);
		::FillRect(hDC, &DrawRect, hBrush);
		::DeleteObject(hBrush); hBrush=NULL;
	}

	DrawMinX = DrawMarginW;
	DrawMinY = DrawMarginH;
	DrawMaxX = WndSizeWum-DrawMarginW;
	DrawMaxY = WndSizeHum-DrawMarginH;

	MaxUint = (int)((DrawMaxX-DrawMinX)/10000);//base on cm
	DrawMaxX = MaxUint*10000+DrawMinX;

	MaxUint = MaxUint*10;//cm to mm
	//繪製公制
	LongLine = (int)(10000.0/RulerScaleY);
	MidLine  = (int)(8000.0/RulerScaleY);
	ShortLine = (int)(5000.0/RulerScaleY);

	LinePt1.y = (int)(DrawMinY/RulerScaleY);
	LinePt1.x = (int)(DrawMinX/RulerScaleX)/2;	
	str.Format(_T("%s"), _T("CM"));
	strLen = str.GetLength();
	Offset = strLen*5;
	::TextOut(hDC, LinePt1.x-Offset, LinePt1.y, str, strLen);
	for ( i=0; i<=MaxUint; i++ )
	{
		mmUnit = DrawMinX+(i*1000.0);//每一小格多少um
		LinePt2.x = LinePt1.x = (int)(mmUnit/RulerScaleX);

		LinePt1.y = (int)(DrawMinY/RulerScaleY);
		if ( i%10 == 0 )
		{	LinePt2.y = LinePt1.y+LongLine;	}
		else if ( i%5 == 0 )
		{	LinePt2.y = LinePt1.y+MidLine;	}
		else
		{	LinePt2.y = LinePt1.y+ShortLine;	}

		::MoveToEx(hDC, LinePt1.x, LinePt1.y, NULL);
		::LineTo(hDC, LinePt2.x, LinePt2.y);

		if ( i%10 == 0 )
		{
			str.Format(_T("%d"), (i/10));			
			strLen = str.GetLength();
			Offset = strLen*4;
			::TextOut(hDC, LinePt2.x-Offset, LinePt2.y+2, str, strLen);
		}
	}
	
	if ( m_RulerWndRect.bottom < (LongLine*4) ) { return; }

	//英制
	MaxUint = (int)((DrawMaxX-DrawMinX)/25400);//base on inch
	DrawMaxX = MaxUint*25400+DrawMinX;

	MaxUint = MaxUint*16;//inch to SubInch	
	//繪製英制
	LongLine = (int)(15000.0/RulerScaleY);
	MidLine  = (int)(10000.0/RulerScaleY);
	ShortLine = (int)(5000.0/RulerScaleY);

	LinePt1.y = (int)(DrawMaxY/RulerScaleY);
	LinePt1.x = (int)(DrawMinX/RulerScaleX)/2;	
	str.Format(_T("%s"), _T("Inch"));
	strLen = str.GetLength();
	Offset = strLen*3;
	::TextOut(hDC, LinePt1.x-Offset, LinePt1.y-ShortLine, str, strLen);
	for ( i=0; i<=MaxUint; i++ )
	{
		//mmUnit = DrawMinX+(i*1587.0);//每一小格多少um
		mmUnit = DrawMinX+(i*1587.5);//每一小格多少um
		LinePt2.x = LinePt1.x = (int)(mmUnit/RulerScaleX);

		LinePt1.y = (int)(DrawMaxY/RulerScaleY);
		if ( i%16 == 0 )
		{	LinePt2.y = LinePt1.y-LongLine;	}
		else if ( i%4 == 0 )
		{	LinePt2.y = LinePt1.y-MidLine;	}
		else
		{	LinePt2.y = LinePt1.y-ShortLine;	}

		::MoveToEx(hDC, LinePt1.x, LinePt1.y, NULL);
		::LineTo(hDC, LinePt2.x, LinePt2.y);

		if ( i%16 == 0 )
		{
			str.Format(_T("%d"), (i/16));			
			strLen = str.GetLength();
			int Offset = strLen*4;
			::TextOut(hDC, LinePt2.x-Offset, LinePt2.y-2, str, strLen);
		}
	}

}
//-------------------------------------------------------------------------------------//
bool CRulerWnd::SaveRulerWndParameter()
{
	const size_t textlen = 128;
	CString Error;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");	
	
	Section = _T("Parameter");
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("RulerWnd.INI"));	

	//儲存-Ruler Scale X
	KeyName.Format(_T("Ruler Scale X"));
	String.Format(_T("%f"), m_RulerScaleX);
	if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, Error) == false )
	{	return false;	}

	//儲存-Ruler Scale Y
	KeyName.Format(_T("Ruler Scale Y"));
	String.Format(_T("%f"), m_RulerScaleY);
	if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, Error) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRulerWnd::LoadRulerWndParameter()
{
	const size_t textlen = 128;
	CString Error;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");

	Section = _T("Parameter");
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("RulerWnd.INI"));	

	//儲存-Ruler Scale X
	KeyName.Format(_T("Ruler Scale X"));
	Default.Format(_T("%f"), m_RulerScaleX);
	if ( JetAPI::LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, Error) == true ) 	
	{	m_RulerScaleX = ::_tcstod(String, NULL); }

	//儲存-Ruler Scale Y
	KeyName.Format(_T("Ruler Scale Y"));
	Default.Format(_T("%f"), m_RulerScaleY);
	if ( JetAPI::LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, Error) == true ) 	
	{	m_RulerScaleY = ::_tcstod(String, NULL); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRulerWnd::ExecChangeScaleValue()
{
	CInputBoxWnd InputBox;
	CString strCaption;
	CString strValue1, strValue2;
	CString strLabel1, strLabel2;
	
	strCaption = _T("");
	strLabel1 = _T("X Scale");
	strLabel2 = _T("Y Scale");
	strValue1.Format(_T("%.2f"), m_RulerScaleX);
	strValue2.Format(_T("%.2f"), m_RulerScaleY);

	strLabel1 = _T("Pixel Scale");
	InputBox.SetParam1(strCaption, strLabel1, strValue1);
	//InputBox.SetParam2(strCaption, strLabel1, strValue1, strLabel2, strValue2);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true; }
	InputBox.m_DataEdit2 = InputBox.m_DataEdit1;
	double ScaleX=JetAPI::StrToDbl(InputBox.m_DataEdit1);
	double ScaleY=JetAPI::StrToDbl(InputBox.m_DataEdit2);
	if ( ScaleX > 1.0 )
	{	m_RulerScaleX = ScaleX;	}
	if ( ScaleY > 1.0 )
	{	m_RulerScaleY = ScaleY;	}
	SaveRulerWndParameter();
	HDC hDC = this->m_RulerWndMemDC.GetSafeHdc();	
	this->DrawRulerImage(hDC, true);		
	this->RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CRulerWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadRulerWndParameter();
	SaveRulerWndParameter();

	RECT      WndRect;
	const int MaxWinW = ::GetSystemMetrics(SM_CXMAXIMIZED);
	const int MaxWinH = ::GetSystemMetrics(SM_CYMAXIMIZED);
	const int WndSizeW = MaxWinW-32;
	const int WndSizeH = MaxWinH-32;
	CWnd::GetWindowRect(&WndRect);
	this->ClientToScreen(&WndRect);
	WndRect.left = (MaxWinW-WndSizeW)/2;
	WndRect.right = WndRect.left+WndSizeW;
	CWnd::MoveWindow(&WndRect);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
LRESULT CRulerWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{	
	case WM_CHANGE_RULER_SCALE:
		ExecChangeScaleValue();
		break;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CRulerWnd::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default		
	if ( AOIDataCollect.UserLevel_Supervisor(true) == true )
	{	CWnd::PostMessage(WM_CHANGE_RULER_SCALE, NULL, NULL);	}
	CDialog::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//