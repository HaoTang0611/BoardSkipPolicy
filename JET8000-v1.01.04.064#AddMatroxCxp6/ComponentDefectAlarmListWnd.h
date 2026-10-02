#if !defined(AFX_COMPONENTDEFECTALARMLISTWND_H__6EFDC711_A1C5_481F_800B_572635C3C3CF__INCLUDED_)
#define AFX_COMPONENTDEFECTALARMLISTWND_H__6EFDC711_A1C5_481F_800B_572635C3C3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentDefectAlarmListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_67     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_67     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentDefectAlarmListWnd dialog

class CComponentDefectAlarmListWnd : public CBaseDialog
{
// Construction
public:
	CComponentDefectAlarmListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComponentDefectAlarmListWnd)
	enum { IDD = IDD_COMPONENT_DEFECT_ALARM_LIST_WND };
	CComboBox	m_AlarmFromCombox;	
	CThisListCtrl_67	m_ComponentListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentDefectAlarmListWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        CompareComponentItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	DEFECT_FROM_MODE           m_AlarmFromMode;
	int                        m_ComponentColID;
	int                        m_ComponentListSortMode;
	bool                       m_StopComponentListBeSelected;
	std::vector<WND_DEFECT_ID> m_WndDefectIDList;
	//-------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	CAOIComponent             *m_ComponentPtr;
	std::vector<CAOIComponent*> m_ComponentList; 	
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//
	bool                       GetBuildAllComponentList() const;	
	bool                       BuildComponentListWnd(bool bAll);
	bool                       UpdateComponentListWnd();
	bool                       BuildComponentListWndHeader();		
	//-------------------------------------------------------------------------//
	bool                       ClearComponentListWnd();
	//-------------------------------------------------------------------------//
	void                       BuildWndDefectIDList();
	const std::vector<WND_DEFECT_ID>& GetWndDefectIDList() const;
	//-------------------------------------------------------------------------//
	DEFECT_FROM_MODE           GetAlarmFromMode() const;
	void                       SetAlarmFromMode(DEFECT_FROM_MODE val);
	//-------------------------------------------------------------------------//
	void                       SetComponentListColID(int val);
	int                        GetComponentListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetComponentListSortMode(int val);
	int                        GetComponentListSortMode() const;	
	//-------------------------------------------------------------------------//		
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponentDefectAlarmListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeAlarmFromCombo();
	afx_msg void OnSaveFileBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPONENTDEFECTALARMLISTWND_H__6EFDC711_A1C5_481F_800B_572635C3C3CF__INCLUDED_)
