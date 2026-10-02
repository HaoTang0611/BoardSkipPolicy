#if !defined(AFX_EDITIMAGEVIEW_H__2A8479E8_B156_41FE_9D8D_FA209B339F0F__INCLUDED_)
#define AFX_EDITIMAGEVIEW_H__2A8479E8_B156_41FE_9D8D_FA209B339F0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
#include "PaneTabCtrl.h"
#include "PageSplitterWnd.h"
#include "JETPropertyGridCtrl.h"
//-------------------------------------------------------------------------------------//
#include "EditImageProcessPage.h"
#include "EditImageView3DPage.h"
#include "EditImageBlobWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageView window

class CEditImageView : public CWnd
{
// Construction
public:
	CEditImageView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL     Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual ~CEditImageView();
	void                       OnChangeVisualStyle();

protected:	
	//---------------------------------------------------------------------------------//
	CAOIModel                 *m_ModelPtr;
	//---------------------------------------------------------------------------------//
	CPaneTabCtrl	           m_wndViewTabs;
	CEditImageProcessPage      m_wndImageProcess;	
	CEditImageView3DPage       m_wndImage3DWnd;
	CEditImageBlobWnd          m_wndImageBlob;	
	//---------------------------------------------------------------------------------//
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditImageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGEVIEW_H__2A8479E8_B156_41FE_9D8D_FA209B339F0F__INCLUDED_)
