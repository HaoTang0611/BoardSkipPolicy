// JET8000.h : main header file for the JET8000 application
//

#if !defined(AFX_JET8000_H__33D8CC04_83CE_435E_945A_119DAD26CFCB__INCLUDED_)
#define AFX_JET8000_H__33D8CC04_83CE_435E_945A_119DAD26CFCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CJET8000App:
// See JET8000.cpp for the implementation of this class
//
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	#define CBasicApp   CWinAppEx
	//typedef CWinAppEx   CBasicApp;
#else
	#define CBasicApp   CWinApp	
	//typedef CWinApp   CBasicApp;
#endif//FRAME_STYLE_TYPE

class CJET8000App : public CBasicApp
{
public:
	CJET8000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJET8000App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL
public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

protected:
	HANDLE m_Mutex;

// Implementation
	//{{AFX_MSG(CJET8000App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CJET8000App theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JET8000_H__33D8CC04_83CE_435E_945A_119DAD26CFCB__INCLUDED_)
