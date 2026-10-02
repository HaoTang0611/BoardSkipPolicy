#if !defined(AFX_PROJECTMAPMASKWND_H__C27EE0F2_CE44_4EB0_807A_60A5CF41210B__INCLUDED_)
#define AFX_PROJECTMAPMASKWND_H__C27EE0F2_CE44_4EB0_807A_60A5CF41210B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectMapMaskWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ColorGroup.h"
#include "JETListCtrl.h"
#include "ColorRGBVWnd.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_21     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapMaskWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectMapMaskWnd : public CBaseDialog
{
// Construction
public:
	CProjectMapMaskWnd(CWnd* pParent = NULL);   // standard constructor
	CProjectMapMaskWnd(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	~CProjectMapMaskWnd();
// Dialog Data
	//{{AFX_DATA(CProjectMapMaskWnd)
	enum { IDD = IDD_PROJECT_MAP_MASK_WND };
	CStatic	m_ImageWnd;
	CComboBox m_FrameIndexComobx;
	CComboBox m_MaskIndexComobx;
	CThisListCtrl_21 m_ColorListCtrl;
	CSpinButtonCtrl	m_ValueMinSpin;
	CSpinButtonCtrl	m_ValueMaxSpin;
	CSpinButtonCtrl	m_ColorMinSpin;
	CSpinButtonCtrl	m_ColorMaxSpin;
	CColorRGBVWnd	m_RGBVWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectMapMaskWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                      SetProjectPtr(CAOIProject *Ptr);	
	IMAGE_PTR                 GetMaskImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	UINT                       m_CtrlMode;
	CColorGroup                m_ColorGroup;
	//---------------------------------------------------------------------------------//	
	COLOR_RGBV_MODE            m_RGBVMode;		
	int                        m_ColorRGBVIndex;
	bool                       m_ShowColorGroup;
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	double                     m_ImageZoom;
	TPOINT2D                   m_ImageOffset;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	//---------------------------------------------------------------------------------//
	UINT                       m_ShowMode;
	bool                       m_ShowRgnOuter;	
	TPOINT2D                   m_ImagePt1;
	TPOINT2D                   m_ImagePt2;
	TREGION4D                  m_ImageRgnRoi;
	TREGION4D                  m_ImageRgnOuter;
	TREGION4D                  m_ImageRgnInner;
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;	
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;		
	IMAGE_PTR                  m_ShowImagePtr;	
	TPOINT2D                   m_FrameResolution;//影像解析度		
	TREGION4D                  m_FrameStageRgn;	
	//-------------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_MapMaskImageW;
	IMAGE_SIZE                 m_MapMaskImageH;
	IMAGE_SIZE                 m_MapMaskImageStep;
	IMAGE_SIZE                 m_MapMaskBitCount;	
	IMAGE_PTR                  m_MapMaskImagePtr;
	IMAGE_PTR                  m_MapMaskImagePtr0;
	IMAGE_PTR                  m_MapMaskImagePtr1;
	IMAGE_PTR                  m_MapMaskImagePtr2;
	IMAGE_PTR                  m_MapMaskImagePtr3;
	IMAGE_PTR                  m_MapMaskImagePtr4;
	//-------------------------------------------------------------------------------------//	
	unsigned int               m_MapIndex;
	unsigned int               m_MaskIndex;
	IMAGE_SIZE                 m_ProjectMapW;
	IMAGE_SIZE                 m_ProjectMapH;
	//-------------------------------------------------------------------------------------//	
	POINT                      m_LBtnUpPt;
	POINT                      m_LBtnDownPt;
	POINT                      m_RBtnUpPt;
	POINT                      m_RBtnDownPt;
	POINT                      m_LastPt;
	POINT                      m_CurrentPt;
	//-------------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	bool                       GetUseRoiRectChk();//取得是否使用局部區域	
	//---------------------------------------------------------------------------------//
	bool                       BuildFrameIndexCombox();
	bool                       BuildMaskIndexCombox();
	//---------------------------------------------------------------------------------//
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
	void                       SetColorRGBVIndex(int val);
	int                        GetColorRGBVIndex() const;
	//---------------------------------------------------------------------------------//
	bool                       CheckUseColorFilterMode();
	bool                       ClearColorListWnd();
	bool                       BuildColorListWnd();
	bool                       UpdateColorListWnd();
	bool                       BuildColorListWndHeader();
	bool                       m_StopColorListBeSelected;	
	//---------------------------------------------------------------------------------//			
	void                       ClearMaskImageBuffer();
	bool                       CreateMaskImageBuffer();
	bool                       CreateMaskImageBuffer_Multi();
	//---------------------------------------------------------------------------------//	
	void                       ClearShowImageBuffer();
	bool                       CreateShowImageBuffer();	
	//---------------------------------------------------------------------------------//	
	virtual bool               BuildShowImage();
	bool                       DrawShowImage();
	void                       SwitchShowImage();	
	virtual void               RedrewWnd();
	void                       DrawRect(HDC hDC, const RECT &Rect);
	void                       DrawCircle(HDC hDC, const RECT &Rect);
	void                       DrawRectLine(HDC hDC, const POINT pt[], size_t num);
	void                       DrawImageRgn(HDC hDC);
	void                       DrawImageRoi(HDC hDC);
	void                       DrawComponent(HDC hDC);	
	void                       DrawCursorLine(HDC hDC);	
	//---------------------------------------------------------------------------------//	
	void                       ResetMaskRect();
	bool                       GetImageRect(RECT &Rect);
	bool                       ModifyMaskImageRect(IMAGE_DATA mask);
	bool                       ModifyMaskImageCircle(IMAGE_DATA mask);
	bool                       ModifyMaskImageByColorFilter(IMAGE_DATA mask);
	//---------------------------------------------------------------------------------//	
	bool                       ModifyMaskImageErode(int KenSize);
	bool                       ModifyMaskImageDilate(int KenSize);
	bool                       ModifyMaskImageOpen(int KenSize);
	bool                       ModifyMaskImageClose(int KenSize);
	bool                       ModifyMaskImageGradient(int KenSize);
	//---------------------------------------------------------------------------------//	
	void                       UpdateEditValue();
	void                       SwitchRGBMasterMode();
	void                       UpdateRGBVToUI(CColorRGBV *rgbvPtr);	
	void                       UpdateRGBVToParam(CColorRGBV *rgbvPtr);	
	virtual void               BuildColorFilterImage(CColorRGBV *rgbvPtr);
	void                       CalcColorFilterParam();//計算抽色參數
	//---------------------------------------------------------------------------------//
	bool                       CreatePartMask(IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, IMAGE_PTR &MaskPtr);
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	//---------------------------------------------------------------------------------//	
	// Generated message map functions
	//{{AFX_MSG(CProjectMapMaskWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnSelchangeFrameIndexCombo();
	afx_msg void OnSelchangeMaskIndexCombo();
	afx_msg void OnMaskAddBtn();
	afx_msg void OnMaskEraseBtn();
	afx_msg void OnMaskClearBtn();	
	afx_msg void OnShowMaskRadio();
	afx_msg void OnShowImageRadio();
	afx_msg void OnShowCombinedRadio();
	afx_msg void OnShowComponentChk();
	afx_msg void OnItemchangedColorListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRedMasterChk();
	afx_msg void OnGreenMasterChk();
	afx_msg void OnBlueMasterChk();
	afx_msg void OnRedEnabledChk();
	afx_msg void OnGreenEnabledChk();
	afx_msg void OnBlueEnabledChk();
	afx_msg void OnValueEnabledChk();
	afx_msg void OnDeltaposColorMaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposColorMinSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposValueMaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposValueMinSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilterExpandBtn();
	afx_msg void OnFilterShirnkBtn();
	afx_msg void OnColorMaxBtn();
	afx_msg void OnColorMinBtn();
	afx_msg void OnValueMaxBtn();
	afx_msg void OnValueMinBtn();
	afx_msg void OnResetColorBtn();
	afx_msg void OnResetAllBtn();
	afx_msg void OnMergeAllBtn();
	afx_msg void OnCtrlShpaeRectRadio();
	afx_msg void OnCtrlShpaeCircleRadio();
	afx_msg void OnCtrlColorFilterRadio();
	afx_msg void OnGrayColorBtn();
	afx_msg void OnShowGroupColorBtn();
	afx_msg void OnGatherShowRawChk();
	afx_msg void OnMaskErodeBtn();
	afx_msg void OnMaskDilateBtn();
	afx_msg void OnMaskOpenBtn();
	afx_msg void OnMaskCloseBtn();
	afx_msg void OnMaskGradBtn();
	afx_msg void OnMaskMergeBtn();
	afx_msg void OnMaskErasePartBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTMAPMASKWND_H__C27EE0F2_CE44_4EB0_807A_60A5CF41210B__INCLUDED_)
