#if !defined(AFX_PROJECTPARAMPANESPECTEST_H__817C3DA6_C37A_404E_AFF1_FC33E67E9296__INCLUDED_)
#define AFX_PROJECTPARAMPANESPECTEST_H__817C3DA6_C37A_404E_AFF1_FC33E67E9296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamPaneSpecTest.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_27     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneSpecTest dialog

class CProjectParamPaneSpecTest : public CDialog
{
// Construction
public:
	CProjectParamPaneSpecTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamPaneSpecTest)
	enum { IDD = IDD_PROJECT_PARAM_SPEC_TEST_PANE };
	CEdit	m_EditCtrl;
	CButton	m_BtnCtrl;
	CButton	m_DropBasePlaneBtn;
	CButton	m_DropSpaceFilterBtn;
	CButton m_DropRegionBtn;
	CComboBox	m_ComboxCtrl;		
	CThisListCtrl_27	m_DropOutParamListCtrl;
	CThisListCtrl_27	m_ScratchParamListCtrl;		
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamPaneSpecTest)
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
	CParamList                 m_DropOutList;
	CParamList                 m_ScratchList;
	CAOIProject               *m_ProjectPtr;
	TProjectParameter         *m_ProParameterPtr;			
	CParamList                 m_DefectEnableList;
	//---------------------------------------------------------------------------------//		
	bool                       m_StopDefectEnableListBeSelected;
	bool                       m_StopDropOutParamListBeSelected;
	bool                       m_StopScratchParamListBeSelected;
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildDropOutParamList();
	bool                       BuildDropOutParamListWnd();
	bool                       BuildDropOutParamListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       BuildScratchParamList();
	bool                       BuildScratchParamListWnd();
	bool                       BuildScratchParamListWndHeader();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_27 &ListCtrl, CParamList &ParamList, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_27 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamPaneSpecTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnParamBtn();
	virtual void OnOK();
	virtual void OnCancel();	
	afx_msg void OnItemchangedDropOutListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDropOutListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedScratchListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkScratchListWnd(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDropBasePlaneBtn();
	afx_msg void OnDropSpaceFilterBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMPANESPECTEST_H__817C3DA6_C37A_404E_AFF1_FC33E67E9296__INCLUDED_)
