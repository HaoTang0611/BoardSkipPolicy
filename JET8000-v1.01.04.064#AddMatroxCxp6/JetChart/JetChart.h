// JetChart.h: interface for the CJetChart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETCHART_H__EFC7D09A_6AAC_4BB7_BD45_858FF488B915__INCLUDED_)
#define AFX_JETCHART_H__EFC7D09A_6AAC_4BB7_BD45_858FF488B915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "JetSeries.h"
//Constant---------------------------------------------------
#define PI		3.1415926535897932384626433832795//3.141592
//#define PI2		2.0*(double)PI
#define RAD2DEG		180.0/PI
#define DEG2RAD		PI/180.0
//#define CAD2UM		1000.0
//#define UM2CAD		0.001
//---------------------------------------------------
#define JET_CHART_BK_COLOR			0xddffff
#define JET_CHART_FRAME_COLOR		0X000000
#define JET_CHART_TITLE_COLOR		0xcccccc
#define JET_CHART_TITLE_FONT_COLOR	0xff0000
#define JET_CHART_GRID_COLOR		0xcccccc
#define JET_CHART_INFO_FONT_COLOR	0x0000ff	//chia 1050517

#define JET_CHART_LABEL_MODE_NONE	0
#define JET_CHART_LABEL_MODE_LABEL	1

#define CHART_TYPE_LINE				0
#define CHART_TYPE_PIE				1
#define CHART_TYPE_RADAR			2
#define CHART_TYPE_BAR				3

#define MAX_INDEX_LINE				10

#define MAX_INFO_STR_LENGTH			32	//chia 1050517

#define JET_CHART_INFO_LOCATION_MODE_RIGHT	    0	//Info Mode - Kai-20250320
#define JET_CHART_INFO_LOCATION_MODE_LEFT	    1

#define JET_CHART_LENGEND_LOCATION_MODE_BOTTOM	0	//Legend Mode - Vic 20161108
#define JET_CHART_LENGEND_LOCATION_MODE_RIGHT	1


typedef struct ST_Font
{
	int	 sFontSize;
	bool sIsBold;
	bool sIsItalic;
	bool sIsUnderline;
	UINT sEscapement;	//идл╫ 0~3600
	COLORREF sFontColor;
	TCHAR sFontType[32];
}Font_ST, *PFont_ST; 

typedef struct ST_INDEX_LINE
{
	int	 sIndex;
	bool sEnabled;
	COLORREF sFontColor;
	COLORREF sLineColor;
	char sText[64];
}IndexLine_ST, *PIndexLine_ST; 

class CJetChart  
{
public:
	CJetChart();
	virtual ~CJetChart();

	void CopyChartData(const CJetChart *chart);
	bool SaveChart(LPCTSTR lpFileName);

protected:
	HDC m_DrawDC;
	HBITMAP m_DrawBitmap;
	int m_ChartType;
	CJetSeries *m_pSeries1;
	CJetSeries *m_pSeries2;
	CJetSeries *m_pSeries3;
	CJetSeries *m_pSeries4;
	CJetSeries *m_pSeries5;
	CJetSeries *m_pSeries6;
	CJetSeries *m_pSeries7;
	CJetSeries *m_pSeries8;
	CJetSeries *m_pSeries9;
	CJetSeries *m_pSeries10;
	CJetSeries *m_pSeries11;
	CJetSeries *m_pSeries12;

	IndexLine_ST m_IndexLineList[MAX_INDEX_LINE];


	COLORREF m_BKColor;
	COLORREF m_FrameColor;	//chia 0921
	COLORREF m_TitleColor;
	COLORREF m_GridColor;	
	COLORREF m_PieColorList[20];

	RECT	m_DrawRect;
	RECT	m_ChartRect;
	RECT	m_SeriesRect;
	RECT	m_LegendRect;

	float   m_RadarCXValue;
	float   m_RadarCYValue;
	float	m_RadarRadus;

	int		m_MinXIndex;
	int		m_MaxXIndex;
	float	m_MinYValue;
	float	m_MaxYValue;

	int		m_OffsetX;
	int		m_OffsetY;

	float	m_SeriesRatioX;
	float	m_SeriesRatioY;

	int m_ChartTopSpace;
	int m_ChartBottomSpace;
	int m_ChartLeftSpace;
	int m_ChartRightSpace;

