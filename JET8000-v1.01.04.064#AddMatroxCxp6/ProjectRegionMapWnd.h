#if !defined(AFX_PROJECTREGIONMAPWND_H__79C27D97_A42C_4E20_A5A6_FEBADE90653D__INCLUDED_)
#define AFX_PROJECTREGIONMAPWND_H__79C27D97_A42C_4E20_A5A6_FEBADE90653D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectRegionMapWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ImageWnd.h"
#include "AOIProject.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectRegionMapWnd dialog

class CProjectRegionMapWnd : public CBaseDialog
{
// Construction
public:
	CProjectRegionMapWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectRegionMapWnd)
	enum { IDD = IDD_PROJECT_REGION_MAP_WND };
	CComboBox	m_MapScaleCombox;
	CComboBox   m_HeightRatioCombox;
	CComboBox   m_DlpLEDColorCombox;	
	CComboBox	m_FrameCombox08;
	CComboBox	m_FrameCombox07;
	CComboBox	m_FrameCombox06;
	CComboBox	m_FrameCombox05;
	CComboBox	m_FrameCombox04;
	CComboBox	m_FrameCombox03;
	CComboBox	m_FrameCombox02;
	CComboBox	m_FrameCombox01;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectRegionMapWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	//---------------------------------------------------------------------------------//	
	void                       SetProjectPtr(CAOIProject* Ptr);
	//---------------------------------------------------------------------------------//	
	DISTRICT_ID                GetDistrictID() const;
	void                       SetDistrictID(DISTRICT_ID val);	
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject *              m_ProjectPtr;
	DISTRICT_ID                m_DistrictID;
	CProjectMapWnd             m_ProjectMapWnd;
	bool                       m_EnableMultiDistrictMode;
	//---------------------------------------------------------------------------------//	
	bool                       m_Finish;
	bool                       m_Finish_DA;
	bool                       m_Finish_DB;
	BOOL                       m_LiveGrab;
	FRAME_TYPE                 m_FrameType;
	unsigned int               m_FrameUniqueID;//取像畫面的唯一碼
	std::vector<unsigned int>  m_ProjectFrameUniqueIDList;
	//---------------------------------------------------------------------------------//
	bool                       m_ResetView;
	double                     m_FovStageX;
	double                     m_FovStageY;
	bool                       m_BackupJogMode;
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
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//		
	bool                       InitProjectFrameCombox();
	bool                       BuildProjectFrameCombox();
	bool                       CloaseAllProjectFrameCombox();
	bool                       BuildFrameUniqueIDList(std::vector<unsigned int> &FrameUniqueIDList);
	bool                       AddFrameUniqueIDList(CComboBox &Combox, std::vector<unsigned int> &FrameUniqueIDList, bool b3DMode);
	//---------------------------------------------------------------------------------//	
	bool                       UpdateParamToUI();//將參數更新至介面
	bool                       UpdateUIToParam();//將介面更新至參數
	//---------------------------------------------------------------------------------//	
	bool                       CreateImageBuffer();
	bool                       ReleaseImageBuffer();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	BOOL                       CreateBKDC(bool bResetView);//建立背景DC	
	bool                       ExecGrabImage();
	//---------------------------------------------------------------------------------//
	bool                       CheckFinish();
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	bool                       ExecFinish(LPARAM lParam);
	bool                       ExecFinishProjectMap();
	bool                       ExecLoadOfflineProgram(LPCTSTR filename);
	bool                       ChangeDistrictID(DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	void                       SwitchFrameImage();
	bool                       UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);//取得相機影像	
	bool                       RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);//取得相機影像	
	bool                       LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage);
	//---------------------------------------------------------------------------------//
	bool                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	void                       ClearMapBufferSet(DISTRICT_ID DistrictID);
	void                       ClearMapBuffer(size_t index, DISTRICT_ID DistrictID);
	bool                       SetMapBuffer(size_t index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	bool                       ExecAlignFd();//對齊定位點
	bool                       ExecAlignFd_Finish();
	bool                       ExecCombinMapBuffer();//合併兩張底圖	
	//---------------------------------------------------------------------------------//
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectRegionMapWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRegionMoveToPosBtnA();
	afx_msg void OnRegionMoveToPosBtnB();
	afx_msg void OnRegionSetPosBtnA();
	afx_msg void OnRegionSetPosBtnB();
	afx_msg void OnRegionMoveToSizeCornerBtn();
	afx_msg void OnRegionMoveToStopBarBtn();
	afx_msg void OnRegionGrabRegionImageBtn();
	afx_msg void OnRegionResetSystemBtn();
	afx_msg void OnRegionLoadOfflineBtn();
	afx_msg void OnRegionPCBInBtn();
	afx_msg void OnRegionPCBOutBtn();
	afx_msg void OnRegionPCBBackBtn();
	afx_msg void OnRegionPCBClampOnBtn();
	afx_msg void OnRegionViewProjectMapWnd();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRegionShowRulerBtn();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRegionFrameShowChk();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnRegionPCBIn2ndBtn();
	afx_msg void OnRegionPCBIn3rdBtn();
	afx_msg void OnRegionLaneAdjustWidthBtn();	
	afx_msg void OnRegionDistrictABtn();
	afx_msg void OnRegionDistrictBBtn();
	afx_msg void OnRegionCombineDistrictBtn();
	afx_msg void OnRegionAlignFdBtn();
	afx_msg void OnRegionFrameDefaultBtn();
	afx_msg void OnRegionFrameCloseAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTREGIONMAPWND_H__79C27D97_A42C_4E20_A5A6_FEBADE90653D__INCLUDED_)
