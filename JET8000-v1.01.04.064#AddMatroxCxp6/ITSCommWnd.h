#if !defined(AFX_ITSCOMMWND_H__62007BAD_E00F_4518_AA2B_90CB994DC519__INCLUDED_)
#define AFX_ITSCOMMWND_H__62007BAD_E00F_4518_AA2B_90CB994DC519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ITSCommWnd.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CITSCommWnd dialog
//-------------------------------------------------------------------------------------//
class CITSCommWnd : public CDialog
{
// Construction
public:
	CITSCommWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CITSCommWnd)
	enum { IDD = IDD_ITS_COMM_WND };
	CListBox m_ClientSendListBox;
	CListBox m_ClientRecvListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CITSCommWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	bool                       SetShowTimer(DWORD val);//設定顯示時間	
	bool                       SetMessage(CString &str);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CFont                      m_Font;
	UINT                       m_ConvertCode;
	CString                    m_JSONFilename;
	//---------------------------------------------------------------------------------//	
	DWORD                      m_TickCountRecv;	
	DWORD                      m_TickCountSend;		
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ToggleConnected(bool bConnected);
	//---------------------------------------------------------------------------------//
	bool                       ClearListBox(CListBox &ListBox);
	bool                       FillListBox(BOOL bClear, CString &str, CListBox &ListBox);
	//---------------------------------------------------------------------------------//
	bool                       SetMesCallback(bool Used);
	bool                       UpdateCommunicationUI();//更新連線介面
	bool                       UpdateSystemSocketListToUI();
	//---------------------------------------------------------------------------------//
	void                       ExecConnectBtn_ITS();
	void                       ExecConnectBtn_IPS();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CITSCommWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnConnectBtn();
	afx_msg void OnDisconnectBtn();
	afx_msg void OnSendMsgBtn();
	afx_msg void OnSendFileBtn();
	afx_msg void OnProjectUploadBtn();
	afx_msg void OnProjectDownloadBtn();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CITSCommWnd ITSCommWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITSCOMMWND_H__62007BAD_E00F_4518_AA2B_90CB994DC519__INCLUDED_)
