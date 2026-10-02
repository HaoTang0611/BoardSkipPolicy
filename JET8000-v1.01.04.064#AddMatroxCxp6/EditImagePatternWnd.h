#if !defined(AFX_EDITIMAGEPATTERNWND_H__B2580760_6D17_451E_8928_653D2B6A3A6A__INCLUDED_)
#define AFX_EDITIMAGEPATTERNWND_H__B2580760_6D17_451E_8928_653D2B6A3A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImagePatternWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "PatternParam.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImagePatternWnd dialog
//-------------------------------------------------------------------------------------//
class CEditImagePatternWnd : public CDialog
{
// Construction
public:
	CEditImagePatternWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImagePatternWnd)
	enum { IDD = IDD_EDIT_IMAGE_PATTERN_WND };
	CSpinButtonCtrl	m_IndexSpin;
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImagePatternWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	bool                       ExecAddPattern();
	bool                       ExecTextPattern();
	//---------------------------------------------------------------------------------//
	void                       SetImagePatternWndPtr(CAOIWnd *WndPtr, bool UpdateToUI);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CDib                       m_Dib;
	COLORREF                   m_BkColor;
	CJetMemDC                  m_MemDC;
	CAOIWnd                   *m_WndPtr;
	RESULT_ID                  m_WndResultID;	
	int                        m_PatternIndex;
	int                        m_PatternCount;	
	CPatternParam              m_PatternParam;		
	std::vector<TPATTERN_ROI>  m_PatternRoiList;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       ChangeDrawModelMode();
	//---------------------------------------------------------------------------------//	
	void                       ResetWnd();
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       CreateBKImage();
	void                       DrawImage(HDC hDC, const RECT &Rect);
	void                       UpdateParamToUI(UINT FromCtrlID);
	void                       UpdateWndCtrlUI();	
	bool                       SwitchPatternImage(int index, bool UpdateUI=true);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecAddPattern_Field();
	bool                       ExecAddPattern_Model();	
	bool                       ExtractPattern_Field(bool bExtend, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &PatBitCount, IMAGE_PTR &PatPtr);
	bool                       ExtractPattern_Model(bool bExtend, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &PatBitCount, IMAGE_PTR &PatPtr);	
	bool                       ExecEditPattern();
	bool                       ExecDelPattern();
	bool                       ExecClearPattern();
	bool                       ResetPatternParam();//復歸樣板參數
	void                       UpdatePatternParamToParentWnd();
	bool                       ExecTextSetting();//文字設定		
	bool                       ExecAdvanceSetting();//進階設定	
	bool                       ExecUpdateBinaryToAlg();
	bool                       ExecUpdateBinaryToPattern();
	//---------------------------------------------------------------------------------//
	void                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImagePatternWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAddBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnTextBtn();
	afx_msg void OnAdvanceBtn();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposIndexSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBinaryChk();
	afx_msg void OnGrayChk();
	afx_msg void OnShowInfoChk();
	afx_msg void OnEditBtn();
	afx_msg void OnUpdateBinToAlgBtn();
	afx_msg void OnUpdateBinToPatBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGEPATTERNWND_H__B2580760_6D17_451E_8928_653D2B6A3A6A__INCLUDED_)
