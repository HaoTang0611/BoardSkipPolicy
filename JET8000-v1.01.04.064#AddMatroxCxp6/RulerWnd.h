#if !defined(AFX_RULERWND_H__D64EBBCF_1A58_428B_A589_B34916D6978C__INCLUDED_)
#define AFX_RULERWND_H__D64EBBCF_1A58_428B_A589_B34916D6978C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RulerWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CRulerWnd dialog
//-------------------------------------------------------------------------------------//
class CRulerWnd : public CDialog
{
// Construction
public:
	CRulerWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRulerWnd)
	enum { IDD = IDD_RULER_WND };
	CStatic	m_RulerWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRulerWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	double                     m_RulerScaleX;
	double                     m_RulerScaleY;
	POINT                      m_RulerWndCP;
	COLORREF                   m_BkColor;
	RECT                       m_RulerWndRect;
	CJetMemDC                  m_RulerWndMemDC;

	void                       RedrawWnd();
	void                       DrawRulerImage(HDC hDC, bool bEraseBK);
	bool                       SaveRulerWndParameter();
	bool                       LoadRulerWndParameter();

	bool                       ExecChangeScaleValue();
protected:

	// Generated message map functions
	//{{AFX_MSG(CRulerWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CRulerWnd RulerWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULERWND_H__D64EBBCF_1A58_428B_A589_B34916D6978C__INCLUDED_)
