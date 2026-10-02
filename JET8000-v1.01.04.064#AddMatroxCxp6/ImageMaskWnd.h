#if !defined(AFX_IMAGEMASKWND_H__803A27A6_CF59_4259_8814_A33192F7B8A0__INCLUDED_)
#define AFX_IMAGEMASKWND_H__803A27A6_CF59_4259_8814_A33192F7B8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageMaskWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "dib.h"
#include "ImageWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CImageMaskWnd dialog
//-------------------------------------------------------------------------------------//
class CImageMaskWnd : public CBaseDialog
{
// Construction
public:
	CImageMaskWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageMaskWnd)
	enum { IDD = IDD_IMAGE_MASK_WND };
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMaskWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	bool                      ExecLoadMaskFile(LPCTSTR filename);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CDib                       m_Dib;	
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_ImageStep;
	unsigned char*             m_ImagePtr;
	void                       ReleaseImageBuffer();
	//---------------------------------------------------------------------------------//
	void                       AdjustCtrlWnd(int cx=-1, int cy=-1);
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	void                       UpdateShowImage();
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	//---------------------------------------------------------------------------------//	
	
	// Generated message map functions
	//{{AFX_MSG(CImageMaskWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLoadImageBtn();	
	afx_msg void OnShowMaskChk();
	afx_msg void OnShowBit01Chk();
	afx_msg void OnShowBit02Chk();
	afx_msg void OnShowBit03Chk();
	afx_msg void OnShowBit04Chk();
	afx_msg void OnShowBit05Chk();
	afx_msg void OnShowBit06Chk();
	afx_msg void OnShowBit07Chk();
	afx_msg void OnShowBit08Chk();
	afx_msg void OnDisableAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMASKWND_H__803A27A6_CF59_4259_8814_A33192F7B8A0__INCLUDED_)
