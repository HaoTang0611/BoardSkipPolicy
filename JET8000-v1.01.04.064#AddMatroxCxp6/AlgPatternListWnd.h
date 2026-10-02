#if !defined(AFX_ALGPATTERNLISTWND_H__A10C3230_2375_4DCE_B409_B9C33E9013F9__INCLUDED_)
#define AFX_ALGPATTERNLISTWND_H__A10C3230_2375_4DCE_B409_B9C33E9013F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgPatternListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "JETListCtrl.h"
#define CThisListCtrl_02     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternListWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgPatternListWnd : public CBaseDialog
{
// Construction
public:
	CAlgPatternListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgPatternListWnd)
	enum { IDD = IDD_ALG_PATTERN_LIST_WND };
	CComboBox	m_PatternIconSizeCombox;
	CThisListCtrl_02	m_InfoListWnd;
	CThisListCtrl_02	m_PatternListWnd;
	CStatic	m_ImageWnd;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgPatternListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       SetWndPtr(CAOIWnd *WndPtr);
	void                       SetPatternFolder(LPCTSTR str);
	bool                       SetImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr);
	bool                       GetModified() const;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIWnd                   *m_WndPtr;
	CString                    m_NativeFolder;//樣板本來路徑
	bool                       m_Modified;
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_ImagePtr;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_ImageBitCount;
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ShowPtr;
	IMAGE_SIZE                 m_ShowW;
	IMAGE_SIZE                 m_ShowH;
	IMAGE_SIZE                 m_ShowStep;
	IMAGE_SIZE                 m_ShowBitCount;
	BITMAPINFO                *m_BitmapInfoPtr;
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BkColor;
	CJetMemDC                  m_ImageMemDC;
	//---------------------------------------------------------------------------------//
	CImageList                 m_PatternImageList;
	bool                       m_StopPatternListBeSelected;
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   GetWndPtr();
	void                       SetModified(bool val);
	void                       ReleaseImageBuffer();
	IMAGE_PTR                  GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	//---------------------------------------------------------------------------------//
	bool                       ReleaseShowBuffer();
	IMAGE_PTR                  GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	bool                       BuildShowBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);		
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       UpdateWndInfoText(CAOIWnd *WndPtr);
	bool                       BuildInfoListCtrlHeader(CThisListCtrl_02 &ListCtrl);
	bool                       BuildInfoListCtrl(CAOIWnd *WndPtr, CThisListCtrl_02 &ListCtrl);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       DrawImageWnd();
	void                       CreateBKImageWnd();
	void                       DrawImage(HDC hDC, const RECT &Rect);	
	//---------------------------------------------------------------------------------//
	void                       BuildPatternIconSizeCombox();
	//---------------------------------------------------------------------------------//
	bool                       ClearPatternListCtrl(CThisListCtrl_02 &ListCtrl);
	bool                       BuildPatternListCtrl(CAOIWnd *WndPtr, CThisListCtrl_02 &ListCtrl);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgPatternListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnGrayChk();
	afx_msg void OnBinaryChk();
	afx_msg void OnSelchangePatternIconSizeCombo();
	afx_msg void OnDeleteBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnShowRoiChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGPATTERNLISTWND_H__A10C3230_2375_4DCE_B409_B9C33E9013F9__INCLUDED_)
