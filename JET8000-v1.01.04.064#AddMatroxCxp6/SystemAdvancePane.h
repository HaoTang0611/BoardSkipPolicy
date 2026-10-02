#if !defined(AFX_SYSTEMADVANCEPANE_H__A85DDC48_9FD8_4867_B603_E004B0F64528__INCLUDED_)
#define AFX_SYSTEMADVANCEPANE_H__A85DDC48_9FD8_4867_B603_E004B0F64528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemAdvancePane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_29     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemAdvancePane dialog
//-------------------------------------------------------------------------------------//
class CSystemAdvancePane : public CDialog
{
// Construction
public:
	CSystemAdvancePane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemAdvancePane)
	enum { IDD = IDD_SYSTEM_ADVANCE_PANE };
	CEdit	m_EditCtrl;
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_29	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemAdvancePane)
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
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_29 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_29 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemAdvancePane)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnParamBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMADVANCEPANE_H__A85DDC48_9FD8_4867_B603_E004B0F64528__INCLUDED_)
