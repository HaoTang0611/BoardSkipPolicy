// ColorRGBVWnd.cpp : implementation file
//
//-------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ColorRGBVWnd.h"
//-------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------//
//Pick Mode
#define    PICK_MODE_NONE                      0//沒有點到
#define    PICK_MODE_COLOR_TUNING              1//右方調適條
#define    PICK_MODE_BRIGHTNESS                2//下方亮度條
#define    PICK_MODE_TRIANGLE                  3//中間三角區
//-------------------------------------------------------------------------//
//Timer訊息-滑鼠移動
#define    TIMER_EVENT_MOUSE_MOVE              100//事件編號
#define    TIMER_ELAPSE_MOUSE_MOVE             400//延遲時間ms
#define    TIMER_ELAPSE_MOUSE_MOVE_CHECK       TIMER_ELAPSE_MOUSE_MOVE-50//延遲時間ms
//Timer訊息-滑鼠滾輪
#define    TIMER_EVENT_MOUSE_WHEEL             101//事件編號
#define    TIMER_ELAPSE_MOUSE_WHEEL            400//延遲時間ms
#define    TIMER_ELAPSE_MOUSE_WHEEL_CHECK      TIMER_ELAPSE_MOUSE_WHEEL-50//延遲時間ms
//-------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CColorRGBVWnd
//-------------------------------------------------------------------------//
CColorRGBVWnd::CColorRGBVWnd()
{
	this->m_BKClr = 0x000000;	
	this->m_BKClr = 0xFEECDD;	
	this->m_BKClr = 0xFFFFFF;	
	this->m_BKClr = 0x000000;	

	m_ColorRGBVPtr = NULL;
//	m_ColorRGBVPtr = &m_ColorRGBV;

	CColorRGBVWnd::m_RGBVMode = COLOR_RGBV_RED;

	this->m_MouseMode = CURSOR_POS_NONE;
	this->m_PickMode = PICK_MODE_NONE;

	this->m_CallBackHWnd = NULL;
	this->m_ColorFilterID = -1;

	this->m_MouseMoveTime = 0;
	this->m_MouseWheelTime = 0;

	this->m_RGBVTriangleSize.cx = 120*2;
	this->m_RGBVTriangleSize.cy = 208;

	this->m_RGBVTriangleSize.cx = 90*2;
	this->m_RGBVTriangleSize.cy = 156;

	this->m_RGBVTriangleSize.cx = 72*2;
	this->m_RGBVTriangleSize.cy = 124;

	m_bLockUI = false;
}
//-------------------------------------------------------------------------//
CColorRGBVWnd::~CColorRGBVWnd()
{
}
//-------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CColorRGBVWnd, CStatic)
	//{{AFX_MSG_MAP(CColorRGBVWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CColorRGBVWnd message handlers
//-------------------------------------------------------------------------//
void CColorRGBVWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	this->ReDrawWnd();
	// Do not call CStatic::OnPaint() for painting messages
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_MouseUpPt = point;
	this->DoMouseMoveUI_RGBV(point, m_ColorRGBVPtr);
	if ( this != this->GetCapture() ) 
	{
		CStatic::OnMouseMove(nFlags, point);
		return;
	}
	//this->ReDrawWnd();
	this->m_LastMousePt = this->m_MouseUpPt;
	CStatic::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------//
