#if !defined(AFX_ALGCHARVERIFYWND_H__3DDAB42A_5845_4F74_AB76_194799EDC800__INCLUDED_)
#define AFX_ALGCHARVERIFYWND_H__3DDAB42A_5845_4F74_AB76_194799EDC800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgCharVerifyWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgCharVerifyWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgCharVerifyWnd : public CBaseDialog
{
// Construction
public:
	CAlgCharVerifyWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgCharVerifyWnd)
	enum { IDD = IDD_ALG_CHAR_VERIFY_WND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgCharVerifyWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   GetWndPtr();
	void                       SetWndPtr(CAOIWnd *WndPtr);	
	void                       SetWndUniFrameList(std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CAOIWnd                   *m_WndPtr;
	std::vector<TUNI_FRAME>    m_UniFrameList;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgCharVerifyWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRoiAddBtn();
	afx_msg void OnRoiAddMatrixBtn();
	afx_msg void OnRoiDeleteBtn();
	afx_msg void OnRoiClearBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ALGCHARVERIFYWND_H__3DDAB42A_5845_4F74_AB76_194799EDC800__INCLUDED_)
