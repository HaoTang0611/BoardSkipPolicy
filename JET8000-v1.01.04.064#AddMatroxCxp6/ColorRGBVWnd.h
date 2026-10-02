#if !defined(AFX_COLORRGBVWND_H__36F2CDB6_41CE_4144_94AE_126FDE240799__INCLUDED_)
#define AFX_COLORRGBVWND_H__36F2CDB6_41CE_4144_94AE_126FDE240799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorRGBVWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include <vector>
#include "JetMemDC.h"
#include "ColorRGBV.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CColorRGBVWnd window
//-------------------------------------------------------------------------------------//
class CColorRGBVWnd : public CStatic
{
// Construction
public:
	CColorRGBVWnd();

// Attributes
public:

// Operations
public:
	//-------------------------------------------------------------------------//
	void               SetCallBackWnd(HWND hWnd);
	//-------------------------------------------------------------------------//
	void               CreateMemDC();	
	void               ReDrawWnd();
	//-------------------------------------------------------------------------//
	void               SetRGBVMode(COLOR_RGBV_MODE Mode);
	int                GetRGBVMode();
	//-------------------------------------------------------------------------//
	void               SetColorRGBVPtr(CColorRGBV *rgbvPtr);
	CColorRGBV        *GetColorRGBVPtr();
	//-------------------------------------------------------------------------//
	void               ClearColorRGBVTmp();
	void               SetColorRGBVTmp(const CColorRGBV &rgbvPtr);	
	//-------------------------------------------------------------------------//	
	void               ClearColorRGBVOld();
	void               SetColorRGBVOld(const CColorRGBV &rgbvPtr);	
	CColorRGBV        *GetColorRGBVOldPtr();
	//-------------------------------------------------------------------------//	
	void               SetColorFilterID(int ID);
	int                GetColorFilterID();
	//-------------------------------------------------------------------------//
	void               SetTriangleSize(int cx, int cy);
	//-------------------------------------------------------------------------//
	bool               GetLockWnd();
	void               SetLockWnd(bool bLock);	
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                m_ColorFilterID;

	HWND               m_CallBackHWnd;
	SIZE               m_RGBVTriangleSize;
	RECT               m_WndRect;
	RECT               m_RGBVTriangleRect;
	RECT               m_RGBVHorBarRect;
	RECT               m_RGBVTuningBarRect;
	DWORD              m_MouseMoveTime;//滑鼠移動時間點
	DWORD              m_MouseWheelTime;//滑鼠滾輪時間點

	COLORREF           m_BKClr;
	CJetMemDC          m_MemDC;
	COLOR_RGBV_MODE    m_RGBVMode;
	CColorRGBV        *m_ColorRGBVPtr;
	
	CColorRGBV         m_ColorRGBVTmp;	
	CColorRGBV         m_ColorRGBVOld;//原始顏色

	bool               m_bLockUI;
	CURSOR_POS_MODE    m_MouseMode;
	int                m_PickMode;
	POINT              m_LastMousePt;
	POINT              m_MouseUpPt;

	double             m_CurrentMainT;
	double             m_CurrentMainB;

	double             m_CurrentVT;
	double             m_CurrentVB;	

	//-------------------------------------------------------------------------//	
	void               CreateRGBVBKImage();
	void               DrawRGBVTriangleImage(HDC hDC);
	void               DrawRGBVBarImage(HDC hDC);
	void               DrawRGBVLine(HDC hDC, CColorRGBV *rgbvPtr, bool bAct);
	void               DrawDotPos(HDC hDC, double x, double y);
	void               DrawRectLine(HDC hDC, const RECT &Rect);
	void               BuildRect(const POINT &Pt1, const POINT &Pt2, RECT &Rect);	
	//-------------------------------------------------------------------------//	
	void               DoMouseMoveUIEnd_RGBV();
	void               StartMouseMoveUIEndEvent_RGBV();
	void               DoMouseMoveUI_RGBV(POINT point, CColorRGBV *rgbvPtr);	
	void               DoMouseWheelUIEnd_RGBV();
	void               StartMouseWheelUIEndEvent_RGBV();
	void               DoMouseWheelUI_RGBV(UINT nFlags, short zDelta, CPoint &pt, CColorRGBV *rgbvPtr);	

	CURSOR_POS_MODE    GetTriangleMouseMode(POINT point, CColorRGBV *rgbvPtr);	
	CURSOR_POS_MODE    GetTrunningMouseMode(POINT point, CColorRGBV *rgbvPtr);	
	CURSOR_POS_MODE    GetBrightnessMouseMode(POINT point, CColorRGBV *rgbvPtr);	

	bool               DoLButtonDownForUI_RGBV(POINT point, CColorRGBV *rgbvPtr);
	//-------------------------------------------------------------------------//

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorRGBVWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	//------------------------------------------------------------------------//
	virtual ~CColorRGBVWnd();
	//------------------------------------------------------------------------//
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorRGBVWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORRGBVWND_H__36F2CDB6_41CE_4144_94AE_126FDE240799__INCLUDED_)
