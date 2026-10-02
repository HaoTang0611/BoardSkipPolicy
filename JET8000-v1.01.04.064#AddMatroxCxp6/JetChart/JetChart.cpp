// JetChart.cpp: implementation of the CJetChart class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\stdafx.h"
#include "JetChart.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------//
CJetChart::CJetChart()
{
	PreInitialChart();
}
//--------------------------------------------------------------------------------//
CJetChart::~CJetChart()
{
	ReleaseChart();
}
//--------------------------------------------------------------------------------//
void CJetChart::ReleaseChart()
{
	Clear();
	m_DrawDC = NULL;
	m_DrawBitmap = NULL;

//	if( m_DrawDC != NULL )
//	{ ::DeleteObject(m_DrawDC); m_DrawDC=NULL; } 

//	if( m_DrawBitmap != NULL )
//	{ ::DeleteObject(m_DrawBitmap); m_DrawBitmap=NULL; } 
	
}
//--------------------------------------------------------------------------------//
void CJetChart::Clear()
{
	m_OffsetX = 0;
	m_OffsetY = 0;
	m_3DThickness = 0;
	ReleaseAllSeries();
	ClearIndexLineList();
}
//--------------------------------------------------------------------------------//
void CJetChart::PreInitialChart()
{
	m_FrameColor = JET_CHART_FRAME_COLOR;
	m_BKColor = JET_CHART_BK_COLOR;
	m_TitleColor = JET_CHART_TITLE_COLOR;
	m_GridColor = JET_CHART_GRID_COLOR;

	m_IsTranspose = false; 

	RECT Rect		= {0,0,0,0};
	m_DrawDC		= NULL;
	m_DrawBitmap	= NULL;
	m_DrawRect		= Rect;
	m_ChartRect		= Rect;
	m_SeriesRect	= Rect;
	m_LegendRect	= Rect;
	m_ChartType		= CHART_TYPE_LINE;

	m_IsZoom = false;
	m_IsFixYAxis = false; 
	m_YAxisFixMin = 0;
	m_YAxisFixMax = 100;

	m_3DThickness = 0;
	m_IsIntYValue = false;

	m_RadarCXValue = 0;
	m_RadarCYValue = 0;
	m_RadarRadus = -1;

	m_OffsetX = 0;
	m_OffsetY = 0;

	m_pSeries1	= NULL;
	m_pSeries2	= NULL;
	m_pSeries3	= NULL;
	m_pSeries4	= NULL;
	m_pSeries5	= NULL;
	m_pSeries6	= NULL;
	m_pSeries7	= NULL;
	m_pSeries8	= NULL;
	m_pSeries9	= NULL;
	m_pSeries10	= NULL;
	m_pSeries11	= NULL;
	m_pSeries12	= NULL;

	_stprintf(m_InfoStr01, _T(""));	//chia 1050517
	_stprintf(m_InfoStr02, _T(""));	//chia 1050517
	_stprintf(m_InfoStr03, _T(""));	//chia 1050517
	_stprintf(m_InfoStr04, _T(""));	

	m_Panel = -1;
	m_Board = -1;
	m_ComponentName = "";
	m_Title = "";
	m_ChartTopSpace			= 60;
	m_ChartBottomSpace		= 40;
	m_ChartLeftSpace		= 70;
	m_ChartRightSpace		= 50;

	m_SeriesTopSpace		= 30;
	m_SeriesBottomSpace		= 30;
	m_SeriesLeftSpace		= 30;
	m_SeriesRightSpace		= 30;
	
	m_MinXIndex = 0;
	m_MaxXIndex = 0;
	m_MinYValue = 0;
	m_MaxYValue = 0;

	m_LegendVisible = false;
	m_DrawCurrentLine = false;
	m_ShowGridLine = true;

	m_ChartW = 6;
	m_ChartH = 6;
	m_LegendSpace = 10;
	
	m_XAxisLabelMode = JET_CHART_LABEL_MODE_NONE;
	m_XAxisUnit = "";	
	m_YAxisUnit = "";

	m_PieColorList[0] = 0xaaaaff;
	m_PieColorList[1] = 0x00ffff;
	m_PieColorList[2] = 0xaaffaa;
	m_PieColorList[3] = 0xffffaa;
	m_PieColorList[4] = 0xffaaaa;
	m_PieColorList[5] = 0xffaaff;
	m_PieColorList[6] = 0xffffff;
	m_PieColorList[7] = 0xaaaaaa;

	m_PieColorList[8] = 0x0000ff;
	m_PieColorList[9] = 0x00aaff;
	m_PieColorList[10] = 0xAAffff;
	m_PieColorList[11] = 0x00ff00;
	m_PieColorList[12] = 0xffff00;
	m_PieColorList[13] = 0xff0000;
	m_PieColorList[14] = 0xff00aa;
	m_PieColorList[15] = 0xff00ff;
	m_PieColorList[16] = 0x000000;

	m_PieColorList[17] = 0x000088;
	m_PieColorList[18] = 0x008800;
	m_PieColorList[19] = 0x880000;

	m_FrameFont.sFontSize = 14;
	m_FrameFont.sIsBold = true;
	m_FrameFont.sIsItalic = true;
	m_FrameFont.sIsUnderline = false;
	m_FrameFont.sFontColor = 0x000000;
	m_FrameFont.sEscapement = 0;
	_stprintf(m_FrameFont.sFontType, _T("Times New Roman"));

	m_TitleFont.sFontSize = 14;
	m_TitleFont.sIsBold = false;
	m_TitleFont.sIsItalic = false;
	m_TitleFont.sIsUnderline = false;
	m_TitleFont.sFontColor = JET_CHART_TITLE_FONT_COLOR;
	m_TitleFont.sEscapement = 0;
	_stprintf(m_TitleFont.sFontType, _T("Times New Roman"));	
	
	//chia 1050517
	m_InfoFont.sFontSize = 12;
	m_InfoFont.sIsBold = false;
	m_InfoFont.sIsItalic = false;
	m_InfoFont.sIsUnderline = false;
	m_InfoFont.sFontColor = JET_CHART_INFO_FONT_COLOR;
	m_InfoFont.sEscapement = 0;
	_stprintf(m_InfoFont.sFontType, _T("Times New Roman"));	
	
	m_InfoLocationMode = JET_CHART_INFO_LOCATION_MODE_RIGHT;
	m_LegendLocationMode = JET_CHART_LENGEND_LOCATION_MODE_BOTTOM;//Legend Mode - Vic 20161108

	m_YAxisPercentageVisible = false;//Show Y-Axis Percentage - Kai-20220805
	m_YAxisPercentageTarget = -1;//Y-Axis Percentage Target - Kai-20220805
	ClearIndexLineList();
}
//--------------------------------------------------------------------------------//
void CJetChart::ReleaseAllSeries()
{
	if ( NULL != m_pSeries1 )
	{	delete m_pSeries1; m_pSeries1 = NULL; }
	if ( NULL != m_pSeries2 )
	{	delete m_pSeries2; m_pSeries2 = NULL; }
	if ( NULL != m_pSeries3 )
	{	delete m_pSeries3; m_pSeries3 = NULL; }
	if ( NULL != m_pSeries4 )
	{	delete m_pSeries4; m_pSeries4 = NULL; }
	if ( NULL != m_pSeries5 )
	{	delete m_pSeries5; m_pSeries5 = NULL; }
	if ( NULL != m_pSeries6 )
	{	delete m_pSeries6; m_pSeries6 = NULL; }
	if ( NULL != m_pSeries7 )
	{	delete m_pSeries7; m_pSeries7 = NULL; }
	if ( NULL != m_pSeries8 )
	{	delete m_pSeries8; m_pSeries8 = NULL; }
	if ( NULL != m_pSeries9 )
	{	delete m_pSeries9; m_pSeries9 = NULL; }
	if ( NULL != m_pSeries10 )
	{	delete m_pSeries10; m_pSeries10 = NULL; }
	if ( NULL != m_pSeries11 )
	{	delete m_pSeries11; m_pSeries11 = NULL; }
	if ( NULL != m_pSeries2 )
	{	delete m_pSeries12; m_pSeries12 = NULL; }

	m_YAxisPercentageTarget = -1;
}
//--------------------------------------------------------------------------------//
const CJetChart& CJetChart::operator =( const CJetChart &chart)
{
	if( this == &chart ) { return *this; }
	CloneChart(&chart);
	return *this;
}
//--------------------------------------------------------------------------------//
bool CJetChart::SaveChart(LPCTSTR lpFileName)
{
	CString ErrStr = _T("");
	HBITMAP hBitmap = m_DrawBitmap;
		
	HDC hDC; 
	//當前顯示分辨率下每個像素所占字節數
	int iBits = 0; 
	//位圖中每個像素所占字節數
	WORD wBitCount = 8; 
	//定義調色板大小， 位圖中像素字節大小 ， 位圖文件大小 
	DWORD dwPaletteSize=0, dwBmBitsSize=0, m_dwDibSize=0;

	//位圖屬性結構
	BITMAP Bitmap; 
	//位圖信息頭結構 
	BITMAPINFOHEADER bi; 
	//調色板句柄
	HANDLE hPal,hOldPal=NULL;

	//計算位圖文件每個像素所占字節數
/*	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
	{ wBitCount = 1; }
	else if (iBits <= 4)
	{ wBitCount = 4; }
	else if (iBits <= 8)
	{ wBitCount = 8; }
	else if (iBits <= 24)
	{ wBitCount = 24; }*/
	//計算調色板大小
	if (wBitCount <= 8)
	{ dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); }

	//設置位圖信息頭結構
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32)* 4 *Bitmap.bmHeight ;
	//為位圖內容分配內存

	BITMAPINFO *pBmiImage = NULL;
	m_dwDibSize = sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	pBmiImage = (BITMAPINFO *)new unsigned char[m_dwDibSize];
	pBmiImage->bmiHeader = bi;

	// 處理調色板 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	unsigned char *pBitArray = (unsigned char*)pBmiImage;
	pBitArray = pBitArray + sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//獲取該調色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, pBitArray , pBmiImage, DIB_RGB_COLORS);

	//恢复調色板 
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	CFile cf;
	if( !cf.Open( lpFileName, CFile::modeCreate | CFile::modeWrite ) )
	{
		delete[] pBmiImage; pBmiImage=NULL;	
		ErrStr.Format(_T("Open File Fault (%s)"), lpFileName);
		::AfxMessageBox(ErrStr);		
		return false;
	}
	
	// Write the data.
	try
	{
		BITMAPFILEHEADER BFH;
		memset( &BFH, 0, sizeof( BITMAPFILEHEADER ) );
		BFH.bfType = 'MB';
		BFH.bfSize = sizeof( BITMAPFILEHEADER ) + m_dwDibSize;
		BFH.bfReserved1 = 0;
		BFH.bfReserved2 = 0;
		BFH.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + dwPaletteSize;
		cf.Write( &BFH, sizeof( BITMAPFILEHEADER ) );
		cf.Write( pBmiImage, m_dwDibSize );
	}
	catch( CFileException *e )
	{
		e->Delete();
		cf.Close();
		delete[] pBmiImage; pBmiImage=NULL;	
		ErrStr.Format(_T("Open File Fault (%s)"), lpFileName);
		::AfxMessageBox(ErrStr);			
		return false ;
	}
	cf.Close();
	delete[] pBmiImage; pBmiImage=NULL;	

	return true;
}
//--------------------------------------------------------------------------------//
void CJetChart::ClearIndexLineList()
{
	int i=0;
	PIndexLine_ST pIndeLine = NULL;
	for( i=0 ; i<MAX_INDEX_LINE ; i++ )	
	{
		pIndeLine = &m_IndexLineList[i];
		pIndeLine->sEnabled = false;
		pIndeLine->sIndex = -1;
		pIndeLine->sFontColor = 0x000000;
		pIndeLine->sLineColor = 0x000000;		
		sprintf(pIndeLine->sText, "");
	}
}
//--------------------------------------------------------------------------------//
void CJetChart::CopyChartData(const CJetChart *chart)
{
	ReleaseAllSeries();

	int i=0;
	for( i=0 ; i<MAX_INDEX_LINE ; i++ )	
	{
		m_IndexLineList[i] = chart->m_IndexLineList[i];
	}
	
	AddSeries(chart->m_pSeries1, this->m_pSeries1);
	AddSeries(chart->m_pSeries2, this->m_pSeries2);
	AddSeries(chart->m_pSeries3, this->m_pSeries3);
	AddSeries(chart->m_pSeries4, this->m_pSeries4);
	AddSeries(chart->m_pSeries5, this->m_pSeries5);
	AddSeries(chart->m_pSeries6, this->m_pSeries6);
	AddSeries(chart->m_pSeries7, this->m_pSeries7);
	AddSeries(chart->m_pSeries8, this->m_pSeries8);
	AddSeries(chart->m_pSeries9, this->m_pSeries9);
	AddSeries(chart->m_pSeries10, this->m_pSeries10);
	AddSeries(chart->m_pSeries11, this->m_pSeries11);
	AddSeries(chart->m_pSeries12, this->m_pSeries12);

	this->m_IsTranspose = chart->m_IsTranspose;
	this->m_ChartType = chart->m_ChartType;
	this->m_LegendVisible = chart->m_LegendVisible;
	
	this->m_ChartRect = chart->m_ChartRect;
	this->m_SeriesRect = chart->m_SeriesRect;
	this->m_LegendRect = chart->m_LegendRect;

	this->m_MinXIndex = chart->m_MinXIndex;
	this->m_MaxXIndex = chart->m_MaxXIndex;
	this->m_MinYValue = chart->m_MinYValue;
	this->m_MaxYValue = chart->m_MaxYValue;

	this->m_SeriesRatioX = chart->m_SeriesRatioX;
	this->m_SeriesRatioY = chart->m_SeriesRatioY;

	this->m_ChartTopSpace = chart->m_ChartTopSpace;
	this->m_ChartBottomSpace = chart->m_ChartBottomSpace;
	this->m_ChartLeftSpace = chart->m_ChartLeftSpace;
	this->m_ChartRightSpace = chart->m_ChartRightSpace;

	this->m_SeriesTopSpace = chart->m_SeriesTopSpace;
	this->m_SeriesBottomSpace = chart->m_SeriesBottomSpace;
	this->m_SeriesLeftSpace = chart->m_SeriesLeftSpace;
	this->m_SeriesRightSpace = chart->m_SeriesRightSpace;

	this->m_LegendSpace = chart->m_LegendSpace;
	this->m_ChartW = chart->m_ChartW;
	this->m_ChartH = chart->m_ChartH;

	this->m_Panel = chart->m_Panel;
	this->m_Board = chart->m_Board;
	this->m_ComponentName = chart->m_ComponentName;
	this->m_LegendVisible = chart->m_LegendVisible;
	this->m_DrawCurrentLine = chart->m_DrawCurrentLine;
	this->m_ShowGridLine = chart->m_ShowGridLine;

	//chia 1050517
	_tcscpy(m_InfoStr01, chart->m_InfoStr01);
	_tcscpy(m_InfoStr02, chart->m_InfoStr02);
	_tcscpy(m_InfoStr03, chart->m_InfoStr03);
	_tcscpy(m_InfoStr04, chart->m_InfoStr04);
	this->m_Title = chart->m_Title;
	this->m_XAxisUnit = chart->m_XAxisUnit;	
	this->m_YAxisUnit = chart->m_YAxisUnit;	
	
	this->m_BKColor = chart->m_BKColor;	
	this->m_FrameColor = chart->m_FrameColor;	
	this->m_TitleColor = chart->m_TitleColor;
	this->m_GridColor = chart->m_GridColor;
	::memcpy(this->m_PieColorList, chart->m_PieColorList, sizeof(m_PieColorList));

	
	this->m_OffsetX = chart->m_OffsetX;	
	this->m_OffsetY = chart->m_OffsetY;	

	this->m_SeriesRatioX = chart->m_SeriesRatioX;	
	this->m_SeriesRatioY = chart->m_SeriesRatioY;	
	
	this->m_PieLegendItemBlockW = chart->m_PieLegendItemBlockW;	
	this->m_PieLegendItemLength = chart->m_PieLegendItemLength;	
	this->m_PieLegendItemHeight = chart->m_PieLegendItemHeight;	
	this->m_PieLegendNLine = chart->m_PieLegendNLine;	
	this->m_PieLegendNItemPerLine = chart->m_PieLegendNItemPerLine;	

	this->m_IsFixYAxis = chart->m_IsFixYAxis;
	this->m_YAxisFixMin = chart->m_YAxisFixMin;	
	this->m_YAxisFixMax = chart->m_YAxisFixMax;	

	this->m_SeiresValueSum = chart->m_SeiresValueSum;	

	this->m_XAxisLabelMode = chart->m_XAxisLabelMode;

	this->m_ZoomMinXIndex = chart->m_ZoomMinXIndex;
	this-> m_ZoomMaxXIndex = chart->m_ZoomMaxXIndex;
	this->m_ZoomMinYValue = chart->m_ZoomMinYValue;
	this->m_ZoomMaxYValue = chart->m_ZoomMaxYValue;
	this->m_IsZoom = chart->m_IsZoom;
	this->m_3DThickness = chart->m_3DThickness;

	this->m_TitleFont = chart->m_TitleFont;
	this->m_FrameFont = chart->m_FrameFont;
	this->m_IsIntYValue = chart->m_IsIntYValue;
	
	this->m_RadarCXValue = chart->m_RadarCXValue;
	this->m_RadarCYValue = chart->m_RadarCYValue;
	this->m_RadarRadus = chart->m_RadarRadus;

	this->m_YAxisPercentageVisible = chart->m_YAxisPercentageVisible;
	this->m_YAxisPercentageTarget = chart->m_YAxisPercentageTarget;	
}
//--------------------------------------------------------------------------------//
void CJetChart::CloneChart(const CJetChart *chart)
{
	this->ReleaseChart();
	CopyChartData(chart);

	this->m_DrawDC = chart->m_DrawDC;
	this->m_DrawBitmap = chart->m_DrawBitmap;
	this->m_DrawRect = chart->m_DrawRect;
/*
	AddSeries(chart->m_pSeries1, this->m_pSeries1);
	AddSeries(chart->m_pSeries2, this->m_pSeries2);
	AddSeries(chart->m_pSeries3, this->m_pSeries3);
	AddSeries(chart->m_pSeries4, this->m_pSeries4);
	AddSeries(chart->m_pSeries5, this->m_pSeries5);
	AddSeries(chart->m_pSeries6, this->m_pSeries6);
	AddSeries(chart->m_pSeries7, this->m_pSeries7);
	AddSeries(chart->m_pSeries8, this->m_pSeries8);
	AddSeries(chart->m_pSeries9, this->m_pSeries9);
	AddSeries(chart->m_pSeries10, this->m_pSeries10);
	AddSeries(chart->m_pSeries11, this->m_pSeries11);
	AddSeries(chart->m_pSeries12, this->m_pSeries12);

	this->m_ChartType = chart->m_ChartType;
	this->m_LegendVisible = chart->m_LegendVisible;
	
	this->m_ChartRect = chart->m_ChartRect;
	this->m_SeriesRect = chart->m_SeriesRect;
	this->m_LegendRect = chart->m_LegendRect;

	this->m_MinXIndex = chart->m_MinXIndex;
	this->m_MaxXIndex = chart->m_MaxXIndex;
	this->m_MinYValue = chart->m_MinYValue;
	this->m_MaxYValue = chart->m_MaxYValue;

	this->m_SeriesRatioX = chart->m_SeriesRatioX;
	this->m_SeriesRatioY = chart->m_SeriesRatioY;

	this->m_ChartTopSpace = chart->m_ChartTopSpace;
	this->m_ChartBottomSpace = chart->m_ChartBottomSpace;
	this->m_ChartLeftSpace = chart->m_ChartLeftSpace;
	this->m_ChartRightSpace = chart->m_ChartRightSpace;

	this->m_SeriesTopSpace = chart->m_SeriesTopSpace;
	this->m_SeriesBottomSpace = chart->m_SeriesBottomSpace;
	this->m_SeriesLeftSpace = chart->m_SeriesLeftSpace;
	this->m_SeriesRightSpace = chart->m_SeriesRightSpace;

	this->m_LegendSpace = chart->m_LegendSpace;
	this->m_ChartW = chart->m_ChartW;
	this->m_ChartH = chart->m_ChartH;

	this->m_Panel = chart->m_Panel;
	this->m_Board = chart->m_Board;
	this->m_ComponentName = chart->m_ComponentName;
	this->m_LegendVisible = chart->m_LegendVisible;
	this->m_DrawCurrentLine = chart->m_DrawCurrentLine;

	this->m_XAxisUnit = chart->m_XAxisUnit;	
	this->m_YAxisUnit = chart->m_YAxisUnit;

	this->m_YAxisPercentageVisible = chart->m_YAxisPercentageVisible;
	this->m_YAxisPercentageTarget = chart->m_YAxisPercentageTarget;	
	*/
}
//--------------------------------------------------------------------------------//
void CJetChart::SetXAxisUnit(LPCSTR unit)
{
	this->m_XAxisUnit = unit;
}
//----------------------------------------------------------------------------//
void CJetChart::SetXAxisUnit(LPCWSTR  unit)
{
	this->m_XAxisUnit = unit;
}
//----------------------------------------------------------------------------//
LPCTSTR  CJetChart::GetXAxisUnit()
{
	return this->m_XAxisUnit;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisUnit(LPCSTR unit)
{
	this->m_YAxisUnit = unit;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisUnit(LPCWSTR  unit)
{
	this->m_YAxisUnit = unit;
}
//----------------------------------------------------------------------------//
LPCTSTR  CJetChart::GetYAxisUnit()
{
	return this->m_YAxisUnit;
}
//----------------------------------------------------------------------------//
void CJetChart::Set3DThicness(const int Thickness)
{
	m_3DThickness = Thickness;
}
//----------------------------------------------------------------------------//
void CJetChart::SetChartType(const int type)
{
	this->m_ChartType = type;
	if( type == CHART_TYPE_PIE )
	{
		m_ChartLeftSpace		= 80;
		m_ChartRightSpace		= 80;
	}
	else if( type == CHART_TYPE_BAR )//fang 1020618
	{
		m_ChartLeftSpace		= 20;
		m_ChartRightSpace       = 20;
	}
	else
	{
		m_ChartLeftSpace		= 70;
		m_ChartRightSpace		= 50;
	}
}
//----------------------------------------------------------------------------//
int CJetChart::GetChartType()
{
	return this->m_ChartType;
}
//----------------------------------------------------------------------------//
void CJetChart::AddIndexLine(int index, COLORREF FontColor, COLORREF LineColor)			//chia 1031002
{	
	int i=0;
	PIndexLine_ST pIndexLine = NULL;
	for( i=0 ; i<MAX_INDEX_LINE ; i++)
	{
		pIndexLine = &m_IndexLineList[i];
		if( pIndexLine->sEnabled == true ) { continue; }

		pIndexLine->sEnabled = true;
		pIndexLine->sIndex = index;
		pIndexLine->sFontColor = FontColor;
		pIndexLine->sLineColor = LineColor;
		break;
	}
}
//-------------------------------------------------------------------------//
void CJetChart::AddIndexLine(int index, const char*text, COLORREF FontColor, COLORREF LineColor)	//chia 1031002
{
	int i=0;
	PIndexLine_ST pIndexLine = NULL;
	for( i=0 ; i<MAX_INDEX_LINE ; i++)
	{
		pIndexLine = &m_IndexLineList[i];
		if( pIndexLine->sEnabled == true ) { continue; }

		pIndexLine->sEnabled = true;
		pIndexLine->sIndex = index;
		sprintf(pIndexLine->sText, "%s", text);
		pIndexLine->sFontColor = FontColor;
		pIndexLine->sLineColor = LineColor;
		break;
	}
}
//-------------------------------------------------------------------------//
void CJetChart::AddSeries(CJetSeries *pSeries)
{
	if( pSeries == NULL ) { return; }

	if( m_pSeries1 == NULL )
	{
		AddSeries(pSeries, m_pSeries1);
		return;
	}
	if( m_pSeries2 == NULL )
	{
		AddSeries(pSeries, m_pSeries2);
		return;
	}
	if( m_pSeries3 == NULL )
	{
		AddSeries(pSeries, m_pSeries3);
		return;
	}
	if( m_pSeries4 == NULL )
	{
		AddSeries(pSeries, m_pSeries4);
		return;
	}
	if( m_pSeries5 == NULL )
	{
		AddSeries(pSeries, m_pSeries5);
		return;
	}
	if( m_pSeries6 == NULL )
	{
		AddSeries(pSeries, m_pSeries6);
		return;
	}
	if( m_pSeries7 == NULL )
	{
		AddSeries(pSeries, m_pSeries7);
		return;
	}
	if( m_pSeries8 == NULL )
	{
		AddSeries(pSeries, m_pSeries8);
		return;
	}	
	if( m_pSeries9 == NULL )
	{
		AddSeries(pSeries, m_pSeries9);
		return;
	}	
	if( m_pSeries10 == NULL )
	{
		AddSeries(pSeries, m_pSeries10);
		return;
	}	
	if( m_pSeries11 == NULL )
	{
		AddSeries(pSeries, m_pSeries11);
		return;
	}	
	if( m_pSeries12 == NULL )
	{
		AddSeries(pSeries, m_pSeries12);
		return;
	}	
}
//----------------------------------------------------------------------------//
void CJetChart::AddSeries(CJetSeries *pSrcSeries, CJetSeries *&pDestSeries)
{
	if( pSrcSeries == NULL ) { return; }
	if( pDestSeries == NULL )
	{ pDestSeries = new CJetSeries(); }

	*pDestSeries = *pSrcSeries;
}
//----------------------------------------------------------------------------//
void CJetChart::SetDrawHdc(HDC hdc, RECT Rect)//fang 1020624
{
	//chia charat
	this->m_DrawRect = Rect;
	this->m_DrawDC = hdc;
	/*if( m_DrawDC == NULL )
	{
		this->m_DrawRect = Rect;
		this->m_DrawDC = ::CreateCompatibleDC(hdc);
		this->m_DrawBitmap = ::CreateCompatibleBitmap(hdc, Rect.right, Rect.bottom);
		::SelectObject(m_DrawDC, m_DrawBitmap);			
	}
	else
	{
		bool IsCreateNew = false;
		if( Rect.left != m_DrawRect.left )		{ IsCreateNew = true; }
		if( Rect.right != m_DrawRect.right )	{ IsCreateNew = true; }
		if( Rect.top != m_DrawRect.top )		{ IsCreateNew = true; }
		if( Rect.bottom != m_DrawRect.bottom )	{ IsCreateNew = true; }
		
		if( IsCreateNew == true )
		{
			if( m_DrawDC != NULL )
			{ ::DeleteObject(m_DrawDC); m_DrawDC=NULL; } 
			
			if( m_DrawBitmap != NULL )
			{ ::DeleteObject(m_DrawBitmap); m_DrawBitmap=NULL; } 
			
			this->m_DrawRect = Rect;
			this->m_DrawDC = ::CreateCompatibleDC(hdc);
			this->m_DrawBitmap = ::CreateCompatibleBitmap(hdc, Rect.right, Rect.bottom);
			::SelectObject(m_DrawDC, m_DrawBitmap);			
		}
	}
*/
}
//----------------------------------------------------------------------------//
HDC CJetChart::GetBKDC()
{
	return this->m_DrawDC;
}
//----------------------------------------------------------------------------//
HBITMAP CJetChart::GetBKBitmap()
{
	return this->m_DrawBitmap;
}
//----------------------------------------------------------------------------//
bool CJetChart::CheckDrawTitle() const
{
	const size_t Len = ::_tcslen(m_Title);
	if ( 0 == Len ) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
int CJetChart::GetNVisibleSeries() const
{
	int count = 0;
	if( this->m_pSeries1 != NULL ) { count++; }
	if( this->m_pSeries2 != NULL ) { count++; }
	if( this->m_pSeries3 != NULL ) { count++; }
	if( this->m_pSeries4 != NULL ) { count++; }
	if( this->m_pSeries5 != NULL ) { count++; }
	if( this->m_pSeries6 != NULL ) { count++; }
	if( this->m_pSeries7 != NULL ) { count++; }
	if( this->m_pSeries8 != NULL ) { count++; }
	if( this->m_pSeries9 != NULL ) { count++; }
	if( this->m_pSeries10 != NULL ) { count++; }
	if( this->m_pSeries11 != NULL ) { count++; }
	if( this->m_pSeries12 != NULL ) { count++; }

	return count;
}
//----------------------------------------------------------------------------//
bool CJetChart::GetIsSeriesExist() const
{
	if( this->m_pSeries1 != NULL ) { return true; }
	if( this->m_pSeries2 != NULL ) { return true; }
	if( this->m_pSeries3 != NULL ) { return true; }
	if( this->m_pSeries4 != NULL ) { return true; }
	if( this->m_pSeries5 != NULL ) { return true; }
	if( this->m_pSeries6 != NULL ) { return true; }
	if( this->m_pSeries7 != NULL ) { return true; }
	if( this->m_pSeries8 != NULL ) { return true; }
	if( this->m_pSeries9 != NULL ) { return true; }
	if( this->m_pSeries10 != NULL ) { return true; }
	if( this->m_pSeries11 != NULL ) { return true; }
	if( this->m_pSeries12 != NULL ) { return true; }
	return false;
}
//----------------------------------------------------------------------------//
void CJetChart::GetRadarRadius(float &Radius)
{
	if( Radius != -1 ) { return; }

	int RadiusX = 0;
	int RadiusY = 0;

	float MinXValue = 999999999.0f;
	float MaxXValue = -999999999.0f;
	float MinYValue = 999999999.0f;
	float MaxYValue = -999999999.0f;

	float TempMinXValue = 999999999.0f;
	float TempMaxXValue = -999999999.0f;
	float TempMinYValue = 999999999.0f;
	float TempMaxYValue = -999999999.0f;

	bool IsEmpty = true;
	if( this->GetSeriesLimitRadarValue(this->m_pSeries1, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
	{
		IsEmpty = false;
		if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
		if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
		if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
		if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
	}

	if( m_ChartType == CHART_TYPE_RADAR )
	{
		if( this->GetSeriesLimitRadarValue(this->m_pSeries2, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries3, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries4, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries5, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries6, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries7, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries8, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries9, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries10, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries11, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
		if( this->GetSeriesLimitRadarValue(this->m_pSeries12, TempMinXValue, TempMaxXValue, TempMinYValue, TempMaxYValue) == true ) 
		{
			IsEmpty = false;
			if( MinXValue > TempMinXValue ) { MinXValue = TempMinXValue; }
			if( MaxXValue < TempMaxXValue ) { MaxXValue = TempMaxXValue; }
			if( MinYValue > TempMinYValue ) { MinYValue = TempMinYValue; }
			if( MaxYValue < TempMaxYValue ) { MaxYValue = TempMaxYValue; }
		}
	}

	if( IsEmpty == true )
	{
		MinXValue = 0.0f;
		MaxXValue = 0.0f;
		MinYValue = 0.0f;
		MaxYValue = 0.0f;
	}

	//RadiusX = (MaxXValue-MinXValue)*0.5;
	//RadiusY = (MaxYValue-MinYValue)*0.5;

	if( MaxXValue > fabs(MinXValue) ) 
	{ RadiusX = static_cast<int>(MaxXValue);}
	else 
	{ RadiusX = static_cast<int>(fabs(MinXValue));}

	if( MaxYValue > fabs(MinYValue) ) 
	{ RadiusY = static_cast<int>(MaxYValue);}
	else 
	{ RadiusY = static_cast<int>(fabs(MinYValue));}

	Radius = static_cast<float>( (RadiusX>RadiusY)?RadiusX:RadiusY );
}
//----------------------------------------------------------------------------//
void CJetChart::GetAllDataLimit(int &MinXIndex, int &MaxXIndex, float &MinYValue, float &MaxYValue)
{
	MinXIndex = 999999999;
	MaxXIndex = -999999999;
	MinYValue = 999999999.0f;
	MaxYValue = -999999999.0f;

	int TempMinIndex = 999999999;
	int TempMaxIndex = -999999999;
	float TempMinValue = 999999999.0f;
	float TempMaxValue = -999999999.0f;

	bool IsEmpty = true;
	if( this->GetSeriesLimitValue(this->m_pSeries1, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
	{
		IsEmpty = false;
		if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
		if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
		if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
		if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
	}
	if( m_ChartType == CHART_TYPE_LINE )
	{
		if( this->GetSeriesLimitValue(this->m_pSeries2, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries3, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries4, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries5, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{			
			IsEmpty = false;		
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries6, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries7, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries8, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries9, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries10, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries11, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}
		if( this->GetSeriesLimitValue(this->m_pSeries12, TempMinIndex, TempMaxIndex, TempMinValue, TempMaxValue) == true ) 
		{
			IsEmpty = false;
			if( MinXIndex > TempMinIndex ) { MinXIndex = TempMinIndex; }
			if( MaxXIndex < TempMaxIndex ) { MaxXIndex = TempMaxIndex; }
			if( MinYValue > TempMinValue ) { MinYValue = TempMinValue; }
			if( MaxYValue < TempMaxValue ) { MaxYValue = TempMaxValue; }
		}	
	}

	if( IsEmpty == true )
	{
		MinXIndex = 0;
		MaxXIndex = 1;
		MinYValue = 0.0f;
		MaxYValue = 1.0f;
	}
}
//----------------------------------------------------------------------------//
int CJetChart::GetLegendWidth()
{
	int Width = 0;
	int TempD = 0;

	CJetSeries *pSeries = NULL;
	pSeries = m_pSeries1;
	if( pSeries != NULL )
	{ TempD = (int)(int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries2;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries3;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries4;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries5;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries6;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries7;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries8;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries9;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries10;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries11;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	pSeries = m_pSeries12;
	if( pSeries != NULL )
	{ TempD = (int)_tcslen(pSeries->GetTitle()); if( TempD > Width ) { Width = TempD; } }

	return Width;
}
//----------------------------------------------------------------------------//
void CJetChart::GetChartRect(RECT *ChartRect, RECT *LegendRect)//fang 1020624
{
	if( m_ChartType == CHART_TYPE_BAR )
	{
		ChartRect->left =	this->m_DrawRect.left		+	10;
		ChartRect->right =	this->m_DrawRect.right		-	10;
		ChartRect->top =	this->m_DrawRect.top		+	25;
		ChartRect->bottom =	this->m_DrawRect.bottom		-	10;
	}
	else
	{
		ChartRect->left =	this->m_DrawRect.left		+	m_ChartLeftSpace;
		ChartRect->right =	this->m_DrawRect.right		-	m_ChartRightSpace;
		ChartRect->top =	this->m_DrawRect.top		+	m_ChartTopSpace;
		ChartRect->bottom =	this->m_DrawRect.bottom		-	m_ChartBottomSpace;
	}

	if( m_LegendVisible == true )
	{
		//Legend Mode - Vic 20161108
		if( m_ChartType == CHART_TYPE_PIE )
		{
			CJetSeries *pSeries = m_pSeries1;
			if( pSeries == NULL )  { return; }

			int VisibleCount = pSeries->GetNVisiblePieItem();
			int Count = pSeries->GetNSeriesNode();

			if( VisibleCount <= 0 ) { return; }

			RECT Rect = {0,0,0,0};

			if ( m_LegendLocationMode == JET_CHART_LENGEND_LOCATION_MODE_BOTTOM )
			{
				Rect.left	=	this->m_DrawRect.left	+	5;
				Rect.right	=	this->m_DrawRect.right	-	5;
				Rect.bottom =	this->m_DrawRect.bottom	-	5;
				int Rw = Rect.right-Rect.left;
				int Rh = Rect.bottom-Rect.top;
				int ItemW = Rw/VisibleCount;
				int ItemBlockW = 10;
				int ItemTextLength = 140;
				int ItemSpace = 10;
				int ItemLength = ItemBlockW+ItemTextLength+ItemSpace;
				
				int nItem = Rw/ItemLength;
				int nLine = (int)ceil((double)VisibleCount/(double)nItem);
				if( nItem > Count ) { nItem = VisibleCount; }
					
				int LineH = 20;
				Rect.top	=	Rect.bottom		-	nLine*LineH;
				
				m_PieLegendItemBlockW = ItemBlockW;
				m_PieLegendItemLength = ItemLength;
				m_PieLegendItemHeight = LineH;
				m_PieLegendNLine = nLine;
				m_PieLegendNItemPerLine = nItem;
				*LegendRect = Rect;

				ChartRect->bottom = Rect.top-30;
			}
			else
			{
				Rect.top	=	this->m_DrawRect.top	+	30;
				Rect.left	=	this->m_DrawRect.left	+	5;
				Rect.right	=	this->m_DrawRect.right	-	5;
				Rect.bottom =	this->m_DrawRect.bottom	-	5;
				int Rw = Rect.right-Rect.left;
				int Rh = Rect.bottom-Rect.top;

				int ItemW = Rw/VisibleCount;
				int ItemBlockW = 10;
				int ItemTextLength = 140;
				int ItemSpace = 10;
				int ItemLength = ItemBlockW+ItemTextLength+ItemSpace;
				
				int nItem = 1;//Rw/ItemLength;
				int nLine = (int)ceil((double)VisibleCount/(double)nItem);
				if( nItem > Count ) { nItem = VisibleCount; }
					
				int LineH = 20;
				Rect.left	=	Rect.right - ItemLength;
				
				m_PieLegendItemBlockW = ItemBlockW;
				m_PieLegendItemLength = ItemLength;
				m_PieLegendItemHeight = LineH;
				m_PieLegendNLine = nLine;
				m_PieLegendNItemPerLine = nItem;
				*LegendRect = Rect;

				ChartRect->right = Rect.left-40;

/*				const int ChartW = m_ChartW;
				const int ChartH = m_ChartH;
				const int Length = this->GetLegendWidth()*ChartW;
				const int LegendW = Length+m_LegendSpace*3;
				const int LegendH = VisibleCount*(m_LegendSpace+ChartH);

*/
			}
		}
		else
		{
			const int NSeries = this->GetNVisibleSeries();
			const int ChartW = m_ChartW;
			const int ChartH = m_ChartH;
			const int Length = this->GetLegendWidth()*ChartW;
			const int LegendW = Length+m_LegendSpace*3;
			const int LegendH = NSeries*(m_LegendSpace+ChartH);

			if( this->m_IsTranspose == true )
			{
				ChartRect->bottom =	this->m_DrawRect.bottom-m_ChartBottomSpace-LegendW;

				LegendRect->right = this->m_DrawRect.right - 50;
				LegendRect->left = LegendRect->right - LegendH;
				LegendRect->top = m_DrawRect.bottom - LegendW-30;	
				LegendRect->bottom = m_DrawRect.bottom - m_LegendSpace;
			}
			else
			{
				ChartRect->right =	this->m_DrawRect.right-m_ChartRightSpace-LegendW;
				LegendRect->left = this->m_DrawRect.right - LegendW-30;
				LegendRect->right = this->m_DrawRect.right - m_LegendSpace;
				LegendRect->top = this->m_DrawRect.top + 50;
				LegendRect->bottom = LegendRect->top + LegendH;	
			}
		}
	}

	if ( true == m_YAxisPercentageVisible )
	{	
		ChartRect->top   += 8;
		ChartRect->right -= 24;	
	}
}
//----------------------------------------------------------------------------//
void CJetChart::GetSeriesRect(RECT *SeriesRect)
{
	RECT ChartRect = {0,0,0,0};
	RECT LegendRect = {0,0,0,0};
	GetChartRect(&ChartRect, &LegendRect);

	SeriesRect->left =		ChartRect.left	 + m_SeriesLeftSpace;
	SeriesRect->top =		ChartRect.top	 + m_SeriesTopSpace;
	SeriesRect->right =		ChartRect.right	 - m_SeriesRightSpace;
	SeriesRect->bottom =	ChartRect.bottom - m_SeriesBottomSpace;
}
//----------------------------------------------------------------------------//
void CJetChart::GetSeriesRatio(float &XRatio, float &YRatio)
{
	int SeriesW = m_SeriesRect.right - m_SeriesRect.left;
	int SeriesH = m_SeriesRect.bottom - m_SeriesRect.top;

	if( this->m_ChartType == CHART_TYPE_RADAR )
	{
		int Length = (SeriesW>SeriesH)?SeriesH:SeriesW;
		Length = static_cast<int>( Length*0.5 );
		XRatio = Length/m_RadarRadus;
		YRatio = XRatio;
		
		if( m_IsZoom == true )
		{
			float TempHeight = 0;
			TempHeight = m_ZoomMaxYValue-m_ZoomMinYValue;
			if( TempHeight <= 0 )
			{ m_IsZoom = false; }	

			if( m_IsZoom == true )
			{
				Length = static_cast<int>(TempHeight);
				Length = static_cast<int>( Length*0.5 );//Length *= 0.5;
				XRatio = Length/m_RadarRadus;
				YRatio = XRatio;
			}
		}

		return;
	}


	int MinXIndex = m_MinXIndex;
	int MaxXIndex = m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;

	int		Width	= abs(MaxXIndex - MinXIndex);
	float	Height	= fabs(MaxYValue - MinYValue);

	if( m_IsFixYAxis == true )
	{
		float TempHeight = 0;
		TempHeight = m_YAxisFixMax - m_YAxisFixMin;
		if( TempHeight <= 0 )
		{ m_IsFixYAxis = false; }
		else
		{ Height = TempHeight; }
	}
	
	if( m_IsZoom == true )
	{
		float TempHeight = 0;
		TempHeight = m_ZoomMaxYValue-m_ZoomMinYValue;
		if( TempHeight <= 0 )
		{ m_IsZoom = false; }	

		int TempWidth = m_ZoomMaxXIndex-m_ZoomMinXIndex;
		if(TempWidth <= 0 )
		{ m_IsZoom = false; }

		if( m_IsZoom == true )
		{
			Height = TempHeight;
			Width = TempWidth;
		}
	}

	if( Width <= 0 )	{ Width = 1; }
	if( Height <= 0.0 ) { Height = 1; }

	if( this->m_IsTranspose )
	{
		XRatio = (float)SeriesH/(float)Width;
		YRatio = (float)SeriesW/(float)Height;
	}
	else
	{
		XRatio = (float)SeriesW/(float)Width;
		YRatio = (float)SeriesH/(float)Height;
	}
}
//----------------------------------------------------------------------------//
void CJetChart::SortSeries()
{
	if( m_pSeries1 == NULL ) { return; }
	
	CJetSeries *pSeries = m_pSeries1;
	pSeries->SortSeries();
}
//----------------------------------------------------------------------------//
void CJetChart::PreSetChartInfo()
{
	m_SeiresValueSum = 0;
	if( (this->m_ChartType == CHART_TYPE_PIE) || (m_ChartType == CHART_TYPE_BAR) )//fang 1020624
	{
		SortSeries();
	}
	this->GetChartRect(&m_ChartRect, &m_LegendRect);
	this->GetSeriesRect(&m_SeriesRect);
	if( this->m_ChartType == CHART_TYPE_RADAR )
	{ 
		this->GetRadarRadius(m_RadarRadus);
		if( m_RadarRadus <= 0 ) { m_RadarRadus = 5; }
	}
	else
	{ this->GetAllDataLimit(m_MinXIndex, m_MaxXIndex, m_MinYValue, m_MaxYValue); }
	
	this->GetSeriesRatio(m_SeriesRatioX, m_SeriesRatioY);

	//const int NSeries = GetNVisibleSeries();
}
//----------------------------------------------------------------------------//
void CJetChart::GetPieTextPos(HDC hDC, const int PieAspect, POINT *pt)
{
	POINT point = *pt;
	int CharH = 8;
	switch(PieAspect)
	{
	case 0://右
		::SetTextAlign(hDC, TA_LEFT);
		point.y -= CharH;
		break;
	case 1://下
		::SetTextAlign(hDC, TA_CENTER);
		break;
	case 2:	//左
		::SetTextAlign(hDC, TA_RIGHT);
		point.y -= CharH;
		break;
	case 3://上
		::SetTextAlign(hDC, TA_CENTER);
		point.y -= (2*CharH);
		break;
	default:
		break;
	}
	*pt = point;
}
//----------------------------------------------------------------------------//
void CJetChart::GetPieLinePos(RECT FullRect, RECT Rect, POINT *pt, int &PieAspect)
{
	int Thickness = this->m_3DThickness+5;
	int FullRectW = FullRect.right-FullRect.left;
	int FullRectH = FullRect.bottom-FullRect.top;
	float FullHalfW = FullRectW*0.5f;
	float FullHalfH = FullRectH*0.5f;
	int RectW = Rect.right-Rect.left;
	int RectH = Rect.bottom-Rect.top;
	float HalfW = RectW*0.5f+20;
	float HalfH = RectH*0.5f+20;
	int cx = (int)((Rect.right+Rect.left)*0.5);
	int cy = (int)((Rect.bottom+Rect.top)*0.5);

	int dx = pt->x-cx;
	int dy = pt->y-cy;
	
	float Rad = ::atan2f((float)dy, (float)dx);
	float Deg = (float)(Rad*RAD2DEG);
	float TempValue = 0.0f;
	float TempX = 0.0f;
	float TempY = 0.0f;
	int PosY=pt->y, PosX=pt->x;

	if( Deg < 0.0f )
	{
		Deg = 360+Deg;
	}

	if( Deg <= 45.0f )
	{	
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 0;
		TempValue = Deg;
		Rad = (float)(TempValue*DEG2RAD);
		TempY = fabs(HalfW*::tanf(Rad));
		TempX = HalfW;
		if( TempY > fabs(FullHalfH) )
		{
			HalfW = RectW*0.5f+Thickness;
			HalfH = RectH*0.5f+Thickness;
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempX = fabs(HalfH*::tanf(Rad));
			TempY = HalfH;
			PieAspect = 1;
		}
		PosY = cy+(int)TempY;
		PosX = cx+(int)TempX;
	}
	else if ( Deg <= 90.0f )
	{
		HalfW = RectW*0.5f+Thickness;
		HalfH = RectH*0.5f+Thickness;
		PieAspect = 1;
		TempValue = 90.0f-Deg;
 		Rad = (float)(TempValue*DEG2RAD);
		TempX = fabs(HalfH*::tanf(Rad));
		TempY = HalfH;
		if( TempX > fabs(FullHalfW) )
		{
			HalfW = RectW*0.5f+10;
			HalfH = RectH*0.5f+10;
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempY = fabs(HalfW*::tanf(Rad));
			TempX = HalfW;
			PieAspect = 0;
		}
		PosY = cy+(int)TempY;
		PosX = cx+(int)TempX;
	}
	else if( Deg <= 135.0f ) 
	{
		HalfW = RectW*0.5f+Thickness;
		HalfH = RectH*0.5f+Thickness;
		PieAspect = 1;
		TempValue = Deg-90.0f;
		Rad = (float)(TempValue*DEG2RAD);
		TempX = fabs(HalfH*::tanf(Rad));
		TempY = HalfH;
		if( TempX > fabs(FullHalfW) )
		{
			HalfW = RectW*0.5f+10;
			HalfH = RectH*0.5f+10;
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempY = fabs(HalfW*::tanf(Rad));
			TempX = HalfW;
			PieAspect = 2;
		}
		PosY = cy+(int)TempY;
		PosX = cx-(int)TempX;
	}
	else if( Deg <= 180.0f ) 
	{
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 2;
		TempValue = 180.0f-Deg;
		Rad = (float)(TempValue*DEG2RAD);
		TempY = fabs(HalfW*::tanf(Rad));
		TempX = HalfW;
		if( TempY > fabs(FullHalfH) )
		{
			HalfW = RectW*0.5f+Thickness;
			HalfH = RectH*0.5f+Thickness;
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempX = fabs(HalfH*::tanf(Rad));
			TempY = HalfH;
			PieAspect = 1;
		}
		PosY = cy+(int)TempY;
		PosX = cx-(int)TempX;
	}
	else if( Deg <= 225.0f ) 
	{
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 2;
		TempValue = Deg-180.0f;
		Rad = (float)(TempValue*DEG2RAD);
		TempY = fabs(HalfW*::tanf(Rad));
		TempX = HalfW;
		if( TempY > fabs(FullHalfH) )
		{
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempX = fabs(HalfH*::tanf(Rad));
			TempY = HalfH;
			PieAspect = 3;
		}
		PosY = cy-(int)TempY;
		PosX = cx-(int)TempX;
	}
	else if( Deg <= 270.0f ) 
	{
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 3;
		TempValue = 270.0f-Deg;
		Rad = (float)(TempValue*DEG2RAD);
		TempX = fabs(HalfH*::tanf(Rad));
		TempY = HalfH;
		if( TempX > fabs(FullHalfW) )
		{
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempY = fabs(HalfW*::tanf(Rad));
			TempX = HalfW;
			PieAspect = 2;
		}
		PosY = cy-(int)TempY;
		PosX = cx-(int)TempX;
	}
	else if( Deg <= 315.0f ) 
	{
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 3;
		TempValue = Deg-270.0f;
		Rad = (float)(TempValue*DEG2RAD);
		TempX = fabs(HalfH*::tanf(Rad));
		TempY = HalfH;
		if( TempX > fabs(FullHalfW) )
		{
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempY = fabs(HalfW*::tanf(Rad));
			TempX = HalfW;
			PieAspect = 0;
		}
		PosY = cy-(int)TempY;
		PosX = cx+(int)TempX;
	}
	else if( Deg <= 360.0f ) 
	{	
		HalfW = RectW*0.5f+10;
		HalfH = RectH*0.5f+10;
		PieAspect = 0;
		TempValue = 360.0f-Deg;
		Rad = (float)(TempValue*DEG2RAD);
		TempY = fabs(HalfW*::tanf(Rad));
		TempX = HalfW;
		if( TempY > fabs(FullHalfH) )
		{
			TempValue = 90-TempValue;
			Rad = (float)(TempValue*DEG2RAD);
			TempX = fabs(HalfH*::tanf(Rad));
			TempY = HalfH;
			PieAspect = 3;
		}
		PosY = cy-(int)TempY;
		PosX = cx+(int)TempX;
	}

	pt->x = PosX;
	pt->y = PosY;


}
//----------------------------------------------------------------------------//
void CJetChart::GetPieLinePos(float value, POINT *pt, int &PieAspect)
{
	RECT SeriesRect = m_ChartRect;
	int RectW = SeriesRect.right-SeriesRect.left;
	int RectH = SeriesRect.bottom-SeriesRect.top;
	float HalfW = RectW*0.5f;
	float HalfH = RectH*0.5f;
	int Cx = (int)((SeriesRect.right+SeriesRect.left)*0.5);
	int Cy = (int)((SeriesRect.bottom+SeriesRect.top)*0.5);
	int PosX = 0;
	int PosY = 0;

	float TempValue = 0.0f;
	float Rad = 0.0f;
	if( value <= 12.5 )
	{
		TempValue = (float)(value*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosY = (int)fabs(HalfW*::tanf(Rad));
		PosX = (int)HalfW;
		PosY = Cy+PosY;
		PosX = Cx+PosX;		
		PieAspect = 0;
	}
	else if( value <= 25.0 )
	{
		TempValue = (float)((25.0-value)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosX = (int)fabs(HalfH*::tanf(Rad));
		PosY = (int)HalfH;
		PosY = Cy+PosY;
		PosX = Cx-PosX;	
		PieAspect = 1;
	}
	else if( value <= 37.5 )
	{
		TempValue = (float)((value-25.0)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosX = (int)fabs(HalfH*::tanf(Rad));
		PosY = (int)HalfH;
		PosY = Cy+PosY;
		PosX = Cx+PosX;	
		PieAspect = 1;
	}
	else if( value <= 50.0 )
	{
		TempValue = (float)((50.0-value)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosY = (int)fabs(HalfW*::tanf(Rad));
		PosX = (int)HalfW;
		PosY = Cy+PosY;
		PosX = Cx-PosX;		
		PieAspect = 2;
	}
	else if( value <= 62.5 )
	{
		TempValue = (float)((value-50.0)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosY = (int)fabs(HalfW*::tanf(Rad));
		PosX = (int)HalfW;
		PosY = Cy-PosY;
		PosX = Cx-PosX;		
		PieAspect = 2;
	}
	else if( value <= 75.0 )
	{
		TempValue = (float)((75.0-value)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosX = (int)fabs(HalfH*::tanf(Rad));
		PosY = (int)HalfH;
		PosY = Cy-PosY;
		PosX = Cx-PosX;	
		PieAspect = 3;
	}
	else if( value <= 87.5 )
	{
		TempValue = (float)((value-75.0)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosX = (int)fabs(HalfH*::tanf(Rad));
		PosY = (int)HalfH;
		PosY = Cy-PosY;
		PosX = Cx+PosX;	
		PieAspect = 3;
	}
	else if( value <= 100.0f )
	{
		TempValue = (float)((100.0f-value)*0.01*360.0);
		Rad = (float)(TempValue*DEG2RAD);
		PosY = (int)fabs(HalfW*::tanf(Rad));
		PosX = (int)HalfW;
		PosY = Cy-PosY;
		PosX = Cx+PosX;	
	}
/*
	RECT SeriesRect = m_ChartRect;
	int RectW = SeriesRect.right-SeriesRect.left;
	int RectH = SeriesRect.bottom-SeriesRect.top;
	int Cx = (int)((SeriesRect.right+SeriesRect.left)*0.5);
	int Cy = (int)((SeriesRect.bottom+SeriesRect.top)*0.5);
	float HalfW = RectW*0.5f;
	float HalfH = RectH*0.5f;
	float RatioX = (float)RectW/25.0f;
	float RatioY = (float)RectH/25.0f; 
	int TempDX = 0;
	int TempDY = 0;
	int PosX = 0;
	int PosY = 0;

	if( value > 100.0 ) { value -= 100.0; }
		
	if( value <= 25 )
	{			
		TempDY = (int)(value*RatioY);
		PosY = SeriesRect.top+TempDY;
		PosX = SeriesRect.left;
		PieAspect = 0;
	}
	else if( value <= 50 )
	{
		TempDX = (int)((value-25)*RatioX);
		PosX = SeriesRect.left+TempDX;			
		PosY = SeriesRect.bottom;
		PieAspect = 1;		
	}
	else if( value <= 75 )
	{
		TempDY = (int)((value-50)*RatioY);
		PosY = SeriesRect.bottom-TempDY;			
		PosX = SeriesRect.right;
		PieAspect = 2;	
	}
	else if( value <= 100 )
	{
		TempDX = (int)((value-75)*RatioX);
		PosX = SeriesRect.right-TempDX;		
		PosY = SeriesRect.top;
		PieAspect = 3;	
	}
	else 
	{
		PosX = SeriesRect.left;		
		PosY = SeriesRect.top;
		PieAspect = 4;	
	}			
			 */
	pt->x = PosX;
	pt->y = PosY;
}
//----------------------------------------------------------------------------//
void CJetChart::DrawBarSeries(HDC hDC, CJetSeries *pSeries)//fang 1020624
{
	int Thickness = m_3DThickness;
	SeriesNode_S *pNode = NULL;
	RECT FullRect = m_ChartRect;
	int DrawWidth = FullRect.right - FullRect.left;
	int DrawHeight = FullRect.bottom - FullRect.top;

	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	//	HBRUSH Brush = ::CreateSolidBrush(pSeries->GetLineColor());
	HBRUSH Brush = ::CreateSolidBrush(RGB(175,175,175));
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

	HPEN Pen = ::CreatePen(PS_SOLID, 1, 0xbbbbbb);
	HPEN ArrowPen = ::CreatePen(PS_SOLID, 1, 0xff8833);
	HPEN HidePen = ::CreatePen(PS_NULL, 1, 0x000000);
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, Pen);

	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,							// bItalic
		0,							// bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"
	
	CFont* def_font = (CFont*)::SelectObject(hDC, font);	

	int size = pSeries->GetNSeriesNode();

	int Pitch = 3;
	int BarHeight = static_cast<int>( (DrawHeight * 0.1) - Pitch );
	int BarWidth = static_cast<int>( DrawWidth * 0.6 );


	//先計算要畫幾條橫線圖
	int i = 0;
	int NBar = 0;
	for( i = 0 ;i < size ; i++ )
	{
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		if( pNode->m_YValue <= 0 ) { continue; }
		NBar++;
	}
	
	if( NBar == 0 )
	{  	
		::SelectObject(hDC, OldPen);
		::DeleteObject(Pen);
		::DeleteObject(ArrowPen);
		::DeleteObject(HidePen);
		
		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
		
		::SelectObject(hDC, def_font);
		font.DeleteObject();
		return;
	}

	Pitch = 3 +(10-NBar) * 1;//重新調整Pitch 值
	

	POINT pt = {0};
	RECT TextRect = {0};
	RECT BarRect = {0};
	RECT PercentRect = {0};
	
	TextRect.left = FullRect.left;
	TextRect.right = TextRect.left + DrawWidth * 0.1;
	TextRect.top = FullRect.top;

	BarRect.left = TextRect.left + 42;
	BarRect.right = BarRect.left + BarWidth;
	BarRect.top = FullRect.top;
	
	COLORREF color = 0;

	float PercentValue = 0.0f;
	int CountValue = 0;
	CString Text = _T("");
	COLORREF TextColor = RGB(90,90,40);


	for( i = (size-1) ; i>=0 ;i--)
	{
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		if( pNode->m_YValue <= 0 ) { continue; }

		color = m_PieColorList[pNode->m_XIndex];
		
		TextRect.bottom = TextRect.top + BarHeight;
		BarRect.bottom = BarRect.top + BarHeight;
	
		CountValue = static_cast<int>(pNode->m_YValue);
		PercentValue = (float)(100.0*pNode->m_YValue/m_SeiresValueSum);
		Text.Format(_T("%s "), pNode->m_XLabel);

		::SetTextAlign(hDC, TA_LEFT);
		::SetBkMode(hDC, TRANSPARENT);
		TextColor = RGB(90,90,40);
		::SetTextColor(hDC, TextColor);
		pt.x = TextRect.left;
		pt.y = TextRect.top;
		::TextOut(hDC, pt.x, pt.y, Text, Text.GetLength());


		//畫外框
		::SelectObject(hDC, Pen);
		::MoveToEx(hDC, BarRect.left, BarRect.top, NULL);
		::LineTo(hDC, BarRect.right, BarRect.top);
		::LineTo(hDC, BarRect.right, BarRect.bottom);
		::LineTo(hDC, BarRect.left, BarRect.bottom);
		::LineTo(hDC, BarRect.left, BarRect.top);

		//畫Percent橫條圖
		PercentRect.left = BarRect.left + 1;
		PercentRect.top = BarRect.top + 1;
		PercentRect.bottom = BarRect.bottom;
		PercentRect.right = PercentRect.left + BarWidth * PercentValue * 0.01;

		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
		Brush = ::CreateSolidBrush(color);
		::SelectObject(hDC, Brush);
		::FillRect(hDC, &PercentRect, Brush);

		//在Percent橫條圖旁邊畫Percent 數值
		TextColor = RGB(0,0,200);
		::SetTextColor(hDC, TextColor);
		Text.Format(_T("%.1f%%"), PercentValue);
		pt.x = PercentRect.right + 5;
		pt.y = PercentRect.top;
		::TextOut(hDC, pt.x, pt.y, Text, Text.GetLength());

		//在橫條圖最後畫錯誤數量
		TextColor = RGB(90,90,90);
		::SetTextColor(hDC, TextColor);
		Text.Format(_T("%d Counts"), CountValue);
		pt.x = BarRect.right + 5;
		pt.y = BarRect.top;
		::TextOut(hDC, pt.x, pt.y, Text, Text.GetLength());


		BarRect.top = BarRect.bottom + Pitch;
		TextRect.top = TextRect.bottom + Pitch;
	}


	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);
	::DeleteObject(ArrowPen);
	::DeleteObject(HidePen);

	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);

	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawPieSeries(HDC hDC, CJetSeries *pSeries)
{
	
	int Thickness = m_3DThickness;
	int size = pSeries->GetNSeriesNode();
	SeriesNode_S *pNode = NULL;
	RECT FullRect = m_ChartRect;

	RECT SeriesRect = FullRect;
	FullRect.left+=10;
	FullRect.top+=10;
	FullRect.right-=10;
	FullRect.bottom-=10;

	SeriesRect.top = SeriesRect.top+Thickness;
	SeriesRect.left = SeriesRect.left+15;
	SeriesRect.right = SeriesRect.right-15;
	//SeriesRect.bottom = SeriesRect.bottom-Thickness;
	int RectW = SeriesRect.right-SeriesRect.left;
	int RectH = SeriesRect.bottom-SeriesRect.top;
	
	float HalfW = (float)RectW*0.5f;
	float HalfH = (float)RectH*0.5f;

	float RatioX = (float)RectW/25.0f;
	float RatioY = (float)RectH/25.0f; 

	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	HBRUSH Brush = ::CreateSolidBrush(pSeries->GetLineColor());
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

	HPEN Pen = ::CreatePen(PS_SOLID, 1, 0xbbbbbb);
	HPEN ArrowPen = ::CreatePen(PS_SOLID, 1, 0xff8833);
	HPEN HidePen = ::CreatePen(PS_NULL, 1, 0x000000);
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, Pen);

	int i=0;
	float SumValue = 0.0f;	
	int StartX = SeriesRect.left;
	int StartY = SeriesRect.top;
	int EndX = SeriesRect.left;
	int EndY = SeriesRect.top;
	int TempDX = 0;
	int TempDY = 0;
	float Value = 0.0f;
	float OldValue = 0.0f;
	float CenterValue = 0.0f;
	float TotalValue = 0.0f;
	float XRatio = (float)RectW/(float)RectH;
	float YRatio = (float)RectW/(float)RectH;
	float TempF = 0;
	COLORREF color = 0;
	CString Text = "";
	POINT pt = {0,0};

	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,							// bItalic
		0,							// bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);	
	int PieAspect = 0;
	int Rw = SeriesRect.right-SeriesRect.left;
	int Rh = SeriesRect.bottom-SeriesRect.top;
	int a = abs((int)(Rw*0.5));
	int b = abs((int)(Rh*0.5));
	int cx = (int)((SeriesRect.right+SeriesRect.left)*0.5);
	int cy = (int)((SeriesRect.bottom+SeriesRect.top)*0.5);

	float Deg = 0.0f;
	float Theta = 0.0f;
	float Px=0.0f, Py=0.0f;
	float Px1=0.0f, Py1=0.0f;
	float Px2=0.0f, Py2=0.0f;
	
//	::SelectObject(hDC, ColorPen);
	
	TotalValue = 0;
	Value = 0;
	float Deg1=0;
	float Deg2=0;
	COLORREF TempColor = 0;
	for( i=0 ; i<size ; i++ ) 
	{
		if( Thickness <= 0 ) { break; }
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		
		color = pNode->m_Color;
		if( color == NULL )
		{ color = m_PieColorList[pNode->m_XIndex]; }	
	//	color -= 100;

	//B
		TempColor = color&0xff0000;
		if( TempColor > 0x440000 ) { color -= 0x440000;  }
	//G
		TempColor = color&0x00ff00;
		if( TempColor > 0x004400 ) { color -= 0x004400;  }
	//R
		TempColor = color&0x0000ff;
		if( TempColor > 0x000044 ) { color -= 0x000044;  }


		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
		Brush = ::CreateSolidBrush(color);
		::SelectObject(hDC, Brush);

		if( pNode->m_YValue <= 0 ) { continue; }
		Value = (float)(100.0*pNode->m_YValue/m_SeiresValueSum);
		Text.Format(_T("%s %.1f%%"), pNode->m_XLabel, Value);

		Deg1 = (float)(TotalValue*360*0.01);
		TotalValue += Value;
		Deg2 = (float)(TotalValue*360*0.01);

		Theta = (float)(Deg1*DEG2RAD);
		Px1 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py1 = (float)(cy+(b*::sinf(Theta)+0.5));

		Theta = (float)(Deg2*DEG2RAD);
		Px2 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py2 = (float)(cy+(b*::sinf(Theta)+0.5));
		

		::Pie(hDC, SeriesRect.left, SeriesRect.top, SeriesRect.right, SeriesRect.bottom
			, (int)Px2, (int)Py2, (int)Px1, (int)Py1);
	}

	TotalValue = 0;
	Value = 0;
	Deg1=0;
	Deg2=0;
	::SelectObject(hDC, HidePen);
	for( i=0 ; i<size ; i++ ) 
	{
		if( Thickness <= 0 ) { break; }
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		
		color = pNode->m_Color;
		if( color == NULL )
		{ color = m_PieColorList[pNode->m_XIndex]; }

	//	if( color > 0x223333)
	//	{ color -= 0x223333; }

	//B
		TempColor = color&0xff0000;
		if( TempColor > 0x440000 ) { color -= 0x440000;  }
	//G
		TempColor = color&0x00ff00;
		if( TempColor > 0x004400 ) { color -= 0x004400;  }
	//R
		TempColor = color&0x0000ff;
		if( TempColor > 0x000044 ) { color -= 0x000044;  }

		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
		Brush = ::CreateSolidBrush(color);
		::SelectObject(hDC, Brush);
		
		if( pNode->m_YValue <= 0 ) { continue; }
		Value = (float)(100.0*pNode->m_YValue/m_SeiresValueSum);

		Deg1 = (float)(TotalValue*360*0.01);
		TotalValue += Value;
		Deg2 = (float)(TotalValue*360*0.01);

		if( Deg1 > 180.0f ) { continue; }
		if( Deg2 > 180.0f )
		{ Deg2 = 180.0f; }

		Theta = (float)(Deg1*DEG2RAD);
		Px1 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py1 = (float)(cy+(b*::sinf(Theta)+0.5));

		Theta = (float)(Deg2*DEG2RAD);
		Px2 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py2 = (float)(cy+(b*::sinf(Theta)+0.5));


		::MoveToEx(hDC, (int)Px1, (int)Py1-2, NULL);
		::BeginPath(hDC);
		::LineTo(hDC, (int)Px2, (int)Py2-2);
		::LineTo(hDC, (int)Px2, (int)Py2-Thickness-2);
		::LineTo(hDC, (int)Px1, (int)Py1-Thickness-2);
		::LineTo(hDC, (int)Px1, (int)Py1-2);
		::EndPath(hDC);
		::StrokeAndFillPath(hDC);
	}
	
	::SelectObject(hDC, Pen);
	
	if( Thickness > 0 )
	{
		Theta = 0;
		Px = (float)(cx+(a*::cosf(Theta)+0.5));
		Py = (float)(cy+(b*::sinf(Theta)+0.5));
		::MoveToEx(hDC, (int)Px, (int)Py, NULL); 
		::LineTo(hDC, (int)Px, (int)Py-Thickness);

		Theta = (float)(180.0*DEG2RAD);
		Px = (float)(cx+(a*::cosf(Theta)+0.5));
		Py = (float)(cy+(b*::sinf(Theta)+0.5));
		::MoveToEx(hDC, (int)Px, (int)Py, NULL); 
		::LineTo(hDC, (int)Px, (int)Py-Thickness);
	}

	TotalValue = 0;
	Value = 0;
	Deg1=0;
	Deg2=0;
	SeriesRect.top -= Thickness;
	SeriesRect.bottom -= Thickness;
	cx = (int)((SeriesRect.right+SeriesRect.left)*0.5);
	cy = (int)((SeriesRect.bottom+SeriesRect.top)*0.5);
	
	COLORREF MarkColor = 0xff0000;	//chia 1050518
	::SetTextColor(hDC, MarkColor);
	for( i=0 ; i<size ; i++ ) 
	{
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		
		color = pNode->m_Color;
		if( color == NULL )
		{ color = m_PieColorList[pNode->m_XIndex]; }
	
		::SelectObject(hDC, OldBrush);
		::DeleteObject(Brush);
		Brush = ::CreateSolidBrush(color);
		::SelectObject(hDC, Brush);

		if( pNode->m_YValue <= 0 ) { continue; }
		Value = (float)(100.0*pNode->m_YValue/m_SeiresValueSum);
		//Text.Format(_T("%s %.1f%%"), pNode->m_XLabel, Value);
		Text.Format(_T("%s"), pNode->m_XLabel);

		Deg1 = (float)(TotalValue*360*0.01);
		TotalValue += Value;
		Deg2 = (float)(TotalValue*360*0.01);

		Theta = (float)(Deg1*DEG2RAD);
		Px1 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py1 = (float)(cy+(b*::sinf(Theta)+0.5));

		Theta = (float)(Deg2*DEG2RAD);
		Px2 = (float)(cx+(a*::cosf(Theta)+0.5));
		Py2 = (float)(cy+(b*::sinf(Theta)+0.5));

		CenterValue = (Deg1 + Deg2)*0.5f;
		Theta = (float)(CenterValue*DEG2RAD);
		Px = (float)(cx+(a*::cosf(Theta)+0.5));
		Py = (float)(cy+(b*::sinf(Theta)+0.5));


		pt.x = (LONG)Px;
		pt.y = (LONG)Py;
		GetPieLinePos(FullRect, SeriesRect, &pt, PieAspect);
		
		::SelectObject(hDC, ArrowPen);
		::MoveToEx(hDC, cx, cy, NULL);
		::LineTo(hDC, pt.x, pt.y);

		GetPieTextPos(hDC, PieAspect, &pt);
		::TextOut(hDC, pt.x, pt.y, Text, Text.GetLength());


		::SelectObject(hDC, Pen);
		::Pie(hDC, SeriesRect.left, SeriesRect.top, SeriesRect.right, SeriesRect.bottom
			, (int)Px2, (int)Py2, (int)Px1, (int)Py1);
	}

	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);
	::DeleteObject(HidePen);
	::DeleteObject(ArrowPen);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);

	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawRadarSeries(HDC hDC, CJetSeries *pSeries)
{
	if( pSeries == NULL ) { return; }
	int size = pSeries->GetNSeriesNode();
	if( size <= 0 ) { return; }
 
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;

	RECT ChartRect	= {0};
	RECT SeriesRect = {0};
	ChartRect	= m_ChartRect;
	SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetX;
	SeriesRect.left = m_SeriesRect.left+m_OffsetX;
	SeriesRect.top = m_SeriesRect.top+m_OffsetY;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetY;
	
	int Rw = SeriesRect.right-SeriesRect.left;

	CFont font;
	VERIFY(font.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		0,                     // bItalic
		0,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	::SetTextAlign(hDC, TA_CENTER);

	//繪製數列
	COLORREF MarkColor = 0xff0000;
	COLORREF color = 0x000000;
	int  LineWidth = pSeries->GetLineWidth();
	int	 DotWidth  = pSeries->GetDotWidth();
	HPEN MarkPen = ::CreatePen(PS_DOT, 1, MarkColor);
	HPEN DotPen = ::CreatePen(PS_SOLID, 1, pSeries->GetDotColor());
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, DotPen);
	
	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	
	HBRUSH Brush = ::CreateSolidBrush(pSeries->GetDotColor());
	HBRUSH ColorBarBrush = NULL;
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

	int i=0;
	CString Text = _T("");
	SeriesNode_S *pNode = NULL;
	int Index = 0;
	float ValueX = 0.0f;
	float ValueY = 0.0f;
	int Ox = (int)((SeriesRect.left+SeriesRect.right)*0.5);
	int Oy = (int)((SeriesRect.top+SeriesRect.bottom)*0.5);
	int Px = 0;
	int Py = 0;
	
	bool ShowMarkValue = pSeries->GetIsShowMarkValue();
	bool IsFirst = true;

	bool IsVisible = true;

	::SetTextColor(hDC, MarkColor);
	if( size > 0 )
	{
		int XIndex = -1;
		//if( DotWidth > 0 && size < 200 ) { DotWidth = 2; }
		for( i=0 ; i<size ; i++ )
		{
			XIndex++;
			pNode = pSeries->GetSeriesNode(XIndex);
			if( pNode == NULL ) { continue; }
			IsVisible = pNode->m_IsVisible;
			if( IsVisible == false ) { continue; }
			ValueX = pNode->m_XValue-this->m_RadarCXValue;
			ValueY = pNode->m_YValue-this->m_RadarCYValue;

			Px = (int)(Ox+ValueX*XRatio);
			Py = (int)(Oy-ValueY*YRatio);
			
			//Draw Dot
			if( DotWidth > 0 )
			{
			//	::Ellipse(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth);
				
				::Arc(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth, 0,0,0,0);
				if( ShowMarkValue )
				{
					Text.Format(_T("(%.2f,%.2f)"), pNode->m_XValue, pNode->m_YValue);
					::TextOut(hDC, Px+5, Py+5, Text, Text.GetLength());
				}
			}
		}
	}
	::SelectObject(hDC, def_font);
	::SelectObject(hDC, OldPen);
	::DeleteObject(font);
	::DeleteObject(MarkPen);	
	::DeleteObject(DotPen);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawSeries(HDC hDC, CJetSeries *pSeries)
{
	if( this->m_IsTranspose == true ) 
	{ 
		DrawSeries_Transpose(hDC, pSeries); 
		return;
	}
	
	if( pSeries == NULL ) { return; }
	int size = pSeries->GetNSeriesNode();
	if( size <= 0 ) { return; }
 
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetX;
	SeriesRect.left = m_SeriesRect.left+m_OffsetX;
	SeriesRect.top = m_SeriesRect.top+m_OffsetY;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetY;
	
	int Rw = SeriesRect.right-SeriesRect.left;

	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
		size = MaxXIndex-MinXIndex+1;
	}
	
	CFont font;
	VERIFY(font.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,                     // bItalic
		0,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	::SetTextAlign(hDC, TA_CENTER);

	//繪製數列

	COLORREF MarkColor = 0xff0000;	
	COLORREF color = 0x000000;
	int  LineWidth = pSeries->GetLineWidth();
	int	 DotWidth  = pSeries->GetDotWidth();
	HPEN MarkPen = ::CreatePen(PS_DOT, 1, MarkColor);
	HPEN Pen = ::CreatePen(PS_SOLID, LineWidth, pSeries->GetLineColor());
	HPEN DotPen = ::CreatePen(PS_SOLID, 1, pSeries->GetDotColor());
	HPEN ColorBarPen = ::CreatePen(PS_SOLID, 1, MarkColor);
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, Pen);
	
	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	
	HBRUSH BarBrush = ::CreateSolidBrush(pSeries->GetLineColor());
	HBRUSH Brush = ::CreateSolidBrush(pSeries->GetDotColor());
	HBRUSH ColorBarBrush = NULL;
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

	int i=0;
	CString Text = "";
	SeriesNode_S *pNode = NULL;
	int Index = 0;
	float Value = 0.0f;
	int Ox = SeriesRect.left;
	int Oy = SeriesRect.bottom;
	int Px = 0;
	int Py = 0;
	
	int SeriesType = pSeries->GetSeriesType();
	bool ShowMarkValue = pSeries->GetIsShowMarkValue();
	bool IsFirst = true;
	COLORREF DotColor = pSeries->GetDotColor();
	RECT BarRect = {0,0,0,0};

	int BarWidth = Rw/size;
	int BarHalfWidth = (int)(BarWidth*0.5);
	if( BarHalfWidth > Ox-ChartRect.left ) { BarHalfWidth = Ox-ChartRect.left-1; }
	BarWidth = 2*BarHalfWidth;

	bool OldIsVisible = true;
	bool IsVisible = true;

	::SetTextColor(hDC, MarkColor);
	if( size > 0 )
	{
		int XIndex = -1;
		if( m_IsZoom == true )
		{
			XIndex = MinXIndex-1;
		}
	//	DotWidth = 1;
		if( DotWidth > 0 && size < 200 ) { DotWidth = 2; }
		for( i=0 ; i<size ; i++ )
		{
			XIndex++;
			pNode = pSeries->GetSeriesNode(XIndex);
			if( pNode == NULL ) { continue; }
			IsVisible = pNode->m_IsVisible;
			if( IsVisible == false )
			{
				OldIsVisible = IsVisible;
				continue;
			}
			Index = pNode->m_XIndex;
			Value = pNode->m_YValue;
			Index = Index - MinXIndex;
			Value = Value - MinYValue;

			Px = (int)(Ox+Index*XRatio);
			Py = (int)(Oy-Value*YRatio);

			::SelectObject(hDC, Pen);
			if( SeriesType == SERIES_TYPE_DOT )
			{
				//Draw Dot
				if( DotWidth > 0 )
				{
					::SelectObject(hDC, DotPen);
					::Ellipse(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth);
					if( ShowMarkValue )
					{
						::SelectObject(hDC, MarkPen);
						::MoveToEx(hDC, Px, Py-20, NULL);
						::LineTo(hDC, Px, Py);
						Text.Format(_T("%.2f"), pNode->m_YValue);
						::TextOut(hDC, Px, Py-30, Text, Text.GetLength());
					}
				}
			}
			else if( SeriesType == SERIES_TYPE_LINE )
			{
				//Draw Line
				::SelectObject(hDC, Pen);
				if( IsFirst == true )
				{ ::MoveToEx(hDC, Px, Py, NULL); }
				else
				{ ::LineTo(hDC, Px, Py); }
				//Draw Dot
				if( DotWidth > 0 )
				{
					::SelectObject(hDC, DotPen);
					::Ellipse(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth);
				}
					
				if( ShowMarkValue )
				{
					::SelectObject(hDC, MarkPen);
					::MoveToEx(hDC, Px, Py-20, NULL);
					::LineTo(hDC, Px, Py);
					Text.Format(_T("%.2f"), pNode->m_YValue);
					::TextOut(hDC, Px, Py-30, Text, Text.GetLength());
				}
			}
			else if( SeriesType == SERIES_TYPE_BAR )
			{
				BarRect.left = Px-BarHalfWidth;
				BarRect.top = Py;
				BarRect.right = BarRect.left+BarWidth;
				BarRect.bottom = SeriesRect.bottom;
				if ( pNode->m_Width > 0 ) 
				{
					int W = (int)(pNode->m_Width*XRatio*0.5);
					BarRect.left = Px-W;
					BarRect.right = Px+W;
				}
				color = pNode->m_Color;
				if( color == NULL )
				{ 
					if( BarRect.right==BarRect.left)
					{
						::SelectObject(hDC, Pen);
						::MoveToEx(hDC, BarRect.left, BarRect.bottom, NULL);
						::LineTo(hDC, BarRect.left, BarRect.top);
					}
					else
					{ ::FillRect(hDC, &BarRect, BarBrush);  }					
				}				
				else
				{
					if( BarRect.right==BarRect.left)
					{
						::SelectObject(hDC, OldPen);
						::DeleteObject(ColorBarPen); ColorBarPen=NULL;
						ColorBarPen = ::CreatePen(PS_SOLID, 1, color);
						::SelectObject(hDC, ColorBarPen);
						::MoveToEx(hDC, BarRect.left, BarRect.bottom, NULL);
						::LineTo(hDC, BarRect.left, BarRect.top);
					}
					else
					{
						::DeleteObject(ColorBarBrush); ColorBarBrush=NULL;
						ColorBarBrush = ::CreateSolidBrush(color);
						::FillRect(hDC, &BarRect, ColorBarBrush);
					}
				}
					
				if( ShowMarkValue )
				{
					::SelectObject(hDC, MarkPen);
					::MoveToEx(hDC, Px, Py-10, NULL);
					::LineTo(hDC, Px, Py);
					Text.Format(_T("%.2f"), pNode->m_YValue);
					::TextOut(hDC, Px, Py-20, Text, Text.GetLength());
				}
			}
			IsFirst = false;
			OldIsVisible = IsVisible;
		}
	}


	PIndexLine_ST pIndexLine = NULL;
	for( i=0 ; i<MAX_INDEX_LINE ; i++)
	{
		pIndexLine = &m_IndexLineList[i];
		if( pIndexLine->sEnabled == false ) { continue; }

		Index = pIndexLine->sIndex;
		if( Index < MinXIndex ) { continue; } 
		if( Index > MaxXIndex ) { continue; } 
		
		Index = Index - MinXIndex;
		Value = m_MaxYValue;
		Px = (int)(Ox+Index*XRatio);
		Py = (int)(Oy-Value*YRatio);

		color = pIndexLine->sLineColor;
		
		::SelectObject(hDC, OldPen);
		::DeleteObject(Pen); Pen=NULL;
		Pen = ::CreatePen(PS_SOLID, 1, color);
		::SelectObject(hDC, Pen);
		::MoveToEx(hDC, Px, Py, NULL);
		::LineTo(hDC, Px, SeriesRect.bottom);
		

		Text.Format(_T("%s"), pIndexLine->sText);
		if( Text.GetLength() > 0 )
		{
			color = pIndexLine->sFontColor;
			::SetTextColor(hDC, color);
			::TextOut(hDC, Px, Py-16, Text, Text.GetLength());
		}
	}

	::SelectObject(hDC, def_font);
	::SelectObject(hDC, OldPen);
	::DeleteObject(font);
	::DeleteObject(Pen);	
	::DeleteObject(MarkPen);	
	::DeleteObject(DotPen);
	::DeleteObject(ColorBarPen);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);
	::DeleteObject(BarBrush);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawSeries_Transpose(HDC hDC, CJetSeries *pSeries)
{
	if( pSeries == NULL ) { return; }
	int size = pSeries->GetNSeriesNode();
	if( size <= 0 ) { return; }
 
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetY;
	SeriesRect.left = m_SeriesRect.left+m_OffsetY;
	SeriesRect.top = m_SeriesRect.top+m_OffsetX;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetX;
	
	int Rw = SeriesRect.bottom-SeriesRect.top;

	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
		size = MaxXIndex-MinXIndex+1;
	}

	CFont font;
	VERIFY(font.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,                     // bItalic
		0,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	::SetTextAlign(hDC, TA_CENTER);

	//繪製數列

	COLORREF MarkColor = 0xff0000;
	COLORREF color = 0x000000;
	int  LineWidth = pSeries->GetLineWidth();
	int	 DotWidth  = pSeries->GetDotWidth();
	HPEN MarkPen = ::CreatePen(PS_DOT, 1, MarkColor);
	HPEN Pen = ::CreatePen(PS_SOLID, LineWidth, pSeries->GetLineColor());
	HPEN DotPen = ::CreatePen(PS_SOLID, 1, pSeries->GetDotColor());
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, Pen);
	
	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	
	HBRUSH BarBrush = ::CreateSolidBrush(pSeries->GetLineColor());
	HBRUSH Brush = ::CreateSolidBrush(pSeries->GetDotColor());
	HBRUSH ColorBarBrush = NULL;
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);

	int i=0;
	CString Text = _T("");
	SeriesNode_S *pNode = NULL;
	int Index = 0;
	float Value = 0.0f;
	int Ox = SeriesRect.left;
	int Oy = SeriesRect.top;
	int Px = 0;
	int Py = 0;
	
	int SeriesType = pSeries->GetSeriesType();
	bool ShowMarkValue = pSeries->GetIsShowMarkValue();
	bool IsFirst = true;
	COLORREF DotColor = pSeries->GetDotColor();
	RECT BarRect = {0,0,0,0};

	int BarWidth = Rw/size;
	int BarHalfWidth = (int)(BarWidth*0.5);
	if( BarHalfWidth > Oy-ChartRect.top ) { BarHalfWidth = Oy-ChartRect.top-1; }
	BarWidth = 2*BarHalfWidth;

	bool OldIsVisible = true;
	bool IsVisible = true;

	::SetTextColor(hDC, MarkColor);
	if( size > 0 )
	{
		int XIndex = -1;
		if( m_IsZoom == true )
		{
			XIndex = MinXIndex-1;
		}
	//	DotWidth = 1;
		if( DotWidth > 0 && size < 200 ) { DotWidth = 2; }
		for( i=0 ; i<size ; i++ )
		{
			XIndex++;
			pNode = pSeries->GetSeriesNode(XIndex);
			if( pNode == NULL ) { continue; }
			IsVisible = pNode->m_IsVisible;
			if( IsVisible == false )
			{
				OldIsVisible = IsVisible;
				continue;
			}
			Index = pNode->m_XIndex;
			Value = pNode->m_YValue;
			Index = Index - MinXIndex;
			Value = Value - MinYValue;

			Px = (int)(Ox+Value*YRatio);
			Py = (int)(Oy+Index*XRatio);

			::SelectObject(hDC, Pen);
			if( SeriesType == SERIES_TYPE_DOT )
			{
				//Draw Dot
				if( DotWidth > 0 )
				{
					::SelectObject(hDC, DotPen);
					::Ellipse(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth);
					if( ShowMarkValue )
					{
						::SelectObject(hDC, MarkPen);
						::MoveToEx(hDC, Px+20, Py, NULL);
						::LineTo(hDC, Px, Py);
						Text.Format(_T("%.2f"), pNode->m_YValue);
						::TextOut(hDC, Px+25, Py, Text, Text.GetLength());
					}
				}
			}
			else if( SeriesType == SERIES_TYPE_LINE )
			{
				//Draw Line
				::SelectObject(hDC, Pen);
				if( IsFirst == true )
				{ ::MoveToEx(hDC, Px, Py, NULL); }
				else
				{ ::LineTo(hDC, Px, Py); }
				//Draw Dot
				if( DotWidth > 0 )
				{
					::SelectObject(hDC, DotPen);
					::Ellipse(hDC, Px-DotWidth, Py-DotWidth, Px+DotWidth, Py+DotWidth);
				}
					
				if( ShowMarkValue )
				{
					::SelectObject(hDC, MarkPen);
					::MoveToEx(hDC, Px+20, Py, NULL);
					::LineTo(hDC, Px, Py);
					Text.Format(_T("%.2f"), pNode->m_YValue);
					::TextOut(hDC, Px+25, Py, Text, Text.GetLength());
				}
			}
			else if( SeriesType == SERIES_TYPE_BAR )
			{				
				BarRect.left = SeriesRect.left;
				BarRect.right = Px;
				BarRect.top = Py-BarHalfWidth;
				BarRect.bottom = BarRect.top+BarWidth;

				color = pNode->m_Color;
				if( color == NULL )
				{ ::FillRect(hDC, &BarRect, BarBrush); }				
				else
				{
					::DeleteObject(ColorBarBrush); ColorBarBrush=NULL;
					ColorBarBrush = ::CreateSolidBrush(color);
					::FillRect(hDC, &BarRect, ColorBarBrush);
				}
					
				if( ShowMarkValue )
				{
					::SelectObject(hDC, MarkPen);
					::MoveToEx(hDC, Px+20, Py, NULL);
					::LineTo(hDC, Px, Py);
					Text.Format(_T("%.2f"), pNode->m_YValue);
					::TextOut(hDC, Px+25, Py, Text, Text.GetLength());
				}
			}
			IsFirst = false;
			OldIsVisible = IsVisible;
		}
	}
	::SelectObject(hDC, def_font);
	::SelectObject(hDC, OldPen);
	::DeleteObject(font);
	::DeleteObject(Pen);	
	::DeleteObject(MarkPen);	
	::DeleteObject(DotPen);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);
	::DeleteObject(BarBrush);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawTitle(HDC hDC)
{
	if ( CheckDrawTitle() == false ) { return; }
	COLORREF color = m_TitleColor;
	COLORREF TitleFontColor = this->m_TitleFont.sFontColor;	
	HBRUSH Brush = ::CreateSolidBrush(color);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(this->m_DrawDC, Brush);
	RECT TitelRect = m_DrawRect;
	int TitelH = this->m_TitleFont.sFontSize;
	TitelRect.bottom = TitelRect.top+TitelH;
	::FillRect(hDC, &TitelRect, Brush);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);

	//HPEN Pen = ::CreatePen(PS_SOLID, 2, 0x555555);	//chia 0921
	HPEN Pen = ::CreatePen(PS_SOLID, 2, m_FrameColor);
	HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);
	::MoveToEx(hDC, TitelRect.left+1, TitelRect.top+1, NULL);
	::LineTo(hDC, TitelRect.right-1, TitelRect.top+1);
	::LineTo(hDC, TitelRect.right-1, TitelRect.bottom);
	::LineTo(hDC, TitelRect.left+1, TitelRect.bottom);
	::LineTo(hDC, TitelRect.left+1, TitelRect.top+1);
	::LineTo(hDC, TitelRect.left+1, m_DrawRect.bottom-1);
	::LineTo(hDC, TitelRect.right-1, m_DrawRect.bottom-1);
	::LineTo(hDC, TitelRect.right-1, m_DrawRect.top+1);

	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);

	::SetTextAlign(hDC, TA_CENTER);
	::SetTextColor(hDC, TitleFontColor);

	int FontWeight = FW_NORMAL;
	int FontItalic = 0;
	int FontUnderLine = 0;
	int FontSize =  this->m_TitleFont.sFontSize;
	int Escapement = this->m_TitleFont.sEscapement;
	if( this->m_TitleFont.sIsBold == true  ) { FontWeight = FW_BOLD; }
	if( this->m_TitleFont.sIsItalic == true  ) { FontItalic = 1; }
	if( this->m_TitleFont.sIsUnderline == true  ) { FontUnderLine = 1; }
	CFont font;
	VERIFY(font.CreateFont(
		FontSize,                        // nHeight
		0,                         // nWidth
		Escapement,                         // nEscapement
		0,                         // nOrientation
		FontWeight,                 // nWeight
		FontItalic,                 // bItalic
		FontUnderLine,              // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		this->m_TitleFont.sFontType));                 // lpszFacename		//"Times New Roman"Arial
	
	int Px=0, Py=0;
	int Len = 0;
	CString TempStr = m_Title;
	Len = TempStr.GetLength();
	Px = (int)((TitelRect.right-TitelRect.left)*0.5);
	Py = TitelRect.top+5;
	
	CFont* def_font = (CFont*)::SelectObject(hDC, font);

	::TextOut(hDC, Px, Py, TempStr, Len);

	::SelectObject(hDC, def_font);
	font.DeleteObject();	
}
//----------------------------------------------------------------------------//
void CJetChart::DrawInfo(HDC hDC)//chia 1050517		//Draw InfoStr
{
	COLORREF FontColor = this->m_InfoFont.sFontColor;
	
	::SetTextAlign(hDC, TA_LEFT);
	::SetTextColor(hDC, FontColor);

	int FontWeight = FW_NORMAL;
	int FontItalic = 0;
	int FontUnderLine = 0;
	int FontSize =  this->m_InfoFont.sFontSize;
	int Escapement = this->m_InfoFont.sEscapement;
	if( this->m_InfoFont.sIsBold == true  ) { FontWeight = FW_BOLD; }
	if( this->m_InfoFont.sIsItalic == true  ) { FontItalic = 1; }
	if( this->m_InfoFont.sIsUnderline == true  ) { FontUnderLine = 1; }
	CFont font;
	VERIFY(font.CreateFont(
		FontSize,                        // nHeight
		0,                         // nWidth
		Escapement,                         // nEscapement
		0,                         // nOrientation
		FontWeight,                 // nWeight
		FontItalic,                 // bItalic
		FontUnderLine,              // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		this->m_InfoFont.sFontType));                 // lpszFacename		//"Times New Roman"Arial
		
	CFont* def_font = (CFont*)::SelectObject(hDC, font);	
	
	int Gap = 4;
	int Len1 = (int)(_tcslen(m_InfoStr01));
	int Len2 = (int)(_tcslen(m_InfoStr02));
	int Len3 = (int)(_tcslen(m_InfoStr03));
	int Len4 = (int)(_tcslen(m_InfoStr04));	
	int Len = Len1;
	if( Len < Len2 ) { Len = Len2;}
	if( Len < Len3 ) { Len = Len3;}
	if( Len < Len4 ) { Len = Len4;}
	int Px = (int)(m_DrawRect.right-Len*FontSize*0.5);	
	int Py = (int)(m_DrawRect.top+Gap);
	//int Py = (int)(m_DrawRect.top+40);
	if ( CheckDrawTitle() )
	{	Py += m_TitleFont.sFontSize+Gap;	}
	if( this->m_ChartType == CHART_TYPE_RADAR )
	{	Py = m_DrawRect.top+20;	}
	if ( JET_CHART_INFO_LOCATION_MODE_LEFT == m_InfoLocationMode )
	{	Px = m_DrawRect.left+16;	}

	CString InfoStr = _T("");
	if( Len1 > 0 )
	{
		::TextOut(hDC, Px, Py, m_InfoStr01, Len1);
		Py += FontSize; 
	}
	if( Len2 > 0 )
	{
		::TextOut(hDC, Px, Py, m_InfoStr02, Len2);
		Py += FontSize; 
	}
	if( Len3 > 0 )
	{
		::TextOut(hDC, Px, Py, m_InfoStr03, Len3);
		Py += FontSize; 
	}	
	if( Len4 > 0 )
	{
		::TextOut(hDC, Px, Py, m_InfoStr04, Len4);
		Py += FontSize; 
	}

	::SelectObject(hDC, def_font);
	font.DeleteObject();	
}
//----------------------------------------------------------------------------//
void CJetChart::DrawCurrentLine(HDC hDC)
{	
	return;
}
//----------------------------------------------------------------------------//
void CJetChart::DrawFrame_Transpos(HDC hDC)
{
	int FontWeight = FW_NORMAL;
	int FontItalic = 0;
	int FontUnderLine = 0;
	int FontSize = this->m_FrameFont.sFontSize;
	int Escapement = this->m_FrameFont.sEscapement;
	if( this->m_FrameFont.sIsBold == true  ) { FontWeight = FW_BOLD; }
	if( this->m_FrameFont.sIsItalic == true  ) { FontItalic = 1; }
	if( this->m_FrameFont.sIsUnderline == true  ) { FontUnderLine = 1; }

//	Escapement = 900;

	CFont font;
	VERIFY(font.CreateFont(
		FontSize,                        // nHeight
		0,                         // nWidth
		Escapement,                         // nEscapement
		0,                         // nOrientation
		FontWeight,                 // nWeight
		FontItalic,                     // bItalic
		FontUnderLine,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		this->m_FrameFont.sFontType));                 // lpszFacename

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	
	//繪製框架
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = _T("");

	
	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
	}

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;
	RECT OutLineRectBottom = {0,0,0,0};
	RECT OutLineRectLeft = {0,0,0,0};

	OutLineRectBottom.left = this->m_DrawRect.left;
	OutLineRectBottom.bottom = this->m_DrawRect.bottom;
	OutLineRectBottom.right = this->m_DrawRect.right;
	OutLineRectBottom.top = ChartRect.bottom;

	OutLineRectLeft.top = this->m_DrawRect.top;
	OutLineRectLeft.right = ChartRect.left;

	HBRUSH BKBrush = ::CreateSolidBrush(m_BKColor);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, BKBrush);
	::FillRect(hDC, &OutLineRectBottom, BKBrush);
	::FillRect(hDC, &OutLineRectLeft, BKBrush);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(BKBrush);	

	COLORREF color = this->m_FrameFont.sFontColor;
	::SetTextColor(hDC, 0x000000);
	HPEN Pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);

	int Px = ChartRect.left;
	int Py = ChartRect.top;
	::MoveToEx(m_DrawDC, Px, Py, NULL);
	
	Px = ChartRect.right;
	Py = ChartRect.top;
	::LineTo(hDC, Px, Py);
	::LineTo(hDC, Px-7, Py-3);
	::MoveToEx(hDC, Px, Py, NULL);
	::LineTo(hDC, Px-7, Py+3);

	::SetTextAlign(hDC, TA_LEFT);
	UnitStr = GetYAxisUnit();
	::TextOut(hDC, Px, Py-FontSize, UnitStr, UnitStr.GetLength());

	Px = ChartRect.left;
	Py = ChartRect.top;
	::MoveToEx(hDC, Px, Py, NULL);
	Px = ChartRect.left;
	Py = ChartRect.bottom;
	::LineTo(hDC, Px, Py);
	::LineTo(hDC, Px-3, Py-7);
	::MoveToEx(hDC, Px, Py, NULL);
	::LineTo(hDC, Px+3, Py-7);
		
	::SetTextAlign(hDC, TA_CENTER);
	UnitStr = GetXAxisUnit();
	::TextOut(hDC, Px-5, Py+5, UnitStr, UnitStr.GetLength());


	int Ox = SeriesRect.left;
	int Oy = SeriesRect.top;

	const size_t TextSize = 32;
	TCHAR Text[TextSize] = _T("");
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = (int)(FontSize*1.5);

//Y軸

	Px = ChartRect.left;
	Py = ChartRect.top;
	TempD = ChartRect.right-Ox;
	int YCount = (int)(TempD/Pitch);
	int i=0;
	::SetTextAlign(hDC, TA_RIGHT);
	
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	for( i=0 ; i<=YCount ; i++ )
	{
		::SelectObject(hDC, Pen);
		if( m_IsIntYValue == true )
		{
			TempD = (int)(i*IniDivideValue+MinYValue);
			Px = (int)(Ox+i*IniDivideValue*YRatio+m_OffsetX);
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);
			_stprintf(Text, _T("%d"), TempD);
		}
		else
		{
			Px = Ox+i*Pitch+m_OffsetX;
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);
			
			//數據
			TempF = (double)i*Pitch/(double)YRatio;
			TempF = TempF+MinYValue;
			
			if( TempF > 10000 )
			{	_stprintf(Text, _T("%.2e"), TempF);	}
			else
			{	_stprintf(Text, _T("%.2f"), TempF);		}
		}			
		TextLen = (int)_tcslen(Text);
		::TextOut(hDC, Px, ChartRect.top-FontSize, Text, TextLen);		
	}
	
	//X軸
	::SetTextAlign(hDC, TA_CENTER);
	int XCount = 0;
	int nNode = 0;
	int OldPos = -1;			//chia 1050517
	bool SpaceLabel = false;	//chia 1050517
	Px = ChartRect.left;
	Py = ChartRect.top;

	Pitch = (int)(FontSize*1.5);
	if( m_XAxisLabelMode == JET_CHART_LABEL_MODE_LABEL )
	{
		CJetSeries *pSeries = m_pSeries1;
		SeriesNode_S *pNode = NULL; 
		bool Flag = false;
		int XIndex = 0;
		if( pSeries != NULL )
		{
			nNode = pSeries->GetNSeriesNode();
			XCount = MaxXIndex-MinXIndex+1;
			TempD = SeriesRect.bottom-SeriesRect.top;
			if( XCount <= 1)
			{ TempF = TempD; }
			else
			{ TempF = (double)TempD/(double)(XCount-1); }

			if( nNode != XCount ) { XIndex = MinXIndex; }
			else { XIndex = 0; }

			if( TempF < Pitch )
			{	
				
				for( i=0 ; i<XCount ; i++ )
				{
					::SelectObject(hDC, Pen);
					pNode = pSeries->GetSeriesNode(XIndex++);
					if( pNode == NULL ) { continue; }  
					Py = (int)(Oy+i*TempF+m_OffsetY);
					::MoveToEx(hDC, Px, Py, NULL);
					
					//chia 1050517
					SpaceLabel = false;
					if( OldPos > 0 )
					{ if( abs(OldPos-Py) < FontSize ) { SpaceLabel = true; } }					
					if( SpaceLabel == true )//chia 1050517
					{ ::LineTo(hDC, Px-2, Py); }
					else
					{ ::LineTo(hDC, Px-4, Py); }
										

					//數據
					//chia 1050517
					if( SpaceLabel == false )
					{ 
						_stprintf(Text, _T("%s"), pNode->m_XLabel);
						TextLen = (int)_tcslen(Text);
					
						OldPos=Py;
						::TextOut(hDC, Px-FontSize, (int)(Py-FontSize*0.5), Text, TextLen); 
					}
				}
			}
			else
			{
				if( TempF < Pitch )
				{ TempD = 1+(int)(Pitch/TempF); }
				else{ TempD = 1; }
				if(TempD <= 0) { TempD = 1;}
					
				for( i=0 ; i<XCount ; i+=TempD )
				{
					::SelectObject(hDC, Pen);
					pNode = pSeries->GetSeriesNode(XIndex);
					if( pNode == NULL ) { continue; }
					XIndex += TempD;
					Py = Oy+(int)(i*TempF)+m_OffsetY;
					::MoveToEx(hDC, Px, Py, NULL);
					
					//chia 1050517
					SpaceLabel = false;
					if( OldPos > 0 )
					{ if( abs(OldPos-Py) < FontSize ) { SpaceLabel = true; } }					
					if( SpaceLabel == true )//chia 1050517
					{ ::LineTo(hDC, Px-2, Py); }
					else
					{ ::LineTo(hDC, Px-4, Py); }

					
					//chia 1050517
					if( SpaceLabel == false )
					{ 
						_stprintf(Text, _T("%s"), pNode->m_XLabel);
						TextLen = (int)_tcslen(Text);
					
						OldPos=Py;
						::TextOut(hDC, Px-FontSize, (int)(Py-FontSize*0.5), Text, TextLen); 
					}
				}	
			}
		}
	}
	else
	{
		TempD = (int)(Pitch/XRatio);
		if( TempD <= 0 ) { TempD = 1; }
		Pitch = TempD;
		TempF = TempD*XRatio;
		TempD = ChartRect.bottom-Oy;
		XCount = (int)(TempD/TempF);

		int XIndex = 0;
		if( m_IsZoom == true ) { XIndex = MinXIndex; }
		if( MinXIndex+XCount > MaxXIndex )
		{
			XCount = MaxXIndex-MinXIndex;
		}

		Py = ChartRect.bottom;
		for( i=0 ; i<=XCount ; i++ )
		{
			::SelectObject(hDC, Pen);
			Py = Oy+(int)(i*TempF)+m_OffsetY;
			::MoveToEx(hDC, Px, Py, NULL);
					
			//chia 1050517
			SpaceLabel = false;
			if( OldPos > 0 )
			{ if( abs(OldPos-Py) < FontSize ) { SpaceLabel = true; } }					
			if( SpaceLabel == true )//chia 1050517
			{ ::LineTo(hDC, Px-2, Py); }
			else
			{ ::LineTo(hDC, Px-4, Py); }

			
			//數據
			//chia 1050517
			if( SpaceLabel == false )
			{ 
				_stprintf(Text, _T("%d"), (int)((i*Pitch)+XIndex));
				TextLen = (int)_tcslen(Text);
					
				OldPos=Py;
				::TextOut(hDC, Px-FontSize, (int)(Py-FontSize*0.5), Text, TextLen); 
			}
		}
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);
	
	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawRadarFrame(HDC hDC)
{
	int FontWeight = FW_NORMAL;
	int FontItalic = 0;
	int FontUnderLine = 0;
	int FontSize = this->m_FrameFont.sFontSize;
	int Escapement = this->m_FrameFont.sEscapement;
	if( this->m_FrameFont.sIsBold == true  ) { FontWeight = FW_BOLD; }
	if( this->m_FrameFont.sIsItalic == true  ) { FontItalic = 1; }
	if( this->m_FrameFont.sIsUnderline == true  ) { FontUnderLine = 1; }

	CFont font;
	VERIFY(font.CreateFont(
		FontSize,                        // nHeight
		0,                         // nWidth
		Escapement,                         // nEscapement
		0,                         // nOrientation
		FontWeight,                 // nWeight
		FontItalic,                     // bItalic
		FontUnderLine,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		this->m_FrameFont.sFontType));                 // lpszFacename

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	
	//繪製框架
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = _T("");
	
	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;
	RECT OutLineRectBottom = {0,0,0,0};
	RECT OutLineRectLeft = {0,0,0,0};

	OutLineRectBottom.left = this->m_DrawRect.left;
	OutLineRectBottom.bottom = this->m_DrawRect.bottom;
	OutLineRectBottom.right = this->m_DrawRect.right;
	OutLineRectBottom.top = ChartRect.bottom;

	OutLineRectLeft.top = this->m_DrawRect.top;
	OutLineRectLeft.right = ChartRect.left;

	HBRUSH BKBrush = ::CreateSolidBrush(m_BKColor);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, BKBrush);
	::FillRect(hDC, &OutLineRectBottom, BKBrush);
	::FillRect(hDC, &OutLineRectLeft, BKBrush);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(BKBrush);	

	COLORREF color = this->m_FrameFont.sFontColor;
	::SetTextColor(hDC, color);
	HPEN Pen = ::CreatePen(PS_SOLID, 1, 0x888888);
	HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);

	int Px = ChartRect.left;
	int Py = ChartRect.bottom;
	int Ox = (int)((SeriesRect.left+SeriesRect.right)*0.5);
	int Oy = (int)((SeriesRect.bottom+SeriesRect.top)*0.5);

	::MoveToEx(m_DrawDC, Ox, Oy, NULL);

	::SetTextAlign(hDC, TA_RIGHT);
	UnitStr = GetXAxisUnit();
	::TextOut(hDC, Px-10, Py+5, UnitStr, UnitStr.GetLength());

//	int HalfRadius = this->m_RadarRadus*XRatio;
//	::Arc(hDC, Ox-HalfRadius, Oy-HalfRadius, Ox+HalfRadius, Oy+HalfRadius, 0,0,0,0);

	const size_t TextSize = 64;
	TCHAR Text[TextSize] = _T("");
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = 40;

//Y軸	
	Px = ChartRect.left;
	Py = ChartRect.bottom;
	::SetTextAlign(hDC, TA_RIGHT);
	Pitch = (int)((this->m_RadarRadus*YRatio)/2);
	if( Pitch < FontSize*1.5 ) { (int)(Pitch = FontSize*1.5); }
	TempD = Oy-ChartRect.top;
	int YCount = (int)(m_RadarRadus*YRatio/Pitch);	
	int i=0;	
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	if( m_IsIntYValue == true )
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			TempD = (int)(i*IniDivideValue+this->m_RadarCYValue);
			Py = (int)(Oy-i*IniDivideValue*YRatio);		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
			_stprintf(Text, _T("%d"), TempD);	
			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, ChartRect.left-10, Py-8, Text, TextLen);

			TempD = (int)(-i*IniDivideValue+this->m_RadarCYValue);
			Py = (int)(Oy+i*IniDivideValue*YRatio);		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
			_stprintf(Text, _T("%d"), TempD);	
			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, ChartRect.left-10, Py-8, Text, TextLen);
		}
	}
	else
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			TempF = i*Pitch/YRatio+this->m_RadarCYValue;
			Py = Oy-i*Pitch;			
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
				
			if( TempF > 10000 )
			{ _stprintf(Text, _T("%.2e"), TempF); }
			else
			{ _stprintf(Text, _T("%.2f"), TempF);	}

			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, ChartRect.left-10, Py-8, Text, TextLen);
			
			TempF = -i*Pitch/YRatio+this->m_RadarCYValue;
			Py = Oy+i*Pitch;			
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
				
			if( TempF > 10000 )
			{ _stprintf(Text, _T("%.2e"), TempF); }
			else
			{ _stprintf(Text, _T("%.2f"), TempF);	}

			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, ChartRect.left-10, Py-8, Text, TextLen);
		}	
	}
	
	//X軸
	Px = ChartRect.left;
	Py = ChartRect.bottom;

	::SetTextAlign(hDC, TA_CENTER);
	Pitch = (int)((this->m_RadarRadus*XRatio)/2);
	if( Pitch < FontSize*1.5 ) { (int)(Pitch = FontSize*1.5); }
	TempD = Ox-ChartRect.left;
	//int XCount = TempD/Pitch;	
	int XCount = (int)(m_RadarRadus*XRatio/Pitch);
	IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	int TextOutPy = 0;
	bool ChangePos = false;
	bool Flag = false;
	TempD = (ChartRect.right-ChartRect.left)/(XCount*2+1);
	if( TempD < Pitch ) { ChangePos = true; }

	if( m_IsIntYValue == true )
	{
		for( i=0 ; i<=XCount ; i++ )
		{
			TempD = (int)(-i*IniDivideValue+this->m_RadarCXValue);
			Px = (int)(Ox-i*IniDivideValue*XRatio);		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);	
			_stprintf(Text, _T("%d"), TempD);	
			TextLen = (int)_tcslen(Text);
			if( ChangePos == true && Flag == true)
			{ TextOutPy = Py+20; }
			else { TextOutPy = Py+5; }
			::TextOut(hDC, Px, TextOutPy, Text, TextLen); 			

			TempD = (int)(i*IniDivideValue+this->m_RadarCXValue);
			Px = (int)(Ox+i*IniDivideValue*XRatio);			
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);	
			_stprintf(Text, _T("%d"), TempD);	
			TextLen = (int)_tcslen(Text);
			if( ChangePos == true && Flag == true)
			{ TextOutPy = Py+20; }
			else { TextOutPy = Py+5; }
			::TextOut(hDC, Px, TextOutPy, Text, TextLen);
			Flag = !Flag;	
		}
	}
	else
	{
		for( i=0 ; i<=XCount ; i++ )
		{
			TempF = -i*Pitch/XRatio+this->m_RadarCXValue;
			Px = Ox-i*Pitch;				
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);	
				
			if( TempF > 10000 )
			{ _stprintf(Text, _T("%.2e"), TempF); }
			else
			{ _stprintf(Text, _T("%.2f"), TempF);	}

			TextLen = (int)_tcslen(Text);
			if( ChangePos == true && Flag == true)
			{ TextOutPy = Py+20; }
			else { TextOutPy = Py+5; }
			::TextOut(hDC, Px, TextOutPy, Text, TextLen);
			
			TempF = i*Pitch/XRatio+this->m_RadarCXValue;
			Px = Ox+i*Pitch;				
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);	
				
			if( TempF > 10000 )
			{ _stprintf(Text, _T("%.2e"), TempF); }
			else
			{ _stprintf(Text, _T("%.2f"), TempF);	}

			TextLen = (int)_tcslen(Text);
			if( ChangePos == true && Flag == true)
			{ TextOutPy = Py+20; }
			else { TextOutPy = Py+5; }
			::TextOut(hDC, Px, TextOutPy, Text, TextLen);
			Flag = !Flag;	
		}	
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);
	
	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawFrame(HDC hDC)
{
	if( this->m_IsTranspose == true ) 
	{
		DrawFrame_Transpos(hDC);
		return;
	}
	int FontWeight = FW_NORMAL;
	int FontItalic = 0;
	int FontUnderLine = 0;
	int FontSize = this->m_FrameFont.sFontSize;
	int Escapement = this->m_FrameFont.sEscapement;
	if( this->m_FrameFont.sIsBold == true  ) { FontWeight = FW_BOLD; }
	if( this->m_FrameFont.sIsItalic == true  ) { FontItalic = 1; }
	if( this->m_FrameFont.sIsUnderline == true  ) { FontUnderLine = 1; }

	CFont font;
	VERIFY(font.CreateFont(
		FontSize,                        // nHeight
		0,                         // nWidth
		Escapement,                         // nEscapement
		0,                         // nOrientation
		FontWeight,                 // nWeight
		FontItalic,                     // bItalic
		FontUnderLine,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		this->m_FrameFont.sFontType));                 // lpszFacename

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	
	//繪製框架
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = _T("");

	
	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
	}

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;
	RECT OutLineRectBottom = {0,0,0,0};
	RECT OutLineRectLeft = {0,0,0,0};

	OutLineRectBottom.left = this->m_DrawRect.left;
	OutLineRectBottom.bottom = this->m_DrawRect.bottom;
	OutLineRectBottom.right = this->m_DrawRect.right;
	OutLineRectBottom.top = ChartRect.bottom;

	OutLineRectLeft.top = this->m_DrawRect.top;
	OutLineRectLeft.right = ChartRect.left;

	HBRUSH BKBrush = ::CreateSolidBrush(m_BKColor);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, BKBrush);
	::FillRect(hDC, &OutLineRectBottom, BKBrush);
	::FillRect(hDC, &OutLineRectLeft, BKBrush);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(BKBrush);	

	COLORREF color = this->m_FrameFont.sFontColor;
	::SetTextColor(hDC, color);
	HPEN Pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);

	int Px = ChartRect.left;
	int Py = ChartRect.bottom;
	::MoveToEx(m_DrawDC, Px, Py, NULL);
	
	Px = ChartRect.right;
	Py = ChartRect.bottom;
	::LineTo(hDC, Px, Py);
	::LineTo(hDC, Px-7, Py-3);
	::MoveToEx(hDC, Px, Py, NULL);
	::LineTo(hDC, Px-7, Py+3);

	::SetTextAlign(hDC, TA_LEFT);
	UnitStr = GetXAxisUnit();
	::TextOut(hDC, Px+5, Py-5, UnitStr, UnitStr.GetLength());

	Px = ChartRect.left;
	Py = ChartRect.bottom;
	::MoveToEx(hDC, Px, Py, NULL);
	Px = ChartRect.left;
	Py = ChartRect.top;
	::LineTo(hDC, Px, Py);
	::LineTo(hDC, Px-3, Py+7);
	::MoveToEx(hDC, Px, Py, NULL);
	::LineTo(hDC, Px+3, Py+7);
		
	::SetTextAlign(hDC, TA_CENTER);
	UnitStr = GetYAxisUnit();
	::TextOut(hDC, Px, Py-20, UnitStr, UnitStr.GetLength());


	int Ox = SeriesRect.left;
	int Oy = SeriesRect.bottom;

	const size_t TextSize = 64;
	TCHAR Text[TextSize] = _T("");	
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = 40;

