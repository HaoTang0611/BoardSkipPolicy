#if !defined(AFX_IMAGEDEBUGWND_H__670E61F5_D292_4A98_B411_62DD12003F00__INCLUDED_)
#define AFX_IMAGEDEBUGWND_H__670E61F5_D292_4A98_B411_62DD12003F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageDebugWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "dib.h"
#include "JetBlob.h"
#include "JetMemDC.h"
#include "JetMatch.h"
//-------------------------------------------------------------------------------------//
enum IMAGE_DEBUG_MODE
{
	IMAGE_DEBUG_DISABLE=0,
	IMAGE_DEBUG_PROFILE=1,
	IMAGE_DEBUG_MATCH=2,
	IMAGE_DEBUG_BARCODE=3
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageDebugWnd dialog

class CImageDebugWnd : public CBaseDialog
{
// Construction
public:
	CImageDebugWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageDebugWnd)
	enum { IDD = IDD_IMAGE_DEBUG_WND };
	CComboBox	m_DebugCombox;
	CStatic	m_ProfileWnd;
	CComboBox	m_ImageSourceComboxWnd;
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDebugWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	COLORREF                   m_clrBK;
	CJetBlob                   m_Blob;
	CDib                       m_Dib;	
	PHASE_PTR                  m_PhasePtr;	
	MASK_PTR                   m_PhaseMaskPtr;
	unsigned char             *m_pImage;
	unsigned char             *m_pImageShow;	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_ImageStep;

	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;
	TPOINT2D                   m_ImageOffset;	
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;	
	double                     m_ImageZoom;	
	IMAGE_DEBUG_MODE           m_DebugMode;

	TPOINT2D                   m_ImageWndPt1;
	TPOINT2D                   m_ImageWndPt2;
	
	CString                    m_strPixel;
	
	void                       RedrawWnd();
	void                       DrawProfileWnd();
	bool                       PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2);	

	CJetMatch                  m_LibMatch;

	LARGE_INTEGER              m_nFreq;
	LARGE_INTEGER              m_nStartTime;
	LARGE_INTEGER              m_nEndTime;
	bool                       BuildSinPatternImage(int ImageW, int ImageH, int ImageStep, int PhaseStep, LPCTSTR Folder);
	bool                       CalcPhasePeriod();

	void                       TestJetImage();
	void                       TestExtractSubRoiSubPix();
	void                       TestMorphImage();
	void                       TestRotateImage();
	void                       TestMiMLibImage();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageDebugWnd)
	afx_msg void OnLoadBmpBtn();
	afx_msg void OnSaveBmpBtn();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeImageSourceCombo();
	afx_msg void OnSaveRoiBmpBtn();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFloodFillBtn();
	afx_msg void OnBlobBtn();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeDebugCombo();
	afx_msg void OnLearnPatternBtn();
	afx_msg void OnMatchPatternBtn();
	afx_msg void On1DBarcodeBtn();
	afx_msg void OnDataMatrixBtn();
	afx_msg void OnThinBtn();
	afx_msg void OnSmoothBtn();
	afx_msg void OnLoadRawBtn();
	afx_msg void OnSharpBtn();
	afx_msg void OnAverageBtn();
	afx_msg void OnSobelBtn();
	afx_msg void OnThresholdBtn();
	afx_msg void OnUnWrappingBtn();
	afx_msg void OnMaskWndBtn();
	afx_msg void OnImage32bitBtn();
	afx_msg void OnImageHSVBtn();
	afx_msg void OnModifySaturationBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDEBUGWND_H__670E61F5_D292_4A98_B411_62DD12003F00__INCLUDED_)
