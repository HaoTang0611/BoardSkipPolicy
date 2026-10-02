#if !defined(AFX_NEWPROJECTREGIONIMAGEPANE_H__D8BA80B9_740C_49BE_ADD0_FF78A2D8F7EA__INCLUDED_)
#define AFX_NEWPROJECTREGIONIMAGEPANE_H__D8BA80B9_740C_49BE_ADD0_FF78A2D8F7EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectRegionImagePane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ImageWnd.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneRegionImage dialog

class CNewProjectPaneRegionImage : public CDialog
{
// Construction
public:
	CNewProjectPaneRegionImage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneRegionImage)
	enum { IDD = IDD_NEW_PROJECT_PANE_REGION_IMAGE };
	CComboBox	m_DlpLEDColorCombox;
	CComboBox	m_HeightRatioCombox;
	CComboBox	m_MapScaleCombox;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneRegionImage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	bool                       ExecNextPane();
	bool                       ExecPrevPane();
	bool                       ExecFinishPane();
	bool                       ReInitialPane();
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *ProjectPtr);
	void                       SetShowBuffer(size_t BufferSize, IMAGE_PTR Ptr);
	void                       SetImageBuffer(size_t BufferSize, IMAGE_PTR ImagePtr);
	//---------------------------------------------------------------------------------//	
	DISTRICT_ID			       GetDistrictID() const;
	void                       SetDistrictID(DISTRICT_ID Mode);
	//---------------------------------------------------------------------------------//	
	NEW_PROJECT_MODE           GetNewProjectMode() const;
	void                       SetNewProjectMode(NEW_PROJECT_MODE Mode);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableMultiDistrictMode() const;
	void                       SetEnableMultiDistrictMode(bool Mode);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;	
	DISTRICT_ID                m_DistrictID;
	CString                    m_SizeGroupText;
	NEW_PROJECT_MODE           m_NewProjectMode;		
	bool                       m_EnableMultiDistrictMode;//多段模式	
	//---------------------------------------------------------------------------------//	
	bool                       m_Finish;
	bool                       m_Finish_DA;
	bool                       m_Finish_DB;
	BOOL                       m_LiveGrab;
	FRAME_TYPE                 m_FrameType;
	unsigned int               m_FrameUniqueID;//取像畫面的唯一碼
	//---------------------------------------------------------------------------------//
	bool                       m_ResetView;
	double                     m_FovStageX;
	double                     m_FovStageY;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_PTR                  m_ImageBuffer;
	size_t                     m_ImageBufferSize;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowBitCount;	
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_PTR                  m_ShowBuffer;
	size_t                     m_ShowBufferSize;	
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_MapResolution_DA;
	TREGION4D                  m_MapStageRgn_DA;////專案檢測底圖範圍
	IMAGE_SIZE                 m_MapW_DA[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_MapH_DA[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_BitCount_DA[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 m_MapStep_DA[FRAME_MAX_COUNT];
	IMAGE_PTR                  m_MapBuffer_DA[FRAME_MAX_COUNT];
	size_t                     m_MapSize_DA[FRAME_MAX_COUNT];	
	//---------------------------------------------------------------------------------//
	TPOINT2D                   m_MapResolution_DB;
	TREGION4D                  m_MapStageRgn_DB;////專案檢測底圖範圍
	IMAGE_SIZE                 m_MapW_DB[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_MapH_DB[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_BitCount_DB[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 m_MapStep_DB[FRAME_MAX_COUNT];
	IMAGE_PTR                  m_MapBuffer_DB[FRAME_MAX_COUNT];
	size_t                     m_MapSize_DB[FRAME_MAX_COUNT];	
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//
	//RECT                       m_ImageWndRect;	
	//CJetMemDC                  m_ImageWndMemDC;	
	//---------------------------------------------------------------------------------//	
	double                     m_RegionPosXA;
	double                     m_RegionPosYA;
	double                     m_RegionPosZA;
	double                     m_RegionPosXB;
	double                     m_RegionPosYB;
	double                     m_RegionPosZB;
	double                     m_RegionWidth;
	double                     m_RegionLength;
	//---------------------------------------------------------------------------------//		
	double                     m_RegionPosXA_DB;
	double                     m_RegionPosYA_DB;
	double                     m_RegionPosZA_DB;
	double                     m_RegionPosXB_DB;
	double                     m_RegionPosYB_DB;
	double                     m_RegionPosZB_DB;
	double                     m_RegionWidth_DB;
	double                     m_RegionLength_DB;
	//---------------------------------------------------------------------------------//		
	void                       RedrawWnd();
	BOOL                       CreateBKDC(bool bResetView);//建立背景DC	
	BOOL                       ExecGrabImage();	
	BOOL                       DrawCtrlWnd(WPARAM wParam, LPARAM lParam);//在控制像繪圖後重新繪圖
	bool                       UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);//取得相機影像	
	bool                       RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);//取得相機影像	
	bool                       LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage);
	BOOL                       RedrawProjectImageWnd();//重繪專案影像
	//---------------------------------------------------------------------------------//
	bool                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	bool                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	//---------------------------------------------------------------------------------//	
	bool                       CheckFinish();
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//	
	BOOL                       UpdateParamToUI();//將參數更新至介面
	BOOL                       UpdateUIToParam();//將介面更新至參數
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       ExecFinishProjectMap();
	bool                       ExecLoadOfflineProgram(LPCTSTR filename);
	bool                       ChangeDistrictID(DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	void                       ClearMapBufferSet(DISTRICT_ID DistrictID);
	void                       ClearMapBuffer(size_t index, DISTRICT_ID DistrictID);
	bool                       SetMapBuffer(size_t index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	bool                       ExecCombinMapBuffer();//合併兩張底圖
	//---------------------------------------------------------------------------------//
	bool                       UpdateProjectLandWidth();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneRegionImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetPosBtnA();
	afx_msg void OnSetPosBtnB();
	afx_msg void OnMoveToPosBtnA();
	afx_msg void OnMoveToPosBtnB();
	afx_msg void OnMoveToSizeCornerBtn();
	afx_msg void OnMoveToStopBarBtn();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnGrabRegionImageBtn();
	afx_msg void OnResetSystemBtn();
	afx_msg void OnGrabLiveChk();
	afx_msg void OnLoadOfflineBtn();
	afx_msg void OnPCBInBtn();
	afx_msg void OnPCBOutBtn();
	afx_msg void OnPCBBackBtn();
	afx_msg void OnPCBClampOnBtn();	
	afx_msg void OnShowRulerBtn();
	afx_msg void OnSelchangeMapScaleCombo();
	afx_msg void OnSelchangeDlpLedColorCombo();
	afx_msg void OnPCBIn2ndBtn();
	afx_msg void OnPCBIn3rdBtn();
	afx_msg void OnLaneAdjustWidthBtn();
	afx_msg void OnDistrictABtn();
	afx_msg void OnDistrictBBtn();
	afx_msg void OnCombineDistrictBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTREGIONIMAGEPANE_H__D8BA80B9_740C_49BE_ADD0_FF78A2D8F7EA__INCLUDED_)
