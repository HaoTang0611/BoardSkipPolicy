#if !defined(AFX_NEWPANELWIZARDWND_H__D4EEE659_B0F6_44F0_A42F_7E4FD7BBDA09__INCLUDED_)
#define AFX_NEWPANELWIZARDWND_H__D4EEE659_B0F6_44F0_A42F_7E4FD7BBDA09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPanelWizardWnd.h : header file
//
//-------------------------------------------------------------------------------------//
enum NEW_CAD_MODE
{
	NEW_CAD_PROJECT = 1,
	NEW_CAD_PANEL   = 2,
	NEW_CAD_BOARD   = 3,
	NEW_CAD_RETURN
};
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "MotionCtrlWnd.h"
#include "ProjectMapWnd.h"
#include "LightCtrlBoardWnd.h"
//-------------------------------------------------------------------------------------//
#include "NewProjectLoadCadxyPane.h"
#include "NewProjectPanelAlignPane.h"
#include "NewProjectFiducialPane.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewPanelWizardWnd dialog
//-------------------------------------------------------------------------------------//
class CNewPanelWizardWnd : public CBaseDialog
{
// Construction
public:
	CNewPanelWizardWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPanelWizardWnd)
	enum { IDD = IDD_NEW_PANEL_WIZARD_WND };
	CButton	m_ExitBtn;
	CButton	m_FinishBtn;
	CButton	m_PrePaneBtn;
	CButton	m_NextPaneBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPanelWizardWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);	
	CAOIPanel*                 GetActivePanelPtr();
	//---------------------------------------------------------------------------------//
	void                       SetNewCadMode(NEW_CAD_MODE Mode);
	//---------------------------------------------------------------------------------//
protected:
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
	CAOIPanel                 *m_PanelPtr;
	CAOIProject               *m_ProjectPtr;	
	NEW_CAD_MODE               m_NewCadMode;
	size_t                     m_CurPaneIndex;
	CWnd*                      m_CurPaneWnd;
	std::vector<CWnd*>         m_PanePtrList;
	//---------------------------------------------------------------------------------//
	CMotionCtrlWnd              m_MotionCtrlWnd;
	CProjectMapWnd              m_ProjectMapWnd;	
	CNewProjectPaneLoadCadxy   m_PaneLoadCadxy;
	CNewProjectPaneAlignPanel  m_PaneAlignPanel;
	CNewProjectPaneFiducial    m_PaneFiducial;
	//---------------------------------------------------------------------------------//
	CAOIProject*                GetActiveProjectPtr();
	//---------------------------------------------------------------------------------//
	void                       ModifyPreNextFinishBtn();
	void                       ModifyControlWndPos();
	void                       DoSelchangePaneWnd();
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPanelWizardWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPrePaneBtn();
	afx_msg void OnNextPaneBtn();
	afx_msg void OnExitBtn();
	afx_msg void OnProjectMapBtn();
	afx_msg void OnMotionCtrlBtn();
	afx_msg void OnFinishBtn();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPANELWIZARDWND_H__D4EEE659_B0F6_44F0_A42F_7E4FD7BBDA09__INCLUDED_)
