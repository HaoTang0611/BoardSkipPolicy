#if !defined(AFX_ALGPATTERNEDITWND_H__08C139E6_B8CD_4C05_B089_602178D4BD8E__INCLUDED_)
#define AFX_ALGPATTERNEDITWND_H__08C139E6_B8CD_4C05_B089_602178D4BD8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgPatternEditWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternEditWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgPatternEditWnd : public CBaseDialog
{
// Construction
public:
	CAlgPatternEditWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgPatternEditWnd)
	enum { IDD = IDD_ALG_PATTERN_EDIT_WND };
	CImageWnd	m_ImageWnd;
	CSpinButtonCtrl	m_AngleSpin;
	CSpinButtonCtrl	m_MoveSpinX;
	CSpinButtonCtrl	m_MoveSpinY;
	CSpinButtonCtrl	m_ShrinkSpinW;
	CSpinButtonCtrl	m_ShrinkSpinH;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgPatternEditWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetImageFilename(LPCTSTR filename, LPCTSTR filenameRet);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	RECT                       m_ClipImageRect;
	bool                       m_FirstLoadImage;
	CString                    m_ImageFilename;
	CString                    m_ImageFilenameResult;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	int                        m_MoveMax;//移動上限
	int                        m_MoveMin;//移動下限
	//---------------------------------------------------------------------------------//
	int                        m_AngleMax;//角度上限
	int                        m_AngleMin;//角度下限
	int                        m_AngleStepRatio;//角度步長比例
	//---------------------------------------------------------------------------------//
	int                        m_SrinkMaxW;//縮小上限
	int                        m_SrinkMinW;//縮小下限
	int                        m_SrinkMaxH;//縮小上限
	int                        m_SrinkMinH;//縮小下限
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW;//原始圖寬度
	IMAGE_SIZE                 m_ImageH;//原始圖長度
	IMAGE_SIZE                 m_ImageStep;//原始圖步長
	IMAGE_SIZE                 m_BitCount;//原始圖位元數
	IMAGE_PTR                  m_ImagePtr;//原始圖指標
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ShowW;//顯示圖寬度
	IMAGE_SIZE                 m_ShowH;//顯示圖長度
	IMAGE_SIZE                 m_ShowStep;//顯示圖步長
	IMAGE_SIZE                 m_ShowBitCnt;//顯示圖位元數
	IMAGE_PTR                  m_ShowPtr;//顯示圖指標
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_MovedW;//移動圖寬度
	IMAGE_SIZE                 m_MovedH;//移動圖長度
	IMAGE_SIZE                 m_MovedStep;//移動圖步長
	IMAGE_SIZE                 m_MovedBitCnt;//移動圖位元數
	IMAGE_PTR                  m_MovedPtr;//移動圖指標
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_RotatedW;//旋轉圖寬度
	IMAGE_SIZE                 m_RotatedH;//旋轉圖長度
	IMAGE_SIZE                 m_RotatedStep;//旋轉圖步長
	IMAGE_SIZE                 m_RotatedBitCnt;//旋轉圖位元數
	IMAGE_PTR                  m_RotatedPtr;//旋轉圖指標
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ClippedW;//裁切圖寬度
	IMAGE_SIZE                 m_ClippedH;//裁切圖長度
	IMAGE_SIZE                 m_ClippedStep;//裁切圖步長
	IMAGE_SIZE                 m_ClippedBitCnt;//裁切圖位元數
	IMAGE_PTR                  m_ClippedPtr;//裁切圖指標
	//---------------------------------------------------------------------------------//	
	void                       ReleaseShowBuffer();
	void                       ReleaseImageBuffer();
	void                       ReleaseMovedBuffer();
	void                       ReleaseRotatedBuffer();
	void                       ReleaseClippedBuffer();
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadImageFile(LPCTSTR filename);
	//---------------------------------------------------------------------------------//
	bool                       ExecMoveImageByOffsetEdit();
	bool                       ExecMoveImageByOffsetEditKernel();
	bool                       ExecMoveImage(int sX, int sY);
	//---------------------------------------------------------------------------------//
	bool                       ExecRotateImageByAngleEdit();
	bool                       ExecRotateImageByAngleEditKernel();	
	bool                       ExecRotateImage(double Angle);
	//---------------------------------------------------------------------------------//
	bool                       ExecShowImageByShrinkEdit();	
	bool                       ExecShowImageByShrinkEditKernel();	
	bool                       ExecShowImage(int W, int H);
	//---------------------------------------------------------------------------------//
	bool                       ExecEditImageByEditKernel();
	//---------------------------------------------------------------------------------//
	bool                       ExecClippedImage();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgPatternEditWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnKillfocusAngleEdit();
	afx_msg void OnDeltaposAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAngleBtn();
	virtual void OnOK();
	afx_msg void OnKillfocusShrinkEditW();
	afx_msg void OnKillfocusShrinkEditH();
	afx_msg void OnDeltaposShrinkSpinW(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposShrinkSpinH(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAngleStepRdo01();
	afx_msg void OnAngleStepRdo05();
	afx_msg void OnAngleStepRdo10();
	afx_msg void OnKillfocusMoveEditX();
	afx_msg void OnKillfocusMoveEditY();
	afx_msg void OnDeltaposMoveSpinX(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDeltaposMoveSpinY(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGPATTERNEDITWND_H__08C139E6_B8CD_4C05_B089_602178D4BD8E__INCLUDED_)
