// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#if !defined(AFX_MAINFRM_H__8CB37586_4B26_4517_884B_F7631BDE662C__INCLUDED_)
#define AFX_MAINFRM_H__8CB37586_4B26_4517_884B_F7631BDE662C__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//#ifdef _XTP_STATICLINK
//	typedef CXTPFrameWnd         FRAME_WND_BASE;
//	typedef CSplitterWnd         SPLITTER_WND_BASE;
//#else
//	typedef CFrameWnd            FRAME_WND_BASE;
//	typedef CSplitterWnd         SPLITTER_WND_BASE;

//	const UINT WM_XTP_DOCKINGPANE_BASE = (WM_USER + 9900);
//	#define XTPWM_DOCKINGPANE_NOTIFY  (WM_XTP_DOCKINGPANE_BASE + 1)	
//#endif
//-------------------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	#include "PropertiesWnd.h"
	#include "EditWndDockPane.h"	
    #include "EditResultDockPane.h"
	#include "EditLibraryDockPane.h"
	#include "EditImageDockPane.h"
	#include "EditProjectMapDockPane.h"	
	#include "EditComponentListDockPane.h"
	#include "EditModelListDockPane.h"
	#include "EditPartNumberListDockPane.h"	
#endif//FRAME_STYLE_TYPE
//-------------------------------------------------------------------------------------//
class CMainFrame : public CBasicFrame
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members	
	//---------------------------------------------------------------------------------//
	HACCEL                     m_hUserAccel;
	ACCEL                     *m_UserAccelList;
	size_t                     m_UserAccelCount;
	//---------------------------------------------------------------------------------//
	SIZE                       m_EditLibrarySize;
	SIZE                       m_EditProjectMapSize;
	CString                    m_DefaultDocTitle;	
	//---------------------------------------------------------------------------------//
	bool                       m_MainFrameWndLocked;//主視窗鎖住
	int                        m_RibbonPcbDirection;
	//---------------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE   == FRAME_STYLE_STUDIO	
	CMFCMenuBar                m_wndMenuBar;//選單
	CMFCToolBar                m_wndToolBar;//標準工具列	
	CMFCStatusBar              m_wndStatusBar;//標準狀態列	
	CPropertiesWnd             m_wndProperties;//屬性工具列
	CEditWndDockPane           m_wndEditWnd;//檢測框列表	
	CEditResultDockPane        m_wndEditResultList;//結果列表
	CEditLibraryDockPane       m_wndEditLibrary;//資料庫列表
	CEditImageDockPane         m_wndEditImage;//影像處理視窗
	CEditProjectMapDockPane    m_wndEditProjectMap;//專案底圖
	CEditComponentListDockPane m_wndEditMarkList;//特徵點列表
	CEditComponentListDockPane m_wndEditComponentList;//零件列表
	CEditModelListDockPane     m_wndEditModelList;//模組列表
	CEditPartNumberListDockPane m_wndPartNumberList;//料號列表
#elif FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE	
	CMFCRibbonBar              m_wndRibbonBar;//帶狀工具列	
	CMFCRibbonCategory        *m_CategoryPtrLast;
	//CMFCRibbonStatusBar        m_wndStatusBar;//帶狀狀態列	
	CMFCStatusBar              m_wndStatusBar;//標準狀態列
	CPropertiesWnd             m_wndProperties;//屬性工具列	
	CEditWndDockPane           m_wndEditWnd;//檢測框列表	
	CEditResultDockPane        m_wndEditResultList;//結果列表
	CEditLibraryDockPane       m_wndEditLibrary;//資料庫列表
	CEditImageDockPane         m_wndEditImage;//影像處理視窗
	CEditProjectMapDockPane    m_wndEditProjectMap;//專案底圖
	CEditComponentListDockPane m_wndEditMarkList;//特徵點列表
	CEditComponentListDockPane m_wndEditComponentList;//零件列表
	CEditModelListDockPane     m_wndEditModelList;//模組列表
	CEditPartNumberListDockPane m_wndPartNumberList;//料號列表
