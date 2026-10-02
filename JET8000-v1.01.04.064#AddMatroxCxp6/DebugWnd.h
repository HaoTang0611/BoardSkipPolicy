#if !defined(AFX_DEBUGWND_H__607FBB82_75D2_4646_BF9B_6B7214ADC9B1__INCLUDED_)
#define AFX_DEBUGWND_H__607FBB82_75D2_4646_BF9B_6B7214ADC9B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "dib.h"
#include "ImageWnd.h"
#include "AOIProject.h"
#include "ProjectMapWnd.h"
#include "TreeCtrlComponent.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugWnd dialog
//-------------------------------------------------------------------------------------//
class CDebugWnd : public CBaseDialog
{
// Construction
public:
	CDebugWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebugWnd)
	enum { IDD = IDD_DEBUG_WND };
	CImageWnd	m_ImageWnd;
	CTreeCtrlComponent	m_TreeWndComponent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:	
	CDib                         m_Dib;	
	CAMERA_ID                    m_CameraID;	
	CAOIProject*                 GetProjectPtr();
	CAOIProject*                 GetLastProjectPtr();
	CAOIProject*                 GetFirstProjectPtr();
	CProjectMapWnd               m_ProjectMapWnd;
	void                         ShowProjectInfo();
	void                         UpdatProjectMapWnd();
	bool                         BuildTreeWndCompolnent();
	void                         RedrawWnd();
	void                         DrawCtrlWnd(WPARAM wParam, LPARAM lParam);
	void                         MoveToActiveObj(WPARAM wParam, LPARAM lParam);
	BOOL                         ExecGrabImage();
	BOOL                         RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	void                         ExecTreeCtrlComponentMenu(CPoint point);	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugWnd)
	afx_msg void OnAddPanelBtn();
	afx_msg void OnAddBoardBtn();
	afx_msg void OnAddComponentBtn();
	afx_msg void OnDumpBtn();
	afx_msg void OnClonePanelBtn();
	afx_msg void OnCloneBoardBtn();
	afx_msg void OnCloneComponentBtn();
	afx_msg void OnDeletePanelBtn();
	afx_msg void OnDeleteBoardBtn();
	afx_msg void OnDeleteComponentBtn();
	afx_msg void OnClearAllBtn();
	afx_msg void OnDestroy();
	afx_msg void OnAddProjectBtn();
	afx_msg void OnCloneProjectBtn();
	afx_msg void OnDeleteProjectBtn();
	afx_msg void OnAddFdBtn();
	afx_msg void OnCloneFdBtn();
	afx_msg void OnDeleteFdBtn();
	afx_msg void OnAddSBBtn();
	afx_msg void OnCloneSBBtn();
	afx_msg void OnDeleteSBBtn();
	afx_msg void OnSaveBtn();
	afx_msg void OnLoadBtn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickTreeComponentWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadDibBtn();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnProjectImageBtn();
	afx_msg void OnGrabPanelFdBtn();
	afx_msg void OnResetThreadBtn();
	afx_msg void OnGrabBoardFdBtn();
	afx_msg void OnGrabComponentBtn();
	afx_msg void OnGoOrgBtn();
	afx_msg void OnShowMemoryBtn();
	afx_msg void OnShowThreadBtn();
	afx_msg void OnRebuildFieldBtn();
	afx_msg void OnShowTimeBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGWND_H__607FBB82_75D2_4646_BF9B_6B7214ADC9B1__INCLUDED_)
