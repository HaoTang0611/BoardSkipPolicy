#if !defined(AFX_IMAGEPHASEWND_H__F8B39A63_CB80_4651_A4C6_710404B47731__INCLUDED_)
#define AFX_IMAGEPHASEWND_H__F8B39A63_CB80_4651_A4C6_710404B47731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImagePhaseWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
const int PHASE_TO_IMAGE_FIXED_SCALE    = 1;//固定比率
const int PHASE_TO_IMAGE_DYNAMIC_SCALE  = 2;//動態比率
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseWnd dialog

class CImagePhaseWnd : public CBaseDialog
{
// Construction
public:
	CImagePhaseWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImagePhaseWnd)
	enum { IDD = IDD_IMAGE_PHASE_WND };
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImagePhaseWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	//---------------------------------------------------------------------------------//	
	void                       SetStartPos(int nX, int nY);
	bool                       SetSpaceBuffer(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ScaleMode, BOOL bClone);
    bool                       SetPhaseBuffer(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, int ScaleMode, BOOL bClone);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;
	POINT                      m_StartPos;//開始位置
	double                     m_ImageZoom;	
	TPOINT2D                   m_ImageWndPt1;
	TPOINT2D                   m_ImageWndPt2;
	TPOINT2D                   m_ImageOffset;
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	//---------------------------------------------------------------------------------//	
	int                        m_ScaleMode;
	BOOL                       m_bClonePhase;
	BOOL                       m_bCloneSpace;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_PhaseStep;
	IMAGE_SIZE                 m_SpaceStep;
	IMAGE_SIZE                 m_BitCount;	
	IMAGE_PTR                  m_ImageBuffer;
	PHASE_PTR                  m_PhaseBuffer;
	SPACE_PTR                  m_SpaceBuffer;
	MASK_PTR                   m_MaskBuffer;
	double                     m_PhaseAve;
	double                     m_SpaceAve;
	void                       ReleaseBuffer();
	//---------------------------------------------------------------------------------//
	void                       AdjustCtrlWnd(int cx=-1, int cy=-1);		
	void                       DrawImageWndMemDC();
	//---------------------------------------------------------------------------------//
	bool                       BuildImage();
	bool                       BuildPhaseImage();
	bool                       BuildSpaceImage();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	void                       DrawNoiseColorWnd();
	void                       DrawNoiseColorWnd(UINT CtrlID, COLORREF clr);
	//---------------------------------------------------------------------------------//	
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImagePhaseWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSaveBtn();
	afx_msg void OnNoiseLowContrastBtn();
	afx_msg void OnNoiseLowPotentialBtn();
	afx_msg void OnNoiseOverSaturatedBtn();
	afx_msg void OnNoiseVoidExtendedBtn();
	afx_msg void OnNoiseHeightUnexpectedBtn();
	afx_msg void OnNoiseOverLowBtn();
	afx_msg void OnValidBestBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPHASEWND_H__F8B39A63_CB80_4651_A4C6_710404B47731__INCLUDED_)
