#if !defined(AFX_PROJECTIMAGEWND_H__8E9CDD22_0703_4CC6_8F0B_84976DA61342__INCLUDED_)
#define AFX_PROJECTIMAGEWND_H__8E9CDD22_0703_4CC6_8F0B_84976DA61342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectMapWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "resource.h"
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectMapWnd : public CBaseDialog
{
// Construction
public:
	//---------------------------------------------------------------------------------//	
	CProjectMapWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectMapWnd)
	enum { IDD = IDD_PROJECT_MAP_WND };
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectMapWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//		
	CImageWnd*                 GetProjectImageWnd();
	void                       RedrawWnd(BOOL bRedrawBK);//重繪視窗	
	CAOIProject*               GetProjectPtr() const;
	void                       SetProjectPtr(CAOIProject *ProjectPtr, bool bForce);//設定專案指標	
	void                       SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_MODE Mode);
	void                       BuildProjectMapWnd();
	void                       UpdateProjectMapWnd();
	void                       ClearProjectMapWnd();
	void                       MoveViewToStagePos(double PosX, double PosY);//移至機台位置
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_ProjectPtr;	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
	void                       UpdateCommandUI();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectMapWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowFd();
	afx_msg void OnShowPanel();
	afx_msg void OnShowBoard();
	afx_msg void OnShowBarcode();
	afx_msg void OnShowComponent();	
	afx_msg void OnShowCamera();		
	afx_msg void OnShowAll();
	afx_msg void OnShowNone();
	afx_msg void OnFuncNone();
	afx_msg void OnFuncMeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTIMAGEWND_H__8E9CDD22_0703_4CC6_8F0B_84976DA61342__INCLUDED_)
