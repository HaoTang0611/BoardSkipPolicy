// AOIDataCollect.h: interface for the CAOIDataCollect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIDATACOLLECT_H__4D0868BB_E44F_4045_89BB_60E9E43F4433__INCLUDED_)
#define AFX_AOIDATACOLLECT_H__4D0868BB_E44F_4045_89BB_60E9E43F4433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIFileIO.h"
#include "AOISystem.h"
#include "AOIProject.h"
#include "ColorGroupSet.h"
#include "Barcode_Device.h"
#include "Barcode_Handheld.h"
#include "AlgBinaryParam.h"
#include "FilenameSyntax.h"
#include "SystemParameterDef.h"
#include "BoardSkipPolicy.h"
//-------------------------------------------------------------------------------------//
#define MAX_OPEN_MP_COUNT             64//最多同時64組Open MP數量
#define MAX_THREAD_COUNT_SLICE_FILL    8//最多同時2組相機影像分割執行緒
#define MAX_THREAD_COUNT_PATCH_MERGE   8//最多同時2組碎布合併執行緒
#define MAX_THREAD_COUNT_FRAME_MERGE  32//最多同時32組影像合併執行緒
#define MAX_THREAD_COUNT_FIELD_MERGE  32//最多同時32組區域合併執行緒
#define MAX_THREAD_COUNT_FRAME_LOAD   32//最多同時32組影像載入執行緒
#define MAX_THREAD_COUNT_FIELD_CALC   32//最多同時32組視野計算執行緒
#define MAX_THREAD_COUNT_REGION_CALC  32//最多同時32組檢測框執行緒
#define MAX_THREAD_COUNT_PROC_IDLE     8//最多同時8組軟體閒置核心
#define MAX_THREAD_COUNT_GRAB_IDLE    16//最多同時16組取像閒置核心
#define MAX_THREAD_COUNT_LANE_AUTO_RUN LANE_ID_RETURN//最多幾個軌道自動運轉
//-------------------------------------------------------------------------------------//
#define AOI_LIBRARY_FROM_HOST           0//本機資料庫
#define AOI_LIBRARY_FROM_REMOTE         1//伺服器資料庫
//-------------------------------------------------------------------------------------//
enum THREAD_GRAB_MODE//執行緒取像模式
{
	THREAD_GRAB_NONE=0,//沒有
	THREAD_GRAB_PROJECT_MAP,//專案底圖
	THREAD_GRAB_PROJECT_MARK,//專案特徵	
	THREAD_GRAB_PANEL_FD,//整板定位點
	THREAD_GRAB_BOARD_FD,//單板定位點
	THREAD_GRAB_BARCODE,//條碼檢測
	THREAD_GRAB_PROJECT_TEST,//專案檢測
	THREAD_GRAB_PROJECT_OPEN_CODE,//專案開檔條碼
	THREAD_GRAB_ONLINE_CALIBRATION,//線上校正
	THREAD_GRAB_MODE_RETURN
};
//-------------------------------------------------------------------------------------//
enum DRAW_IMAGE_MODE
{
	DRAW_IMAGE_BY_RAW, //顯示原圖
	DRAW_IMAGE_NORMAL, //顯示不用2值化的圖
	DRAW_IAMGE_BY_ALG  //顯示檢測框的參數圖
};
//-------------------------------------------------------------------------------------//
enum DRAW_COMPONENT_MODE
{
	DRAW_COMPONENT_ALL,     //顯示所有零件
	DRAW_COMPONENT_FOCUSED  //顯示單一零件
};
//-------------------------------------------------------------------------------------//
enum MANIPULATE_MAIN_MODE
{
	MANIPULATE_MAIN_SELECT       ,//選取
	MANIPULATE_MAIN_ADD          ,//新增加
	MANIPULATE_MAIN_PASTE        ,//貼上	
	MANIPULATE_MAIN_MARK         ,//專案標記
	MANIPULATE_MAIN_RETURN
};
//-------------------------------------------------------------------------------------//
enum INSPECTING_MODE
{
	INSPECTING_NONE     = 0, //未定義
	INSPECTING_ONLINE   = 1,//在線檢測
	INSPECTING_TUNNING  = 2,//調適程式
	INSPECTING_SELECTED = 3,//選取檢測
};
//-------------------------------------------------------------------------------------//
#define    REMOVE_FOLDER_TYPE_ONLINE_TUNING_FOLDER   1//線上調適資料夾
#define    REMOVE_FOLDER_TYPE_PROJECT_TEMP_FOLDER    2//專案暫存資料夾
//-------------------------------------------------------------------------------------//
typedef struct tagRemoveFolder
{
	CString            Folder;
	CTime              DateTime;
	bool               Removed;
	int                FolderType;
	tagRemoveFolder()
	{
		Removed = false;
		FolderType = 0;
	}
} TRemoveFolder, *PRemoveFolder;
//-------------------------------------------------------------------------------------//
class CAOIDataCollect  
{
public:
	//---------------------------------------------------------------------------------//		
	static USER_LEVEL_MODE     LoadUserLevel(LPCTSTR sLevel);//載入使用者權限	
	static CString             ObtainSystemParameterDescText(SYSTEM_PARAM_ID Param);//取得系統參數的說明文字	
	static bool                ObtainSystemParameterKeyName(SYSTEM_PARAM_ID ParamID, CString &Section, CString &KeyName);//取得系統參數的KeyName
	static bool                SetSystemParameterStringByID(SYSTEM_PARAM_ID ParamID, TSystemParameter &SysParam, LPCTSTR String);//設定系統參數
	static bool                GetSystemParameterStringByID(SYSTEM_PARAM_ID ParamID, const TSystemParameter &SysParam, CString &String);//取得系統參數	
	//---------------------------------------------------------------------------------//	
private:
	// 各軌獨立保存跳板決策；逐板通知於下一步接入。
	CBoardSkipPolicy           m_BoardSkipPolicyLA;
	CBoardSkipPolicy           m_BoardSkipPolicyLB;
	void                       ResetBoardSkipPolicies();//重設雙軌跳板狀態
	void                       IncrementBoardCountForSkipPolicy(TOnlineProcParam &Param);//增加指定軌道的抽檢板子計數
	bool                       IsCurrentBoardSkippedByPolicy(LANE_ID laneID) const;//查詢指定軌道本片是否依抽檢規則跳過，不改變計數
	//---------------------------------------------------------------------------------//	
	std::map<int, std::string> m_AsciiTable;//ASCII 表格
	//---------------------------------------------------------------------------------//
	CFont                      m_WndFont;//視窗字型
	UINT                       m_MainViewWndID;//主要顯示視窗的控制編號
	UINT                       m_RibbonCategoryIndex;//Ribbon的分類引數
	UINT                       m_EditImagePageWndID;//主要影像編輯視窗編號
	bool                       m_IsReleased;//要離開系統			
	bool                       m_IsReleasedDone;//離開系統結束	
	CString                    m_AppFilename;//軟體檔案名稱
	CString                    m_ErrorString;	
	CString                    m_ErrorString_OnlineRun;//錯誤敘述-線上運作
	//---------------------------------------------------------------------------------//			
	int                        m_MessageReturn;//訊息結果
	UINT                       m_MessageType;  //訊息樣式
	UINT                       m_MessageIDHelp;//訊息編號
	CString                    m_MessageString;//訊息文字
	//---------------------------------------------------------------------------------//			
	CRITICAL_SECTION           m_csGlobal;//同步化
	CRITICAL_SECTION           m_csThread;//同步化	
	CRITICAL_SECTION           m_csITSProc;//同步化	
	CRITICAL_SECTION           m_csUserInput;//同步化
	CRITICAL_SECTION           m_csSystemException;//同步化
	CRITICAL_SECTION           m_csThreadSystemRun;//同步化-系統運作
	CRITICAL_SECTION           m_csThreadSliceFill;//同步化-影像填滿
	CRITICAL_SECTION           m_csThreadFrameMerge;//同步化-影像合併
	CRITICAL_SECTION           m_csThreadFieldMerge;//同步化-區域合併	
	CRITICAL_SECTION           m_csThreadFrameLoad;//同步化-影像載入
	CRITICAL_SECTION           m_csThreadFieldCalc;//同步化-視野計算
	CRITICAL_SECTION           m_csThreadFrameRelease;//同步化-影像釋放
	CRITICAL_SECTION           m_csThreadRegionCalc;//同步化-區域計算
	CRITICAL_SECTION           m_csThreadSequenceThread;//同步化-循序執行緒
	CRITICAL_SECTION           m_csThreadOnlineInspection;//同步化-線上檢測
	CRITICAL_SECTION           m_csThreadRemoveFolder;//同步化-移除資料夾
	CRITICAL_SECTION           m_csThreadConveyerAutoRun;//同步化-軌道自動運轉
	CRITICAL_SECTION           m_csThreadSimpleJob;//同步化-簡易工作	
	CRITICAL_SECTION           m_csRepairResultList;//同步化-維修站結果列表
	CRITICAL_SECTION           m_csThreadLoadRepairFile;//同步化-載入維修站檔案
	CRITICAL_SECTION           m_csThreadHASI_Monitor;//HASI 監測執行緒
	//---------------------------------------------------------------------------------//			
	char                       m_AOIDirectoryA[MAX_JET_PATH];//系統資料夾
	wchar_t                    m_AOIDirectoryW[MAX_JET_PATH];//系統資料夾
	char                       m_AOILogDirectoryA[MAX_JET_PATH];//訊息資料夾
	wchar_t                    m_AOILogDirectoryW[MAX_JET_PATH];//訊息資料夾
	char                       m_AOITempDirectoryA[MAX_JET_PATH];//暫存資料夾
	wchar_t                    m_AOITempDirectoryW[MAX_JET_PATH];//暫存資料夾
	char                       m_AOIDefaultModelDirectoryA[MAX_JET_PATH];//模組預設視窗資料夾
	wchar_t                    m_AOIDefaultModelDirectoryW[MAX_JET_PATH];//模組預設視窗資料夾	
	
	CString                    m_AOITempDirectory;//暫存的資料夾	
	CString                    m_AOIServerProjectFolder;//伺服器專案資料夾
	CString                    m_AOIServerLibraryFolder;//伺服器資料庫資料夾
	CString                    m_AOITempProjectDirectory;//暫存的專案資料夾
	CString                    m_AOIDefaultModelDirectory;//預設模組的資料夾	
	DWORD                      m_ComputerCPUCoreNumber;//電腦CPU核心數
	DWORD_PTR                  m_ThreadAffinityMask;//執行緒的CPU親和性遮罩	
	DWORD_PTR                  m_ThreadAffinityMask_Backup;//執行緒的CPU親和性遮罩-備份
	TImageParameter            m_ImageParameter;//影像參數
	TSystemParameter           m_SystemParameter;//系統參數	
	TCalibrationParameter      m_CalibrationParameter;//校正參數	
	TBasePlaneParam            m_SystemBasePlaneParam[MAX_SYSTEM_BASE_PLANE_PARAM_COUNT];//系統的空間雜訊過濾參數
	TNoiseFilterParam          m_SystemNoiseFilterParam[MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT];//系統的空間雜訊過濾參數
	//---------------------------------------------------------------------------------//	
	WND_MESSAGE_FROM_ID        m_WndMessageFromID;//視窗訊息來源編號	
	std::vector<WND_DEFECT_ID> m_WndDefectIDList;//檢測框瑕疵代碼列表
	//---------------------------------------------------------------------------------//	
	CString                    m_OpenProjectFolder;//開啟專案資料夾
	//---------------------------------------------------------------------------------//		
	DWORD                      m_BarcodeHandHeld_Result;//手持式條碼機-結果
	LANE_ID                    m_BarcodeHandHeld_LaneID;//手持式條碼機-軌道
	CAOIProject*               m_BarcodeHandHeld_Project;//手持式條碼機-專案
	CBarcode_Handheld          m_BarcodeHandHeld_LA;//A軌道的手持式條碼機
	CBarcode_Handheld          m_BarcodeHandHeld_LB;//B軌道的手持式條碼機
	//---------------------------------------------------------------------------------//	
	TBarcodeDevice             m_BarcodeParam[MAX_BARCODE_DEVICE_COUNT];//條碼機參數
	CBarcode_Basic            *m_BarcodeDevicePtr[MAX_BARCODE_DEVICE_COUNT];//條碼機參數
	int                        m_BarcodeDeviceIDList_LA[MAX_BARCODE_DEVICE_COUNT];//A軌道的條碼機編號
	int                        m_BarcodeDeviceIDList_LB[MAX_BARCODE_DEVICE_COUNT];//B軌道的條碼機編號
	//---------------------------------------------------------------------------------//
	LANE_ID                    m_ActiveLaneID;//現在操作的軌道編號	
	LANE_ID                    m_ExceptionLaneID;//現在異常的軌道編號	
	DISTRICT_ID                m_ActiveDistrictID;//現在操作的分段編號
	CAOIProject*               m_ActiveProjectPtr;
	//---------------------------------------------------------------------------------//			
	int                        m_DLPLedColor;//DLP燈源顏色
	bool                       m_IsNeedGrabFiducial_LA;//是否需要掃描定位點
	bool                       m_IsNeedGrabFiducial_LB;//是否需要掃描定位點
	bool                       m_IsNeedResetLightCtrlDLP;//是否需要重設燈源控制板-DLP
	bool                       m_IsNeedReCheckPCBInside;//是否需要重新確認PCB板是否在裡面
	DWORD                      m_TurnOnOffTargetCapTickCount;//開關塊規上蓋的TickCount
	//---------------------------------------------------------------------------------//
	bool                       m_IsLockUIWnd;//是否鎖住介面視窗	
	bool                       m_IsRepeatTest;//是否重複檢測
	bool                       m_IsRepeatTestUI;//是否重複檢測-介面	
	bool                       m_IsAskRepeatTest;//是否詢問重複檢測次數	
	bool                       m_IsOnAutoRetry;//是否正在自動重測
	bool                       m_IsOnInspection;//是否正在檢測中	
	bool                       m_IsEnableAutoRetry;//是否啟用自動重測
	bool                       m_IsIgnoreAutoRetry;//是否忽略自動重測			
	bool                       m_IsEnhanceDisplayImage;//是否使用顯示畫面強化
	bool                       m_IsLockSystemParameter;//是否鎖住系統參數
	bool                       m_IsOnlineCheckSystemReady;//是否在線時可以確認系統狀態
	int                        m_AOILibraryFromID;//AOI資料庫來源編號
	int                        m_AutoRetryCount;//自動重測次數
	size_t                     m_RepeatedTestCount;//重複檢測數量
	size_t                     m_RepeatedTestMaxCount;//重複檢測數量
	bool                       m_IsNeedResetOKNGSignal;//需要重置OK-NG訊號
	bool                       m_IsModelWndGroupSelChange;//是否模組檢測框群組選取切換
	LANE_ID                    m_SwitchMultiLine_NextLaneID;//切換多軌道次序-下個軌道
	//---------------------------------------------------------------------------------//
	//User Input
	bool                       m_IsWaitUserInput;//等待使用者輸入		
	bool                       m_IsStopUserInput;//停止使用者輸入		
	int                        m_WaitUserInputCount;//等待使用者輸入次數		
	//---------------------------------------------------------------------------------//		
	bool                       m_IsOnlineRunLock;//是否線上運作鎖住
	//---------------------------------------------------------------------------------//			
	//Exception 
	bool                       m_IsSystemException;//是否系統異常
	bool                       m_IsGetSystemExceptionMsg;//是否取得系統異常訊息
	//---------------------------------------------------------------------------------//		
	TASK_MODE                  m_TaskMode;//任務模式
	bool                       m_OnlineTaskCancel;//任務取消
	TASK_STATE_MODE            m_OnlineTaskState;//任務狀態
	LANE_STATE_MODE            m_OnlineLaneTaskState_LA;//任務狀態-lane A
	LANE_STATE_MODE            m_OnlineLaneTaskState_LB;//任務狀態-lane B
	INSPECTING_MODE            m_InspectingMode;//檢測模式, 線上檢測或者調適檢測
	THREAD_GRAB_MODE           m_ThreadGrabMode;//執行緒取像模式

	HWND                       m_CallbackHWnd;//回傳的視窗
	HWND                       m_MainFrameWnd;//主要框架視窗-多執行緒下可能會取不到
	HWND                       m_LockScreenLimitWnd;//限制視窗 LockScreen
	bool                       m_LockScreenEnabledReal;//鎖住螢幕啟用
	OFFLINE_FILE_MODE          m_OfflineFileMode;//離線檔案模式
	bool                       m_InspectionDrawing;//檢測繪圖
	MODEL_ATTACHED_OBJ         m_ModelAttachedObj;//模組掛載的物件	
	CAD_FILE_CONTENT_MODE      m_LoadCADFileContentMode;//載入CAD檔案內容模式-開啟新專案使用	
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_CameraImageW[MAX_CAMERA_COUNT];//最多八個相機的影像尺寸-寬度
	IMAGE_SIZE                 m_CameraImageH[MAX_CAMERA_COUNT];//最多八個相機的影像尺寸-高度
	CAMERA_IMAGE_MODE          m_CameraImageMode[MAX_CAMERA_COUNT];//最多八個相機的影像模式
	double                     m_CameraResolutionX[MAX_CAMERA_COUNT];//最多八個相機的解析度-X
	double                     m_CameraResolutionY[MAX_CAMERA_COUNT];//最多八個相機的解析度-Y
	//---------------------------------------------------------------------------------//		
	std::vector<TRemoteParam>  m_SystemRemoteParamList;//遠端調機參數列表
	//---------------------------------------------------------------------------------//	
	std::vector<TSliceParam>   m_SystemSliceParamList;//單1影像參數列表
	std::vector<TFrameParam>   m_SystemFrameParamList;//影像參數列表	
	//---------------------------------------------------------------------------------//	
	std::vector<TSliceParam>   m_GrabSliceParamList;//單1影像參數列表
	std::vector<TFrameParam>   m_GrabFrameParamList;//影像參數列表	
	//---------------------------------------------------------------------------------//		
	std::vector<CAOIRgn*>      m_RgnPtrList;//檢測區域指標列表	
	long                       m_RgnFinishedCnt;//檢測區域計算數量
	//---------------------------------------------------------------------------------//
	std::vector<CAOIFov*>      m_FovPtrList;//視野指標列表	
	//---------------------------------------------------------------------------------//	
	std::vector<CAOISlice*>    m_SlicePtrList;//相機片指標列表
	long                       m_SliceFinishedCnt;//相機片計算數量
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIField*>    m_FieldPtrList;//相機區域影像指標列表	
	long                       m_FieldCalcFinishedCnt;//視野區域計算數量
	long                       m_FieldMergeFinishedCnt;//相機區域計算數量
	//---------------------------------------------------------------------------------//
	std::vector<CAOIFrame*>    m_FramePtrList;//畫面影像指標列表
	long                       m_FrameFinishedCnt;//畫面影像計算數量
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIProject*>  m_ProjectPtrList;//專案指標列表		
	//---------------------------------------------------------------------------------//		
	unsigned int               m_LaneProjectTestCountForTurn[MULTI_LANE_RETURN];//軌道專案檢測數量-輪流檢測用	
	//---------------------------------------------------------------------------------//
	std::vector<CAOIProject*>  m_LaneProjectList_LA;//軌道專案指標列表			
	std::vector<TSliceParam>   m_LaneProjectSliceParamList_LA;//單1影像參數列表
	std::vector<TFrameParam>   m_LaneProjectFrameParamList_LA;//影像參數列表	
	std::vector<unsigned int>  m_LaneProjectFrameUniqueIDList_LA;//專案影像表格-檢測使用

	std::vector<CAOIProject*>  m_LaneProjectList_LB;//軌道專案指標列表		
	std::vector<TSliceParam>   m_LaneProjectSliceParamList_LB;//單1影像參數列表
	std::vector<TFrameParam>   m_LaneProjectFrameParamList_LB;//影像參數列表	
	std::vector<unsigned int>  m_LaneProjectFrameUniqueIDList_LB;//專案影像表格-檢測使用
	//---------------------------------------------------------------------------------//	
	//Project Mark
	std::vector<CAOIProject*>  m_MarkProjectList;//軌道專案指標列表		
	std::vector<TSliceParam>   m_MarkSliceParamList;//單1影像參數列表
	std::vector<TFrameParam>   m_MarkFrameParamList;//影像參數列表	
	std::vector<CAOIRgn*>      m_MarkRgnPtrList;//標記檢測區域指標列表	
	std::vector<CAOIFov*>      m_MarkFovPtrList;//標記視野指標列表
	std::vector<CAOISlice*>    m_MarkSlicePtrList;//標記相機片指標列表
	std::vector<CAOIField*>    m_MarkFieldPtrList;//標記區域影像指標列表
	std::vector<CAOIFrame*>    m_MarkFramePtrList;//標記畫面影像指標列表
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_OpenProjectProjectObj;	
	bool                       m_OnlineOpenProjectFinish;//線上開啟專案結束		
	DWORD                      m_OnlineOpenProjectHandHeldResult;//線上開啟專案-手持條碼-結果	
	CString                    m_OnlineOpenProjectHandHeldBarcode;//線上開啟專案-手持條碼-條碼
	BARCODE_CAMERA_TEST_STATE  m_OnlineOpenProjectCameraBarcodeTestState;//線上開啟專案-相機條碼檢測狀態
	//---------------------------------------------------------------------------------//	
	double                     m_ViewImageZoom;//顯示的影像縮放
	double                     m_FovMinSizeW;//視野最低尺寸W
	double                     m_FovMinSizeH;//視野最低尺寸H
	double                     m_FovPositionX;//視野位置-X
	double                     m_FovPositionY;//視野位置-Y
	double                     m_TargetMinSizeW;//目標最低尺寸W
	double                     m_TargetMinSizeH;//目標最低尺寸H	
	double                     m_FovTargetOffsetX;//視野目的相對中心偏差-X
	double                     m_FovTargetOffsetY;//視野目的相對中心偏差-Y
	//---------------------------------------------------------------------------------//	
	bool                       m_SaveProjectRawImage;//儲存專案原始圖像	
	CString                    m_SaveProjectRawImageFolder;//儲存專案原始圖像路徑
	//---------------------------------------------------------------------------------//	
	bool                       m_SaveDefectImage;//儲存瑕疵圖像			
	bool                       m_SaveModelBkImage;//儲存模組底圖
	//---------------------------------------------------------------------------------//
	bool                       m_OfflineMode;//離線模式
	bool                       m_SaveOfflineFiles;//儲存離線檔案
	bool                       m_SaveOfflineImageFiles;//儲存離線檔案
	CString                    m_OfflineFolder;
	CString                    m_OfflineFileName;	
	//---------------------------------------------------------------------------------//
	DWORD                      m_UserLastInputTickCount;//使用者上次輸入時間戳記
	//---------------------------------------------------------------------------------//			
	CString                    m_OnlineStateNote_LA;//線上檢測狀態補充說明
	CString                    m_OnlineStateNote_LB;//線上檢測狀態補充說明
	ONLINE_STATE_MODE          m_OnlineStateMode;//線上檢測狀態	
	ONLINE_STATE_MODE          m_OnlineStateMode_LA;//A軌道線上檢測狀態	
	ONLINE_STATE_MODE          m_OnlineStateMode_LB;//B軌道線上檢測狀態	
	ONLINE_STATE_MODE          m_OnlineStateMode_GUI;//線上檢測狀態-GUI
	ONLINE_STATE_MODE          m_OnlineStateMode_GUI_LA;//線上檢測狀態-GUI
	ONLINE_STATE_MODE          m_OnlineStateMode_GUI_LB;//線上檢測狀態-GUI
	ONLINE_STATE_MODE          m_OnlineStateMode_Next;//線上檢測狀態-下一步
	DWORD                      m_OnlineFirstIdleTickCount;//線上初次閒置時間戳記	
	__int64                    m_OnlineFirstRunDateTime;//線上檢測初始時間	
	CString                    m_OnlineAutoCalibrationDateTime_XYZHome;//在線自動校正日期時間-XYZ歸零
	bool                       m_OnlineAutoCalibrationToExec_XYZHome;//在線自動校正去執行-XYZ歸零
	CString                    m_OnlineAutoCalibrationDateTime_2DCurrent;//在線自動校正日期時間-2D電流
	bool                       m_OnlineAutoCalibrationToExec_2DCurrent;//在線自動校正去執行-2D電流
	CString                    m_OnlineAutoCalibrationDateTime_3DCurrent;//在線自動校正日期時間-3D電流
	CString                    m_OnlineAutoCalibrationDateTime_3DZeroPlane;//在線自動校正日期時間-3D相平面
	bool                       m_OnlineAutoCalibrationToExec_3DZeroPlane;//在線自動校正去執行-3D相平面
	CString                    m_OnlineAutoCalibrationDateTime_3DHeightFactor;//在線自動校正日期時間-3D高度比例
	int                        m_OnlineAutoCalibrationDLPLEDColorUsed;//在線自動校正DLP燈源顏色
	bool                       m_OnlineAutoCalibrationDLPZeroPlaneCopyToWhite;//複製在線自動校正的DLP相平面至白燈
	//---------------------------------------------------------------------------------//	
	PCB_OUT_MODE               m_LanePCBOutMode_LA;//A軌道PCB出板模式
	PCB_OUT_MODE               m_LanePCBOutMode_LB;//B軌道PCB出板模式
	PCB_OUT_MODE               m_LanePCBOutModeRunning_LA;//A軌道PCB出板模式-執行中
	PCB_OUT_MODE               m_LanePCBOutModeRunning_LB;//B軌道PCB出板模式-執行中
	LANE_WORK_MODE             m_LaneWorkMode_LA;//A軌道運轉模式
	LANE_WORK_MODE             m_LaneWorkMode_LB;//B軌道運轉模式
	DEFECT_HANDLE_MODE         m_LaneDefectHandleMode_LA;//A軌道檢出異常處理模式	
	DEFECT_HANDLE_MODE         m_LaneDefectHandleMode_LB;//B軌道檢出異常處理模式	
	MULTI_PROJECT_TEST_ORDER_MODE m_MultiProjectTestOrderMode;//多專案檢測順序模式
	bool                       m_MultiProjectTestResetDone_LA;//多專案檢測復歸完成-A軌道
	bool                       m_MultiProjectTestResetDone_LB;//多專案檢測復歸完成-B軌道
	int                        m_EnableConveyerPreRun;//軌道提前運轉功能	
	std::vector<CColorGroupSet> m_SystemColorGroupSetList;//系統內定顏色	
	//---------------------------------------------------------------------------------//
	TUserNode                  m_LoginUserNode;//登入使用者
	CString                    m_CurrentUserName;//目前使用者名
	//---------------------------------------------------------------------------------//	
	UINT                       m_RibbonTuneGroupCmdID;//調適的視窗命令編號
	CString                    m_RibbonTuneGroupCmdText;//調適的視窗命令文字
	//---------------------------------------------------------------------------------//	
	UINT                       m_RibbonDefaultWndCmdID;//預設檢測框的視窗命令編號
	CString                    m_RibbonDefaultWndCmdText;//預設檢測框的視窗命令文字
	//---------------------------------------------------------------------------------//	
	bool                       m_OnlineTuningEnable;//啟用在線調機
	//---------------------------------------------------------------------------------//		
	bool                       m_AutoSwitchToOnlineRemoteCtrlEnable;//自動切換線上遠端控制啟用
	//---------------------------------------------------------------------------------//	
	std::vector<TRemoveFolder> m_RemoveFolderListNew;//移除資料夾列表-新增加
	std::vector<TRemoveFolder> m_RemoveFolderListExec;//移除資料夾列表-刪除用
	//---------------------------------------------------------------------------------//		
	std::vector<TRepairResultNode>  m_RepairResultList;//維修站結果資料列表
	//---------------------------------------------------------------------------------//	
	bool                       m_CCSDataTimeLockTest_LA;//中控中心時間鎖住檢測-A軌
	bool                       m_CCSDataTimeLockTest_LB;//中控中心時間鎖住檢測-B軌
	bool                       m_CCSDataTimeLockPCBInOut_LA;//中控中心時間鎖住進出板-A軌
	bool                       m_CCSDataTimeLockPCBInOut_LB;//中控中心時間鎖住進出板-B軌
	std::string                m_CCSDateTimeFinish_LA;//中控中心時間檢測完-A軌
	std::string                m_CCSDateTimeToCheck_LA;//中控中心時間要確認-A軌	
	std::string                m_CCSDateTimeFinish_LB;//中控中心時間檢測完-B軌
	std::string                m_CCSDateTimeToCheck_LB;//中控中心時間要確認-B軌	
	//---------------------------------------------------------------------------------//
	std::string                m_RepairDateTimeToCheck_LA;//維修站時間要確認-A軌	
	std::string                m_RepairDateTimeToCheck_LB;//維修站時間要確認-B軌	
	//---------------------------------------------------------------------------------//
	DWORD                      m_PCBOKNGSignalOnTickCount_LA;//PCB OK/NG延遲時間
	DWORD                      m_PCBOKNGSignalOnTickCount_LB;//PCB OK/NG延遲時間
	//---------------------------------------------------------------------------------//
	//Main View
	MANIPULATE_MAIN_MODE       m_ManipulateMainMode;         //主畫面操作方式
	//---------------------------------------------------------------------------------//
	//Model
	int                        m_ModelIPCLevel;              //模組的IPC等級
	MULTI_BOARD_CTRL_MODE      m_MultiBoardCtrlMode;         //多聯板操作模式	
	MANIPULATE_MODEL_MODE      m_ManipulateModelMode;        //操作模組模式	
	MANIPULATE_MODEL_MODE      m_ManipulateModelModeDefault; //操作模組模式	- 預設