	int m_SeriesTopSpace;
	int m_SeriesBottomSpace;
	int m_SeriesLeftSpace;
	int m_SeriesRightSpace;
	
	int m_PieLegendItemBlockW;
	int m_PieLegendItemLength;
	int m_PieLegendItemHeight;
	int m_PieLegendNLine;
	int m_PieLegendNItemPerLine;

	int m_LegendSpace;
	int m_ChartW;
	int m_ChartH;

	bool  m_IsFixYAxis;
	float m_YAxisFixMin;
	float m_YAxisFixMax;

	float m_SeiresValueSum;


	int m_Panel;
	int m_Board;
	CString m_ComponentName;	
	CString m_Title;
	CString m_XAxisUnit;
	CString m_YAxisUnit;
	TCHAR m_InfoStr01[MAX_INFO_STR_LENGTH];	//chia 1050517
	TCHAR m_InfoStr02[MAX_INFO_STR_LENGTH];	//chia 1050517
	TCHAR m_InfoStr03[MAX_INFO_STR_LENGTH];	//chia 1050517
	TCHAR m_InfoStr04[MAX_INFO_STR_LENGTH];
	bool m_LegendVisible;
	bool m_DrawCurrentLine;
	bool m_ShowGridLine;

	int m_XAxisLabelMode;

	int m_ZoomMinXIndex ;
	int m_ZoomMaxXIndex ;
	float m_ZoomMinYValue ;
	float m_ZoomMaxYValue ;
	bool m_IsZoom;
	int m_3DThickness;
	bool m_IsIntYValue;

	Font_ST m_FrameFont;
	Font_ST m_TitleFont;
	Font_ST m_InfoFont;	//chia 1050517

	bool m_IsTranspose;

	int m_InfoLocationMode;//Info Mode - Kai-20250320
	int m_LegendLocationMode;//Legend Mode - Vic 20161108

	bool m_YAxisPercentageVisible;//Show Y-Axis Percentage - Kai-20220805
	float m_YAxisPercentageTarget;//Y-Axis Percentage Target - Kai-20220805

	bool m_XAxisGribLineVisible;//Show X-Axis
	std::vector<int> m_XAxisGribLineTarget;//Show X-Axis pos
protected:
	void PreInitialChart();
	void ReleaseChart();
	void ReleaseAllSeries();
	void ClearIndexLineList();
	void CloneChart(const CJetChart *chart);
	void AddSeries(CJetSeries *pSrcSeries, CJetSeries *&pDestSeries);
	bool GetSeriesLimitValue(CJetSeries *pSeries, int &MinIndex, int &MaxIndex, float &MinValue, float &MaxValue );
	bool GetSeriesLimitRadarValue(CJetSeries *pSeries, float &MinXValue, float &MaxXValue, float &MinYValue, float &MaxYValue );
	void  GetRadarRadius(float &Radius);
	void GetAllDataLimit(int &MinXIndex, int &MaxXIndex, float &MinYValue, float &MaxYValue);	
	bool CheckDrawTitle() const;
	bool GetIsSeriesExist() const;
	int  GetNVisibleSeries() const;	

	void GetChartRect(RECT *ChartRect, RECT *LegendRect);
	void GetSeriesRect(RECT *SeriesRect);
	void GetSeriesRatio(float &XRatio, float &YRatio);
	void PreSetChartInfo();
	void DrawSeries(HDC hDC, CJetSeries *pSeries);
	void DrawRadarSeries(HDC hDC, CJetSeries *pSeries);
	void DrawPieSeries(HDC hDC, CJetSeries *pSeries);
	void DrawBarSeries(HDC hDC, CJetSeries *pSeries);//fang 1020624
	void DrawSeries_Transpose(HDC hDC, CJetSeries *pSeries);
	
