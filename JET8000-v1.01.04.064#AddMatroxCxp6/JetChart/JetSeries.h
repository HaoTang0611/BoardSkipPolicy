// JetSeries.h: interface for the CJetSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETSERIES_H__1FFAB143_B82A_4885_929E_2D15AD246FDB__INCLUDED_)
#define AFX_JETSERIES_H__1FFAB143_B82A_4885_929E_2D15AD246FDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#define SERIES_TYPE_LINE	1
#define SERIES_TYPE_BAR		2
#define SERIES_TYPE_PIE		3
#define SERIES_TYPE_DOT		4


typedef struct ST_SeriesNode
{
	bool  m_IsVisible;
	int	  m_XIndex;
	int   m_Width;
	float m_XValue;
	float m_YValue;	
	CString m_XLabel;
	CString m_YLabel;
	COLORREF m_Color;
}SeriesNode_S, *PSeriesNode_S;

class CJetSeries  
{
public:
	CJetSeries();
	virtual ~CJetSeries();

protected:
	int m_SeriesType;
	COLORREF m_LineColor;
	COLORREF m_DotColor;
	std::vector<SeriesNode_S> m_NodeList;
	
	int  m_LineWidth;
	int  m_DotWidth;
	bool m_IsVisible;
	bool m_IsShowMarkValue;
	
	CString m_SeriesTitle;
	bool m_LegendVisible;

protected:
	void ReleaseSeries();
	void PreInitialSeries();
	void InitialSeries();
	void InitialSeriesNode(SeriesNode_S *node);
	void CloneSeries(const CJetSeries *Series);

public:
	int GetSeriesType();
	void SetSeriesType(const int type);
	void AddXYValue(bool IsVisible, int XIndex, int XValue, float YValue, LPCTSTR XLabel=NULL, LPCTSTR YLabel=NULL, COLORREF color = NULL, int Width=-1);
	int GetNSeriesNode();
	SeriesNode_S *GetSeriesNode(const int index);

	const CJetSeries& operator =( const CJetSeries &Series);

	void SetTitle(LPCSTR SeriesTitle);
	void SetTitle(LPCWSTR  SeriesTitle);
	void SetLegendVisible(bool Visible);
	void SetIsVisible(bool Visible);
	void SetIsShowMarkValue(const bool show);
	
	LPCTSTR GetTitle();
	bool GetLegendVisible();
	bool GetIsVisible();
	bool GetIsShowMarkValue();

	void SetLineColor(COLORREF color);
	COLORREF GetLineColor();
	void SetLineWidth(const int width);
	int  GetLineWidth();

	void SetDotColor(COLORREF color);
	COLORREF GetDotColor();

	void SetDotWidth(const int DotWidth);
	int GetDotWidth();
	int GetNVisiblePieItem();

	void SortSeries();
};

#endif // !defined(AFX_JETSERIES_H__1FFAB143_B82A_4885_929E_2D15AD246FDB__INCLUDED_)