	DRAW_IMAGE_MODE            m_DrawImageMode;//顯示圖片模式
	DRAW_IMAGE_MODE            m_DrawingImageMode;//現在顯示中圖片模式
	DRAW_MODEL_MODE            m_DrawModelMode;//顯示模組模式
	DRAW_COMPONENT_MODE        m_DrawComponentMode;//顯示零件模式
	PROJECT_TASK_MODE          m_ProjectTaskMode;//專案作業模式
	bool                       m_DrawModelActivedLine;//繪製選取到的線	
	MOVE_TO_EVENT_MODE         m_MoveToEventMode;//移動至的事件模式
	DEFECT_FROM_MODE           m_OnlineViewDefectFromeMode;	//在線顯示瑕疵來源模式	
	bool                       m_ShowModelLandBox;//是否顯示模組特徵框模式		
	bool                       m_ProjectMapMode;//顯示專案底圖模式
	bool                       m_ChangeRibbonTuneID;//變更Ribbon的調式編號
	//---------------------------------------------------------------------------------//
	bool                       m_ShowModelWndIndex;//顯示檢測框編號
	bool                       m_ShowModelLandIndex;//顯示特徵框編號	
	//---------------------------------------------------------------------------------//
	bool                       m_ShowFdList;//顯示定位點列表
	bool                       m_ShowMarkList;//顯示特徵點列表
	bool                       m_ShowPanelList;//顯示整板列表
	bool                       m_ShowBoardList;//顯示單板列表
	bool                       m_ShowBarcodeList;//顯示條碼列表	
	bool                       m_ShowComponentList;//顯示零件列表
	//---------------------------------------------------------------------------------//
	bool                       m_ShowUIWndWndList;//顯示檢測框列表視窗
	bool                       m_ShowUIWndMarkList;//顯示特徵框列表視窗	
	bool                       m_ShowUIWndModelList;//顯示模組列表視窗	
	bool                       m_ShowUIWndResultList;//顯示結果列表視窗
	bool                       m_ShowUIWndComponentList;//顯示零件列表視窗
	bool                       m_ShowUIWndPartNumberList;//顯示料號列表視窗	
	//---------------------------------------------------------------------------------//
	CColorRGBV                 m_ColorRGBVTemp;              //影像處理的暫存顏色
	CAlgBinaryParam            m_BinaryParamTemp;            //影像處理的暫存參數檔案
	TModelUpdateToGroupParam   m_ModelUpdateToGroupParam;    //模組更新至群組參數
	//---------------------------------------------------------------------------------//
	CFilenameSyntax            m_FilenameSyntax_BoardMap;  //單板底圖
	CFilenameSyntax            m_FilenameSyntax_PanelMap;  //整板底圖
	CFilenameSyntax            m_FilenameSyntax_ProjectMap;  //專案底圖
	CFilenameSyntax            m_FilenameSyntax_CustomerReport;//客戶報告
	//---------------------------------------------------------------------------------//
	//Log Files	
	unsigned int               m_LogIdx_LogFile;            //訊息檔案
	unsigned int               m_LogIdx_LogOper;            //訊息操作
	unsigned int               m_LogIdx_MovingTime;         //機台檢測訊息
	unsigned int               m_LogIdx_CurrentProcess;     //機台現今程序訊息
	unsigned int               m_LogIdx_ControlCenter;      //機台中央控制訊息
	unsigned int               m_LogIdx_CustomerLogFile;    //客戶訊息檔案
	//---------------------------------------------------------------------------------//	
	//NPM APC
	bool                       m_NPM_FuncBypass;            //NPM功能跳過
	CString                    m_NPM_DateTime_LA;           //NPM的時間-A軌道
	CString                    m_NPM_DateTime_LB;           //NPM的時間-B軌道
	CString                    m_NPM_PCBSerialName_LA;      //NPM的PCB序號-A軌道
	CString                    m_NPM_PCBSerialName_LB;      //NPM的PCB序號-B軌道  
	std::vector<CString>       m_NPM_DeleteFileList_LA;     //要刪除的檔案-A軌道
	std::vector<CString>       m_NPM_DeleteFileList_LB;     //要刪除的檔案-B軌道
	std::vector<CString>       m_NPM_BackupFileList_LA;     //要備份的檔案-A軌道
	std::vector<CString>       m_NPM_BackupFileList_LB;     //要備份的檔案-B軌道
	//---------------------------------------------------------------------------------//
	tHASIPCBSerial             m_HASI_PCBSerialProperty_LA;      //PCB Serial 屬性-Hanwha-A軌道
	tHASIPCBSerial             m_HASI_PCBSerialProperty_LB;      //PCB Serial 屬性-Hanwha-B軌道
	//HASI_STATE_MODE            m_HASI_StateMode_LA;              //SPI 檔案讀取模式-Hanwha-A軌道
	//HASI_STATE_MODE            m_HASI_StateMode_LB;              //SPI 檔案讀取模式-Hanwha-B軌道
	//---------------------------------------------------------------------------------//		
	CString                    m_MES_RemoteCtrlMessage;     //MES遠端控制訊息
	CString                    m_MES_RemoteCtrlProject;      //MES遠端控制專案	
	//---------------------------------------------------------------------------------//
	bool                       m_ExternalCopyFileEnabed;    //外部複製檔案啟用
	std::vector<TCopyFileNode> m_ExternalCopyFileList;      //外部複製檔案列表
	//---------------------------------------------------------------------------------//
	bool                      m_SaveDebugMessage_RGN;       //除錯用的訊息
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIDataCollect(const CAOIDataCollect &collect);
	CAOIDataCollect& operator=(const CAOIDataCollect &collect);
	//---------------------------------------------------------------------------------//	
	void                       InitialAOIDataCollect();//初始化
	void                       InitialAOIColorGroupSetList();//初始化AOI抽色群組列表
	//---------------------------------------------------------------------------------//	
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	TCHAR                      m_ThreadStateModeText[32];//取執行緒的文字敘述的buffer
	TCHAR                      m_ThreadCommandModeText[32];//取執行緒的文字敘述的buffer
	//---------------------------------------------------------------------------------//	
	bool                       m_IsAllCalcThreadStop;//是否所有計算執行緒停止
	bool                       m_IsAnyCalcThreadWorking;//計算的執行緒工作中
	//---------------------------------------------------------------------------------//	
	THREAD_STATE_MODE          m_SystemRunThreadState;//系統運作執行緒狀態
	THREAD_COMMAND_MODE        m_SystemRunThreadCmd;//系統運作執行緒命令
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_SliceFillThreadState[MAX_THREAD_COUNT_SLICE_FILL];//相機圖填滿執行緒狀態
	THREAD_COMMAND_MODE        m_SliceFillThreadCmd[MAX_THREAD_COUNT_SLICE_FILL];//相機圖填滿執行緒命令
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_FrameMergeThreadState[MAX_THREAD_COUNT_FRAME_MERGE];//影像合併執行緒狀態
	THREAD_COMMAND_MODE        m_FrameMergeThreadCmd[MAX_THREAD_COUNT_FRAME_MERGE];//影像合併填滿執行緒命令	
	//---------------------------------------------------------------------------------//	
	THREAD_STATE_MODE          m_FieldMergeThreadState[MAX_THREAD_COUNT_FIELD_MERGE];//區域合併執行緒狀態
	THREAD_COMMAND_MODE        m_FieldMergeThreadCmd[MAX_THREAD_COUNT_FIELD_MERGE];//區域合併填滿執行緒命令	
	//---------------------------------------------------------------------------------//	
	bool                       m_FrameLoadThreadLocked;//影像載入執行緒鎖住
	THREAD_STATE_MODE          m_FrameLoadThreadState[MAX_THREAD_COUNT_FRAME_LOAD];//影像載入執行緒狀態
	THREAD_COMMAND_MODE        m_FrameLoadThreadCmd[MAX_THREAD_COUNT_FRAME_LOAD];//影像載入填滿執行緒命令	
	//---------------------------------------------------------------------------------//	
	THREAD_STATE_MODE          m_FieldCalcThreadState[MAX_THREAD_COUNT_FIELD_CALC];//視野計算入執行緒狀態
	THREAD_COMMAND_MODE        m_FieldCalcThreadCmd[MAX_THREAD_COUNT_FIELD_CALC];//視野計算執行緒命令	
	//---------------------------------------------------------------------------------//	
	THREAD_STATE_MODE          m_RegionCalcThreadState[MAX_THREAD_COUNT_REGION_CALC];//區域計算執行緒狀態
	THREAD_COMMAND_MODE        m_RegionCalcThreadCmd[MAX_THREAD_COUNT_REGION_CALC];//區域計算執行緒命令	
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_ThreadSequenceThreadState;//執行緒程序執行緒狀態
	THREAD_COMMAND_MODE        m_ThreadSequenceThreadCmd;//執行緒程序計算執行緒命令		
	double                     m_ThreadSequenceThreadElapseTimems;//執行緒程序執行緒經過時間
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_FieldFrameReleaseThreadState;//區域影像釋放執行緒狀態
	THREAD_COMMAND_MODE        m_FieldFrameReleaseThreadCmd;//區域影像釋放執行緒命令	
	bool                       m_FieldFrameReleaseThreadNeedCheck;//區域影像釋放執行緒需要確認
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_OnlineInspectionThreadState;//線上檢測行緒狀態
	THREAD_COMMAND_MODE        m_OnlineInspectionThreadCmd;//線上檢測執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_RemoveFolderThreadState;//移除資料夾執行緒狀態
	THREAD_COMMAND_MODE        m_RemoveFolderThreadCmd;//移除資料夾執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_SwitchProjectByMarkThreadState;//依標記切換專案行緒狀態
	THREAD_COMMAND_MODE        m_SwitchProjectByMarkThreadCmd;//依標記切換專案執行緒命令		
	//---------------------------------------------------------------------------------//
	bool                       m_ConveyerAutoRunPreRunMode[MAX_THREAD_COUNT_LANE_AUTO_RUN];//軌道自動運轉-提前運轉//20230425
	CAOIProject               *m_ConveyerAutoRunProjectPtr[MAX_THREAD_COUNT_LANE_AUTO_RUN];//軌道自動運轉狀態
	ONLINE_STATE_MODE          m_ConveyerAutoRunState[MAX_THREAD_COUNT_LANE_AUTO_RUN];//軌道自動運轉狀態
	THREAD_STATE_MODE          m_ConveyerAutoRunThreadState[MAX_THREAD_COUNT_LANE_AUTO_RUN];//軌道自動運轉執行緒狀態
	THREAD_COMMAND_MODE        m_ConveyerAutoRunThreadCmd[MAX_THREAD_COUNT_LANE_AUTO_RUN];//軌道自動運轉執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_ITSProcThreadState;//ITS執行緒狀態
	THREAD_COMMAND_MODE        m_ITSProcThreadCmd;//ITS執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_LoadRepairFileThreadState;//載入維修站檔案執行緒狀態
	THREAD_COMMAND_MODE        m_LoadRepairFileThreadCmd;//載入維修站檔案執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_RepairResultSignalThreadState;//維修站結果訊號執行緒狀態
	THREAD_COMMAND_MODE        m_RepairResultSignalThreadCmd;//維修站結果訊號執行緒命令		
	//---------------------------------------------------------------------------------//		
	SIMPLE_JOB_MODE            m_SimpleJobMode;//簡單工作模式
	LANE_ID                    m_SimpleJob_LaneID;//簡單工作模式-軌道編號
	DISTRICT_ID                m_SimpleJob_DistrictID;//簡單工作模式-分段編號
	THREAD_STATE_MODE          m_SimpleJobThreadState;//簡單工作執行緒狀態
	THREAD_COMMAND_MODE        m_SimpleJobThreadCmd;//簡單工作執行緒命令		
	//---------------------------------------------------------------------------------//		
	THREAD_STATE_MODE          m_HASI_MonitorThreadState;//HASI M2M監測傳訊執行序狀態
	THREAD_COMMAND_MODE        m_HASI_MonitorThreadCmd;//HASI M2M監測傳訊執行序命令
	//---------------------------------------------------------------------------------//		
	double                     m_SpaceMaxHeight;//空間最高高度
	double                     m_SpaceBaseHeight;//空間基本高度
	int                        m_SpaceToGrayRatioMode;//高度轉灰階比例
	double                     m_SpaceToGrayRatio;//高度轉灰階比例
	//---------------------------------------------------------------------------------//
	std::vector<CAOIWnd*>      m_TempWndList;//暫存檢測框列表
	CAOIModel                 *m_ModelPreViewPtr;//模組預覽模組
	//---------------------------------------------------------------------------------//
	CAOIModel                 *m_ModelUniFrameModelPtr;//模組暫存的模組
	TREGION4D                  m_ModelUniFrameRgnStage;//模組暫存的機台座標
	std::vector<TUNI_FRAME>    m_ModelUniFrameList;//模組暫存的影像列表
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_FieldUniFrameRgnStage;//區域暫存的機台座標
	std::vector<TUNI_FRAME>    m_FieldUniFrameList;//區域暫存的影像列表
	//---------------------------------------------------------------------------------//	
	void                       AddMarkRgnPtr(CAOIRgn* RgnPtr);
	void                       AddMarkFovPtr(CAOIFov* FovPtr);
	void                       AddMarkSlicePtr(CAOISlice* SlicePtr);
	void                       AddMarkFieldPtr(CAOIField* FieldPtr);
	void                       AddMarkFramePtr(CAOIFrame* FramePtr);
	bool                       ClearMarkObjList();//刪除標記指標列表
	//---------------------------------------------------------------------------------//
	std::vector<TSliceParam>&  GetSystemSliceParamList();//單次影像參數列表	
	std::vector<TFrameParam>&  GetSystemFrameParamList();//取得影像表單列表		
	CString                    GetImageConfigIniFilename() const;//取得影像組態INI檔名
	bool                       CloneSystemSliceParamList_No3D(std::vector<TSliceParam> &ParamList);//取得單次影像表
	bool                       CloneSystemFrameParamList_No3D(std::vector<TFrameParam> &ParamList);//取得影像表單列表
	bool                       CloneSystemFrameParamList_Only3D(std::vector<TFrameParam> &ParamList);//取得影像表單列表
	//---------------------------------------------------------------------------------//
public:
	CAOIDataCollect();
	virtual ~CAOIDataCollect();
	//---------------------------------------------------------------------------------//	
	LARGE_INTEGER              m_SystemFreq;//系統計數頻率
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetAppFilename() const;//軟體檔案名稱
	//---------------------------------------------------------------------------------//
	bool                       BuildAsciiTable();
	const std::map<int, std::string>& GetAsciiTable() const;
	//---------------------------------------------------------------------------------//
	bool                       GetDisable3D() const;
	//---------------------------------------------------------------------------------//
	void                       LockGlobal();
	void                       UnlockGlobal();
	//---------------------------------------------------------------------------------//		
	void                       LockThread();//鎖住通用執行緒同步化
	void                       UnlockThread();//釋放通用執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockITSProc();//鎖住ITS執行緒同步化
	void                       UnlockITSProc();//釋放ITS執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockUserInput();//鎖住使用者輸入同步化
	void                       UnlockUserInput();//釋放使用者輸入同步化
	//---------------------------------------------------------------------------------//
	void                       LockSystemException();//鎖住系統異常
	void                       UnlockSystemException();//釋放系統異常
	//---------------------------------------------------------------------------------//	
	void                       LockThreadSystemRun();//鎖住系統運作執行緒同步化
	void                       UnlockThreadSystemRun();//釋放系統運作執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadSliceFill();//鎖住影像填滿執行緒同步化
	void                       UnlockThreadSliceFill();//釋放影像填滿執行緒同步化
//---------------------------------------------------------------------------------//	
	void                       LockThreadFrameMerge();//鎖住影像合併執行緒同步化
	void                       UnlockThreadFrameMerge();//釋放影像合併執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadFieldMerge();//鎖住區域合併執行緒同步化
	void                       UnlockThreadFieldMerge();//釋放區域合併執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadFrameLoad();//鎖住影像載入執行緒同步化
	void                       UnlockThreadFrameLoad();//釋放影像載入執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadFieldCalc();//鎖住視野計算執行緒同步化
	void                       UnlockThreadFieldCalc();//釋放視野計算執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadFrameRelease();//鎖住影像釋放執行緒同步化
	void                       UnlockThreadFrameRelease();//釋放影像釋放執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadRegionCalc();//鎖住區域計算執行緒同步化
	void                       UnlockThreadRegionCalc();//釋放區域計算執行緒同步化
	//---------------------------------------------------------------------------------//
	void                       LockThreadSequenceThread();//鎖住循序執行緒同步化
	void                       UnlockThreadSequenceThread();//釋放循序執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadOnlineInspection();//鎖住線上檢測執行緒同步化
	void                       UnlockThreadOnlineInspection();//釋放線上檢測執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadRemoveFolder();//鎖住移除資料夾執行緒同步化
	void                       UnlockThreadRemoveFolder();//釋放移除資料夾執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadConveyerAutoRun();//鎖住通軌道自動運轉執行緒同步化
	void                       UnlockThreadConveyerAutoRun();//釋放軌道自動運轉執行緒同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadSimpleJob();//鎖住簡易工作執行緒同步化
	void                       UnlockThreadSimpleJob();//釋放簡易工作執行緒同步化
	//---------------------------------------------------------------------------------//			
	void                       LockRepairResultList();//鎖住維修站結果同步化
	void                       UnlockRepairResultList();//釋放維修站結果同步化
	//---------------------------------------------------------------------------------//	
	void                       LockThreadLoadRepairFile();//鎖住載入維修站檔案同步化
	void                       UnlockThreadLoadRepairFile();//釋放載入維修站檔案同步化
	//---------------------------------------------------------------------------------//	
	CString                    GetSystemParamFilename() const;
	CString                    GetLight3DParamFilename() const;
	//---------------------------------------------------------------------------------//	
	void                       SetSystemParameter(const TSystemParameter &Param);//設定系統參數
	TSystemParameter&          GetSystemParameter();//取得系統參數	
	TSystemParameter*          GetSystemParameterPtr();//取得系統參數
	const TSystemParameter&    GetSystemParameter() const;//取得系統參數
	//---------------------------------------------------------------------------------//		
	bool                       SaveSystemParameter();//儲存系統參數		
	bool                       SaveSystemParamFile(LPCTSTR filename, const TSystemParameter &Param);//儲存系統參數		
	bool                       SaveSystemParamFileFn(LPCTSTR filename, const TSystemParameter &Param);//儲存系統參數		
	bool                       SaveSystemParamNode(SYSTEM_PARAM_ID SysParam, const TSystemParameter &Param, LPCTSTR filename);//儲存系統參數
	//---------------------------------------------------------------------------------//	
	bool                       LoadSystemParameter(bool bCreateTempFolder);//載入系統參數
	bool                       LoadSystemParamFile(LPCTSTR filename, TSystemParameter &Param);//載入系統參數
	bool                       LoadSystemParamFileFn(LPCTSTR filename, TSystemParameter &Param);//載入系統參數
	bool                       LoadSystemParamNode(SYSTEM_PARAM_ID SysParam, const TSystemParameter &Param, TCHAR String[], int textlen, LPCTSTR filename);//載入系統參數
	//---------------------------------------------------------------------------------//	
	bool                       UpdateSystemParameter(bool ToModule=true);//載入調整系統參數
	bool                       UpdateSystemParameterToProject();//更新系統參數至專案	
	//---------------------------------------------------------------------------------//			
	bool                       LoadSystemParamFileFromOther(LPCTSTR SystemFolder);//載入其他的系統參數
	bool                       LoadSystemParamFileFromOtherFn(LPCTSTR SystemFolder);//載入其他的系統參數
	//---------------------------------------------------------------------------------//			
	bool                       CopySystemParamFileToProjectFolder();//複製系統參數到專案資料夾-給遠端調體使用
	bool                       LoadSystemParamFileFromRemoteParam(const TRemoteParam &RemoteParam);//載入遠端系統參數-給遠端調體使用
	bool                       LoadSystemParamFileFromRemoteParamFn(const TRemoteParam &RemoteParam);//載入遠端系統參數-給遠端調體使用
	bool                       AdjustRemoteParamToHostParameter(const TSystemParameter &HostParam, TSystemParameter &RemoteParam);//將遠端系統參數調回本機參數
	//---------------------------------------------------------------------------------//		
	bool                       LoadSystemParamFileFromProjectParam(LPCTSTR ProjectFolder);//載入專案的系統參數
	bool                       LoadSystemParamFileFromProjectParamFn(LPCTSTR ProjectFolder);//載入專案的系統參數
	//---------------------------------------------------------------------------------//		
	CString                    GetCalibrationSectionName() const;//取得校正參數	
	TCalibrationParameter&     GetCalibrationParameter();//取得校正參數	
	TCalibrationParameter*     GetCalibrationParameterPtr();//取得校正參數	
	const TCalibrationParameter&  GetCalibrationParameter() const;//取得校正參數	
	bool                       SaveCalibrationParameter();//儲存校正參數
	bool                       SaveCalibrationParamFile(LPCTSTR filename, const TCalibrationParameter &CaliParam);//儲存校正參數
	bool                       SaveCalibrationParamFileFn(LPCTSTR filename, const TCalibrationParameter &CaliParam);//儲存校正參數
	bool                       LoadCalibrationParameter();//載入校正參數
	bool                       LoadCalibrationParamFile(LPCTSTR filename, TCalibrationParameter &CaliParam);//載入校正參數
	bool                       LoadCalibrationParamFileFn(LPCTSTR filename, TCalibrationParameter &CaliParam);//載入校正參數
	bool                       ApplyCalibrationParameter();//套用校正參數	
	bool                       SaveAllCalibrationParameter();//儲存所有校正參數
	//---------------------------------------------------------------------------------//
	TImageParameter&           GetImageParameter();//取得影像參數
	TImageParameter*           GetImageParameterPtr();//取得影像參數
	bool                       SaveSystemImageParameter();//儲存影像參數
	bool                       SaveSystemImageParamFile(LPCTSTR filename, const TImageParameter &ImageParam);//儲存影像參數
	bool                       SaveSystemImageParamFileFn(LPCTSTR filename, const TImageParameter &ImageParam);//儲存影像參數
	bool                       LoadSystemImageParameter();//載入影像參數
	bool                       LoadSystemImageParamFile(LPCTSTR filename, TImageParameter &ImageParam);//載入影像參數
	bool                       LoadSystemImageParamFileFn(LPCTSTR filename, TImageParameter &ImageParam);//載入影像參數
	//---------------------------------------------------------------------------------//
	//外接條碼機
	bool                       SaveSystemBarcodeParameter();//儲存條碼機參數
	bool                       SaveSystemBarcodeParamFile(LPCTSTR filename, const TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[]);//儲存條碼機參數
	bool                       SaveSystemBarcodeParamFileFn(LPCTSTR filename, const TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[]);//儲存條碼機參數
	bool                       LoadSystemBarcodeParameter();//載入條碼機參數
	bool                       LoadSystemBarcodeParamFile(LPCTSTR filename, TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[]);//載入條碼機參數
	bool                       LoadSystemBarcodeParamFileFn(LPCTSTR filename, TBarcodeDevice BarcodeParamList[], int IDList_LA[], int IDList_LB[]);//載入條碼機參數
	//---------------------------------------------------------------------------------//
	//空間基準面參數
	CString                    GetSystemBasePlaneParamFilename();
	CString                    GetSystemBasePlaneParamSection(int index);	
	bool                       DefaultSystemBasePlaneParam();
	bool                       SaveSystemBasePlaneParam();
	bool                       LoadSystemBasePlaneParam();	
	bool                       UpdateSystemBasePlaneParamToProject();//更新系統基準面過濾至專案
	bool                       TurnOffBasePlaneParam(TBasePlaneParam &Param);//關閉空間基準面器
	bool                       CloneSystemBasePlaneParamList(std::vector<TBasePlaneParam> &List);//複製空間基準面表	
	bool                       GetSystemBasePlaneParam(int index, TBasePlaneParam &BasePlaneParam) const;	
	bool                       SetSystemBasePlaneParam(int index, const TBasePlaneParam &BasePlaneParam);		
	bool                       LoadSystemBasePlaneParamFile(LPCTSTR filename, int index, TBasePlaneParam &BasePlaneParam);
	bool                       LoadSystemBasePlaneParamFileFn(LPCTSTR filename, int index, TBasePlaneParam &BasePlaneParam);
	bool                       SaveSystemBasePlaneParamFile(LPCTSTR filename, int index, const TBasePlaneParam &BasePlaneParam);	
	bool                       SaveSystemBasePlaneParamFileFn(LPCTSTR filename, int index, const TBasePlaneParam &BasePlaneParam);	
	//---------------------------------------------------------------------------------//
	//空間雜訊過濾參數
	CString                    GetSystemNoiseFilterParamFilename();
	CString                    GetSystemNoiseFilterParamSection(int index);	
	bool                       DefaultSystemNoiseFilterParam();
	bool                       SaveSystemNoiseFilterParam();
	bool                       LoadSystemNoiseFilterParam();	
	bool                       UpdateSystemNoiseFilterParamToProject();//更新系統空間雜訊過濾至專案
	bool                       TurnOffNoiseFilterParam(TNoiseFilterParam &Param);//關閉空間雜訊過濾器
	bool                       CloneSystemNoiseFilterParamList(std::vector<TNoiseFilterParam> &List);//複製空間雜訊過濾表	
	bool                       GetSystemNoiseFilterParam(int index, TNoiseFilterParam &NoiseFilterParam) const;	
	bool                       SetSystemNoiseFilterParam(int index, const TNoiseFilterParam &NoiseFilterParam);		
	bool                       LoadSystemNoiseFilterParamFile(LPCTSTR filename, int index, TNoiseFilterParam &NoiseFilterParam);
	bool                       LoadSystemNoiseFilterParamFileFn(LPCTSTR filename, int index, TNoiseFilterParam &NoiseFilterParam);
	bool                       SaveSystemNoiseFilterParamFile(LPCTSTR filename, int index, const TNoiseFilterParam &NoiseFilterParam);		
	bool                       SaveSystemNoiseFilterParamFileFn(LPCTSTR filename, int index, const TNoiseFilterParam &NoiseFilterParam);
	//---------------------------------------------------------------------------------//
	//專案預設的抽色餐數		
	bool                       SaveSystemColorGroupSetFile();	
	bool                       SaveSystemColorGroupSetFileFn();	
	bool                       LoadSystemColorGroupSetFile();
	bool                       LoadSystemColorGroupSetFileFn();
	bool                       ReadSystemColorGroupSetList(CAOIFileIO &FileIO);
	bool                       WriteSystemColorGroupSetList(CAOIFileIO &FileIO);
	size_t                     GetSystemColorGroupSetCount();	
	CColorGroupSet*            GetSystemColorGroupSetPtr(size_t index, bool bCheck);	
	bool                       CloneSystemColorGroupSetList(size_t index, std::vector<CColorGroup> &ColorGroupList);
	bool                       SetSystemColorGroupSetList(size_t index, const std::vector<CColorGroup> &ColorGroupList);
	//---------------------------------------------------------------------------------//
	CString                    GetSystemBackupFolder() const;
	bool                       BackupAllSystemFilesFirst();//備份系統檔案-首次
	bool                       BackupAllSystemIniFiles();//備份系統INI檔案
	bool                       BackupAllSystemBinFiles();//備份系統Bin檔案
	bool                       CheckDlpBinFiles(LPCTSTR DstFolder);//確認DLP-BIN檔案
	//---------------------------------------------------------------------------------//
	//模組預設檢測框參數	
	bool                       SaveModelDefaultWndParam();		
	bool                       SaveModelDefaultWndParam(MDW_VERSION eVersion);
	bool                       SaveModelDefaultWndParam(const TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion);	
	bool                       SaveModelDefaultWndParamFn(const TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion);	
	CString                    GetModelDefaultWndParamSection(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode);
	bool                       LoadModelDefaultWndParam(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode, LPCTSTR GroupName, TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion);
	bool                       LoadModelDefaultWndParamFn(MODEL_TYPE ModelType, CHIP_SIZE_MODE ChipSizeMode, LPCTSTR GroupName, TMODEL_DEFAULT_WND_PARAM &ModelParam, MDW_VERSION eVersion);
	MODEL_TYPE                 GetModelDefaultTransistorType() const;//取得模組預設的SOT樣式
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetMachineLocation() const;//本機廠區
	LPCTSTR                    GetMachineBuilding() const;//本機棟別
	LPCTSTR                    GetMachineFloor() const;//本機樓層
	LPCTSTR                    GetMachineRoom() const;//本機車間
	LPCTSTR                    GetMachineLine(LANE_ID LaneID) const;//本機線別
	LPCTSTR                    GetMachineStation(LANE_ID LaneID) const;//本機站別
	LPCTSTR                    GetMachineSN() const;//本機序號	
	LPCTSTR                    GetMachineName() const;//機台型號
	LPCTSTR                    GetMachineVendor() const;//機台廠商
	LPCTSTR                    GetMachineAlias() const;//機台別名
	int                        GetMachineType() const;//機台樣式
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetMachineMES_Name() const;//機台MES名稱
	LPCTSTR                    GetMachineMES_Password() const;//機MES台密碼
	LPCTSTR                    GetMachineMES_Device() const;//機台MES裝置
	LPCTSTR                    GetMachineMES_Device2() const;//機台MES裝置-2
	LPCTSTR                    GetMachineMES_CodeName() const;//機台MES裝置代號	
	//---------------------------------------------------------------------------------//	
	int                        GetOpenMPCount_Max() const;
	int                        GetOpenMPCount_General() const;//Open MP核心數量-一般操作
	int                        GetOpenMPCount_Inspection() const;//Open MP核心數量-檢測中	
	int                        GetOpenMPCheckSize_Inspection() const;//Open MP確認尺寸-檢測中	
	bool                       CheckOpenMPUsed_CalcPhase(int nImageSize) const;//確認使用Open MP
	bool                       CheckOpenMPUsed_ColorFilter(int nImageSize) const;//確認使用Open MP
	bool                       CheckOpenMPUsed_SpaceFilter(size_t nImageSize) const;//確認使用Open MP
	int                        CheckOpenMPCount_SpaceFilter(bool bOpenMp, size_t nImageSize) const;//Open MP核心數量-空間雜訊
	int                        GetOnlineTuningKeepMaxTime() const;//線上調機保留最久時間	
	int                        GetOnlineTuningSavedMaxCount() const;//線上調機儲存最多數量
	bool                       CheckDualRunMode() const;//確認是否啟用雙軌運轉模式
	MULTI_LANE_MODE            CheckMultiLaneMode() const;//多軌道模式			
	bool                       CheckMultiLaneMode_Off() const;//關閉軌道模式
	PCB_OUT_DIRECTION          GetPCBOutDirection() const;//進出板方向
	JET_MATCH_LIB_TYPE         GetMatchLibType() const;//影像匹配函式庫	
	int                        GetBypassLastSignal() const;//忽略上一站訊號
	int                        GetBypassNextSignal() const;//忽略上一站訊號
	int                        GetLastStationLineMode() const;//與上一站連線模式
	int                        GetMoveCameraBeforePCBIn() const;//進板前移動相機頭
	bool                       GetVerifyJsonStringEnabled()const ;//啟用驗證Json字串
	int                        GetPCBInUseCameraImageMode() const;//進板使用相機影像
	bool                       GetPCBInUseCameraImageMode_V7() const;//進板使用相機影像-使用V7控制器, 測試使用
	bool                       CheckHoneywellSwiftDecoderEnabled() const;//確認HoneywellSwiftDecoder啟用
	void                       GetMaskImageColor(unsigned int FrameUniqueID, unsigned char &maskR, unsigned char &maskG, unsigned char &maskB, unsigned char &maskV, unsigned char &Alpha);//取得遮罩影像顏色	
	//---------------------------------------------------------------------------------//	
	int                        GetSystemDlpLedColor() const;//取得系統的DLP-LED顏色
	void                       SetSystemDlpLedColor(int Color);//設定系統的DLP-LED顏色
	//---------------------------------------------------------------------------------//
	//A軌道運轉模式
	LANE_WORK_MODE             GetLaneWorkMode_LA() const;
	void                       SetLaneWorkMode_LA(LANE_WORK_MODE val);
	//B軌道運轉模式
	LANE_WORK_MODE             GetLaneWorkMode_LB() const;
	void                       SetLaneWorkMode_LB(LANE_WORK_MODE val);

