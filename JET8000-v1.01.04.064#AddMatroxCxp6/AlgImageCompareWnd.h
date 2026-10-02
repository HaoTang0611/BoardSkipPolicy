#if !defined(AFX_ALGIMAGECOMPAREWND_H__CE4FC40E_4E52_48ED_8258_B5D5534F1D80__INCLUDED_)
#define AFX_ALGIMAGECOMPAREWND_H__CE4FC40E_4E52_48ED_8258_B5D5534F1D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgImageCompareWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageCompareWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgImageCompareWnd : public CBaseDialog
{
// Construction
public:
	CAlgImageCompareWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgImageCompareWnd)
	enum { IDD = IDD_ALG_IMAGE_COMPARE_WND };
	CComboBox	m_ShowModeCombox;
	CComboBox	m_MorphShapeCombox;
	CComboBox	m_MinLogModeCombox;
	CComboBox	m_MaxLogModeCombox;
	CStatic m_ImageWnd;
	CStatic m_PatternWnd;
	CStatic m_ResultWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgImageCompareWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	UINT                       m_ActWndID;
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_WndBKColor;
	RECT                       m_WndRect;
	RECT                       m_ImageWndRect;
	RECT                       m_PatternWndRect;
	RECT                       m_ResultWndRect;	
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;	
	CJetMemDC                  m_PatternWndMemDC;
	CJetMemDC                  m_PatternWndMemDC2;
	CJetMemDC                  m_ResultWndMemDC;
	CJetMemDC                  m_ResultWndMemDC2;
	//---------------------------------------------------------------------------------//	
	bool                       m_Matched;
	double                     m_ColorRatio;
	CString                    m_TempFolder;
	TPOINT2D                   m_PatToImgOffset;
	//---------------------------------------------------------------------------------//	
	CString                    m_ImageFile;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;	
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_ImageBitCount;
	IMAGE_PTR                  m_ImagePtr;
	TPOINT2D                   m_ImageWndPt;
	TPOINT2D                   m_ImageImgPt;
	TPOINT2D                   m_ImageOffset;
	double                     m_ImageZoom;
	//---------------------------------------------------------------------------------//	
	CString                    m_PatternFile;
	IMAGE_SIZE                 m_PatternW;
	IMAGE_SIZE                 m_PatternH;	
	IMAGE_SIZE                 m_PatternStep;
	IMAGE_SIZE                 m_PatternBitCount;
	IMAGE_PTR                  m_PatternPtr;
	TPOINT2D                   m_PatternWndPt;
	TPOINT2D                   m_PatternImgPt;
	TPOINT2D                   m_PatternOffset;
	double                     m_PatternZoom;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ResultW;
	IMAGE_SIZE                 m_ResultH;	
	IMAGE_SIZE                 m_ResultStep;
	IMAGE_SIZE                 m_ResultBitCount;
	IMAGE_PTR                  m_ResultPtr;
	TPOINT2D                   m_ResultWndPt;
	TPOINT2D                   m_ResultImgPt;
	TPOINT2D                   m_ResultOffset;
	double                     m_ResultZoom;
	std::vector<RECT>          m_ResultBlobList;
	//---------------------------------------------------------------------------------//
	POINT                      m_LBtnPtUp;
	POINT                      m_LBtnPtDown;
	POINT                      m_RBtnPtUp;
	POINT                      m_RBtnPtDown;
	POINT                      m_MousePtLast;
	POINT                      m_MousePtCurrent;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildShowModeCombox();	
	bool                       BuildMorphShapeModeCombox();
	bool                       BuildSizeLogicModeCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//	
	bool                       LoadImage(LPCTSTR filename);	
	bool                       ReleaseImageBuffer();
	bool                       SetImageBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//---------------------------------------------------------------------------------//	
	bool                       LoadPattern(LPCTSTR filename);
	bool                       ReleasePatternBuffer();
	bool                       SetPatternBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//---------------------------------------------------------------------------------//	
	bool                       ReleaseResultBuffer();
	bool                       SetResultBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//---------------------------------------------------------------------------------//	
	bool                       UpdateInfoText(UINT EditID);
	bool                       SetFocusEditWnd();
	UINT                       CheckBtnClickWndID(POINT pt);
	bool                       UpdateCursorPosToWnds(POINT pt);
	bool                       ResetAllPt();
	bool                       UpdateImageWndPtToOtherWnds(POINT pt);
	bool                       UpdatePatternWndPtToOtherWnds(POINT pt);	
	bool                       UpdateResultWndPtToOtherWnds(POINT pt);	
	bool                       UpdatePatToImgOffset(double PatResX, double PatResY);	
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();	
	//---------------------------------------------------------------------------------//	
	void                       DrawImageWnd();	
	void                       DrawImageWndBkDC();
	void                       DrawImage(HDC hDC);
	//---------------------------------------------------------------------------------//	
	void                       DrawPatternWnd();
	void                       DrawPatternWndBkDC();
	void                       DrawPattern(HDC hDC);
	//---------------------------------------------------------------------------------//	
	void                       DrawResultWnd();
	void                       DrawResultWndBkDC();
	void                       DrawResult(HDC hDC);
	//---------------------------------------------------------------------------------//	
	bool                       ExecMatch();
	bool                       ClearMatch();
	bool                       ExecCalculate();
	CString                    GetShowModeImageFile(int ShowMode);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgImageCompareWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLoadImageBtn();
	afx_msg void OnLoadPatternBtn();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMatchBtn();
	afx_msg void OnResetViewBtn();
	afx_msg void OnPatternPosSetBtn();
	afx_msg void OnPatternCalcBtn();
	afx_msg void OnSelchangeShowModeCombox();
	afx_msg void OnPatternResBtn();
	afx_msg void OnPatternUseScaleChk();
	afx_msg void OnEnhanceImageChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGIMAGECOMPAREWND_H__CE4FC40E_4E52_48ED_8258_B5D5534F1D80__INCLUDED_)
