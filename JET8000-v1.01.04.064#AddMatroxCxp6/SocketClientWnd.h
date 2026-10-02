#if !defined(AFX_SOCKETCLIENTWND_H__9C517733_CD60_43E4_BC04_7864950D3865__INCLUDED_)
#define AFX_SOCKETCLIENTWND_H__9C517733_CD60_43E4_BC04_7864950D3865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketClientWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetSocket.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSocketClientWnd dialog
//-------------------------------------------------------------------------------------//
class CSocketClientWnd : public CBaseDialog
{
// Construction
public:
	CSocketClientWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSocketClientWnd)
	enum { IDD = IDD_SOCKET_CLIENT_WND };
	CListBox m_ClientSendListBox;
	CListBox m_ClientRecvListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketClientWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------------------//	
	CFont                      m_Font;
	UINT                       m_ConvertCode;
	CString                    m_JSONFilename;
	CJetSocketClient           m_JetSocketClient;//´ú¸Õ¥ÎªºWinSocket
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ToggleConnected(bool bConnected);
	//---------------------------------------------------------------------------------//
	bool                       ClearListBox(CListBox &ListBox);
	bool                       FillListBox(BOOL bClear, CString &str, CListBox &ListBox);
	//---------------------------------------------------------------------------------//
	bool                       UpdateSystemSocketListToUI();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSocketClientWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnConnectBtn();
	afx_msg void OnDisconnectBtn();
	afx_msg void OnSendMsgBtn();	
	afx_msg void OnSendFileBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCLIENTWND_H__9C517733_CD60_43E4_BC04_7864950D3865__INCLUDED_)
