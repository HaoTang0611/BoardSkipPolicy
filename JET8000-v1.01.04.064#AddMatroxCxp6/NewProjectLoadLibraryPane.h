#if !defined(AFX_NEWPROJECTLOADLIBRARYPANE_H__13B7F950_D4A2_43AF_92D6_352DDDEEB858__INCLUDED_)
#define AFX_NEWPROJECTLOADLIBRARYPANE_H__13B7F950_D4A2_43AF_92D6_352DDDEEB858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectLoadLibraryPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneLoadLibrary dialog

class CNewProjectPaneLoadLibrary : public CDialog
{
// Construction
public:
	CNewProjectPaneLoadLibrary(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneLoadLibrary)
	enum { IDD = IDD_NEW_PROJECT_PANE_LOAD_LIBRARY };
	CStatic	m_ModelImageWnd;
	CListCtrl	m_ModelListWnd;
	CComboBox	m_ModelFrameIndexCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneLoadLibrary)
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
	NEW_PROJECT_MODE           GetNewProjectMode() const;
	void                       SetNewProjectMode(NEW_PROJECT_MODE Mode);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableMultiDistrictMode() const;
	void                       SetEnableMultiDistrictMode(bool Mode);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	CString                    m_ErrorString;
	CAOIProject               *m_ProjectPtr;	
	NEW_PROJECT_MODE           m_NewProjectMode;
	bool                       m_EnableMultiDistrictMode;//多段模式
	//---------------------------------------------------------------------------------//			
	bool                       m_LibraryLoaded;
	RECT                       m_ModelImageWndRect;
	COLORREF                   m_ModelImageWndBkClr;
	CJetMemDC                  m_ModelImageWndMemDC;
	BOOL                       m_StopModelListBeSelected;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       CheckProjectPtr(CAOIProject *Ptr);
	//---------------------------------------------------------------------------------//		
	bool                       ClearModelListWnd();
	bool                       BuildModelListWnd();
	bool                       BuildModelListWndHeader();
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       DrawModelImageWnd();
	void                       DrawModelImage(HDC hDC, CAOIModel *ModelPtr);
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadLibrary();
	bool                       ExecLoadProject();
	bool                       ExecLoadServerLibrary();
	//---------------------------------------------------------------------------------//
	bool                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	bool                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneLoadLibrary)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnItemchangedModelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadLibraryBtn();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeModelBKImageIndexCombox();
	afx_msg void OnLoadProjectBtn();
	afx_msg void OnLoadServerLibraryBtn();
	afx_msg void OnSelectServerLibraryBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTLOADLIBRARYPANE_H__13B7F950_D4A2_43AF_92D6_352DDDEEB858__INCLUDED_)
