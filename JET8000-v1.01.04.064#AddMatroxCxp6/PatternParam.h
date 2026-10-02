// PatternParam.h: interface for the CPatternParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNPARAM_H__59833118_844F_4FF6_A56F_1B06FBAE196C__INCLUDED_)
#define AFX_PATTERNPARAM_H__59833118_844F_4FF6_A56F_1B06FBAE196C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AlgParamDef.h"
#include "AlgBinaryParam.h"
//-------------------------------------------------------------------------------------//
#define MAX_PATTERN_ROI_COUNT 2
//-------------------------------------------------------------------------------------//
typedef struct tagPATTERN_ROI
{ 
	RECT         RoiRect;
	POINT        RoiShift;
	RESULT_ID    ResultID;
	RECT         RoiRectRes;
	double       ResultScore;	
	tagPATTERN_ROI()
	{
		RoiRect.left   = 0;
		RoiRect.top    = 0;
		RoiRect.right  = 0;
		RoiRect.bottom = 0;
		RoiShift.x     = 0;
		RoiShift.y     = 0;
		ResultScore    = 0;
		RoiRectRes.left= 0;
		RoiRectRes.top = 0;
		RoiRectRes.right=0;
		RoiRectRes.bottom=0;
		ResultID       = RESULT_ID_NONE;
	}
} TPATTERN_ROI, *PPATTERN_ROI;
//-------------------------------------------------------------------------------------//
class CPatternParam  
{
private:
	//---------------------------------------------------------------------------------//	
	int                        m_ResultPolarityIdx;	
	//---------------------------------------------------------------------------------//	
	CAlgBinaryParam            m_BinaryParam;
	//---------------------------------------------------------------------------------//	
	int                        m_PatResultW[MAX_PATTERN_ROI_COUNT];//樣板寬度
	int                        m_PatResultH[MAX_PATTERN_ROI_COUNT];//樣板長度
	double                     m_PatResultX[MAX_PATTERN_ROI_COUNT];//樣板位置-X
	double                     m_PatResultY[MAX_PATTERN_ROI_COUNT];//樣板位置-Y
	double                     m_PatResultSkew[MAX_PATTERN_ROI_COUNT];//樣板角度
	double                     m_PatResultScore[MAX_PATTERN_ROI_COUNT];//樣板成績	
	double                     m_PatResultScaleX[MAX_PATTERN_ROI_COUNT];//樣板縮放-X
	double                     m_PatResultScaleY[MAX_PATTERN_ROI_COUNT];//樣板縮放-Y	
	//---------------------------------------------------------------------------------//	
	double                     m_ResultX[MAX_PATTERN_ROI_COUNT];//結果位置-X
	double                     m_ResultY[MAX_PATTERN_ROI_COUNT];//結果位置-Y
	double                     m_ResultSkew[MAX_PATTERN_ROI_COUNT];//偏差角度	
	double                     m_ResultScaleX[MAX_PATTERN_ROI_COUNT];//結果縮放-X
	double                     m_ResultScaleY[MAX_PATTERN_ROI_COUNT];//結果縮放-Y
	double                     m_ResultReading[MAX_PATTERN_ROI_COUNT];//結果讀值
	RESULT_ID                  m_ReultID[MAX_PATTERN_ROI_COUNT];//結果代碼
	//---------------------------------------------------------------------------------//		
	std::wstring               m_PatText;//樣板文字	
	std::vector<std::wstring>  m_PatSimilarTextList;//樣板相似字列表
	std::vector<TPATTERN_ROI>  m_PatRoiListDummy;//空的框列表
	std::vector<TPATTERN_ROI>  m_PatRoiListL[MAX_PATTERN_ROI_COUNT];//包含兩個極性
	std::vector<TPATTERN_ROI>  m_PatRoiListT[MAX_PATTERN_ROI_COUNT];
	std::vector<TPATTERN_ROI>  m_PatRoiListR[MAX_PATTERN_ROI_COUNT];
	std::vector<TPATTERN_ROI>  m_PatRoiListB[MAX_PATTERN_ROI_COUNT];	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitPatternParam();
	void                       InitialPatternParam();
	void                       ClonePatternParam(const CPatternParam &Param);
	//---------------------------------------------------------------------------------//
	bool                       CheckPatternRoiIndex(int index) const;
	//---------------------------------------------------------------------------------//
	bool                       InitPatRoiListInspection(std::vector<TPATTERN_ROI> &List);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CPatternParam();
	CPatternParam(const CPatternParam &Param);
	virtual ~CPatternParam();
	CPatternParam& operator=(const CPatternParam &Param);
	//---------------------------------------------------------------------------------//
	bool                       WritePatternParamFile(CAOIFileIO &FileIO);//儲存樣板參數
	bool                       ReadPatternParamFile(CAOIFileIO &FileIO);//載入樣板參數
	//---------------------------------------------------------------------------------//
	bool                       WritePatternRoiListFile(CAOIFileIO &FileIO, std::vector<TPATTERN_ROI> &RoiRectList);//儲存樣板區域參數
	bool                       ReadPatternRoiListFile(CAOIFileIO &FileIO, std::vector<TPATTERN_ROI> &RoiRectList);//載入樣板區域參數
	//---------------------------------------------------------------------------------//	
	int                        GetPatResultW(int idx) const;
	void                       SetPatResultW(int idx, int val);	
	//---------------------------------------------------------------------------------//
	int                        GetPatResultH(int idx) const;
	void                       SetPatResultH(int idx, int val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultX(int idx) const;
	void                       SetPatResultX(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultY(int idx) const;
	void                       SetPatResultY(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultSkew(int idx) const;
	void                       SetPatResultSkew(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultScore(int idx) const;
	void                       SetPatResultScore(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultScaleX(int idx) const;
	void                       SetPatResultScaleX(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetPatResultScaleY(int idx) const;
	void                       SetPatResultScaleY(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetResultX(int idx) const;
	void                       SetResultX(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetResultY(int idx) const;
	void                       SetResultY(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetResultSkew(int idx) const;
	void                       SetResultSkew(int idx, double val);	
	//---------------------------------------------------------------------------------//	
	double                     GetResultScaleX(int idx) const;
	void                       SetResultScaleX(int idx, double val);	
	//---------------------------------------------------------------------------------//
	double                     GetResultScaleY(int idx) const;
	void                       SetResultScaleY(int idx, double val);	
	//---------------------------------------------------------------------------------//	
	double                     GetResultReading(int idx) const;
	void                       SetResultReading(int idx, double val);	
	//---------------------------------------------------------------------------------//
	int                        GetResultPolarityIdx() const;
	void                       SetResultPolarityIdx(int val);	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetReultID(int idx) const;
	void                       SetReultID(int idx, RESULT_ID val);	
	//---------------------------------------------------------------------------------//
	void                       ResetResultValue();
	void                       CheckResultIndex();
	//---------------------------------------------------------------------------------//
	void                       SetBinaryParam(const CAlgBinaryParam &val)
	{ 
		m_BinaryParam=val; 
		m_BinaryParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_PATTERN_IMAGE);
	}
	CAlgBinaryParam&           GetBinaryParam() { return m_BinaryParam; }	
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetPatText() const;
	void                       SetPatText(const wchar_t *val);		
	//---------------------------------------------------------------------------------//
	bool                       GetPatInputStringMode() const;//樣板輸入字串模式
	//---------------------------------------------------------------------------------//	
	std::vector<std::wstring>& GetPatSimilarTextList();	
	void                       ClearPatSimilarTextList();
	size_t                     GetPatSimilarTextCount() const;
	void                       AddPatSimilarText(const wchar_t *val);	
	const wchar_t*             GetPatSimilarText(size_t idx, bool bCheck) const;
	void                       SetPatSimilarTextList(const std::vector<std::wstring> &val);
	//---------------------------------------------------------------------------------//		
	std::vector<TPATTERN_ROI>& GetPatRoiList(BOX_TOWARD Toward, int idx);
	//---------------------------------------------------------------------------------//
	std::vector<TPATTERN_ROI>& GetPatRoiListL(int idx);
	void                       SetPatRoiListL(const std::vector<TPATTERN_ROI> &val);	
	void                       SetPatRoiListL(int idx, const std::vector<TPATTERN_ROI> &val);	

	std::vector<TPATTERN_ROI>& GetPatRoiListT(int idx);
	void                       SetPatRoiListT(const std::vector<TPATTERN_ROI> &val);	
	void                       SetPatRoiListT(int idx, const std::vector<TPATTERN_ROI> &val);	

	std::vector<TPATTERN_ROI>& GetPatRoiListR(int idx);
	void                       SetPatRoiListR(const std::vector<TPATTERN_ROI> &val);	
	void                       SetPatRoiListR(int idx, const std::vector<TPATTERN_ROI> &val);	

	std::vector<TPATTERN_ROI>& GetPatRoiListB(int idx);
	void                       SetPatRoiListB(const std::vector<TPATTERN_ROI> &val);	
	void                       SetPatRoiListB(int idx, const std::vector<TPATTERN_ROI> &val);	
	//---------------------------------------------------------------------------------//	
	bool                       ModifyPatternSize(BOX_TOWARD Toward, IMAGE_SIZE BeforeW, IMAGE_SIZE BeforeH, unsigned int PatW, unsigned int PatH);
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PATTERNPARAM_H__59833118_844F_4FF6_A56F_1B06FBAE196C__INCLUDED_)