	bool                       SwitchWorkLane();//切換工作軌道
	bool                       CheckLaneWorkMode();//確認軌道運轉模式
	LANE_WORK_MODE             GetLaneWorkMode(LANE_ID LaneID) const;//軌道運轉模式
	void                       UpdateLaneWorkMode(TASK_MODE TaskMode);//更新軌道運轉模式	
	bool                       CheckLaneWorkMode_Disable(LANE_ID LaneID) const;//確認軌道運轉模式關閉
	//---------------------------------------------------------------------------------//		
	bool                       CheckServerLibraryQuery(PROJECT_LINK_SERVER_MODE LinkServerMode);//判斷伺服器資料庫是否詢問
	bool                       CheckServerLibraryEnabled(PROJECT_LINK_SERVER_MODE LinkServerMode);//判斷伺服器資料庫是否啟用
	bool                       CheckServerLibraryCanOnlineTuning(PROJECT_LINK_SERVER_MODE LinkServerMode);//判斷伺服器資料庫可以使用在線調機
	bool                       GetPreLoadProjectProgramImage();//取得是否提前載入編程影像
	bool                       GetAutoReleaseFieldFrame();//取得是否自動釋放區域圖像
	bool                       GetAutoReleaseOfflineFieldFrame();//取得是否自動釋放離線編程區域圖像
	bool                       GetInspectionFinishShowResultList();//取得檢測完畢是否顯示結果列表
	//---------------------------------------------------------------------------------//
	bool                       RegisterLogFile();//註冊訊息檔案
	int                        GetMessageReturn() const;//訊息結果
	void                       SetMessageReturn(int val);//訊息結果	
	CString                    GetMessageInfo(UINT &nType, UINT &nIDHelp) const;//設定訊息
	void                       SetMessageInfo(LPCTSTR Str, UINT nType, UINT nIDHelp);//設定訊息		
	int                        ShowMessageSync(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
	int                        ShowMessage(int ErrorCode,MSG_MB_TYPE BoxType,MSG_MB_ICON_TYPE BoxIcon,MSG_MB_BTN BoxBtn,LPCTSTR Content,bool bSaveFile, bool bShowMSG=true);	
	bool                       SaveErrMessage(LPCTSTR Content);
	bool                       SaveErrMessageFn(LPCTSTR Content);
	bool                       SaveLogMessage(LPCTSTR pMessage);//儲存訊息檔案
	bool                       SaveInitialReleaseLog(LPCTSTR Msg);//儲存初始化與釋放訊息	
	bool                       DeleteCurrentProcess();//刪除現在狀態檔案
	bool                       BackupCurrentProcess();//備份現在狀態檔案
	bool                       BackupCurrentProcess_Error();//備份現在狀態檔案
	bool                       SaveCurrentProcess(const char *pContext);//儲存現在狀態
	bool                       SaveCurrentProcess(const wchar_t *pContext);//儲存現在狀態
	bool                       SaveControlCenterLog(const char *pContext);//儲存中控狀態
	bool                       SaveControlCenterLog(const wchar_t *pContext);//儲存中控狀態
	bool                       CreateSystemDirectory();//建立系統路徑
	//---------------------------------------------------------------------------------//
	bool                      SaveDebugMessage(LPCTSTR str);
	bool                      GetSaveDebugMessage_RGN() const;
	void                      SetSaveDebugMessage_RGN(bool val);
	bool                      SaveDebugMessage_RGN(LPCTSTR str);
	//--------------------------------------------------------------------------//	
	bool                       SaveCameraTemperatureLog(CAMERA_ID CameraID);
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogOper_Func(int OperMode, LPCTSTR tag, LPCTSTR Content);//儲存操作訊息
	bool                       SaveLogOper_FuncFn(int OperMode, LPCTSTR tag, LPCTSTR Content);//儲存操作訊息
	bool                       SaveLogOper_Func(LPCTSTR OperName, LPCTSTR tag, LPCTSTR Content);//儲存操作訊息	
	bool                       SaveLogOper_FuncFn(LPCTSTR OperName, LPCTSTR tag, LPCTSTR Content);//儲存操作訊息	
	bool                       SaveLogOper_UserFunc(LPCTSTR tag, LPCTSTR Content);//儲存操作訊息-使用者函式	
	bool                       SaveLogOper_SystemFunc(LPCTSTR tag, LPCTSTR Content);//儲存操作訊息-系統函式	
	bool                       SaveLogOper_ProjectFunc(LPCTSTR tag, LPCTSTR Content);//儲存操作訊息-專案函式	
	bool                       SaveLogOper_ProjectFunc(CAOIProject *ProjectPtr, LPCTSTR Content);//儲存操作訊息-專案函式		
	bool                       SaveLogOper_PanelFunc(CAOIPanel *PanelPtr, LPCTSTR Content);//儲存操作訊息-整板函式			
	bool                       SaveLogOper_BoardFunc(CAOIBoard *BoardPtr, LPCTSTR Content);//儲存操作訊息-單板函式			
	bool                       SaveLogOper_FdFunc(CAOIFd *FdPtr, LPCTSTR Content);//儲存操作訊息-定位點函式	
	bool                       SaveLogOper_MarkFunc(CAOIMark *MarkPtr, LPCTSTR Content);//儲存操作訊息-特徵點函式
	bool                       SaveLogOper_BarcodeFunc(CAOIBarcode *BarcodePtr, LPCTSTR Content);//儲存操作訊息-條碼函式		
	bool                       SaveLogOper_ComponentFunc(CAOIComponent *ComponentPtr, LPCTSTR Content);//儲存操作訊息-零件函式		
	bool                       SaveLogOper_PartGroupFunc(CAOIPartGroup *PartGroupPtr, LPCTSTR Content);//儲存操作訊息-元件群組函式		
	bool                       SaveLogOper_ModelFunc(CAOIModel *ModelPtr, LPCTSTR Content);//儲存操作訊息-模組函式		
	bool                       SaveLogOper_LandFunc(CAOILand *LandPtr, LPCTSTR Content);//儲存操作訊息-特徵框函式			
	bool                       SaveLogOper_WndFunc(CAOIWnd *WndPtr, LPCTSTR Content);//儲存操作訊息-檢測框函式		
	bool                       SaveLogOper_WndRoiFunc(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR Content);//儲存操作訊息-檢測子框函式		
	bool                       SaveLogOper_WndMaskFunc(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR Content);//儲存操作訊息-遮罩框函式		
	//---------------------------------------------------------------------------------//		
	bool                       SaveUIDrawFuncLog(const char *pContext);//儲存介面繪製函式訊息
	bool                       SaveUIDrawFuncLog(const wchar_t *pContext);//儲存介面繪製函式訊息
	//---------------------------------------------------------------------------------//
	bool                       SaveMovingTimeMsg(const char *pContext);//儲存移動時間訊息
	bool                       SaveMovingTimeMsg(const wchar_t *pContext);//儲存移動時間訊
	bool                       SaveMovingTimeMsg(int MsgFilter, int MsgLevel, const char *pContext);//儲存移動時間訊息
	bool                       SaveMovingTimeMsg(int MsgFilter, int MsgLevel, const wchar_t *pContext);//儲存移動時間訊息	
	bool                       DeleteMovingTimeMsg();//刪除移動時間訊息
	bool                       BackupMovingTimeMsg();//備份移動時間訊息
	bool                       BackupMovingTimeMsg_Error();//備份移動時間訊息
	//---------------------------------------------------------------------------------//
	bool                       SetMotionInPositionDelayTime_Fd();//設定XYZ定位延遲時間-定位點
	bool                       SetMotionInPositionDelayTime_Normal();//設定XYZ定位延遲時間-一般
	//---------------------------------------------------------------------------------//
	void                       SetSystemExceptionCode_Param(LPCTSTR Err=NULL);//設定系統錯誤代碼-參數異常
	void                       SetSystemExceptionCode_FileRead(LPCTSTR Err=NULL);//設定系統錯誤代碼-檔案讀取
	void                       SetSystemExceptionCode_FileWrite(LPCTSTR Err=NULL);//設定系統錯誤代碼-檔案寫入		
	void                       SetSystemExceptionCode(DWORD Code, LPCTSTR Err=NULL);//設定系統錯誤代碼-系統		
	void                       SetSystemExceptionCode(DWORD Code, bool bCheckOK, LPCTSTR Err=NULL);//設定系統錯誤代碼-系統		
	//---------------------------------------------------------------------------------//	
	void                       SetThreadExceptionCode(DWORD Code, LPCTSTR Err=NULL);//設定執行緒錯誤代碼
	void                       SetThreadExceptionCode(DWORD Code, bool bCheckOK, LPCTSTR Err=NULL);//設定執行緒錯誤代碼
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeDeviceExceptionCode(DWORD Code, LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-建立
	void                       SetBarcodeDeviceExceptionCode_Param(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-參數
	void                       SetBarcodeDeviceExceptionCode_FileRead(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-檔案讀取
	void                       SetBarcodeDeviceExceptionCode_FileWrite(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-檔案寫入
	void                       SetBarcodeDeviceExceptionCode_Create(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-建立
	void                       SetBarcodeDeviceExceptionCode_Connect(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-連線
	void                       SetBarcodeDeviceExceptionCode_Start(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-開始
	void                       SetBarcodeDeviceExceptionCode_Decode(LPCTSTR Err=NULL);//設定系統錯誤代碼-條碼機-解碼		
	//---------------------------------------------------------------------------------//	
	void                       SetErrorString(const char *String);//設定錯誤訊息
	void                       SetErrorString(const wchar_t *String);//設定錯誤訊息
	CString                    GetErrorString();//取得錯誤訊息
	CString                    GetErrorStringRaw();//取得錯誤訊息
	void                       SetErrorString_Locked(const char *String);//設定錯誤訊息-鎖住設定
	void                       SetErrorString_Locked(const wchar_t *String);//設定錯誤訊息-鎖住設定
	//---------------------------------------------------------------------------------//	
	void                       SetErrorString_OnlineRun(const char *String);//設定錯誤訊息-線上運作
	void                       SetErrorString_OnlineRun(const wchar_t *String);//設定錯誤訊息-線上運作
	CString                    GetErrorString_OnlineRun();//取得錯誤訊息-線上運作
	//---------------------------------------------------------------------------------//
	bool                       CreateNewFolder(LPCTSTR Folder);//建立AOI資料夾
	//---------------------------------------------------------------------------------//
	void                       SetAOIDirectory(LPCTSTR value);//設定主要資料夾
	LPCSTR                     GetAOIDirectoryA() const;//取回主要資料夾
	LPCWSTR                    GetAOIDirectoryW() const;//取回主要資料夾
	LPCTSTR                    GetAOIDirectory() const;//取回主要資料夾
	//---------------------------------------------------------------------------------//	
	void                       SetAOILogDirectory(LPCTSTR value);//設定訊息資料夾
	LPCSTR                     GetAOILogDirectoryA() const;//取回訊息資料夾
	LPCWSTR                    GetAOILogDirectoryW() const;//取回訊息資料夾
	LPCTSTR                    GetAOILogDirectory() const;//取回訊息資料夾
	CString                    GetAOILogOperDirectory() const;//取回訊息操作資料夾
	CString                    GetAOILogControlCenterDirectory() const;//取回訊息中控資料夾
	//---------------------------------------------------------------------------------//
	bool                       CreateAOITempDirectory();//建立暫存資料夾
	LPCSTR                     GetAOITempDirectoryA() const;//取回暫存資料夾	
	LPCWSTR                    GetAOITempDirectoryW() const;//取回暫存資料夾		
	LPCTSTR                    GetAOITempDirectory() const;//取回暫存資料夾	
	LPCTSTR                    GetAOITempProjectDirectory() const;//取回暫存專案資料夾	
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetAOIResultDirectory() const;//取回結果資料夾		
	LPCTSTR                    GetAOIProjectDirectory() const;//取回專案資料夾		
	LPCTSTR                    GetOpenProjectDirectory() const;//取回開啟專案資料夾		
	void                       SetOpenProjectDirectory(LPCTSTR val);//設定開啟專案資料夾
	LPCTSTR                    GetAOIServerFolder() const;//取回伺服器資料夾	
	LPCTSTR                    GetAOIServerProjectFolder() const;//取回伺服器專案資料夾	
	LPCTSTR                    GetAOIServerLibraryFolder();//取回伺服器專案資料夾		
	LPCTSTR                    GetAOIOnlineTuningDirectory() const;//取回線上調機資料夾	
	LPCTSTR                    GetAOIOnlineBarcodeDirectory() const;//取回線上條碼資料夾	
	LPCTSTR                    GetAOIOnlineOfflineDirectory() const;//取回線上離線編程資料夾	
	LPCTSTR                    GetAOIStaticDataDirectory() const;//取回統計數據資料夾	
	LPCTSTR                    GetAOITextReportDirectory() const;//取回文字報告資料夾
	LPCTSTR                    GetAOIProjectRawDirectory() const;//取回專案原圖資料夾
	LPCTSTR                    GetAOIProjectDebugDirectory() const;//取回專案除錯資料夾	
	LPCTSTR                    GetAOIProjectTestMapDirectory() const;//取回專案儲存底圖資料夾
	LPCTSTR                    GetAOIProjectTestMapDirectoryTemp() const;//取回專案儲存底圖資料夾-暫存
	LPCTSTR                    GetAOIProjectTestMapDirectoryBackup() const;//取回專案儲存底圖資料夾-備份
	//---------------------------------------------------------------------------------//
	LPCSTR                     GetAOIDefaultModelDirectoryA() const;//取回模組預設視窗訊息資料夾
	LPCWSTR                    GetAOIDefaultModelDirectoryW() const;//取回模組預設視窗資料夾	
	LPCTSTR                    GetAOIDefaultModelDirectory() const;//取回模組預設視窗訊息資料夾
	//---------------------------------------------------------------------------------//
	AOI_CUSTOMER_ID            GetAOICustomerID() const;//取得AOI客戶編號
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetAIFileExportFolder() const;//取回AI檔案資料夾
	LPCTSTR                    GetAIImageExportFolder() const;//取回AI影像資料夾
	CString                    CreateAIImageTempFolder(LPCTSTR ProjectName, LPCTSTR DateTime) const;//建立AI影像暫存資料夾	
	//---------------------------------------------------------------------------------//
	DWORD                      GetComputerCPUCoreNumber() const;
	//---------------------------------------------------------------------------------//
	void                       CalcThreadAffinityMask();//計算執行緒的CPU親和性
	bool                       UpdateThreadAffinityMask();//更新執行緒的CPU親和性
	DWORD_PTR                  GetThreadAffinityMask() const;//取得執行緒的CPU親和性
	//---------------------------------------------------------------------------------//
	//主要顯示視窗的控制編號
	void                       SetMainViewWndID(UINT value) { m_MainViewWndID = value; }
	UINT                       GetMainViewWndID() const { return m_MainViewWndID; }
	//---------------------------------------------------------------------------------//	
	//Ribbon的分類引數
	bool                       CheckRibbonCategoryIndex_OnlineFormView() const;	
	void                       SetRibbonCategoryIndex(UINT value) { m_RibbonCategoryIndex = value; }
	UINT                       GetRibbonCategoryIndex() const { return m_RibbonCategoryIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetEditImagePageWndID(UINT value) { m_EditImagePageWndID = value; }
	UINT                       GetEditImagePageWndID() const { return m_EditImagePageWndID; }
	//---------------------------------------------------------------------------------//	
	//視窗特殊參數
	bool                       SaveWndUIParam(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR Default);
	bool                       SaveWndUIParamFn(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR Default);
	bool                       LoadWndUIParam(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR Default, CString &String);
	bool                       LoadWndUIParamFn(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR Default, CString &String);
	//---------------------------------------------------------------------------------//
	//多國語系
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	CString                    LoadMultiLanguageString(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR Default);
	CString                    LoadMultiLanguageString_OnlineFunc(LPCTSTR KeyName, LPCTSTR Default);	
	CString                    LoadMultiLanguageString_OnlineInput(LPCTSTR KeyName, LPCTSTR Default);	
	CString                    LoadMultiLanguageString_OnlineCalibration(LPCTSTR KeyName, LPCTSTR Default);	
	bool                       GetUILanguageString(LPCTSTR Section, LPCTSTR Key, LPCTSTR Default, CString &String);//取得視窗文字
	void                       SwitchMultiLanguageMenuGroup(LPCTSTR Section, CMenu &Menu);
	void                       SwitchMultiLanguageMenu(CMenu &Menu, UINT MenuID);
	void                       SwitchMultiLanguageMenu_Draw3D(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Image(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Project(CMenu &Menu);
	void                       SwitchMultiLanguageMenu_Fd(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Mark(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Panel(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Board(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Barcode(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Component(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Main(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_Model(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_ModelAdd(CMenu &Menu);
	void                       SwitchMultiLanguageMenu_ModelAutoAdd(CMenu &Menu);
	void                       SwitchMultiLanguageMenu_ModelEdit(CMenu &Menu);
	void                       SwitchMultiLanguageMenu_ProjectMap(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_PartNumber(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_LibraryModel(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_LinkProjectColor(CMenu &Menu);	
	void                       SwitchMultiLanguageMenu_SendProjectColor(CMenu &Menu);		
	bool                       SwitchMultiLanguageWnd(CWnd *WndPtr, LPCTSTR Section, UINT ID, LPCTSTR Text, bool bCtrlID);
	//---------------------------------------------------------------------------------//		
	void                       SetActiveLaneID(LANE_ID value);//設定軌道編號		
	LANE_ID                    GetActiveLaneID() const; //取得軌道編號	
	//---------------------------------------------------------------------------------//
	void                       SetExceptionLaneID(LANE_ID value);//設定異常的軌道編號	
	LANE_ID                    GetExceptionLaneID() const; //取得異常的軌道編號	
	//---------------------------------------------------------------------------------//
	bool                       GetMultiDistrictMode();//取得分段模式
	void                       SetActiveDistrictID(DISTRICT_ID value);//設定分段編號
	DISTRICT_ID                GetActiveDistrictID() const; //取得分段編號
	bool                       MovePCBToDistrictID(LANE_ID LaneID, DISTRICT_ID value);//移動PCB板至分段編號	
	//---------------------------------------------------------------------------------//
	bool                       VerifyJsonString(LPCTSTR Text);//驗證Json字串
	bool                       CheckProjectParamNeedToVerifyJsonString(PROJECT_PARAM_ID ParamID) const;//確認專案參數需要驗證Json字串	
	//---------------------------------------------------------------------------------//
	MACHINE_CAMERA_SIDE        GetMachineCameraSide() const;//取得機台相機方位
	//---------------------------------------------------------------------------------//
	void                       SetIsNeedGrabFiducial(bool value);//設定是否取像定位點
	bool                       GetIsNeedGrabFiducial() const; //取得是否取像定位點
	void                       SetIsNeedGrabFiducial(bool value, LANE_ID LaneID);//設定是否取像定位點
	bool                       GetIsNeedGrabFiducial(LANE_ID LaneID) const; //取得是否取像定位點
	//---------------------------------------------------------------------------------//	
	void                       SetIsNeedResetLightCtrlDLP(bool value); //設定是否需要重設燈源控制板-DLP
	bool                       GetIsNeedResetLightCtrlDLP() const; //取得是否需要重設燈源控制板-DLP
	//---------------------------------------------------------------------------------//
	void                       SetIsNeedReCheckPCBInside(bool value); //是否需要重新確認PCB板是否在裡面
	bool                       GetIsNeedReCheckPCBInside() const; //是否需要重新確認PCB板是否在裡面
	//---------------------------------------------------------------------------------//	
	void                       SetTurnOnOffTargetCapTickCount(DWORD value); //設定開關塊規上蓋的TickCount
	DWORD                      GetTurnOnOffTargetCapTickCount() const; //取得開關塊規上蓋的TickCount
	DWORD                      GetTurnOnOffTargetCapDelayTime() const;//取得開關塊規上蓋的延遲時間
	//---------------------------------------------------------------------------------//
	void                       SetIsOnInspection(bool value); //設定是否正在檢測中
	bool                       GetIsOnInspection() const; //取得是否正在檢測中
	//---------------------------------------------------------------------------------//
	void                       ToggleIsEnhanceDisplayImage();//切換是否強化顯示影像
	void                       SetIsEnhanceDisplayImage(bool value); //設定是否強化顯示影像
	bool                       GetIsEnhanceDisplayImage() const; //取得是否強化顯示影像
	//---------------------------------------------------------------------------------//	
	void                       SetIsOnlineCheckSystemReady(bool value); //設定是否在線時可以確認系統狀態
	bool                       GetIsOnlineCheckSystemReady() const; //取得是否在線時可以確認系統狀態
	//---------------------------------------------------------------------------------//	
	bool                       CheckIsLockSystemParameter(); //確認是否鎖住系統參數
	void                       SetIsLockSystemParameter(bool value); //設定是否鎖住系統參數
	bool                       GetIsLockSystemParameter() const; //取得是否鎖住系統參數	
	//---------------------------------------------------------------------------------//	
	bool                       GetSeparateDLP2ExpTable() const;//取得是否分開DLP2次曝光表格
	bool                       CheckReSortCameraImage(SLICE_FUNC_MODE SliceFuncMode) const;//取得是否重新排序相機影像
	bool                       CheckUseSeparateDLP2ExpTable(SLICE_FUNC_MODE SliceFuncMode) const;//取得是否分開DLP2次曝光表格
	//---------------------------------------------------------------------------------//		
	bool                       GetIsUseProjectSystemParameter() const;//取得是否使用遠端系統參數	
	//---------------------------------------------------------------------------------//	
	bool                       GetProjectLocalFolderEnabled() const;//取得專案是否使用本機資料夾
	//---------------------------------------------------------------------------------//	
	bool                       GetPartialCopyProjectLibrary() const;//取得是否部分複製專案資料庫
	//---------------------------------------------------------------------------------//	
	bool                       GetAutoArrangeModelBkImageFiles() const;//取得是否自動重新整理模組底圖檔案
	//---------------------------------------------------------------------------------//			
	bool                       GetAutoBypassGrab3DFrame() const;//取得是否自動跳過取3D影像
	//---------------------------------------------------------------------------------//	
	MULTI_PROJECT_TEST_ORDER_MODE CheckMultiProjectTestOrderMode();//確認多專案檢測順序模式
	MULTI_PROJECT_TEST_ORDER_MODE GetMultiProjectTestOrderMode() const;//取得多專案檢測順序模式
	void                          SetMultiProjectTestOrderMode(MULTI_PROJECT_TEST_ORDER_MODE val);//設定多專案檢測順序模式
	//---------------------------------------------------------------------------------//
	bool                          GetMultiProjectTestResetDone(LANE_ID LaneID) const;//取得多專案檢測復歸完成
	void                          SetMultiProjectTestResetDone(bool val, LANE_ID LaneID);//設定多專案檢測復歸完成
	//---------------------------------------------------------------------------------//
	void                       SetIsLockUIWnd(bool value); //設定是否鎖住視窗
	bool                       GetIsLockUIWnd() const; //取得是否鎖住視窗
	//---------------------------------------------------------------------------------//
	void                       SetIsRepeatTest(bool value); //設定是否重複檢測
	bool                       GetIsRepeatTest() const; //取得是否重複檢測
	//---------------------------------------------------------------------------------//
	void                       SetIsRepeatTestUI(bool value); //設定是否重複檢測-介面
	bool                       GetIsRepeatTestUI() const; //取得是否重複檢測-介面
	//---------------------------------------------------------------------------------//		
	void                       SetIsAskRepeatTest(bool value); //設定是否詢問重複檢測次數
	bool                       GetIsAskRepeatTest() const; //取得是否詢問重複檢測次數
	//---------------------------------------------------------------------------------//		
	void                       SetIsOnAutoRetry(bool value); //設定是否正在自動重測
	bool                       GetIsOnAutoRetry() const; //取得是否正在自動重測
	//---------------------------------------------------------------------------------//	
	void                       SetIsEnableAutoRetry(bool value); //設定是否啟用自動重測
	bool                       GetIsEnableAutoRetry() const; //取得是否啟用自動重測
	//---------------------------------------------------------------------------------//	
	void                       SetIsIgnoreAutoRetry(bool value); //設定是否忽略自動重測
	bool                       GetIsIgnoreAutoRetry() const; //取得是否忽略自動重測
	//---------------------------------------------------------------------------------//	
	bool                       CheckNeedAutoRetry(ONLINE_STATE_MODE OnlineStateMode, TASK_MODE &TaskMode);//確認是否需要自動重測
	int                        AddAutoRetryCount(); //累加自動重測次數
	void                       SetAutoRetryCount(int value); //設定自動重測次數
	int                        GetAutoRetryCount() const; //取得自動重測次數
	bool                       ExecAutoRetry(TASK_MODE &TaskMode);//執行自動重測
	//---------------------------------------------------------------------------------//
	size_t                     AddRepeatedTestCount(); //累加重複檢測數量
	void                       SetRepeatedTestCount(size_t value); //設定重複檢測數量
	size_t                     GetRepeatedTestCount() const; //取得重複檢測數量	
	//---------------------------------------------------------------------------------//	
	void                       SetRepeatedTestMaxCount(size_t value); //設定重複檢測數量上限
	size_t                     GetRepeatedTestMaxCount() const; //取得重複檢測數量上限
	//---------------------------------------------------------------------------------//	
	void                       SetIsNeedResetOKNGSignal(bool value); //設定需要重置OK-NG訊號
	bool                       GetIsNeedResetOKNGSignal() const; //取得需要重置OK-NG訊號
	bool                       ResetPCBOKNGSignal(LANE_ID LaneID, DEFECT_HANDLE_MODE HandleMode);//重置PCB板OK-NG訊號
	//---------------------------------------------------------------------------------//	
	void                       SetIsModelWndGroupSelChange(bool value); //設定是否模組檢測框群組選取切換
	bool                       GetIsModelWndGroupSelChange() const; //取得是否模組檢測框群組選取切換
	//---------------------------------------------------------------------------------//	
	void                       SetIsSystemReleased(bool value); //設定是否系統釋放
	bool                       GetIsSystemReleased() const; //取得是否系統釋放
	//---------------------------------------------------------------------------------//	
	void                       SetIsSystemReleasedDone(bool value); //設定是否系統釋放結束
	bool                       GetIsSystemReleasedDone() const; //取得是否系統釋放結束
	//---------------------------------------------------------------------------------//
	void                       ResetSystemException();//清除系統異常
	//---------------------------------------------------------------------------------//
	bool                       CheckIsOnlineRunLock() const;//確認線上運作是否鎖住
	//---------------------------------------------------------------------------------//
	void                       SetIsWaitUserInput(bool value); //設定是否等待使用者輸入
	bool                       GetIsWaitUserInput() const; //取得是否等待使用者輸入
	//---------------------------------------------------------------------------------//	
	void                       SetIsStopUserInput(bool value); //設定是否停止使用者輸入
	bool                       GetIsStopUserInput() const; //取得是否停止使用者輸入
	//---------------------------------------------------------------------------------//	
	void                       AddWaitUserInputCount(); //累加使用者輸入數量
	void                       ResetWaitUserInputCount(); //清除使用者輸入數量
	void                       ReleaseWaitUserInputCount(); //減少使用者輸入數量	
	bool                       CheckWaitUserInputCountZero() const; //確認使用者輸入數量為零
	//---------------------------------------------------------------------------------//	
	void                       SetIsOnlineRunLock(bool value);//設定是否鎖住系統異常
	bool                       GetIsOnlineRunLock() const; //取得是否鎖住系統異常
	//---------------------------------------------------------------------------------//
	void                       SetIsSystemException(bool value);//設定是否系統異常
	bool                       GetIsSystemException() const; //取得是否系統異常	
	//---------------------------------------------------------------------------------//
	void                       SetIsGetSystemExceptionMsg(bool value);//設定是否系統異常錯誤碼
	bool                       GetIsGetSystemExceptionMsg() const; //取得是否系統異常錯誤碼	
	//---------------------------------------------------------------------------------//
	bool                       GetIsOfflineHostTuningVersion() const;//取得是否為離線本機調適版本
	bool                       GetIsOfflineRemoteTuningVersion() const;//取得是否為離線遠端調適版本
	//---------------------------------------------------------------------------------//
	bool                       GetSystemMultiFdLight() const;//多定位點燈源使用
	//---------------------------------------------------------------------------------//
	SAVE_SPC_FILE_MODE         GetSaveProjectSpcFileMode() const;//取得儲存專案Spc檔案模式	
	//---------------------------------------------------------------------------------//
	bool                       SetJetMemoryLevel();//設定記憶體層級 
	bool                       SetCudaMemoryLevel();//設定Cuda記憶體層級 
	//---------------------------------------------------------------------------------//
	void                       SetThreadGrabMode(THREAD_GRAB_MODE value);//設定執行緒取像模式
	THREAD_GRAB_MODE           GetThreadGrabMode() const; //取得執行緒取像模式	
	//---------------------------------------------------------------------------------//
	void                       SetTaskMode(TASK_MODE value);//設定任務模式
	TASK_MODE                  GetTaskMode() const; //取得任務模式	
	//---------------------------------------------------------------------------------//
	void                       SetOnlineTaskCancel(bool value);//設定任務取消
	bool                       GetOnlineTaskCancel() const; //取得任務取消
	//---------------------------------------------------------------------------------//
	void                       SetOnlineTaskState(TASK_STATE_MODE value);//設定任務階段
	TASK_STATE_MODE            GetOnlineTaskState() const; //取得任務階段
	bool                       CheckStopOnlineTask() const;//確認停止任務	
	//---------------------------------------------------------------------------------//
	void                       SetOnlineLaneTaskState(LANE_ID LaneID, LANE_STATE_MODE value);//設定任務階段
	LANE_STATE_MODE            GetOnlineLaneTaskState(LANE_ID LaneID) const; //取得任務階段
	bool                       CheckStopOnlineLaneTask(LANE_ID LaneID) const;//確認停止任務	
	//---------------------------------------------------------------------------------//
	void                       SetOnlineLaneTaskState_LA(LANE_STATE_MODE value);//設定任務階段-A軌
	LANE_STATE_MODE            GetOnlineLaneTaskState_LA() const; //取得任務階段-A軌
	bool                       CheckStopOnlineLaneTask_LA() const;//確認停止任務-A軌
	//---------------------------------------------------------------------------------//
	void                       SetOnlineLaneTaskState_LB(LANE_STATE_MODE value);//設定任務階段-B軌
	LANE_STATE_MODE            GetOnlineLaneTaskState_LB() const; //取得任務階段-B軌
	bool                       CheckStopOnlineLaneTask_LB() const;//確認停止任務-B軌
	//---------------------------------------------------------------------------------//
	void                       SetLanePCBOutMode_LA(PCB_OUT_MODE value);//A軌道PCB出板模式
	void                       SetLanePCBOutMode_LB(PCB_OUT_MODE value);//B軌道PCB出板模式
	void                       SetLanePCBOutMode(LANE_ID LaneID, PCB_OUT_MODE value);//PCB出板模式
	PCB_OUT_MODE               GetLanePCBOutMode_LA() const; //A軌道PCB出板模式
	PCB_OUT_MODE               GetLanePCBOutMode_LB() const; //B軌道PCB出板模式
	PCB_OUT_MODE               GetLanePCBOutMode(LANE_ID LaneID) const; //PCB出板模式
	//---------------------------------------------------------------------------------//	
	void                       SetLanePCBOutModeRunning(LANE_ID LaneID, PCB_OUT_MODE value);//PCB出板模式-執行中
	PCB_OUT_MODE               GetLanePCBOutModeRunning(LANE_ID LaneID) const; //PCB出板模式-執行中
	//---------------------------------------------------------------------------------//	
	void                       SetLaneDefectHandleMode_LA(DEFECT_HANDLE_MODE value); //A軌道檢出異常處理模式
	void                       SetLaneDefectHandleMode_LB(DEFECT_HANDLE_MODE value); //B軌道檢出異常處理模式
	void                       SetLaneDefectHandleMode(LANE_ID LaneID, DEFECT_HANDLE_MODE value);//檢出異常處理模式
	//---------------------------------------------------------------------------------//		
	DEFECT_HANDLE_MODE         GetLaneDefectHandleMode_LA() const; //A軌道檢出異常處理模式
	DEFECT_HANDLE_MODE         GetLaneDefectHandleMode_LB() const; //B軌道檢出異常處理模式	
	DEFECT_HANDLE_MODE         GetLaneDefectHandleMode(LANE_ID LaneID) const; //檢出異常處理模式
	//---------------------------------------------------------------------------------//	
	void                       SetEnableConveyerPreRun(int value);//軌道提前運轉功能
	int                        GetEnableConveyerPreRun() const; //軌道提前運轉功能
	//---------------------------------------------------------------------------------//	
	bool                       GetUIEnablePCBOutButton() const;//取得UI啟用PCB出板按鈕
	//---------------------------------------------------------------------------------//	
	COPY_HUGE_FILES_MODE       GetCopyHugeFilesMode() const;//取得複製大量檔案模式
	//---------------------------------------------------------------------------------//
	int                        GetStretchBltMode(double ZoomValue) const;//取得縮放Blt模式
	//---------------------------------------------------------------------------------//	
	//調適的視窗命令
	void                       SetRibbonTuneGroupCmdID(UINT value);
	UINT                       GetRibbonTuneGroupCmdID() const { return m_RibbonTuneGroupCmdID; }	
	LPCTSTR                    GetRibbonTuneGroupCmdText() const { return m_RibbonTuneGroupCmdText; }		
	//---------------------------------------------------------------------------------//
	//預設檢測框的視窗命令
	void                       SetRibbonDefaultWndCmdID(UINT value);
	UINT                       GetRibbonDefaultWndCmdID() const { return m_RibbonDefaultWndCmdID; }	
	LPCTSTR                    GetRibbonDefaultWndCmdText() const { return m_RibbonDefaultWndCmdText; }		
	//---------------------------------------------------------------------------------//
	//檢測模式, 線上檢測或者調適檢測
	void                       SetInspectingMode(INSPECTING_MODE value) { m_InspectingMode = value; }
	INSPECTING_MODE            GetInspectingMode() const { return m_InspectingMode; }	
	//---------------------------------------------------------------------------------//	
	void                       SetInspectionDrawing(bool value) { m_InspectionDrawing = value; }//設定檢測繪圖
	bool                       GetInspectionDrawing() const { return m_InspectionDrawing; } //取得檢測繪圖
	//---------------------------------------------------------------------------------//
	//模組掛載的物件
	void                       SetModelAttachedObj(MODEL_ATTACHED_OBJ value) { m_ModelAttachedObj = value; }
	MODEL_ATTACHED_OBJ         GetModelAttachedObj() const { return m_ModelAttachedObj; }
	//---------------------------------------------------------------------------------//
	//載入CAD檔案內容模式-開啟新專案使用	
	void                       SetLoadCADFileContentMode(CAD_FILE_CONTENT_MODE value) { m_LoadCADFileContentMode = value; }
	CAD_FILE_CONTENT_MODE      GetLoadCADFileContentMode() const { return m_LoadCADFileContentMode; }
	//---------------------------------------------------------------------------------//
	void                       SetCallbackWnd(HWND value);//設定回傳的視窗
	HWND                       GetCallbackWnd() const; //取得回傳的視窗	
	//---------------------------------------------------------------------------------//	
	void                       SetMainFrameWnd(HWND value);//設定主要框架視窗
	HWND                       GetMainFrameWnd() const; //取得主要框架視窗
	//---------------------------------------------------------------------------------//	
	bool                       HideGlobalWndForInspection();//隱藏全域視窗-檢測
	//---------------------------------------------------------------------------------//	
	bool                       SendCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳視窗 
	bool                       PostCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳視窗
	//---------------------------------------------------------------------------------//
	bool                       FilterMainFrameWndMessage(UINT message);//過濾訊息給回傳主框架視窗 
	bool                       SendMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳主框架視窗 
	bool                       PostMainFrameWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳主框架視窗
	//---------------------------------------------------------------------------------//
	bool                       SendParentWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	bool                       PostParentWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	//---------------------------------------------------------------------------------//
	bool                       CheckApplicationVersion(CString &Filename, CString &VersionS);//確認系統版本
	bool                       SystemReleaseStep(SYSTEM_INITIAL_STEP Step);//系統初始化
	bool                       SystemInitializeStep(SYSTEM_INITIAL_STEP Step);//系統釋放
	//---------------------------------------------------------------------------------//	
	void                       BuildCameraIDList(std::vector<CAMERA_ID> &List);//建立相機編號列表 
	IMAGE_SIZE                 GetCameraImageW(CAMERA_ID CameraID); //取得相機影像寬度
	IMAGE_SIZE                 GetCameraImageH(CAMERA_ID CameraID); //取得相機影像長度 
	CAMERA_IMAGE_MODE          GetCameraImageMode(CAMERA_ID CameraID); //取得相機影像模式	
	double                     GetCameraResolutionX(CAMERA_ID CameraID); //取得相機影像解析度
	double                     GetCameraResolutionY(CAMERA_ID CameraID); //取得相機影像解析度 		
	bool                       GetCameraImageInfo(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, TPOINT2D &Res); //取得相機影像資訊
	bool                       GetCameraImageInfo(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, double &ResX, double &ResY); //取得相機影像資訊
	//---------------------------------------------------------------------------------//	
	int                        GetFovResolutionMode() const;//取得視野解析度模式
	bool                       SetFovResolutionMode(int Mode);//設定視野解析度模式
	double                     GetFovResolutionValue() const;//取得視野解析度-理論解析度
	double                     GetFovResolutionValueFn(int Mode) const;//取得視野解析度-理論解析度
	double                     GetFovSizeRealW() const;//取得視野實際範圍-寬度
	double                     GetFovSizeRealH() const;//取得視野實際範圍-長度
	bool                       GetFovStageRegionReal(TREGION4D &Region);//取得視野實際範圍
	bool                       GetFovStageRegionInner(TREGION4D &Region);//取得視野內部範圍
	bool                       GetFovStageRegionOuter(TREGION4D &Region);//取得視野外部範圍	
	bool                       GetFovSizeUsed(double &FovW, double &FovH);//取得視野使用範圍-依專案切換
	bool                       GetFovSizeReal(double &FovW, double &FovH) const;//取得視野實際範圍	
	bool                       GetFovSizeInner(double &FovW, double &FovH) const;//取得視野內部範圍	
	bool                       GetFovSizeOuter(double &FovW, double &FovH) const;//取得視野外部範圍		
	double                     GetFovSizeByMode(double Size, FIELD_SIZE_MODE szMode) const;//取得尺寸模式下的大小範圍	
	bool                       GetFovImageInfo(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, double &Resolution) const;//取得視野影像訊息
	bool                       ModifyFovSizeUsed(FIELD_SIZE_MODE szModeW, FIELD_SIZE_MODE szModeH, double &FovW, double &FovH);//修正視野使用範圍	
	//---------------------------------------------------------------------------------//
	size_t                     AnalsysFovFieldCount();//取得FOV所需要的區域張數	
	//---------------------------------------------------------------------------------//
	bool                       MapImageSizeToReal(CAMERA_ID CameraID, const RECT &Rect, TSIZE2D &ImageSize, const THREAD_ID ThreadID=THREAD_01);//影像對應到物理尺寸	
	bool                       MapImageSizeToReal(const TPOINT2D &Res, const RECT &Rect, TSIZE2D &ImageSize, const THREAD_ID ThreadID=THREAD_01);//影像對應到物理尺寸	
	bool                       MapImageSizeToReal(CAMERA_ID CameraID, int ImageW, int ImageH, TSIZE2D &ImageSize, const THREAD_ID ThreadID=THREAD_01);//影像對應到物理尺寸	
	bool                       MapImageSizeToReal(const TPOINT2D &Res, int ImageW, int ImageH, TSIZE2D &ImageSize, const THREAD_ID ThreadID=THREAD_01);//影像對應到物理尺寸	
	//---------------------------------------------------------------------------------//	
	bool                       MapStagePtToCamera(CAMERA_ID CameraID, const TPOINT2D &StagePos, const TPOINT2D &StageCp, TPOINT2D &ImagePos, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像
	bool                       MapStagePtToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TPOINT2D &StagePos, const TPOINT2D &StageCp, TPOINT2D &ImagePos, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像
	bool                       MapStageRectToCamera(CAMERA_ID CameraID, const TRECT4D &StageRect, const TPOINT2D &StageCp, TRECT4D &ImageRect, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像
	bool                       MapStageRectToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TRECT4D &StageRect, const TPOINT2D &StageCp, TRECT4D &ImageRect, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像
	bool                       MapStageRegionToCamera(CAMERA_ID CameraID, const TREGION4D &StageRgn, const TPOINT2D &StageCp, TREGION4D &ImageRgn, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像	
	bool                       MapStageRegionToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TREGION4D &StageRgn, const TPOINT2D &StageCp, TREGION4D &ImageRgn, const THREAD_ID ThreadID=THREAD_01);//機台對應到影像
	bool                       MapStageCornerToCamera(CAMERA_ID CameraID, const TPOINT2D CornerPt[], const TPOINT2D &StageCp, TPOINT2D ImagePt[], const THREAD_ID ThreadID=THREAD_01);//機台4端點對應到影像四點
	bool                       MapStageCornerToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TPOINT2D CornerPt[], const TPOINT2D &StageCp, TPOINT2D ImagePt[], const THREAD_ID ThreadID=THREAD_01);//機台4端點對應到影像四點
	bool                       MapStageCornerToCameraPt(CAMERA_ID CameraID, const TPOINT2D CornerPt[],  const TPOINT2D &StageCp, POINT ImagePt[], const THREAD_ID ThreadID=THREAD_01);//機台4端點對應到影像四點	
	
	bool                       MapCameraRectToStage(CAMERA_ID CameraID, const TRECT4D &ImageRect, const TPOINT2D &StageCp, TRECT4D &StageRect, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraRectToStage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TRECT4D &ImageRect, const TPOINT2D &StageCp, TRECT4D &StageRect, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraRegionToStage(CAMERA_ID CameraID, const TREGION4D &ImageRgn, const TPOINT2D &StageCp, TREGION4D &StageRgn, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraRegionToStage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TREGION4D &ImageRgn, const TPOINT2D &StageCp, TREGION4D &StageRgn, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraPtToStage(CAMERA_ID CameraID, const TPOINT2D &ImagePt, const TPOINT2D &StageCp, TPOINT2D &StagePt, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraPtToStage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TPOINT2D &ImagePt, const TPOINT2D &StageCp, TPOINT2D &StagePt, const THREAD_ID ThreadID=THREAD_01);//相機影像對應到機台
	bool                       MapCameraCornerToStage(CAMERA_ID CameraID, const TPOINT2D CornerPt[], const TPOINT2D &StageCp, TPOINT2D StagePt[], const THREAD_ID ThreadID=THREAD_01);//相機影像4點對應到機台4點	

	bool                       MapImageOffsetToCad(const TPOINT2D &ImgPos, const TPOINT2D &Res, TPOINT2D &CadPos);//影像偏差量轉成Cad偏差量

	bool                       MapStageAngleToCad(double StageAngle, double &CadAngle);
	bool                       MapStageOffsetPtToCad(const TPOINT2D &PosStage, TPOINT2D &PosCad);
	bool                       MapStageOffsetRgnToCad(const TREGION4D &RgnStage, TREGION4D &RgnCad);
	bool                       MapCadAngleToStage(double CadAngle, double &StageAngle);
	bool                       MapCadOffsetPtToStage(const TPOINT2D &PosCad, TPOINT2D &PosStage);	
	bool                       MapCadOffsetPtToStage(double CadX, double CadY, double &StageX, double &StageY);	
	bool                       MapCadOffsetRgnToStage(const TREGION4D &RgnCad, TREGION4D &RgnStage);
	bool                       MapCadOffsetCornerPtsToStage(const TPOINT2D CornerPtsCad[4], TPOINT2D CornerPtsStage[4]);	
	bool                       MapCadPtToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TPOINT2D &CadPt, const TPOINT2D &CadCp, TPOINT2D &ImagePt, const THREAD_ID ThreadID=THREAD_01);//Cad對應到影像	
	bool                       MapCadRegionToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TREGION4D &CadRgn, const TPOINT2D &CadCp, TREGION4D &ImageRgn, const THREAD_ID ThreadID=THREAD_01);//Cad對應到影像
	bool                       MapCadCornerToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const TPOINT2D CadCorner[], const TPOINT2D &CadCp, TPOINT2D ImageCorner[], const THREAD_ID ThreadID=THREAD_01);//Cad對應到影像
	bool                       MapCadPolygonToCamera(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &Res, const std::vector<TPOINT2D> &CadPolygon, const TPOINT2D &CadCp, std::vector<TPOINT2D> &ImagePolygon, const THREAD_ID ThreadID=THREAD_01);//Cad對應到影像
	//---------------------------------------------------------------------------------//	
	bool                       PreInitResource();//初始化資源-特定需求在建立視窗前呼叫
	bool                       InitialResource();//初始化資源
	bool                       ReleaseResource();//釋放資源
	//---------------------------------------------------------------------------------//
	bool                       RemoveAllPtrList();//移除所有指標列表	
	bool                       CloneAllPtrList(CAOIProject *ProjectPtr);//複製所有指標列表	
	//---------------------------------------------------------------------------------//		
	size_t                     GetRgnPtrCount() const;//取得檢測區域指標列表數量
	CAOIRgn*                   GetRgnPtr(size_t idx, bool check);//取得檢測區域指標
	bool                       RemoveAllRgns();//移除所有檢測區域
	//---------------------------------------------------------------------------------//		
	size_t                     GetFovPtrCount() const;//取得視野指標列表數量
	CAOIFov*                   GetFovPtr(size_t idx, bool check);//取得視野指標
	bool                       RemoveAllFovs();//移除所有視野
	//---------------------------------------------------------------------------------//		
	size_t                     GetSlicePtrCount() const;//取得相機像指標列表數量
	CAOISlice*                 GetSlicePtr(size_t idx, bool check);//取得相機像指標
	bool                       RemoveAllSlices();//移除所有相機像
	//---------------------------------------------------------------------------------//
	size_t                     GetFramePtrCount() const;//取得畫面影像指標列表數量
	CAOIFrame*                 GetFramePtr(size_t idx, bool check);//取得畫面影像指標
	bool                       RemoveAllFrames();//移除所有區塊影像
	//---------------------------------------------------------------------------------//	
	size_t                     GetFieldPtrCount() const;//取得區域影像指標列表數量
	CAOIField*                 GetFieldPtr(size_t idx, bool check);//取得區域影像指標
	bool                       RemoveAllFields();//移除所有區域影像
	//---------------------------------------------------------------------------------//			
	bool                       CheckObjListInitialized();//確認Obj列表是否初始化
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetThreadStateModeText(THREAD_STATE_MODE State);
	LPCTSTR                    GetThreadCommandModeText(THREAD_COMMAND_MODE State);
	//---------------------------------------------------------------------------------//	
	bool                       GetIsAllCalcThreadStop() const;//取得是否所有計算執行緒停止
	void                       SetIsAllCalcThreadStop(bool Value);//設定是否所有計算執行緒停止
	//---------------------------------------------------------------------------------//	
	void                       ResetIsAnyCalcThreadWorking();//復歸計算的執行緒工作中
	bool                       GetIsAnyCalcThreadWorking() const;//取得計算的執行緒工作中
	void                       SetIsAnyCalcThreadWorking(bool Value);//設定計算的執行緒工作中
	//---------------------------------------------------------------------------------//
	bool                       CreateAllThread();//建立所有執行緒
	bool                       DeleteAllThread();//刪除所有執行緒
	bool                       IdleAllThread(bool bWait);//停止所有執行緒至Idle狀態
	bool                       WaitForAllThreadFinish();//等待所有執行緒結束
	bool                       WaitForAllThreadIdle();//等待所有執行緒閒置
	bool                       WaitForAllThreadStop();//等待所有執行緒停止
	bool                       SaveAllThreadState(LPCTSTR filename);//儲存所有執行緒狀態
	bool                       SaveAllThreadStateFn(LPCTSTR filename);//儲存所有執行緒狀態
	//---------------------------------------------------------------------------------//
	bool                       IdleAllCalcThread(bool bWait);//停止所有計算執行緒至Idle狀態
	bool                       CheckAllCalcThreadStateFinish();//確認所有計算執行緒狀態
	bool                       WaitForAllCalcThreadIdle();//等待所有計算執行緒閒置
	bool                       WaitForAllCalcThreadStop();//等待所有計算執行緒停止
	//---------------------------------------------------------------------------------//
	bool                       IdleAllProjectThread(bool bWait);//停止所有專案執行緒至Idle狀態
	//---------------------------------------------------------------------------------//
	bool                       ExecSystemRunFn();					   //執行系統運作執行緒-常駐運作	
	bool                       CreateSystemRunThread();                //建立系統運作執行緒
	bool                       DeleteSystemRunThread();                //刪除系統運作執行緒
	void                       SetSystemRunThreadState(THREAD_STATE_MODE State);//設定系統運作執行緒狀態
	THREAD_STATE_MODE          GetSystemRunThreadState();              //取得系統運作執行緒狀態
	void                       SetSystemRunThreadCmd(THREAD_COMMAND_MODE Cmd); //設定系統運作執行緒命令
	THREAD_COMMAND_MODE        GetSystemRunThreadCmd();	               //取得系統運作執行緒命令
	//---------------------------------------------------------------------------------//
	bool                       ExecSliceFillFn(size_t ThreadIdx);      //執行相機圖填滿執行緒
	bool                       CreateSliceFillThread();                //建立相機圖填滿執行緒
	bool                       DeleteSliceFillThread();                //刪除相機圖填滿執行緒
	bool                       StartSliceFillThread(bool WaitOn);      //開始相機圖填滿執行緒
	bool                       WaitForSliceFillThreadIdle();           //等待相機圖填滿執行緒閒置
	bool                       WaitForSliceFillThreadStop();           //等待相機圖填滿執行緒停止
	bool                       WaitForSliceFillThreadStart();          //等待相機圖填滿執行緒開始
	bool                       WaitForSliceFillThreadFinish();         //等待相機圖填滿執行緒結束
	bool                       CheckSliceFillThreadStateFinish();      //確認相機圖填滿執行緒狀態
	void                       SetSliceFillThreadState(size_t idx, THREAD_STATE_MODE State);//設定相機圖填滿執行緒狀態
	THREAD_STATE_MODE          GetSliceFillThreadState(size_t idx);              //取得相機圖填滿執行緒狀態
	void                       SetSliceFillThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定相機圖填滿執行緒命令
	THREAD_COMMAND_MODE        GetSliceFillThreadCmd(size_t idx);	               //取得相機圖填滿執行緒命令
	size_t                     GetSliceFillThreadUsingCount() const;   //取得相機圖填滿實際使用數量 
	//---------------------------------------------------------------------------------//		
	bool                       ExecFrameMergeFn(size_t ThreadIdx);      //執行影像合併執行緒
	bool                       CreateFrameMergeThread();                //建立影像合併執行緒
	bool                       DeleteFrameMergeThread();                //刪除影像合併執行緒
	bool                       StartFrameMergeThread(bool WaitOn);      //開始影像合併執行緒	
	bool                       WaitForFrameMergeThreadIdle();           //等待影像合併執行緒閒置
	bool                       WaitForFrameMergeThreadStop();           //等待影像合併執行緒停止
	bool                       WaitForFrameMergeThreadStart();          //等待影像合併執行緒開始
	bool                       WaitForFrameMergeThreadFinish();         //等待影像合併執行緒結束
	bool                       CheckFrameMergeThreadStateFinish();      //確認影像合併執行緒狀態
	void                       SetFrameMergeThreadState(size_t idx, THREAD_STATE_MODE State);//設定影像合併執行緒狀態
	THREAD_STATE_MODE          GetFrameMergeThreadState(size_t idx);              //取得影像合併執行緒狀態
	void                       SetFrameMergeThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定影像合併執行緒命令
	THREAD_COMMAND_MODE        GetFrameMergeThreadCmd(size_t idx);	               //取得影像合併執行緒命令
	size_t                     GetFrameMergeThreadUsingCount() const;   //取得影像合併實際使用數量 
	//---------------------------------------------------------------------------------//	
	bool                       ExecFieldMergeFn(size_t ThreadIdx);      //執行區域合併執行緒
	bool                       CreateFieldMergeThread();                //建立區域合併執行緒
	bool                       DeleteFieldMergeThread();                //刪除區域合併執行緒
	bool                       StartFieldMergeThread(bool WaitOn);      //開始區域合併執行緒	
	bool                       WaitForFieldMergeThreadIdle();           //等待區域合併執行緒閒置
	bool                       WaitForFieldMergeThreadStop();           //等待區域合併執行緒停止
	bool                       WaitForFieldMergeThreadStart();          //等待區域合併執行緒開始
	bool                       WaitForFieldMergeThreadFinish();         //等待區域合併執行緒結束
	bool                       CheckFieldMergeThreadStateFinish();      //確認區域合併執行緒狀態
	void                       SetFieldMergeThreadState(size_t idx, THREAD_STATE_MODE State);//設定區域合併執行緒狀態
	THREAD_STATE_MODE          GetFieldMergeThreadState(size_t idx);              //取得區域合併執行緒狀態
	void                       SetFieldMergeThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定區域合併執行緒命令
	THREAD_COMMAND_MODE        GetFieldMergeThreadCmd(size_t idx);	               //取得區域合併執行緒命令
	size_t                     GetFieldMergeThreadUsingCount() const;   //取得區域合併實際使用數量 
	//---------------------------------------------------------------------------------//
	bool                       ExecFrameLoadFn(size_t ThreadIdx);      //執行影像載入執行緒
	bool                       CreateFrameLoadThread();                //建立影像載入執行緒
	bool                       DeleteFrameLoadThread();                //刪除影像載入執行緒
	bool                       StartFrameLoadThread(bool WaitOn);      //開始影像載入執行緒	
	bool                       WaitForFrameLoadThreadIdle();           //等待影像載入執行緒閒置
	bool                       WaitForFrameLoadThreadStop();           //等待影像載入執行緒停止
	bool                       WaitForFrameLoadThreadStart();          //等待影像載入執行緒開始
	bool                       WaitForFrameLoadThreadFinish();         //等待影像載入執行緒結束
	bool                       CheckFrameLoadThreadStateFinish();      //確認影像載入執行緒狀態
	void                       SetFrameLoadThreadState(size_t idx, THREAD_STATE_MODE State);//設定影像載入執行緒狀態
	THREAD_STATE_MODE          GetFrameLoadThreadState(size_t idx);              //取得影像載入執行緒狀態
	void                       SetFrameLoadThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定影像載入執行緒命令
	THREAD_COMMAND_MODE        GetFrameLoadThreadCmd(size_t idx);	               //取得影像載入執行緒命令
	size_t                     GetFrameLoadThreadUsingCount() const;   //取得影像載入實際使用數量 
	//---------------------------------------------------------------------------------//	
	bool                       ExecFieldCalcFn(size_t ThreadIdx);      //執行視野計算執行緒
	bool                       CreateFieldCalcThread();                //建立視野計算執行緒
	bool                       DeleteFieldCalcThread();                //刪除視野計算執行緒
	bool                       StartFieldCalcThread(bool WaitOn);      //開始視野計算執行緒	
	bool                       WaitForFieldCalcThreadIdle();           //等待視野計算執行緒閒置
	bool                       WaitForFieldCalcThreadStop();           //等待視野計算執行緒停止
	bool                       WaitForFieldCalcThreadStart();          //等待視野計算執行緒開始
	bool                       WaitForFieldCalcThreadFinish();         //等待視野計算執行緒結束
	bool                       CheckFieldCalcThreadStateFinish();      //確認視野計算執行緒狀態
	void                       SetFieldCalcThreadState(size_t idx, THREAD_STATE_MODE State);//設定視野計算執行緒狀態
	THREAD_STATE_MODE          GetFieldCalcThreadState(size_t idx);              //取得視野計算執行緒狀態
	void                       SetFieldCalcThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定視野計算執行緒命令
	THREAD_COMMAND_MODE        GetFieldCalcThreadCmd(size_t idx);	               //取得視野計算執行緒命令
	size_t                     GetFieldCalcThreadUsingCount() const;   //取得視野計算實際使用數量 
	//---------------------------------------------------------------------------------//	
	bool                       ExecRegionCalcFn(size_t ThreadIdx);      //執行區域計算執行緒
	bool                       ExecRegionCalcFn_ProjectMap(size_t ThreadIdx);//執行區域計算執行緒-專案底圖
	bool                       ExecRegionCalcFn_PanelFd(size_t ThreadIdx);//執行區域計算執行緒-整板定位點
	bool                       ExecRegionCalcFn_BoardFd(size_t ThreadIdx);//執行區域計算執行緒-單板定位點
	bool                       ExecRegionCalcFn_Inspection(size_t ThreadIdx);//執行區域計算執行緒-專案檢測
	bool                       ExecRegionCalcFn_ProjectMark(size_t ThreadIdx);//執行區域計算執行緒-專案標記
	bool                       ExecRegionCalcFn_ProjectOpenCode(size_t ThreadIdx);//執行區域計算執行緒-專案開檔條碼
	//---------------------------------------------------------------------------------//	
	bool                       CreateRegionCalcThread();                //建立區域計算執行緒
	bool                       DeleteRegionCalcThread();                //刪除區域計算執行緒
	bool                       StartRegionCalcThread(bool WaitOn);      //開始區域計算執行緒
	bool                       StartRegionCalcThread_Full(bool WaitOn); //開始區域計算執行緒-全部執行緒
	bool                       StartRegionCalcThread_Partial(bool WaitOn); //開始區域計算執行緒-部分執行緒
	size_t                     CalcRegionCalcPartialThreadCount() const;//計算區域計算部分執行緒數量
	bool                       WaitForRegionCalcThreadIdle();           //等待區域計算執行緒閒置
	bool                       WaitForRegionCalcThreadStop();           //等待區域計算執行緒停止
	bool                       WaitForRegionCalcThreadStart();          //等待區域計算執行緒開始
	bool                       WaitForRegionCalcThreadStart_Full();     //等待區域計算執行緒開始-全部執行緒
	bool                       WaitForRegionCalcThreadStart_Partial();  //等待區域計算執行緒開始-部分執行緒
	bool                       WaitForRegionCalcThreadFinish();         //等待區域計算執行緒結束
	bool                       CheckRegionCalcThreadStateFinish();      //確認區域計算執行緒狀態
	void                       SetRegionCalcThreadState(size_t idx, THREAD_STATE_MODE State);//設定區域計算執行緒狀態
	THREAD_STATE_MODE          GetRegionCalcThreadState(size_t idx);              //取得區域計算執行緒狀態
	void                       SetRegionCalcThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定區域計算執行緒命令
	THREAD_COMMAND_MODE        GetRegionCalcThreadCmd(size_t idx);	               //取得區域計算執行緒命令
	size_t                     GetRegionCalcThreadMaxCount() const;   //取得區域計算設定數量 
	size_t                     GetRegionCalcThreadUsingCount() const;   //取得區域計算實際使用數量 		
	//---------------------------------------------------------------------------------//			
	bool                       CreateThreadSequenceThread();                //建立執行緒程序執行緒
	bool                       DeleteThreadSequenceThread();                //刪除執行緒程序執行緒
	bool                       StartThreadSequenceThread(bool WaitOn);      //開始執行緒程序執行緒
	bool                       WaitForThreadSequenceThreadIdle();           //等待執行緒程序執行緒閒置
	bool                       WaitForThreadSequenceThreadStop();           //等待執行緒程序執行緒停止
	bool                       WaitForThreadSequenceThreadStart();           //等待執行緒程序執行緒開始
	bool                       WaitForThreadSequenceThreadFinish();         //等待執行緒程序執行緒結束
	bool                       CheckThreadSequenceThreadStateFinish();      //確認執行緒程序執行緒狀態	
	void                       SetThreadSequenceThreadState(THREAD_STATE_MODE State);//設定執行緒程序執行緒狀態
	THREAD_STATE_MODE          GetThreadSequenceThreadState();              //取得執行緒程序執行緒狀態
	void                       SetThreadSequenceThreadCmd(THREAD_COMMAND_MODE Cmd); //設定執行緒程序執行緒命令
	THREAD_COMMAND_MODE        GetThreadSequenceThreadCmd();	               //取得執行緒程序執行緒命令
	void                       SetThreadSequenceThreadElapseTimems(double value); //設定執行緒程序執行緒經過時間
	double                     GetThreadSequenceThreadElapseTimems();	               //取得執行緒程序執行緒經過時間	
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseFieldFrame();                        //執行釋放區域影像
	bool                       ExecFieldFrameReleaseFn();                      //執行區域影像釋放執行緒	
	bool                       CreateFieldFrameReleaseThread();                //建立區域影像釋放執行緒
	bool                       DeleteFieldFrameReleaseThread();                //刪除區域影像釋放執行緒
	bool                       StopFieldFrameReleaseThread(bool WaitOn);       //停止區域影像釋放執行緒
	bool                       StartFieldFrameReleaseThread(bool WaitOn);      //開始區域影像釋放執行緒
	bool                       WaitForFieldFrameReleaseThreadIdle();           //等待區域影像釋放執行緒閒置
	bool                       WaitForFieldFrameReleaseThreadStop();           //等待區域影像釋放執行緒停止
	bool                       WaitForFieldFrameReleaseThreadStart();          //等待區域影像釋放執行緒開始
	bool                       WaitForFieldFrameReleaseThreadFinish();         //等待區域影像釋放執行緒結束
	bool                       CheckFieldFrameReleaseThreadStateFinish();      //確認區域影像釋放執行緒狀態
	void                       SetFieldFrameReleaseThreadState(THREAD_STATE_MODE State);//設定區域影像釋放執行緒狀態
	THREAD_STATE_MODE          GetFieldFrameReleaseThreadState();              //取得區域影像釋放執行緒狀態
	void                       SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_MODE Cmd); //設定區域影像釋放執行緒命令
	THREAD_COMMAND_MODE        GetFieldFrameReleaseThreadCmd();	               //取得區域影像釋放執行緒命令
	void                       SetFieldFrameReleaseThreadNeedCheck(bool Val);  //設定區域影像釋放執行緒需要確認
	bool                       GetFieldFrameReleaseThreadNeedCheck() const;	   //取得區域影像釋放執行緒需要確認	
	//---------------------------------------------------------------------------------//		
	//線上檢測
	bool                       ExecOnlineInspectionFn();                      //執行線上檢測執行緒	
	bool                       CreateOnlineInspectionThread();                //建立線上檢測執行緒
	bool                       DeleteOnlineInspectionThread();                //刪除線上檢測執行緒
	bool                       StartOnlineInspectionThread(bool WaitOn);      //開始線上檢測執行緒
	bool                       WaitForOnlineInspectionThreadIdle();           //等待線上檢測執行緒閒置
	bool                       WaitForOnlineInspectionThreadStop();           //等待線上檢測執行緒停止
	bool                       WaitForOnlineInspectionThreadStart();          //等待線上檢測執行緒開始
	bool                       WaitForOnlineInspectionThreadFinish();         //等待線上檢測執行緒結束
	bool                       CheckOnlineInspectionThreadStateFinish();      //確認線上檢測執行緒狀態
	void                       SetOnlineInspectionThreadState(THREAD_STATE_MODE State);//設定線上檢測執行緒狀態
	THREAD_STATE_MODE          GetOnlineInspectionThreadState();              //取得線上檢測執行緒狀態
	void                       SetOnlineInspectionThreadCmd(THREAD_COMMAND_MODE Cmd); //設定線上檢測執行緒命令
	THREAD_COMMAND_MODE        GetOnlineInspectionThreadCmd();	               //取得線上檢測執行緒命令
	bool                       ExecOnlineInspectionStopFn(ONLINE_STATE_MODE State); //執行線上檢測停止函式
	bool                       ExecOnlineInspectionFinish();                     //執行線上檢測結束
	//---------------------------------------------------------------------------------//		
	//移除資料夾
	bool                       ExecRemoveFolderFn();                      //執行移除資料夾執行緒
	bool                       CreateRemoveFolderThread();                //建立移除資料夾執行緒
	bool                       DeleteRemoveFolderThread();                //刪除移除資料夾執行緒
	bool                       StartRemoveFolderThread(bool WaitOn);      //開始移除資料夾執行緒
	bool                       WaitForRemoveFolderThreadIdle();           //等待移除資料夾除執行緒閒置
	bool                       WaitForRemoveFolderThreadStop();           //等待移除資料夾除執行緒停止
	bool                       WaitForRemoveFolderThreadStart();          //等待移除資料夾除執行緒開始
	bool                       WaitForRemoveFolderThreadFinish();         //等待移除資料夾除執行緒結束
	bool                       CheckRemoveFolderThreadState(THREAD_STATE_MODE State);//確認移除資料夾執行緒狀態
	void                       SetRemoveFolderThreadState(THREAD_STATE_MODE State);//設定移除資料夾執行緒狀態
	THREAD_STATE_MODE          GetRemoveFolderThreadState();              //取得移除資料夾執行緒狀態
	void                       SetRemoveFolderThreadCmd(THREAD_COMMAND_MODE Cmd); //設定移除資料夾執行緒命令
	THREAD_COMMAND_MODE        GetRemoveFolderThreadCmd();	               //取得移除資料夾執行緒命令
	//---------------------------------------------------------------------------------//
	//軌道自動運轉
	LANE_ID                    GetConveyerAutoRunLaneID(size_t ThreadIdx) const;//取得軌道自動運轉軌道號
	size_t                     GetConveyerAutoRunLaneIdx(LANE_ID LaneID) const;//取得軌道自動運轉軌道引數
	bool                       ExecConveyerAutoRunFn(size_t idx);      //執行軌道自動運轉執行緒
	bool                       CreateConveyerAutoRunThread();                //建立軌道自動運轉入執行緒
	bool                       DeleteConveyerAutoRunThread();                //刪除軌道自動運轉執行緒
	bool                       StopConveyerAutoRunThread(bool WaitOn);      //停止軌道自動運轉執行緒	
	bool                       StopConveyerAutoRunThread(size_t idx, bool WaitOn);      //停止軌道自動運轉執行緒	
	bool                       StartConveyerAutoRunThreadFn(size_t idx, bool WaitOn);      //開始軌道自動運轉執行緒
	bool                       StartConveyerAutoRunThread(size_t idx, ONLINE_STATE_MODE State, CAOIProject *ProjectPtr, bool bPreRun, bool WaitOn);//開始軌道自動運轉執行緒		
	bool                       WaitForConveyerAutoRunThreadIdle();           //等待軌道自動運轉執行緒閒置
	bool                       WaitForConveyerAutoRunThreadStop();           //等待軌道自動運轉執行緒停止
	bool                       WaitForConveyerAutoRunThreadStart(size_t idx);          //等待軌道自動運轉執行緒開始
	bool                       WaitForConveyerAutoRunThreadFinish();         //等待軌道自動運轉執行緒結束	
	void                       SetConveyerAutoRunPreRunMode(size_t idx, bool value);//設定軌道自動運轉-提前運轉
	bool                       GetConveyerAutoRunPreRunMode(size_t idx) const;              //取得軌道自動運轉-提前運轉
	void                       SetConveyerAutoRunState(size_t idx, ONLINE_STATE_MODE State);//設定軌道自動運轉狀態
	ONLINE_STATE_MODE          GetConveyerAutoRunState(size_t idx);              //取得軌道自動運轉狀態
	void                       SetConveyerAutoRunProjectPtr(size_t idx, CAOIProject *Ptr);//設定軌道自動運轉專案
	CAOIProject*               GetConveyerAutoRunProjectPtr(size_t idx);              //取得軌道自動運轉專案
	bool                       CheckConveyerAutoRunIdle(size_t idx);                //確認軌道自動運轉閒置
	bool                       CheckConveyerAutoRunFinish(size_t idx);                //確認軌道自動運轉完成
	bool                       CheckConveyerAutoRunException(size_t idx);             //確認軌道自動運轉異常
	void                       SetConveyerAutoRunThreadState(size_t idx, THREAD_STATE_MODE State);//設定軌道自動運轉執行緒狀態
	THREAD_STATE_MODE          GetConveyerAutoRunThreadState(size_t idx);              //取得軌道自動運轉執行緒狀態
	void                       SetConveyerAutoRunThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd); //設定軌道自動運轉執行緒命令
	THREAD_COMMAND_MODE        GetConveyerAutoRunThreadCmd(size_t idx);	               //取得軌道自動運轉執行緒命令	
	//---------------------------------------------------------------------------------//
	//MES執行	
	bool                       CreateMESProcThread();                //建立MES執行執行緒
	bool                       DeleteMESProcThread();                //刪除MES執行執行緒
	//---------------------------------------------------------------------------------//	
	//載入維修站檔案執行緒
	bool                       ExecLoadRepairFileFn();            //執行載入維修站檔案執行緒
	bool                       CreateLoadRepairFileThread();      //建立載入維修站檔案執行緒
	bool                       DeleteLoadRepairFileThread();      //刪除載入維修站檔案執行緒
	bool                       StopLoadRepairFileThread(bool WaitOn);//停止載入維修站檔案執行緒
	bool                       StartLoadRepairFileThread(bool WaitOn);//開始載入維修站檔案執行緒		
	bool                       WaitForLoadRepairFileThreadIdle();           //等待載入維修站檔案執行緒閒置
	bool                       WaitForLoadRepairFileThreadStop();           //等待載入維修站檔案執行緒停止
	bool                       WaitForLoadRepairFileThreadStart();          //等待載入維修站檔案執行緒開始
	bool                       WaitForLoadRepairFileThreadFinish();         //等待載入維修站檔案執行緒結束
	bool                       CheckLoadRepairFileThreadState(THREAD_STATE_MODE State);//確認載入維修站檔案執行緒狀態
	void                       SetLoadRepairFileThreadState(THREAD_STATE_MODE State);//設定載入維修站檔案執行緒狀態
	THREAD_STATE_MODE          GetLoadRepairFileThreadState();              //取得載入維修站檔案執行緒狀態
	void                       SetLoadRepairFileThreadCmd(THREAD_COMMAND_MODE Cmd); //設定載入維修站檔案執行緒命令
	THREAD_COMMAND_MODE        GetLoadRepairFileThreadCmd();	               //取得載入維修站檔案執行緒命令
	//---------------------------------------------------------------------------------//
	//維修站結果執行緒-中央控制
	bool                       ExecRepairResultSignalFn();            //執行維修站結果訊號執行緒
	bool                       ExecRepairResultSignalLaneFn(LANE_ID LaneID); //執行維修站結果訊號執行緒-軌道
	bool                       ExecRepairResultSignalLaneRepairFn(LANE_ID LaneID, TEST_RESULT_ID &TestResultID); //執行維修站結果訊號執行緒-軌道-維修站
	bool                       ExecRepairResultSignalLaneControlCenterFn(LANE_ID LaneID, TEST_RESULT_ID &TestResultID); //執行維修站結果訊號執行緒-軌道-中央控制
	bool                       CreateRepairResultSignalThread();      //建立維修站結果訊號執行緒
	bool                       DeleteRepairResultSignalThread();      //刪除維修站結果訊號執行緒
	bool                       StopRepairResultSignalThread(bool WaitOn);//停止維修站結果訊號執行緒
	bool                       StartRepairResultSignalThread(bool WaitOn);//開始維修站結果訊號執行緒	
	bool                       WaitForRepairResultSignalThreadIdle();           //等待維修站結果訊號執行緒閒置
	bool                       WaitForRepairResultSignalThreadStop();           //等待維修站結果訊號執行緒停止
	bool                       WaitForRepairResultSignalThreadStart();          //等待維修站結果訊號執行緒開始
	bool                       WaitForRepairResultSignalThreadFinish();         //等待維修站結果訊號執行緒結束
	bool                       CheckRepairResultSignalThreadState(THREAD_STATE_MODE State);//確認維修站結果訊號執行緒狀態
	void                       SetRepairResultSignalThreadState(THREAD_STATE_MODE State);//設定維修站結果訊號執行緒狀態
	THREAD_STATE_MODE          GetRepairResultSignalThreadState();              //取得維修站結果訊號執行緒狀態
	void                       SetRepairResultSignalThreadCmd(THREAD_COMMAND_MODE Cmd); //設定維修站結果訊號執行緒命令
	THREAD_COMMAND_MODE        GetRepairResultSignalThreadCmd();	               //取得維修站結果訊號執行緒命令
	//---------------------------------------------------------------------------------//
	//簡單工作執行緒
	bool                       ExecSimpleJobFn();     //執行簡單工作執行緒
	bool                       ExecSimpleJobFn_MoveToDistrict();            //執行簡單工作執行緒
	bool                       StartSimpleJobFn_MoveToDistrict(LANE_ID LaneID, DISTRICT_ID DistrictID, bool bWait);//開始簡單工作-移至分段位置
	bool                       WaitForSimpleJobFn_MoveToDistrict();         //等待簡單工作緒結束-移至分段位置
	bool                       CreateSimpleJobThread();      //建立簡單工作執行緒
	bool                       DeleteSimpleJobThread();      //刪除簡單工作執行緒
	bool                       StopSimpleJobThread(bool WaitOn);//停止簡單工作執行緒
	bool                       StartSimpleJobThread(bool WaitOn);//開始簡單工作執行緒	
	bool                       WaitForSimpleJobThreadIdle();           //等待簡單工作緒閒置
	bool                       WaitForSimpleJobThreadStop();           //等待簡單工作緒停止
	bool                       WaitForSimpleJobThreadStart();          //等待簡單工作緒開始
	bool                       WaitForSimpleJobThreadFinish();         //等待簡單工作緒結束
	bool                       CheckSimpleJobThreadState(THREAD_STATE_MODE State);//確認簡單工作執行緒狀態
	void                       SetSimpleJobThreadState(THREAD_STATE_MODE State);//設定簡單工作執行緒狀態
	THREAD_STATE_MODE          GetSimpleJobThreadState();              //取得簡單工作執行緒狀態
	void                       SetSimpleJobThreadCmd(THREAD_COMMAND_MODE Cmd); //設定簡單工作執行緒命令
	THREAD_COMMAND_MODE        GetSimpleJobThreadCmd();	               //取得簡單工作執行緒命令
	void                       SetSimpleJobMode(SIMPLE_JOB_MODE Mode); //設定簡單工作模式
	SIMPLE_JOB_MODE            GetSimpleJobMode();	               //取得簡單工作模式
	void                       SetSimpleJob_LaneID(LANE_ID ID); //設定簡單工作模式-軌道編號
	LANE_ID                    GetSimpleJob_LaneID();	               //取得簡單工作模式-軌道編號
	void                       SetSimpleJob_DistrictID(DISTRICT_ID ID); //設定簡單工作模式-分段編號
	DISTRICT_ID                GetSimpleJob_DistrictID();	               //取得簡單工作模式-分段編號	
	//---------------------------------------------------------------------------------//		
	bool                       CreateProjectMarkObj();          //建立依標記切換專案物件	
	bool                       SelectProjectMarkBestOne();      //選擇專案標記成績最好 
	//---------------------------------------------------------------------------------//
	bool                       OpenProjectByBarcodeCamera();    //由相機條碼開啟專案
	//---------------------------------------------------------------------------------//
	bool                       ExecTaskLaneBypass();//執行軌道流片
	bool                       ExecInspectionProject();//執行檢測專案-調機測試
	bool                       ExecInspectionProjectMark();//執行檢測專案特徵	
	bool                       ExecInspectionProjectByTurn(LANE_ID LaneID);//執行檢測專案輪流切換		
	CAOIProject*               GetNextProjectByTurnOneCycle(LANE_ID LaneID);//執行檢測專案輪流切換,一次檢測內-取得下一個檢測專案
	bool                       ExecInspectionProjectByTurnOneCycleReset(LANE_ID LaneID);//執行檢測專案輪流切換, 一次檢測內
	bool                       ExecInspectionProjectByTurnOneCycleNext(LANE_ID LaneID, bool &bNext);//執行檢測專案輪流切換, 一次檢測內-下一個	
	bool                       ExecInspectionProjectTuning();//執行檢測專案-調機測試
	bool                       ExecInspectionProjectOnline();//執行檢測專案-線上檢測		
	bool                       ExecInspectionProjectOnline_Prep();//執行檢測專案-線上檢測-預備
	bool                       ExecInspectionProjectOpenCode();//執行檢測專案開啟條碼	
	bool                       ExecInspectionProjectOpenCode_2();//執行檢測專案開啟條碼
	//---------------------------------------------------------------------------------//		
	bool                       ExecThreadSequenceFn();                      //執行執行緒程序執行緒
	bool                       ExecThreadSequenceFn_ProjectMap();           //執行執行緒程序執行緒-專案底圖
	bool                       ExecThreadSequenceFn_AlignProject();         //執行執行緒程序執行緒-專案對齊
	bool                       ExecThreadSequenceFn_ExportOffline();        //執行執行緒程序執行緒-離線程式
	bool                       ExecThreadSequenceFn_InspectProject();       //執行執行緒程序執行緒-專案檢測	
	bool                       ExecThreadSequenceFn_InspectProjectKernel(bool bNewTest); //執行執行緒程序執行緒-專案檢測核心
	bool                       ExecThreadSequenceFn_InspectOffline();       //執行執行緒程序執行緒-離線檢測		
	bool                       ExecThreadSequenceFn_InspectOfflineKernel(); //執行執行緒程序執行緒-離線檢測核心
	//---------------------------------------------------------------------------------//		
	bool                       ExecSystemException(WPARAM wParam, LPARAM lParam);//解出系統異常訊息
	bool                       DecodeSystemException(WPARAM wParam, LPARAM lParam, CString &str);//解出系統異常訊息
	//---------------------------------------------------------------------------------//		
	void                       ResetActiveIndex();
	void                       SetActiveFdIndex(unsigned int val);
	unsigned int               GetActiveFdIndex();
	void                       SetActivePanelIndex(unsigned int val);
	unsigned int               GetActivePanelIndex();
	void                       SetActiveBoardIndex(unsigned int val);
	unsigned int               GetActiveBoardIndex();
	void                       SetActiveBarcodeIndex(unsigned int val);
	unsigned int               GetActiveBarcodeIndex();
	void                       SetActiveComponentIndex(unsigned int val);
	unsigned int               GetActiveComponentIndex();
	void                       SetActiveComponentWindowIndex(unsigned int val);
	unsigned int               GetActiveComponentWindowIndex();
	void                       ResetActiveModelWndUUID();
	UUID                       GetActiveModelWndUUID();
	void                       SetActiveModelWndUUID(UUID val);	
	void                       ResetActiveModelBoxUUID();
	UUID                       GetActiveModelBoxUUID();
	void                       SetActiveModelBoxUUID(UUID val);
	UUID                       GetActiveModelLandUUID();
	void                       ResetActiveModelLandUUID();
	void                       SetActiveModelLandUUID(UUID val);	
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();//取得檢測的專案指標
	CAOIProject*               GetActiveTaskProject();//依照專案作業來取得目前工作專案指標
	CAOIFd*                    GetActiveFd();
	CAOIMark*                  GetActiveMark();
	CAOIPanel*                 GetActivePanel();
	CAOIBoard*                 GetActiveBoard();
	CAOIBarcode*               GetActiveBarcode();
	CAOIComponent*             GetActiveComponent();
	CAOIWindow*                GetActiveWindow();
	//---------------------------------------------------------------------------------//	
	bool                       SaveOpenProjectBarcodeFile();//儲存開啟專案條碼檔案
	bool                       SaveOpenProjectBarcodeFileFn();//儲存開啟專案條碼檔案
	bool                       LoadOpenProjectBarcodeFile();//載入開啟專案條碼檔案
	bool                       LoadOpenProjectBarcodeFileFn();//載入開啟專案條碼檔案
	CString                    GetOpenProjectBarcodeFilename();//取得開啟專案條碼檔名	
	//---------------------------------------------------------------------------------//	
	bool                       ClearOpenProjectBarcodeList();//清除開啟專案條碼列表			
	size_t                     GetOpenProjectBarcodeCount();//取得開啟專案條碼數量	
	bool                       AddOpenProjectBarcode(CAOIBarcode* Ptr, bool bClone);//加入開啟專案條碼
	CAOIBarcode*               GetOpenProjectBarcodePtr(size_t idx, bool bCheck);//取得開啟專案條碼指標		
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetOpenProjectProjectPtr();		
	bool                       GetOnlineOpenProjectFinish() const;
	void                       SetOnlineOpenProjectFinish(bool val);	
	DWORD                      GetOnlineOpenProjectHandHeldResult() const;
	void                       SetOnlineOpenProjectHandHeldResult(DWORD val);
	LPCTSTR                    GetOnlineOpenProjectHandHeldBarcode() const;
	void                       SetOnlineOpenProjectHandHeldBarcode(LPCTSTR val);	
	void                       ResetOnlineOpenProjectCameraBarcodeTestState();//線上開啟專案-相機條碼檢測狀態-復歸
	BARCODE_CAMERA_TEST_STATE  GetOnlineOpenProjectCameraBarcodeTestState() const;//線上開啟專案-相機條碼檢測狀態
	void                       SetOnlineOpenProjectCameraBarcodeTestState(BARCODE_CAMERA_TEST_STATE val);//線上開啟專案-相機條碼檢測狀態	
	//---------------------------------------------------------------------------------//
	bool                       CheckActiveProjectPtr();
	bool                       DestroyAllProject();//關閉所有專案
	bool                       CloseActiveProject();//關閉作業中專案
	bool                       DestroyActiveProject();//關閉作業中專案
	bool                       SwitchActiveProject();//切換作業中的專案指標		
	size_t                     GetProjectPtrCount();//取得專案數量
	bool                       ClearActiveProject();//清除作業中專案
	bool                       ClearProjectPtrList();//清除專案指標列表		
	unsigned int               GetActiveProjectIndex();//取得作業中專案引數
	bool                       SetActiveProjectIndex(size_t idx);//設定作業中專案引數	
	bool                       CheckProjectPtr(CAOIProject *Ptr);//確認專案指標
	void                       SetActiveProjectPtr(CAOIProject* Ptr);//設定主要專案	
	CAOIProject*               GetProjectPtr(size_t idx, bool check);//取得專案指標
	CAOIProject*               GetProjectPtrByFilename(LPCTSTR filename);//取得專案指標
	bool                       AddProjectPtr(CAOIProject* Ptr, bool clone);//增加專案指標
	bool                       GetProjectNameList(std::vector<CString> &List, bool bFullName=true);//取得專案檔名列表
	bool                       DestroyProjectSelected();//刪除被選到的專案
	bool                       SelectAllProjects(bool bSelected);//選取所有專案	
	bool                       CloneProjectNewModel(CAOIProject* ProjectPtr, CAOIModel *ModelPtr, CAOIModel *&NewModelPtr);//複製模組成新的	
	bool                       ChceckProjectFileNameExist(unsigned int ExcludeIndex, LPCTSTR pfilename);//確認專案名稱存在	
	bool                       ApplyProjectParameterToSystem(CAOIProject* ProjectPtr);//套用專案參數至系統內
	bool                       ApplySystemParameterToAllProject();//套用系統參數至所有專案參數內
	bool                       GetProjectBarcodeFilename(LANE_ID LaneID, bool bSave, CString &Filename);//取得專案條碼檔案名稱
	bool                       CheckProjectFrameUniqueIDValid(CAOIProject *ProjectPtr);//確認專案影像唯一碼有效
	bool                       ModifyProjectFrameUniqueIDList(CAOIProject *ProjectPtr);//修改專案影像唯一碼列表
	bool                       SetProjectHasModified(WPARAM wParam, CAOIProject *ProjectPtr);
	bool                       SetProjectHasModified(WPARAM wParam, CAOIProject *ProjectPtr, const std::vector<TActiveObj> &ObjList);
	//---------------------------------------------------------------------------------//
	void                       AddLaneProjectTestCountForTurn(LANE_ID LaneID);//增加軌道專案檢測次數-輪流檢測使用
	unsigned int               GetLaneProjectTestCountForTurn(LANE_ID LaneID) const;//取得軌道專案檢測次數-輪流檢測使用
	void                       ResetLaneProjectTestCountForTurn(LANE_ID LaneID);//復歸軌道專案檢測次數-輪流檢測使用
	void                       AddLaneProjectTestCountForTurn_LA();//增加軌道專案檢測次數-A軌道-輪流檢測使用	
	unsigned int               GetLaneProjectTestCountForTurn_LA() const;//取得軌道專案檢測次數-A軌道-輪流檢測使用
	void                       ResetLaneProjectTestCountForTurn_LA();//復歸軌道專案檢測次數-A軌道-輪流檢測使用
	void                       AddLaneProjectTestCountForTurn_LB();//增加軌道專案檢測次數-B軌道-輪流檢測使用	
	unsigned int               GetLaneProjectTestCountForTurn_LB() const;//取得軌道專案檢測次數-B軌道-輪流檢測使用
	void                       ResetLaneProjectTestCountForTurn_LB();//復歸軌道專案檢測次數-B軌道-輪流檢測使用
	//---------------------------------------------------------------------------------//	
	bool                       LayoutLaneProjectList();//整理出軌道上專案			
	bool                       CheckLaneProjectMarkImage(LANE_ID LaneID);//確認軌道專案的特徵影像
	size_t                     GetLaneProjectCount(LANE_ID LaneID) const;//屬於軌道的專案	
	CAOIProject*               GetLaneProjectPtr(LANE_ID LaneID, size_t idx, bool check);//取得軌道專案指標
	bool                       CheckLandProjectPtr(LANE_ID LaneID, CAOIProject *ProjectPtr);//確認該專案是否在此軌道上
	bool                       CloneLaneProjectList(LANE_ID LaneID, std::vector<CAOIProject*> &LaneProjectList);//複製軌道專案列表
	bool                       BuildLaneProjectList(LANE_ID LaneID, std::vector<CAOIProject*> &LaneProjectList);//建立出軌道上專案			
	//---------------------------------------------------------------------------------//
	void                       ClearLaneProjectList_LA();//清除A軌道上的專案列表	
	size_t                     GetLaneProjectCount_LA() const;//屬於A軌道上的專案
	CAOIProject*               GetLaneProjectPtr_LA(size_t idx, bool check);//取得A軌道專案指標

	void                       ClearLaneProjectList_LB();//清除B軌道上的專案列表
	size_t                     GetLaneProjectCount_LB() const;//屬於B軌道上的專案
	CAOIProject*               GetLaneProjectPtr_LB(size_t idx, bool check);//取得B軌道專案指標	
	//---------------------------------------------------------------------------------//	
	bool                       ExecGrabNextUniFrameImage();//執行Frame畫面的取像		
	bool                       CheckCanGrabNextUniFrameImage();//確認可以取Frame畫面影像
	bool                       SetProjectLightSetting(CAOIProject* Ptr);//專案燈源設定	
	bool                       SetSliceParam2DCurrent(TSliceParam &Param, int Current) const;//設定SliceParam的電流	
	bool                       GetSliceParam2DCurrentCmpText(const TSliceParam &ParamBef, const TSliceParam &ParamAft, CString &Str) const;//取得SliceParam的2D電流比較文字
	bool                       GetGrabSliceParamList(std::vector<TSliceParam> &SliceParamList);//執行SliceParam畫面的取像
	bool                       GetGrabFrameParamList(std::vector<TFrameParam> &FrameParamList);//執行Frame畫面的取像		
	bool                       ExecPrepareSliceImageSetting(const std::vector<TSliceParam> &ParamList, bool ResetLight);//執行SliceParam的取像
	bool                       ExecPrepareFrameImageSetting(const std::vector<TFrameParam> &FrameParamList, bool ResetLight);//執行Frame畫面的取像		
	SLICE_FUNC_MODE            CheckSliceFuncModeBasicMode(SLICE_FUNC_MODE SliceFuncMode) const;//確認Slice函式的基本模式
	bool                       CheckSliceFuncModeDlpUse2Exp(SLICE_FUNC_MODE SliceFuncMode) const;//確認Slice函式的雙曝光模式
	int                        CheckSliceFuncModeDlpPatternMode(SLICE_FUNC_MODE SliceFuncMode) const;//確認Slice函式的DLP樣板模式
	int                        CheckSliceFuncModeDlpPatternCountAll(SLICE_FUNC_MODE SliceFuncMode) const;//確認Slice函式的DLP樣板數
	int                        CheckSliceFuncModeDlpPatternCountOnce(SLICE_FUNC_MODE SliceFuncMode) const;//確認Slice函式的DLP樣板數
	bool                       CheckSliceFuncModeDecodePhaseMode(int ImageCount, SLICE_FUNC_MODE SliceFuncMode, DECODE_PHASE_MODE &DecodeMode);//確認解相位模式
	bool                       CheckSliceFuncModeDecodePhaseModeExp2(int ImageCount, SLICE_FUNC_MODE SliceFuncMode, DECODE_PHASE_MODE &DecodeMode);//確認解相位模式	
	bool                       SaveCastParamImageList(const TCastParam &CastParam, LPCTSTR Folder);//儲存投光圖像列表
	bool                       SaveCastParamImageListFn(const TCastParam &CastParam, LPCTSTR Folder);//儲存投光圖像列表
	bool                       BuildCastParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam);//建立投光參數
	bool                       BuildCastParam2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam);//建立投光參數
	bool                       BuildCastParamArray_Calibration(size_t TotalCount, IMAGE_SIZE ImageW[], IMAGE_SIZE ImageH[], IMAGE_SIZE ImageStep[], IMAGE_PTR ImagePtr[], const TSliceParam &SliceParam, TCastParam CastParam[]);//建立投光參數	
	//---------------------------------------------------------------------------------//
	bool                       ExecGrabFrameImage(unsigned int FrameUniqueID, FRAME_TYPE &FrameType);//執行Frame畫面的取像	
	//---------------------------------------------------------------------------------//
	bool                       SetupAllLightSetting();//設定所有燈原設定
	bool                       AutoSetupAllLightSetting();//自動設定所有燈原設定
	bool                       AutoSetupAllLightSettingFn();//自動設定所有燈原設定
	//---------------------------------------------------------------------------------//			
	bool                       ExecCaptureProjectMap(CAOIProject *ProjectPtr);//擷取專案底圖-備份	
	bool                       ExecAlignProjectPanelFd(CAOIProject *ProjectPtr);//對齊專案整板定位點
	bool                       ExecAlignProjectBoardFd(CAOIProject *ProjectPtr);//對齊專案單板定位點
	bool                       ExecInspectProjectBarcode(CAOIProject *ProjectPtr, OFFLINE_FILE_MODE OfflineFileMode);//檢測專案條碼
	bool                       ExecInspectProjectComponent(CAOIProject *ProjectPtr, OFFLINE_FILE_MODE OfflineFileMode);//檢測專案零件
	bool                       ExecInspectProjectOffline(CAOIProject *ProjectPtr);//檢測專案離線專案
	//---------------------------------------------------------------------------------//	
	bool                       ExecAnalysisProjectInspection(CAOIProject *ProjectPtr);//分析專案檢測結果	
	bool                       ExecAnalysisProjectInspection_Barcode(CAOIProject *ProjectPtr);//分析專案檢測結果-條碼
	//---------------------------------------------------------------------------------//	
	bool                       ExecRemoveProjectRawImage(CAOIProject *ProjectPtr);//移除專案儲存原圖	
	//---------------------------------------------------------------------------------//	
	//離線模式
	void                       SetOfflineMode(bool Mode);
	bool                       GetOfflineMode() const;	
	void                       SetSaveOfflineFiles(bool value);
	bool                       GetSaveOfflineFiles() const;
	void                       SetSaveOfflineImageFiles(bool value);
	bool                       GetSaveOfflineImageFiles() const;

	LPCTSTR                    GetOfflineFolder() const;	
	LPCTSTR                    GetOfflineFileName() const;
	void                       SetOfflineFileName(LPCTSTR filename);
	//---------------------------------------------------------------------------------//		
	bool                       CreateTempProjectFile(LPCTSTR filename, CString &TempFileName);//建立暫存的專案	
	//---------------------------------------------------------------------------------//	
	bool                       LimitImageZoomScale(double &ZoomRatio, bool bInverte) const;//限制影像縮放比例
	//---------------------------------------------------------------------------------//
	IMAGE_DISPLAY_MODE         MapFrameTypeToImageDisplayMode(FRAME_TYPE FrameType);//映射Frame樣式轉成顯示模式
	bool                       ExecEnhanceDisplayImage(const TUNI_FRAME &UniFrame, IMAGE_PTR DstPtr);
	bool                       ExecEnhanceDisplayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR SrcPtr, IMAGE_PTR DstPtr);
	//---------------------------------------------------------------------------------//	
	//對焦影像
	bool                       BuildFocusImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR SrcPtr, IMAGE_PTR DstPtr);
	bool                       CalcImageFocusValue(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR SrcPtr, const IMAGE_PTR TarPtr, CALC_FOCUS_MODE Mode, const RECT &RoiRect, double &val);//計算影像對	
	//---------------------------------------------------------------------------------//		
	int                        GetPhaseCombinePeriodMode() const;//取得相位合併週期方式
	bool                       GetPhaseUseGammaCalibration() const;//取得相位是否使用Gamma校正	
	//---------------------------------------------------------------------------------//		
	void                       GetSpaceBasePlaneParam(TBasePlaneParam &BasePlaneParam) const;//取得空間基準面參數
	//---------------------------------------------------------------------------------//			
	void                       GetPhaseNoiseDefineParam(TPhaseNoiseParam &NoiseParam) const;//取得相位雜訊定義參數
	void                       DisableSpaceNoiseFilterParam(TNoiseFilterParam &NoiseFilterParam);//關閉空間雜訊過濾
	void                       GetSpaceNoiseFilterParam(TNoiseFilterParam &NoiseFilterParam) const;//取得空間資料雜訊濾除參數	
	void                       SetSpaceNoiseFilterParam(const TNoiseFilterParam &NoiseFilterParam);//設定空間資料雜訊濾除參數	
	//---------------------------------------------------------------------------------//	
	bool                       SaveObjExecTime(LPCTSTR filename);//儲存物件執行時間
	bool                       SaveObjExecTimeFn(LPCTSTR filename);//儲存物件執行時間
	//---------------------------------------------------------------------------------//		
	bool                       BuildRemoteParamList();//建立遠端調機參數列表	
	bool                       SaveSystemRemoteParameter();//儲存遠端調機參數列表
	bool                       LoadSystemRemoteParameter();//載入遠端調機參數列表
	void                       GetSystemRemoteParamList(std::vector<TRemoteParam> &ParamList) const;//取得遠端調機參數列表
	void                       SetSystemRemoteParamList(const std::vector<TRemoteParam> &ParamList);//設定遠端調機參數列表	
	size_t                     GetRemoteParamActivedIndex(const std::vector<TRemoteParam> &ParamList);//由主要狀態取得引數
	bool                       SetRemoteParamActived(size_t index, std::vector<TRemoteParam> &ParamList);//設定哪個參數為主要狀態
	void                       ArrangeRemoteParamList(const std::vector<TRemoteParam> &SrcList, std::vector<TRemoteParam> &DstList);
	bool                       LoadSystemRemoteParamFile(LPCTSTR filename, std::vector<TRemoteParam> &ParamList);//儲存遠端調機參數
	bool                       LoadSystemRemoteParamFileFn(LPCTSTR filename, std::vector<TRemoteParam> &ParamList);//儲存遠端調機參數
	bool                       SaveSystemRemoteParamFile(LPCTSTR filename, const std::vector<TRemoteParam> &ParamList);//載入遠端調機參數
	bool                       SaveSystemRemoteParamFileFn(LPCTSTR filename, const std::vector<TRemoteParam> &ParamList);//載入遠端調機參數
	//---------------------------------------------------------------------------------//
	bool                       LoadSystemSliceParamINI();//載入Slice Param組態檔
	bool                       LoadSystemSliceParamINIFn();//載入Slice Param組態檔
	bool                       SaveSystemSliceParamINI();//儲存Slice Param組態檔
	bool                       SaveSystemSliceParamINIFn();//儲存Slice Param組態檔
	bool                       BuildSystemSliceParamDefault();//建立預設的Slice Param
	size_t                     GetSystemSliceParamCount() const;//取得Slice Param數量
	bool                       AddSystemSliceParam(const TSliceParam &Param, bool bCheck);//增加Slice Param		
	size_t                     GetSystemSliceParamIndex(size_t UniqueID);//依據Slice ID來取得Slice Param指標
	TSliceParam*               GetSystemSliceParamPtr(size_t idx, bool Check);//依據Slice index來取得Slice Param指標
	TSliceParam*               GetSystemSliceParamPtrByUniqueID(size_t UniqueID);//依據Slice ID來取得Slice Param指標
	unsigned int               GetSystemSliceParamIndexByUniqueID(size_t UniqueID);//依據Slice ID來取得Slice Param引數
	unsigned int               GetSystemSliceParamFreeUniqueID();//取得Slice Param新的識別碼	
	bool                       CheckSystemSliceParamFreeUniqueID(unsigned int ID);//確認Slice Param新的識別碼	
	bool                       DeleteSystemSliceParamIndex(size_t index);//刪除Slice Param	
	bool                       DeleteSystemSliceParamUserDefined();//刪除Slice Param 使用者定義
	bool                       SetSystemSliceParamGain(double Gain);//設定Slice的增益值
	bool                       SetSystemSliceParamTargetGray(int Gray);//設定Slice的灰階值
	bool                       SetSystemSliceParamVerifyTolerance(int Tol);//設定Slice的驗證公差	
	bool                       SetSystemSliceParamExposureTime(unsigned int ExposureTime);//設定Slice的曝光時間
	bool                       ModifySystemSliceParamByIndex(size_t index, const TSliceParam &Param);//修改Slice Param	
	bool                       SetSystemSliceParamList(const std::vector<TSliceParam> &ParamList);//設定Slice Param列表
	bool                       CloneSystemSliceParamList(std::vector<TSliceParam> &ParamList);//複製Slice Param列表		
	bool                       UpdateSystemSliceParamList(const std::vector<TSliceParam> &ParamList);//更新Slice Param列表	
	bool                       CompareSliceParamList(const std::vector<TSliceParam> &List1, const std::vector<TSliceParam> &List2);//比較兩個SliceParam列表是否相等
	//---------------------------------------------------------------------------------//			
	void                       BuildSystemFrameParamDefault();//初始化影像表單	
	size_t                     GetSystemFrameParamCount() const;//取得Frame Param數量
	bool                       AddSystemFrameParam(const TFrameParam &Param, bool bCheck);//增加Frame Param		
	size_t                     GetSystemFrameParamIndex(size_t UniqueID);//依據Frame ID來取得Frame Param指標
	TFrameParam*               GetSystemFrameParamPtr(size_t index, bool bCheck);//取得影像表單指標
	TFrameParam*               GetSystemFrameParamPtrByUniqueID(size_t UniqueID);//依據Frame ID來取得Frame Param指標
	unsigned int               GetSystemFrameParamFreeUniqueID();//取得Frame Param新的識別碼		
	bool                       CheckSystemFrameParamFreeUniqueID(unsigned int ID);//確認Frame Param新的識別碼 
	bool                       DeleteSystemFrameParamIndex(size_t index);//刪除Frame Param	
	bool                       DeleteSystemFrameParamUserDefined();//刪除Frame Param 使用者定義
	bool                       ModifySystemFrameParamIndex(size_t index, const TFrameParam &Param);//修改Frame Param
	bool                       SetSystemFrameParamList(const std::vector<TFrameParam> &ParamList);//設定Frame Param列表	
	bool                       CloneSystemFrameParamList(std::vector<TFrameParam> &ParamList);//複製Frame Param列表				
	bool                       SaveSystemFrameParamINI();//儲存影像表單
	bool                       SaveSystemFrameParamINIFn();//儲存影像表單
	bool                       LoadSystemFrameParamINI();//儲存影像表單		
	bool                       LoadSystemFrameParamINIFn();//儲存影像表單		
	//---------------------------------------------------------------------------------//
	bool                       RemoveUniqueIDListByUniqueID(std::vector<unsigned int> &UniqueIDList, unsigned int UniqueID);//移除特定ID的Frame Param列表			
	bool                       BuildSliceParamListByFrameUniqueID(unsigned int UniqueID, std::vector<TSliceParam> &SliceParamList);//取得Frame的Slice Param List
	bool                       BuildSliceParamListByFrameList(const std::vector<TFrameParam> &FrameParamList, std::vector<TSliceParam> &SliceParamList);//取得Frame的Slice Param List
	bool                       AddFrameSliceParamListByFrameUniqueID(unsigned int FrameUniqueID, std::vector<TFrameParam> &FrameParamList, std::vector<TSliceParam> &SliceParamList);
	bool                       BuildLightCtrlBoardTableListByFrameUniqueID(unsigned int UniqueID, std::vector<TLCB_TRIG_TABLE> &TableList);//取得Frame的Light Ctrl Board Table List	
	bool                       CheckSliceParamUsed3DCastID(const TLightTable &LightTable, LIGHT_3D_CAST_ID CastID[]);//確認SliceParam使用到的CastID	
	
	bool                       CheckCanRetrieveCameraImage();//確認是否可以接收相機影像
	int                        CheckFrameExpCountBySliceFuncMode(SLICE_FUNC_MODE FuncMode) const;//確認Freame的曝光數量
	int                        CheckFrameLightCountBySliceFuncMode(SLICE_FUNC_MODE FuncMode) const;//確認Freame的亮度變化數量
	bool                       ModifyCameraUniFrame(const std::vector<TFrameParam> &FrameParamList);//修正相機通用畫面列表	
	bool                       RetrieveCameraUniFrame(const std::vector<TFrameParam> &FrameParamList, std::vector<TUNI_FRAME> &UniFrameList);//接收相機通用畫面列表		
	bool                       CreateFramePtrByFrameParam(const CAOIField *FieldPtr, TFrameParam *FrameParamPtr, unsigned int &CameraFrameIdx, CAOIFrame *&FramePtr, std::vector<CAOISlice*> &SlicePtrList);//建立專案的Frame指標
	//---------------------------------------------------------------------------------//	
	CString                    GetPhaseFactorFovZFilename(LIGHT_3D_CAST_ID CastID, int Index, bool bTemp);//取得相位高度FOV相位檔名
	//---------------------------------------------------------------------------------//	
	double                     Calc2ParamVecotr(const std::vector<double> &List1, const std::vector<double> &List2);	
	bool                       GetHeightFactorXYParam(int ParamCount, const TPhaseFactorGrid &Grid, std::vector<double> &ParamList);//取得TPhaseFactorGrid計算相位參數值
	bool                       GetHeightFactorPhaseParam(int Mode, int ParamCount, const TPhaseFactorGrid &Grid, std::vector<double> &ParamList);//取得TPhaseFactorGrid計算高度參數值

	bool                       CalcPhaseFactorMappingFunc(const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList);//計算相位高度比例參數-由相位轉成高度
	bool                       CalcPhaseFactorZMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList);//計算相位高度比例參數-由相位轉成高度
	bool                       CalcPhaseFactorXYKMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList);//計算相位高度比例參數-由XY計算出K值
	bool                       CalcPhaseFactorXYPhaseMappingFunc(int Mode, const std::vector<TPhaseFactorGrid> &List, std::vector<double> &KList);//計算相位高度比例參數-由XY計算出相位值
	CString                    GetPhaseFactorFolder(bool bTemp);//取得相位高度比例資料夾
	CString                    GetPhaseFactorFilename(LIGHT_3D_CAST_ID CastID, double TargetHeight, bool bTemp);//取得相位高度比例參數檔案	
	bool                       LoadPhaseFactorTableFile(LPCTSTR pfilename, TPhaseFactorTable &GridTable);//載入相位高度比例參數檔案
	bool                       LoadPhaseFactorTableFileFn(LPCTSTR pfilename, TPhaseFactorTable &GridTable);//載入相位高度比例參數檔案
	bool                       SavePhaseFactorTableFile(LPCTSTR pfilename, const TPhaseFactorTable &GridTable);//儲存相位高度比例參數檔案			
	bool                       SavePhaseFactorTableFileFn(LPCTSTR pfilename, const TPhaseFactorTable &GridTable);//儲存相位高度比例參數檔案			
	//---------------------------------------------------------------------------------//
	CString                    GetDotNodeCaliFilename(LANE_ID LaneID);//取得DotNode參數檔案	
	CString                    GetDotNodeCaliFilename(LPCTSTR Folder, LANE_ID LaneID);//取得DotNode參數檔案	
	bool                       MoveXYDotNodeList(double OffsetX, double OffsetY, std::vector<TDotNode> &List);//移動XY校正檔案
	bool                       LoadXYDotNodeCaliFile(LPCTSTR pfilename, int &CountX, int &CountY, std::vector<TDotNode> &List);//載入XY校正檔案
	bool                       LoadXYDotNodeCaliFileFn(LPCTSTR pfilename, int &CountX, int &CountY, std::vector<TDotNode> &List);//載入XY校正檔案
	bool                       SaveXYDotNodeCaliFile(LPCTSTR pfilename, int CountX, int CountY, const std::vector<TDotNode> &List);//儲存XY校正檔案
	bool                       SaveXYDotNodeCaliFileFn(LPCTSTR pfilename, int CountX, int CountY, const std::vector<TDotNode> &List);//儲存XY校正檔案
	bool                       ConvertXYDotNodeToXYCali(int CountX, int CountY, const std::vector<TDotNode> &DotList, std::vector<TXYCali> &XYCaliList);//轉換XY節點至XY校正
	bool                       RefineXYDotNodeListByLane(const std::vector<TDotNode> &DotList_LA, const std::vector<TDotNode> &DotList_LB, std::vector<TDotNode> &DotListA, std::vector<TDotNode> &DotListB);//提煉AB軌道的XY節點
	//---------------------------------------------------------------------------------//
	bool                       CheckImageConfiguration();//確認影像組態狀態
	//---------------------------------------------------------------------------------//
	bool                       BuildFOVResolutionCombox(CComboBox &Combox, double ShowScale);//建立FOV解析度列表
	//---------------------------------------------------------------------------------//				
	bool                       GetStageSignPositiveX() const;//取得機台X軸方向-相對於Cad
	bool                       GetStageSignPositiveY() const;//取得機台Y軸方向-相對於Cad
	bool                       GetStageSignPositiveZ() const;//取得機台Z軸方向-相對於Cad
	bool                       CheckPCBStopAtRightSide(bool RightIn, DISTRICT_ID DistrictID);//確認PCB板停在右邊
	//---------------------------------------------------------------------------------//
	bool                       CheckDoubleSideEdit();//雙邊控制模式	
	bool                       CheckMoveObjectMode() const;//確認移動物件模式
	bool                       CheckMultiSelectMode() const;//多選擇模式
	UINT                       GetCombineColorVrKey() const;//取得合併顏色模式按鍵碼
	bool                       CheckCombineColorMode() const;//確認合併顏色模式
	bool                       CheckSwitchFrameMode() const;//確認確認切換畫面模式
	bool                       VerifyProjectFilename();//驗證專案檔名
	bool                       VerifyProjectFilename(LPCTSTR filename);//驗證專案檔名
	bool                       VerifyProjectFilename(LANE_ID LaneID, LPCTSTR filename);//驗證專案檔名
	bool                       CheckProjectFilenameValided(LPCTSTR filename);//確認專案檔名是有效檔名
	//---------------------------------------------------------------------------------//
	void                       RegistUserLastInputTickCount();//使用者上次輸入時間戳記
	DWORD                      GetUserLastInputTickCount() const;//使用者上次輸入時間戳記	
	void                       SetUserLastInputTickCount(DWORD value);//使用者上次輸入時間戳記
	void                       SetUserLastInputTickCountByMsg(UINT Msg);//使用者上次輸入時間戳記
	//---------------------------------------------------------------------------------//
	//線上檢測狀態	
	CString                    GetOnlineStateNode(LANE_ID LaneID);
	void                       SetOnlineStateNode(LPCTSTR val, LANE_ID LaneID);
	ONLINE_STATE_MODE          GetOnlineStateMode() const;
	void                       SetOnlineStateMode(ONLINE_STATE_MODE value);				
	ONLINE_STATE_MODE          GetOnlineStateMode_LA() const;
	void                       SetOnlineStateMode_LA(ONLINE_STATE_MODE value);
	ONLINE_STATE_MODE          GetOnlineStateMode_LB() const;
	void                       SetOnlineStateMode_LB(ONLINE_STATE_MODE value);
	ONLINE_STATE_MODE          GetOnlineStateMode_Lane(LANE_ID LaneID) const;
	void                       SetOnlineStateMode_Lane(ONLINE_STATE_MODE value, LANE_ID LaneID);
	//---------------------------------------------------------------------------------//
	ONLINE_STATE_MODE          GetOnlineStateMode_GUI() const;
	void                       SetOnlineStateMode_GUI(ONLINE_STATE_MODE value);	
	ONLINE_STATE_MODE          GetOnlineStateMode_GUI_LA() const;
	void                       SetOnlineStateMode_GUI_LA(ONLINE_STATE_MODE value);	
	ONLINE_STATE_MODE          GetOnlineStateMode_GUI_LB() const;
	void                       SetOnlineStateMode_GUI_LB(ONLINE_STATE_MODE value);	
	ONLINE_STATE_MODE          GetOnlineStateMode_GUI_Lane(LANE_ID LaneID) const;
	void                       SetOnlineStateMode_GUI_Lane(ONLINE_STATE_MODE value, LANE_ID LaneID);	
	//---------------------------------------------------------------------------------//
	ONLINE_STATE_MODE          GetOnlineStateMode_Next() const;
	void                       SetOnlineStateMode_Next(ONLINE_STATE_MODE value);
	ONLINE_STATE_MODE          ModifyFirstOnlineStateMode(LANE_ID LaneID, ONLINE_STATE_MODE State);//修改首次線上狀態
	//---------------------------------------------------------------------------------//
	ONLINE_FROMVIEW_MODE       GetOnlineFormViewMode() const;//取得線上檢測頁面模式
	//---------------------------------------------------------------------------------//
	bool                       CheckOnlineIdleMode(ONLINE_STATE_MODE Mode) const;//確認線上為閒置狀態
	bool                       CheckOnlineCanAutoStop(ONLINE_STATE_MODE Mode) const;//確認線上可以自動停機
	bool                       CheckOnlineConveyerAutoRunCanStop(ONLINE_STATE_MODE Mode) const;//確認線上軌道自動運轉可以停止
	//---------------------------------------------------------------------------------//	
	__int64                    GetOnlineFirstRunDateTime() const;//線上檢測初始時間
	void                       SetOnlineFirstRunDateTime(__int64 value);//線上檢測初始時間
	//---------------------------------------------------------------------------------//
	DWORD                      GetOnlineFirstIdleTickCount() const;//線上初次閒置時間戳記
	void                       SetOnlineFirstIdleTickCount(DWORD value);//線上初次閒置時間戳記
	//---------------------------------------------------------------------------------//
	int                        GetOnlineAutoCalibrationDLPLedColor() const;//取得在線自動校正的DLP燈源
	int                        GetOnlineAutoCalibrationDLPLedColorUsed() const;//設定在線自動校正的DLP燈源顏色	
	void                       SetOnlineAutoCalibrationDLPLedColorUsed(int LEDColor);//設定在線自動校正的DLP燈源顏色
	bool                       GetOnlineAutoCalibrationDLPZeroPlaneCopyToWhite() const;//取得複製在線自動校正的DLP相平面至白燈
	void                       SetOnlineAutoCalibrationDLPZeroPlaneCopyToWhite(bool val);//設定複製在線自動校正的DLP相平面至白燈		
	ONLINE_STATE_MODE          CheckExecOnlineAutoCalibrationMode() const;//確認是否執行在線自動校正	
	bool                       CheckExecOnlineAutoCalibration(TOnlineProcParam &Param);//確認是否執行在線自動校正	
	bool                       CheckExecOnlineAutoCalibrationDateTime(LPCTSTR LastTime, DWORD TimeHour) const;//確認是否執行在線自動校正日期時間
	bool                       CheckExecOnlineAutoCalibration_XYZ_Home() const;//確認是否執行在線自動校正-XYZ歸零
	bool                       CheckExecOnlineAutoCalibration_2D_Current() const;//確認是否執行在線自動校正-2D電流
	bool                       CheckExecOnlineAutoCalibration_3D_Current() const;//確認是否執行在線自動校正-3D電流
	bool                       CheckExecOnlineAutoCalibration_3D_ZeroPlane() const;//確認是否執行在線自動校正-3D相平面
	bool                       CheckExecOnlineAutoCalibration_3D_HeightFactor() const;//確認是否執行在線自動校正-3D高度比例	
	LPCTSTR                    GetOnlineAutoCalibrationDateTime_XYZ_Home() const;//在線自動校正日期時間-XYZ歸零
	void                       SetOnlineAutoCalibrationDateTime_XYZ_Home();//在線自動校正日期時間-XYZ歸零
	void                       SetOnlineAutoCalibrationDateTime_XYZ_Home(LPCTSTR value);//在線自動校正日期時間-XYZ歸零
	bool                       GetOnlineAutoCalibrationToExec_XYZ_Home() const;//在線自動校正去執行-XYZ歸零
	void                       SetOnlineAutoCalibrationToExec_XYZ_Home(bool value);//在線自動校正去執行-XYZ歸零	
	LPCTSTR                    GetOnlineAutoCalibrationDateTime_2D_Current() const;//在線自動校正日期時間-2D電流
	void                       SetOnlineAutoCalibrationDateTime_2D_Current();//在線自動校正日期時間-2D電流
	void                       SetOnlineAutoCalibrationDateTime_2D_Current(LPCTSTR value);//在線自動校正日期時間-2D電流
	bool                       GetOnlineAutoCalibrationToExec_2DCurrent() const;//在線自動校正去執行-2D電流
	void                       SetOnlineAutoCalibrationToExec_2DCurrent(bool value);//在線自動校正去執行-2D電流
	LPCTSTR                    GetOnlineAutoCalibrationDateTime_3D_Current() const;//在線自動校正日期時間-3D電流
	void                       SetOnlineAutoCalibrationDateTime_3D_Current();//在線自動校正日期時間-3D電流
	void                       SetOnlineAutoCalibrationDateTime_3D_Current(LPCTSTR value);//在線自動校正日期時間-3D電流
	LPCTSTR                    GetOnlineAutoCalibrationDateTime_3D_ZeroPlane() const;//在線自動校正日期時間-3D相平面
	void                       SetOnlineAutoCalibrationDateTime_3D_ZeroPlane();//在線自動校正日期時間-3D相平面
	void                       SetOnlineAutoCalibrationDateTime_3D_ZeroPlane(LPCTSTR value);//在線自動校正日期時間-3D相平面
	bool                       GetOnlineAutoCalibrationToExec_3DZeroPlane() const;//在線自動校正去執行-3D相平面
	void                       SetOnlineAutoCalibrationToExec_3DZeroPlane(bool value);//在線自動校正去執行-3D相平面	
	LPCTSTR                    GetOnlineAutoCalibrationDateTime_3D_HeightFactor() const;//在線自動校正日期時間-3D高度比例
	void                       SetOnlineAutoCalibrationDateTime_3D_HeightFactor();//在線自動校正日期時間-3D高度比例
	void                       SetOnlineAutoCalibrationDateTime_3D_HeightFactor(LPCTSTR value);//在線自動校正日期時間-3D高度比例	
	bool                       SaveOnlineAutoCalibrationDateTime_XYZ_Home();//儲存在線自動校正日期時間-XYZ歸零
	bool                       LoadOnlineAutoCalibrationDateTime_XYZ_Home();//載入在線自動校正日期時間-XYZ歸零
	bool                       SaveOnlineAutoCalibrationDateTime_2D_Current();//儲存在線自動校正日期時間-2D電流	
	bool                       LoadOnlineAutoCalibrationDateTime_2D_Current();//載入在線自動校正日期時間-2D電流	
	bool                       SaveOnlineAutoCalibrationDateTime_3D_Current();//儲存在線自動校正日期時間-3D電流	
	bool                       LoadOnlineAutoCalibrationDateTime_3D_Current();//載入在線自動校正日期時間-3D電流	
	bool                       SaveOnlineAutoCalibrationDateTime_3D_ZeroPlane();//儲存在線自動校正日期時間-3D相平面	
	bool                       LoadOnlineAutoCalibrationDateTime_3D_ZeroPlane();//載入在線自動校正時日期時間-3D相平面	
	bool                       SaveOnlineAutoCalibrationDateTime_3D_HeightFactor();//儲存在線自動校正日期時間-3D高度比例	
	bool                       LoadOnlineAutoCalibrationDateTime_3D_HeightFactor();//載入在線自動校正日期時間-3D高度比例	
	bool                       SaveOnlineAutoCalibrationLog(LPCTSTR Text);//儲存在線自動校正訊息
	bool                       SaveOnlineAutoCalibrationLogFn(LPCTSTR Text);//儲存在線自動校正訊息
	//---------------------------------------------------------------------------------//
	//操作主畫面模式	
	bool                       CancelManipulateMainMode();//取消操作主畫面模式-換成選取模式
	MANIPULATE_MAIN_MODE       GetManipulateMainMode() const;
	void                       SetManipulateMainMode(MANIPULATE_MAIN_MODE value);	
	//---------------------------------------------------------------------------------//	
	int                        GetModelIPCLevel() const;//取得模組的IPC等級
	void                       SetModelIPCLevel(int value);//設定模組的IPC等級
	//---------------------------------------------------------------------------------//
	//多聯板操作模式
	MULTI_BOARD_CTRL_MODE      GetMultiBoardCtrlMode() const;
	void                       SetMultiBoardCtrlMode(MULTI_BOARD_CTRL_MODE value);	
	//---------------------------------------------------------------------------------//
	//操作模組模式
	bool                       CancelGatherColorMode();//取消收集顏色模式
	MANIPULATE_MODEL_MODE      GetManipulateModelMode() const;
	void                       SetManipulateModelMode(MANIPULATE_MODEL_MODE value);	
	MANIPULATE_MODEL_MODE      GetManipulateModelModeDefault() const;
	void                       SetManipulateModelModeDefault(MANIPULATE_MODEL_MODE value);	
	//---------------------------------------------------------------------------------//	
	//顯示圖片模式
	DRAW_IMAGE_MODE            GetDrawImageMode() const;
	void                       SetDrawImageMode(DRAW_IMAGE_MODE value);	
	//---------------------------------------------------------------------------------//	
	//現在顯示中圖片模式
	DRAW_IMAGE_MODE            GetDrawingImageMode() const;
	void                       SetDrawingImageMode(DRAW_IMAGE_MODE value);	
	//---------------------------------------------------------------------------------//	
	//顯示模組模式
	DRAW_MODEL_MODE            GetDrawModelMode() const;
	void                       SetDrawModelMode(DRAW_MODEL_MODE value);
	//---------------------------------------------------------------------------------//
	//顯示零件模式
	DRAW_COMPONENT_MODE        GetDrawComponentMode() const;
	void                       SetDrawComponentMode(DRAW_COMPONENT_MODE value);
	//---------------------------------------------------------------------------------//
	//專案作業模式
	PROJECT_TASK_MODE          GetProjectTaskMode() const;
	void                       SetProjectTaskMode(PROJECT_TASK_MODE value);
	bool                       SwitchProjectTaskMode(PROJECT_TASK_MODE value);
	CString                    GetProjectReportFilename(CAOIProject *ProjectPtr) const;
	//---------------------------------------------------------------------------------//
	//繪製選取到物件線
	bool                       GetDrawModelActivedLine() const;
	void                       SetDrawModelActivedLine(bool value);
	//---------------------------------------------------------------------------------//	
	//移動至的事件模式
	void                       SetMoveToEventMode(MOVE_TO_EVENT_MODE value) { m_MoveToEventMode = value;	}
	MOVE_TO_EVENT_MODE         GetMoveToEventMode() const { return m_MoveToEventMode;	}
	//---------------------------------------------------------------------------------//
	//在線顯示瑕疵來源模式
	void                       SetOnlineViewDefectFromeMode(DEFECT_FROM_MODE value) { m_OnlineViewDefectFromeMode = value;	}
	DEFECT_FROM_MODE           GetOnlineViewDefectFromeMode() const { return m_OnlineViewDefectFromeMode;	}
	//---------------------------------------------------------------------------------//
	//是否顯示模組特徵框
	void                       SetShowModelLandBox(bool value) { m_ShowModelLandBox = value;	}
	bool                       GetShowModelLandBox() const { return m_ShowModelLandBox;	}
	//---------------------------------------------------------------------------------//	
	//顯示專案底圖模式
	void                       SetProjectMapMode(bool value) { m_ProjectMapMode = value;	}
	bool                       GetProjectMapMode() const { return m_ProjectMapMode;	}
	//---------------------------------------------------------------------------------//	
	//顯示專案檢測底圖模式	
	bool                       GetProjectShowTestMapMode() const;
	//---------------------------------------------------------------------------------//	
	//變更Ribbon的調式編號
	void                       SetChangeRibbonTuneID(bool value) { m_ChangeRibbonTuneID = value;	}
	bool                       GetChangeRibbonTuneID() const { return m_ChangeRibbonTuneID;	}
	//---------------------------------------------------------------------------------//	
	//顯示檢測框編號
	void                       SetShowModelWndIndex(bool value) { m_ShowModelWndIndex = value;	}
	bool                       GetShowModelWndIndex() const { return m_ShowModelWndIndex;	}
	//---------------------------------------------------------------------------------//	
	//顯示特徵框編號	
	void                       SetShowModelLandIndex(bool value) { m_ShowModelLandIndex = value;	}
	bool                       GetShowModelLandIndex() const { return m_ShowModelLandIndex;	}	
	//---------------------------------------------------------------------------------//	
	//顯示定位點列表
	void                       SetShowFdList(bool value) { m_ShowFdList = value;	}
	bool                       GetShowFdList() const { return m_ShowFdList;	}
	//---------------------------------------------------------------------------------//	
	//顯示特徵點列表
	void                       SetShowMarkList(bool value) { m_ShowMarkList = value;	}
	bool                       GetShowMarkList() const { return m_ShowMarkList;	}
	//---------------------------------------------------------------------------------//	
	//顯示條碼列表
	void                       SetShowBarcodeList(bool value) { m_ShowBarcodeList = value;	}
	bool                       GetShowBarcodeList() const { return m_ShowBarcodeList;	}
	//---------------------------------------------------------------------------------//	
	//顯示整板列表
	void                       SetShowPanelList(bool value) { m_ShowPanelList = value;	}
	bool                       GetShowPanelList() const { return m_ShowPanelList;	}
	//---------------------------------------------------------------------------------//	
	//顯示單板列表
	void                       SetShowBoardList(bool value) { m_ShowBoardList = value;	}
	bool                       GetShowBoardList() const { return m_ShowBoardList;	}
	//---------------------------------------------------------------------------------//
	//顯示零件列表
	void                       SetShowComponentList(bool value) { m_ShowComponentList = value;	}
	bool                       GetShowComponentList() const { return m_ShowComponentList;	}
	//---------------------------------------------------------------------------------//
	//顯示檢測框列表視窗
	bool                       GetShowUIWndWndList() const;
	void                       SetShowUIWndWndList(bool value);	
	//---------------------------------------------------------------------------------//
	//顯示特徵列表視窗	
	bool                       GetShowUIWndMarkList() const;
	void                       SetShowUIWndMarkList(bool value);	
	//---------------------------------------------------------------------------------//
	//顯示模組列表視窗	
	bool                       GetShowUIWndModelList() const;
	void                       SetShowUIWndModelList(bool value);	
	//---------------------------------------------------------------------------------//
	//顯示結果列表視窗
	bool                       GetShowUIWndResultList() const;
	void                       SetShowUIWndResultList(bool value);	
	//---------------------------------------------------------------------------------//
	//顯示零件列表視窗
	bool                       GetShowUIWndComponentList() const;
	void                       SetShowUIWndComponentList(bool value);	
	//---------------------------------------------------------------------------------//
	//顯示料號列表視窗	
	bool                       GetShowUIWndPartNumberList() const;
	void                       SetShowUIWndPartNumberList(bool value);	
	//---------------------------------------------------------------------------------//
	void                       ResetWndMessageFormID();//復原視窗訊息來源編號
	bool                       CheckWndMessageFromMES() const;//確認視窗訊息來源編號-MES
	WND_MESSAGE_FROM_ID        GetWndMessageFromID() const;//視窗訊息來源編號
	void                       SetWndMessageFromID(WND_MESSAGE_FROM_ID val);//視窗訊息來源編號
	//---------------------------------------------------------------------------------//		
	bool                       CheckComponentTypeVisible(COMPONENT_TYPE Type);
	//---------------------------------------------------------------------------------//
	bool                       GetMemoryKeepMode() const;
	bool                       CheckMustUpdateMsg(UINT message, WPARAM wParam, LPARAM lParam);//確認是否一定要更新的訊息
	//---------------------------------------------------------------------------------//		
	void                       SetSpaceMaxHeight(double dHeight);//設定空間最高高度
	double                     GetSpaceMaxHeight() const;//設定空間最高高度
	//---------------------------------------------------------------------------------//
	void                       SetSpaceBaseHeight(double dLevel);//設定空間基準平面高度
	double                     GetSpaceBaseHeight() const;//取得空間基準平面高度
	//---------------------------------------------------------------------------------//	
	void                       SetSpaceToGrayRatioMode(int Mode);//設定高度轉灰階比例
	int                        GetSpaceToGrayRatioMode() const;//取得高度轉灰階比例
	double                     GetSpaceToGrayRatio(int Mode) const;//取得高度轉灰階比例
	double                     GetSpaceToGrayRatio() const;//取得高度轉灰階比例	
	//---------------------------------------------------------------------------------//
	void                       CalcSystemMapCoordinate(CMapCoordinate &CTS, CMapCoordinate &STC);	
	//---------------------------------------------------------------------------------//
	bool                       CheckAIServerIsReady(bool bRealCheck=true);//確認AI伺服器準備好
	void                       BuildAIModelIDList(std::vector<ALG_AI_MODEL_ID> &List);//建立AI模型編號列表
	//---------------------------------------------------------------------------------//	
	const std::vector<WND_DEFECT_ID> &GetWndDefectIDList() const;
	void                       BuildWndDefectIDList(std::vector<WND_DEFECT_ID> &List);//建立檢測框瑕疵代號列表
	void                       RemoveWndDefectID(WND_DEFECT_ID DefectID, std::vector<WND_DEFECT_ID> &List);//移除檢測框瑕疵代號列表	
	void                       BuildModelBasicDefectIDList(MODEL_TYPE ModelType, std::vector<WND_DEFECT_ID> &List);//建立模組基本檢測框瑕疵代號列表	
	//---------------------------------------------------------------------------------//	
	void                       ReleaseModelUniFrameList();
	CAOIModel*                 GetModelUniFrameListModelPtr();	
	bool                       CheckModelUniFrameListModelPtr(CAOIModel *ModelPtr);
	bool                       CheckModelUniFrameListRegion(const TREGION4D RgnStage);	
	bool                       SetModelUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList);//設定模組的影像
	bool                       CopyModelUniFrameList(std::vector<TUNI_FRAME> &UniFrameList, bool bClone) const;//複製模組的影像	
	void                       SetModelUniFrameList(CAOIModel *ModelPtr, const TREGION4D RgnStage, const std::vector<TUNI_FRAME> &UniFrameList, bool bClone);//設定模組的影像	
	bool                       CreateModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &ModelUniFrameList, int nAlign, bool bNoFilter);//將Frame的影像列表轉成Model的影像列表	
	bool                       CreateModelUniFrameList(TUNI_FRAME UniFrameList[],const TPOINT2D &StageCP, const TPOINT2D &ImageRes, CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &ModelUniFrameList, int nAlign, bool bNoFilter);//將Frame的影像列表轉成Model的影像列表	
	bool                       BuildModelUIObjList(CAOIModel *ModelPtr, bool ActiveOnly, std::vector<TActiveObj> &ObjList);//建立模組介面物件列表
	//---------------------------------------------------------------------------------//
	void                       ReleaseFieldUniFrameList();
	TREGION4D                  GetFieldUniFrameStageRegion();
	bool                       SetFieldUniFrameList(const TREGION4D &Region, const std::vector<TUNI_FRAME> &UniFrameList);//設定模組的影像
	bool                       SetFieldUniFrameList(const TREGION4D &Region, const TUNI_FRAME *UniFrameList, size_t Count);//設定模組的影像
	bool                       CopyFieldUniFrameList(std::vector<TUNI_FRAME> &UniFrameList, bool bClone) const;//複製模組的影像		
	bool                       ModifyFieldSpaceImage(std::vector<TUNI_FRAME> &UniFrameList, double nX, double nY, double nZ, double OffsetZ, double OverHigh, double OverLow);//修正灰階影像
	//---------------------------------------------------------------------------------//
	bool                       CreateWndUniFrameList(bool bExtend, CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList);//建立視窗的影像列表
	bool                       CreateWndUniFrameListByModel(bool bExtend, CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList);//建立視窗的影像列表
	bool                       CreateWndUniFrameListByField(bool bExtend, CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList);//建立視窗的影像列表	
	//---------------------------------------------------------------------------------//
	bool                       CloseActiveModel(CAOIModel *ModelPtr);//關閉啟用中的模組
	bool                       CloseActiveComponent(CAOIComponent *ComponentPtr);//關閉啟用中的零件
	//---------------------------------------------------------------------------------//
	void                       SetColorRGBVTemp(const CColorRGBV &rgbv);//影像處理的暫存顏色參數
	void                       GetColorRGBVTemp(CColorRGBV &rgbv) const;//影像處理的暫存顏色參數
	//---------------------------------------------------------------------------------//		
	void                       SetBinaryParamTemp(const CAlgBinaryParam &Param);//影像處理的暫存參數檔案
	void                       GetBinaryParamTemp(CAlgBinaryParam &Param) const;//影像處理的暫存參數檔案
	//---------------------------------------------------------------------------------//	
	void                       SetModelUpdateToGroupParam(const TModelUpdateToGroupParam &Param);//影像處理的暫存參數檔案
	void                       GetModelUpdateToGroupParam(TModelUpdateToGroupParam &Param) const;//影像處理的暫存參數檔案	
	//---------------------------------------------------------------------------------//
	void                       SetViewImageZoom(double val);//設定顯示的影像縮放
	double                     GetViewImageZoom() const;//取得顯示的影像縮放
	//---------------------------------------------------------------------------------//		
	void                       SetFovSizeMinW(double val);//設定視野最低尺寸W
	double                     GetFovSizeMinW() const;//取得視野最低尺寸W
	double                     GetFovSizeMinW_Zoom() const;//取得視野最低尺寸W
	//---------------------------------------------------------------------------------//		
	void                       SetFovSizeMinH(double val);//設定視野最低尺寸H
	double                     GetFovSizeMinH() const;//取得視野最低尺寸H
	double                     GetFovSizeMinH_Zoom() const;//取得視野最低尺寸H
	//---------------------------------------------------------------------------------//
	void                       SetFovPositionX(double val);//設定視野位置-X
	double                     GetFovPositionX() const;//取得視野位置-X
	//---------------------------------------------------------------------------------/
	void                       SetFovPositionY(double val);//設定視野位置-Y
	double                     GetFovPositionY() const;//取得視野位置-Y
	//---------------------------------------------------------------------------------/
	void                       SetTargetMinSizeW(double val);//設定目標最低尺寸W
	double                     GetTargetMinSizeW() const;//取得目標最低尺寸W
	double                     GetTargetMinSizeW_Zoom() const;//取得目標最低尺寸W
	//---------------------------------------------------------------------------------//
	void                       SetTargetMinSizeH(double val);//設定目標最低尺寸H
	double                     GetTargetMinSizeH() const;//取得目標最低尺寸H
	double                     GetTargetMinSizeH_Zoom() const;//取得目標最低尺寸H
	//---------------------------------------------------------------------------------//
	void                       SetFovTargetOffsetX(double val);//設定視野目的相對中心偏差-X
	double                     GetFovTargetOffsetX() const;//取得視野目的相對中心偏差-X
	//---------------------------------------------------------------------------------//		
	void                       SetFovTargetOffsetY(double val);//設定視野目的相對中心偏差-Y
	double                     GetFovTargetOffsetY() const;//取得視野目的相對中心偏差-Y
	//---------------------------------------------------------------------------------//			
	void                       ResetFovTargetParam();//復歸視野與目的參數
	//---------------------------------------------------------------------------------//			
	void                       SetSaveProjectRawImage(bool val);//設定儲存專案原始圖像
	bool                       GetSaveProjectRawImage() const;//取得儲存專案原始圖像
	//---------------------------------------------------------------------------------//	
	void                       SetSaveProjectRawImageFolder(LPCTSTR val);//設定儲存專案原始圖像路徑
	LPCTSTR                    GetSaveProjectRawImageFolder() const;//取得儲存專案原始圖像路徑
	//---------------------------------------------------------------------------------//
	void                       SetSaveDefectImage(bool val);//設定儲存瑕疵影像
	bool                       GetSaveDefectImage() const;//取得儲存瑕疵影像
	//---------------------------------------------------------------------------------//
	void                       SetSaveModelBkImage(bool val);//設定儲存模組底圖
	bool                       GetSaveModelBkImage() const;//取得儲存模組底圖
	//---------------------------------------------------------------------------------//	
	void                       SetOnlineTuningEnable(bool val);//設定啟用線上調機 
	bool                       GetOnlineTuningEnable() const;//取得啟用線上調機 
	//---------------------------------------------------------------------------------//	
	bool                       CheckAutoSwitchToOnlineRemoteCtrlEnable() const;//確認自動切換線上遠端控制啟用
	void                       SetAutoSwitchToOnlineRemoteCtrlEnable(bool val);//設定自動切換線上遠端控啟用
	bool                       GetAutoSwitchToOnlineRemoteCtrlEnable() const;//取得自動切換線上遠端控制啟用
	UINT                       GetAutoSwitchToOnlineRemoteCtrlCmdID(MES_EQP_CTRL_STATE_MODE Mode) const;//取得自動切換線上遠端控制命令編號
	//---------------------------------------------------------------------------------//	
	bool                       AdjustModelBinaryMaskRect(const RECT &ModelRect, const RECT &WndRect, RECT &MaskRect);//調整模組二值化區域	
	//---------------------------------------------------------------------------------//	
	bool                       MoveStageToFd(CAOIFd *FdPtr, bool bForce);//移動機台至定位點
	bool                       MoveStageToPanel(CAOIPanel *PanelPtr, bool bForce);//移動機台至單板中心
	bool                       MoveStageToBoard(CAOIBoard *BoardPtr, bool bForce);//移動機台至單板中心
	bool                       MoveStageToBarcode(CAOIBarcode *BarcodePtr, bool bForce);//移動機台至條碼中心
	//---------------------------------------------------------------------------------//	
	bool                       MoveStageToInspectionFinishComponent(CAOIProject *ProjectPtr);//移動至專案的檢測結果零件	
	bool                       MoveStageToComponent(CAOIComponent *ComponentPtr, bool bForce);//移動機台至零件
	bool                       MoveStageToComponentField(CAOIComponent *ComponentPtr, bool bForce);//移動機台至零件Fov
	bool                       MoveStageToComponentOrField(CAOIComponent *ComponentPtr, bool bForce);//移動機台至零件或零件Fov
	bool                       MoveStageTo(double StageX, double StageY, const TREGION4D &StageRgn);//移動機台至	
	bool                       MoveStageToAct(double StageX, double StageY, const TREGION4D &StageRgn, const TREGION4D &ActStageRgn);//移動機台至
	bool                       ShowStageToAct(double StageX, double StageY, const TREGION4D &StageRgn, const TREGION4D &ActStageRgn);//顯示機台至
	//---------------------------------------------------------------------------------//
	bool                       TurnOnConveyerSensorPower(DWORD DelayTime=0);//開啟軌道感測器電源
	bool                       CheckSystemReady(bool bChkStartLight, bool bAutoReset);//確認系統是否正常	
	//---------------------------------------------------------------------------------//	
	bool                       CheckPCBInside(LANE_ID LaneID);//確認是否有板
	bool                       ExecTargetCapOnProc(bool bWait);//執行開啟塊規上蓋
	bool                       ExecTargetCapOffProc(bool bWait);//執行關閉塊規上蓋
	bool                       CheckTargetCapOnFinish();//確認開啟塊規上蓋完成
	bool                       WaitForTargetCapOnFinish();//等待開啟塊規上蓋完成
	bool                       CheckPCBPassSlowPosition(LANE_ID LaneID, bool RightIn);//確認PCB經過減速位置
	bool                       CheckPCBPassStopPosition(LANE_ID LaneID, bool RightIn);//確認PCB經過停板位置
	bool                       CheckPCBExitStopPosition(LANE_ID LaneID, bool RightIn);//確認PCB離開停板位置
	bool                       CheckPCBPositionByCamera(LANE_ID LaneID, bool RightIn, int CheckMode);//用相機確認PC位置
	bool                       CheckPCBPositionByCameraFn(LANE_ID LaneID, bool RightIn, int CheckMode, CAMERA_ID CameraID);//用相機確認PC位置
	bool                       CheckPCBImage_SlowSensor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinsih);
	bool                       CheckPCBImage_StopSensor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinsih);
	bool                       CheckPCBImagePosition_StopSensor(int LoopCnt, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, double &AveGray, double &Offset);
	bool                       ExecClampPcbBeforeTestFunc();//執行檢測前夾板動作
	bool                       ExecPCBClampOnProc(LANE_ID LaneID);//執行夾板
	bool                       ExecPCBClampOffProc(LANE_ID LaneID);//執行鬆板
	bool                       ExecPCBInByCamera(LANE_ID LaneID);//執行相機進板
	bool                       ExecPCBInByCameraFn(LANE_ID LaneID);//執行相機進板
	bool                       ExecPCBReInByCamera(LANE_ID LaneID);//執行相機再進板
	bool                       ExecPCBReInByCameraFn(LANE_ID LaneID);//執行相機再進板
	bool                       ExecPCBIn2ndByCamera(LANE_ID LaneID);//執行相機2次進板
	bool                       ExecPCBIn2ndByCameraFn(LANE_ID LaneID);//執行相機2次進板
	bool                       ExecPCBIn3rdByCamera(LANE_ID LaneID);//執行相機3次進板
	bool                       ExecPCBIn3rdByCameraFn(LANE_ID LaneID);//執行相機3次進板	
	bool                       ExecPCBInProc(LANE_ID LaneID, bool bWait, bool bStep);//執行進板
	bool                       ExecPCBOutProc(LANE_ID LaneID, bool bWait, bool bStep);//執行出板
	bool                       ExecPCBBackProc(LANE_ID LaneID, bool bWait, bool bStep);//執行退板
	bool                       ExecPCBBackOutProc(LANE_ID LaneID, bool bWait, bool bStep);//執行退出板	
	bool                       ExecPCBReInProc(LANE_ID LaneID, bool bWait, bool bStep);//執行再進板
	bool                       ExecPCBClearProc(LANE_ID LaneID, bool bWait, bool bStep);//執行清板	
	bool                       ExecPCBIn2ndProc(LANE_ID LaneID, bool bWait, bool bStep);//執行第2段進板
	bool                       ExecPCBIn3rdProc(LANE_ID LaneID, bool bWait, bool bStep);//執行第3段進板
	bool                       ExecPCBOutInProc(LANE_ID LaneID, bool bWait, bool bStep);//執行出板帶進板
	bool                       ExecPCBDualRunProc(LANE_ID LaneID, bool bWait, bool bStep, CAOIProject *ProjectPtr, PCB_OUT_DIRECTION eDirection, ONLINE_STATE_MODE StateMode);//執行雙軌出板//20230425
	bool                       ExecPCBAutoRunProc(LANE_ID LaneID, bool bWait, bool bStep, PCB_OUT_DIRECTION eDirection);//執行自動出板//20230425
	bool                       ExecPCBAutoOutInProc(LANE_ID LaneID, bool bWait, bool bStep);//執行自動出進板
	bool                       ExecPCBAutoBackInProc(LANE_ID LaneID, bool bWait, bool bStep);//執行自動出進板	
	bool                       ExecConveyorMotorStop(LANE_ID LaneID);//執行軌道停止
	bool                       ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow);//執行軌道運轉
	bool                       ResetPCBOKNGSignal(LANE_ID LaneID);//清除OK/NG訊號
	bool                       SendPCBOKNGSignal(LANE_ID LaneID, bool bNG);//發送OK/NG訊號	
	//---------------------------------------------------------------------------------//
	bool                       ExecReloadProject(LANE_ID LaneID);//在線重載專案	
	bool                       ExecReloadServerProject(LANE_ID LaneID);//在線重載伺服器專案
	bool                       ClearProjectStatisticBackup(TProjectStatisticBackup &StatisticBackup);//清除專案統計資料
	bool                       WriteOfflineSaveFinish(const UUID &uuid, LPCTSTR filename);//寫入離線存檔完成
	bool                       CheckOfflineSaveFinish(const UUID &uuid, LPCTSTR filename);//確認離線存檔完成	
	bool                       WriteProjectShareFile(const UUID &uuid, LPCTSTR PrgName, LPCTSTR KeyName, LPCTSTR Value);//寫入專案共享資料	
	bool                       WriteProjectShareFileFn(const UUID &uuid, LPCTSTR PrgName, LPCTSTR KeyName, LPCTSTR Value);//寫入專案共享資料	
	bool                       ReadProjectShareFile(const UUID &uuid, LPCTSTR PrgName, LPCTSTR KeyName, LPCTSTR Default, CString &Value);//讀取專案共享資料
	bool                       ReadProjectShareFileFn(const UUID &uuid, LPCTSTR PrgName, LPCTSTR KeyName, LPCTSTR Default, CString &Value);//讀取專案共享資料
	//---------------------------------------------------------------------------------//
	//About XYZ Motion
	bool                       MoveCameraToLaneLEDSlowSensorPos(LANE_ID LaneID, bool RightSide, bool Wait);//移動至PCB減速感測器位置
	bool                       MoveCameraToLaneLEDStopSensorPos(LANE_ID LaneID, bool RightSide, bool Wait);//移動至PCB停板感測器位置
	bool                       MoveCameraToBeforePCBInPosition(bool Wait);//移動至進板前位置
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetConveyerFirstProjectPtr(LANE_ID LaneID);//取得軌道上的第1個專案
	bool                       MoveCameraToProjectFocusPos(CAOIProject *ProjectPtr);//移動至專案焦距位置
	bool                       MoveCameraToConveyerFirstPos(TOnlineProcParam &Param, CAOIProject *ProjectPtr, bool bResetOKNG);
	bool                       MoveCameraToProjectFirstPos(CAOIProject *ProjectPtr, LANE_ID LaneID, DISTRICT_ID  DistrictID, bool bResetOKNG);//移動至專案第1個位置
	//---------------------------------------------------------------------------------//	
	bool                       ExecMemoryTest();//記憶體測試
	//---------------------------------------------------------------------------------//	
	bool                       GetStagePos(double &PosX, double &PosY, double &PosZ);//取得機台位置, 會考慮是否為Offline
	//---------------------------------------------------------------------------------//	
	double                     GetImageZoomMin() const;//取得影像縮放最小
	double                     GetImageZoomMax() const;//取得影像縮放最大
	double                     GetImageZoomMin_Act() const;//取得影像縮放最小-局部標的
	double                     AdjustImageZoom(double Zoom);//調整影像縮放
	//---------------------------------------------------------------------------------//
	int                        GetAOILibraryFromID() const;//取得AOI資料庫來源編號
	void                       SetAOILibraryFromID(int val);//設定AOI資料庫來源編號
	//---------------------------------------------------------------------------------//
	bool                       CloseProject(CAOIProject *ProjectPtr, bool bOnline);//關閉專案
	bool                       OpenProject(CAOIProject *ProjectPtr, LPCTSTR filename, bool bOnline);//開起專案
	bool                       OpenProjectFn(CAOIProject *ProjectPtr, LPCTSTR filename, bool bOnline);//開起專案
	bool                       ExecProjectLoadFromServer(CAOIProject *ProjectPtr, bool bOnline);//執行載入伺服器專案
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetAOILibraryName() const;
	CString                    GetAOILibraryAliasFilename() const;
	bool                       ClearAOILibrary();//清除資料庫
	CAOIProject*               LoadAOILibrary(LPCTSTR pfilename);//載入資料庫
	CAOIProject*               LoadAOILibraryFn(LPCTSTR pfilename);//載入資料庫
	bool                       SaveAOILibraryAs(LPCTSTR pfilename);//另存資料庫	
	bool                       ReBuildAOILibrary(LPCTSTR pfilename);//重整資料庫
	bool                       MergeAOILibrary(CAOIProject *ProjectPtr);//合併資料庫	
	//---------------------------------------------------------------------------------//	
	bool                       SaveAOIMonitorStatus(MONITOR_STATUS_MODE Status, LPCTSTR Text);//儲存AOI設備監控狀態	
	bool                       SaveAOIMonitorStatusFn(MONITOR_STATUS_MODE Status, LPCTSTR Text);//儲存AOI設備監控狀態	
	bool                       SaveAOIMonitorStatus(ONLINE_STATE_MODE OnlineStatus, LPCTSTR Text);//儲存AOI設備監控狀態
	bool                       SaveAOIMonitorStatusFn(ONLINE_STATE_MODE OnlineStatus, LPCTSTR Text);//儲存AOI設備監控狀態
	//---------------------------------------------------------------------------------//
	bool                       LoadNameFile(LPCTSTR pfilename, std::vector<std::wstring> &NameList);//載入名稱檔案
	bool                       LoadNameFileFn(LPCTSTR pfilename, std::vector<std::wstring> &NameList);//載入名稱檔案
	//---------------------------------------------------------------------------------//
	bool                       LoadAliasFile(LPCTSTR pfilename, std::vector<TAliasNode> &AliasList);//載入別名檔案
	bool                       LoadAliasFileFn(LPCTSTR pfilename, std::vector<TAliasNode> &AliasList);//載入別名檔案
	bool                       SaveAliasFile(LPCTSTR pfilename, std::vector<TAliasNode> &AliasList);//儲存別名檔案	
	bool                       SaveAliasFileFn(LPCTSTR pfilename, std::vector<TAliasNode> &AliasList);//儲存別名檔案	
	bool                       MergeAliasList(const std::vector<TAliasNode> &List1, const std::vector<TAliasNode> &List2, std::vector<TAliasNode> &AliasList);//合併兩個別名列表
	//---------------------------------------------------------------------------------//	
	bool                       AddRemoveFolder_ProjectTemp(LPCTSTR Filename);//新增移除資料夾-暫存專案
	bool                       AddRemoveFolder_OnlineTuning(LPCTSTR Folder);//新增移除資料夾-線上調機
	//---------------------------------------------------------------------------------//
	CString                    GetDefaultLatestFilename() const;//預設最近檔案名稱
	size_t                     GetMaxLatestFileCount() const;//最多紀錄最近檔案數量
	bool                       AddLatestFilename(LPCTSTR pfilename);//增加最近檔案列表
	bool                       UpdateLatestFilenameList(bool Offline);//更新最近檔案列表
	bool                       CheckLatestFilenameList(std::vector<CString> &FileNameList, bool Offline);//取得最近檔案列表	
	//---------------------------------------------------------------------------------//	
	bool                       ExecLaneAdjustHome();//執行軌道移動
	bool                       ExecLaneAdjustHome(LANE_ID LaneID);//執行軌道移動	
	bool                       ExecLaneAdjustWidth(LANE_ID LaneID, double Width);//執行軌道移動
	//---------------------------------------------------------------------------------//		
	bool                       SendToVRS_AOIState(ONLINE_STATE_MODE State);//傳送至VRS軟體-機台狀態	
	bool                       SendToVRS_ProjectList();//傳送至VRS軟體-目前專案列表		
	bool                       SendToVRS_MachineInfo();//傳送至VRS軟體-機台訊息
	bool                       SendToVRS_SaveINIData(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR String);//傳送至VRS軟體-儲存INI資料
	bool                       SendToVRS_SaveINIDataFn(LPCTSTR Section, LPCTSTR KeyName, LPCTSTR String);//傳送至VRS軟體-儲存INI資料
	//---------------------------------------------------------------------------------//	
	double                     CalcLaneAB_PCBStopOffsetX() const;//計算AB軌道偏差值-X
	double                     CalcLaneAB_PCBStopOffsetY() const;//計算AB軌道偏差值-Y
	double                     CalcLaneAB_PCBStopOffsetZ() const;//計算AB軌道偏差值-Z	
	bool                       MapStagePosLaneAtoB(double &PosX, double &PosY);//鏡射A軌道座標至B軌道
	bool                       MapStagePosLaneBtoA(double &PosX, double &PosY);//鏡射B軌道座標至A軌道		
	bool                       MapStageRegionLaneAtoB(TREGION4D &Region);//鏡射A軌道座標至B軌道
	bool                       MapStageRegionLaneBtoA(TREGION4D &Region);//鏡射B軌道座標至A軌道
	bool                       MapStagePosToLaneA(double &PosX, double &PosY, LANE_ID LaneID);//鏡射軌道座標, 以A軌道為主	
	bool                       MapStagePosLaneByLaneID(double &PosX, double &PosY, LANE_ID LaneID);//鏡射軌道座標, 以A軌道為主		
	bool                       MapStagePosLaneByLaneID(double &PosX, double &PosY, double &PosZ, LANE_ID LaneID);//鏡射軌道座標, 以A軌道為主		
	bool                       MapStageRegionLaneByLaneID(TREGION4D &Region, LANE_ID LaneID);//鏡射軌道座標, 以A軌道為主	
	//---------------------------------------------------------------------------------//	
	void                       ResetSwitchMultiLine_NextLaneID();//復歸切換多軌道次序已做過//20230425	
	LANE_ID                    GetSwitchMultiLine_NextLaneID() const;//切換多軌道次序已做過//20230425	
	void                       SetSwitchMultiLine_NextLaneID(LANE_ID LaneID);////切換多軌道次序已做過//20230425		
	//---------------------------------------------------------------------------------//	
	bool                       SwitchMultiLineOrder(TOnlineProcParam &Param, ONLINE_STATE_MODE &OnlineState);//切換多軌道次序	
	bool                       CheckLaneAutoRunFinish(LANE_ID LaneID, bool &Finish);//確認軌道自動運轉完成
	bool                       CheckOtherLaneAutoRunFinish(LANE_ID CurLaneID, LANE_ID &NextLaneID);//確認其他軌道完成
	//---------------------------------------------------------------------------------//	
	bool                       SwitchMultiLineOrder_Dual(TOnlineProcParam &Param, ONLINE_STATE_MODE &OnlineState);//切換多軌道次序//20230425
	bool                       CheckLaneDualRunFinish(LANE_ID LaneID, bool &Finish);//確認軌道雙軌運轉完成	
	bool                       CheckOtherLaneDualRunFinish(LANE_ID CurLaneID, LANE_ID &NextLaneID);//確認其他軌道完成
	//---------------------------------------------------------------------------------//
	//操作人員		
	CString                    GetUserFilename() const;//使用者檔案名稱
	TUserNode                  GetLoginUserNode() const;//使用者資料
	LPCTSTR                    GetCurrentUserName() const;//使用者名稱
	USER_LEVEL_MODE            GetCurrentUserLevel() const;//使用者權限	
	bool                       CreateDefaultUserFile();//建立預設使用者	
	bool                       AddDefaultUser_JetSenior();//新增加預設使用者
	bool                       CheckOnlineViewAutoLogout() const;//確認上線頁面自動登出
	bool                       CheckOperatorLoginWithPassword() const;//確認作業員需要密碼
	bool                       SwitchUser(bool bWait=false);//切換使用者
	bool                       UserLogout();//使用者登出
	bool                       UserLogoutFn();//使用者登出
	bool                       UserLogout_Check();//使用者登出
	bool                       UserLogin_Operator();//使用者登入-作業員	
	bool                       UserLogin_Engineer();//使用者登入-工程師
	bool                       UserLogin_Supervisor();//使用者登入-管理者
	bool                       UserLogin_VendorJET();//使用者登入-捷智工程師
	bool                       UserLogin_OnlineRun();//使用者登入-線上運行
	bool                       UserLogin_OnlineAutoStop();//使用者登入-線上停機
	bool                       UserLogin_OnlineCalibration();//使用者登入-線上校正
	bool                       UserLogin_Sync(USER_LEVEL_MODE Level);//使用者登入
	bool                       UserLogin(USER_LEVEL_MODE Level, bool bForce);//使用者登入
	bool                       UserLoginFn(USER_LEVEL_MODE Level, bool bForce);//使用者登入
	bool                       UserLevel_MachineSN();//使用者等級-編輯機台序號
	bool                       UserLevel_Operator(bool bChkLoginMode=true);//使用者等級-作業員
	bool                       UserLevel_Engineer(bool bChkLoginMode=true);//使用者等級-工程師
	bool                       UserLevel_Supervisor(bool bChkLoginMode=true);//使用者等級-管理者
	bool                       UserLevel_VendorJET(bool bChkLoginMode=true);//使用者等級-捷智工程師	
	bool                       UserLevel_Check(USER_LEVEL_MODE Level, bool bChkLoginMode);//使用者等級-確認
	bool                       FindUserNode(LPCTSTR name, LPCTSTR password, TUserNode &User);//確認使用者	
	bool                       FindUserNode(const std::vector<TUserNode> &UserList, LPCTSTR name);//確認使用者
	bool                       ReadUserFile(LPCTSTR pfilename, std::vector<TUserNode> &UserList);//讀取使用者列表
	bool                       ReadUserFileFn(LPCTSTR pfilename, std::vector<TUserNode> &UserList);//讀取使用者列表
	bool                       WriteUserFile(LPCTSTR pfilename, const std::vector<TUserNode> &UserList);//寫入使用者列表
	bool                       WriteUserFileFn(LPCTSTR pfilename, const std::vector<TUserNode> &UserList);//寫入使用者列表
	bool                       BuildUserNode(LPCTSTR name, LPCTSTR password, USER_LEVEL_MODE Level, TUserNode &User);//建立使用者	
	//---------------------------------------------------------------------------------//		
	bool                       OperateLevelProjectOpen();//操作等級-專案開啟
	bool                       OperateLevelProjectSave();//操作等級-專案儲存
	bool                       OperateLevelProjectParam();//操作等級-專案參數
	bool                       OperateLevelOnlineRun();//操作等級-線上運行
	bool                       OperateLevelOnlineBypass();//操作等級-線上直通
	bool                       OperateLevelOnlineStop();//操作等級-線上停止
	bool                       OperateLevelOnlineSaveImage();//操作等級-線上存圖	
	bool                       OperateLevelOnlineUnlock(bool bThread);//操作等級-線上解鎖
	bool                       OperateLevelOnlineUnlockAlarm(bool bThread);//操作等級-線上解鎖警報
	bool                       OperateLevelOnlineUnlockAutoStop();//操作等級-線上解鎖自動停機
	bool                       OperateLevelOnlineUnlockCalibration();//操作等級-線上解鎖校正
	bool                       OperateLevelEditFuncAdd();//操作等級-編輯功能-新增
	bool                       OperateLevelEditFuncAddFd();//操作等級-編輯功能-新增定位點
	bool                       OperateLevelEditFuncAddMark();//操作等級-編輯功能-新增特徵
	bool                       OperateLevelEditFuncAddModel();//操作等級-編輯功能-新增模組
	bool                       OperateLevelEditFuncAddPanel();//操作等級-編輯功能-新增整板
	bool                       OperateLevelEditFuncAddBoard();//操作等級-編輯功能-新增單板
	bool                       OperateLevelEditFuncAddBarcode();//操作等級-編輯功能-新增條碼
	bool                       OperateLevelEditFuncAddModelWnd();//操作等級-編輯功能-新增模組檢測框
	bool                       OperateLevelEditFuncAddComponent();//操作等級-編輯功能-新增零件
	bool                       OperateLevelEditFuncDel();//操作等級-編輯功能-刪除
	bool                       OperateLevelEditFuncDelFd();//操作等級-編輯功能-刪除定位點
	bool                       OperateLevelEditFuncDelMark();//操作等級-編輯功能-刪除特徵	
	bool                       OperateLevelEditFuncDelModel();//操作等級-編輯功能-刪除模組
	bool                       OperateLevelEditFuncDelPanel();//操作等級-編輯功能-刪除整板
	bool                       OperateLevelEditFuncDelBoard();//操作等級-編輯功能-刪除單板
	bool                       OperateLevelEditFuncDelBarcode();//操作等級-編輯功能-刪除條碼
	bool                       OperateLevelEditFuncDelModelWnd();//操作等級-編輯功能-刪除模組檢測框
	bool                       OperateLevelEditFuncDelComponent();//操作等級-編輯功能-刪除零件
	bool                       OperateLevelEditFuncDelProjectFile();//操作等級-編輯功能-刪除專案檔案
	bool                       OperateLevelEditFuncBypass();//操作等級-編輯功能-不檢測
	bool                       OperateLevelEditFuncBypassFd();//操作等級-編輯功能-不檢測定位點
	bool                       OperateLevelEditFuncBypassMark();//操作等級-編輯功能-不檢測特徵
	bool                       OperateLevelEditFuncBypassModel();//操作等級-編輯功能-不檢測模組
	bool                       OperateLevelEditFuncBypassPanel();//操作等級-編輯功能-不檢測整板
	bool                       OperateLevelEditFuncBypassBoard();//操作等級-編輯功能-不檢測單板
	bool                       OperateLevelEditFuncBypassBarcode();//操作等級-編輯功能-不檢測條碼
	bool                       OperateLevelEditFuncBypassModelWnd();//操作等級-編輯功能-不檢測模組檢測框 
	bool                       OperateLevelEditFuncBypassComponent();//操作等級-編輯功能-不檢測零件
	bool                       OperateLevelEditFuncSetPlcSaftyPass();//操作等級-編輯功能-設定PLC關閉安全檢知
	bool                       OperateLevelEditFuncUsePlcSaftyPass();//操作等級-編輯功能-使用PLC關閉安全檢知
	bool                       OperateLevelEditFuncControlCenter();//操作等級-編輯功能-清除中控資料
	bool                       OperateLevelEditWndSystemConfig();//操作等級-編輯視窗-系統組態
	bool                       OperateLevelEditWndBarcodeDevice();//操作等級-編輯視窗-外接條碼機
	bool                       OperateLevelEditWndProjectColor();//操作等級-編輯視窗-專案色彩
	bool                       OperateLevelEditWndProjectFdList();//操作等級-編輯視窗-專案定位點列表
	bool                       OperateLevelEditWndProjectPanelList();//操作等級-編輯視窗-專案整板列表
	bool                       OperateLevelEditWndProjectBoardList();//操作等級-編輯視窗-專案單板列表
	bool                       OperateLevelEditWndProjectBarcodeList();//操作等級-編輯視窗-專案條碼列表
	bool                       OperateLevelEditWndProjectComponentList();//操作等級-編輯視窗-專案零件列表
	bool                       OperateLevelEditWndProjectFdSort();//操作等級-編輯視窗-專案定位點排序
	bool                       OperateLevelEditWndProjectOpenCode();//操作等級-編輯視窗-專案開檔碼
	bool                       OperateLevelEditWndOperatorLog();//操作等級-編輯視窗-操作訊息	
	bool                       OperateLevelEditWndUserRegister();//操作等級-編輯視窗-使用者註冊
	bool                       OperateLevelEditWndRemoteParam();//操作等級-編輯視窗-設定遠端參數
	bool                       OperateLevelEditWndPhaseCtrl();//操作等級-編輯視窗-相位控制
	bool                       OperateLevelEditWndCalibration();//操作等級-編輯視窗-系統校正
	bool                       OperateLevelEditWndCudaCtrl();//操作等級-編輯視窗-Cuda控制
	bool                       OperateLevelEditWndImageConfig();//操作等級-編輯視窗-影像組態
	bool                       OperateLevelEditWndLightCtrlBoard();//操作等級-編輯視窗-燈源控制板
	bool                       OperateLevelEditWndAllPhaseDebug();//操作等級-編輯視窗-四相位偵錯
	bool                       OperateLevelEditWndITSComm();//操作等級-編輯視窗-ITS通訊	
	bool                       OperateLevelMesCtrlState(MES_EQP_CTRL_STATE_MODE Mode);//操作等級-Mes控制狀態
	bool                       OperateLevelMesShowContext();//操作等級-Mes顯示內容
	bool                       OperateLevel_Operator();//操作等級-作業員
	bool                       OperateLevel_Engineer();//操作等級-工程師
	bool                       OperateLevel_Supervisor();//操作等級-主管
	bool                       OperateLevel_VendorJET();//操作等級-JET廠商
	bool                       OperateLevelFn(USER_LEVEL_MODE Level, bool bForce=false, bool bSync=false);//操作等級
	//---------------------------------------------------------------------------------//	
	//外接條碼機
	bool                       CreateBarcodeDevice();//建立條碼機物件
	bool                       CreateBarcodeDevice(const TBarcodeDevice &BarcodeParam);//建立條碼機物件	
	bool                       DestroyBarcodeDevice();//摧毀條碼機物件	
	bool                       SetBarcodeDeviceIDList_LA(int BarcodeIDList[]);//設定條碼機編號列表-LA
	bool                       CloneBarcodeDeviceIDList_LA(int BarcodeIDList[]);//複製條碼機編號列表-LA
	bool                       SetBarcodeDeviceIDList_LB(int BarcodeIDList[]);//設定條碼機編號列表-LB
	bool                       CloneBarcodeDeviceIDList_LB(int BarcodeIDList[]);//複製條碼引機編號表-LB
	bool                       CheckBarcodeDeviceIDList_LA(bool EnableList[]);//確認條碼引數列表-LA
	bool                       CheckBarcodeDeviceIDList_LB(bool EnableList[]);//確認條碼引數列表-LA
	bool                       CloneOpenProjectBarcodeDeviceUsedList(bool UsedList[]);//複製開啟專案條碼機使用列表
	//---------------------------------------------------------------------------------//	
	CBarcode_Basic*            GetBarcodeDevicePtr(int DeviceID);//取得條碼機指標
	bool                       GetBarcodeDevicePtr(int DeviceID, CBarcode_Basic *&Ptr);//取得條碼機指標
	bool                       GetBarcodeDeviceParam(int DeviceID, TBarcodeDevice &BarcodeParam);//取得條碼參數
	bool                       SetBarcodeDeviceParam(int DeviceID, const TBarcodeDevice &BarcodeParam);//設定條碼參數	
	//---------------------------------------------------------------------------------//	
	bool                       ExecBarcodeDeviceEndReading(LANE_ID LaneID);//執行條碼機結束讀取條碼	
	//---------------------------------------------------------------------------------//
	bool                       CheckOpenProjectBarcodeCanRun(ONLINE_FROM_MODE eFrom) const;//確認可執行開專案條碼
	bool                       ExecOpenProjectBarcodeHandHeldReading(LANE_ID LaneID);//執行開專案手持條碼機讀取條碼	
	bool                       ExecOpenProjectBarcodeDeviceStartToRead(ONLINE_STATE_MODE OnlineState, LANE_ID LaneID, bool BeforeRetrieveCode);//執行開專案條碼機開始讀取條碼
	bool                       ExecOpenProjectBarcodeDeviceRetrieveCode(ONLINE_STATE_MODE OnlineState, LANE_ID LaneID);//執行開專案條碼機讀取條碼	
	//---------------------------------------------------------------------------------//
	bool                       ExecProjectBarcodeDeviceInputCode(CAOIProject *ProjectPtr, CBarcode_Basic* BarcodeDevicePtr, LANE_ID LaneID);//執行專案條碼機輸入條碼	
	bool                       ExecProjectBarcodeDeviceStartToRead(CAOIProject *ProjectPtr, ONLINE_STATE_MODE OnlineState, LANE_ID LaneID, bool BeforeRetrieveCode);//執行專案條碼機開始讀取條碼
	bool                       ExecProjectBarcodeDeviceRetrieveCode(CAOIProject *ProjectPtr, ONLINE_STATE_MODE OnlineState, LANE_ID LaneID);//執行專案條碼機讀取條碼		
	//---------------------------------------------------------------------------------//	
	//手持式條碼機
	DWORD                      GetBarcodeHandHeldResult() const;
	void                       SetBarcodeHandHeldResult(DWORD val);
	LANE_ID                    GetBarcodeHandHeldLaneID() const;
	void                       SetBarcodeHandHeldLaneID(LANE_ID val);
	CAOIProject*               GetBarcodeHandHeld_Project() const;
	void                       SetBarcodeHandHeld_Project(CAOIProject* Ptr);	
	CBarcode_Handheld*         GetBarcodeHandHeldPtr(LANE_ID LaneID);
	void                       SetBarcodeHandHeld(LANE_ID LaneID, const CBarcode_Handheld &BarcodeHandHeld);
	bool                       ExecProjectBarcodeHandHeldReading(CAOIProject *ProjectPtr, LANE_ID LaneID);//執行專案手持條碼機讀取條碼	
	//---------------------------------------------------------------------------------//
	//條碼帶專案-專案開檔碼
	CString                    GetProjectOpenCodeFilename();//取得專案開檔碼檔案名稱
	bool                       DeleteProjectOpenCode(LPCTSTR ProjectName);//刪除專案開檔碼
	bool                       UpdateProjectOpenCode(const std::vector<TProjectOpenCode> &ProjectOpenCodeList);//更新專案開檔碼檔案
	bool                       SaveProjectOpenCode(LPCTSTR ProjectName, LPCTSTR OpenCode);//儲存專案開檔碼
	bool                       LoadProjectOpenCodeFile(LPCTSTR filename, std::vector<TProjectOpenCode> &ProjectOpenCodeList);//載入專案開檔碼檔案
	bool                       LoadProjectOpenCodeFileFn(LPCTSTR filename, std::vector<TProjectOpenCode> &ProjectOpenCodeList);//載入專案開檔碼檔案
	bool                       SaveProjectOpenCodeFile(LPCTSTR filename, const std::vector<TProjectOpenCode> &ProjectOpenCodeList);//儲存專案開檔碼檔案
	bool                       SaveProjectOpenCodeFileFn(LPCTSTR filename, const std::vector<TProjectOpenCode> &ProjectOpenCodeList);//儲存專案開檔碼檔案
	bool                       FindProjectOpenCode(LPCTSTR filename, std::wstring &OpenCode);//尋找專案的開檔碼
	bool                       FindProjectListByCode(const std::wstring &OpenCode, std::vector<CString> &ProjectList);//尋找相同的專案開檔碼	
	//---------------------------------------------------------------------------------//
	//預覽模組指標
	CAOIModel*                 GetModelPreViewPtr();//取得預覽模組指標	
	bool                       GetShowModelPreViewMode() const;//取得是否顯示預覽模組	
	bool                       CreateModelPreViewPtr(CAOIModel *ModelPtr);//建立預覽模組指標
	bool                       DestroyModelPreViewPtr();//清除預覽模組指標
	//---------------------------------------------------------------------------------//	
	bool                       UpdateUIWndFont(HWND hWnd);//更新視窗字型
	bool                       UpdateUIWndFontFn(HWND hWnd);//更新視窗字型
	bool                       CreateUIWndFont(HWND hWnd, CFont &rFont);//建立視窗字型
	bool                       CreateUIWndFontFn(HWND hWnd, CFont &rFont);//建立視窗字型
	//---------------------------------------------------------------------------------//	
	bool                       DrawProjectDistrictRect(HDC hDC, const RECT &WndRect, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-分段區域
	bool                       DrawProjectPanel(HDC hDC, const RECT &WndRect, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-單板列表
	bool                       DrawProjectBoard(HDC hDC, const RECT &WndRect, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-單板列表
	bool                       DrawProjectCameraRect(HDC hDC, const RECT &WndRect, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-零件列表	
	bool                       DrawProjectComponent(HDC hDC, const RECT &WndRect, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-零件列表
	bool                       DrawEditViewInspection(HDC hDC, const RECT &WndRect, ONLINE_STATE_MODE OnlineState, CAOIProject *ProjectPtr, double Zoom);//繪製編輯頁面-檢測畫面	
	//---------------------------------------------------------------------------------//
	SPC_TOWARD                 MapBoxTowardToSpcToward(BOX_TOWARD Toward);//將框朝向轉成SPC的朝向
	//---------------------------------------------------------------------------------//
	AI_RESULT_ID               MapResultIDToAiResultID(RESULT_ID ResultID);//將結果編號轉成AI的結果編號		
	RESULT_ID                  MapAiResultIDToResultID(AI_RESULT_ID AiResultID);//將AI結果編號轉成的結果編號
	//---------------------------------------------------------------------------------//		
	SPC_RESULT_ID              MapResultIDToSpcResultID(RESULT_ID ResultID);//將結果編號轉成SPC的結果編號		
	RESULT_ID                  MapSpcResultIDToResultID(SPC_RESULT_ID SpcResultID);//將SPC結果編號轉成的結果編號
	//---------------------------------------------------------------------------------//		
	SPC_RESULT_ID              MapTestResultIDToSpcResultID(TEST_RESULT_ID ResultID);//將檢測結果編號轉成SPC的結果編號
	TEST_RESULT_ID             MapSpcResultIDToTestResultID(SPC_RESULT_ID SpcResultID);//將檢測結果編號轉成SPC的結果編號
	//---------------------------------------------------------------------------------//		
	bool                       BuildMotionXYCaliList(bool Rebuild);//建立運動系統的 XY校正表
	//--------------------------------------------------------------------------//
	bool                       GetShowRibbonBarWndCategoryDebug() const;//取得是否顯示Ribbon視窗的偵錯分類
	//--------------------------------------------------------------------------//
	COLORREF                   GetColorModelUnset();//未設定顏色
	//--------------------------------------------------------------------------//	
	void                       ClearTempWndList();//清除暫時檢測框列表
	size_t                     GetTempWndCount() const;//取得暫時檢測框數量
	CAOIWnd*                   GetTempWndPtr(size_t idx, bool bCheck);//取得暫時檢測框指標
	void                       SetTempWndList(std::vector<CAOIWnd*> WndList);//設定暫時檢測框列表
	void                       GetTempWndList(std::vector<CAOIWnd*> &WndList);//取得暫時檢測框列表
	//--------------------------------------------------------------------------//
	//螢幕鎖住	
	bool                       ExecLockScreen(WPARAM wParam, HWND hWnd);
	bool                       UnlockScreenFunc();//取消鎖住
	bool                       LockScreenFunc(HWND hWnd);//啟用鎖住	
	bool                       GetLockScreenEnabled();//取得是否鎖住螢幕
	void                       SetLockScreenEnabled(bool bVal);//設定是否鎖住螢幕
	HWND                       GetLockScreenLimitWnd();//取得限制視窗 LockScreen
	void                       SetLockScreenLimitWnd(HWND hWnd);//設定限制視窗 LockScreen	
	//--------------------------------------------------------------------------//	
	//MES軟體
	bool                       GetMES_RemoteCtrlOn();//MES遠端啟用	
	MES_EQP_CTRL_STATE_MODE    GetMES_EqpCtrlStateMode();//MES機台控制狀態	
	LPCTSTR                    GetMES_RemoteCtrlMessage() const;//MES遠端控制訊息
	void                       SetMES_RemoteCtrlMessage(LPCTSTR val);//MES遠端控制訊息
	void                       ResetMESComm_RemoteCtrlProject();//MES遠端控制專案
	LPCTSTR                    GetMESComm_RemoteCtrlProject() const;//MES遠端控制專案
	void                       SetMESComm_RemoteCtrlProject(LPCTSTR val);//MES遠端控制專案
	CString                    GetMESComm_ProjectFullName(LPCTSTR val) const;//MES遠端控制專案
	bool                       CheckMESComm_ProjectIsEmpty();//MES專案空的
	bool                       CheckMESComm_ProjectIsFree(LPCTSTR val);//MES專案是自由的
	bool                       CheckMESComm_ProjectUploadCanRun(LPCTSTR val);//專案上傳檔名
	bool                       CheckMESComm_ProjectDownloadCanRun(LPCTSTR val);//專案下載檔名
	bool                       CheckMESComm_ProjectDeleteCanRun(LPCTSTR val);//專案刪除檔名	

	size_t                     GetMESRecvNodeCount();	
	bool                       GetMES_SecsGemEnabled() const;
	bool                       GetMES_CommunicationEnabled() const;
	bool                       CheckMES_CtrlState(MES_EQP_CTRL_STATE_MODE Mode);	
	bool                       ProcesMESRecvNodeList(bool bThread);//處理收到MES的訊息列表
	bool                       SaveMESTimeMsg(const char *pContext);//儲存MES時間訊息
	bool                       SaveMESTimeMsg(const wchar_t *pContext);//儲存MES時間訊息
	bool                       ExecMESComm_ApplySystemParam();//執行MES溝通-更新系統參數至MES
	bool                       ExecMESComm_CheckMESReady();//執行MES溝通-確認MES就緒
	bool                       ExecMESComm_SetSystemParam();//執行MES溝通-設定系統參數
	bool                       ExecMESComm_SetProjectParam();//執行MES溝通-設定專案參數
	bool                       ExecMESComm_SetParamChanged();//執行MES溝通-參數變更
	bool                       ExecMESComm_SetProjectLoadFinished(CAOIProject *ProjectPtr, bool bSucc, LPCTSTR Err);//執行MES溝通-專案載入完畢
	bool                       ExecMESComm_UploadProjectFile(LPCTSTR filename);//執行MES溝通-上傳專案檔案
	bool                       ExecMESComm_DownloadProjectFile(LPCTSTR filename);//執行MES溝通-下載專案檔案
	bool                       ExecMESComm_SetMachineStatus();//執行MES溝通-設定機台狀態
	bool                       ExecMESComm_SetMachineStatus(ONLINE_STATE_MODE Status);//執行MES溝通-設定機台狀態	
	bool                       ExecMESComm_ProcessID(int ProcessID, bool ChkEnb=true);//執行MES溝通-程序運作
	bool                       ExecMESComm_SetAOIExceptionCode(LPCTSTR ErrStr);//執行MES溝通-系統異常碼
	bool                       ExecMESComm_SetUserLogin_out(bool bLogin);//執行MES溝通-使用者登入
	bool                       ExecMESComm_SetControlStateMode(MES_EQP_CTRL_STATE_MODE Mode);//執行MES溝通-設定控制狀態模式	
	bool                       ExecMESComm_CheckBarcode(CAOIProject *ProjectPtr);//執行MES溝通-確認條碼
	bool                       ExecMESComm_AskBarcode(CAOIProject *ProjectPtr);//執行MES溝通-詢問條碼
	bool                       ExecMESComm_BoardMapping(CAOIProject *ProjectPtr);//執行MES溝通-報廢板映射
	bool                       ExecMESComm_UnCheckTestFile(LANE_ID LaneID, int &Count);//執行MES溝通-未判定檢測檔案數
	bool                       ExecMESComm_LoadProject();//執行MES溝通-開啟專案	
	bool                       SendToMESNode(const char *strBuf, bool bAck, int nPPID, DWORD AckTime);//送資料給MES
	//--------------------------------------------------------------------------//
	bool                       ClearRepairResultList();//清除維修站結果列表
	bool                       RemoveRepairResultNode(LANE_ID LaneID, const char *DateTime);//移除維修站結果資料
	bool                       AddRepairResultNode(const TRepairResultNode &Node);//新增維修站結果資料	
	size_t                     GetRepairResultNodeCount() const;//取得維修站結果資料點數	
	TRepairResultNode*         GetRepairResultNodePtr(size_t idx, bool bCheck);//取得維修站結果資料		
	bool                       GetRepairResultNodeByDateTime(const char *DateTime, TRepairResultNode &Node);//取得維修站結果資料	
	void                       CloneRepairResultNodeList(const std::vector<TRepairResultNode> &List);//複製維修站結果資料列表
	//--------------------------------------------------------------------------//		
	bool                       ExecCCSDataChange(LANE_ID LaneID, const char *DateTimeBuffer);//執行中控中心資料更換
	void                       ClearCCSDateTime(LANE_ID LaneID);//清除中控中心時間
	bool                       ShiftCCSDateTime(LANE_ID LaneID);//移動中控中心時間
	bool                       GetIsCCSDataTimeLockTest(LANE_ID LaneID);//取得中控中心時間鎖住檢測
	bool                       GetIsCCSDataTimeLockPCBInOut(LANE_ID LaneID);//取得中控中心時間鎖住進出板
	bool                       GetIsCCSDateTimeFinishEmpty(LANE_ID LaneID);//取得中控中心要檢測時間是否空白
	bool                       GetIsCCSDateTimeToCheckEmpty(LANE_ID LaneID);//取得中控中心要確認時間是否空白
	void                       SetCCSDataTimeLockTest(LANE_ID LaneID, bool bLock);//中控中心時間鎖住檢測
	void                       SetCCSDataTimeLockPCBInOut(LANE_ID LaneID, bool bLock);//中控中心時間鎖住進出板
	void                       SetCCSDateTimeFinish(LANE_ID LaneID, const char *DateTime);//中控中心時間檢測完
	void                       SetCCSDateTimeToCheck(LANE_ID LaneID, const char *DateTime);//中控中心時間要確認
	bool                       GetCCSDateTimeToCheck(LANE_ID LaneID, char *DateTime) const;//中控中心時間要確認
	//--------------------------------------------------------------------------//	
	void                       ClearCCSDateTime_LA();//清除中控中心時間-A軌
	bool                       ShiftCCSDateTime_LA();//移動中控中心時間-A軌	
	bool                       GetIsCCSDataTimeLockTest_LA();//取得中控中心時間鎖住檢測-A軌
	bool                       GetIsCCSDataTimeLockPCBInOut_LA();//取得中控中心時間鎖住進出板-A軌
	bool                       GetIsCCSDateTimeFinishEmpty_LA();//取得中控中心要檢測時間是否空白-A軌
	bool                       GetIsCCSDateTimeToCheckEmpty_LA();//取得中控中心要確認時間是否空白-A軌
	void                       SetCCSDataTimeLockTest_LA(bool bLock);//中控中心時間鎖住檢測-A軌
	void                       SetCCSDataTimeLockPCBInOut_LA(bool bLock);//中控中心時間鎖住進出板-A軌
	void                       SetCCSDateTimeFinish_LA(const char *DateTime);//中控中心時間檢測完-A軌
	void                       SetCCSDateTimeToCheck_LA(const char *DateTime);//中控中心時間要確認-A軌		
	bool                       GetCCSDateTimeToCheck_LA(char *DateTime) const;//中控中心時間要確認-A軌		
	//--------------------------------------------------------------------------//	
	void                       ClearCCSDateTime_LB();//清除中控中心時間-B軌
	bool                       ShiftCCSDateTime_LB();//移動中控中心時間-B軌	
	bool                       GetIsCCSDataTimeLockTest_LB();//取得中控中心時間鎖住檢測-B軌
	bool                       GetIsCCSDataTimeLockPCBInOut_LB();//取得中控中心時間鎖住進出板-B軌
	bool                       GetIsCCSDateTimeFinishEmpty_LB();//取得中控中心要檢測時間是否空白-B軌
	bool                       GetIsCCSDateTimeToCheckEmpty_LB();//取得中控中心要確認時間是否空白-B軌
	void                       SetCCSDataTimeLockTest_LB(bool bLock);//中控中心時間鎖住檢測-B軌
	void                       SetCCSDataTimeLockPCBInOut_LB(bool bLock);//中控中心時間鎖住進出板-B軌
	void                       SetCCSDateTimeFinish_LB(const char *DateTime);//設定中控中心時間檢測完-B軌
	void                       SetCCSDateTimeToCheck_LB(const char *DateTime);//設定中控中心時間要確認-B軌	
	bool                       GetCCSDateTimeToCheck_LB(char *DateTime) const;//中控中心時間要確認-B軌		
	//--------------------------------------------------------------------------//
	void                       ClearRepairDateTimeToCheck(LANE_ID LaneID);//清除維修站時間要確認
	void                       SetRepairDateTimeToCheck(LANE_ID LaneID, const char *DateTime);//設定維修站時間要確認
	bool                       GetRepairDateTimeToCheck(LANE_ID LaneID, char *DateTime) const;//維修站時間要確認
	bool                       GetIsRepairDateTimeToCheckEmpty(LANE_ID LaneID);//取得維修站時間要確認時間是否空白
	//--------------------------------------------------------------------------//
	//等待維修站判定	
	void                       ClearRepairDateTimeToCheck_LA();//清除維修站時間要確認-A軌	
	void                       SetRepairDateTimeToCheck_LA(const char *DateTime);//設定維修站時間要確認-A軌	
	bool                       GetRepairDateTimeToCheck_LA(char *DateTime) const;//維修站時間要確認-A軌		
	bool                       GetIsRepairDateTimeToCheckEmpty_LA();//取得維修站時間要確認時間是否空白-A軌
	void                       ClearRepairDateTimeToCheck_LB();//清除維修站時間要確認-B軌	
	void                       SetRepairDateTimeToCheck_LB(const char *DateTime);//設定維修站時間要確認-B軌	
	bool                       GetRepairDateTimeToCheck_LB(char *DateTime) const;//維修站時間要確認-B軌	
	bool                       GetIsRepairDateTimeToCheckEmpty_LB();//取得維修站時間要確認時間是否空白-B軌
	//--------------------------------------------------------------------------//
	bool                       WaitForPCBOKNGSignalOnDelayTime(LANE_ID LaneID);
	void                       SetPCBOKNGSignalOnTickCount(LANE_ID LaneID);//PCB OK/NG開啟時間
	void                       SetPCBOKNGSignalOnTickCount(LANE_ID LaneID, DWORD TickCount);//PCB OK/NG開啟時間
	DWORD                      GetPCBOKNGSignalOnTickCount(LANE_ID LaneID) const;//PCB OK/NG開啟時間
	void                       SetPCBOKNGSignalOnTickCount_LA();//PCB OK/NG開啟時間-A軌	
	void                       SetPCBOKNGSignalOnTickCount_LA(DWORD TickCount);//PCB OK/NG開啟時間-A軌	
	DWORD                      GetPCBOKNGSignalOnTickCount_LA() const;//PCB OK/NG開啟時間-A軌	
	void                       SetPCBOKNGSignalOnTickCount_LB();//PCB OK/NG開啟時間-B軌	
	void                       SetPCBOKNGSignalOnTickCount_LB(DWORD TickCount);//PCB OK/NG開啟時間-B軌	
	DWORD                      GetPCBOKNGSignalOnTickCount_LB() const;//PCB OK/NG開啟時間-B軌
	//---------------------------------------------------------------------------------//
	bool                       ExecSystemRun_OnlineRunLock();//系統運作-線上運作鎖住
	bool                       ExecSystemRun_AutoSwitchToOnlineView();//系統運作-自動切換線上畫面
	bool                       ExecSystemRun_AutoSwitchToOnlineRemoteCtrlMode();//系統運作-自動切換線上遠端控制模式
	//--------------------------------------------------------------------------//	
	ONLINE_STATE_MODE          GetNextOnlineStateMode(ONLINE_STATE_MODE OnlineState, LANE_ID LaneID, PCB_OUT_DIRECTION Direction, TEST_RESULT_ID TestResultID) const;	
	//---------------------------------------------------------------------------------//	
	bool                       ExecOnlineProcAutoStopByIdleTime();//線上檢測-自動停機-依據閒置時間
	bool                       ExecOnlineProcAutoStopBySpecTime();//線上檢測-自動停機-依據特定時間
	bool                       ExecOnlineProcAutoStopBySpecTimeFn(__int64 SpecTime);//線上檢測-自動停機-依據特定時間
	//--------------------------------------------------------------------------//
	CAOIProject*               GetOnlineProcParamProject(TOnlineProcParam &Param);	
	bool                       GetOnlineProcLaneRunBypassMode(TOnlineProcParam &Param) const;//取得該軌道是否流片模式
	bool                       CheckOnlineProcPreMoveCamera(TOnlineProcParam &Param) const;//線上檢測-確認是否可提前移動相機
	bool                       ExecOnlineProcLaneBypass(TOnlineProcParam &Param);//線上檢測-軌道直通
	bool                       ExecOnlineProcInspectionStop(TOnlineProcParam &Param);//線上檢測-檢測停止-首次檢測
	bool                       ExecOnlineProcPCBReady(TOnlineProcParam &Param);//線上檢測-PCB就緒
	bool                       ExecOnlineProcInputBarcode(TOnlineProcParam &Param);//線上檢測-輸入條碼
	bool                       ExecOnlineProcInspectProjectMark(TOnlineProcParam &Param);//線上檢測-檢測專案標誌
	bool                       ExecOnlineProcInspectProjectOpenCode(TOnlineProcParam &Param);//線上檢測-檢測專案開啟條碼
	bool                       ExecOnlineProcInspectionStart(TOnlineProcParam &Param);//線上檢測-檢測開始
	bool                       ExecOnlineProcInspectionWaiting(TOnlineProcParam &Param);//線上檢測-檢測等待中
	bool                       ExecOnlineProcInspectFdPanel(TOnlineProcParam &Param);//線上檢測-檢測定位點-整板
	bool                       ExecOnlineProcInspectFdBoard(TOnlineProcParam &Param);//線上檢測-檢測定位點-單板
	bool                       ExecOnlineProcInspectProject(TOnlineProcParam &Param);//線上檢測-檢測專案
	bool                       ExecOnlineProcStaticsProject(TOnlineProcParam &Param);//線上檢測-統計專案
	bool                       ExecOnlineProcInspectionFinish(TOnlineProcParam &Param);//線上檢測-檢測完畢
	bool                       ExecOnlineProcProjectSwitchProcMode(TOnlineProcParam &Param);//線上檢測-專案切換程序
	bool                       ExecOnlineProcProjectSwitchByTurn(TOnlineProcParam &Param);//線上檢測-專案切換-輪流
	bool                       ExecOnlineProcProjectSwitchByTurnOneCycleReset(TOnlineProcParam &Param);//線上檢測-專案切換-輪流
	bool                       ExecOnlineProcWaitForLastStation(TOnlineProcParam &Param);//線上檢測-等待上一站
	bool                       ExecOnlineProcWaitForNextStation(TOnlineProcParam &Param);//線上檢測-等待下一站
	bool                       ExecOnlineProcPCBInStart(TOnlineProcParam &Param);//線上檢測-進板開始
	bool                       ExecOnlineProcPCBInChecking(TOnlineProcParam &Param);//線上檢測-進板確認
	bool                       ExecOnlineProcPCBInFinish(TOnlineProcParam &Param);//線上檢測-進板完成
	bool                       ExecOnlineProcPCBOutStart(TOnlineProcParam &Param);//線上檢測-出板開始
	bool                       ExecOnlineProcPCBOutChecking(TOnlineProcParam &Param);//線上檢測-出板確認
	bool                       ExecOnlineProcPCBOutFinish(TOnlineProcParam &Param);//線上檢測-出板完成
	bool                       ExecOnlineProcPCBOutInsideStart(TOnlineProcParam &Param);//線上檢測-停側邊開始
	bool                       ExecOnlineProcPCBOutInsideChecking(TOnlineProcParam &Param);//線上檢測-停側邊確認
	bool                       ExecOnlineProcPCBOutInsideFinish(TOnlineProcParam &Param);//線上檢測-停側邊完成
	bool                       ExecOnlineProcPCBBackStart(TOnlineProcParam &Param);//線上檢測-退板開始
	bool                       ExecOnlineProcPCBBackChecking(TOnlineProcParam &Param);//線上檢測-退板確認
	bool                       ExecOnlineProcPCBBackFinish(TOnlineProcParam &Param);//線上檢測-退板完成
	bool                       ExecOnlineProcPCBBackOutStart(TOnlineProcParam &Param);//線上檢測-退出板開始
	bool                       ExecOnlineProcPCBBackOutChecking(TOnlineProcParam &Param);//線上檢測-退出板確認
	bool                       ExecOnlineProcPCBBackOutFinish(TOnlineProcParam &Param);//線上檢測-退出板完成
	bool                       ExecOnlineProcWaitForPCBRemoved(TOnlineProcParam &Param);//線上檢測-等待板子移除
	bool                       ExecOnlineProcWaitForRepairVerify(TOnlineProcParam &Param);//線上檢測-等待維修站確認	
	bool                       ExecOnlineProcPCBOutInStart(TOnlineProcParam &Param);//線上檢測-出板帶進板開始
	bool                       ExecOnlineProcPCBOutInChecking(TOnlineProcParam &Param);//線上檢測-出板帶進板確認
	bool                       ExecOnlineProcPCBOutInFinish(TOnlineProcParam &Param);//線上檢測-出板帶進板完成
	bool                       ExecOnlineProcPCBAutoRunStart(TOnlineProcParam &Param);//線上檢測-自動進出板開始
	bool                       ExecOnlineProcPCBAutoRunChecking(TOnlineProcParam &Param);//線上檢測-自動進出板確認	
	bool                       ExecOnlineProcPCBAutoRunFinish(TOnlineProcParam &Param);//線上檢測-自動進出板完成
	bool                       ExecOnlineProcPCBDualRunStart(TOnlineProcParam &Param);//線上檢測-雙軌進出板開始//20230425	
	bool                       ExecOnlineProcPCBDualRunChecking(TOnlineProcParam &Param);//線上檢測-雙軌進出板確認//20230425		
	bool                       ExecOnlineProcPCBDualRunFinish(TOnlineProcParam &Param);//線上檢測-雙軌進出板完成//20230425
	bool                       ExecOnlineProcPCBDualRunPauseOne(TOnlineProcParam &Param); //線上檢測-執行線上檢測暫停函式
	bool                       ExecOnlineProcPCBDualRunResume(TOnlineProcParam &Param); //線上檢測-執行線上檢測回復函式
	//--------------------------------------------------------------------------//	
	bool                       ExecOnlineInputProjectWorkNumber(ONLINE_INPUT_TIMING CurTiming);//執行線上輸入專案工單號碼
	//--------------------------------------------------------------------------//
	bool                       ExecOnlineProcAutoCalibrationFinish(TOnlineProcParam &Param);//線上檢測-結束

	bool                       ExecOnlineProcAutoCalibration_XYZ_Home(TOnlineProcParam &Param);//線上檢測-自動校正-XYZ歸零
	bool                       ExecOnlineProcAutoCalibration_XYZ_HomeFn(TOnlineProcParam &Param);//線上檢測-自動校正-XYZ歸零
	bool                       ExecOnlineProcAutoCalibration_XYZ_HomeMoveToTarget();//線上檢測-自動校正-XYZ歸零-移動至原點上	
	bool                       ExecOnlineProcAutoCalibration_XYZ_HomeCalibrate();//線上檢測-自動校正-XYZ歸零-執行歸零	

	bool                       ExecOnlineProcAutoCalibration_2D_Current(TOnlineProcParam &Param);//線上檢測-自動校正-2D電流
	bool                       ExecOnlineProcAutoCalibration_2D_CurrentFn(TOnlineProcParam &Param);//線上檢測-自動校正-2D電流
	bool                       ExecOnlineProcAutoCalibration_2D_CurrentMoveToTarget();//線上檢測-自動校正-2D電流-移動至塊規上	
	bool                       ExecOnlineProcAutoCalibration_2D_CurrentVerify();//線上檢測-自動校正-2D電流-驗證	
	bool                       ExecOnlineProcAutoCalibration_2D_CurrentCalibrate();//線上檢測-自動校正-2D電流-校正	
	bool                       ExecOnlineProcAutoCalibration_3D_Current(TOnlineProcParam &Param);//線上檢測-自動校正-3D電流
	bool                       ExecOnlineProcAutoCalibration_3D_CurrentMoveToTarget();//線上檢測-自動校正-3D電流-移動至塊規上

	bool                       ExecOnlineProcAutoCalibration_3D_VerifyFn();//線上檢測-自動校正-3D驗證
	bool                       ExecOnlineProcAutoCalibration_3D_VerifyMoveToTarget();//線上檢測-自動校正-3D驗證-移動至塊規上	
	bool                       ExecOnlineProcAutoCalibration_3D_VerifyGrabImage();//線上檢測-自動校正-3D驗證-取像	
	bool                       ExecOnlineProcAutoCalibration_3D_VerifyWaitForGrabDone();//線上檢測-自動校正-3D驗證-等待取像結束		
	bool                       ExecOnlineProcAutoCalibration_3D_VerifyPlaneCalculate(const TFrameParam &FrameParam, const TSliceParam &SliceParam);//線上檢測-自動校正-3D驗證

	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlane(TOnlineProcParam &Param);//線上檢測-自動校正-3D相平面
	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlaneFn(TOnlineProcParam &Param);//線上檢測-自動校正-3D相平面
	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlaneMoveToTarget();//線上檢測-自動校正-3D相平面-移動至塊規上	
	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlaneGrabImage();//線上檢測-自動校正-3D相平面-取像	
	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlaneWaitForGrabDone();//線上檢測-自動校正-3D相平面-等待取像結束
	bool                       ExecOnlineProcAutoCalibration_3D_ZeroPlaneCalculate(const TFrameParam &FrameParam, const TSliceParam &SliceParam);//線上檢測-自動校正-相平面
	bool                       ExecOnlineProcAutoCalibration_3D_HeightFactor(TOnlineProcParam &Param);//線上檢測-自動校正-3D高度比例
	//--------------------------------------------------------------------------//	
	bool                       ExecOnlineProcBarcodeOpenProjectCheckEnd(ONLINE_OPEN_PROJECT_MODE eOpenMode);//線上檢測-確認是否最後一站	
	bool                       ExecOnlineProcBarcodeOpenProject(const std::vector<std::string> &BarcodeList, ONLINE_OPEN_PROJECT_MODE eOpenMode);//線上檢測-條碼開專案
	bool                       ExecOnlineProcBarcodeOpenProject(const std::vector<std::string> &BarcodeList, ONLINE_OPEN_PROJECT_MODE eOpenMode, std::string &Barcode);//線上檢測-條碼開專案
	bool                       ExecOnlineProcPCBAutoRunCheckingOnInspecting();//線上檢測-自動進出板確認-檢測中
	//--------------------------------------------------------------------------//		
	bool                       ExecAutoSwitchWnd3DFrame(CAOIModel *ModelPtr, CAOIWnd *WndPtr);//自動切換3D畫面
	//--------------------------------------------------------------------------//	
	bool                       ExecDebayerImage(const char *fnName, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, IMAGE_SIZE &DstStep, IMAGE_SIZE &DstBit, unsigned char *&pDst);
	//--------------------------------------------------------------------------//
	bool                       OpenJetDongle();//開啟JET硬體鎖	
	bool                       CheckJetDongleValid();//確認JET硬體鎖授權
	bool                       ExecCheckJetDongleValid();//確認JET硬體鎖授權
	bool                       CheckJetDongleWarning();//確認JET硬體鎖警告
	bool                       CheckJetDongleFunction();//確認JET硬體鎖功能
	bool                       ExecCheckJetDongleFunction();//確認JET硬體鎖功能	
	//--------------------------------------------------------------------------//
	bool                       CheckOnlineFormViewID() const;//確認是否為在線檢測介面	
	bool                       CheckOnlineFormViewID(UINT ID) const;//確認是否為在線檢測介面	
	//--------------------------------------------------------------------------//		
	bool                       SaveSystemFilenameSyntax();  //儲存檔名語句
	bool                       LoadSystemFilenameSyntax();  //載入檔名語句
	bool                       SaveSystemFilenameSyntax(LPCTSTR Filename, LPCTSTR Section, CFilenameSyntax &Syntax, CString &ErrString);  //儲存檔名語句
	bool                       LoadSystemFilenameSyntax(LPCTSTR Filename, LPCTSTR Section, CFilenameSyntax &Syntax, CString &ErrString);  //載入檔名語句
	
	CFilenameSyntax&           GetFilenameSyntax_BoardMap();  //單板底圖檔名語句
	void                       SetFilenameSyntax_BoardMap(CFilenameSyntax &Syntax);//單板底圖檔名語句
	CFilenameSyntax&           GetFilenameSyntax_PanelMap();  //整板底圖檔名語句
	void                       SetFilenameSyntax_PanelMap(CFilenameSyntax &Syntax);//整板底圖檔名語句
	CFilenameSyntax&           GetFilenameSyntax_ProjectMap();  //專案底圖檔名語句
	void                       SetFilenameSyntax_ProjectMap(CFilenameSyntax &Syntax);//專案底圖檔名語句
	CFilenameSyntax&           GetFilenameSyntax_CustomerReport();  //客戶報告檔名語句
	void                       SetFilenameSyntax_CustomerReport(CFilenameSyntax &Syntax);//客戶報告檔名語句
	//--------------------------------------------------------------------------//		
	//M2M NPM	
	bool                       CheckNPM_FuncBypass();
	CString                    GetNPM_LogFolder() const;
	bool                       GetNPM_FuncBypass() const;
	void                       SetNPM_FuncBypass(bool val);
	bool                       CheckNPM_APC_Enable() const;	
	bool                       GetNPM_BarcodeEnable() const;
	bool                       GetNPM_APC_FF1_Enable() const;
	bool                       GetNPM_APC_FF2_Enable() const;
	bool                       GetNPM_APC_MFB_Enable() const;				
	LPCTSTR                    GetNPM_LaneName(LANE_ID LaneID) const;
	LPCTSTR                    GetNPM_SrcShareFolder(LANE_ID LaneID) const;		
	LPCTSTR                    GetNPM_DstShareFolder(LANE_ID LaneID) const;
	LPCTSTR                    GetNPM_PCBSerialName(LANE_ID LaneID) const;
	CString                    GetNPM_LocalPCBSerialName(LANE_ID LaneID) const;	
	LPCTSTR                    GetNPM_DateTime(LANE_ID LaneID) const;	
	void                       SetNPM_DateTime(LPCTSTR DateTime, LANE_ID LaneID);	
	void                       SetNPM_DateTime(const CTime &DateTime, LANE_ID LaneID);
	std::vector<CString>&      GetNPM_DeleteFilenameList(LANE_ID LaneID);
	bool                       RemoveNPM_DeleteFilenameList(LANE_ID LaneID);	
	bool                       AddNPM_DeleteFilename(LPCTSTR Filename, LANE_ID LaneID);			
	std::vector<CString>&      GetNPM_BackupFilenameList(LANE_ID LaneID);
	bool                       BackupNPM_BackupFilenameList(LANE_ID LaneID);	
	bool                       AddNPM_BackupFilename(LPCTSTR Filename, LANE_ID LaneID);	
	void                       SetNPM_PCBSerialName(LANE_ID LaneID, LPCTSTR Name);
	bool                       ExecNPM_CopyPCBSerialFile(LANE_ID LaneID);//執行複製NPM的PCBSerial檔案
	bool                       ExecNPM_LoadPCBSerialFile(LANE_ID LaneID);//執行載入NPM的PCBSerial檔案
	bool                       ExecNPM_LoadAPC_FF1_File(LANE_ID LaneID);//執行載入NPM的APC-FF1檔案
	bool                       ExecNPM_LoadAPC_FF2_File(LANE_ID LaneID);//執行載入NPM的APC-FF2檔案
	bool                       ExecNPM_LoadAPC_MFB_File(LANE_ID LaneID);//執行載入NPM的APC-MFB檔案
	bool                       ExecNPM_SaveAOILog1File(CAOIProject *ProjectPtr);//執行儲存NPM的AOI-訊息檔案-1
	bool                       ExecNPM_SaveAOILog2File(CAOIProject *ProjectPtr);//執行儲存NPM的AOI-訊息檔案-2
	bool                       ExecNPM_SaveAOIInspectionResultFile(CAOIProject *ProjectPtr);//執行儲存NPM的AOI檢測結果檔案
	//--------------------------------------------------------------------------//	
	//Alan-add Hanwha
	THREAD_COMMAND_MODE        GetHASI_MonitorThreadCmd();
	THREAD_STATE_MODE          GetHASI_MonitorThreadState();
	void                       SetHASI_MonitorThreadCmd(THREAD_COMMAND_MODE Cmd);
	void                       SetHASI_MonitorThreadState(THREAD_STATE_MODE State);
	void                       LockThreadHASI_Monitor();
	void                       UnlockThreadHASI_Monitor();

	bool                       CreateHASI_MoniterThread();
	bool                       DeleteHASI_MoniterThread();
	bool                       StartHAIS_MoniterThread(bool WaitOn);
	bool                       WaitForHAIS_MoniterThreadStart();
	bool                       ExecHASI_MoniterFn();

	bool                       GetHASI_Enable() const;
	LPCTSTR                    GetHASI_ShareFolder() const;
	bool                       GetHASI_SerialFileEnable() const;
	int                        GetHASI_SerialFileQueueSize() const;
	DWORD                      GetHASI_SerialFileDwellTime() const;
	bool                       GetHASI_SPIOffsetFileEnable() const;
	HASI_AOI_STAGE             GetHASI_AOIStage() const;
	bool                       GetHASI_PnPEnable()const;

	bool					   CreateHASI_ShareFolder();
	bool                       SetHASI_PCBSerialPorperty(LANE_ID LaneID, std::vector<CString> &Field, std::vector<CString> &Data);
	tHASIPCBSerial             GetHASI_PCBSerialPorperty(LANE_ID LaneID);
	bool                       ClearHASI_PCBSerialPorperty(LANE_ID LaneID);
	bool                       SetHASI_SPIFileStateMode(LANE_ID LaneID, HASI_STATE_MODE mode);
	HASI_STATE_MODE            GetHASI_SPIFileStateMode(LANE_ID LaneID);
	//bool					   GetHASI_ProjectMinCad(CAOIProject *ProjectPtr, TPOINT2D &MinCad);

	bool                       ExecHASI_LoadTableFile(FILE *pfile, CString &Section, std::vector<CString> &Field, std::vector<CString> &Data);
	bool                       ExecHASI_LoadTableFile(FILE *pfile, CString &Section, std::vector<CString> &Field, std::vector<std::vector<CString>> &Data);
	bool                       ExecHASI_SaveTableFile(FILE *pfile, CString Section, std::vector<CString> Field, std::vector<CString> Data);
	bool                       ExecHASI_SaveTableFile(FILE *pfile, CString Section, std::vector<CString> Field, HASI_FieldMap& Table);
	bool                       ExecHASI_LoadStateFile(bool &bIsok);//執行載入來自T-M2M的State檔案
	bool                       ExecHASI_LoadPCBSerialFile(LANE_ID LaneID);//執行載入Hanwha的PCBSerial檔案
	bool                       ExecHASI_LoadSPIOffsetFile(CAOIProject *ProjectPtr);//執行載入Hanwha的SPI Offset檔案
	bool                       ExecHASI_SaveStateFile();//執行儲存要送往T-M2M的State檔案
	bool                       ExecHASI_SaveAOIJobFile(LANE_ID LaneID, bool bDual = true);//執行儲存AOI檢測檔案
	bool                       ExecHASI_SaveAOIJobFile(CAOIProject * ProjectPtr);//執行儲存AOI檢測檔案
	bool                       ExecHASI_SaveAOIInspectionResultFile(CAOIProject *ProjectPtr);//執行儲存AOI檢測結果檔案
	bool                       ExecHASI_SaveAOIInspectionResultFile_VPnP(CAOIProject *ProjectPtr, bool bCurrent = true);//執行儲存AOI檢測結果檔案-PnP
	bool                       ExecHASI_SaveAOIInspectionImage_VPnP(CAOIProject *ProjectPtr);//執行儲存AOI檢測圖片-PnP
	//--------------------------------------------------------------------------//	
	bool                       LaunchExternalCopyFileApp(bool &bLaunch);//啟動外部複製檔案軟體
	bool                       CheckExternalCopyFileEnabed();//確認外部複製檔案啟用
	bool                       GetExternalCopyFileEnabed() const;//取得外部複製檔案啟用
	bool                       SaveExternalCopyFileList();//儲存外部複製檔案列表
	bool                       SaveExternalCopyFileListFn();//儲存外部複製檔案列表
	bool                       ClearExternalCopyFileList();//清除外部複製檔案列表
	bool                       AddExternalCopyFile(LPCTSTR Src, LPCTSTR Dst, bool bFile, bool bDelSrc);//加入外部複製檔案
	//--------------------------------------------------------------------------//	
	double                     CalcProjectFocusOffset(LANE_ID LaneID);//計算專案焦距偏移值
	//--------------------------------------------------------------------------//	
	bool                       GetCpkChartEnabled() const;//取得CPK圖表是否啟用
	//--------------------------------------------------------------------------//	
	bool					   CheckXboardMappingFileViaMESCommBoardCountIsCorrect(CAOIProject *ProjectPtr);
	//--------------------------------------------------------------------------//	
	bool                      SaveGrabListStats();
	bool                      SaveGrabRgnListStats(LPCTSTR filename, const std::vector<CAOIRgn*> &List);
	bool                      SaveGrabFovListStats(LPCTSTR filename, const std::vector<CAOIFov*> &List);
	bool                      SaveGrabSliceListStats(LPCTSTR filename, const std::vector<CAOISlice*> &List);
	bool                      SaveGrabFrameListStats(LPCTSTR filename, const std::vector<CAOIFrame*> &List);
	bool                      SaveGrabFieldListStats(LPCTSTR filename, const std::vector<CAOIField*> &List);
	//--------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
extern CAOIDataCollect AOIDataCollect;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIDATACOLLECT_H__4D0868BB_E44F_4045_89BB_60E9E43F4433__INCLUDED_)
