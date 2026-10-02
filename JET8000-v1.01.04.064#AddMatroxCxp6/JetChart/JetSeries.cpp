// JetSeries.cpp: implementation of the CJetSeries class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\stdafx.h"
#include "JetSeries.h"
#include "..\SortObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
CJetSeries::CJetSeries()
{
	PreInitialSeries();
}
//----------------------------------------------------------------------------//
CJetSeries::~CJetSeries()
{
	ReleaseSeries();
}
//----------------------------------------------------------------------------//
void CJetSeries::ReleaseSeries()
{
	m_NodeList.clear();
	std::vector<SeriesNode_S>().swap(m_NodeList);
	
	m_IsVisible = true;	
	m_LegendVisible = true;
	m_SeriesTitle = _T("");
}
//----------------------------------------------------------------------------//
void CJetSeries::PreInitialSeries()
{
	ReleaseSeries();
	m_SeriesType = SERIES_TYPE_LINE;
	this->m_LineColor = 0x000000;
	this->m_DotColor = 0x000000;	
	m_LineWidth = 1;
	m_DotWidth = 0;
	m_IsShowMarkValue = false;
}
//----------------------------------------------------------------------------//
void CJetSeries::InitialSeries()
{
}
//----------------------------------------------------------------------------//
void CJetSeries::InitialSeriesNode(SeriesNode_S *node)
{
	node->m_XLabel = _T("");
	node->m_YLabel = _T("");
	node->m_XIndex = -1;
	node->m_Width = 0;
	node->m_XValue = -1.0f;	
	node->m_YValue = -1.0f;	
	node->m_IsVisible = true;
	node->m_Color = 0x000000;
}
//----------------------------------------------------------------------------//
void CJetSeries::AddXYValue(bool IsVisible, int XIndex, int XValue, float YValue, LPCTSTR XLabel, LPCTSTR YLabel, COLORREF color, int Width )
{
	SeriesNode_S Node;
	InitialSeriesNode(&Node);

	if( XLabel != NULL ) { Node.m_XLabel = XLabel; }
	if( YLabel != NULL ) { Node.m_YLabel = YLabel; }
	Node.m_XIndex = XIndex;
	Node.m_Width = Width;
	Node.m_XValue = (float)(XValue);
	Node.m_YValue = (float)(YValue);
	Node.m_IsVisible = IsVisible;
	Node.m_Color = color;

	m_NodeList.push_back(Node);
}
//----------------------------------------------------------------------------//
void CJetSeries::SetSeriesType(const int type)
{
	this->m_SeriesType = type;
}
//----------------------------------------------------------------------------//
int CJetSeries::GetSeriesType()
{
	return this->m_SeriesType;
}
//----------------------------------------------------------------------------//
const CJetSeries& CJetSeries::operator =( const CJetSeries &Series)
{
	if( this == &Series ) { return *this; }
	CloneSeries(&Series);
	return *this;
}
//----------------------------------------------------------------------------//
void CJetSeries::CloneSeries(const CJetSeries *Series)
{
	this->ReleaseSeries();
	this->m_SeriesType = Series->m_SeriesType;
	this->m_NodeList = Series->m_NodeList;
	this->m_LineColor = Series->m_LineColor;
	this->m_DotColor = Series->m_DotColor;	
	this->m_IsVisible = Series->m_IsVisible;
	this->m_LegendVisible = Series->m_IsVisible;
	this->m_SeriesTitle = Series->m_SeriesTitle;
	this->m_LineWidth = Series->m_LineWidth;
	this->m_DotWidth = Series->m_DotWidth;
	this->m_IsShowMarkValue = Series->m_IsShowMarkValue;
}
//----------------------------------------------------------------------------//
int CJetSeries::GetNSeriesNode()
{
	return (int)m_NodeList.size();
}
//----------------------------------------------------------------------------//
SeriesNode_S *CJetSeries::GetSeriesNode(const int index)
{
	const int size = this->GetNSeriesNode();
	if( index < 0 ) { return NULL; }
	if( index >= size ) { return NULL; }
	return &this->m_NodeList[index];
} 
//----------------------------------------------------------------------------//
void CJetSeries::SetDotColor(COLORREF color)
{
	this->m_DotColor = color;
}
//----------------------------------------------------------------------------//
COLORREF CJetSeries::GetDotColor()
{
	return m_DotColor;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetDotWidth(const int DotWidth)
{
	this->m_DotWidth = DotWidth;
}
//----------------------------------------------------------------------------//
int CJetSeries::GetDotWidth()
{
	return m_DotWidth;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetLineColor(COLORREF color)
{
	this->m_LineColor = color;
}
//----------------------------------------------------------------------------//
COLORREF CJetSeries::GetLineColor()
{
	return this->m_LineColor;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetIsVisible(bool Visible)
{
	m_IsVisible = Visible;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetLegendVisible(bool Visible)
{
	m_LegendVisible = Visible;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetTitle(LPCSTR SeriesTitle)
{
	this->m_SeriesTitle = SeriesTitle;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetTitle(LPCWSTR SeriesTitle)
{
	this->m_SeriesTitle = SeriesTitle;
}
//----------------------------------------------------------------------------//
LPCTSTR CJetSeries::GetTitle()
{
	return m_SeriesTitle;
}
//----------------------------------------------------------------------------//
bool CJetSeries::GetLegendVisible()
{
	return m_LegendVisible;
}
//----------------------------------------------------------------------------//
bool CJetSeries::GetIsVisible()
{
	return m_IsVisible;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetLineWidth(const int width)
{
	m_LineWidth = width;
}
//----------------------------------------------------------------------------//
int CJetSeries::GetLineWidth()
{
	return m_LineWidth;
}
//----------------------------------------------------------------------------//
void CJetSeries::SetIsShowMarkValue(const bool show)
{
	m_IsShowMarkValue = show;
}
//----------------------------------------------------------------------------//
bool CJetSeries::GetIsShowMarkValue()
{
	return m_IsShowMarkValue;
}
//----------------------------------------------------------------------------//
int CJetSeries::GetNVisiblePieItem()
{
	int size = this->GetNSeriesNode();
	if( size <= 0  ) { return 0; }

	int i=0; 
	int NVisible = 0;
	SeriesNode_S *pNode = NULL;
	for( i=0 ; i<size ; i++ )
	{
		pNode = this->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }
		if( pNode->m_YValue <= 0 ) { continue; }
		NVisible++;
	}
	return NVisible;
}
//----------------------------------------------------------------------------//
void CJetSeries::SortSeries()
{
	CJetSeries *pSeries = NULL;
	int size = this->GetNSeriesNode();
	if( size <= 0  ) { return; }

	int i=0;
	CString    SortText;
	CSortObj   SortNode;
	CSortObj  *pSortNode = NULL;	
	std::vector<CSortObj> SortList;
	SortNode.SetSortMode(SORT_BY_DBL);

	SeriesNode_S *TempNodeSet = new SeriesNode_S[size];
	SeriesNode_S *pNode = NULL;	
	for( i=0 ; i<size ; i++ )
	{
		pNode = this->GetSeriesNode(i);
		if( pNode == NULL ) { continue; }

		TempNodeSet[i] = *pNode;
							
		SortNode.SetID(i);
		SortNode.SetValueDbl(pNode->m_YValue);
		SortList.push_back(SortNode);
	}
	
	std::sort(SortList.begin(), SortList.end());
	size = (int)SortList.size();

	m_NodeList.clear();
	std::vector<SeriesNode_S>().swap(m_NodeList);

	int Index = 0;
	for( i=0 ; i<size ; i++ )
	{
		pSortNode = &SortList[i];
		Index = pSortNode->GetID();
		m_NodeList.push_back(TempNodeSet[Index]);			
	}
	delete []TempNodeSet; TempNodeSet=NULL;
}
//----------------------------------------------------------------------------//