#if !defined(AFX_PROJECTPARAMPANEVERSIONCODE_H__8B6A70B5_95AE_4308_BA12_055F0E77ECB9__INCLUDED_)
#define AFX_PROJECTPARAMPANEVERSIONCODE_H__8B6A70B5_95AE_4308_BA12_055F0E77ECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamPaneVersionCode.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_51     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_51     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneVersionCode dialog

class CProjectParamPaneVersionCode : public CDialog
{
// Construction
public:
	CProjectParamPaneVersionCode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamPaneVersionCode)
	enum { IDD = IDD_PROJECT_PARAM_VERSION_CODE_PANE };
	CEdit	m_EditCtrl;
	CThisListCtrl_51	m_VersionCodeListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamPaneVersionCode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;
	TProjectParameter         *m_ProParameterPtr;	
	//---------------------------------------------------------------------------------//	
	int                        m_ItemActIdx;
	int                        m_SubItemActIdx;
	bool                       m_StopVersionCodeListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	CThisListCtrl_51&          GetVersionCodeListWnd();
	int                        GetVersionCodeListWndItem();
	bool                       BuildVersionCodeListWndHeader();
	bool                       BuildVersionCodeListWnd();
	//---------------------------------------------------------------------------------//		
	bool                       ExecReleaseParamCtrl();
	bool                       ExecUpdateParamByEdit();
	bool                       ExecDblclkParamListWnd(CThisListCtrl_51 &ListCtrl, int nItem, int nSubItem);
	//---------------------------------------------------------------------------------//	
	bool                       ExecAddCodeBtn();
	bool                       ExecCopyCodeBtn();
	bool                       ExecClearCodeBtn();
	bool                       ExecDeleteCodeBtn();
	bool                       ExecModifyCodeBtn();
	bool                       ExecActiveCodeBtn();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamPaneVersionCode)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnItemchangedVersionCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkVersionCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddCodeBtn();
	afx_msg void OnModifyCodeBtn();
	afx_msg void OnCopyCodeBtn();
	afx_msg void OnDeleteCodeBtn();
	afx_msg void OnClearCodeBtn();
	afx_msg void OnActiveCodeBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMPANEVERSIONCODE_H__8B6A70B5_95AE_4308_BA12_055F0E77ECB9__INCLUDED_)