//Y軸

	Pitch = (int)(FontSize*1.5);
	TempD = Oy-ChartRect.top;
	int YCount = TempD/Pitch;
	int i=0;
	::SetTextAlign(hDC, TA_RIGHT);
	
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	for( i=0 ; i<=YCount ; i++ )
	{
		::SelectObject(hDC, Pen);
		if( m_IsIntYValue == true )
		{
			TempD = (int)(i*IniDivideValue+MinYValue);
			Py = (int)(Oy-i*IniDivideValue*YRatio+m_OffsetY);
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
			_stprintf(Text, _T("%d"), TempD);
		}
		else
		{
			Py = Oy-i*Pitch+m_OffsetY;
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px-4, Py);
			
			//數據
			TempF = (double)i*Pitch/(double)YRatio;
			TempF = TempF+MinYValue;
			
			if( TempF > 10000 )
			{
				_stprintf(Text, _T("%.2e"), TempF);
			}
			else
			{
				_stprintf(Text, _T("%.2f"), TempF);	
			}
		}

		TextLen = (int)_tcslen(Text);
		::TextOut(hDC, ChartRect.left-10, Py-8, Text, TextLen);
		
	}
	
	//Show Y-Axis Percentage - Kai-20220805	
	if ( true == m_YAxisPercentageVisible )
	{		
		float Target=MaxYValue;
		const int YCountR=4;
		const int PxR=ChartRect.right;
		HPEN PenR = ::CreatePen(PS_DOT, 1, 0xE8A200);
		HPEN OldPenR = (HPEN)::SelectObject(hDC, PenR);
		if ( m_YAxisPercentageTarget > 0 )
		{	Target = m_YAxisPercentageTarget;	}		
		for ( i=0; i<=YCountR; i++ )
		{	
			const float Percentage=i*25;
			const float Value=Percentage*Target/100.0f;			
			const int   nV =(int)(((Value-MinYValue)*YRatio)+0.5);
			Py = Oy-nV+m_OffsetY;			
			::MoveToEx(hDC, PxR, Py, NULL);
			::LineTo(hDC, Px, Py);			
			_stprintf(Text, _T("%.1f%%"), Percentage);
			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, ChartRect.right+36, Py-8, Text, TextLen);
		}
		_stprintf(Text, _T("(%.0f %s)"), Target, UnitStr);
		TextLen = (int)_tcslen(Text);
		::TextOut(hDC, ChartRect.right+36, Py-FontSize-8, Text, TextLen);
		::SelectObject(hDC, OldPenR);
		::DeleteObject(PenR);
	}

	if (true == m_XAxisGribLineVisible)
	{
		HPEN PenR = ::CreatePen(PS_DASH, 3, 0xcccccc);
		HPEN OldPenR = (HPEN)::SelectObject(hDC, PenR);

		for (i = 0; i < m_XAxisGribLineTarget.size(); i++)
		{
			Px = (int)(Ox + m_XAxisGribLineTarget[i] * XRatio);
			Py = Oy + MinYValue + m_OffsetY;
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, SeriesRect.top + 30);
		}
		::SelectObject(hDC, OldPenR);
		::DeleteObject(PenR);
	}
	//X軸
	::SetTextAlign(hDC, TA_CENTER);
	int XCount = 0;
	int nNode = 0;
	Py = ChartRect.bottom;

	Pitch = (int)(FontSize*1.5);
	if( m_XAxisLabelMode == JET_CHART_LABEL_MODE_LABEL )
	{
		CJetSeries *pSeries = m_pSeries1;
		SeriesNode_S *pNode = NULL; 
		bool Flag = false;
		int XIndex = 0;
		if( pSeries != NULL )
		{
			nNode = pSeries->GetNSeriesNode();
			XCount = MaxXIndex-MinXIndex+1;
			TempD = SeriesRect.right-SeriesRect.left;
			if( XCount <= 1)
			{ TempF = TempD; }
			else
			{ TempF = (double)TempD/(double)(XCount-1); }

			if( nNode != XCount ) { XIndex = MinXIndex; }
			else { XIndex = 0; }

			if( TempF < Pitch )
			{					
				for( i=0 ; i<XCount ; i++ )
				{
					::SelectObject(hDC, Pen);
					pNode = pSeries->GetSeriesNode(XIndex++);
					if( pNode == NULL ) { continue; }  
					Px = (int)(Ox+i*TempF+m_OffsetX);
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, Px, Py+4);
					
					//數據
					_stprintf(Text, _T("%s"), pNode->m_XLabel);
					TextLen = (int)_tcslen(Text);
					if( Flag == false )
					{ ::TextOut(hDC, Px, Py+5, Text, TextLen); }
					else
					{ ::TextOut(hDC, Px, Py+20, Text, TextLen); }
					Flag = !Flag;
				}
			}
			else
			{
				if( TempF < Pitch )
				{ TempD = 1+(int)(Pitch/TempF); }
				else{ TempD = 1; }
				if(TempD <= 0) { TempD = 1;}
					
				for( i=0 ; i<XCount ; i+=TempD )
				{
					::SelectObject(hDC, Pen);
					pNode = pSeries->GetSeriesNode(XIndex);
					if( pNode == NULL ) { continue; }
					XIndex += TempD;
					Px = Ox+(int)(i*TempF)+m_OffsetX;
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, Px, Py+4);

					//數據
					_stprintf(Text, _T("%s"), pNode->m_XLabel);
					TextLen = (int)_tcslen(Text);
					::TextOut(hDC, Px, Py+5, Text, TextLen);	
				}	
			}
		}
	}
	else
	{
		TempD = (int)(Pitch/XRatio);
		if( TempD <= 0 ) { TempD = 1; }
		Pitch = TempD;
		TempF = TempD*XRatio;
		TempD = ChartRect.right-Ox;
		XCount = (int)(TempD/TempF);

		int XIndex = 0;
		if( m_IsZoom == true ) { XIndex = MinXIndex; }
		if( MinXIndex+XCount > MaxXIndex )
		{
			XCount = MaxXIndex-MinXIndex;
		}

		Py = ChartRect.bottom;
		for( i=0 ; i<=XCount ; i++ )
		{
			::SelectObject(hDC, Pen);
			Px = Ox+(int)(i*TempF)+m_OffsetX;
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, Py+4);


			//數據
			_stprintf(Text, _T("%d"), (i*Pitch)+XIndex);
			TextLen = (int)_tcslen(Text);
			::TextOut(hDC, Px, Py+5, Text, TextLen);
		}
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);
	
	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawGridLine_Transpos(HDC hDC)
{
	if( m_ShowGridLine == false ) { return; }
	//繪製框架
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = "";

	
	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
	}

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetX;
	SeriesRect.left = m_SeriesRect.left+m_OffsetX;
	SeriesRect.top = m_SeriesRect.top+m_OffsetY;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetY;

	COLORREF GridColor = m_GridColor;
	HPEN PenDot = ::CreatePen(PS_DOT, 1, GridColor);
	HPEN OldPen = (HPEN)::SelectObject(hDC, PenDot);

	int Px = ChartRect.left;
	int Py = ChartRect.bottom;
	::MoveToEx(m_DrawDC, Px, Py, NULL);	

