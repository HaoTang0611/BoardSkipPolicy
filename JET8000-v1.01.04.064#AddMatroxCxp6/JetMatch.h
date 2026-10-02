// JetMatch.h: interface for the CJetMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETMATCH_H__04145B1C_CBD6_4348_85FB_A184F5C0BFA7__INCLUDED_)
#define AFX_JETMATCH_H__04145B1C_CBD6_4348_85FB_A184F5C0BFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EvsMatch.h"
#include "MimMatch.h"
//-------------------------------------------------------------------------------------//
class CJetMatch  
{
	//---------------------------------------------------------------------------------//
	static CString            GetMatchLibTypeText(JET_MATCH_LIB_TYPE Type);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	CString                    m_ErrorString;
	JET_MATCH_LIB_TYPE         m_MatchLibType;//匹配函式庫樣式
	//---------------------------------------------------------------------------------//	
	CEvsMatch                  m_EvsMatch;//Euresys Match
	CMimMatch                  m_MimMatch;//MiM Match
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitMatch();
	void                       InitialMatch();
	void                       CloneMatch(const CJetMatch &Match);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CJetMatch();
	CJetMatch(const CJetMatch &Match);
	virtual ~CJetMatch();
	//---------------------------------------------------------------------------------//
	CJetMatch& operator=(const CJetMatch &Match);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//
	JET_MATCH_LIB_TYPE         GetMatchLibType() const;
	bool                       SetMatchLibType(JET_MATCH_LIB_TYPE val);	
	//---------------------------------------------------------------------------------//
	void                       SetMatchDefaultParam();
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
	bool                       GetPatternLearnt();
	//---------------------------------------------------------------------------------//
	bool                       LearnPattern(const char *filename, bool IsColor);
	bool                       LearnPattern(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc);
	bool                       LearnPatternRoi(int ImageW, int ImageH, int RowPitch, int BitCount, const unsigned char *pImage, bool reAlloc, const RECT &Roi);
	//---------------------------------------------------------------------------------//	
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
#endif // !defined(AFX_JETMATCH_H__04145B1C_CBD6_4348_85FB_A184F5C0BFA7__INCLUDED_)