BOOL CColorRGBVWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	this->DoMouseWheelUI_RGBV(nFlags, zDelta, pt, m_ColorRGBVPtr);		
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CColorRGBVWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_PickMode = PICK_MODE_NONE;	
	if ( GetLockWnd() == true ) { return ; }
	if ( DoLButtonDownForUI_RGBV(point, m_ColorRGBVPtr) == false )
	{
		CStatic::OnLButtonDown(nFlags, point);
		return; 
	}
	
	this->SetFocus();
	this->SetCapture();
	if ( NULL != m_ColorRGBVPtr )
	{	SetColorRGBVOld(*m_ColorRGBVPtr); }
	CStatic::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_MouseUpPt = point;
	this->DoMouseMoveUI_RGBV(point, m_ColorRGBVPtr);	
	this->m_PickMode = PICK_MODE_NONE;	
	this->m_MouseMode = CURSOR_POS_NONE;
	if ( this->m_CallBackHWnd != NULL )
	{	
		::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER_BTN_UP, NULL);	
		::PostMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER_LEFT_BTN_UP, NULL);	
	}
	CStatic::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	this->m_PickMode = PICK_MODE_NONE;	
	if ( GetLockWnd() == true ) { return ; }
	this->SetFocus();
	this->SetCapture();
	if ( NULL != m_ColorRGBVPtr )
	{	SetColorRGBVOld(*m_ColorRGBVPtr); }
	CStatic::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	if ( this->m_CallBackHWnd != NULL )
	{	
		::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER_BTN_UP, NULL);	
		::PostMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER_RIGHT_BTN_UP, NULL);	
	}
	CStatic::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::CreateMemDC()
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();	
	this->GetClientRect(&m_WndRect);		
	m_MemDC.CreateMemDC(hDC, m_WndRect, true, m_BKClr);	
	this->CreateRGBVBKImage();
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::ReDrawWnd()
{	
	if ( this == NULL ) { return; }
	if ( this->GetSafeHwnd() == NULL ) { return; }
	if ( m_MemDC.GetSafeHdc()==NULL ) { return; }	

	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();	
	::IntersectClipRect(hDC, m_WndRect.left, m_WndRect.top, m_WndRect.right, m_WndRect.bottom);

	::BitBlt(hDC, m_WndRect.left, m_WndRect.top, m_WndRect.right, m_WndRect.bottom, m_MemDC, 0, 0, SRCCOPY);	
	
	if ( m_ColorRGBVTmp.GetUsed() == true ) 
	{	DrawRGBVLine(hDC, &m_ColorRGBVTmp, false);	}
	const bool bAct = true;
	DrawRGBVLine(hDC, m_ColorRGBVPtr, bAct);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::CreateRGBVBKImage()
{
	if ( this->GetSafeHwnd() == NULL ) { return; }
	if ( m_MemDC.GetSafeHdc() == NULL ) { return; }

	double RectW = m_WndRect.right-m_WndRect.left;
	double RectH = m_WndRect.bottom - m_WndRect.top;
	
	POINT CP;
	const int W = (this->m_RGBVTriangleSize.cx/2);
	const int H = (this->m_RGBVTriangleSize.cy);
	CP.x = (m_WndRect.left+m_WndRect.right)/2;
	CP.y = (m_WndRect.top+m_WndRect.bottom)/2;
	
	m_RGBVTriangleRect = m_WndRect;

	m_RGBVTriangleRect.left  = CP.x-W;
	m_RGBVTriangleRect.right = CP.x+W;
	m_RGBVTriangleRect.bottom = H;

	int OffsetX = 8;
	int OffsetY = 32;//16
	m_RGBVTriangleRect.left   -= OffsetX;
	m_RGBVTriangleRect.right  -= OffsetX;
	m_RGBVTriangleRect.top    += OffsetY;
	m_RGBVTriangleRect.bottom += OffsetY;

	m_RGBVHorBarRect = m_RGBVTriangleRect;	
	m_RGBVHorBarRect.top = m_RGBVTriangleRect.bottom+16;
	m_RGBVHorBarRect.bottom = m_RGBVHorBarRect.top+16;//12

	m_RGBVTuningBarRect = m_RGBVTriangleRect;	
	m_RGBVTuningBarRect.left = m_RGBVTriangleRect.right+4;//4
	m_RGBVTuningBarRect.right = m_RGBVTuningBarRect.left+16;//12
	
	this->DrawRGBVTriangleImage(m_MemDC);
	this->DrawRGBVBarImage(m_MemDC);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DrawRGBVTriangleImage(HDC hDC)
{
	RECT Rect = m_WndRect;	
	HBRUSH hBrush=::CreateSolidBrush(m_BKClr);
	::FillRect(hDC, &Rect, hBrush);
	::DeleteObject(hBrush);	

	POINT CP;
	CP.x = (m_RGBVTriangleRect.left+m_RGBVTriangleRect.right)/2;
	CP.y = (m_RGBVTriangleRect.top+m_RGBVTriangleRect.bottom)/2;	

	// Draw Gradient 	
	COLOR16  ClrX=0xFFFF, ClrY=0xFFFF, ClrZ=0xFFFF;
	TRIVERTEX         vert[7] ;
	GRADIENT_TRIANGLE gTri;

	::memset(vert, 0x00, sizeof(vert));
	//O
	vert[0].x       =  CP.x;
	vert[0].y       =  (m_RGBVTriangleRect.bottom-m_RGBVTriangleRect.top)*2/3+m_RGBVTriangleRect.top; 	
	vert[0].Alpha   =  0x0000;

	//X
	vert[1].x       =  m_RGBVTriangleRect.left;
	vert[1].y       =  m_RGBVTriangleRect.bottom;	
	vert[1].Alpha   =  0x0000;

	//Y
	vert[2].x       =  m_RGBVTriangleRect.right;
	vert[2].y       =  m_RGBVTriangleRect.bottom;	
	vert[2].Alpha   =  0x0000;

	//Z
	vert[3].x       =  CP.x;
	vert[3].y       =  m_RGBVTriangleRect.top;	
	vert[3].Alpha   =  0x0000;

	//XY
	vert[4].x       =  (vert[1].x+vert[2].x)/2;
	vert[4].y       =  (vert[1].y+vert[2].y)/2;	
	vert[4].Alpha   =  0x0000;

	//XZ
	vert[5].x       =  (vert[1].x+vert[3].x)/2;
	vert[5].y       =  (vert[1].y+vert[3].y)/2;	
	vert[5].Alpha   =  0x0000;

	//YZ-6
	vert[6].x       =  (vert[2].x+vert[3].x)/2;
	vert[6].y       =  (vert[2].y+vert[3].y)/2;	
	vert[6].Alpha   =  0x0000;

	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_RED://
		vert[0].Red     =  0xCFFF;//O
		vert[0].Green   =  0xCFFF;//O
		vert[0].Blue    =  0xCFFF;//O

		vert[1].Red     =  0x0000;//X
		vert[1].Green   =  ClrX;//X
		vert[1].Blue    =  0x0000;//X

		vert[2].Red     =  0x0000;//Y
		vert[2].Green   =  0x0000;//Y
		vert[2].Blue    =  ClrY;//Y

		vert[3].Red     =  ClrZ;//Z
		vert[3].Green   =  0x0000;//Z
		vert[3].Blue    =  0x0000;//Z

		vert[4].Red     =  0x0000;//XY
		vert[4].Green   =  ClrX;//XY
		vert[4].Blue    =  ClrY;//XY

		vert[5].Red     =  ClrZ;//XZ
		vert[5].Green   =  ClrX;//XZ
		vert[5].Blue    =  0x0000;//XZ
		
		vert[6].Red     =  ClrZ;//YZ
		vert[6].Green   =  0x0000;//YZ
		vert[6].Blue    =  ClrY;//YZ
		break;
	case COLOR_RGBV_GREEN://
		vert[0].Red     =  0xCFFF;//O
		vert[0].Green   =  0xCFFF;//O
		vert[0].Blue    =  0xCFFF;//O

		vert[1].Red     =  0x0000;//X
		vert[1].Green   =  0x0000;//X
		vert[1].Blue    =  ClrX;//X

		vert[2].Red     =  ClrY;//Y
		vert[2].Green   =  0x0000;//Y
		vert[2].Blue    =  0x0000;//Y

		vert[3].Red     =  0x0000;//Z
		vert[3].Green   =  ClrZ;//Z
		vert[3].Blue    =  0x0000;//Z

		vert[4].Red     =  ClrY;//XY
		vert[4].Green   =  0x0000;//XY
		vert[4].Blue    =  ClrX;//XY

		vert[5].Red     =  0x0000;//XZ
		vert[5].Green   =  ClrZ;//XZ
		vert[5].Blue    =  ClrX;//XZ
		
		vert[6].Red     =  ClrY;//YZ
		vert[6].Green   =  ClrZ;//YZ
		vert[6].Blue    =  0x0000;//YZ
		break;
	default://COLOR_RGBV_BLUE
		vert[0].Red     =  0xCFFF;//O
		vert[0].Green   =  0xCFFF;//O
		vert[0].Blue    =  0xCFFF;//O

		vert[1].Red     =  ClrX;//X
		vert[1].Green   =  0x0000;//X
		vert[1].Blue    =  0x0000;//X

		vert[2].Red     =  0x0000;//Y
		vert[2].Green   =  ClrY;//Y
		vert[2].Blue    =  0x0000;//Y

		vert[3].Red     =  0x0000;//Z
		vert[3].Green   =  0x0000;//Z
		vert[3].Blue    =  ClrZ;//Z

		vert[4].Red     =  ClrX;//XY
		vert[4].Green   =  ClrY;//XY
		vert[4].Blue    =  0x0000;//XY

		vert[5].Red     =  ClrX;//XZ
		vert[5].Green   =  0x0000;//XZ
		vert[5].Blue    =  ClrZ;//XZ
		
		vert[6].Red     =  0x000;//YZ
		vert[6].Green   =  ClrY;//YZ
		vert[6].Blue    =  ClrZ;//YZ		
		break;
	}

	//O X XY
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 1;
	gTri.Vertex3   = 4;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	//O XY Y
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 4;
	gTri.Vertex3   = 2;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	//O Y YZ
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 2;
	gTri.Vertex3   = 6;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	//O YZ Z
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 6;
	gTri.Vertex3   = 3;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	//O Z ZX
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 3;
	gTri.Vertex3   = 5;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	//O ZX X
	gTri.Vertex1   = 0;
	gTri.Vertex2   = 5;
	gTri.Vertex3   = 1;	
	::GradientFill(hDC, vert, 7, &gTri, 1, GRADIENT_FILL_TRIANGLE);	

	HPEN hPen = NULL;
	HPEN hOldPen = NULL;

	hPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
	hOldPen = (HPEN)::SelectObject(hDC, hPen);

	::MoveToEx(hDC, CP.x, m_RGBVTriangleRect.top, NULL);
	::LineTo(hDC, m_RGBVTriangleRect.right, m_RGBVTriangleRect.bottom);
	::LineTo(hDC, m_RGBVTriangleRect.left, m_RGBVTriangleRect.bottom);
	::LineTo(hDC, CP.x, m_RGBVTriangleRect.top);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DrawRGBVBarImage(HDC hDC)
{
	int OffsetY = 16;
	RECT BarRect;	

	TRIVERTEX         vert[2];
	GRADIENT_RECT     gRect;

	//Horizontal bar
	::memset(vert, 0x00, sizeof(vert));
	BarRect = this->m_RGBVHorBarRect;	
	vert[0].x       =  BarRect.left;
	vert[0].y       =  BarRect.top; 	
	vert[0].Red     =  0x0000;
	vert[0].Green   =  0x0000;
	vert[0].Blue    =  0x0000;
	vert[0].Alpha   =  0x0000;	

	vert[1].x       =  BarRect.right;
	vert[1].y       =  BarRect.bottom; 	
	vert[1].Red     =  0xFFFF;
	vert[1].Green   =  0xFFFF;
	vert[1].Blue    =  0xFFFF;
	vert[1].Alpha   =  0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	::GradientFill(hDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

	//Tunning bar
	::memset(vert, 0x00, sizeof(vert));
	BarRect = this->m_RGBVTuningBarRect;	
	vert[0].x       =  BarRect.left;
	vert[0].y       =  BarRect.top;
	vert[0].Alpha   =  0x0000;	

	vert[1].x       =  BarRect.right;
	vert[1].y       =  BarRect.bottom; 	
	vert[1].Alpha   =  0x0000;

	switch ( this->m_RGBVMode )
	{
	case COLOR_RGBV_GREEN:
		vert[0].Red     =  0x0000;
		vert[0].Green   =  0xFFFF;
		vert[0].Blue    =  0x0000;

		vert[1].Red     =  0xFFFF;
		vert[1].Green   =  0x0000;
		vert[1].Blue    =  0xFFFF;
		break;
	case COLOR_RGBV_BLUE:
		vert[0].Red     =  0x0000;
		vert[0].Green   =  0x0000;
		vert[0].Blue    =  0xFFFF;

		vert[1].Red     =  0xFFFF;
		vert[1].Green   =  0xFFFF;
		vert[1].Blue    =  0x0000;
		break;
	default:
		vert[0].Red     =  0xFFFF;
		vert[0].Green   =  0x0000;
		vert[0].Blue    =  0x0000;

		vert[1].Red     =  0x0000;
		vert[1].Green   =  0xFFFF;
		vert[1].Blue    =  0xFFFF;
		break;
	}
	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	::GradientFill(hDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DrawRGBVLine(HDC hDC, CColorRGBV *rgbvPtr, bool bAct)
{	
	if ( NULL == rgbvPtr ) { return ; }		
	HPEN  hPen = NULL;
	HPEN  hOldPen = NULL;	
	POINT LP1, LP2, RP1, RP2;	

	double LX = 0, RX = 0;
	double MinX = this->m_RGBVHorBarRect.left;
	double MaxX = this->m_RGBVHorBarRect.right;
	double ScaleX= (MaxX-MinX)/255.0;

	//Hor
	RP1.y = LP1.y = this->m_RGBVHorBarRect.top;
	RP2.y = LP2.y = this->m_RGBVHorBarRect.bottom;
	LX = rgbvPtr->GetValueMin();
	RX = rgbvPtr->GetValueMax();
	LP2.x = LP1.x = (int)(LX*ScaleX+MinX);
	RP2.x = RP1.x = (int)(RX*ScaleX+MinX);
	//BuildRect();	
	if ( false == bAct )
	{			
		RECT  Rect={0};
		Rect.left = LP1.x;
		Rect.top = LP1.y+1;
		Rect.right = RP2.x;
		Rect.bottom = RP2.y-1;

		hPen = ::CreatePen(PS_SOLID, 1, 0x00A000); 
		hOldPen = (HPEN)::SelectObject(hDC, hPen);		
		DrawRectLine(hDC, Rect);		
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);		
	}
	else
	{	
		hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF); 
		hOldPen = (HPEN)::SelectObject(hDC, hPen);		
		::MoveToEx(hDC, LP1.x, LP1.y, NULL);
		::LineTo(hDC, LP2.x, LP2.y);
		::LineTo(hDC, RP2.x, RP2.y);
		::LineTo(hDC, RP1.x, RP1.y);
		::LineTo(hDC, LP1.x, LP1.y);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
	
	}
	

	//Ver
	double TY = 0, BY = 0;
	double ZT = 0, ZB = 0;
	double MinY = this->m_RGBVTuningBarRect.top;
	double MaxY = this->m_RGBVTuningBarRect.bottom;
	double ScaleY = (MaxY-MinY)/255.0;

	switch ( this->m_RGBVMode )
	{
	case COLOR_RGBV_GREEN:
		TY = rgbvPtr->GetGreenMax();
		BY = rgbvPtr->GetGreenMin();
		break;
	case COLOR_RGBV_BLUE:
		TY = rgbvPtr->GetBlueMax();
		BY = rgbvPtr->GetBlueMin();
		break;
	default:
		TY = rgbvPtr->GetRedMax();
		BY = rgbvPtr->GetRedMin();
		break;
	}	
	LP1.x = LP2.x = this->m_RGBVTriangleRect.left;
	RP1.x = RP2.x = this->m_RGBVTuningBarRect.right;

	LP1.y = RP1.y = m_RGBVTuningBarRect.bottom-(int)(TY*ScaleY);
	LP2.y = RP2.y = m_RGBVTuningBarRect.bottom-(int)(BY*ScaleY);

	ZT = LP1.y;
	ZB = LP2.y;

	if ( true == bAct )
	{
		hPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);

		::MoveToEx(hDC, LP1.x, LP1.y, NULL);
		::LineTo(hDC, RP1.x, RP1.y);

		::MoveToEx(hDC, LP2.x, LP2.y, NULL);
		::LineTo(hDC, RP2.x, RP2.y);

		::MoveToEx(hDC, m_RGBVTuningBarRect.left, LP1.y, NULL);
		::LineTo(hDC, m_RGBVTuningBarRect.right, LP1.y);
		::LineTo(hDC, m_RGBVTuningBarRect.right, RP2.y);
		::LineTo(hDC, m_RGBVTuningBarRect.left, RP2.y);
		::LineTo(hDC, m_RGBVTuningBarRect.left, LP1.y);	

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
	}

	//Draw Line in Triangle
	double LineXZB=0, LineXZT=0;
	double LineYZB=0, LineYZT=0;

	double vb1 = 296;
	double vb2 = 246;

	double x1 = this->m_RGBVTriangleRect.left;
	double y1 = (this->m_RGBVTriangleRect.bottom);
	double x2 = (this->m_RGBVTriangleRect.left+this->m_RGBVTriangleRect.right)/2;	
	double y2 = (this->m_RGBVTriangleRect.top);
	double bb1 = (y1*x2 - y2*x1)/(x2-x1);
	x1 = this->m_RGBVTriangleRect.right;
	y1 = (this->m_RGBVTriangleRect.bottom);
	double bb2 = -(y1*x2 - y2*x1)/(x2-x1);

	vb1 = bb1;
	vb2 = bb2;

	switch ( this->m_RGBVMode )
	{
	case COLOR_RGBV_GREEN:
		//XZ => Blue
		if ( rgbvPtr->GetRedEnabled() == true )
		{
			LineYZB = rgbvPtr->GetRedMin();
			LineYZT = rgbvPtr->GetRedMax();
		}
		else
		{
			LineYZB = 0;
			LineYZT = 255;
		}
		//YZ => Green
		if ( rgbvPtr->GetBlueEnabled() == true )
		{
			LineXZB = rgbvPtr->GetBlueMin();
			LineXZT = rgbvPtr->GetBlueMax();
		}
		else
		{
			LineXZB = 0;
			LineXZT = 255;
		}
		if ( rgbvPtr->GetGreenEnabled() == true )
		{
			TY = rgbvPtr->GetGreenMax();
			BY = rgbvPtr->GetGreenMin();
		}
		else
		{
			TY = 255;
			BY = 0;
		}
		break;
	case COLOR_RGBV_BLUE:
		//XZ => Blue
		if ( rgbvPtr->GetGreenEnabled() == true )
		{
			LineYZB = rgbvPtr->GetGreenMin();
			LineYZT = rgbvPtr->GetGreenMax();
		}
		else
		{
			LineYZB = 0;
			LineYZT = 255;
		}

		//YZ => Green
		if ( rgbvPtr->GetRedEnabled() == true )
		{
			LineXZB = rgbvPtr->GetRedMin();
			LineXZT = rgbvPtr->GetRedMax();
		}
		else
		{
			LineXZB = 0;
			LineXZT = 255;
		}
		if ( rgbvPtr->GetBlueEnabled() == true )
		{
			TY = rgbvPtr->GetBlueMax();
			BY = rgbvPtr->GetBlueMin();
		}
		else
		{
			TY = 255;
			BY = 0;
		}
		break;
	default:
		//XZ => Blue
		if ( rgbvPtr->GetBlueEnabled() == true )
		{
			LineYZB = rgbvPtr->GetBlueMin();
			LineYZT = rgbvPtr->GetBlueMax();
		}
		else
		{
			LineYZB = 0;
			LineYZT = 255;
		}

		//YZ => Green
		if ( rgbvPtr->GetGreenEnabled() == true )
		{
			LineXZB = rgbvPtr->GetGreenMin();
			LineXZT = rgbvPtr->GetGreenMax();
		}
		else
		{
			LineXZB = 0;
			LineXZT = 255;
		}
		if ( rgbvPtr->GetRedEnabled() == true )
		{
			TY = rgbvPtr->GetRedMax();
			BY = rgbvPtr->GetRedMin();
		}
		else
		{
			TY = 255;
			BY = 0;
		}
		break;
	}
	ZT = m_RGBVTuningBarRect.bottom-(int)(TY*ScaleY);
	ZB = m_RGBVTuningBarRect.bottom-(int)(BY*ScaleY);

	LineXZB = LineXZB*ScaleY;
	LineXZT = LineXZT*ScaleY;
	LineYZB = LineYZB*ScaleY;
	LineYZT = LineYZT*ScaleY;

	double mXZ = ::sqrt((double)3);
	double mYZ = -1*::sqrt((double)3);
	//Y = sqrt(3)X+255-V
	//Line XZb
	LP1.x = (int)(MinX);
	LP1.y = (int)(mXZ*MinX+LineXZB+LineXZB-vb2);
	LP2.x = (int)(MaxX);
	LP2.y = (int)(mXZ*MaxX+LineXZB+LineXZB-vb2);
#ifdef _DEBUG
	::MoveToEx(hDC, LP1.x, LP1.y, NULL);
	::LineTo(hDC, LP2.x, LP2.y);
#endif	

	//Line XZt
	RP1.x = (int)(MinX);
	RP1.y = (int)(mXZ*MinX+LineXZT+LineXZT-vb2);
	RP2.x = (int)(MaxX);
	RP2.y = (int)(mXZ*MaxX+LineXZT+LineXZT-vb2);
#ifdef _DEBUG
	::MoveToEx(hDC, RP1.x, RP1.y, NULL);
	::LineTo(hDC, RP2.x, RP2.y);
#endif	

	//Line YZb
	RP1.x = (int)(MinX);
	RP1.y = (int)(mYZ*MinX+vb1+LineYZB+LineYZB);
	RP2.x = (int)(MaxX);
	RP2.y = (int)(mYZ*MaxX+vb1+LineYZB+LineYZB);	
#ifdef _DEBUG
	::MoveToEx(hDC, RP1.x, RP1.y, NULL);
	::LineTo(hDC, RP2.x, RP2.y);
#endif

	//Line YZt
	RP1.x = (int)(MinX);
	RP1.y = (int)(mYZ*MinX+vb1+LineYZT+LineYZT);
	RP2.x = (int)(MaxX);
	RP2.y = (int)(mYZ*MaxX+vb1+LineYZT+LineYZT);
#ifdef _DEBUG
	::MoveToEx(hDC, RP1.x, RP1.y, NULL);
	::LineTo(hDC, RP2.x, RP2.y);	
#endif	

	//GetTwoLinesCrossPoint
	std::vector<POINT> RegionPts;	
	int DotSize = 2;
	double b1=0, b2=0;
	double TriangleCX=0, TriangleCY=0;
	double CrossPTx=0, CrossPTy=0;
	double CrossPLx=0, CrossPLy=0;
	double CrossPRx=0, CrossPRy=0;
	double CrossPBx=0, CrossPBy=0;

	double CrossPTx1=0, CrossPTy1=0;
	double CrossPTx2=0, CrossPTy2=0;
	double CrossPTx3=0, CrossPTy3=0;
	double CrossPTx4=0, CrossPTy4=0;

	double CrossPBx1=0, CrossPBy1=0;
	double CrossPBx2=0, CrossPBy2=0;
	double CrossPBx3=0, CrossPBy3=0;
	double CrossPBx4=0, CrossPBy4=0;
	
	if ( false == bAct )
	{	
		//hPen = ::CreatePen(PS_DOT, 1, 0xFFFFFF);	
		hPen = ::CreatePen(PS_SOLID, 1, 0x00A000);	
	}	
	else
	{	hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF); }
	hOldPen = (HPEN)::SelectObject(hDC, hPen);

	b1 = 2*LineXZB-vb2;
	b2 = vb1+2*LineYZB;
	JetAPI::GetTwoLinesCrossPoint(mXZ, b1, mYZ, b2, CrossPTx, CrossPTy);	

	b1 = 2*LineXZB-vb2;
	b2 = vb1+2*LineYZT;
	JetAPI::GetTwoLinesCrossPoint(mXZ, b1, mYZ, b2, CrossPRx, CrossPRy);	
	
	b1 = 2*LineXZT-vb2;
	b2 = vb1+2*LineYZB;
	JetAPI::GetTwoLinesCrossPoint(mXZ, b1, mYZ, b2, CrossPLx, CrossPLy);	

	b1 = 2*LineXZT-vb2;
	b2 = vb1+2*LineYZT;
	JetAPI::GetTwoLinesCrossPoint(mXZ, b1, mYZ, b2, CrossPBx, CrossPBy);	

	b2 = 2*LineXZB-vb2;
	JetAPI::GetTwoLinesCrossPoint(0, ZT, mXZ, b2, CrossPTx1, CrossPTy1);	

	b2 = 2*LineXZT-vb2;
	JetAPI::GetTwoLinesCrossPoint(0, ZT, mXZ, b2, CrossPTx2, CrossPTy2);	

	b2 = vb1+2*LineYZB;
	JetAPI::GetTwoLinesCrossPoint(0, ZT, mYZ, b2, CrossPTx3, CrossPTy3);	
	
	b2 = vb1+2*LineYZT;
	JetAPI::GetTwoLinesCrossPoint(0, ZT, mYZ, b2, CrossPTx4, CrossPTy4);	
	
	b2 = 2*LineXZB-vb2;
	JetAPI::GetTwoLinesCrossPoint(0, ZB, mXZ, b2, CrossPBx1, CrossPBy1);	

	b2 = 2*LineXZT-vb2;
	JetAPI::GetTwoLinesCrossPoint(0, ZB, mXZ, b2, CrossPBx2, CrossPBy2);		

	b2 = vb1+2*LineYZB;
	JetAPI::GetTwoLinesCrossPoint(0, ZB, mYZ, b2, CrossPBx3, CrossPBy3);		

	b2 = vb1+2*LineYZT;
	JetAPI::GetTwoLinesCrossPoint(0, ZB, mYZ, b2, CrossPBx4, CrossPBy4);	

	TriangleCX = (m_RGBVTriangleRect.left+m_RGBVTriangleRect.right)/2;
	TriangleCY = (m_RGBVTriangleRect.bottom-(m_RGBVTriangleRect.bottom-m_RGBVTriangleRect.top)*1/3);
#ifdef _DEBUG
	this->DrawDotPos(hDC, TriangleCX, TriangleCY);

	this->DrawDotPos(hDC, CrossPTx, CrossPTy);
	this->DrawDotPos(hDC, CrossPRx, CrossPRy);
	this->DrawDotPos(hDC, CrossPLx, CrossPLy);
	this->DrawDotPos(hDC, CrossPBx, CrossPBy);
	this->DrawDotPos(hDC, CrossPTx1, CrossPTy1);	
	this->DrawDotPos(hDC, CrossPTx2, CrossPTy2);	
	this->DrawDotPos(hDC, CrossPTx3, CrossPTy3);
	this->DrawDotPos(hDC, CrossPTx4, CrossPTy4);	
	this->DrawDotPos(hDC, CrossPBx1, CrossPBy1);	
	this->DrawDotPos(hDC, CrossPBx2, CrossPBy2);
	this->DrawDotPos(hDC, CrossPBx3, CrossPBy3);
	this->DrawDotPos(hDC, CrossPBx4, CrossPBy4);
#endif
	if ( ZT < CrossPTy )
	{
		if ( ZB > CrossPBy )
		{
			RP1.x = (int)(CrossPTx);
			RP1.y = (int)(CrossPTy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx);
			RP1.y = (int)(CrossPBy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx);
			RP1.y = (int)(CrossPTy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB<=CrossPRy )
		{
			RP1.x = (int)(CrossPTx);
			RP1.y = (int)(CrossPTy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);
		}
		else if ( ZB<=CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx);
			RP1.y = (int)(CrossPTy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>=CrossPTy )
		{
			RP1.x = (int)(CrossPTx);
			RP1.y = (int)(CrossPTy);
			RegionPts.push_back(RP1);		

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);		
		}
	}
	else if ( ZT<CrossPLy && ZT<CrossPRy )
	{		
		if ( ZB > CrossPBy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);			

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx);
			RP1.y = (int)(CrossPBy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB<=CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);
		}
		else if ( ZB<=CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>=CrossPTy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);			

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);		
		}
	}
	else if ( ZT<CrossPLy && ZT>=CrossPRy )
	{
		if ( ZB > CrossPBy )
		{
			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);			

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx);
			RP1.y = (int)(CrossPBy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);	
		}
		else if ( ZB>CrossPLy && ZB<=CrossPRy )
		{
			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPLx);
			RP1.y = (int)(CrossPLy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);
		}
		else if ( ZB<=CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);			
		}
		else if ( ZB>=CrossPTy )
		{
			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx3);
			RP1.y = (int)(CrossPTy3);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);		
		}
	}
	else if ( ZT<CrossPRy && ZT>=CrossPLy )
	{
		if ( ZB > CrossPBy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx);
			RP1.y = (int)(CrossPBy);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);
		

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>CrossPLy && ZB<=CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);		

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);
		}
		else if ( ZB<=CrossPLy && ZB> CrossPRy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPRx);
			RP1.y = (int)(CrossPRy);
			RegionPts.push_back(RP1);
		}
		else if ( ZB>=CrossPTy )
		{
			RP1.x = (int)(CrossPTx1);
			RP1.y = (int)(CrossPTy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPBx1);
			RP1.y = (int)(CrossPBy1);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx3);
			RP1.y = (int)(CrossPBy3);
			RegionPts.push_back(RP1);		
		}
	}
	else if ( ZT<CrossPBy )
	{
		if ( ZB > CrossPBy )
		{
			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);	

			RP1.x = (int)(CrossPBx);
			RP1.y = (int)(CrossPBy);
			RegionPts.push_back(RP1);	
		}
		else
		{
			RP1.x = (int)(CrossPTx2);
			RP1.y = (int)(CrossPTy2);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPTx4);
			RP1.y = (int)(CrossPTy4);
			RegionPts.push_back(RP1);			

			RP1.x = (int)(CrossPBx4);
			RP1.y = (int)(CrossPBy4);
			RegionPts.push_back(RP1);

			RP1.x = (int)(CrossPBx2);
			RP1.y = (int)(CrossPBy2);
			RegionPts.push_back(RP1);
		}
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);


	if ( false == bAct )
	{	
		//hPen = ::CreatePen(PS_DOT, 1, 0x808080);	
		hPen = ::CreatePen(PS_SOLID, 1, 0x00A000);	
	}
	else
	{	hPen = ::CreatePen(PS_SOLID, 2, 0x000000); }
	hOldPen = (HPEN)::SelectObject(hDC, hPen);
	int NPts = (int)(RegionPts.size());
	int i=0;
	for ( i=0; i<NPts; i++ )
	{
		if ( i<NPts-1 )
		{
			RP1 = RegionPts[i];
			RP2 = RegionPts[i+1];
		}
		else
		{
			RP1 = RegionPts[i];
			RP2 = RegionPts[0];
		}

		::MoveToEx(hDC, RP1.x, RP1.y, NULL);
		::LineTo(hDC, RP2.x, RP2.y);
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DrawDotPos(HDC hDC, double x, double y)
{
	int DotSize = 2;
	RECT DotRect;
	DotRect.left = (int)(x-DotSize);
	DotRect.right = (int)(x+DotSize);
	DotRect.top = (int)(y-DotSize);
	DotRect.bottom = (int)(y+DotSize);
	::MoveToEx(hDC, DotRect.left, DotRect.top, NULL);
	::LineTo(hDC, DotRect.right, DotRect.top);
	::LineTo(hDC, DotRect.right, DotRect.bottom);
	::LineTo(hDC, DotRect.left, DotRect.bottom);
	::LineTo(hDC, DotRect.left, DotRect.top);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DrawRectLine(HDC hDC, const RECT &Rect)
{
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::BuildRect(const POINT &Pt1, const POINT &Pt2, RECT &Rect)
{
	if ( Pt1.x < Pt2.x )
	{
		Rect.left = Pt1.x;
		Rect.right = Pt2.x;
	}
	else
	{
		Rect.left = Pt2.x;
		Rect.right = Pt1.x;
	}
	if ( Pt1.y < Pt2.y )
	{
		Rect.top = Pt1.y;
		Rect.bottom = Pt2.y;
	}
	else
	{
		Rect.top = Pt2.y;
		Rect.bottom = Pt1.y;
	}
}
//-------------------------------------------------------------------------//
int CColorRGBVWnd::GetRGBVMode()
{
	return m_RGBVMode;
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::SetRGBVMode(COLOR_RGBV_MODE Mode)
{
	this->m_RGBVMode = Mode;
	if ( NULL != m_ColorRGBVPtr )
	{	m_ColorRGBVPtr->SetColorMode(Mode); }
	this->CreateRGBVBKImage();
	this->ReDrawWnd();
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::SetColorRGBVPtr(CColorRGBV *rgbvPtr)
{
	m_ColorRGBVPtr = rgbvPtr;
}
//-------------------------------------------------------------------------//
CColorRGBV *CColorRGBVWnd::GetColorRGBVPtr()
{
	return m_ColorRGBVPtr;
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::ClearColorRGBVTmp()
{
	m_ColorRGBVTmp.ResetColor();
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::SetColorRGBVTmp(const CColorRGBV &rgbvPtr)
{
	m_ColorRGBVTmp = rgbvPtr;
	m_ColorRGBVTmp.CheckUsed();
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::ClearColorRGBVOld()
{
	m_ColorRGBVOld.ResetColor();
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::SetColorRGBVOld(const CColorRGBV &rgbvPtr)
{
	m_ColorRGBVOld = rgbvPtr;
	m_ColorRGBVOld.CheckUsed();
}
//-------------------------------------------------------------------------//
CColorRGBV* CColorRGBVWnd::GetColorRGBVOldPtr()
{
	return &m_ColorRGBVOld;
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DoMouseMoveUIEnd_RGBV()
{
	DWORD CurTime = ::GetTickCount();
	DWORD GapTime = CurTime-m_MouseMoveTime;
	if ( GapTime < TIMER_ELAPSE_MOUSE_MOVE_CHECK ) { return; }	
	if ( this->m_CallBackHWnd != NULL )
	{	::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER, NULL);	}
	return ;
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::StartMouseMoveUIEndEvent_RGBV()
{
	bool bTimer=true;
	if ( true == bTimer )
	{
		m_MouseMoveTime = ::GetTickCount();
		CWnd::SetTimer(TIMER_EVENT_MOUSE_MOVE, TIMER_ELAPSE_MOUSE_MOVE, NULL);
	}
	else
	{
		if ( this->m_CallBackHWnd != NULL )
		{	::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER, NULL);	}
	}
	return;
}
//-------------------------------------------------------------------------//
void CColorRGBVWnd::DoMouseMoveUI_RGBV(POINT point, CColorRGBV *rgbvPtr)
{	
	if ( this == NULL ) { return; }
	if ( this->GetSafeHwnd() == NULL ) { return; }
	if ( GetLockWnd() == true ) { return ; }

	RECT Rect = this->m_WndRect;	
	POINT CP;
	CP.x = (Rect.left+Rect.right)/2;
	CP.y = (Rect.top+Rect.bottom)/2;
	CString str;

	CURSOR_POS_MODE MouseMode = this->m_MouseMode;
	if ( MouseMode == CURSOR_POS_NONE )
	{	
		MouseMode = this->GetTrunningMouseMode(point, rgbvPtr); 
		if ( MouseMode == CURSOR_POS_NONE )
		{	
			MouseMode = this->GetBrightnessMouseMode(point, rgbvPtr);	
			if ( MouseMode == CURSOR_POS_NONE )
			{	MouseMode = this->GetTriangleMouseMode(point, rgbvPtr);	}
		}

	}
	JetAPI::UpdateCursor(MouseMode);
	
	if ( m_PickMode == PICK_MODE_COLOR_TUNING )//at tunning bar
	{		
		RECT Rect2 = this->m_RGBVTuningBarRect;		
		double ScaleH = Rect2.bottom-Rect2.top;		
		ScaleH = 255/ScaleH;

		double DPY = m_LastMousePt.y - this->m_MouseUpPt.y;		
		double DV = DPY*ScaleH;			
		if ( this->m_MouseMode != CURSOR_POS_NONE )
		{
			switch ( this->m_MouseMode )
			{
			case CURSOR_POS_BOTTOM:
				if ( m_CurrentMainB >= 0 ) 
				{
					m_CurrentMainB = m_CurrentMainB+(DV);
					if ( m_CurrentMainB >= m_CurrentMainT ) { m_CurrentMainB = m_CurrentMainT-1; }
					if ( m_CurrentMainB < 0 ) { m_CurrentMainB = 0; }
					if ( m_CurrentMainB > 255 ) { m_CurrentMainB = 255; }
					if ( rgbvPtr != NULL )
					{
						switch ( m_RGBVMode )
						{
						case COLOR_RGBV_GREEN:
							rgbvPtr->SetGreenMin((int)m_CurrentMainB);
							rgbvPtr->SetGreenEnabled(true);
							break;
						case COLOR_RGBV_BLUE:
							rgbvPtr->SetBlueMin((int)m_CurrentMainB);
							rgbvPtr->SetBlueEnabled(true);
							break;
						default:
							rgbvPtr->SetRedMin((int)m_CurrentMainB);
							rgbvPtr->SetRedEnabled(true);
							break;
						}
					}
				}
				break;
			case CURSOR_POS_TOP:
				if ( m_CurrentMainT >= 0 ) 
				{
					m_CurrentMainT = m_CurrentMainT+(DV);
					if ( m_CurrentMainT <= m_CurrentMainB ) { m_CurrentMainT = m_CurrentMainB+1; }
					if ( m_CurrentMainT < 0 ) { m_CurrentMainT = 0; }
					if ( m_CurrentMainT > 255 ) { m_CurrentMainT = 255; }
					if ( rgbvPtr != NULL )
					{
						switch ( m_RGBVMode )
						{
						case COLOR_RGBV_GREEN:
							rgbvPtr->SetGreenMax((int)m_CurrentMainT);
							rgbvPtr->SetGreenEnabled(true);
							break;
						case COLOR_RGBV_BLUE:
							rgbvPtr->SetBlueMax((int)m_CurrentMainT);
							rgbvPtr->SetBlueEnabled(true);
							break;
						default:
							rgbvPtr->SetRedMax((int)m_CurrentMainT);
							rgbvPtr->SetRedEnabled(true);
							break;
						}
					}
				}
				break;				
			case CURSOR_POS_INNER:
				if ( m_CurrentMainT>=0 && m_CurrentMainB>=0 )
				{
					m_CurrentMainT = m_CurrentMainT+(DV);
					m_CurrentMainB = m_CurrentMainB+(DV);					
					if ( m_CurrentMainT < 0 ) { m_CurrentMainT = 0; }
					if ( m_CurrentMainT > 255 ) { m_CurrentMainT = 255; }
					if ( m_CurrentMainB < 0 ) { m_CurrentMainB = 0; }
					if ( m_CurrentMainB > 255 ) { m_CurrentMainB = 255; }

					if ( rgbvPtr != NULL )
					{
						switch ( m_RGBVMode )
						{
						case COLOR_RGBV_GREEN:
							rgbvPtr->SetGreenMax((int)m_CurrentMainT);
							rgbvPtr->SetGreenMin((int)m_CurrentMainB);
							rgbvPtr->SetGreenEnabled(true);							
							break;
						case COLOR_RGBV_BLUE:
							rgbvPtr->SetBlueMax((int)m_CurrentMainT);
							rgbvPtr->SetBlueMin((int)m_CurrentMainB);
							rgbvPtr->SetBlueEnabled(true);
							break;
						default:
							rgbvPtr->SetRedMax((int)m_CurrentMainT);
							rgbvPtr->SetRedMin((int)m_CurrentMainB);
							rgbvPtr->SetRedEnabled(true);
							break;
						}
					}
				}
				break;
			}							
			this->ReDrawWnd();
			this->StartMouseMoveUIEndEvent_RGBV();
		}		
	}
	else if ( m_PickMode == PICK_MODE_BRIGHTNESS )//at hor. bar
	{
		RECT Rect2 = this->m_RGBVHorBarRect;
		double ScaleW = Rect2.right-Rect2.left;			
		ScaleW = 255/ScaleW;			
		double DPX = m_MouseUpPt.x - this->m_LastMousePt.x;
		double DS = DPX*ScaleW;				
		if ( this->m_MouseMode != CURSOR_POS_NONE )
		{
			switch ( this->m_MouseMode )
			{
			case CURSOR_POS_LEFT:
				if ( m_CurrentVB >= 0 ) 
				{
					m_CurrentVB = m_CurrentVB+(DS);
					if ( m_CurrentVB >= m_CurrentVT ) { m_CurrentVB = m_CurrentVT-1; }
					if ( m_CurrentVB < 0 ) { m_CurrentVB = 0; }
					if ( m_CurrentVB > 255 ) { m_CurrentVB = 255; }
					if ( rgbvPtr != NULL )
					{	rgbvPtr->SetValueMin((int)(m_CurrentVB));	}
				}
				break;
			case CURSOR_POS_RIGHT:
				if ( m_CurrentVT >= 0 ) 
				{
					m_CurrentVT = m_CurrentVT+(DS);
					if ( m_CurrentVT <= m_CurrentVB ) { m_CurrentVT = m_CurrentVB+1; }
					if ( m_CurrentVT < 0 ) { m_CurrentVT = 0; }
					if ( m_CurrentVT > 255 ) { m_CurrentVT = 255; }
					if ( rgbvPtr != NULL )
					{	rgbvPtr->SetValueMax((int)(m_CurrentVT)); }
				}
				break;				
			case CURSOR_POS_INNER:
				if ( m_CurrentVB>=0 && m_CurrentVT>=0 )
				{
					m_CurrentVT = m_CurrentVT+(DS);
					m_CurrentVB = m_CurrentVB+(DS);					
					if ( m_CurrentVB < 0 ) { m_CurrentVB = 0; }
					if ( m_CurrentVB > 255 ) { m_CurrentVB = 255; }
					if ( m_CurrentVT < 0 ) { m_CurrentVT = 0; }
					if ( m_CurrentVT > 255 ) { m_CurrentVT = 255; }

					if ( rgbvPtr != NULL )
					{
						rgbvPtr->SetValueMax((int)(m_CurrentVT));
						rgbvPtr->SetValueMin((int)(m_CurrentVB));
					}					
				}
				break;
			}
			this->ReDrawWnd();
			this->StartMouseMoveUIEndEvent_RGBV();
		}
	}	
	else if ( m_PickMode == PICK_MODE_TRIANGLE )//at Triangle
	{		
		RECT Rect2 = this->m_RGBVTuningBarRect;		
		double ScaleH = Rect2.bottom-Rect2.top;		
		ScaleH = 255/ScaleH;
		double DPX = m_MouseUpPt.x - this->m_LastMousePt.x;
		double DPY = m_LastMousePt.y - this->m_MouseUpPt.y;		
		double DV = DPY*ScaleH;			
		if ( this->m_MouseMode != CURSOR_POS_NONE )
		{
			int nDVX=(int)(DPX+0.5);
			int nDVY=(int)(DPY+0.5);
			switch ( this->m_MouseMode )
			{			
			case CURSOR_POS_INNER:
				//rgbvPtr->MoveColorRGBV(nDV, true);				
				break;
			}							
			this->ReDrawWnd();
			this->StartMouseMoveUIEndEvent_RGBV();
		}
	}
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::DoMouseWheelUIEnd_RGBV()
{
	DWORD CurTime = ::GetTickCount();
	DWORD GapTime = CurTime-m_MouseWheelTime;
	if ( GapTime < TIMER_ELAPSE_MOUSE_WHEEL_CHECK ) { return; }	
	if ( this->m_CallBackHWnd != NULL )
	{	::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER, NULL);	}
	return ;
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::StartMouseWheelUIEndEvent_RGBV()
{
	bool bTimer=true;
	if ( true == bTimer )
	{
		m_MouseWheelTime = ::GetTickCount();
		CWnd::SetTimer(TIMER_EVENT_MOUSE_WHEEL, TIMER_ELAPSE_MOUSE_WHEEL, NULL);
	}
	else
	{
		if ( this->m_CallBackHWnd != NULL )
		{	::SendMessage(this->m_CallBackHWnd, MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER, NULL);	}
	}
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::DoMouseWheelUI_RGBV(UINT nFlags, short zDelta, CPoint &pt, CColorRGBV *rgbvPtr)
{
	if ( this == NULL ) { return; }
	if ( this->GetSafeHwnd() == NULL ) { return; }
	if ( GetLockWnd() == true ) { return ; }

	int Offset=0;
	bool  bUpdate=true;
	POINT point=pt;
	if ( zDelta > 0 ) 
	{	Offset = 1;	}
	else
	{	Offset = -1; }
	CWnd::ScreenToClient(&point);
	if ( ::PtInRect(&m_RGBVTriangleRect, point) == TRUE )
	{	rgbvPtr->ExpandColorRGBV(Offset, true);		}
	else if ( ::PtInRect(&m_RGBVHorBarRect, point) == TRUE )
	{	rgbvPtr->ExpandColorBrightness(Offset);	}
	else
	{	bUpdate = false; }	
	
	if ( true == bUpdate )
	{
		this->ReDrawWnd();
		StartMouseWheelUIEndEvent_RGBV();		
	}
	return;
}
//-------------------------------------------------------------------------------//
CURSOR_POS_MODE CColorRGBVWnd::GetTriangleMouseMode(POINT point, CColorRGBV *rgbvPtr)
{
	return CURSOR_POS_NONE;
	if ( rgbvPtr == NULL ) { return CURSOR_POS_NONE; }

	CURSOR_POS_MODE MouseMode = CURSOR_POS_NONE;	
	POINT pt = point;
	RECT Rect = this->m_RGBVTriangleRect;
	RECT TagRect = this->m_RGBVTriangleRect;
	RECT InnerRect, OutterRect;
	SIZE size;			
	size.cx = 4; size.cy = 4;

	BOOL Enable = 0;
	int TY=0, BY = 0;
	double MinY = TagRect.top;
	double MaxY = TagRect.bottom;
	double ScaleY = (MaxY-MinY)/255.0;

	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_GREEN:
		TY = rgbvPtr->GetGreenMax();
		BY = rgbvPtr->GetGreenMin();
		Enable = rgbvPtr->GetGreenEnabled();
		break;
	case COLOR_RGBV_BLUE:
		TY = rgbvPtr->GetBlueMax();
		BY = rgbvPtr->GetBlueMin();
		Enable = rgbvPtr->GetBlueEnabled();
		break;
	default:
		TY = rgbvPtr->GetRedMax();
		BY = rgbvPtr->GetRedMin();
		Enable = rgbvPtr->GetRedEnabled();
		break;
	}		

	Rect.top    = TagRect.bottom-(int)(TY*ScaleY);
	Rect.bottom = TagRect.bottom-(int)(BY*ScaleY);
	
	InnerRect = Rect;
	OutterRect = Rect;	

	OutterRect.right  = OutterRect.right+size.cx;
	OutterRect.left   = OutterRect.left-size.cx;
	OutterRect.top    = OutterRect.top-size.cy;
	OutterRect.bottom = OutterRect.bottom+size.cy;

	InnerRect.right  = InnerRect.right-size.cx;
	InnerRect.left   = InnerRect.left+size.cx;
	InnerRect.top    = InnerRect.top+size.cy;
	InnerRect.bottom = InnerRect.bottom-size.cy;

	m_CurrentMainT = -1;
	m_CurrentMainB = -1;
	if ( (pt.x<OutterRect.left) || (pt.x>OutterRect.right) || (pt.y<OutterRect.top) || (pt.y>OutterRect.bottom) )
	{	
		return MouseMode; 
	}

	if ( (pt.x>InnerRect.left) && (pt.x<InnerRect.right) && (pt.y>InnerRect.top) && (pt.y<InnerRect.bottom) )
	{	
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
		MouseMode = CURSOR_POS_INNER;		
	}
	else if ( (pt.y>OutterRect.top) && (pt.y<InnerRect.top) )//在上側
	{
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
		MouseMode = CURSOR_POS_TOP;		
	}
	else if ( (pt.y>InnerRect.bottom) && (pt.y<OutterRect.bottom) )//在下側
	{
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
	    MouseMode = CURSOR_POS_BOTTOM;		
	}
	return MouseMode;
}
//-------------------------------------------------------------------------------//
CURSOR_POS_MODE CColorRGBVWnd::GetTrunningMouseMode(POINT point, CColorRGBV *rgbvPtr)
{
	if ( rgbvPtr == NULL ) { return CURSOR_POS_NONE; }

	CURSOR_POS_MODE MouseMode = CURSOR_POS_NONE;	
	POINT pt = point;
	RECT Rect = this->m_RGBVTuningBarRect;
	RECT TagRect = this->m_RGBVTuningBarRect;
	RECT InnerRect, OutterRect;
	SIZE size;			
	size.cx = 4; size.cy = 4;

	BOOL Enable = 0;
	int TY=0, BY = 0;
	double MinY = TagRect.top;
	double MaxY = TagRect.bottom;
	double ScaleY = (MaxY-MinY)/255.0;

	switch ( m_RGBVMode )
	{
	case COLOR_RGBV_GREEN:
		TY = rgbvPtr->GetGreenMax();
		BY = rgbvPtr->GetGreenMin();
		Enable = rgbvPtr->GetGreenEnabled();
		break;
	case COLOR_RGBV_BLUE:
		TY = rgbvPtr->GetBlueMax();
		BY = rgbvPtr->GetBlueMin();
		Enable = rgbvPtr->GetBlueEnabled();
		break;
	default:
		TY = rgbvPtr->GetRedMax();
		BY = rgbvPtr->GetRedMin();
		Enable = rgbvPtr->GetRedEnabled();
		break;
	}		

	Rect.top    = TagRect.bottom-(int)(TY*ScaleY);
	Rect.bottom = TagRect.bottom-(int)(BY*ScaleY);
	
	InnerRect = Rect;
	OutterRect = Rect;	

	OutterRect.right  = OutterRect.right+size.cx;
	OutterRect.left   = m_RGBVTriangleRect.left-size.cx;
	OutterRect.top    = OutterRect.top-size.cy;
	OutterRect.bottom = OutterRect.bottom+size.cy;

	InnerRect.right  = InnerRect.right-size.cx;
	InnerRect.left   = InnerRect.left+size.cx;
	InnerRect.top    = InnerRect.top+size.cy;
	InnerRect.bottom = InnerRect.bottom-size.cy;

	m_CurrentMainT = -1;
	m_CurrentMainB = -1;
	if ( (pt.x<OutterRect.left) || (pt.x>OutterRect.right) || (pt.y<OutterRect.top) || (pt.y>OutterRect.bottom) )
	{	
		return MouseMode; 
	}

	if ( (pt.x>InnerRect.left) && (pt.x<InnerRect.right) && (pt.y>InnerRect.top) && (pt.y<InnerRect.bottom) )
	{	
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
		MouseMode = CURSOR_POS_INNER;		
	}
	else if ( (pt.y>OutterRect.top) && (pt.y<InnerRect.top) )//在上側
	{
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
		MouseMode = CURSOR_POS_TOP;		
	}
	else if ( (pt.y>InnerRect.bottom) && (pt.y<OutterRect.bottom) )//在下側
	{
		m_CurrentMainT = TY;
		m_CurrentMainB = BY;
		
	    MouseMode = CURSOR_POS_BOTTOM;		
	}
	return MouseMode;
}
//-------------------------------------------------------------------------------//
CURSOR_POS_MODE CColorRGBVWnd::GetBrightnessMouseMode(POINT point, CColorRGBV *rgbvPtr)
{
	CURSOR_POS_MODE MouseMode = CURSOR_POS_NONE;
	if ( rgbvPtr == NULL ) 
	{	return MouseMode; }
	
	POINT pt = point;
	RECT Rect = this->m_RGBVHorBarRect;
	RECT TagRect = this->m_RGBVHorBarRect;
	RECT InnerRect, OutterRect;
	SIZE size;			
	size.cx = 4; size.cy = 0;

	double LX = 0, RX = 0;
	double MinX = TagRect.left;
	double MaxX = TagRect.right;
	double ScaleX= (MaxX-MinX)/255.0;

	//Hor
	LX = rgbvPtr->GetValueMin();	
	RX = rgbvPtr->GetValueMax();	
	
	Rect.left = (int)(LX*ScaleX+MinX);
	Rect.right = (int)(RX*ScaleX+MinX);

	InnerRect = Rect;
	OutterRect = Rect;

	OutterRect.right  = OutterRect.right+size.cx;
	OutterRect.left   = OutterRect.left-size.cx;
	OutterRect.top    = OutterRect.top-size.cy;
	OutterRect.bottom = OutterRect.bottom+size.cy;

	InnerRect.right  = InnerRect.right-size.cx;
	InnerRect.left   = InnerRect.left+size.cx;
	InnerRect.top    = InnerRect.top+size.cy;
	InnerRect.bottom = InnerRect.bottom-size.cy;

	this->m_CurrentVB = -1;
	this->m_CurrentVT = -1;

	if ( (pt.x<OutterRect.left) || (pt.x>OutterRect.right) || (pt.y<OutterRect.top) || (pt.y>OutterRect.bottom) )
	{	return MouseMode; }

	if ( (pt.x>InnerRect.left) && (pt.x<InnerRect.right) && (pt.y>InnerRect.top) && (pt.y<InnerRect.bottom) )
	{
		this->m_CurrentVB = LX;
		this->m_CurrentVT = RX;
	
		MouseMode = CURSOR_POS_INNER;		
	}
	else if ( (pt.x>OutterRect.left) && (pt.x<InnerRect.left) )//在左側
	{
		this->m_CurrentVB = LX;
		this->m_CurrentVT = RX;
		
		MouseMode = CURSOR_POS_LEFT;		
	}
	else if ( (pt.x>InnerRect.right) && (pt.x<OutterRect.right) )//在右側
	{
		this->m_CurrentVB = LX;
		this->m_CurrentVT = RX;
		
	    MouseMode = CURSOR_POS_RIGHT;		
	}
	return MouseMode;
}
//-------------------------------------------------------------------------------//
bool CColorRGBVWnd::DoLButtonDownForUI_RGBV(POINT point, CColorRGBV *rgbvPtr)
{
	this->m_MouseMode = CURSOR_POS_NONE;	
	this->m_PickMode = PICK_MODE_NONE;
	this->m_MouseMode = this->GetTrunningMouseMode(point, rgbvPtr);
	if ( this->m_MouseMode != CURSOR_POS_NONE )
	{
		switch ( this->m_MouseMode )
		{
		case CURSOR_POS_INNER:		
		case CURSOR_POS_TOP:
		case CURSOR_POS_BOTTOM:
			m_PickMode = PICK_MODE_COLOR_TUNING;	
			m_MouseUpPt = m_LastMousePt = point;			
			JetAPI::UpdateCursor(m_MouseMode);			
			break;
		}
	}
	else
	{
		this->m_MouseMode = this->GetBrightnessMouseMode(point, rgbvPtr);
		if ( this->m_MouseMode != CURSOR_POS_NONE )
		{
			switch ( this->m_MouseMode )
			{
			case CURSOR_POS_INNER:		
			case CURSOR_POS_LEFT:
			case CURSOR_POS_RIGHT:				
				if ( rgbvPtr != NULL )
				{	rgbvPtr->SetValueEnabled(true); }				
				m_PickMode = PICK_MODE_BRIGHTNESS;	
				m_MouseUpPt = m_LastMousePt = point;			
				JetAPI::UpdateCursor(m_MouseMode);				
				break;
			}
		}
		else
		{
			this->m_MouseMode = this->GetTriangleMouseMode(point, rgbvPtr);
			if ( this->m_MouseMode != CURSOR_POS_NONE )
			{
				switch ( this->m_MouseMode )
				{
				case CURSOR_POS_INNER:					
					m_PickMode = PICK_MODE_TRIANGLE;	
					m_MouseUpPt = m_LastMousePt = point;			
					JetAPI::UpdateCursor(m_MouseMode);				
					break;
				}
			}
		}
		
	}	
	
	return true;
}
//-------------------------------------------------------------------------------//
BOOL CColorRGBVWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,SS_NOTIFY, TRUE);
	return CStatic::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::SetCallBackWnd(HWND hWnd)
{
	this->m_CallBackHWnd = hWnd;
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::SetColorFilterID(int ID)
{
	this->m_ColorFilterID = ID;
}
//-------------------------------------------------------------------------------//
int CColorRGBVWnd::GetColorFilterID()
{
	return this->m_ColorFilterID;
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::SetTriangleSize(int cx, int cy)
{
	this->m_RGBVTriangleSize.cx = cx;
	this->m_RGBVTriangleSize.cy = cy;
}
//-------------------------------------------------------------------------------//
bool CColorRGBVWnd::GetLockWnd()
{
	return m_bLockUI;
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::SetLockWnd(bool bLock)
{
	m_bLockUI = bLock;
}
//-------------------------------------------------------------------------------//
BOOL CColorRGBVWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	UINT ControlID = pWnd->GetDlgCtrlID();
	
	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosMode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	if ( CursorMode != OldCursorMode )
	{	CModelEditWnd::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CDialog::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	*/
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------//
void CColorRGBVWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default		
	switch ( nIDEvent )
	{
	case TIMER_EVENT_MOUSE_MOVE:
		CWnd::KillTimer(nIDEvent);
		DoMouseMoveUIEnd_RGBV();		
		break;
	case TIMER_EVENT_MOUSE_WHEEL:
		CWnd::KillTimer(nIDEvent);
		DoMouseWheelUIEnd_RGBV();		
		break;
	}
	CStatic::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//