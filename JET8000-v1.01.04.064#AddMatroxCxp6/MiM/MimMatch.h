// MimMatch.h: interface for the CMimMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIMMATCH_H__6CDA6064_3AEF_4F17_94D4_6122FC72055D__INCLUDED_)
#define AFX_MIMMATCH_H__6CDA6064_3AEF_4F17_94D4_6122FC72055D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "MimRoiBW8.h"
#include "MimRoiC24.h"
//-------------------------------------------------------------------------------------//
class CJetMatch;
//-------------------------------------------------------------------------------------//
class CMimMatch  
{
	friend CJetMatch;
private:
	//---------------------------------------------------------------------------------//	
	char                       m_ErrorString[128];
	//---------------------------------------------------------------------------------//		
	double                     m_AngleMin;
	double                     m_AngleMax;
	double                     m_ScaleMin;
	double                     m_ScaleMinX;
	double                     m_ScaleMinY;
	double                     m_ScaleMax;	
	double                     m_ScaleMaxX;
	double                     m_ScaleMaxY;
	bool                       m_UseDontCareArea;
	bool                       m_AdvancedLearning;
	//---------------------------------------------------------------------------------//	
	int                        m_RoiStartX;
	int                        m_RoiStartY;
	//---------------------------------------------------------------------------------//	
#ifdef MIM_MATCH_USE	
	LONG_PTR                   m_MatchPtr;//實際物件的指標
	E_iVision_ERRORS           m_MimErrCode;
#endif//MIM_MATCH_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitMatch();
	void                       InitialMatch();
	void                       CloneMatch(const CMimMatch &Match);
	//---------------------------------------------------------------------------------//
	bool                       CheckMimMatch();	
	bool                       CheckMimMatch() const;	
	bool                       CheckIsMimError();		
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CMimMatch();
	CMimMatch(const CMimMatch &Match);
	virtual ~CMimMatch();
	//---------------------------------------------------------------------------------//	
	CMimMatch& operator=(const CMimMatch &Match);
	//---------------------------------------------------------------------------------//	
#ifdef MIM_MATCH_USE
	LONG_PTR*                  GetMatchPtr();
#endif//MIM_MATCH_USE
	//---------------------------------------------------------------------------------//	
	const char*                GetErrorString() const;
	//---------------------------------------------------------------------------------//	
	void                       SetIMatchDefaultParam();
	//---------------------------------------------------------------------------------//	
	void                       SetContrastMode(int Param);
	int                        GetContrastMode();
	//---------------------------------------------------------------------------------//	
	void                       SetCorrelationMode(int Param);
	int                        GetCorrelationMode();
	//---------------------------------------------------------------------------------//	
	void                       SetFilteringMode(int Param);
	int                        GetFilteringMode();
	//---------------------------------------------------------------------------------//	
	void                       SetUseDontCareArea(bool Param);
	bool                       GetUseDontCareArea();
	//---------------------------------------------------------------------------------//	
	void                       SetDontCareThreshold(int Param);
	int                        GetDontCareThreshold();
	//---------------------------------------------------------------------------------//	
	void                       SetMinReducedArea(int Param);
	int                        GetMinReducedArea();
	//---------------------------------------------------------------------------------//	
	void                       SetUseAngle(bool Param);
	bool                       GetUseAngle();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxAngle(float Param);
	float                      GetMaxAngle();
	//---------------------------------------------------------------------------------//	
	void                       SetMinAngle(float Param);	
	float                      GetMinAngle();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxPositions(int Param);
	int                        GetMaxPositions();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxInitialPositions(int Param);
	int                        GetMaxInitialPositions();
	//---------------------------------------------------------------------------------//	
	void                       SetMinScore(float Param);	
	float                      GetMinScore();
	//---------------------------------------------------------------------------------//	
	void                       SetFinalReduction(int Param);
	int                        GetFinalReduction();
	//---------------------------------------------------------------------------------//	
	void                       SetInterpolate(bool Param);
	bool                       GetInterpolate();
	//---------------------------------------------------------------------------------//	
	void                       SetUseScale(bool Param);
	bool                       GetUseScale();
	//---------------------------------------------------------------------------------//	
	void                       SetMinScale(float Param);
	float                      GetMinScale();
	//---------------------------------------------------------------------------------//	
	void                       SetMinScaleX(float Param);
	float                      GetMinScaleX();
	//---------------------------------------------------------------------------------//	
	void                       SetMinScaleY(float Param);
	float                      GetMinScaleY();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxScale(float Param);	
	float                      GetMaxScale();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxScaleX(float Param);	
	float                      GetMaxScaleX();
	//---------------------------------------------------------------------------------//	
	void                       SetMaxScaleY(float Param);	
	float                      GetMaxScaleY();
	//---------------------------------------------------------------------------------//	
	void                       SetRobustness(bool Param);
	bool                       GetRobustness();
	//---------------------------------------------------------------------------------//	
	int                        GetPatternWidth();
	int                        GetPatternHeight();
	//---------------------------------------------------------------------------------//	
	bool                       SetAdvancedLearning(bool Param);
	bool                       GetAdvancedLearning();
	//---------------------------------------------------------------------------------//	
	BOOL                       GetPatternLearnt();
	//---------------------------------------------------------------------------------//	
	bool                       LearnPattern(CMimImageBW8 *Img);
	bool                       LearnPattern(CMimImageC24 *Img);
	bool                       LearnPattern(CMimRoiBW8 *Roi);
	bool                       LearnPattern(CMimRoiC24 *Roi);	
	bool                       LearnPattern(const char *filename, bool IsColor);
	bool                       LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc);
	bool                       LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi);
	//---------------------------------------------------------------------------------//	
	bool                       Match(CMimImageBW8 *Img);
	bool                       Match(CMimImageC24 *Img);
	bool                       Match(CMimRoiBW8 *Roi);
	bool                       Match(CMimRoiC24 *Roi);
	bool                       Match(const char *filename, bool IsColor);
	bool                       Match(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc);
	bool                       MatchRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi);
	//---------------------------------------------------------------------------------//	
	bool                       SaveModel(const char *filename);//儲存Model
	bool                       LoadModel(const char *filename);//儲存Model
	//---------------------------------------------------------------------------------//	
	int                        GetNumPositions();//取得匹配後多少個相似的
	double                     GetResultScore(int index);
	double                     GetResultAngle(int index);
	double                     GetResultPosX(int index);
	double                     GetResultPosY(int index);
	double                     GetResultScaleX(int index);
	double                     GetResultScaleY(int index);
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_MIMMATCH_H__6CDA6064_3AEF_4F17_94D4_6122FC72055D__INCLUDED_)
