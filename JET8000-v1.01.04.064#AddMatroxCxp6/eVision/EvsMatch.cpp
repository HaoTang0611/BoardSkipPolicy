// EvsMatch.cpp: implementation of the CEvsMatch class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsMatch.h"
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
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
CEvsMatch::CEvsMatch()
{
	CEvsMatch::PreInitMatch();
	CEvsMatch::InitialMatch();
}
//-------------------------------------------------------------------------------------//
CEvsMatch::CEvsMatch(const CEvsMatch &Match)
{
	CEvsMatch::PreInitMatch();
	CEvsMatch::CloneMatch(Match);
}
//-------------------------------------------------------------------------------------//
CEvsMatch::~CEvsMatch()
{
}
//-------------------------------------------------------------------------------------//
CEvsMatch& CEvsMatch::operator=(const CEvsMatch &Match)
{
	if ( this == &Match ) { return *this; }
	CEvsMatch::CloneMatch(Match);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::PreInitMatch()
{
	m_ErrorCode = 0;
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));	
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::InitialMatch()
{
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::CloneMatch(const CEvsMatch &Match)
{
	this->m_ErrorCode = Match.m_ErrorCode;
	::strcpy(m_ErrorString, Match.m_ErrorString);
#ifdef EVISION_MATCH_USE
	this->m_Match = Match.m_Match;	
#endif//EVISION_MATCH_USE	
}
//-------------------------------------------------------------------------------------//
#ifdef EVISION_MATCH_USE
EVS_MATCH_CLS* CEvsMatch::GetMatchPtr()
{
	return &m_Match;	
}
#endif//EVISION_MATCH_USE
//-------------------------------------------------------------------------------------//
const char* CEvsMatch::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::GetIsEVisionError()
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		if (EGetError( ) != E_OK)
		{
			sprintf(this->m_ErrorString, "%s", EGetErrorText());
			EOk();
			return true;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		if ( this->m_ErrorCode != EError_Ok )
		{	return true;	}		
	#endif	
	return false;
#endif//EVISION_MATCH_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
void  CEvsMatch::SetEMatchDefaultParam()
{
#ifdef EVISION_MATCH_USE
	this->m_Match.SetPixelDimensions(1.00f, 1.00f);			
	this->m_Match.SetContrastMode(MCH_CONTRAST_NORMAL);
	this->m_Match.SetCorrelationMode(E_MATCH_NORMALIZED);
	//this->m_Match.SetFilteringMode(MCH_LOWPASS);
	this->m_Match.SetFilteringMode(MCH_UNIFORM);			
	this->m_Match.SetDontCareThreshold(0);			
	this->m_Match.SetMinReducedArea(EMATCH_REDUCE_AREA_MIN);					
	this->m_Match.SetMaxAngle(0);
	this->m_Match.SetMinAngle(0);
						
	this->m_Match.SetMaxPositions(1);
	this->m_Match.SetMaxInitialPositions(1);
	this->m_Match.SetMinScore(-1); 
	this->m_Match.SetFinalReduction(0);
	this->m_Match.SetInterpolate(TRUE);
	this->m_Match.SetMinScale(1.00f);
	this->m_Match.SetMaxScale(1.00f);
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetContrastMode(int Param)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetContrastMode((enum MCH_CONTRAST_MODE)Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetContrastMode((EMatchContrastMode)Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetContrastMode()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetContrastMode();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetCorrelationMode(int Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetCorrelationMode((enum E_CORRELATION_MODE)Param); 
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetCorrelationMode((ECorrelationMode)Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetCorrelationMode()
{
#ifdef EVISION_MATCH_USE	
	return this->m_Match.GetCorrelationMode();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetFilteringMode(int Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetFilteringMode((enum MCH_FILTERING_MODE)Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetFilteringMode((EFilteringMode)Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetFilteringMode()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetFilteringMode();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetDontCareThreshold(int Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetDontCareThreshold(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetDontCareThreshold(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetDontCareThreshold()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetDontCareThreshold();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinReducedArea(int Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinReducedArea(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinReducedArea(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetMinReducedArea()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMinReducedArea();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMaxAngle(float Param)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxAngle(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMaxAngle(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMaxAngle()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxAngle();
#endif
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinAngle(float Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinAngle(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinAngle(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//	
float CEvsMatch::GetMinAngle()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMinAngle();	
#endif
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMaxPositions(int Param)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxPositions(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMaxPositions(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetMaxPositions()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxPositions();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMaxInitialPositions(int Param)
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxInitialPositions(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMaxInitialPositions(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}	
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetMaxInitialPositions()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxInitialPositions();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinScore(float Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinScore(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinScore(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}	
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMinScore()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMinScore();		
#endif
	return 0.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetFinalReduction(int Param)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetFinalReduction(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetFinalReduction(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}	
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetFinalReduction()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetFinalReduction();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetInterpolate(bool Param)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetInterpolate(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetInterpolate(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}	
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::GetInterpolate()
{
#ifdef EVISION_MATCH_USE
	if ( FALSE == this->m_Match.GetInterpolate() ) { return false; }
	return true;
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinScale(float Param)
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinScale(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinScale(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMinScale()
{	
#ifdef EVISION_MATCH_USE	
	return this->m_Match.GetMinScale();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinScaleX(float Param)
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinScaleX(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinScaleX(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMinScaleX()
{
#ifdef EVISION_MATCH_USE	
	return this->m_Match.GetMinScaleX();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMinScaleY(float Param)
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMinScaleY(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.SetMinScaleY(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMinScaleY()
{
#ifdef EVISION_MATCH_USE	
	return this->m_Match.GetMinScaleY();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMaxScale(float Param)
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxScale(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.SetMaxScale(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMaxScale()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxScale();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetMaxScaleX(float Param)
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxScaleX(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.SetMaxScaleX(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMaxScaleX()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxScaleX();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//	
void CEvsMatch::SetMaxScaleY(float Param)
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.SetMaxScaleY(Param);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.SetMaxScaleY(Param);
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}
	#endif//EVISION_MODE
#endif
	return ;
}
//-------------------------------------------------------------------------------------//
float CEvsMatch::GetMaxScaleY()
{
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetMaxScaleY();	
#endif
	return 1.0f;
}
//-------------------------------------------------------------------------------------//
void CEvsMatch::SetRobustness(bool Param)
{
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::GetRobustness()
{
	return false;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetPatternWidth()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetPatternWidth();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetPatternHeight()
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetPatternHeight();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::SetAdvancedLearning(bool Param)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_2_5_0_1106
		try
		{	
			this->m_ErrorCode = EError_Ok;						
			this->m_Match.SetAdvancedLearning(Param);			
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());			
			return false;
		}
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::GetAdvancedLearning()
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE_OPEN_EVISION_VERSION>=EVISION_MODE_OPEN_EVISION_2_5_0_1106 	
		return (bool)(m_Match.GetAdvancedLearning());
	#else
		return false;
	#endif//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_2_5_0_1106	 
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
BOOL CEvsMatch::GetPatternLearnt()
{
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		return this->m_Match.IsPatternLearnt();
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		return this->m_Match.GetPatternLearnt();		
	#endif//EVISION_MODE
#endif
	return FALSE;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(CEvsRoiBW8 *Roi)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION		
		this->m_Match.LearnPattern(Roi->GetRoiBW8Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;						
			this->m_Match.LearnPattern(Roi->GetRoiBW8Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(CEvsRoiC24 *Roi)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.LearnPattern(Roi->GetRoiC24Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.LearnPattern(Roi->GetRoiC24Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(CEvsImageBW8 *Img)
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.LearnPattern(Img->GetImageBW8Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.LearnPattern(Img->GetImageBW8Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(CEvsImageC24 *Img)
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.LearnPattern(Img->GetImageC24Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.LearnPattern(Img->GetImageC24Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(const char *filename, bool IsColor)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( false == IsColor )
		{
			CEvsImageBW8 ImageBW8;
			if ( ImageBW8.LoadImage(filename) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			this->m_Match.LearnPattern(ImageBW8.GetImageBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			CEvsImageC24 ImageC24;
			if ( ImageC24.LoadImage(filename) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			this->m_Match.LearnPattern(ImageC24.GetImageC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( false == IsColor )
			{
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.LoadImage(filename) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(ImageBW8.GetImageBW8Ptr());
			}
			else
			{
				CEvsImageC24 ImageC24;
				if ( ImageC24.LoadImage(filename) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(ImageC24.GetImageC24Ptr());
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( 8 == BitCount )
		{
			CEvsImageBW8 ImageBW8;
			if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			this->m_Match.LearnPattern(ImageBW8.GetImageBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else if ( 24 == BitCount )
		{
			CEvsImageC24 ImageC24;
			if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			this->m_Match.LearnPattern(ImageC24.GetImageC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			::sprintf(m_ErrorString, "Error, EvsMatch LearnPattern Exception (BitCount Error:%d)", BitCount);
			return false; 
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( 8 == BitCount )
			{
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(ImageBW8.GetImageBW8Ptr());
			}
			else if ( 24 == BitCount )
			{
				CEvsImageC24 ImageC24;
				if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(ImageC24.GetImageC24Ptr());
			}
			else
			{
				::sprintf(m_ErrorString, "Error, EvsMatch LearnPattern Exception (BitCount Error:%d)", BitCount);
				return false; 
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( 8 == BitCount )
		{
			CEvsRoiBW8   RoiBW8;
			CEvsImageBW8 ImageBW8;			
			if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			if ( RoiBW8.Attach(&ImageBW8) == false ) 
			{	
				::strcpy(m_ErrorString, RoiBW8.GetErrorString());
				return false; 
			}
			RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
			this->m_Match.LearnPattern(RoiBW8.GetRoiBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else if ( 24 == BitCount )
		{
			CEvsRoiC24   RoiC24;
			CEvsImageC24 ImageC24;
			if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			if ( RoiC24.Attach(&ImageC24) == false ) 
			{	
				::strcpy(m_ErrorString, RoiC24.GetErrorString());
				return false; 
			}
			RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
			this->m_Match.LearnPattern(RoiC24.GetRoiC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			::sprintf(m_ErrorString, "Error, EvsMatch LearnPattern Exception (BitCount Error:%d)", BitCount);
			return false; 
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( 8 == BitCount )
			{
				CEvsRoiBW8   RoiBW8;
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				if ( RoiBW8.Attach(&ImageBW8) == false ) 
				{	
					::strcpy(m_ErrorString, RoiBW8.GetErrorString());
					return false; 
				}
				RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(RoiBW8.GetRoiBW8Ptr());
			}
			else if ( 24 == BitCount )
			{
				CEvsRoiC24   RoiC24;
				CEvsImageC24 ImageC24;
				if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				if ( RoiC24.Attach(&ImageC24) == false ) 
				{	
					::strcpy(m_ErrorString, RoiC24.GetErrorString());
					return false; 
				}
				RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.LearnPattern(RoiC24.GetRoiC24Ptr());
			}
			else
			{
				::sprintf(m_ErrorString, "Error, EvsMatch LearnPattern Exception (BitCount Error:%d)", BitCount);
				return false; 
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(CEvsRoiBW8 *Roi)//匹配影像-BW8
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Match(Roi->GetRoiBW8Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;

	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.Match(Roi->GetRoiBW8Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(CEvsRoiC24 *Roi)//匹配影像-C24
{	
#ifdef EVISION_MATCH_USE		
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Match(Roi->GetRoiC24Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;

	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.Match(Roi->GetRoiC24Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}	
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(CEvsImageBW8 *Img)//匹配影像-BW8
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Match(Img->GetImageBW8Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.Match(Img->GetImageBW8Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(CEvsImageC24 *Img)//匹配影像-C24
{	
#ifdef EVISION_MATCH_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Match(Img->GetImageC24Ptr());
		if ( this->GetIsEVisionError() == true )
		{	return false; }

		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Match.Match(Img->GetImageC24Ptr());
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(const char *filename, bool IsColor)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( false == IsColor )
		{
			CEvsImageBW8 ImageBW8;
			if ( ImageBW8.LoadImage(filename) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			this->m_Match.Match(ImageBW8.GetImageBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			CEvsImageC24 ImageC24;
			if ( ImageC24.LoadImage(filename) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			this->m_Match.Match(ImageC24.GetImageC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( false == IsColor )
			{
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.LoadImage(filename) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(ImageBW8.GetImageBW8Ptr());
			}
			else
			{
				CEvsImageC24 ImageC24;
				if ( ImageC24.LoadImage(filename) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(ImageC24.GetImageC24Ptr());
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::Match(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( 8 == BitCount )
		{
			CEvsImageBW8 ImageBW8;
			if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			this->m_Match.Match(ImageBW8.GetImageBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else if ( 24 == BitCount )
		{
			CEvsImageC24 ImageC24;
			if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			this->m_Match.Match(ImageC24.GetImageC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			::sprintf(m_ErrorString, "Error, EvsMatch Match Exception (BitCount Error:%d)", BitCount);
			return false; 
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( 8 == BitCount )
			{
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(ImageBW8.GetImageBW8Ptr());
			}
			else if ( 24 == BitCount )
			{
				CEvsImageC24 ImageC24;
				if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(ImageC24.GetImageC24Ptr());
			}
			else
			{
				::sprintf(m_ErrorString, "Error, EvsMatch Match Exception (BitCount Error:%d)", BitCount);
				return false; 
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::MatchRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi)
{
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		if ( 8 == BitCount )
		{
			CEvsRoiBW8   RoiBW8;
			CEvsImageBW8 ImageBW8;
			if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageBW8.GetErrorString());
				return false; 
			}
			if ( RoiBW8.Attach(&ImageBW8) == false ) 
			{	
				::strcpy(m_ErrorString, RoiBW8.GetErrorString());
				return false; 
			}
			RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);				
			this->m_Match.Match(RoiBW8.GetRoiBW8Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else if ( 24 == BitCount )
		{
			CEvsRoiC24   RoiC24;
			CEvsImageC24 ImageC24;
			if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
			{
				::strcpy(m_ErrorString, ImageC24.GetErrorString());
				return false; 
			}
			if ( RoiC24.Attach(&ImageC24) == false ) 
			{	
				::strcpy(m_ErrorString, RoiC24.GetErrorString());
				return false; 
			}
			RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
			this->m_Match.Match(RoiC24.GetRoiC24Ptr());
			if ( this->GetIsEVisionError() == true )
			{	return false; }
		}
		else
		{
			::sprintf(m_ErrorString, "Error, EvsMatch Match Exception (BitCount Error:%d)", BitCount);
			return false; 
		}		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			if ( 8 == BitCount )
			{
				CEvsRoiBW8   RoiBW8;
				CEvsImageBW8 ImageBW8;
				if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageBW8.GetErrorString());
					return false; 
				}
				if ( RoiBW8.Attach(&ImageBW8) == false ) 
				{	
					::strcpy(m_ErrorString, RoiBW8.GetErrorString());
					return false; 
				}
				RoiBW8.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(RoiBW8.GetRoiBW8Ptr());
			}
			else if ( 24 == BitCount )
			{
				CEvsRoiC24   RoiC24;
				CEvsImageC24 ImageC24;
				if ( ImageC24.SetImagePtr(pImage, ImageW, ImageH, RowPitch, reAlloc) == false )
				{
					::strcpy(m_ErrorString, ImageC24.GetErrorString());
					return false; 
				}
				if ( RoiC24.Attach(&ImageC24) == false ) 
				{	
					::strcpy(m_ErrorString, RoiC24.GetErrorString());
					return false; 
				}
				RoiC24.SetPlacement(Roi.left, Roi.top, Roi.right-Roi.left, Roi.bottom-Roi.top);
				this->m_ErrorCode = EError_Ok;			
				this->m_Match.Match(RoiC24.GetRoiC24Ptr());
			}
			else
			{
				::sprintf(m_ErrorString, "Error, EvsMatch Match Exception (BitCount Error:%d)", BitCount);
				return false; 
			}
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::SaveMCH(const char *filename)//儲存MCH
{	
#ifdef EVISION_MATCH_USE
	::DeleteFileA(filename);
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Save(filename);	
		if( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.Save(filename);	
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsMatch::LoadMCH(const char *filename)//儲存MCH
{	
#ifdef EVISION_MATCH_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Match.Load(filename);
		if( this->GetIsEVisionError() == true ) 
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_Match.Load(filename);	
			return true;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}	
	#endif//EVISION_MODE
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
int CEvsMatch::GetNumPositions()//取得匹配後多少個相似的
{	
#ifdef EVISION_MATCH_USE
	return this->m_Match.GetNumPositions();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultScore(int index)
{
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data =  PosPtr->m_f32Score;		
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data =  PosPtr->Score;		
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultAngle(int index)
{	
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data = PosPtr->m_f32Angle;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data = PosPtr->Angle;		
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultPosX(int index)
{	
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data = PosPtr->m_f32CenterX;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data = PosPtr->CenterX;
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultPosY(int index)
{	
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data = PosPtr->m_f32CenterY;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data = PosPtr->CenterY;
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultScaleX(int index)
{	
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data = PosPtr->m_f32ScaleX;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data = PosPtr->ScaleX;
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
double CEvsMatch::GetResultScaleY(int index)
{	
#ifdef EVISION_MATCH_USE
	double data = 0.0;
	int counts = this->m_Match.GetNumPositions();
	if( index >= counts ) { return data; }
	EVS_MATCH_POS *PosPtr = NULL;

	#if EVISION_MODE == EVISION_MODE_EVISION
		PosPtr = this->m_Match.GetPosition(index);
		data = PosPtr->m_f32ScaleY;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		PosPtr = &(this->m_Match.GetPosition(index));
		data = PosPtr->ScaleY;
	#endif//EVISION_MODE
	return data;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
