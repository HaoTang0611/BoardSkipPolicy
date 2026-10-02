// EvsMatch.h: interface for the CEvsMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSMATCH_H__7682307D_5680_41F1_AA0B_28C0A1E296F7__INCLUDED_)
#define AFX_EVSMATCH_H__7682307D_5680_41F1_AA0B_28C0A1E296F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EvsRoiBW8.h"
#include "EvsRoiC24.h"
//-------------------------------------------------------------------------------------//
class CJetMatch;
//-------------------------------------------------------------------------------------//
class CEvsMatch  
{
	friend CJetMatch;
private:
	//---------------------------------------------------------------------------------//
	int                        m_ErrorCode;
	char                       m_ErrorString[128];
#ifdef EVISION_MATCH_USE
	EVS_MATCH_CLS              m_Match;//實際物件的指標
#endif//EVISION_MATCH_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitMatch();
	void                       InitialMatch();
	void                       CloneMatch(const CEvsMatch &Match);
	//---------------------------------------------------------------------------------//
	bool                       GetIsEVisionError();		
	//---------------------------------------------------------------------------------//
public:
	CEvsMatch();
	CEvsMatch(const CEvsMatch &Match);
	virtual ~CEvsMatch();
	//---------------------------------------------------------------------------------//	
	CEvsMatch& operator=(const CEvsMatch &Match);
	//---------------------------------------------------------------------------------//	
#ifdef EVISION_MATCH_USE
	EVS_MATCH_CLS*             GetMatchPtr();
#endif//EVISION_MATCH_USE
	//---------------------------------------------------------------------------------//	
	const char*                GetErrorString() const;	
	//---------------------------------------------------------------------------------//	
	void                       SetEMatchDefaultParam();
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
	void                       SetUseDontCareArea(bool Param) {}
	bool                       GetUseDontCareArea() const { return true; }
	//---------------------------------------------------------------------------------//	
	void                       SetDontCareThreshold(int Param);
	int                        GetDontCareThreshold();
	//---------------------------------------------------------------------------------//	
	void                       SetMinReducedArea(int Param);
	int                        GetMinReducedArea();
	//---------------------------------------------------------------------------------//		
	void                       SetUseAngle(bool Param) { }
	bool                       GetUseAngle() { return true; }
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
	void                       SetUseScale(bool Param) { } 
	bool                       GetUseScale() { return true; }
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
	bool                       LearnPattern(CEvsRoiBW8 *Roi);
	bool                       LearnPattern(CEvsRoiC24 *Roi);
	bool                       LearnPattern(CEvsImageBW8 *Img);
	bool                       LearnPattern(CEvsImageC24 *Img);
	bool                       LearnPattern(const char *filename, bool IsColor);
	bool                       LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc);
	bool                       LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi);
	//---------------------------------------------------------------------------------//	
	bool                       Match(CEvsRoiBW8 *Roi);//匹配影像-BW8
	bool                       Match(CEvsRoiC24 *Roi);//匹配影像-C24
	bool                       Match(CEvsImageBW8 *Img);//匹配影像-BW8
	bool                       Match(CEvsImageC24 *Img);//匹配影像-C24
	bool                       Match(const char *filename, bool IsColor);
	bool                       Match(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc);
	bool                       MatchRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi);
	//---------------------------------------------------------------------------------//	
	bool                       SaveMCH(const char *filename);//儲存MCH
	bool                       LoadMCH(const char *filename);//儲存MCH
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
#endif // !defined(AFX_EVSMATCH_H__7682307D_5680_41F1_AA0B_28C0A1E296F7__INCLUDED_)
