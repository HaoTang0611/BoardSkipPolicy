#if !defined(AFX_COMPONENTDEFECTALARMWND_H__D78C159A_93C8_40C1_B7BF_9D18DFBF7D65__INCLUDED_)
#define AFX_COMPONENTDEFECTALARMWND_H__D78C159A_93C8_40C1_B7BF_9D18DFBF7D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentDefectAlarmWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_66     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmWnd dialog

class CComponentDefectAlarmWnd : public CBaseDialog
{
// Construction
public:
	CComponentDefectAlarmWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComponentDefectAlarmWnd)
	enum { IDD = IDD_COMPONENT_DEFECT_ALARM_WND };
	CEdit	m_EditCtrl;
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;	
	CComboBox	m_AlarmComboxAOI;
	CComboBox	m_AlarmComboxARS;
	CThisListCtrl_66 m_AlarmListWndAOI;
	CThisListCtrl_66 m_AlarmListWndARS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentDefectAlarmWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:	
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableAlarm() const;
	void                       SetEnableAlarm(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableAlarmOnAOI() const;
	void                       SetEnableAlarmOnAOI(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableAlarmOnARS() const;
	void                       SetEnableAlarmOnARS(bool val);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableDefectCountOnARS() const;
	void                       SetEnableDefectCountOnARS(bool val);
	//---------------------------------------------------------------------------------//
	const CWndDefectItem&      GetDefectAlarmAOI() const;
	void                       SetDefectAlarmAOI(const CWndDefectItem &val);
	//---------------------------------------------------------------------------------//	
	const CWndDefectItem&      GetDefectAlarmARS() const;
	void                       SetDefectAlarmARS(const CWndDefectItem &val);
	//---------------------------------------------------------------------------------//	
	DEFECT_PARAM_FROM_MODE     GetAlarmParamFromModeAOI() const;
	void                       SetAlarmParamFromModeAOI(DEFECT_PARAM_FROM_MODE val);
	//---------------------------------------------------------------------------------//	
	DEFECT_PARAM_FROM_MODE     GetAlarmParamFromModeARS() const;
	void                       SetAlarmParamFromModeARS(DEFECT_PARAM_FROM_MODE val);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	bool                       m_EnableAlarm;	
	bool                       m_EnableAlarmOnAOI;
	bool                       m_EnableAlarmOnARS;
	bool                       m_EnableDefectCountOnARS;//計數警報
	CParamList                 m_AlarmListAOI;	
	CParamUni                 *m_ParamActPtr;
	CWndDefectItem             m_DefectAlarmAOI;
	DEFECT_PARAM_FROM_MODE     m_AlarmParamFromModeAOI;
	bool                       m_StopAlarmListBeSelectedAOI;
	CParamList                 m_AlarmListARS;	
	CWndDefectItem             m_DefectAlarmARS;
	DEFECT_PARAM_FROM_MODE     m_AlarmParamFromModeARS;
	bool                       m_StopAlarmListBeSelectedARS;
	//---------------------------------------------------------------------------------//
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);		
	void                       SetDescriptionText(const CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       BuildAlarmListWndHeader(CThisListCtrl_66 &ListCtrl);
	bool                       BuildAlarmListWnd(CThisListCtrl_66 &ListCtrl, bool &StopSelected, CWndDefectItem &DefectItem, CParamList &ParamList);
	bool                       BuildAlarmList(CWndDefectItem &DefectItem, CParamList &ParamList);
	bool                       AddDefectAlarmItem(WND_DEFECT_ID WndDefectID, CWndDefectItem &DefectItem, CParamList &ParamList);
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_66 &ListCtrl, CParamList &ParamList, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_66 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem, int SetCol);
	bool                       ExecReleaseParamCtrl();
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
	bool                       BuildAlarmListWndHeader();
	bool                       BuildAlarmListWnd();	
	//---------------------------------------------------------------------------------//		
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponentDefectAlarmWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnItemchangedAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAlarmListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedAlarmListWndRepair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAlarmListWndRepair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnParamBtn();
	afx_msg void OnSelchangeAlarmFromComboAOI();
	afx_msg void OnAlarmEnableAllBtnAOI();
	afx_msg void OnAlarmDisableAllBtnAOI();
	afx_msg void OnAlarmCopyArsBtnAOI();
	afx_msg void OnSelchangeAlarmFromComboARS();
	afx_msg void OnAlarmEnableAllBtnARS();
	afx_msg void OnAlarmDisableAllBtnARS();
	afx_msg void OnAlarmCopyAoiBtnARS();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPONENTDEFECTALARMWND_H__D78C159A_93C8_40C1_B7BF_9D18DFBF7D65__INCLUDED_)
