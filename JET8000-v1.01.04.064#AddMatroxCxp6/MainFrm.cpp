// MainFrm.cpp : implementation of the CMainFrame class
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "MainFrm.h"
#include "JETRibbonPanel.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"

#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "MessageBoxWnd.h"
#include "EditFormView.h"
#include "EditModelView.h"
#include "DebugFormView.h"
#include "OnlineFormView.h"
#include "OnlineFormView_Dual.h"
#include "EditMainView.h"
#include "EditFdView.h"
#include "EditMarkView.h"
#include "EditBarcodeView.h"
#include "ProjectListWnd.h"
#include "UserRegisterWnd.h"
#include "BarcodeDeviceWnd.h"
#include "ITSCommWnd.h"

#include "RulerWnd.h"
#include "PlcCtrlWnd.h"
#include "LoadBomWnd.h"
#include "CudaCtrlWnd.h"
#include "FdConfirmWnd.h"
#include "Light3DTiDLPWnd.h"
#include "CameraCtrlWnd.h"
#include "MotionCtrlWnd.h"
#include "ImageConfigWnd.h"
#include "CalibrationWnd.h"
#include "ProjectParamWnd.h"
#include "SystemConfigWnd.h"
#include "SystemInitialWnd.h"
#include "SystemConvertWnd.h"
#include "ImagePhaseAllWnd.h"
#include "LightCtrlBoardWnd.h"
#include "BarcodeConfirmWnd.h"
#include "NewProjectWizardWnd.h"
#include "ProjectColorWnd.h"
#include "FdListWnd.h"
#include "PanelListWnd.h"
#include "BoardListWnd.h"
#include "BarcodeListWnd.h"
#include "BarcodeInputWnd.h"
#include "ComponentListWnd.h"
#include "AlgImageCompareWnd.h"
#include "ProjectFieldConfigWnd.h"
#include "RemoteParamWnd.h"
#include "FdSortWnd.h"
#include "MachineStatusWnd.h"
#include "ProjectCodeListWnd.h"
#include "ProjectGroupConfigWnd.h"
#include "LogOperViewerWnd.h"
#include "ComponentDefectAlarmListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define LIST_ITEM_PATCH_ENABLE_COUNT   1000
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CMainFrame, CBasicFrame)
//-------------------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE == FRAME_STYLE_STUDIO
	const int  iMaxUserToolbars = 10;
	const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
	const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
#elif FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	UINT AFX_WM_ON_CHANGE_RIBBON_CATEGORY= ::RegisterWindowMessage(_T("AFX_WM_ON_CHANGE_RIBBON_CATEGORY")); 
	UINT AFX_WM_POSTRECALCLAYOUT= ::RegisterWindowMessage(_T("AFX_WM_POSTRECALCLAYOUT"));
	UINT AFX_WM_ON_RIBBON_CUSTOMIZE= ::RegisterWindowMessage(_T("AFX_WM_ON_RIBBON_CUSTOMIZE"));
	UINT AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM= ::RegisterWindowMessage(_T("AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM"));
	UINT AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU= ::RegisterWindowMessage(_T("AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU"));
#endif//FRAME_STYLE_TYPE
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMainFrame, CBasicFrame)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()	
	ON_COMMAND(ID_VIEW_PLC_CTRL_WND, OnViewPlcCtrlWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLC_CTRL_WND, OnUpdateViewPlcCtrlWnd)	
	ON_COMMAND(ID_VIEW_MOTION_CTRL_WND, OnViewMotionCtrlWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOTION_CTRL_WND, OnUpdateViewMotionCtrlWnd)	
	ON_COMMAND(ID_VIEW_EDIT_MODEL_VIEW, OnViewEditModelView)
	ON_COMMAND(ID_VIEW_PROJECT_PARAM_WND, OnViewProjectParamWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_PARAM_WND, OnUpdateViewProjectParamWnd)	
	ON_COMMAND(ID_VIEW_SYSTEM_CONFIG_WND, OnViewSystemConfigWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SYSTEM_CONFIG_WND, OnUpdateViewSystemConfigWnd)	
	ON_COMMAND(ID_VIEW_SYSTEM_INITIAL_WND, OnViewSystemInitialWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SYSTEM_INITIAL_WND, OnUpdateViewSystemInitialWnd)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CAMERA_CTRL_WND, OnViewCameraCtrlWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAMERA_CTRL_WND, OnUpdateViewCameraCtrlWnd)	
	ON_COMMAND(ID_VIEW_PHASE_CTRL_WND, OnViewPhaseCtrlWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PHASE_CTRL_WND, OnUpdateViewPhaseCtrlWnd)
	ON_COMMAND(ID_VIEW_CALIBRATION_WND, OnViewCalibrationWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CALIBRATION_WND, OnUpdateViewCalibrationWnd)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, OnUpdateApplicationLook)	
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_XYZ_HOME_ALL, OnXYZHomeAll)
	ON_UPDATE_COMMAND_UI(ID_XYZ_HOME_ALL, OnUpdateXYZHomeAll)
	ON_COMMAND(ID_XYZ_GO_TO_ORG, OnXYZGoToORG)
	ON_UPDATE_COMMAND_UI(ID_XYZ_GO_TO_ORG, OnUpdateXYZGoToORG)		
	ON_COMMAND(ID_XYZ_GO_TO_FOCUS, OnXYZGoToFocus)
	ON_UPDATE_COMMAND_UI(ID_XYZ_GO_TO_FOCUS, OnUpdateXYZGoToFocus)
	ON_COMMAND(ID_XYZ_GO_TO_LEAVE, OnXYZGoToLeave)
	ON_UPDATE_COMMAND_UI(ID_XYZ_GO_TO_LEAVE, OnUpdateXYZGoToLeave)
	ON_COMMAND(ID_XYZ_CTRL_GROUP, OnXYZCtrlGroup)
	ON_UPDATE_COMMAND_UI(ID_XYZ_CTRL_GROUP, OnUpdateXYZCtrlGroup)	
	ON_COMMAND(ID_LANE_PCB_IN, OnLanePCBIn)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_IN, OnUpdateLanePCBIn)	
	ON_COMMAND(ID_LANE_PCB_IN_2ND, OnLanePCBIn2nd)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_IN_2ND, OnUpdateLanePCBIn2nd)	
	ON_COMMAND(ID_LANE_PCB_IN_3RD, OnLanePCBIn3rd)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_IN_3RD, OnUpdateLanePCBIn3rd)	
	ON_COMMAND(ID_LANE_PCB_OUT, OnLanePCBOut)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_OUT, OnUpdateLanePCBOut)
	ON_COMMAND(ID_LANE_PCB_BACK, OnLanePCBBack)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_BACK, OnUpdateLanePCBBack)
	ON_COMMAND(ID_LANE_PCB_BACK_OUT, OnLanePCBBackOut)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_BACK_OUT, OnUpdateLanePCBBackOut)
	ON_COMMAND(ID_LANE_PCB_CLEAR, OnLanePCBClear)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_CLEAR, OnUpdateLanePCBClear)
	ON_COMMAND(ID_LANE_PCB_CLAMP_ON, OnLanePCBClampOn)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_CLAMP_ON, OnUpdateLanePCBClampOn)
	ON_COMMAND(ID_LANE_PCB_CLAMP_OFF, OnLanePCBClampOff)
	ON_UPDATE_COMMAND_UI(ID_LANE_PCB_CLAMP_OFF, OnUpdateLanePCBClampOff)
	ON_COMMAND(ID_LANE_CTRL_GROUP, OnLaneCtrlGroup)
	ON_UPDATE_COMMAND_UI(ID_LANE_CTRL_GROUP, OnUpdateLaneCtrlGroup)
	ON_COMMAND(ID_VIEW_CUDA_CTRL_WND, OnViewCudaCtrlWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CUDA_CTRL_WND, OnUpdateViewCudaCtrlWnd)
	ON_COMMAND(ID_VIEW_IMAGE_CONFIG_WND, OnViewImageConfigWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGE_CONFIG_WND, OnUpdateViewImageConfigWnd)	
	ON_COMMAND(ID_VIEW_LIGHT_CTRL_BOARD_WND, OnViewLightCtrlBoardWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIGHT_CTRL_BOARD_WND, OnUpdateViewLightCtrlBoardWnd)
	ON_COMMAND(ID_VIEW_RULER_WND, OnViewRulerWnd)
	ON_COMMAND(ID_VIEW_PROPERTIES_WND, OnViewPropertiesWnd)
	ON_COMMAND(ID_VIEW_EDITLIST_WND, OnViewEditlistWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITLIST_WND, OnUpdateViewEditlistWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES_WND, OnUpdateViewPropertiesWnd)
	ON_COMMAND(ID_VIEW_MODEL_LIST_WND, OnViewModelListWnd)
	ON_COMMAND(ID_VIEW_PROJECT_MAP_WND, OnViewProjectMapWnd)		
	ON_REGISTERED_MESSAGE(AFX_WM_ON_CHANGE_RIBBON_CATEGORY, OnRibbonCategoryChanged)
	ON_COMMAND(ID_VIEW_EDIT_MAIN_VIEW, OnViewEditMainView)	
	ON_COMMAND(ID_VIEW_WND_DOCKING_WND, OnViewWndDockingWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WND_DOCKING_WND, OnUpdateViewWndDockingWnd)
	ON_COMMAND(ID_VIEW_IMAGE_DOCKING_WND, OnViewImageDockingWnd)	
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW, OnUpdateProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_OPEN, OnUpdateProjectOpen)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	ON_COMMAND(ID_PROJECT_SAVE_AS, OnProjectSaveAs)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_AS, OnUpdateProjectSaveAs)
	ON_COMMAND(ID_PROJECT_NEW_OFFLINE, OnProjectNewOffline)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW_OFFLINE, OnUpdateProjectNewOffline)	
	ON_COMMAND(ID_PROJECT_NEW_PANEL, OnProjectNewPanel)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW_PANEL, OnUpdateProjectNewPanel)
	ON_COMMAND(ID_PROJECT_SAVE_SPC_FILE, OnProjectSaveSpcFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_SPC_FILE, OnUpdateProjectSaveSpcFile)
	ON_COMMAND(ID_PROJECT_SAVE_CADXY_FILE, OnProjectSaveCadXYFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_CADXY_FILE, OnUpdateProjectSaveCadXYFile)
	ON_COMMAND(ID_PROJECT_SAVE_TEST_COVERAGE_FILE, OnProjectSaveTestCoverageFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_TEST_COVERAGE_FILE, OnUpdateProjectSaveTestCoverageFile)
	ON_COMMAND(ID_PROJECT_SAVE_TEST_MAPPING_FILE, OnProjectSaveTestMappingFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_TEST_MAPPING_FILE, OnUpdateProjectSaveTestMappingFile)
	ON_COMMAND(ID_VIEW_RESULTLIST_WND, OnViewResultlistWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESULTLIST_WND, OnUpdateViewResultlistWnd)
	ON_COMMAND(ID_VIEW_EDIT_FD_VIEW, OnViewEditFdView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDIT_FD_VIEW, OnUpdateViewEditFdView)
	ON_COMMAND(ID_VIEW_EDIT_MARK_VIEW, OnViewEditMarkView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDIT_MARK_VIEW, OnUpdateViewEditMarkView)
	ON_COMMAND(ID_VIEW_EDIT_BARCODE_VIEW, OnViewEditBarcodeView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDIT_BARCODE_VIEW, OnUpdateViewEditBarcodeView)
	ON_COMMAND(ID_VIEW_ALL_PHASE_WND, OnViewAllPhaseWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALL_PHASE_WND, OnUpdateViewAllPhaseWnd)
	ON_COMMAND(ID_ONLINE_RUN, OnOnlineRun)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_RUN, OnUpdateOnlineRun)
	ON_COMMAND(ID_ONLINE_BYPASS, OnOnlineBypass)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_BYPASS, OnUpdateOnlineBypass)
	ON_COMMAND(ID_ONLINE_STOP, OnOnlineStop)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_STOP, OnUpdateOnlineStop)	
	ON_COMMAND(ID_SYSTEM_RESET, OnSystemReset)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_RESET, OnUpdateSystemReset)
	ON_COMMAND(ID_SETTING_OFFLINE_MODE, OnSettingOfflineMode)
	ON_UPDATE_COMMAND_UI(ID_SETTING_OFFLINE_MODE, OnUpdateSettingOfflineMode)
	ON_COMMAND(ID_SETTING_SAVE_FOV_IMAGES, OnSettingSaveFovImages)
	ON_UPDATE_COMMAND_UI(ID_SETTING_SAVE_FOV_IMAGES, OnUpdateSettingSaveFovImages)
	ON_COMMAND(ID_SETTING_OFFLINE_INSPECTION_MODE, OnSettingOfflineInspectionMode)
	ON_UPDATE_COMMAND_UI(ID_SETTING_OFFLINE_INSPECTION_MODE, OnUpdateSettingOfflineInspectionMode)
	ON_COMMAND(ID_REBUILD_INSPECTION_FIELD, OnRebuildInspectionField)
	ON_UPDATE_COMMAND_UI(ID_REBUILD_INSPECTION_FIELD, OnUpdateRebuildInspectionField)
	ON_COMMAND(ID_PROJECT_GROUP_CONFIG_WND, OnProjectGroupConfigWnd)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_GROUP_CONFIG_WND, OnUpdateProjectGroupConfigWnd)
	ON_COMMAND(ID_RESET_PROJECT_LIGHTING, OnResetProjectLighting)
	ON_UPDATE_COMMAND_UI(ID_RESET_PROJECT_LIGHTING, OnUpdateResetProjectLighting)
	ON_COMMAND(ID_LOAD_SYSTEM_PARAMETER, OnLoadSystemParameter)
	ON_UPDATE_COMMAND_UI(ID_LOAD_SYSTEM_PARAMETER, OnUpdateLoadSystemParameter)	
	ON_COMMAND(ID_SAVE_SYSTEM_PARAMETER, OnSaveSystemParameter)
	ON_UPDATE_COMMAND_UI(ID_SAVE_SYSTEM_PARAMETER, OnUpdateSaveSystemParameter)
	ON_COMMAND(ID_SAVE_RAW_IMAGE, OnSaveRawImage)
	ON_UPDATE_COMMAND_UI(ID_SAVE_RAW_IMAGE, OnUpdateSaveRawImage)
	ON_COMMAND(ID_CONVERT_SYSTEM_PARAMETER, OnConvertSystemParameter)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_SYSTEM_PARAMETER, OnConvertLoadSystemParameter)
	ON_COMMAND(ID_BUILD_MOTION_XY_CALI_TABLE, OnBuildMotionXYCaliTable)
	ON_UPDATE_COMMAND_UI(ID_BUILD_MOTION_XY_CALI_TABLE, OnUpdateBuildMotionXYCaliTable)	
	ON_COMMAND(ID_LIBRARY_MERGE, OnLibraryMerge)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_MERGE, OnUpdateLibraryMerge)
	ON_COMMAND(ID_LIBRARY_SAVE_AS, OnLibrarySaveAs)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_SAVE_AS, OnUpdateLibrarySaveAs)
	ON_COMMAND(ID_LIBRARY_CLEAR, OnLibraryClear)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_CLEAR, OnUpdateLibraryClear)
	ON_COMMAND(ID_LIBRARY_LOAD, OnLibraryLoad)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_LOAD, OnUpdateLibraryLoad)
	ON_COMMAND(ID_LIBRARY_REBUILD, OnLibraryReBuild)
	ON_UPDATE_COMMAND_UI(ID_LIBRARY_REBUILD, OnUpdateLibraryReBuild)
	ON_COMMAND(ID_SERVER_LIBRARY_MERGE, OnServerLibraryMerge)
	ON_UPDATE_COMMAND_UI(ID_SERVER_LIBRARY_MERGE, OnUpdateServerLibraryMerge)	
	ON_COMMAND(ID_SERVER_LIBRARY_LOAD, OnServerLibraryLoad)
	ON_UPDATE_COMMAND_UI(ID_SERVER_LIBRARY_LOAD, OnUpdateServerLibraryLoad)
	ON_COMMAND(ID_SERVER_LIBRARY_CLEAR, OnServerLibraryClear)
	ON_UPDATE_COMMAND_UI(ID_SERVER_LIBRARY_CLEAR, OnUpdateServerLibraryClear)
	ON_COMMAND(ID_SET_SAVE_DEFECT_IMAGE, OnSetSaveDefectImage)
	ON_UPDATE_COMMAND_UI(ID_SET_SAVE_DEFECT_IMAGE, OnUpdateSetSaveDefectImage)
	ON_COMMAND(ID_SET_ONLINE_TUNING_ENABLE, OnSetOnlineTuningEnable)
	ON_UPDATE_COMMAND_UI(ID_SET_ONLINE_TUNING_ENABLE, OnUpdateSetOnlineTuningEnable)
	ON_COMMAND(ID_ONLINE_TUNING_LAST_FILE, OnOnlineTuningLastFile)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_LAST_FILE, OnUpdateOnlineTuningLastFile)
	ON_COMMAND(ID_ONLINE_TUNING_NEXT_FILE, OnOnlineTuningNextFile)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_NEXT_FILE, OnUpdateOnlineTuningNextFile)
	ON_COMMAND(ID_ONLINE_TUNING_LIST_FILE, OnOnlineTuningListFile)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_LIST_FILE, OnUpdateOnlineTuningListFile)
	ON_COMMAND(ID_SET_MULTI_BOARD_CTRL_MODE, OnSetMultiBoardCtrlMode)
	ON_UPDATE_COMMAND_UI(ID_SET_MULTI_BOARD_CTRL_MODE, OnUpdateSetMultiBoardCtrlMode)
	ON_COMMAND(ID_LANE_ADJUST_HOME, OnLaneAdjustHome)
	ON_UPDATE_COMMAND_UI(ID_LANE_ADJUST_HOME, OnUpdateLaneAdjustHome)
	ON_COMMAND(ID_LANE_ADJUST_WIDTH, OnLaneAdjustWidth)
	ON_UPDATE_COMMAND_UI(ID_LANE_ADJUST_WIDTH, OnUpdateLaneAdjustWidth)
	ON_COMMAND(ID_LANE_ACTIVE_A, OnLaneActiveA)
	ON_UPDATE_COMMAND_UI(ID_LANE_ACTIVE_A, OnUpdateLaneActiveA)
	ON_COMMAND(ID_LANE_ACTIVE_B, OnLaneActiveB)
	ON_UPDATE_COMMAND_UI(ID_LANE_ACTIVE_B, OnUpdateLaneActiveB)
	ON_COMMAND(ID_PROJECT_OPEN_2ND, OnProjectOpen2nd)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_OPEN_2ND, OnUpdateProjectOpen2nd)
	ON_COMMAND(ID_PROJECT_SWITCH_TO_1, OnProjectSwitchTo1)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SWITCH_TO_1, OnUpdateProjectSwitchTo1)
	ON_COMMAND(ID_PROJECT_SWITCH_TO_2, OnProjectSwitchTo2)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SWITCH_TO_2, OnUpdateProjectSwitchTo2)
	ON_COMMAND(ID_PROJECT_SHOW_REPORT_TXT, OnProjectShowReportTxt)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SHOW_REPORT_TXT, OnUpdateProjectShowReportTxt)
	ON_COMMAND(ID_PROJECT_CLEAR_PROJECT_LIBRARY, OnProjectClearProjectLibrary)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLEAR_PROJECT_LIBRARY, OnUpdateClearProjectLibrary)	
	ON_COMMAND(ID_PROJECT_AUTO_LABEL_MODEL_GROUP, OnProjectAutoLabelModelGroup)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_AUTO_LABEL_MODEL_GROUP, OnUpdateProjectAutoLabelModelGroup)
	ON_COMMAND(ID_PROJECT_CLOSE_ALL, OnProjectCloseAll)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE_ALL, OnUpdateProjectCloseAll)	
	ON_COMMAND(ID_VIEW_USER_REGISTER_WND, OnViewUserRegisterWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_USER_REGISTER_WND, OnUpdateViewUserRegisterWnd)
	ON_COMMAND(ID_VIEW_BARCODE_DEVICE_WND, OnViewBarcodeDeviceWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BARCODE_DEVICE_WND, OnUpdateViewBarcodeDeviceWnd)
	ON_COMMAND(ID_PROJECT_OPEN_NEXT, OnProjectOpenNext)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_OPEN_NEXT, OnUpdateProjectOpenNext)
	ON_COMMAND(ID_PROJECT_CLOSE, OnProjectClose)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_PROJECT_LIST_COMBO, OnProjectListCombo)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_LIST_COMBO, OnUpdateProjectListCombo)	
	ON_COMMAND(ID_SET_PROJECT_COLOR_LIST_WND, OnSetProjectColorListWnd)
	ON_UPDATE_COMMAND_UI(ID_SET_PROJECT_COLOR_LIST_WND, OnUpdateSetProjectColorListWnd)
	ON_COMMAND(ID_VIEW_FD_LIST_WND, OnViewProjectFdListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FD_LIST_WND, OnUpdateViewProjectFdListWnd)		
	ON_COMMAND(ID_VIEW_PANEL_LIST_WND, OnViewProjectPanelListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_LIST_WND, OnUpdateViewProjectPanelListWnd)
	ON_COMMAND(ID_VIEW_BOARD_LIST_WND, OnViewProjectBoardListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOARD_LIST_WND, OnUpdateViewProjectBoardListWnd)
	ON_COMMAND(ID_VIEW_BARCODE_LIST_WND, OnViewProjectBarcodeListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BARCODE_LIST_WND, OnUpdateViewProjectBarcodeListWnd)
	ON_COMMAND(ID_VIEW_COMPONENT_LIST_WND, OnViewProjectComponentListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMPONENT_LIST_WND, OnUpdateViewProjectComponentListWnd)
	ON_COMMAND(ID_VIEW_COMPONENT_DEFECT_ALARM_LIST_WND, OnViewProjectComponentDefectAlarmListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMPONENT_DEFECT_ALARM_LIST_WND, OnUpdateViewProjectComponentDefectAlarmListWnd)
	ON_COMMAND(ID_PROJECT_LOAD_OFFLINE_FILE, OnProjectLoadOfflineFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_LOAD_OFFLINE_FILE, OnUpdateProjectLoadOfflineFile)
	ON_COMMAND(ID_PROJECT_LOAD_BOM_FILE, OnProjectLoadBOMFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_LOAD_BOM_FILE, OnUpdateProjectLoadBOMFile)
	ON_COMMAND(ID_PROJECT_LOAD_SPI_CAD, OnProjectLoadSPICad)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_LOAD_SPI_CAD, OnUpdateProjectLoadSPICad)
	ON_COMMAND(ID_VIEW_ONLINE_FORMVIEW, OnViewOnlineFormView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ONLINE_FORMVIEW, OnUpdateViewOnlineFormView)	
	ON_COMMAND(ID_VIEW_DROP_OUT_VERIFY_WND, OnViewDropOutVerifyWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DROP_OUT_VERIFY_WND, OnUpdateViewDropOutVerifyWnd)		
	ON_COMMAND(ID_VIEW_ITS_COMM_WND, OnViewITSCommWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITS_COMM_WND, OnUpdateViewITSCommWnd)		
	ON_COMMAND(ID_RESET_CONTROL_CENTER, OnResetControlCenter)
	ON_UPDATE_COMMAND_UI(ID_RESET_CONTROL_CENTER, OnUpdateResetControlCenter)		
	ON_COMMAND(ID_PROJECT_DISTRICT_ACTIVE_A, OnProjectDistrictActiveA)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_DISTRICT_ACTIVE_A, OnUpdateProjectDistrictActiveA)		
	ON_COMMAND(ID_PROJECT_DISTRICT_ACTIVE_B, OnProjectDistrictActiveB)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_DISTRICT_ACTIVE_B, OnUpdateProjectDistrictActiveB)
	ON_COMMAND(ID_VIEW_REMOTE_PARAM_WND, OnViewRemoteParamWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REMOTE_PARAM_WND, OnUpdateViewRemoteParamWnd)
	ON_COMMAND(ID_USER_SIGN_IN, OnUserSignIn)
	ON_UPDATE_COMMAND_UI(ID_USER_SIGN_IN, OnUpdateUserSignIn)
	ON_COMMAND(ID_USER_SIGN_OUT, OnUserSignOut)
	ON_UPDATE_COMMAND_UI(ID_USER_SIGN_OUT, OnUpdateUserSignOut)
	ON_COMMAND(ID_SET_FD_SORT_WND, OnSetFdSortWnd)
	ON_UPDATE_COMMAND_UI(ID_SET_FD_SORT_WND, OnUpdateSetFdSortWnd)
	ON_COMMAND(ID_SET_FD_SORT_WND_BOARD, OnSetFdSortWndBoard)
	ON_UPDATE_COMMAND_UI(ID_SET_FD_SORT_WND_BOARD, OnUpdateSetFdSortWndBoard)
	ON_COMMAND(ID_SET_PROJECT_MODE_NORMAL, OnSetProjectModeNormal)
	ON_UPDATE_COMMAND_UI(ID_SET_PROJECT_MODE_NORMAL, OnUpdateSetProjectModeNormal)
	ON_COMMAND(ID_SET_PROJECT_MODE_OPEN_BARCODE, OnSetProjectModeOpenBarcode)
	ON_UPDATE_COMMAND_UI(ID_SET_PROJECT_MODE_OPEN_BARCODE, OnUpdateSetProjectModeOpenBarcode)
	ON_COMMAND(ID_VIEW_PROJECT_CODE_LIST_WND, OnViewProjectOpenCodeListWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_CODE_LIST_WND, OnUpdateViewProjectOpenCodeListWnd)
	ON_COMMAND(ID_VIEW_OPERATOR_LOG_WND, OnViewOperatorLogWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPERATOR_LOG_WND, OnUpdateViewOperatorLogWnd)
	ON_COMMAND(ID_MES_SET_CTRL_STATE_OFFLINE, OnMesSetCtrlStateOffline)
	ON_UPDATE_COMMAND_UI(ID_MES_SET_CTRL_STATE_OFFLINE, OnUpdateMesSetCtrlStateOffline)		
	ON_COMMAND(ID_MES_SET_CTRL_STATE_LOCAL, OnMesSetCtrlStateLocal)
	ON_UPDATE_COMMAND_UI(ID_MES_SET_CTRL_STATE_LOCAL, OnUpdateMesSetCtrlStateLocal)
	ON_COMMAND(ID_MES_SET_CTRL_STATE_REMOTE, OnMesSetCtrlStateRemote)
	ON_UPDATE_COMMAND_UI(ID_MES_SET_CTRL_STATE_REMOTE, OnUpdateMesSetCtrlStateRemote)
	ON_COMMAND(ID_HOTKEY_ROTATE_OBJ, OnHotKeyRotateObj)
	ON_UPDATE_COMMAND_UI(ID_HOTKEY_ROTATE_OBJ, OnUpdateHotKeyRotateObj)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
//-------------------------------------------------------------------------------------//
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hUserAccel = NULL;
	m_UserAccelList = NULL;
	m_UserAccelCount = 0;

	this->m_pSplitterWnd = NULL;
	m_MainFrameWndLocked = false;
#if  FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

#ifdef OFFLINE_VERSION
	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_BLUE;
#else
	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_BLACK;
#endif//OFFLINE_VERSION

#endif//FRAME_STYLE_TYPE
	m_CategoryPtrLast = NULL;
	m_EditLibrarySize.cx = 400;
	m_EditLibrarySize.cy = 400;
	m_EditProjectMapSize.cx = 400;
	m_EditProjectMapSize.cy = 400;		
	m_RibbonPcbDirection = 0;
}
//-------------------------------------------------------------------------------------//
CMainFrame::~CMainFrame()
{
}
//-------------------------------------------------------------------------------------//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBasicFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	bool IsOK = true;
	//CMainFrame::ShowWindow(SW_HIDE);
	CMainFrame::ShowWindow(SW_SHOWMAXIMIZED);

#if FRAME_STYLE_TYPE == FRAME_STYLE_STUDIO
	IsOK = CMainFrame::CreateFrameWnd_Studio();
#elif FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	IsOK = CMainFrame::CreateFrameWnd_Office();
#else
	IsOK = CMainFrame::CreateFrameWnd_MFC();	
#endif//FRAME_STYLE_TYPE
	if ( false == IsOK )
	{	return -1; }
	
	CString SpecVersion;	
#ifndef OFFLINE_VERSION	
	#ifdef NO_3D_VERSION
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("NO_3D_VERSION"); }
	else { SpecVersion += CString(_T("\nNO_3D_VERSION")); }
	#endif//NO_3D_VERSION

	#ifdef LABORATORY_VERSION
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("LABORATORY_VERSION"); }
	else { SpecVersion += CString(_T("\nLABORATORY_VERSION")); }
	#endif//LABORATORY_VERSION
#endif//OFFLINE_VERSION

#ifdef TB_SYSTEM_ONLY_BOT
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("TB_SYSTEM_ONLY_BOT"); }
	else { SpecVersion += CString(_T("\nTB_SYSTEM_ONLY_BOT")); }
#endif//TB_SYSTEM_ONLY_BOT

#ifdef BYPASS_DLP1_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("BYPASS_DLP1_USE"); }
	else { SpecVersion += CString(_T("\nBYPASS_DLP1_USE")); }
#endif//BYPASS_DLP1_USE
#ifdef BYPASS_DLP2_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("BYPASS_DLP2_USE"); }
	else { SpecVersion += CString(_T("\nBYPASS_DLP2_USE")); }
#endif//BYPASS_DLP2_USE
#ifdef BYPASS_DLP3_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("BYPASS_DLP3_USE"); }
	else { SpecVersion += CString(_T("\nBYPASS_DLP3_USE")); }
#endif//BYPASS_DLP3_USE
#ifdef BYPASS_DLP4_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("BYPASS_DLP4_USE"); }
	else { SpecVersion += CString(_T("\nBYPASS_DLP4_USE")); }
#endif//BYPASS_DLP4_USE

#ifdef BURNING_TEST_FD_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("BURNING_TEST_FD_USE"); }
	else { SpecVersion += CString(_T("\nBURNING_TEST_FD_USE")); }
#endif//BURNING_TEST_FD_USE

#ifdef SAVE_LOG_MSG_SYNC_USE
	if ( SpecVersion.GetLength() == 0 ) { SpecVersion = _T("SAVE_LOG_MSG_SYNC_USE"); }
	else { SpecVersion += CString(_T("\nSAVE_LOG_MSG_SYNC_USE")); }
