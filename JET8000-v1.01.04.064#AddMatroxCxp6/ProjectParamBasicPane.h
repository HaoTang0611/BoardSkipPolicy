#if !defined(AFX_PROJECTPARAMBASICPANE_H__F6088F66_0FD9_4454_8DE9_C4EFEE11578E__INCLUDED_)
#define AFX_PROJECTPARAMBASICPANE_H__F6088F66_0FD9_4454_8DE9_C4EFEE11578E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamBasicPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_24     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneBasic dialog
//-------------------------------------------------------------------------------------//
class CProjectParamPaneBasic : public CDialog
{
// Construction
public:
	CProjectParamPaneBasic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamPaneBasic)
	enum { IDD = IDD_PROJECT_PARAM_BASIC_PANE };
	CButton	m_SetFocusBtn;
	CButton	m_SetLaneWidthBtn;
	CEdit	m_EditCtrl;
	CButton	m_SetFolderBtn;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_24	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamPaneBasic)
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
	CParamUni                 *m_ParamActPtr;
	CParamList                 m_ParamList;
	CAOIProject               *m_ProjectPtr;
	TProjectParameter         *m_ProParameterPtr;	
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
	bool                       HideCtrlBtn();
	bool                       ExecReleaseParamCtrl();	
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_24 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_24 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();	
	//---------------------------------------------------------------------------------//	
	bool                       CheckJsonStringValid(LPCTSTR Text);//確認符合Json字串
	bool                       CheckNeedToVerifyJsonString(PROJECT_PARAM_ID ParamID);//確認是否需要確認Json字串
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamPaneBasic)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnParamBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFocusPosBtn();
	afx_msg void OnLaneWidthBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMBASICPANE_H__F6088F66_0FD9_4454_8DE9_C4EFEE11578E__INCLUDED_)
