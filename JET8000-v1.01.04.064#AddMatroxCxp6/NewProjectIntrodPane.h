#if !defined(AFX_NEWPROJECTINTRODPANE_H__5D68BAB7_336C_439D_AB7D_1AD743753C78__INCLUDED_)
#define AFX_NEWPROJECTINTRODPANE_H__5D68BAB7_336C_439D_AB7D_1AD743753C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectIntrodPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneIntroduction dialog
//-------------------------------------------------------------------------------------//
class CNewProjectPaneIntroduction : public CDialog
{
// Construction
public:
	CNewProjectPaneIntroduction(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneIntroduction)
	enum { IDD = IDD_NEW_PROJECT_PANE_INTROD };
	CComboBox	m_LaneCombox;
	CComboBox	m_PanelSideCombox;
	CComboBox	m_DlpLEDColorCombox;
	CComboBox	m_HeightRatioCombox;
	CComboBox	m_FieldSizeModeComboxW;
	CComboBox	m_FieldSizeModeComboxH;
	CComboBox	m_FrameCombox08;
	CComboBox	m_FrameCombox07;
	CComboBox	m_FrameCombox06;
	CComboBox	m_FrameCombox05;
	CComboBox	m_FrameCombox04;
	CComboBox	m_FrameCombox03;
	CComboBox	m_FrameCombox02;
	CComboBox	m_FrameCombox01;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneIntroduction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       ExecNextPane();
	bool                       ExecPrevPane();
	bool                       ExecFinishPane();
	bool                       ReInitialPane();
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//	
	bool                       GetIsNewProject() const;
	void                       SetIsNewProject(bool Val);
	//---------------------------------------------------------------------------------// 
	NEW_PROJECT_MODE           GetNewProjectMode() const;
	void                       SetNewProjectMode(NEW_PROJECT_MODE Mode);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableMultiDistrictMode() const;
	void                       SetEnableMultiDistrictMode(bool Mode);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	bool                       m_IsNewProject;
	NEW_PROJECT_MODE           m_NewProjectMode;	
	bool                       m_EnableMultiDistrictMode;//¦h¬q¼Ò¦¡
	//---------------------------------------------------------------------------------//	
	bool                       InitProjectFrameCombox();
	bool                       BuildProjectFrameCombox();
	bool                       CloaseAllProjectFrameCombox();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       BuildFrameUniqueIDList(std::vector<unsigned int> &FrameUniqueIDList);
	bool                       AddFrameUniqueIDList(CComboBox &Combox, std::vector<unsigned int> &FrameUniqueIDList, bool b3DMode);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneIntroduction)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFileNameFormatBtn();
	afx_msg void OnFileFolderBrowserBtn();
	afx_msg void OnFrameShowChk();
	afx_msg void OnPCBInBtn();
	afx_msg void OnPCBBackBtn();
	afx_msg void OnPCBOutBtn();
	afx_msg void OnPCBClampOnBtn();
	afx_msg void OnLaneWidthGetBtn();
	afx_msg void OnLaneWidthSetBtn();
	afx_msg void OnPCBIn2ndBtn();
	afx_msg void OnPCBIn3rdBtn();
	afx_msg void OnLaneAdjustWidthBtn();
	afx_msg void OnFrameCloseAllBtn();
	afx_msg void OnFrameDefaultBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTINTRODPANE_H__5D68BAB7_336C_439D_AB7D_1AD743753C78__INCLUDED_)