	void DrawGridLine_Transpos(HDC hDC);
	void DrawFrame_Transpos(HDC hDC);
	void DrawGridLine(HDC hDC);
	void DrawRadarGridLine(HDC hDC);
	void DrawRadarFrame(HDC hDC);
	void DrawFrame(HDC hDC);
	void DrawTitle(HDC hDC);
	void DrawInfo(HDC hDC);	//chia 1050517		//Draw InfoStr
	void DrawLegend(HDC hDC);
	void DrawPieLegend(HDC hDC, CJetSeries *pSeries);
	void DrawCurrentLine(HDC hDC);
	void DrawLegendLine(HDC hDC, CJetSeries *pSeries, int &index);
	int  GetLegendWidth();
	void SortSeries();
	void GetPieLinePos(float value, POINT *pt, int &PieAspect);
	void GetPieLinePos(RECT FullRect, RECT Rect, POINT *pt, int &PieAspect);
	void GetPieTextPos(HDC hDC, const int PieAspect, POINT *pt);
public:
	Font_ST& GetFrameFont();
	Font_ST& GetTitleFont();
	Font_ST& GetInfoFont();
	void SetBKColor(COLORREF color);
	void SetFrameColor(COLORREF color);	//chia 0921
	void SetTitleColor(COLORREF color);
	void SetGridColor(COLORREF color);
	const CJetChart& operator =( const CJetChart &chart);
	void SetChartType(const int type);
	void Set3DThicness(const int Thickness);
	int GetChartType();
	void AddSeries(CJetSeries *pSeries);
	void AddIndexLine(int index, const char*text, COLORREF FontColor, COLORREF LineColor);	//chia 1031002
	void AddIndexLine(int index, COLORREF FontColor, COLORREF LineColor);	//chia 1031002
	void Clear();
	void SetDrawHdc(HDC hdc, RECT Rect);
	bool BuildChart();
	HDC GetBKDC();
	HBITMAP GetBKBitmap();

	void SetPanelID(const int id);
	void SetBoardID(const int id);
	void SetComponentName(LPCSTR pName);
	void SetComponentName(LPCWSTR pName);
	void SetTitle(LPCSTR pTitle);
	void SetTitle(LPCWSTR pTitle);
	void SetInfoStr(LPCTSTR str1, LPCTSTR str2, LPCTSTR str3, LPCTSTR str4);	//chia 1050517

	void SetIsShowGrid(const bool IsShow);
	void SetIsShowCurrentLine(const bool IsShow);
	void SetIsShowLegend(const bool IsVisible);
	bool GetIsShowCurrentLine();
	bool GetIsShowLegend();
	void SetInfoLocationMode(const int Mode);	//Info Mode - Kai-20250320
	void SetLegendLocationMode(const int Mode);	//Legend Mode - Vic 20161108

	void SetXAxisLabelMode(const int mode);
	
	void ReDraw(HDC hDC, RECT *Rect);

	void SetXAxisUnit(LPCSTR unit);
	void SetXAxisUnit(LPCWSTR  unit);
	void SetYAxisUnit(LPCSTR unit);
	void SetYAxisUnit(LPCWSTR unit);
	LPCTSTR  GetXAxisUnit();
	LPCTSTR  GetYAxisUnit();
	int GetXIndex(const int PosX);
	float GetYAxisValue(const int PosY);

	void SetYAxisFix(bool IsFix);
	void SetYAxisMin(float value);
	void SetYAxisMax(float value);
	
	bool GetYAxisFix();
	float GetYAxisMin();
	float GetYAxisMax();

	void SetZoomRoi(RECT Rect);
	void SetOffset(const int OffsetX, const int OffsetY);

	
	void SetSeriesTopSpace(const int space);
	void SetSeriesBottomSpace(const int space);
	void SetSeriesLeftSpace(const int space);
	void SetSeriesRightSpace(const int space);
	
	void SetChartTopSpace(const int space);
	void SetChartBottomSpace(const int space);
	void SetChartLeftSpace(const int space);
	void SetChartRightSpace(const int space);

	void SetIsIntYValue(const bool IsINT);
	
	void SetInfoFont(Font_ST font);	//chia 1050517
	void SetTitleFont(Font_ST font);
	void SetFrameFont(Font_ST font);

	void SetIsTranspose(const bool Transpose);

	void SetRadarCenter(float Cx, float Cy);
	void SetRadarRadius(float radius);

	void SetYAxisPercentageVisible(bool bShow);//Show Y-Axis Percentage
	void SetYAxisPercentageTarget(float fValue);//Y-Axis Percentage Target	

	void SetXAxisGribLineVisible(bool bShow);//Show X-Axis
	void SetXAxisGribLineTarget(std::vector<int> Value);//X-Axis Target	
};

#endif // !defined(AFX_JETCHART_H__EFC7D09A_6AAC_4BB7_BD45_858FF488B915__INCLUDED_)
