#if !defined(AFX_PLCCTRLPANEPARAMLIST_H__32C75D99_F0A6_4C7C_A0A0_F71858FBA2B7__INCLUDED_)
#define AFX_PLCCTRLPANEPARAMLIST_H__32C75D99_F0A6_4C7C_A0A0_F71858FBA2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlcParamListPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_16     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneParamList dialog
//-------------------------------------------------------------------------------------//
class CPLCCtrlPaneParamList : public CDialog
{
// Construction
public:
	CPLCCtrlPaneParamList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCCtrlPaneParamList)
	enum { IDD = IDD_PLC_PARAM_LIST_PANE };
	CEdit	m_EditCtrl;
	CComboBox	m_ComboxCtrl;	
	CListCtrl m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCCtrlPaneParamList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CParamList                 m_ParamList;
	CParamUni                 *m_ParamActPtr;
	bool                       m_StopParamListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeaer();
	//---------------------------------------------------------------------------------//
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       HideCtrlBtn();
	bool                       ExecReleaseParamCtrl();	
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_16 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_16 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();	
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCCtrlPaneParamList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnSaveBtn();
	afx_msg void OnUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCCTRLPANEPARAMLIST_H__32C75D99_F0A6_4C7C_A0A0_F71858FBA2B7__INCLUDED_)
