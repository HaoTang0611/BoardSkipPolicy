#if !defined(AFX_NEWPROJECTWIZARDWND_H__5E533010_C2D8_40BF_B8E7_728056D616E1__INCLUDED_)
#define AFX_NEWPROJECTWIZARDWND_H__5E533010_C2D8_40BF_B8E7_728056D616E1__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectWizardWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIProject.h"
#include "MotionCtrlWnd.h"
#include "ProjectMapWnd.h"
#include "LightCtrlBoardWnd.h"
#include "NewProjectIntrodPane.h"
#include "NewProjectRegionImagePane.h"
#include "NewProjectLoadLibraryPane.h"
#include "NewProjectLoadCadxyPane.h"
#include "NewProjectPaneDivideDistrict.h"
#include "NewProjectPanelAlignPane.h"
#include "NewProjectFiducialPane.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectWizardWnd dialog
//-------------------------------------------------------------------------------------//
class CNewProjectWizardWnd : public CBaseDialog
{
// Construction
public:
	CNewProjectWizardWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectWizardWnd)
	enum { IDD = IDD_NEW_PROJECT_WIZARD_WND };
	CButton	m_ExitBtn;
	CButton	m_FinishBtn;
	CButton	m_PrePaneBtn;
	CButton	m_NextPaneBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectWizardWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:	
	//---------------------------------------------------------------------------------//	
	NEW_PROJECT_MODE           GetNewProjectMode() const { return m_NewProjectMode; }
	void                       SetNewProjectMode(NEW_PROJECT_MODE Mode) { m_NewProjectMode=Mode; }	
	//---------------------------------------------------------------------------------//		
	const TProjectParameter*   GetProjectParameter() const { return &m_ProjectParameter; }
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetProjectPtr() { return m_ProjectPtr; }
	CAOIPanel*                 GetActivePanelPtr() { return m_PanelPtr; }
	void                       SetProjectExist(CAOIProject *Ptr) { m_ProjectExist=Ptr; }
	//---------------------------------------------------------------------------------//
	bool                       GetEnableMultiDistrictMode() const { return m_EnableMultiDistrictMode; }
	void                       SetEnableMultiDistrictMode(bool Mode) { m_EnableMultiDistrictMode=Mode; }	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	NEW_PROJECT_MODE           m_NewProjectMode;
	bool                       m_EnableMultiDistrictMode;//多段模式
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_ImageBuffer;//影像記憶體空間
	size_t                     m_ImageBufferSize;//影像記憶體尺寸	
	bool                       CreateImageBuffer();//建立影像資料
	bool                       DestroyImageBuffer();//摧毀影像資料

	IMAGE_PTR                  m_ShowBuffer;//顯示記憶體空間
	size_t                     m_ShowBufferSize;//顯記憶體尺寸
	bool                       CreateShowBuffer();//建立顯示資料
	bool                       DestroyShowBuffer();//摧毀顯示資料
	//---------------------------------------------------------------------------------//
	CString                    m_WndText;
	CAOIProject*               m_ProjectPtr;	
	CAOIProject*               m_ProjectExist;	
	CAOIPanel*                 m_PanelPtr;	
	size_t                     m_CurPaneIndex;
	CWnd*                      m_CurPaneWnd;
	std::vector<CWnd*>         m_PanePtrList;
	TProjectParameter          m_ProjectParameter;
	//---------------------------------------------------------------------------------//	
	CLightCtrlBoardWnd          m_LightCtrlWnd;
	CMotionCtrlWnd              m_MotionCtrlWnd;
	CProjectMapWnd              m_ProjectMapWnd;	
	CNewProjectPaneIntroduction m_PaneIntroduction;
	CNewProjectPaneRegionImage m_PaneRgnImage;
	CNewProjectPaneLoadLibrary m_PaneLoadLib;
	CNewProjectPaneLoadCadxy   m_PaneLoadCadxy;
	CNewProjectPaneDivideDistrict m_PaneDivideDistrict;
	CNewProjectPaneAlignPanel  m_PaneAlignPanel;
	CNewProjectPaneFiducial    m_PaneFiducial;	
	CNewProjectPaneAlignPanel  m_PaneAlignPanel_DB;
	CNewProjectPaneFiducial    m_PaneFiducial_DB;	
	//---------------------------------------------------------------------------------//
	bool                       CheckNewProject() const;
	void                       ModifyPreNextFinishBtn();
	void                       ModifyControlWndPos();
	void                       DoSelchangePaneWnd();
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectWizardWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNextPaneBtn();
	afx_msg void OnPrePaneBtn();
	afx_msg void OnFinishBtn();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExitBtn();
	afx_msg void OnMotionCtrlBtn();
	afx_msg void OnProjectMapBtn();
	afx_msg void OnLightCtrlWndBtn();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTWIZARDWND_H__5E533010_C2D8_40BF_B8E7_728056D616E1__INCLUDED_)
