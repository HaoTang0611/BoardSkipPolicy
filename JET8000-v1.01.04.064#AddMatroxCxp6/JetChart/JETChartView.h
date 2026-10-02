#if !defined(AFX_JETCHARTVIEW_H__D8D6CA1F_72D5_4209_9DF1_61023B762459__INCLUDED_)
#define AFX_JETCHARTVIEW_H__D8D6CA1F_72D5_4209_9DF1_61023B762459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JETChartView.h : header file
//
#include "JetChart.h"

/////////////////////////////////////////////////////////////////////////////
// CJETChartView window

class CJETChartView : public CStatic
{
// Construction
public:
	CJETChartView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJETChartView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJETChartView();

	void CopyChartData(const CJETChartView *chart);
	bool SaveChart(LPCTSTR lpFileName);

	// Generated message map functions
public:
	Font_ST& GetFrameFont();
	Font_ST& GetTitleFont();
	Font_ST& GetInfoFont();
	void SetBKColor(COLORREF color);
	void SetFrameColor(COLORREF color);	//chia 0921
	void SetTitleColor(COLORREF color);
	void SetGridColor(COLORREF color);
	void SetChartType(const int type);
	void Set3DThicness(const int Thickness);
	int GetChartType();
	void AddSeries(CJetSeries *pSeries);
	void AddIndexLine(int index, COLORREF FontColor, COLORREF LineColor);	//chia 1031002
	void AddIndexLine(int index, const char*text, COLORREF FontColor, COLORREF LineColor);	//chia 1031002
	void Clear();
	void BuildChart();
	HDC GetBKDC();
	HBITMAP GetBKBitmap();

	void SetPanelID(const int id);
	void SetBoardID(const int id);
	void SetComponentName(LPCSTR  pName);
	void SetComponentName(LPCWSTR  pName);
	void SetTitle(LPCSTR pTitle);
	void SetTitle(LPCWSTR pTitle);
	void SetInfoStr(LPCTSTR str1=NULL, LPCTSTR str2=NULL, LPCTSTR str3=NULL, LPCTSTR str4=NULL);	//chia 1050517

	void SetIsShowGrid(const bool IsShow);
	void SetIsShowCurrentLine(const bool IsShow);
	void SetIsShowLegend(const bool IsVisible);
	void SetInfoLocationMode(const int Mode);	//Info Mode - Kai-20250320
	void SetLegendLocationMode(const int Mode);	//Legend Mode - Vic 20161108

	bool GetIsShowCurrentLine();
	bool GetIsShowLegend();

	void SetXAxisLabelMode(const int mode);	

	void SetXAxisUnit(LPCSTR unit);
	void SetXAxisUnit(LPCWSTR unit);
	void SetYAxisUnit(LPCSTR unit);
	void SetYAxisUnit(LPCWSTR unit);
	LPCTSTR GetXAxisUnit();
	LPCTSTR GetYAxisUnit();
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

	void SetTitleFont(Font_ST font);
	void SetFrameFont(Font_ST font);
	void SetInfoFont(Font_ST font);	//chia 1050517

	void SetIsTranspose(const bool Transpose);

	void SetRadarCenter(float Cx, float Cy);
	void SetRadarRadius(float radius);

	void SetYAxisPercentageVisible(bool bShow);
	void SetYAxisPercentageTarget(float fValue);//Y-Axis Percentage Target

	void SetXAxisGribLineVisible(bool bShow);//Show X-Axis
	void SetXAxisGribLineTarget(std::vector<int> Value);//X-Axis Target	
protected:
	HDC m_BKChartDC;
	HBITMAP m_BKChartBitmap;
	RECT m_BKRect;
	CJetChart m_chart;

	void PreInitial();
	void ReleaseAll();
	void ReleaseBKDC();
	void CreateBKDC();
	void Drawing();

	//{{AFX_MSG(CJETChartView)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JETCHARTVIEW_H__D8D6CA1F_72D5_4209_9DF1_61023B762459__INCLUDED_)
