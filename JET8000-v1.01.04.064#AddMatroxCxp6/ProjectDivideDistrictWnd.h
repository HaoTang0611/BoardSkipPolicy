#if !defined(AFX_PROJECTDIVIDEDISTRICTWND_H__E2C5D532_F3E9_4180_B7CC_6A21DE89C285__INCLUDED_)
#define AFX_PROJECTDIVIDEDISTRICTWND_H__E2C5D532_F3E9_4180_B7CC_6A21DE89C285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectDivideDistrictWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include <vector>
#include "JetMemDC.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectDivideDistrictWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectDivideDistrictWnd : public CBaseDialog
{
// Construction
public:
	CProjectDivideDistrictWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectDivideDistrictWnd)
	enum { IDD = IDD_PROJECT_DIVIDE_DISTRICT_WND };
	CStatic	m_ImageWnd;	
	CComboBox m_PanelListCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDivideDistrictWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	bool                       SetProjectPtr(CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableMultiDistrictMode() const;
	void                       SetEnableMultiDistrictMode(bool Mode);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;	
	double                     m_ZoomScale;	
	TPOINT2D                   m_ViewOffset;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	//---------------------------------------------------------------------------------//		
	DISTRICT_ID                m_DistrictID;
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
	DISTRICT_ID                GetDistrictID() const;
	void                       SetDistrictID(DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	CAOIPanel*                 GetActivePanelPtr();
	CAOIProject*               GetActiveProjectPtr();	
	//---------------------------------------------------------------------------------//	
	bool                       BuildPanelCombox();
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawProjectFd(HDC hDC);
	void                       DrawProjectBoard(HDC hDC);
	void                       DrawProjectBarcode(HDC hDC);
	void                       DrawProjectComponent(HDC hDC);
	//---------------------------------------------------------------------------------//
	bool                       ClearImageBuffer();
	bool                       CreateImageBuffer();
	bool                       BuildShowImage(size_t index);
	//---------------------------------------------------------------------------------//	
	bool                       ExecDivideDistrict();
	bool                       ExecMoveMap(int x, int y);	
	bool                       ExecMoveDivideLine(int x);
	bool                       BuildDefaultMap();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectDivideDistrictWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnDistrictDivideDistrictBtn();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnSelchangeDistrictPanelListCombo();
	afx_msg void OnDistrictDivideFdChk();
	afx_msg void OnDistrictDivideBarcodeChk();
	afx_msg void OnDistrictDivideComponentChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDIVIDEDISTRICTWND_H__E2C5D532_F3E9_4180_B7CC_6A21DE89C285__INCLUDED_)
