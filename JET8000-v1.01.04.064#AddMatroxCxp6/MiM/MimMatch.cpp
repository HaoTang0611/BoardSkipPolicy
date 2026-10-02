// MimMatch.cpp: implementation of the CMimMatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimMatch.h"
//-------------------------------------------------------------------------------------//
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
CMimMatch::CMimMatch()
{
	CMimMatch::PreInitMatch();
	CMimMatch::InitialMatch();
}
//-------------------------------------------------------------------------------------//
CMimMatch::CMimMatch(const CMimMatch &Match)
{
	CMimMatch::PreInitMatch();
	CMimMatch::CloneMatch(Match);
}
//-------------------------------------------------------------------------------------//
CMimMatch::~CMimMatch()
{
#ifdef MIM_MATCH_USE
	DestroyNCCMatch(m_MatchPtr);//實際物件的指標
	m_MatchPtr = NULL;	
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
CMimMatch& CMimMatch::operator=(const CMimMatch &Match)
{
	if ( this == &Match ) { return *this; }
	CMimMatch::CloneMatch(Match);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::PreInitMatch()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));	
#ifdef MIM_MATCH_USE
	m_MimErrCode = E__OK;
	m_MatchPtr = CreateNCCMatch();//實際物件的指標
	if ( NULL != m_MatchPtr )
	{
		iGetIsDontArea(m_MatchPtr,&m_UseDontCareArea);
		iGetAngle(m_MatchPtr, &m_AngleMax, &m_AngleMin);
		iGetScale(m_MatchPtr, &m_ScaleMax, &m_ScaleMin);
	}
	else
	{
		m_AngleMax = m_AngleMin = 0.0;
		m_ScaleMax = m_ScaleMin = 1.0;
		m_UseDontCareArea = false;
	}
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
void CMimMatch::InitialMatch()
{
	m_RoiStartX = 0;
	m_RoiStartY = 0;

	m_AngleMin = 0;
	m_AngleMax = 0;
	m_ScaleMin = 1.0;
	m_ScaleMax = 1.0;

	m_ScaleMinX = 1.0;
	m_ScaleMaxX = 1.0;
	m_ScaleMinY = 1.0;
	m_ScaleMaxY = 1.0;
	m_UseDontCareArea = false;
	m_AdvancedLearning = false;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::CloneMatch(const CMimMatch &Match)
{	
	::strcpy(m_ErrorString, Match.m_ErrorString);
	m_AngleMin = Match.m_AngleMin;
	m_AngleMax = Match.m_AngleMax;
	m_ScaleMin = Match.m_ScaleMin;
	m_ScaleMax = Match.m_ScaleMax;
	m_ScaleMinX = Match.m_ScaleMinX;
	m_ScaleMaxX = Match.m_ScaleMaxX;
	m_ScaleMinY = Match.m_ScaleMinY;
	m_ScaleMaxY = Match.m_ScaleMaxY;

	m_RoiStartX = Match.m_RoiStartX;
	m_RoiStartY = Match.m_RoiStartY;
	m_UseDontCareArea = Match.m_UseDontCareArea;
	m_AdvancedLearning = Match.m_AdvancedLearning;
#ifdef MIM_MATCH_USE
	m_MimErrCode = Match.m_MimErrCode;	
	//LONG_PTR                   m_MatchPtr;//實際物件的指標
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
inline bool CMimMatch::CheckMimMatch()
{
#ifdef MIM_MATCH_USE	
	if ( NULL == m_MatchPtr ) 	
	{
		::sprintf(m_ErrorString, "Error, Mim Match Pointer Is NULL");
		return false; 
	}
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CMimMatch::CheckMimMatch() const
{
#ifdef MIM_MATCH_USE	
	if ( NULL == m_MatchPtr ) 	
	{	return false; }
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::CheckIsMimError()
{
#ifdef MIM_LIB_USE
	if ( E__OK != m_MimErrCode ) 
	{
		char *pBuf = iGetErrorText(m_MimErrCode);
		::strcpy(m_ErrorString, pBuf);
		return true;
	}
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
const char* CMimMatch::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetIMatchDefaultParam()
{
	m_AngleMin = 0.0;
	m_AngleMax = 0.0;
	m_ScaleMin = 1.0;
	m_ScaleMinX = 1.0;
	m_ScaleMinY = 1.0;
	m_ScaleMax = 1.0;
	m_ScaleMaxX = 1.0;
	m_ScaleMaxY = 1.0;
	m_RoiStartX = 0;
	m_RoiStartY = 0;
	m_UseDontCareArea = false;
#ifdef MIM_MATCH_USE	
	m_MimErrCode = iSetIsDontArea(m_MatchPtr, false);
	m_MimErrCode = iSetDontCareThreshold(m_MatchPtr, 0);
	m_MimErrCode = iSetMinReduceArea(m_MatchPtr,128);//EMATCH_REDUCE_AREA_MIN	
	
	m_MimErrCode = iSetAngle(m_MatchPtr, 0, 0);	
	m_MimErrCode = iSetIsRotated(m_MatchPtr, false);
	
	m_MimErrCode = iSetOccurrence(m_MatchPtr, 1);
	m_MimErrCode = iSetOutsideFOV(m_MatchPtr, false);		
	
	m_MimErrCode = iSetScale(m_MatchPtr, 1, 1);
	m_MimErrCode = iSetIsScaled(m_MatchPtr, false);			
	
	m_MimErrCode = iSetMinScore(m_MatchPtr, 0);	
	m_MimErrCode = iSetSubPixel(m_MatchPtr, true);	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetContrastMode(int Param)
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE
	return ;
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetContrastMode()
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetCorrelationMode(int Param)
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE
	return ;
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetCorrelationMode()
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetFilteringMode(int Param)
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetFilteringMode()
{
#ifdef MIM_MATCH_USE
#endif//MIM_MATCH_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetUseDontCareArea(bool Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetIsDontArea(m_MatchPtr, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_UseDontCareArea = Param;
#endif//MIM_MATCH_USE
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetUseDontCareArea()
{	
#ifdef MIM_MATCH_USE
	return m_UseDontCareArea;	
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetDontCareThreshold(int Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetDontCareThreshold(m_MatchPtr,Param);
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetDontCareThreshold()
{
	int Param=0;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetDontCareThreshold(m_MatchPtr, &Param);
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinReducedArea(int Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetMinReduceArea(m_MatchPtr,Param);
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetMinReducedArea()
{
	int Param=0;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetMinReduceArea(m_MatchPtr, &Param);
	CMimMatch::CheckIsMimError();	
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetUseAngle(bool Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetIsRotated(m_MatchPtr, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetUseAngle()
{
	bool Rotated=false;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Rotated; }
	m_MimErrCode = iGetIsRotated(m_MatchPtr, &Rotated);
	if ( CMimMatch::CheckIsMimError() == true ) { return false; }	
	return Rotated;
#endif//MIM_MATCH_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxAngle(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetAngle(m_MatchPtr, Param, m_AngleMin);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_AngleMax = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMaxAngle()
{
#ifdef MIM_MATCH_USE
	return (float)(m_AngleMax);
#endif//MIM_MATCH_USE
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinAngle(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetAngle(m_MatchPtr, m_AngleMax, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_AngleMin = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMinAngle()
{
#ifdef MIM_MATCH_USE
	return (float)(m_AngleMin);
#endif//MIM_MATCH_USE
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxPositions(int Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetOccurrence(m_MatchPtr, Param);
	CMimMatch::CheckIsMimError();	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetMaxPositions()
{
	int Param=0;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetOccurrence(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return 1; }
	return Param;
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxInitialPositions(int Param)
{
#ifdef MIM_MATCH_USE
	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetMaxInitialPositions()
{
#ifdef MIM_MATCH_USE
	
#endif//MIM_MATCH_USE
	return 1;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinScore(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetMinScore(m_MatchPtr, Param);
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMinScore()
{
#ifdef MIM_MATCH_USE
	double Param=0.0;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0f; }
	m_MimErrCode = iGetMinScore(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == false ) { return 0.0f; }
	return (float)(Param);
#endif//MIM_MATCH_USE
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetFinalReduction(int Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetFinalReduction(m_MatchPtr, Param);
	CMimMatch::CheckIsMimError();	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetFinalReduction()
{
#ifdef MIM_MATCH_USE	
	int Param=0;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0; }
	m_MimErrCode = iGetFinalReduction(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == false ) { return 0; }
	return (int)(Param);
#endif//MIM_MATCH_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetInterpolate(bool Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetSubPixel(m_MatchPtr, (bool)(Param));
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetInterpolate()
{
	bool Param=false;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetSubPixel(m_MatchPtr, &Param);
	CMimMatch::CheckIsMimError();
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetUseScale(bool Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetIsScaled(m_MatchPtr, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetUseScale()
{
	bool Scaled=false;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Scaled; }
	m_MimErrCode = iGetIsScaled(m_MatchPtr, &Scaled);
	if ( CMimMatch::CheckIsMimError() == true ) { return false; }	
	return Scaled;
#endif//MIM_MATCH_USE	
	return Scaled;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinScale(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, m_ScaleMax, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMin = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMinScale()
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMin);
#endif//MIM_MATCH_USE
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinScaleX(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, m_ScaleMax, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMinX = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMinScaleX()
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMinX);
#endif//MIM_MATCH_USE
	return 1.0f;	
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMinScaleY(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, m_ScaleMax, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMinY = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMinScaleY()	
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMinY);
#endif//MIM_MATCH_USE
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxScale(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, Param, m_ScaleMin);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMax = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMaxScale()
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMax);
#endif//MIM_MATCH_USE
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxScaleX(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, Param, m_ScaleMin);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMaxX = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMaxScaleX()
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMaxX);
#endif//MIM_MATCH_USE
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetMaxScaleY(float Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetScale(m_MatchPtr, Param, m_ScaleMin);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }
	m_ScaleMaxY = Param;
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
float CMimMatch::GetMaxScaleY()
{
#ifdef MIM_MATCH_USE
	return (float)(m_ScaleMaxY);
#endif//MIM_MATCH_USE
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CMimMatch::SetRobustness(bool Param)
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return ; }
	m_MimErrCode = iSetRobustness(m_MatchPtr, Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return; }	
#endif//MIM_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetRobustness()
{
	bool Param=false;
#ifdef MIM_MATCH_USE	
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetRobustness(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return Param; }	
#endif//MIM_MATCH_USE	
	return Param;
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetPatternWidth()
{
	int Param=0;
#ifdef MIM_MATCH_USE	
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetModelWidth(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0; }
	return Param;	
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetPatternHeight()
{
	int Param=0;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetModelHeight(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0; }
	return Param;		
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
BOOL CMimMatch::GetPatternLearnt()
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return FALSE; }
	m_MimErrCode = iIsPatternLearn(m_MatchPtr);
	if ( E_TRUE == m_MimErrCode ) { return TRUE; }	
	return FALSE;
#endif//MIM_MATCH_USE
	return FALSE;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(CMimImageBW8 *Img)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Img ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Img->CheckImageBW8Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Img->GetErrorString());
		return false;
	}
	LONG_PTR ImgPtr = Img->GetImageBW8Ptr();
	m_MimErrCode = CreateNCCModel(ImgPtr,m_MatchPtr, m_UseDontCareArea);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::SetAdvancedLearning(bool Param)
{
	m_AdvancedLearning = Param;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::GetAdvancedLearning()
{
	return false;
	return m_AdvancedLearning;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(CMimImageC24 *Img)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Img ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Img->CheckImageC24Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Img->GetErrorString());
		return false; 
	}
	LONG_PTR ImgPtr = Img->GetImageC24Ptr();
	m_MimErrCode = CreateNCCModel(ImgPtr,m_MatchPtr, m_UseDontCareArea);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(CMimRoiBW8 *Roi)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Roi ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Roi->CheckRoiBW8Ptr() == false )
	{
		::strcpy(this->m_ErrorString, Roi->GetErrorString());
		return false; 
	}	
	mRect rect = Roi->GetRoiRect();
	LONG_PTR ImgPtr = Roi->GetRoiBW8Ptr();
	m_MimErrCode = CreateNCCModelFromROI(ImgPtr, m_MatchPtr, rect, m_UseDontCareArea);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(CMimRoiC24 *Roi)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Roi ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Roi->CheckRoiC24Ptr() == false )
	{
		::strcpy(this->m_ErrorString, Roi->GetErrorString());
		return false; 
	}
	mRect rect = Roi->GetRoiRect();
	LONG_PTR ImgPtr = Roi->GetRoiC24Ptr();	
	m_MimErrCode = CreateNCCModelFromROI(ImgPtr, m_MatchPtr, rect, m_UseDontCareArea);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(const char *filename, bool IsColor)
{
#ifdef MIM_MATCH_USE
	if ( false == IsColor )
	{
		CMimImageBW8 ImageBW8;
		if ( ImageBW8.LoadImage(filename) == false )
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}
		if ( CMimMatch::LearnPattern(&ImageBW8) == false )
		{	return false; }
	}
	else
	{
		CMimImageC24 ImageC24;
		if ( ImageC24.LoadImage(filename) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( CMimMatch::LearnPattern(&ImageC24) == false )
		{	return false; }		
	}
	return true;
	
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{	
#ifdef MIM_MATCH_USE
	if ( 8 == BitCount )
	{
		CMimImageBW8 ImageBW8;		
		if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}		
		if ( CMimMatch::LearnPattern(&ImageBW8) == false )
		{	return false; }
		return true;
	}
	else if ( 24 == BitCount )
	{
		CMimImageC24 ImageC24;
		if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( CMimMatch::LearnPattern(&ImageC24) == false )
		{	return false; }
		return true;
	}
	else
	{
		::sprintf(m_ErrorString, "Error, MimMatch LearnPattern Exception (BitCount Error:%d)", BitCount);			
		return false;
	}
	
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
#ifdef MIM_MATCH_USE
	if ( 8 == BitCount )
	{
		CMimRoiBW8   RoiBW8;
		CMimImageBW8 ImageBW8;		
		if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}
		if ( RoiBW8.Attach(&ImageBW8) == false ) 
		{
			::strcpy(this->m_ErrorString, RoiBW8.GetErrorString());
			return false; 
		}
		RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
		if ( CMimMatch::LearnPattern(&RoiBW8) == false )
		{	return false; }
		return true;
	}
	else if ( 24 == BitCount )
	{
		CMimRoiC24   RoiC24;
		CMimImageC24 ImageC24;
		if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( RoiC24.Attach(&ImageC24) == false ) 
		{
			::strcpy(this->m_ErrorString, RoiC24.GetErrorString());
			return false; 
		}
		RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
		if ( CMimMatch::LearnPattern(&RoiC24) == false )
		{	return false; }
		return true;
	}
	else
	{
		::sprintf(m_ErrorString, "Error, MimMatch LearnPattern Exception (BitCount Error:%d)", BitCount);			
		return false;
	}
	
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(CMimImageBW8 *Img)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Img ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Img->CheckImageBW8Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Img->GetErrorString());
		return false; 
	}
	LONG_PTR ImgPtr = Img->GetImageBW8Ptr();
	m_RoiStartX = m_RoiStartY = 0;
	m_MimErrCode = MatchNCCModel(ImgPtr,m_MatchPtr);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(CMimImageC24 *Img)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Img ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Img->CheckImageC24Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Img->GetErrorString());
		return false; 
	}
	LONG_PTR ImgPtr = Img->GetImageC24Ptr();	
	m_RoiStartX = m_RoiStartY = 0;
	m_MimErrCode = MatchNCCModel(ImgPtr,m_MatchPtr);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(CMimRoiBW8 *Roi)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Roi ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Roi->CheckRoiBW8Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Roi->GetErrorString());
		return false;
	}
	LONG_PTR ImgPtr = Roi->GetRoiBW8Ptr();
	mRect rect = Roi->GetRoiRect();
	m_RoiStartX = rect.left;
	m_RoiStartY = rect.top;
	m_MimErrCode = MatchNCCModelFromROI(ImgPtr, m_MatchPtr, rect);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(CMimRoiC24 *Roi)
{
#ifdef MIM_MATCH_USE
	if ( NULL == Roi ) { return false; }
	if ( CMimMatch::CheckMimMatch() == false ) { return false; }
	if ( Roi->CheckRoiC24Ptr() == false ) 
	{
		::strcpy(this->m_ErrorString, Roi->GetErrorString());
		return false;
	}

	LONG_PTR ImgPtr = Roi->GetRoiC24Ptr();
	mRect rect = Roi->GetRoiRect();
	m_RoiStartX = rect.left;
	m_RoiStartY = rect.top;
	m_MimErrCode = MatchNCCModelFromROI(ImgPtr, m_MatchPtr, rect);
	if ( CMimMatch::CheckIsMimError() == true ) 
	{	return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(const char *filename, bool IsColor)
{
#ifdef MIM_MATCH_USE
	if ( false == IsColor )
	{
		CMimImageBW8 ImageBW8;
		if ( ImageBW8.LoadImage(filename) == false )		
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}
		if ( CMimMatch::Match(&ImageBW8) == false )
		{	return false; }		
	}
	else
	{
		CMimImageC24 ImageC24;
		if ( ImageC24.LoadImage(filename) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( CMimMatch::Match(&ImageC24) == false )
		{	return false; }		
	}
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::Match(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{	
#ifdef MIM_MATCH_USE
	if ( 8 == BitCount )
	{
		CMimImageBW8 ImageBW8;
		if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}
		if ( CMimMatch::Match(&ImageBW8) == false )
		{	return false; }
		return true;
	}
	else if ( 24 == BitCount )
	{
		CMimImageC24 ImageC24;
		if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( CMimMatch::Match(&ImageC24) == false )
		{	return false; }
		return true;
	}
	else
	{
		::sprintf(m_ErrorString, "Error, MimMatch LearnPattern Exception (BitCount Error:%d)", BitCount);			
		return false;
	}
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::MatchRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
#ifdef MIM_MATCH_USE
	if ( 8 == BitCount )
	{
		CMimRoiBW8   RoiBW8;
		CMimImageBW8 ImageBW8;
		if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageBW8.GetErrorString());
			return false; 
		}
		if ( RoiBW8.Attach(&ImageBW8) == false )
		{
			::strcpy(this->m_ErrorString, RoiBW8.GetErrorString());
			return false; 
		}
		RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
		if ( CMimMatch::Match(&RoiBW8) == false )
		{	return false; }
		return true;
	}
	else if ( 24 == BitCount )
	{
		CMimRoiC24   RoiC24;
		CMimImageC24 ImageC24;
		if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
		{
			::strcpy(this->m_ErrorString, ImageC24.GetErrorString());
			return false; 
		}
		if ( RoiC24.Attach(&ImageC24) == false )
		{
			::strcpy(this->m_ErrorString, RoiC24.GetErrorString());
			return false; 
		}
		RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
		if ( CMimMatch::Match(&RoiC24) == false )
		{	return false; }
		return true;
	}
	else
	{
		::sprintf(m_ErrorString, "Error, MimMatch LearnPattern Exception (BitCount Error:%d)", BitCount);			
		return false;
	}
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::SaveModel(const char *filename)//儲存Model
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return FALSE; }
	char file[MAX_JET_PATH]="";
	strcpy(file, filename);
	m_MimErrCode = SaveiMatchModel(m_MatchPtr,file);
	if ( CMimMatch::CheckIsMimError() == true ) { return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimMatch::LoadModel(const char *filename)//儲存Model
{
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return FALSE; }
	char file[MAX_JET_PATH]="";
	strcpy(file, filename);
	m_MimErrCode = LoadiMatchModel(m_MatchPtr, file);
	if ( CMimMatch::CheckIsMimError() == true ) { return false; }
	return true;
#endif//MIM_MATCH_USE
	return false;
}
//-------------------------------------------------------------------------------------//
int CMimMatch::GetNumPositions()//取得匹配後多少個相似的
{
	int Param=0;
#ifdef MIM_MATCH_USE
	if ( CMimMatch::CheckMimMatch() == false )	{	return Param; }
	m_MimErrCode = iGetNCCMatchNum(m_MatchPtr, &Param);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0; }
	return Param;
#endif//MIM_MATCH_USE
	return Param;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultScore(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	return iResult.Score;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultAngle(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	return iResult.Angle;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultPosX(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	double StartX = m_RoiStartX;
	double ResultX = iResult.CX-StartX;
	return ResultX;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultPosY(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	double StartY = m_RoiStartY;
	double ResultY = iResult.CY-StartY;
	return ResultY;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultScaleX(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	return iResult.Scale;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//
double CMimMatch::GetResultScaleY(int index)
{
#ifdef MIM_MATCH_USE
	NCCFind iResult;
	if ( CMimMatch::CheckMimMatch() == false )	{	return 0.0; }
	m_MimErrCode = iGetNCCMatchResults(m_MatchPtr, index, &iResult);
	if ( CMimMatch::CheckIsMimError() == true ) { return 0.0; }
	return iResult.Scale;
#endif//MIM_MATCH_USE
	return 0.0;
}
//-------------------------------------------------------------------------------------//