#endif//SAVE_LOG_MSG_SYNC_USE

	if ( SpecVersion.GetLength() != 0 )
	{	JetAPI::ShowMessageBox(SpecVersion); }

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_INITIAL_MAIN_FRAME, NULL);	
	return 0;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBasicFrame::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, (LPARAM)(this));
	return CBasicFrame::DestroyWindow();
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBasicFrame::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::Dump(CDumpContext& dc) const
{
	CBasicFrame::Dump(dc);
}
//-------------------------------------------------------------------------------------//
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//-------------------------------------------------------------------------------------//
void CMainFrame::OnDestroy() 
{	
	CBasicFrame::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyGlobalWnd();
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::GetMainFrameWndLocked() const//取得是否鎖住主視窗
{
	return m_MainFrameWndLocked;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::SetMainFrameWndLocked(bool val)//設定是否鎖住主視窗
{
	m_MainFrameWndLocked = val;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateUserAccelTable()//建立使用者快速建表單
{	
	DestroyUserAccelTable();
	//Set HotKey	
	UserHotKeyCtrl.BuildUserHotKeyList();
	UserHotKeyCtrl.LoadUserHotKeyList();
	UserHotKeyCtrl.SaveUserHotKeyList();
	
	const size_t UserHotKeyCount = UserHotKeyCtrl.GetUserHotKeyCount();	
	if ( 0 == UserHotKeyCount ) { return true; }

	std::vector<ACCEL> UserAccelList;
	for ( size_t i=0; i<UserHotKeyCount; i++ )
	{
		CUserHotKeyItem *HotKeyItemPtr=UserHotKeyCtrl.GetUserHotKeyPtr(i, false);
		if ( NULL == HotKeyItemPtr ) { continue; }
		ACCEL UserAccel;
		if ( HotKeyItemPtr->Convert2Accel(UserAccel) == false ) { continue; }
		UserAccelList.push_back(UserAccel);
	}
	const size_t UserAccelCount = UserAccelList.size();
	if ( 0 == UserAccelCount ) { return true; }

	m_UserAccelList = new ACCEL[UserAccelCount];
	if ( NULL == m_UserAccelList )
	{
		JetAPI::ShowMessageBox(_T("Error, User Accel List Is null"));
		return false;
	}

	for ( size_t i=0; i<UserAccelCount; i++ )
	{	m_UserAccelList[i] = UserAccelList[i];	}

	m_UserAccelCount = UserAccelCount;
	m_hUserAccel = CreateAcceleratorTable(m_UserAccelList, UserAccelCount);
	if ( NULL == m_hUserAccel )
	{
		m_UserAccelCount = 0;
		delete [] m_UserAccelList; m_UserAccelList=NULL;
		JetAPI::ShowMessageBox(_T("Error, CreateAcceleratorTable Fault"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::DestroyUserAccelTable()//刪除使用者快速建表單
{
	if ( NULL != m_hUserAccel )
	{	DestroyAcceleratorTable(m_hUserAccel);	}
	if ( NULL != m_UserAccelList )
	{
		delete[] m_UserAccelList;
		m_UserAccelList = NULL;
	}
	m_UserAccelCount = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecUserAccelTable(MSG* pMsg)//執行使用者快速建表單
{
	if ( NULL == m_hUserAccel ) { return false; }	
	if ( TranslateAccelerator(m_hWnd, m_hUserAccel, pMsg) )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateFrameWnd_MFC()//建立VC6模式的介面
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return false;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return false;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateFrameWnd_Studio()//建立VS10模式的介面
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_STUDIO
	m_strTitle = theApp.m_pszAppName;

	// 根據持續值設定視覺化管理員和樣式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("無法建立功能表列\n");
		return false;      // 無法建立
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	// 防止功能表列在啟動時取得焦點
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("無法建立工具列\n");
		return false;      // 無法建立
	}

	CString strToolBarName = _T("Standard");
	//bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);	
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize = _T("Customize");
	//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允許使用者定義的工具列作業:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("無法建立狀態列\n");
		return false;      // 無法建立
	}
	CString strTitlePane1 = _T("窗格1");
	CString strTitlePane2 = _T("窗格2");
	//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不希望工具列和功能表列為可停駐，請刪除這 5 行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// 啟用 Visual Studio 2005 樣式停駐視窗行為
	CDockingManager::SetDockingMode(DT_SMART);
	// 啟用 Visual Studio 2005 樣式停駐視窗自動隱藏行為
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 載入功能表項目影像 (不放在任何標準工具列上):
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 建立停駐視窗
	if (CreateDockingWnd()==false)
	{
		TRACE0("無法建立停駐視窗\n");
		return false;
	}

	// 啟用工具列和停駐視窗功能表取代
//	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 啟用快速 (Alt+拖曳) 工具列自訂
	CMFCToolBar::EnableQuickCustomization();

//	if (CMFCToolBar::GetUserImages() == NULL)
//	{
//		// 載入使用者定義的工具列影像
//		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
//		{
//			CMFCToolBar::SetUserImages(&m_UserImages);
//		}
//	}

	// 啟用功能表個人化 (最近使用的命令)
	// TODO: 定義您自己的基本命令，確定每個下拉式功能表都至少有一個基本命令。
//	CList<UINT, UINT> lstBasicCommands;
//	lstBasicCommands.AddTail(ID_FILE_NEW);
//	lstBasicCommands.AddTail(ID_FILE_OPEN);
//	lstBasicCommands.AddTail(ID_FILE_SAVE);
//	lstBasicCommands.AddTail(ID_FILE_PRINT);
//	lstBasicCommands.AddTail(ID_APP_EXIT);
//	lstBasicCommands.AddTail(ID_EDIT_CUT);
//	lstBasicCommands.AddTail(ID_EDIT_PASTE);
//	lstBasicCommands.AddTail(ID_EDIT_UNDO);
//	lstBasicCommands.AddTail(ID_APP_ABOUT);
//	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
//	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
//	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
//	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
//	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
//	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
//	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
//	CMFCToolBar::SetBasicCommands(lstBasicCommands);


#endif //FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateFrameWnd_Office()//建立VS10模式的介面
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	m_strTitle = theApp.m_pszAppName;

	// 根據持續值設定視覺化管理員和樣式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	
	// 允許使用者定義的工具列作業:	
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("無法建立狀態列\n");
		return false;      // 無法建立
	}
	CString strTitlePane1 = _T("窗格1");
	CString strTitlePane2 = _T("窗格2");
	//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不希望工具列和功能表列為可停駐，請刪除這 5 行	
	EnableDocking(CBRS_ALIGN_ANY);

	// 啟用 Visual Studio 2005 樣式停駐視窗行為
	CDockingManager::SetDockingMode(DT_SMART);
	// 啟用 Visual Studio 2005 樣式停駐視窗自動隱藏行為
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 將在左側建立巡覽窗格，所以會暫時停用於左側停駐:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// 建立停駐視窗
	if (CreateDockingWnd()==false )
	{
		TRACE0("無法建立停駐視窗\n");
		return false;
	}

	m_CategoryPtrLast = m_wndRibbonBar.GetActiveCategory();	
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	CMFCRibbonApplicationButton *pAppButton = m_wndRibbonBar.GetApplicationButton();//左上方的應用程式按鈕
	if ( NULL != pAppButton )
	{	pAppButton->SetImage(IDB_MAIN_APP);	}

#ifndef _DEBUG
	int nShowDebugForm = AOIDataCollect.GetSystemParameter().m_ShowDebugFormView;		
#ifdef ODM_BRAND_VERSION
	nShowDebugForm = FN_DISABLE;
#endif//ODM_BRAND_VERSION
	if ( FN_DISABLE == nShowDebugForm )
	{
		if ( CategoryCount > 0 ) 
		{	m_wndRibbonBar.ShowCategory(CategoryCount-1, FALSE); }
	}
#endif//_DEBUG
	SwitchMultiLanguageRibbonBar();

#ifdef OFFLINE_VERSION
	UINT nID = 0;	
	BOOL bVisible = FALSE;
	const bool bModify = false;

	if ( true == bModify )
	{
		//About Panel
		nID = ID_ONLINE_RUN;
		RemoveRibbonPanel(nID, true);
		nID = ID_XYZ_HOME_ALL;
		RemoveRibbonPanel(nID, true);
		nID = ID_LANE_PCB_IN;
		RemoveRibbonPanel(nID, true);

		//About Button
		nID = ID_VIEW_CAMERA_CTRL_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_MOTION_CTRL_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_PLC_CTRL_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_BARCODE_DEVICE_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_CALIBRATION_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_IMAGE_CONFIG_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_PHASE_CTRL_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_LIGHT_CTRL_BOARD_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_CUDA_CTRL_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_ITS_COMM_WND;
		RemoveRibbonElement(nID, true);	
		nID = ID_VIEW_REMOTE_PARAM_WND;
		RemoveRibbonElement(nID, true);			
		//m_wndRibbonBar.RecalcLayout();
		m_wndRibbonBar.ForceRecalcLayout();
	}
#endif//OFFLINE_VERSION


#endif //FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateDockingWnd()//建立駐列的視窗
{
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
	EnableLoadDockState(FALSE);

	DWORD dwStyle = 0;
	
	CSize   szComList;
	CString strComList;	
	//AFX_DEFAULT_DOCKING_PANE_STYLE = AFX_CBRS_FLOAT | AFX_CBRS_CLOSE | AFX_CBRS_RESIZE | AFX_CBRS_AUTOHIDE;

	szComList.cx = 224;
	szComList.cy = 200;
	strComList = _T("Part List");		
	strComList = LoadMultiLanguageString(strComList, strComList);
	
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditComponentList.Create(strComList, this, CRect(0, 0, szComList.cx, szComList.cy), TRUE, ID_VIEW_EDITLIST_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [元件列表] 視窗\n");
		return false; // 無法建立
	}
	szComList.cx /= 2;
	szComList.cy /= 2;
	m_wndEditComponentList.SetMinSize(szComList);
	m_wndEditComponentList.SetControlBarStyle(dwStyle);		
	m_wndEditComponentList.SetEditComponentListType(EDIT_COMPONENT_LIST_NORMAL);	

	CSize   szModelList;
	CString strModelList;
	szModelList.cx = 224;
	szModelList.cy = 200;
	strModelList = _T("Model List");		
	strModelList = LoadMultiLanguageString(strModelList, strModelList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditModelList.Create(strModelList, this, CRect(0, 0, szModelList.cx, szModelList.cy), TRUE, ID_VIEW_EDITLIST_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [模組列表] 視窗\n");
		return false; // 無法建立
	}
	szModelList.cx /= 2;
	szModelList.cy /= 2;
	m_wndEditModelList.SetMinSize(szModelList);
	m_wndEditModelList.SetControlBarStyle(dwStyle);		

	CSize   szPNList;
	CString strPNList;
	szPNList.cx = 224;
	szPNList.cy = 200;
	strPNList = _T("PN List");		
	strPNList = LoadMultiLanguageString(strPNList, strPNList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndPartNumberList.Create(strPNList, this, CRect(0, 0, szPNList.cx, szPNList.cy), TRUE, ID_VIEW_EDITLIST_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [料號列表] 視窗\n");
		return false; // 無法建立
	}
	szPNList.cx /= 2;
	szPNList.cy /= 2;
	m_wndPartNumberList.SetMinSize(szPNList);
	m_wndPartNumberList.SetControlBarStyle(dwStyle);		

	CSize   szMarkList;
	CString strMarkList;
	szMarkList.cx = 224;
	szMarkList.cy = 200;
	strMarkList = _T("Mark List");		
	strMarkList = LoadMultiLanguageString(strMarkList, strMarkList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditMarkList.Create(strMarkList, this, CRect(0, 0, szPNList.cx, szPNList.cy), TRUE, ID_VIEW_EDITLIST_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [特點列表] 視窗\n");
		return false; // 無法建立
	}
	szMarkList.cx /= 2;
	szMarkList.cy /= 2;
	m_wndEditMarkList.SetMinSize(szMarkList);
	m_wndEditMarkList.SetControlBarStyle(dwStyle);	
	m_wndEditMarkList.SetEditComponentListType(EDIT_COMPONENT_LIST_MARK);	

	//結果列表
	CSize   szResultList;
	CString strResultList;
	szResultList.cx = 224;
	szResultList.cy = 200;
	strResultList = _T("Result List");	
	strResultList = LoadMultiLanguageString(strResultList, strResultList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditResultList.Create(strResultList, this, CRect(0, 0, szResultList.cx, szResultList.cy), TRUE, ID_VIEW_RESULTLIST_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [結果列表] 視窗\n");
		return false; // 無法建立
	}
	szResultList.cx /= 2;
	szResultList.cy /= 2;
	m_wndEditResultList.SetMinSize(szResultList);
	m_wndEditResultList.SetControlBarStyle(dwStyle);

	
	CSize   szWndList;
	CString strWndList;
	szWndList.cx = 224;
	szWndList.cy = 200;
	strWndList = _T("Wnd Param");	
	strWndList = LoadMultiLanguageString(strWndList, strWndList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditWnd.Create(strWndList, this, CRect(0, 0, szWndList.cx, szWndList.cy), TRUE, ID_VIEW_WND_DOCKING_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [視窗列表] 視窗\n");
		return false; // 無法建立
	}
	szWndList.cx /= 2;
	szWndList.cy /= 2;
	m_wndEditWnd.SetMinSize(szWndList);
	m_wndEditWnd.SetControlBarStyle(dwStyle);

	CSize   szImageList;
	CString strImageList;
	szImageList.cx = 460;
	szImageList.cy = 400;
	strImageList = _T("Image Process");	
	strImageList = LoadMultiLanguageString(strImageList, strImageList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	if (!m_wndEditImage.Create(strImageList, this, CRect(0, 0, szImageList.cx, szImageList.cy), TRUE, ID_VIEW_IMAGE_DOCKING_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [影像顯示] 視窗\n");
		return false; // 無法建立
	}
	szImageList.cx /= 2;
	szImageList.cy /= 2;
	m_wndEditImage.SetMinSize(szImageList);
	m_wndEditImage.SetControlBarStyle(dwStyle);
	
	//CSize   szPropertiesWnd;
	//CString strPropertiesWnd;
	//szPropertiesWnd.cx = 200;
	//szPropertiesWnd.cy = 200;
	//strPropertiesWnd = _T("Property");
	//dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, szPropertiesWnd.cx, szPropertiesWnd.cy), TRUE, ID_VIEW_PROPERTIES_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("無法建立 [屬性] 視窗\n");
	//	return false; // 無法建立
	//}
	//m_wndProperties.SetMinSize(szPropertiesWnd);
	//m_wndProperties.SetControlBarStyle(dwStyle);
	
	CSize   szLibraryList;
	CString strLibraryList;
	szLibraryList.cx = 920;
	szLibraryList.cy = 400;		
	strLibraryList = _T("Library");//WS_VISIBLE	
	strLibraryList = LoadMultiLanguageString(strLibraryList, strLibraryList);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE);
	m_EditLibrarySize = szLibraryList;
	if (!m_wndEditLibrary.Create(strLibraryList, this, CRect(0, 0, szLibraryList.cx, szLibraryList.cy), TRUE, ID_VIEW_MODEL_LIST_WND, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [資料庫列表] 視窗\n");
		return false; // 無法建立
	}	
	szLibraryList.cx /= 2;
	szLibraryList.cy /= 2;
	m_wndEditLibrary.SetMinSize(szLibraryList);
	//m_wndEditLibrary.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);
	//m_wndEditLibrary.ShowPane(FALSE, FALSE, FALSE);	
	m_wndEditLibrary.SetControlBarStyle(dwStyle);
	
	CSize   szProjectMap;
	CString strProjectMap;
	szProjectMap.cx = 920;
	szProjectMap.cy = 768;	
	strProjectMap = _T("Project Map");//WS_VISIBLE	
	strProjectMap = LoadMultiLanguageString(strProjectMap, strProjectMap);
	dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE);
	m_EditProjectMapSize = szProjectMap;
	if (!m_wndEditProjectMap.Create(strProjectMap, this, CRect(0, 0, szProjectMap.cx, szProjectMap.cy), TRUE, ID_VIEW_PROJECT_MAP_WND, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("無法建立 [專案底圖] 視窗\n");
		return false; // 無法建立
	}	
	szProjectMap.cx /= 2;
	szProjectMap.cy /= 2;
	m_wndEditProjectMap.SetMinSize(szProjectMap);
	//m_wndEditProjectMap.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);
	//m_wndEditProjectMap.ShowPane(FALSE, FALSE, FALSE);	
	m_wndEditProjectMap.SetControlBarStyle(dwStyle);
	
	CDockablePane* pTabbedBar = NULL;	
	m_wndEditComponentList.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY		
	m_wndEditModelList.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY		
	m_wndPartNumberList.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY		
	m_wndEditResultList.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY	
	m_wndEditMarkList.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY	
	m_wndEditWnd.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY
	m_wndEditImage.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY	
	//m_wndProperties.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY	
	m_wndEditLibrary.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY
	m_wndEditProjectMap.EnableDocking(CBRS_ALIGN_RIGHT);//CBRS_ALIGN_ANY	
	//DockPane(&m_wndEditComponentList);	
	//DockPane(&m_wndEditResultList);
	//DockPane(&m_wndEditWnd);	
	//DockPane(&m_wndEditImage);	
	//DockPane(&m_wndProperties);
	//DockPane(&m_wndEditLibrary);
	//DockPane(&m_wndEditProjectMap);

	//要照此順序才會在右邊//20170103	
	DockPane(&m_wndEditProjectMap);//Kai-20170112
	DockPane(&m_wndEditLibrary);//Kai-20170112	
	
	DockPane(&m_wndEditComponentList);		
	//DockPane(&m_wndEditResultList);	
	m_wndEditModelList.AttachToTabWnd(&m_wndEditComponentList, DM_SHOW, TRUE, &pTabbedBar);
	m_wndPartNumberList.AttachToTabWnd(&m_wndEditModelList, DM_SHOW, TRUE, &pTabbedBar);
	m_wndEditMarkList.AttachToTabWnd(&m_wndPartNumberList, DM_SHOW, TRUE, &pTabbedBar);
	m_wndEditResultList.AttachToTabWnd(&m_wndEditMarkList, DM_SHOW, TRUE, &pTabbedBar);	
	if ( NULL!=pTabbedBar && pTabbedBar->GetSafeHwnd()!=NULL )//20190304
	{
		dwStyle = AFX_DEFAULT_DOCKING_PANE_STYLE & ~(AFX_CBRS_CLOSE|AFX_CBRS_AUTOHIDE);
		pTabbedBar->SetControlBarStyle(dwStyle);
	}

	DockPane(&m_wndEditWnd);	
	DockPane(&m_wndEditImage);	
	//DockPane(&m_wndProperties);	

	//Kai-20170112	
	if ( m_wndEditComponentList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditComponentList, FALSE, FALSE, FALSE); }	
	if ( m_wndEditModelList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditModelList, FALSE, FALSE, FALSE); }	
	if ( m_wndPartNumberList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndPartNumberList, FALSE, FALSE, FALSE); }	
	if ( m_wndEditMarkList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditMarkList, FALSE, FALSE, FALSE); }
	if ( m_wndEditResultList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditResultList, FALSE, FALSE, FALSE); }
	if ( m_wndEditWnd.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditWnd, FALSE, FALSE, FALSE); }
	if ( m_wndEditImage.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditImage, FALSE, FALSE, FALSE); }
	if ( m_wndProperties.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndProperties, FALSE, FALSE, FALSE); }
	
	//m_wndEditLibrary.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);	
	//m_wndEditProjectMap.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);
	if ( m_wndEditLibrary.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditLibrary, FALSE, FALSE, FALSE); }
	if ( m_wndEditProjectMap.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditProjectMap, FALSE, FALSE, FALSE); }		
	//m_wndEditLibrary.ShowPane(FALSE, FALSE, FALSE);
	//m_wndEditProjectMap.ShowPane(FALSE, FALSE, FALSE);
	
	
	//要先SHOW過一次才會正常
	//this->SwitchPane(TRUE);
	//先隱藏
	//this->SwitchPane(FALSE);	
	//CDockingManager* pDockManager = GetDockingManager();
	//ASSERT_VALID(pDockManager);
	//pDockManager->HideAutoHidePanes();
	//pDockManager->RecalcLayout();	

	CDockingManager* pDockManager = GetDockingManager();
	//if ( NULL != pDockManager )
	//{	pDockManager->DisableRestoreDockState(); }
	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::PreShowPane(CBasePane* pBar, BOOL bShow, BOOL bDelay, BOOL bActivate, bool &Done)
{
	if ( NULL == pBar ) { return false; }
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	if ( pBar->GetSafeHwnd() == NULL )
	{	Done = true;	}
	else
	{
		if ( pBar->IsWindowVisible() == bShow )
		{
			Done = true;
			ShowPane(pBar, bShow, bDelay, bActivate); 
		}
	}
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MAIN_FRAME");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MAIN_FRAME;
	WndKey = _T("IDD_MAIN_FRAME");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CMainFrame::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MAIN_FRAME");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::RemoveRibbonPanel(UINT nID, bool AllCategories)//設定-Ribbon Bar按鈕顯示
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE	
	int  i=0, j=0;
	int  PanelCount=0;
	CString strCategory;
	BOOL bVisibleOnly = FALSE;	
	const BOOL bDelete = FALSE;
	CMFCRibbonPanel  *pPanel = NULL;
	CMFCRibbonPanel  *pPanel2 = NULL;
	CMFCRibbonCategory *pCategory = NULL;
	CMFCRibbonBaseElement *pElement = NULL;

	if ( false == AllCategories )
	{
		pElement = m_wndRibbonBar.FindByID(nID, bVisibleOnly);	
		if ( NULL == pElement ) { return false; }
		pPanel = pElement->GetParentPanel();
		if ( NULL == pPanel ) { return false; }
		pCategory = pPanel->GetParentCategory();
		if ( NULL == pCategory ) { return false; }
		PanelCount = pCategory->GetPanelCount();
		if ( PanelCount > 0 )
		{
			for ( j=PanelCount-1; j>=0; j-- )
			{
				pPanel2 = pCategory->GetPanel(j);
				if ( NULL == pPanel2 ) { continue; }
				if ( pPanel2 != pPanel ) { continue; }
				pCategory->RemovePanel(j, bDelete);
				break;
			}
		}
	}
	else
	{	
		const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
		for ( i=1; i<CategoryCount; i++ )
		{
			pCategory = m_wndRibbonBar.GetCategory(i);
			if ( NULL == pCategory ) { continue; }
			strCategory = pCategory->GetName();
			pElement = pCategory->FindByID(nID, bVisibleOnly);
			if ( NULL == pElement ) { continue; }			
			pPanel = pElement->GetParentPanel();
			if ( NULL == pPanel ) { continue; }
			
			PanelCount = pCategory->GetPanelCount();
			if ( PanelCount > 0 )
			{
				for ( j=PanelCount-1; j>=0; j-- )
				{
					pPanel2 = pCategory->GetPanel(j);
					if ( NULL == pPanel2 ) { continue; }
					if ( pPanel2 != pPanel ) { continue; }
					pCategory->RemovePanel(j, bDelete);
					break;
				}
			}
		}
	}
	
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::RemoveRibbonElement(UINT nID, bool AllCategories)//設定-Ribbon Bar按鈕顯示
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE	
	int  i=0, j=0;
	int  ItemCount=0;
	CString strCategory;
	BOOL bVisibleOnly = FALSE;	
	const BOOL bDelete = FALSE;
	CMFCRibbonPanel  *pPanel = NULL;
	CMFCRibbonCategory *pCategory = NULL;
	CMFCRibbonBaseElement *pElement = NULL;
	CMFCRibbonBaseElement *pElement2 = NULL;
	if ( false == AllCategories )
	{
		pElement = m_wndRibbonBar.FindByID(nID, bVisibleOnly);	
		if ( NULL == pElement ) { return false; }
		pPanel = pElement->GetParentPanel();
		if ( NULL == pPanel ) { return false; }		
		ItemCount = pPanel->GetCount();
		if ( ItemCount > 0 )
		{
			for ( j=ItemCount-1; j>=0; j--)
			{
				pElement2 = pPanel->GetElement(j);
				if ( NULL == pElement2 ) { continue; }
				if ( pElement2 != pElement ) { continue; }
				pPanel->Remove(j, bDelete);
			}
		}
	}
	else
	{	
		const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
		for ( i=1; i<CategoryCount; i++ )
		{
			pCategory = m_wndRibbonBar.GetCategory(i);
			if ( NULL == pCategory ) { continue; }
			strCategory = pCategory->GetName();
			pElement = pCategory->FindByID(nID, bVisibleOnly);
			if ( NULL == pElement ) { continue; }
			
			pPanel = pElement->GetParentPanel();
			if ( NULL == pPanel ) { continue; }		
			ItemCount = pPanel->GetCount();
			if ( ItemCount > 0 )
			{
				for ( j=ItemCount-1; j>=0; j--)
				{
					pElement2 = pPanel->GetElement(j);
					if ( NULL == pElement2 ) { continue; }
					if ( pElement2 != pElement ) { continue; }
					pPanel->Remove(j, bDelete);
				}
			}
		}
	}
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateRibbonElementText(UINT nID, LPCTSTR sText, bool AllCategories)//設定-Ribbon Bar
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE	
	BOOL bVisibleOnly = FALSE;
	CMFCRibbonBaseElement *pElement = NULL;
	pElement = m_wndRibbonBar.FindByID(nID, bVisibleOnly);	
	if ( NULL == pElement ) { return false; }
			
	CString NewLabelText;
	CString NewToolTipText;	
	CString NewDescription;	
	CString LabelText = pElement->GetText();
	CString ToolTipText = pElement->GetToolTipText();
	CString Description = pElement->GetDescription();
	
	AOIDataCollect.GetUILanguageString(_T("IDR_RIBBON"), sText, LabelText, NewLabelText);	
	pElement->SetText(NewLabelText);

	AOIDataCollect.GetUILanguageString(_T("IDR_RIBBON_TOOLTIP"), sText, ToolTipText, NewToolTipText);
	if ( NewToolTipText.GetLength() > 0 )
	{	pElement->SetToolTipText(NewToolTipText);	}

	AOIDataCollect.GetUILanguageString(_T("IDR_RIBBON_DESCRIPTION"), sText, Description, NewDescription);
	if ( NewDescription.GetLength() > 0 )
	{	pElement->SetDescription(NewDescription);	}

	if ( true == AllCategories )
	{
		int     i=0;
		CString strCategory;
		CMFCRibbonCategory *pCategory = NULL;
		const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
		for ( i=1; i<CategoryCount; i++ )
		{
			pCategory = m_wndRibbonBar.GetCategory(i);
			if ( NULL == pCategory ) { continue; }
		#ifdef _DEBUG
			strCategory = pCategory->GetName();
		#endif//_DEBUG
			pElement = pCategory->FindByID(nID, bVisibleOnly);
			if ( NULL == pElement ) { continue; }
			pElement->SetText(NewLabelText);

			if ( NewToolTipText.GetLength() > 0 )
			{	pElement->SetToolTipText(NewToolTipText);	}

			if ( NewDescription.GetLength() > 0 )
			{	pElement->SetDescription(NewDescription);	}
		}
	}
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateRibbonButtonImageIndex(UINT nID, int Index, bool LargeImage, bool AllCategories)//設定-Ribbon Bar按鈕圖示編號
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE	
	BOOL bVisibleOnly = FALSE;
	CMFCRibbonBaseElement *pElement = NULL;
	pElement = m_wndRibbonBar.FindByID(nID, bVisibleOnly);	
	if ( NULL == pElement ) { return false; }	
	if ( pElement->IsKindOf(RUNTIME_CLASS(CMFCRibbonButton)) == FALSE ) 
	{	return true; }
	CMFCRibbonButton *pButton = (CMFCRibbonButton*)(pElement);

	int  LargeImageIndex = pButton->GetImageIndex(TRUE);
	int  SmallImageIndex = pButton->GetImageIndex(FALSE);
	if ( true==LargeImage && LargeImageIndex != -1 )
	{	pButton->SetImageIndex(Index, LargeImage);	}
	if ( false==LargeImage && SmallImageIndex != -1 )
	{	pButton->SetImageIndex(Index, LargeImage);	}
			
	if ( true == AllCategories )
	{
		int     i=0;
		CString strCategory;
		CMFCRibbonCategory *pCategory = NULL;
		const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
		for ( i=1; i<CategoryCount; i++ )
		{
			pCategory = m_wndRibbonBar.GetCategory(i);
			if ( NULL == pCategory ) { continue; }
		#ifdef _DEBUG
			strCategory = pCategory->GetName();
		#endif//_DEBUG
			pElement = pCategory->FindByID(nID, bVisibleOnly);
			if ( NULL == pElement ) { continue; }
			if ( pElement->IsKindOf(RUNTIME_CLASS(CMFCRibbonButton)) == FALSE ) { continue; }
			pButton = (CMFCRibbonButton*)(pElement);
			LargeImageIndex = pButton->GetImageIndex(TRUE);
			SmallImageIndex = pButton->GetImageIndex(FALSE);
			if ( true==LargeImage && LargeImageIndex != -1 )
			{	pButton->SetImageIndex(Index, LargeImage);	}
			if ( false==LargeImage && SmallImageIndex != -1 )
			{	pButton->SetImageIndex(Index, LargeImage);	}
		}
	}
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::SwitchMultiLanguageRibbonBar()//切換多國語系-Ribbon Bar
{
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	int     i=0, j=0, k=0;	
	int     PanelCount=0;
	int     ElementCount=0;
	UINT    CommandID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CMFCRibbonCategory *pCategory = NULL;
	CMFCRibbonPanel    *pMFCPanel = NULL;
	CMFCRibbonBaseElement *pElement = NULL;
	CString strCategory, strPanel, strElement;
	CString LabelText, NewLabelText;
	CString Section=_T("IDR_RIBBON");	
	const BOOL bVisibleOnly = TRUE;
	//---------------------------------------------------------------------------------//
	//WndID = IDR_RIBBON;
	WndKey = _T("IDR_RIBBON");
	//this->GetWindowText(LabelText);
	//AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	//this->SetWindowText(NewLabelText);

	//CMFCRibbonBar              m_wndRibbonBar;//帶狀工具列	
	m_wndRibbonBar.SetRedraw(FALSE);
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	for ( i=1; i<CategoryCount; i++ )
	{
		pCategory = m_wndRibbonBar.GetCategory(i);
		if ( NULL == pCategory ) { continue; }
		strCategory = pCategory->GetName();
		WndKey.Format(_T("%s[%s]"), _T("CATEGORY"), strCategory);
		LabelText = strCategory;
		AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
		pCategory->SetName(NewLabelText);

		PanelCount = pCategory->GetPanelCount();
		for ( j=0; j<PanelCount; j++ )
		{
			pMFCPanel = pCategory->GetPanel(j);
			if ( NULL == pMFCPanel ) { continue; }
			strPanel = pMFCPanel->GetName();
			WndKey.Format(_T("%s[%s]_%s[%s]"), _T("CATEGORY"), strCategory, _T("PANEL"), strPanel);
			LabelText = strPanel;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			//CMFCRibbonButton &MFCBtn = pMFCPanel->GetDefaultButton();
			//pMFCPanel->SetName(NewLabelText);			
			//MFCBtn.SetText(NewLabelText);

			CJETRibbonPanel *pJETPanel = STATIC_DOWNCAST(CJETRibbonPanel, pMFCPanel);
			if ( NULL != pJETPanel )
			{	pJETPanel->SetName(NewLabelText);	}
		}
	}

	//File
	UpdateRibbonElementText(ID_PROJECT_NEW, _T("ID_PROJECT_NEW"), true);	
	UpdateRibbonElementText(ID_PROJECT_OPEN, _T("ID_PROJECT_OPEN"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE, _T("ID_PROJECT_SAVE"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE_AS, _T("ID_PROJECT_SAVE_AS"), true);	
	UpdateRibbonElementText(ID_PROJECT_NEW_OFFLINE, _T("ID_PROJECT_NEW_OFFLINE"), true);	
	UpdateRibbonElementText(ID_PROJECT_LOAD_BOM_FILE, _T("ID_PROJECT_LOAD_BOM_FILE"), true);
	UpdateRibbonElementText(ID_PROJECT_LOAD_SPI_CAD, _T("ID_PROJECT_LOAD_SPI_CAD"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE_SPC_FILE, _T("ID_PROJECT_SAVE_SPC_FILE"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE_CADXY_FILE, _T("ID_PROJECT_SAVE_CADXY_FILE"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE_TEST_COVERAGE_FILE, _T("ID_PROJECT_SAVE_TEST_COVERAGE_FILE"), true);
	UpdateRibbonElementText(ID_PROJECT_SAVE_TEST_MAPPING_FILE, _T("ID_PROJECT_SAVE_TEST_MAPPING_FILE"), true);
	UpdateRibbonElementText(ID_PROJECT_CLOSE, _T("ID_PROJECT_CLOSE"), true);
	UpdateRibbonElementText(ID_PROJECT_CLOSE_ALL, _T("ID_PROJECT_CLOSE_ALL"), true);
	
	UpdateRibbonElementText(ID_PROJECT_OPEN_2ND, _T("ID_PROJECT_OPEN_2ND"), true);
	UpdateRibbonElementText(ID_PROJECT_OPEN_NEXT, _T("ID_PROJECT_OPEN_NEXT"), true);
	UpdateRibbonElementText(ID_PROJECT_LIST_COMBO, _T("ID_PROJECT_LIST_COMBO"), true);
	UpdateRibbonElementText(ID_PROJECT_SWITCH_TO_1, _T("ID_PROJECT_SWITCH_TO_1"), true);
	UpdateRibbonElementText(ID_PROJECT_SWITCH_TO_2, _T("ID_PROJECT_SWITCH_TO_2"), true);
	UpdateRibbonElementText(ID_PROJECT_SHOW_REPORT_TXT, _T("ID_PROJECT_SHOW_REPORT_TXT"), true);
	UpdateRibbonElementText(ID_PROJECT_CLEAR_PROJECT_LIBRARY, _T("ID_PROJECT_CLEAR_PROJECT_LIBRARY"), true);		
	UpdateRibbonElementText(ID_PROJECT_AUTO_LABEL_MODEL_GROUP, _T("ID_PROJECT_AUTO_LABEL_MODEL_GROUP"), true);

	UpdateRibbonElementText(ID_PROJECT_DISTRICT_ACTIVE_A, _T("ID_PROJECT_DISTRICT_ACTIVE_A"), true);
	UpdateRibbonElementText(ID_PROJECT_DISTRICT_ACTIVE_B, _T("ID_PROJECT_DISTRICT_ACTIVE_B"), true);
	
	UpdateRibbonElementText(ID_SET_PROJECT_MODE_NORMAL, _T("ID_SET_PROJECT_MODE_NORMAL"), true);	
	UpdateRibbonElementText(ID_SET_PROJECT_MODE_OPEN_BARCODE, _T("ID_SET_PROJECT_MODE_OPEN_BARCODE"), true);	

	//Library
	UpdateRibbonElementText(ID_LIBRARY_MERGE, _T("ID_LIBRARY_MERGE"), true);	
	UpdateRibbonElementText(ID_LIBRARY_LOAD, _T("ID_LIBRARY_LOAD"), true);
	UpdateRibbonElementText(ID_LIBRARY_SAVE_AS, _T("ID_LIBRARY_SAVE_AS"), true);
	UpdateRibbonElementText(ID_LIBRARY_CLEAR, _T("ID_LIBRARY_CLEAR"), true);
	UpdateRibbonElementText(ID_LIBRARY_REBUILD, _T("ID_LIBRARY_REBUILD"), true);

	//Server Library
	UpdateRibbonElementText(ID_SERVER_LIBRARY_LOAD, _T("ID_SERVER_LIBRARY_LOAD"), true);

	//Online
	UpdateRibbonElementText(ID_ONLINE_RUN, _T("ID_ONLINE_RUN"), true);
	UpdateRibbonElementText(ID_ONLINE_BYPASS, _T("ID_ONLINE_BYPASS"), true);
	UpdateRibbonElementText(ID_ONLINE_STOP, _T("ID_ONLINE_STOP"), true);

	//XYZ
	UpdateRibbonElementText(ID_XYZ_CTRL_GROUP, _T("ID_XYZ_CTRL_GROUP"), true);
	UpdateRibbonElementText(ID_XYZ_HOME_ALL, _T("ID_XYZ_HOME_ALL"), true);
	UpdateRibbonElementText(ID_XYZ_GO_TO_ORG, _T("ID_XYZ_GO_TO_ORG"), true);
	UpdateRibbonElementText(ID_XYZ_GO_TO_FOCUS, _T("ID_XYZ_GO_TO_FOCUS"), true);
	UpdateRibbonElementText(ID_XYZ_GO_TO_LEAVE, _T("ID_XYZ_GO_TO_LEAVE"), true);

	//Lane
	UpdateRibbonElementText(ID_LANE_CTRL_GROUP, _T("ID_LANE_CTRL_GROUP"), true);
	UpdateRibbonElementText(ID_LANE_PCB_IN, _T("ID_LANE_PCB_IN"), true);
	UpdateRibbonElementText(ID_LANE_PCB_OUT, _T("ID_LANE_PCB_OUT"), true);	
	UpdateRibbonElementText(ID_LANE_PCB_BACK, _T("ID_LANE_PCB_BACK"), true);
	UpdateRibbonElementText(ID_LANE_PCB_BACK_OUT, _T("ID_LANE_PCB_BACK_OUT"), true);
	UpdateRibbonElementText(ID_LANE_PCB_CLAMP_ON, _T("ID_LANE_PCB_CLAMP_ON"), true);
	UpdateRibbonElementText(ID_LANE_PCB_CLAMP_OFF, _T("ID_LANE_PCB_CLAMP_OFF"), true);
	UpdateRibbonElementText(ID_LANE_PCB_CLEAR, _T("ID_LANE_PCB_CLEAR"), true);
	UpdateRibbonElementText(ID_LANE_PCB_IN_2ND, _T("ID_LANE_PCB_IN_2ND"), true);
	UpdateRibbonElementText(ID_LANE_PCB_IN_3RD, _T("ID_LANE_PCB_IN_3RD"), true);
	UpdateRibbonElementText(ID_LANE_ADJUST_HOME, _T("ID_LANE_ADJUST_HOME"), true);
	UpdateRibbonElementText(ID_LANE_ADJUST_WIDTH, _T("ID_LANE_ADJUST_WIDTH"), true);
	UpdateRibbonElementText(ID_LANE_ACTIVE_A, _T("ID_LANE_ACTIVE_A"), true);
	UpdateRibbonElementText(ID_LANE_ACTIVE_B, _T("ID_LANE_ACTIVE_B"), true);
	
	//Parameters
	UpdateRibbonElementText(ID_VIEW_PROJECT_PARAM_WND, _T("ID_VIEW_PROJECT_PARAM_WND"), true);
	UpdateRibbonElementText(ID_VIEW_FD_LIST_WND, _T("ID_VIEW_FD_LIST_WND"), true);	
	UpdateRibbonElementText(ID_VIEW_PANEL_LIST_WND, _T("ID_VIEW_PANEL_LIST_WND"), true);
	UpdateRibbonElementText(ID_VIEW_BOARD_LIST_WND, _T("ID_VIEW_BOARD_LIST_WND"), true);
	UpdateRibbonElementText(ID_VIEW_BARCODE_LIST_WND, _T("ID_VIEW_BARCODE_LIST_WND"), true);
	UpdateRibbonElementText(ID_VIEW_COMPONENT_LIST_WND, _T("ID_VIEW_COMPONENT_LIST_WND"), true);	
	UpdateRibbonElementText(ID_VIEW_COMPONENT_DEFECT_ALARM_LIST_WND, _T("ID_VIEW_COMPONENT_DEFECT_ALARM_LIST_WND"), true);	

	UpdateRibbonElementText(ID_PROJECT_LOAD_OFFLINE_FILE, _T("ID_PROJECT_LOAD_OFFLINE_FILE"), true);
	UpdateRibbonElementText(ID_SET_PROJECT_COLOR_LIST_WND, _T("ID_SET_PROJECT_COLOR_LIST_WND"), true);
	UpdateRibbonElementText(ID_VIEW_SYSTEM_CONFIG_WND, _T("ID_VIEW_SYSTEM_CONFIG_WND"), true);
	UpdateRibbonElementText(ID_SAVE_SYSTEM_PARAMETER, _T("ID_SAVE_SYSTEM_PARAMETER"), true);
	UpdateRibbonElementText(ID_LOAD_SYSTEM_PARAMETER, _T("ID_LOAD_SYSTEM_PARAMETER"), true);
	UpdateRibbonElementText(ID_SAVE_RAW_IMAGE, _T("ID_SAVE_RAW_IMAGE"), true);
	UpdateRibbonElementText(ID_CONVERT_SYSTEM_PARAMETER, _T("ID_CONVERT_SYSTEM_PARAMETER"), true);
	UpdateRibbonElementText(ID_BUILD_MOTION_XY_CALI_TABLE, _T("ID_BUILD_MOTION_XY_CALI_TABLE"), true);

	//System
	UpdateRibbonElementText(ID_VIEW_SYSTEM_INITIAL_WND, _T("ID_VIEW_SYSTEM_INITIAL_WND"), true);	
	UpdateRibbonElementText(ID_VIEW_CAMERA_CTRL_WND, _T("ID_VIEW_CAMERA_CTRL_WND"), true);
	UpdateRibbonElementText(ID_VIEW_MOTION_CTRL_WND, _T("ID_VIEW_MOTION_CTRL_WND"), true);
	UpdateRibbonElementText(ID_VIEW_PLC_CTRL_WND, _T("ID_VIEW_PLC_CTRL_WND"), true);
	UpdateRibbonElementText(ID_VIEW_CALIBRATION_WND, _T("ID_VIEW_CALIBRATION_WND"), true);
	UpdateRibbonElementText(ID_VIEW_IMAGE_CONFIG_WND, _T("ID_VIEW_IMAGE_CONFIG_WND"), true);
	UpdateRibbonElementText(ID_VIEW_CUDA_CTRL_WND, _T("ID_VIEW_CUDA_CTRL_WND"), true);	
	UpdateRibbonElementText(ID_VIEW_ITS_COMM_WND, _T("ID_VIEW_ITS_COMM_WND"), true);		
	UpdateRibbonElementText(ID_VIEW_REMOTE_PARAM_WND, _T("ID_VIEW_REMOTE_PARAM_WND"), true);
	UpdateRibbonElementText(ID_VIEW_PHASE_CTRL_WND, _T("ID_VIEW_PHASE_CTRL_WND"), true);
	UpdateRibbonElementText(ID_VIEW_LIGHT_CTRL_BOARD_WND, _T("ID_VIEW_LIGHT_CTRL_BOARD_WND"), true);
	UpdateRibbonElementText(ID_VIEW_USER_REGISTER_WND, _T("ID_VIEW_USER_REGISTER_WND"), true);
	UpdateRibbonElementText(ID_VIEW_BARCODE_DEVICE_WND, _T("ID_VIEW_BARCODE_DEVICE_WND"), true);
	UpdateRibbonElementText(ID_VIEW_ALL_PHASE_WND, _T("ID_VIEW_ALL_PHASE_WND"), true);
	UpdateRibbonElementText(ID_SYSTEM_RESET, _T("ID_SYSTEM_RESET"), true);
	UpdateRibbonElementText(ID_RESET_CONTROL_CENTER, _T("ID_RESET_CONTROL_CENTER"), true);
	UpdateRibbonElementText(ID_VIEW_DROP_OUT_VERIFY_WND, _T("ID_VIEW_DROP_OUT_VERIFY_WND"), true);
	UpdateRibbonElementText(ID_VIEW_PROJECT_CODE_LIST_WND, _T("ID_VIEW_PROJECT_CODE_LIST_WND"), true);
	UpdateRibbonElementText(ID_VIEW_OPERATOR_LOG_WND, _T("ID_VIEW_OPERATOR_LOG_WND"), true);	

	//User
	UpdateRibbonElementText(ID_USER_SIGN_IN, _T("ID_USER_SIGN_IN"), true);
	UpdateRibbonElementText(ID_USER_SIGN_OUT, _T("ID_USER_SIGN_OUT"), true);

	//MES
	UpdateRibbonElementText(ID_MES_SET_CTRL_STATE_OFFLINE, _T("ID_MES_SET_CTRL_STATE_OFFLINE"), true);
	UpdateRibbonElementText(ID_MES_SET_CTRL_STATE_LOCAL, _T("ID_MES_SET_CTRL_STATE_LOCAL"), true);
	UpdateRibbonElementText(ID_MES_SET_CTRL_STATE_REMOTE, _T("ID_MES_SET_CTRL_STATE_REMOTE"), true);

	//Setting	
	UpdateRibbonElementText(ID_SET_FD_SORT_WND, _T("ID_SET_FD_SORT_WND"), true);
	UpdateRibbonElementText(ID_SET_FD_SORT_WND_BOARD, _T("ID_SET_FD_SORT_WND_BOARD"), true);
	UpdateRibbonElementText(ID_SETTING_OFFLINE_MODE, _T("ID_SETTING_OFFLINE_MODE"), true);
	UpdateRibbonElementText(ID_SET_SAVE_DEFECT_IMAGE, _T("ID_SET_SAVE_DEFECT_IMAGE"), true);
	UpdateRibbonElementText(ID_SET_ONLINE_TUNING_ENABLE, _T("ID_SET_ONLINE_TUNING_ENABLE"), true);	
	UpdateRibbonElementText(ID_SET_MULTI_BOARD_CTRL_MODE, _T("ID_SET_MULTI_BOARD_CTRL_MODE"), true);	
	UpdateRibbonElementText(ID_RESET_PROJECT_LIGHTING, _T("ID_RESET_PROJECT_LIGHTING"), true);		
	UpdateRibbonElementText(ID_REBUILD_INSPECTION_FIELD, _T("ID_REBUILD_INSPECTION_FIELD"), true);
	UpdateRibbonElementText(ID_SETTING_OFFLINE_INSPECTION_MODE, _T("ID_SETTING_OFFLINE_INSPECTION_MODE"), true);	

	//Online Tuning
	UpdateRibbonElementText(ID_ONLINE_TUNING_LAST_FILE, _T("ID_ONLINE_TUNING_LAST_FILE"), true);
	UpdateRibbonElementText(ID_ONLINE_TUNING_NEXT_FILE, _T("ID_ONLINE_TUNING_NEXT_FILE"), true);
	UpdateRibbonElementText(ID_ONLINE_TUNING_LIST_FILE, _T("ID_ONLINE_TUNING_LIST_FILE"), true);
	UpdateRibbonElementText(ID_ONLINE_TUNING_SMART_CHART, _T("ID_ONLINE_TUNING_SMART_CHART"), true);
	UpdateRibbonElementText(ID_ONLINE_TUNING_DATA_STATIC, _T("ID_ONLINE_TUNING_DATA_STATIC"), true);
	UpdateRibbonElementText(ID_ONLINE_TUNING_SCAN_STATIC, _T("ID_ONLINE_TUNING_SCAN_STATIC"), true);

	//Tools
	UpdateRibbonElementText(ID_VIEW_RULER_WND, _T("ID_VIEW_RULER_WND"), true);
	
	//Main Edit
	UpdateRibbonElementText(MENU_MAIN_EDIT_MODE_COMPONENT, _T("MENU_MAIN_EDIT_MODE_COMPONENT"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MODE_BOARD, _T("MENU_MAIN_EDIT_MODE_BOARD"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MODE_PANEL, _T("MENU_MAIN_EDIT_MODE_PANEL"), false);

	UpdateRibbonElementText(MENU_MAIN_EDIT_MOVE, _T("MENU_MAIN_EDIT_MOVE"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MIRROR_POS_X, _T("MENU_MAIN_EDIT_MIRROR_POS_X"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MIRROR_POS_Y, _T("MENU_MAIN_EDIT_MIRROR_POS_Y"), false);
	//旋轉
	UpdateRibbonElementText(MENU_MAIN_EDIT_ROTATE_090, _T("MENU_MAIN_EDIT_ROTATE_090"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_ROTATE_180, _T("MENU_MAIN_EDIT_ROTATE_180"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_ROTATE_270, _T("MENU_MAIN_EDIT_ROTATE_270"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_ROTATE_OTHERS, _T("MENU_MAIN_EDIT_ROTATE_OTHERS"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_ROTATE_REVERSE, _T("MENU_MAIN_EDIT_ROTATE_REVERSE"), false);

	UpdateRibbonElementText(MENU_MAIN_EDIT_SELECT_INVERT_BOARD, _T("MENU_MAIN_EDIT_SELECT_INVERT_BOARD"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_SELECT_INVERT_PANEL, _T("MENU_MAIN_EDIT_SELECT_INVERT_PANEL"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_SELECT_INVERT_PROJECT, _T("MENU_MAIN_EDIT_SELECT_INVERT_PROJECT"), false);
	//設定	
	UpdateRibbonElementText(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_INDEX, _T("MENU_MAIN_EDIT_SET_BARCODE_DEVICE_INDEX"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_SET_BARCODE_DEVICE_CODE_INDEX, _T("MENU_MAIN_EDIT_SET_BARCODE_DEVICE_CODE_INDEX"), false);
	//刪除
	UpdateRibbonElementText(MENU_MAIN_EDIT_DELETE_SELECTED, _T("MENU_MAIN_EDIT_DELETE_SELECTED"), false);
	//操作-manipulate
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_SELECT, _T("MENU_MAIN_EDIT_MANI_SELECT"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_CLONE, _T("MENU_MAIN_EDIT_MANI_CLONE"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_PASTE, _T("MENU_MAIN_EDIT_MANI_PASTE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_ADD, _T("MENU_MAIN_EDIT_MANI_ADD"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_PASTE_ARRAY, _T("MENU_MAIN_EDIT_MANI_PASTE_ARRAY"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_BOARDS, _T("MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_BOARDS"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_PANELS, _T("MENU_MAIN_EDIT_MANI_PASTE_TO_OTHER_PANELS"), false);		
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_PROJECT_MARK, _T("MENU_MAIN_EDIT_MANI_PROJECT_MARK"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CREATE, _T("MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CREATE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CLEAR, _T("MENU_MAIN_EDIT_FULL_MAP_COMPONENT_CLEAR"), false);	

	//顯示
	UpdateRibbonElementText(MENU_MAIN_EDIT_VIEW_1X1, _T("MENU_MAIN_EDIT_VIEW_1X1"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_VIEW_ALL, _T("MENU_MAIN_EDIT_VIEW_ALL"), false);	
	//Others
	UpdateRibbonElementText(MENU_MAIN_EDIT_ALIGN_FIDUCIAL, _T("MENU_MAIN_EDIT_ALIGN_FIDUCIAL"), true);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_CAPTURE_PROJECT_MAP, _T("MENU_MAIN_EDIT_CAPTURE_PROJECT_MAP"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_SAVE_COMPONENT_SAMPLE, _T("MENU_MAIN_EDIT_SAVE_COMPONENT_SAMPLE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_ADD_COMPONENT_SAMPLE, _T("MENU_MAIN_EDIT_ADD_COMPONENT_SAMPLE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_REPLACE_COMPONENT_SAMPLE, _T("MENU_MAIN_EDIT_REPLACE_COMPONENT_SAMPLE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_COPY_COMPONENT_SAMPLE, _T("MENU_MAIN_EDIT_COPY_COMPONENT_SAMPLE"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_PROJECT_MAP_MASK, _T("MENU_MAIN_EDIT_PROJECT_MAP_MASK"), false);		
	UpdateRibbonElementText(MENU_MAIN_EDIT_PROJECT_MAP_MASK_REGION, _T("MENU_MAIN_EDIT_PROJECT_MAP_MASK_REGION"), false);
	UpdateRibbonElementText(MENU_MAIN_EDIT_PROJECT_COMPARE, _T("MENU_MAIN_EDIT_PROJECT_COMPARE"), false);		
	UpdateRibbonElementText(MENU_MAIN_EDIT_COMPONENT_COMPARE, _T("MENU_MAIN_EDIT_COMPONENT_COMPARE"), false);		
	UpdateRibbonElementText(MENU_MAIN_EDIT_VIEW_PROJECT_MAP, _T("MENU_MAIN_EDIT_VIEW_PROJECT_MAP"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_DIVIDE_DISTRICT_WND, _T("MENU_MAIN_EDIT_DIVIDE_DISTRICT_WND"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_SET_BOARD_ORDER, _T("MENU_MAIN_EDIT_SET_BOARD_ORDER"), false);	
	UpdateRibbonElementText(MENU_MAIN_EDIT_MANI_AUTO_ADD, _T("MENU_MAIN_EDIT_MANI_AUTO_ADD"), false);
	UpdateRibbonElementText(ID_PROJECT_GROUP_CONFIG_WND, _T("ID_PROJECT_GROUP_CONFIG_WND"), true);	

	//Model View
	//調機-Tune
	UpdateRibbonElementText(ID_TUNE_ALIGN_FIDUCIAL, _T("ID_TUNE_ALIGN_FIDUCIAL"), true);	
	UpdateRibbonElementText(ID_TUNE_INSPECTION, _T("ID_TUNE_INSPECTION"), false);
	UpdateRibbonElementText(ID_TUNE_REPEATE_INSPECTION, _T("ID_TUNE_REPEATE_INSPECTION"), false);

	UpdateRibbonTuneGroupText();
	UpdateRibbonElementText(ID_TUNE_SELECTED_MODEL, _T("ID_TUNE_SELECTED_MODEL"), true);		
	UpdateRibbonElementText(ID_TUNE_SELECTED_PART_NUMBER, _T("ID_TUNE_SELECTED_PART_NUMBER"), true);
	UpdateRibbonElementText(ID_TUNE_SELECTED_MODEL_GROUP, _T("ID_TUNE_SELECTED_MODEL_GROUP"), true);	
	UpdateRibbonElementText(ID_TUNE_SELECTED_COMPONENT, _T("ID_TUNE_SELECTED_COMPONENT"), true);	
	UpdateRibbonElementText(MENU_MODEL_INSPECT_COMPONENT, _T("MENU_MODEL_INSPECT_COMPONENT"), true);	
	//模式
	UpdateRibbonElementText(ID_EDIT_MODEL_ADD, _T("ID_EDIT_MODEL_ADD"), false);		
	UpdateRibbonElementText(ID_EDIT_MODEL_SELECT, _T("ID_EDIT_MODEL_SELECT"), false);	
	UpdateRibbonElementText(ID_EDIT_MODEL_EDIT, _T("ID_EDIT_MODEL_EDIT"), false);
	UpdateRibbonElementText(ID_EDIT_MODEL_AUTO_ADD, _T("ID_EDIT_MODEL_AUTO_ADD"), false);
	//旋轉
	UpdateRibbonElementText(MENU_MODEL_EDIT_MODIFY, _T("MENU_MODEL_EDIT_MODIFY"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_ROTATE_090, _T("MENU_MODEL_EDIT_ROTATE_090"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_ROTATE_180, _T("MENU_MODEL_EDIT_ROTATE_180"), false);	
	UpdateRibbonElementText(MENU_MODEL_EDIT_ROTATE_270, _T("MENU_MODEL_EDIT_ROTATE_270"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_MIRROR_X_POS, _T("MENU_MODEL_EDIT_MIRROR_X_POS"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_MIRROR_Y_POS, _T("MENU_MODEL_EDIT_MIRROR_Y_POS"), false);		
	//腳數與間距
	UpdateRibbonElementText(MENU_MODEL_EDIT_LAND_COUNT, _T("MENU_MODEL_EDIT_LAND_COUNT"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_LAND_ARRAY, _T("MENU_MODEL_EDIT_LAND_ARRAY"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_LAND_PITCH, _T("MENU_MODEL_EDIT_LAND_PITCH"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_LAND_ALIGN, _T("MENU_MODEL_EDIT_LAND_ALIGN"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_LAND_COUNT_2D, _T("MENU_MODEL_EDIT_LAND_COUNT_2D"), false);
	//複製貼上	
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE, _T("MENU_MODEL_EDIT_CLONE"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_PASTE, _T("MENU_MODEL_EDIT_CLONE_PASTE"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_ARRAY_PASTE, _T("MENU_MODEL_EDIT_CLONE_ARRAY_PASTE"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_MIRROR_X_POS, _T("MENU_MODEL_EDIT_CLONE_MIRROR_X_POS"), false);	
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_MIRROR_Y_POS, _T("MENU_MODEL_EDIT_CLONE_MIRROR_Y_POS"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_ROTATE_090, _T("MENU_MODEL_EDIT_CLONE_ROTATE_090"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_ROTATE_180, _T("MENU_MODEL_EDIT_CLONE_ROTATE_180"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_ROTATE_270, _T("MENU_MODEL_EDIT_CLONE_ROTATE_270"), false);	
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_DIAGONAL, _T("MENU_MODEL_EDIT_CLONE_DIAGONAL"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_CLONE_CORNER_4, _T("MENU_MODEL_EDIT_CLONE_CORNER_4"), false);
	//刪除	
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE, _T("MENU_MODEL_EDIT_DELETE"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_SELECT, _T("MENU_MODEL_EDIT_DELETE_SELECT"), false);			
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_OTHERS, _T("MENU_MODEL_EDIT_DELETE_OTHERS"), false);		
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_OTHERS, _T("MENU_MODEL_EDIT_DELETE_OTHERS"), false);	
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_GROUP, _T("MENU_MODEL_EDIT_DELETE_GROUP"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_ALL, _T("MENU_MODEL_EDIT_DELETE_ALL"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_ALL_WND, _T("MENU_MODEL_EDIT_DELETE_ALL_WND"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_DERIVATIVE, _T("MENU_MODEL_EDIT_DELETE_DERIVATIVE"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_COMPONENT, _T("MENU_MODEL_EDIT_DELETE_COMPONENT"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DELETE_GROUP_LIBRARY, _T("MENU_MODEL_EDIT_DELETE_GROUP_LIBRARY"), false);
	//資料庫
	UpdateRibbonDefaultWndGroupText();
	UpdateRibbonElementText(MENU_MODEL_EDIT_ADD_ALL_WND, _T("MENU_MODEL_EDIT_ADD_ALL_WND"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_ADD_ALL_WND_V2, _T("MENU_MODEL_EDIT_ADD_ALL_WND_V2"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_CREATE_BK_IMAGE, _T("MENU_MODEL_EDIT_CREATE_BK_IMAGE"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_CREATE_BK_IMAGE_ALL, _T("MENU_MODEL_EDIT_CREATE_BK_IMAGE_ALL"), false);	
	UpdateRibbonElementText(MENU_MODEL_EDIT_PROPERTY_WND, _T("MENU_MODEL_EDIT_PROPERTY_WND"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY, _T("MENU_MODEL_EDIT_UPDATE_TO_LIBRARY"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_UPDATE_FROM_LIBRARY, _T("MENU_MODEL_EDIT_UPDATE_FROM_LIBRARY"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY_GROUP, _T("MENU_MODEL_EDIT_UPDATE_TO_LIBRARY_GROUP"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_SAVE_DEFAULT_MODEL, _T("MENU_MODEL_EDIT_SAVE_DEFAULT_MODEL"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_ARRANGE_MODEL, _T("MENU_MODEL_EDIT_ARRANGE_MODEL"), false);
	//顯示模式
	UpdateRibbonElementText(ID_SHOW_MODEL_EDIT_MODE, _T("ID_SHOW_MODEL_EDIT_MODE"), false);
	UpdateRibbonElementText(ID_SHOW_MODEL_RESULT_MODE, _T("ID_SHOW_MODEL_RESULT_MODE"), false);	
	//
	UpdateRibbonElementText(ID_SHOW_MODEL_LANDS, _T("ID_SHOW_MODEL_LANDS"), false);
	UpdateRibbonElementText(ID_HIDE_MODEL_LANDS, _T("ID_HIDE_MODEL_LANDS"), false);
	UpdateRibbonElementText(ID_SHOW_MODEL_ACTIVED_LINE, _T("ID_SHOW_MODEL_ACTIVED_LINE"), false);
	UpdateRibbonElementText(ID_SHOW_ALL_COMPONENTS, _T("ID_SHOW_ALL_COMPONENTS"), false);
	UpdateRibbonElementText(ID_SHOW_MODEL_WND_INDEX, _T("ID_SHOW_MODEL_WND_INDEX"), false);
	UpdateRibbonElementText(ID_SHOW_MODEL_LAND_INDEX, _T("ID_SHOW_MODEL_LAND_INDEX"), false);
	//連動模式
	UpdateRibbonElementText(MENU_MODEL_LINK_LAND_POS, _T("MENU_MODEL_LINK_LAND_POS"), false);
	UpdateRibbonElementText(MENU_MODEL_LINK_LAND_SIZE, _T("MENU_MODEL_LINK_LAND_SIZE"), false);
	UpdateRibbonElementText(MENU_MODEL_LINK_LAND_WND_POS, _T("MENU_MODEL_LINK_LAND_WND_POS"), false);
	UpdateRibbonElementText(MENU_MODEL_LINK_LAND_WND_SIZE, _T("MENU_MODEL_LINK_LAND_WND_SIZE"), false);	
	//設定		
	UpdateRibbonElementText(ID_SETTING_SAVE_FOV_IMAGES, _T("ID_SETTING_SAVE_FOV_IMAGES"), false);
	//啟用檢測框
	UpdateRibbonElementText(MENU_MODEL_EDIT_ENABLE_ALL_WND, _T("MENU_MODEL_EDIT_ENABLE_ALL_WND"), false);
	UpdateRibbonElementText(MENU_MODEL_EDIT_DISABLE_ALL_WND, _T("MENU_MODEL_EDIT_DISABLE_ALL_WND"), false);
	

	//About Software Barcode
	//調機-Tuning
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_ALIGN_FIDUCIAL, _T("MENU_SOFTWARE_BARCODE_ALIGN_FIDUCIAL"), true);
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_INSPECT_ALL, _T("MENU_SOFTWARE_BARCODE_INSPECT_ALL"), false);
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_INSPECT_SELECTED, _T("MENU_SOFTWARE_BARCODE_INSPECT_SELECTED"), false);
	//模式
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_ADD_MODE, _T("MENU_SOFTWARE_BARCODE_ADD_MODE"), false);	
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_EDIT_MODE, _T("MENU_SOFTWARE_BARCODE_EDIT_MODE"), false);
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_ADD_BARCODE_WND, _T("MENU_SOFTWARE_BARCODE_ADD_BARCODE_WND"), false);		
	//操作
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_BOARDS, _T("MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_BOARDS"), false);
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_PANELS, _T("MENU_SOFTWARE_BARCODE_PASTE_TO_OTHER_PANELS"), false);
	//刪除
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_DEL_SELECTED, _T("MENU_SOFTWARE_BARCODE_DEL_SELECTED"), false);	
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_DEL_OTHERS, _T("MENU_SOFTWARE_BARCODE_DEL_OTHERS"), false);	
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_DEL_ALL, _T("MENU_SOFTWARE_BARCODE_DEL_ALL"), false);	
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_DEL_SELECTED_WND, _T("MENU_SOFTWARE_BARCODE_DEL_SELECTED_WND"), false);	
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_DEL_OTHER_WNDS, _T("MENU_SOFTWARE_BARCODE_DEL_OTHER_WNDS"), false);	
	//條碼模組
	UpdateRibbonElementText(MENU_SOFTWARE_BARCODE_PROPERTY_WND, _T("MENU_SOFTWARE_BARCODE_PROPERTY_WND"), false);

	//About Mark
	//刪除
	UpdateRibbonElementText(MENU_MARK_EDIT_ALIGN_FIDUCIAL, _T("MENU_MARK_EDIT_ALIGN_FIDUCIAL"), false);	
	UpdateRibbonElementText(MENU_MARK_DEL_SELECTED, _T("MENU_MARK_DEL_SELECTED"), false);	
	UpdateRibbonElementText(MENU_MARK_DEL_GROUP, _T("MENU_MARK_DEL_GROUP"), false);
	UpdateRibbonElementText(MENU_MARK_DEL_GROUP_OTHERS, _T("MENU_MARK_DEL_GROUP_OTHERS"), false);	
	UpdateRibbonElementText(MENU_MARK_DEL_ALL, _T("MENU_MARK_DEL_ALL"), false);
	//模式
	UpdateRibbonElementText(MENU_MARK_GROUND_ADD_MODE, _T("MENU_MARK_GROUND_ADD_MODE"), false);	
	UpdateRibbonElementText(MENU_MARK_GROUND_EDIT_MODE, _T("MENU_MARK_GROUND_EDIT_MODE"), false);
	//操作
	UpdateRibbonElementText(MENU_MARK_GROUND_PASTE_TO_OTHER_BOARDS, _T("MENU_MARK_GROUND_PASTE_TO_OTHER_BOARDS"), false);
	//特徵點模組
	UpdateRibbonElementText(MENU_MARK_EDIT_PROPERTY_WND, _T("MENU_MARK_EDIT_PROPERTY_WND"), false);

	//Fiducial
	//調機-Tuning
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_FD, _T("MENU_FIDUCIAL_EDIT_ALIGN_FD"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_FD_ALL, _T("MENU_FIDUCIAL_EDIT_ALIGN_FD_ALL"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_FD_SELECTED, _T("MENU_FIDUCIAL_EDIT_ALIGN_FD_SELECTED"), false);	
	//模式
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ADD_MODE, _T("MENU_FIDUCIAL_EDIT_ADD_MODE"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_EDIT_MODE, _T("MENU_FIDUCIAL_EDIT_EDIT_MODE"), false);	
	//校正
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_CALIBRATION, _T("MENU_FIDUCIAL_EDIT_CALIBRATION"), false);
	//複製
	UpdateRibbonElementText(MENU_FIDUCIAL_PASTE_TO_OTHER_BOARDS, _T("MENU_FIDUCIAL_PASTE_TO_OTHER_BOARDS"), false);
	//刪除
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_DEL_SELECTED, _T("MENU_FIDUCIAL_EDIT_DEL_SELECTED"), false);	
	//對齊
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_PANEL, _T("MENU_FIDUCIAL_EDIT_ALIGN_PANEL"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_BOARD, _T("MENU_FIDUCIAL_EDIT_ALIGN_BOARD"), false);		
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO1"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO1"), false);		
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT1, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT1"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO2"), false);		
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO2"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT2, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT2"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO3"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO3"), false);		
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT3, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT3"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_BOARD_COMBO4"), false);		
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT_COMBO4"), false);
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT4, _T("MENU_FIDUCIAL_EDIT_ALIGN_SET_COMPONENT4"), false);		
	//定位點模組
	UpdateRibbonElementText(MENU_FIDUCIAL_EDIT_PROPERTY_WND, _T("MENU_FIDUCIAL_EDIT_PROPERTY_WND"), false);
	/*
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);		
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);		
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);		
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);		
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);
	UpdateRibbonElementText(AAAAAAAAAAAAAAAAAAAAAAA, _T("AAAAAAAAAAAAAAAAAAAAAAA"), false);			

*/
	m_wndRibbonBar.SetRedraw(TRUE);
	m_wndRibbonBar.ForceRecalcLayout();
#endif//FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	return;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CreateGlobalWnd()//建立全域的視窗
{
	bool IsOK = true;
	if ( SystemInitialWnd.Create(IDD_SYSTEM_INITIAL_WND, this) == FALSE )
	{	
		JetAPI::ShowMessageBox(_T("Failed to create System Init Wnd\n"));	
		IsOK = false;
	}

#ifndef CAMERA_OBJ_DISABLE
	if ( CameraCtrlWnd.Create(IDD_CAMERA_CTRL_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create Camera Ctrl Wnd\n"));			
		IsOK = false;
	}
#endif//CAMERA_OBJ_DISABLE

#ifndef MOTION_OBJ_DISABLE
	if ( MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this) == FALSE )
	{	
		JetAPI::ShowMessageBox(_T("Failed to create Motion Ctrl Wnd\n"));			
		IsOK = false;
	}
#endif//MOTION_OBJ_DISABLE

#ifndef PLC_OBJ_DISABLE
	if ( PlcCtrlWnd.Create(IDD_PLC_CTRL_WND, this) == FALSE )
	{	
		JetAPI::ShowMessageBox(_T("Failed to create PLC Ctrl Wnd\n"));	
		IsOK = false;
	}
#endif//PLC_OBJ_DISABLE

#ifndef PHASE_CTRL_DISABLE
	if ( Light3DTiDLPWnd.Create(IDD_LIGHT_3D_TIDLP_WND, this) == FALSE )
	{	
		JetAPI::ShowMessageBox(_T("Failed to create Light 3D TiDlp Ctrl Wnd\n"));	
		IsOK = false;
	}
#endif//PHASE_CTRL_DISABLE

#ifndef LIGHT_CTRL_DISABLE
	if ( LightCtrlBoardWnd.Create(IDD_LIGHT_CTRL_BOARD_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create Light Ctrl Board Wnd\n"));	
		IsOK = false;
	}
#endif//LIGHT_CTRL_DISABLE

#ifndef MES_DISABLE
	if ( ITSCommWnd.Create(IDD_ITS_COMM_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create ITS Comm Wnd\n"));	
		IsOK = false;
	}
#endif//MES_DISABLE

	if ( RulerWnd.Create(IDD_RULER_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create Ruler Wnd\n"));	
		IsOK = false;
	}	

	if ( MachineStatusWnd.Create(IDD_MACHEIN_STATUS_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create Machine Status Wnd\n"));	
		IsOK = false;
	}

	if ( ProjectGroupConfigWnd.Create(IDD_PROJECT_GROUP_CONFIG_WND, this) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Failed to create Project Group Config Wnd\n"));	
		IsOK = false;
	}

	if ( m_pSplitterWnd )
	{
		if ( m_pSplitterWnd->GetSafeHwnd() != NULL )
		{	m_pSplitterWnd->DestroyWindow();	}
		delete m_pSplitterWnd; m_pSplitterWnd=NULL; 
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::DestroyGlobalWnd()//摧毀全域的視窗
{	
	if ( RulerWnd.GetSafeHwnd() != NULL )
	{	RulerWnd.DestroyWindow(); }

	if ( ProjectGroupConfigWnd.GetSafeHwnd() != NULL )
	{	ProjectGroupConfigWnd.DestroyWindow(); }

	if ( LightCtrlBoardWnd.GetSafeHwnd() != NULL )
	{	LightCtrlBoardWnd.DestroyWindow(); }

	if ( Light3DTiDLPWnd.GetSafeHwnd() != NULL )
	{	Light3DTiDLPWnd.DestroyWindow(); }

	if ( PlcCtrlWnd.GetSafeHwnd() != NULL )
	{	PlcCtrlWnd.DestroyWindow(); }

	if ( MotionCtrlWnd.GetSafeHwnd() != NULL )
	{	MotionCtrlWnd.DestroyWindow(); }

	if ( CameraCtrlWnd.GetSafeHwnd() != NULL )
	{	CameraCtrlWnd.DestroyWindow(); }

	if ( SystemInitialWnd.GetSafeHwnd() != NULL )
	{	SystemInitialWnd.DestroyWindow();	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewPlcCtrlWnd() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam ) 
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	PlcCtrlWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewPlcCtrlWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();	
#ifdef PLC_OBJ_DISABLE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewMotionCtrlWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	MotionCtrlWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewMotionCtrlWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_XYZCtrl(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::SwitchPane(BOOL bShow)//切換主畫面視窗
{//20170103	
	return;
#if FRAME_STYLE_TYPE   != FRAME_STYLE_MFC
	BOOL bDelay=FALSE;
	BOOL bActivate=FALSE;
	
	if ( m_wndEditComponentList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditComponentList, bShow, bDelay, bActivate);	}	
	if ( m_wndEditModelList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditModelList, bShow, bDelay, bActivate);	}	
	if ( m_wndPartNumberList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndPartNumberList, bShow, bDelay, bActivate);	}	
	if ( m_wndEditMarkList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditMarkList, bShow, bDelay, bActivate);	}
	if ( m_wndEditResultList.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditResultList, bShow, bDelay, bActivate); }
	if ( m_wndEditWnd.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditWnd, bShow, bDelay, bActivate);	}
	if ( m_wndEditImage.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditImage, bShow, bDelay, bActivate);	}	

	if( TRUE == bShow )
	{	
		int FixedSize=0;
		if(FALSE==this->IsZoomed())
		{	FixedSize=-7;	}//不是最大化時須減7

		if ( m_wndEditLibrary.GetSafeHwnd() != NULL )
		{
			m_wndEditLibrary.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);
			
			RECT WndRect;
			m_wndEditLibrary.GetWindowRect(&WndRect);
			WndRect.right=WndRect.left+m_EditLibrarySize.cx;
			CRect Rect;
			Rect.left=WndRect.left+FixedSize;//不是最大化時須減7
			Rect.right=WndRect.right+FixedSize;//不是最大化時須減7
			Rect.top=WndRect.top+FixedSize;//不是最大化時須減7
			Rect.bottom=WndRect.bottom+FixedSize;//不是最大化時須減7
			m_wndEditLibrary.MoveWindow(Rect);
									
			m_wndEditLibrary.ShowWindow(SW_SHOW);
		}

		if ( m_wndEditProjectMap.GetSafeHwnd() != NULL )
		{
			m_wndEditProjectMap.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);
			
			RECT WndRect;
			m_wndEditProjectMap.GetWindowRect(&WndRect);
			WndRect.right=WndRect.left+m_EditProjectMapSize.cx;
			CRect Rect;
			Rect.left=WndRect.left+FixedSize;//不是最大化時須減7
			Rect.right=WndRect.right+FixedSize;//不是最大化時須減7
			Rect.top=WndRect.top+FixedSize;//不是最大化時須減7
			Rect.bottom=WndRect.bottom+FixedSize;//不是最大化時須減7
			m_wndEditProjectMap.MoveWindow(Rect);

			m_wndEditProjectMap.ShowWindow(SW_SHOW);
		}
		
	}
	else
	{
		
		if ( m_wndEditLibrary.GetSafeHwnd() != NULL )
		{
			m_wndEditLibrary.SetAutoHideMode(FALSE, CBRS_ALIGN_RIGHT, NULL, FALSE);
			m_wndEditLibrary.UndockPane();//不加這行邊界會多一塊
			m_wndEditLibrary.ShowWindow(SW_HIDE);
		}

		if ( m_wndEditProjectMap.GetSafeHwnd() != NULL )
		{
			m_wndEditProjectMap.SetAutoHideMode(FALSE, CBRS_ALIGN_RIGHT, NULL, FALSE);
			m_wndEditProjectMap.UndockPane();//不加這行邊界會多一塊
			m_wndEditProjectMap.ShowWindow(SW_HIDE);
		}		
	}		
	CDockingManager* pDockManager = GetDockingManager();
	ASSERT_VALID(pDockManager);
	pDockManager->HideAutoHidePanes();
	pDockManager->RecalcLayout();
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::SwitchDockingWnd(UINT ID)//切換駐列視窗
{		
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	BOOL bShow=TRUE;
	BOOL bShowEditWnd=TRUE;
	BOOL bShowEditComponentList=TRUE;
	BOOL bShowEditModelList=TRUE;
	BOOL bShowEditPartNumberList=TRUE;
	BOOL bShowEditMarkList=TRUE;
	BOOL bShowEditList=TRUE;
	BOOL bShowEditLibrary=TRUE;
	BOOL bShowEditImage=TRUE;
	BOOL bShowProjectMap=TRUE;
	BOOL bShowProperties=FALSE;
	BOOL bShowEditResultList=TRUE;
	BOOL bDelay=FALSE;
	BOOL bActivate=FALSE;	
	switch ( ID )
	{
	case IDD_EDIT_FORMVIEW:
	case IDD_EDIT_MODEL_VIEW:
		bShowEditWnd=TRUE;
		bShowEditList=TRUE;		
		bShowEditImage=TRUE;
		bShowProperties=TRUE;
		bShowEditLibrary=TRUE;
		bShowProjectMap=TRUE;
		bShowEditComponentList=TRUE;
		bShowEditModelList=TRUE;
		bShowEditPartNumberList=TRUE;
		bShowEditMarkList=FALSE;
		bShowEditResultList = TRUE;
		break;
	case IDD_EDIT_MAIN_VIEW:		
		bShowEditList=TRUE;	
		bShowEditWnd=FALSE;
		bShowEditImage=FALSE;
		bShowProperties=FALSE;
		bShowEditLibrary=TRUE;
		bShowProjectMap=TRUE;
		bShowEditComponentList=TRUE;
		bShowEditModelList=TRUE;
		bShowEditPartNumberList=TRUE;
		bShowEditMarkList=TRUE;
		bShowEditResultList = TRUE;
		break;
	case IDD_EDIT_FD_VIEW:
		bShowEditList=TRUE;	
		bShowEditWnd=TRUE;
		bShowEditImage=TRUE;
		bShowProperties=FALSE;
		bShowEditLibrary=FALSE;
		bShowProjectMap=TRUE;
		bShowEditComponentList=TRUE;
		bShowEditModelList=FALSE;
		bShowEditPartNumberList=FALSE;
		bShowEditMarkList=FALSE;
		bShowEditResultList = FALSE;
		break;
	case IDD_EDIT_BARCODE_VIEW:
		bShowEditList=TRUE;	
		bShowEditWnd=TRUE;
		bShowEditImage=TRUE;
		bShowProperties=FALSE;
		bShowEditLibrary=FALSE;
		bShowProjectMap=TRUE;
		bShowEditComponentList=TRUE;
		bShowEditModelList=FALSE;
		bShowEditPartNumberList=FALSE;
		bShowEditMarkList=FALSE;
		bShowEditResultList = FALSE;
		break;
	case IDD_EDIT_MARK_VIEW:
		bShowEditList=TRUE;	
		bShowEditWnd=TRUE;
		bShowEditImage=TRUE;
		bShowProperties=FALSE;
		bShowEditLibrary=FALSE;
		bShowProjectMap=TRUE;
		bShowEditComponentList=TRUE;
		bShowEditModelList=FALSE;
		bShowEditPartNumberList=FALSE;
		bShowEditMarkList=TRUE;
		bShowEditResultList = TRUE;
		break;
	case IDD_ONLINE_FORMVIEW:
	case IDD_ONLINE_FORMVIEW_DUAL:
		bShowEditWnd=FALSE;
		bShowEditList=FALSE;
		bShowEditImage=FALSE;
		bShowProperties=FALSE;
		bShowEditLibrary=FALSE;
		bShowProjectMap=FALSE;
		bShowEditComponentList=FALSE;
		bShowEditModelList=FALSE;
		bShowEditPartNumberList=FALSE;
		bShowEditMarkList=FALSE;
		bShowEditResultList = FALSE;
		break;
	case IDD_DEBUG_FORMVIEW:
		bShowEditWnd=FALSE;
		bShowEditList=FALSE;
		bShowEditImage=FALSE;
		bShowProperties=FALSE;
		bShowEditLibrary=FALSE;
		bShowProjectMap=FALSE;
		bShowEditComponentList=FALSE;
		bShowEditModelList=FALSE;
		bShowEditPartNumberList=FALSE;
		bShowEditMarkList=FALSE;
		bShowEditResultList = FALSE;
		break;
	}
	this->SwitchPane(bShow);
	
	bool bDoneEditComponentList=false;
	bool bDoneEditModelList=false;
	bool bDoneEditPartNumberList=false;
	bool bDoneEditMarkList=false;	
	bool bDoneEditResultList=false;	
	
	PreShowPane(&m_wndEditComponentList, bShowEditComponentList, bDelay, bActivate, bDoneEditComponentList);
	PreShowPane(&m_wndEditModelList, bShowEditModelList, bDelay, bActivate, bDoneEditModelList);	
	PreShowPane(&m_wndPartNumberList, bShowEditPartNumberList, bDelay, bActivate, bDoneEditPartNumberList);	
	PreShowPane(&m_wndEditMarkList, bShowEditMarkList, bDelay, bActivate, bDoneEditMarkList);		
	PreShowPane(&m_wndEditResultList, bShowEditResultList, bDelay, bActivate, bDoneEditResultList); 
	if ( false == bDoneEditComponentList )
	{	ShowPane(&m_wndEditComponentList, bShowEditComponentList, bDelay, bActivate);	}
	if ( false == bDoneEditModelList )
	{	ShowPane(&m_wndEditModelList, bShowEditModelList, bDelay, bActivate);	}
	if ( false == bDoneEditPartNumberList  )
	{	ShowPane(&m_wndPartNumberList, bShowEditPartNumberList, bDelay, bActivate);	}
	if ( false == bDoneEditMarkList )
	{	ShowPane(&m_wndEditMarkList, bShowEditMarkList, bDelay, bActivate);	}
	if ( false == bDoneEditResultList )
	{	ShowPane(&m_wndEditResultList, bShowEditResultList, bDelay, bActivate); }

	//if ( m_wndEditComponentList.GetSafeHwnd() != NULL )
	//{	ShowPane(&m_wndEditComponentList, bShowEditComponentList, bDelay, bActivate);	}
	//if ( m_wndEditModelList.GetSafeHwnd() != NULL )
	//{	ShowPane(&m_wndEditModelList, bShowEditModelList, bDelay, bActivate);	}
	//if ( m_wndPartNumberList.GetSafeHwnd() != NULL )
	//{	ShowPane(&m_wndPartNumberList, bShowEditPartNumberList, bDelay, bActivate);	}
	//if ( m_wndEditMarkList.GetSafeHwnd() != NULL )
	//{	ShowPane(&m_wndEditMarkList, bShowEditMarkList, bDelay, bActivate);	}
	//if ( m_wndEditResultList.GetSafeHwnd() != NULL )
	//{	ShowPane(&m_wndEditResultList, bShowEditResultList, bDelay, bActivate); }
	if ( m_wndEditWnd.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditWnd, bShowEditWnd, bDelay, bActivate);	}
	if ( m_wndEditImage.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndEditImage, bShowEditImage, bDelay, bActivate);	}			
	if ( m_wndProperties.GetSafeHwnd() != NULL )
	{	ShowPane(&m_wndProperties, bShowProperties, bDelay, bActivate);	}	

	if ( m_wndEditProjectMap.GetSafeHwnd() != NULL )
	{
		if ( FALSE == bShowProjectMap )
		{	
			if ( m_wndEditProjectMap.IsWindowVisible() == TRUE )
			{	m_wndEditProjectMap.ShowWindow(SW_HIDE); }
			m_wndEditProjectMap.SetAutoHideMode(FALSE, CBRS_ALIGN_RIGHT, NULL, FALSE);	
			ShowPane(&m_wndEditProjectMap, bShowProjectMap, bDelay, bActivate);	
		}
		else
		{	
			m_wndEditProjectMap.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE); 
			m_wndEditProjectMap.PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
		}
		
	}	

	if ( m_wndEditLibrary.GetSafeHwnd() != NULL )
	{
		if ( FALSE == bShowEditLibrary ) 
		{	
			if ( m_wndEditLibrary.IsWindowVisible() == TRUE )
			{	m_wndEditLibrary.ShowWindow(SW_HIDE); }
			m_wndEditLibrary.SetAutoHideMode(FALSE, CBRS_ALIGN_RIGHT, NULL, FALSE);	
			ShowPane(&m_wndEditLibrary, bShowEditLibrary, bDelay, bActivate);	
		}
		else
		{	
			m_wndEditLibrary.SetAutoHideMode(TRUE, CBRS_ALIGN_RIGHT, NULL, FALSE);	
			m_wndEditLibrary.PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
		}		
		
	}
	/*
	CDockingManager *pDockingManager = GetDockingManager();
	if ( NULL != pDockingManager )
	{
		ASSERT_VALID(pDockingManager);
		pDockingManager->HideAutoHidePanes();
		pDockingManager->RecalcLayout();
	}*/
#endif//FRAME_STYLE_TYPE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ShowGlobalWnd(CWnd *pWnd, BOOL bShow)//顯示全域視窗
{
	if ( NULL == pWnd ) { return false; }
	if ( pWnd->GetSafeHwnd() == NULL ) { return false; }
	BOOL bVisible=pWnd->IsWindowVisible();
	if ( bVisible == bShow ) { return true; }
	if ( TRUE == bShow )
	{	pWnd->ShowWindow(SW_SHOW);	}
	else
	{	pWnd->ShowWindow(SW_HIDE);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ResetMainViewWndAndGrabImage(bool ResetProjectLight, bool ReGrab)//重新取像
{
	HWND   hWnd  = NULL;
	CView *pView = GetActiveView();
	if ( NULL == pView ) { return false; }

	if ( true == ResetProjectLight )
	{
		CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
			{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
		}
	}
	hWnd = pView->GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);
	if ( true == ReGrab )
	{	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::SwitchViewWnd(UINT ID)//切換主畫面視窗
{
	const UINT ViewIDBefore = AOIDataCollect.GetMainViewWndID();
	if ( ViewIDBefore == ID ) { return; }

	BOOL bShow = TRUE;
	//this->CloseSubWindow();
	CCreateContext ctx;
	CView *pNewView = NULL;
	CWnd  *pPane1 = NULL;
	CWnd  *pPane2 = NULL;
	CFrameWnd  *pChildFrame = NULL;
	CBasicSplitterWnd *pNewSplitterWnd = NULL;
	CView *pLastView = this->GetActiveView();	
	if ( pLastView == NULL )
	{
		JetAPI::ShowMessageBox(_T("Error, No Last View Ptr"));
		return;
	}
    
	CDocument *pDoc2 = NULL;
	CDocument *pDocument = pLastView->GetDocument();//call before you destroy m_splitter.
	if ( pDocument == NULL ) 
	{
		JetAPI::ShowMessageBox(_T("Error, No Document Ptr"));
		return;
	}

	pLastView->ShowWindow(SW_HIDE);

	//ctx.m_pCurrentDoc = GetActiveDocument();	//CodeJock不能使用
	ctx.m_pCurrentDoc     = NULL;
	ctx.m_pCurrentFrame   = this;
	ctx.m_pNewDocTemplate = pDocument->GetDocTemplate();	
	
	SwitchDockingWnd(ID);
	AOIDataCollect.SetMainViewWndID(ID);	
	AOIDataCollect.RegistUserLastInputTickCount();
	switch ( ViewIDBefore )
	{
	case IDD_EDIT_FORMVIEW:
		break;
	case IDD_EDIT_MODEL_VIEW:
		break;
	case IDD_EDIT_MAIN_VIEW:
		break;
	case IDD_EDIT_FD_VIEW:		
		PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
		break;
	case IDD_EDIT_BARCODE_VIEW:		
		PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
		break;
	case IDD_EDIT_MARK_VIEW:
		PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);		
		break;
	case IDD_ONLINE_FORMVIEW:
	case IDD_ONLINE_FORMVIEW_DUAL:
		break;
	case IDD_DEBUG_FORMVIEW:
		break;
	}
	if ( GetLockUIWnd() == false ) 
	{	ResetMainViewWndAndGrabImage(true, false); }

#ifndef OFFLINE_VERSION
	MES_STATAUS_ID MesStatusID=MES_STATAUS_NONE;
	if ( IDD_ONLINE_FORMVIEW==ID || IDD_ONLINE_FORMVIEW_DUAL==ID )
	{	MesStatusID=MES_STATAUS_AOI_ONLINE_TEST;	}
	else
	{	MesStatusID=MES_STATAUS_AOI_EDIT_MODE;	}
	if ( MES_STATAUS_NONE != MesStatusID )
	{	AOIDataCollect.ExecMESComm_ProcessID(MesStatusID);	}
#endif//OFFLINE_VERSION

	switch ( ID ) // these IDs are the dialog IDs of the view but can use anything
	{
	case IDD_EDIT_FORMVIEW:		
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditFormView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditFormView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);		
	 	break;	
	case IDD_EDIT_MODEL_VIEW:			
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditModelView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditModelView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);		
		break;
	case IDD_EDIT_MAIN_VIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditMainView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditMainView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);		
		break;
	case IDD_EDIT_FD_VIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditFdView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditFdView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);
		//::PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
		//::PostMessage(pNewView->GetSafeHwnd(), MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
		break;
	case IDD_EDIT_BARCODE_VIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditBarcodeView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditBarcodeView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);
		//PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
		//::PostMessage(pNewView->GetSafeHwnd(), MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);		
		break;
	case IDD_EDIT_MARK_VIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(CEditMarkView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CEditMarkView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);
		//PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
		//::PostMessage(pNewView->GetSafeHwnd(), MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);		
		break;
	case IDD_ONLINE_FORMVIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(COnlineFormView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [COnlineFormView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(TRUE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);		
		break;
	case IDD_ONLINE_FORMVIEW_DUAL:
		ctx.m_pNewViewClass = RUNTIME_CLASS(COnlineFormView_Dual);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [COnlineFormView_Dual]"));
			return;
		}
		//CMainFrame::ShowDockingPane(TRUE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);		
		break;
	case IDD_DEBUG_FORMVIEW:
		ctx.m_pNewViewClass = RUNTIME_CLASS(CDebugFormView);
		pNewView = (CView*)CreateView(&ctx);
		if ( pNewView == NULL )
		{
			JetAPI::ShowMessageBox(_T("Error, Create View Fault [CDebugFormView]"));
			return;
		}
		//CMainFrame::ShowDockingPane(FALSE);	
	 	pDocument->AddView(pNewView);
		pNewView->OnInitialUpdate();
	 	SetActiveView(pNewView);	
		break;
	}	

	if ( m_pSplitterWnd ) //之前的狀況
	{ 
		if ( m_pSplitterWnd->GetSafeHwnd() != NULL )
		{	m_pSplitterWnd->DestroyWindow();	}
		delete m_pSplitterWnd; m_pSplitterWnd=NULL; 		
	}
	else
	{
		if ( pLastView != NULL )
		{	pLastView->DestroyWindow();  } //刪除目前主要
	}

	if ( pNewSplitterWnd != NULL )
	{	m_pSplitterWnd = pNewSplitterWnd;	}

	CView *pView= (CView*) this->GetActiveView();
	CWnd *ParWnd = pView->GetParent();

	//m_ProgressWnd.ShowWindow(SW_HIDE);
	// Redisplay frame.	
	//CMainDoc *pDoc = (CMainDoc *)this->GetActiveDocument();
	//if ( pDoc != NULL )
	//{	pDoc->UpdateTitle();	}

	RecalcLayout();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditModelView() 
{
	// TODO: Add your command handler code here
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	SwitchViewWnd(IDD_EDIT_MODEL_VIEW);
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_EDIT_MODEL_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_EDIT_MODEL_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewProjectParamWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return false; }
	
	CProjectParamWnd  Wnd;	
	TProjectParameter ParamOld = ProjectPtr->GetProjectParameter();
	ProjectPtr->BackupProjectParameter();
	Wnd.SetProjectParameter(ProjectPtr, ParamOld);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{
		ProjectPtr->SetProjectParameter(ParamOld);	
		ProjectPtr->RestoreProjectParameter();		
	}
	else
	{
		TProjectParameter  &ParamNew = Wnd.GetProjectParameter();
		ProjectPtr->SetProjectParameter(ParamNew);	
		LogOperCtrl.SaveLogProjectComparParam(ProjectPtr, ParamOld);
	}	
	ProjectPtr->ApplyProjectParameter();	
	AOIDataCollect.ApplyProjectParameterToSystem(ProjectPtr);		
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_UPDATE_DOCUMENT_TITLE, NULL);
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)this);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectParamWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( AOIDataCollect.OperateLevelProjectParam() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	ExecViewProjectParamWnd();
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectParamWnd(CCmdUI* pCmdUI) 
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewSystemConfigWnd()
{
	CString str;
	CSystemConfigWnd Wnd;
	TSystemParameter SysParamOld = AOIDataCollect.GetSystemParameter();
	str.Format(_T("CMainFrame::ExecViewSystemConfigWnd Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	Wnd.SetSystemParameter(SysParamOld);
	if ( Wnd.DoModal() == IDCANCEL ) { return false; }
	const UINT ViewID = AOIDataCollect.GetMainViewWndID();
	TSystemParameter SysParamNew = Wnd.GetSystemParameter();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	AOIDataCollect.SetSystemParameter(SysParamNew);	
	AOIDataDefine.LoadDefineTextFile();	
	AOIDataCollect.UpdateLaneWorkMode(TASK_NONE);		
	if ( AOIDataCollect.CreateSystemDirectory() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }	
	if ( AOIDataCollect.UpdateSystemParameter() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( AOIDataCollect.UpdateSystemParameterToProject() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
	LogOperCtrl.SaveLogSystemComparParam(SysParamOld, SysParamNew);
	AOIDataCollect.ApplyProjectParameterToSystem(ProjectPtr);	
	if ( AOIDataCollect.ExecMESComm_ApplySystemParam() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}	
	AOIDataCollect.BackupAllSystemIniFiles();

	if ( IDD_ONLINE_FORMVIEW==ViewID || IDD_ONLINE_FORMVIEW_DUAL==ViewID )
	{
		if ( SysParamNew.m_CpkChartEnabled != SysParamOld.m_CpkChartEnabled )
		{	AOIDataCollect.SetMainViewWndID(0);	}
	}
	str.Format(_T("CMainFrame::ExecViewSystemConfigWnd End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewSystemConfigWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	if ( AOIDataCollect.OperateLevelEditWndSystemConfig() == false )
	{	return; }
	ExecViewSystemConfigWnd();
	AOIDataCollect.UserLogout_Check();
	CWnd::PostMessage(MSG_RIBBON_BAR_WND, WPARAM_SHOW_DEBUG_CATEGORY, AOIDataCollect.GetShowRibbonBarWndCategoryDebug());
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewSystemConfigWnd(CCmdUI* pCmdUI) 
{
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewSystemInitialWnd() 
{
	// TODO: Add your command handler code here
	if ( SystemInitialWnd.IsWindowVisible() == FALSE )
	{	SystemInitialWnd.ShowWindow(SW_SHOW);	}
	else
	{	SystemInitialWnd.ShowWindow(SW_HIDE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewSystemInitialWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( SystemInitialWnd.IsWindowVisible() == TRUE )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }

	const bool bRemoteState = CheckMesCtrlState_Remote();
	if ( true == bRemoteState )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default		
	const bool bLock = this->GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	if ( true == bLock || true==bRemoteState )
	{	return; }

	CMainFrame::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);
	CMainFrame::ReleaseApp();
	CBasicFrame::OnClose();
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::InitialMainFrame()//執行應用框架的建立
{
	AOIDataCollect.PreInitResource();
	AOIDataCollect.InitialResource();	
	AOIDataCollect.CopySystemParamFileToProjectFolder();
	CMainFrame::CreateGlobalWnd();
	CMainFrame::InitialApp();		
#ifndef OFFLINE_VERSION
	HWND hWnd = GetSafeHwnd();
	HWND hWndLimit = JetAPI::GetAncestorHWnd(hWnd);
	AOIDataCollect.SetLockScreenLimitWnd(hWndLimit);	
#endif//OFFLINE_VERSION	
	
	CString Title;
	CDocument *pDoc = this->GetActiveDocument();
	if ( NULL != pDoc )
	{	Title = pDoc->GetTitle();	}
	m_DefaultDocTitle = Title;
	UpdateDocumentTitle();

	AOIDataCollect.SetOnlineTaskCancel(false);
	AOIDataCollect.SetAutoSwitchToOnlineRemoteCtrlEnable(true);
	if ( AOIDataCollect.CheckAutoSwitchToOnlineRemoteCtrlEnable() == true )
	{
		MES_EQP_CTRL_STATE_MODE MesEqpCtrlStateMode=AOIDataCollect.GetSystemParameter().m_AutoSwitchToOnlineRemoteCtrlMode;	
		UINT WndCmdID = AOIDataCollect.GetAutoSwitchToOnlineRemoteCtrlCmdID(MesEqpCtrlStateMode);	
		if ( 0 != WndCmdID )
		{	CWnd::PostMessage(WM_COMMAND, WndCmdID, NULL);	}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::InitialApp()//軟體初始化
{
	CString str;
	HWND hWnd = this->GetSafeHwnd();
	AOIDataCollect.SetMainFrameWnd(hWnd);
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	CDockablePane::m_nSlideSteps=SysParam.m_UIDockWndSlideSteps;//側邊視窗動畫速度, 值越小, 速度越快
	
	CreateUserAccelTable();	
	AOIExceptionCodeCtrl.SetExceptionCodeRunning(true);
#ifndef OPENCV_DISABLE	
	bool bEnableCVOptimized=false;
	bool bSupportedList[32]={false};
	bool opt_status = cv::useOptimized();
	if ( true == bEnableCVOptimized )
	{
		cv::setUseOptimized(true);
		bSupportedList[CV_CPU_MMX] = cv::checkHardwareSupport(CV_CPU_MMX);
		bSupportedList[CV_CPU_SSE] = cv::checkHardwareSupport(CV_CPU_SSE);
		bSupportedList[CV_CPU_SSE2] = cv::checkHardwareSupport(CV_CPU_SSE2);
		bSupportedList[CV_CPU_SSE3] = cv::checkHardwareSupport(CV_CPU_SSE3);
		bSupportedList[CV_CPU_SSSE3] = cv::checkHardwareSupport(CV_CPU_SSSE3);
		bSupportedList[CV_CPU_SSE4_1] = cv::checkHardwareSupport(CV_CPU_SSE4_1);
		bSupportedList[CV_CPU_SSE4_2] = cv::checkHardwareSupport(CV_CPU_SSE4_2);
		bSupportedList[CV_CPU_POPCNT] = cv::checkHardwareSupport(CV_CPU_POPCNT);
		bSupportedList[CV_CPU_AVX] = cv::checkHardwareSupport(CV_CPU_AVX);
		bSupportedList[CV_CPU_AVX2] = cv::checkHardwareSupport(CV_CPU_AVX2);
		bSupportedList[31] = false;
	}
#endif//OPENCV_DISABLE	
	
	//AOIDataCollect.InitialResource();
	if ( SystemInitialWnd.GetSafeHwnd() != NULL )
	{
		SystemInitialWnd.ShowWindow(SW_SHOW);
		::Sleep(10);
		if ( SystemInitialWnd.SystemInitialize() == false )
		{	JetAPI::ShowMessageBox(SystemInitialWnd.GetErrorString());	}
		else
		{	
			::Sleep(1000);
			SystemInitialWnd.ShowWindow(SW_HIDE); 
		}
	}

#ifndef MES_DISABLE
	//ITSCommWnd.ShowWindow(SW_SHOW);
#endif//MES_DISABLE

	AOIDataCollect.SwitchUser(true);
	AOIDataCollect.SendToVRS_ProjectList();	
	if ( AOIDataCollect.BackupAllSystemFilesFirst() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	if ( AOIDataCollect.ExecMESComm_SetSystemParam() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	else
	{	AOIDataCollect.ExecMESComm_SetMachineStatus();	}
	//if ( PlcCtrlPtr->GetCurrentGreenLightStats() == true )//啟動燈亮起來		

	bool Done = false;
	bool bIsOK = true;	
	const bool HostTuning = AOIDataCollect.GetIsOfflineHostTuningVersion();
	const bool RemoteTuning = AOIDataCollect.GetIsOfflineRemoteTuningVersion();
	bIsOK = ExecHomeAll(true, true, Done);
	if ( true == bIsOK )
	{	
		if ( true == Done )
		{	ShowHomeAllFinish(); }
		if ( true == HostTuning )
		{	
			bIsOK = ExecProjectAutoLoad();	 
			if ( true==bIsOK )
			{
				const bool OnlineTuningEnable=true;
				CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
				if ( NULL != ProjectPtr )
				{
					AOIDataCollect.SetOnlineTuningEnable(OnlineTuningEnable);
					ProjectPtr->WriteProjectOnlineTuningEnable(OnlineTuningEnable); 
				}		
			}
		}		
		if ( true == RemoteTuning )
		{	CWnd::PostMessage(WM_COMMAND, ID_VIEW_REMOTE_PARAM_WND, NULL);	 }	
	}		
	PostMessage(MSG_RIBBON_BAR_WND, WPARAM_UPDATE_PCB_DIRECTION, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ReleaseApp()//軟體釋放
{
	DestroyUserAccelTable();
	AOIExceptionCodeCtrl.SetExceptionCodeRunning(false);
	if ( SystemInitialWnd.GetSafeHwnd() != NULL )
	{	
		SystemInitialWnd.ShowWindow(SW_SHOW);
		::Sleep(1000);
		SystemInitialWnd.SystemRelease();
		::Sleep(1000);		
	}
	AOIDataCollect.ReleaseResource();	
	AOIDataCollect.SetMainFrameWnd(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CheckMesCtrlState_Offline() const
{
	return CheckMesCtrlState(MES_EQP_CTRL_STATE_OFFLINE);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CheckMesCtrlState_Local() const
{
	return CheckMesCtrlState(MES_EQP_CTRL_STATE_LOCAL);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CheckMesCtrlState_Remote() const
{
	return CheckMesCtrlState(MES_EQP_CTRL_STATE_REMOTE);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::CheckMesCtrlState(MES_EQP_CTRL_STATE_MODE Mode) const
{
	return AOIDataCollect.CheckMES_CtrlState(Mode);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateUI_System(CCmdUI* pCmdUI)
{
	const bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	if ( true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateUI_XYZCtrl(CCmdUI* pCmdUI)
{
	const bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
#ifdef MOTION_OBJ_DISABLE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//MOTION_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::CheckUpdateUI_LaneCtrl()
{
	BOOL bEnable=TRUE;
	const bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const bool bLaneWorkDisable = AOIDataCollect.CheckLaneWorkMode_Disable(LaneID);
#ifdef PLC_OBJ_DISABLE
	bEnable = FALSE;	
#else
	if ( true==bLockUIWnd || true==bRemoteState || true==bLaneWorkDisable )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
#endif//OFFLINE_VERSION
	return bEnable;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateUI_LaneCtrl(CCmdUI* pCmdUI)
{
	BOOL bEnable=CheckUpdateUI_LaneCtrl();
	pCmdUI->Enable(bEnable);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateUI_ProjectParam(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateUI_ProjectTuning(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewCameraCtrlWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);	
	if ( CameraCtrlWnd.IsWindowVisible() == FALSE )
	{
		MotionCtrlPtr->XYMoveTo(PosX, PosY);
		MotionCtrlPtr->WaitForMotionStop();
		CameraCtrlWnd.ShowWindow(SW_SHOW);	
	}
	else
	{	CameraCtrlWnd.ShowWindow(SW_HIDE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewCameraCtrlWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
#ifdef CAMERA_OBJ_DISABLE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	if ( CameraCtrlWnd.IsWindowVisible() == TRUE )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewPhaseCtrlWnd() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndPhaseCtrl() == false )
	{	return; }
	Light3DTiDLPWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewPhaseCtrlWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();	
#ifdef PHASE_CTRL_DISABLE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	if ( Light3DTiDLPWnd.IsWindowVisible() == TRUE )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
#endif//PHASE_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewCalibrationWnd()
{
	DWORD Res=0;
	CString str;
	CCalibrationWnd Wnd;	
	TSystemParameter SysParamOld=AOIDataCollect.GetSystemParameter();
	str.Format(_T("CMainFrame::ExecViewCalibrationWnd Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	AOIDataCollect.CreateAOITempDirectory();
	Res = Wnd.DoModal(); 	
	AOIDataCollect.SetupAllLightSetting();	
	ResetMainViewWndAndGrabImage(true, true);
	if ( IDCANCEL == Res )
	{	return false; }		
	AOIDataCollect.CreateAOITempDirectory();	
	TSystemParameter SysParamNew = AOIDataCollect.GetSystemParameter();
	LogOperCtrl.SaveLogSystemComparParam(SysParamOld, SysParamNew);	
	str.Format(_T("CMainFrame::ExecViewCalibrationWnd End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewCalibrationWnd() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndCalibration() == false )
	{	return; }
	ExecViewCalibrationWnd();
	AOIDataCollect.UserLogout_Check();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewCalibrationWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
#ifdef OFFLINE_VERSION
	#ifndef _DEBUG
		pCmdUI->Enable(FALSE);
	#else
		pCmdUI->Enable(TRUE);
	#endif//_DEBUG
#else
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnApplicationLook(UINT id)
{	
//	id = ID_VIEW_APPLOOK_OFF_2007_SILVER;
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	//在JET8000.rc最末端要加入以下-afxribbon.rc
	//#if !defined(_AFXDLL)
	//#include "l.CHT\afxribbon.rc"        // MFC 功能區和控制列資源
	//#endif
	BOOL   bWin7Look = FALSE;
	CWaitCursor wait;
	theApp.m_nAppLook = id;
	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));				
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));		
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));		
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);		
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);		
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		bWin7Look = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);		
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);		
	}
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	m_wndRibbonBar.SetWindows7Look(bWin7Look);
#endif//FRAME_STYLE_TYPE

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CBasicFrame::OnSettingChange(uFlags, lpszSection);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnXYZHomeAll() 
{
	// TODO: Add your command handler code here
	bool Done = false;
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	if ( ExecHomeAll(false, true, Done) == false )
	{	return; }
	if ( true == Done )
	{	ShowHomeAllFinish();	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateXYZHomeAll(CCmdUI* pCmdUI)
{
	ExecUpdateUI_XYZCtrl(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnXYZGoToORG() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	//if ( MotionCtrlPtr->XYZMoveTo(0, 0, 0) == false )
	if ( MotionCtrlPtr->XYMoveTo(0, 0) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateXYZGoToORG(CCmdUI* pCmdUI)
{
	ExecUpdateUI_XYZCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnXYZGoToFocus() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	double PosZ = 0;			
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )
	{	PosZ = MotionCtrlPtr->GetMotionParameter().m_StageStartPosZ;	}
	else
	{	PosZ = ProjectPtr->GetProjectFocusPos();	}	
	
	if ( MotionCtrlPtr->MoveTo(AXIS_Z, PosZ, MOTION_MOVING_NORMAL) == false )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateXYZGoToFocus(CCmdUI* pCmdUI)
{
	ExecUpdateUI_XYZCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnXYZGoToLeave()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	const double PosX = MotionCtrlPtr->GetMotionParameter().m_StageLeavePosX;
	const double PosY = MotionCtrlPtr->GetMotionParameter().m_StageLeavePosY;
	const double PosZ = MotionCtrlPtr->GetMotionParameter().m_StageLeavePosZ;
	//if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateXYZGoToLeave(CCmdUI* pCmdUI)
{
	ExecUpdateUI_XYZCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnXYZCtrlGroup() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateXYZCtrlGroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBIn() 
{
	// TODO: Add your command handler code here
	ExecPCBIn();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBIn(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBIn2nd()
{
	ExecPCBIn2nd();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBIn2nd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBIn3rd()
{
	ExecPCBIn3rd();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBIn3rd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBOut() 
{
	// TODO: Add your command handler code here
	ExecPCBOut();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBOut(CCmdUI* pCmdUI)
{
	//ExecUpdateUI_LaneCtrl(pCmdUI);
	BOOL bEnable=TRUE;
	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	bEnable = FALSE;	}
	else
	{	bEnable = CheckUpdateUI_LaneCtrl();	}
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBBack() 
{
	// TODO: Add your command handler code here
	ExecPCBBack();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBBack(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBBackOut()
{
	// TODO: Add your command handler code here
	ExecPCBBackOut();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBBackOut(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBClear()
{
	ExecPCBClear();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBClear(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBClampOn() 
{
	// TODO: Add your command handler code here
	ExecPCBClampOn();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBClampOn(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLanePCBClampOff() 
{
	// TODO: Add your command handler code here
	ExecPCBClampOff();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLanePCBClampOff(CCmdUI* pCmdUI)
{
	ExecUpdateUI_LaneCtrl(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLaneCtrlGroup() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLaneCtrlGroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewCudaCtrlWnd()
{
	CCudaCtrlWnd Wnd;	
	Wnd.DoModal();
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewCudaCtrlWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( AOIDataCollect.OperateLevelEditWndCudaCtrl() == false)
	{	return ; }
	ExecViewCudaCtrlWnd();	
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewCudaCtrlWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
#ifndef CUDA_USE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//CUDA_USE
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewImageConfigWnd()
{
	CString  str;
	CImageConfigWnd Wnd;
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return false; }
	AOIDataCollect.BackupAllSystemIniFiles();
	Light3DCtrl.ClearAllLight3DPatternList();//確保每次都重新設定樣板
	Light3DCtrl.ReleaseAllLight3DBinParam();//確保每次都重新載入設定Bin參數
	if ( AOIDataCollect.UpdateSystemParameterToProject() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str); 
	}
	CAOIProject *ProjectPtr=AOIDataCollect.GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str); 
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewImageConfigWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndImageConfig() == false )
	{	return; }
	ExecViewImageConfigWnd();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewImageConfigWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewLightCtrlBoardWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndLightCtrlBoard() == false )
	{	return; }

	//ID_VIEW_LIGHT_CTRL_BOARD_WND
//	CLightCtrlBoardWnd Wnd;
//	Wnd.DoModal();
	LightCtrlBoardWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewLightCtrlBoardWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();	
#ifdef LIGHT_CTRL_DISABLE
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewRulerWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	if ( RulerWnd.GetSafeHwnd() == NULL ) { return ; }
	RulerWnd.ShowWindow(SW_SHOW);
	//CRulerWnd Wnd;
	//Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewPropertiesWnd() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewPropertiesWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditlistWnd() 
{
	// TODO: Add your command handler code here	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewEditlistWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if ( ExecUserAccelTable(pMsg) == true )	
	{	return TRUE; }

	CView *ActViewPtr=NULL;
	AOIDataCollect.SetUserLastInputTickCountByMsg(pMsg->message);	
	switch ( pMsg->message )
	{	
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{	
		case VK_INSPECTION://F5
			ActViewPtr = GetActiveView();
			if ( NULL != ActViewPtr )
			{	::PostMessage(ActViewPtr->GetSafeHwnd(), WM_COMMAND, ID_TUNE_INSPECTION_GROUP, NULL);	}
			break;
		case VK_ONLINE_RUN://F6
			PostMessage(WM_COMMAND, ID_ONLINE_RUN, NULL);
			break;
		case VK_ENHANCE_IMAGE://F9
			AOIDataCollect.ToggleIsEnhanceDisplayImage();
			break;
		}
		AOIDataCollect.ExecLockScreen(pMsg->wParam, CWnd::GetSafeHwnd());
		break;
	case WM_KEYUP:		
		break;
	}	
	return CBasicFrame::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	UINT   uInt=0;
	DWORD  Res = 0;
	HWND   hWnd = NULL;
	CWnd  *pWnd = NULL;		
	switch ( message )
	{
	case MSG_LOCK_SCREEN_HOOK:
	#ifndef OFFLINE_VERSION
		if ( AOIDataCollect.GetLockScreenEnabled() == true ) 
		{
			AOIDataCollect.UnlockScreenFunc();
			if ( AOIDataCollect.OperateLevelOnlineUnlockAlarm(false) == false ) 
			{	AOIDataCollect.LockScreenFunc(GetSafeHwnd()); }
			else
			{
				uInt = AOIDataCollect.GetMainViewWndID();
				if ( AOIDataCollect.CheckOnlineFormViewID(uInt) == true )				
				{	AOIDataCollect.UserLogout(); }			
			}
	}
	#endif//OFFLINE_VERSION
		break;
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{		
		case WPARAM_INITIAL_MAIN_FRAME:
			InitialMainFrame();			
			break;
		case WPARAM_RESET_MSG_FROM_ID:
			AOIDataCollect.ResetWndMessageFormID();
			break;
		case WPARAM_SWITCH_TO_ONLINE_VIEW:
			OnViewOnlineFormView();
			break;
		case WPARAM_SWITCH_TO_EDIT_MAIN_VIEW:
			OnViewEditMainView();
			break;
		case WPARAM_SWITCH_TO_EDIT_MODEL_VIEW:
			OnViewEditModelView();
			break;	
		case WPARAM_SWITCH_TO_EDIT_FD_VIEW:
			OnViewEditFdView();
			break;
		case WPARAM_SWITCH_TO_EDIT_SB_VIEW:
			break;
		case WPARAM_RESET_CALLBACK_HWND:
			if ( TRUE == lParam )
			{	ResetMainViewWndAndGrabImage(true, true); }
			else
			{	ResetMainViewWndAndGrabImage(false, true); }
			break;
		case WPARAM_PROJECT_SWITCH:
			UpdateDocumentTitle();
			break;
		case WPARAM_SWITCH_USER:
			AOIDataCollect.SwitchUser();
			UpdateDocumentTitle();
			break;
		case WPARAM_UPDATE_DOCUMENT_TITLE:
			UpdateDocumentTitle();
			break;
		case WPARAM_UPDATE_RIBBON_UI_TEXT:
			UpdateRibbonUIText(lParam);
			break;			
		case WPARAM_EXEC_FD_CONFIRM_WND:
			ExecFdConfirmWnd(lParam);
			break;
		case WPARAM_EXEC_BARCODE_CONFIRM_WND:
			ExecBarcodeConfirmWnd(lParam);
			break;
		case WPARAM_EXEC_COMPONENT_BARCODE_CONFIRM_WND:
			ExecComponentBarcodeConfirmWnd(lParam);
			break;
		case WPARAM_EXEC_BARCODE_HANDHELD_WND:
			ExecBarcodeHandHeldWnd(lParam);
			break;
		case WPARAM_EXEC_OPEN_PROJECT_BARCODE_WND:
			ExecOpenProjectBarcodeWnd(lParam);
			break;
		case WPARAM_EXEC_USER_LOGIN_WND:
			ExecUserLoginWnd(lParam);
			break;
		case WPARAM_EXEC_SHOW_MESSAGE_WND://執行顯示訊息視窗
			ExecShowMessageWnd(lParam);
			break;
		case WPARAM_MES_SET_SYSTEM_PARAM:
			ExecMESComm_SetSystemParam();			
			break;
		case WPARAM_MES_SET_PROJECT_PARAM:
			ExecMESComm_SetProjectParam();			
			break;
		case WPARAM_MES_SET_USER_LOGIN_OUT:
			ExecMESComm_SetUserLogin_out((bool)(lParam));			
			break;
		case WPARAM_MES_CMD_OPEN_PROJECT:
			ExecMESComm_LoadProject();
			break;
		case WPARAM_MES_CMD_SHOW_MESSAGE:
			ExecMESComm_ShowMessageWnd(lParam);
			break;
		case WPARAM_MES_CMD_ONLINE_RUN:
			OnOnlineRun();
			break;
		case WPARAM_MES_CMD_ONLINE_BYPASS:
			OnOnlineBypass();
			break;		
		case WPARAM_MES_CMD_ONLINE_STOP:
			OnOnlineStop();
			break;
		}
		hWnd = ProjectGroupConfigWnd.GetSafeHwnd() ;
		if ( NULL != hWnd )
		{
			if ( ProjectGroupConfigWnd.IsWindowVisible() == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam);	 }
		}
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC			
		hWnd = m_wndEditComponentList.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		hWnd = m_wndEditModelList.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndPartNumberList.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditMarkList.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		hWnd = m_wndEditResultList.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditImage.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		hWnd = m_wndEditLibrary.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditProjectMap.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}

		pWnd = CMainFrame::GetActiveView();
		if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
		{	::SendMessage(pWnd->GetSafeHwnd(), message, wParam, lParam);	}
		
		if ( WPARAM_SHOW_PART_LIST_DOCK_PANE == wParam )
		{	this->ShowPane(&m_wndEditComponentList, (BOOL)lParam, FALSE, TRUE);	}

		if ( WPARAM_SHOW_RESULT_LIST_DOCK_PANE == wParam )
		{	this->ShowPane(&m_wndEditResultList, (BOOL)lParam, FALSE, TRUE);	}

		if ( WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE == wParam )
		{	
			//if ( m_wndEditLibrary.IsWindowVisible() == TRUE )
			//{	m_wndEditLibrary.ShowPane(FALSE, FALSE, FALSE);	}
			//else
			//{	m_wndEditLibrary.ShowPane(TRUE, TRUE, TRUE);	}
			this->ShowPane(&m_wndEditLibrary, (BOOL)lParam, FALSE, (BOOL)lParam);	
		}

		if ( WPARAM_SHOW_PROJECT_MAP_DOCK_PANE == wParam )
		{	this->ShowPane(&m_wndEditProjectMap, (BOOL)lParam, FALSE, TRUE);	}

		if ( WPARAM_HIDE_GLOBAL_WND_FOR_INSPECTION == wParam )
		{	ExecHideWndForInspection();	}
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;
	case MSG_EDIT_PART_LIST_WND:
		hWnd = ProjectGroupConfigWnd.GetSafeHwnd() ;
		if ( NULL != hWnd )
		{	
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam);	 }
		}
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
		hWnd = m_wndEditComponentList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam);	}
		}
		hWnd = m_wndEditModelList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
		hWnd = m_wndPartNumberList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		hWnd = m_wndEditMarkList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		hWnd = m_wndEditResultList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}		
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;

	case MSG_EDIT_RESULT_LIST_WND:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
		hWnd = m_wndEditResultList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam);	}
		}
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;

	case MSG_EDIT_MAIN_VIEW_WND:
		pWnd = CMainFrame::GetActiveView();
		if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
		{	::SendMessage(pWnd->GetSafeHwnd(), message, wParam, lParam);	}
		break;
	
	case MSG_EDIT_WND_PROPERTY_WND:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
		hWnd = m_wndEditWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;

	case MSG_EDIT_LIBRARY_WND:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
		hWnd = m_wndEditLibrary.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;

	case MSG_EDIT_IMAGE_VIEW_WND:
	case MSG_EDIT_IMAGE_PROCESS_WND:	
	case MSG_EDIT_VIEW_3D_WND:
	case MSG_EDIT_VIEW_BLOB_WND:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		hWnd = m_wndEditImage.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;
	case MSG_RIBBON_BAR_WND:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		ExecRibbonBarMessage(message, wParam, lParam);
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;
	case MSG_INSPECTION_CALLBACK:
	#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
		hWnd = m_wndEditComponentList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam);	}
		}
		hWnd = m_wndEditModelList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		hWnd = m_wndPartNumberList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		hWnd = m_wndEditMarkList.GetSafeHwnd();
		if ( NULL != hWnd )
		{
			if ( ::IsWindowVisible(hWnd) == TRUE )
			{	::SendMessage(hWnd, message, wParam, lParam); }
		}
		hWnd = m_wndEditWnd.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditImage.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}		
		hWnd = m_wndEditLibrary.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}
		hWnd = m_wndEditProjectMap.GetSafeHwnd();
		if ( NULL != hWnd )
		{	::SendMessage(hWnd, message, wParam, lParam);	}

		pWnd = CMainFrame::GetActiveView();
		if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
		{	::SendMessage(pWnd->GetSafeHwnd(), message, wParam, lParam);	}
	#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
		break;
	
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
		break;
		/*
	case MSG_INSPECTION_CALLBACK:
		if ( wParam == WPARAM_INSPECTION_FINISH )
		{
			//CDebugWnd::ShowProjectInfo();
			//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
			//if ( CWnd::IsDlgButtonChecked(IDC_REPEAT_CHK) == TRUE )
			//{
			//	CtrlID = IDC_GRAB_COMPONENT_BTN;
			//	pWnd = this->GetDlgItem(CtrlID);
			//	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
			//	{	this->PostMessage(WM_COMMAND, MAKEWPARAM(CtrlID, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd()); }				
			//}
			//else
			//{					
			//	AOIDataCollect.SetIsLockUIWnd(false);
			//	ExecGrabImage();					
			//}
		}
		else if ( wParam == WPARAM_INSPECTION_PROJECT_TEST )
		{	
			//CDebugWnd::ShowProjectInfo();	
		}
		break;*/
	}
	return CBasicFrame::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewModelListWnd() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectMapWnd() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecRibbonCategoryChanged(WPARAM wParam, LPARAM lParam)
{	
#if FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetActiveCategory();
	if ( NULL == ActiveCategoryPtr ) { return false; }
	if ( m_CategoryPtrLast == ActiveCategoryPtr ) { return true; }

	CString   CategoryName = ActiveCategoryPtr->GetName();
	const int CategoryIndex = m_wndRibbonBar.GetCategoryIndex(ActiveCategoryPtr);
	/*
	if ( RIBBON_INDEX_ONLINE_MAIN_VIEW != CategoryIndex )
	{
		const int CategoryIndexBack = RIBBON_INDEX_ONLINE_MAIN_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtrBack = m_wndRibbonBar.GetCategory(CategoryIndexBack);
		if ( NULL != ActiveCategoryPtrBack )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtrBack);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
		return false;
	}
	*/
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));			
	const bool bForce = false;
	USER_LEVEL_MODE EditLevel = USER_LEVEL_ENGINEER;
	ONLINE_FROMVIEW_MODE OnlineFormViewMode = AOIDataCollect.GetOnlineFormViewMode();
	CMFCRibbonCategory *ActiveCategoryPtrBack = m_wndRibbonBar.GetCategory(RIBBON_INDEX_ONLINE_MAIN_VIEW);		
	switch ( CategoryIndex )
	{
	case 0:
		break;
	case RIBBON_INDEX_ONLINE_MAIN_VIEW:		
		AOIDataCollect.UserLogout();
		AOIDataCollect.SetOnlineTaskCancel(false);		
		if ( ONLINE_FROMVIEW_DUAL == OnlineFormViewMode)
		{	SwitchViewWnd(IDD_ONLINE_FORMVIEW_DUAL); }
		else
		{	SwitchViewWnd(IDD_ONLINE_FORMVIEW); }		
		break;
	case RIBBON_INDEX_EDIT_MODEL_VIEW:		
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);			
			return false;
		}
		SwitchViewWnd(IDD_EDIT_MODEL_VIEW);		
		break;
	case RIBBON_INDEX_EDIT_MAIN_VIEW:
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
			return false;
		}
		SwitchViewWnd(IDD_EDIT_MAIN_VIEW);
		break;
	case RIBBON_INDEX_EDIT_BARCODE_VIEW:
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
			return false;
		}
		SwitchViewWnd(IDD_EDIT_BARCODE_VIEW);
		break;
	case RIBBON_INDEX_EDIT_FD_VIEW:
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
			return 0;
		}
		SwitchViewWnd(IDD_EDIT_FD_VIEW);
		break;	
	case RIBBON_INDEX_EDIT_MARK_VIEW:
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
			return false;
		}
		SwitchViewWnd(IDD_EDIT_MARK_VIEW);
		break;
	case RIBBON_INDEX_EDIT_SYSTEM_VIEW:
		if ( AOIDataCollect.UserLogin(EditLevel, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);			
			return false;
		}		
		break;
	case RIBBON_INDEX_EDIT_DEBUG_VIEW:
		if ( AOIDataCollect.UserLogin(USER_LEVEL_JET_FAE, bForce) == false )
		{
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);			
			return false;
		}
		SwitchViewWnd(IDD_DEBUG_FORMVIEW);
		break;	
	}	
	m_CategoryPtrLast = ActiveCategoryPtr;	
	AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);	
#endif//FRAME_STYLE_TYPE	
	return true;	
}
//-------------------------------------------------------------------------------------//
LRESULT CMainFrame::OnRibbonCategoryChanged(WPARAM wParam, LPARAM lParam)
{
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode();
	if ( TASK_NONE == TaskMode )
	{
		if ( CameraCtrl.GetBatchGrabbing() )
		{	
			CString str;
			str = _T("Wait for Camera Grabbing");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
			return 0;
		}
	}

	const bool bLocalState = CheckMesCtrlState_Local();
	const bool bRemoteState = CheckMesCtrlState_Remote();	
	if ( true==bRemoteState || true==bLocalState )
	{
		m_wndRibbonBar.SetActiveCategory(m_CategoryPtrLast);
		return 0; 
	}
	ExecRibbonCategoryChanged(wParam, lParam);
	CWnd::PostMessage(MSG_RIBBON_BAR_WND, WPARAM_UPDATE_PCB_DIRECTION, NULL);
	CWnd::PostMessage(MSG_RIBBON_BAR_WND, WPARAM_SHOW_DEBUG_CATEGORY, AOIDataCollect.GetShowRibbonBarWndCategoryDebug());
	return 0;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditMainView() 
{
	// TODO: Add your command handler code here
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));		
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	SwitchViewWnd(IDD_EDIT_MAIN_VIEW);		
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_EDIT_MAIN_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_EDIT_MAIN_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewWndDockingWnd() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewWndDockingWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewImageDockingWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectNew()//執行新專案
{
	CString str;
	bool bAutoReset = false;
	bool bChkStartLight = false;
	AOIDataCollect.DestroyModelPreViewPtr();//清除預覽模組指標
	if ( AOIDataCollect.CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	DWORD dwCopyFlag = COPY_PROJECT_FOLDER_ALL;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL != ProjectPtr ) 
	{ 
		str = _T("Do you want to save current project first ?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			TCHAR szFilters[]=_T("Project Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
			CFileDialog dialog(FALSE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters, this);
			if ( dialog.DoModal() == IDOK ) 
			{					
				CString filename = dialog.GetPathName();				
				const bool bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
				if ( ProjectPtr->SaveProject(filename, filename, dwCopyFlag, bPartialCopy) == false )
				{
					JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
					return false;
				}
				CString OpenCode = ProjectPtr->GetProjectParameter().m_ProjectOpenCode.c_str();
				AOIDataCollect.SaveProjectOpenCode(filename, OpenCode);
			}
		}
		//釋放原來專案的記憶體區塊
		ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();
		ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();	
	}		
	DWORD  Res=0;	
	bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	bool bMultiDistrictMode = AOIDataCollect.GetMultiDistrictMode();
	if ( true == bMultiDistrictMode )
	{
		str = _T("Do you want to create Multi-District project ?");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		if ( IDCANCEL == Res ) 
		{	return false; } 
		if ( IDYES == Res )
		{	bMultiDistrictMode = true; }
		else 
		{	bMultiDistrictMode = false; }
	}
	CNewProjectWizardWnd Wnd;//NewProjectWizardWnd		
	Wnd.SetNewProjectMode(NEW_PROJECT_ONLINE);
	Wnd.SetEnableMultiDistrictMode(bMultiDistrictMode);	
	if ( Wnd.DoModal() == IDCANCEL ) 
	{		
		AOIDataCollect.SetOfflineMode(OfflineMode);
		AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
		ResetMainViewWndAndGrabImage(false, true);
		return true; 
	}	
	
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	CAOIProject *NewProjectPtr = Wnd.GetProjectPtr();
	if ( NULL == NewProjectPtr ) 
	{
		AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
		return false; 
	}

	if ( NULL != ProjectPtr )
	{
		AOIDataCollect.SetActiveProjectPtr(NULL);		
		CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);	
		AOIDataCollect.SelectAllProjects(false);
		ProjectPtr->SetProjectSelected(true);		
		AOIDataCollect.DestroyProjectSelected();	
		AOIDataCollect.LayoutLaneProjectList();
		ProjectPtr = NULL;
	}	
	
	std::vector<CColorGroup> ColorGroupList;
	CProjectColorWnd ProjectColorWnd;
	AOIDataCollect.CloneSystemColorGroupSetList(0, ColorGroupList);
	NewProjectPtr->SetProjectColorGroupList(ColorGroupList);
	ProjectColorWnd.SetProjectPtr(NewProjectPtr);
	if ( ProjectColorWnd.DoModal() == IDOK )
	{	
		ProjectColorWnd.GetColorGroupList(ColorGroupList);
		NewProjectPtr->SetProjectColorGroupList(ColorGroupList);		
	}
	const bool SaveOfflineImageFiles = NewProjectPtr->GetProjectSaveOfflineImageFiles();
	
	if ( true == SaveOfflineImageFiles )
	{	AOIDataCollect.SetOfflineMode(true); }
	AOIDataCollect.SetProjectMapMode(false);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_A);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);
	AOIDataCollect.AddProjectPtr(NewProjectPtr, false);
	AOIDataCollect.SetActiveProjectIndex(0);	

	//NewProjectPtr->BuildProjectDefaultLibrary();
	NewProjectPtr->RegisterProjectOperLogFile();
	NewProjectPtr->SetProjectLaneEnable(LaneID, true);
	NewProjectPtr->WriteProjectOnlineTuningEnable(false);	
	NewProjectPtr->ApplyProjectLibraryConfiguration();
	NewProjectPtr->ApplyProjectLibraryToComponents();
	NewProjectPtr->UpdateProjectColorGroupListToModel();
	
	CString filename=NewProjectPtr->GetProjectShowName();
	LogOperCtrl.SaveLogProjectNew(NewProjectPtr, filename);
	
	const double LaneWidth = NewProjectPtr->GetProjectLaneWidth();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();		
	if ( 1==ProjectCount && MULTI_LANE_2==MultiLaneMode )
	{
		if ( PlcCtrlPtr->GetLaneAdjustCanMove(LANE_ID_A)==false )		
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LANE_ID_A, LaneWidth);	}
		if ( PlcCtrlPtr->GetLaneAdjustCanMove(LANE_ID_B)==false )		
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LANE_ID_B, LaneWidth);	}	
	}
	else
	{
		if ( PlcCtrlPtr->GetLaneAdjustCanMove(LaneID)==false )		
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LaneID, LaneWidth);	}
	}

	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_NEW, NULL);	
	
	UpdateDocumentTitle();
	SetDocumentModifiedFlag(FALSE);	

	ResetMainViewWndAndGrabImage(false, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectOpen()//執行開啟專案
{
	CAOIProject  *ProjectPtr=NULL;
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	unsigned int ProjectIndex = AOIDataCollect.GetActiveProjectIndex();		
	const size_t ProjectCounatLA = AOIDataCollect.GetLaneProjectCount_LA();
	const size_t ProjectCounatLB = AOIDataCollect.GetLaneProjectCount_LB();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();		
	if ( MULTI_LANE_2 != MultiLaneMode )
	{
		if ( -1 == ProjectIndex )
		{	ProjectIndex = 0; }	
	}
	else
	{
		switch ( LaneID )
		{
		case LANE_ID_A:
			if ( 0 == ProjectCounatLA )
			{	ProjectIndex = ProjectCount;	}
			else
			{
				ProjectPtr = AOIDataCollect.GetLaneProjectPtr_LA(0, true);
				if ( NULL == ProjectPtr )
				{	ProjectIndex = ProjectCount; }
				else
				{
					if ( ProjectPtr->GetProjectLaneEnable(LaneID) == true )
					{	ProjectIndex = ProjectPtr->GetProjectIndex(); }
					else
					{	ProjectIndex = ProjectCount; } 
				}
			}
			break;
		case LANE_ID_B:
			if ( 0 == ProjectCounatLB )
			{	ProjectIndex = ProjectCount;	}
			else
			{
				ProjectPtr = AOIDataCollect.GetLaneProjectPtr_LB(0, true);
				if ( NULL == ProjectPtr )
				{	ProjectIndex = ProjectCount; }
				else
				{
					if ( ProjectPtr->GetProjectLaneEnable(LaneID) == true )
					{	ProjectIndex = ProjectPtr->GetProjectIndex(); }
					else
					{	ProjectIndex = ProjectCount; } 
				}
			}
			break;
		}
	}

	if ( ExecProjectOpen(ProjectIndex) == false )
	{	return false;	}
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectOpen(unsigned int ProjectIndex)//執行開啟專案
{
	//return true;
	/*
	CString Folder = AOIDataCollect.GetOpenProjectDirectory();
	TCHAR szFilters[]=_T("Project Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog(TRUE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters, this);	
	dialog.m_ofn.lpstrInitialDir = Folder;
	if ( dialog.DoModal() == IDCANCEL ) 
	{	return true; }
	CString filename = dialog.GetPathName();		
	*/	
	CString         str, str2;
	if ( ProjectIndex >= MAX_PROJECT_COUNT )
	{
		str = _T("Error, Project Count is too many !!!");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s [>%d]"), str, MAX_PROJECT_COUNT);
		JetAPI::ShowMessageBox(str2);
		return false;
	}	
	DWORD Res = 0;
	CProjectListWnd ProjectListWnd;	
	USER_LEVEL_MODE UserLevelMode = AOIDataCollect.GetCurrentUserLevel();
	Res = ProjectListWnd.DoModal();
	if ( UserLevelMode < USER_LEVEL_ENGINEER ) 
	{	AOIDataCollect.UserLogout(); }
	if ( IDCANCEL == Res )
	{	return false;	}
	CString filename = ProjectListWnd.GetSelectedFilename();
	if ( AOIDataCollect.VerifyProjectFilename(filename) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return true;
	}
	if ( AOIDataCollect.CheckProjectFilenameValided(filename) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return true;
	}
	if (JetAPI::IsFileExist(filename) == false)
	{
		str = _T("Error, the file does not exist!");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s\n[%s]"), str, filename);
		JetAPI::ShowMessageBox(str2);
		return false;
	}
	if ( AOIDataCollect.ChceckProjectFileNameExist(ProjectIndex, filename) == true ) 
	{
		str = _T("Error, the project has opened!");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s\n[%s]"), str, filename);
		JetAPI::ShowMessageBox(str2);
		return false;
	}
	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);		
	//CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();	
	AOIDataCollect.CloseActiveProject();
	CAOIProject *ProjectPtr = AOIDataCollect.GetProjectPtr(ProjectIndex, true);
	if ( NULL == ProjectPtr ) 
	{ 
		ProjectPtr = AOIObjManager.CreateProjectObj();
		if ( NULL == ProjectPtr ) 
		{
			str = AOIObjManager.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false; 
		}			
		AOIDataCollect.AddProjectPtr(ProjectPtr, false);
		AOIDataCollect.SetActiveProjectIndex(ProjectIndex);		
	}
	else
	{	
		const bool bOnline = false;
		if ( AOIDataCollect.CloseProject(ProjectPtr, bOnline) == false )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
		AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
	}		
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::ExecProjectLoad Start"));
	if ( ExecProjectLoad(ProjectPtr, filename) == false )
	{	return true; }	
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::ExecProjectLoad End"));

	if ( ProjectPtr->SaveProjectSpcHeader_JSON(false) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectAutoLoad()//執行自動開啟專案
{
	const bool TuningVersion = AOIDataCollect.GetIsOfflineHostTuningVersion();
	if ( false == TuningVersion )
	{	return true; }
	
	CString    str;
	const bool bOffline=false;	
	std::vector<CString> LatestFilenameList;

	if ( AOIDataCollect.CheckLatestFilenameList(LatestFilenameList, bOffline) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	const size_t FilenameCount=LatestFilenameList.size();
	if ( 0 == FilenameCount )
	{	return true; }
	
	const unsigned int ProjectIndex = 0;		
	CAOIProject *ProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ProjectPtr ) 
	{ 
		str = AOIObjManager.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	CString  Filename = LatestFilenameList[0];		
	AOIDataCollect.AddProjectPtr(ProjectPtr, false);
	AOIDataCollect.SetActiveProjectIndex(ProjectIndex);		
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::ExecProjectLoad Start"));
	if ( ExecProjectLoad(ProjectPtr, Filename) == false )
	{	return false; }
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::ExecProjectLoad End"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectLoad(CAOIProject *ProjectPtr, LPCTSTR filename)//執行開啟專案
{
	if ( NULL == ProjectPtr ) { return false; }
	
	double  Time;
	CString str;
	CString str1;
	CString Folder;	
	CString strLaneID;
	CString OfflineFile;	
	CString OfflineFolder;
	CString OfflineFiducial;
	CString tmpfilename;	
	CString FileMainName;
	CString ResultFolder;
	CString SpcFileFolder;
	CString SpcProjectFolder;	
	CString SpcResultFolder;
	LARGE_INTEGER  fnStart, fnEnd;	
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ProjectIndex = ProjectPtr->GetProjectIndex();
	const bool IsUseProjectSysParam = AOIDataCollect.GetIsUseProjectSystemParameter();
	const SAVE_SPC_FILE_MODE SaveSpcFileMode=AOIDataCollect.GetSaveProjectSpcFileMode();

	JetAPI::ExtractMainFileName(filename, Folder);	
	JetAPI::ExtractMainFileNameNoPath(filename, FileMainName);
	if ( AOIDataCollect.CheckMESComm_ProjectIsFree(filename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	if ( true == IsUseProjectSysParam )
	{
		if ( AOIDataCollect.LoadSystemParamFileFromProjectParam(Folder) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}
	}	
	AOIDataCollect.AddLatestFilename(filename);	
	JetAPI::SetFuncTimeStart(fnStart);
	if ( AOIDataCollect.CreateTempProjectFile(filename, tmpfilename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	JetAPI::SetFuncTimeStart(fnEnd);
	Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("AOIDataCollect.CreateTempProjectFile Time=%.3f ms"), Time);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	
	strLaneID = AOIDataDefine.GetLaneIDText(LaneID);
	ResultFolder = AOIDataCollect.GetAOIResultDirectory();	
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(Folder);
	OfflineFile = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DistrictID);	
	OfflineFiducial = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
	const bool bLibraryMode = false;
	JetAPI::SetFuncTimeStart(fnStart);	
	if ( ProjectPtr->LoadProject(tmpfilename, filename, bLibraryMode) == false )
	{	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	JetAPI::SetFuncTimeStart(fnEnd);
	Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("ProjectPtr.LoadProject Time=%.3f ms"), Time);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	JetAPI::SetFuncTimeStart(fnStart);	
	if ( ProjectPtr->RegisterProjectOperLogFile() == false )
	{	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	JetAPI::SetFuncTimeStart(fnEnd);
	Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("ProjectPtr.RegisterProjectOperLogFile Time=%.3f ms"), Time);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	std::wstring OpenCode;
	if ( AOIDataCollect.FindProjectOpenCode(filename, OpenCode) == true ) 
	{	ProjectPtr->GetProjectParameter().m_ProjectOpenCode = OpenCode;	}

	ExecProjectLoadFromServer(ProjectPtr);

	ProjectPtr->SetProjectSaveOfflineImageFiles(false);
	if ( JetAPI::IsFileExist(OfflineFile) == true )	
	{
		if ( ProjectPtr->LoadProjectProgramOfflineFile(OfflineFile) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
		else
		{
			if ( ProjectPtr->LoadProjectOfflineFdFile(OfflineFiducial) == false )
			{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
			else
			{
				ProjectPtr->SetProjectSaveOfflineImageFiles(true);
				ProjectPtr->SaveProjectOfflineFdFileToInspection(OfflineFiducial);	
			}
		}
	}

	if ( ProjectPtr->BuildProjectImageConfig() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}

	const size_t ModelCount = ProjectPtr->GetProjectModelCount();
	if ( 0 == ModelCount )
	{	ProjectPtr->BuildProjectDefaultLibrary(); }

	ProjectPtr->CheckProjectOfflineSaveFinish();	
	const bool bOnlineTuningEnable = ProjectPtr->GetProjectOnlineTuningEnableUI();
#ifdef OFFLINE_VERSION
	ProjectPtr->WriteProjectOnlineTuningEnable(bOnlineTuningEnable);
#else
	ProjectPtr->WriteProjectOnlineTuningEnable(false);
#endif//OFFLINE_VERSION	

	SpcProjectFolder.Format(_T("%s\\%s"), ResultFolder, FileMainName);
	::CreateDirectory(SpcProjectFolder, NULL);	::Sleep(0);	
	SpcFileFolder.Format(_T("%s\\%s"), SpcProjectFolder, _T("Spc"));
	::CreateDirectory(SpcFileFolder, NULL);	::Sleep(0);	
	SpcResultFolder = AOIDataDefine.GetProjectSpcResultFolder(SpcProjectFolder, SaveSpcFileMode);
	::CreateDirectory(SpcResultFolder, NULL);	::Sleep(0);			

	ProjectPtr->SetProjectActLaneID(LaneID);
	double PosX=0, PosY=0, PosZ=0;
	const double FocusPosZ = ProjectPtr->GetProjectFocusPos();
	ProjectPtr->SetProjectSpcFolder(SpcProjectFolder);	
	ProjectPtr->SetProjectSpcFileFolder(SpcFileFolder);
	ProjectPtr->SetProjectSpcImageFolder(SpcProjectFolder);
	ProjectPtr->SetProjectSpcResultFolder(SpcResultFolder);	
	ProjectPtr->SetProjectActLaneID(LaneID);
	ProjectPtr->SetProjectLaneEnable(LaneID, true);			
	ProjectPtr->SetProjectOnlineTuningEnable(false);
	if ( AOIDataCollect.ExecMESComm_ProcessID(MES_STATAUS_AOI_PROJECT_OPEN) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }

	AOIDataCollect.SetAutoRetryCount(0);
	AOIDataCollect.LayoutLaneProjectList();
	AOIDataCollect.SendToVRS_ProjectList();
	AOIDataCollect.SetProjectMapMode(false);
	AOIDataCollect.SetOnlineTuningEnable(false);
	AOIDataCollect.SetSaveProjectRawImage(false);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetIsNeedGrabFiducial(true, LaneID);
	AOIDataCollect.ClearCCSDateTime(LaneID);
	AOIDataCollect.ClearRepairDateTimeToCheck(LaneID);
	AOIDataCollect.SetOfflineFileName(OfflineFile);		
	AOIDataCollect.UpdateSystemBasePlaneParamToProject();
	AOIDataCollect.UpdateSystemNoiseFilterParamToProject();	
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);	
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_A);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);	
	AOIDataCollect.SaveAOIMonitorStatus(ONLINE_STATE_INSPECTION_STOP, NULL);
	AOIDataCollect.ApplyProjectParameterToSystem(ProjectPtr);	
	LogOperCtrl.SaveLogProjectOpen(ProjectPtr, OfflineFile);
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )	
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}		
	if ( AOIDataCollect.ExecMESComm_SetProjectParam() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
#ifdef OFFLINE_VERSION
	#ifdef _X64
		MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
		if ( AOIDataCollect.CheckSystemReady(true, false) == true )
		{
			CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
			if ( NULL != PanelPtr )
			{
				TREGION4D Region = PanelPtr->GetPanelRgnStage(DistrictID);
				PosX = Region.GetCpX();
				PosY = Region.GetCpY();
			}		
		}
		MotionCtrlPtr->XYZMoveTo(PosX, PosY, FocusPosZ);
	#endif//_X64
#else
	JetAPI::ClearFolder(SpcResultFolder);
	MotionCtrlPtr->MoveTo(AXIS_Z, FocusPosZ, MOTION_MOVING_NORMAL);
#endif//OFFLINE_VERSION
	const double LaneWidth = ProjectPtr->GetProjectLaneWidth();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();		
	if ( 1==ProjectCount && MULTI_LANE_2==MultiLaneMode )
	{			
		CString      strLaneID_LA = AOIDataDefine.GetLaneIDText(LANE_ID_A);
		CString      strLaneID_LB = AOIDataDefine.GetLaneIDText(LANE_ID_B);
		const bool   PCBInside_LA = PlcCtrlPtr->CheckLaneAdjustPCBInside(LANE_ID_A);
		const bool   PCBInside_LB = PlcCtrlPtr->CheckLaneAdjustPCBInside(LANE_ID_B);
		const double CurLaneWidth_LA = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LANE_ID_A);
		const double CurLaneWidth_LB = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LANE_ID_B);
		const double LaneWidthDif_LA = ::fabs(CurLaneWidth_LA-LaneWidth);
		const double LaneWidthDif_LB = ::fabs(CurLaneWidth_LB-LaneWidth);
		if ( LaneWidthDif_LA<0.01 || true==PCBInside_LA || PlcCtrlPtr->GetLaneAdjustDisableBtn_LA()==true )
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LANE_ID_A, LaneWidth); }
		else
		{			
			str = _T("Do you want to adjust lane width");
			str = LoadMultiLanguageString(str, str);		
			str1.Format(_T("%s [%s::%.2f mm]?"), str, strLaneID_LA, LaneWidth);
			if ( JetAPI::ShowMessageBox(str1, MB_YESNO) == IDYES )
			{
				if ( AOIDataCollect.ExecLaneAdjustWidth(LANE_ID_A, LaneWidth) == false )
				{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
			}			
		}
		if ( LaneWidthDif_LB<0.01 || true==PCBInside_LB || PlcCtrlPtr->GetLaneAdjustDisableBtn_LB()==true )
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LANE_ID_B, LaneWidth); }
		else
		{			
			str = _T("Do you want to adjust lane width");
			str = LoadMultiLanguageString(str, str);		
			str1.Format(_T("%s [%s::%.2f mm]?"), str, strLaneID_LB, LaneWidth);
			if ( JetAPI::ShowMessageBox(str1, MB_YESNO) == IDYES )
			{
				if ( AOIDataCollect.ExecLaneAdjustWidth(LANE_ID_B, LaneWidth) == false )
				{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
			}			
		}
	}
	else
	{		
		const bool   PCBInside = PlcCtrlPtr->CheckLaneAdjustPCBInside(LaneID);
		const double CurLaneWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);		
		const double LaneWidthDif = ::fabs(CurLaneWidth-LaneWidth);		
		if ( LaneWidthDif<0.01 || true==PCBInside || PlcCtrlPtr->GetLaneAdjustDisableBtn(LaneID)==true )		
		{	PlcCtrlPtr->SetLaneAdjustCurrentPos(LaneID, LaneWidth);	}
		else
		{
			str = _T("Do you want to adjust lane width");
			str = LoadMultiLanguageString(str, str);		
			str1.Format(_T("%s [%s::%.2f mm]?"), str, strLaneID, LaneWidth);
			if ( JetAPI::ShowMessageBox(str1, MB_YESNO) == IDYES )
			{
				if ( AOIDataCollect.ExecLaneAdjustWidth(LaneID, LaneWidth) == false )
				{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
			}
		}		
	}
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);	
	
	UpdateDocumentTitle();	
	SetDocumentModifiedFlag(FALSE);	

	if ( ExecProjectPreLoadOffline(ProjectPtr) == false )
	{	return false; }
	
	AOIDataCollect.PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, (LPARAM)(this));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectPreLoadOffline(CAOIProject *ProjectPtr)//執行預先載入專案離線編輯		
{
#ifndef OFFLINE_VERSION
	return true;
#endif//OFFLINE_VERSION

	CString str;	
	DWORD   res=0;
	if ( AOIDataCollect.GetPreLoadProjectProgramImage() == false ) { return true; }
	
#ifdef _DEBUG	
	str.Format(_T("Do you want to pre load all program images?"));
	str = LoadMultiLanguageString(str, str);
	res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( IDNO == res ) { return TRUE; }
#endif//_DEBUG

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.ReleaseFieldUniFrameList();
	if ( ProjectPtr->ExecProjectPreLoadProgramImage() == false )
	{
		::SetCursor(hOldCursor);	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	::SetCursor(hOldCursor);

	//str.Format(_T("Preload all offline images Finish"));
	//res = JetAPI::ShowMessageBox(str);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectLoadFromServer(CAOIProject *ProjectPtr)//執行載入伺服器專案
{
	CString  str;
	const bool bOnline = false;
	if ( AOIDataCollect.ExecProjectLoadFromServer(ProjectPtr, bOnline) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);	
		return false;
	}
	return true;

	const bool bForce=false;
	if ( NULL == ProjectPtr ) { return false; }
	PROJECT_LINK_SERVER_MODE ProjectLinkServerMode=ProjectPtr->GetProjectParameter().m_ProjectLinkServerMode;
	if ( PROJECT_LINK_SERVER_DISABLE == ProjectLinkServerMode )
	{	return true; }

	
	CString  ErrorString;
	bool    bLockFile=false;		
	bool    bNewProject=false;
	bool    bNewLibrary=false;
	bool    bServerLibraryEnabled = AOIDataCollect.CheckServerLibraryEnabled(ProjectLinkServerMode);
	//確認伺服器相同專案下的共享檔案時間	
	ProjectPtr->CheckProjectServerProjectShareFileSavedDateTime(bNewProject);
	if ( true == bServerLibraryEnabled  )//使用外部資料庫	
	{			
		ProjectPtr->CheckProjectServerShareFileSavedDateTime(bNewLibrary);  
		if ( true == bNewLibrary )
		{	bNewProject = false;	}
	}	
	if ( false==bNewLibrary && false==bNewProject )
	{	return true; }
	if ( ProjectPtr->CreateProjectServerLockFile(bLockFile) == false )
	{
		ErrorString = ProjectPtr->GetErrorString();		
		JetAPI::ShowMessageBox(ErrorString);	
		return false; 
	}
	if ( true == bLockFile )
	{	return true; }

	const bool bQuery = AOIDataCollect.CheckServerLibraryQuery(ProjectLinkServerMode);
	if ( true == bQuery  )
	{
		str = _T("Do you want to load the project from server?");
		str = ProjectPtr->LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{
			ProjectPtr->DeleteProjectServerLockFile();
			return true; 
		}	
	}

	CString ProjectFoler = AOIDataCollect.GetAOIServerProjectFolder();
	CString LibraryFoler = AOIDataCollect.GetAOIServerLibraryFolder();	
	CreateDirectory(ProjectFoler, NULL);
	CreateDirectory(LibraryFoler, NULL);	
	::Sleep(0);

	if ( true == bNewProject )
	{
		if ( ProjectPtr->LoadProjectFileFromServerProject(ProjectFoler, LibraryFoler) == false )
		{
			ProjectPtr->DeleteProjectServerLockFile();
			ErrorString = ProjectPtr->GetErrorString();		
			JetAPI::ShowMessageBox(ErrorString);
			return false;
		}
	}
	else if ( true == bNewLibrary )
	{
		const bool bLoadAll = true;
		const bool bIncNewModel = true;		
		if ( ProjectPtr->LoadProjectLibryFromServerLibrary(LibraryFoler, bIncNewModel, bLoadAll) == false )
		{
			ProjectPtr->DeleteProjectServerLockFile();
			ErrorString = ProjectPtr->GetErrorString();		
			JetAPI::ShowMessageBox(ErrorString);
			return false;
		}
	}
	ProjectPtr->DeleteProjectServerLockFile();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveLibraryToServer(CAOIProject *ProjectPtr)//執行專案資料庫儲存至伺服器
{	
	if ( NULL == ProjectPtr ) { return false; }

	if ( ProjectPtr->SaveProjectToServerProject() == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectClose()//執行關閉專案
{
	CString str;
	UpdateDocumentTitle();
	str = _T("Do you want to close the current project?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return false; }
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);

	LARGE_INTEGER fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	AOIDataCollect.DestroyActiveProject();	
	JetAPI::SetFuncTimeEnd(fnEnd);
	double Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Close Project Time=%.3f ms"), Time);
	AOIDataCollect.SaveMovingTimeMsg(str);
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectCloseAll(bool bAsk)//執行關閉所有專案
{
	CString str;
	if ( true == bAsk )
	{
		str = _T("Do you want to close all projects?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
		{	return false; }
	}
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);
	LARGE_INTEGER fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	AOIDataCollect.DestroyAllProject();
	JetAPI::SetFuncTimeEnd(fnEnd);
	double Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Close All Project Time=%.3f ms"), Time);
	AOIDataCollect.SaveMovingTimeMsg(str);
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateDocumentTitle()//更新主視窗的標題
{
	CString      str;
	CString      User;
	CString      Title;
	CString      filename;	
	CDocument   *pDoc = GetActiveDocument();
	CString      UserName = AOIDataCollect.GetCurrentUserName();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();	
	if ( NULL == pDoc ) { return false; }		

	if ( NULL == ProjectPtr )
	{	Title = m_DefaultDocTitle; }
	else
	{
		filename = ProjectPtr->GetProjectShowName();
		JetAPI::ExtractMainFileName(filename, Title);	

		TVersionCode *VersionCode = ProjectPtr->GetProjectVersionCodeActivePtr();
		if ( NULL != VersionCode )
		{			
			CString ProjectName=Title;
			CString VersionName=VersionCode->wsCodeName.c_str();
			CString strVersion=AOIDataDefine.GetVersionCodeText();			
			Title.Format(_T("%s (%s:%s)"), ProjectName, strVersion, VersionName);
		}
	}
	
	USER_LEVEL_MODE UserLevel=AOIDataCollect.GetCurrentUserLevel();
	User = AOIDataDefine.GetUserLevelModeText(UserLevel);	
	//str = _T("User");
	//User = LoadMultiLanguageString(str, str);
	
	str.Format(_T("%s    [%s:%s]"), Title, User, UserName);
	pDoc->SetTitle(str); 	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateRibbonUIText(LPARAM lParam)//更新主視窗的Ribbon調機介面
{
	bool IsOK = false;
	switch ( lParam )
	{
	case LPARAM_UPDATE_RIBBON_TUNE_GROUP_TEXT:
		IsOK = UpdateRibbonTuneGroupText();
		break;
	case LPARAM_UPDATE_RIBBON_DEFAULT_WND_GROUP_TEXT:
		IsOK = UpdateRibbonDefaultWndGroupText();
		break;
	}
	if ( true == IsOK )
	{	m_wndRibbonBar.ForceRecalcLayout();	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateRibbonTuneGroupText()//更新主視窗的Ribbon調機介面
{
	CString TuneGroupText = AOIDataCollect.GetRibbonTuneGroupCmdText();
	UpdateRibbonElementText(ID_TUNE_INSPECTION_GROUP, TuneGroupText, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::UpdateRibbonDefaultWndGroupText()//更新主視窗的Ribbon預設檢測框介面			
{	
	CString GroupText = AOIDataCollect.GetRibbonDefaultWndCmdText();
	UpdateRibbonElementText(MENU_MODEL_EDIT_ADD_ALL_WND_GROUP, GroupText, true);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSave()//執行儲存專案
{		
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	if ( PROJECT_TASK_OPEN_BARCODE == ProjectTaskMode )
	{
		if ( AOIDataCollect.SaveOpenProjectBarcodeFile() == false )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}		
		return false;	
	}
	
	bool          IsOK=true;
	double        fnTime;
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;
	CString       str;	
	CString       filenameTmp;	
	CString       FileMainName;	
	DWORD         dwCopyFlag = COPY_PROJECT_FOLDER_NO_OFFLINE;
	CString       filename = ProjectPtr->GetProjectFileName();
	CString       filename_show = ProjectPtr->GetProjectShowName();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ProjectIndex = ProjectPtr->GetProjectIndex();
	const bool    bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
	if ( filename.GetLength() == 0 ) { return FALSE; }	

	LogOperCtrl.SaveLogProjectSave(ProjectPtr, filename_show);

	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = ProjectPtr->SaveProject(filename, filename_show, dwCopyFlag, bPartialCopy);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("CProjectPtr::SaveProject Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( false == IsOK )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}	
	AOIDataCollect.AddLatestFilename(filename_show);
	JetAPI::ExtractFileNameNoPath(filename_show, FileMainName);	
	CString OpenCode = ProjectPtr->GetProjectParameter().m_ProjectOpenCode.c_str();
	AOIDataCollect.SaveProjectOpenCode(filename_show, OpenCode);

	dwCopyFlag = COPY_PROJECT_FOLDER_ALL;
	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = ProjectPtr->CopyProjectFileToOtherFile(filename_show, dwCopyFlag);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("CProjectPtr::CopyProjectFileToOtherFile Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( false == IsOK )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	//避免蓋掉原來的Offline參數檔案，因此後面重新寫上	
	CString fileFolderShow;	
	CString OfflineFolderShow;
	CString OfflineFdNameShow;
	CString OfflineFileNameShow;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	JetAPI::ExtractMainFileName(filename_show, fileFolderShow);//新的專案的資料夾	
	OfflineFolderShow = AOIDataDefine.GetProjectOfflineFolderName(fileFolderShow);	
	OfflineFdNameShow = AOIDataDefine.GetProjectOfflineFdName(OfflineFolderShow, DistrictID);
	OfflineFileNameShow = AOIDataDefine.GetProjectOfflineFileName(OfflineFolderShow, DistrictID);	
	ProjectPtr->AssignProjectProgramOfflineFolder(OfflineFolderShow);
	if ( ProjectPtr->GetProjectFdModified() == true )	
	{
		if ( true == OfflineMode )//一般時不可以儲存, 會以另外一個板子狀態的定位點儲存
		{	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdNameShow); }
		//ProjectPtr->SaveProjectProgramOfflineFile(OfflineFileNameShow);
		ProjectPtr->SetProjectFdModified(false);
	}	
	if ( ProjectPtr->WriteProjectOfflineSaveFinish() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	

	const bool OnlineTuningEnable = ProjectPtr->GetProjectOnlineTuningEnableUI();
	ProjectPtr->WriteProjectOnlineTuningEnable(OnlineTuningEnable);
	AOIDataCollect.SendToVRS_ProjectList();		

	ExecProjectSaveLibraryToServer(ProjectPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveAs()//執行另存專案
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(ComponentPtr);	

	CString ShowName = ProjectPtr->GetProjectShowName();
	TCHAR szFilename[MAX_JET_PATH]=_T("");
	TCHAR szFilters[]=_T("PRG Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters);
	::_tcscpy(szFilename, ShowName);
	dialog.m_ofn.lpstrFile  = szFilename;
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	DWORD   res=0;
	CString str, str1;
	CString filename = dialog.GetPathName();
	if ( AOIDataCollect.CheckProjectFilenameValided(filename) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return true;
	}

	if ( JetAPI::IsFileExist(filename) == true )
	{		
		str1 = _T("File Exist, do you want to over write it?");
		str1 = LoadMultiLanguageString(str1, str1);
		str.Format(_T("%s\n%s"), str1, filename);
		res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if ( res == IDNO ) { return true; }
	}
	if ( AOIDataCollect.CheckMESComm_ProjectIsFree(filename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}

	bool    theSame = false;
	DWORD   dwCopyFlag = COPY_PROJECT_FOLDER_NO_OFFLINE;		
	bool    IsOK=true;
	double  fnTime;
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	CString folderNew;
	CString folderOrgTmp;
	CString FileMainName;
	CString FileMainNameNew;
	CString SpcFileFolder;
	CString SpcProjectFolder;	
	CString SpcResultFolder;		
	CString FileMainNameNewTmp;
	CString OfflineFolderNew;
	CString OfflineFdNameNew;	
	CString OfflineFileNameNew;	
	CString SysTempProjectFolder;
	const bool bPartialCopy = false;
	CString filenameNew = filename;//新的專案檔名
	CString ResultFolder = AOIDataCollect.GetAOIResultDirectory();
	CString SysTempFolder = AOIDataCollect.GetAOITempDirectory();	
	CString filenameOrg = ProjectPtr->GetProjectShowName();
	CString filenameOrgTmp = ProjectPtr->GetProjectFileName();//原來專案的暫存檔名
	CString filenameNewTmp = filenameOrgTmp;//新的專案暫存檔名		
	CString InspectionOfflineFolderOrg = ProjectPtr->GetProjectInspectionOfflineFolder();
	CString InspectionOfflineFolderNew = InspectionOfflineFolderOrg;
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const unsigned int ProjectIndex = ProjectPtr->GetProjectIndex();	
	const SAVE_SPC_FILE_MODE SaveSpcFileMode=AOIDataCollect.GetSaveProjectSpcFileMode();

	JetAPI::ExtractMainFileName(filenameNew, folderNew);//新的專案的資料夾
	JetAPI::ExtractMainFileName(filenameOrgTmp, folderOrgTmp);//原來專案的暫存資料夾
	JetAPI::ExtractMainFileNameNoPath(filenameNew, FileMainNameNew);	
	
	SpcProjectFolder.Format(_T("%s\\%s"), ResultFolder, FileMainNameNew);
	::CreateDirectory(SpcProjectFolder, NULL);	::Sleep(0);	
	SpcFileFolder.Format(_T("%s\\%s"), SpcProjectFolder, _T("Spc"));
	::CreateDirectory(SpcFileFolder, NULL);	::Sleep(0);	
	SpcResultFolder = AOIDataDefine.GetProjectSpcResultFolder(SpcProjectFolder, SaveSpcFileMode);
	::CreateDirectory(SpcResultFolder, NULL);	::Sleep(0);			

	if ( filenameNew.CompareNoCase(filenameOrg) == 0 )
	{	theSame = true;	}
	else
	{	
		theSame = false; 
		AOIDataCollect.AddLatestFilename(filenameNew);
		JetAPI::ExtractFileNameNoPath(filenameNew, FileMainName);		
		filenameNewTmp = AOIDataDefine.GetProjectTempFilename(filename);//新的專案暫存檔名		
		JetAPI::ExtractMainFileNameNoPath(filenameNewTmp, FileMainNameNewTmp);		
		SysTempProjectFolder.Format(_T("%s\\%s"), SysTempFolder, FileMainNameNewTmp);
		::CreateDirectory(SysTempProjectFolder, NULL);
		InspectionOfflineFolderNew = AOIDataDefine.GetProjectOfflineFolderName(SysTempProjectFolder);
		::CreateDirectory(InspectionOfflineFolderNew, NULL);
		ProjectPtr->SetProjectProgramFieldDoNotSave(false);
	}
	
	UUID  ProjectObjUuid = ProjectPtr->GetObjUuid();
	if ( false == theSame )//檔名不同時需要變更uuid, 視為不同專案
	{	ProjectPtr->ChangeProjectObjUUID();	}

	LogOperCtrl.SaveLogProjectSave(ProjectPtr, filenameNew);

	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = ProjectPtr->SaveProject(filenameNewTmp, filenameNew, dwCopyFlag, bPartialCopy);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("CProjectPtr::SaveProject Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( false == IsOK )
	{
		ProjectPtr->SetObjUuid(ProjectObjUuid);
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	CString OpenCode = ProjectPtr->GetProjectParameter().m_ProjectOpenCode.c_str();
	AOIDataCollect.SaveProjectOpenCode(filenameNew, OpenCode);

	if ( false == theSame )
	{	dwCopyFlag = COPY_PROJECT_FOLDER_ALL;	}
	else
	{	dwCopyFlag = COPY_PROJECT_FOLDER_NO_OFFLINE;	}	
	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = ProjectPtr->CopyProjectFileToOtherFile(filenameNew, dwCopyFlag);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("CProjectPtr::CopyProjectFileToOtherFile Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( false == IsOK )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	
	//避免蓋掉原來的Offline參數檔案，因此後面重新寫上	
	OfflineFolderNew = AOIDataDefine.GetProjectOfflineFolderName(folderNew);
	OfflineFdNameNew = AOIDataDefine.GetProjectOfflineFdName(OfflineFolderNew, DistrictID);	
	OfflineFileNameNew = AOIDataDefine.GetProjectOfflineFileName(OfflineFolderNew, DistrictID);

	ProjectPtr->AssignProjectProgramOfflineFolder(OfflineFolderNew);
	ProjectPtr->SetProjectInspectionOfflineFolder(InspectionOfflineFolderNew);
	ProjectPtr->SetProjectInspectionOfflineFolderDefault(InspectionOfflineFolderNew);
	if ( ProjectPtr->GetProjectFdModified() == true )	
	{
		ProjectPtr->SaveProjectOfflineFdFile(OfflineFdNameNew);//一般時不可以儲存, 會以另外一個板子狀態的定位點儲存
		//ProjectPtr->SaveProjectProgramOfflineFile(OfflineFileNameNew);
		ProjectPtr->SetProjectFdModified(false);
	}

	if ( false == theSame )
	{	ProjectPtr->RegisterProjectOperLogFile(); }
	ProjectPtr->SetProjectOnlineTuningEnable(false);
	ProjectPtr->SetProjectSpcFolder(SpcProjectFolder);	
	ProjectPtr->SetProjectSpcFileFolder(SpcFileFolder);
	ProjectPtr->SetProjectSpcImageFolder(SpcProjectFolder);
	ProjectPtr->SetProjectSpcResultFolder(SpcResultFolder);		
	AOIDataCollect.SendToVRS_ProjectList();	

	AOIDataCollect.SetOnlineTuningEnable(false);
	AOIDataCollect.SetOfflineFileName(OfflineFileNameNew);	
	AOIDataCollect.SaveAOIMonitorStatus(ONLINE_STATE_INSPECTION_STOP, NULL);	
	if ( AOIDataCollect.ExecMESComm_SetProjectParam() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}

	UpdateDocumentTitle();
	SetDocumentModifiedFlag(FALSE);	

	if ( false == theSame )
	{	
		CAOIProject::DeleteProjectTempFile(filenameOrgTmp);	
		AOIDataCollect.ClearCCSDateTime(LaneID);
		AOIDataCollect.ClearRepairDateTimeToCheck(LaneID);
	}

	ExecProjectSaveLibraryToServer(ProjectPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectNewOffline()//執行新專案離線編程
{	
	CString str;
	bool bAutoReset = false;
	bool bChkStartLight = false;
	AOIDataCollect.DestroyModelPreViewPtr();//清除預覽模組指標
	if ( AOIDataCollect.CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	DWORD dwCopyFlag = COPY_PROJECT_FOLDER_ALL;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL != ProjectPtr ) 
	{ 
		str = _T("Do you want to save current project first ?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			TCHAR szFilters[]=_T("Project Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
			CFileDialog dialog(FALSE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters, this);
			if ( dialog.DoModal() == IDOK ) 
			{					
				CString filename = dialog.GetPathName();
				const bool bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
				if ( ProjectPtr->SaveProject(filename, filename, dwCopyFlag, bPartialCopy) == false )
				{
					JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
					return false;
				}
				CString OpenCode = ProjectPtr->GetProjectParameter().m_ProjectOpenCode.c_str();
				AOIDataCollect.SaveProjectOpenCode(filename, OpenCode);
			}
		}
		//釋放原來專案的記憶體區塊
		ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();
		ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();
	}
	DWORD  Res=0;	
	bool OfflineMode = AOIDataCollect.GetOfflineMode();		
	bool bMultiDistrictMode = AOIDataCollect.GetMultiDistrictMode();
	if ( true == bMultiDistrictMode )
	{
		str = _T("Do you want to create Multi-District offline ?");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		if ( IDCANCEL == Res ) 
		{	return false; } 
		if ( IDYES == Res )
		{	bMultiDistrictMode = true; }
		else 
		{	bMultiDistrictMode = false; }
	}	
	CNewProjectWizardWnd Wnd;//NewProjectWizardWnd	
	Wnd.SetNewProjectMode(NEW_PROJECT_OFFLINE);
	Wnd.SetEnableMultiDistrictMode(bMultiDistrictMode);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{		
		AOIDataCollect.SetOfflineMode(OfflineMode);
		AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
		ResetMainViewWndAndGrabImage(false, true);
		return true; 
	}	
	
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	CAOIProject *NewProjectPtr = Wnd.GetProjectPtr();
	if ( NULL != NewProjectPtr ) 
	{	
		bool    bIsOK = true;
		CString ErrorString;		
		DWORD dwCopyFlag = COPY_PROJECT_FOLDER_ALL;
		CString Filename = NewProjectPtr->GetProjectFileName();
		CString ShowName = NewProjectPtr->GetProjectShowName();				
		const bool bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
		if ( NewProjectPtr->SaveProject(ShowName, ShowName, dwCopyFlag, bPartialCopy) == false )
		{
			bIsOK = false;
			ErrorString = NewProjectPtr->GetErrorString();
		}
		
		NewProjectPtr->SetProjectFileName(Filename);
		AOIObjManager.DestroyProjectObj(NewProjectPtr);
		NewProjectPtr = NULL;		

		if ( false == bIsOK ) 
		{	JetAPI::ShowMessageBox(ErrorString);	}
	}
	NewProjectPtr = ProjectPtr;
	AOIDataCollect.SetProjectMapMode(false);
	AOIDataCollect.SetOfflineMode(OfflineMode);
	AOIDataCollect.SetActiveProjectPtr(NewProjectPtr);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetIsNeedGrabFiducial(true, LaneID);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);	
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_A);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);
	AOIDataCollect.SetActiveProjectIndex(0);
	if ( NULL != NewProjectPtr ) 
	{
		//NewProjectPtr->BuildProjectDefaultLibrary();
		NewProjectPtr->SetProjectLaneEnable(LaneID, true);
		NewProjectPtr->WriteProjectOnlineTuningEnable(false);		
	}
	ResetMainViewWndAndGrabImage(false, true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectNewPanel()//執行新整板資料
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	
	CNewProjectWizardWnd Wnd;	
	HWND hCallbackWnd=AOIDataCollect.GetCallbackWnd();	
	const bool bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();			
	Wnd.SetProjectExist(ProjectPtr);
	Wnd.SetNewProjectMode(NEW_PROJECT_ONLINE);					
	Wnd.SetEnableMultiDistrictMode(bMultiDistrictMode);
	if ( IDOK == Wnd.DoModal() )
	{
		CAOIProject *ProjectNew=Wnd.GetProjectPtr();
		if ( ProjectNew != ProjectPtr )
		{	AOIObjManager.DestroyProjectObj(ProjectNew);	}
	}			
	AOIDataCollect.SetCallbackWnd(hCallbackWnd);
	AOIDataCollect.SetActiveProjectPtr(ProjectPtr);	
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveSpcFile()//執行儲存Spc
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	
	if ( ProjectPtr->SaveProjectSpcFile() == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}	

//	if ( ProjectPtr->SaveProjectReportWndReading_Text() == false )
//	{
//		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
//		return false;
//	}	
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveCadXYFile()//執行儲存CAD-XY
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	
	TCHAR szFilename[MAX_JET_PATH]=_T("");
	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	::_tcscpy(szFilename, _T("CadXY.TXT"));
	dialog.m_ofn.lpstrFile  = szFilename;
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	DWORD   res=0;
	CString str, str1;
	CString filename = dialog.GetPathName();
	if ( JetAPI::IsFileExist(filename) == true )
	{		
		str1 = _T("File Exist, do you want to over write it?");
		str1 = LoadMultiLanguageString(str1, str1);
		str.Format(_T("%s\n%s"), str1, filename);
		res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if ( res == IDNO ) { return true; }
	}

	if ( ProjectPtr->SaveProjectCadXYFile(filename) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveTestCoverageFile()//執行儲存檢測涵蓋率檔案
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	
	TCHAR szFilename[MAX_JET_PATH]=_T("");
	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	::_tcscpy(szFilename, _T("TestCoverage.TXT"));
	dialog.m_ofn.lpstrFile  = szFilename;
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	DWORD   res=0;
	CString str, str1;
	CString filename = dialog.GetPathName();
	if ( JetAPI::IsFileExist(filename) == true )
	{		
		str1 = _T("File Exist, do you want to over write it?");
		str1 = LoadMultiLanguageString(str1, str1);
		str.Format(_T("%s\n%s"), str1, filename);
		res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if ( res == IDNO ) { return true; }
	}

	TListNode       Node;	
	CInputListWnd   EnumWnd;
	DWORD_PTR       OldIndex=0;
	CString         strCaption, strLabel;	
	std::vector<TListNode> NodelList;
	Node.Text=_T("Normal");
	Node.Data=TEST_COVERAGE_FILE_MODE_NORMAL;
	NodelList.push_back(Node);

	Node.Text=_T("Percentage");
	Node.Data=TEST_COVERAGE_FILE_MODE_PERCENT;
	NodelList.push_back(Node);
	
	strLabel = _T("Set Unit Mode");
	strCaption = _T("Unit Mode");
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true; }	
	const int UnitMode=EnumWnd.GetSelData();
	if ( ProjectPtr->SaveProjectTestCoverageFile(filename, UnitMode) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::SetDocumentModifiedFlag(BOOL bFlag)
{
	CDocument *pDocument = this->GetActiveDocument();
	// start off with unmodified
	if ( NULL != pDocument )
	{	pDocument->SetModifiedFlag(bFlag);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSaveMappingFile()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if (NULL == ProjectPtr) { return true; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);

	TCHAR szFilename[MAX_JET_PATH] = _T("");
	TCHAR szFilters[] = _T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog(FALSE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	::_tcscpy(szFilename, _T("CadXY.TXT"));
	dialog.m_ofn.lpstrFile = szFilename;
	if (dialog.DoModal() == IDCANCEL)
	{
		return true;
	}

	DWORD   res = 0;
	CString str, str1;
	CString filename = dialog.GetPathName();
	if (JetAPI::IsFileExist(filename) == true)
	{
		str1 = _T("File Exist, do you want to over write it?");
		str1 = LoadMultiLanguageString(str1, str1);
		str.Format(_T("%s\n%s"), str1, filename);
		res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if (res == IDNO) { return true; }
	}

	if (ProjectPtr->SaveProjectMappingCadXYFile(filename) == false)
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectNew() 
{
	// TODO: Add your command handler code here
	bool bIsOK = true;
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	bIsOK = ExecProjectNew();	
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectOpen() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	if ( AOIDataCollect.OperateLevelProjectOpen() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return ;
	}
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::OnProjectOpen Start"));
	if ( ExecProjectOpen() == false )
	{
	}
	AOIDataCollect.UserLogout_Check();
	AOIDataCollect.SaveMovingTimeMsg(_T("CMainFrame::OnProjectOpen End"));

	if ( AOIDataCollect.CheckOnlineFormViewID() == false )
	{
		CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			if ( ProjectPtr->CheckProjectShowNewProjectWizardWnd() == true )
			{	CWnd::SendMessage(WM_COMMAND, ID_PROJECT_NEW_PANEL, NULL);	}
		}	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSave() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( AOIDataCollect.OperateLevelProjectSave() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return ;
	}
	SetMainFrameWndLocked(true);
	if ( ExecProjectSave() == false )
	{	
	}
	SetMainFrameWndLocked(false);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSaveAs() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( AOIDataCollect.OperateLevelProjectSave() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return ;
	}
	SetMainFrameWndLocked(true);
	if ( ExecProjectSaveAs() == false )
	{	
	}
	SetMainFrameWndLocked(false);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();
	if ( NULL==ProjectPtr || true==bLockUIWnd || PROJECT_TASK_NORMAL!=ProjectTaskMode || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectNewOffline()
{
	bool bIsOK = true;
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bIsOK = ExecProjectNewOffline();
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectNewOffline(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
#ifdef OFFLINE_VERSION
	bLockUIWnd = true;
#endif//OFFLINE_VERSION
	if ( true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectNewPanel()
{
	bool bIsOK = true;
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bIsOK = ExecProjectNewPanel();
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectNewPanel(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSaveSpcFile()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	if ( ExecProjectSaveSpcFile() == false )
	{	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSaveSpcFile(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSaveCadXYFile()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	if ( ExecProjectSaveCadXYFile() == false )
	{	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSaveCadXYFile(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSaveTestCoverageFile()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	if ( ExecProjectSaveTestCoverageFile() == false )
	{	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSaveTestCoverageFile(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSaveTestMappingFile()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if (false == ExecProjectSaveMappingFile()) {

	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSaveTestMappingFile(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewResultlistWnd() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewResultlistWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditFdView() 
{
	// TODO: Add your command handler code here
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	SwitchViewWnd(IDD_EDIT_FD_VIEW);
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_EDIT_FD_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_EDIT_FD_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewEditFdView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditMarkView()
{
	// TODO: Add your command handler code here
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	SwitchViewWnd(IDD_EDIT_MARK_VIEW);
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_EDIT_MARK_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_EDIT_MARK_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewEditMarkView(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewEditBarcodeView() 
{
	// TODO: Add your command handler code here
	//CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_CALC_CURRENT_FOV_POSITION, (LPARAM)(this));
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	SwitchViewWnd(IDD_EDIT_BARCODE_VIEW);
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_EDIT_BARCODE_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_EDIT_BARCODE_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewEditBarcodeView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonBuildProjectCombox(LPARAM lParam)//執行RibbonBar訊息-建立專案列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	pCombox->RemoveAllItems();

	size_t       i=0;
	CString      str;
	CAOIProject *ProjectPtr = NULL;
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	for ( i=0; i<ProjectCount; i++ )
	{
		ProjectPtr = AOIDataCollect.GetProjectPtr(i, false);
		if ( NULL == ProjectPtr ) { continue; }
		str.Format(_T("%d"), i+1);
		pCombox->AddItem(str, i);
	}	
	const INT_PTR ItemCount = pCombox->GetCount();
	if ( ItemCount > 0 ) 
	{
		unsigned int ProjectIndex = AOIDataCollect.GetActiveProjectIndex();		
		pCombox->SelectItem((DWORD_PTR)ProjectIndex);
	}
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonSelChangeProjectCombox(LPARAM lParam)//執行RibbonBar訊息-選取切換專案列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	
	CString   ItemText = pCombox->GetEditText();
	const int ItemIndex = pCombox->FindItem(ItemText);
	if ( ItemIndex < 0 ) { return FALSE; }
	if ( ItemIndex >= ProjectCount ) { return FALSE; }
	unsigned int ProjectIndex = (unsigned int)(pCombox->GetItemData(ItemIndex));
	ExecProjectSwitch(ProjectIndex);
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonBuildPanelCombox(LPARAM lParam)//執行RibbonBar訊息-建立整板列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	pCombox->RemoveAllItems();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }

	size_t       i=0;
	CString      str;
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		str.Format(_T("%d"), i+1);
		pCombox->AddItem(str, (DWORD_PTR)(PanelPtr));
	}

	const INT_PTR ItemCount = pCombox->GetCount();
	if ( ItemCount > 0 ) 
	{
		unsigned int PanelIndex = ProjectPtr->GetProjectRibbonPanelIndex();
		PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
		if ( NULL == PanelPtr )
		{
			PanelPtr = (CAOIPanel*)(pCombox->GetItemData(0));
			PanelIndex = PanelPtr->GetPanelIndex_Project();
			ProjectPtr->SetProjectRibbonPanelIndex(PanelIndex);
		}		
		pCombox->SelectItem((DWORD_PTR)(PanelPtr));
	}
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonSelChangePanelCombox(LPARAM lParam)//執行RibbonBar訊息-選取切換整板列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	ProjectPtr->SetProjectRibbonPanelIndex(INVALID_INDEX);	
	
	CString   ItemText = pCombox->GetEditText();
	const int ItemIndex = pCombox->FindItem(ItemText);
	if ( ItemIndex < 0 ) { return FALSE; }
	CAOIPanel   *PanelPtr = (CAOIPanel*)(pCombox->GetItemData(ItemIndex));	
	if ( NULL == PanelPtr ) { return FALSE; }
	if ( PanelPtr->IsKindOf(RUNTIME_CLASS(CAOIPanel)) == FALSE )
	{ return FALSE; }
	const unsigned int PanelIndex = PanelPtr->GetPanelIndex_Project();
	ProjectPtr->SetProjectRibbonPanelIndex(PanelIndex);	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonBuildBoardCombox(LPARAM lParam)//執行RibbonBar訊息-建立單板列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	pCombox->RemoveAllItems();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }

	size_t       i=0;
	CString      str;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	unsigned int PanelIndex = ProjectPtr->GetProjectRibbonPanelIndex();
	if ( INVALID_INDEX == PanelIndex ) 
	{	
		PanelIndex = 0; 
		ProjectPtr->SetProjectRibbonPanelIndex(PanelIndex);
	}

	PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if ( NULL == PanelPtr ) { return TRUE; }
	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	for ( i=0; i<PanelBoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }

		str.Format(_T("%d"), i+1);
		pCombox->AddItem(str, (DWORD_PTR)(BoardPtr));
	}
		
	const INT_PTR ItemCount = pCombox->GetCount();
	if ( ItemCount > 0 ) 
	{
		unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();
		BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);	
		if ( NULL == BoardPtr )
		{
			BoardPtr = (CAOIBoard*)(pCombox->GetItemData(0));
			BoardIndex = BoardPtr->GetBoardIndex_Project();
			ProjectPtr->SetProjectRibbonBoardIndex(BoardIndex);
		}		
		pCombox->SelectItem((DWORD_PTR)(BoardPtr));
	}
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonSelChangeBoardCombox(LPARAM lParam)//執行RibbonBar訊息-選取切換單板列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	ProjectPtr->SetProjectRibbonBoardIndex(INVALID_INDEX);	
	CString   ItemText = pCombox->GetEditText();
	const int ItemIndex = pCombox->FindItem(ItemText);
	if ( ItemIndex < 0 ) { return FALSE; }
	CAOIBoard   *BoardPtr = (CAOIBoard*)(pCombox->GetItemData(ItemIndex));	
	if ( NULL == BoardPtr ) { return FALSE; }
	if ( BoardPtr->IsKindOf(RUNTIME_CLASS(CAOIBoard)) == FALSE )
	{ return FALSE; }
	const unsigned int BoardIndex = BoardPtr->GetBoardIndex_Project();
	ProjectPtr->SetProjectRibbonBoardIndex(BoardIndex);	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonBuildComponentCombox(LPARAM lParam)//執行RibbonBar訊息-建立零件列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	pCombox->RemoveAllItems();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }

	size_t       i=0;
	CString      str;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	unsigned int PanelIndex = ProjectPtr->GetProjectRibbonPanelIndex();
	unsigned int BoardIndex = ProjectPtr->GetProjectRibbonBoardIndex();
	if ( INVALID_INDEX == PanelIndex ) 
	{	PanelIndex = 0; }
	PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if ( NULL == PanelPtr ) { return TRUE; }

	if ( INVALID_INDEX == BoardIndex ) 
	{	
		BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
		if ( NULL == BoardPtr ) { return TRUE; }
		BoardIndex = BoardPtr->GetBoardIndex_Project();
		ProjectPtr->SetProjectRibbonBoardIndex(BoardIndex);
	}
	BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
	if ( NULL == PanelPtr ) { return TRUE; }
	if ( BoardPtr->GetBoardPanelPtr() != PanelPtr ) { return FALSE; }	

	size_t BuildComponentCount = 0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t BoadComponentCount = BoardPtr->GetBoardComponentCount();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();	
	if ( MANIPULATE_MODEL_CALIBRATION==ManiMode || BoadComponentCount<LIST_ITEM_PATCH_ENABLE_COUNT )
	{	BuildComponentCount = BoadComponentCount;	}
	else
	{
		BuildComponentCount = MIN(10, BoadComponentCount);
		ProjectPtr->SetProjectRibbonComponentReBuild(true);		
	}
	for ( i=0; i<BuildComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }

		str = ComponentPtr->GetComponentName();
		pCombox->AddItem(str, (DWORD_PTR)(ComponentPtr));
	}
	
	const INT_PTR ItemCount = pCombox->GetCount();
	if ( ItemCount > 0 ) 
	{
		unsigned int ComponentIndex = ProjectPtr->GetProjectRibbonComponentIndex();
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
		if ( NULL == ComponentPtr )
		{
			ComponentPtr = (CAOIComponent*)(pCombox->GetItemData(0));
			ComponentIndex = ComponentPtr->GetComponentIndex_Project();
			ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);
		}		
		pCombox->SelectItem((DWORD_PTR)(ComponentPtr));
	}	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonSelChangeComponentCombox(LPARAM lParam)//執行RibbonBar訊息-選取切換零件列表
{
	const UINT CommandID = (UINT)(lParam);
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	CMFCRibbonBaseElement *pBase = m_wndRibbonBar.FindByID(CommandID);
	if ( pBase == NULL ) { return FALSE; }
	CMFCRibbonComboBox *pCombox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pBase);
	if ( pCombox == NULL ) { return FALSE; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }
	ProjectPtr->SetProjectRibbonComponentIndex(INVALID_INDEX);	
	CString   ItemText = pCombox->GetEditText();
	const int ItemIndex = pCombox->FindItem(ItemText);
	const int CurSelIdx = pCombox->GetCurSel();
	if ( ItemIndex < 0 ) 
	{ 
		pCombox->SelectItem(CurSelIdx);
		return FALSE; 
	}
	else if ( CurSelIdx != ItemIndex )
	{	pCombox->SelectItem(ItemIndex); }
	CAOIComponent   *ComponentPtr = (CAOIComponent*)(pCombox->GetItemData(ItemIndex));
	if ( NULL == ComponentPtr ) { return FALSE; }
	if ( ComponentPtr->IsKindOf(RUNTIME_CLASS(CAOIComponent)) == FALSE )
	{ return FALSE; }
	const unsigned int ComponentIndex = ComponentPtr->GetComponentIndex_Project();
	ProjectPtr->SetProjectRibbonComponentIndex(ComponentIndex);	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonShowDebugCategory(LPARAM lParam)//執行RibbonBar訊息-顯示偵錯群組
{	
#ifdef _DEBUG
	return TRUE;
#endif//_DEBUG
#ifdef ODM_BRAND_VERSION
	return TRUE;
#endif//ODM_BRAND_VERSION
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();	
	const int nShowDebugForm = AOIDataCollect.GetSystemParameter().m_ShowDebugFormView;		
	if ( CategoryCount>RIBBON_INDEX_EDIT_DEBUG_VIEW && FN_DISABLE==nShowDebugForm )
	{
		m_wndRibbonBar.ShowCategory(RIBBON_INDEX_EDIT_DEBUG_VIEW, lParam);
		m_wndRibbonBar.RecalcLayout();
	}
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecRibbonUpdatePcbDirection()//執行RibbonBar訊息-PCB流向
{		
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	int nPCBIn=0;
	int nPCBOut=0;
	int nPCBBack=0;
	int nPCBBackOut=0;
	std::vector<UINT> IDList;
	std::vector<int> IndexList;		
	const bool bRightIn = PlcCtrlPtr->GetIsPCBRightInDirection();
	if ( m_RibbonPcbDirection == bRightIn ) { return true; }
	m_RibbonPcbDirection = bRightIn;
	if ( false == bRightIn )
	{
		nPCBIn   = 11;
		nPCBOut  = 12;
		nPCBBack = 15;		
		nPCBBackOut=68;
	}
	else
	{
		nPCBIn   = 67;
		nPCBOut  = 68;
		nPCBBack = 69;		
		nPCBBackOut=12;
	}

	IndexList.push_back(nPCBIn);	IDList.push_back(ID_LANE_PCB_IN);
	IndexList.push_back(nPCBOut);	IDList.push_back(ID_LANE_PCB_OUT);
	IndexList.push_back(nPCBBack);	IDList.push_back(ID_LANE_PCB_BACK);
	IndexList.push_back(nPCBBack);	IDList.push_back(ID_LANE_PCB_IN_2ND);
	IndexList.push_back(nPCBIn);	IDList.push_back(ID_LANE_PCB_IN_3RD);
	IndexList.push_back(nPCBBackOut);	IDList.push_back(ID_LANE_PCB_BACK_OUT);

	const size_t IDCount=IDList.size();
	const size_t IndexCount=IndexList.size();
	if ( IDCount != IndexCount ) { return false; }

	size_t i=0;
	UINT nID = 0;
	int nIndex = 0;	
	const bool bAllCategories = true;
	for ( i=0; i<IDCount; i++ )
	{
		nID = IDList[i];
		nIndex = IndexList[i];
		UpdateRibbonButtonImageIndex(nID, nIndex, true, bAllCategories);
		UpdateRibbonButtonImageIndex(nID, nIndex, false, bAllCategories);
	}
	m_wndRibbonBar.RedrawWindow();	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC		
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CMainFrame::ExecRibbonBarMessage(UINT message, WPARAM wParam, LPARAM lParam)//執行RibbonBar訊息
{
	BOOL IsOK=TRUE;
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC	
	switch ( wParam )
	{
	case WPARAM_PROJECT_COMBOX_BUILD:
		IsOK = ExecRibbonBuildProjectCombox(lParam);
		break;
	case WPARAM_PROJECT_COMBOX_SEL_CHANGE:
		IsOK = ExecRibbonSelChangeProjectCombox(lParam);
		break;
	case WPARAM_PANEL_COMBOX_BUILD:
		IsOK = ExecRibbonBuildPanelCombox(lParam);
		break;
	case WPARAM_PANEL_COMBOX_SEL_CHANGE:
		IsOK = ExecRibbonSelChangePanelCombox(lParam);
		break;
	case WPARAM_BOARD_COMBOX_BUILD:
		IsOK = ExecRibbonBuildBoardCombox(lParam);
		break;
	case WPARAM_BOARD_COMBOX_SEL_CHANGE:
		IsOK = ExecRibbonSelChangeBoardCombox(lParam);
		break;
	case WPARAM_COMPONENT_COMBOX_BUILD:
		IsOK = ExecRibbonBuildComponentCombox(lParam);		
		break;
	case WPARAM_COMPONENT_COMBOX_SEL_CHANGE:
		IsOK = ExecRibbonSelChangeComponentCombox(lParam);
		break;
	case WPARAM_SHOW_DEBUG_CATEGORY:
		IsOK = ExecRibbonShowDebugCategory(lParam);
		break;
	case WPARAM_UPDATE_PCB_DIRECTION:
		IsOK = ExecRibbonUpdatePcbDirection();
		break;
	}	
#endif//FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewAllPhaseWnd()
{
	CImagePhaseAllWnd Wnd;
	if ( Wnd.DoModal() == IDCANCEL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewAllPhaseWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( AOIDataCollect.OperateLevelEditWndAllPhaseDebug() == false )
	{	return; }
	ExecViewAllPhaseWnd();
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewAllPhaseWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
#ifdef DISABLE_3D
	bLockUIWnd = true;
#endif//DISABLE_3D
	if ( true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	if ( GetMainFrameWndLocked() == true )
	{	return true; }
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOnlineRun()//執行上線檢測
{
	CString  str, str2;
	str.Format(_T("CMainFrame::ExecOnlineRun Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( AOIDataCollect.ExecMESComm_CheckMESReady() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( AOIDataCollect.ExecMESComm_SetSystemParam() == false )
	{	
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	if ( AOIDataCollect.CheckOnlineFormViewID() == false )
	{
		if ( AOIDataCollect.ExecMESComm_SetParamChanged() == false )
		{	
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}	
	}

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	UINT ViewID = AOIDataCollect.GetMainViewWndID();	
	const bool AlarmDefect = ProjectPtr->GetProjectAlarmDefect();		
	if ( true == AlarmDefect )
	{
		str.Format(_T("Error, Project Test Result NG"));
		str2 = _T("Do you want to clear the statistic records?");
		//str = LoadMultiLanguageString(str, str);
		//str2 = LoadMultiLanguageString(str2, str2);
		str = AOIDataDefine.GetInspectionResultFaultText();//檢測結果異常	
		str2 = AOIDataDefine.GetDoYouWantToClearTheStatisticRecordsText();//是否清除統計資料
		JetAPI::ShowMessageBox(str);
		if ( AOIDataCollect.OperateLevelOnlineUnlockAlarm(false) == false ) 
		{	return false; }
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO )
		{	return false;	}
		ProjectPtr->ResetProjectStatisticRecords();		
	}	

	if ( ProjectPtr->SaveProjectSpcHeader_JSON(true) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	if (AOIDataCollect.CheckXboardMappingFileViaMESCommBoardCountIsCorrect(ProjectPtr) == false) {
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}	

	ProjectPtr->SetProjectAlarmStop(false);
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(ComponentPtr);
	
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=AOIDataCollect.CheckMultiProjectTestOrderMode();
	if ( MULTI_PROJECT_TEST_ORDER_BY_TURN == MultiProjectTestOrderMode )
	{	
		if ( AOIDataCollect.ExecInspectionProjectByTurn(LaneID) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}
		ProjectPtr = AOIDataCollect.GetActiveProject();
	}
	if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B==MultiProjectTestOrderMode || MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A==MultiProjectTestOrderMode )
	{			
		if ( AOIDataCollect.ExecInspectionProjectByTurnOneCycleReset(LaneID) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}
		ProjectPtr = AOIDataCollect.GetActiveProject();
	}

	if ( FN_DISABLE != SystemParam.m_ConfirmComponentBarcodeEnabled )
	{
		if ( ProjectPtr->CheckProjectComponentBarcodeReadyToInspection() == false )
		{
			str = ProjectPtr->GetProjectErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}		
	}

	bool bShowContinue=false;
	if ( FN_DISABLE != SystemParam.m_CheckProjectFdReady )
	{
		if ( ProjectPtr->CheckProjectFdReadyToInspection() == false )
		{
			bShowContinue = true;
			str = ProjectPtr->GetProjectErrorString();
			JetAPI::ShowMessageBox(str);
		}	
	}
	if ( ProjectPtr->CheckProjectComponentReadyToInspection() == false )	
	{
		bShowContinue = true;
		str = ProjectPtr->GetProjectErrorString();
		JetAPI::ShowMessageBox(str);
	}
	if ( true == bShowContinue )
	{
		str = _T("Do you want to inspect the project?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return false; }
	}

	if (false == AOIDataCollect.ExecHASI_SaveStateFile()) {
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if (false == AOIDataCollect.ExecHASI_SaveAOIJobFile(LaneID)) {
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	AOIDataCollect.CreateAOITempDirectory();
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.ReleaseFieldUniFrameList();
	AOIDataCollect.CheckExternalCopyFileEnabed();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);

	CView *MainViewPtr = CMainFrame::GetActiveView();
	if ( NULL == MainViewPtr ) { return false; }
	HWND hWnd = MainViewPtr->GetSafeHwnd();
	if ( NULL == hWnd ) { return false; }

	//確認JET硬體鎖授權
	if ( AOIDataCollect.CheckJetDongleValid() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( AOIDataCollect.CheckJetDongleWarning() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	if ( AOIDataCollect.VerifyProjectFilename() == false ) 
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( AOIDataCollect.ExecClampPcbBeforeTestFunc() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	AOIDataCollect.SetCallbackWnd(hWnd);	
	AOIDataCollect.SetInspectingMode(INSPECTING_ONLINE);
	AOIDataCollect.ApplyProjectParameterToSystem(ProjectPtr);
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	ExecHideWndForInspection();
	MainViewPtr->SendMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_INSPECTING);	
	AOIDataCollect.SetOnlineFirstRunDateTime(CTime::GetCurrentTime().GetTime());
	if ( AOIDataCollect.ExecInspectionProjectOnline() == false )
	{
		MainViewPtr->SendMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	str.Format(_T("CMainFrame::ExecOnlineRun End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineRun() 
{
	// TODO: Add your command handler code here
	//if ( AOIDataCollect.UserLogin_OnlineRun() == false )
	//{
	//	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
	//	return ;
	//}
	if ( AOIDataCollect.ExecOnlineInputProjectWorkNumber(ONLINE_INPUT_BEFORE_SIGN_IN) == false ) { return; }
	if ( AOIDataCollect.OperateLevelOnlineRun() == false ) { return ; }
	if ( ExecOnlineRun() == true ) 
	{	LockUIWnd(true);	}	
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineRun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifndef OFFLINE_VERSION
	const bool bLock = GetLockUIWnd();
	bool bRemoteState = CheckMesCtrlState_Remote();
	const bool bOfflineState = CheckMesCtrlState_Offline();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();	
	if ( true == bRemoteState )
	{
		if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_ITSSetSecsGemRemoteLocal )
		{	bRemoteState = false;	}
	}
	if ( NULL==ProjectPtr || true==bLock || true==bRemoteState || true==bOfflineState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOnlineBypass()//執行上線直通
{
	CView *MainViewPtr = CMainFrame::GetActiveView();
	if ( NULL == MainViewPtr ) { return false; }
	HWND hWnd = MainViewPtr->GetSafeHwnd();
	if ( NULL == hWnd ) { return false; }
	CString  str, str2;
	if ( AOIDataCollect.ExecMESComm_CheckMESReady() == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( AOIDataCollect.ExecMESComm_SetSystemParam() == false )
	{	
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	AOIDataCollect.SetOfflineMode(false);		
	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetRepeatedTestCount(0);
	AOIDataCollect.SetRepeatedTestMaxCount(0);
	AOIDataCollect.SetCallbackWnd(hWnd);	
	AOIDataCollect.SetInspectingMode(INSPECTING_ONLINE);
	ExecHideWndForInspection();
	MainViewPtr->SendMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_INSPECTING);	
	if ( AOIDataCollect.ExecTaskLaneBypass() == false )
	{
		MainViewPtr->SendMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineBypass() 
{
	// TODO: Add your command handler code here		
	//if ( AOIDataCollect.UserLogin_Operator() == false )
	//{
	//	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
	//	return false;
	//}
	if ( AOIDataCollect.OperateLevelOnlineBypass() == false ) { return ; }
	if ( ExecOnlineBypass() == true )
	{	LockUIWnd(true);	}		
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineBypass(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifndef OFFLINE_VERSION
	const bool bLock = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const bool bOfflineState = CheckMesCtrlState_Offline();
	const bool bMultiLaneModeOff=AOIDataCollect.CheckMultiLaneMode_Off();
	if ( true == bLock || true==bRemoteState || true==bOfflineState || true==bMultiLaneModeOff )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineStop() 
{
	// TODO: Add your command handler code here	
	if ( AOIDataCollect.OperateLevelOnlineStop() == false ) { return ; }
	TASK_STATE_MODE  TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if ( TASK_STATE_RUNNING == TaskStateMode )
	{
		if ( AOIDataCollect.CheckWndMessageFromMES() == false )
		{	AOIDataCollect.SetOnlineTaskCancel(true); }
		AOIDataCollect.SetOnlineTaskState(TASK_STATE_TO_STOP);	
	}	
	if ( TASK_STATE_TO_STOP == TaskStateMode )
	{
		if ( AOIDataCollect.CheckWndMessageFromMES() == false )
		{	AOIDataCollect.SetOnlineTaskCancel(false); }
		AOIDataCollect.SetOnlineTaskState(TASK_STATE_RUNNING);	
	}	
	AOIDataCollect.UserLogout_Check();
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	TASK_STATE_MODE  TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if ( TASK_STATE_IDLE==TaskStateMode || TASK_STATE_NONE==TaskStateMode)
	{	pCmdUI->Enable(FALSE);	}
	else
	{	
		pCmdUI->Enable(TRUE);
		if ( TASK_STATE_TO_STOP == TaskStateMode )
		{	pCmdUI->SetCheck(TRUE); }
		else
		{	pCmdUI->SetCheck(FALSE); }
	}
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecHomeAll(bool AskXYZ, bool AskLane, bool &Done)//執行歸零
{
#ifndef OFFLINE_VERSION
	CString str;
	bool    Ask=false;
	bool    IsOK=true;
	bool    Exec=false;
	bool    bAutoReset=false;
	bool    bChkStartLight=true;
	Done = false;
	if ( PlcCtrlPtr->CheckPLCReady(bChkStartLight, bAutoReset) == false )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	Ask = AskXYZ;
	if ( true == Ask )
	{
		if ( MotionCtrlPtr->CheckInit() == true )
		{
			const bool bPcbInsideLA=PlcCtrlPtr->CheckPCBInside_LA();
			const bool bPcbInsideLB=PlcCtrlPtr->CheckPCBInside_LB();
			if ( true==bPcbInsideLA || true==bPcbInsideLB )
			{	str = _T("PCB Inside, Do you want to Home XYZ?");	}
			else
			{	str = _T("Do you want to Home XYZ?");	}
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	Exec = true; }
			else
			{	Exec = false; }
		}
		else
		{	Exec = false; }
	}
	else
	{	
		Exec = true; 
		const bool bPcbInsideLA=PlcCtrlPtr->CheckPCBInside_LA();
		const bool bPcbInsideLB=PlcCtrlPtr->CheckPCBInside_LB();
		if ( true==bPcbInsideLA || true==bPcbInsideLB )
		{	
			str = _T("PCB Inside, Do you want to Home XYZ?");	
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	Exec = true; }
			else
			{	Exec = false; }
		}	
	}
	
	if ( true == Exec )
	{
		if ( MotionCtrlPtr->ExecHomeAll(true) == false )
		{
			IsOK = false;
			str = MotionCtrlPtr->GetErrorString();				
			JetAPI::ShowMessageBox(str);
			AOIDataCollect.ShowMessage(0, MSG_MB_OK, MSG_MB_ICONINFORMATION, MSG_MB_DEFBUTTON1, str, true, false);
		}
		else 
		{
			Done = true;
			AOIDataCollect.SetOnlineAutoCalibrationDateTime_XYZ_Home();
			AOIDataCollect.SaveOnlineAutoCalibrationDateTime_XYZ_Home(); 
		}
	}

	LANE_WORK_MODE LaneWorkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();
	bool LaneAdjustDisable_LA = PlcCtrlPtr->GetLaneAdjustDisableBtn_LA();
	bool LaneAdjustDisable_LB = PlcCtrlPtr->GetLaneAdjustDisableBtn_LB();
	if ( LANE_WORK_DISABLE == LaneWorkMode_LA )
	{	LaneAdjustDisable_LA = true; }
	if ( LANE_WORK_DISABLE == LaneWorkMode_LB )
	{	LaneAdjustDisable_LB = true; }
	if ( false==LaneAdjustDisable_LA || false==LaneAdjustDisable_LB )
	{
		Ask = AskLane;
		if ( true == Ask )
		{
			str = _T("Do you want to Home Lane Width?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	Exec = true; }
			else
			{	Exec = false; }
		}
		else
		{	Exec = true; }

		if ( true == Exec )
		{
			if ( AOIDataCollect.ExecLaneAdjustHome() == false )				
			{
				IsOK = false;
				str = AOIDataCollect.GetErrorString();
				JetAPI::ShowMessageBox(str);
				AOIDataCollect.ShowMessage(0, MSG_MB_OK, MSG_MB_ICONINFORMATION, MSG_MB_DEFBUTTON1, str, true, false);
			}
			else
			{	Done = true; }
		}			
	}
	PostMessageW(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	return IsOK;
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ShowHomeAllFinish()
{
#ifndef OFFLINE_VERSION
	CString str;
	str = _T("Motion Home All Finish");
	str = LoadMultiLanguageString(str, str);
	JetAPI::ShowMessageBox(str);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBIn()//執行進板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBIn2nd()//執行進板-2
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBIn2ndProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBIn3rd()//執行進板-3
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBIn3rdProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBOut()//執行出板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBBack()//執行退板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBBackOut()//執行退出板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	if ( AOIDataCollect.ExecPCBBackOutProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBClear()//執行清板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClearProc(LaneID, true, bStep) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBClampOn()//執行夾板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOnProc(LaneID) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecPCBClampOff()//執行鬆板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOffProc(LaneID) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecLaneAdjustHome()//執行軌道間距歸零
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return TRUE; }
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecLaneAdjustHome(LaneID) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}
	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecLaneAdjustWidth()//執行軌道間距寬度
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	if ( PlcCtrlPtr->CheckLaneAdjustCanMove(LaneID) == false )
	{
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return false;
	}

	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	double Pos = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
	
	strValue.Format(_T("%.3f"), Pos);
	strLabel = _T("Width");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Set Lane Adjust Width");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	
	double NewPos = JetAPI::StrToDbl(InputBox.m_DataEdit1);
	if ( AOIDataCollect.ExecLaneAdjustWidth(LaneID, NewPos) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}
	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSystemReset() 
{
	// TODO: Add your command handler code here
	LockUIWnd(false);
	CString str;
	CString filename;	
	str.Format(_T("CMainFrame::OnSystemReset Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ThreadInfoTmp.txt"));	
	if ( AOIDataCollect.SaveAllThreadState(filename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}		
	AOIDataCollect.IdleAllThread(false);
	AOIDataCollect.IdleAllProjectThread(false);
	AOIDataCollect.SetIsEnableAutoRetry(false);
	AOIDataCollect.SetAutoRetryCount(0);		
	AOIDataCollect.SetupAllLightSetting();	
	AOIDataCollect.ResetSystemException();
	AOIDataCollect.SetOnlineStateNode(_T(""), LANE_ID_A);	
	AOIDataCollect.SetOnlineStateNode(_T(""), LANE_ID_B);	
	AOIDataCollect.SetTaskMode(TASK_NONE);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	AOIDataCollect.ResetOnlineOpenProjectCameraBarcodeTestState();
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);	
	AOIDataCollect.SetOnlineStateMode_GUI(ONLINE_STATE_INSPECTION_STOP);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_A);	
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);	
	AOIDataCollect.SetOnlineStateMode_GUI_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_A);	
	AOIDataCollect.SetOnlineStateMode_GUI_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);
	AOIDataCollect.ExecMESComm_ProcessID(MES_STATAUS_AOI_ALARM_CLEAR);//執行MES溝通-程序運作

	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_NONE);
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_NONE);

	CameraCtrl.ResetBatchGrabbing();
	Light3DCtrl.ClearAllLight3DPatternList();
	AOIExceptionCodeCtrl.ResetAOIExceptionCode();

	PlcCtrlPtr->UpdateTowerLightState_Stop();
	PlcCtrlPtr->PushDownStopBtn();
	::Sleep(250);
	PlcCtrlPtr->PushDownResetBtn();
	::Sleep(1500);
	PlcCtrlPtr->PushDownStartBtn();
	::Sleep(250);
	MotionCtrlPtr->ResetMotionDriver();

	PlcCtrlPtr->SetPCBAutoRunMode(LANE_ID_A, PLC_PCB_AUTO_RUN_STOP);
	PlcCtrlPtr->SetPCBAutoRunMode(LANE_ID_B, PLC_PCB_AUTO_RUN_STOP);	

	ResetMainViewWndAndGrabImage(true, false);
	str.Format(_T("CMainFrame::OnSystemReset End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
#ifdef DEBUG
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
#endif//DEBUG	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSystemReset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	return;
	const bool bRemoteState = CheckMesCtrlState_Remote();
	if ( true==bRemoteState )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSettingOfflineMode() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	

	double PosX=0, PosY=0, PosZ=0;	
	const bool bDistrictChange = false;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool ProjectMapMode = AOIDataCollect.GetProjectMapMode();
	UINT MainViewID = AOIDataCollect.GetMainViewWndID();
	OFFLINE_FILE_MODE OldOfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();	
	OFFLINE_FILE_MODE NewOfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();	

	if ( false == OfflineMode )
	{	
		OfflineMode = true; 
		NewOfflineFileMode = OldOfflineFileMode;
	}
	else
	{	//Online
		OfflineMode = false; 
		NewOfflineFileMode = OFFLINE_FILE_INSPECTION;
	}
	/*
	if ( IDD_EDIT_MAIN_VIEW == MainViewID )
	{
		if ( true==ProjectMapMode || true==OfflineMode )
		{	
			OfflineMode = true;	
			NewOfflineFileMode = OFFLINE_FILE_PROGRAM;
		}
	}*/
	AOIDataCollect.SetOfflineMode(OfflineMode);	
	AOIDataCollect.ReleaseFieldUniFrameList();
	AOIDataCollect.ReleaseModelUniFrameList();
	if ( false==OfflineMode  )
	{	
		MotionCtrlPtr->GetCommandPos(PosX, PosY, PosZ);
		if ( ProjectPtr->GetProjectMultiDistrictMode() == true )
		{
			DISTRICT_ID DistrictID = AOIDataCollect.GetActiveDistrictID();
			if ( DistrictID != ProjectPtr->GetProjectActDistrictID() )
			{	
				DistrictID = ProjectPtr->GetProjectActDistrictID();
				AOIDataCollect.SetActiveDistrictID(DistrictID);
				if ( AOIDataCollect.MovePCBToDistrictID(LaneID, DistrictID) == false )
				{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
			}
		}
		MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ);		
		if ( NewOfflineFileMode != OldOfflineFileMode )
		{	
			if ( ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineFileMode, bDistrictChange, false) == false )
			{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
		}		
	}
	else
	{		
		//if ( NewOfflineFileMode != OldOfflineFileMode )
		//{	ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineFileMode, bDistrictChange, true);	}				
		if ( ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineFileMode, bDistrictChange, true) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
	}
	ResetMainViewWndAndGrabImage(false, true);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSettingOfflineMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	bool bLockUIWnd = GetLockUIWnd();		
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
	pCmdUI->SetCheck(AOIDataCollect.GetOfflineMode());
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSettingSaveFovImages() 
{
	// TODO: Add your command handler code here
	const bool bSaved = AOIDataCollect.GetSaveOfflineFiles();
	if ( false == bSaved )
	{	AOIDataCollect.SetSaveOfflineFiles(true);	}
	else
	{	AOIDataCollect.SetSaveOfflineFiles(false);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSettingSaveFovImages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	const bool bSaved = AOIDataCollect.GetSaveOfflineFiles();	
	pCmdUI->SetCheck(bSaved);
	ExecUpdateUI_System(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSettingOfflineInspectionMode() 
{
	// TODO: Add your command handler code here	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	bool  LoadFrame = false;	
	bool bDistrictChange = false;
	bool  OfflineMode = AOIDataCollect.GetOfflineMode();	
	OFFLINE_FILE_MODE NewOfflineMode = ProjectPtr->GetProjectOfflineFileMode();
	OFFLINE_FILE_MODE OldOfflineMode = ProjectPtr->GetProjectOfflineFileMode();
	if ( OFFLINE_FILE_INSPECTION == OldOfflineMode )
	{	
		OfflineMode = true;
		NewOfflineMode = OFFLINE_FILE_PROGRAM;	
	}
	else
	{	NewOfflineMode = OFFLINE_FILE_INSPECTION;	}
	if ( true == OfflineMode )
	{	LoadFrame = true; }
	else
	{	LoadFrame = false; }
	if ( ProjectPtr->GetProjectMultiDistrictMode() == true )
	{	bDistrictChange = true;	}
	AOIDataCollect.SetOfflineMode(OfflineMode);
	if ( ProjectPtr->SwtichProjectOfflineFileMode(NewOfflineMode, bDistrictChange, LoadFrame) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); }
	ResetMainViewWndAndGrabImage(false, true);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSettingOfflineInspectionMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd || true==bRemoteState )
	{	pCmdUI->Enable(FALSE);	}
	else
	{
		OFFLINE_FILE_MODE OfflineMode = OFFLINE_FILE_PROGRAM;
		if ( NULL != ProjectPtr )
		{	OfflineMode = ProjectPtr->GetProjectOfflineFileMode(); }
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(OFFLINE_FILE_INSPECTION==OfflineMode);
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnRebuildInspectionField() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return; }	
	if ( AOIDataCollect.GetIsLockUIWnd() == true ) { return; }

	CProjectFieldConfigWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();

	/*
	bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( true == OfflineMode ) { return ; }

	CString str;
	str = _T("Do you want to rebuild inspection fields ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON2) == IDNO ) 
	{	return; }
	ProjectPtr->ClearProjectAllInspectionField();
	*/
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRebuildInspectionField(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
#define _DEBUG
#ifdef _DEBUG
	OfflineMode = false;
#endif//_DEBUG
	if ( NULL==ProjectPtr || true==bLockUIWnd || true==OfflineMode || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectGroupConfigWnd()
{
#ifdef PART_GROUP_USE
	if ( ProjectGroupConfigWnd.IsWindowVisible() == TRUE )
	{	return; }
	ProjectGroupConfigWnd.ShowWindow(SW_SHOW);
#endif//PART_GROUP_USE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectGroupConfigWnd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
#ifdef PART_GROUP_USE
	ExecUpdateUI_ProjectParam(pCmdUI);
#else
	pCmdUI->Enable(FALSE);
#endif//PART_GROUP_USE
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnResetProjectLighting() 
{
	// TODO: Add your command handler code here	
	ResetMainViewWndAndGrabImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateResetProjectLighting(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
#ifndef OFFLINE_VERSION
	ExecUpdateUI_ProjectParam(pCmdUI);
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLoadSystemParameter() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString str;	
	str = _T("Do you want to load system parameters?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }

	const bool bCreateTempFolder = false;
	const bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
	const TSystemParameter OldParam=AOIDataCollect.GetSystemParameter();
	str.Format(_T("CMainFrame::OnLoadSystemParameter Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( AOIDataCollect.LoadSystemFilenameSyntax() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( AOIDataCollect.LoadSystemParameter(bCreateTempFolder) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( MotionCtrlPtr->LoadMotionParameter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	if ( PlcCtrlPtr->LoadPLCINIParameter() == false )
	{		
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		if ( PlcCtrlPtr->PLC_WriteINIParameter() == false )
		{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	}
	AOIDataDefine.LoadDefineTextFile();		
	AOIDataCollect.UpdateSystemParameter();
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( AOIDataCollect.ApplySystemParameterToAllProject() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
	if ( AOIDataCollect.ExecMESComm_SetSystemParam() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	AOIDataCollect.SetIsLockSystemParameter(false);

	const TSystemParameter NewParam=AOIDataCollect.GetSystemParameter();
	LogOperCtrl.SaveLogSystemComparParam(OldParam, NewParam);
	CWnd::PostMessage(MSG_RIBBON_BAR_WND, WPARAM_SHOW_DEBUG_CATEGORY, AOIDataCollect.GetShowRibbonBarWndCategoryDebug());
	str.Format(_T("CMainFrame::OnLoadSystemParameter End"));
	AOIDataCollect.SaveMovingTimeMsg(str);

	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	if ( true==IsLockSystemParam && ProjectCount>0 )
	{		
		str = _T("System Parameter Changed, Please Re-Load Project");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLoadSystemParameter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSaveSystemParameter() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	CString str;
	str = _T("Do you want to save system parameters?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }	
	if ( AOIDataCollect.UpdateSystemParameter() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	if ( MotionCtrlPtr->SaveMotionParameter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	//Light3DCtrl.SaveAllLight3DCastParameter();
	//AOIDataCollect.SaveSystemSliceParamINI();
	AOIDataCollect.SaveSystemParameter();
	AOIDataCollect.SaveSystemFilenameSyntax();
	//AOIDataCollect.SaveCalibrationParameter();		
	//AOIDataCollect.ApplyCalibrationParameter();
	AOIDataCollect.CopySystemParamFileToProjectFolder();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSaveSystemParameter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSaveRawImage()
{
	// TODO: Add your command handler code here
	const bool bSaveRawImage = AOIDataCollect.GetSaveProjectRawImage();
	AOIDataCollect.SetSaveProjectRawImage(!bSaveRawImage);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSaveRawImage(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
#ifdef OFFLINE_VERSION
	bLockUIWnd = true;
#endif//OFFLINE_VERSION
	if ( true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	

	const bool bSaveRawImage = AOIDataCollect.GetSaveProjectRawImage();
	pCmdUI->SetCheck(bSaveRawImage);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnConvertSystemParameter()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd ) { return ; }
	CSystemConvertWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnConvertLoadSystemParameter(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnBuildMotionXYCaliTable()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString  str;
	DWORD    Res=0;
	bool     Rebuild = true;
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
	{
		str = "Please disable XY-Calibration, and Calibrate again.";
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);		
		return ;
	}

	str = "Do you want to rebuild XY calibration table?";
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( Res == IDCANCEL ) { return; }

	if ( Res == IDNO ) { Rebuild = false; }
	else { Rebuild = true; }
	
	AOIDataCollect.BuildMotionXYCaliList(Rebuild);	

	str = "Do you want to enable XY calibration?";
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( Res == IDYES )
	{ 
		MotionCtrlPtr->GetMotionParameter().m_XYCaliEnable = FN_ENABLE;
		return; 
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateBuildMotionXYCaliTable(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	int  nEnableXYCali = MotionCtrlPtr->GetMotionParameter().m_XYCaliEnable;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr || FN_ENABLE==nEnableXYCali )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLibraryMerge() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	CString str2;
	CString LibraryName;
	CString ExtName;
	bool    bIsOK = true;

	LibraryName = AOIDataCollect.GetAOILibraryName();
	bIsOK = JetAPI::ExtractExtendFileName(LibraryName, ExtName);
	if ( false == bIsOK ) 
	{
		str = _T("Error, AOI Library Name Exception?");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s [%s]"), str, LibraryName);
		JetAPI::ShowMessageBox(str2);
		return;
	}

	str = _T("Do you want to update project library to AOI library");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s] ?"), str, LibraryName);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }

	if ( AOIDataCollect.MergeAOILibrary(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLibraryMerge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLibrarySaveAs() 
{
	// TODO: Add your command handler code here	
	TCHAR   filename[MAX_JET_PATH]=_T("");
	CString Filename = AOIDataCollect.GetAOILibraryName();
	TCHAR szFilters[]=_T("Library Files (*.LIB)|*.LIB|All Files (*.*)|*.*||");
	CFileDialog dialog(FALSE, _T("LIB"), _T("*.LIB"), OFN_FILEMUSTEXIST, szFilters, this);
	::_tcscpy(filename, Filename);	
	dialog.m_ofn.lpstrFile  = filename;	
	if ( dialog.DoModal() == IDCANCEL ) 
	{	return ; }

	CString str, str2;
	Filename = dialog.GetPathName();
	if ( JetAPI::IsFileExist(Filename) == true )
	{
		str = _T("File exist, do you want to overwrite it?");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s [%s]"), str, Filename);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
		{	return ; }
	}
	if ( AOIDataCollect.SaveAOILibraryAs(Filename) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLibrarySaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLibraryClear() 
{
	// TODO: Add your command handler code here
	CString str;
	str = _T("Do you want to clear AOI library?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	AOIDataCollect.ClearAOILibrary();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLibraryClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLibraryLoad() 
{
	// TODO: Add your command handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString      str;
	CString      str2;
	CString      LibraryName;
	CAOIProject *LibraryPtr = NULL;

	LibraryName = AOIDataCollect.GetAOILibraryName();
	str = _T("Do you want to replace project library with AOI library");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s] ?"), str, LibraryName);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }

	LibraryPtr = AOIDataCollect.LoadAOILibrary(LibraryName);
	if ( NULL == LibraryPtr )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); 
		return ;
	}
	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);
	if ( ProjectPtr->MergeProjectLibrary(LibraryPtr) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); 
		AOIObjManager.DestroyProjectObj(LibraryPtr);
		CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
		return ;
	}
	AOIObjManager.DestroyProjectObj(LibraryPtr);

	if ( ProjectPtr->ApplyProjectLibraryConfiguration() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}

	if ( ProjectPtr->ApplyProjectLibraryToComponents() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLibraryLoad(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLibraryReBuild()
{
	CString      str;
	CString      str2;
	CString      LibraryName;
	CAOIProject *LibraryPtr = NULL;

	LibraryName = AOIDataCollect.GetAOILibraryName();
	str = _T("Do you want to ReBuild AOI library");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s] ?"), str, LibraryName);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }
		
	if ( AOIDataCollect.ReBuildAOILibrary(LibraryName) == false )
	{	str = AOIDataCollect.GetErrorString();	}
	else
	{	str = AOIDataDefine.GetFinishText();	}
	JetAPI::ShowMessageBox(str);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLibraryReBuild(CCmdUI* pCmdUI)
{	
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnServerLibraryMerge()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ExecProjectSaveLibraryToServer(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateServerLibraryMerge(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnServerLibraryLoad()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString      str;
	CString      str2;	
	CString      ServerLibrary;	

	ServerLibrary = AOIDataCollect.GetAOIServerLibraryFolder();
	str = _T("Do you want to replace project library with Server library");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s] ?"), str, ServerLibrary);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO ) 
	{	return; }

	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);

	if ( ProjectPtr->LoadProjectServerLibrary(ServerLibrary) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString()); 		
		CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
		return ;
	}

	if ( ProjectPtr->ApplyProjectLibraryConfiguration() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}

	if ( ProjectPtr->ApplyProjectLibraryToComponents() == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateServerLibraryLoad(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnServerLibraryClear()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	str = _T("Do you want to clear Server library?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	//ProjectPtr->ClearProjectServerLibrary();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateServerLibraryClear(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetSaveDefectImage() 
{
	// TODO: Add your command handler code here
	if ( AOIDataCollect.OperateLevelOnlineSaveImage() == false ) { return; }
	bool bSave = AOIDataCollect.GetSaveDefectImage();
	if ( true == bSave )
	{	bSave = false;	}
	else
	{	bSave = true; }
	AOIDataCollect.SetSaveDefectImage(bSave);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetSaveDefectImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here		
	bool bSave = AOIDataCollect.GetSaveDefectImage();	
	pCmdUI->SetCheck(bSave);
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetOnlineTuningEnable() 
{
	// TODO: Add your command handler code here	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	bool bEnable = ProjectPtr->GetProjectOnlineTuningEnableUI();
	if ( true == bEnable )
	{	bEnable = false;	}
	else
	{	bEnable = true; }
	AOIDataCollect.SetOnlineTuningEnable(bEnable);		
	ProjectPtr->WriteProjectOnlineTuningEnable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetOnlineTuningEnable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bSave = false;
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL != ProjectPtr )
	{	bSave = ProjectPtr->GetProjectOnlineTuningEnableUI(); }
#ifdef OFFLINE_VERSION
	if ( true == bLockUIWnd || NULL==ProjectPtr )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
	pCmdUI->SetCheck(bSave);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineTuningLastFile() 
{
	// TODO: Add your command handler code here
	if ( ExecOnlineTuningLastFile() == false ) { return; }
	//ResetMainViewWndAndGrabImage(false, true);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineTuningLastFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectTuning(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineTuningNextFile() 
{
	// TODO: Add your command handler code here	
	if ( ExecOnlineTuningNextFile() == false ) { return; }
	//ResetMainViewWndAndGrabImage(false, true);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineTuningNextFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	ExecUpdateUI_ProjectTuning(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnOnlineTuningListFile() 
{
	// TODO: Add your command handler code here
	if ( ExecOnlineTuningListFile() == false ) { return; }
	//ResetMainViewWndAndGrabImage(false, true);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOnlineTuningListFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectTuning(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOnlineTuningLastFile()//線上調機-上一筆
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd || NULL==ProjectPtr )
	{	return false;}
#ifdef OFFLINE_VERSION
	//讀取在線軟體目前是否有啟用線上調機功能
	const bool OnlineTuningEnable=ProjectPtr->ReadProjectOnlineTuningEnable();
	ProjectPtr->SetProjectOnlineTuningEnableUI(OnlineTuningEnable);
#endif//OFFLINE_VERSION
	
	CString str, str2, str3;
	CString DateTime;
	CString SpcFilename;
	CString TmpFilename;
	CString OfflineName;
	CString ShortFolder;
	CString FileMainName;	
	CString OfflineFolder;
	CString CurrentDateTime;
	CString OnlineTuningFolder;
	CString CurrentOfflineFolder;
	CString OnlineTuningProjectFolder;	
	std::vector<CString> FolderList;		
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);

	FileMainName = ProjectPtr->GetProjectFileMainName();	
	OnlineTuningFolder = AOIDataCollect.GetAOIOnlineTuningDirectory();
	CurrentOfflineFolder = ProjectPtr->GetProjectOnlineTuningFolder();

	OnlineTuningProjectFolder.Format(_T("%s\\%s"), OnlineTuningFolder, FileMainName);
	JetAPI::ListFolder(OnlineTuningProjectFolder, FolderList);

	const size_t FolderCount = FolderList.size();
	if ( 0 == FolderCount ) { return false; }
	std::sort(FolderList.begin(), FolderList.end());

	if ( CurrentOfflineFolder.GetLength() == 0 ) 
	{
		if ( FolderCount > 1 )//抓最新第2筆
		{	ShortFolder=FolderList[FolderCount-2];	}
		else
		{	ShortFolder=FolderList[FolderCount-1];	}
	}
	else
	{
		JetAPI::ExtractTopFolder(CurrentOfflineFolder, CurrentDateTime);		
		ShortFolder = JetAPI::SearchOtherTestFolder(FolderList, CurrentDateTime, false);		
	}
	OfflineFolder.Format(_T("%s\\%s"), OnlineTuningProjectFolder, ShortFolder);	
	if ( CurrentOfflineFolder == OfflineFolder )
	{ 
		OFFLINE_FILE_MODE OfflineFileMode=ProjectPtr->GetProjectOfflineFileMode();
		if ( OFFLINE_FILE_INSPECTION == OfflineFileMode )
		{	return false; }
	}	
	JetAPI::ExtractTopFolder(OfflineFolder, DateTime);
	SpcFilename.Format(_T("%s\\%s.DAT"), OfflineFolder, DateTime);	
	OfflineName = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DISTRICT_ID_A);
	TmpFilename.Format(_T("%s\\%s.TXT"), AOIDataCollect.GetAOITempDirectory(), DateTime);	
	::DeleteFile(TmpFilename); ::Sleep(10); 
	::CopyFile(OfflineName, TmpFilename, FALSE);
	if ( JetAPI::IsFileExist(TmpFilename) == false )
	{
		if ( ShortFolder.CompareNoCase(FolderList[FolderCount-1]) == 0 )//最後一筆, 應該是尚未輸出結束
		{
			str = _T("Inspection data is exporting");
			str = LoadMultiLanguageString(str, str);
			UserLevel = USER_LEVEL_SUPERVISOR;
			if ( UserLevel < USER_LEVEL_SUPERVISOR )
			{	str2 = str; }
			else
			{	str2.Format(_T("%s\n%s"), str, OfflineName);	}
			JetAPI::ShowMessageBox(str2);
			return false; 
		}
		//不是最後一筆, 應該是資料夾無檢測檔案
		str = _T("The folder not inspection data");
		str = LoadMultiLanguageString(str, str);
		str3 = _T("Do you want to delete the folder ?");
		str3 = LoadMultiLanguageString(str3, str3);			
		str2.Format(_T("%s\n%s\n[%s]"), str, str3, OfflineFolder);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDYES )
		{	JetAPI::RemoveFolder(OfflineFolder); }		
		return false; 
	}
	::DeleteFile(TmpFilename);

	::Sleep(50);
	AOIDataCollect.SetOfflineMode(true);
	AOIDataCollect.SetIsLockUIWnd(true);	
	if ( ProjectPtr->LoadProjectOnlineTuningFile(SpcFilename) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		AOIDataCollect.SetIsLockUIWnd(false);	
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOnlineTuningNextFile()//線上調機-下一筆
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd || NULL==ProjectPtr )
	{	return false;}
#ifdef OFFLINE_VERSION
	//讀取在線軟體目前是否有啟用線上調機功能
	const bool OnlineTuningEnable=ProjectPtr->ReadProjectOnlineTuningEnable();
	ProjectPtr->SetProjectOnlineTuningEnableUI(OnlineTuningEnable);
#endif//OFFLINE_VERSION

	CString str, str2, str3;
	CString DateTime;
	CString SpcFilename;
	CString TmpFilename;
	CString OfflineName;
	CString ShortFolder;
	CString FileMainName;	
	CString OfflineFolder;
	CString CurrentDateTime;
	CString OnlineTuningFolder;
	CString CurrentOfflineFolder;
	CString OnlineTuningProjectFolder;
	std::vector<CString> FolderList;
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);

	FileMainName = ProjectPtr->GetProjectFileMainName();	
	OnlineTuningFolder = AOIDataCollect.GetAOIOnlineTuningDirectory();
	CurrentOfflineFolder = ProjectPtr->GetProjectOnlineTuningFolder();

	OnlineTuningProjectFolder.Format(_T("%s\\%s"), OnlineTuningFolder, FileMainName);
	JetAPI::ListFolder(OnlineTuningProjectFolder, FolderList);
	const size_t FolderCount = FolderList.size();
	if ( 0 == FolderCount ) { return false; }
	std::sort(FolderList.begin(), FolderList.end());
	
	if ( CurrentOfflineFolder.GetLength() == 0 ) 
	{
		if ( FolderCount > 1 )//抓最新第2筆
		{	ShortFolder=FolderList[FolderCount-2];	}
		else
		{	ShortFolder=FolderList[FolderCount-1];	}
	}
	else
	{
		JetAPI::ExtractTopFolder(CurrentOfflineFolder, CurrentDateTime);		
		ShortFolder = JetAPI::SearchOtherTestFolder(FolderList, CurrentDateTime, true);		
	}
	OfflineFolder.Format(_T("%s\\%s"), OnlineTuningProjectFolder, ShortFolder);	
	if ( CurrentOfflineFolder == OfflineFolder )
	{
		OFFLINE_FILE_MODE OfflineFileMode=ProjectPtr->GetProjectOfflineFileMode();
		if ( OFFLINE_FILE_INSPECTION == OfflineFileMode )
		{	return false; }
	}	
	JetAPI::ExtractTopFolder(OfflineFolder, DateTime);	
	SpcFilename.Format(_T("%s\\%s.DAT"), OfflineFolder, DateTime);	
	OfflineName = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DISTRICT_ID_A);
	TmpFilename.Format(_T("%s\\%s.TXT"), AOIDataCollect.GetAOITempDirectory(), DateTime);	
	::DeleteFile(TmpFilename); ::Sleep(10); 
	::CopyFile(OfflineName, TmpFilename, FALSE);
	if ( JetAPI::IsFileExist(TmpFilename) == false )
	{
		if ( ShortFolder.CompareNoCase(FolderList[FolderCount-1]) == 0 )//最後一筆, 應該是尚未輸出結束
		{
			str = _T("Inspection data is exporting");
			str = LoadMultiLanguageString(str, str);
			UserLevel = USER_LEVEL_SUPERVISOR;
			if ( UserLevel < USER_LEVEL_SUPERVISOR )
			{	str2 = str; }
			else
			{	str2.Format(_T("%s\n%s"), str, OfflineName);	}
			JetAPI::ShowMessageBox(str2);
			return false; 
		}
		//不是最後一筆, 應該是資料夾無檢測檔案
		str = _T("The folder not inspection data");
		str = LoadMultiLanguageString(str, str);
		str3 = _T("Do you want to delete the folder ?");
		str3 = LoadMultiLanguageString(str3, str3);			
		str2.Format(_T("%s\n%s\n[%s]"), str, str3, OfflineFolder);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDYES )
		{	JetAPI::RemoveFolder(OfflineFolder); }		
		return false; 
	}
	::DeleteFile(TmpFilename);

	::Sleep(50);
	AOIDataCollect.SetOfflineMode(true);
	AOIDataCollect.SetIsLockUIWnd(true);
	if ( ProjectPtr->LoadProjectOnlineTuningFile(SpcFilename) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		AOIDataCollect.SetIsLockUIWnd(false);	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOnlineTuningListFile()//線上調機-顯示列表
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd || NULL==ProjectPtr )
	{	return false;}
#ifdef OFFLINE_VERSION
	//讀取在線軟體目前是否有啟用線上調機功能
	const bool OnlineTuningEnable=ProjectPtr->ReadProjectOnlineTuningEnable();
	ProjectPtr->SetProjectOnlineTuningEnableUI(OnlineTuningEnable);
#endif//OFFLINE_VERSION

	CString str, str2;
	CString DateTime;
	CString SpcFilename;
	CString TmpFilename;
	CString OfflineName;	
	CString FileMainName;	
	CString OfflineFolder;
	CString CurrentDateTime;
	CString OnlineTuningFolder;
	CString CurrentOfflineFolder;
	CString OnlineTuningProjectFolder;
	std::vector<CString> FolderList;
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);

	FileMainName = ProjectPtr->GetProjectFileMainName();	
	OnlineTuningFolder = AOIDataCollect.GetAOIOnlineTuningDirectory();
	//OnlineTuningFolder = AOIDataCollect.GetAOIOnlineOfflineDirectory();
	CurrentOfflineFolder = ProjectPtr->GetProjectOnlineTuningFolder();
	if ( CurrentOfflineFolder.GetLength() == 0 )
	{	OnlineTuningProjectFolder.Format(_T("%s\\%s"), OnlineTuningFolder, FileMainName);	}
	else
	{	OnlineTuningProjectFolder = CurrentOfflineFolder;	}
	if ( JetAPI::IsFolderExist(OnlineTuningProjectFolder) == false )
	{	OnlineTuningProjectFolder = OnlineTuningFolder; }

	OfflineFolder = OnlineTuningProjectFolder;
	if ( JetAPI::OpenFolderDialog(this, OfflineFolder) == false ) { return false; }
	if ( CurrentOfflineFolder == OfflineFolder )
	{
		OFFLINE_FILE_MODE OfflineFileMode=ProjectPtr->GetProjectOfflineFileMode();
		if ( OFFLINE_FILE_INSPECTION == OfflineFileMode )
		{	return false; }		
	}		
	JetAPI::ExtractTopFolder(OfflineFolder, DateTime);	
	SpcFilename.Format(_T("%s\\%s.DAT"), OfflineFolder, DateTime);		
	OfflineName = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DISTRICT_ID_A);	
	TmpFilename.Format(_T("%s\\%s.TXT"), AOIDataCollect.GetAOITempDirectory(), DateTime);	
	::DeleteFile(TmpFilename); ::Sleep(10); 
	::CopyFile(OfflineName, TmpFilename, FALSE);
	if ( JetAPI::IsFileExist(TmpFilename) == false )
	{
		str = _T("Inspection data is exporting");
		str = LoadMultiLanguageString(str, str);
		UserLevel = USER_LEVEL_SUPERVISOR;
		if ( UserLevel < USER_LEVEL_SUPERVISOR )
		{	str2 = str; }
		else
		{	str2.Format(_T("%s\n%s"), str, OfflineName);	}
		JetAPI::ShowMessageBox(str2);
		return false; 
	}
	::DeleteFile(TmpFilename);

	::Sleep(50);
	AOIDataCollect.SetOfflineMode(true);
	AOIDataCollect.SetIsLockUIWnd(true);	
	if ( ProjectPtr->LoadProjectOnlineTuningFile(SpcFilename) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		AOIDataCollect.SetIsLockUIWnd(false);	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetMultiBoardCtrlMode() 
{
	// TODO: Add your command handler code here
	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( MULTI_BOARD_CTRL_BOARD == MultiBoardCtrlMode ) { MultiBoardCtrlMode = MULTI_BOARD_CTRL_DISABLE; }
	else { MultiBoardCtrlMode = MULTI_BOARD_CTRL_BOARD; }
	AOIDataCollect.SetMultiBoardCtrlMode(MultiBoardCtrlMode);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetMultiBoardCtrlMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }	

	MULTI_BOARD_CTRL_MODE MultiBoardCtrlMode = AOIDataCollect.GetMultiBoardCtrlMode();
	if ( MULTI_BOARD_CTRL_DISABLE == MultiBoardCtrlMode ) { pCmdUI->SetCheck(FALSE); }
	else { pCmdUI->SetCheck(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLaneAdjustHome() 
{
	// TODO: Add your command handler code here
	ExecLaneAdjustHome();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLaneAdjustHome(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool LaneAdjustDisabel = PlcCtrlPtr->GetLaneAdjustDisableBtn(LaneID);
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || true==LaneAdjustDisabel || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLaneAdjustWidth()
{
	// TODO: Add your command handler code here
	ExecLaneAdjustWidth();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLaneAdjustWidth(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool bCanMove = PlcCtrlPtr->GetLaneAdjustCanMove(LaneID);
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || false==bCanMove || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecSwitchActiveLaneID(LANE_ID LaneID)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }	
	const UINT ViewID = AOIDataCollect.GetMainViewWndID();	
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();			
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();		
	const bool bOnlineFormView = AOIDataCollect.CheckOnlineFormViewID(ViewID);
	AOIDataCollect.SetActiveLaneID(LaneID);
	
	if ( MULTI_LANE_2==MultiLaneMode && ProjectCount>1 )
	{
		unsigned int ProjectIndex=-1;
		CAOIProject *ProjectPtr = NULL;
		ProjectPtr = AOIDataCollect.GetLaneProjectPtr(LaneID, 0, true);
		if ( NULL != ProjectPtr )
		{			
			ProjectIndex = ProjectPtr->GetProjectIndex();
			if ( false == bOnlineFormView )
			{	ExecProjectSwitch(ProjectIndex);	}
			else
			{
				AOIDataCollect.CloseActiveProject();
				AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
				UpdateDocumentTitle();
				ResetMainViewWndAndGrabImage(true, false);
			}
		}		
	}
	return true;

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		bool bEnable = ProjectPtr->GetProjectLaneEnable(LaneID);
		if ( true == bEnable )
		{	ProjectPtr->ApplySystemParameter(LaneID);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLaneActiveA() 
{
	// TODO: Add your command handler code here		
	ExecSwitchActiveLaneID(LANE_ID_A);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLaneActiveA(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	LANE_WORK_MODE   LaneWorkMode = AOIDataCollect.GetLaneWorkMode_LA();
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || LANE_WORK_DISABLE==LaneWorkMode || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
#endif//OFFLINE_VERSION	

	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( LANE_ID_A == LaneID )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnLaneActiveB() 
{
	// TODO: Add your command handler code here
	ExecSwitchActiveLaneID(LANE_ID_B);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateLaneActiveB(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	LANE_WORK_MODE   LaneWorkMode = AOIDataCollect.GetLaneWorkMode_LB();
#ifdef OFFLINE_VERSION
	pCmdUI->Enable(FALSE);
#else
	if ( true==bLockUIWnd || LANE_WORK_DISABLE==LaneWorkMode || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
#endif//OFFLINE_VERSION	

	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( LANE_ID_B == LaneID )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectOpen2nd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	if ( true==bLockUIWnd || 1!=ProjectCount )
	{	return; }
	if ( ExecProjectOpen(1) == false )
	{	return; }
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
	if ( AOIDataCollect.CheckOnlineFormViewID() == false )
	{
		CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			if ( ProjectPtr->CheckProjectShowNewProjectWizardWnd() == true )
			{	CWnd::SendMessage(WM_COMMAND, ID_PROJECT_NEW_PANEL, NULL);	}
		}	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectOpen2nd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = TRUE;
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	if ( true==bLockUIWnd || ProjectCount<1 || true==bRemoteState )
	{	bEnable = FALSE; }	
	else
	{
#ifndef OFFLINE_VERSION
		if ( MULTI_LANE_1 != MultiLaneMode )
		{	bEnable = FALSE; }
#endif//OFFLINE_VERSION
	}
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSwitch(unsigned int TargetIndex)//執行切換專案
{
	const unsigned int ProjectIndex = AOIDataCollect.GetActiveProjectIndex();
	if ( TargetIndex == ProjectIndex ) { return true; }
	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE, NULL);	
	AOIDataCollect.CloseActiveProject();	
	AOIDataCollect.SetActiveProjectIndex(TargetIndex);
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);	

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	CString Folder;
	CString filename;
	CString FileMainName;
	CString OfflineFile;
	CString OfflineFolder;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();

	filename = ProjectPtr->GetProjectFileName();
	JetAPI::ExtractMainFileName(filename, Folder);	
	JetAPI::ExtractMainFileNameNoPath(filename, FileMainName);
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(Folder);
	OfflineFile = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DistrictID);	

	AOIDataCollect.SetProjectMapMode(false);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetIsNeedGrabFiducial(true, LaneID);
	AOIDataCollect.SetOfflineFileName(OfflineFile);		
	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
	AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LaneID);
	//AOIDataCollect.SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LANE_ID_B);
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )	
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	
	UpdateDocumentTitle();	
	SetDocumentModifiedFlag(FALSE);	
	if ( ExecProjectPreLoadOffline(ProjectPtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSwitchTo1() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	if ( true==bLockUIWnd || ProjectCount<1 )
	{	return; }
	ExecProjectSwitch(0);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSwitchTo1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	//if ( true==bLockUIWnd || ProjectCount<1 ||MULTI_LANE_2==MultiLaneMode )
	if ( true==bLockUIWnd || ProjectCount<1 || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	const unsigned int ProjectIndex = AOIDataCollect.GetActiveProjectIndex();
	pCmdUI->SetCheck(0==ProjectIndex);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectSwitchTo2() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	if ( true==bLockUIWnd || ProjectCount<2 )
	{	return; }	
	ExecProjectSwitch(1);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectSwitchTo2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	//if ( true==bLockUIWnd || ProjectCount<2 || MULTI_LANE_2==MultiLaneMode )
	if ( true==bLockUIWnd || ProjectCount<2 || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
	const unsigned int ProjectIndex = AOIDataCollect.GetActiveProjectIndex();
	pCmdUI->SetCheck(1==ProjectIndex);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectShowReportTxt()
{
	CString ReportTxt;	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ReportTxt = AOIDataCollect.GetProjectReportFilename(ProjectPtr);	
	::ShellExecute(NULL, _T("open"), ReportTxt, NULL, NULL, SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectShowReportTxt(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectClearProjectLibrary()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	str = _T("Do you want to clear project library?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return ; }
	ProjectPtr->ClearProjectLibrary();
	PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateClearProjectLibrary(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectAutoLabelModelGroup()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString str;	
	str = _T("Do you want to Auto Label Component Package?");	
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return ; }
	const bool bSucc=ProjectPtr->AutoLabelProjectComponentModelGroupByAI();	
	if ( false == bSucc )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	
	AOIDataCollect.GetSystemParameter().m_ModelNameUsePartNumber = true;
	CWnd::PostMessage(MSG_EDIT_LIBRARY_WND, WPARAM_UPDATE_USE_PARTNUMBER_CHK, FALSE);
	CWnd::PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectAutoLabelModelGroup(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectCloseAll() 
{
	// TODO: Add your command handler code here
	if ( ExecProjectCloseAll(true) == false )
	{	return; }
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectCloseAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewUserRegisterWnd()
{
	CUserRegisterWnd Wnd;
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewUserRegisterWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }
	if ( AOIDataCollect.OperateLevelEditWndUserRegister() == false )
	{	return;	}
	ExecViewUserRegisterWnd();	
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewUserRegisterWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewBarcodeDeviceWnd()
{
	CBarcodeDeviceWnd Wnd;
	if ( Wnd.DoModal() == IDCANCEL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewBarcodeDeviceWnd() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndBarcodeDevice() == false )
	{	return;	}
	ExecViewBarcodeDeviceWnd();
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewBarcodeDeviceWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	bEnable=FALSE; }
	else
	{	bEnable=TRUE; }
#ifdef BARCODE_DEVICE_DISABLE
	bEnable=FALSE;
#endif//BARCODE_DEVICE_DISABLE	
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectOpenNext() 
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	const unsigned int ProjectCount = (unsigned int)AOIDataCollect.GetProjectPtrCount();
	if ( true == bLockUIWnd )
	{	return; }
	if ( ExecProjectOpen(ProjectCount) == false )
	{	return ; }
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
	if ( AOIDataCollect.CheckOnlineFormViewID() == false )
	{
		CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
		if ( NULL != ProjectPtr )
		{
			if ( ProjectPtr->CheckProjectShowNewProjectWizardWnd() == true )
			{	CWnd::SendMessage(WM_COMMAND, ID_PROJECT_NEW_PANEL, NULL);	}
		}	
	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectOpenNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	if ( true==bLockUIWnd || ProjectCount<1 || true==bRemoteState )
	{	bEnable = FALSE; }	
	else
	{
#ifndef OFFLINE_VERSION
		if ( MULTI_LANE_1 != MultiLaneMode )
		{	bEnable = FALSE; }
#endif//OFFLINE_VERSION
	}
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectClose() 
{	
	// TODO: Add your command handler code here	
	if ( ExecProjectClose() == false )
	{	return; }
	SendMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_BUILD, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectListCombo()
{
	PostMessage(MSG_RIBBON_BAR_WND, WPARAM_PROJECT_COMBOX_SEL_CHANGE, ID_PROJECT_LIST_COMBO);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectListCombo(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetProjectColorListWnd() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectColor() == false )
	{	return; }

	CProjectColorWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{
		ResetMainViewWndAndGrabImage(false, true);//重設回傳視窗並且取像
		return; 
	}

	std::vector<CColorGroup> ColorGroupList;
	Wnd.GetColorGroupList(ColorGroupList);
	ProjectPtr->SetProjectColorGroupList(ColorGroupList);
	ProjectPtr->UpdateProjectColorGroupListToModel();
	ResetMainViewWndAndGrabImage(false, true);//重設回傳視窗並且取像
	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetProjectColorListWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecHideWndForInspection()//隱藏視窗來檢測
{	
	BOOL bShow=FALSE;	
#ifndef CAMERA_OBJ_DISABLE
	ShowGlobalWnd(&CameraCtrlWnd, bShow);
#endif//CAMERA_OBJ_DISABLE

#ifndef MOTION_OBJ_DISABLE
	ShowGlobalWnd(&MotionCtrlWnd, bShow);	
#endif//MOTION_OBJ_DISABLE

#ifndef PLC_OBJ_DISABLE
	ShowGlobalWnd(&PlcCtrlWnd, bShow);	
#endif//PLC_OBJ_DISABLE

#ifndef PHASE_CTRL_DISABLE
	ShowGlobalWnd(&Light3DTiDLPWnd, bShow);	
#endif//PHASE_CTRL_DISABLE

#ifndef LIGHT_CTRL_DISABLE
	ShowGlobalWnd(&LightCtrlBoardWnd, bShow);	
#endif//LIGHT_CTRL_DISABLE
	ShowGlobalWnd(&ProjectGroupConfigWnd, bShow);

	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_MAP_DOCK_PANE, NULL);
	CWnd::SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectFdListWnd()
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectFdList() == false )
	{	return; }

	CFdListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectFdListWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectPanelListWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectPanelList() == false )
	{	return; }

	CPanelListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectPanelListWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectBoardListWnd()
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectBoardList() == false )
	{	return; }

	CBoardListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectBoardListWnd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectBarcodeListWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectBarcodeList() == false )
	{	return; }

	CBarcodeListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectBarcodeListWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectComponentListWnd() 
{
	// TODO: Add your command handler code here	
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	if ( AOIDataCollect.OperateLevelEditWndProjectComponentList() == false )
	{	return; }

	CComponentListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectComponentListWnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectComponentDefectAlarmListWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return ; }
	
	//if ( AOIDataCollect.OperateLevelEditWndProjectComponentList() == false )
	//{	return; }	
	CComponentDefectAlarmListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return;		}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectComponentDefectAlarmListWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectLoadOfflineFile()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return; }
	
	CString ProjectFolder;
	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);

	ProjectFolder = ProjectPtr->GetProjectProgramOfflineFolder();
	dialog.m_ofn.lpstrInitialDir = ProjectFolder;
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }		
	
	CString OfflineFile;
	CString OfflineFolder;
	CString OfflineFiducial;
	DISTRICT_ID DistrictID = DISTRICT_ID_A;

	OfflineFile = dialog.GetPathName();
	if ( JetAPI::ExtractLastPath(OfflineFile, OfflineFolder) == false )
	{	return; }
	
	AOIDataCollect.SetActiveDistrictID(DistrictID);
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);
	OfflineFiducial = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);	
	if ( ProjectPtr->LoadProjectProgramOfflineFile(OfflineFile) == false )
	{	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	
		return;
	}
	ProjectPtr->SetProjectMapIndex(0);
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.ReleaseFieldUniFrameList();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	ProjectPtr->SetProjectProgramFieldDoNotSave(true);	
	if ( ExecProjectPreLoadOffline(ProjectPtr) == true )	
	{
		if ( ProjectPtr->LoadProjectOfflineFdFile(OfflineFiducial) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	
		else
		{	ProjectPtr->SaveProjectOfflineFdFileToInspection(OfflineFiducial);	}
	}
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);
	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectLoadOfflineFile(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectLoadBOMFile()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return; }
	CAOIComponent *ComponentPtr=ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(ComponentPtr);

	CLoadBomWnd LoadBomWnd;
	if ( LoadBomWnd.DoModal() != IDOK )
	{	return; }	
	
	std::vector<TComponentNode> BomNodeList;
	LoadBomWnd.GetBomNodeList(BomNodeList);
	if ( ProjectPtr->AddProjectComponentAgentByBomNodeList(BomNodeList) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectLoadBOMFile(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectLoadSPICad()
{
	CString ErrorString;
	CAOIProject *Project = AOIDataCollect.GetActiveProject();
	CAOIPanel *PanelPtr = NULL;
	TCHAR szFilters[] = _T("PID Files (*.pid)|*.pid|All Files (*.*)|*.*||");
	//CFileDialog dialog (TRUE, _T("pid;asc;prn;csv"), _T("*.asc"), OFN_FILEMUSTEXIST, szFilters);
	CFileDialog dialog(TRUE, _T("pid"), _T("*.pid"), OFN_FILEMUSTEXIST, szFilters);
	if (dialog.DoModal() == IDCANCEL) { return; }

	const double AddAngle = 0.0;
	const bool bReverseAngle = false;
	const CString Filename = dialog.GetPathName();
	if (Project->LoadProjectSpiPidFileToCurrent(Filename, PanelPtr, AddAngle, bReverseAngle) == false)
	{
		ErrorString = Project->GetErrorString();
		JetAPI::ShowMessageBox(ErrorString);
		return;
	}
	JetAPI::ShowMessageBox(_T("Loading PID File Finished."), MB_ICONINFORMATION);

}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectLoadSPICad(CCmdUI * pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewOnlineFormView()
{
	ONLINE_FROMVIEW_MODE OnlineFormViewMode = AOIDataCollect.GetOnlineFormViewMode();
#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	if ( ONLINE_FROMVIEW_DUAL == OnlineFormViewMode)
	{	SwitchViewWnd(IDD_ONLINE_FORMVIEW_DUAL); }
	else
	{	SwitchViewWnd(IDD_ONLINE_FORMVIEW); }	
#else
	const int CategoryCount = m_wndRibbonBar.GetCategoryCount();
	if ( CategoryCount > RIBBON_INDEX_ONLINE_MAIN_VIEW )
	{	
		const int CategoryIndex = RIBBON_INDEX_ONLINE_MAIN_VIEW;
		CMFCRibbonCategory *ActiveCategoryPtr = m_wndRibbonBar.GetCategory(CategoryIndex);
		if ( NULL != ActiveCategoryPtr )
		{	
			m_wndRibbonBar.SetActiveCategory(ActiveCategoryPtr);	
			AOIDataCollect.SetRibbonCategoryIndex(CategoryIndex);
		}
	}	
#endif//FRAME_STYLE_TYPE
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewOnlineFormView(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewDropOutVerifyWnd()
{
	CAlgImageCompareWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewDropOutVerifyWnd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);	return;
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewSocketClientWnd()
{	
#ifndef MES_DISABLE
	if ( ITSCommWnd.IsWindowVisible() == false )
	{	ITSCommWnd.ShowWindow(SW_SHOW);	}
#endif//MES_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewITSCommWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}
	if ( AOIDataCollect.OperateLevelEditWndITSComm() == false)
	{	return ; }
	ExecViewSocketClientWnd();	
	AOIDataCollect.UserLogout_Check();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewITSCommWnd(CCmdUI* pCmdUI)
{
#ifndef MES_DISABLE
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	bool IsLockSystemParam = AOIDataCollect.GetIsLockSystemParameter();
	if ( true==bLockUIWnd || true==IsLockSystemParam || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#else
	pCmdUI->Enable(FALSE);
#endif//MES_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecFdConfirmWnd(LPARAM lParam)
{
	if ( NULL == lParam ) { return false; }
	CAOIFd *FdPtr = (CAOIFd*)(lParam);
	if ( FdPtr->IsKindOf(RUNTIME_CLASS(CAOIFd)) == FALSE ) { return false; }
	CAOIProject *ProjectPtr = FdPtr->GetFdProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CFdConfirmWnd FdConfirmWnd;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	FdConfirmWnd.SetFdPtr(FdPtr);
	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);			
	UINT ResultID = FdConfirmWnd.DoModal();
	if ( IDOK == ResultID )
	{
		std::vector<TUNI_FRAME> UniFrameList;
		FdPtr->GetRgnUniFrameList(UniFrameList);
		FdPtr->CalcFdSpaceBasePlane(UniFrameList);
		FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
		FdPtr->SetFdResultID_Alarm(RESULT_ID_OK);
	}
	else
	{	ProjectPtr->SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_FD_MATCH, _T("Error, Fd Search Fault"));	}
	PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	FdPtr->SetFdKeepImage(false);
	FdPtr->ClearRgnImageBuffer();	
	FdPtr->SetFdConfirmUIResultID(ResultID);
	//ModelPtr->SetModelResultID(RESULT_ID_OK);
	//ModelPtr->SetModelResultID_Alarm(RESULT_ID_OK);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecBarcodeConfirmWnd(LPARAM lParam)
{
	if ( NULL == lParam ) { return false; }
	CAOIBarcode *BarcodePtr = (CAOIBarcode*)(lParam);
	if ( BarcodePtr->IsKindOf(RUNTIME_CLASS(CAOIBarcode)) == FALSE ) { return false; }

	CAOIProject *ProjectPtr = BarcodePtr->GetBarcodeProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString        str;
	CString        strBarcode;
	UINT           UIResultID=0;
	CAOIWnd       *WndPtr = NULL;	
	CAOIModel     *ModelPtr = NULL;
	const size_t   szBuffer=256;
	wchar_t        wBuffer[szBuffer]=L"";
	unsigned int   FrameIndex = 0;
	CBarcodeConfirmWnd BarcodeConfirmWnd;
	std::vector<TUNI_FRAME> UniFrameList;
	std::vector<TUNI_FRAME> UniFrameListTmp;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	const double  SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();

	ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	BarcodePtr->GetRgnUniFrameList(UniFrameList);
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }

	WndPtr = BarcodePtr->GetBarcodeWndPtr();
	if ( NULL == WndPtr)
	{	FrameIndex=0; }
	else
	{	FrameIndex = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameIndex();	}	
	BarcodeConfirmWnd.SetActiveProject(ProjectPtr);
	BarcodeConfirmWnd.SetActiveBarcodePtr(BarcodePtr);	
	if ( false == IsExceptionAngle ) 
	{
		ProjectPtr->UpdateProjectFrameUniqueIDToFrameList(UniFrameList);
		BarcodeConfirmWnd.SetUniFrameList(FrameIndex, UniFrameList); 
	}
	else
	{		
		TREGION4D  ModelRgn;
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgnRotated;		
		ModelPtr->GetModelTotalRegion(ModelRgn);
		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, ModelCornerPts);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( CAOIModel::RotateModelUniFrameList(-AttachedAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListTmp) == false )
		{	return false;	}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BarcodeRotate.PNG"));
		ImageAPI.SaveUniFrameImage(str, UniFrameListTmp, true, false, false, false, SpaceRatio);
	#endif //_DEBUG
		ProjectPtr->UpdateProjectFrameUniqueIDToFrameList(UniFrameListTmp);
		BarcodeConfirmWnd.SetUniFrameList(FrameIndex, UniFrameListTmp);
	}
	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);			
	UIResultID = BarcodeConfirmWnd.DoModal();
	PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	BarcodePtr->ClearRgnImageBuffer();
	JetAPI::ClearUniFrameList(UniFrameListTmp);
	BarcodePtr->SetBarcodeConfirmUIResultID(UIResultID);
	if ( IDOK != UIResultID )
	{
		ProjectPtr->SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_BARCODE_DECODE, _T("Error, User Cancel Input Barcode"));
		return true;	
	}

	strBarcode = BarcodeConfirmWnd.GetBarcodeContext();
	const int BarcodeLen = strBarcode.GetLength();
	if ( BarcodeLen > 0 ) 
	{	
		JetAPI::TCHAR2wchar(strBarcode, wBuffer, szBuffer);
		BarcodePtr->SetBarcodeResultText(wBuffer);
		BarcodePtr->SetBarcodeResultID_AOI(RESULT_ID_OK);
		BarcodePtr->SetBarcodeResultID_Alarm(RESULT_ID_OK);
	}				
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecBarcodeHandHeldWnd(LPARAM lParam)
{	
	CAOIProject *ProjectPtr = AOIDataCollect.GetBarcodeHandHeld_Project();
	if ( NULL == ProjectPtr ) { return false; }
	if ( ProjectPtr->IsKindOf(RUNTIME_CLASS(CAOIProject)) == FALSE ) { return false; }

	DWORD   Res=0;
	CBarcodeInputWnd BarcodeInputWnd;
	const LANE_ID LaneID = AOIDataCollect.GetBarcodeHandHeldLaneID();
	CBarcode_Handheld *BarcodeHandHeldPtr = AOIDataCollect.GetBarcodeHandHeldPtr(LaneID);	
	BARCODE_NG_HANDLE_MODE BarcodeNGHandleMode = ProjectPtr->GetProjectParameter().m_BarcodeNGHandleMode;
	BARCODE_HANDHELD_READ_MODE BarcodeReadMode = ProjectPtr->GetProjectParameter().m_BarcodeHandHeldReadMode;
	
	if ( NULL != BarcodeHandHeldPtr )
	{	BarcodeHandHeldPtr->ClearBarcodeInfoList();	}

	BarcodeInputWnd.SetLaneID(LaneID);
	BarcodeInputWnd.SetProjectPtr(ProjectPtr);
	BarcodeInputWnd.SetBarcodeReadMode(BarcodeReadMode);
	Res = BarcodeInputWnd.DoModal();
	AOIDataCollect.SetBarcodeHandHeldResult(Res);		
	if ( IDOK != Res ) 
	{
		ProjectPtr->SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_BARCODE_INPUT, _T("Error, User Cancel Input Barcode")); 
		return true;	
	}

	CBarcode_Handheld &BarcodeHandheld = BarcodeInputWnd.GetBarcodeHandheld();
	AOIDataCollect.SetBarcodeHandHeld(LaneID, BarcodeHandheld);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecOpenProjectBarcodeWnd(LPARAM lParam)
{
	DWORD        dwRet=0;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;		
	CInputBoxWnd InputBox;
	
	strCaption = _T("Input Open Project Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();
	strValue = _T("");
	InputBox.SetParam1(strCaption, strLabel, strValue);
	dwRet = InputBox.DoModal();
	AOIDataCollect.SetOnlineOpenProjectHandHeldResult(dwRet);
	if ( IDCANCEL == dwRet)
	{
		//m_ErrorString = _T("Error, User cancel input barcode");		
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Project(AOI_EXCEPTION_PROJECT_BARCODE_INPUT, _T("Error, User Cancel Input Barcode"));
		return false;  
	}
	AOIDataCollect.SetOnlineOpenProjectHandHeldBarcode(InputBox.m_DataEdit1);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecComponentBarcodeConfirmWnd(LPARAM lParam)
{
	if ( NULL == lParam ) { return false; }
	CAOIComponent *ComponentPtr = (CAOIComponent*)(lParam);
	if ( ComponentPtr->IsKindOf(RUNTIME_CLASS(CAOIComponent)) == FALSE ) { return false; }

	CAOIProject *ProjectPtr = ComponentPtr->GetComponentProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString        str;
	CString        strBarcode;
	UINT           UIResultID=0;
	CAOIWnd       *WndPtr = NULL;	
	CAOIModel     *ModelPtr = NULL;
	const size_t   szBuffer=256;
	wchar_t        wBuffer[szBuffer]=L"";
	unsigned int   FrameIndex = 0;
	CBarcodeConfirmWnd BarcodeConfirmWnd;
	std::vector<TUNI_FRAME> UniFrameList;
	std::vector<TUNI_FRAME> UniFrameListTmp;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	const double  SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();

	ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	ComponentPtr->GetRgnUniFrameList(UniFrameList);	
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }

	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr)
	{	FrameIndex=0; }
	else
	{	FrameIndex = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameIndex();	}	
	BarcodeConfirmWnd.SetActiveProject(ProjectPtr);
	BarcodeConfirmWnd.SetActiveComponentPtr(ComponentPtr);	
	if ( false == IsExceptionAngle ) 
	{
		ProjectPtr->UpdateProjectFrameUniqueIDToFrameList(UniFrameList);
		BarcodeConfirmWnd.SetUniFrameList(FrameIndex, UniFrameList); 
	}
	else
	{		
		TREGION4D  ModelRgn;
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgnRotated;		
		ModelPtr->GetModelTotalRegion(ModelRgn);
		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, ModelCornerPts);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( CAOIModel::RotateModelUniFrameList(-AttachedAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListTmp) == false )
		{	return false;	}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BarcodeRotate.PNG"));
		ImageAPI.SaveUniFrameImage(str, UniFrameListTmp, true, false, false, false, SpaceRatio);
	#endif //_DEBUG
		ProjectPtr->UpdateProjectFrameUniqueIDToFrameList(UniFrameListTmp);
		BarcodeConfirmWnd.SetUniFrameList(FrameIndex, UniFrameListTmp);
	}
	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);			
	UIResultID = BarcodeConfirmWnd.DoModal();
	PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	//ComponentPtr->ClearRgnImageBuffer();//後面會清除
	JetAPI::ClearUniFrameList(UniFrameListTmp);
	ComponentPtr->SetComponentConfirmUIResultID(UIResultID);
	if ( IDOK != UIResultID )
	{
		ProjectPtr->SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_BARCODE_DECODE, _T("Error, User Cancel Input Barcode"));
		return true;	
	}
	
	strBarcode = BarcodeConfirmWnd.GetBarcodeContext();
	const int BarcodeLen = strBarcode.GetLength();
	if ( BarcodeLen > 0 ) 
	{	
		JetAPI::TCHAR2wchar(strBarcode, wBuffer, szBuffer);
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		TALG_PARAM_BARCODE_RECOGNIZE &barParam=AlgParam.GetAlgParamBarcodeRecognize();
		barParam.brBarcodeResult=wBuffer;		
		WndPtr->SetWndResultID(RESULT_ID_OK);
		WndPtr->SetWndLogicResultID(RESULT_ID_OK);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecUserLoginWnd(LPARAM lParam)
{
	bool IsOK = true;
	switch ( lParam)
	{
	case USER_LEVEL_OPERATOR:	IsOK = AOIDataCollect.UserLogin_Operator();	break;
	case USER_LEVEL_ENGINEER:	IsOK = AOIDataCollect.UserLogin_Engineer();	break;
	case USER_LEVEL_SUPERVISOR:	IsOK = AOIDataCollect.UserLogin_Supervisor();	break;
	case USER_LEVEL_JET_FAE:	IsOK = AOIDataCollect.UserLogin_VendorJET();	break;
	case USER_LEVEL_JET_SENIOR: IsOK = AOIDataCollect.UserLogin_VendorJET();	break;
	case USER_LEVEL_JET_RD:		IsOK = AOIDataCollect.UserLogin_VendorJET();	break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecShowMessageWnd(LPARAM lParam)
{
	UINT nType=0, nIDHelp=0;
	CString Str=AOIDataCollect.GetMessageInfo(nType, nIDHelp);
	//const int Ret=JetAPI::ShowMessageBox(Str, nType, nIDHelp);
	CMessageBoxWnd MsgWnd;
	MsgWnd.SetFontInfo(24);
	MsgWnd.SetTextColor(0x0000FF);
	const int Ret=MsgWnd.ShowMessageWnd(Str, nType, nIDHelp);
	AOIDataCollect.SetMessageReturn(Ret);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnResetControlCenter()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if (AOIDataCollect.OperateLevelEditFuncControlCenter() == false)
	{	return ; }
	ExecResetControlCenter();	
	AOIDataCollect.UserLogout_Check();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateResetControlCenter(CCmdUI* pCmdUI)
{
#ifndef OFFLINE_VERSION
	ExecUpdateUI_System(pCmdUI);
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecResetControlCenter()
{
	size_t          i=0;
	POINT           Point;
	CString         strLabel;
	CString         strCaption;
	TListNode     Node;	
	CInputListWnd EnumWnd;
	std::vector<TListNode> NodelList;	
	const DWORD_PTR OldLaneIndex = LANE_ID_NULL;
	
	strCaption = _T("Set Lane Wnd");
	strLabel = AOIDataDefine.GetLaneText();	
	Node.Data = LANE_ID_NULL;	Node.Text = AOIDataDefine.GetAllText();	NodelList.push_back(Node);
	Node.Data = LANE_ID_A;	Node.Text = AOIDataDefine.GetLaneIDText(LANE_ID_A);	NodelList.push_back(Node);
	Node.Data = LANE_ID_B;	Node.Text = AOIDataDefine.GetLaneIDText(LANE_ID_B);	NodelList.push_back(Node);
	
	//::GetCursorPos(&Point);
	//ComboxWnd.SetWndPos(Point);
	EnumWnd.SetParam1(strCaption, strLabel, OldLaneIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return true; }

	const int nValue = (int)(EnumWnd.GetSelData());		
	switch ( nValue )
	{
	case LANE_ID_A:
		AOIDataCollect.ClearCCSDateTime_LA();
		break;
	case LANE_ID_B:
		AOIDataCollect.ClearCCSDateTime_LB();
		break;
	default:
		AOIDataCollect.ClearCCSDateTime_LA();
		AOIDataCollect.ClearCCSDateTime_LB();
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecProjectSwitchDistrict(DISTRICT_ID DistrictID)//執行切換專案段落
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);		
	
	bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	if ( false == OfflineMode ) 
	{
		AOIDataCollect.SetActiveDistrictID(DistrictID);
		AOIDataCollect.MovePCBToDistrictID(LaneID, DistrictID);	
	}
	else
	{
		const bool bLoadFrame = true;
		const bool bDistrictChange = true;
		OFFLINE_FILE_MODE OfflineMode = ProjectPtr->GetProjectOfflineFileMode();		
		if ( OFFLINE_FILE_PROGRAM == OfflineMode )
		{
			ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();
			ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();
			ProjectPtr->SwtichProjectOfflineFileMode(OfflineMode, bDistrictChange, bLoadFrame);
		}
		AOIDataCollect.ReleaseFieldUniFrameList();
		AOIDataCollect.ReleaseModelUniFrameList();
	}
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH_MAP, NULL);
	CWnd::PostMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
	CWnd::PostMessage(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecUpdateProjectDistrictActive(CCmdUI* pCmdUI, DISTRICT_ID DistrictID)//執行切換專案段落
{
	const bool bLockUIWnd = GetLockUIWnd();
	const bool bRemoteState = CheckMesCtrlState_Remote();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	
		const DISTRICT_ID ActDistrictID=ProjectPtr->GetProjectActDistrictID();
		const bool MultiDistrictMode=ProjectPtr->GetProjectMultiDistrictMode();
		if ( true == MultiDistrictMode )
		{	
			pCmdUI->Enable(TRUE);	
			if ( DistrictID == ActDistrictID )
			{	pCmdUI->SetCheck(TRUE);	}
			else
			{	pCmdUI->SetCheck(FALSE);	}
		}
		else
		{	pCmdUI->Enable(FALSE); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectDistrictActiveA()
{
	ExecProjectSwitchDistrict(DISTRICT_ID_A);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectDistrictActiveA(CCmdUI* pCmdUI)
{
	ExecUpdateProjectDistrictActive(pCmdUI, DISTRICT_ID_A);	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnProjectDistrictActiveB()
{
	ExecProjectSwitchDistrict(DISTRICT_ID_B);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateProjectDistrictActiveB(CCmdUI* pCmdUI)
{
	ExecUpdateProjectDistrictActive(pCmdUI, DISTRICT_ID_B);	
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewRemoteParamWnd()
{
	bool bLockUIWnd = GetLockUIWnd();		
	const bool RemoteTuning = AOIDataCollect.GetIsOfflineRemoteTuningVersion();
	if ( true==bLockUIWnd || false==RemoteTuning )
	{	return true; }

	CRemoteParamWnd Wnd;
	std::vector<TRemoteParam> RemoteParamList;	
	AOIDataCollect.GetSystemRemoteParamList(RemoteParamList);
	Wnd.SetRemoteParamList(RemoteParamList);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return false; }

	CString str;
	size_t i=0;
	
	Wnd.GetRemoteParamList(RemoteParamList);		
	AOIDataCollect.SetSystemRemoteParamList(RemoteParamList);
	AOIDataCollect.SaveSystemRemoteParameter();	

	bool  bIsOK=false;
	const size_t RemoteParamCount=RemoteParamList.size();
	const size_t ActIndex=AOIDataCollect.GetRemoteParamActivedIndex(RemoteParamList);	
	if ( ActIndex>=0 && ActIndex<RemoteParamCount )
	{		
		TRemoteParam RemoteParam=RemoteParamList[ActIndex];		
		bIsOK = AOIDataCollect.LoadSystemParamFileFromRemoteParam(RemoteParam);
		if ( false == bIsOK )		
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
		}
	}

	if ( true == bIsOK )
	{
		ExecProjectCloseAll(false);		
		OnProjectOpen();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewRemoteParamWnd()
{
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }
	if ( AOIDataCollect.OperateLevelEditWndRemoteParam() == false )
	{	return;	}
	ExecViewRemoteParamWnd();	
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewRemoteParamWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	const bool RemoteTuning = AOIDataCollect.GetIsOfflineRemoteTuningVersion();
	if ( true==bLockUIWnd || false==RemoteTuning || true==bRemoteState )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUserSignIn()
{
	const bool bForce = true;
	AOIDataCollect.UserLogin(USER_LEVEL_ENGINEER, bForce);	
//	UpdateDocumentTitle();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateUserSignIn(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	USER_LEVEL_MODE UserLevelMode = AOIDataCollect.GetCurrentUserLevel();	
	if ( USER_LEVEL_SIGN_OUT!=UserLevelMode || true==bLockUIWnd)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUserSignOut()
{
	CString  str;
	str = _T("Do you want to sign out?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	AOIDataCollect.UserLogout();//使用者登出
//	UpdateDocumentTitle();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateUserSignOut(CCmdUI* pCmdUI)
{	
	bool bLockUIWnd = GetLockUIWnd();
	USER_LEVEL_MODE UserLevelMode = AOIDataCollect.GetCurrentUserLevel();	
	if ( USER_LEVEL_SIGN_OUT==UserLevelMode || true==bLockUIWnd)
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecSetFdSortWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return true; }

	CFdSortWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr, FD_SORT_SCOPE_PANEL);
	Wnd.DoModal();
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetFdSortWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }		
	if ( AOIDataCollect.OperateLevelEditWndProjectFdSort() == false )
	{	return; }
	ExecSetFdSortWnd();
	AOIDataCollect.UserLogout_Check();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetFdSortWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecSetFdSortWndBoard()
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( true==bLockUIWnd || NULL==ProjectPtr )
	{	return true; }

	CFdSortWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr, FD_SORT_SCOPE_BOARD);
	Wnd.DoModal();
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetFdSortWndBoard()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }		
	if ( AOIDataCollect.OperateLevelEditWndProjectFdSort() == false )
	{	return; }
	ExecSetFdSortWndBoard();
	AOIDataCollect.UserLogout_Check();	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetFdSortWndBoard(CCmdUI* pCmdUI)
{
	ExecUpdateUI_ProjectParam(pCmdUI);
}
//-------------------------------------------------------------------------------------//

void CMainFrame::OnSetProjectModeNormal()
{
	AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetProjectModeNormal(CCmdUI* pCmdUI)
{
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();
	if ( PROJECT_TASK_NORMAL == ProjectTaskMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }

	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnSetProjectModeOpenBarcode()
{
#ifdef ONLINE_OPEN_PROJECT_USE
	AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_OPEN_BARCODE);
#endif//ONLINE_OPEN_PROJECT_USE
	AOIDataCollect.ResetOnlineOpenProjectCameraBarcodeTestState();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSetProjectModeOpenBarcode(CCmdUI* pCmdUI)
{
	PROJECT_TASK_MODE ProjectTaskMode = AOIDataCollect.GetProjectTaskMode();
	if ( PROJECT_TASK_OPEN_BARCODE == ProjectTaskMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }	

	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	const bool bRemoteState = CheckMesCtrlState_Remote();
	//CAOIProject *ProjectPtr = AOIDataCollect.GetOpenProjectProjectPtr();
	if ( true == bLockUIWnd || true==bRemoteState )
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
#ifndef ONLINE_OPEN_PROJECT_USE
	bEnable = FALSE;
#endif//ONLINE_OPEN_PROJECT_USE
	pCmdUI->Enable(bEnable);	
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewProjectOpenCodeListWnd()
{
	CProjectCodeListWnd Wnd;	
	CString      TxtFilename;
	std::vector<TProjectOpenCode> OpenCodeList;	
	TxtFilename = AOIDataCollect.GetProjectOpenCodeFilename();
	AOIDataCollect.LoadProjectOpenCodeFile(TxtFilename, OpenCodeList);
	Wnd.SetOpenCodeList(OpenCodeList);
	if ( Wnd.DoModal() != IDOK )
	{	return true; }	

	Wnd.CloneOpenCodeList(OpenCodeList);
	AOIDataCollect.UpdateProjectOpenCode(OpenCodeList);
	AOIDataCollect.SaveProjectOpenCodeFile(TxtFilename, OpenCodeList);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewProjectOpenCodeListWnd()
{
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }	
	if ( AOIDataCollect.OperateLevelEditWndProjectOpenCode() == false )
	{	return;	}
	ExecViewProjectOpenCodeListWnd();
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewProjectOpenCodeListWnd(CCmdUI* pCmdUI)
{	
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecViewOperatorLogWnd()
{
	CLogOperViewerWnd Wnd;		
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();
	if ( UserLevel < USER_LEVEL_SUPERVISOR )
	{	Wnd.SetShowFolder(false);	}
	if ( Wnd.DoModal() != IDOK )
	{	return true; }	
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnViewOperatorLogWnd()
{
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }	
	if ( AOIDataCollect.OperateLevelEditWndOperatorLog() == false )
	{	return;	}
	ExecViewOperatorLogWnd();
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateViewOperatorLogWnd(CCmdUI* pCmdUI)
{
	ExecUpdateUI_System(pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnMesSetCtrlStateOffline()
{
	// TODO: Add your command handler code here
	ExecMesSetCtrlState(MES_EQP_CTRL_STATE_OFFLINE);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateMesSetCtrlStateOffline(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ExecUpdateMesSetCtrlState(MES_EQP_CTRL_STATE_OFFLINE, pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnMesSetCtrlStateLocal()
{
	// TODO: Add your command handler code here
	ExecMesSetCtrlState(MES_EQP_CTRL_STATE_LOCAL);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateMesSetCtrlStateLocal(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ExecUpdateMesSetCtrlState(MES_EQP_CTRL_STATE_LOCAL, pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnMesSetCtrlStateRemote()
{
	// TODO: Add your command handler code here
	ExecMesSetCtrlState(MES_EQP_CTRL_STATE_REMOTE);
	AOIDataCollect.UserLogout_Check();
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateMesSetCtrlStateRemote(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ExecUpdateMesSetCtrlState(MES_EQP_CTRL_STATE_REMOTE, pCmdUI);
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecMesSetCtrlState(MES_EQP_CTRL_STATE_MODE Mode)
{
#ifndef MES_DISABLE		
	if ( AOIDataCollect.OperateLevelMesCtrlState(Mode) == false ) { return; }
	if ( Mode == AOIDataCollect.GetMES_EqpCtrlStateMode() ) { return; }
	if ( AOIDataCollect.ExecMESComm_SetControlStateMode(Mode) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return ; 
	}
#endif//MES_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::ExecUpdateMesSetCtrlState(MES_EQP_CTRL_STATE_MODE Mode, CCmdUI* pCmdUI)
{
#ifdef MES_DISABLE
	pCmdUI->Enable(FALSE);
#else	
	BOOL bCheck=TRUE;	
	BOOL bEnable=TRUE;				
	if ( AOIDataCollect.GetMES_SecsGemEnabled() == false ||
		 AOIDataCollect.GetMES_CommunicationEnabled() == false )
	{	
		bCheck = FALSE;
		bEnable = FALSE; 		
	}
	if ( Mode != AOIDataCollect.GetMES_EqpCtrlStateMode() )
	{	bCheck = FALSE;	}
	if ( TASK_STATE_RUNNING == AOIDataCollect.GetOnlineTaskState() )
	{	bEnable = FALSE; }	
	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(bCheck);
#endif//MES_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_ShowMsg() const
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_SetSystemParam()
{
	const bool bShowMsg=ExecMESComm_ShowMsg();
	if ( AOIDataCollect.ExecMESComm_SetSystemParam() == false )
	{
		if ( true == bShowMsg )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	 }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_SetProjectParam()
{
	const bool bShowMsg=ExecMESComm_ShowMsg();
	if ( AOIDataCollect.ExecMESComm_SetProjectParam() == false )
	{
		if ( true == bShowMsg )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	 }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_SetUserLogin_out(bool bLogin)
{
	const bool bShowMsg=ExecMESComm_ShowMsg();
	if ( AOIDataCollect.ExecMESComm_SetUserLogin_out(bLogin) == false )
	{
		if ( true == bShowMsg )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	 }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_LoadProject()
{	
	const bool bShowMsg=ExecMESComm_ShowMsg();	
	if ( AOIDataCollect.ExecMESComm_LoadProject() == false )
	{
		if ( true == bShowMsg )
		{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	 }
	}
	CWnd::PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMainFrame::ExecMESComm_ShowMessageWnd(DWORD ShowType)
{
#ifndef MES_DISABLE
	if ( ITSCommWnd.GetSafeHwnd() == NULL ) { return true; }
	const bool bShowMsg=ExecMESComm_ShowMsg();
	CString MesMessage=AOIDataCollect.GetMES_RemoteCtrlMessage();
	ITSCommWnd.SetMessage(MesMessage);
	if ( ITSCommWnd.IsWindowVisible() == false )
	{	ITSCommWnd.ShowWindow(SW_SHOW); }

	const int ShowTypeSecondsDisplay=1;
	if ( ShowTypeSecondsDisplay == ShowType )
	{	ITSCommWnd.SetShowTimer(5000);	}
#endif//MES_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnHotKeyRotateObj()
{
	// TODO: Add your command handler code here		
	bool bLockUIWnd = GetLockUIWnd();		
	if ( true==bLockUIWnd )
	{	return; }	
	CView *ActViewPtr = GetActiveView();
	if ( NULL == ActViewPtr ) { return; }	
	UINT ViewID = AOIDataCollect.GetMainViewWndID();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	CUserHotKeyItem  *HotKeyItemPtr=UserHotKeyCtrl.FindUserHotKeyPtrByFuncID(USER_HOT_KEY_ROTATE_OBJ);
	if ( NULL == HotKeyItemPtr ) { return ; }
	if ( IDD_EDIT_MODEL_VIEW == ViewID )
	{	
		if ( NULL == ProjectPtr ) { return; }		
		const double RotateAngle = HotKeyItemPtr->GetExecID();
		if ( fabs(RotateAngle) < 0.00001 ) { return; }
		ProjectPtr->RotateProjectComponentSelected(RotateAngle);
		//ActViewPtr->PostMessage(WM_COMMAND, MENU_MODEL_EDIT_ROTATE_090, NULL);	
		ActViewPtr->PostMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);	
	}	
}
//-------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateHotKeyRotateObj(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//