#else
	CToolBar                   m_wndToolBar;//工具列
	CStatusBar                 m_wndStatusBar;//狀態列		
#endif//FRAME_STYLE_TYPE
	//---------------------------------------------------------------------------------//
	bool                       GetMainFrameWndLocked() const;//取得是否鎖住主視窗
	void                       SetMainFrameWndLocked(bool val);//設定是否鎖住主視窗
	//---------------------------------------------------------------------------------//
	bool                       CreateUserAccelTable();//建立使用者快速建表單
	bool                       DestroyUserAccelTable();//刪除使用者快速建表單
	bool                       ExecUserAccelTable(MSG* pMsg);//執行使用者快速建表單
	//---------------------------------------------------------------------------------//
	bool                       CreateFrameWnd_MFC();//建立MFC傳統模式的介面
	bool                       CreateFrameWnd_Studio();//建立Studio模式的介面
	bool                       CreateFrameWnd_Office();//建立Office模式的介面		
	bool                       CreateDockingWnd();//建立駐列的視窗	
	bool                       PreShowPane(CBasePane* pBar, BOOL bShow, BOOL bDelay, BOOL bActivate, bool &Done);
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();	
	void                       SwitchMultiLanguageRibbonBar();//切換多國語系-Ribbon Bar
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	bool                       RemoveRibbonPanel(UINT nID, bool AllCategories);//設定-Ribbon Bar區域移除
	bool                       RemoveRibbonElement(UINT nID, bool AllCategories);//設定-Ribbon Bar按鈕移除
	bool                       UpdateRibbonElementText(UINT nID, LPCTSTR sText, bool AllCategories);//設定-Ribbon Bar文字	
	bool                       UpdateRibbonButtonImageIndex(UINT nID, int Index, bool LargeImage, bool AllCategories);//設定-Ribbon Bar按鈕圖示編號
	//---------------------------------------------------------------------------------//
	CBasicSplitterWnd         *m_pSplitterWnd;//分割視窗	
	//---------------------------------------------------------------------------------//
	bool                       CreateGlobalWnd();//建立全域的視窗
	bool                       DestroyGlobalWnd();//摧毀全域的視窗
	//---------------------------------------------------------------------------------//	
	void                       SwitchViewWnd(UINT ID);//切換主畫面視窗		
	void                       SwitchPane(BOOL bShow);
	bool                       SwitchDockingWnd(UINT ID);//切換駐列視窗	
	bool                       ShowGlobalWnd(CWnd *pWnd, BOOL bShow);//顯示全域視窗
	bool                       ResetMainViewWndAndGrabImage(bool ResetProjectLight, bool ReGrab);//重設回傳視窗並且取像
	//---------------------------------------------------------------------------------//
	bool                       InitialMainFrame();//執行應用框架的建立
	//---------------------------------------------------------------------------------//
	bool                       InitialApp();//軟體初始化
	bool                       ReleaseApp();//軟體釋放
	//---------------------------------------------------------------------------------//
	bool                       CheckMesCtrlState_Offline() const;
	bool                       CheckMesCtrlState_Local() const;
	bool                       CheckMesCtrlState_Remote() const;		
	bool                       CheckMesCtrlState(MES_EQP_CTRL_STATE_MODE Mode) const;
	//---------------------------------------------------------------------------------//
	void                       ExecUpdateUI_System(CCmdUI* pCmdUI);	
	void                       ExecUpdateUI_XYZCtrl(CCmdUI* pCmdUI);	
	BOOL                       CheckUpdateUI_LaneCtrl(); 
	void                       ExecUpdateUI_LaneCtrl(CCmdUI* pCmdUI);
	void                       ExecUpdateUI_ProjectParam(CCmdUI* pCmdUI);	
	void                       ExecUpdateUI_ProjectTuning(CCmdUI* pCmdUI);	
	//---------------------------------------------------------------------------------//
	bool                       ExecProjectNew();//執行新專案
	bool                       ExecProjectOpen();//執行開啟專案	
	bool                       ExecProjectOpen(unsigned int ProjectIndex);//執行開啟專案	
	bool                       ExecProjectClose();//執行關閉專案
	bool                       ExecProjectSave();//執行儲存專案
	bool                       ExecProjectSaveAs();//執行另存專案
	bool                       ExecProjectNewOffline();//執行新專案離線編程
	bool                       ExecProjectNewPanel();//執行新整板資料
	bool                       ExecProjectSaveSpcFile();//執行儲存Spc
	bool                       ExecProjectSaveCadXYFile();//執行儲存CAD-XY	
	bool                       ExecProjectSaveTestCoverageFile();//執行儲存檢測涵蓋率檔案
	bool                       ExecProjectSwitch(unsigned int TargetIndex);//執行切換專案
	bool                       ExecProjectSwitchDistrict(DISTRICT_ID DistrictID);//執行切換專案段落
	bool                       ExecUpdateProjectDistrictActive(CCmdUI* pCmdUI, DISTRICT_ID DistrictID);//執行切換專案段落
	bool                       ExecProjectCloseAll(bool bAsk);//執行關閉所有專案
	bool                       ExecProjectAutoLoad();//執行自動開啟專案
	bool                       ExecProjectLoad(CAOIProject *ProjectPtr, LPCTSTR filename);//執行開啟專案
	bool                       ExecProjectPreLoadOffline(CAOIProject *ProjectPtr);//執行預先載入專案離線編輯	
	bool                       ExecProjectLoadFromServer(CAOIProject *ProjectPtr);//執行載入伺服器專案
	bool                       ExecProjectSaveLibraryToServer(CAOIProject *ProjectPtr);//執行專案資料庫儲存至伺服器
	bool                       UpdateDocumentTitle();//更新主視窗的標題
	bool                       UpdateRibbonUIText(LPARAM lParam);//更新主視窗的Ribbon調機介面
	bool                       UpdateRibbonTuneGroupText();//更新主視窗的Ribbon調機介面			
	bool                       UpdateRibbonDefaultWndGroupText();//更新主視窗的Ribbon預設檢測框介面
	bool                       SetDocumentModifiedFlag(BOOL bFlag);
	bool                       ExecProjectSaveMappingFile();//執行儲存CAD-XY-Panasonic
	//---------------------------------------------------------------------------------//
	bool                       ExecHomeAll(bool AskXYZ, bool AskLane, bool &Done);//執行歸零
	bool                       ShowHomeAllFinish();
	//---------------------------------------------------------------------------------//
	bool                       ExecPCBIn();//執行進板
	bool                       ExecPCBIn2nd();//執行進板-2
	bool                       ExecPCBIn3rd();//執行進板-3
	bool                       ExecPCBOut();//執行出板
	bool                       ExecPCBBack();//執行退板
	bool                       ExecPCBBackOut();//執行退出板
	bool                       ExecPCBClear();//執行清板
	bool                       ExecPCBClampOn();//執行夾板
	bool                       ExecPCBClampOff();//執行鬆板	
	bool                       ExecLaneAdjustHome();//執行軌道間距歸零
	bool                       ExecLaneAdjustWidth();//執行軌道間距寬度
	//---------------------------------------------------------------------------------//
	bool                       ExecOnlineRun();//執行上線檢測
	bool                       ExecOnlineBypass();//執行上線直通
	//---------------------------------------------------------------------------------//
	bool                       ExecRibbonCategoryChanged(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	BOOL                       ExecRibbonBuildProjectCombox(LPARAM lParam);//執行RibbonBar訊息-建立專案列表
	BOOL                       ExecRibbonSelChangeProjectCombox(LPARAM lParam);//執行RibbonBar訊息-選取切換專案列表
	//---------------------------------------------------------------------------------//
	BOOL                       ExecRibbonBuildPanelCombox(LPARAM lParam);//執行RibbonBar訊息-建立整板列表
	BOOL                       ExecRibbonSelChangePanelCombox(LPARAM lParam);//執行RibbonBar訊息-選取切換整板列表
	BOOL                       ExecRibbonBuildBoardCombox(LPARAM lParam);//執行RibbonBar訊息-建立單板列表
	BOOL                       ExecRibbonSelChangeBoardCombox(LPARAM lParam);//執行RibbonBar訊息-選取切換單板列表
	BOOL                       ExecRibbonBuildComponentCombox(LPARAM lParam);//執行RibbonBar訊息-建立零件列表
	BOOL                       ExecRibbonSelChangeComponentCombox(LPARAM lParam);//執行RibbonBar訊息-選取切換零件列表
	BOOL                       ExecRibbonShowDebugCategory(LPARAM lParam);//執行RibbonBar訊息-顯示偵錯群組
	bool                       ExecRibbonUpdatePcbDirection();//執行RibbonBar訊息-PCB流向
	BOOL                       ExecRibbonBarMessage(UINT message, WPARAM wParam, LPARAM lParam);//執行RibbonBar訊息  	
	//---------------------------------------------------------------------------------//	
	bool                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd	
	//---------------------------------------------------------------------------------//
	bool                       ExecOnlineTuningLastFile();//線上調機-上一筆
	bool                       ExecOnlineTuningNextFile();//線上調機-下一筆
	bool                       ExecOnlineTuningListFile();//線上調機-顯示列表
	//---------------------------------------------------------------------------------//
	bool                       ExecHideWndForInspection();//隱藏視窗來檢測
	//---------------------------------------------------------------------------------//
	bool                       ExecViewSystemConfigWnd();
	bool                       ExecViewCalibrationWnd();
	bool                       ExecViewCudaCtrlWnd();
	bool                       ExecViewSocketClientWnd();
	bool                       ExecViewImageConfigWnd();
	bool                       ExecViewAllPhaseWnd();
	bool                       ExecViewUserRegisterWnd();
	bool                       ExecViewBarcodeDeviceWnd();
	bool                       ExecViewRemoteParamWnd();
	//---------------------------------------------------------------------------------//
	bool                       ExecFdConfirmWnd(LPARAM lParam);
	bool                       ExecBarcodeConfirmWnd(LPARAM lParam);
	bool                       ExecBarcodeHandHeldWnd(LPARAM lParam);
	bool                       ExecOpenProjectBarcodeWnd(LPARAM lParam);	
	bool                       ExecComponentBarcodeConfirmWnd(LPARAM lParam);
	bool                       ExecUserLoginWnd(LPARAM lParam);	
	bool                       ExecShowMessageWnd(LPARAM lParam);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecResetControlCenter();
	bool                       ExecSwitchActiveLaneID(LANE_ID LaneID);
	//---------------------------------------------------------------------------------//
	bool                       ExecSetFdSortWnd();
	bool                       ExecSetFdSortWndBoard();
	//---------------------------------------------------------------------------------//
	bool                       ExecViewProjectParamWnd();
	bool                       ExecViewProjectOpenCodeListWnd();
	bool                       ExecViewOperatorLogWnd();
	//---------------------------------------------------------------------------------//
	void                       ExecMesSetCtrlState(MES_EQP_CTRL_STATE_MODE Mode);
	void                       ExecUpdateMesSetCtrlState(MES_EQP_CTRL_STATE_MODE Mode, CCmdUI* pCmdUI);
	//---------------------------------------------------------------------------------//
	bool                       ExecMESComm_ShowMsg() const;
	bool                       ExecMESComm_SetSystemParam();
	bool                       ExecMESComm_SetProjectParam();
	bool                       ExecMESComm_SetUserLogin_out(bool bLogin);
	bool                       ExecMESComm_LoadProject();
	bool                       ExecMESComm_ShowMessageWnd(DWORD ShowType);
	//---------------------------------------------------------------------------------//
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();	
	afx_msg void OnViewPlcCtrlWnd();
	afx_msg void OnUpdateViewPlcCtrlWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewMotionCtrlWnd();
	afx_msg void OnUpdateViewMotionCtrlWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewEditModelView();
	afx_msg void OnViewProjectParamWnd();
	afx_msg void OnUpdateViewProjectParamWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewSystemConfigWnd();
	afx_msg void OnUpdateViewSystemConfigWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewSystemInitialWnd();
	afx_msg void OnUpdateViewSystemInitialWnd(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnViewCameraCtrlWnd();
	afx_msg void OnUpdateViewCameraCtrlWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewPhaseCtrlWnd();
	afx_msg void OnUpdateViewPhaseCtrlWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewCalibrationWnd();
	afx_msg void OnUpdateViewCalibrationWnd(CCmdUI* pCmdUI);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);	
	afx_msg void OnXYZHomeAll();
	afx_msg void OnUpdateXYZHomeAll(CCmdUI* pCmdUI);
	afx_msg void OnXYZGoToORG();
	afx_msg void OnUpdateXYZGoToORG(CCmdUI* pCmdUI);
	afx_msg void OnXYZGoToFocus();
	afx_msg void OnUpdateXYZGoToFocus(CCmdUI* pCmdUI);
	afx_msg void OnXYZGoToLeave();
	afx_msg void OnUpdateXYZGoToLeave(CCmdUI* pCmdUI);
	afx_msg void OnXYZCtrlGroup();
	afx_msg void OnUpdateXYZCtrlGroup(CCmdUI* pCmdUI);	
	afx_msg void OnLanePCBIn();	
	afx_msg void OnUpdateLanePCBIn(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBIn2nd();	
	afx_msg void OnUpdateLanePCBIn2nd(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBIn3rd();	
	afx_msg void OnUpdateLanePCBIn3rd(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBOut();
	afx_msg void OnUpdateLanePCBOut(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBBack();
	afx_msg void OnUpdateLanePCBBack(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBBackOut();
	afx_msg void OnUpdateLanePCBBackOut(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBClear();
	afx_msg void OnUpdateLanePCBClear(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBClampOn();
	afx_msg void OnUpdateLanePCBClampOn(CCmdUI* pCmdUI);
	afx_msg void OnLanePCBClampOff();
	afx_msg void OnUpdateLanePCBClampOff(CCmdUI* pCmdUI);	
	afx_msg void OnLaneCtrlGroup();
	afx_msg void OnUpdateLaneCtrlGroup(CCmdUI* pCmdUI);
	afx_msg void OnViewCudaCtrlWnd();
	afx_msg void OnUpdateViewCudaCtrlWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewImageConfigWnd();
	afx_msg void OnUpdateViewImageConfigWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewLightCtrlBoardWnd();
	afx_msg void OnUpdateViewLightCtrlBoardWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewRulerWnd();
	afx_msg void OnViewPropertiesWnd();
	afx_msg void OnViewEditlistWnd();
	afx_msg void OnUpdateViewEditlistWnd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPropertiesWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewModelListWnd();
	afx_msg void OnViewProjectMapWnd();		
	afx_msg LRESULT OnRibbonCategoryChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewEditMainView();	
	afx_msg void OnViewWndDockingWnd();
	afx_msg void OnUpdateViewWndDockingWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewImageDockingWnd();	
	afx_msg void OnProjectNew();
	afx_msg void OnUpdateProjectNew(CCmdUI* pCmdUI);
	afx_msg void OnProjectOpen();
	afx_msg void OnUpdateProjectOpen(CCmdUI* pCmdUI);
	afx_msg void OnProjectSave();
	afx_msg void OnUpdateProjectSave(CCmdUI* pCmdUI);
	afx_msg void OnProjectSaveAs();
	afx_msg void OnUpdateProjectSaveAs(CCmdUI* pCmdUI);	
	afx_msg void OnProjectNewOffline();
	afx_msg void OnUpdateProjectNewOffline(CCmdUI* pCmdUI);		
	afx_msg void OnProjectNewPanel();
	afx_msg void OnUpdateProjectNewPanel(CCmdUI* pCmdUI);		
	afx_msg void OnProjectSaveSpcFile();
	afx_msg void OnUpdateProjectSaveSpcFile(CCmdUI* pCmdUI);	
	afx_msg void OnProjectSaveCadXYFile();
	afx_msg void OnUpdateProjectSaveCadXYFile(CCmdUI* pCmdUI);	
	afx_msg void OnProjectSaveTestCoverageFile();
	afx_msg void OnUpdateProjectSaveTestCoverageFile(CCmdUI* pCmdUI);	
	afx_msg void OnProjectSaveTestMappingFile();
	afx_msg void OnUpdateProjectSaveTestMappingFile(CCmdUI* pCmdUI);
	afx_msg void OnViewResultlistWnd();
	afx_msg void OnUpdateViewResultlistWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewEditFdView();
	afx_msg void OnUpdateViewEditFdView(CCmdUI* pCmdUI);
	afx_msg void OnViewEditMarkView();
	afx_msg void OnUpdateViewEditMarkView(CCmdUI* pCmdUI);
	afx_msg void OnViewEditBarcodeView();
	afx_msg void OnUpdateViewEditBarcodeView(CCmdUI* pCmdUI);
	afx_msg void OnViewAllPhaseWnd();
	afx_msg void OnUpdateViewAllPhaseWnd(CCmdUI* pCmdUI);
	afx_msg void OnOnlineRun();
	afx_msg void OnUpdateOnlineRun(CCmdUI* pCmdUI);
	afx_msg void OnOnlineBypass();
	afx_msg void OnUpdateOnlineBypass(CCmdUI* pCmdUI);
	afx_msg void OnOnlineStop();
	afx_msg void OnUpdateOnlineStop(CCmdUI* pCmdUI);	
	afx_msg void OnSystemReset();
	afx_msg void OnUpdateSystemReset(CCmdUI* pCmdUI);
	afx_msg void OnSettingOfflineMode();
	afx_msg void OnUpdateSettingOfflineMode(CCmdUI* pCmdUI);
	afx_msg void OnSettingSaveFovImages();
	afx_msg void OnUpdateSettingSaveFovImages(CCmdUI* pCmdUI);
	afx_msg void OnSettingOfflineInspectionMode();
	afx_msg void OnUpdateSettingOfflineInspectionMode(CCmdUI* pCmdUI);
	afx_msg void OnRebuildInspectionField();
	afx_msg void OnUpdateRebuildInspectionField(CCmdUI* pCmdUI);
	afx_msg void OnProjectGroupConfigWnd();
	afx_msg void OnUpdateProjectGroupConfigWnd(CCmdUI* pCmdUI);
	afx_msg void OnResetProjectLighting();
	afx_msg void OnUpdateResetProjectLighting(CCmdUI* pCmdUI);
	afx_msg void OnLoadSystemParameter();
	afx_msg void OnUpdateLoadSystemParameter(CCmdUI* pCmdUI);	
	afx_msg void OnSaveSystemParameter();
	afx_msg void OnUpdateSaveSystemParameter(CCmdUI* pCmdUI);
	afx_msg void OnSaveRawImage();
	afx_msg void OnUpdateSaveRawImage(CCmdUI* pCmdUI);
	afx_msg void OnConvertSystemParameter();
	afx_msg void OnConvertLoadSystemParameter(CCmdUI* pCmdUI);
	afx_msg void OnBuildMotionXYCaliTable();
	afx_msg void OnUpdateBuildMotionXYCaliTable(CCmdUI* pCmdUI);
	afx_msg void OnLibraryMerge();
	afx_msg void OnUpdateLibraryMerge(CCmdUI* pCmdUI);
	afx_msg void OnLibrarySaveAs();
	afx_msg void OnUpdateLibrarySaveAs(CCmdUI* pCmdUI);
	afx_msg void OnLibraryClear();
	afx_msg void OnUpdateLibraryClear(CCmdUI* pCmdUI);	
	afx_msg void OnLibraryLoad();
	afx_msg void OnUpdateLibraryLoad(CCmdUI* pCmdUI);
	afx_msg void OnLibraryReBuild();
	afx_msg void OnUpdateLibraryReBuild(CCmdUI* pCmdUI);
	afx_msg void OnServerLibraryMerge();
	afx_msg void OnUpdateServerLibraryMerge(CCmdUI* pCmdUI);	
	afx_msg void OnServerLibraryLoad();
	afx_msg void OnUpdateServerLibraryLoad(CCmdUI* pCmdUI);
	afx_msg void OnServerLibraryClear();
	afx_msg void OnUpdateServerLibraryClear(CCmdUI* pCmdUI);
	afx_msg void OnSetSaveDefectImage();
	afx_msg void OnUpdateSetSaveDefectImage(CCmdUI* pCmdUI);
	afx_msg void OnSetOnlineTuningEnable();
	afx_msg void OnUpdateSetOnlineTuningEnable(CCmdUI* pCmdUI);
	afx_msg void OnOnlineTuningLastFile();
	afx_msg void OnUpdateOnlineTuningLastFile(CCmdUI* pCmdUI);
	afx_msg void OnOnlineTuningNextFile();
	afx_msg void OnUpdateOnlineTuningNextFile(CCmdUI* pCmdUI);
	afx_msg void OnOnlineTuningListFile();
	afx_msg void OnUpdateOnlineTuningListFile(CCmdUI* pCmdUI);
	afx_msg void OnSetMultiBoardCtrlMode();
	afx_msg void OnUpdateSetMultiBoardCtrlMode(CCmdUI* pCmdUI);
	afx_msg void OnLaneAdjustHome();
	afx_msg void OnUpdateLaneAdjustHome(CCmdUI* pCmdUI);
	afx_msg void OnLaneAdjustWidth();
	afx_msg void OnUpdateLaneAdjustWidth(CCmdUI* pCmdUI);	
	afx_msg void OnLaneActiveA();
	afx_msg void OnUpdateLaneActiveA(CCmdUI* pCmdUI);
	afx_msg void OnLaneActiveB();
	afx_msg void OnUpdateLaneActiveB(CCmdUI* pCmdUI);
	afx_msg void OnProjectOpen2nd();
	afx_msg void OnUpdateProjectOpen2nd(CCmdUI* pCmdUI);
	afx_msg void OnProjectSwitchTo1();
	afx_msg void OnUpdateProjectSwitchTo1(CCmdUI* pCmdUI);
	afx_msg void OnProjectSwitchTo2();
	afx_msg void OnUpdateProjectSwitchTo2(CCmdUI* pCmdUI);
	afx_msg void OnProjectShowReportTxt();
	afx_msg void OnUpdateProjectShowReportTxt(CCmdUI* pCmdUI);
	afx_msg void OnProjectClearProjectLibrary();
	afx_msg void OnUpdateClearProjectLibrary(CCmdUI* pCmdUI);
	afx_msg void OnProjectAutoLabelModelGroup();
	afx_msg void OnUpdateProjectAutoLabelModelGroup(CCmdUI* pCmdUI);
	afx_msg void OnProjectCloseAll();
	afx_msg void OnUpdateProjectCloseAll(CCmdUI* pCmdUI);	
	afx_msg void OnViewUserRegisterWnd();
	afx_msg void OnUpdateViewUserRegisterWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewBarcodeDeviceWnd();
	afx_msg void OnUpdateViewBarcodeDeviceWnd(CCmdUI* pCmdUI);
	afx_msg void OnProjectOpenNext();
	afx_msg void OnUpdateProjectOpenNext(CCmdUI* pCmdUI);
	afx_msg void OnProjectClose();
	afx_msg void OnUpdateProjectClose(CCmdUI* pCmdUI);
	afx_msg void OnProjectListCombo();
	afx_msg void OnUpdateProjectListCombo(CCmdUI* pCmdUI);		
	afx_msg void OnSetProjectColorListWnd();
	afx_msg void OnUpdateSetProjectColorListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectFdListWnd();
	afx_msg void OnUpdateViewProjectFdListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectPanelListWnd();
	afx_msg void OnUpdateViewProjectPanelListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectBoardListWnd();
	afx_msg void OnUpdateViewProjectBoardListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectBarcodeListWnd();
	afx_msg void OnUpdateViewProjectBarcodeListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectComponentListWnd();
	afx_msg void OnUpdateViewProjectComponentListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectComponentDefectAlarmListWnd();
	afx_msg void OnUpdateViewProjectComponentDefectAlarmListWnd(CCmdUI* pCmdUI);
	afx_msg void OnProjectLoadOfflineFile();
	afx_msg void OnUpdateProjectLoadOfflineFile(CCmdUI* pCmdUI);
	afx_msg void OnProjectLoadBOMFile();
	afx_msg void OnUpdateProjectLoadBOMFile(CCmdUI* pCmdUI);
	afx_msg void OnProjectLoadSPICad();
	afx_msg void OnUpdateProjectLoadSPICad(CCmdUI* pCmdUI);
	afx_msg void OnViewOnlineFormView();
	afx_msg void OnUpdateViewOnlineFormView(CCmdUI* pCmdUI);	
	afx_msg void OnViewDropOutVerifyWnd();
	afx_msg void OnUpdateViewDropOutVerifyWnd(CCmdUI* pCmdUI);	
	afx_msg void OnViewITSCommWnd();
	afx_msg void OnUpdateViewITSCommWnd(CCmdUI* pCmdUI);	
	afx_msg void OnResetControlCenter();
	afx_msg void OnUpdateResetControlCenter(CCmdUI* pCmdUI);
	afx_msg void OnProjectDistrictActiveA();
	afx_msg void OnUpdateProjectDistrictActiveA(CCmdUI* pCmdUI);
	afx_msg void OnProjectDistrictActiveB();
	afx_msg void OnUpdateProjectDistrictActiveB(CCmdUI* pCmdUI);
	afx_msg void OnViewRemoteParamWnd();
	afx_msg void OnUpdateViewRemoteParamWnd(CCmdUI* pCmdUI);
	afx_msg void OnUserSignIn();
	afx_msg void OnUpdateUserSignIn(CCmdUI* pCmdUI);
	afx_msg void OnUserSignOut();
	afx_msg void OnUpdateUserSignOut(CCmdUI* pCmdUI);
	afx_msg void OnSetFdSortWnd();
	afx_msg void OnUpdateSetFdSortWnd(CCmdUI* pCmdUI);
	afx_msg void OnSetFdSortWndBoard();
	afx_msg void OnUpdateSetFdSortWndBoard(CCmdUI* pCmdUI);
	afx_msg void OnSetProjectModeNormal();
	afx_msg void OnUpdateSetProjectModeNormal(CCmdUI* pCmdUI);
	afx_msg void OnSetProjectModeOpenBarcode();
	afx_msg void OnUpdateSetProjectModeOpenBarcode(CCmdUI* pCmdUI);
	afx_msg void OnViewProjectOpenCodeListWnd();
	afx_msg void OnUpdateViewProjectOpenCodeListWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewOperatorLogWnd();
	afx_msg void OnUpdateViewOperatorLogWnd(CCmdUI* pCmdUI);
	afx_msg void OnMesSetCtrlStateOffline();
	afx_msg void OnUpdateMesSetCtrlStateOffline(CCmdUI* pCmdUI);
	afx_msg void OnMesSetCtrlStateLocal();
	afx_msg void OnUpdateMesSetCtrlStateLocal(CCmdUI* pCmdUI);
	afx_msg void OnMesSetCtrlStateRemote();
	afx_msg void OnUpdateMesSetCtrlStateRemote(CCmdUI* pCmdUI);
	afx_msg void OnHotKeyRotateObj();
	afx_msg void OnUpdateHotKeyRotateObj(CCmdUI* pCmdUI);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8CB37586_4B26_4517_884B_F7631BDE662C__INCLUDED_)
