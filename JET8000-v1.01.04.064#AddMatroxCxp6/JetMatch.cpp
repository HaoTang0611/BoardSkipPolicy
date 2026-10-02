// JetMatch.cpp: implementation of the CJetMatch class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetMatch.h"
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
CString CJetMatch::GetMatchLibTypeText(JET_MATCH_LIB_TYPE Type)
{
	CString str;
	switch ( Type )
	{
	case JET_MATCH_LIB_EVS:	str = _T("Open eVision Match");		break;
	case JET_MATCH_LIB_MIM: str = _T("Mim Library Match");		break;
	default:
		str = _T("Undefined Match Library");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CJetMatch::CJetMatch()
{
	CJetMatch::PreInitMatch();
	CJetMatch::InitialMatch();
}
//-------------------------------------------------------------------------------------//
CJetMatch::CJetMatch(const CJetMatch &Match)
{
	CJetMatch::PreInitMatch();
	CJetMatch::CloneMatch(Match);
}
//-------------------------------------------------------------------------------------//
CJetMatch::~CJetMatch()
{
}
//-------------------------------------------------------------------------------------//
CJetMatch& CJetMatch::operator=(const CJetMatch &Match)
{
	if ( this == &Match ) { return *this; }
	CJetMatch::CloneMatch(Match);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::PreInitMatch()
{	
	m_MatchLibType = JET_MATCH_LIB_NONE;

#ifdef EVISION_MATCH_USE
	m_MatchLibType = JET_MATCH_LIB_EVS;
#endif//EVISION_MATCH_USE

#ifdef MIM_MATCH_USE
	m_MatchLibType = JET_MATCH_LIB_MIM;
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
void CJetMatch::InitialMatch()
{
	m_ErrorString = _T("");
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.InitialMatch();
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.InitialMatch();
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CJetMatch::CloneMatch(const CJetMatch &Match)
{
	m_ErrorString = Match.m_ErrorString;
	m_MatchLibType = Match.m_MatchLibType;
#ifdef EVISION_MATCH_USE
	m_EvsMatch = Match.m_EvsMatch;
#endif//EVISION_MATCH_USE

#ifdef MIM_MATCH_USE
	m_MimMatch = Match.m_MimMatch;
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
LPCTSTR CJetMatch::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
JET_MATCH_LIB_TYPE CJetMatch::GetMatchLibType() const
{
	return m_MatchLibType;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::SetMatchLibType(JET_MATCH_LIB_TYPE val)
{
	switch ( val )
	{
	case JET_MATCH_LIB_EVS:
	#ifdef EVISION_MATCH_USE
		m_MatchLibType = val;
	#endif//EVISION_MATCH_USE
		break;
	case JET_MATCH_LIB_MIM:
	#ifdef MIM_MATCH_USE
		m_MatchLibType = val;
	#endif//MIM_MATCH_USE
		break;
	}
	if ( m_MatchLibType != val ) 
	{
		this->m_ErrorString.Format(_T("Error, Not Support %s"), CJetMatch::GetMatchLibTypeText(val) );
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMatchDefaultParam()
{	
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetEMatchDefaultParam();
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetIMatchDefaultParam();		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetContrastMode(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetContrastMode(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetContrastMode(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetContrastMode()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetContrastMode();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetContrastMode();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetCorrelationMode(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetCorrelationMode(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetCorrelationMode(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetCorrelationMode()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetCorrelationMode();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetCorrelationMode();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetFilteringMode(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetFilteringMode(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetFilteringMode(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetFilteringMode()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetFilteringMode();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetFilteringMode();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetUseDontCareArea(bool Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetUseDontCareArea(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetUseDontCareArea(Param);
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetUseDontCareArea()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:		
		Value = m_EvsMatch.GetUseDontCareArea();		
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetUseDontCareArea();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetDontCareThreshold(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetDontCareThreshold(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetDontCareThreshold(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetDontCareThreshold()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetDontCareThreshold();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetDontCareThreshold();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinReducedArea(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinReducedArea(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinReducedArea(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetMinReducedArea()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinReducedArea();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinReducedArea();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetUseAngle(bool Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetUseAngle(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetUseAngle(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetUseAngle()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:	
		Value = m_EvsMatch.GetUseAngle();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetUseAngle();
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxAngle(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxAngle(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxAngle(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMaxAngle()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxAngle();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxAngle();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinAngle(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinAngle(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinAngle(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMinAngle()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinAngle();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinAngle();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxPositions(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxPositions(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxPositions(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetMaxPositions()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxPositions();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxPositions();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxInitialPositions(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxInitialPositions(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxInitialPositions(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetMaxInitialPositions()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxInitialPositions();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxInitialPositions();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinScore(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinScore(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinScore(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMinScore()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinScore();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinScore();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetFinalReduction(int Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetFinalReduction(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetFinalReduction(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetFinalReduction()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetFinalReduction();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetFinalReduction();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetInterpolate(bool Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetInterpolate(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetInterpolate(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetInterpolate()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetInterpolate();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetInterpolate();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetUseScale(bool Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetUseScale(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetUseScale(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetUseScale()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetUseScale();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetUseScale();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinScale(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinScale(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinScale(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMinScale()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinScale();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinScale();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinScaleX(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinScaleX(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinScaleX(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMinScaleX()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinScaleX();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinScaleX();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMinScaleY(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMinScaleY(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMinScaleY(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMinScaleY()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMinScaleY();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMinScaleY();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxScale(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxScale(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxScale(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMaxScale()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxScale();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxScale();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxScaleX(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxScaleX(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxScaleX(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMaxScaleX()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxScaleX();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxScaleX();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetMaxScaleY(float Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		m_EvsMatch.SetMaxScaleY(Param);		
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetMaxScaleY(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
float CJetMatch::GetMaxScaleY()
{
	float Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetMaxScaleY();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetMaxScaleY();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
void CJetMatch::SetRobustness(bool Param)
{
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:		
		m_EvsMatch.SetRobustness(Param);
		break;
	case JET_MATCH_LIB_MIM:
		m_MimMatch.SetRobustness(Param);		
		break;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetRobustness()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetRobustness();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetRobustness();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetPatternWidth()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetPatternWidth();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetPatternWidth();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetPatternHeight()
{
	int Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetPatternWidth();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetPatternWidth();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::SetAdvancedLearning(bool Param)
{
	bool IsOK=true;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:		
		IsOK=m_EvsMatch.SetAdvancedLearning(Param);
		break;
	case JET_MATCH_LIB_MIM:
		IsOK=m_MimMatch.SetAdvancedLearning(Param);		
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetAdvancedLearning()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetAdvancedLearning();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetAdvancedLearning();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::GetPatternLearnt()
{
	bool Value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		Value = m_EvsMatch.GetPatternLearnt();
		break;
	case JET_MATCH_LIB_MIM:
		Value = m_MimMatch.GetPatternLearnt();		
		break;
	}
	return Value;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::LearnPattern(const char *filename, bool IsColor)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.LearnPattern(filename, IsColor);
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.LearnPattern(filename, IsColor);	
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.LearnPattern(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc);
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.LearnPattern(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc);	
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.LearnPatternRoi(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc, Roi);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.LearnPatternRoi(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc, Roi);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::Match(const char *filename, bool IsColor)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.Match(filename, IsColor);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.Match(filename, IsColor);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::Match(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.Match(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.Match(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::MatchRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.MatchRoi(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc, Roi);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.MatchRoi(ImageW, ImageH, RowPitch, BitCount, pImage, reAlloc, Roi);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::SaveModel(const char *filename)//儲存Model
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.SaveMCH(filename);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.SaveModel(filename);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetMatch::LoadModel(const char *filename)//儲存Model
{
	bool IsOK = false;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		IsOK = m_EvsMatch.LoadMCH(filename);		
		if ( false == IsOK ) 
		{	m_ErrorString = m_EvsMatch.GetErrorString(); }
		break;
	case JET_MATCH_LIB_MIM:
		IsOK = m_MimMatch.LoadModel(filename);
		if ( false == IsOK ) 
		{	m_ErrorString = m_MimMatch.GetErrorString(); }
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
int CJetMatch::GetNumPositions()//取得匹配後多少個相似的
{
	int value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetNumPositions();
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetNumPositions();		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultScore(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultScore(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultScore(index);		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultAngle(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultAngle(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultAngle(index);
		value = -1*value;//注意iMatch與eVision的方向不同
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultPosX(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultPosX(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultPosX(index);		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultPosY(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultPosY(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultPosY(index);		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultScaleX(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultScaleX(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultScaleX(index);		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	
double CJetMatch::GetResultScaleY(int index)
{
	double value=0;
	switch ( m_MatchLibType )
	{
	case JET_MATCH_LIB_EVS:
		value = m_EvsMatch.GetResultScaleY(index);
		break;
	case JET_MATCH_LIB_MIM:
		value = m_MimMatch.GetResultScaleY(index);		
		break;
	}
	return value;
}
//---------------------------------------------------------------------------------//	