#if !defined(AFX_PROJECTPARAMPANEALARM_H__4A280654_B84C_4308_896E_C36FA1FAFACF__INCLUDED_)
#define AFX_PROJECTPARAMPANEALARM_H__4A280654_B84C_4308_896E_C36FA1FAFACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectParamPaneAlarm.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_22     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneAlarm dialog

class CProjectParamPaneAlarm : public CDialog
{
// Construction
public:
	CProjectParamPaneAlarm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectParamPaneAlarm)
	enum { IDD = IDD_PROJECT_PARAM_ALARM_PANE };	
	CEdit	m_EditCtrl;
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;	
	CThisListCtrl_22	m_DefectAlarmListCtrl;
	CThisListCtrl_22	m_DefectTestListCtrl;
	CThisListCtrl_22	m_DefectParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectParamPaneAlarm)
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
	CParamList                 m_AlarmList;
	CParamList                 m_DefectList;	
	CAOIProject               *m_ProjectPtr;
	TProjectParameter         *m_ProParameterPtr;	
	bool                       m_StopParamListBeSelected;	
	bool                       m_StopAlarmListBeSelected;	
	bool                       m_StopDefectListBeSelected;		
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildDefectParamList();
	bool                       BuildDefectParamListWnd();
	//---------------------------------------------------------------------------------//		
	bool                       BuildDefectAlarmList();
	bool                       BuildDefectAlarmListWnd();		
	bool                       AddDefectAlarmItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList);
	//---------------------------------------------------------------------------------//	
	bool                       BuildDefectTestList();
	bool                       BuildDefectTestListWnd();	
	bool                       AddDefectTestItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList);
	//---------------------------------------------------------------------------------//
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_22 &ListCtrl, CParamList &ParamList, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_22 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem, int SetCol);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectParamPaneAlarm)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnParamBtn();	
	afx_msg void OnDefectEnableAllBtn();
	afx_msg void OnDefectDisableAllBtn();
	afx_msg void OnDefectCopyAlarmBtn();
	afx_msg void OnAlarmEnableAllBtn();
	afx_msg void OnAlarmDisableAllBtn();
	afx_msg void OnAlarmCopyDefectBtn();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPARAMPANEALARM_H__4A280654_B84C_4308_896E_C36FA1FAFACF__INCLUDED_)
