#if !defined(AFX_SYSTEMDEBUGPANE_H__372DDEC6_20CE_4F6E_AAD4_5E5EC391AFF0__INCLUDED_)
#define AFX_SYSTEMDEBUGPANE_H__372DDEC6_20CE_4F6E_AAD4_5E5EC391AFF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDebugPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_45     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemDebugPane dialog
//-------------------------------------------------------------------------------------//
class CSystemDebugPane : public CDialog
{
// Construction
public:
	CSystemDebugPane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemDebugPane)
	enum { IDD = IDD_SYSTEM_DEBUG_PANE };
	CEdit	m_EditCtrl;
	CButton	m_BtnFolder;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_45	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDebugPane)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetSystemParameterPtr(TSystemParameter *Ptr);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//				
	CParamUni                 *m_ParamActPtr;
	CParamList                 m_ParamList;
	TSystemParameter          *m_SysParameterPtr;	
	bool                       m_StopParamListBeSelected;	
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_45 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_45 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemDebugPane)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnFolderBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMDEBUGPANE_H__372DDEC6_20CE_4F6E_AAD4_5E5EC391AFF0__INCLUDED_)
