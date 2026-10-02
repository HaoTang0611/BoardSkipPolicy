#if !defined(AFX_NEWPROJECTPANEDIVIDEDISTRICT_H__1F5B83D1_6234_41F9_9E06_179E7343A0E0__INCLUDED_)
#define AFX_NEWPROJECTPANEDIVIDEDISTRICT_H__1F5B83D1_6234_41F9_9E06_179E7343A0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectPaneDivideDistrict.h : header file
//
//-------------------------------------------------------------------------------------//
#include <vector>
//#include "AOIProject.h"
#include "JetMemDC.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneDivideDistrict dialog
//-------------------------------------------------------------------------------------//
class CNewProjectPaneDivideDistrict : public CDialog
{
// Construction
public:
	CNewProjectPaneDivideDistrict(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneDivideDistrict)
	enum { IDD = IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT };
	CStatic	m_ImageWnd;	
	//}}AFX_DATA

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
	bool                       m_Finish;
	CString                    m_ErrorString;
	bool                       m_ReBuild;
	double                     m_ZoomScale;	
	TPOINT2D                   m_ViewOffset;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_ProjectPtr;
	NEW_PROJECT_MODE           m_NewProjectMode;	
	CMapCoordinate             m_MapCTS;
	int                        m_DividePosX;//分割線
	bool                       m_EnableMultiDistrictMode;//多段模式
	//---------------------------------------------------------------------------------//	
	POINT                      m_LastPos;	
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_PTR                  m_ImagePtr;
	size_t                     m_ImageSize;
	TPOINT2D                   m_ImageRes;
	TREGION4D                  m_ImageStageRgn;
	BITMAPINFO                *m_ImageInfoPtr;
	//---------------------------------------------------------------------------------//	
	RECT                       m_MapRect_DA;
	RECT                       m_MapRect_DB;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
	DISTRICT_ID                GetDistrictID();
	//---------------------------------------------------------------------------------//
	CAOIPanel*                 GetActivePanelPtr();
	CAOIProject*               GetActiveProjectPtr();	
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawProjectBoard(HDC hDC);
	void                       DrawProjectComponent(HDC hDC);
	//---------------------------------------------------------------------------------//
	bool                       ClearImageBuffer();
	bool                       CreateImageBuffer();
	bool                       BuildShowImage(size_t index);
	//---------------------------------------------------------------------------------//	
	bool                       CheckFinish();
	void                       SetFinish(bool val);
	bool                       ExecDivideDistrict();
	bool                       ExecMoveMap(int x, int y);	
	bool                       ExecMoveDivideLine(int x);
	bool                       BuildDefaultMap();	
	//---------------------------------------------------------------------------------//
	bool                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	bool                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	//---------------------------------------------------------------------------------//	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneDivideDistrict)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneDivideDistrict)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();		
	afx_msg void OnOrientationRotate090Btn();
	afx_msg void OnOrientationRotate180Btn();
	afx_msg void OnOrientationRotate270Btn();
	afx_msg void OnOrientationCenteredBtn();
	afx_msg void OnOrientationMirrorxBtn();
	afx_msg void OnOrientationMirroryBtn();	
	afx_msg void OnDivideDistrictBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTPANEDIVIDEDISTRICT_H__1F5B83D1_6234_41F9_9E06_179E7343A0E0__INCLUDED_)

