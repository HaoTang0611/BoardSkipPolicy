#if !defined(AFX_ALGPATTERNTEXTWND_H__A340DA3D_D161_483D_9375_3C8F35C37157__INCLUDED_)
#define AFX_ALGPATTERNTEXTWND_H__A340DA3D_D161_483D_9375_3C8F35C37157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgPatternTextWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "PatternParam.h"
//-------------------------------------------------------------------------------------//
typedef struct tagPatRoiInfo
{
	size_t   index;	
	bool     bSet;
	RECT     RoiRect;
	RECT     ShowRect;	
	CString  sText;
	tagPatRoiInfo()
	{
		index = -1;		
		bSet = false;
		sText = _T("");
		::memset(&RoiRect, 0x00, sizeof(RoiRect));
		::memset(&ShowRect, 0x00, sizeof(ShowRect));
	};
} TPatRoiInfo, *PPatRoiInfo;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternTextWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgPatternTextWnd : public CBaseDialog
{
// Construction
public:
	CAlgPatternTextWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgPatternTextWnd)
	enum { IDD = IDD_ALG_PATTERN_TEXT_WND };
	CStatic	m_ImageWnd;	
	CListBox    m_SimilarTextListBox;
	CComboBox	m_PatternAngleCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgPatternTextWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	CPatternParam&             GetPatternParam();
	float                      GetPatternAngle() const;
	void                       SetPatternParam(const CPatternParam &PatParam, BOX_TOWARD Toward, float Angle);
	void                       SetImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//			
	bool                       m_FirstShow;
	bool                       m_Modified;	
	float                      m_PatternAngle;
	BOX_TOWARD                 m_WndToward;	
	CString                    m_PatternText;
	bool                       m_InputStringMode;
	std::vector<CString>       m_SimilarTextList;
	size_t                     m_ActiveRoiIdx;
	CPatternParam              m_PatternParam;
	RECT                       m_RoiRectRgn;
	std::vector<TPatRoiInfo>   m_PatRoiInfoList;
	//---------------------------------------------------------------------------------//		
	IMAGE_PTR                  m_RawPtr;
	IMAGE_PTR                  m_ImagePtr;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BkColor;
	POINT                      m_OffsetPt;
	double                     m_ZoomScale;
	CJetMemDC                  m_ImageMemDC;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       CreateBKImageWnd();
	void                       ReleaseBuffer();
	//---------------------------------------------------------------------------------//		
	void                       BuildPatternAngleList();
	void                       BuildSimilarTextListBox();
	//---------------------------------------------------------------------------------//
	std::vector<TPatRoiInfo>&  GetPatRoiInfoList();	
	//---------------------------------------------------------------------------------//
	size_t                     GetActiveRoiIdx() const;
	void                       SetActiveRoiIdx(size_t idx);	
	//---------------------------------------------------------------------------------//		
	bool                       GetInputStringMode() const;
	//---------------------------------------------------------------------------------//		
	bool                       InputString();//輸入字串
	bool                       InputRoiChar();//輸入Roi字元	
	bool                       BuildPatternText();
	bool                       AskSetPatternText();//詢問設定樣板文字
	bool                       BuildPatRoiListShowRect();
	bool                       InitialPatRoiListSetting();		
	bool                       CheckPatRoiListSetFinish();
	size_t                     GetPatRoiListSetFinishCount();
	bool                       InputPatRoiListTextString();
	bool                       SetPatRoiListText(const POINT &pt);
	bool                       SwapPatRoi(size_t idx1, size_t idx2);	
	bool                       SwapPatRoiList(const std::vector<TPATTERN_ROI> &SrcList, std::vector<TPATTERN_ROI> &DstList);
	//---------------------------------------------------------------------------------//	
	bool                       DetectAIModelOcrText(std::wstring &OcrText);
	bool                       SaveAIModelFile();
	bool                       LoadAIModelFile(TALG_PARAM_AI_MODEL &aiModelParam);
	bool                       BuildAIModelOcrText(const TALG_PARAM_AI_MODEL &aiModelParam, std::wstring &string);
	//---------------------------------------------------------------------------------//
	void                       CloneSimilarListBoxContent();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgPatternTextWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	virtual void OnOK();	
	afx_msg void OnSetTextChk();
	afx_msg void OnShowIndexChk();
	afx_msg void OnSimilarTextAddBtn();
	afx_msg void OnSimilarTextDelBtn();
	afx_msg void OnSimilarTextClearBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGPATTERNTEXTWND_H__A340DA3D_D161_483D_9375_3C8F35C37157__INCLUDED_)