//繪製格線
	int Ox = SeriesRect.left;
	int Oy = SeriesRect.top;

	char Text[16] = "";
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = 40;

//Y軸	
	int FontSize = this->m_FrameFont.sFontSize;
	Pitch = (int)(FontSize*1.5);
	TempD = ChartRect.right-Ox;
	int YCount = TempD/Pitch;
	int i=0;
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	::SelectObject(hDC, PenDot);
	if( m_IsIntYValue == true )
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			TempD = (int)(i*IniDivideValue+MinYValue);
			Px = (int)(Ox+i*IniDivideValue*YRatio);
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, ChartRect.top);		
		}
	}
	else
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			Py = Ox-i*Pitch;		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, ChartRect.top);	
		}	
	}

	
	//X軸
	int XCount = 0;
	int nNode = 0;
	Px = ChartRect.left;
	Pitch = (int)(FontSize*1.5);
	if( m_XAxisLabelMode == JET_CHART_LABEL_MODE_LABEL )
	{
		CJetSeries *pSeries = m_pSeries1;
		SeriesNode_S *pNode = NULL; 
		bool Flag = false;
		int XIndex = 0;
		if( pSeries != NULL )
		{
			nNode = pSeries->GetNSeriesNode();
			XCount = MaxXIndex-MinXIndex+1;
			TempD = SeriesRect.bottom-SeriesRect.top;
			if( XCount <= 1)
			{ TempF = TempD; }
			else
			{ TempF = (double)TempD/(double)(XCount-1); }

			if( nNode != XCount ) { XIndex = MinXIndex; }
			else { XIndex = 0; }

			if( TempF < Pitch )
			{					
				for( i=0 ; i<XCount ; i++ )
				{
					pNode = pSeries->GetSeriesNode(XIndex++);
					if( pNode == NULL ) { continue; }  
					Py = Oy+(int)(i*TempF);

					::SelectObject(hDC, PenDot);
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, ChartRect.right, Py);
				}
			}
			else
			{
				if( TempF < Pitch )
				{ TempD = 1+(int)(Pitch/TempF); }
				else{ TempD = 1; }
				if(TempD <= 0) { TempD = 1;}
					
				for( i=0 ; i<XCount ; i+=TempD )
				{
					pNode = pSeries->GetSeriesNode(XIndex);
					if( pNode == NULL ) { continue; }
					XIndex += TempD;
					Py = Oy+(int)(i*TempF);

					::SelectObject(hDC, PenDot);
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, ChartRect.right, Py);	
				}	
			}
		}
	}
	else
	{
		TempD = (int)(Pitch/XRatio);
		if( TempD <= 0 ) { TempD = 1; }
		Pitch = TempD;
		TempF = TempD*XRatio;
		TempD = ChartRect.bottom-Oy;
		XCount = (int)(TempD/TempF);

		int XIndex = 0;
		if( m_IsZoom == true ) { XIndex = MinXIndex; }
		if( MinXIndex+XCount > MaxXIndex )
		{
			XCount = MaxXIndex-MinXIndex;
		}

		Py = ChartRect.bottom;
		for( i=0 ; i<=XCount ; i++ )
		{
			Py = Oy+(int)(i*TempF);

			::SelectObject(hDC, PenDot);
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);	
		}
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(PenDot);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawRadarGridLine(HDC hDC)
{
	//繪製框架
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = "";

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetX;
	SeriesRect.left = m_SeriesRect.left+m_OffsetX;
	SeriesRect.top = m_SeriesRect.top+m_OffsetY;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetY;

	COLORREF CircleColor = 0x888888;
	COLORREF GridColor = 0xcccccc;
	HPEN Pen = ::CreatePen(PS_SOLID, 1, CircleColor);
	HPEN PenDot = ::CreatePen(PS_DOT, 1, GridColor);
	HPEN OldPen = (HPEN)::SelectObject(hDC, PenDot);


//繪製格線
	int Ox = (int)((SeriesRect.left+SeriesRect.right)*0.5);
	int Oy = (int)((SeriesRect.top+SeriesRect.bottom)*0.5);

	int Px = Ox;
	int Py = Oy;

	char Text[16] = "";
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = 40;

		
	::MoveToEx(hDC, ChartRect.left, Oy, NULL);
	::LineTo(hDC, ChartRect.right, Oy);	
		
	::MoveToEx(hDC, Ox, ChartRect.top, NULL);
	::LineTo(hDC, Ox, ChartRect.bottom);

//Y軸
	bool IsDrawGrid = true;
	bool IsDrawCircle = true;
	int HalfRadius = 0;
	::SelectObject(hDC, PenDot);
	int FontSize = this->m_FrameFont.sFontSize;
	
	Pitch = (int)((this->m_RadarRadus*YRatio)/2);
	if( Pitch < FontSize*1.5 ) { (int)(Pitch = FontSize*1.5); }
	TempD = Oy-ChartRect.top;
	int YCount = (int)(m_RadarRadus*YRatio/Pitch);	
	int i=0;	
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	::SelectObject(hDC, Pen);
	if( m_IsIntYValue == true )
	{		
		for( i=0 ; i<=YCount ; i++ )
		{
			if( i>0 && IsDrawCircle == true )
			{
				::SelectObject(hDC, Pen);
				HalfRadius = (int)(i*IniDivideValue*YRatio);
				::Arc(hDC, Ox-HalfRadius, Oy-HalfRadius, Ox+HalfRadius, Oy+HalfRadius, 0,0,0,0);
			}
			if( IsDrawGrid == false ) { continue; }	
			
			if( i > 0 )
			{ ::SelectObject(hDC, PenDot); }

			Py = (int)(Oy-i*IniDivideValue*YRatio);		
			::MoveToEx(hDC, ChartRect.left, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);	

			Py = (int)(Oy+i*IniDivideValue*YRatio);		
			::MoveToEx(hDC, ChartRect.left, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);
			
			::SelectObject(hDC, PenDot);
		}
	}
	else
	{
		for( i=0 ; i<=YCount ; i++ )
		{

			if( i>0 && IsDrawCircle == true )
			{
				::SelectObject(hDC, Pen);
				HalfRadius = i*Pitch;
				::Arc(hDC, Ox-HalfRadius, Oy-HalfRadius, Ox+HalfRadius, Oy+HalfRadius, 0,0,0,0);
			}
			if( IsDrawGrid == false ) { continue; }	
			if( i > 0 )
			{ ::SelectObject(hDC, PenDot); }

			Py = Oy-i*Pitch;				
			::MoveToEx(hDC, ChartRect.left, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);
			
			Py = Oy+i*Pitch;				
			::MoveToEx(hDC, ChartRect.left, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);	
		}	
	}

	
	//X軸
	Px = Ox;
	Py = Oy;

	Pitch = (int)((this->m_RadarRadus*XRatio)/2);
	if( Pitch < FontSize*1.5 ) { Pitch = (int)(FontSize*1.5); }
	TempD = Ox-ChartRect.left;
	int XCount = (int)(m_RadarRadus*XRatio/Pitch);	
	IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	if( IsDrawGrid == true )
	{
		::SelectObject(hDC, Pen);

		if( m_IsIntYValue == true )
		{
			for( i=0 ; i<=XCount ; i++ )
			{
				if( i > 0 )
				{ ::SelectObject(hDC, PenDot); }
				Px = (int)(Ox-i*IniDivideValue*XRatio);		
				::MoveToEx(hDC, Px, ChartRect.top, NULL);
				::LineTo(hDC, Px, ChartRect.bottom);	

				Px = (int)(Ox+i*IniDivideValue*XRatio);	
				::MoveToEx(hDC, Px, ChartRect.top, NULL);
				::LineTo(hDC, Px, ChartRect.bottom);
			}
		}
		else
		{
			for( i=0 ; i<=YCount ; i++ )
			{
				if( i > 0 )
				{ ::SelectObject(hDC, PenDot); }
				Px = Ox-i*Pitch;					
				::MoveToEx(hDC, Px, ChartRect.top, NULL);
				::LineTo(hDC, Px, ChartRect.bottom);
				
				Px = Ox+i*Pitch;			
				::MoveToEx(hDC, Px, ChartRect.top, NULL);
				::LineTo(hDC, Px, ChartRect.bottom);
			}	
		}
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(PenDot);
	::DeleteObject(Pen);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawGridLine(HDC hDC)
{		
	if( m_ShowGridLine == false ) { return; }
	if( m_IsTranspose == true )
	{
		DrawGridLine_Transpos(hDC);
		return;
	}
	//繪製框架
	int MinXIndex	= m_MinXIndex;
	int MaxXIndex	= m_MaxXIndex;
	float MinYValue = m_MinYValue;
	float MaxYValue = m_MaxYValue;
	float XRatio	= m_SeriesRatioX;
	float YRatio	= m_SeriesRatioY;
	CString UnitStr = "";
	
	if( m_IsFixYAxis == true )
	{
		MinYValue = m_YAxisFixMin;
		MaxYValue = m_YAxisFixMax;
	}
	if( m_IsZoom == true )
	{
		MinYValue = m_ZoomMinYValue;
		MaxYValue = m_ZoomMaxYValue;
		MinXIndex = m_ZoomMinXIndex;
		MaxXIndex = m_ZoomMaxXIndex;
	}

	RECT ChartRect	= m_ChartRect;
	RECT SeriesRect = m_SeriesRect;

	SeriesRect.right = m_SeriesRect.right+m_OffsetX;
	SeriesRect.left = m_SeriesRect.left+m_OffsetX;
	SeriesRect.top = m_SeriesRect.top+m_OffsetY;
	SeriesRect.bottom = m_SeriesRect.bottom+m_OffsetY;

	COLORREF GridColor = m_GridColor;
	HPEN PenDot = ::CreatePen(PS_DOT, 1, GridColor);
	HPEN OldPen = (HPEN)::SelectObject(hDC, PenDot);

	int Px = ChartRect.left;
	int Py = ChartRect.bottom;
	::MoveToEx(m_DrawDC, Px, Py, NULL);	

//繪製格線
	int Ox = SeriesRect.left;
	int Oy = SeriesRect.bottom;

	char Text[16] = "";
	int TextLen = 0;
	int TempD = 0;
	double TempF = 0.0;
	int Pitch = 40;

//Y軸
	int FontSize = this->m_FrameFont.sFontSize;
	Pitch = (int)(FontSize*1.5);
	TempD = Oy-ChartRect.top;
	int YCount = TempD/Pitch;
	int i=0;
	
	int IniDivideValue = (int)(Pitch/(double)YRatio);
	Pitch = (int)(IniDivideValue*YRatio);

	::SelectObject(hDC, PenDot);
	if( m_IsIntYValue == true )
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			TempD = (int)(i*IniDivideValue+MinYValue);
			Py = (int)(Oy-i*IniDivideValue*YRatio);		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);		
		}
	}
	else
	{
		for( i=0 ; i<=YCount ; i++ )
		{
			Py = Oy-i*Pitch;		
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, ChartRect.right, Py);		
		}	
	}
	
	//X軸
	int XCount = 0;
	int nNode = 0;
	Py = ChartRect.bottom;
	Pitch = (int)(FontSize*1.5);
	if( m_XAxisLabelMode == JET_CHART_LABEL_MODE_LABEL )
	{
		CJetSeries *pSeries = m_pSeries1;
		SeriesNode_S *pNode = NULL; 
		bool Flag = false;
		int XIndex = 0;
		if( pSeries != NULL )
		{
			nNode = pSeries->GetNSeriesNode();
			XCount = MaxXIndex-MinXIndex+1;
			TempD = SeriesRect.right-SeriesRect.left;
			if( XCount <= 1)
			{ TempF = TempD; }
			else
			{ TempF = (double)TempD/(double)(XCount-1); }

			if( nNode != XCount ) { XIndex = MinXIndex; }
			else { XIndex = 0; }

			if( TempF < Pitch )
			{					
				for( i=0 ; i<XCount ; i++ )
				{
					pNode = pSeries->GetSeriesNode(XIndex++);
					if( pNode == NULL ) { continue; }  
					Px = Ox+(int)(i*TempF);

					::SelectObject(hDC, PenDot);
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, Px, ChartRect.top);
				}
			}
			else
			{
				if( TempF < Pitch )
				{ TempD = 1+(int)(Pitch/TempF); }
				else{ TempD = 1; }
				if(TempD <= 0) { TempD = 1;}
					
				for( i=0 ; i<XCount ; i+=TempD )
				{
					pNode = pSeries->GetSeriesNode(XIndex);
					if( pNode == NULL ) { continue; }
					XIndex += TempD;
					Px = Ox+(int)(i*TempF);

					::SelectObject(hDC, PenDot);
					::MoveToEx(hDC, Px, Py, NULL);
					::LineTo(hDC, Px, ChartRect.top);	
				}	
			}
		}
	}
	else
	{
		TempD = (int)(Pitch/XRatio);
		if( TempD <= 0 ) { TempD = 1; }
		Pitch = TempD;
		TempF = TempD*XRatio;
		TempD = ChartRect.right-Ox;
		XCount = (int)(TempD/TempF);

		int XIndex = 0;
		if( m_IsZoom == true ) { XIndex = MinXIndex; }
		if( MinXIndex+XCount > MaxXIndex )
		{
			XCount = MaxXIndex-MinXIndex;
		}

		Py = ChartRect.bottom;
		for( i=0 ; i<=XCount ; i++ )
		{
			Px = Ox+(int)(i*TempF);

			::SelectObject(hDC, PenDot);
			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px, ChartRect.top);
		}
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(PenDot);
}
//----------------------------------------------------------------------------//
void CJetChart::DrawLegendLine(HDC hDC, CJetSeries *pSeries, int &index)
{
	if( pSeries == NULL ) { return; } 
	RECT Rect = m_LegendRect;
	COLORREF color = 0x000000;
	int PenStyle = PS_SOLID;
	int LineWidth = 1;
	
	if( pSeries->GetSeriesType() == SERIES_TYPE_LINE ) 
	{ 
		color = pSeries->GetLineColor(); 
		PenStyle = PS_SOLID;
		LineWidth = 2;
	}


	if( pSeries->GetSeriesType() == SERIES_TYPE_DOT ) 
	{ 
		color = pSeries->GetDotColor(); 		
		PenStyle = PS_DOT;
		LineWidth = 1;
	}

	::SetTextAlign(hDC, TA_LEFT);
	::SetTextColor(hDC, 0x555555);

	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,                     // bItalic
		0,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);
	
	int Space = this->m_LegendSpace;
	int LegendDash = Space;
	int HalfSpace = (int)(Space*0.5);
	int Px = 0;
	int Py = Rect.top;
	int ChartW = this->m_ChartW; 
	int ChartH = this->m_ChartH;
	CString Title = "";
	int Length = 0;	
	if( pSeries != NULL )
	{
		if( pSeries->GetIsVisible() == true && pSeries->GetLegendVisible() == true )
		{
			HPEN Pen = ::CreatePen(PS_DOT, LineWidth, color);
			HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);

			Px = Rect.left+HalfSpace;
			Py = Rect.top+index*(ChartW+Space)+HalfSpace;
			Title = pSeries->GetTitle();

			::MoveToEx(hDC, Px, Py, NULL);
			::LineTo(hDC, Px+LegendDash+10, Py);

			Px = Rect.left+2*Space+10;
			Length = Title.GetLength();
			::TextOut(hDC, Px, Py-ChartH, Title, Length);			
			index++;
			
			::SelectObject(hDC, OldPen);
			::DeleteObject(Pen);
		}
	}	
	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawPieLegend(HDC hDC, CJetSeries *pSeries)
{
	if( hDC == NULL ) { return; }
	if( pSeries == NULL ) { return; }
	
	HPEN Pen = ::CreatePen(PS_SOLID, 1, 0x000000);
	HPEN OldPen = NULL;
	OldPen = (HPEN)::SelectObject(hDC, Pen);

	int VisibleCount = pSeries->GetNVisiblePieItem();
	int Count = pSeries->GetNSeriesNode();


	RECT Rect = {0,0,0,0};
	Rect = m_LegendRect;
	int LineH = m_PieLegendItemHeight;
	int nLine = m_PieLegendNLine;
	int ItemBlockW = m_PieLegendItemBlockW;
	int ItemLength = m_PieLegendItemLength;
	int nItem = m_PieLegendNItemPerLine;
	int DiffLineH=0;

	COLORREF BkColor = 0xffffff;
	//COLORREF BkColor = m_BKColor;
	::SetBkMode(hDC, TRANSPARENT);
	HBRUSH Brush = ::CreateSolidBrush(BkColor);
	HBRUSH FrameBrush = ::CreateSolidBrush(0x000000);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);
	::FillRect(hDC, &Rect, Brush);			//畫底色
	::FrameRect(hDC, &Rect, FrameBrush);	//畫外框
	::SelectObject(hDC, OldBrush);


	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		6,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		0,                     // bItalic
		0,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Consolas")));                 // lpszFacename		//"Times New Roman" "Consolas"

	CFont* def_font = (CFont*)::SelectObject(hDC, font);


	int i=0, j=0;
	POINT pt={0,0};
	RECT SubRect = {0,0,0,0};
	COLORREF ItemColor = 0;
	int ItemIndex = 0;
	float Value = 0.0f,
		  ValueP = 0.0f;
	SeriesNode_S *pNode = NULL;
	CString ItemText = _T("");
	::SetTextAlign(hDC, TA_LEFT);

	if ( m_LegendLocationMode == JET_CHART_LENGEND_LOCATION_MODE_BOTTOM ) //Vic 20161123
	{
		pt.y = Rect.bottom-LineH+5;	//在下就從下開始
		DiffLineH = -LineH;
	}
	else
	{
		pt.y = Rect.top+5;	//在右就從上開始
		DiffLineH = LineH;
	}

	for( i=0 ; i<nLine ; i++ )
	{
		pt.x = Rect.left+5;
		j=0;
		while( j<nItem )
		{
			if( ItemIndex >= Count ) { break; } 
			pNode = pSeries->GetSeriesNode(ItemIndex++);
			if( pNode == NULL ) 
			{ continue; }
			Value = pNode->m_YValue;
			if( Value <= 0 ) { continue; }
			j++;
			
			ValueP = 100.0f*Value/m_SeiresValueSum;
			ItemText.Format(_T("%-10s %4.1f%% %5.0f"), pNode->m_XLabel, ValueP, Value);

			//ItemText = pNode->m_XLabel;
			ItemColor = m_PieColorList[pNode->m_XIndex];	

			SubRect.left = pt.x;
			SubRect.right = SubRect.left+ItemBlockW;
			SubRect.top = pt.y;
			SubRect.bottom = SubRect.top + ItemBlockW;

			::DeleteObject(Brush);
			Brush = ::CreateSolidBrush(ItemColor);
			::SelectObject(hDC, Brush);
			::FillRect(hDC, &SubRect, Brush);
			::FrameRect(hDC, &SubRect, FrameBrush);

			::TextOut(hDC, SubRect.right+5, SubRect.top, ItemText, ItemText.GetLength());
			pt.x +=  ItemLength;

			if( ItemIndex >= Count ) { break; } 
		};
		pt.y += DiffLineH;	//Vic 20161123
		if( ItemIndex >= Count ) { break; } 
	}
	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);

	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);
	::DeleteObject(FrameBrush);
	
	::SelectObject(hDC, def_font);
	font.DeleteObject();
}
//----------------------------------------------------------------------------//
void CJetChart::DrawLegend(HDC hDC)
{	
	if( m_LegendVisible == false ) { return; }

	RECT Rect = m_LegendRect;
	
	COLORREF color = 0xddffdd;
	::SetBkMode(hDC, TRANSPARENT);
	HBRUSH Brush = ::CreateSolidBrush(color);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hDC, Brush);
	::FillRect(hDC, &Rect, Brush);
	::SelectObject(hDC, OldBrush);
	::DeleteObject(Brush);
 
	color = 0x555555;
	HPEN Pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN OldPen = (HPEN)::SelectObject(hDC, Pen);
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
	::SelectObject(hDC, OldPen);
	::DeleteObject(Pen);

	int index = 0;
	DrawLegendLine(hDC, m_pSeries1, index);
	DrawLegendLine(hDC, m_pSeries2, index);
	DrawLegendLine(hDC, m_pSeries3, index);
	DrawLegendLine(hDC, m_pSeries4, index);
	DrawLegendLine(hDC, m_pSeries5, index);
	DrawLegendLine(hDC, m_pSeries6, index);
	DrawLegendLine(hDC, m_pSeries7, index);
	DrawLegendLine(hDC, m_pSeries8, index);
	DrawLegendLine(hDC, m_pSeries9, index);
	DrawLegendLine(hDC, m_pSeries10, index);
	DrawLegendLine(hDC, m_pSeries11, index);
	DrawLegendLine(hDC, m_pSeries12, index);	
}
//----------------------------------------------------------------------------//
bool CJetChart::BuildChart()
{	
	//清除
	::SetBkMode(this->m_DrawDC, TRANSPARENT);
	HBRUSH Brush = ::CreateSolidBrush(m_BKColor);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(this->m_DrawDC, Brush);
	::FillRect(this->m_DrawDC, &this->m_DrawRect, Brush);
	::SelectObject(this->m_DrawDC, OldBrush);
	::DeleteObject(Brush);

	if( GetIsSeriesExist() == false ) { return true; }

	PreSetChartInfo();

	if( m_ChartType == CHART_TYPE_PIE )
	{
		if( m_MaxYValue > 0 )
		{
			DrawPieSeries(m_DrawDC, m_pSeries1);
			DrawPieLegend(m_DrawDC, m_pSeries1);		
		}
		DrawTitle(m_DrawDC);
		//DrawInfo(m_DrawDC);	//chia 1050517
	}
	else if( m_ChartType == CHART_TYPE_RADAR )
	{
		if( m_RadarRadus > 0 )
		{
			DrawRadarGridLine(m_DrawDC);
			DrawRadarSeries(m_DrawDC, m_pSeries1);
			DrawRadarFrame(m_DrawDC);		
		}
		DrawTitle(m_DrawDC);
		DrawInfo(m_DrawDC);	//chia 1050517
	}
	else if( m_ChartType == CHART_TYPE_BAR )//fang 1020624
	{
		if( m_MaxYValue > 0 )
		{
			DrawBarSeries(m_DrawDC, m_pSeries1);
		}
		DrawTitle(m_DrawDC);
		DrawInfo(m_DrawDC);	//chia 1050517
	}
	else
	{		
		DrawGridLine(m_DrawDC);		
		DrawSeries(m_DrawDC, m_pSeries1);
		DrawSeries(m_DrawDC, m_pSeries2);
		DrawSeries(m_DrawDC, m_pSeries3);
		DrawSeries(m_DrawDC, m_pSeries4);
		DrawSeries(m_DrawDC, m_pSeries5);
		DrawSeries(m_DrawDC, m_pSeries6);
		DrawSeries(m_DrawDC, m_pSeries7);
		DrawSeries(m_DrawDC, m_pSeries8);
		DrawSeries(m_DrawDC, m_pSeries9);
		DrawSeries(m_DrawDC, m_pSeries10);
		DrawSeries(m_DrawDC, m_pSeries11);
		DrawSeries(m_DrawDC, m_pSeries12);
		DrawFrame(m_DrawDC);
		DrawTitle(m_DrawDC);
		DrawLegend(m_DrawDC);
		DrawCurrentLine(m_DrawDC);
		DrawInfo(m_DrawDC);	//chia 1050517		
	}
	return true;
}
//----------------------------------------------------------------------------//
void CJetChart::ReDraw(HDC hDC, RECT *Rect)
{
	if( hDC != NULL )
	{ this->SetDrawHdc(hDC, *Rect); }
	BuildChart();
}
//----------------------------------------------------------------------------//
bool CJetChart::GetSeriesLimitRadarValue(CJetSeries *pSeries, float &MinXValue, float &MaxXValue, float &MinYValue, float &MaxYValue )
{
	if( pSeries == NULL ) { return false; }
	if(this->m_ChartType != CHART_TYPE_RADAR ) { return false; }

	const int size = pSeries->GetNSeriesNode();
	if( size <= 0 ) { return false; }

	float TempMinXValue = 999999999.0f;
	float TempMaxXValue = -999999999.0f;
	float TempMinYValue = 999999999.0f;
	float TempMaxYValue = -999999999.0f;

	int i=0;
	int Index = 0;
	float ValueX = 0.0f;
	float ValueY = 0.0f;
	float SeiresValueSum  = 0.0f;
	SeriesNode_S *pNode = NULL;
	for( i=0 ; i<size ; i++ )
	{
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		ValueX = pNode->m_XValue;
		ValueY = pNode->m_YValue;

		if( pNode->m_IsVisible == false ) { continue; }

		if( TempMinXValue > ValueX ) { TempMinXValue = ValueX; }
		if( TempMaxXValue < ValueX ) { TempMaxXValue = ValueX; }
		
		if( TempMinYValue > ValueY ) { TempMinYValue = ValueY; }
		if( TempMaxYValue < ValueY ) { TempMaxYValue = ValueY; }

	}
	
	MinXValue = TempMinXValue;
	MaxXValue = TempMaxXValue;
	
	MinYValue = TempMinYValue;
	MaxYValue = TempMaxYValue;

	return true;
}
//----------------------------------------------------------------------------//
bool CJetChart::GetSeriesLimitValue(CJetSeries *pSeries, int &MinIndex, int &MaxIndex, float &MinValue, float &MaxValue )
{
	if( pSeries == NULL ) { return false; }

	const int size = pSeries->GetNSeriesNode();
	if( size <= 0 ) { return false; }

	int TempMinIndex = 999999999;
	int TempMaxIndex = -999999999;
	float TempMinValue = 999999999.0f;
	float TempMaxValue = -999999999.0f;

	int i=0;
	int Index = 0;
	float Value = 0.0f;
	float SeiresValueSum  = 0.0f;
	SeriesNode_S *pNode = NULL;
	for( i=0 ; i<size ; i++ )
	{
		pNode = pSeries->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		Index = pNode->m_XIndex;
		Value = pNode->m_YValue;

		if( TempMinIndex > Index ) { TempMinIndex = Index; }
		if( TempMaxIndex < Index ) { TempMaxIndex = Index; }

		if( pNode->m_IsVisible == false && this->m_ChartType == CHART_TYPE_LINE ) { continue; }
		if( TempMinValue > Value ) { TempMinValue = Value; }
		if( TempMaxValue < Value ) { TempMaxValue = Value; }
		SeiresValueSum += Value;
	}
	
	m_SeiresValueSum = SeiresValueSum;
	MinIndex = TempMinIndex;
	MaxIndex = TempMaxIndex;
	MinValue = TempMinValue;
	MaxValue = TempMaxValue;

	if( pSeries->GetSeriesType() == SERIES_TYPE_BAR )
	{ MinValue = 0; }

	return true;
}
//----------------------------------------------------------------------------//
void CJetChart::SetPanelID(const int id)
{
	m_Panel = id;
}
//----------------------------------------------------------------------------//
void CJetChart::SetBoardID(const int id)
{
	m_Board = id;
}
//----------------------------------------------------------------------------//
void CJetChart::SetComponentName(LPCSTR pName)
{
	m_ComponentName = pName;
}
//----------------------------------------------------------------------------//
void CJetChart::SetComponentName(LPCWSTR  pName)
{
	m_ComponentName = pName;
}
//----------------------------------------------------------------------------//
void CJetChart::SetTitle(LPCSTR pTitle)
{
	this->m_Title = pTitle;
}
//----------------------------------------------------------------------------//
void CJetChart::SetTitle(LPCWSTR pTitle)
{
	this->m_Title = pTitle;
}
//----------------------------------------------------------------------------//
void CJetChart::SetInfoStr(LPCTSTR str1, LPCTSTR str2, LPCTSTR str3, LPCTSTR str4)	//chia 1050517
{
	::memset(m_InfoStr01, 0x00, sizeof(m_InfoStr01));
	::memset(m_InfoStr02, 0x00, sizeof(m_InfoStr02));
	::memset(m_InfoStr03, 0x00, sizeof(m_InfoStr03));
	::memset(m_InfoStr04, 0x00, sizeof(m_InfoStr04));

	if( str1 != NULL )
	{
		if( _tcslen(str1) <= MAX_INFO_STR_LENGTH )
		{ _stprintf(m_InfoStr01, _T("%s"), str1); }
		else
		{ _stprintf(m_InfoStr01, _T("")); }
	}
	
	if( str2 != NULL )
	{
		if( _tcslen(str2) <= MAX_INFO_STR_LENGTH )
		{ _stprintf(m_InfoStr02, _T("%s"), str2); }
		else
		{ _stprintf(m_InfoStr02, _T("")); }
	}
	
	if( str3 != NULL )
	{
		if( _tcslen(str3) <= MAX_INFO_STR_LENGTH )
		{ _stprintf(m_InfoStr03, _T("%s"), str3); }
		else
		{ _stprintf(m_InfoStr03, _T("")); }	
	}
	if( str4 != NULL )
	{
		if( _tcslen(str4) <= MAX_INFO_STR_LENGTH )
		{ _stprintf(m_InfoStr04, _T("%s"), str4); }
		else
		{ _stprintf(m_InfoStr04, _T("")); }	
	}
}
//----------------------------------------------------------------------------//
void CJetChart::SetIsShowLegend(const bool IsVisible)
{
	this->m_LegendVisible = IsVisible;
}
//----------------------------------------------------------------------------//
bool CJetChart::GetIsShowLegend()
{
	return this->m_LegendVisible;
}
//----------------------------------------------------------------------------//
void CJetChart::SetInfoLocationMode(const int Mode)	//Info Mode - Kai-20250320
{	
	switch( Mode )
	{
	case JET_CHART_INFO_LOCATION_MODE_RIGHT:
	case JET_CHART_INFO_LOCATION_MODE_LEFT:
		m_InfoLocationMode = Mode;
		break;	
	}
}
//----------------------------------------------------------------------------//	
void CJetChart::SetLegendLocationMode(const int Mode)	//Legend Mode - Vic 20161108
{
	this->m_LegendLocationMode = Mode;
	switch(this->m_LegendLocationMode)
	{
	case JET_CHART_LENGEND_LOCATION_MODE_BOTTOM:
	case JET_CHART_LENGEND_LOCATION_MODE_RIGHT:
		break;
	default:
		this->m_LegendLocationMode = JET_CHART_LENGEND_LOCATION_MODE_BOTTOM;
		break;
	}
}
//----------------------------------------------------------------------------//
void CJetChart::SetIsShowGrid(const bool IsShow)
{
	this->m_ShowGridLine = IsShow;
}
//----------------------------------------------------------------------------//
void CJetChart::SetIsShowCurrentLine(const bool IsShow)
{
	this->m_DrawCurrentLine = IsShow;
}
//----------------------------------------------------------------------------//
bool CJetChart::GetIsShowCurrentLine()
{
	return this->m_DrawCurrentLine;
}
//----------------------------------------------------------------------------//
void CJetChart::SetXAxisLabelMode(const int mode)
{
	this->m_XAxisLabelMode = mode;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisFix(bool IsFix)
{
	m_IsFixYAxis = IsFix;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisMin(float value)
{
	m_YAxisFixMin = value;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisMax(float value)
{
	m_YAxisFixMax = value;
}
//----------------------------------------------------------------------------//
bool CJetChart::GetYAxisFix()
{
	return this->m_IsFixYAxis;
}
//----------------------------------------------------------------------------//
float CJetChart::GetYAxisMin()
{
	return this->m_YAxisFixMin;
}
//----------------------------------------------------------------------------//
float CJetChart::GetYAxisMax()
{
	return this->m_YAxisFixMax;
}
//----------------------------------------------------------------------------//
void CJetChart::SetZoomRoi(RECT Rect)
{
	if( Rect.right <= Rect.left ) 
	{ 		
		m_IsZoom = false; 
		return; 
	}
	POINT pt = {0,0};

	pt.x = Rect.left;
	pt.y = Rect.top;	
	int XIndexL = this->GetXIndex(pt.x);
	float YValueT = this->GetYAxisValue(pt.y);

	pt.x = Rect.right;
	pt.y = Rect.bottom;
	int XIndexR = this->GetXIndex(pt.x);
	float YValueB = this->GetYAxisValue(pt.y);

	m_ZoomMinXIndex = XIndexL;
	m_ZoomMaxXIndex = XIndexR;
	m_ZoomMinYValue = YValueB;
	m_ZoomMaxYValue = YValueT;
	m_IsZoom = true; 
}
//----------------------------------------------------------------------------//
int CJetChart::GetXIndex(const int PosX)
{
	int Length = PosX-m_SeriesRect.left;
	if( Length < 0 ) { return -1; }
	int AxisCount = m_MaxXIndex-m_MinXIndex;

	double TempF = (double)Length/m_SeriesRatioX;
	int Index = (int)(TempF+0.5);
	if( m_IsZoom == true )
	{ Index += m_ZoomMinXIndex; }
	if( Index > AxisCount ) 
	{ 
		if( m_IsZoom == true )
		{ Index = m_ZoomMaxXIndex; }
		else 
		{ Index = AxisCount; }		
	}

	return Index;
}
//----------------------------------------------------------------------------//
float CJetChart::GetYAxisValue(const int PosY)
{
	int Length = m_SeriesRect.bottom-PosY;
	if( Length < 0 ) { return -1; }

	float Value = (float)Length/m_SeriesRatioY;
	if( m_IsZoom == true )
	{ Value += m_ZoomMinYValue; }
	else
	{ Value += m_MinYValue; }

	return Value;
}
//----------------------------------------------------------------------------//
void CJetChart::SetOffset(const int OffsetX, const int OffsetY)
{
	m_OffsetX = OffsetX;
	m_OffsetY = OffsetY;
}
//----------------------------------------------------------------------------//
Font_ST& CJetChart::GetFrameFont()
{
	return m_FrameFont;
}
//----------------------------------------------------------------------------//
Font_ST& CJetChart::GetTitleFont()
{
	return m_TitleFont;
}
//----------------------------------------------------------------------------//
Font_ST& CJetChart::GetInfoFont()
{
	return m_InfoFont;
}
//----------------------------------------------------------------------------//
void CJetChart::SetBKColor(COLORREF color)
{
	m_BKColor = color;
}
//----------------------------------------------------------------------------//
void CJetChart::SetFrameColor(COLORREF color)	//chia 0921
{
	m_FrameColor = color;
}
//----------------------------------------------------------------------------//
void CJetChart::SetTitleColor(COLORREF color)
{
	m_TitleColor = color;
}
//----------------------------------------------------------------------------//
void CJetChart::SetGridColor(COLORREF color)
{
	m_GridColor = color;
}
//----------------------------------------------------------------------------//
void CJetChart::SetSeriesTopSpace(const int space)
{
	m_SeriesTopSpace		= space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetSeriesBottomSpace(const int space)
{
	m_SeriesBottomSpace		= space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetSeriesLeftSpace(const int space)
{
	m_SeriesLeftSpace		= space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetSeriesRightSpace(const int space)
{
	m_SeriesRightSpace		= space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetChartTopSpace(const int space)
{
	m_ChartTopSpace = space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetChartBottomSpace(const int space)
{
	m_ChartBottomSpace = space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetChartLeftSpace(const int space)
{
	m_ChartLeftSpace = space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetChartRightSpace(const int space)
{
	m_ChartRightSpace = space;
}
//----------------------------------------------------------------------------//
void CJetChart::SetIsIntYValue(const bool IsINT)
{
	m_IsIntYValue = IsINT;
}
//----------------------------------------------------------------------------//
void CJetChart::SetTitleFont(Font_ST font)
{
	this->m_TitleFont = font;
}
//----------------------------------------------------------------------------//
void CJetChart::SetInfoFont(Font_ST font)	//chia 1050517
{
	this->m_InfoFont = font;	
}
//----------------------------------------------------------------------------//
void CJetChart::SetFrameFont(Font_ST font)
{
	this->m_FrameFont = font;	
}
//----------------------------------------------------------------------------//
void CJetChart::SetIsTranspose(const bool Transpose)
{
	this->m_IsTranspose = Transpose;
}
//----------------------------------------------------------------------------//
void CJetChart::SetRadarCenter(float Cx, float Cy)
{
	this->m_RadarCXValue = Cx;
	this->m_RadarCYValue = Cy;
}
//----------------------------------------------------------------------------//
void CJetChart::SetRadarRadius(float radius)
{
	this->m_RadarRadus = radius;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisPercentageVisible(bool bShow)//Show Y-Axis Percentage
{
	m_YAxisPercentageVisible = bShow;
}
//----------------------------------------------------------------------------//
void CJetChart::SetYAxisPercentageTarget(float fValue)//Y-Axis Percentage Target
{
	m_YAxisPercentageTarget = fValue;
}
//----------------------------------------------------------------------------//
void CJetChart::SetXAxisGribLineVisible(bool bShow)//Show X
{
	m_XAxisGribLineVisible = bShow;
}
//----------------------------------------------------------------------------//
void CJetChart::SetXAxisGribLineTarget(std::vector<int> Value)//X-Axis Target
{
	m_XAxisGribLineTarget = Value;
}
//----------------------------------------------------------------------------//