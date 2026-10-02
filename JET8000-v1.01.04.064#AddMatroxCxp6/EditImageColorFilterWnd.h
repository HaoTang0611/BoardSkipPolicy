#if !defined(AFX_EDITIMAGECOLORFILTERWND_H__988232CC_A0D6_4A84_8369_661A1C3C21D0__INCLUDED_)
#define AFX_EDITIMAGECOLORFILTERWND_H__988232CC_A0D6_4A84_8369_661A1C3C21D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageColorFilterWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
#include "ColorRGBVWnd.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_09     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_09     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageColorFilterWnd dialog
//-------------------------------------------------------------------------------------//
class CEditImageColorFilterWnd : public CBasicDialog
{
// Construction
public:
	CEditImageColorFilterWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImageColorFilterWnd)
	enum { IDD = IDD_EDIT_IMAGE_COLOR_FILTER_WND };	
	CThisListCtrl_09	m_FilterListWnd;
	CSpinButtonCtrl	m_ValueMinSpin;
	CSpinButtonCtrl	m_ValueMaxSpin;
	CSpinButtonCtrl	m_ColorMinSpin;
	CSpinButtonCtrl	m_ColorMaxSpin;
	CColorRGBVWnd	m_RGBVWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageColorFilterWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	void                       SetColorFilterParam(CAOIWnd *WndPtr, CAlgBinaryParam *ParamPtr, WND_DEFECT_ID WndDefectID, bool UpdateToUI);
	void                       BuildFilterListWnd();
	void                       UpdateFilterListWnd();	
	bool                       UpdateGatherColorCheckButton();
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	COLOR_RGBV_MODE            m_RGBVMode;	
	//---------------------------------------------------------------------------------//	
	CAOIWnd*                   m_WndPtr;
	unsigned int               m_RGBVIndex;
	WND_DEFECT_ID              m_WndDefectID;
	CAlgBinaryParam*           m_ColorFilterParamPtr;	
	BOOL                       m_StopFilterListBeSelected;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       ChangeDrawModelMode();
	DRAW_MODEL_MODE            GetDrawModelMode() const;	
	//---------------------------------------------------------------------------------//	
	void                       ClearFilterListWnd();
	void                       BuildFilterListWndHeader();	
	//---------------------------------------------------------------------------------//		
	bool                       ExecLinkProjectColorGroup(bool bShowDisable, size_t Begin, size_t End);	
	bool                       ExecSendProjectColorGroup(size_t Begin, size_t End);	
	//---------------------------------------------------------------------------------//		
	bool                       ExecLinkProjectColorGroupList(bool bShowDisable, size_t Begin, size_t End);
	bool                       ExecSendProjectColorGroupList(size_t Begin, size_t End);	
	//---------------------------------------------------------------------------------//		
	bool                       ExecLinkProjectColorGroupCombox(bool bShowDisable, size_t Begin, size_t End);	
	bool                       ExecSendProjectColorGroupCombox(size_t Begin, size_t End);
	//---------------------------------------------------------------------------------//		
	bool                       CheckLinkProjectColorGroup(WND_DEFECT_ID WndDefectID, size_t &Begin, size_t &End);
	//---------------------------------------------------------------------------------//
	bool                       CheckColorFilterParamPtr();
	void                       UpdateRGBVToUI(CColorRGBV *rgbvPtr);		
	void                       UpdateRGBVToParam(CColorRGBV *rgbvPtr);	
	bool                       UpdateColorModeToUI(CColorRGBV *rgbvPtr);
	//---------------------------------------------------------------------------------//
	void                       UpdateColorParamToParentWnd(CColorRGBV *rgbvPtr);
	void                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	void                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	void                       ExecRedMasterChk();
	void                       ExecGreenMasterChk();
	void                       ExecBlueMasterChk();
	void                       SwitchRGBMasterMode();
	//---------------------------------------------------------------------------------//	
	bool                       TurnOffGatherColor();
	bool                       TurnOnGatherColor(int nItem);
	bool                       ExecGatherColorChk(int nItem);		
	//---------------------------------------------------------------------------------//
	bool                       UpdateWndColor();
	bool                       ExecShowWndColor();
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveLogModelWndOperateColorFilterCheckBack();
	bool                       ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR Content);
	bool                       ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, int nOld, int nNew);
	bool                       ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, bool bOld, bool bNew);
	bool                       ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, COLOR_RGBV_MODE RGBV_Mode, LPCTSTR sKey, LPCTSTR bOld, LPCTSTR bNew);	
	bool                       ExecSaveLogModelWndOperateColorFilter(CColorRGBV *rgbvPtr, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImageColorFilterWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRedMasterChk();
	afx_msg void OnGreenMasterChk();
	afx_msg void OnBlueMasterChk();
	afx_msg void OnRedEnabledChk();
	afx_msg void OnGreenEnabledChk();
	afx_msg void OnBlueEnabledChk();
	afx_msg void OnDeltaposColorMaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposColorMinSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposValueMinSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposValueMaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnValueEnabledChk();
	afx_msg void OnFilterExpandBtn();
	afx_msg void OnFilterShirnkBtn();
	afx_msg void OnGatherColorChk();
	afx_msg void OnResetColorBtn();
	afx_msg void OnResetAllBtn();
	afx_msg void OnMergeAllBtn();
	afx_msg void OnColorMaxBtn();
	afx_msg void OnColorMinBtn();
	afx_msg void OnValueMinBtn();
	afx_msg void OnValueMaxBtn();	
	afx_msg void OnColorGroupBtnLink();
	afx_msg void OnLinkColorDisable();
	afx_msg void OnLinkColorPad();
	afx_msg void OnLinkColorVoid();
	afx_msg void OnLinkColorBody();
	afx_msg void OnLinkColorBoard();
	afx_msg void OnLinkColorSolder();
	afx_msg void OnLinkColorOthers();
	afx_msg void OnColorGroupBtnSend();
	afx_msg void OnSendColorPad();
	afx_msg void OnSendColorVoid();
	afx_msg void OnSendColorBody();
	afx_msg void OnSendColorBoard();
	afx_msg void OnSendColorSolder();
	afx_msg void OnSendColorOthers();		
	afx_msg void OnGrayColorBtn();
	afx_msg void OnShowGroupColorBtn();
	afx_msg void OnGatherShowRawChk();	
	afx_msg void OnExtractWndColorBtn();
	afx_msg void OnShowWndColorBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGECOLORFILTERWND_H__988232CC_A0D6_4A84_8369_661A1C3C21D0__INCLUDED_)
