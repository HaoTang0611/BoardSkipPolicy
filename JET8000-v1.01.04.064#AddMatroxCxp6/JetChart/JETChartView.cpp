// JETChartView.cpp : implementation file
//

#include "stdafx.h"
#include "..\stdafx.h"
#include "JETChartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJETChartView

CJETChartView::CJETChartView()
{
	PreInitial();
}

CJETChartView::~CJETChartView()
{
	ReleaseAll();
}


BEGIN_MESSAGE_MAP(CJETChartView, CStatic)
	//{{AFX_MSG_MAP(CJETChartView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJETChartView message handlers
//-------------------------------------------------------------------------//
void CJETChartView::PreInitial()
{
	m_BKChartDC = NULL;
	m_BKChartBitmap = NULL;
	m_BKRect.left = 0;
	m_BKRect.top = 0;
	m_BKRect.right = 0;
	m_BKRect.bottom = 0;
}
//-------------------------------------------------------------------------//
void CJETChartView::ReleaseAll()
{
	//m_chart.ReleaseChart();
	ReleaseBKDC();
}
//-------------------------------------------------------------------------//
void CJETChartView::CopyChartData(const CJETChartView *chart)
{
	this->m_chart.CopyChartData(&chart->m_chart);
}
//-------------------------------------------------------------------------//
bool CJETChartView::SaveChart(LPCTSTR lpFileName)
{
	return m_chart.SaveChart(lpFileName);
}
//-------------------------------------------------------------------------//
Font_ST& CJETChartView::GetFrameFont()
{
	return m_chart.GetFrameFont();
}
//-------------------------------------------------------------------------//
Font_ST& CJETChartView::GetTitleFont()
{
	return m_chart.GetTitleFont();
}
//-------------------------------------------------------------------------//
Font_ST& CJETChartView::GetInfoFont()
{
	return m_chart.GetInfoFont();
}
//-------------------------------------------------------------------------//
void CJETChartView::SetBKColor(COLORREF color)
{
	m_chart.SetBKColor(color);	
}
//-------------------------------------------------------------------------//
void CJETChartView::SetFrameColor(COLORREF color)
{
	m_chart.SetFrameColor(color);	
}
//-------------------------------------------------------------------------//
void CJETChartView::SetTitleColor(COLORREF color)
{
	m_chart.SetTitleColor(color);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetGridColor(COLORREF color)
{
	m_chart.SetGridColor(color);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetChartType(const int type)
{
	m_chart.SetChartType(type);
}
//-------------------------------------------------------------------------//
int CJETChartView::GetChartType()
{
	return m_chart.GetChartType();
}
//-------------------------------------------------------------------------//
void CJETChartView::Set3DThicness(const int Thickness)
{
	m_chart.Set3DThicness(Thickness);
}
//-------------------------------------------------------------------------//
void CJETChartView::AddSeries(CJetSeries *pSeries)
{
	m_chart.AddSeries(pSeries);
}
//-------------------------------------------------------------------------//
void CJETChartView::AddIndexLine(int index, COLORREF FontColor, COLORREF LineColor)			//chia 1031002
{
	m_chart.AddIndexLine(index, FontColor, LineColor);
}
//-------------------------------------------------------------------------//
void CJETChartView::AddIndexLine(int index, const char*text, COLORREF FontColor, COLORREF LineColor)	//chia 1031002
{
	m_chart.AddIndexLine(index, text, FontColor, LineColor);
}
//-------------------------------------------------------------------------//
void CJETChartView::Clear()
{
	m_chart.Clear();
}
//-------------------------------------------------------------------------//
void CJETChartView::BuildChart()
{	
	//CClientDC dc(this);
	//HDC hDC = dc.GetSafeHdc();	
	CDC *pDC = this->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	if( hDC == NULL ) { return; }

	if( m_chart.GetBKDC() == NULL ) 
	{ 		
		this->CreateBKDC(); 
		ReleaseDC(pDC);
		return;
	}

	m_chart.BuildChart();
	ReleaseDC(pDC);
	this->OnPaint();
}
//-------------------------------------------------------------------------//
HDC CJETChartView::GetBKDC()
{
	return m_BKChartDC;
}
//-------------------------------------------------------------------------//
HBITMAP CJETChartView::GetBKBitmap()
{
	return m_BKChartBitmap;
}
//-------------------------------------------------------------------------//
void CJETChartView::SetPanelID(const int id)
{
	m_chart.SetPanelID(id);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetBoardID(const int id)
{
	m_chart.SetBoardID(id);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetComponentName(LPCSTR pName)
{
	m_chart.SetComponentName(pName);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetComponentName(LPCWSTR pName)
{
	m_chart.SetComponentName(pName);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetTitle(LPCSTR pTitle)
{
	m_chart.SetTitle(pTitle);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetTitle(LPCWSTR pTitle)
{
	m_chart.SetTitle(pTitle);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetInfoStr(LPCTSTR str1, LPCTSTR str2, LPCTSTR str3, LPCTSTR str4)	//chia 1050517
{
	m_chart.SetInfoStr(str1, str2, str3, str4);
}
//-------------------------------------------------------------------------//

void CJETChartView::SetIsShowGrid(const bool IsShow)
{
	m_chart.SetIsShowGrid(IsShow);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetIsShowCurrentLine(const bool IsShow)
{
	m_chart.SetIsShowCurrentLine(IsShow);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetIsShowLegend(const bool IsVisible)
{
	m_chart.SetIsShowLegend(IsVisible);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetInfoLocationMode(const int Mode)//Info Mode - Kai-20250320
{
	m_chart.SetInfoLocationMode(Mode);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetLegendLocationMode(const int Mode)	//Legend Mode - Vic 20161108
{
	m_chart.SetLegendLocationMode(Mode);
}
//-------------------------------------------------------------------------//
bool CJETChartView::GetIsShowCurrentLine()
{
	return m_chart.GetIsShowCurrentLine();
}
//-------------------------------------------------------------------------//
bool CJETChartView::GetIsShowLegend()
{
	return m_chart.GetIsShowLegend();
}
//-------------------------------------------------------------------------//
void CJETChartView::SetXAxisLabelMode(const int mode)
{
	m_chart.SetXAxisLabelMode(mode);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetXAxisUnit(LPCSTR unit)
{
	m_chart.SetXAxisUnit(unit);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetXAxisUnit(LPCWSTR  unit)
{
	m_chart.SetXAxisUnit(unit);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisUnit(LPCSTR unit)
{
	m_chart.SetYAxisUnit(unit);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisUnit(LPCWSTR  unit)
{
	m_chart.SetYAxisUnit(unit);
}
//-------------------------------------------------------------------------//
LPCTSTR CJETChartView::GetXAxisUnit()
{
	return m_chart.GetXAxisUnit();
}
//-------------------------------------------------------------------------//
LPCTSTR CJETChartView::GetYAxisUnit()
{
	return m_chart.GetYAxisUnit();
}
//-------------------------------------------------------------------------//
int CJETChartView::GetXIndex(const int PosX)
{
	return m_chart.GetXIndex(PosX);
}
//-------------------------------------------------------------------------//
float CJETChartView::GetYAxisValue(const int PosY)
{
	return m_chart.GetYAxisValue(PosY);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisFix(bool IsFix)
{
	m_chart.SetYAxisFix(IsFix);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisMin(float value)
{
	m_chart.SetYAxisMin(value);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisMax(float value)
{
	m_chart.SetYAxisMax(value);
}
//-------------------------------------------------------------------------//
bool CJETChartView::GetYAxisFix()
{
	return m_chart.GetYAxisFix();
}
//-------------------------------------------------------------------------//
float CJETChartView::GetYAxisMin()
{
	return m_chart.GetYAxisMin();
}
//-------------------------------------------------------------------------//
float CJETChartView::GetYAxisMax()
{
	return m_chart.GetYAxisMax();
}
//-------------------------------------------------------------------------//
void CJETChartView::SetZoomRoi(RECT Rect)
{
	m_chart.SetZoomRoi(Rect);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetOffset(const int OffsetX, const int OffsetY)
{
	m_chart.SetOffset(OffsetX, OffsetY);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetSeriesTopSpace(const int space)
{
	m_chart.SetSeriesTopSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetSeriesBottomSpace(const int space)
{
	m_chart.SetSeriesBottomSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetSeriesLeftSpace(const int space)
{
	m_chart.SetSeriesLeftSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetSeriesRightSpace(const int space)
{
	m_chart.SetSeriesRightSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetChartTopSpace(const int space)
{
	m_chart.SetChartTopSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetChartBottomSpace(const int space)
{
	m_chart.SetChartBottomSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetChartLeftSpace(const int space)
{
	m_chart.SetChartLeftSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetChartRightSpace(const int space)
{
	m_chart.SetChartRightSpace(space);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetIsIntYValue(const bool IsINT)
{
	m_chart.SetIsIntYValue(IsINT);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetTitleFont(Font_ST font)
{
	m_chart.SetTitleFont(font);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetFrameFont(Font_ST font)
{
	m_chart.SetFrameFont(font);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetInfoFont(Font_ST font)	//chia 1050517
{
	m_chart.SetInfoFont(font);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetRadarCenter(float Cx, float Cy)
{
	m_chart.SetRadarCenter(Cx, Cy);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetRadarRadius(float radius)
{
	m_chart.SetRadarRadius(radius);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisPercentageVisible(bool bShow)
{
	m_chart.SetYAxisPercentageVisible(bShow);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetYAxisPercentageTarget(float fValue)
{
	m_chart.SetYAxisPercentageTarget(fValue);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetXAxisGribLineVisible(bool bShow)
{
	m_chart.SetXAxisGribLineVisible(bShow);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetXAxisGribLineTarget(std::vector<int> Value)
{
	m_chart.SetXAxisGribLineTarget(Value);
}
//-------------------------------------------------------------------------//
void CJETChartView::SetIsTranspose(const bool Transpose)
{
	m_chart.SetIsTranspose(Transpose);
}
//-------------------------------------------------------------------------//
void CJETChartView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Drawing();
	
	// Do not call CStatic::OnPaint() for painting messages
}
//-------------------------------------------------------------------------//
void CJETChartView::ReleaseBKDC()
{
	if( m_BKChartDC != NULL ) { ::DeleteDC(m_BKChartDC); }
	if( m_BKChartBitmap != NULL ) { ::DeleteObject(m_BKChartBitmap); }
	
	m_BKChartDC = NULL;
	m_BKChartBitmap = NULL;
	m_BKRect.left = 0;
	m_BKRect.top = 0;
	m_BKRect.right = 0;
	m_BKRect.bottom = 0;
}
//-------------------------------------------------------------------------//
void CJETChartView::CreateBKDC()
{
	ReleaseBKDC();
	RECT Rect = {0,0,0,0};
	this->GetClientRect(&Rect);	
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();	
	if( hDC == NULL ) { return; }

	m_BKChartDC = ::CreateCompatibleDC(hDC);
	m_BKChartBitmap = ::CreateCompatibleBitmap(hDC, Rect.right, Rect.bottom);
	::SelectObject(m_BKChartDC, m_BKChartBitmap);
	m_BKRect = Rect;

	m_chart.SetDrawHdc(m_BKChartDC, m_BKRect);
	BuildChart();
}
//-------------------------------------------------------------------------//
void CJETChartView::Drawing() 
{
	RECT Rect = {0,0,0,0};
	this->GetClientRect(&Rect);	
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();	
	HDC hBKDC = m_BKChartDC;
	if( m_BKChartDC != NULL )
	{
		::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hBKDC, 0, 0, SRCCOPY);
	}
	else
	{
		HBRUSH Brush = ::CreateSolidBrush(0x55ffff);
		HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

		::FillRect(hDC, &Rect, Brush);

		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
	}
}
//-------------------------------------------------------------------------//
void CJETChartView::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	this->ReleaseAll();
}
//-------------------------------------------------------------------------//
void CJETChartView::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->CreateBKDC();
}
//-------------------------------------------------------------------------//
int CJETChartView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	
	this->CreateBKDC();		//chia 1030820
	return 0;
}
//-------------------------------------------------------------------------//
void CJETChartView::OnRButtonUp(UINT nFlags, CPoint point)	//chia 1040907
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	POINT pt = point;
	this->ClientToScreen(&pt);
	::ScreenToClient(this->GetParent()->GetSafeHwnd(), &pt);
	::SendMessage(this->GetParent()->m_hWnd,WM_RBUTTONUP, MAKEWPARAM(nFlags, 0) ,MAKELPARAM(pt.x,pt.y));
	
	CStatic::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------//
