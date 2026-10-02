#if !defined(AFX_PROJECTCOLORWND_H__9ED85FBC_D8D5_4C0F_A718_D8C79E02E839__INCLUDED_)
#define AFX_PROJECTCOLORWND_H__9ED85FBC_D8D5_4C0F_A718_D8C79E02E839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectColorWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JETListCtrl.h"
#include "ColorRGBVWnd.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_17     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_17     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectColorWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectColorWnd : public CBaseDialog
{
// Construction
public:
	CProjectColorWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectColorWnd)
	enum { IDD = IDD_PROJECT_COLOR_WND };
	CImageWnd   m_ImageWnd;
	CThisListCtrl_17 m_ColorGroupListCtrl;
	CThisListCtrl_17 m_ColorListCtrl;
	CSpinButtonCtrl	m_ValueMinSpin;
	CSpinButtonCtrl	m_ValueMaxSpin;
	CSpinButtonCtrl	m_ColorMinSpin;
	CSpinButtonCtrl	m_ColorMaxSpin;
	CColorRGBVWnd	m_RGBVWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectColorWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                      SetProjectPtr(CAOIProject *Ptr);	
	void                      GetColorGroupList(std::vector<CColorGroup> &ColorGroupList);
	//---------------------------------------------------------------------------------//	
protected:	
	//---------------------------------------------------------------------------------//	
	COLOR_RGBV_MODE            m_RGBVMode;			
	UINT                       m_ColorGroupCtrlID;
	unsigned int               m_SystemColorGroupSetIndex;
	//---------------------------------------------------------------------------------//	
	int                        m_ColorRGBVIndex;
	CAOIProject               *m_ProjectPtr;
	CColorGroup                m_ColorGroup;
	CProjectMapWnd             m_ProjectMapWnd;
	bool                       m_ShowColorGroup;
	std::vector<CColorGroup>   m_ColorGroupList;	
	//---------------------------------------------------------------------------------//
	unsigned int               m_ImageIndex;
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;	
	IMAGE_PTR                  m_RawImagePtr;
	IMAGE_PTR                  m_ShowImagePtr;	
	TPOINT2D                   m_FrameResolution;//影像解析度		
	TREGION4D                  m_FrameStageRgn;
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	//-------------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       SetColorRGBVIndex(int val);
	int                        GetColorRGBVIndex() const;
	//---------------------------------------------------------------------------------//	
	bool                       ClearColorGroupListWnd();
	bool                       BuildColorGroupListWnd();	
	bool                       BuildColorGroupListWndHeader();
	bool                       CheckColorGroupVisible(size_t idx);	
	bool                       m_StopColorGroupListBeSelected;
	//---------------------------------------------------------------------------------//
	bool                       CheckColorGroupValid();//確認目前顏色群組有效
	bool                       ClearColorListWnd();
	bool                       BuildColorListWnd();
	bool                       UpdateColorListWnd();
	bool                       BuildColorListWndHeader();
	bool                       m_StopColorListBeSelected;	
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       CreateBKImage();	
	//---------------------------------------------------------------------------------//	
	void                       UpdateEditValue();
	void                       SwitchRGBMasterMode();
	void                       UpdateRGBVToUI(CColorRGBV *rgbvPtr);	
	void                       UpdateRGBVToParam(CColorRGBV *rgbvPtr);	
	void                       BuildColorFilterImage(CColorRGBV *rgbvPtr);
	void                       CalcColorFilterParam();//計算抽色參數
	//---------------------------------------------------------------------------------//
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	unsigned int               GetMaxFrameCount() const;
	void                       PreInitUniFrameBuffer();//預先影像記憶體
	void                       ReleaseUniFrameBuffer();//釋放影像記憶體	
	void                       BuildShowImageBuffer(bool ResetView);//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體
	void                       SwitchFrameImage();
	bool                       FillCurrentFrames(double Ratio);	
	//-------------------------------------------------------------------------------------//
	bool                       ExecMoveToStage();
	bool                       ExecShowWndPosition();
	bool                       ExecGrabFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov(double PosX, double PosY, double PosZ);
	//---------------------------------------------------------------------------------//
	void                       UpdateColorGroupToList(CColorGroup &ColorGroup);
	//---------------------------------------------------------------------------------//
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadProjectColor(UINT GroupBegin, UINT GroupEnd);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectColorWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnItemchangedColorGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnGatherColorChk();
	afx_msg void OnShowMapBtn();	
	afx_msg void OnColorGroupBtnPad();
	afx_msg void OnColorGroupBtnVoid();
	afx_msg void OnColorGroupBtnBody();
	afx_msg void OnColorGroupBtnBoard();
	afx_msg void OnColorGroupBtnSolder();
	afx_msg void OnColorGroupBtnOthers();
	afx_msg void OnClickColorGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickColorListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSendSystemColorBtn();
	afx_msg void OnResetColorGroupListBtn();
	afx_msg void OnLoadSystemColorBtn();
	afx_msg void OnSetColorGroupFrameBtn();
	afx_msg void OnColorProjectBtnPad();
	afx_msg void OnColorProjectBtnVoid();
	afx_msg void OnColorProjectBtnBody();
	afx_msg void OnColorProjectBtnBoard();
	afx_msg void OnColorProjectBtnSolder();
	afx_msg void OnColorProjectBtnOthers();
	afx_msg void OnColorProjectBtnAll();
	afx_msg void OnGrayColorBtn();
	afx_msg void OnShowGroupColorBtn();
	afx_msg void OnGatherShowRawChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PROJECTCOLORWND_H__9ED85FBC_D8D5_4C0F_A718_D8C79E02E839__INCLUDED_)
