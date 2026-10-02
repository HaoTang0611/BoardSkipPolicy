#if !defined(AFX_PROJECTPARAMPANESAVE_H__0CB03225_B4C6_4269_8551_FE083B58829A__INCLUDED_)
#define AFX_PROJECTPARAMPANESAVE_H__0CB03225_B4C6_4269_8551_FE083B58829A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamPaneSave.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_26     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneSave dialog

class CProjectParamPaneSave : public CDialog
{
// Construction
public:
	CProjectParamPaneSave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamPaneSave)
	enum { IDD = IDD_PROJECT_PARAM_SAVE_PANE };
	CEdit	m_EditCtrl;	
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_26	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamPaneSave)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
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
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_26 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_26 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();	
	//---------------------------------------------------------------------------------//	
	bool                       ExecBtn_SaveModelImageOnOff_AI(CParamUni *ParamUni, CString &ResultText);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamPaneSave)
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
	afx_msg void OnParamBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMPANESAVE_H__0CB03225_B4C6_4269_8551_FE083B58829A__INCLUDED_)
