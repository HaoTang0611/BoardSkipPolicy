#if !defined(AFX_ALGBLOBCOUNTWND_H__FCC67E9A_276A_4A94_A87E_B38C1498BE4E__INCLUDED_)
#define AFX_ALGBLOBCOUNTWND_H__FCC67E9A_276A_4A94_A87E_B38C1498BE4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgBlobCountWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBlobCountWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgBlobCountWnd : public CBaseDialog
{
// Construction
public:
	CAlgBlobCountWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgBlobCountWnd)
	enum { IDD = IDD_ALG_BLOB_COUNT_WND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgBlobCountWnd)
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
	//{{AFX_MSG(CAlgBlobCountWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGBLOBCOUNTWND_H__FCC67E9A_276A_4A94_A87E_B38C1498BE4E__INCLUDED_)
