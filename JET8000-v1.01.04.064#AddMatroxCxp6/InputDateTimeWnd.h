#if !defined(AFX_INPUTDATETIMEWND_H__D4DEBAA0_7FA1_4F66_9A52_C9D515348921__INCLUDED_)
#define AFX_INPUTDATETIMEWND_H__D4DEBAA0_7FA1_4F66_9A52_C9D515348921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDateTimeWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
enum INPUT_DATE_TIME_MODE//輸入日期時間模式
{
	INPUT_DATE_TIME_DATE      = 1,
	INPUT_DATE_TIME_TIME      = 2,
	INPUT_DATE_TIME_BOTH      = 3,
	INPUT_DATE_TIME_RETURN
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputDateTimeWnd dialog
//-------------------------------------------------------------------------------------//
class CInputDateTimeWnd : public CBaseDialog
{
// Construction
public:
	CInputDateTimeWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDateTimeWnd)
	enum { IDD = IDD_INPUT_DATE_TIME_WND };	
	CString         m_WndText;
	CDateTimeCtrl	m_TimeCtrl;
	CDateTimeCtrl	m_DateCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDateTimeWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	const CTime&               GetDate() const;
	const CTime&               GetTime() const;
	const CTime&               GetResult() const;
	const CTime&               GetDateTime() const;
	//---------------------------------------------------------------------------------//
	void                       SetDate(const CTime &Date, LPCTSTR WndText=_T(""), bool bNone=false);
	void                       SetTime(const CTime &Time, LPCTSTR WndText=_T(""), bool bNone=false);
	void                       SetDateTime(const CTime &DateTime, LPCTSTR WndText=_T(""), bool bNone=false);
	void                       SetDateTime(const CTime &Date, const CTime &Time, LPCTSTR WndText=_T(""), bool bNone=false);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CTime                      m_Date;
	CTime                      m_Time;
	CTime                      m_DateTime;
	INPUT_DATE_TIME_MODE       m_DateTimeMode;
	bool                       m_EnabledNone;//支援取消[None]模式
	//---------------------------------------------------------------------------------//
	void                       UpdateWndText();
	void                       SwitchDateTimeMode();//切換日期時間模式	
	bool                       ModifyDateTimeShowNone(CDateTimeCtrl &Ctrl, bool bNone, UINT ID);
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	//---------------------------------------------------------------------------------//		
	void                       SetEnableNone(bool bNone);
	void                       SetWndText(LPCTSTR WndText);
	void                       SetupDate(const CTime &Date);
	void                       SetupTime(const CTime &Time);
	void                       SetDateTimeMode(INPUT_DATE_TIME_MODE Mode);
	bool                       CombineDateTime(const CTime &Date, const CTime &Time, CTime &DateTime);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDateTimeWnd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDATETIMEWND_H__D4DEBAA0_7FA1_4F66_9A52_C9D515348921__INCLUDED_)
