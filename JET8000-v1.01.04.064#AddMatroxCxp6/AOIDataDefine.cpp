// AOIDataDefine.cpp: implementation of the CAOIDataDefine class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AOIDataDefine.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CAOIDataDefine AOIDataDefine;
//-------------------------------------------------------------------------------------//
unsigned int BitPlanes[24][8] =
{
    { 0x01, 0x03, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G0 Bit-depth 1-8
    { 0x02, 0x03, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G1 Bit-depth 1-8
    { 0x04, 0x0C, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G2 Bit-depth 1-8
    { 0x08, 0x0C, 0x38, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G3 Bit-depth 1-8
    { 0x10, 0x30, 0x38, 0xF0, 0x3E, 0x3F, 0xFE, 0xFF },  //G4 Bit-depth 1-8
    { 0x20, 0x30, 0x38, 0xF0, 0x3E, 0x3F, 0xFE, 0xFF },  //G5 Bit-depth 1-8
    { 0x40, 0xC0, 0x1C0, 0xF0, 0xF80, 0xFC0,0xFE, 0xFF },  //G6 Bit-depth 1-8
    { 0x80, 0xC0, 0x1C0, 0xF0, 0xF80, 0xFC0,0xFE, 0xFF }, //G7 Bit-depth 1-8


    { 0x0100, 0x0300, 0x1C0, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R0 Bit-depth 1-8
    { 0x0200, 0x0300, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R1 Bit-depth 1-8
    { 0x0400, 0x0C00, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R2 Bit-depth 1-8
    { 0x0800, 0x0C00, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R3 Bit-depth 1-8
    { 0x1000, 0x3000, 0x7000, 0xF000, 0x3E000,        0x3F000, 0xFE00, 0xFF00 },  //R4 Bit-depth 1-8
    { 0x2000, 0x3000, 0x7000, 0xF000, 0x3E000,  0x3F000, 0xFE00, 0xFF00 },  //R5 Bit-depth 1-8
    { 0x4000, 0xC000, 0x7000, 0xF000, 0x3E000, 0x3F000, 0xFE00, 0xFF00 },  //R6 Bit-depth 1-8
    { 0x8000, 0xC000, 0x38000, 0xF000, 0x3E000, 0x3F000, 0xFE00, 0xFF00 }, //R7 Bit-depth 1-8

    { 0x010000, 0x030000, 0x38000, 0xF0000,   0x3E000,  0x3F000,  0xFE0000, 0xFF0000 },  //B0 Bit-depth 1-8
    { 0x020000, 0x030000, 0x38000, 0xF0000,   0x3E000,  0x3F000,  0xFE0000, 0xFF0000 },  //B1 Bit-depth 1-8
    { 0x040000, 0x0C0000, 0x1C0000, 0xF0000,  0xF80000,   0xFC0000, 0xFE0000, 0xFF0000 },  //B2 Bit-depth 1-8
    { 0x080000, 0x0C0000, 0x1C0000, 0xF0000,  0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B3 Bit-depth 1-8
    { 0x100000, 0x300000, 0x1C0000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B4 Bit-depth 1-8
    { 0x200000, 0x300000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B5 Bit-depth 1-8
    { 0x400000, 0xC00000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B6 Bit-depth 1-8
    { 0x800000, 0xC00000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B7 Bit-depth 1-8
};
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CAOIDataDefine::CAOIDataDefine()
{
	PreInitDefine();
	InitialDefine();
	LoadDefineTextFile();
	BuildWndDefectIDMapIndex();
}
//-------------------------------------------------------------------------------------//
//CAOIDataDefine::CAOIDataDefine(const CAOIDataDefine &Define)
//{
//	PreInitDefine();
//	CloneDefine(Define);
//}
//-------------------------------------------------------------------------------------//
CAOIDataDefine::~CAOIDataDefine()
{

}
//-------------------------------------------------------------------------------------//
//CAOIDataDefine& CAOIDataDefine::operator=(const CAOIDataDefine &Define)
//{
//	if ( this == &Define ) { return *this; }
//	CAOIDataDefine::CloneDefine(Define);
//	return *this;
//}
//-------------------------------------------------------------------------------------//
void CAOIDataDefine::PreInitDefine()
{
}
//-------------------------------------------------------------------------------------//
void CAOIDataDefine::InitialDefine()
{
	//ID
	m_IDText = _T("ID");
	m_ToText = _T("To");
	//Fiducial
	m_FdText = _T("Fd");
	m_AddText = _T("Add");//新增文字
	m_SetText = _T("Set");
	m_GetText = _T("Get");
	m_PadText = _T("Pad");
	m_WndText = _T("Wnd");//檢測框文字	
	m_AveText = _T("Ave");
	m_AllText = _T("All");
	m_MaxText = _T("Max");
	m_MinText = _T("Min");
	m_FromText = _T("From");
	m_AxisText = _T("Axis");
	m_LineText = _T("Line");	
	m_AreaText = _T("Area");
	m_LaneText = _T("Lane");
	m_TimeText = _T("Time");
	m_TestText = _T("Test");
	m_NameText = _T("Name");	
	m_SkewText = _T("Skew");
	m_MarkText = _T("Mark");
	m_GainText = _T("Gain");
	m_GrayText = _T("Gray");
	m_SizeText = _T("Size");
	m_LandText = _T("Land");
	m_RangeText = _T("Range");
	m_ClassText = _T("Class");
	m_ClearText = _T("Clear");
	m_RatioText = _T("Ratio");
	m_IndexText = _T("Index");	
	m_ErrorText = _T("Error");
	
	m_PanelText = _T("Panel");
	m_BoardText = _T("Board");
	m_ModelText = _T("Model");
	m_GroupText = _T("Group");
	m_ScoreText = _T("Score");	
	m_ScaleText = _T("Scale");
	m_YieldText = _T("Yield");
	m_PixelText = _T("Pixel");//像素文字
	m_SizeXText = _T("Size X");//X尺寸文字
	m_SizeYText = _T("Size Y");//Y尺寸文字

	m_MethodText = _T("Method");
	m_TowardText = _T("Toward");
	m_CloneText = _T("Clone");
	m_CreateText = _T("Create");
	m_ModifyText = _T("Modify");
	m_DeleteText = _T("Delete");
	m_DefectText = _T("Defect");
	m_ResultText = _T("Result");
	m_UnsetText = _T("Unset");
	m_AngleText = _T("Angle");
	m_WidthText = _T("Width");
	m_HeightText = _T("Height");
	m_VolumeText = _T("Volume");
	m_OffsetText = _T("Offset");
	m_FinishText = _T("Finish");
	m_ProjectText = _T("Project");
	m_PatternText = _T("Pattern");
	m_DefaultText = _T("Default");
	m_ThroughText = _T("Through");
	m_BarcodeText = _T("Barcode");
	m_WarningText = _T("Warning");
	m_GroupIDText = _T("Group ID");//群組編號文字	
	m_BypassedText = _T("Bypassed");
	m_DistrictText = _T("District");
	m_DistanceText = _T("Distance");
	m_VelocityText = _T("Velocity");
	m_RelativeText = _T("Relative");
	m_ContinueText = _T("Continue");
	m_ContrastText = _T("Contrast");
	m_ThicknessText = _T("Thickness");
	m_AlgorithmText = _T("Algorithm");//演算法文字
	m_LockScreenText = _T("Lock Screen");	 
	m_VerticalText = _T("Vertical");
	m_HorizontalText = _T("Horizontal");	
	m_ComponentText = _T("Component");
	m_PartNumberText = _T("Part Number");
	m_NozzleNameText = _T("Nozzle Name");

	//Undefined
	m_UndefinedText = _T("Undefined");
	m_AccelerationText = _T("Acceleration");
	//Barcode Device
	m_BarcodeDeviceText = _T("Barcode Device");
	//Safty Bypass
	m_SaftyBypassText = _T("Safty Bypass");
	m_WaitForCCSText = _T("Wait for CCS");
	m_WaitForRepairText = _T("Wait for Repair");	
	//Inspection
	m_InspectionResultFaultText = _T("Error, Project Test Result NG");//檢測結果異常
	m_DoYouWantToClearTheStatisticRecordsText = _T("Do you want to clear the statistic records?");//是否清除統計資料
	//OK/Cancel
	m_OKText = _T("OK");
	m_CancelText = _T("Cancel");
	m_DecodeText = _T("Decode");
	m_LevelText = _T("Level");
	//Enable/Disable
	m_EnableText = _T("Enable");
	m_DisabeText = _T("Disable");
	//Positive/Negative
	m_PositiveText = _T("Pos.");
	m_NegativeText = _T("Neg.");
	//DISTRICT_ID
	m_DistrictText_A = _T("A");
	m_DistrictText_B = _T("B");
	//WND_DEFECT_ITEM
	m_WndDefectItem_Disable = _T("Disable");
	m_WndDefectItem_Enable = _T("Enable");
	m_WndDefectItem_NoShow = _T("No Show");
	//Grab//Count//Calculate
	m_GrabText = _T("Grab");//取像文字
	m_CountText = _T("Count");//數量文字
	m_CalculateText = _T("Calculate");//計算文字
	m_VersionCodeText = _T("Version");//版本號文字
	//COLOR_MODE
	m_ColorText_Red=_T("Red");//顏色文字-紅色
	m_ColorText_Green=_T("Green");//顏色文字-綠色
	m_ColorText_Blue=_T("Blue");//顏色文字-藍色
	m_ColorText_Black=_T("Black");//顏色文字-黑色	
	m_ColorText_Gray = _T("Gray");//顏色文字-灰色	
	m_ColorText_White=_T("White");//顏色文字-白色	
	m_ColorText_Color=_T("Color");//顏色文字-彩色
	//USER_LEVEL_MODE
	m_UserLevelText_SignOut = _T("Sign Out");//使用者權限-登出
	m_UserLevelText_Operator = _T("Operaotr");//使用者權限-作業員
	m_UserLevelText_Engineer = _T("Engineer");//使用者權限-工程師
	m_UserLevelText_Supervisor = _T("Supervisor");//使用者權限-管理者
	m_UserLevelText_JETFAE = _T("JET FAE");//使用者權限-JET廠商
	m_UserLevelText_JETSENIOR = _T("JET Senior");//使用者權限-JET廠商
	//USER_LOGIN_MODE
	m_UserLoginText_Disable  = _T("Disable");//使用者登入模式-關閉
	m_UserLoginText_Operator = _T("Operator");//使用者登入模式-作業員
	m_UserLoginText_Engineer = _T("Engineer");//使用者登入模式-工程師	
	//USER_LOGIN_OPTIONS
	m_UserLoginOptionsText_Password = _T("Password");//使用者登入選項-密碼
	m_UserLoginOptionsText_Fingerprint = _T("Fingerprint");//使用者登入選項-指紋
	m_UserLoginOptionsText_FingerprintOnly = _T("Only Fingerprint");//使用者登入選項-兩者
	//OPEN_PROJECT_MODE
	m_OpenProjectText_File = _T("File");
	m_OpenProjectText_Code = _T("Code");
	//VERIFY_PROJECT_MODE
	m_VerifyProjectText_Disable = _T("Disable");
	m_VerifyProjectText_Filename = _T("Filename");	
	//ONLINE_INPUT_TIMING
	m_OnlineInputTiming_Disalbe = _T("Disable");
	m_OnlineInputTiming_BeforeSignIn = _T("Before Sign-In");	
	//AOI_CUSTOMER_ID
	m_AOICustomerIDText_JET_TWN = _T("JET Taiwan");
	m_AOICustomerIDText_PegaTron_TWN = _T("PegaTron Taiwan");
	m_AOICustomerIDText_Kinpo_YueYang = _T("Kinpo YueYang");
	m_AOICustomerIDText_Foxconn_LongHua = _T("Foxconn LongHua");	
	//OFFLINE_VERSION_MODE
	m_OfflineVersionText_Normal = _T("Normal");
	m_OfflineVersionText_HostTuning = _T("Host Tuning");
	m_OfflineVersionText_RemoteTuning = _T("Remote Tuning");
	//ONLINE_OPEN_PROJECT_MODE
	m_OnlineOpenProjectText_Disable = _T("Disable");
	m_OnlineOpenProjectText_BarcodeDevice = _T("Barcode Device");
	m_OnlineOpenProjectText_BarcodeHandHeld = _T("Handheld Reader");
	m_OnlineOpenProjectText_BarcodeCamera = _T("Camera Barcode");
	//PROJECT_LINK_SERVER_MODE
	m_ProjectLinkServerText_Disable = _T("Disable");
	m_ProjectLinkServerText_EnableAll = _T("Enable All");
	m_ProjectLinkServerText_ProjectOnly = _T("Project Only"); 
	m_ProjectLinkServerText_EnableAllAsk = _T("Enable Ask");
	m_ProjectLinkServerText_ProjectOnlyAsk = _T("Project Ask"); 
	//XBOARD_MAPPING_FILE_MODE
	m_XBoardMappingFileText_Disable = _T("Disable");
	m_XBoardMappingFileText_MES_Comm = _T("MES Comm");
	//XBOARD_MAPPING_FILE_FLOW
	m_XBoardMappingFileFlowText_Default = _T("Default");
	m_XBoardMappingFileFlowText_After_Barcode = _T("After Barcode");
	//Lane ID
	m_LaneText_A = _T("A");
	m_LaneText_B = _T("B");
	//Multi Lane Mode
	m_MultiLaneText_1 = _T("1");
	m_MultiLaneText_2 = _T("2");
	//FUNC_EXEC_MODE
	m_FuncExecText_Off = _T("Off");
	m_FuncExecText_Auto= _T("Auto");
	m_FuncExecText_Ask = _T("Ask");	
	//Lane Work Mode
	m_LaneWorkText_Disable = _T("Disable");
	m_LaneWorkText_Run = _T("Run");
	m_LaneWorkText_Bypass = _T("Bypass");
	//CONNECTED_BUFFER_TYPE
	m_ConnectedBufferType_Fixed = _T("Fixed");
	m_ConnectedBufferType_Movable = _T("Movable");
	//M2M AOI Stage
	m_HASI_AOIStage_Pre = _T("Pre-Reflow");
	m_HASI_AOIStage_Post = _T("Post-Reflow");
	m_HASI_State_mode_STOP = _T("STOP");
	m_HASI_State_mode_AC = _T("AC");
	m_HASI_State_mode_SC = _T("SC");
	m_HASI_State_mode_SCAC = _T("SCAC");
	m_HASI_State_mode_RUN = _T("RUN");
	m_HASI_State_mode_TEST = _T("TEST");
	//PCB Out
	m_PCBOutText_Normal = _T("Normal");
	m_PCBOutText_SideOut = _T("Side->Out");
	m_PCBOutText_WithIn = _T("Out->In");
	m_PCBOutText_LaneAuto= _T("Lane Auto");
	m_PCBOutText_OkOutNgSide = _T("Ok Out, Ng Side");
	//PCB Out Direction
	m_PCBOutDirText_Forward = _T("Forward");
	m_PCBOutDirText_Backward = _T("Backward");
	m_PCBOutDirText_BackwardOut = _T("Backward-Out");
	//PCB Side Mode
	m_PanelSideText_Top = _T("Top");//板面-上面
	m_PanelSideText_Bottom = _T("Bottom");//板面-下面
	m_PanelSideText_Hybrid = _T("Hybrid");//板面-雙面	
	//Board Side Mode
	m_BoardSideText_Top = _T("Top");//板面-上面
	m_BoardSideText_Bottom = _T("Bottom");//板面-下面
	//BARCODE_DECODER_TYPE
	m_BarcodeDecoder_EVS = _T("EVS");//Open-Evision
	m_BarcodeDecoder_DTK = _T("DTK");//DTK
	m_BarcodeDecoder_HON = _T("Honeywell");//Honeywell
	//BARCODE_SPREAD_MODE
	m_BarcodeSpread_Off = _T("Off");
	m_BarcodeSpread_Local= _T("Local");
	m_BarcodeSpread_All  = _T("All");
	//BARCODE_BELONG_MODE
	m_BarcodeBelong_None = _T("None");
	m_BarcodeBelong_Project = _T("Project");
	m_BarcodeBelong_Panel = _T("Panel");
	m_BarcodeBelong_Board = _T("Board");
	m_BarcodeBelong_Tray = _T("Tray");
	m_BarcodeBelong_Cover = _T("Cover");
	//Save Test Map Mode
	m_SaveTestMapText_Disable = _T("Disable");
	m_SaveTestMapText_Prog = _T("Project");
	m_SaveTestMapText_Panel = _T("Panel");
	m_SaveTestMapText_ProgPanel = _T("Prog+Panel");
	m_SaveTestMapText_Board = _T("Board");
	m_SaveTestMapText_ProgBoard = _T("Prog+Board");
	//Offline Image Scope
	m_OfflineImageText_Fov = _T("FOV");
	m_OfflineImageText_Part = _T("Part");
	//Save Test Image Mode
	m_SaveTestImageText_Disable  = _T("Disable");//不儲存
	m_SaveTestImageText_EveryOne = _T("EveryOne");//總是儲存
	m_SaveTestImageText_Defect   = _T("Defect");//瑕疵儲存	
	//Save Test Data Mode
	m_SaveTestDataText_Disable = _T("Disable");
	m_SaveTestDataText_Enable = _T("Enable");
	m_SaveTestDataText_Defect = _T("Defect");	
	//Fd NG Handle Mode
	m_FdNGHandleText_None = _T("None");
	m_FdNGHandleText_Pass = _T("Pass");
	m_FdNGHandleText_Stop = _T("Stop");	
	m_FdNGHandleText_XBoard = _T("X-Board");	
	//Board Fd Grab Mode
	m_BoardFdGrabText_AfterPanel = _T("After Panel");
	m_BoardFdGrabText_Inspecting = _T("Inspecting");
	//Defect Handle Mode	
	m_DefectHandleText_Pass = _T("Pass");
	m_DefectHandleText_Stop = _T("Stop");
	m_DefectHandleText_Next = _T("Next");
	m_DefectHandleText_Repair = _T("Wair for Repair");
	m_DefectHandleText_ControlCenter = _T("Control Center");
	//Online State Mode
	m_OnlineState_InspectionStop = _T("Inspection Stop");
	m_OnlineState_PCBReady = _T("PCB Ready");
	m_OnlineState_InputBarcode = _T("Input Barcode");
	m_OnlineState_ProjectMap = _T("Project Map");
	m_OnlineState_ProjectMark = _T("Project Mark");
	m_OnlineState_ProjectOpenCode = _T("Project Open Barcode");
	m_OnlineState_ProjectReload = _T("Project Reload");
	m_OnlineState_ProjectReloadServer = _T("Project Reload Server");
	m_OnlineState_ProjectSwitchByTurn = _T("Project Switch By Turn");
	m_OnlineState_ProjectSwitchByTurnOneCycleReset = _T("Project Switch By Turn One Cycle Reset");
	m_OnlineState_InspectionStart = _T("Inspection Start");
	m_OnlineState_InspectionWaittng = _T("Inspecting");
	m_OnlineState_InspectFDPanel = _T("Inspect Panel Fiducial");
	m_OnlineState_InspectFDBoard = _T("Inspect Board Fiducial");
	m_OnlineState_InspectBarcode = _T("Inspect Barcode");
	m_OnlineState_InspectProject = _T("Inspect Project");
	m_OnlineState_StatisticProject = _T("Statistic Project");
	m_OnlineState_InspectionFinish = _T("Inspection Finish");	
	m_OnlineState_WaitForLast = _T("Wait for Last Station");
	m_OnlineState_WaitForNext = _T("Wait for Next Station");
	m_OnlineState_WaitForPCBRemoved = _T("Wait for PCB Removed");
	m_OnlineState_WaitForRepairVerify = _T("Wait for Repair Verify");
	m_OnlineState_PCBInStart = _T("PCB In Start");
	m_OnlineState_PCBInChecking = _T("PCB In Checking");
	m_OnlineState_PCBInFinish = _T("PCB In Finish");
	m_OnlineState_PCBOutStart = _T("PCB Out Start");
	m_OnlineState_PCBOutChecking = _T("PCB Out Checking");
	m_OnlineState_PCBOutFinish = _T("PCB-Out Finish");
	m_OnlineState_PCBOutInsideStart = _T("PCB Out Inside Start");
	m_OnlineState_PCBOutInsideChecking = _T("PCB Out Inside Checking");
	m_OnlineState_PCBOutInsideFinish = _T("PCB Out Inside Finish");
	m_OnlineState_PCBBackStart = _T("PCB Back Start");
	m_OnlineState_PCBBackChecking = _T("PCB Back Checking");
	m_OnlineState_PCBBackFinish = _T("PCB Back Finish");
	m_OnlineState_PCBBackOutStart = _T("PCB Back-Out Start");
	m_OnlineState_PCBBackOutChecking = _T("PCB Back-Out Checking");
	m_OnlineState_PCBBackOutFinish = _T("PCB Back-Out Finish");
	m_OnlineState_PCBOutInStart = _T("PCB Out-In Start");
	m_OnlineState_PCBOutInChecking = _T("PCB Out-In Checking");
	m_OnlineState_PCBOutInFinish = _T("PCB Out-In Finish");	
	m_OnlineState_PCBAutoRunStart = _T("PCB Auto-Run Start");	
	m_OnlineState_PCBAutoRunChecking = _T("PCB Auto-Run Checking");	
	m_OnlineState_PCBAutoRunFinish = _T("PCB Auto-Run Finish");	
	m_OnlineState_PCBDualRunStart = _T("PCB Dual-Run Start");	
	m_OnlineState_PCBDualRunChecking = _T("PCB Dual-Run Checking");	
	m_OnlineState_PCBDualRunFinish = _T("PCB Dual-Run Finish");	
	m_OnlineState_PCBInspectionPause = _T("PCB Inspection Pause ");
	m_OnlineState_AutoCalibration_XYZ_Home = _T("Auto Calibration-XYZ Home");//自動校正-XYZ歸零
	m_OnlineState_AutoCalibration_2D_Current = _T("Auto Calibration-2D Current");;//自動校正-2D電流
	m_OnlineState_AutoCalibration_3D_Current = _T("Auto Calibration-3D Current");;//自動校正-3D電流
	m_OnlineState_AutoCalibration_3D_ZeroPlane = _T("Auto Calibration-3D Zero Plane");;//自動校正-3D相平面
	m_OnlineState_AutoCalibration_3D_FactorFactor = _T("Auto Calibration-3D Factor Factor");;//自動校正-3D高度比例
	m_OnlineState_AppOpen = AOI3D_APP_CAT(" Open");//_T("JET8000 Open");	
	m_OnlineState_AppClose = AOI3D_APP_CAT(" Close");//_T("JET8000 Close");	
	//MES_EQP_CTRL_STATE_MODE
	m_MesEqpCtrlStateText_None = _T("None");
	m_MesEqpCtrlStateText_Offline = _T("Offline");
	m_MesEqpCtrlStateText_Local = _T("Local");
	m_MesEqpCtrlStateText_Remote = _T("Remote");
	//Field Path Mode
	m_FieldPathModeText_Hor = _T("Hor");//水平優先
	m_FieldPathModeText_Ver = _T("Ver");//垂直優先
	m_FieldPathModeText_User = _T("User");//手動配置
	//Field Division Mode
	m_FieldDivisionModeText_MassArea=_T("Mass Area");//最大面積
	m_FieldDivisionModeText_Diagonal=_T("Diagonal Line");//對角線
	m_FieldDivisionModeText_Horizontal=_T("Horizontal Line");//水平線
	m_FieldDivisionModeText_Vertical=_T("Vertical Line");//垂直線
	//Field Build Mode
	m_FieldBuildModeText_Matrix = _T("Matrix");//等間距
	m_FieldBuildModeText_Random_Panel=_T("Random-Panel");//任意位置-整板
	m_FieldBuildModeText_Random_Board=_T("Random-Board");//任意位置-單板
	m_FieldBuildModeText_Random_Project=_T("Random-Project");
	//Barcode Input Type
	m_BarcodeInputText_Disabled = _T("Disabled");
	m_BarcodeInputText_Device = _T("Device");
	m_BarcodeInputText_Handheld = _T("Handheld");	
	//Barcode NG Handle Mode
	m_BarcodeNGHandleText_Pass = _T("Pass");
	m_BarcodeNGHandleText_Alarm = _T("Alarm");
	m_BarcodeNGHandleText_Input = _T("Input");
	//Multi Project Test Order Mode
	m_MultiProjectTestOrderText_ByMark = _T("By Mark");
	m_MultiProjectTestOrderText_InTurn = _T("In Turn");	
	m_MultiProjectTestOrderText_OneCycleAB = _T("One Cycle (A->B)");	
	m_MultiProjectTestOrderText_OneCycleBA = _T("One Cycle (B->A)");	
	//Barcode Camera Grab Mode
	m_BarcodeCameraGrabModeText_AfterFd = _T("After Fd");
	m_BarcodeCameraGrabModeText_Inspecting = _T("Inspecting");
	//---------------------------------------------------------------------------------//		
	//Barcode Device Grab Mode
	m_BarcodeDeviceGrabModeText_BeforePCBIn = _T("Before PCB In");
	m_BarcodeDeviceGrabModeText_WhilePCBIn = _T("While PCB In");
	m_BarcodeDeviceGrabModeText_AfterPCBIn = _T("After PCB In");
	m_BarcodeDeviceGrabModeText_BeforeInspect = _T("Before Inspection");
	//---------------------------------------------------------------------------------//		
	//Barcode Handheld Read Mode
	m_BarcodeHandHeldReadModeText_Manual = _T("Manual");
	m_BarcodeHandHeldReadModeText_Project = _T("By Project");
	m_BarcodeHandHeldReadModeText_Panel = _T("By Panel");
	m_BarcodeHandHeldReadModeText_Board = _T("By Board");
	//---------------------------------------------------------------------------------//		
	//BARCODE_AUTO_EXPAND_MODE
	m_BarcodeAutoExpandModeText_Disable = _T("Disable");
	m_BarcodeAutoExpandModeText_Increment = _T("Increment");
	m_BarcodeAutoExpandModeText_AddChar01 = _T("Add Char(01)");
	m_BarcodeAutoExpandModeText_AddChar02 = _T("Add Char(02)");
	m_BarcodeAutoExpandModeText_Replace01 = _T("Replace(01)");
	m_BarcodeAutoExpandModeText_Replace02 = _T("Replace(02)");	
	m_BarcodeAutoExpandModeText_Inc_Base36 = _T("Increment(36)");
	//---------------------------------------------------------------------------------//		
	//ALG_BARCODE_DIR_MODE
	m_BarcodeDirectionModeText_Auto = _T("Auto");
	m_BarcodeDirectionModeText_Hor = _T("Hor");
	m_BarcodeDirectionModeText_Ver = _T("Ver");
	m_BarcodeDirectionModeText_All = _T("All");	
	//---------------------------------------------------------------------------------//		
	//AUTO_SWITCH_WND_3D_FRAME_MODE
	m_AutoSwitchWnd3DFrame_Disable = _T("Disable");
	m_AutoSwitchWnd3DFrame_Enable = _T("Enable");
	m_AutoSwitchWnd3DFrame_BySize = _T("By Size");
	m_AutoSwitchWnd3DFrame_ByType = _T("By Type");
	m_AutoSwitchWnd3DFrame_ByGroupChange = _T("By Change Group");
	//---------------------------------------------------------------------------------//	
	//ALARM_LOCK_MODE
	m_AlarmLockText_None = _T("None");
	m_AlarmLockText_AOI = _T("AOI");
	m_AlarmLockText_ARS = _T("ARS");	
	//---------------------------------------------------------------------------------//		
	//Defect From ID
	m_DefectFromText_None = _T("None");
	m_DefectFromText_AOI = _T("AOI");
	m_DefectFromText_ARS = _T("ARS");
	//TOP10_SCOPE;
	m_Top10ScopeText_Model = _T("Model");
	m_Top10ScopeText_PartNumber = _T("Part Number");
	m_Top10ScopeText_Component = _T("Component");
	//YIELDING_SCOPE
	m_YieldingScopeText_Test = _T("Test");
	m_YieldingScopeText_Panel = _T("Panel");
	m_YieldingScopeText_Board = _T("Board");
	m_YieldingScopeText_Component = _T("Component");
	//DEFECT_PARAM_FROM_MODE
	m_DefectParamFromText_Disable = _T("Disable");
	m_DefectParamFromText_Project = _T("Project");
	m_DefectParamFromText_Component = _T("Component");
	//CPK_FROM_MODE
	m_CpkFromText_OffsetX = _T("Offset X");
	m_CpkFromText_OffsetY = _T("Offset Y");
	m_CpkFromText_SkewAngle = _T("Angle");
	//MULTI_LANGUAGE_MODE
	m_MultiLanguageText_English = _T("English");
	m_MultiLanguageText_ChinTrad = _T("Chinese Trad.");
	m_MultiLanguageText_ChinSimp = _T("Chinese Simp.");
	m_MultiLanguageText_Local = _T("Local");
	//ALG_TYPE
	m_AlgText_BrightRatio = _T("Bright Ratio");
	m_AlgText_OuterShort = _T("Outer Short");
	m_AlgText_BlobCount = _T("Blob Count");
	m_AlgText_BodyTilt = _T("Body Tilt");
	m_AlgText_BarcodeRecognize = _T("Barcode Recognize");
	m_AlgText_ObjectMeasure = _T("Object Measure");
	m_AlgText_WidthRatio = _T("Width Ratio");
	m_AlgText_Resin = _T("Resin Height");
	m_AlgText_WireWidth = _T("Wire Width");
	m_AlgHeightDetectionType1 = _T("Solder");
	m_AlgHeightDetectionType2 = _T("Resin");
	m_AlgHeightDetectionMeasureMode1 = _T("Ratio");
	m_AlgHeightDetectionMeasureMode2 = _T("Absolute");
	m_AlgHeightDetectionOutputType1 = _T("ALG_3D_BASE_HEIGHT_AVE");
	m_AlgHeightDetectionOutputType2 = _T("ALG_3D_BASE_HEIGHT_MAX");

	m_AlgText_ColorCode = _T("Color Code");
	m_AlgText_ModelMatch = _T("Model Match");
	m_AlgText_ImageMatch = _T("Image Match");
	m_AlgText_CharVerify = _T("Char Verify");
	m_AlgText_FdMatch = _T("Fd Match");
	m_AlgText_EdgeSearch = _T("Edge Search");
	m_AlgText_ShapeVerify = _T("Shape Verify");
	m_AlgText_AngleMeasure = _T("Angle Measure");
	m_AlgText_PixelCompare = _T("Pixel Compare");	
	m_AlgText_SolderWetting = _T("Solder Wetting");
	m_AlgText_MeasureBlackGlue = _T("Measure Black Glue");
	m_AlgText_MeasureFluxArea = _T("Measure Flux Area");
	m_AlgText_MeasureCpuPin = _T("Measure CPU Pin");
	m_AlgText_MeasureSIP = _T("Measure SIP");
	m_AlgText_MeasureConnector = _T("Measure Connector");

	m_AlgText_GroupCompare = _T("Group Compare");
	//ALG_CALC_UNIT_MODE
	m_AlgCalcUnitMode_Abs = _T("Absolute");//計算單位模式-絕對數值 
	m_AlgCalcUnitMode_Diff = _T("Difference");//計算單位模式-相對差距
	m_AlgCalcUnitMode_Ratio = _T("Percentage");//計算單位模式-比例數值
	//ALG_BRIGHT_AVERAGE_MODE
	m_AlgBrightAverageMode_Full = _T("Full");
	m_AlgBrightAverageMode_Partial = _T("Partial");	
	//ALG_MATCH_DOCK_MODE
	m_AlgMatchDockMode_Disable = _T("Disable");
	m_AlgMatchDockMode_ToTip = _T("To Tip");
	m_AlgMatchDockMode_Shoulder = _T("Shoulder");	
	//ALG_OBJECT_SIZE_CALC_MODE
	m_AlgObjectSizeCalcMode_Boundary = _T("Boundary");
	m_AlgObjectSizeCalcMode_Average = _T("Average");
	m_AlgObjectSizeCalcMode_AveRect = _T("Ave.+Rect");	
	m_AlgObjectSizeCalcMode_BlurRect = _T("Blur Rect");
	//ALG_OBJECT_HEIGHT_AVERAGE_MODE
	m_AlgObjectHeightAverageMode_Full = _T("Full");
	m_AlgObjectHeightAverageMode_Partial = _T("Partial");
	//ANGLE_MEASURE_MODE
	m_AlgAngleMeasureAngleMode_Skew = _T("Skew");
	m_AlgAngleMeasureAngleMode_Tilt = _T("Tilt");
	//LINE_EQUATION_MODE
	m_AlgAngleMeasureBaseLineMode_Calc = _T("Calc");
	m_AlgAngleMeasureBaseLineMode_Hor = _T("Hor.");
	m_AlgAngleMeasureBaseLineMode_Ver = _T("Ver.");	
	//RESULT_ID;
	m_ResultText_None = _T("None");
	m_ResultText_OK = _T("OK");
	m_ResultText_NG = _T("NG");
	m_ResultText_Skip = _T("Skip");
	m_ResultText_Bypass = _T("Bypass");
	m_ResultText_Exception = _T("Exception");	
	//PART_GROUP_MODE
	m_PartGroupText_Colinearity = _T("Colinearity");
	m_PartGroupText_ColinearityToLine = _T("Colinearity-Line");
	m_PartGroupText_DistPartToPart = _T("Dist. Part To Part");
	m_PartGroupText_DistPartNeighbor = _T("Dist. Neighbor Parts");
	m_PartGroupText_DistPartToGroup = _T("Dist. Part To Group");
	m_PartGroupText_DistGroupToPart = _T("Dist. Group To Part");
	m_PartGroupText_DistGroupCoordMap = _T("Dist. Group Coord. Map");	
	//IMAGE_SRC_MODE
	m_ImageSrcText_Gray = _T("Gray");	
	m_ImageSrcText_Color = _T("Color");	
	m_ImageSrcText_Red = _T("Red");	
	m_ImageSrcText_Green = _T("Green");	
	m_ImageSrcText_Blue = _T("Blue");	
	m_ImageSrcText_Lightness = _T("Lightness");	
	m_ImageSrcText_Darkness = _T("Darkness");
	m_ImageSrcText_Saturation = _T("Saturation");
	m_ImageSrcText_Synthesis = _T("Synthesis");	
	m_ImageSrcText_RedRatio = _T("Red Ratio");//影像來源-紅色
	m_ImageSrcText_GreenRatio = _T("Green Ratio");//影像來源-綠色
	m_ImageSrcText_BlueRatio = _T("Blue Ratio");//影像來源-藍色
	m_ImageSrcText_MaxGrnBlu = _T("Max G/B");//影像來源-最亮綠藍色
	//MASK_FUNC_MODE
	m_MaskFuncText_Calc = _T("Calc.");
	m_MaskFuncText_Erase = _T("Erase");
	//BINARY_MODE
	m_BinaryText_Disable = _T("Disable");		
	m_BinaryText_ColorFilter = _T("Color Filter");		
	m_BinaryText_FixedTh = _T("Fixed Threshold");		
	m_BinaryText_DynamicTh = _T("Dynamic Threshold");		
	m_BinaryText_RelativeTh = _T("Relative Ave Threshold");		
	m_BinaryText_AdaptiveTh = _T("Adaptive Threshold");	
	//EDGE_ENHANCE_MODE
	m_EdgeEnhanceText_Disable = _T("Disable");		
	m_EdgeEnhanceText_Sobel = _T("Sobel");
	m_EdgeEnhanceText_DarkTop = _T("Top Dark");
	m_EdgeEnhanceText_DarkLeft = _T("Left Dark");
	m_EdgeEnhanceText_DarkBot = _T("Bottom Dark");
	m_EdgeEnhanceText_DarkRight = _T("Right Dark");
	//NOISE_FILTER_MODE
	m_NoiseFilterText_Diable = _T("Disable");
	m_NoiseFilterText_Level = _T("Level");
	m_NoiseFilterText_Smooth = _T("Smooth");			
	m_NoiseFilterText_Median = _T("Median");
	m_NoiseFilterText_Median2 = _T("Median2");	
	m_NoiseFilterText_PyramidMedian = _T("PyramidMedian");
	m_NoiseFilterText_ContentAware = _T("ContentAware");
	m_NoiseFilterText_Fast_Median = _T("Fast Median");
	m_NoiseFilterText_Fast_Average = _T("Fast Average");
	m_NoiseFilterText_Open = _T("Open");		
	m_NoiseFilterText_Close = _T("Close");		
	m_NoiseFilterText_Erosion = _T("Erosion");
	m_NoiseFilterText_Dilation= _T("Dilation");
	m_NoiseFilterText_Gradient = _T("Gradient");
	//ALG_BRIGHT_LINE_MODE
	m_AlgBrightLineMode_Bright=_T("Bright");//貫穿線模式-抓亮的
	m_AlgBrightLineMode_Dark=_T("Dark");//貫穿線模式-抓暗的
	//ALG_OUTER_SHORT_EXT_MODE
	m_AlgOuterShortExtendText_None = _T("None");//外接短路延伸文字-無
	m_AlgOuterShortExtendText_Left = _T("Left");//外接短路延伸文字-單左
	m_AlgOuterShortExtendText_Right= _T("Right");//外接短路延伸文字-單右
	m_AlgOuterShortExtendText_Both = _T("Both");//外接短路延伸文字-雙側
	//ALG_DIRECTION
	m_AlgDirText_Hor = _T("Horizontal");
	m_AlgDirText_Ver = _T("Vertical");
	//ALG_BARCODE_STEP_MODE
	m_AlgBarcodeStepText_None = _T("Disable");
	m_AlgBarcodeStepText_Scale = _T("Scale");
	m_AlgBarcodeStepText_GainOffset = _T("Gain Offset");
	m_AlgBarcodeStepText_Smooth = _T("Smooth");
	m_AlgBarcodeStepText_Open = _T("Open");
	m_AlgBarcodeStepText_Close = _T("Close");
	m_AlgBarcodeStepText_Median = _T("Median");
	m_AlgBarcodeStepText_Invert = _T("Invert");
	m_AlgBarcodeStepText_Flip = _T("Flip");	
	m_AlgBarcodeStepText_Fill = _T("Fill");
	m_AlgBarcodeStepText_Erode = _T("Erode");
	m_AlgBarcodeStepText_Dilate = _T("Dilate");
	m_AlgBarcodeStepText_Fill2D = _T("Fill-2D");	
	m_AlgBarcodeStepText_Sharp = _T("Sharp");
	m_AlgBarcodeStepText_Range = _T("Gray Range");
	//FD_MATCH_MODE
	m_AlgFdMatchText_Model = _T("Model");
	m_AlgFdMatchText_Image = _T("Image");
	//ALG_GROUP_CMP_DIR_MODE
	m_AlgGroupCompareDirText_Any=_T("Any");//群組比較方向-任方向
	m_AlgGroupCompareDirText_One=_T("One");//群組比較方向-同方向
	//ALG_3D_BASE_HEIGHT_MODE
	m_Alg3DHeightBaseText_Min=_T("Min");//3D高度基本模式-最小
	m_Alg3DHeightBaseText_Max=_T("Max");//3D高度基本模式-最大
	m_Alg3DHeightBaseText_Ave=_T("Ave");//3D高度基本模式-平均
	m_Alg3DHeightBaseText_Mid=_T("Mid");//3D高度基本模式-中位數
	m_Alg3DHeightBaseText_SQR=_T("Sqr");//3D高度基本模式-Least sq
	//ALG_SEARCH_DIRECTION
	m_AlgSearchDirectionText_Forward=_T("Forward");//搜尋方向-同向
	m_AlgSearchDirectionText_Backward=_T("Backward");//搜尋方向-反向
	//ALG_EDGE_FEATURE_MODE
	m_AlgEdgeFeatureText_W2B=_T("W to B");//邊緣特徵-白到黑
	m_AlgEdgeFeatureText_B2W=_T("B to W");//邊緣特徵-黑到白
	//BOX_TOWARD
	m_BoxTowardText_Up=_T("Up");//框朝向-上
	m_BoxTowardText_Left=_T("Left");//框朝向-左
	m_BoxTowardText_Down=_T("Down");//框朝向-下
	m_BoxTowardText_Right=_T("Right");//框朝向-右
	//BOX_SHAPE_MODE
	m_BoxShapeText_Rect = _T("Rectangle");
	m_BoxShapeText_RectRound = _T("Round Rect.");
	m_BoxShapeText_Ellipse = _T("Ellipse");
	m_BoxShapeText_Capsule = _T("Capsule");
	m_BoxShapeText_Bullet = _T("Bullet");
	m_BoxShapeText_RectHalfRound = _T("Half Round");
	m_BoxShapeText_TShape = _T("T-Shape");
	//LAND_TYPE
	m_LandTypeText_Pad = _T("Pad");
	m_LandTypeText_Electrode = _T("Electrode");
	m_LandTypeText_ICLead = _T("IC Lead");
	m_LandTypeText_ConLead = _T("Con Lead");
	m_LandTypeText_DipLead = _T("DIP Lead");
	//MODEL_PART//模組部位
	m_ModelPadText = _T("Pad");
	m_ModelBodyText = _T("Body");
	m_ModelLeadText = _T("Lead");
	m_ModelLeadTipText = _T("Lead Tip");
	m_ModelLeadShoulderText = _T("Lead Shoulder");
	//MODEL_MASK
	m_ModelMaskPadText = _T("Pad");
	m_ModelMaskBodyText = _T("Body");
	m_ModelMaskBodyNoLeadText = _T("Body No Lead");
	m_ModelMaskLeadText = _T("Lead");
	m_ModelMaskLeadTipText = _T("Lead Tip");
	m_ModelMaskLeadShoulderText = _T("Lead Shoulder");
	//MODEL_GROUP
	m_ModelGroupText_All = _T("All");	
	//MODEL_TYPE
	m_ModelTypeText_Null = _T("Undefined");
	m_ModelTypeText_Chip = _T("Chip");
	m_ModelTypeText_ChipC = _T("Chip-C");
	m_ModelTypeText_ChipR = _T("Chip-R");
	m_ModelTypeText_ChipL = _T("Chip-L");
	m_ModelTypeText_ChipLed = _T("LED");
	m_ModelTypeText_Melf = _T("MELF");
	m_ModelTypeText_Electrode = _T("Electrode");
	m_ModelTypeText_Tant = _T("Tantalum");
	m_ModelTypeText_CN = _T("CA");
	m_ModelTypeText_RN = _T("RA");
	m_ModelTypeText_SOT = _T("Transistor");
	m_ModelTypeText_ElecCap = _T("Elec. Cap.");
	m_ModelTypeText_LedArray = _T("LED Array");
	m_ModelTypeText_NoLead = _T("No Lead");
	m_ModelTypeText_NoLeadDN = _T("DFN");
	m_ModelTypeText_NoLeadQFN = _T("QFN");
	m_ModelTypeText_NoLeadOSC = _T("OSC");
	m_ModelTypeText_LeadCom = _T("Lead Component");
	m_ModelTypeText_LeadComSOP = _T("SOP");
	m_ModelTypeText_LeadComQFP = _T("QFP");
	m_ModelTypeText_LeadComSOT = _T("Lead Transistor");
	m_ModelTypeText_JLeadCom = _T("JLead Component");
	m_ModelTypeText_JLeadComSOJ = _T("SOJ");
	m_ModelTypeText_JLeadComPLCC = _T("PLCC");
	m_ModelTypeText_CompositeCom = _T("Composite Component");
	m_ModelTypeText_PowerTransistor = _T("Power Transistor");
	m_ModelTypeText_Connector = _T("Connector");
	m_ModelTypeText_BGA = _T("BGA");
	m_ModelTypeText_Fd = _T("FD");
	m_ModelTypeText_Barcode = _T("Barcode");
	m_ModelTypeText_Pad = _T("Pad");
	m_ModelTypeText_GoldFinger = _T("Gold Finger");
	m_ModelTypeText_DipLead = _T("DIP Lead");
	m_ModelTypeText_Ohters = _T("Others");
	//WND_LOGIC_TYPE
	m_WndLogText_None = _T("Disable");
	m_WndLogText_GroupID = _T("Group");
	m_WndLogText_DefectID = _T("Defect");
	//WND_FOLLOW_MODE
	m_WndFollowText_None = _T("Disable");
	m_WndFollowText_Pad = _T("Pad");
	m_WndFollowText_Part = _T("Part");	
	m_WndFollowText_PadBody = _T("Pad(Body)");
	m_WndFollowText_PadLead = _T("Pad(Lead)");
	m_WndFollowText_PartBody = _T("Body");
	m_WndFollowText_PartLead = _T("Lead");
	//WND_RGN_LINK_MODE
	m_WndRgnLinkText_None = _T("None");
	m_WndRgnLinkText_Pad = _T("Pad");
	m_WndRgnLinkText_Body = _T("Body");
	m_WndRgnLinkText_Lead = _T("Lead");
	m_WndRgnLinkText_PadTip = _T("Pad Tip");
	m_WndRgnLinkText_PadRgn = _T("Pad Rgn");
	m_WndRgnLinkText_PadBodyRgn = _T("Pad+Body");
	m_WndRgnLinkText_PadRgnInner = ("Pad Rgn Inner");
	m_WndRgnLinkText_LeadTip = _T("Lead Tip");
	m_WndRgnLinkText_LeadShoulder = _T("Lead Shoulder");
	m_WndRgnLinkText_LeadTipShoulder = _T("Tip+Shoulder");

	m_WndSyncMoveText_Mirror = _T("Mirror");
	m_WndSyncMoveText_Rotate = _T("Rotate");
	m_WndSyncMoveText_Symmetry = _T("Symmetry");

	m_WndConstrainText_Disable = _T("Disable");//局限-關閉
	m_WndConstrainText_PadRgnMove = _T("Pad Rgn-Move");//局限-焊盤範圍內-All
	m_WndConstrainText_PadRgnXMove = _T("Pad Rgn X-Move");//局限-焊盤範圍內-X
	m_WndConstrainText_PadRgnYMove = _T("Pad Rgn Y-Move");//局限-焊盤範圍內-Y	
	m_WndConstrainText_PadRgnScale = _T("Pad Rgn-Scale");//局限-焊盤範圍內-All
	m_WndConstrainText_PadRgnXScale = _T("Pad Rgn X-Scale");//局限-焊盤範圍內-X
	m_WndConstrainText_PadRgnYScale = _T("Pad Rgn Y-Scale");//局限-焊盤範圍內-Y		
	//WND_DEFECT_ID
	m_WndDefectText_None = _T("None");
	m_WndDefectText_PadAlign = _T("Pad Align");
	m_WndDefectText_PartAlign = _T("Part Align");
	m_WndDefectText_PadAdjust = _T("Pad Adjust");
	m_WndDefectText_LeadAdjust = _T("Lead Adjust");	
	m_WndDefectText_ClassCheck = _T("Class Check");
	m_WndDefectText_BaseValue = _T("Base Value");
	m_WndDefectText_BodyMissing = _T("Body Missing");
	m_WndDefectText_BodyOffset = _T("Body Offset");
	m_WndDefectText_BodyTilt = _T("Body Tilte");
	m_WndDefectText_BodyPolarity = _T("Body Polarity");
	m_WndDefectText_BodyTurnOver = _T("Turn Over");
	m_WndDefectText_BodyMount = _T("Mount");
	m_WndDefectText_BodyWrongCode = _T("Wrong Code");
	m_WndDefectText_BodyWrongText = _T("Wrong Text");
	m_WndDefectText_BodyTombstone = _T("Tombstone");
	m_WndDefectText_BodyBillboard = _T("Billboard");
	m_WndDefectText_BodyDamaged = _T("Damaged");

	m_WndDefectText_SolderPoor = _T("Solder Poor");
	m_WndDefectText_SolderOpen = _T("Solder Open");
	m_WndDefectText_SolderPadExposed = _T("Pad Exposed");
	m_WndDefectText_SolderBridge = _T("Bridge");
	m_WndDefectText_SolderBead = _T("Bead");
	m_WndDefectText_SolderExcess = _T("Solder Excess");

	m_WndDefectText_LeadLifted = _T("Lead Lifted");
	m_WndDefectText_LeadBended = _T("Lead Bened");
	m_WndDefectText_LeadProtruded = _T("Lead Protruded");
	m_WndDefectText_PadScratch = _T("Pad Scratch");
	m_WndDefectText_ForeignBody = _T("Foreign Body");

	m_WndDefectText_UserDefine_01 = _T("User Define 01");
	m_WndDefectText_UserDefine_02 = _T("User Define 02");
	m_WndDefectText_UserDefine_03 = _T("User Define 03");
	m_WndDefectText_UserDefine_04 = _T("User Define 04");
	m_WndDefectText_UserDefine_05 = _T("User Define 05");
	m_WndDefectText_UserDefine_06 = _T("User Define 06");
	m_WndDefectText_UserDefine_07 = _T("User Define 07");
	m_WndDefectText_UserDefine_08 = _T("User Define 08");
	m_WndDefectText_UserDefine_09 = _T("User Define 09");
	m_WndDefectText_UserDefine_10 = _T("User Define 10");

	//BASE_PLANE_PROC_TYPE
	m_BasePlaneProcText_1 = _T("Type I");
	m_BasePlaneProcText_2 = _T("Type II");

	//BASE_PLANE_AUTO_REGION_MODE
	m_BasePlaneAutoRgnText_Disable = _T("Disable");
	m_BasePlaneAutoRgnText_Group = _T("Group");
	m_BasePlaneAutoRgnText_Lowest = _T("Lowest");

	//BASE_PLANE_BODY_OUTSIDE_MODE
	m_BasePlaneBodyOutsideText_Disable = _T("Disable");
	m_BasePlaneBodyOutsideText_Body = _T("Body");
	m_BasePlaneBodyOutsideText_BodyLand = _T("Body+Land");

	//CALC_BASE_PLANE_MODE
	m_CalcBasePlaneText_Disable = _T("Disable");	
	m_CalcBasePlaneText_Ave = _T("Average");//平均值
	m_CalcBasePlaneText_Corner = _T("Corner");//四個端點
	m_CalcBasePlaneText_IsoData = _T("ISO DATA");//Iso data
	m_CalcBasePlaneText_Ostu = _T("OSTU");//OTSU
	m_CalcBasePlaneText_CornerOnly = _T("Corner Only");//只有四個端點
	m_CalcBasePlaneText_Surround = _T("Surround");//周圍
	m_CalcBasePlaneText_AutoLower = _T("Auto Lower");
	m_CalcBasePlaneText_Panel = _T("Panel");
	m_CalcBasePlaneText_Local = _T("Local");

	//Fd 
	m_FdText_NG = _T("Error, Fd NG");

	//Barcode 
	m_BarcodeText_NG = _T("Error, Barcode NG");

	//色彩群組文字
	m_ProjectColorGroupText_Pad = _T("Pad Color");
	m_ProjectColorGroupText_Void = _T("Void Color");
	m_ProjectColorGroupText_Body = _T("Body Color");
	m_ProjectColorGroupText_Board = _T("Board Color");
	m_ProjectColorGroupText_Solder = _T("Solder Color");
	m_ProjectColorGroupText_Ohters = _T("Ohters Color");

	//軸控加速度調整模式
	m_MotionAccTimeAdjustText_Off = _T("Disable");
	m_MotionAccTimeAdjustText_Fix = _T("Fix Time");
	m_MotionAccTimeAdjustText_Min = _T("Min Time");
	m_MotionAccTimeAdjustText_Gamma = _T("Gamma Adjust");		

	//指紋辨識命令
	m_AS608CommandText_GetImage = _T("Get Fingerprint");
	m_AS608CommandText_Match = _T("Matching");
	m_AS608CommandText_RegMode = _T("Merge Features");
	m_AS608CommandText_GenChar = _T("Generate Features");
	m_AS608CommandText_UpChar = _T("Enroll Features");
	m_AS608CommandText_DownChar = _T("Reload User Features");

	//指紋辨識回傳狀態
	m_AS608StatusText_OK = _T("OK");
	m_AS608StatusText_Error = _T("Error");
	m_AS608StatusText_NOFingerprint = _T("Please place your finger");
	m_AS608StatusText_InputError = _T("Input Error");
	m_AS608StatusText_ImageTooDry = _T("Too Dry");
	m_AS608StatusText_ImageTooWet = _T("Too Wet");
	m_AS608StatusText_ImageTooClutter = _T("Too Cluttered");
	m_AS608StatusText_ImageTooFewFeature = _T("Too Few Features in Fingerprint");
	m_AS608StatusText_NotMatch = _T("Matching Failed");
	m_AS608StatusText_NoMove = _T("Please take your finger off and try again.");
	m_AS608StatusText_FeatureCombineError = _T("Features Combine failed.");

	//AAAAAAAAAAAAAAAAAA = _T("AAAAAAAAAAAAAAAAAA");
}
//-------------------------------------------------------------------------------------//
void CAOIDataDefine::CloneDefine(const CAOIDataDefine &Define)
{	return;//以後要拿掉
	//ID
	m_IDText = Define.m_IDText;
	m_ToText = Define.m_ToText;	
	//Fiducial
	m_FdText = Define.m_FdText;
	m_AddText = Define.m_AddText;	
	m_SetText = Define.m_SetText;
	m_GetText = Define.m_GetText;
	m_PadText = Define.m_PadText;
	m_WndText = Define.m_WndText;	

	m_AveText = Define.m_AveText;	
	m_AllText = Define.m_AllText;
	m_MaxText = Define.m_MaxText;	
	m_MinText = Define.m_MinText;
	m_FromText = Define.m_FromText;	
	m_AxisText = Define.m_AxisText;	
	m_LineText = Define.m_LineText;	
	m_AreaText = Define.m_AreaText;		
	m_LaneText = Define.m_LaneText;
	m_TimeText = Define.m_TimeText;	
	m_TestText = Define.m_TestText;	
	m_NameText = Define.m_NameText;//Name	
	m_SkewText = Define.m_SkewText;
	m_MarkText = Define.m_MarkText;	
	m_GainText = Define.m_GainText;		
	m_GrayText = Define.m_GrayText;		
	m_SizeText = Define.m_SizeText;			
	m_LandText = Define.m_LandText;
	m_RangeText = Define.m_RangeText;
	m_ClassText = Define.m_ClassText;	
	m_ClearText = Define.m_ClearText;	
	m_RatioText = Define.m_RatioText;	
	m_IndexText = Define.m_IndexText;//Index	
	m_ErrorText = Define.m_ErrorText;//Error
	
	m_PanelText = Define.m_PanelText;	
	m_BoardText = Define.m_BoardText;	
	m_ModelText = Define.m_ModelText;
	m_GroupText = Define.m_GroupText;
	m_ScoreText = Define.m_ScoreText;	
	m_ScaleText = Define.m_ScaleText;		
	m_YieldText = Define.m_YieldText;		
	m_PixelText = Define.m_PixelText;	

	m_SizeXText = Define.m_SizeXText;//X尺寸文字
	m_SizeYText = Define.m_SizeYText;//Y尺寸文字	
	
	m_MethodText = Define.m_MethodText;
	m_TowardText = Define.m_TowardText;
	m_CloneText = Define.m_CloneText;
	m_CreateText = Define.m_CreateText;
	m_ModifyText = Define.m_ModifyText;
	m_DeleteText = Define.m_DeleteText;
	m_DefectText = Define.m_DefectText;
	m_ResultText = Define.m_ResultText;
	m_UnsetText = Define.m_UnsetText;
	m_AngleText = Define.m_AngleText;
	m_WidthText = Define.m_WidthText;
	m_HeightText = Define.m_HeightText;	
	m_VolumeText = Define.m_VolumeText;
	m_OffsetText = Define.m_OffsetText;		
	m_FinishText = Define.m_FinishText;
	m_ProjectText = Define.m_ProjectText;
	m_PatternText = Define.m_PatternText;	
	m_DefaultText = Define.m_DefaultText;	
	m_ThroughText = Define.m_ThroughText;		
	m_BarcodeText = Define.m_BarcodeText;			
	m_WarningText = Define.m_WarningText;
	m_GroupIDText = Define.m_GroupIDText;	
	m_ContinueText = Define.m_ContinueText;
	m_ContrastText = Define.m_ContrastText;		
	m_BypassedText = Define.m_BypassedText;
	m_DistrictText = Define.m_DistrictText;		
	m_DistanceText = Define.m_DistanceText;		
	m_VelocityText = Define.m_VelocityText;
	m_RelativeText = Define.m_RelativeText;		
	m_ThicknessText = Define.m_ThicknessText;	
	m_AlgorithmText = Define.m_AlgorithmText;	
	m_LockScreenText = Define.m_LockScreenText;
	m_VerticalText = Define.m_VerticalText;
	m_HorizontalText = Define.m_HorizontalText;
	m_ComponentText = Define.m_ComponentText;
	m_PartNumberText = Define.m_PartNumberText;
	m_NozzleNameText = Define.m_NozzleNameText;	

	//Undefined
	m_UndefinedText = Define.m_UndefinedText;
	m_AccelerationText = Define.m_AccelerationText;	
	//Barcode Device
	m_BarcodeDeviceText = Define.m_BarcodeDeviceText;
	//Safty Bypass	
	m_SaftyBypassText = Define.m_SaftyBypassText;

	m_WaitForCCSText = Define.m_WaitForCCSText;
	m_WaitForRepairText = Define.m_WaitForRepairText;
	
	m_InspectionResultFaultText = Define.m_InspectionResultFaultText;
	m_DoYouWantToClearTheStatisticRecordsText = Define.m_DoYouWantToClearTheStatisticRecordsText;

	//OK/Cancel
	m_OKText = Define.m_OKText;
	m_CancelText = Define.m_CancelText;
	m_DecodeText = Define.m_DecodeText;
	m_LevelText = Define.m_LevelText;	
	//Enable/Disable
	m_EnableText = Define.m_EnableText;
	m_DisabeText = Define.m_DisabeText;
	//Positive/Negative
	m_PositiveText = Define.m_PositiveText;
	m_NegativeText = Define.m_NegativeText;
	//DISTRICT_ID
	m_DistrictText_A = Define.m_DistrictText_A;
	m_DistrictText_B = Define.m_DistrictText_B;	 	
	//WND_DEFECT_ITEM
	m_WndDefectItem_Disable = Define.m_WndDefectItem_Disable;	 	
	m_WndDefectItem_Enable = Define.m_WndDefectItem_Enable;	 	
	m_WndDefectItem_NoShow = Define.m_WndDefectItem_NoShow;	 
	//Grab/Calculate
	m_GrabText = Define.m_GrabText;
	m_CountText = Define.m_CountText;	
	m_CalculateText = Define.m_CalculateText;
	m_VersionCodeText = Define.m_VersionCodeText;	
	//COLOR_MODE
	m_ColorText_Red = Define.m_ColorText_Red;//顏色文字-紅色
	m_ColorText_Green = Define.m_ColorText_Green;//顏色文字-綠色
	m_ColorText_Blue = Define.m_ColorText_Blue;//顏色文字-藍色
	m_ColorText_Black = Define.m_ColorText_Black;//顏色文字-黑色	
	m_ColorText_Gray = Define.m_ColorText_Gray;//顏色文字-灰色		
	m_ColorText_White = Define.m_ColorText_White;//顏色文字-白色	
	m_ColorText_Color = Define.m_ColorText_Color;//顏色文字-彩色
	//USER LEVEL
	m_UserLevelText_SignOut = Define.m_UserLevelText_SignOut;
	m_UserLevelText_Operator = Define.m_UserLevelText_Operator;
	m_UserLevelText_Engineer = Define.m_UserLevelText_Engineer;
	m_UserLevelText_Supervisor = Define.m_UserLevelText_Supervisor;
	m_UserLevelText_JETFAE = Define.m_UserLevelText_JETFAE;
	m_UserLevelText_JETSENIOR = Define.m_UserLevelText_JETSENIOR;
	//USER_LOGIN_MODE
	m_UserLoginText_Disable = Define.m_UserLoginText_Disable;
	m_UserLoginText_Operator = Define.m_UserLoginText_Operator;
	m_UserLoginText_Engineer = Define.m_UserLoginText_Engineer;
	//USER_LOGIN_OPTIONS
	m_UserLoginOptionsText_Password = Define.m_UserLoginOptionsText_Password;
	m_UserLoginOptionsText_Fingerprint = Define.m_UserLoginOptionsText_Fingerprint;
	m_UserLoginOptionsText_FingerprintOnly = Define.m_UserLoginOptionsText_FingerprintOnly;
	//OPEN_PROJECT_MODE
	m_OpenProjectText_File = Define.m_OpenProjectText_File;
	m_OpenProjectText_Code = Define.m_OpenProjectText_Code;
	//VERIFY_PROJECT_MODE
	m_VerifyProjectText_Disable = Define.m_VerifyProjectText_Disable;
	m_VerifyProjectText_Filename = Define.m_VerifyProjectText_Filename;	
	//ONLINE_INPUT_TIMING
	m_OnlineInputTiming_Disalbe = Define.m_OnlineInputTiming_Disalbe;
	m_OnlineInputTiming_BeforeSignIn = Define.m_OnlineInputTiming_BeforeSignIn;			
	//AOI_CUSTOMER_ID
	m_AOICustomerIDText_JET_TWN = Define.m_AOICustomerIDText_JET_TWN;
	m_AOICustomerIDText_PegaTron_TWN = Define.m_AOICustomerIDText_PegaTron_TWN;
	m_AOICustomerIDText_Kinpo_YueYang = Define.m_AOICustomerIDText_Kinpo_YueYang;
	m_AOICustomerIDText_Foxconn_LongHua = Define.m_AOICustomerIDText_Foxconn_LongHua;	
	//OFFLINE_VERSION_MODE
	m_OfflineVersionText_Normal = Define.m_OfflineVersionText_Normal;
	m_OfflineVersionText_HostTuning = Define.m_OfflineVersionText_HostTuning;
	m_OfflineVersionText_RemoteTuning = Define.m_OfflineVersionText_RemoteTuning;	
	//ONLINE_OPEN_PROJECT_MODE
	m_OnlineOpenProjectText_Disable = Define.m_OnlineOpenProjectText_Disable;
	m_OnlineOpenProjectText_BarcodeDevice = Define.m_OnlineOpenProjectText_BarcodeDevice;
	m_OnlineOpenProjectText_BarcodeHandHeld = Define.m_OnlineOpenProjectText_BarcodeHandHeld;
	m_OnlineOpenProjectText_BarcodeCamera = Define.m_OnlineOpenProjectText_BarcodeCamera;
	//PROJECT_LINK_SERVER_MODE
	m_ProjectLinkServerText_Disable = Define.m_ProjectLinkServerText_Disable;
	m_ProjectLinkServerText_EnableAll = Define.m_ProjectLinkServerText_EnableAll;
	m_ProjectLinkServerText_ProjectOnly = Define.m_ProjectLinkServerText_ProjectOnly;
	m_ProjectLinkServerText_EnableAllAsk = Define.m_ProjectLinkServerText_EnableAllAsk;
	m_ProjectLinkServerText_ProjectOnlyAsk = Define.m_ProjectLinkServerText_ProjectOnlyAsk;
	//XBOARD_MAPPING_FILE_MODE
	m_XBoardMappingFileText_Disable = Define.m_XBoardMappingFileText_Disable;
	m_XBoardMappingFileText_MES_Comm = Define.m_XBoardMappingFileText_MES_Comm;
	//XBOARD_MAPPING_FILE_FLOW
	m_XBoardMappingFileFlowText_Default = Define.m_XBoardMappingFileFlowText_Default;
	m_XBoardMappingFileFlowText_After_Barcode = Define.m_XBoardMappingFileFlowText_After_Barcode;
	//Lane ID
	m_LaneText_A = Define.m_LaneText_A;
	m_LaneText_B = Define.m_LaneText_B;
	//Multi Lane Mode
	m_MultiLaneText_1 = Define.m_MultiLaneText_1;
	m_MultiLaneText_2 = Define.m_MultiLaneText_2;
	//FUNC_EXEC_MODE
	m_FuncExecText_Off = Define.m_FuncExecText_Off;
	m_FuncExecText_Auto = Define.m_FuncExecText_Auto;
	m_FuncExecText_Ask = Define.m_FuncExecText_Ask;	
	//Lane Work Mode
	m_LaneWorkText_Disable = Define.m_LaneWorkText_Disable;
	m_LaneWorkText_Run = Define.m_LaneWorkText_Run;
	m_LaneWorkText_Bypass = Define.m_LaneWorkText_Bypass;
	//CONNECTED_BUFFER_TYPE
	m_ConnectedBufferType_Fixed = Define.m_ConnectedBufferType_Fixed;
	m_ConnectedBufferType_Movable = Define.m_ConnectedBufferType_Movable;	
	//M2M AOI Stage
	m_HASI_AOIStage_Pre = Define.m_HASI_AOIStage_Pre;
	m_HASI_AOIStage_Post = Define.m_HASI_AOIStage_Post;
	m_HASI_State_mode_STOP = Define.m_HASI_State_mode_STOP;
	m_HASI_State_mode_AC = Define.m_HASI_State_mode_AC;
	m_HASI_State_mode_SC = Define.m_HASI_State_mode_SC;
	m_HASI_State_mode_SCAC = Define.m_HASI_State_mode_SCAC;
	m_HASI_State_mode_RUN = Define.m_HASI_State_mode_RUN;
	m_HASI_State_mode_TEST = Define.m_HASI_State_mode_TEST;
	//PCB Out
	m_PCBOutText_Normal = Define.m_PCBOutText_Normal;
	m_PCBOutText_SideOut = Define.m_PCBOutText_SideOut;
	m_PCBOutText_WithIn = Define.m_PCBOutText_WithIn;
	m_PCBOutText_LaneAuto = Define.m_PCBOutText_LaneAuto;
	m_PCBOutText_OkOutNgSide = Define.m_PCBOutText_OkOutNgSide;
	//PCB Out Direction
	m_PCBOutDirText_Forward = Define.m_PCBOutDirText_Forward;
	m_PCBOutDirText_Backward = Define.m_PCBOutDirText_Backward;	
	m_PCBOutDirText_BackwardOut = Define.m_PCBOutDirText_BackwardOut;
	//PCB Side Mode
	m_PanelSideText_Top = Define.m_PanelSideText_Top;
	m_PanelSideText_Bottom = Define.m_PanelSideText_Bottom;
	m_PanelSideText_Hybrid = Define.m_PanelSideText_Hybrid;	
	//Board Side Mode
	m_BoardSideText_Top = Define.m_BoardSideText_Top;
	m_BoardSideText_Bottom = Define.m_BoardSideText_Bottom;
	//BARCODE_DECODER_TYPE
	m_BarcodeDecoder_EVS = Define.m_BarcodeDecoder_EVS;
	m_BarcodeDecoder_DTK = Define.m_BarcodeDecoder_DTK;	
	m_BarcodeDecoder_HON = Define.m_BarcodeDecoder_HON;
	//BARCODE_SPREAD_MODE
	m_BarcodeSpread_Off = Define.m_BarcodeSpread_Off;
	m_BarcodeSpread_Local = Define.m_BarcodeSpread_Local;
	m_BarcodeSpread_All = Define.m_BarcodeSpread_All;	 	
	//BARCODE_BELONG_MODE
	m_BarcodeBelong_None = Define.m_BarcodeBelong_None;
	m_BarcodeBelong_Project = Define.m_BarcodeBelong_Project;
	m_BarcodeBelong_Panel = Define.m_BarcodeBelong_Panel;
	m_BarcodeBelong_Board = Define.m_BarcodeBelong_Board;	
	m_BarcodeBelong_Tray = Define.m_BarcodeBelong_Tray;
	m_BarcodeBelong_Cover = Define.m_BarcodeBelong_Cover;
	//Save Test Map Mode
	m_SaveTestMapText_Disable=Define.m_SaveTestMapText_Disable;
	m_SaveTestMapText_Prog=Define.m_SaveTestMapText_Prog;
	m_SaveTestMapText_Panel=Define.m_SaveTestMapText_Panel;
	m_SaveTestMapText_ProgPanel=Define.m_SaveTestMapText_ProgPanel;
	m_SaveTestMapText_Board=Define.m_SaveTestMapText_Board;
	m_SaveTestMapText_ProgBoard=Define.m_SaveTestMapText_ProgBoard;
	//Offline Image Scope
	m_OfflineImageText_Fov=Define.m_OfflineImageText_Fov;
	m_OfflineImageText_Part=Define.m_OfflineImageText_Part;
	//Save Test Image Mode
	m_SaveTestImageText_Disable=Define.m_SaveTestImageText_Disable;//不儲存	
	m_SaveTestImageText_Defect=Define.m_SaveTestImageText_Defect;//瑕疵儲存
	m_SaveTestImageText_EveryOne=Define.m_SaveTestImageText_EveryOne;//總是儲存	
	//Save Test Data Mode
	m_SaveTestDataText_Disable=Define.m_SaveTestDataText_Disable;
	m_SaveTestDataText_Enable=Define.m_SaveTestDataText_Enable;
	m_SaveTestDataText_Defect=Define.m_SaveTestDataText_Defect;	 	
	//Fd NG Handle Mode
	m_FdNGHandleText_None = Define.m_FdNGHandleText_None;
	m_FdNGHandleText_Pass = Define.m_FdNGHandleText_Pass;
	m_FdNGHandleText_Stop = Define.m_FdNGHandleText_Stop;		
	m_FdNGHandleText_XBoard = Define.m_FdNGHandleText_XBoard;	
	//Board Fd Grab Mode
	m_BoardFdGrabText_AfterPanel = Define.m_BoardFdGrabText_AfterPanel;
	m_BoardFdGrabText_Inspecting = Define.m_BoardFdGrabText_Inspecting;
	//Defect Handle Mode	
	m_DefectHandleText_Pass = Define.m_DefectHandleText_Pass;
	m_DefectHandleText_Stop = Define.m_DefectHandleText_Stop;
	m_DefectHandleText_Next = Define.m_DefectHandleText_Next;
	m_DefectHandleText_Repair = Define.m_DefectHandleText_Repair;
	m_DefectHandleText_ControlCenter = Define.m_DefectHandleText_ControlCenter;
	//Online State Mode
	m_OnlineState_InspectionStop = Define.m_OnlineState_InspectionStop;
	m_OnlineState_PCBReady = Define.m_OnlineState_PCBReady;
	m_OnlineState_InputBarcode = Define.m_OnlineState_InputBarcode;
	m_OnlineState_ProjectMap = Define.m_OnlineState_ProjectMap;
	m_OnlineState_ProjectMark = Define.m_OnlineState_ProjectMark;
	m_OnlineState_ProjectOpenCode = Define.m_OnlineState_ProjectOpenCode;
	m_OnlineState_ProjectReload = Define.m_OnlineState_ProjectReload;
	m_OnlineState_ProjectReloadServer = Define.m_OnlineState_ProjectReloadServer;
	m_OnlineState_ProjectSwitchByTurn = Define.m_OnlineState_ProjectSwitchByTurn;	
	m_OnlineState_ProjectSwitchByTurnOneCycleReset = Define.m_OnlineState_ProjectSwitchByTurnOneCycleReset;
	m_OnlineState_InspectionStart = Define.m_OnlineState_InspectionStart;
	m_OnlineState_InspectionWaittng = Define.m_OnlineState_InspectionWaittng;
	m_OnlineState_InspectFDPanel = Define.m_OnlineState_InspectFDPanel;
	m_OnlineState_InspectFDBoard = Define.m_OnlineState_InspectFDBoard;
	m_OnlineState_InspectBarcode = Define.m_OnlineState_InspectBarcode;
	m_OnlineState_InspectProject = Define.m_OnlineState_InspectProject;
	m_OnlineState_StatisticProject = Define.m_OnlineState_StatisticProject;;
	m_OnlineState_InspectionFinish = Define.m_OnlineState_InspectionFinish;
	m_OnlineState_WaitForLast = Define.m_OnlineState_WaitForLast;
	m_OnlineState_WaitForNext = Define.m_OnlineState_WaitForNext;
	m_OnlineState_WaitForPCBRemoved = Define.m_OnlineState_WaitForPCBRemoved;
	m_OnlineState_WaitForRepairVerify = Define.m_OnlineState_WaitForRepairVerify;
	m_OnlineState_PCBInStart = Define.m_OnlineState_PCBInStart;
	m_OnlineState_PCBInChecking = Define.m_OnlineState_PCBInChecking;
	m_OnlineState_PCBInFinish = Define.m_OnlineState_PCBInFinish;
	m_OnlineState_PCBOutStart = Define.m_OnlineState_PCBOutStart;
	m_OnlineState_PCBOutChecking = Define.m_OnlineState_PCBOutChecking;
	m_OnlineState_PCBOutFinish = Define.m_OnlineState_PCBOutFinish;
	m_OnlineState_PCBOutInsideStart = Define.m_OnlineState_PCBOutInsideStart;
	m_OnlineState_PCBOutInsideChecking = Define.m_OnlineState_PCBOutInsideChecking;;
	m_OnlineState_PCBOutInsideFinish = Define.m_OnlineState_PCBOutInsideFinish;
	m_OnlineState_PCBBackStart = Define.m_OnlineState_PCBBackStart;;
	m_OnlineState_PCBBackChecking = Define.m_OnlineState_PCBBackChecking;
	m_OnlineState_PCBBackFinish = Define.m_OnlineState_PCBBackFinish;
	m_OnlineState_PCBBackOutStart = Define.m_OnlineState_PCBBackOutStart;
	m_OnlineState_PCBBackOutChecking = Define.m_OnlineState_PCBBackOutChecking;
	m_OnlineState_PCBBackOutFinish = Define.m_OnlineState_PCBBackOutFinish;
	m_OnlineState_PCBOutInStart = Define.m_OnlineState_PCBOutInStart;
	m_OnlineState_PCBOutInChecking = Define.m_OnlineState_PCBOutInChecking;
	m_OnlineState_PCBOutInFinish = Define.m_OnlineState_PCBOutInFinish;
	m_OnlineState_PCBAutoRunStart = Define.m_OnlineState_PCBAutoRunStart;
	m_OnlineState_PCBAutoRunChecking = Define.m_OnlineState_PCBAutoRunChecking;
	m_OnlineState_PCBAutoRunFinish = Define.m_OnlineState_PCBAutoRunFinish;
	m_OnlineState_PCBDualRunStart = Define.m_OnlineState_PCBDualRunStart;
	m_OnlineState_PCBDualRunChecking = Define.m_OnlineState_PCBDualRunChecking;
	m_OnlineState_PCBDualRunFinish = Define.m_OnlineState_PCBDualRunFinish;	
	m_OnlineState_PCBInspectionPause = Define.m_OnlineState_PCBInspectionPause;
	m_OnlineState_AutoCalibration_XYZ_Home = Define.m_OnlineState_AutoCalibration_XYZ_Home;	
	m_OnlineState_AutoCalibration_2D_Current = Define.m_OnlineState_AutoCalibration_2D_Current;	
	m_OnlineState_AutoCalibration_3D_Current = Define.m_OnlineState_AutoCalibration_3D_Current;	
	m_OnlineState_AutoCalibration_3D_ZeroPlane = Define.m_OnlineState_AutoCalibration_3D_ZeroPlane;	
	m_OnlineState_AutoCalibration_3D_FactorFactor = Define.m_OnlineState_AutoCalibration_3D_FactorFactor;
	m_OnlineState_AppOpen = Define.m_OnlineState_AppOpen;
	m_OnlineState_AppClose = Define.m_OnlineState_AppClose;
	//MES_EQP_CTRL_STATE_MODE
	m_MesEqpCtrlStateText_None = Define.m_MesEqpCtrlStateText_None;
	m_MesEqpCtrlStateText_Offline = Define.m_MesEqpCtrlStateText_Offline;
	m_MesEqpCtrlStateText_Local = Define.m_MesEqpCtrlStateText_Local;
	m_MesEqpCtrlStateText_Remote = Define.m_MesEqpCtrlStateText_Remote;		
	//Field Path Mode
	m_FieldPathModeText_Hor = Define.m_FieldPathModeText_Hor;//水平優先
	m_FieldPathModeText_Ver = Define.m_FieldPathModeText_Ver;//垂直優先
	m_FieldPathModeText_User = Define.m_FieldPathModeText_User;	
	//Field Division Mode
	m_FieldDivisionModeText_MassArea = Define.m_FieldDivisionModeText_MassArea;//最大面積
	m_FieldDivisionModeText_Diagonal = Define.m_FieldDivisionModeText_Diagonal;//對角線
	m_FieldDivisionModeText_Horizontal = Define.m_FieldDivisionModeText_Horizontal;//水平線
	m_FieldDivisionModeText_Vertical = Define.m_FieldDivisionModeText_Vertical;//垂直線
	//Field Build Mode
	m_FieldBuildModeText_Matrix = Define.m_FieldBuildModeText_Matrix;//等間距
	m_FieldBuildModeText_Random_Panel=Define.m_FieldBuildModeText_Random_Panel;//任意位置-整板
	m_FieldBuildModeText_Random_Board=Define.m_FieldBuildModeText_Random_Board;//任意位置-單板
	m_FieldBuildModeText_Random_Project=Define.m_FieldBuildModeText_Random_Project;
	//Barcode Input Type
	m_BarcodeInputText_Disabled = Define.m_BarcodeInputText_Disabled;
	m_BarcodeInputText_Device = Define.m_BarcodeInputText_Device;
	m_BarcodeInputText_Handheld = Define.m_BarcodeInputText_Handheld;	
	//Barcode NG Handle Mode
	m_BarcodeNGHandleText_Pass = Define.m_BarcodeNGHandleText_Pass;
	m_BarcodeNGHandleText_Alarm = Define.m_BarcodeNGHandleText_Alarm;	
	m_BarcodeNGHandleText_Input = Define.m_BarcodeNGHandleText_Input;
	//Multi Project Test Order Mode
	m_MultiProjectTestOrderText_ByMark = Define.m_MultiProjectTestOrderText_ByMark;
	m_MultiProjectTestOrderText_InTurn = Define.m_MultiProjectTestOrderText_InTurn;
	m_MultiProjectTestOrderText_OneCycleAB = Define.m_MultiProjectTestOrderText_OneCycleAB;
	m_MultiProjectTestOrderText_OneCycleBA = Define.m_MultiProjectTestOrderText_OneCycleBA;
	//Barcode Camera Grab Mode
	m_BarcodeCameraGrabModeText_AfterFd = Define.m_BarcodeCameraGrabModeText_AfterFd;
	m_BarcodeCameraGrabModeText_Inspecting = Define.m_BarcodeCameraGrabModeText_Inspecting;	 
	//Barcode Device Grab Mode
	m_BarcodeDeviceGrabModeText_BeforePCBIn = Define.m_BarcodeDeviceGrabModeText_BeforePCBIn;
	m_BarcodeDeviceGrabModeText_WhilePCBIn = Define.m_BarcodeDeviceGrabModeText_WhilePCBIn;
	m_BarcodeDeviceGrabModeText_AfterPCBIn = Define.m_BarcodeDeviceGrabModeText_AfterPCBIn;
	m_BarcodeDeviceGrabModeText_BeforeInspect = Define.m_BarcodeDeviceGrabModeText_BeforeInspect;
	//Barcode Handheld Read Mode
	m_BarcodeHandHeldReadModeText_Manual = Define.m_BarcodeHandHeldReadModeText_Manual;
	m_BarcodeHandHeldReadModeText_Project = Define.m_BarcodeHandHeldReadModeText_Project;
	m_BarcodeHandHeldReadModeText_Panel = Define.m_BarcodeHandHeldReadModeText_Panel;
	m_BarcodeHandHeldReadModeText_Board = Define.m_BarcodeHandHeldReadModeText_Board;	
	//BARCODE_AUTO_EXPAND_MODE
	m_BarcodeAutoExpandModeText_Disable = Define.m_BarcodeAutoExpandModeText_Disable;
	m_BarcodeAutoExpandModeText_Increment = Define.m_BarcodeAutoExpandModeText_Increment;
	m_BarcodeAutoExpandModeText_AddChar01 = Define.m_BarcodeAutoExpandModeText_AddChar01;
	m_BarcodeAutoExpandModeText_AddChar02 = Define.m_BarcodeAutoExpandModeText_AddChar02;
	m_BarcodeAutoExpandModeText_Replace01 = Define.m_BarcodeAutoExpandModeText_Replace01;
	m_BarcodeAutoExpandModeText_Replace02 = Define.m_BarcodeAutoExpandModeText_Replace02;	
	m_BarcodeAutoExpandModeText_Inc_Base36 = Define.m_BarcodeAutoExpandModeText_Inc_Base36;
	//ALG_BARCODE_DIR_MODE
	m_BarcodeDirectionModeText_Auto = Define.m_BarcodeDirectionModeText_Auto;	
	m_BarcodeDirectionModeText_Hor = Define.m_BarcodeDirectionModeText_Hor;	
	m_BarcodeDirectionModeText_Ver = Define.m_BarcodeDirectionModeText_Ver;	
	m_BarcodeDirectionModeText_All = Define.m_BarcodeDirectionModeText_All;		
	//AUTO_SWITCH_WND_3D_FRAME_MODE
	m_AutoSwitchWnd3DFrame_Disable = Define.m_AutoSwitchWnd3DFrame_Disable;
	m_AutoSwitchWnd3DFrame_Enable = Define.m_AutoSwitchWnd3DFrame_Enable;
	m_AutoSwitchWnd3DFrame_BySize = Define.m_AutoSwitchWnd3DFrame_BySize;
	m_AutoSwitchWnd3DFrame_ByType = Define.m_AutoSwitchWnd3DFrame_ByType;
	m_AutoSwitchWnd3DFrame_ByGroupChange = Define.m_AutoSwitchWnd3DFrame_ByGroupChange;
	//ALARM_LOCK_MODE
	m_AlarmLockText_None = Define.m_AlarmLockText_None;
	m_AlarmLockText_AOI = Define.m_AlarmLockText_AOI;
	m_AlarmLockText_ARS = Define.m_AlarmLockText_ARS;
	//Defect From ID
	m_DefectFromText_None = Define.m_DefectFromText_None;
	m_DefectFromText_AOI = Define.m_DefectFromText_AOI;
	m_DefectFromText_ARS = Define.m_DefectFromText_ARS;
	//TOP10_SCOPE;
	m_Top10ScopeText_Model = Define.m_Top10ScopeText_Model;
	m_Top10ScopeText_PartNumber = Define.m_Top10ScopeText_PartNumber;
	m_Top10ScopeText_Component = Define.m_Top10ScopeText_Component;
	//YIELDING_SCOPE
	m_YieldingScopeText_Test = Define.m_YieldingScopeText_Test;
	m_YieldingScopeText_Panel = Define.m_YieldingScopeText_Panel;
	m_YieldingScopeText_Board = Define.m_YieldingScopeText_Board;
	m_YieldingScopeText_Component = Define.m_YieldingScopeText_Component;
	//DEFECT_PARAM_FROM_MODE
	m_DefectParamFromText_Disable = Define.m_DefectParamFromText_Disable;
	m_DefectParamFromText_Project = Define.m_DefectParamFromText_Project;
	m_DefectParamFromText_Component = Define.m_DefectParamFromText_Component;	
	//CPK_FROM_MODE
	m_CpkFromText_OffsetX = Define.m_CpkFromText_OffsetX;
	m_CpkFromText_OffsetY = Define.m_CpkFromText_OffsetY;
	m_CpkFromText_SkewAngle = Define.m_CpkFromText_SkewAngle;	 	
	//MULTI_LANGUAGE_MODE
	m_MultiLanguageText_English = Define.m_MultiLanguageText_English;
	m_MultiLanguageText_ChinTrad = Define.m_MultiLanguageText_ChinTrad;
	m_MultiLanguageText_ChinSimp = Define.m_MultiLanguageText_ChinSimp;
	m_MultiLanguageText_Local = Define.m_MultiLanguageText_Local;
	//ALG_TYPE
	m_AlgText_BrightRatio = Define.m_AlgText_BrightRatio;
	m_AlgText_OuterShort = Define.m_AlgText_OuterShort;
	m_AlgText_BlobCount = Define.m_AlgText_BlobCount;
	m_AlgText_BodyTilt = Define.m_AlgText_BodyTilt;
	m_AlgText_BarcodeRecognize = Define.m_AlgText_BarcodeRecognize;
	m_AlgText_ObjectMeasure = Define.m_AlgText_ObjectMeasure;
	m_AlgText_WidthRatio = Define.m_AlgText_WidthRatio;
	m_AlgText_Resin = Define.m_AlgText_Resin;
	m_AlgText_WireWidth = Define.m_AlgText_WireWidth;
	m_AlgHeightDetectionType1 = Define.m_AlgHeightDetectionType1;
	m_AlgHeightDetectionType2 = Define.m_AlgHeightDetectionType2;
	m_AlgHeightDetectionMeasureMode1 = Define.m_AlgHeightDetectionMeasureMode1;
	m_AlgHeightDetectionMeasureMode2 = Define.m_AlgHeightDetectionMeasureMode2;
	m_AlgHeightDetectionOutputType1 = Define.m_AlgHeightDetectionOutputType1;
	m_AlgHeightDetectionOutputType2 = Define.m_AlgHeightDetectionOutputType2;
	m_AlgText_ColorCode = Define.m_AlgText_ColorCode;
	m_AlgText_ModelMatch = Define.m_AlgText_ModelMatch;
	m_AlgText_ImageMatch = Define.m_AlgText_ImageMatch;
	m_AlgText_CharVerify = Define.m_AlgText_CharVerify;
	m_AlgText_FdMatch = Define.m_AlgText_FdMatch;
	m_AlgText_EdgeSearch = Define.m_AlgText_EdgeSearch;
	m_AlgText_ShapeVerify = Define.m_AlgText_ShapeVerify;	
	m_AlgText_AngleMeasure = Define.m_AlgText_AngleMeasure;	
	m_AlgText_PixelCompare = Define.m_AlgText_PixelCompare;		
	m_AlgText_SolderWetting = Define.m_AlgText_SolderWetting;
	m_AlgText_MeasureBlackGlue = Define.m_AlgText_MeasureBlackGlue;	
	m_AlgText_MeasureFluxArea = Define.m_AlgText_MeasureFluxArea;
	m_AlgText_MeasureCpuPin = Define.m_AlgText_MeasureCpuPin;
	m_AlgText_MeasureSIP = Define.m_AlgText_MeasureSIP;
	m_AlgText_MeasureConnector = Define.m_AlgText_MeasureConnector;
	m_AlgText_GroupCompare = Define.m_AlgText_GroupCompare;	
	//ALG_CALC_UNIT_MODE
	m_AlgCalcUnitMode_Abs = Define.m_AlgCalcUnitMode_Abs;
	m_AlgCalcUnitMode_Diff = Define.m_AlgCalcUnitMode_Diff;
	m_AlgCalcUnitMode_Ratio = Define.m_AlgCalcUnitMode_Ratio;
	//ALG_BRIGHT_AVERAGE_MODE
	m_AlgBrightAverageMode_Full = Define.m_AlgBrightAverageMode_Full;
	m_AlgBrightAverageMode_Partial = Define.m_AlgBrightAverageMode_Partial;	
	//ALG_MATCH_DOCK_MODE
	m_AlgMatchDockMode_Disable = Define.m_AlgMatchDockMode_Disable;
	m_AlgMatchDockMode_ToTip = Define.m_AlgMatchDockMode_ToTip;
	m_AlgMatchDockMode_Shoulder = Define.m_AlgMatchDockMode_Shoulder;	
	//ALG_OBJECT_SIZE_CALC_MODE
	m_AlgObjectSizeCalcMode_Boundary = Define.m_AlgObjectSizeCalcMode_Boundary;
	m_AlgObjectSizeCalcMode_Average = Define.m_AlgObjectSizeCalcMode_Average;	
	m_AlgObjectSizeCalcMode_AveRect = Define.m_AlgObjectSizeCalcMode_AveRect;
	m_AlgObjectSizeCalcMode_BlurRect = Define.m_AlgObjectSizeCalcMode_BlurRect;
	//ALG_OBJECT_HEIGHT_AVERAGE_MODE
	m_AlgObjectHeightAverageMode_Full = Define.m_AlgObjectHeightAverageMode_Full;
	m_AlgObjectHeightAverageMode_Partial = Define.m_AlgObjectHeightAverageMode_Partial;
	//ANGLE_MEASURE_MODE
	m_AlgAngleMeasureAngleMode_Skew = Define.m_AlgAngleMeasureAngleMode_Skew;
	m_AlgAngleMeasureAngleMode_Tilt = Define.m_AlgAngleMeasureAngleMode_Tilt;	
	//LINE_EQUATION_MODE
	m_AlgAngleMeasureBaseLineMode_Calc = Define.m_AlgAngleMeasureBaseLineMode_Calc;
	m_AlgAngleMeasureBaseLineMode_Hor = Define.m_AlgAngleMeasureBaseLineMode_Hor;
	m_AlgAngleMeasureBaseLineMode_Ver = Define.m_AlgAngleMeasureBaseLineMode_Ver;		
	//RESULT_ID;
	m_ResultText_None = Define.m_ResultText_None;
	m_ResultText_OK = Define.m_ResultText_OK;
	m_ResultText_NG = Define.m_ResultText_NG;
	m_ResultText_Skip = Define.m_ResultText_Skip;
	m_ResultText_Bypass = Define.m_ResultText_Bypass;
	m_ResultText_Exception = Define.m_ResultText_Exception;	
	//PART_GROUP_MODE
	m_PartGroupText_Colinearity = Define.m_PartGroupText_Colinearity;	
	m_PartGroupText_ColinearityToLine = Define.m_PartGroupText_ColinearityToLine;
	m_PartGroupText_DistPartToPart = Define.m_PartGroupText_DistPartToPart;	
	m_PartGroupText_DistPartNeighbor = Define.m_PartGroupText_DistPartNeighbor;	
	m_PartGroupText_DistPartToGroup = Define.m_PartGroupText_DistPartToGroup;	
	m_PartGroupText_DistGroupToPart = Define.m_PartGroupText_DistGroupToPart;
	m_PartGroupText_DistGroupCoordMap = Define.m_PartGroupText_DistGroupCoordMap;
	//IMAGE_SRC_MODE
	m_ImageSrcText_Gray = Define.m_ImageSrcText_Gray;
	m_ImageSrcText_Color = Define.m_ImageSrcText_Color;
	m_ImageSrcText_Red = Define.m_ImageSrcText_Red;
	m_ImageSrcText_Green = Define.m_ImageSrcText_Green;
	m_ImageSrcText_Blue = Define.m_ImageSrcText_Blue;
	m_ImageSrcText_Lightness = Define.m_ImageSrcText_Lightness;
	m_ImageSrcText_Darkness = Define.m_ImageSrcText_Darkness;	
	m_ImageSrcText_Saturation = Define.m_ImageSrcText_Saturation;		
	m_ImageSrcText_Synthesis = Define.m_ImageSrcText_Synthesis;
	m_ImageSrcText_RedRatio = Define.m_ImageSrcText_RedRatio;
	m_ImageSrcText_GreenRatio = Define.m_ImageSrcText_GreenRatio;
	m_ImageSrcText_BlueRatio = Define.m_ImageSrcText_BlueRatio;	
	m_ImageSrcText_MaxGrnBlu = Define.m_ImageSrcText_MaxGrnBlu;	
	//MASK_FUNC_MODE
	m_MaskFuncText_Calc = Define.m_MaskFuncText_Calc;
	m_MaskFuncText_Erase = Define.m_MaskFuncText_Erase;		
	//BINARY_MODE
	m_BinaryText_Disable = Define.m_BinaryText_Disable;
	m_BinaryText_ColorFilter = Define.m_BinaryText_ColorFilter;
	m_BinaryText_FixedTh = Define.m_BinaryText_FixedTh;
	m_BinaryText_DynamicTh = Define.m_BinaryText_DynamicTh;
	m_BinaryText_RelativeTh = Define.m_BinaryText_RelativeTh;	
	m_BinaryText_AdaptiveTh = Define.m_BinaryText_AdaptiveTh;	
	//EDGE_ENHANCE_MODE
	m_EdgeEnhanceText_Disable = Define.m_EdgeEnhanceText_Disable;	
	m_EdgeEnhanceText_Sobel = Define.m_EdgeEnhanceText_Sobel;
	m_EdgeEnhanceText_DarkTop = Define.m_EdgeEnhanceText_DarkTop;
	m_EdgeEnhanceText_DarkLeft = Define.m_EdgeEnhanceText_DarkLeft;
	m_EdgeEnhanceText_DarkBot = Define.m_EdgeEnhanceText_DarkBot;
	m_EdgeEnhanceText_DarkRight = Define.m_EdgeEnhanceText_DarkRight;
	//NOISE_FILTER_MODE
	m_NoiseFilterText_Diable = Define.m_NoiseFilterText_Diable;
	m_NoiseFilterText_Level  = Define.m_NoiseFilterText_Level;
	m_NoiseFilterText_Smooth = Define.m_NoiseFilterText_Smooth;
	m_NoiseFilterText_Median = Define.m_NoiseFilterText_Median;
	m_NoiseFilterText_Median2 = Define.m_NoiseFilterText_Median2;
	m_NoiseFilterText_PyramidMedian = Define.m_NoiseFilterText_PyramidMedian;
	m_NoiseFilterText_ContentAware = Define.m_NoiseFilterText_ContentAware;
	m_NoiseFilterText_Fast_Median = Define.m_NoiseFilterText_Fast_Median;
	m_NoiseFilterText_Fast_Average = Define.m_NoiseFilterText_Fast_Average;
	m_NoiseFilterText_Open = Define.m_NoiseFilterText_Open;
	m_NoiseFilterText_Close = Define.m_NoiseFilterText_Close;	
	m_NoiseFilterText_Erosion = Define.m_NoiseFilterText_Erosion;
	m_NoiseFilterText_Dilation = Define.m_NoiseFilterText_Dilation;
	m_NoiseFilterText_Gradient = Define.m_NoiseFilterText_Gradient;	
	//ALG_BRIGHT_LINE_MODE
	m_AlgBrightLineMode_Bright=Define.m_AlgBrightLineMode_Bright;
	m_AlgBrightLineMode_Dark=Define.m_AlgBrightLineMode_Dark;
	//ALG_OUTER_SHORT_EXT_MODE
	m_AlgOuterShortExtendText_None=Define.m_AlgOuterShortExtendText_None;
	m_AlgOuterShortExtendText_Left=Define.m_AlgOuterShortExtendText_Left;
	m_AlgOuterShortExtendText_Right=Define.m_AlgOuterShortExtendText_Right;
	m_AlgOuterShortExtendText_Both=Define.m_AlgOuterShortExtendText_Both;		
	//ALG_DIRECTION
	m_AlgDirText_Hor = Define.m_AlgDirText_Hor;
	m_AlgDirText_Ver = Define.m_AlgDirText_Ver;
	//ALG_BARCODE_STEP_MODE
	m_AlgBarcodeStepText_None = Define.m_AlgBarcodeStepText_None;
	m_AlgBarcodeStepText_Scale = Define.m_AlgBarcodeStepText_Scale;
	m_AlgBarcodeStepText_GainOffset = Define.m_AlgBarcodeStepText_GainOffset;
	m_AlgBarcodeStepText_Smooth = Define.m_AlgBarcodeStepText_Smooth;
	m_AlgBarcodeStepText_Open = Define.m_AlgBarcodeStepText_Open;
	m_AlgBarcodeStepText_Close = Define.m_AlgBarcodeStepText_Close;
	m_AlgBarcodeStepText_Median = Define.m_AlgBarcodeStepText_Median;
	m_AlgBarcodeStepText_Invert = Define.m_AlgBarcodeStepText_Invert;
	m_AlgBarcodeStepText_Flip = Define.m_AlgBarcodeStepText_Flip;	
	m_AlgBarcodeStepText_Fill = Define.m_AlgBarcodeStepText_Fill;
	m_AlgBarcodeStepText_Erode = Define.m_AlgBarcodeStepText_Erode;
	m_AlgBarcodeStepText_Dilate = Define.m_AlgBarcodeStepText_Dilate;
	m_AlgBarcodeStepText_Fill2D = Define.m_AlgBarcodeStepText_Fill2D;
	m_AlgBarcodeStepText_Sharp = Define.m_AlgBarcodeStepText_Sharp;	
	m_AlgBarcodeStepText_Range = Define.m_AlgBarcodeStepText_Range;	
	//FD_MATCH_MODE
	m_AlgFdMatchText_Model = Define.m_AlgFdMatchText_Model;
	m_AlgFdMatchText_Image = Define.m_AlgFdMatchText_Image;		
	//ALG_GROUP_CMP_DIR_MODE
	m_AlgGroupCompareDirText_Any = Define.m_AlgGroupCompareDirText_Any;
	m_AlgGroupCompareDirText_One = Define.m_AlgGroupCompareDirText_One;
	//ALG_3D_BASE_HEIGHT_MODE
	m_Alg3DHeightBaseText_Min = Define.m_Alg3DHeightBaseText_Min;
	m_Alg3DHeightBaseText_Max = Define.m_Alg3DHeightBaseText_Max;
	m_Alg3DHeightBaseText_Ave = Define.m_Alg3DHeightBaseText_Ave;
	m_Alg3DHeightBaseText_Mid = Define.m_Alg3DHeightBaseText_Mid;
	m_Alg3DHeightBaseText_SQR = Define.m_Alg3DHeightBaseText_SQR;
	//ALG_SEARCH_DIRECTION
	m_AlgSearchDirectionText_Forward = Define.m_AlgSearchDirectionText_Forward;
	m_AlgSearchDirectionText_Backward = Define.m_AlgSearchDirectionText_Backward;	
	//ALG_EDGE_FEATURE_MODE
	m_AlgEdgeFeatureText_W2B = Define.m_AlgEdgeFeatureText_W2B;	
	m_AlgEdgeFeatureText_B2W = Define.m_AlgEdgeFeatureText_B2W;		
	//BOX_TOWARD
	m_BoxTowardText_Up = Define.m_BoxTowardText_Up;
	m_BoxTowardText_Left = Define.m_BoxTowardText_Left;
	m_BoxTowardText_Down = Define.m_BoxTowardText_Down;
	m_BoxTowardText_Right = Define.m_BoxTowardText_Right;
	//BOX_SHAPE_MODE
	m_BoxShapeText_Rect = Define.m_BoxShapeText_Rect;
	m_BoxShapeText_RectRound = Define.m_BoxShapeText_RectRound;
	m_BoxShapeText_Ellipse = Define.m_BoxShapeText_Ellipse;
	m_BoxShapeText_Capsule = Define.m_BoxShapeText_Capsule;
	m_BoxShapeText_Bullet = Define.m_BoxShapeText_Bullet;
	m_BoxShapeText_RectHalfRound = Define.m_BoxShapeText_RectHalfRound;
	m_BoxShapeText_TShape = Define.m_BoxShapeText_TShape;
	//LAND_TYPE
	m_LandTypeText_Pad = Define.m_LandTypeText_Pad;
	m_LandTypeText_Electrode = Define.m_LandTypeText_Electrode;
	m_LandTypeText_ICLead = Define.m_LandTypeText_ICLead;	
	m_LandTypeText_ConLead = Define.m_LandTypeText_ConLead;
	m_LandTypeText_DipLead = Define.m_LandTypeText_DipLead;
	//MODEL_PART//模組部位
	m_ModelPadText = Define.m_ModelPadText;	
	m_ModelBodyText = Define.m_ModelBodyText;	
	m_ModelLeadText = Define.m_ModelLeadText;	
	m_ModelLeadTipText = Define.m_ModelLeadTipText;	
	m_ModelLeadShoulderText = Define.m_ModelLeadShoulderText;
	//MODEL_MASK
	m_ModelMaskPadText = Define.m_ModelMaskPadText;	
	m_ModelMaskBodyText = Define.m_ModelMaskBodyText;	
	m_ModelMaskLeadText = Define.m_ModelMaskLeadText;	
	m_ModelMaskLeadTipText = Define.m_ModelMaskLeadTipText;	
	m_ModelMaskLeadShoulderText = Define.m_ModelMaskLeadShoulderText;
	//MODEL_GROUP
	m_ModelGroupText_All = Define.m_ModelGroupText_All;	
	//MODEL_TYPE
	m_ModelTypeText_Null = Define.m_ModelTypeText_Null;
	m_ModelTypeText_Chip = Define.m_ModelTypeText_Chip;
	m_ModelTypeText_ChipC = Define.m_ModelTypeText_ChipC;
	m_ModelTypeText_ChipR = Define.m_ModelTypeText_ChipR;
	m_ModelTypeText_ChipL = Define.m_ModelTypeText_ChipL;
	m_ModelTypeText_ChipLed = Define.m_ModelTypeText_ChipLed;
	m_ModelTypeText_Melf = Define.m_ModelTypeText_Melf;
	m_ModelTypeText_Electrode = Define.m_ModelTypeText_Electrode;
	m_ModelTypeText_Tant = Define.m_ModelTypeText_Tant;
	m_ModelTypeText_CN = Define.m_ModelTypeText_CN;
	m_ModelTypeText_RN = Define.m_ModelTypeText_RN;
	m_ModelTypeText_SOT = Define.m_ModelTypeText_SOT;
	m_ModelTypeText_ElecCap = Define.m_ModelTypeText_ElecCap;
	m_ModelTypeText_LedArray = Define.m_ModelTypeText_LedArray;		
	m_ModelTypeText_NoLead = Define.m_ModelTypeText_NoLead;
	m_ModelTypeText_NoLeadDN = Define.m_ModelTypeText_NoLeadDN;
	m_ModelTypeText_NoLeadQFN = Define.m_ModelTypeText_NoLeadQFN;
	m_ModelTypeText_NoLeadOSC = Define.m_ModelTypeText_NoLeadOSC;
	m_ModelTypeText_LeadCom = Define.m_ModelTypeText_LeadCom;
	m_ModelTypeText_LeadComSOP = Define.m_ModelTypeText_LeadComSOP;
	m_ModelTypeText_LeadComQFP = Define.m_ModelTypeText_LeadComQFP;
	m_ModelTypeText_LeadComSOT = Define.m_ModelTypeText_LeadComSOT;	
	m_ModelTypeText_JLeadCom = Define.m_ModelTypeText_JLeadCom;
	m_ModelTypeText_JLeadComSOJ = Define.m_ModelTypeText_JLeadComSOJ;
	m_ModelTypeText_JLeadComPLCC = Define.m_ModelTypeText_JLeadComPLCC;
	m_ModelTypeText_CompositeCom = Define.m_ModelTypeText_CompositeCom;
	m_ModelTypeText_PowerTransistor = Define.m_ModelTypeText_PowerTransistor;
	m_ModelTypeText_Connector = Define.m_ModelTypeText_Connector;
	m_ModelTypeText_BGA = Define.m_ModelTypeText_BGA;
	m_ModelTypeText_Fd = Define.m_ModelTypeText_Fd;
	m_ModelTypeText_Barcode = Define.m_ModelTypeText_Barcode;
	m_ModelTypeText_Pad = Define.m_ModelTypeText_Pad;
	m_ModelTypeText_GoldFinger = Define.m_ModelTypeText_GoldFinger;
	m_ModelTypeText_DipLead = Define.m_ModelTypeText_DipLead;
	m_ModelTypeText_Ohters = Define.m_ModelTypeText_Ohters;
	//WND_LOGIC_TYPE
	m_WndLogText_None = Define.m_WndLogText_None;
	m_WndLogText_GroupID = Define.m_WndLogText_GroupID;
	m_WndLogText_DefectID = Define.m_WndLogText_DefectID;
	//WND_FOLLOW_MODE
	m_WndFollowText_None = Define.m_WndFollowText_None;
	m_WndFollowText_Pad = Define.m_WndFollowText_Pad;
	m_WndFollowText_Part = Define.m_WndFollowText_Part;
	m_WndFollowText_PadBody = Define.m_WndFollowText_PadBody;	
	m_WndFollowText_PadLead = Define.m_WndFollowText_PadLead;	
	m_WndFollowText_PartBody = Define.m_WndFollowText_PartBody;	
	m_WndFollowText_PartLead = Define.m_WndFollowText_PartLead;	
	//WND_RGN_LINK_MODE
	m_WndRgnLinkText_None = Define.m_WndRgnLinkText_None;
	m_WndRgnLinkText_Pad = Define.m_WndRgnLinkText_Pad;
	m_WndRgnLinkText_Body = Define.m_WndRgnLinkText_Body;
	m_WndRgnLinkText_Lead = Define.m_WndRgnLinkText_Lead;
	m_WndRgnLinkText_PadTip = Define.m_WndRgnLinkText_PadTip;
	m_WndRgnLinkText_PadRgn = Define.m_WndRgnLinkText_PadRgn;
	m_WndRgnLinkText_PadBodyRgn = Define.m_WndRgnLinkText_PadBodyRgn;
	m_WndRgnLinkText_PadRgnInner = Define.m_WndRgnLinkText_PadRgnInner;	
	m_WndRgnLinkText_LeadTip = Define.m_WndRgnLinkText_LeadTip;
	m_WndRgnLinkText_LeadShoulder = Define.m_WndRgnLinkText_LeadShoulder;
	m_WndRgnLinkText_LeadTipShoulder = Define.m_WndRgnLinkText_LeadTipShoulder;	
	//WND_SYNC_MOVE_MODE
	m_WndSyncMoveText_Mirror = Define.m_WndSyncMoveText_Mirror;
	m_WndSyncMoveText_Rotate = Define.m_WndSyncMoveText_Rotate;
	m_WndSyncMoveText_Symmetry = Define.m_WndSyncMoveText_Symmetry;	
	//WND_CONSTRAIN_MODE
	m_WndConstrainText_Disable = Define.m_WndConstrainText_Disable;//局限-關閉
	m_WndConstrainText_PadRgnMove = Define.m_WndConstrainText_PadRgnMove;//局限-焊盤範圍內-All
	m_WndConstrainText_PadRgnXMove = Define.m_WndConstrainText_PadRgnXMove;//局限-焊盤範圍內-X
	m_WndConstrainText_PadRgnYMove = Define.m_WndConstrainText_PadRgnYMove;//局限-焊盤範圍內-Y	
	m_WndConstrainText_PadRgnScale = Define.m_WndConstrainText_PadRgnScale;//局限-焊盤範圍內-All
	m_WndConstrainText_PadRgnXScale = Define.m_WndConstrainText_PadRgnXScale;//局限-焊盤範圍內-X
	m_WndConstrainText_PadRgnYScale = Define.m_WndConstrainText_PadRgnYScale;//局限-焊盤範圍內-Y
	//WND_DEFECT_ID	
	m_MapWndDefectIDIndex = Define.m_MapWndDefectIDIndex;

	m_WndDefectText_None = Define.m_WndDefectText_None;
	m_WndDefectText_PadAlign = Define.m_WndDefectText_PadAlign;
	m_WndDefectText_PartAlign = Define.m_WndDefectText_PartAlign;
	m_WndDefectText_PadAdjust = Define.m_WndDefectText_PadAdjust;
	m_WndDefectText_LeadAdjust = Define.m_WndDefectText_LeadAdjust;
	m_WndDefectText_ClassCheck = Define.m_WndDefectText_ClassCheck;
	m_WndDefectText_BaseValue = Define.m_WndDefectText_BaseValue;
	m_WndDefectText_BodyMissing = Define.m_WndDefectText_BodyMissing;
	m_WndDefectText_BodyOffset = Define.m_WndDefectText_BodyOffset;
	m_WndDefectText_BodyTilt = Define.m_WndDefectText_BodyTilt;
	m_WndDefectText_BodyPolarity = Define.m_WndDefectText_BodyPolarity;
	m_WndDefectText_BodyTurnOver = Define.m_WndDefectText_BodyTurnOver;
	m_WndDefectText_BodyMount = Define.m_WndDefectText_BodyMount;
	m_WndDefectText_BodyWrongCode = Define.m_WndDefectText_BodyWrongCode;
	m_WndDefectText_BodyWrongText = Define.m_WndDefectText_BodyWrongText;	
	m_WndDefectText_BodyTombstone = Define.m_WndDefectText_BodyTombstone;
	m_WndDefectText_BodyBillboard = Define.m_WndDefectText_BodyBillboard;
	m_WndDefectText_BodyDamaged = Define.m_WndDefectText_BodyDamaged;
	m_WndDefectText_SolderPoor = Define.m_WndDefectText_SolderPoor;
	m_WndDefectText_SolderOpen = Define.m_WndDefectText_SolderOpen;
	m_WndDefectText_SolderPadExposed = Define.m_WndDefectText_SolderPadExposed;
	m_WndDefectText_SolderBridge = Define.m_WndDefectText_SolderBridge;
	m_WndDefectText_SolderBead = Define.m_WndDefectText_SolderBead;
	m_WndDefectText_SolderExcess = Define.m_WndDefectText_SolderExcess;
	m_WndDefectText_LeadLifted = Define.m_WndDefectText_LeadLifted;
	m_WndDefectText_LeadBended = Define.m_WndDefectText_LeadBended;
	m_WndDefectText_LeadProtruded = Define.m_WndDefectText_LeadProtruded;
	m_WndDefectText_PadScratch = Define.m_WndDefectText_PadScratch;
	m_WndDefectText_ForeignBody = Define.m_WndDefectText_ForeignBody;

	m_WndDefectText_UserDefine_01 = Define.m_WndDefectText_UserDefine_01;
	m_WndDefectText_UserDefine_02 = Define.m_WndDefectText_UserDefine_02;
	m_WndDefectText_UserDefine_03 = Define.m_WndDefectText_UserDefine_03;
	m_WndDefectText_UserDefine_04 = Define.m_WndDefectText_UserDefine_04;
	m_WndDefectText_UserDefine_05 = Define.m_WndDefectText_UserDefine_05;
	m_WndDefectText_UserDefine_06 = Define.m_WndDefectText_UserDefine_06;
	m_WndDefectText_UserDefine_07 = Define.m_WndDefectText_UserDefine_07;
	m_WndDefectText_UserDefine_08 = Define.m_WndDefectText_UserDefine_08;
	m_WndDefectText_UserDefine_09 = Define.m_WndDefectText_UserDefine_09;
	m_WndDefectText_UserDefine_10 = Define.m_WndDefectText_UserDefine_10;

	//BASE_PLANE_PROC_TYPE
	m_BasePlaneProcText_1 = Define.m_BasePlaneProcText_1;
	m_BasePlaneProcText_2 = Define.m_BasePlaneProcText_2;

	//BASE_PLANE_AUTO_REGION_MODE
	m_BasePlaneAutoRgnText_Disable = Define.m_BasePlaneAutoRgnText_Disable;
	m_BasePlaneAutoRgnText_Group = Define.m_BasePlaneAutoRgnText_Group;
	m_BasePlaneAutoRgnText_Lowest = Define.m_BasePlaneAutoRgnText_Lowest;

	//BASE_PLANE_BODY_OUTSIDE_MODE
	m_BasePlaneBodyOutsideText_Disable = Define.m_BasePlaneBodyOutsideText_Disable;
	m_BasePlaneBodyOutsideText_Body = Define.m_BasePlaneBodyOutsideText_Body;
	m_BasePlaneBodyOutsideText_BodyLand = Define.m_BasePlaneBodyOutsideText_BodyLand;	

	//CALC_BASE_PLANE_MODE
	m_CalcBasePlaneText_Disable = Define.m_CalcBasePlaneText_Disable;	
	m_CalcBasePlaneText_Ave = Define.m_CalcBasePlaneText_Ave;//平均值
	m_CalcBasePlaneText_Corner = Define.m_CalcBasePlaneText_Corner;//四個端點
	m_CalcBasePlaneText_IsoData = Define.m_CalcBasePlaneText_IsoData;//Iso data
	m_CalcBasePlaneText_Ostu = Define.m_CalcBasePlaneText_Ostu;//OTSU
	m_CalcBasePlaneText_CornerOnly = Define.m_CalcBasePlaneText_CornerOnly;//只有四個端點
	m_CalcBasePlaneText_Surround = Define.m_CalcBasePlaneText_Surround;//周圍
	m_CalcBasePlaneText_AutoLower = Define.m_CalcBasePlaneText_AutoLower;
	m_CalcBasePlaneText_Panel = Define.m_CalcBasePlaneText_Panel;
	m_CalcBasePlaneText_Local = Define.m_CalcBasePlaneText_Local;

	//Fd
	m_FdText_NG = Define.m_FdText_NG;

	//Barcode 
	m_BarcodeText_NG = Define.m_BarcodeText_NG;	 

	//色彩群組文字
	m_ProjectColorGroupText_Pad = Define.m_ProjectColorGroupText_Pad;
	m_ProjectColorGroupText_Void = Define.m_ProjectColorGroupText_Void;
	m_ProjectColorGroupText_Body = Define.m_ProjectColorGroupText_Body;
	m_ProjectColorGroupText_Board = Define.m_ProjectColorGroupText_Board;
	m_ProjectColorGroupText_Solder = Define.m_ProjectColorGroupText_Solder;
	m_ProjectColorGroupText_Ohters = Define.m_ProjectColorGroupText_Ohters;

	//軸控加速度調整模式
	m_MotionAccTimeAdjustText_Off = Define.m_MotionAccTimeAdjustText_Off;
	m_MotionAccTimeAdjustText_Fix = Define.m_MotionAccTimeAdjustText_Fix;
	m_MotionAccTimeAdjustText_Min = Define.m_MotionAccTimeAdjustText_Min;
	m_MotionAccTimeAdjustText_Gamma = Define.m_MotionAccTimeAdjustText_Gamma;
	//AAAAAAAAAAAAAAAAAA = Define.AAAAAAAAAAAAAAAAAA;
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::GetUILanguageString(LPCTSTR Section, LPCTSTR Key, LPCTSTR Default, CString &String)//取得視窗文字
{
	return AOIDataCollect.GetUILanguageString(Section, Key, Default, String);	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildWndDefectIDMapIndex()
{	
	std::map<WND_DEFECT_ID, size_t> &Map=m_MapWndDefectIDIndex;//映射表-瑕疵代碼-引數
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();

	Map.clear();	
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		WND_DEFECT_ID WndDefectID=List[i];
		auto iter=Map.find(WndDefectID);
		if ( iter==Map.end() )
		{	Map.insert(std::pair<WND_DEFECT_ID, size_t>(WndDefectID, i));	}
		else
		{	Map[WndDefectID] = i; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::LoadDefineTextFile()//載入定義文字檔案
{	
	CString Default;
	CString KeyName;
	CString Section;
	
	Section = _T("OTHERS");		
	//Others
	KeyName = _T("ML_ID"); Default = m_IDText;
	GetUILanguageString(Section, KeyName, Default, m_IDText);	
	KeyName = _T("ML_TO"); Default = m_ToText;
	GetUILanguageString(Section, KeyName, Default, m_ToText);
	KeyName = _T("ML_FD"); Default = m_FdText;
	GetUILanguageString(Section, KeyName, Default, m_FdText);	
	KeyName = _T("ML_ADD"); Default = m_AddText;
	GetUILanguageString(Section, KeyName, Default, m_AddText);
	KeyName = _T("ML_SET"); Default = m_SetText;
	GetUILanguageString(Section, KeyName, Default, m_SetText);	
	KeyName = _T("ML_GET"); Default = m_GetText;
	GetUILanguageString(Section, KeyName, Default, m_GetText);	
	KeyName = _T("ML_PAD"); Default = m_PadText;
	GetUILanguageString(Section, KeyName, Default, m_PadText);
	KeyName = _T("ML_WND"); Default = m_WndText;
	GetUILanguageString(Section, KeyName, Default, m_WndText);
	KeyName = _T("ML_AVE"); Default = m_AveText;
	GetUILanguageString(Section, KeyName, Default, m_AveText);	
	KeyName = _T("ML_ALL"); Default = m_AllText;
	GetUILanguageString(Section, KeyName, Default, m_AllText);
	KeyName = _T("ML_MAX"); Default = m_MaxText;
	GetUILanguageString(Section, KeyName, Default, m_MaxText);
	KeyName = _T("ML_MIN"); Default = m_MinText;
	GetUILanguageString(Section, KeyName, Default, m_MinText);	
	KeyName = _T("ML_FROM"); Default = m_FromText;
	GetUILanguageString(Section, KeyName, Default, m_FromText);		
	KeyName = _T("ML_AXIS"); Default = m_AxisText;
	GetUILanguageString(Section, KeyName, Default, m_AxisText);
	KeyName = _T("ML_LINE"); Default = m_LineText;
	GetUILanguageString(Section, KeyName, Default, m_LineText);
	KeyName = _T("ML_AREA"); Default = m_AreaText;
	GetUILanguageString(Section, KeyName, Default, m_AreaText);	
	KeyName = _T("ML_LANE"); Default = m_LaneText;
	GetUILanguageString(Section, KeyName, Default, m_LaneText);	
	KeyName = _T("ML_TIME"); Default = m_TimeText;
	GetUILanguageString(Section, KeyName, Default, m_TimeText);
	KeyName = _T("ML_TEST"); Default = m_TestText;
	GetUILanguageString(Section, KeyName, Default, m_TestText);
	KeyName = _T("ML_NAME"); Default = m_NameText;
	GetUILanguageString(Section, KeyName, Default, m_NameText);
	KeyName = _T("ML_SKEW"); Default = m_SkewText;
	GetUILanguageString(Section, KeyName, Default, m_SkewText);	
	KeyName = _T("ML_MARK"); Default = m_MarkText;
	GetUILanguageString(Section, KeyName, Default, m_MarkText);
	KeyName = _T("ML_GAIN"); Default = m_GainText;
	GetUILanguageString(Section, KeyName, Default, m_GainText);	
	KeyName = _T("ML_GRAY"); Default = m_GrayText;
	GetUILanguageString(Section, KeyName, Default, m_GrayText);	
	KeyName = _T("ML_SIZE"); Default = m_SizeText;
	GetUILanguageString(Section, KeyName, Default, m_SizeText);	
	KeyName = _T("ML_LAND"); Default = m_LandText;
	GetUILanguageString(Section, KeyName, Default, m_LandText);	
	KeyName = _T("ML_RANGE"); Default = m_RangeText;
	GetUILanguageString(Section, KeyName, Default, m_RangeText);
	KeyName = _T("ML_CLASS"); Default = m_ClassText;
	GetUILanguageString(Section, KeyName, Default, m_ClassText);
	KeyName = _T("ML_CLEAR"); Default = m_ClearText;
	GetUILanguageString(Section, KeyName, Default, m_ClearText);
	KeyName = _T("ML_RATIO"); Default = m_RatioText;
	GetUILanguageString(Section, KeyName, Default, m_RatioText);
	KeyName = _T("ML_INDEX"); Default = m_IndexText;
	GetUILanguageString(Section, KeyName, Default, m_IndexText);
	KeyName = _T("ML_ERROR"); Default = m_ErrorText;
	GetUILanguageString(Section, KeyName, Default, m_ErrorText);
	KeyName = _T("ML_PANEL"); Default = m_PanelText;
	GetUILanguageString(Section, KeyName, Default, m_PanelText);
	KeyName = _T("ML_BOARD"); Default = m_BoardText;
	GetUILanguageString(Section, KeyName, Default, m_BoardText);
	KeyName = _T("ML_MODEL"); Default = m_ModelText;
	GetUILanguageString(Section, KeyName, Default, m_ModelText);
	KeyName = _T("ML_GROUP"); Default = m_GroupText;
	GetUILanguageString(Section, KeyName, Default, m_GroupText);	
	KeyName = _T("ML_SCORE"); Default = m_ScoreText;
	GetUILanguageString(Section, KeyName, Default, m_ScoreText);
	KeyName = _T("ML_SCALE"); Default = m_ScaleText;
	GetUILanguageString(Section, KeyName, Default, m_ScaleText);
	KeyName = _T("ML_PIXEL"); Default = m_PixelText;
	GetUILanguageString(Section, KeyName, Default, m_PixelText);
	KeyName = _T("ML_SIZE_X"); Default = m_SizeXText;
	GetUILanguageString(Section, KeyName, Default, m_SizeXText);
	KeyName = _T("ML_SIZE_Y"); Default = m_SizeYText;
	GetUILanguageString(Section, KeyName, Default, m_SizeYText);		
	KeyName = _T("ML_METHOD"); Default = m_MethodText;
	GetUILanguageString(Section, KeyName, Default, m_MethodText);
	KeyName = _T("ML_TOWARD"); Default = m_TowardText;
	GetUILanguageString(Section, KeyName, Default, m_TowardText);
	KeyName = _T("ML_CLONE"); Default = m_CloneText;
	GetUILanguageString(Section, KeyName, Default, m_CloneText);
	KeyName = _T("ML_CREATE"); Default = m_CreateText;
	GetUILanguageString(Section, KeyName, Default, m_CreateText);
	KeyName = _T("ML_MODIFY"); Default = m_ModifyText;
	GetUILanguageString(Section, KeyName, Default, m_ModifyText);
	KeyName = _T("ML_DELETE"); Default = m_DeleteText;
	GetUILanguageString(Section, KeyName, Default, m_DeleteText);
	KeyName = _T("ML_DEFECT"); Default = m_DefectText;
	GetUILanguageString(Section, KeyName, Default, m_DefectText);
	KeyName = _T("ML_RESULT"); Default = m_ResultText;
	GetUILanguageString(Section, KeyName, Default, m_ResultText);
	KeyName = _T("ML_UNSET"); Default = m_UnsetText;
	GetUILanguageString(Section, KeyName, Default, m_UnsetText);
	KeyName = _T("ML_ANGLE"); Default = m_AngleText;
	GetUILanguageString(Section, KeyName, Default, m_AngleText);	
	KeyName = _T("ML_WIDTH"); Default = m_WidthText;
	GetUILanguageString(Section, KeyName, Default, m_WidthText);	
	KeyName = _T("ML_HEIGHT"); Default = m_HeightText;
	GetUILanguageString(Section, KeyName, Default, m_HeightText);
	KeyName = _T("ML_VOLUME"); Default = m_VolumeText;
	GetUILanguageString(Section, KeyName, Default, m_VolumeText);	
	KeyName = _T("ML_OFFSET"); Default = m_OffsetText;
	GetUILanguageString(Section, KeyName, Default, m_OffsetText);
	KeyName = _T("ML_FINISH"); Default = m_FinishText;
	GetUILanguageString(Section, KeyName, Default, m_FinishText);	
	KeyName = _T("ML_PROJECT"); Default = m_ProjectText;
	GetUILanguageString(Section, KeyName, Default, m_ProjectText);
	KeyName = _T("ML_PATTERN"); Default = m_PatternText;
	GetUILanguageString(Section, KeyName, Default, m_PatternText);
	KeyName = _T("ML_DEFAULT"); Default = m_DefaultText;
	GetUILanguageString(Section, KeyName, Default, m_DefaultText);
	KeyName = _T("ML_THROUGH"); Default = m_ThroughText;
	GetUILanguageString(Section, KeyName, Default, m_ThroughText);
	KeyName = _T("ML_BARCODE"); Default = m_BarcodeText;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeText);
	KeyName = _T("ML_WARNING"); Default = m_WarningText;
	GetUILanguageString(Section, KeyName, Default, m_WarningText);
	KeyName = _T("ML_GROUP_ID"); Default = m_GroupIDText;
	GetUILanguageString(Section, KeyName, Default, m_GroupIDText);
	KeyName = _T("ML_BYPASSED"); Default = m_BypassedText;
	GetUILanguageString(Section, KeyName, Default, m_BypassedText);
	KeyName = _T("ML_DISTRICT"); Default = m_DistrictText;
	GetUILanguageString(Section, KeyName, Default, m_DistrictText);
	KeyName = _T("ML_DISTANCE"); Default = m_DistanceText;
	GetUILanguageString(Section, KeyName, Default, m_DistanceText);
	KeyName = _T("ML_VELOCITY"); Default = m_VelocityText;
	GetUILanguageString(Section, KeyName, Default, m_VelocityText);
	KeyName = _T("ML_RELATIVE"); Default = m_RelativeText;
	GetUILanguageString(Section, KeyName, Default, m_RelativeText);	
	KeyName = _T("ML_YIELD"); Default = m_YieldText;
	GetUILanguageString(Section, KeyName, Default, m_YieldText);		
	KeyName = _T("ML_CONTINUE"); Default = m_ContinueText;
	GetUILanguageString(Section, KeyName, Default, m_ContinueText);	
	KeyName = _T("ML_CONTRAST"); Default = m_ContrastText;
	GetUILanguageString(Section, KeyName, Default, m_ContrastText);	
	KeyName = _T("ML_THICKNESS"); Default = m_ThicknessText;
	GetUILanguageString(Section, KeyName, Default, m_ThicknessText);
	KeyName = _T("ML_ALGORITHM"); Default = m_AlgorithmText;
	GetUILanguageString(Section, KeyName, Default, m_AlgorithmText);
	KeyName = _T("ML_LOCK_SCREEN"); Default = m_LockScreenText;
	GetUILanguageString(Section, KeyName, Default, m_LockScreenText);
	KeyName = _T("ML_VERTICAL"); Default = m_VerticalText;
	GetUILanguageString(Section, KeyName, Default, m_VerticalText);
	KeyName = _T("ML_HORIZONTAL"); Default = m_HorizontalText;
	GetUILanguageString(Section, KeyName, Default, m_HorizontalText);
	KeyName = _T("ML_COMPONENT"); Default = m_ComponentText;
	GetUILanguageString(Section, KeyName, Default, m_ComponentText);
	KeyName = _T("ML_PART_NUMBER"); Default = m_PartNumberText;
	GetUILanguageString(Section, KeyName, Default, m_PartNumberText);
	KeyName = _T("ML_NOZZLE_NAME"); Default = m_NozzleNameText;
	GetUILanguageString(Section, KeyName, Default, m_NozzleNameText);

	KeyName = _T("ML_UNDEFINED"); Default = m_UndefinedText;
	GetUILanguageString(Section, KeyName, Default, m_UndefinedText);
	KeyName = _T("ML_ACCELERATION"); Default = m_AccelerationText;
	GetUILanguageString(Section, KeyName, Default, m_AccelerationText);
	KeyName = _T("ML_BARCODE_DEVICE"); Default = m_BarcodeDeviceText;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDeviceText);	
	KeyName = _T("ML_SAFTY_BYPASS"); Default = m_SaftyBypassText;
	GetUILanguageString(Section, KeyName, Default, m_SaftyBypassText);	
	KeyName = _T("ML_WAIT_FOR_CCS"); Default = m_WaitForCCSText;
	GetUILanguageString(Section, KeyName, Default, m_WaitForCCSText);	
	KeyName = _T("ML_WAIT_FOR_REPAIR"); Default = m_WaitForRepairText;
	GetUILanguageString(Section, KeyName, Default, m_WaitForRepairText);	

	KeyName = _T("ML_GRAB"); Default = m_GrabText;
	GetUILanguageString(Section, KeyName, Default, m_GrabText);		
	KeyName = _T("ML_COUNT"); Default = m_CountText;
	GetUILanguageString(Section, KeyName, Default, m_CountText);	
	KeyName = _T("ML_CALCULATE"); Default = m_CalculateText;
	GetUILanguageString(Section, KeyName, Default, m_CalculateText);	
	KeyName = _T("ML_VERSION_CODE"); Default = m_VersionCodeText;
	GetUILanguageString(Section, KeyName, Default, m_VersionCodeText);	

	KeyName = _T("ML_INSPECTION_RESULT_FAULT"); Default = m_InspectionResultFaultText;
	GetUILanguageString(Section, KeyName, Default, m_InspectionResultFaultText);
	KeyName = _T("ML_DO_YOU_WANT_TO_CLEAR_THE_STATISTIC_RECORDS"); Default = m_DoYouWantToClearTheStatisticRecordsText;
	GetUILanguageString(Section, KeyName, Default, m_DoYouWantToClearTheStatisticRecordsText);
	
	//COLOR
	KeyName = _T("ML_COLOR_RED"); Default = m_ColorText_Red;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Red);
	KeyName = _T("ML_COLOR_GREEN"); Default = m_ColorText_Green;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Green);
	KeyName = _T("ML_COLOR_BLUE"); Default = m_ColorText_Blue;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Blue);
	KeyName = _T("ML_COLOR_BLACK"); Default = m_ColorText_Black;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Black);
	KeyName = _T("ML_COLOR_GRAY"); Default = m_ColorText_Gray;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Gray);
	KeyName = _T("ML_COLOR_WHITE"); Default = m_ColorText_White;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_White);
	KeyName = _T("ML_COLOR_COLOR"); Default = m_ColorText_Color;
	GetUILanguageString(Section, KeyName, Default, m_ColorText_Color);	
	//OK/Cancel
	KeyName = _T("ML_OK"); Default = m_OKText;
	GetUILanguageString(Section, KeyName, Default, m_OKText);
	KeyName = _T("ML_CANCEL"); Default = m_CancelText;
	GetUILanguageString(Section, KeyName, Default, m_CancelText);
	KeyName = _T("ML_DECODE"); Default = m_DecodeText;
	GetUILanguageString(Section, KeyName, Default, m_DecodeText);	
	KeyName = _T("ML_LEVEL"); Default = m_LevelText;
	GetUILanguageString(Section, KeyName, Default, m_LevelText);		
	//Enable/Disable
	KeyName = _T("FN_ENABLE"); Default = m_EnableText;
	GetUILanguageString(Section, KeyName, Default, m_EnableText);
	KeyName = _T("FN_DISABLE"); Default = m_DisabeText;
	GetUILanguageString(Section, KeyName, Default, m_DisabeText);	
	//Positive/Negative
	KeyName = _T("FN_POSITIVE"); Default = m_PositiveText;
	GetUILanguageString(Section, KeyName, Default, m_PositiveText);
	KeyName = _T("FN_NEGATIVE"); Default = m_NegativeText;
	GetUILanguageString(Section, KeyName, Default, m_NegativeText);	
	//DISTRICT_ID
	Section = _T("DISTRICT_ID");
	KeyName = _T("DISTRICT_ID_A"); Default = m_DistrictText_A;
	GetUILanguageString(Section, KeyName, Default, m_DistrictText_A);
	KeyName = _T("DISTRICT_ID_B"); Default = m_DistrictText_B;
	GetUILanguageString(Section, KeyName, Default, m_DistrictText_B);	
	//WND_DEFECT_ITEM
	Section = _T("WND_DEFECT_ITEM");
	KeyName = _T("WND_DEFECT_ITEM_DISABLE"); Default = m_WndDefectItem_Disable;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectItem_Disable);
	KeyName = _T("WND_DEFECT_ITEM_ENABLE"); Default = m_WndDefectItem_Enable;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectItem_Enable);	
	KeyName = _T("WND_DEFECT_ITEM_NO_SHOW"); Default = m_WndDefectItem_NoShow;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectItem_NoShow);	
	//USER_LEVEL_MODE
	Section = _T("USER_LEVEL_MODE");	
	KeyName = _T("USER_LEVEL_SIGN_OUT"); Default = m_UserLevelText_SignOut;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_SignOut);
	KeyName = _T("USER_LEVEL_OPERATOR"); Default = m_UserLevelText_Operator;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_Operator);
	KeyName = _T("USER_LEVEL_ENGINEER"); Default = m_UserLevelText_Engineer;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_Engineer);	
	KeyName = _T("USER_LEVEL_SUPERVISOR"); Default = m_UserLevelText_Supervisor;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_Supervisor);	
	KeyName = _T("USER_LEVEL_JET_FAE"); Default = m_UserLevelText_JETFAE;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_JETFAE);
	KeyName = _T("USER_LEVEL_JET_SENIOR"); Default = m_UserLevelText_JETSENIOR;
	GetUILanguageString(Section, KeyName, Default, m_UserLevelText_JETSENIOR);	
	//USER_LOGIN_MODE
	Section = _T("USER_LOGIN_MODE");	
	KeyName = _T("USER_LOGIN_DISABLE"); Default = m_UserLoginText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginText_Disable);
	KeyName = _T("USER_LOGIN_OPERATOR"); Default = m_UserLoginText_Operator;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginText_Operator);
	KeyName = _T("USER_LOGIN_ENGINEER"); Default = m_UserLoginText_Engineer;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginText_Engineer);	
	//USER_LOGIN_OPTIONS
	Section = _T("USER_LOGIN_OPTIONS");
	KeyName = _T("USER_LOGIN_OPTIONS_PASSWORD"); Default = m_UserLoginOptionsText_Password;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginOptionsText_Password);
	KeyName = _T("USER_LOGIN_OPTIONS_FINGERPRINT"); Default = m_UserLoginOptionsText_Fingerprint;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginOptionsText_Fingerprint);
	KeyName = _T("USER_LOGIN_OPTIONS_FINGERPRINT_ONLY"); Default = m_UserLoginOptionsText_FingerprintOnly;
	GetUILanguageString(Section, KeyName, Default, m_UserLoginOptionsText_FingerprintOnly);
	//OPEN_PROJECT_MODE
	Section = _T("OPEN_PROJECT_MODE");
	KeyName = _T("OPEN_PROJECT_FILE"); Default = m_OpenProjectText_File;
	GetUILanguageString(Section, KeyName, Default, m_OpenProjectText_File);
	KeyName = _T("OPEN_PROJECT_CODE"); Default = m_OpenProjectText_Code;
	GetUILanguageString(Section, KeyName, Default, m_OpenProjectText_Code);
	//VERIFY_PROJECT_MODE
	Section = _T("VERIFY_PROJECT_MODE");
	KeyName = _T("VERIFY_PROJECT_DISABLE"); Default = m_VerifyProjectText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_VerifyProjectText_Disable);
	KeyName = _T("VERIFY_PROJECT_FILENAME"); Default = m_VerifyProjectText_Filename;
	GetUILanguageString(Section, KeyName, Default, m_VerifyProjectText_Filename);
	//ONLINE_INPUT_TIMING
	Section = _T("ONLINE_INPUT_TIMING");
	KeyName = _T("ONLINE_INPUT_DISABLE"); Default = m_OnlineInputTiming_Disalbe;
	GetUILanguageString(Section, KeyName, Default, m_OnlineInputTiming_Disalbe);
	KeyName = _T("ONLINE_INPUT_BEFORE_SIGN_IN"); Default = m_OnlineInputTiming_BeforeSignIn;
	GetUILanguageString(Section, KeyName, Default, m_OnlineInputTiming_BeforeSignIn);		
	//AOI_CUSTOMER_ID
	Section = _T("AOI_CUSTOMER_ID");
	KeyName = _T("AOI_CUSTOMER_ID_JET_TWN"); Default = m_AOICustomerIDText_JET_TWN;
	GetUILanguageString(Section, KeyName, Default, m_AOICustomerIDText_JET_TWN);
	KeyName = _T("AOI_CUSTOMER_ID_PEGATRON_TWN"); Default = m_AOICustomerIDText_PegaTron_TWN;
	GetUILanguageString(Section, KeyName, Default, m_AOICustomerIDText_PegaTron_TWN);
	KeyName = _T("AOI_CUSTOMER_ID_KINPO_YUEYANG"); Default = m_AOICustomerIDText_Kinpo_YueYang;
	GetUILanguageString(Section, KeyName, Default, m_AOICustomerIDText_Kinpo_YueYang);
	KeyName = _T("AOI_CUSTOMER_ID_FOXCONN_LONGHUA"); Default = m_AOICustomerIDText_Foxconn_LongHua;
	GetUILanguageString(Section, KeyName, Default, m_AOICustomerIDText_Foxconn_LongHua);	
	//OFFLINE_VERSION_MODE
	Section = _T("OFFLINE_VERSION_MODE");
	KeyName = _T("OFFLINE_VERSION_NORMAL"); Default = m_OfflineVersionText_Normal;
	GetUILanguageString(Section, KeyName, Default, m_OfflineVersionText_Normal);
	KeyName = _T("OFFLINE_VERSION_HOST_TUNING"); Default = m_OfflineVersionText_HostTuning;
	GetUILanguageString(Section, KeyName, Default, m_OfflineVersionText_HostTuning);
	KeyName = _T("OFFLINE_VERSION_REMOTE_TUNING"); Default = m_OfflineVersionText_RemoteTuning;
	GetUILanguageString(Section, KeyName, Default, m_OfflineVersionText_RemoteTuning);	
	//ONLINE_OPEN_PROJECT_MODE
	Section = _T("ONLINE_OPEN_PROJECT_MODE");
	KeyName = _T("ONLINE_OPEN_PROJECT_DISABLE"); Default = m_OnlineOpenProjectText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_OnlineOpenProjectText_Disable);
	KeyName = _T("ONLINE_OPEN_PROJECT_BARCODE_DEVICE"); Default = m_OnlineOpenProjectText_BarcodeDevice;
	GetUILanguageString(Section, KeyName, Default, m_OnlineOpenProjectText_BarcodeDevice);
	KeyName = _T("ONLINE_OPEN_PROJECT_BARCODE_HANDHELD"); Default = m_OnlineOpenProjectText_BarcodeHandHeld;
	GetUILanguageString(Section, KeyName, Default, m_OnlineOpenProjectText_BarcodeHandHeld);	
	KeyName = _T("ONLINE_OPEN_PROJECT_BARCODE_CAMERA"); Default = m_OnlineOpenProjectText_BarcodeCamera;
	GetUILanguageString(Section, KeyName, Default, m_OnlineOpenProjectText_BarcodeCamera);
	//PROJECT_LINK_SERVER_MODE
	Section = _T("PROJECT_LINK_SERVER_MODE");
	KeyName = _T("PROJECT_LINK_SERVER_DISABLE"); Default = m_ProjectLinkServerText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_ProjectLinkServerText_Disable);
	KeyName = _T("PROJECT_LINK_SERVER_ENABLE_ALL"); Default = m_ProjectLinkServerText_EnableAll;
	GetUILanguageString(Section, KeyName, Default, m_ProjectLinkServerText_EnableAll);
	KeyName = _T("PROJECT_LINK_SERVER_PROJECT_ONLY"); Default = m_ProjectLinkServerText_ProjectOnly;
	GetUILanguageString(Section, KeyName, Default, m_ProjectLinkServerText_ProjectOnly);	
	KeyName = _T("PROJECT_LINK_SERVER_ENABLE_ALL_ASK"); Default = m_ProjectLinkServerText_EnableAllAsk;
	GetUILanguageString(Section, KeyName, Default, m_ProjectLinkServerText_EnableAllAsk);
	KeyName = _T("PROJECT_LINK_SERVER_PROJECT_ONLY_ASK"); Default = m_ProjectLinkServerText_ProjectOnlyAsk;
	GetUILanguageString(Section, KeyName, Default, m_ProjectLinkServerText_ProjectOnlyAsk);
	//XBOARD_MAPPING_FILE_MODE
	Section = _T("XBOARD_MAPPING_FILE_MODE");
	KeyName = _T("XBOARD_MAPPING_FILE_DISABLE"); Default = m_XBoardMappingFileText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_XBoardMappingFileText_Disable);
	KeyName = _T("XBOARD_MAPPING_FILE_MES_COMM"); Default = m_XBoardMappingFileText_MES_Comm;
	GetUILanguageString(Section, KeyName, Default, m_XBoardMappingFileText_MES_Comm);
	//XBOARD_MAPPING_FILE_FLOW
	Section = _T("XBOARD_MAPPING_FILE_FLOW");
	KeyName = _T("XBOARD_MAPPING_FILE_FLOW_DEFAULT"); Default = m_XBoardMappingFileFlowText_Default;
	GetUILanguageString(Section, KeyName, Default, m_XBoardMappingFileFlowText_Default);
	KeyName = _T("XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE"); Default = m_XBoardMappingFileFlowText_After_Barcode;
	GetUILanguageString(Section, KeyName, Default, m_XBoardMappingFileFlowText_After_Barcode);
	//Lane
	Section = _T("LANE ID");
	KeyName = _T("LANE_ID_A"); Default = m_LaneText_A;
	GetUILanguageString(Section, KeyName, Default, m_LaneText_A);
	KeyName = _T("LANE_ID_B"); Default = m_LaneText_B;
	GetUILanguageString(Section, KeyName, Default, m_LaneText_B);
	//Multi Lane
	Section = _T("MULTI_LANE_MODE");
	KeyName = _T("MULTI_LANE_1"); Default = m_MultiLaneText_1;
	GetUILanguageString(Section, KeyName, Default, m_MultiLaneText_1);
	KeyName = _T("MULTI_LANE_2"); Default = m_MultiLaneText_2;
	GetUILanguageString(Section, KeyName, Default, m_MultiLaneText_2);
	//FUNC_EXEC_MODE
	Section = _T("FUNC_EXEC_MODE");
	KeyName = _T("FUNC_EXEC_OFF"); Default = m_FuncExecText_Off;
	GetUILanguageString(Section, KeyName, Default, m_FuncExecText_Off);
	KeyName = _T("FUNC_EXEC_AUTO"); Default = m_FuncExecText_Auto;
	GetUILanguageString(Section, KeyName, Default, m_FuncExecText_Auto);
	KeyName = _T("FUNC_EXEC_ASK"); Default = m_FuncExecText_Ask;
	GetUILanguageString(Section, KeyName, Default, m_FuncExecText_Ask);
	//Lane Work Mode
	Section = _T("LANE_WORK_MODE");
	KeyName = _T("LANE_WORK_DISABLE"); Default = m_LaneWorkText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_LaneWorkText_Disable);
	KeyName = _T("LANE_WORK_RUN"); Default = m_LaneWorkText_Run;
	GetUILanguageString(Section, KeyName, Default, m_LaneWorkText_Run);
	KeyName = _T("LANE_WORK_BYPASS"); Default = m_LaneWorkText_Bypass;
	GetUILanguageString(Section, KeyName, Default, m_LaneWorkText_Bypass);
	//CONNECTED_BUFFER_TYPE
	Section = _T("CONNECTED_BUFFER_TYPE");
	KeyName = _T("CONNECTED_BUFFER_FIXED"); Default = m_ConnectedBufferType_Fixed;
	GetUILanguageString(Section, KeyName, Default, m_ConnectedBufferType_Fixed);
	KeyName = _T("CONNECTED_BUFFER_MOVABLE"); Default = m_ConnectedBufferType_Movable;
	GetUILanguageString(Section, KeyName, Default, m_ConnectedBufferType_Movable);	
	//M2M_OPTIONS
	Section = _T("HASI_AOI_STAGE");
	KeyName = _T("HASI_AOI_STAGE_PRE"); Default = m_HASI_AOIStage_Pre;
	GetUILanguageString(Section, KeyName, Default, m_HASI_AOIStage_Pre);
	KeyName = _T("HASI_AOI_STAGE_POST"); Default = m_HASI_AOIStage_Post;
	GetUILanguageString(Section, KeyName, Default, m_HASI_AOIStage_Post);
	//PCB Out
	Section = _T("PCB_OUT_MODE");
	KeyName = _T("PCB_OUT_NORMAL"); Default = m_PCBOutText_Normal;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutText_Normal);
	KeyName = _T("PCB_OUT_SIDE_OUT"); Default = m_PCBOutText_SideOut;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutText_SideOut);
	KeyName = _T("PCB_OUT_WITH_IN"); Default = m_PCBOutText_WithIn;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutText_WithIn);
	KeyName = _T("PCB_OUT_LANE_AUTO"); Default = m_PCBOutText_LaneAuto;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutText_LaneAuto);
	KeyName = _T("PCB_OUT_OK_OUT_NG_SIDE"); Default = m_PCBOutText_OkOutNgSide;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutText_OkOutNgSide);
	//PCB Out Direction
	Section = _T("PCB_OUT_DIRECTION");
	KeyName = _T("PCB_OUT_DIR_FORWARD"); Default = m_PCBOutDirText_Forward;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutDirText_Forward);
	KeyName = _T("PCB_OUT_DIR_BACKWARD"); Default = m_PCBOutDirText_Backward;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutDirText_Backward);
	KeyName = _T("PCB_OUT_DIR_BACKWARD_OUT"); Default = m_PCBOutDirText_BackwardOut;
	GetUILanguageString(Section, KeyName, Default, m_PCBOutDirText_BackwardOut);	
	//PCB Side MODE
	Section = _T("PANEL_SIDE_MODE");
	KeyName = _T("PANEL_SIDE_TOP"); Default = m_PanelSideText_Top;
	GetUILanguageString(Section, KeyName, Default, m_PanelSideText_Top);
	KeyName = _T("PANEL_SIDE_BOTTOM"); Default = m_PanelSideText_Bottom;
	GetUILanguageString(Section, KeyName, Default, m_PanelSideText_Bottom);
	KeyName = _T("PANEL_SIDE_HYBRID"); Default = m_PanelSideText_Hybrid;
	GetUILanguageString(Section, KeyName, Default, m_PanelSideText_Hybrid);	
	//Board Side Mode
	Section = _T("BOARD_SIDE_MODE");
	KeyName = _T("BOARD_SIDE_TOP"); Default = m_BoardSideText_Top;
	GetUILanguageString(Section, KeyName, Default, m_BoardSideText_Top);
	KeyName = _T("BOARD_SIDE_BOT"); Default = m_BoardSideText_Bottom;
	GetUILanguageString(Section, KeyName, Default, m_BoardSideText_Bottom);	
	//BARCODE_DECODER_TYPE
	Section = _T("BARCODE_DECODER_TYPE");
	KeyName = _T("BARCODE_DECODER_EVS"); Default = m_BarcodeDecoder_EVS;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDecoder_EVS);
	KeyName = _T("BARCODE_DECODER_DTK"); Default = m_BarcodeDecoder_DTK;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDecoder_DTK);
	KeyName = _T("BARCODE_DECODER_HON"); Default = m_BarcodeDecoder_HON;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDecoder_HON);	
	//BARCODE_SPREAD_MODE
	Section = _T("BARCODE_SPREAD_MODE");
	KeyName = _T("BARCODE_SPREAD_OFF"); Default = m_BarcodeSpread_Off;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeSpread_Off);
	KeyName = _T("BARCODE_SPREAD_LOCAL"); Default = m_BarcodeSpread_Local;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeSpread_Local);
	KeyName = _T("BARCODE_SPREAD_ALL"); Default = m_BarcodeSpread_All;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeSpread_All);
	//BARCODE_BELONG_MODE
	Section = _T("BARCODE_BELONG_MODE");
	KeyName = _T("BARCODE_BELONG_NONE"); Default = m_BarcodeBelong_None;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_None);
	KeyName = _T("BARCODE_BELONG_PROJECT"); Default = m_BarcodeBelong_Project;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_Project);	
	KeyName = _T("BARCODE_BELONG_PANEL"); Default = m_BarcodeBelong_Panel;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_Panel);
	KeyName = _T("BARCODE_BELONG_BOARD"); Default = m_BarcodeBelong_Board;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_Board);	
	KeyName = _T("BARCODE_BELONG_TRAY"); Default = m_BarcodeBelong_Tray;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_Tray);		
	KeyName = _T("BARCODE_BELONG_COVER"); Default = m_BarcodeBelong_Cover;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeBelong_Cover); 
	//Save Test Map Mode
	Section = _T("SAVE_TEST_MAP_MODE");	
	KeyName = _T("SAVE_TEST_MAP_DISABLE"); Default = m_SaveTestMapText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_Disable);
	KeyName = _T("SAVE_TEST_MAP_ENB_PROG"); Default = m_SaveTestMapText_Prog;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_Prog);
	KeyName = _T("SAVE_TEST_MAP_ENB_PANEL"); Default = m_SaveTestMapText_Panel;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_Panel);
	KeyName = _T("SAVE_TEST_MAP_ENB_PROG_PANEL"); Default = m_SaveTestMapText_ProgPanel;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_ProgPanel);	
	KeyName = _T("SAVE_TEST_MAP_ENB_BOARD"); Default = m_SaveTestMapText_Board;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_Board);	
	KeyName = _T("SAVE_TEST_MAP_ENB_PROG_BOARD"); Default = m_SaveTestMapText_ProgBoard;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestMapText_ProgBoard);	
	//Offline Image Scope
	Section = _T("OFFLINE_IMAGE_SCOPE");	
	KeyName = _T("OFFLINE_IMAGE_FOV"); Default = m_OfflineImageText_Fov;
	GetUILanguageString(Section, KeyName, Default, m_OfflineImageText_Fov);	
	KeyName = _T("OFFLINE_IMAGE_PART"); Default = m_OfflineImageText_Part;
	GetUILanguageString(Section, KeyName, Default, m_OfflineImageText_Part);		
	//Save Test Image Mode
	Section = _T("SAVE_TEST_IMAGE_MODE");	
	KeyName = _T("SAVE_TEST_IMAGE_DISABLE"); Default = m_SaveTestImageText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestImageText_Disable);	
	KeyName = _T("SAVE_TEST_IMAGE_DEFECT"); Default = m_SaveTestImageText_Defect;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestImageText_Defect);	
	KeyName = _T("SAVE_TEST_IMAGE_EVERYONE"); Default = m_SaveTestImageText_EveryOne;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestImageText_EveryOne);	
	//Save Test Data Mode
	Section = _T("SAVE_TEST_DATA_MODE");	
	KeyName = _T("SAVE_TEST_DATA_DISABLE"); Default = m_SaveTestDataText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestDataText_Disable);	
	KeyName = _T("SAVE_TEST_DATA_ENABLE"); Default = m_SaveTestDataText_Enable;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestDataText_Enable);	
	KeyName = _T("SAVE_TEST_DATA_DEFECT"); Default = m_SaveTestDataText_Defect;
	GetUILanguageString(Section, KeyName, Default, m_SaveTestDataText_Defect);
	//Fd NG Handle Mode	 
	Section = _T("FD_NG_HANDLE_MODE");
	KeyName = _T("FD_NG_HANDLE_NONE"); Default = m_FdNGHandleText_None;
	GetUILanguageString(Section, KeyName, Default, m_FdNGHandleText_None);
	KeyName = _T("FD_NG_HANDLE_PASS"); Default = m_FdNGHandleText_Pass;
	GetUILanguageString(Section, KeyName, Default, m_FdNGHandleText_Pass);
	KeyName = _T("FD_NG_HANDLE_STOP"); Default = m_FdNGHandleText_Stop;
	GetUILanguageString(Section, KeyName, Default, m_FdNGHandleText_Stop);	
	KeyName = _T("FD_NG_HANDLE_XBOARD"); Default = m_FdNGHandleText_XBoard;
	GetUILanguageString(Section, KeyName, Default, m_FdNGHandleText_XBoard);	
	//Board Fd Grab Mode
	Section = _T("BOARD_FD_GRAB_MODE");
	KeyName = _T("BOARD_FD_GRAB_AFTER_PANEL"); Default = m_BoardFdGrabText_AfterPanel;
	GetUILanguageString(Section, KeyName, Default, m_BoardFdGrabText_AfterPanel);
	KeyName = _T("BOARD_FD_GRAB_INSPECTING"); Default = m_BoardFdGrabText_Inspecting;
	GetUILanguageString(Section, KeyName, Default, m_BoardFdGrabText_Inspecting);	
	//Defect Handle Mode	 
	Section = _T("DEFECT_HANDLE_MODE");
	KeyName = _T("DEFECT_HANDLE_PASS"); Default = m_DefectHandleText_Pass;
	GetUILanguageString(Section, KeyName, Default, m_DefectHandleText_Pass);
	KeyName = _T("DEFECT_HANDLE_STOP_ALARM"); Default = m_DefectHandleText_Stop;
	GetUILanguageString(Section, KeyName, Default, m_DefectHandleText_Stop);	
	KeyName = _T("DEFECT_HANDLE_NEXT_STOP"); Default = m_DefectHandleText_Next;
	GetUILanguageString(Section, KeyName, Default, m_DefectHandleText_Next);
	KeyName = _T("DEFECT_HANDLE_WAIT_FOR_REPAIR"); Default = m_DefectHandleText_Repair;
	GetUILanguageString(Section, KeyName, Default, m_DefectHandleText_Repair);	
	KeyName = _T("DEFECT_HANDLE_CONTROL_CENTER"); Default = m_DefectHandleText_ControlCenter;
	GetUILanguageString(Section, KeyName, Default, m_DefectHandleText_ControlCenter);	
	//Online State Mode
	Section = _T("ONLINE_STATE_MODE");
	KeyName = _T("ONLINE_STATE_INSPECTION_STOP"); Default = m_OnlineState_InspectionStop;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectionStop);	
	KeyName = _T("ONLINE_STATE_PCB_READY"); Default = m_OnlineState_PCBReady;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBReady);
	KeyName = _T("ONLINE_STATE_INPUT_BARCODE"); Default = m_OnlineState_InputBarcode;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InputBarcode);
	KeyName = _T("ONLINE_STATE_PROJECT_MAP"); Default = m_OnlineState_ProjectMap;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectMap);
	KeyName = _T("ONLINE_STATE_PROJECT_MARK"); Default = m_OnlineState_ProjectMark;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectMark);
	KeyName = _T("ONLINE_STATE_PROJECT_OPEN_CODE"); Default = m_OnlineState_ProjectOpenCode;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectOpenCode);
	KeyName = _T("ONLINE_STATE_PROJECT_RELOAD"); Default = m_OnlineState_ProjectReload;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectReload);
	KeyName = _T("ONLINE_STATE_PROJECT_RELOAD_SERVER"); Default = m_OnlineState_ProjectReloadServer;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectReloadServer);	
	KeyName = _T("ONLINE_STATE_PROJECT_SWITCH_BY_TURN"); Default = m_OnlineState_ProjectSwitchByTurn;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectSwitchByTurn);
	KeyName = _T("ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET"); Default = m_OnlineState_ProjectSwitchByTurnOneCycleReset;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_ProjectSwitchByTurnOneCycleReset);	
	KeyName = _T("ONLINE_STATE_INSPECTION_START"); Default = m_OnlineState_InspectionStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectionStart);
	KeyName = _T("ONLINE_STATE_INSPECTION_WAITING"); Default = m_OnlineState_InspectionWaittng;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectionWaittng);
	KeyName = _T("ONLINE_STATE_INSPECT_FD_PANEL"); Default = m_OnlineState_InspectFDPanel;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectFDPanel);
	KeyName = _T("ONLINE_STATE_INSPECT_FD_BOARD"); Default = m_OnlineState_InspectFDBoard;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectFDBoard);
	KeyName = _T("ONLINE_STATE_INSPECT_BARCODE"); Default = m_OnlineState_InspectBarcode;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectBarcode);
	KeyName = _T("ONLINE_STATE_INSPECT_PROJECT"); Default = m_OnlineState_InspectProject;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectProject);
	KeyName = _T("ONLINE_STATE_STATICS_PROJECT"); Default = m_OnlineState_StatisticProject;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_StatisticProject);
	KeyName = _T("ONLINE_STATE_INSPECTION_FINISH"); Default = m_OnlineState_InspectionFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_InspectionFinish);
	KeyName = _T("ONLINE_STATE_WAIT_FOR_LAST_STATION"); Default = m_OnlineState_WaitForLast;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_WaitForLast);
	KeyName = _T("ONLINE_STATE_WAIT_FOR_NEXT_STATION"); Default = m_OnlineState_WaitForNext;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_WaitForNext);
	KeyName = _T("ONLINE_STATE_WAIT_FOR_PCB_REMOVED"); Default = m_OnlineState_WaitForPCBRemoved;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_WaitForPCBRemoved);
	KeyName = _T("ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY"); Default = m_OnlineState_WaitForRepairVerify;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_WaitForRepairVerify);
	KeyName = _T("ONLINE_STATE_PCB_IN_START"); Default = m_OnlineState_PCBInStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBInStart);
	KeyName = _T("ONLINE_STATE_PCB_IN_CHECKING"); Default = m_OnlineState_PCBInChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBInChecking);
	KeyName = _T("ONLINE_STATE_PCB_IN_FINISH"); Default = m_OnlineState_PCBInFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBInFinish);
	KeyName = _T("ONLINE_STATE_PCB_OUT_START"); Default = m_OnlineState_PCBOutStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutStart);
	KeyName = _T("ONLINE_STATE_PCB_OUT_CHECKING"); Default = m_OnlineState_PCBOutChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutChecking);
	KeyName = _T("ONLINE_STATE_PCB_OUT_FINISH"); Default = m_OnlineState_PCBOutFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutFinish);	
	KeyName = _T("ONLINE_STATE_PCB_OUT_INSIDE_START"); Default = m_OnlineState_PCBOutInsideStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInsideStart);
	KeyName = _T("ONLINE_STATE_PCB_OUT_INSIDE_CHECKING"); Default = m_OnlineState_PCBOutInsideChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInsideChecking);
	KeyName = _T("ONLINE_STATE_PCB_OUT_INSIDE_FINISH"); Default = m_OnlineState_PCBOutInsideFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInsideFinish);
	KeyName = _T("ONLINE_STATE_PCB_BACK_START"); Default = m_OnlineState_PCBBackStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackStart);
	KeyName = _T("ONLINE_STATE_PCB_BACK_CHECKING"); Default = m_OnlineState_PCBBackChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackChecking);
	KeyName = _T("ONLINE_STATE_PCB_BACK_FINISH"); Default = m_OnlineState_PCBBackFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackFinish);
	KeyName = _T("ONLINE_STATE_PCB_BACK_OUT_START"); Default = m_OnlineState_PCBBackOutStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackOutStart);
	KeyName = _T("ONLINE_STATE_PCB_BACK_OUT_CHECKING"); Default = m_OnlineState_PCBBackOutChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackOutChecking);
	KeyName = _T("ONLINE_STATE_PCB_BACK_OUT_FINISH"); Default = m_OnlineState_PCBBackOutFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBBackOutFinish);
	KeyName = _T("ONLINE_STATE_PCB_OUT_IN_START"); Default = m_OnlineState_PCBOutInStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInStart);
	KeyName = _T("ONLINE_STATE_PCB_OUT_IN_CHECKING"); Default = m_OnlineState_PCBOutInChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInChecking);
	KeyName = _T("ONLINE_STATE_PCB_OUT_IN_FINISH"); Default = m_OnlineState_PCBOutInFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBOutInFinish);
	KeyName = _T("ONLINE_STATE_PCB_AUTO_RUN_START"); Default = m_OnlineState_PCBAutoRunStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBAutoRunStart);
	KeyName = _T("ONLINE_STATE_PCB_AUTO_RUN_CHECKING"); Default = m_OnlineState_PCBAutoRunChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBAutoRunChecking);
	KeyName = _T("ONLINE_STATE_PCB_AUTO_RUN_FINISH"); Default = m_OnlineState_PCBAutoRunFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBAutoRunFinish);
	KeyName = _T("ONLINE_STATE_PCB_DUAL_RUN_START"); Default = m_OnlineState_PCBDualRunStart;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBDualRunStart);
	KeyName = _T("ONLINE_STATE_PCB_DUAL_RUN_CHECKING"); Default = m_OnlineState_PCBDualRunChecking;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBDualRunChecking);
	KeyName = _T("ONLINE_STATE_PCB_DUAL_RUN_FINISH"); Default = m_OnlineState_PCBDualRunFinish;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBDualRunFinish);			
	KeyName = _T("ONLINE_STATE_PCB_INSPECTION_PAUSE"); Default = m_OnlineState_PCBInspectionPause;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_PCBInspectionPause);
	KeyName = _T("ONLINE_STATE_AUTO_CALIBRATION_XYZ_HOME"); Default = m_OnlineState_AutoCalibration_XYZ_Home;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AutoCalibration_XYZ_Home);
	KeyName = _T("ONLINE_STATE_AUTO_CALIBRATION_2D_CURRENT"); Default = m_OnlineState_AutoCalibration_2D_Current;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AutoCalibration_2D_Current);
	KeyName = _T("ONLINE_STATE_AUTO_CALIBRATION_3D_CURRENT"); Default = m_OnlineState_AutoCalibration_3D_Current;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AutoCalibration_3D_Current);
	KeyName = _T("ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE"); Default = m_OnlineState_AutoCalibration_3D_ZeroPlane;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AutoCalibration_3D_ZeroPlane);
	KeyName = _T("ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR"); Default = m_OnlineState_AutoCalibration_3D_FactorFactor;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AutoCalibration_3D_FactorFactor);
	KeyName = _T("ONLINE_STATE_APP_OPEN"); Default = m_OnlineState_AppOpen;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AppOpen);
	KeyName = _T("ONLINE_STATE_APP_CLOSE"); Default = m_OnlineState_AppClose;
	GetUILanguageString(Section, KeyName, Default, m_OnlineState_AppClose);
	//MES EQP Ctrl State Mode
	Section = _T("MES_EQP_CTRL_STATE_MODE");
	KeyName = _T("MES_EQP_CTRL_STATE_NONE"); Default = m_MesEqpCtrlStateText_None;
	GetUILanguageString(Section, KeyName, Default, m_MesEqpCtrlStateText_None);
	KeyName = _T("MES_EQP_CTRL_STATE_OFFLINE"); Default = m_MesEqpCtrlStateText_Offline;
	GetUILanguageString(Section, KeyName, Default, m_MesEqpCtrlStateText_Offline);
	KeyName = _T("MES_EQP_CTRL_STATE_LOCAL"); Default = m_MesEqpCtrlStateText_Local;
	GetUILanguageString(Section, KeyName, Default, m_MesEqpCtrlStateText_Local);
	KeyName = _T("MES_EQP_CTRL_STATE_REMOTE"); Default = m_MesEqpCtrlStateText_Remote;
	GetUILanguageString(Section, KeyName, Default, m_MesEqpCtrlStateText_Remote);
	//Field Path Mode
	Section = _T("FIELD_PATH_MODE");
	KeyName = _T("FIELD_PATH_SPATH_HOR"); Default = m_FieldPathModeText_Hor;
	GetUILanguageString(Section, KeyName, Default, m_FieldPathModeText_Hor);
	KeyName = _T("FIELD_PATH_SPATH_VER"); Default = m_FieldPathModeText_Ver;
	GetUILanguageString(Section, KeyName, Default, m_FieldPathModeText_Ver);	 
	KeyName = _T("FIELD_PATH_SPATH_USER"); Default = m_FieldPathModeText_User;
	GetUILanguageString(Section, KeyName, Default, m_FieldPathModeText_User);
	//Field Division Mode
	Section = _T("FIELD_DIVISION_MODE");
	KeyName = _T("FIELD_DIVISION_MASS_AREA"); Default = m_FieldDivisionModeText_MassArea;
	GetUILanguageString(Section, KeyName, Default, m_FieldDivisionModeText_MassArea);
	KeyName = _T("FIELD_DIVISION_DIAGONAL_LINE"); Default = m_FieldDivisionModeText_Diagonal;
	GetUILanguageString(Section, KeyName, Default, m_FieldDivisionModeText_Diagonal);
	KeyName = _T("FIELD_DIVISION_HORIZONTAL_LINE"); Default = m_FieldDivisionModeText_Horizontal;
	GetUILanguageString(Section, KeyName, Default, m_FieldDivisionModeText_Horizontal);
	KeyName = _T("FIELD_DIVISION_VERTICAL_LINE"); Default = m_FieldDivisionModeText_Vertical;
	GetUILanguageString(Section, KeyName, Default, m_FieldDivisionModeText_Vertical);
	//Field Build Mode
	Section = _T("FIELD_BUILD_MODE");
	KeyName = _T("FIELD_BUILD_MATRIX"); Default = m_FieldBuildModeText_Matrix;
	GetUILanguageString(Section, KeyName, Default, m_FieldBuildModeText_Matrix);
	KeyName = _T("FIELD_BUILD_RANDOM_PANEL"); Default = m_FieldBuildModeText_Random_Panel;
	GetUILanguageString(Section, KeyName, Default, m_FieldBuildModeText_Random_Panel);
	KeyName = _T("FIELD_BUILD_RANDOM_BOARD"); Default = m_FieldBuildModeText_Random_Board;
	GetUILanguageString(Section, KeyName, Default, m_FieldBuildModeText_Random_Board);
	KeyName = _T("FIELD_BUILD_RANDOM_PROJECT"); Default = m_FieldBuildModeText_Random_Project;
	GetUILanguageString(Section, KeyName, Default, m_FieldBuildModeText_Random_Project);		
	//Barcode Read Type
	Section = _T("BARCODE_INPUT_TYPE");
	KeyName = _T("BARCODE_INPUT_DISABLED"); Default = m_BarcodeInputText_Disabled;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeInputText_Disabled);
	KeyName = _T("BARCODE_INPUT_DEVICE"); Default = m_BarcodeInputText_Device;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeInputText_Device);
	KeyName = _T("BARCODE_INPUT_HANDHELD"); Default = m_BarcodeInputText_Handheld;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeInputText_Handheld);	
	//Barcode NG Handle Mode
	Section = _T("BARCODE_NG_HANDLE_MODE");
	KeyName = _T("BARCODE_NG_HANDLE_PASS"); Default = m_BarcodeNGHandleText_Pass;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeNGHandleText_Pass);
	KeyName = _T("BARCODE_NG_HANDLE_ALARM"); Default = m_BarcodeNGHandleText_Alarm;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeNGHandleText_Alarm);
	KeyName = _T("BARCODE_NG_HANDLE_INPUT"); Default = m_BarcodeNGHandleText_Input;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeNGHandleText_Input);	
	//Multi Project Test Order Mode
	Section = _T("MULTI_PROJECT_TEST_ORDER_MODE");
	KeyName = _T("MULTI_PROJECT_TEST_ORDER_BY_MARK"); Default = m_MultiProjectTestOrderText_ByMark;
	GetUILanguageString(Section, KeyName, Default, m_MultiProjectTestOrderText_ByMark);
	KeyName = _T("MULTI_PROJECT_TEST_ORDER_BY_TURN"); Default = m_MultiProjectTestOrderText_InTurn;
	GetUILanguageString(Section, KeyName, Default, m_MultiProjectTestOrderText_InTurn);
	KeyName = _T("MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B"); Default = m_MultiProjectTestOrderText_OneCycleAB;
	GetUILanguageString(Section, KeyName, Default, m_MultiProjectTestOrderText_OneCycleAB);	
	KeyName = _T("MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A"); Default = m_MultiProjectTestOrderText_OneCycleBA;
	GetUILanguageString(Section, KeyName, Default, m_MultiProjectTestOrderText_OneCycleBA);
	//Barcode Camera Grab Mode
	Section = _T("BARCODE_CAMERA_GRAB_MODE");
	KeyName = _T("BARCODE_CAMERA_GRAB_AFTER_FD"); Default = m_BarcodeCameraGrabModeText_AfterFd;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeCameraGrabModeText_AfterFd);
	KeyName = _T("BARCODE_CAMERA_GRAB_INSPECTING"); Default = m_BarcodeCameraGrabModeText_Inspecting;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeCameraGrabModeText_Inspecting);
	//Barcode Device Grab Mode
	Section = _T("BARCODE_CAMERA_GRAB_MODE");
	KeyName = _T("BARCODE_DEVICE_GRAB_BEFORE_PCB_IN"); Default = m_BarcodeDeviceGrabModeText_BeforePCBIn;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDeviceGrabModeText_BeforePCBIn);
	KeyName = _T("BARCODE_DEVICE_GRAB_WHILE_PCB_IN"); Default = m_BarcodeDeviceGrabModeText_WhilePCBIn;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDeviceGrabModeText_WhilePCBIn);
	KeyName = _T("BARCODE_DEVICE_GRAB_AFTER_PCB_IN"); Default = m_BarcodeDeviceGrabModeText_AfterPCBIn;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDeviceGrabModeText_AfterPCBIn);
	KeyName = _T("BARCODE_DEVICE_GRAB_BEFORE_INSPECT"); Default = m_BarcodeDeviceGrabModeText_BeforeInspect;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDeviceGrabModeText_BeforeInspect);
	//Barcode Handheld Read Mode
	Section = _T("BARCODE_HANDHELD_READ_MODE");
	KeyName = _T("BARCODE_HANDHELD_READ_MANUAL"); Default = m_BarcodeHandHeldReadModeText_Manual;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeHandHeldReadModeText_Manual);
	KeyName = _T("BARCODE_HANDHELD_READ_PROJECT"); Default = m_BarcodeHandHeldReadModeText_Project;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeHandHeldReadModeText_Project);
	KeyName = _T("BARCODE_HANDHELD_READ_PANEL"); Default = m_BarcodeHandHeldReadModeText_Panel;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeHandHeldReadModeText_Panel);
	KeyName = _T("BARCODE_HANDHELD_READ_BOARD"); Default = m_BarcodeHandHeldReadModeText_Board;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeHandHeldReadModeText_Board);	
	//BARCODE_AUTO_EXPAND_MODE
	Section = _T("BARCODE_AUTO_EXPAND_MODE");
	KeyName = _T("BARCODE_AUTO_EXPAND_DISABLE"); Default = m_BarcodeAutoExpandModeText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_Disable);	
	KeyName = _T("BARCODE_AUTO_EXPAND_INCREMENT"); Default = m_BarcodeAutoExpandModeText_Increment;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_Increment);	
	KeyName = _T("BARCODE_AUTO_EXPAND_ADD_CHAR_1"); Default = m_BarcodeAutoExpandModeText_AddChar01;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_AddChar01);	
	KeyName = _T("BARCODE_AUTO_EXPAND_ADD_CHAR_2"); Default = m_BarcodeAutoExpandModeText_AddChar02;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_AddChar02);	
	KeyName = _T("BARCODE_AUTO_EXPAND_REPLACE_01"); Default = m_BarcodeAutoExpandModeText_Replace01;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_Replace01);	
	KeyName = _T("BARCODE_AUTO_EXPAND_REPLACE_02"); Default = m_BarcodeAutoExpandModeText_Replace02;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_Replace02);			
	KeyName = _T("BARCODE_AUTO_EXPAND_INCREMENT_BASE36"); Default = m_BarcodeAutoExpandModeText_Inc_Base36;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeAutoExpandModeText_Inc_Base36);
	//ALG_BARCODE_DIR_MODE
	Section = _T("ALG_BARCODE_DIR_MODE");
	KeyName = _T("ALG_BARCODE_DIR_AUTO"); Default = m_BarcodeDirectionModeText_Auto;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDirectionModeText_Auto);
	KeyName = _T("ALG_BARCODE_DIR_HOR"); Default = m_BarcodeDirectionModeText_Hor;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDirectionModeText_Hor);
	KeyName = _T("ALG_BARCODE_DIR_VER"); Default = m_BarcodeDirectionModeText_Ver;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDirectionModeText_Ver);
	KeyName = _T("ALG_BARCODE_DIR_ALL"); Default = m_BarcodeDirectionModeText_All;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeDirectionModeText_All);
	//AUTO_SWITCH_WND_3D_FRAME_MODE
	Section = _T("AUTO_SWITCH_WND_3D_FRAME_MODE");
	KeyName = _T("AUTO_SWITCH_WND_3D_FRAME_DISABLE"); Default = m_AutoSwitchWnd3DFrame_Disable;
	GetUILanguageString(Section, KeyName, Default, m_AutoSwitchWnd3DFrame_Disable);
	KeyName = _T("AUTO_SWITCH_WND_3D_FRAME_ENABLE"); Default = m_AutoSwitchWnd3DFrame_Enable;
	GetUILanguageString(Section, KeyName, Default, m_AutoSwitchWnd3DFrame_Enable);
	KeyName = _T("AUTO_SWITCH_WND_3D_FRAME_BY_SIZE"); Default = m_AutoSwitchWnd3DFrame_BySize;
	GetUILanguageString(Section, KeyName, Default, m_AutoSwitchWnd3DFrame_BySize);
	KeyName = _T("AUTO_SWITCH_WND_3D_FRAME_BY_TYPE"); Default = m_AutoSwitchWnd3DFrame_ByType;
	GetUILanguageString(Section, KeyName, Default, m_AutoSwitchWnd3DFrame_ByType);
	KeyName = _T("AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE"); Default = m_AutoSwitchWnd3DFrame_ByGroupChange;
	GetUILanguageString(Section, KeyName, Default, m_AutoSwitchWnd3DFrame_ByGroupChange);		
	//ALARM_LOCK_MODE
	Section = _T("ALARM_LOCK_MODE");	
	KeyName = _T("ALARM_LOCK_NONE"); Default = m_AlarmLockText_None;
	GetUILanguageString(Section, KeyName, Default, m_AlarmLockText_None);
	KeyName = _T("ALARM_LOCK_AOI"); Default = m_AlarmLockText_AOI;
	GetUILanguageString(Section, KeyName, Default, m_AlarmLockText_AOI);
	KeyName = _T("ALARM_LOCK_ARS"); Default = m_AlarmLockText_ARS;
	GetUILanguageString(Section, KeyName, Default, m_AlarmLockText_ARS);
	//Defect From Mode
	Section = _T("DEFECT_FROM_MODE");	
	KeyName = _T("DEFECT_FROM_NONE"); Default = m_DefectFromText_None;
	GetUILanguageString(Section, KeyName, Default, m_DefectFromText_None);
	KeyName = _T("DEFECT_FROM_AOI"); Default = m_DefectFromText_AOI;
	GetUILanguageString(Section, KeyName, Default, m_DefectFromText_AOI);
	KeyName = _T("DEFECT_FROM_ARS"); Default = m_DefectFromText_ARS;
	GetUILanguageString(Section, KeyName, Default, m_DefectFromText_ARS);
	//TOP10_SCOPE
	Section = _T("TOP10_SCOPE");
	KeyName = _T("TOP10_SCOPE_MODEL"); Default = m_Top10ScopeText_Model;
	GetUILanguageString(Section, KeyName, Default, m_Top10ScopeText_Model);
	KeyName = _T("TOP10_SCOPE_PART_NUMBER"); Default = m_Top10ScopeText_PartNumber;
	GetUILanguageString(Section, KeyName, Default, m_Top10ScopeText_PartNumber);
	KeyName = _T("TOP10_SCOPE_COMPONENT"); Default = m_Top10ScopeText_Component;
	GetUILanguageString(Section, KeyName, Default, m_Top10ScopeText_Component);	
	//YIELDING_SCOPE
	Section = _T("YIELDING_SCOPE");
	KeyName = _T("YIELDING_SCOPE_TEST"); Default = m_YieldingScopeText_Test;
	GetUILanguageString(Section, KeyName, Default, m_YieldingScopeText_Test);
	KeyName = _T("YIELDING_SCOPE_PANEL"); Default = m_YieldingScopeText_Panel;
	GetUILanguageString(Section, KeyName, Default, m_YieldingScopeText_Panel);
	KeyName = _T("YIELDING_SCOPE_BOARD"); Default = m_YieldingScopeText_Board;
	GetUILanguageString(Section, KeyName, Default, m_YieldingScopeText_Board);
	KeyName = _T("YIELDING_SCOPE_COMPONENT"); Default = m_YieldingScopeText_Component;
	GetUILanguageString(Section, KeyName, Default, m_YieldingScopeText_Component);	
	//DEFECT_PARAM_FROM_MODE
	Section = _T("DEFECT_PARAM_FROM_MODE");
	KeyName = _T("DEFECT_PARAM_FROM_DISABLE"); Default = m_DefectParamFromText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_DefectParamFromText_Disable);
	KeyName = _T("DEFECT_PARAM_FROM_PROJECT"); Default = m_DefectParamFromText_Project;
	GetUILanguageString(Section, KeyName, Default, m_DefectParamFromText_Project);
	KeyName = _T("DEFECT_PARAM_FROM_COMPONENT"); Default = m_DefectParamFromText_Component;
	GetUILanguageString(Section, KeyName, Default, m_DefectParamFromText_Component);	
	//CPK_FROM_MODE
	Section = _T("CPK_FROM_MODE");
	KeyName = _T("CPK_FROM_OFFSET_X"); Default = m_CpkFromText_OffsetX;
	GetUILanguageString(Section, KeyName, Default, m_CpkFromText_OffsetX);
	KeyName = _T("CPK_FROM_OFFSET_Y"); Default = m_CpkFromText_OffsetY;
	GetUILanguageString(Section, KeyName, Default, m_CpkFromText_OffsetY);
	KeyName = _T("CPK_FROM_SKEW_ANGLE"); Default = m_CpkFromText_SkewAngle;
	GetUILanguageString(Section, KeyName, Default, m_CpkFromText_SkewAngle);	
	//MULTI_LANGUAGE_MODE
	Section = _T("MULTI_LANGUAGE_MODE");
	KeyName = _T("MULTI_LANGUAGE_ENGLISH"); Default = m_MultiLanguageText_English;
	GetUILanguageString(Section, KeyName, Default, m_MultiLanguageText_English);
	KeyName = _T("MULTI_LANGUAGE_CHINESE_TRAD"); Default = m_MultiLanguageText_ChinTrad;
	GetUILanguageString(Section, KeyName, Default, m_MultiLanguageText_ChinTrad);
	KeyName = _T("MULTI_LANGUAGE_CHINESE_SIMP"); Default = m_MultiLanguageText_ChinSimp;
	GetUILanguageString(Section, KeyName, Default, m_MultiLanguageText_ChinSimp);	
	KeyName = _T("MULTI_LANGUAGE_LOCAL"); Default = m_MultiLanguageText_Local;
	GetUILanguageString(Section, KeyName, Default, m_MultiLanguageText_Local);		
	//ALG_TYPE
	Section = _T("ALG_TYPE");
	KeyName = _T("ALG_BRIGHT_RATIO"); Default = m_AlgText_BrightRatio;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_BrightRatio);
	KeyName = _T("ALG_OUTER_SHORT"); Default = m_AlgText_OuterShort;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_OuterShort);
	KeyName = _T("ALG_BLOB_COUNT"); Default = m_AlgText_BlobCount;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_BlobCount);
	KeyName = _T("ALG_BODY_TILT"); Default = m_AlgText_BodyTilt;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_BodyTilt);
	KeyName = _T("ALG_BARCODE_RECOGNIZE"); Default = m_AlgText_BarcodeRecognize;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_BarcodeRecognize);
	KeyName = _T("ALG_OBJECT_MEASURE"); Default = m_AlgText_ObjectMeasure;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_ObjectMeasure);
	KeyName = _T("ALG_WIDTH_RATIO"); Default = m_AlgText_WidthRatio;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_WidthRatio);
	KeyName = _T("ALG_HEIGHT"); Default = m_AlgText_Resin;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_Resin);
	KeyName = _T("ALG_WIRE_WIDTH"); Default = m_AlgText_WireWidth;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_WireWidth);
	KeyName = _T("ALG_COLOR_CODE"); Default = m_AlgText_ColorCode;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_ColorCode);
	KeyName = _T("ALG_MODEL_MATCH"); Default = m_AlgText_ModelMatch;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_ModelMatch);
	KeyName = _T("ALG_IMAGE_MATCH"); Default = m_AlgText_ImageMatch;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_ImageMatch);
	KeyName = _T("ALG_CHAR_VERIFY"); Default = m_AlgText_CharVerify;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_CharVerify);
	KeyName = _T("ALG_FD_MATCH"); Default = m_AlgText_FdMatch;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_FdMatch);	
	KeyName = _T("ALG_EDGE_SEARCH"); Default = m_AlgText_EdgeSearch;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_EdgeSearch);
	KeyName = _T("ALG_SHAPE_VERIFY"); Default = m_AlgText_ShapeVerify;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_ShapeVerify);
	KeyName = _T("ALG_ANGLE_MEASURE"); Default = m_AlgText_AngleMeasure;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_AngleMeasure);	
	KeyName = _T("ALG_PIXEL_COMPARE"); Default = m_AlgText_PixelCompare;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_PixelCompare);	
	KeyName = _T("ALG_SOLDER_WETTING"); Default = m_AlgText_SolderWetting;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_SolderWetting);
	KeyName = _T("ALG_MEASURE_BLACK_GLUE"); Default = m_AlgText_MeasureBlackGlue;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureBlackGlue);
	KeyName = _T("ALG_MEASURE_FLUX_AREA"); Default = m_AlgText_MeasureFluxArea;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureFluxArea);
	KeyName = _T("ALG_MEASURE_CPU_PIN"); Default = m_AlgText_MeasureCpuPin;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureCpuPin);	
	KeyName = _T("ALG_MEASURE_SIP_DISTANCE"); Default = m_AlgText_MeasureSIP;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureSIP);
	KeyName = _T("ALG_MEASURE_CONNECTOR"); Default = m_AlgText_MeasureConnector;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureConnector);
	KeyName = _T("ALG_MEASURE_CONNECTOR_PIN"); Default = m_AlgText_MeasureConnector;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_MeasureConnector);
	KeyName = _T("ALG_GROUP_COMPARE"); Default = m_AlgText_GroupCompare;
	GetUILanguageString(Section, KeyName, Default, m_AlgText_GroupCompare);		
	//ALG_CALC_UNIT_MODE
	Section = _T("ALG_CALC_UNIT_MODE");
	KeyName = _T("ALG_CALC_UNIT_ABS"); Default = m_AlgCalcUnitMode_Abs;
	GetUILanguageString(Section, KeyName, Default, m_AlgCalcUnitMode_Abs);	
	KeyName = _T("ALG_CALC_UNIT_DIFF"); Default = m_AlgCalcUnitMode_Diff;
	GetUILanguageString(Section, KeyName, Default, m_AlgCalcUnitMode_Diff);	
	KeyName = _T("ALG_CALC_UNIT_RATIO"); Default = m_AlgCalcUnitMode_Ratio;
	GetUILanguageString(Section, KeyName, Default, m_AlgCalcUnitMode_Ratio);	
	//ALG_MATCH_DOCK_MODE
	Section = _T("ALG_BRIGHT_AVERAGE_MODE");
	KeyName = _T("ALG_BRIGHT_AVERAGE_FULL"); Default = m_AlgBrightAverageMode_Full;
	GetUILanguageString(Section, KeyName, Default, m_AlgBrightAverageMode_Full);	
	KeyName = _T("ALG_BRIGHT_AVERAGE_PARTIAL"); Default = m_AlgBrightAverageMode_Partial;
	GetUILanguageString(Section, KeyName, Default, m_AlgBrightAverageMode_Partial);		
	//ALG_MATCH_DOCK_MODE
	Section = _T("ALG_MATCH_DOCK_MODE");
	KeyName = _T("ALG_MATCH_DOCK_DISABLE"); Default = m_AlgMatchDockMode_Disable;
	GetUILanguageString(Section, KeyName, Default, m_AlgMatchDockMode_Disable);	
	KeyName = _T("ALG_MATCH_DOCK_TO_TIP"); Default = m_AlgMatchDockMode_ToTip;
	GetUILanguageString(Section, KeyName, Default, m_AlgMatchDockMode_ToTip);	
	KeyName = _T("ALG_MATCH_DOCK_TO_SHOULDER"); Default = m_AlgMatchDockMode_Shoulder;
	GetUILanguageString(Section, KeyName, Default, m_AlgMatchDockMode_Shoulder);
	//ALG_OBJECT_SIZE_CALC_MODE
	Section = _T("ALG_OBJECT_SIZE_CALC_MODE");
	KeyName = _T("ALG_OBJECT_SIZE_CALC_BOUNDARY"); Default = m_AlgObjectSizeCalcMode_Boundary;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectSizeCalcMode_Boundary);	
	KeyName = _T("ALG_OBJECT_SIZE_CALC_AVERAGE"); Default = m_AlgObjectSizeCalcMode_Average;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectSizeCalcMode_Average);		
	KeyName = _T("ALG_OBJECT_SIZE_CALC_AVE_RECT"); Default = m_AlgObjectSizeCalcMode_AveRect;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectSizeCalcMode_AveRect);
	KeyName = _T("ALG_OBJECT_SIZE_CALC_BLUR_RECT"); Default = m_AlgObjectSizeCalcMode_BlurRect;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectSizeCalcMode_BlurRect);	
	//ALG_OBJECT_HEIGHT_AVERAGE_MODE
	Section = _T("ALG_OBJECT_HEIGHT_AVERAGE_MODE");
	KeyName = _T("ALG_OBJECT_HEIGHT_AVERAGE_FULL"); Default = m_AlgObjectHeightAverageMode_Full;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectHeightAverageMode_Full);	
	KeyName = _T("ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL"); Default = m_AlgObjectHeightAverageMode_Partial;
	GetUILanguageString(Section, KeyName, Default, m_AlgObjectHeightAverageMode_Partial);	
	//ANGLE_MEASURE_MODE
	Section = _T("ANGLE_MEASURE_MODE");
	KeyName = _T("ANGLE_MEASURE_SKEW"); Default = m_AlgAngleMeasureAngleMode_Skew;
	GetUILanguageString(Section, KeyName, Default, m_AlgAngleMeasureAngleMode_Skew);	
	KeyName = _T("ANGLE_MEASURE_TILT"); Default = m_AlgAngleMeasureAngleMode_Tilt;
	GetUILanguageString(Section, KeyName, Default, m_AlgAngleMeasureAngleMode_Tilt);		 	
	//LINE_EQUATION_MODE
	Section = _T("LINE_EQUATION_MODE");
	KeyName = _T("LINE_EQUATION_CALC"); Default = m_AlgAngleMeasureBaseLineMode_Calc;
	GetUILanguageString(Section, KeyName, Default, m_AlgAngleMeasureBaseLineMode_Calc);	
	KeyName = _T("LINE_EQUATION_HOR"); Default = m_AlgAngleMeasureBaseLineMode_Hor;
	GetUILanguageString(Section, KeyName, Default, m_AlgAngleMeasureBaseLineMode_Hor);	
	KeyName = _T("LINE_EQUATION_VER"); Default = m_AlgAngleMeasureBaseLineMode_Ver;
	GetUILanguageString(Section, KeyName, Default, m_AlgAngleMeasureBaseLineMode_Ver);		
	//RESULT_ID
	Section = _T("RESULT_ID");
	KeyName = _T("RESULT_ID_NONE"); Default = m_ResultText_None;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_None);
	KeyName = _T("RESULT_ID_OK"); Default = m_ResultText_OK;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_OK);
	KeyName = _T("RESULT_ID_NG"); Default = m_ResultText_NG;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_NG);	
	KeyName = _T("RESULT_ID_SKIP"); Default = m_ResultText_Skip;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_Skip);
	KeyName = _T("RESULT_ID_BYPASS"); Default = m_ResultText_Bypass;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_Bypass);	
	KeyName = _T("RESULT_ID_EXCEPTION"); Default = m_ResultText_Exception;
	GetUILanguageString(Section, KeyName, Default, m_ResultText_Exception);
	//PART_GROUP_MODE
	Section = _T("PART_GROUP_MODE");
	KeyName = _T("PART_GROUP_COLINEARITY"); Default = m_PartGroupText_Colinearity;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_Colinearity);
	KeyName = _T("PART_GROUP_COLINEARITY_TO_LINE"); Default = m_PartGroupText_ColinearityToLine;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_ColinearityToLine);
	KeyName = _T("PART_GROUP_DIST_PART_TO_PART"); Default = m_PartGroupText_DistPartToPart;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_DistPartToPart);
	KeyName = _T("PART_GROUP_DIST_PART_NEIGHBOR"); Default = m_PartGroupText_DistPartNeighbor;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_DistPartNeighbor);
	KeyName = _T("PART_GROUP_DIST_PART_TO_GROUP"); Default = m_PartGroupText_DistPartToGroup;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_DistPartToGroup);
	KeyName = _T("PART_GROUP_DIST_GROUP_TO_PART"); Default = m_PartGroupText_DistGroupToPart;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_DistGroupToPart);
	KeyName = _T("PART_GROUP_DIST_GROUP_COORD_MAP"); Default = m_PartGroupText_DistGroupCoordMap;
	GetUILanguageString(Section, KeyName, Default, m_PartGroupText_DistGroupCoordMap);
	//IMAGE_SRC_MODE
	Section = _T("IMAGE_SRC_MODE");
	KeyName = _T("IMAGE_SRC_GRAY"); Default = m_ImageSrcText_Gray;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Gray);
	KeyName = _T("IMAGE_SRC_COLOR"); Default = m_ImageSrcText_Color;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Color);
	KeyName = _T("IMAGE_SRC_RED"); Default = m_ImageSrcText_Red;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Red);
	KeyName = _T("IMAGE_SRC_GREEN"); Default = m_ImageSrcText_Green;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Green);
	KeyName = _T("IMAGE_SRC_BLUE"); Default = m_ImageSrcText_Blue;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Blue);
	KeyName = _T("IMAGE_SRC_LIGHTNESS"); Default = m_ImageSrcText_Lightness;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Lightness);
	KeyName = _T("IMAGE_SRC_SYNTHESIS"); Default = m_ImageSrcText_Synthesis;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Synthesis);
	KeyName = _T("IMAGE_SRC_DARKNESS"); Default = m_ImageSrcText_Darkness;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Darkness);	
	KeyName = _T("IMAGE_SRC_SATURATION"); Default = m_ImageSrcText_Saturation;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_Saturation);	
	KeyName = _T("IMAGE_SRC_RED_RATIO"); Default = m_ImageSrcText_RedRatio;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_RedRatio);
	KeyName = _T("IMAGE_SRC_GREEN_RATIO"); Default = m_ImageSrcText_GreenRatio;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_GreenRatio);
	KeyName = _T("IMAGE_SRC_BLUE_RATIO"); Default = m_ImageSrcText_BlueRatio;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_BlueRatio);	
	KeyName = _T("IMAGE_SRC_MAX_GRN_BLU"); Default = m_ImageSrcText_MaxGrnBlu;
	GetUILanguageString(Section, KeyName, Default, m_ImageSrcText_MaxGrnBlu);	
	//MASK_FUNC_MODE	
	Section = _T("MASK_FUNC_MODE");
	KeyName = _T("MASK_FUNC_CALC"); Default = m_MaskFuncText_Calc;
	GetUILanguageString(Section, KeyName, Default, m_MaskFuncText_Calc);
	KeyName = _T("MASK_FUNC_ERASE"); Default = m_MaskFuncText_Erase;
	GetUILanguageString(Section, KeyName, Default, m_MaskFuncText_Erase);	
	//BINARY_MODE
	Section = _T("BINARY_MODE");
	KeyName = _T("BINARY_DISABLE"); Default = m_BinaryText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_Disable);
	KeyName = _T("BINARY_COLOR_FILTER"); Default = m_BinaryText_ColorFilter;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_ColorFilter);
	KeyName = _T("BINARY_FIXED_THRESHOLD"); Default = m_BinaryText_FixedTh;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_FixedTh);
	KeyName = _T("BINARY_DYNAMIC_THRESHOLD"); Default = m_BinaryText_DynamicTh;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_DynamicTh);
	KeyName = _T("BINARY_RELATIVE_AVE_THRESHOLD"); Default = m_BinaryText_RelativeTh;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_RelativeTh);
	KeyName = _T("BINARY_ADAPTIVE_THRESHOLD"); Default = m_BinaryText_AdaptiveTh;
	GetUILanguageString(Section, KeyName, Default, m_BinaryText_AdaptiveTh);		
	//EDGE_ENHANCE_MODE
	Section = _T("EDGE_ENHANCE_MODE");
	KeyName = _T("EDGE_ENHANCE_DISABLE"); Default = m_EdgeEnhanceText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_Disable);
	KeyName = _T("EDGE_ENHANCE_SOBEL"); Default = m_EdgeEnhanceText_Sobel;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_Sobel);
	KeyName = _T("EDGE_ENHANCE_DARK_TOP"); Default = m_EdgeEnhanceText_DarkTop;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_DarkTop);
	KeyName = _T("EDGE_ENHANCE_DARK_LEFT"); Default = m_EdgeEnhanceText_DarkLeft;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_DarkLeft);
	KeyName = _T("EDGE_ENHANCE_DARK_BOT"); Default = m_EdgeEnhanceText_DarkBot;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_DarkBot);
	KeyName = _T("EDGE_ENHANCE_DARK_RIGHT"); Default = m_EdgeEnhanceText_DarkRight;
	GetUILanguageString(Section, KeyName, Default, m_EdgeEnhanceText_DarkRight);
	//NOISE_FILTER_MODE
	Section = _T("NOISE_FILTER_MODE");
	KeyName = _T("NOISE_FILTER_DISABLE"); Default = m_NoiseFilterText_Diable;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Diable);
	KeyName = _T("NOISE_FILTER_3LEVEL"); Default = m_NoiseFilterText_Level;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Level);
	KeyName = _T("NOISE_FILTER_SMOOTH"); Default = m_NoiseFilterText_Smooth;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Smooth);
	KeyName = _T("NOISE_FILTER_MEDIAN"); Default = m_NoiseFilterText_Median;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Median);
	KeyName = _T("NOISE_FILTER_MEDIAN2"); Default = m_NoiseFilterText_Median2;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Median2);
	KeyName = _T("NOISE_FILTER_PYRAMID_MEDIAN"); Default = m_NoiseFilterText_PyramidMedian;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_PyramidMedian);
	KeyName = _T("NOISE_FILTER_CONTENTAWARE"); Default = m_NoiseFilterText_ContentAware;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_ContentAware);
	KeyName = _T("NOISE_FILTER_FAST_MEDIAN"); Default = m_NoiseFilterText_Fast_Median;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Fast_Median);
	KeyName = _T("NOISE_FILTER_FAST_AVERAGE"); Default = m_NoiseFilterText_Fast_Average;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Fast_Average);
	KeyName = _T("NOISE_FILTER_OPEN"); Default = m_NoiseFilterText_Open;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Open);
	KeyName = _T("NOISE_FILTER_CLOSE"); Default = m_NoiseFilterText_Close;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Close);	
	KeyName = _T("NOISE_FILTER_EROSION"); Default = m_NoiseFilterText_Erosion;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Erosion);	
	KeyName = _T("NOISE_FILTER_DILATION"); Default = m_NoiseFilterText_Dilation;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Dilation);		
	KeyName = _T("NOISE_FILTER_GRADIENT"); Default = m_NoiseFilterText_Gradient;
	GetUILanguageString(Section, KeyName, Default, m_NoiseFilterText_Gradient);
	//ALG_BRIGHT_LINE_MODE
	Section = _T("ALG_BRIGHT_LINE_MODE");
	KeyName = _T("ALG_BRIGHT_LINE_BRIGHT"); Default = m_AlgBrightLineMode_Bright;
	GetUILanguageString(Section, KeyName, Default, m_AlgBrightLineMode_Bright);
	KeyName = _T("ALG_BRIGHT_LINE_DARK"); Default = m_AlgBrightLineMode_Dark;
	GetUILanguageString(Section, KeyName, Default, m_AlgBrightLineMode_Dark);
	//ALG_OUTER_SHORT_EXT_MODE
	Section = _T("ALG_OUTER_SHORT_EXT_MODE");
	KeyName = _T("ALG_OUTER_SHORT_EXT_NONE"); Default = m_AlgOuterShortExtendText_None;
	GetUILanguageString(Section, KeyName, Default, m_AlgOuterShortExtendText_None);
	KeyName = _T("ALG_OUTER_SHORT_EXT_LEFT"); Default = m_AlgOuterShortExtendText_Left;
	GetUILanguageString(Section, KeyName, Default, m_AlgOuterShortExtendText_Left);	
	KeyName = _T("ALG_OUTER_SHORT_EXT_RIGHT"); Default = m_AlgOuterShortExtendText_Right;
	GetUILanguageString(Section, KeyName, Default, m_AlgOuterShortExtendText_Right);
	KeyName = _T("ALG_OUTER_SHORT_EXT_BOTH"); Default = m_AlgOuterShortExtendText_Both;
	GetUILanguageString(Section, KeyName, Default, m_AlgOuterShortExtendText_Both);
	//ALG_DIRECTION
	Section = _T("ALG_DIRECTION");
	KeyName = _T("ALG_HORIZONTAL"); Default = m_AlgDirText_Hor;
	GetUILanguageString(Section, KeyName, Default, m_AlgDirText_Hor);
	KeyName = _T("ALG_VERTICAL"); Default = m_AlgDirText_Ver;
	GetUILanguageString(Section, KeyName, Default, m_AlgDirText_Ver);
	//ALG_BARCODE_STEP_MODE
	Section = _T("ALG_BARCODE_STEP_MODE");
	KeyName = _T("ALG_BARCODE_STEP_NONE"); Default = m_AlgBarcodeStepText_None;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_None);
	KeyName = _T("ALG_BARCODE_STEP_SCALE"); Default = m_AlgBarcodeStepText_Scale;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Scale);
	KeyName = _T("ALG_BARCODE_STEP_GAIN_OFFSET"); Default = m_AlgBarcodeStepText_GainOffset;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_GainOffset);
	KeyName = _T("ALG_BARCODE_STEP_SMOOTH"); Default = m_AlgBarcodeStepText_Smooth;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Smooth);
	KeyName = _T("ALG_BARCODE_STEP_OPEN"); Default = m_AlgBarcodeStepText_Open;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Open);
	KeyName = _T("ALG_BARCODE_STEP_CLOSE"); Default = m_AlgBarcodeStepText_Close;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Close);
	KeyName = _T("ALG_BARCODE_STEP_MEDIAN"); Default = m_AlgBarcodeStepText_Median;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Median);
	KeyName = _T("ALG_BARCODE_STEP_INVERT"); Default = m_AlgBarcodeStepText_Invert;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Invert);
	KeyName = _T("ALG_BARCODE_STEP_FLIP"); Default = m_AlgBarcodeStepText_Flip;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Flip);	
	KeyName = _T("ALG_BARCODE_STEP_FILL"); Default = m_AlgBarcodeStepText_Fill;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Fill);
	KeyName = _T("ALG_BARCODE_STEP_ERODE"); Default = m_AlgBarcodeStepText_Erode;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Erode);
	KeyName = _T("ALG_BARCODE_STEP_DILATE"); Default = m_AlgBarcodeStepText_Dilate;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Dilate);
	KeyName = _T("ALG_BARCODE_STEP_FILL_2D"); Default = m_AlgBarcodeStepText_Fill2D;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Fill2D);
	KeyName = _T("ALG_BARCODE_STEP_SHARP"); Default = m_AlgBarcodeStepText_Sharp;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Sharp);	
	KeyName = _T("ALG_BARCODE_STEP_GRAY_RANGE"); Default = m_AlgBarcodeStepText_Range;
	GetUILanguageString(Section, KeyName, Default, m_AlgBarcodeStepText_Range);		

	//FD_MATCH_MODE
	Section = _T("FD_MATCH_MODE");
	KeyName = _T("FD_MATCH_MODEL"); Default = m_AlgFdMatchText_Model;
	GetUILanguageString(Section, KeyName, Default, m_AlgFdMatchText_Model);
	KeyName = _T("FD_MATCH_IMAGE"); Default = m_AlgFdMatchText_Image;
	GetUILanguageString(Section, KeyName, Default, m_AlgFdMatchText_Image);

	//ALG_GROUP_CMP_DIR_MODE
	Section = _T("ALG_GROUP_CMP_DIR_MODE");
	KeyName = _T("ALG_GROUP_CMP_DIR_ANY"); Default = m_AlgGroupCompareDirText_Any;
	GetUILanguageString(Section, KeyName, Default, m_AlgGroupCompareDirText_Any);
	KeyName = _T("ALG_GROUP_CMP_DIR_ONE"); Default = m_AlgGroupCompareDirText_One;
	GetUILanguageString(Section, KeyName, Default, m_AlgGroupCompareDirText_One);	

	//ALG_3D_BASE_HEIGHT_MODE
	Section = _T("ALG_3D_BASE_HEIGHT_MODE");
	KeyName = _T("ALG_3D_BASE_HEIGHT_MIN"); Default = m_Alg3DHeightBaseText_Min;
	GetUILanguageString(Section, KeyName, Default, m_Alg3DHeightBaseText_Min);
	KeyName = _T("ALG_3D_BASE_HEIGHT_MAX"); Default = m_Alg3DHeightBaseText_Max;
	GetUILanguageString(Section, KeyName, Default, m_Alg3DHeightBaseText_Max);
	KeyName = _T("ALG_3D_BASE_HEIGHT_AVE"); Default = m_Alg3DHeightBaseText_Ave;
	GetUILanguageString(Section, KeyName, Default, m_Alg3DHeightBaseText_Ave);
	KeyName = _T("ALG_3D_BASE_HEIGHT_MID"); Default = m_Alg3DHeightBaseText_Mid;
	GetUILanguageString(Section, KeyName, Default, m_Alg3DHeightBaseText_Mid);
	KeyName = _T("ALG_3D_BASE_HEIGHT_SQR"); Default = m_Alg3DHeightBaseText_SQR;
	GetUILanguageString(Section, KeyName, Default, m_Alg3DHeightBaseText_SQR);
	//ALG_SEARCH_DIRECTION
	Section = _T("ALG_SEARCH_DIRECTION");
	KeyName = _T("SEARCH_DIRECTION_FORWARD"); Default = m_AlgSearchDirectionText_Forward;
	GetUILanguageString(Section, KeyName, Default, m_AlgSearchDirectionText_Forward);
	KeyName = _T("SEARCH_DIRECTION_BACKWARD"); Default = m_AlgSearchDirectionText_Backward;
	GetUILanguageString(Section, KeyName, Default, m_AlgSearchDirectionText_Backward);	
	//ALG_EDGE_FEATURE_MODE
	Section = _T("ALG_EDGE_FEATURE_MODE");
	KeyName = _T("ALG_EDGE_FEATURE_W2B"); Default = m_AlgEdgeFeatureText_W2B;
	GetUILanguageString(Section, KeyName, Default, m_AlgEdgeFeatureText_W2B);	
	KeyName = _T("ALG_EDGE_FEATURE_B2W"); Default = m_AlgEdgeFeatureText_B2W;
	GetUILanguageString(Section, KeyName, Default, m_AlgEdgeFeatureText_B2W);
	//ALG_RESIN_HEIGHT
	Section = _T("ALG_3D_BASE_HEIGHT_MODE");
	KeyName = _T("ALG_3D_BASE_HEIGHT_AVE"); Default = m_AlgHeightDetectionOutputType1;
	GetUILanguageString(Section, KeyName, Default, m_AlgHeightDetectionOutputType1);
	KeyName = _T("ALG_3D_BASE_HEIGHT_MAX"); Default = m_AlgHeightDetectionOutputType2;
	GetUILanguageString(Section, KeyName, Default, m_AlgHeightDetectionOutputType2);
	Section = _T("ALG_RESIN_HEIGHT");
	KeyName = _T("ALG_RESIN_TYPE1"); Default = m_AlgHeightDetectionType1;
	GetUILanguageString(Section, KeyName, Default, m_AlgHeightDetectionType1);
	KeyName = _T("ALG_RESIN_TYPE2"); Default = m_AlgHeightDetectionType2;
	GetUILanguageString(Section, KeyName, Default, m_AlgHeightDetectionType2);
	m_AlgHeightDetectionMeasureMode1 = m_RatioText;
	KeyName = _T("ABSOLUTE"); Default = m_AlgHeightDetectionMeasureMode2;
	GetUILanguageString(Section, KeyName, Default, m_AlgHeightDetectionMeasureMode2);
	//BOX_TOWARD
	Section = _T("BOX_TOWARD");
	KeyName = _T("BOX_TOWARD_UP"); Default = m_BoxTowardText_Up;
	GetUILanguageString(Section, KeyName, Default, m_BoxTowardText_Up);
	KeyName = _T("BOX_TOWARD_LEFT"); Default = m_BoxTowardText_Left;
	GetUILanguageString(Section, KeyName, Default, m_BoxTowardText_Left);
	KeyName = _T("BOX_TOWARD_DOWN"); Default = m_BoxTowardText_Down;
	GetUILanguageString(Section, KeyName, Default, m_BoxTowardText_Down);
	KeyName = _T("BOX_TOWARD_RIGHT"); Default = m_BoxTowardText_Right;
	GetUILanguageString(Section, KeyName, Default, m_BoxTowardText_Right);
	//BOX_SHAPE_MODE
	Section = _T("BOX_SHAPE_MODE");
	KeyName = _T("BOX_SHAPE_RECTANGLE"); Default = m_BoxShapeText_Rect;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_Rect);
	KeyName = _T("BOX_SHAPE_ROUND_RECT"); Default = m_BoxShapeText_RectRound;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_RectRound);
	KeyName = _T("BOX_SHAPE_ELLIPSE"); Default = m_BoxShapeText_Ellipse;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_Ellipse);
	KeyName = _T("BOX_SHAPE_CAPSULE"); Default = m_BoxShapeText_Capsule;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_Capsule);
	KeyName = _T("BOX_SHAPE_BULLET"); Default = m_BoxShapeText_Bullet;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_Bullet);	
	KeyName = _T("BOX_SHAPE_HALF_ROUND_RECT"); Default = m_BoxShapeText_RectHalfRound;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_RectHalfRound);		
	KeyName = _T("BOX_SHAPE_T_SHAPE"); Default = m_BoxShapeText_TShape;
	GetUILanguageString(Section, KeyName, Default, m_BoxShapeText_TShape);		
	//LAND_TYPE
	Section = _T("LAND_TYPE");
	KeyName = _T("LAND_TYPE_PAD"); Default = m_LandTypeText_Pad;
	GetUILanguageString(Section, KeyName, Default, m_LandTypeText_Pad);
	KeyName = _T("LAND_TYPE_ELECTRODE"); Default = m_LandTypeText_Electrode;
	GetUILanguageString(Section, KeyName, Default, m_LandTypeText_Electrode);
	KeyName = _T("LAND_TYPE_IC_LEAD"); Default = m_LandTypeText_ICLead;
	GetUILanguageString(Section, KeyName, Default, m_LandTypeText_ICLead);
	KeyName = _T("LAND_TYPE_CON_LEAD"); Default = m_LandTypeText_ConLead;
	GetUILanguageString(Section, KeyName, Default, m_LandTypeText_ConLead);	
	KeyName = _T("LAND_TYPE_DIP_LEAD"); Default = m_LandTypeText_DipLead;
	GetUILanguageString(Section, KeyName, Default, m_LandTypeText_DipLead);		
	//MODEL_PART
	Section = _T("MODEL_PART");
	KeyName = _T("MODEL_PART_PAD"); Default = m_ModelPadText;
	GetUILanguageString(Section, KeyName, Default, m_ModelPadText);
	KeyName = _T("MODEL_PART_BODY"); Default = m_ModelBodyText;
	GetUILanguageString(Section, KeyName, Default, m_ModelBodyText);
	KeyName = _T("MODEL_PART_LEAD"); Default = m_ModelLeadText;
	GetUILanguageString(Section, KeyName, Default, m_ModelLeadText);
	KeyName = _T("MODEL_PART_LEAD_TIP"); Default = m_ModelLeadTipText;
	GetUILanguageString(Section, KeyName, Default, m_ModelLeadTipText);
	KeyName = _T("MODEL_PART_LEAD_SHOULDER"); Default = m_ModelLeadShoulderText;
	GetUILanguageString(Section, KeyName, Default, m_ModelLeadShoulderText);
	//MODEL_MASK
	Section = _T("MODEL_MASK");
	KeyName = _T("MODEL_MASK_PAD"); Default = m_ModelMaskPadText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskPadText);
	KeyName = _T("MODEL_MASK_BODY"); Default = m_ModelMaskBodyText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskBodyText);
	KeyName = _T("MODEL_MASK_BODY_NO_LEAD"); Default = m_ModelMaskBodyNoLeadText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskBodyNoLeadText);
	KeyName = _T("MODEL_MASK_LEAD"); Default = m_ModelMaskLeadText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskLeadText);
	KeyName = _T("MODEL_MASK_LEAD_TIP"); Default = m_ModelMaskLeadTipText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskLeadTipText);
	KeyName = _T("MODEL_MASK_LEAD_SHOULDER"); Default = m_ModelMaskLeadShoulderText;
	GetUILanguageString(Section, KeyName, Default, m_ModelMaskLeadShoulderText);
	//MODEL_GROUP
	Section = _T("MODEL_GROUP");
	KeyName = _T("MODEL_GROUP_ALL"); Default = m_ModelGroupText_All;
	GetUILanguageString(Section, KeyName, Default, m_ModelGroupText_All);
	//MODEL_TYPE
	Section = _T("MODEL_TYPE");	
	KeyName = _T("MODEL_TYPE_NULL"); Default = m_ModelTypeText_Null;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Null);
	KeyName = _T("MODEL_TYPE_CHIP"); Default = m_ModelTypeText_Chip;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Chip);
	KeyName = _T("MODEL_TYPE_CHIP_C"); Default = m_ModelTypeText_ChipC;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_ChipC);
	KeyName = _T("MODEL_TYPE_CHIP_R"); Default = m_ModelTypeText_ChipR;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_ChipR);
	KeyName = _T("MODEL_TYPE_CHIP_L"); Default = m_ModelTypeText_ChipL;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_ChipL);
	KeyName = _T("MODEL_TYPE_CHIP_LED"); Default = m_ModelTypeText_ChipLed;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_ChipLed);
	KeyName = _T("MODEL_TYPE_MELF"); Default = m_ModelTypeText_Melf;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Melf);
	KeyName = _T("MODEL_TYPE_ELECTRODE"); Default = m_ModelTypeText_Electrode;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Electrode);
	KeyName = _T("MODEL_TYPE_TANTALUM_CONDENSER"); Default = m_ModelTypeText_Tant;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Tant);
	KeyName = _T("MODEL_TYPE_CAPACITY_ARRAY"); Default = m_ModelTypeText_CN;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_CN);
	KeyName = _T("MODEL_TYPE_RESISTOR_ARRAY"); Default = m_ModelTypeText_RN;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_RN);
	KeyName = _T("MODEL_TYPE_TRANSISTOR"); Default = m_ModelTypeText_SOT;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_SOT);
	KeyName = _T("MODEL_TYPE_ELECTROLYTIC_CAPACITOR"); Default = m_ModelTypeText_ElecCap;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_ElecCap);
	KeyName = _T("MODEL_TYPE_LED_ARRAY"); Default = m_ModelTypeText_LedArray;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_LedArray);
	KeyName = _T("MODEL_TYPE_NO_LEAD_COMPONENT"); Default = m_ModelTypeText_NoLead;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_NoLead);
	KeyName = _T("MODEL_TYPE_NO_LEAD_DFN"); Default = m_ModelTypeText_NoLeadDN;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_NoLeadDN);
	KeyName = _T("MODEL_TYPE_NO_LEAD_QFN"); Default = m_ModelTypeText_NoLeadQFN;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_NoLeadQFN);
	KeyName = _T("MODEL_TYPE_NO_LEAD_OSC"); Default = m_ModelTypeText_NoLeadOSC;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_NoLeadOSC);	
	KeyName = _T("MODEL_TYPE_LEAD_COMPONENT"); Default = m_ModelTypeText_LeadCom;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_LeadCom);
	KeyName = _T("MODEL_TYPE_LEAD_SOP"); Default = m_ModelTypeText_LeadComSOP;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_LeadComSOP);
	KeyName = _T("MODEL_TYPE_LEAD_QFP"); Default = m_ModelTypeText_LeadComQFP;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_LeadComQFP);
	KeyName = _T("MODEL_TYPE_LEAD_TRANSISTOR"); Default = m_ModelTypeText_LeadComSOT;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_LeadComSOT);	
	KeyName = _T("MODEL_TYPE_JLEAD_COMPONENT"); Default = m_ModelTypeText_JLeadCom;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_JLeadCom);
	KeyName = _T("MODEL_TYPE_JLEAD_SOJ"); Default = m_ModelTypeText_JLeadComSOJ;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_JLeadComSOJ);
	KeyName = _T("MODEL_TYPE_JLEAD_PLCC"); Default = m_ModelTypeText_JLeadComPLCC;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_JLeadComPLCC);
	KeyName = _T("MODEL_TYPE_COMPOSITE_COMPONENT"); Default = m_ModelTypeText_CompositeCom;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_CompositeCom);
	KeyName = _T("MODEL_TYPE_POWER_TRANSISTOR"); Default = m_ModelTypeText_PowerTransistor;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_PowerTransistor);
	KeyName = _T("MODEL_TYPE_CONNECTOR"); Default = m_ModelTypeText_Connector;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Connector);
	KeyName = _T("MODEL_TYPE_BGA"); Default = m_ModelTypeText_BGA;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_BGA);
	KeyName = _T("MODEL_TYPE_FD"); Default = m_ModelTypeText_Fd;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Fd);
	KeyName = _T("MODEL_TYPE_BARCODE"); Default = m_ModelTypeText_Barcode;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Barcode);
	KeyName = _T("MODEL_TYPE_PAD_COMPONENT"); Default = m_ModelTypeText_Pad;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Pad);
	KeyName = _T("MODEL_TYPE_GOLD_FINGER"); Default = m_ModelTypeText_GoldFinger;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_GoldFinger);
	KeyName = _T("MODEL_TYPE_DIP_LEAD"); Default = m_ModelTypeText_DipLead;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_DipLead);
	KeyName = _T("MODEL_TYPE_OTHERS"); Default = m_ModelTypeText_Ohters;
	GetUILanguageString(Section, KeyName, Default, m_ModelTypeText_Ohters);
	//WND_LOGIC_TYPE
	Section = _T("WND_LOGIC_TYPE");
	KeyName = _T("WND_LOGIC_NONE"); Default = m_WndLogText_None;
	GetUILanguageString(Section, KeyName, Default, m_WndLogText_None);
	KeyName = _T("WND_LOGIC_GROUP_ID"); Default = m_WndLogText_GroupID;
	GetUILanguageString(Section, KeyName, Default, m_WndLogText_GroupID);
	KeyName = _T("WND_LOGIC_DEFECT_ID"); Default = m_WndLogText_DefectID;
	GetUILanguageString(Section, KeyName, Default, m_WndLogText_DefectID);	
	//WND_FOLLOW_MODE
	Section = _T("WND_FOLLOW_MODE");
	KeyName = _T("WND_FOLLOW_NONE"); Default = m_WndFollowText_None;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_None);
	KeyName = _T("WND_FOLLOW_PAD"); Default = m_WndFollowText_Pad;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_Pad);
	KeyName = _T("WND_FOLLOW_PART"); Default = m_WndFollowText_Part;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_Part);
	KeyName = _T("WND_FOLLOW_PAD_BODY"); Default = m_WndFollowText_PadBody;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_PadBody);
	KeyName = _T("WND_FOLLOW_PAD_LEAD"); Default = m_WndFollowText_PadLead;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_PadLead);	
	KeyName = _T("WND_FOLLOW_PART_BODY"); Default = m_WndFollowText_PartBody;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_PartBody);
	KeyName = _T("WND_FOLLOW_PART_LEAD"); Default = m_WndFollowText_PartLead;
	GetUILanguageString(Section, KeyName, Default, m_WndFollowText_PartLead);	
	//WND_RGN_LINK_MODE
	Section = _T("WND_RGN_LINK_MODE");
	KeyName = _T("WND_RGN_LINK_NONE"); Default = m_WndRgnLinkText_None;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_None);
	KeyName = _T("WND_RGN_LINK_PAD"); Default = m_WndRgnLinkText_Pad;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_Pad);
	KeyName = _T("WND_RGN_LINK_BODY"); Default = m_WndRgnLinkText_Body;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_Body);
	KeyName = _T("WND_RGN_LINK_LEAD"); Default = m_WndRgnLinkText_Lead;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_Lead);
	KeyName = _T("WND_RGN_LINK_PAD_TIP"); Default = m_WndRgnLinkText_PadTip;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_PadTip);
	KeyName = _T("WND_RGN_LINK_PAD_RGN"); Default = m_WndRgnLinkText_PadRgn;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_PadRgn);
	KeyName = _T("WND_RGN_LINK_PAD_BODY_RGN"); Default = m_WndRgnLinkText_PadBodyRgn;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_PadBodyRgn);
	KeyName = _T("WND_RGN_LINK_PAD_RGN_INNER"); Default = m_WndRgnLinkText_PadRgnInner;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_PadRgnInner);
	KeyName = _T("WND_RGN_LINK_LEAD_TIP"); Default = m_WndRgnLinkText_LeadTip;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_LeadTip);
	KeyName = _T("WND_RGN_LINK_LEAD_SHOULDER"); Default = m_WndRgnLinkText_LeadShoulder;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_LeadShoulder);
	KeyName = _T("WND_RGN_LINK_LEAD_TIP_SHOULDER"); Default = m_WndRgnLinkText_LeadTipShoulder;
	GetUILanguageString(Section, KeyName, Default, m_WndRgnLinkText_LeadTipShoulder);

	//WND_SYNC_MOVE_MODE
	Section = _T("WND_SYNC_MOVE_MODE");
	KeyName = _T("WND_SYNC_MOVE_ROTATE"); Default = m_WndSyncMoveText_Rotate;
	GetUILanguageString(Section, KeyName, Default, m_WndSyncMoveText_Rotate);
	KeyName = _T("WND_SYNC_MOVE_MIRROR"); Default = m_WndSyncMoveText_Mirror;
	GetUILanguageString(Section, KeyName, Default, m_WndSyncMoveText_Mirror);	
	KeyName = _T("WND_SYNC_MOVE_SYMMETRY"); Default = m_WndSyncMoveText_Symmetry;
	GetUILanguageString(Section, KeyName, Default, m_WndSyncMoveText_Symmetry);	

	//WND_CONSTRAIN_MODE
	Section = _T("WND_CONSTRAIN_MODE");
	KeyName = _T("WND_CONSTRAIN_DISABLE"); Default = m_WndConstrainText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_Disable);
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_MOVE"); Default = m_WndConstrainText_PadRgnMove;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnMove);
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_X_MOVE"); Default = m_WndConstrainText_PadRgnXMove;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnXMove);	
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_Y_MOVE"); Default = m_WndConstrainText_PadRgnYMove;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnYMove);
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_SCALE"); Default = m_WndConstrainText_PadRgnScale;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnScale);
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_X_SCALE"); Default = m_WndConstrainText_PadRgnXScale;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnXScale);	
	KeyName = _T("WND_CONSTRAIN_PAD_RGN_Y_SCALE"); Default = m_WndConstrainText_PadRgnYScale;
	GetUILanguageString(Section, KeyName, Default, m_WndConstrainText_PadRgnYScale);	

	//WND_DEFECT_ID
	Section = _T("WND_DEFECT_ID");
	KeyName = _T("WND_DEFECT_NONE"); Default = m_WndDefectText_None;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_None);
	KeyName = _T("WND_DEFECT_PAD_ALIGN"); Default = m_WndDefectText_PadAlign;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_PadAlign);
	KeyName = _T("WND_DEFECT_PART_ALIGN"); Default = m_WndDefectText_PartAlign;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_PartAlign);
	KeyName = _T("WND_DEFECT_PAD_ADJUST"); Default = m_WndDefectText_PadAdjust;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_PadAdjust);
	KeyName = _T("WND_DEFECT_LEAD_ADJUST"); Default = m_WndDefectText_LeadAdjust;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_LeadAdjust);
	KeyName = _T("WND_DEFECT_CLASS_CHECK"); Default = m_WndDefectText_ClassCheck;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_ClassCheck);
	KeyName = _T("WND_DEFECT_BASE_VALUE"); Default = m_WndDefectText_BaseValue;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BaseValue);
	KeyName = _T("WND_DEFECT_BODY_MISSING"); Default = m_WndDefectText_BodyMissing;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyMissing);
	KeyName = _T("WND_DEFECT_BODY_OFFSET"); Default = m_WndDefectText_BodyOffset;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyOffset);
	KeyName = _T("WND_DEFECT_BODY_TILT"); Default = m_WndDefectText_BodyTilt;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyTilt);
	KeyName = _T("WND_DEFECT_BODY_POLARITY"); Default = m_WndDefectText_BodyPolarity;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyPolarity);	
	KeyName = _T("WND_DEFECT_BODY_TURNOVER"); Default = m_WndDefectText_BodyTurnOver;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyTurnOver);
	KeyName = _T("WND_DEFECT_BODY_MOUNT"); Default = m_WndDefectText_BodyMount;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyMount);
	KeyName = _T("WND_DEFECT_BODY_WRONG_CODE"); Default = m_WndDefectText_BodyWrongCode;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyWrongCode);
	KeyName = _T("WND_DEFECT_BODY_WRONG_TEXT"); Default = m_WndDefectText_BodyWrongText;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyWrongText);
	KeyName = _T("WND_DEFECT_BODY_TOMBSTONE"); Default = m_WndDefectText_BodyTombstone;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyTombstone);		
	KeyName = _T("WND_DEFECT_BODY_BILLBOARD"); Default = m_WndDefectText_BodyBillboard;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyBillboard);		
	KeyName = _T("WND_DEFECT_BODY_DAMAGED"); Default = m_WndDefectText_BodyDamaged;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_BodyDamaged);	
	KeyName = _T("WND_DEFECT_SOLDER_POOR"); Default = m_WndDefectText_SolderPoor;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderPoor);
	KeyName = _T("WND_DEFECT_SOLDER_OPEN"); Default = m_WndDefectText_SolderOpen;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderOpen);
	KeyName = _T("WND_DEFECT_SOLDER_PAD_EXPOSED"); Default = m_WndDefectText_SolderPadExposed;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderPadExposed);
	KeyName = _T("WND_DEFECT_SOLDER_BRIDGE"); Default = m_WndDefectText_SolderBridge;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderBridge);
	KeyName = _T("WND_DEFECT_SOLDER_BEAD"); Default = m_WndDefectText_SolderBead;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderBead);	
	KeyName = _T("WND_DEFECT_SOLDER_EXCESS"); Default = m_WndDefectText_SolderExcess;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_SolderExcess);		
	KeyName = _T("WND_DEFECT_LEAD_LIFTED"); Default = m_WndDefectText_LeadLifted;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_LeadLifted);
	KeyName = _T("WND_DEFECT_LEAD_BENDED"); Default = m_WndDefectText_LeadBended;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_LeadBended);
	KeyName = _T("WND_DEFECT_LEAD_PROTRUDED"); Default = m_WndDefectText_LeadProtruded;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_LeadProtruded);
	KeyName = _T("WND_DEFECT_PAD_SCRATCH"); Default = m_WndDefectText_PadScratch;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_PadScratch);
	KeyName = _T("WND_DEFECT_FOREIGN_BODY"); Default = m_WndDefectText_ForeignBody;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_ForeignBody);	

	KeyName = _T("WND_DEFECT_USER_DEFINE_01"); Default = m_WndDefectText_UserDefine_01;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_01);
	KeyName = _T("WND_DEFECT_USER_DEFINE_02"); Default = m_WndDefectText_UserDefine_02;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_02);
	KeyName = _T("WND_DEFECT_USER_DEFINE_03"); Default = m_WndDefectText_UserDefine_03;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_03);
	KeyName = _T("WND_DEFECT_USER_DEFINE_04"); Default = m_WndDefectText_UserDefine_04;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_04);
	KeyName = _T("WND_DEFECT_USER_DEFINE_05"); Default = m_WndDefectText_UserDefine_05;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_05);
	KeyName = _T("WND_DEFECT_USER_DEFINE_06"); Default = m_WndDefectText_UserDefine_06;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_06);
	KeyName = _T("WND_DEFECT_USER_DEFINE_07"); Default = m_WndDefectText_UserDefine_07;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_07);
	KeyName = _T("WND_DEFECT_USER_DEFINE_08"); Default = m_WndDefectText_UserDefine_08;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_08);
	KeyName = _T("WND_DEFECT_USER_DEFINE_09"); Default = m_WndDefectText_UserDefine_09;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_09);
	KeyName = _T("WND_DEFECT_USER_DEFINE_10"); Default = m_WndDefectText_UserDefine_10;
	GetUILanguageString(Section, KeyName, Default, m_WndDefectText_UserDefine_10);

	//BASE_PLANE_PROC_TYPE
	Section = _T("BASE_PLANE_PROC_TYPE");	
	KeyName = _T("BASE_PLANE_PROC_TYPE_1"); Default = m_BasePlaneProcText_1;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneProcText_1);
	KeyName = _T("BASE_PLANE_PROC_TYPE_2"); Default = m_BasePlaneProcText_2;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneProcText_2);

	//BASE_PLANE_AUTO_REGION_MODE
	Section = _T("BASE_PLANE_AUTO_REGION_MODE");	
	KeyName = _T("BASE_PLANE_AUTO_REGION_DISABLE"); Default = m_BasePlaneAutoRgnText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneAutoRgnText_Disable);
	KeyName = _T("BASE_PLANE_AUTO_REGION_GROUP"); Default = m_BasePlaneAutoRgnText_Group;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneAutoRgnText_Group);
	KeyName = _T("BASE_PLANE_AUTO_REGION_LOWEST"); Default = m_BasePlaneAutoRgnText_Lowest;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneAutoRgnText_Lowest);

	//BASE_PLANE_BODY_OUTSIDE_MODE
	Section = _T("BASE_PLANE_BODY_OUTSIDE_MODE");	
	KeyName = _T("BASE_PLANE_BODY_OUTSIDE_DISABLE"); Default = m_BasePlaneBodyOutsideText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneBodyOutsideText_Disable);
	KeyName = _T("BASE_PLANE_BODY_OUTSIDE_BODY"); Default = m_BasePlaneBodyOutsideText_Body;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneBodyOutsideText_Body);
	KeyName = _T("BASE_PLANE_BODY_OUTSIDE_BODY_LAND"); Default = m_BasePlaneBodyOutsideText_BodyLand;
	GetUILanguageString(Section, KeyName, Default, m_BasePlaneBodyOutsideText_BodyLand);

	//CALC_BASE_PLANE_MODE
	Section = _T("CALC_BASE_PLANE_MODE");		
	KeyName = _T("CALC_BASE_PLANE_DISABLE"); Default = m_CalcBasePlaneText_Disable;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Disable);
	KeyName = _T("CALC_BASE_PLANE_AVERAGE"); Default = m_CalcBasePlaneText_Ave;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Ave);
	KeyName = _T("CALC_BASE_PLANE_CORNER"); Default = m_CalcBasePlaneText_Corner;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Corner);		
	KeyName = _T("CALC_BASE_PLANE_ISO_DATA"); Default = m_CalcBasePlaneText_IsoData;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_IsoData);		
	KeyName = _T("CALC_BASE_PLANE_OTSU"); Default = m_CalcBasePlaneText_Ostu;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Ostu);		
	KeyName = _T("CALC_BASE_PLANE_CORNER_ONLY"); Default = m_CalcBasePlaneText_CornerOnly;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_CornerOnly);
	KeyName = _T("CALC_BASE_PLANE_SURROUND"); Default = m_CalcBasePlaneText_Surround;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Surround);	
	KeyName = _T("CALC_BASE_PLANE_AUTO_LOWER"); Default = m_CalcBasePlaneText_AutoLower;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_AutoLower);	
	KeyName = _T("CALC_BASE_PLANE_PANEL"); Default = m_CalcBasePlaneText_Panel;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Panel);
	KeyName = _T("CALC_BASE_PLANE_LOCAL"); Default = m_CalcBasePlaneText_Local;
	GetUILanguageString(Section, KeyName, Default, m_CalcBasePlaneText_Local);

	//AOI FD
	Section = _T("AOI_FD");
	KeyName = _T("AOI_FD_NG"); Default = m_FdText_NG;
	GetUILanguageString(Section, KeyName, Default, m_FdText_NG);
	
	//AOI BARCODE
	Section = _T("AOI_BARCODE");
	KeyName = _T("AOI_BARCODE_NG"); Default = m_BarcodeText_NG;
	GetUILanguageString(Section, KeyName, Default, m_BarcodeText_NG);		

	//色彩群組文字
	Section = _T("PROJECT_COLOR_GROUP");
	KeyName = _T("PROJECT_COLOR_GROUP_PAD"); Default = m_ProjectColorGroupText_Pad;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Pad);	
	KeyName = _T("PROJECT_COLOR_GROUP_VOID"); Default = m_ProjectColorGroupText_Void;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Void);	
	KeyName = _T("PROJECT_COLOR_GROUP_BODY"); Default = m_ProjectColorGroupText_Body;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Body);
	KeyName = _T("PROJECT_COLOR_GROUP_BOARD"); Default = m_ProjectColorGroupText_Board;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Board);	
	KeyName = _T("PROJECT_COLOR_GROUP_SOLDER"); Default = m_ProjectColorGroupText_Solder;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Solder);	
	KeyName = _T("PROJECT_COLOR_GROUP_OTHERS"); Default = m_ProjectColorGroupText_Ohters;
	GetUILanguageString(Section, KeyName, Default, m_ProjectColorGroupText_Ohters);	

	//軸控加速度調整模式
	Section = _T("ACC_TIME_ADJUST_MODE");
	KeyName = _T("ACC_TIME_ADJUST_OFF"); Default = m_MotionAccTimeAdjustText_Off;
	GetUILanguageString(Section, KeyName, Default, m_MotionAccTimeAdjustText_Off);
	KeyName = _T("ACC_TIME_ADJUST_FIX_T"); Default = m_MotionAccTimeAdjustText_Fix;
	GetUILanguageString(Section, KeyName, Default, m_MotionAccTimeAdjustText_Fix);
	KeyName = _T("ACC_TIME_ADJUST_MIN_T"); Default = m_MotionAccTimeAdjustText_Min;
	GetUILanguageString(Section, KeyName, Default, m_MotionAccTimeAdjustText_Min);
	KeyName = _T("ACC_TIME_ADJUST_GAMMA"); Default = m_MotionAccTimeAdjustText_Gamma;
	GetUILanguageString(Section, KeyName, Default, m_MotionAccTimeAdjustText_Gamma);
	//Section = _T("CCCCCCCCCCCCCCCccc");
	//KeyName = _T("BBBBBBBBBBBBBBBBBB"); Default = AAAAAAAAAAAAAAAAAAAAAAA;
	//GetUILanguageString(Section, KeyName, Default, AAAAAAAAAAAAAAAAAAAAAAA);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetLaneIDText(LANE_ID LaneID)//取得軌道編號名稱
{
	CString str;
	switch ( LaneID )
	{
	case LANE_ID_A:	str = m_LaneText_A; break;
	case LANE_ID_B:	str = m_LaneText_B; break;
	default:		str = m_UndefinedText; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildLaneIDCombox(CComboBox &Combox)//建立軌道編號列表
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	LANE_ID      LaneID;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	LaneID = LANE_ID_A;
	str = GetLaneIDText(LaneID);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LaneID);
	idx ++;
	
	LaneID = LANE_ID_B;
	str = GetLaneIDText(LaneID);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LaneID);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildLandTypeCombox(CComboBox &Combox)//建立特徵框樣式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	LAND_TYPE    LandType;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	//LandType = LAND_TYPE_NULL;
	//str = GetLandTypeText(LandType);
	//Combox.InsertString(-1, str);
	//Combox.SetItemData(idx, LandType);
	//idx ++;

	LandType = LAND_TYPE_PAD;
	str = GetLandTypeText(LandType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LandType);
	idx ++;

	LandType = LAND_TYPE_ELECTRODE;
	str = GetLandTypeText(LandType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LandType);
	idx ++;

	LandType = LAND_TYPE_IC_LEAD;
	str = GetLandTypeText(LandType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LandType);
	idx ++;

	LandType = LAND_TYPE_CON_LEAD;
	str = GetLandTypeText(LandType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LandType);
	idx ++;

	LandType = LAND_TYPE_DIP_LEAD;
	str = GetLandTypeText(LandType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, LandType);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetIDText() const//取得編號文字
{
	return m_IDText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetToText() const//取得去文字
{
	return m_ToText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetFdText() const//取得定位點文字
{
	return m_FdText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAddText() const//取得新增文字
{
	return m_AddText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetSetText() const//取得設定文字
{
	return m_SetText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetGetText() const//取得取得文字
{
	return m_GetText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetPadText() const//取得焊盤文字
{
	return m_PadText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWndText() const//取得檢測框文字	
{
	return m_WndText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAveText() const//取得平均文字	
{
	return m_AveText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAllText() const//取得全部文字	
{
	return m_AllText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetMaxText() const//取得最大值文字	
{
	return m_MaxText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetMinText() const//取得最小值文字
{
	return m_MinText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetFromText() const//來文字	
{
	return m_FromText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAxisText() const//軸文字	
{
	return m_AxisText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetLineText() const//線文字	
{
	return m_LineText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAreaText() const//面積文字	
{
	return m_AreaText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetLaneText() const//軌道文字
{
	return m_LaneText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetMarkText() const//特徵文字	
{
	return m_MarkText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetSkewText() const//偏角文字
{
	return m_SkewText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetNameText() const//名稱文字
{
	return m_NameText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetGainText() const//增益文字
{
	return m_GainText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CAOIDataDefine::GetGrayText() const//灰階文字
{
	return m_GrayText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetSizeText() const//尺寸文字
{
	return m_SizeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetLandText() const//特徵框文字
{
	return m_LandText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetTimeText() const//時間文字
{
	return m_TimeText;
}
//-------------------------------------------------------------------------------------//;
LPCTSTR CAOIDataDefine::GetTestText() const//檢測文字		
{
	return m_TestText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CAOIDataDefine::GetRangeText() const//範圍文字
{
	return m_RangeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetClassText() const//類別文字
{
	return m_ClassText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetClearText() const//清除文字
{
	return m_ClearText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CAOIDataDefine::GetRatioText() const//比例文字
{
	return m_RatioText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetIndexText() const//取得序號文字
{
	return m_IndexText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetGrabText() const//取得取像文字
{
	return m_GrabText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetCountText() const//取得數量文字
{
	return m_CountText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetErrorText() const//取得錯誤文字
{
	return m_ErrorText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetPanelText() const//整板文字
{
	return m_PanelText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBoardText() const//單板文字
{
	return m_BoardText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectText() const//專案文字	
{
	return m_ProjectText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetPatternText() const//樣板文字
{
	return m_PatternText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDefaultText() const//預設文字	
{
	return m_DefaultText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetThroughText() const//貫穿文字	
{
	return m_ThroughText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBarcodeText() const//條碼文字
{
	return m_BarcodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWarningText() const//警告文字	
{
	return m_WarningText;
}
//-------------------------------------------------------------------------------------//	
LPCTSTR CAOIDataDefine::GetModelText() const//模組文字
{
	return m_ModelText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetGroupText() const//群組文字
{
	return m_GroupText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetScoreText() const//分數文字
{
	return m_ScoreText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetScaleText() const//縮放文字	
{
	return m_ScaleText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetYieldText() const//良率文字	
{
	return m_YieldText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetPixelText() const//像素文字
{
	return m_PixelText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetSizeXText() const//X尺寸文字
{
	return m_SizeXText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetSizeYText() const//Y尺寸文字
{
	return m_SizeYText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetMethodText() const//方法文字
{
	return m_MethodText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetTowardText() const//朝向文字
{
	return m_TowardText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetCloneText() const//複製文字
{
	return m_CloneText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetCreateText() const//創建文字
{
	return m_CreateText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetModifyText() const//修改文字
{
	return m_ModifyText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDeleteText() const//刪除文字
{
	return m_DeleteText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDefectText() const//瑕疵文字
{
	return m_DefectText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetResultText() const//結果文字
{
	return m_ResultText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetUnsetText() const//未設定文字
{
	return m_UnsetText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAngleText() const//角度文字
{
	return m_AngleText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWidthText() const//寬度文字
{
	return m_WidthText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetHeightText() const//長度文字
{	
	return m_HeightText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetVolumeText() const//體積文字	
{
	return m_VolumeText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetOffsetText() const//偏移文字	
{
	return m_OffsetText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetFinishText() const//完成文字	
{
	return m_FinishText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetGroupIDText() const//群組編號文字
{
	return m_GroupIDText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetBypassedText() const//不檢測文字
{
	return m_BypassedText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetDistrictText() const//分段文字
{
	return m_DistrictText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetDistanceText() const//距離文字
{
	return m_DistanceText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetVelocityText() const//速度文字
{
	return m_VelocityText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetRelativeText() const//相對文字	
{
	return m_RelativeText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetContinueText() const//連續文字	
{
	return m_ContinueText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetContrastText() const//對比文字
{
	return m_ContrastText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetThicknessText() const//厚度文字
{
	return m_ThicknessText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetAlgorithmText() const//零件文字
{
	return m_AlgorithmText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetLockScreenText() const//鎖住螢幕文字	
{
	return m_LockScreenText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetVerticalText() const//垂直文字
{
	return m_VerticalText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetHorizontalText() const//水平文字
{
	return m_HorizontalText;
}
//-------------------------------------------------------------------------------------// 
LPCTSTR CAOIDataDefine::GetComponentText() const//零件文字		
{
	return m_ComponentText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetPartNumberText() const//料號文字
{
	return m_PartNumberText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetNozzleNameText() const//吸嘴文字	
{
	return m_NozzleNameText;
}
//-------------------------------------------------------------------------------------//	
LPCTSTR CAOIDataDefine::GetCalculateText() const//取得計算文字
{	
	return m_CalculateText;//計算文字
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetVersionCodeText() const//取得版本號文字
{	
	return m_VersionCodeText;//版本號文字
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetUndefinedText() const//取得未定義文字
{
	return m_UndefinedText;//未定義文字
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAccelerationText() const//取得加速度文字
{
	return m_AccelerationText;//加速度文字
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBarcodeDeviceText() const//條碼機文字	
{
	return m_BarcodeDeviceText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetSaftyBypassText() const//安全檢知文字	
{
	return m_SaftyBypassText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWaitForCCSText() const//等待中控文字	
{
	return m_WaitForCCSText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWaitForRepairText() const//等待維修站文字	
{
	return m_WaitForRepairText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetInspectionResultFaultText() const//檢測結果異常
{
	return m_InspectionResultFaultText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDoYouWantToClearTheStatisticRecordsText() const//是否清除統計資料
{
	return m_DoYouWantToClearTheStatisticRecordsText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Red() const//顏色文字-紅色
{
	return m_ColorText_Red;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Green() const//顏色文字-綠色
{
	return m_ColorText_Green;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Blue() const//顏色文字-藍色
{
	return m_ColorText_Blue;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Black() const//顏色文字-黑色
{
	return m_ColorText_Black;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Gray() const//顏色文字-灰色
{
	return m_ColorText_Gray;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_White() const//顏色文字-白色
{
	return m_ColorText_White;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetColorText_Color() const//顏色文字-彩色
{
	return m_ColorText_Color;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWndOKText() const//視窗OK文字
{
	return m_OKText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetWndCancelText() const//視窗Cancel文字
{
	return m_CancelText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDecodeText() const//取得解碼文字	
{
	return m_DecodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetLevelText() const//等級文字
{
	return m_LevelText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetEnableText() const//取得啟用文字
{
	return m_EnableText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetDisableText() const//取得啟用, 關閉文字
{
	return m_DisabeText;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetEnableDisableText(int nEnable)//取得啟用, 關閉文字
{
	CString str;
	switch ( nEnable )
	{
	case FN_ENABLE:		str = m_EnableText;	break;
	case FN_DISABLE:	str = m_DisabeText;	break;
	default:			str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPositiveNegativeText(int nPositive)//取得正負向文字
{
	CString str;
	switch ( nPositive )
	{
	case FN_ENABLE:		str = m_PositiveText;	break;
	case FN_DISABLE:	str = m_NegativeText;	break;
	default:			str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDistrictIDText(DISTRICT_ID Mode)//取得分區文字
{
	CString str;
	switch ( Mode )
	{
	case DISTRICT_ID_A:	str = m_DistrictText_A;	break;
	case DISTRICT_ID_B:	str = m_DistrictText_B;	break;
	default:			str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndDefectItemModeText(int Mode)//取得檢測框瑕疵項目模式文字
{
	CString str;
	switch ( Mode )
	{
	case WND_DEFECT_ITEM_DISABLE:	str = m_WndDefectItem_Disable;	break;
	case WND_DEFECT_ITEM_ENABLE:	str = m_WndDefectItem_Enable;	break;
	case WND_DEFECT_ITEM_NO_SHOW:	str = m_WndDefectItem_NoShow;	break;
	default:						str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindEnableDisableIDByText(LPCTSTR IDText)//取得啟用關閉編號
{	
	if ( m_EnableText.CompareNoCase(IDText) == 0 )
	{	return FN_ENABLE; }
	if ( m_DisabeText.CompareNoCase(IDText) == 0 )
	{	return FN_DISABLE; }
	return -1; //Exception
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildEnableDisableParamUni(CParamUni &ParamUnit)//建立啟用關閉模式列表	
{	
	ParamUnit.AddSelItem(FN_ENABLE, GetEnableDisableText(FN_ENABLE));
	ParamUnit.AddSelItem(FN_DISABLE, GetEnableDisableText(FN_DISABLE));	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPCBOutModeText(PCB_OUT_MODE Mode)//取得出板模式文字
{
	CString str;
	switch ( Mode )
	{
	case PCB_OUT_NORMAL:	str = m_PCBOutText_Normal;	break;
	case PCB_OUT_SIDE_OUT:	str = m_PCBOutText_SideOut;	break;
	case PCB_OUT_WITH_IN:	str = m_PCBOutText_WithIn;	break;
	case PCB_OUT_LANE_AUTO:	str = m_PCBOutText_LaneAuto;	break;
	case PCB_OUT_OK_OUT_NG_SIDE: str = m_PCBOutText_OkOutNgSide;	break;
	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMultiLaneModeText(MULTI_LANE_MODE Mode)//取得多軌道模式名稱
{
	CString str;
	switch ( Mode )
	{
	case MULTI_LANE_1:	str = m_MultiLaneText_1; break;
	case MULTI_LANE_2:	str = m_MultiLaneText_2; break;
	default:			str = m_UndefinedText; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFuncExecModeText(FUNC_EXEC_MODE Mode)//取得函式執行模式名稱
{
	CString str;
	switch ( Mode )
	{
	case FUNC_EXEC_OFF:	str = m_FuncExecText_Off; break;
	case FUNC_EXEC_AUTO:str = m_FuncExecText_Auto; break;
	case FUNC_EXEC_ASK:	str = m_FuncExecText_Ask; break;
	default:			str = m_UndefinedText; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetLaneWorkModeText(LANE_WORK_MODE Mode)//取得軌道運轉模式名稱
{
	CString str;
	switch ( Mode )
	{
	case LANE_WORK_DISABLE:	str = m_LaneWorkText_Disable; break;
	case LANE_WORK_RUN:		str = m_LaneWorkText_Run; break;
	case LANE_WORK_BYPASS:	str = m_LaneWorkText_Bypass; break;
	default:				str = m_UndefinedText; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetUserLevelModeText(USER_LEVEL_MODE Mode)//取得使用者權限模式文字
{
	CString str;
	switch ( Mode )
	{
	case USER_LEVEL_SIGN_OUT:	str = m_UserLevelText_SignOut;	break;
	case USER_LEVEL_OPERATOR:	str = m_UserLevelText_Operator;	break;
	case USER_LEVEL_ENGINEER:	str = m_UserLevelText_Engineer;	break;
	case USER_LEVEL_SUPERVISOR:	str = m_UserLevelText_Supervisor;	break;
	case USER_LEVEL_JET_FAE:	str = m_UserLevelText_JETFAE;	break;
	case USER_LEVEL_JET_SENIOR:	str = m_UserLevelText_JETSENIOR;	break;
	case USER_LEVEL_JET_RD:		str = AOI3D_VENDOR_CAT(" RD");	break;//_T("JET RD");
	default:					str = m_UndefinedText; 	break;
	}  	
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetUserLoginModeText(USER_LOGIN_MODE Mode)//取得使用者登入模式文字
{
	CString str;
	switch ( Mode )
	{
	case USER_LOGIN_DISABLE:	str = m_UserLoginText_Disable;	break;
	case USER_LOGIN_OPERATOR:	str = m_UserLoginText_Operator;	break;
	case USER_LOGIN_ENGINEER:	str = m_UserLoginText_Engineer;	break;	
	default:					str = m_UndefinedText; 	break;
	}  	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetUserLoginOptionsText(USER_LOGIN_OPTIONS Mode)
{
	CString str;
	switch (Mode)
	{
	case USER_LOGIN_OPTIONS_PASSWORD:		str = m_UserLoginOptionsText_Password;	break;
	case USER_LOGIN_OPTIONS_FINGERPRINT:	str = m_UserLoginOptionsText_Fingerprint;	break;
	case USER_LOGIN_OPTIONS_FINGERPRINT_ONLY: str = m_UserLoginOptionsText_FingerprintOnly;	break;
	default:					str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOpenProjectModeText(OPEN_PROJECT_MODE Mode)//取得開啟專案模式文字
{
	CString str;
	switch ( Mode )
	{
	case OPEN_PROJECT_FILE:	str = m_OpenProjectText_File;	break;
	case OPEN_PROJECT_CODE:	str = m_OpenProjectText_Code;	break;	
	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetVerifyProjectModeText(VERIFY_PROJECT_MODE Mode)//取得驗證專案模式文字	
{
	CString str;
	switch ( Mode )
	{
	case VERIFY_PROJECT_DISABLE:	str = m_VerifyProjectText_Disable;	break;
	case VERIFY_PROJECT_FILENAME:	str = m_VerifyProjectText_Filename;	break;	
	default:						str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOnlineInputTimingText(ONLINE_INPUT_TIMING Mode)//取得線上輸入時機文字
{
	CString str;
	switch ( Mode )
	{
	case ONLINE_INPUT_DISABLE:			str = m_OnlineInputTiming_Disalbe;	break;
	case ONLINE_INPUT_BEFORE_SIGN_IN:	str = m_OnlineInputTiming_BeforeSignIn;	break;			
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAOICustomerIDKey(AOI_CUSTOMER_ID ID)//取得客戶編號文字
{
	CString str;
	switch ( ID )
	{
	case AOI_CUSTOMER_ID_JET_TWN:		str = _T("AOI_CUSTOMER_ID_JET_TWN");	break;
	case AOI_CUSTOMER_ID_PEGATRON_TWN:	str = _T("AOI_CUSTOMER_ID_PEGATRON_TWN");	break;	
	case AOI_CUSTOMER_ID_KINPO_YUEYANG:	str = _T("AOI_CUSTOMER_ID_KINPO_YUEYANG");	break;
	case AOI_CUSTOMER_ID_FOXCONN_LONGHUA: str = _T("AOI_CUSTOMER_ID_FOXCONN_LONGHUA");	break;	
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAOICustomerIDText(AOI_CUSTOMER_ID ID)//取得客戶編號文字
{
	CString str;
	switch ( ID )
	{
	case AOI_CUSTOMER_ID_JET_TWN:		str = m_AOICustomerIDText_JET_TWN;	break;
	case AOI_CUSTOMER_ID_PEGATRON_TWN:	str = m_AOICustomerIDText_PegaTron_TWN;	break;	
	case AOI_CUSTOMER_ID_KINPO_YUEYANG:	str = m_AOICustomerIDText_Kinpo_YueYang;	break;	
	case AOI_CUSTOMER_ID_FOXCONN_LONGHUA: str = m_AOICustomerIDText_Foxconn_LongHua;	break;	
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOfflineVersionModeText(OFFLINE_VERSION_MODE Mode)//取得離線版本模式文字
{
	CString str;
	switch ( Mode )
	{
	case OFFLINE_VERSION_NORMAL:		str = m_OfflineVersionText_Normal;	break;
	case OFFLINE_VERSION_HOST_TUNING:	str = m_OfflineVersionText_HostTuning;	break;	
	case OFFLINE_VERSION_REMOTE_TUNING:	str = m_OfflineVersionText_RemoteTuning;	break;	
	default:							str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOnlineOpenProjectModeText(ONLINE_OPEN_PROJECT_MODE Mode)//取得線上開啟專案模式文字	
{
	CString str;
	switch ( Mode )
	{
	case ONLINE_OPEN_PROJECT_DISABLE:			str = m_OnlineOpenProjectText_Disable;	break;
	case ONLINE_OPEN_PROJECT_BARCODE_DEVICE:	str = m_OnlineOpenProjectText_BarcodeDevice;	break;	
	case ONLINE_OPEN_PROJECT_BARCODE_HANDHELD:  str = m_OnlineOpenProjectText_BarcodeHandHeld;	break;	
	case ONLINE_OPEN_PROJECT_BARCODE_CAMERA:	str = m_OnlineOpenProjectText_BarcodeCamera;	break;		
	default:									str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectLinkServerModeText(PROJECT_LINK_SERVER_MODE Mode)//取得伺服器資料庫模式文字
{
	CString str;
	switch ( Mode )
	{
	case PROJECT_LINK_SERVER_DISABLE:	str = m_ProjectLinkServerText_Disable;	break;
	case PROJECT_LINK_SERVER_ENABLE_ALL:	str = m_ProjectLinkServerText_EnableAll;	break;	
	case PROJECT_LINK_SERVER_PROJECT_ONLY:	str = m_ProjectLinkServerText_ProjectOnly;	break;	
	case PROJECT_LINK_SERVER_ENABLE_ALL_ASK:	str = m_ProjectLinkServerText_EnableAllAsk;	break;	
	case PROJECT_LINK_SERVER_PROJECT_ONLY_ASK:	str = m_ProjectLinkServerText_ProjectOnlyAsk; break;
	default:							str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetXBoardMappingFileModeText(XBOARD_MAPPING_FILE_MODE Mode)//取得報廢板檔案映射模式文字
{
	CString str;
	switch ( Mode )
	{
	case XBOARD_MAPPING_FILE_DISABLE:		str = m_XBoardMappingFileText_Disable;	break;
	case XBOARD_MAPPING_FILE_MES_COMM:      str = m_XBoardMappingFileText_MES_Comm; break;	
	default:								str = m_UndefinedText; 	break;
	}	
	return str;	
}
CString CAOIDataDefine::GetXBoardMappingFileFlowText(XBOARD_MAPPING_FILE_FLOW Mode) {
	CString str;
	switch (Mode)
	{
	case XBOARD_MAPPING_FILE_FLOW_DEFAULT:		str = m_XBoardMappingFileFlowText_Default;			break;
	case XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE:	str = m_XBoardMappingFileFlowText_After_Barcode;	break;
	default:
		break;
	}
	return str;
}


//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPCBOutDirectionText(PCB_OUT_DIRECTION Mode)//取得出板流向文字
{
	CString str;
	switch ( Mode )
	{
	case PCB_OUT_DIR_FORWARD:	str = m_PCBOutDirText_Forward;	break;
	case PCB_OUT_DIR_BACKWARD:	str = m_PCBOutDirText_Backward;	break;	
	case PCB_OUT_DIR_BACKWARD_OUT:	str = m_PCBOutDirText_BackwardOut;	break; 
	default:				str = m_UndefinedText; 	break;
	}
	return str;		
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildRS232PortCombox(CComboBox &Combox)//建立RS232的埠列表
{
	int idx = 0;
	DWORD Data=0;
	CString str;	

	std::vector<int> PortList;
	if ( JetAPI::EnumRS232(PortList) == false )
	{	return false; }

	idx=0;
	JetAPI::ClearCombox(Combox);
	std::sort(PortList.begin(), PortList.end());
	const int PortCount=(int)(PortList.size());
	for ( int i=0; i<PortCount; i++ )
	{
		Data = PortList[i];
		str.Format(_T("COM%d"), Data);
		Combox.AddString(str);
		Combox.SetItemData(idx, Data);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildRS232BaudCombox(CComboBox &Combox)//建立RS232的保率列表
{
	int idx = 0;
	DWORD Data=0;
	CString str;	
	const int Baud[]={CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000};
	const int BaudCount=sizeof(Baud)/sizeof(Baud[0]);

	idx=0;
	JetAPI::ClearCombox(Combox);
	for ( int i=0; i<BaudCount; i++ )
	{
		Data = Baud[i];
		str.Format(_T("%d"), Data);
		Combox.AddString(str);
		Combox.SetItemData(idx, Data);
		idx ++;
	}
	return true;

}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildRS232ParityCombox(CComboBox &Combox)//建立RS232的極性列表
{
	int idx = 0;
	DWORD Data=0;
	CString str;	

	idx=0;
	JetAPI::ClearCombox(Combox);

	Data=NOPARITY; str.Format(_T("[%d]-None"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=ODDPARITY; str.Format(_T("[%d]-Odd"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=EVENPARITY; str.Format(_T("[%d]-Even"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=MARKPARITY; str.Format(_T("[%d]-Mark"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=SPACEPARITY; str.Format(_T("[%d]-Space"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildRS232StopBitsCombox(CComboBox &Combox)//建立RS232的停止位元列表	
{
	int idx = 0;
	DWORD Data=0;
	CString str;	

	idx=0;
	JetAPI::ClearCombox(Combox);

	Data=ONESTOPBIT; str.Format(_T("[%d]-10"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=ONE5STOPBITS; str.Format(_T("[%d]-15"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	Data=TWOSTOPBITS; str.Format(_T("[%d]-20"), Data);	Combox.AddString(str);	Combox.SetItemData(idx, Data);	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildLoadCadxyTextFilterCombox(CComboBox &Combox)//建立載入CadXY的文字過濾列表
{
	int idx = 0;
	CString str;	

	idx=0;
	JetAPI::ClearCombox(Combox);

	str = _T(' ');
	Combox.AddString(str);
	Combox.SetItemData(idx, idx);
	idx ++;

	str = _T('"');
	Combox.AddString(str);
	Combox.SetItemData(idx, idx);
	idx ++;

	str = _T('\'');
	Combox.AddString(str);
	Combox.SetItemData(idx, idx);
	idx ++;

	Combox.SetCurSel(0);
	return true;
}
//-------------------------------------------------------------------------------------//
PANEL_SIDE_MODE CAOIDataDefine::FindPanelSideModeByText(LPCTSTR SideText)//依文字取得板面模式
{
	CString strDefect;
	PANEL_SIDE_MODE Mode;

	Mode = PANEL_SIDE_TOP;
	strDefect = GetPanelSideModeText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }

	Mode = PANEL_SIDE_BOTTOM;
	strDefect = GetPanelSideModeText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }

	Mode = PANEL_SIDE_HYBRID;
	strDefect = GetPanelSideModeText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }

	return PANEL_SIDE_TOP;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPanelSideModeText(PANEL_SIDE_MODE Mode)//取得出板面文字	
{
	CString str;
	switch ( Mode )
	{
	case PANEL_SIDE_TOP:	str = m_PanelSideText_Top;	break;
	case PANEL_SIDE_BOTTOM:	str = m_PanelSideText_Bottom;	break;	
	case PANEL_SIDE_HYBRID: str = m_PanelSideText_Hybrid;	break;	
	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildPanelSideModeCombox(CComboBox &Combox)//建立板面樣式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	PANEL_SIDE_MODE  SideMode;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	SideMode = PANEL_SIDE_TOP;
	str = GetPanelSideModeText(SideMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, SideMode);
	idx ++;

	SideMode = PANEL_SIDE_BOTTOM;
	str = GetPanelSideModeText(SideMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, SideMode);
	idx ++;

	SideMode = PANEL_SIDE_HYBRID;
	str = GetPanelSideModeText(SideMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, SideMode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
BOARD_SIDE_MODE CAOIDataDefine::FindBoardSideModeByText(LPCTSTR SideText)//依文字取得板面模式
{
	CString strDefect;
	BOARD_SIDE_MODE Mode;

	Mode = BOARD_SIDE_TOP;
	strDefect = GetBoardSideModeText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }

	Mode = BOARD_SIDE_BOT;
	strDefect = GetBoardSideModeText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }
	return BOARD_SIDE_TOP;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBoardSideModeText(BOARD_SIDE_MODE Mode)//取得出板面文字
{
	CString str;
	switch ( Mode )
	{
	case BOARD_SIDE_TOP:	str = m_BoardSideText_Top;	break;
	case BOARD_SIDE_BOT:	str = m_BoardSideText_Bottom;	break;		
	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBoardSideModeCombox(CComboBox &Combox)//建立板面樣式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BOARD_SIDE_MODE  SideMode;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	SideMode = BOARD_SIDE_TOP;
	str = GetBoardSideModeText(SideMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, SideMode);
	idx ++;

	SideMode = BOARD_SIDE_BOT;
	str = GetBoardSideModeText(SideMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, SideMode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
BARCODE_DECODER_TYPE CAOIDataDefine::FindBarcodeDecoderTypeByText(LPCTSTR SideText)//依文字取得條碼解碼器
{
	CString strDefect;
	BARCODE_DECODER_TYPE Mode;

	Mode = BARCODE_DECODER_EVS;
	strDefect = GetBarcodeDecoderText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }

#ifdef DTK_BARCODE_USE
	Mode = BARCODE_DECODER_DTK;
	strDefect = GetBarcodeDecoderText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }
#endif//DTK_BARCODE_USE

#ifdef HON_BARCODE_USE
	Mode = BARCODE_DECODER_HON;
	strDefect = GetBarcodeDecoderText(Mode);
	if ( strDefect.CompareNoCase(SideText) == 0 ) 
	{	return Mode; }
#endif//HON_BARCODE_USE

	return BARCODE_DECODER_DTK;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeDecoderText(BARCODE_DECODER_TYPE Mode)//取得條碼解碼器文字
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_DECODER_EVS:	str = m_BarcodeDecoder_EVS;	break;
	case BARCODE_DECODER_DTK:	str = m_BarcodeDecoder_DTK;	break;		
	case BARCODE_DECODER_HON:	str = m_BarcodeDecoder_HON;	break;
	default:					str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
BARCODE_SPREAD_MODE CAOIDataDefine::FindBarcodeSpreadModeByText(LPCTSTR ModeText)//依文字取得條碼擴散模式
{
	CString strDefect;
	BARCODE_SPREAD_MODE Mode = BARCODE_SPREAD_OFF;

	Mode = BARCODE_SPREAD_OFF;
	strDefect = GetBarcodeSpreadModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = BARCODE_SPREAD_LOCAL;
	strDefect = GetBarcodeSpreadModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = BARCODE_SPREAD_ALL;
	strDefect = GetBarcodeSpreadModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }
	return BARCODE_SPREAD_OFF;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeSpreadModeText(BARCODE_SPREAD_MODE Mode)//取得儲條碼擴散模式文字	
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_SPREAD_OFF:	str = m_BarcodeSpread_Off;	break;	
	case BARCODE_SPREAD_LOCAL:	str = m_BarcodeSpread_Local;	break;
	case BARCODE_SPREAD_ALL:	str = m_BarcodeSpread_All;	break;		
	default:					str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
BARCODE_BELONG_MODE CAOIDataDefine::FindBarcodeBelongModeByText(LPCTSTR ModeText)//依文字取得條碼屬於模式
{
	CString strDefect;	
	std::vector<BARCODE_BELONG_MODE> List;
	List.push_back(BARCODE_BELONG_NONE);
	List.push_back(BARCODE_BELONG_PROJECT);
	List.push_back(BARCODE_BELONG_PANEL);
	List.push_back(BARCODE_BELONG_BOARD);
	List.push_back(BARCODE_BELONG_TRAY);
	List.push_back(BARCODE_BELONG_COVER);

	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		BARCODE_BELONG_MODE Mode = List[i];
		strDefect = GetBarcodeBelongModeText(Mode);
		if ( strDefect.CompareNoCase(ModeText) == 0 ) 
		{	return Mode; }
	}
	return BARCODE_BELONG_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeBelongModeText(BARCODE_BELONG_MODE Mode)//取得儲條碼屬於模式文字	
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_BELONG_NONE:	str = m_BarcodeBelong_None;	break;	
	case BARCODE_BELONG_PROJECT:str = m_BarcodeBelong_Project;	break;
	case BARCODE_BELONG_PANEL:	str = m_BarcodeBelong_Panel;	break;	
	case BARCODE_BELONG_BOARD:	str = m_BarcodeBelong_Board;	break;		
	case BARCODE_BELONG_TRAY:	str = m_BarcodeBelong_Tray;	break;
	case BARCODE_BELONG_COVER:	str = m_BarcodeBelong_Cover;	break;
	default:					str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_MAP_MODE CAOIDataDefine::FindSaveTestMapModeByText(LPCTSTR ModeText)//依文字取得儲存檢測底圖模式
{
	CString strDefect;
	SAVE_TEST_MAP_MODE Mode = SAVE_TEST_MAP_DISABLE;

	Mode = SAVE_TEST_MAP_DISABLE;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_MAP_ENB_PROG;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_MAP_ENB_PANEL;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_MAP_ENB_PROG_PANEL;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_MAP_ENB_BOARD;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_MAP_ENB_PROG_BOARD;
	strDefect = GetSaveTestMapModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }
	return SAVE_TEST_MAP_DISABLE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetSaveTestMapModeText(SAVE_TEST_MAP_MODE Mode)//取得儲存檢測底圖模式文字	
{
	CString str;
	switch ( Mode )
	{
	case SAVE_TEST_MAP_DISABLE:			str = m_SaveTestMapText_Disable;	break;	
	case SAVE_TEST_MAP_ENB_PROG:		str = m_SaveTestMapText_Prog;	break;
	case SAVE_TEST_MAP_ENB_PANEL:		str = m_SaveTestMapText_Panel;	break;	
	case SAVE_TEST_MAP_ENB_PROG_PANEL:	str = m_SaveTestMapText_ProgPanel;	break;	
	case SAVE_TEST_MAP_ENB_BOARD:		str = m_SaveTestMapText_Board;	break;	
	case SAVE_TEST_MAP_ENB_PROG_BOARD:	str = m_SaveTestMapText_ProgBoard;	break;	
	default:							str = m_UndefinedText; 	break;
	}		
	return str;	
}
//-------------------------------------------------------------------------------------//
OFFLINE_IMAGE_SCOPE CAOIDataDefine::FindOfflineImageScopeByText(LPCTSTR ScopeText)//依文字取得離線影像範疇
{
	CString strDefect;
	OFFLINE_IMAGE_SCOPE Mode = OFFLINE_IMAGE_RETURN;

	Mode = OFFLINE_IMAGE_FOV;
	strDefect = GetOfflineImageScopeText(Mode);
	if ( strDefect.CompareNoCase(ScopeText) == 0 ) 
	{	return Mode; }

	Mode = OFFLINE_IMAGE_PART;
	strDefect = GetOfflineImageScopeText(Mode);
	if ( strDefect.CompareNoCase(ScopeText) == 0 ) 
	{	return Mode; }

	return OFFLINE_IMAGE_FOV;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOfflineImageScopeText(OFFLINE_IMAGE_SCOPE Mode)//取得離線影像範疇
{
	CString str;
	switch ( Mode )
	{
	case OFFLINE_IMAGE_FOV:		str = m_OfflineImageText_Fov;	break;	
	case OFFLINE_IMAGE_PART:	str = m_OfflineImageText_Part;	break;	
	default:					str = m_UndefinedText; 	break;
	}		
	return str;	
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_IMAGE_MODE CAOIDataDefine::FindSaveTestImageModeByText(LPCTSTR ModeText)//依文字取得儲存檢測圖片模式
{
	CString strDefect;
	SAVE_TEST_IMAGE_MODE Mode = SAVE_TEST_IMAGE_DISABLE;

	Mode = SAVE_TEST_IMAGE_DISABLE;
	strDefect = GetSaveTestImageModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_IMAGE_DEFECT;
	strDefect = GetSaveTestImageModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_IMAGE_EVERYONE;
	strDefect = GetSaveTestImageModeText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	return SAVE_TEST_IMAGE_DISABLE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetSaveTestImageModeText(SAVE_TEST_IMAGE_MODE Mode)//取得儲存檢測圖片模式文字	
{
	CString str;
	switch ( Mode )
	{
	case SAVE_TEST_IMAGE_DISABLE:	str = m_SaveTestImageText_Disable;	break;	
	case SAVE_TEST_IMAGE_DEFECT:	str = m_SaveTestImageText_Defect;	break;
	case SAVE_TEST_IMAGE_EVERYONE:	str = m_SaveTestImageText_EveryOne;	break;	
	default:						str = m_UndefinedText; 	break;
	}		
	return str;	
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_DATA_MODE CAOIDataDefine::FindSaveTestDataModeByText(LPCTSTR ModeText)//依文字取得儲存資料模式
{
	CString strDefect;
	SAVE_TEST_DATA_MODE Mode = SAVE_TEST_DATA_DISABLE;

	Mode = SAVE_TEST_DATA_DISABLE;
	strDefect = GetSaveTestDataText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_DATA_ENABLE;
	strDefect = GetSaveTestDataText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	Mode = SAVE_TEST_DATA_DEFECT;
	strDefect = GetSaveTestDataText(Mode);
	if ( strDefect.CompareNoCase(ModeText) == 0 ) 
	{	return Mode; }

	return SAVE_TEST_DATA_DISABLE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetSaveTestDataText(SAVE_TEST_DATA_MODE Mode)//取得儲存檢測資料模式文字
{
	CString str;
	switch ( Mode )
	{
	case SAVE_TEST_DATA_DISABLE:str = m_SaveTestDataText_Disable;	break;	
	case SAVE_TEST_DATA_ENABLE:	str = m_SaveTestDataText_Enable;	break;
	case SAVE_TEST_DATA_DEFECT:	str = m_SaveTestDataText_Defect;	break;	
	default:						str = m_UndefinedText; 	break;
	}		
	return str;	
}
//-------------------------------------------------------------------------------------//	
CString CAOIDataDefine::GetFdNGHandleModeText(FD_NG_HANDLE_MODE Mode)//取得定位點錯誤處理模式文字
{
	CString str;
	switch ( Mode )
	{
	case FD_NG_HANDLE_NONE:	str = m_FdNGHandleText_None;	break;
	case FD_NG_HANDLE_PASS:	str = m_FdNGHandleText_Pass;	break;
	case FD_NG_HANDLE_STOP:	str = m_FdNGHandleText_Stop;	break;
	case FD_NG_HANDLE_XBOARD:	str = m_FdNGHandleText_XBoard;	break;	
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBoardFdGrabModeText(BOARD_FD_GRAB_MODE Mode)//取得單板定位點取像模式文字
{
	CString str;
	switch ( Mode )
	{
	case BOARD_FD_GRAB_AFTER_PANEL:	str = m_BoardFdGrabText_AfterPanel;	break;
	case BOARD_FD_GRAB_INSPECTING:	str = m_BoardFdGrabText_Inspecting;	break;	
	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDefectHandleModeText(DEFECT_HANDLE_MODE Mode)//取得檢出瑕疵處理模式文字	
{
	CString str;
	switch ( Mode )
	{
	case DEFECT_HANDLE_PASS:			str = m_DefectHandleText_Pass;	break;
	case DEFECT_HANDLE_STOP_ALARM:		str = m_DefectHandleText_Stop;	break;	
	case DEFECT_HANDLE_NEXT_STOP:		str = m_DefectHandleText_Next;	break;	
	case DEFECT_HANDLE_WAIT_FOR_REPAIR:	str = m_DefectHandleText_Repair;	break;	
	case DEFECT_HANDLE_CONTROL_CENTER:	str = m_DefectHandleText_ControlCenter;	break;	
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOnlineStateText(ONLINE_STATE_MODE State)//取得在線檢測狀態文字
{
	CString str;	
	switch ( State )
	{
	case ONLINE_STATE_INSPECTION_STOP:		str = m_OnlineState_InspectionStop;	break;
	case ONLINE_STATE_PCB_READY:			str = m_OnlineState_PCBReady;	break;
	case ONLINE_STATE_INPUT_BARCODE:		str = m_OnlineState_InputBarcode;	break;				
	case ONLINE_STATE_PROJECT_MAP:          str = m_OnlineState_ProjectMap; break;
	case ONLINE_STATE_PROJECT_MARK:         str = m_OnlineState_ProjectMark; break;
	case ONLINE_STATE_PROJECT_OPEN_CODE:	str = m_OnlineState_ProjectOpenCode; break;
	case ONLINE_STATE_PROJECT_RELOAD:		str = m_OnlineState_ProjectReload;	break;	
	case ONLINE_STATE_PROJECT_RELOAD_SERVER:str = m_OnlineState_ProjectReloadServer;	break;	
	case ONLINE_STATE_PROJECT_SWITCH_BY_TURN: str = m_OnlineState_ProjectSwitchByTurn;	break;	
	case ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET: str = m_OnlineState_ProjectSwitchByTurnOneCycleReset;	break;
	case ONLINE_STATE_INSPECTION_START:		str = m_OnlineState_InspectionStart;	break;	
	case ONLINE_STATE_INSPECTION_WAITING:	str = m_OnlineState_InspectionWaittng;	break;	
	case ONLINE_STATE_INSPECT_FD_PANEL:	str = m_OnlineState_InspectFDPanel;	break;	
	case ONLINE_STATE_INSPECT_FD_BOARD:	str = m_OnlineState_InspectFDBoard;	break;	
	case ONLINE_STATE_INSPECT_BARCODE:	str = m_OnlineState_InspectBarcode;	break;
	case ONLINE_STATE_INSPECT_PROJECT:	str = m_OnlineState_InspectProject;	break;	
	case ONLINE_STATE_STATICS_PROJECT:	str = m_OnlineState_StatisticProject;	break;	
	case ONLINE_STATE_INSPECTION_FINISH:	str = m_OnlineState_InspectionFinish;	break;	
	case ONLINE_STATE_WAIT_FOR_LAST_STATION:	str = m_OnlineState_WaitForLast;	break;	
	case ONLINE_STATE_WAIT_FOR_NEXT_STATION:	str = m_OnlineState_WaitForNext;	break;	
	case ONLINE_STATE_WAIT_FOR_PCB_REMOVED:	str = m_OnlineState_WaitForPCBRemoved;	break;	
	case ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY:	str = m_OnlineState_WaitForRepairVerify;	break;
	case ONLINE_STATE_PCB_IN_START:	str = m_OnlineState_PCBInStart;	break;	
	case ONLINE_STATE_PCB_IN_CHECKING:	str = m_OnlineState_PCBInChecking;	break;	
	case ONLINE_STATE_PCB_IN_FINISH:	str = m_OnlineState_PCBInFinish;	break;		
	case ONLINE_STATE_PCB_OUT_START:	str = m_OnlineState_PCBOutStart;	break;	
	case ONLINE_STATE_PCB_OUT_CHECKING:	str = m_OnlineState_PCBOutChecking;	break;	
	case ONLINE_STATE_PCB_OUT_FINISH:	str = m_OnlineState_PCBOutFinish;	break;	
	case ONLINE_STATE_PCB_OUT_INSIDE_START:	str = m_OnlineState_PCBOutInsideStart;	break;	
	case ONLINE_STATE_PCB_OUT_INSIDE_CHECKING:	str = m_OnlineState_PCBOutInsideChecking;	break;	
	case ONLINE_STATE_PCB_OUT_INSIDE_FINISH:	str = m_OnlineState_PCBOutInsideFinish;	break;	
	case ONLINE_STATE_PCB_BACK_START:	str = m_OnlineState_PCBBackStart;	break;	
	case ONLINE_STATE_PCB_BACK_CHECKING:	str = m_OnlineState_PCBBackChecking;	break;	
	case ONLINE_STATE_PCB_BACK_FINISH:	str = m_OnlineState_PCBBackFinish;	break;	
	case ONLINE_STATE_PCB_BACK_OUT_START:	str = m_OnlineState_PCBBackOutStart;	break;	
	case ONLINE_STATE_PCB_BACK_OUT_CHECKING:	str = m_OnlineState_PCBBackOutChecking;	break;	
	case ONLINE_STATE_PCB_BACK_OUT_FINISH:	str = m_OnlineState_PCBBackOutFinish;	break;	
	case ONLINE_STATE_PCB_OUT_IN_START:	str = m_OnlineState_PCBOutInStart;	break;	
	case ONLINE_STATE_PCB_OUT_IN_CHECKING:	str = m_OnlineState_PCBOutInChecking;	break;	
	case ONLINE_STATE_PCB_OUT_IN_FINISH:	str = m_OnlineState_PCBOutInFinish;	break;	
	case ONLINE_STATE_PCB_AUTO_RUN_START:	 str = m_OnlineState_PCBAutoRunStart;	break;	
	case ONLINE_STATE_PCB_AUTO_RUN_CHECKING: str = m_OnlineState_PCBAutoRunChecking;	break;	
	case ONLINE_STATE_PCB_AUTO_RUN_FINISH:	 str = m_OnlineState_PCBAutoRunFinish;	break;			
	case ONLINE_STATE_PCB_DUAL_RUN_START:	 str = m_OnlineState_PCBDualRunStart;	break;	
	case ONLINE_STATE_PCB_DUAL_RUN_CHECKING: str = m_OnlineState_PCBDualRunChecking;	break;	
	case ONLINE_STATE_PCB_DUAL_RUN_FINISH:	 str = m_OnlineState_PCBDualRunFinish;	break;
	case ONLINE_STATE_PCB_INSPECTION_PAUSE:  str = m_OnlineState_PCBInspectionPause; break;
	case ONLINE_STATE_AUTO_CALIBRATION_XYZ_HOME:	str = m_OnlineState_AutoCalibration_XYZ_Home; break;
	case ONLINE_STATE_AUTO_CALIBRATION_2D_CURRENT:		str = m_OnlineState_AutoCalibration_2D_Current;	break;
	case ONLINE_STATE_AUTO_CALIBRATION_3D_CURRENT:		str = m_OnlineState_AutoCalibration_3D_Current;	break;
	case ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE:	 str = m_OnlineState_AutoCalibration_3D_ZeroPlane;	break;
	case ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR: str = m_OnlineState_AutoCalibration_3D_FactorFactor;	break;
	case ONLINE_STATE_APP_OPEN:	 str = m_OnlineState_AppOpen;	break;			
	case ONLINE_STATE_APP_CLOSE:	 str = m_OnlineState_AppClose;	break;			
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_MODE Mode)//取得MES機台控制模式文字
{
	CString str;
	switch ( Mode )
	{	
	case MES_EQP_CTRL_STATE_NONE:	str = m_MesEqpCtrlStateText_None; break;
	case MES_EQP_CTRL_STATE_OFFLINE:str = m_MesEqpCtrlStateText_Offline; break;
	case MES_EQP_CTRL_STATE_LOCAL:	str = m_MesEqpCtrlStateText_Local; break;		
	case MES_EQP_CTRL_STATE_REMOTE:	str = m_MesEqpCtrlStateText_Remote; break;		
	default:						str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFieldPathMode(FIELD_PATH_MODE Mode)//取得區域路徑模式文字
{
	CString str;
	switch ( Mode )
	{	
	case FIELD_PATH_SPATH_HOR: str = m_FieldPathModeText_Hor; break;
	case FIELD_PATH_SPATH_VER: str = m_FieldPathModeText_Ver; break;
	case FIELD_PATH_SPATH_USER: str = m_FieldPathModeText_User; break;		
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFieldDivisionMode(FIELD_DIVISION_MODE Mode)//取得區域分割模式文字
{
	CString str;
	switch ( Mode )
	{	
	case FIELD_DIVISION_MASS_AREA:       str = m_FieldDivisionModeText_MassArea; break;
	case FIELD_DIVISION_DIAGONAL_LINE:   str = m_FieldDivisionModeText_Diagonal; break;
	case FIELD_DIVISION_HORIZONTAL_LINE: str = m_FieldDivisionModeText_Horizontal; break;
	case FIELD_DIVISION_VERTICAL_LINE:   str = m_FieldDivisionModeText_Vertical; break;
	default:							str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetInspectionFieldBuildMode(FIELD_BUILD_MODE Mode)//取得檢測區域配置模式文字
{
	CString str;
	switch ( Mode )
	{
	case FIELD_BUILD_MATRIX:	str = m_FieldBuildModeText_Matrix; break;
	case FIELD_BUILD_RANDOM_PANEL: str = m_FieldBuildModeText_Random_Panel; break;
	case FIELD_BUILD_RANDOM_BOARD: str = m_FieldBuildModeText_Random_Board; break;
	case FIELD_BUILD_RANDOM_PROJECT: str = m_FieldBuildModeText_Random_Project; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetConnectedBufferTypeText(CONNECTED_BUFFER_TYPE Type)//取得連接軌道機樣式名稱	
{
	CString str;
	switch ( Type )
	{
	case CONNECTED_BUFFER_FIXED:	str = m_ConnectedBufferType_Fixed; break;
	case CONNECTED_BUFFER_MOVABLE: str = m_ConnectedBufferType_Movable; break;
	default:						str = m_UndefinedText; break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetHASIStageText(HASI_AOI_STAGE Stage)
{
	CString str;
	switch (Stage)
	{
	case HASI_AOI_STAGE_NONE:	str = m_DisabeText;	break;
	case HASI_AOI_STAGE_PRE:	str = m_HASI_AOIStage_Pre;	break;
	case HASI_AOI_STAGE_POST:	str = m_HASI_AOIStage_Post; break;
	default:		str = m_UndefinedText; break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetHASIStateModeText(HASI_STATE_MODE Mode)
{
	CString str;
	switch (Mode)
	{
	case HASI_STATE_MODE_STOP:	str = m_HASI_State_mode_STOP;	break;
	case HASI_STATE_MODE_SC:	str = m_HASI_State_mode_AC;	break;
	case HASI_STATE_MODE_AC:	str = m_HASI_State_mode_SC;	break;
	case HASI_STATE_MODE_SCAC:	str = m_HASI_State_mode_SCAC;	break;
	case HASI_STATE_MODE_RUN:	str = m_HASI_State_mode_RUN;	break;
	case HASI_STATE_MODE_TEST:	str = m_HASI_State_mode_TEST;	break;
	default:		str = m_UndefinedText; break;
	}
	return str;
}

//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeInputTypeText(BARCODE_INPUT_TYPE ReadType)//取得條碼輸入樣式名稱
{
	CString str;
	switch ( ReadType )
	{
	case BARCODE_INPUT_DISABLED: str = m_BarcodeInputText_Disabled;	break;
	case BARCODE_INPUT_DEVICE:	 str = m_BarcodeInputText_Device;	break;		
	case BARCODE_INPUT_HANDHELD: str = m_BarcodeInputText_Handheld;	break;	
	default:					 str = m_UndefinedText; 	break;
	}		
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeInputTypeCombox(CComboBox &Combox)//建立條碼輸入樣式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BARCODE_INPUT_TYPE  ReadType=BARCODE_INPUT_DISABLED;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	ReadType = BARCODE_INPUT_DISABLED;
	str = GetBarcodeInputTypeText(ReadType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ReadType);
	idx ++;

	ReadType = BARCODE_INPUT_DEVICE;
	str = GetBarcodeInputTypeText(ReadType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ReadType);
	idx ++;

	ReadType = BARCODE_INPUT_HANDHELD;
	str = GetBarcodeInputTypeText(ReadType);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ReadType);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeNGHandleModeText(BARCODE_NG_HANDLE_MODE Mode)//取得條碼失敗處理模式名稱
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_NG_HANDLE_PASS:	str = m_BarcodeNGHandleText_Pass;	break;
	case BARCODE_NG_HANDLE_ALARM:	str = m_BarcodeNGHandleText_Alarm;	break;
	case BARCODE_NG_HANDLE_INPUT:	str = m_BarcodeNGHandleText_Input;	break;
	default:					str = m_UndefinedText; 	break;
	}		
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeNGHandleModeCombox(CComboBox &Combox)//建立條碼失敗處理模式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BARCODE_NG_HANDLE_MODE  Mode=BARCODE_NG_HANDLE_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = BARCODE_NG_HANDLE_PASS;
	str = GetBarcodeNGHandleModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_NG_HANDLE_ALARM;
	str = GetBarcodeNGHandleModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_NG_HANDLE_INPUT;
	str = GetBarcodeNGHandleModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMultiProjectTestOrderText(MULTI_PROJECT_TEST_ORDER_MODE Mode)//取得多專案檢測次序文字
{
	CString str;
	switch ( Mode )
	{
	case MULTI_PROJECT_TEST_ORDER_BY_MARK:	str = m_MultiProjectTestOrderText_ByMark;	break;
	case MULTI_PROJECT_TEST_ORDER_BY_TURN:	str = m_MultiProjectTestOrderText_InTurn;	break;			
	case MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B:	str = m_MultiProjectTestOrderText_OneCycleAB;	break;
	case MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A:	str = m_MultiProjectTestOrderText_OneCycleBA;	break;
	default:								str = m_UndefinedText; 	break;
	}		
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildMultiProjectTestOrderCombox(CComboBox &Combox)//建立多專案檢測次序列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	MULTI_PROJECT_TEST_ORDER_MODE  Mode=MULTI_PROJECT_TEST_ORDER_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = MULTI_PROJECT_TEST_ORDER_BY_MARK;
	str = GetMultiProjectTestOrderText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = MULTI_PROJECT_TEST_ORDER_BY_TURN;
	str = GetMultiProjectTestOrderText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B;
	str = GetMultiProjectTestOrderText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A;
	str = GetMultiProjectTestOrderText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeCameraGrabModeText(BARCODE_CAMERA_GRAB_MODE Mode)//取得相機條碼取像模式文字
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_CAMERA_GRAB_AFTER_FD:		str = m_BarcodeCameraGrabModeText_AfterFd;	break;
	case BARCODE_CAMERA_GRAB_INSPECTING:	str = m_BarcodeCameraGrabModeText_Inspecting;	break;			
	default:								str = m_UndefinedText; 	break;
	}		
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeCameraGrabModeCombox(CComboBox &Combox)//建立相機條碼取像模式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BARCODE_CAMERA_GRAB_MODE  Mode=BARCODE_CAMERA_GRAB_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = BARCODE_CAMERA_GRAB_AFTER_FD;
	str = GetBarcodeCameraGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_CAMERA_GRAB_INSPECTING;
	str = GetBarcodeCameraGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeDeviceGrabModeText(BARCODE_DEVICE_GRAB_MODE Mode)//取得條碼機取像模式文字
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_DEVICE_GRAB_BEFORE_PCB_IN:	str = m_BarcodeDeviceGrabModeText_BeforePCBIn;	break;
	case BARCODE_DEVICE_GRAB_WHILE_PCB_IN:	str = m_BarcodeDeviceGrabModeText_WhilePCBIn;	break;			
	case BARCODE_DEVICE_GRAB_AFTER_PCB_IN:	str = m_BarcodeDeviceGrabModeText_AfterPCBIn;	break;			
	case BARCODE_DEVICE_GRAB_BEFORE_INSPECT: str = m_BarcodeDeviceGrabModeText_BeforeInspect; break;
	default:								str = m_UndefinedText; 	break;
	}		
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeDeviceGrabModeCombox(CComboBox &Combox)//建立條碼機取像模式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BARCODE_DEVICE_GRAB_MODE  Mode=BARCODE_DEVICE_GRAB_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = BARCODE_DEVICE_GRAB_BEFORE_PCB_IN;
	str = GetBarcodeDeviceGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_DEVICE_GRAB_WHILE_PCB_IN;
	str = GetBarcodeDeviceGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_DEVICE_GRAB_AFTER_PCB_IN;
	str = GetBarcodeDeviceGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_DEVICE_GRAB_BEFORE_INSPECT;
	str = GetBarcodeDeviceGrabModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeHandHeldReadModeText(BARCODE_HANDHELD_READ_MODE Mode)//取得手持條碼機取像模式文字
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_HANDHELD_READ_MANUAL:	str = m_BarcodeHandHeldReadModeText_Manual;	break;
	case BARCODE_HANDHELD_READ_PROJECT:	str = m_BarcodeHandHeldReadModeText_Project;	break;			
	case BARCODE_HANDHELD_READ_PANEL:	str = m_BarcodeHandHeldReadModeText_Panel;	break;			
	case BARCODE_HANDHELD_READ_BOARD:	str = m_BarcodeHandHeldReadModeText_Board;	break;			
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeHandHeldReadModeCombox(CComboBox &Combox)//建立手持條碼機取像模式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	BARCODE_HANDHELD_READ_MODE  Mode=BARCODE_HANDHELD_READ_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = BARCODE_HANDHELD_READ_MANUAL;
	str = GetBarcodeHandHeldReadModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_HANDHELD_READ_PROJECT;
	str = GetBarcodeHandHeldReadModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_HANDHELD_READ_PANEL;
	str = GetBarcodeHandHeldReadModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = BARCODE_HANDHELD_READ_BOARD;
	str = GetBarcodeHandHeldReadModeText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
BARCODE_AUTO_EXPAND_MODE CAOIDataDefine::GetBarcodeAutoExpandModeByInt(int Param) const
{
	BARCODE_AUTO_EXPAND_MODE Mode=BARCODE_AUTO_EXPAND_RETURN;
	switch ( Param )
	{	
	case BARCODE_AUTO_EXPAND_INCREMENT:
	case BARCODE_AUTO_EXPAND_ADD_CHAR_1:
	case BARCODE_AUTO_EXPAND_ADD_CHAR_2:
	case BARCODE_AUTO_EXPAND_REPLACE_01:
	case BARCODE_AUTO_EXPAND_REPLACE_02:
	case BARCODE_AUTO_EXPAND_INCREMENT_BASE36:
		Mode=(BARCODE_AUTO_EXPAND_MODE)(Param);
		break;
	default:
	case BARCODE_AUTO_EXPAND_DISABLE:
		Mode=BARCODE_AUTO_EXPAND_DISABLE;
		break;
	}
	return Mode;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::GetBarcodeAutoExpandModeList(std::vector<BARCODE_AUTO_EXPAND_MODE> &List)//取得條碼自動擴展模式列表
{
	List.clear();
	List.push_back(BARCODE_AUTO_EXPAND_DISABLE);
	List.push_back(BARCODE_AUTO_EXPAND_INCREMENT);
	List.push_back(BARCODE_AUTO_EXPAND_ADD_CHAR_1);
	List.push_back(BARCODE_AUTO_EXPAND_ADD_CHAR_2);
	List.push_back(BARCODE_AUTO_EXPAND_REPLACE_01);
	List.push_back(BARCODE_AUTO_EXPAND_REPLACE_02);
	List.push_back(BARCODE_AUTO_EXPAND_INCREMENT_BASE36);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeAutoExpandModeText(BARCODE_AUTO_EXPAND_MODE Mode)//取得條碼自動擴展模式文字
{
	CString str;
	switch ( Mode )
	{
	case BARCODE_AUTO_EXPAND_DISABLE:		str = m_BarcodeAutoExpandModeText_Disable;	break;
	case BARCODE_AUTO_EXPAND_INCREMENT:		str = m_BarcodeAutoExpandModeText_Increment;	break;			
	case BARCODE_AUTO_EXPAND_ADD_CHAR_1:	str = m_BarcodeAutoExpandModeText_AddChar01;	break;			
	case BARCODE_AUTO_EXPAND_ADD_CHAR_2:	str = m_BarcodeAutoExpandModeText_AddChar02;	break;			
	case BARCODE_AUTO_EXPAND_REPLACE_01:	str = m_BarcodeAutoExpandModeText_Replace01;	break;			
	case BARCODE_AUTO_EXPAND_REPLACE_02:	str = m_BarcodeAutoExpandModeText_Replace02;	break;			
	case BARCODE_AUTO_EXPAND_INCREMENT_BASE36:	str = m_BarcodeAutoExpandModeText_Inc_Base36;	break;
	default:								str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeAutoExpandModeCombox(CComboBox &Combox)//建立條碼自動擴展模式列表	
{
	size_t       i=0;
	int          idx=0;	
	CString      str;
	std::vector<BARCODE_AUTO_EXPAND_MODE> List;
	BARCODE_AUTO_EXPAND_MODE  Mode=BARCODE_AUTO_EXPAND_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	if ( GetBarcodeAutoExpandModeList(List) == false ) { return false; }
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		Mode = List[i];
		str = GetBarcodeAutoExpandModeText(Mode);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Mode);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeAutoExpandModeParamUni(CParamUni &ParamUnit)//建立條碼自動擴展模式列表	
{
	CString strValue;
	std::vector<BARCODE_AUTO_EXPAND_MODE> List;
	BARCODE_AUTO_EXPAND_MODE  Mode=BARCODE_AUTO_EXPAND_RETURN;
	if ( GetBarcodeAutoExpandModeList(List) == false ) { return false; }
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		Mode = List[i];
		strValue = GetBarcodeAutoExpandModeText(Mode);
		ParamUnit.AddSelItem(Mode, strValue);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeDirectionModeText(ALG_BARCODE_DIR_MODE Mode)//取得條碼方向模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_BARCODE_DIR_AUTO:	str = m_BarcodeDirectionModeText_Auto;	break;
	case ALG_BARCODE_DIR_HOR:	str = m_BarcodeDirectionModeText_Hor;	break;			
	case ALG_BARCODE_DIR_VER:	str = m_BarcodeDirectionModeText_Ver;	break;			
	case ALG_BARCODE_DIR_ALL:	str = m_BarcodeDirectionModeText_All;	break;			
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_BARCODE_DIR_MODE CAOIDataDefine::FindBarcodeDirectionModeByText(LPCTSTR DirText)//依文字取得條碼方向模式
{
	CString  strDirText;
	ALG_BARCODE_DIR_MODE DirMode;

	DirMode = ALG_BARCODE_DIR_AUTO;
	strDirText = GetBarcodeDirectionModeText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	DirMode = ALG_BARCODE_DIR_HOR;
	strDirText = GetBarcodeDirectionModeText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	DirMode = ALG_BARCODE_DIR_VER;
	strDirText = GetBarcodeDirectionModeText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	DirMode = ALG_BARCODE_DIR_ALL;
	strDirText = GetBarcodeDirectionModeText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	return ALG_BARCODE_DIR_ALL;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::CheckAutoSwitchWnd3DFrameByType(MODEL_TYPE Type)//確認自動切換檢測框3D畫面依據樣式
{
	bool bEnable = false;
	switch ( Type ) 
	{
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:
		bEnable = true;
		break;

	case MODEL_TYPE_ELECTRODE:
	case MODEL_TYPE_TANTALUM_CONDENSER:
	case MODEL_TYPE_CAPACITY_ARRAY:
	case MODEL_TYPE_RESISTOR_ARRAY:
	case MODEL_TYPE_TRANSISTOR:
	case MODEL_TYPE_LEAD_TRANSISTOR:
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
	case MODEL_TYPE_LED_ARRAY:	
		bEnable = true;
		break;

	default:
		bEnable = false;
		break;
	}
	return bEnable;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAutoSwitchWnd3DFrameModeText(AUTO_SWITCH_WND_3D_FRAME_MODE Mode)//取得自動切換檢測框3D模式文字
{
	CString str;
	switch ( Mode )
	{
	case AUTO_SWITCH_WND_3D_FRAME_DISABLE:	str = m_AutoSwitchWnd3DFrame_Disable;	break;
	case AUTO_SWITCH_WND_3D_FRAME_ENABLE:	str = m_AutoSwitchWnd3DFrame_Enable;	break;
	case AUTO_SWITCH_WND_3D_FRAME_BY_SIZE:	str = m_AutoSwitchWnd3DFrame_BySize;	break;
	case AUTO_SWITCH_WND_3D_FRAME_BY_TYPE:	str = m_AutoSwitchWnd3DFrame_ByType;	break;	
	case AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE:	str = m_AutoSwitchWnd3DFrame_ByGroupChange;	break;
	default:							str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
AUTO_SWITCH_WND_3D_FRAME_MODE CAOIDataDefine::FindAutoSwitchWnd3DFrameModeByText(LPCTSTR SwitchText)//依自動切換檢測框3D模式
{
	CString  strSwitchText;
	AUTO_SWITCH_WND_3D_FRAME_MODE SwitchMode;

	SwitchMode = AUTO_SWITCH_WND_3D_FRAME_DISABLE;
	strSwitchText = GetAutoSwitchWnd3DFrameModeText(SwitchMode);
	if ( strSwitchText.CompareNoCase(SwitchText) == 0 ) 
	{	return SwitchMode; }

	SwitchMode = AUTO_SWITCH_WND_3D_FRAME_ENABLE;
	strSwitchText = GetAutoSwitchWnd3DFrameModeText(SwitchMode);
	if ( strSwitchText.CompareNoCase(SwitchText) == 0 ) 
	{	return SwitchMode; }

	SwitchMode = AUTO_SWITCH_WND_3D_FRAME_BY_SIZE;
	strSwitchText = GetAutoSwitchWnd3DFrameModeText(SwitchMode);
	if ( strSwitchText.CompareNoCase(SwitchText) == 0 ) 
	{	return SwitchMode; }

	SwitchMode = AUTO_SWITCH_WND_3D_FRAME_BY_TYPE;
	strSwitchText = GetAutoSwitchWnd3DFrameModeText(SwitchMode);
	if ( strSwitchText.CompareNoCase(SwitchText) == 0 ) 
	{	return SwitchMode; }

	SwitchMode = AUTO_SWITCH_WND_3D_FRAME_BY_GROUP_CHANGE;
	strSwitchText = GetAutoSwitchWnd3DFrameModeText(SwitchMode);
	if ( strSwitchText.CompareNoCase(SwitchText) == 0 ) 
	{	return SwitchMode; }
	return AUTO_SWITCH_WND_3D_FRAME_DISABLE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlarmLockText(ALARM_LOCK_MODE Mode)//取得警報鎖住名稱
{
	CString str;
	switch ( Mode )
	{
	case ALARM_LOCK_NONE:	str = m_AlarmLockText_None;	break;
	case ALARM_LOCK_AOI:	str = m_AlarmLockText_AOI;	break;
	case ALARM_LOCK_ARS:	str = m_AlarmLockText_ARS;	break;		
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildAlamLockCombox(CComboBox &Combox)//建立警報鎖住列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	ALARM_LOCK_MODE  DefectFrom=ALARM_LOCK_NONE;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	DefectFrom = ALARM_LOCK_AOI;
	str = GetAlarmLockText(DefectFrom);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DefectFrom);
	idx ++;

	DefectFrom = ALARM_LOCK_ARS;
	str = GetAlarmLockText(DefectFrom);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DefectFrom);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDefectFromText(DEFECT_FROM_MODE DefectFrom)//取得瑕疵來源名稱
{
	CString str;
	switch ( DefectFrom )
	{
	case DEFECT_FROM_NONE:	str = m_DefectFromText_None;	break;
	case DEFECT_FROM_AOI:	str = m_DefectFromText_AOI;	break;
	case DEFECT_FROM_ARS:	str = m_DefectFromText_ARS;	break;		
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDefectFromCombox(CComboBox &Combox)//建立瑕疵來源列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DEFECT_FROM_MODE  DefectFrom=DEFECT_FROM_NONE;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	DefectFrom = DEFECT_FROM_AOI;
	str = GetDefectFromText(DefectFrom);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DefectFrom);
	idx ++;

	DefectFrom = DEFECT_FROM_ARS;
	str = GetDefectFromText(DefectFrom);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DefectFrom);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetTop10ScopeText(TOP10_SCOPE Scope)//取得前十大模式
{
	CString str;
	switch ( Scope )
	{
	case TOP10_SCOPE_MODEL:			str = m_Top10ScopeText_Model;	break;
	case TOP10_SCOPE_PART_NUMBER:	str = m_Top10ScopeText_PartNumber;	break;		
	case TOP10_SCOPE_COMPONENT:		str = m_Top10ScopeText_Component;	break;		
	default:						str = m_UndefinedText; 	break;
	}	
	return str;		
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildTop10ScopeCombox(CComboBox &Combox)//建立前十大來源列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	TOP10_SCOPE  Scope=TOP10_SCOPE_NONE;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Scope = TOP10_SCOPE_COMPONENT;
	str = GetTop10ScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;

	Scope = TOP10_SCOPE_PART_NUMBER;
	str = GetTop10ScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;

	Scope = TOP10_SCOPE_MODEL;
	str = GetTop10ScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetYieldingScopeText(YIELDING_SCOPE Scope)//取得良率來源名稱
{
	CString str;
	switch ( Scope )
	{
	case YIELDING_SCOPE_TEST:	str = m_YieldingScopeText_Test;	break;
	case YIELDING_SCOPE_PANEL:	str = m_YieldingScopeText_Panel;	break;		
	case YIELDING_SCOPE_BOARD:	str = m_YieldingScopeText_Board;	break;		
	case YIELDING_SCOPE_COMPONENT:	str = m_YieldingScopeText_Component;	break;		
	default:						str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildYieldingScopeCombox(CComboBox &Combox)//建立良率來源列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	YIELDING_SCOPE  Scope=YIELDING_SCOPE_NONE;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Scope = YIELDING_SCOPE_TEST;
	str = GetYieldingScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;

	Scope = YIELDING_SCOPE_PANEL;
	str = GetYieldingScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;

	Scope = YIELDING_SCOPE_BOARD;
	str = GetYieldingScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;

	Scope = YIELDING_SCOPE_COMPONENT;
	str = GetYieldingScopeText(Scope);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Scope);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDefectParamFromText(DEFECT_PARAM_FROM_MODE Mode)//取得瑕疵參數來源名稱
{
	CString str;
	switch ( Mode )
	{
	case DEFECT_PARAM_FROM_DISABLE:	str = m_DefectParamFromText_Disable;	break;
	case DEFECT_PARAM_FROM_PROJECT:	str = m_DefectParamFromText_Project;	break;		
	case DEFECT_PARAM_FROM_COMPONENT:	str = m_DefectParamFromText_Component;	break;			
	default:						str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDefectParamFromCombox(CComboBox &Combox)//建立瑕疵參數來源列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DEFECT_PARAM_FROM_MODE  Mode=DEFECT_PARAM_FROM_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	//Mode = DEFECT_PARAM_FROM_DISABLE;
	//str = GetDefectParamFromText(Mode);
	//Combox.InsertString(-1, str);
	//Combox.SetItemData(idx, Mode);
	//idx ++;

	Mode = DEFECT_PARAM_FROM_PROJECT;
	str = GetDefectParamFromText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = DEFECT_PARAM_FROM_COMPONENT;
	str = GetDefectParamFromText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetCpkFromText(CPK_FROM_MODE Mode)//取得Cpk來源名稱
{
	CString str;
	switch ( Mode )
	{
	case CPK_FROM_OFFSET_X:		str = m_CpkFromText_OffsetX;	break;
	case CPK_FROM_OFFSET_Y:		str = m_CpkFromText_OffsetY;	break;		
	case CPK_FROM_SKEW_ANGLE:	str = m_CpkFromText_SkewAngle;	break;			
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildCpkFromCombox(CComboBox &Combox)//建立Cpk來源列表
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	CPK_FROM_MODE  Mode=CPK_FROM_RETURN;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Mode = CPK_FROM_OFFSET_X;
	str = GetCpkFromText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = CPK_FROM_OFFSET_Y;
	str = GetCpkFromText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;

	Mode = CPK_FROM_SKEW_ANGLE;
	str = GetCpkFromText(Mode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Mode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOnlineLaneImageName(LANE_ID LaneID)//取得線上軌道圖檔名稱	
{
	CString str;
	CString Folder = AOIDataCollect.GetAOIDirectory();
	switch ( LaneID )
	{
	case LANE_ID_A:
		str.Format(_T("%s\\%s"), Folder, _T("Lane_A.BMP"));
		break;
	case LANE_ID_B:
		str.Format(_T("%s\\%s"), Folder, _T("Lane_B.BMP"));
		break;	
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetTestResultImageName(TEST_RESULT_ID ResultID)//取得檢測結果圖檔名稱	
{
	CString str;
	CString Folder = AOIDataCollect.GetAOIDirectory();
	switch ( ResultID )
	{
	case TEST_RESULT_NONE:
		str.Format(_T("%s\\%s"), Folder, _T("Result_UnTest.BMP"));
		break;
	case TEST_RESULT_OK:
		str.Format(_T("%s\\%s"), Folder, _T("Result_OK.BMP"));
		break;
	case TEST_RESULT_NG:
		str.Format(_T("%s\\%s"), Folder, _T("Result_NG.BMP"));
		break;
	case TEST_RESULT_FD:
		str.Format(_T("%s\\%s"), Folder, _T("Result_FdNG.BMP"));
		break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMultiLanguageModeText(MULTI_LANGUAGE_MODE Mode)//取得多國語系文字
{
	CString str;
	switch ( Mode )
	{
	case MULTI_LANGUAGE_ENGLISH:		str = m_MultiLanguageText_English;	break;
	case MULTI_LANGUAGE_CHINESE_TRAD:	str = m_MultiLanguageText_ChinTrad;	break;
	case MULTI_LANGUAGE_CHINESE_SIMP:	str = m_MultiLanguageText_ChinSimp;	break;
	case MULTI_LANGUAGE_LOCAL:			str = m_MultiLanguageText_Local;	break;
	default:							str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgTypeText(ALG_TYPE Type)//取得演算法文字
{
	CString str;
	switch ( Type )
	{
	case ALG_BRIGHT_RATIO:	str = m_AlgText_BrightRatio;	break;
	case ALG_OUTER_SHORT:	str = m_AlgText_OuterShort;	break;		
	case ALG_BLOB_COUNT:	str = m_AlgText_BlobCount;	break;			
	case ALG_BODY_TILT:		str = m_AlgText_BodyTilt;	break;			
	case ALG_BARCODE_RECOGNIZE:	str = m_AlgText_BarcodeRecognize;	break;			
	case ALG_OBJECT_MEASURE:	str = m_AlgText_ObjectMeasure;	break;
	case ALG_WIDTH_RATIO:   str = m_AlgText_WidthRatio; break;
	case ALG_HEIGHT:			str = m_AlgText_Resin; break;
	case ALG_WIRE_WIDTH:	str = m_AlgText_WireWidth; break;
	case ALG_COLOR_CODE:	str = m_AlgText_ColorCode; break;
	case ALG_MODEL_MATCH:	str = m_AlgText_ModelMatch;	break;			
	case ALG_IMAGE_MATCH:	str = m_AlgText_ImageMatch;	break;			
	case ALG_CHAR_VERIFY:	str = m_AlgText_CharVerify;	break;			
	case ALG_FD_MATCH:		str = m_AlgText_FdMatch;	break;
	case ALG_EDGE_SEARCH:   str = m_AlgText_EdgeSearch; break;
	case ALG_SHAPE_VERIFY:   str = m_AlgText_ShapeVerify; break;
	case ALG_ANGLE_MEASURE:   str = m_AlgText_AngleMeasure; break;
	case ALG_PIXEL_COMPARE: str = m_AlgText_PixelCompare; break;	
	case ALG_SOLDER_WETTING: str = m_AlgText_SolderWetting; break;
	case ALG_MEASURE_BLACK_GLUE: str = m_AlgText_MeasureBlackGlue; break;
	case ALG_MEASURE_FLUX_AREA: str = m_AlgText_MeasureFluxArea; break;
	case ALG_MEASURE_CPU_PIN: str = m_AlgText_MeasureCpuPin; break;
	case ALG_MEASURE_SIP_DISTANCE: str = m_AlgText_MeasureSIP; break;
	case ALG_MEASURE_CONNECTOR: str = m_AlgText_MeasureConnector; break;
	case ALG_MEASURE_CONNECTOR_PIN: str = m_AlgText_MeasureConnector; break;
	default:				str = m_UndefinedText; 	break;
	}	
	return str;

}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetAlgGroupCompareText() const//取得群組比較文字
{
	return m_AlgText_GroupCompare;
}
//-------------------------------------------------------------------------------------//
ALG_TYPE CAOIDataDefine::FindAlgTypeByText(LPCTSTR AlgText)//依名稱尋找演算法編號
{
	ALG_TYPE AlgType;
	CString  strAlgText;
	std::vector<ALG_TYPE> AlgList;	
	CAlgParam::BuildAlgTypeList(AlgList);
	const size_t Count=AlgList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		AlgType = AlgList[i];
		strAlgText = GetAlgTypeText(AlgType);
		if ( strAlgText.CompareNoCase(AlgText) == 0 ) 
		{	return AlgType; }
	}
	return ALG_EMPTY;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgCalcUnitModeText(ALG_CALC_UNIT_MODE Mode)//取得計算單位模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_CALC_UNIT_ABS:	 str = m_AlgCalcUnitMode_Abs;	break;
	case ALG_CALC_UNIT_DIFF: str = m_AlgCalcUnitMode_Diff;	break;
	case ALG_CALC_UNIT_RATIO: str = m_AlgCalcUnitMode_Ratio;	break;
	default:			 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_CALC_UNIT_MODE CAOIDataDefine::FindAlgCalcUnitModeByText(LPCTSTR Text)//依名稱尋找計算單位模式
{
	CString  strDockText;
	ALG_CALC_UNIT_MODE UnitMode = CALC_UNIT_RETURN;

	UnitMode = ALG_CALC_UNIT_ABS;
	strDockText = GetAlgCalcUnitModeText(UnitMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return UnitMode; }

	UnitMode = ALG_CALC_UNIT_DIFF;
	strDockText = GetAlgCalcUnitModeText(UnitMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return UnitMode; }

	UnitMode = ALG_CALC_UNIT_RATIO;
	strDockText = GetAlgCalcUnitModeText(UnitMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return UnitMode; }
	return ALG_CALC_UNIT_ABS;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgBrightAverageModeText(ALG_BRIGHT_AVERAGE_MODE Mode)//取得亮度平均模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_BRIGHT_AVERAGE_FULL:	 str = m_AlgBrightAverageMode_Full;	break;
	case ALG_BRIGHT_AVERAGE_PARTIAL: str = m_AlgBrightAverageMode_Partial;	break;
	default:						 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_BRIGHT_AVERAGE_MODE CAOIDataDefine::FindAlgBrightAverageModeByText(LPCTSTR Text)//依名稱尋找亮度平均模式
{
	CString  strDockText;
	ALG_BRIGHT_AVERAGE_MODE AveMode = ALG_BRIGHT_AVERAGE_RETURN;

	AveMode = ALG_BRIGHT_AVERAGE_FULL;
	strDockText = GetAlgBrightAverageModeText(AveMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return AveMode; }

	AveMode = ALG_BRIGHT_AVERAGE_PARTIAL;
	strDockText = GetAlgBrightAverageModeText(AveMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return AveMode; }

	return ALG_BRIGHT_AVERAGE_FULL;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgMatchDockModeText(ALG_MATCH_DOCK_MODE Mode)//取得匹配靠邊模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_MATCH_DOCK_DISABLE:	 str = m_AlgMatchDockMode_Disable;	break;
	case ALG_MATCH_DOCK_TO_TIP:		 str = m_AlgMatchDockMode_ToTip;	break;		
	case ALG_MATCH_DOCK_TO_SHOULDER: str = m_AlgMatchDockMode_Shoulder;	break;				
	default:						 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_MATCH_DOCK_MODE CAOIDataDefine::FindAlgMatchDockModeByText(LPCTSTR Text)//依名稱尋找匹配靠邊模式
{
	CString  strDockText;
	ALG_MATCH_DOCK_MODE DockMode = ALG_MATCH_DOCK_DISABLE;

	DockMode = ALG_MATCH_DOCK_DISABLE;
	strDockText = GetAlgMatchDockModeText(DockMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return DockMode; }

	DockMode = ALG_MATCH_DOCK_TO_TIP;
	strDockText = GetAlgMatchDockModeText(DockMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return DockMode; }

	DockMode = ALG_MATCH_DOCK_TO_SHOULDER;
	strDockText = GetAlgMatchDockModeText(DockMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return DockMode; }
	return ALG_MATCH_DOCK_DISABLE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_MODE Mode)//取得物件尺寸計算模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_OBJECT_SIZE_CALC_BOUNDARY:	 str = m_AlgObjectSizeCalcMode_Boundary;	break;
	case ALG_OBJECT_SIZE_CALC_AVERAGE:	 str = m_AlgObjectSizeCalcMode_Average;	break;
	case ALG_OBJECT_SIZE_CALC_AVE_RECT:	 str = m_AlgObjectSizeCalcMode_AveRect;	break;	
	case ALG_OBJECT_SIZE_CALC_BLUR_RECT:	 str = m_AlgObjectSizeCalcMode_BlurRect;	break;
	default:							 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_OBJECT_SIZE_CALC_MODE CAOIDataDefine::FindAlgObjectSizeCalcModeByText(LPCTSTR Text)//依名稱尋找物件尺寸計算模式
{
	CString  strDockText;
	ALG_OBJECT_SIZE_CALC_MODE CalcMode = ALG_OBJECT_SIZE_CALC_RETURN;

	CalcMode = ALG_OBJECT_SIZE_CALC_BOUNDARY;
	strDockText = GetAlgObjectSizeCalcModeText(CalcMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return CalcMode; }

	CalcMode = ALG_OBJECT_SIZE_CALC_AVERAGE;
	strDockText = GetAlgObjectSizeCalcModeText(CalcMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return CalcMode; }

	CalcMode = ALG_OBJECT_SIZE_CALC_AVE_RECT;
	strDockText = GetAlgObjectSizeCalcModeText(CalcMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return CalcMode; }
	
	CalcMode = ALG_OBJECT_SIZE_CALC_BLUR_RECT;
	strDockText = GetAlgObjectSizeCalcModeText(CalcMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return CalcMode; }
	
	return ALG_OBJECT_SIZE_CALC_BOUNDARY;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgObjectHeightAverageModeText(ALG_OBJECT_HEIGHT_AVERAGE_MODE Mode)//取得物件高度平均模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_OBJECT_HEIGHT_AVERAGE_FULL:	 str = m_AlgObjectHeightAverageMode_Full;	break;
	case ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL: str = m_AlgObjectHeightAverageMode_Partial;	break;
	default:						 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_OBJECT_HEIGHT_AVERAGE_MODE CAOIDataDefine::FindAlgObjectHeightAverageModeByText(LPCTSTR Text)//依名稱尋找物件高度平均模式
{
	CString  strDockText;
	ALG_OBJECT_HEIGHT_AVERAGE_MODE AveMode = ALG_VOLUME_HEIGHT_AVERAGE_RETURN;

	AveMode = ALG_OBJECT_HEIGHT_AVERAGE_FULL;
	strDockText = GetAlgObjectHeightAverageModeText(AveMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return AveMode; }

	AveMode = ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL;
	strDockText = GetAlgObjectHeightAverageModeText(AveMode);
	if ( strDockText.CompareNoCase(Text) == 0 ) 
	{	return AveMode; }
	return ALG_OBJECT_HEIGHT_AVERAGE_FULL;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgAngleMeasureAngleModeText(ANGLE_MEASURE_MODE Mode)//取得角度量測角度模式文字
{
	CString str;
	switch ( Mode )
	{
	case ANGLE_MEASURE_SKEW:	str = m_AlgAngleMeasureAngleMode_Skew;	break;
	case ANGLE_MEASURE_TILT:	str = m_AlgAngleMeasureAngleMode_Tilt;	break;	
	default:					str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
ANGLE_MEASURE_MODE CAOIDataDefine::FindAlgAngleMeasureAngleModeByText(LPCTSTR Text)//依名稱尋找角度量角度模式
{
	CString  str;
	ANGLE_MEASURE_MODE Mode = ANGLE_MEASURE_RETURN;

	Mode = ANGLE_MEASURE_SKEW;
	str = GetAlgAngleMeasureAngleModeText(Mode);
	if ( str.CompareNoCase(Text) == 0 ) 
	{	return Mode; }

	Mode = ANGLE_MEASURE_TILT;
	str = GetAlgAngleMeasureAngleModeText(Mode);
	if ( str.CompareNoCase(Text) == 0 ) 
	{	return Mode; }

	return ANGLE_MEASURE_SKEW;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgAngleMeasureLineEqnModeText(LINE_EQUATION_MODE Mode)//取得角度量測基準線模式文字
{
	CString str;
	switch ( Mode )
	{
	case LINE_EQUATION_CALC: str = m_AlgAngleMeasureBaseLineMode_Calc;	break;
	case LINE_EQUATION_HOR:  str = m_AlgAngleMeasureBaseLineMode_Hor;	break;
	case LINE_EQUATION_VER:  str = m_AlgAngleMeasureBaseLineMode_Ver;	break;
	default:				 str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
LINE_EQUATION_MODE CAOIDataDefine::FindAlgAngleMeasureLineEqnModeByText(LPCTSTR Text)//依名稱尋找角度量測基準線模式
{
	CString  str;
	LINE_EQUATION_MODE Mode = LINE_EQUATION_RETURN;

	Mode = LINE_EQUATION_CALC;
	str = GetAlgAngleMeasureLineEqnModeText(Mode);
	if ( str.CompareNoCase(Text) == 0 ) 
	{	return Mode; }

	Mode = LINE_EQUATION_HOR;
	str = GetAlgAngleMeasureLineEqnModeText(Mode);
	if ( str.CompareNoCase(Text) == 0 ) 
	{	return Mode; }

	Mode = LINE_EQUATION_VER;
	str = GetAlgAngleMeasureLineEqnModeText(Mode);
	if ( str.CompareNoCase(Text) == 0 ) 
	{	return Mode; }

	return Mode;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetResultIDText(RESULT_ID ResultID)//取得演算法結果文字
{
	CString str;
	switch ( ResultID )
	{
	case RESULT_ID_NONE:	str = m_ResultText_None;	break;
	case RESULT_ID_OK:		str = m_ResultText_OK;	break;		
	case RESULT_ID_NG:		str = m_ResultText_NG;	break;			
	case RESULT_ID_SKIP:	str = m_ResultText_Skip;	break;			
	case RESULT_ID_BYPASS:	str = m_ResultText_Bypass;	break;
	case RESULT_ID_EXCEPTION:	str = m_ResultText_Exception;	break;			
	default:					str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAOIGroupModeText(PART_GROUP_MODE Mode)//取得AOI群組模式文字
{
	CString str;
	switch ( Mode )
	{
	case PART_GROUP_COLINEARITY:         str = m_PartGroupText_Colinearity;	break;	
	case PART_GROUP_COLINEARITY_TO_LINE: str = m_PartGroupText_ColinearityToLine;	break;
	case PART_GROUP_DIST_PART_TO_PART:   str = m_PartGroupText_DistPartToPart; break;
	case PART_GROUP_DIST_PART_NEIGHBOR:  str = m_PartGroupText_DistPartNeighbor; break;
	case PART_GROUP_DIST_PART_TO_GROUP:  str = m_PartGroupText_DistPartToGroup; break;
	case PART_GROUP_DIST_GROUP_TO_PART:  str = m_PartGroupText_DistGroupToPart;	break;
	case PART_GROUP_DIST_GROUP_COORD_MAP:str = m_PartGroupText_DistGroupCoordMap;	break;
	default:							 str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildAOIGroupModeCombox(CComboBox &Combox)//建立群組模式列表
{
	CString text;
	int      index = 0;
	PART_GROUP_MODE GroupMode;
	JetAPI::ClearCombox(Combox);
	
	GroupMode = PART_GROUP_COLINEARITY;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;	

	GroupMode = PART_GROUP_COLINEARITY_TO_LINE;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;	

	GroupMode = PART_GROUP_DIST_PART_TO_PART;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;	

	GroupMode = PART_GROUP_DIST_PART_NEIGHBOR;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;	

	GroupMode = PART_GROUP_DIST_PART_TO_GROUP;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;	

	GroupMode = PART_GROUP_DIST_GROUP_TO_PART;
	text = GetAOIGroupModeText(GroupMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, GroupMode); index ++;		

	Combox.SetCurSel(0);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgImageSourceModeText(IMAGE_SRC_MODE Mode)//取得演算法影像來源文字
{
	CString str;
	switch ( Mode )
	{
	case IMAGE_SRC_GRAY:	str = m_ImageSrcText_Gray;	break;
	case IMAGE_SRC_COLOR:	str = m_ImageSrcText_Color;	break;		
	case IMAGE_SRC_RED:		str = m_ImageSrcText_Red;	break;			
	case IMAGE_SRC_GREEN:	str = m_ImageSrcText_Green;	break;			
	case IMAGE_SRC_BLUE:	str = m_ImageSrcText_Blue;	break;			
	case IMAGE_SRC_LIGHTNESS:	str = m_ImageSrcText_Lightness;	break;
	case IMAGE_SRC_SYNTHESIS:	str = m_ImageSrcText_Synthesis;	break;
	case IMAGE_SRC_DARKNESS:	str = m_ImageSrcText_Darkness;	break;
	case IMAGE_SRC_SATURATION:	str = m_ImageSrcText_Saturation;	break;		
	case IMAGE_SRC_RED_RATIO:	str = m_ImageSrcText_RedRatio;	break;	
	case IMAGE_SRC_GREEN_RATIO:	str = m_ImageSrcText_GreenRatio;	break;	
	case IMAGE_SRC_BLUE_RATIO:	str = m_ImageSrcText_BlueRatio;	break;
	case IMAGE_SRC_MAX_GRN_BLU:	str = m_ImageSrcText_MaxGrnBlu;	break;
	default:					str = m_UndefinedText; 	break;
	}		
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildImageSourceModeCombox(CComboBox &Combox, FRAME_TYPE FrameType)
{
	CString text;
	int      index = 0;
	IMAGE_SRC_MODE ImageSource;
	JetAPI::ClearCombox(Combox);

	if ( FRAME_COLOR==FrameType || FRAME_BAYER==FrameType )
	{
		ImageSource = IMAGE_SRC_COLOR;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;	

		ImageSource = IMAGE_SRC_GRAY;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;	

		ImageSource = IMAGE_SRC_RED;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_GREEN;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_BLUE;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_LIGHTNESS;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_DARKNESS;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;		

		ImageSource = IMAGE_SRC_SATURATION;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_SYNTHESIS;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;
		
		ImageSource = IMAGE_SRC_RED_RATIO;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_GREEN_RATIO;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;

		ImageSource = IMAGE_SRC_BLUE_RATIO;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;			

		ImageSource = IMAGE_SRC_MAX_GRN_BLU;
		text = GetAlgImageSourceModeText(ImageSource);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;		
	}
	else 
	{
		ImageSource = IMAGE_SRC_GRAY;
		if ( FRAME_SPACE == FrameType )
		{	text = GetThicknessText();	}
		else
		{	text = GetAlgImageSourceModeText(ImageSource);	}
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ImageSource); index ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgMaskFuncModeText(MASK_FUNC_MODE Mode)//取得遮罩功能模式文字
{
	CString str;
	switch ( Mode )
	{
	case MASK_FUNC_CALC:	str = m_MaskFuncText_Calc;	break;
	case MASK_FUNC_ERASE:	str = m_MaskFuncText_Erase;	break;
	default:				str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
MASK_FUNC_MODE CAOIDataDefine::FindAlgMaskFuncModeByText(LPCTSTR Text)//取得遮罩功能模式
{
	CString  strText;
	MASK_FUNC_MODE  MaskMode;

	MaskMode = MASK_FUNC_CALC;
	strText = GetAlgMaskFuncModeText(MaskMode);
	if ( strText.CompareNoCase(Text) == 0 ) 
	{	return MaskMode; }

	MaskMode = MASK_FUNC_ERASE;
	strText = GetAlgMaskFuncModeText(MaskMode);
	if ( strText.CompareNoCase(Text) == 0 ) 
	{	return MaskMode; }
	return MASK_FUNC_RETURN;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgBinaryModeText(BINARY_MODE Mode)//取得二值化模式文字
{
	CString str;
	switch ( Mode )
	{
	case BINARY_DISABLE:				str = m_BinaryText_Disable;	break;
	case BINARY_COLOR_FILTER:			str = m_BinaryText_ColorFilter;	break;		
	case BINARY_FIXED_THRESHOLD:		str = m_BinaryText_FixedTh;	break;			
	case BINARY_DYNAMIC_THRESHOLD:		str = m_BinaryText_DynamicTh;	break;			
	case BINARY_RELATIVE_AVE_THRESHOLD:	str = m_BinaryText_RelativeTh;	break;	
	case BINARY_ADAPTIVE_THRESHOLD:		str = m_BinaryText_AdaptiveTh;	break;			
	default:							str = m_UndefinedText; 	break;
	}	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBinaryModeCombox(CComboBox &Combox, FRAME_TYPE FrameType)
{
	CString text;
	int      index = 0;
	BINARY_MODE BinaryMode;
	JetAPI::ClearCombox(Combox);

	BinaryMode = BINARY_DISABLE;
	text = GetAlgBinaryModeText(BinaryMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, BinaryMode); index ++;

	if ( FRAME_COLOR==FrameType || FRAME_BAYER==FrameType )
	{
		BinaryMode = BINARY_COLOR_FILTER;
		text = GetAlgBinaryModeText(BinaryMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, BinaryMode); index ++;
	}

	BinaryMode = BINARY_FIXED_THRESHOLD;
	text = GetAlgBinaryModeText(BinaryMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, BinaryMode); index ++;

	if ( FRAME_SPACE != FrameType )
	{
		BinaryMode = BINARY_DYNAMIC_THRESHOLD;
		text = GetAlgBinaryModeText(BinaryMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, BinaryMode); index ++;
	}

	BinaryMode = BINARY_RELATIVE_AVE_THRESHOLD;
	text = GetAlgBinaryModeText(BinaryMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, BinaryMode); index ++;

	//if ( FRAME_SPACE != FrameType )
	{
		BinaryMode = BINARY_ADAPTIVE_THRESHOLD;
		text = GetAlgBinaryModeText(BinaryMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, BinaryMode); index ++;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgEdgeEnhanceModeText(EDGE_ENHANCE_MODE Mode)//取得邊緣強化模式文字
{
	CString str;
	switch ( Mode )
	{
	case EDGE_ENHANCE_DISABLE:			str = m_EdgeEnhanceText_Disable;	break;
	case EDGE_ENHANCE_SOBEL:			str = m_EdgeEnhanceText_Sobel;	break;			
	case EDGE_ENHANCE_DARK_TOP:			str = m_EdgeEnhanceText_DarkTop;	break;
	case EDGE_ENHANCE_DARK_LEFT:		str = m_EdgeEnhanceText_DarkLeft;	break;
	case EDGE_ENHANCE_DARK_BOT:			str = m_EdgeEnhanceText_DarkBot;	break;
	case EDGE_ENHANCE_DARK_RIGHT:		str = m_EdgeEnhanceText_DarkRight;	break;
	default:							str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildEdgeEnhanceModeCombox(CComboBox &Combox)
{
	CString text;
	int      index = 0;
	EDGE_ENHANCE_MODE EdgeMode;	
	std::vector<EDGE_ENHANCE_MODE> List;
	JetAPI::ClearCombox(Combox);
	List.push_back(EDGE_ENHANCE_DISABLE);
	List.push_back(EDGE_ENHANCE_SOBEL);
	List.push_back(EDGE_ENHANCE_DARK_TOP);
	List.push_back(EDGE_ENHANCE_DARK_LEFT);
	List.push_back(EDGE_ENHANCE_DARK_BOT);
	List.push_back(EDGE_ENHANCE_DARK_RIGHT);

	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		EdgeMode = List[i];
		text = GetAlgEdgeEnhanceModeText(EdgeMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, EdgeMode); index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildEdgeEnhanceFilterModeCombox(CComboBox &Combox)
{
	CString text;
	int      index = 0;
	NOISE_FILTER_MODE Mode;
	JetAPI::ClearCombox(Combox);
	
	Mode = NOISE_FILTER_DISABLE;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;

	Mode = NOISE_FILTER_OPEN;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;

	Mode = NOISE_FILTER_CLOSE;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;
	
	Mode = NOISE_FILTER_EROSION;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;

	Mode = NOISE_FILTER_DILATION;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;

	Mode = NOISE_FILTER_GRADIENT;
	text = GetAlgNoiseFilterModeText(Mode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, Mode); index ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgColorRGBVModeText(COLOR_RGBV_MODE Mode)//取得彩色-RGBV模式文字
{
	CString str;
	switch ( Mode )
	{
	case COLOR_RGBV_NONE:
		break;
	case COLOR_RGBV_RED:	str = GetColorText_Red();	break;
	case COLOR_RGBV_GREEN:	str = GetColorText_Green();	break;
	case COLOR_RGBV_BLUE:	 str = GetColorText_Blue();	break;
	case COLOR_RGBV_VALUE:	 str = GetColorText_Gray();	break;	
	default:
		str = m_UndefinedText;
		break;

	}
	return str;
}
//-------------------------------------------------------------------------------------//	
CString CAOIDataDefine::GetAlgNoiseFilterModeText(NOISE_FILTER_MODE Mode)//取得雜訊過濾文字
{
	CString str;
	switch ( Mode )
	{
	case NOISE_FILTER_DISABLE:	str = m_NoiseFilterText_Diable;	break;		
	case NOISE_FILTER_SMOOTH:	str = m_NoiseFilterText_Smooth;	break;		
	case NOISE_FILTER_MEDIAN:	str = m_NoiseFilterText_Median;	break;			
	case NOISE_FILTER_OPEN:		str = m_NoiseFilterText_Open;	break;			
	case NOISE_FILTER_CLOSE:	str = m_NoiseFilterText_Close;	break;
	case NOISE_FILTER_3LEVEL:	str = m_NoiseFilterText_Level;	break;
	case NOISE_FILTER_MEDIAN2:	str = m_NoiseFilterText_Median2;	break;
	case NOISE_FILTER_PYRAMID_MEDIAN: str = m_NoiseFilterText_PyramidMedian; break;
	case NOISE_FILTER_CONTENTAWARE: str = m_NoiseFilterText_ContentAware; break;
	case NOISE_FILTER_FAST_MEDIAN: str = m_NoiseFilterText_Fast_Median; break;
	case NOISE_FILTER_FAST_AVERAGE: str = m_NoiseFilterText_Fast_Average; break;
	case NOISE_FILTER_EROSION: str = m_NoiseFilterText_Erosion; break;
	case NOISE_FILTER_DILATION: str = m_NoiseFilterText_Dilation; break;
	case NOISE_FILTER_GRADIENT: str = m_NoiseFilterText_Gradient; break;
	default:					str = m_UndefinedText; 	break;
	}	
	return str;		
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildNoiseFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType)
{
	CString text;
	int      index = 0;
	NOISE_FILTER_MODE FilterMode;
	JetAPI::ClearCombox(Combox);

	FilterMode = NOISE_FILTER_DISABLE;
	text = GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, FilterMode); index ++;

	if ( FRAME_SPACE != FrameType )
	{
		FilterMode = NOISE_FILTER_SMOOTH;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_MEDIAN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_OPEN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_CLOSE;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildGrayFilterParamCombox(CComboBox &Combox)
{
	CString text;
	int      i=0;
	int      Data=0;
	int      index=0;
	JetAPI::ClearCombox(Combox);

	Data = 3;
	for ( i=0; i<9; i++ )
	{	
		text.Format(_T("%.2dx%.2d"), Data, Data);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, Data); index ++;

		Data += 2;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildGrayFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType)
{
	CString text;
	int      index = 0;
	NOISE_FILTER_MODE FilterMode;
	JetAPI::ClearCombox(Combox);

	FilterMode = NOISE_FILTER_DISABLE;
	text = GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, FilterMode); index ++;

	//if ( FRAME_SPACE != FrameType )
	{
		FilterMode = NOISE_FILTER_SMOOTH;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_MEDIAN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_OPEN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		//Combox.InsertString(-1, text);//-1表示加在最後面
		//Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_CLOSE;
		text = GetAlgNoiseFilterModeText(FilterMode);
		//Combox.InsertString(-1, text);//-1表示加在最後面
		//Combox.SetItemData(index, FilterMode); index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBinaryFilterParamCombox(CComboBox &Combox)
{
	CString text;
	int      i=0;
	int      Data=0;
	int      index=0;
	JetAPI::ClearCombox(Combox);

	Data = 3;
	for ( i=0; i<9; i++ )
	{	
		text.Format(_T("%.2dx%.2d"), Data, Data);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, Data); index ++;

		Data += 2;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBinaryFilterModeCombox(CComboBox &Combox, FRAME_TYPE FrameType)
{
	CString text;
	int      index = 0;
	NOISE_FILTER_MODE FilterMode;
	JetAPI::ClearCombox(Combox);

	FilterMode = NOISE_FILTER_DISABLE;
	text = GetAlgNoiseFilterModeText(FilterMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, FilterMode); index ++;

	//if ( FRAME_SPACE != FrameType )
	{
		FilterMode = NOISE_FILTER_SMOOTH;
		text = GetAlgNoiseFilterModeText(FilterMode);
		//Combox.InsertString(-1, text);//-1表示加在最後面
		//Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_MEDIAN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		//Combox.InsertString(-1, text);//-1表示加在最後面
		//Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_OPEN;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_CLOSE;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_EROSION;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;

		FilterMode = NOISE_FILTER_DILATION;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index ++;		
	}

	if ( FRAME_SPACE == FrameType ) 
	{
		FilterMode = NOISE_FILTER_GRADIENT;
		text = GetAlgNoiseFilterModeText(FilterMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, FilterMode); index++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgBrightLineModeText(int Mode)//取得演算法亮度貫穿模式
{
	CString str;
	switch ( Mode )
	{
	case LINE_MODE_BRIGHT:	str = m_AlgBrightLineMode_Bright;	break;
	case LINE_MODE_DARK:	str = m_AlgBrightLineMode_Dark;		break;			
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindAlgBrightLineModeByText(LPCTSTR DirText)
{
	CString  strDirText;
	int      LineMode = 0;

	LineMode = LINE_MODE_BRIGHT;
	strDirText = GetAlgBrightLineModeText(LineMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return LineMode; }

	LineMode = LINE_MODE_DARK;
	strDirText = GetAlgBrightLineModeText(LineMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return LineMode; }

	return 0;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_MODE Mode)//取得演算法外接短路延伸模式
{
	CString str;
	switch ( Mode )
	{
	case ALG_OUTER_SHORT_EXT_NONE:	str = m_AlgOuterShortExtendText_None;	break;
	case ALG_OUTER_SHORT_EXT_LEFT:	str = m_AlgOuterShortExtendText_Left;		break;	
	case ALG_OUTER_SHORT_EXT_RIGHT:	str = m_AlgOuterShortExtendText_Right;		break;	
	case ALG_OUTER_SHORT_EXT_BOTH:	str = m_AlgOuterShortExtendText_Both;		break;	
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_OUTER_SHORT_EXT_MODE CAOIDataDefine::FindAlgOuterShortExtendModeByText(LPCTSTR DirText)
{
	CString  strDirText;
	ALG_OUTER_SHORT_EXT_MODE  ExtMode;

	ExtMode = ALG_OUTER_SHORT_EXT_NONE;
	strDirText = GetAlgOuterShortExtendModeText(ExtMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return ExtMode; }

	ExtMode = ALG_OUTER_SHORT_EXT_LEFT;
	strDirText = GetAlgOuterShortExtendModeText(ExtMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return ExtMode; }

	ExtMode = ALG_OUTER_SHORT_EXT_RIGHT;
	strDirText = GetAlgOuterShortExtendModeText(ExtMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return ExtMode; }

	ExtMode = ALG_OUTER_SHORT_EXT_BOTH;
	strDirText = GetAlgOuterShortExtendModeText(ExtMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return ExtMode; }
	return ALG_OUTER_SHORT_EXT_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgDirectionText(ALG_DIRECTION Direction)//取得演算法方向文字
{
	CString str;
	switch ( Direction )
	{
	case ALG_HORIZONTAL:	str = m_AlgDirText_Hor;	break;
	case ALG_VERTICAL:		str = m_AlgDirText_Ver;	break;			
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_DIRECTION CAOIDataDefine::FindAlgDirectionByText(LPCTSTR DirText)
{
	CString  strDirText;
	ALG_DIRECTION Dirtion = ALG_DIR_NONE;

	Dirtion = ALG_HORIZONTAL;
	strDirText = GetAlgDirectionText(Dirtion);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return Dirtion; }

	Dirtion = ALG_VERTICAL;
	strDirText = GetAlgDirectionText(Dirtion);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return Dirtion; }

	return ALG_DIR_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgDirectionXYText(ALG_DIRECTION Direction)
{
	CString str;
	switch ( Direction )
	{
	case ALG_HORIZONTAL:	str = _T("X");	break;
	case ALG_VERTICAL:		str = _T("Y");	break;
	default:				str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_DIRECTION CAOIDataDefine::FindAlgDirectionByXYText(LPCTSTR DirText)
{
	CString  strDirText;
	ALG_DIRECTION Dirtion = ALG_DIR_NONE;

	Dirtion = ALG_HORIZONTAL;
	strDirText = GetAlgDirectionXYText(Dirtion);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return Dirtion; }

	Dirtion = ALG_VERTICAL;
	strDirText = GetAlgDirectionXYText(Dirtion);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return Dirtion; }

	return ALG_DIR_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgBarcodeDecodeStepText(ALG_BARCODE_STEP_MODE Mode)//取得條碼步驟模式文字
{
	CString str;
	switch ( Mode )
	{
	case ALG_BARCODE_STEP_NONE:		str = m_AlgBarcodeStepText_None;	break;
	case ALG_BARCODE_STEP_SCALE:	str = m_AlgBarcodeStepText_Scale;	break;
	case ALG_BARCODE_STEP_GAIN_OFFSET:	str = m_AlgBarcodeStepText_GainOffset;	break;
	case ALG_BARCODE_STEP_SMOOTH:	str = m_AlgBarcodeStepText_Smooth;	break;
	case ALG_BARCODE_STEP_OPEN:		str = m_AlgBarcodeStepText_Open;	break;
	case ALG_BARCODE_STEP_CLOSE:	str = m_AlgBarcodeStepText_Close;	break;
	case ALG_BARCODE_STEP_MEDIAN:	str = m_AlgBarcodeStepText_Median;	break;
	case ALG_BARCODE_STEP_INVERT:	str = m_AlgBarcodeStepText_Invert;	break;
	case ALG_BARCODE_STEP_FLIP:		str = m_AlgBarcodeStepText_Flip;	break;	
	case ALG_BARCODE_STEP_FILL:		str = m_AlgBarcodeStepText_Fill;	break;
	case ALG_BARCODE_STEP_ERODE:	str = m_AlgBarcodeStepText_Erode;	break;
	case ALG_BARCODE_STEP_DILATE:	str = m_AlgBarcodeStepText_Dilate;	break;
	case ALG_BARCODE_STEP_FILL_2D:	str = m_AlgBarcodeStepText_Fill2D;	break;
	case ALG_BARCODE_STEP_SHARP:	str = m_AlgBarcodeStepText_Sharp;	break;
	case ALG_BARCODE_STEP_GRAY_RANGE:	str = m_AlgBarcodeStepText_Range;	break;
	default:						str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
ALG_BARCODE_STEP_MODE CAOIDataDefine::FindAlgBarcodeStepModeByText(LPCTSTR StepText)//取得條碼解碼步驟模式
{
	CString     strStep;	
	std::vector<ALG_BARCODE_STEP_MODE> DecodeList;
	CAlgParam::BuildBarcodeDecodeStepList(DecodeList);
	const size_t DecodeCount=DecodeList.size();
	ALG_BARCODE_STEP_MODE DecodeMode=ALG_BARCODE_STEP_NONE;
	for ( size_t i=0; i<DecodeCount; i++ )
	{
		DecodeMode = DecodeList[i];
		strStep = GetAlgBarcodeDecodeStepText(DecodeMode);
		if ( strStep == StepText ) { return DecodeMode; }
	}
	return ALG_BARCODE_STEP_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_MODE Mode, int index)//取得條碼步驟參數文字
{
	CString str;		
	str = m_AlgBarcodeStepText_None;	
	switch ( Mode )
	{
	case ALG_BARCODE_STEP_NONE:		
		str = m_AlgBarcodeStepText_None;		
		break;
	case ALG_BARCODE_STEP_SCALE:		
		switch ( index )
		{
		case 0:			
			str = GetRatioText();
			break;		
		}		
		break;
	case ALG_BARCODE_STEP_GAIN_OFFSET:
		switch ( index )
		{
		case 0:
			str = GetGainText();
			break;		
		case 1:
			str = GetOffsetText();//_T("平移")
			break;		
		}
		break;
	case ALG_BARCODE_STEP_SMOOTH:
		switch ( index )
		{
		case 0:
			str = GetSizeText();//_T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_OPEN:
		switch ( index )
		{
		case 0:
			str = GetSizeText();// _T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_CLOSE:
		switch ( index )
		{
		case 0:
			str = GetSizeText();//_T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_MEDIAN:
		switch ( index )
		{
		case 0:			
			str = GetSizeText();//_T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_INVERT:		
		break;
	case ALG_BARCODE_STEP_FLIP:
		break;
	case ALG_BARCODE_STEP_FILL:
	case ALG_BARCODE_STEP_FILL_2D:
		switch ( index )
		{
		case 0:			
			str = GetSizeText();//_T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_ERODE:
		switch ( index )
		{
		case 0:
			str = GetSizeText();// _T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_DILATE:
		switch ( index )
		{
		case 0:
			str = GetSizeText();//_T("遮罩大小");	
			break;		
		}
		break;
	case ALG_BARCODE_STEP_SHARP:
		break;
	case ALG_BARCODE_STEP_GRAY_RANGE:
		switch ( index )
		{
		case 0: str = GetMinText();	break;		
		case 1: str = GetMaxText();	break;		
		}
		break;
	default:	
		str = m_UndefinedText;
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgFdMatchModeText(FD_MATCH_MODE MatchMode)//取得演算法定位點匹配模式文字
{
	CString str;
	switch ( MatchMode )
	{
	case FD_MATCH_MODEL:	str = m_AlgFdMatchText_Model;	break;
	case FD_MATCH_IMAGE:	str = m_AlgFdMatchText_Image;	break;	
	default:				str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
FD_MATCH_MODE CAOIDataDefine::FindAlgFdMatchModeByText(LPCTSTR ModeText)
{
	CString  strModeText;
	FD_MATCH_MODE MatchMode = FD_MATCH_MODEL;

	MatchMode = FD_MATCH_MODEL;
	strModeText = GetAlgFdMatchModeText(MatchMode);
	if ( strModeText.CompareNoCase(ModeText) == 0 ) 
	{	return MatchMode; }

	MatchMode = FD_MATCH_IMAGE;
	strModeText = GetAlgFdMatchModeText(MatchMode);
	if ( strModeText.CompareNoCase(ModeText) == 0 ) 
	{	return MatchMode; }

	return FD_MATCH_IMAGE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgGroupCompareDirText(ALG_GROUP_CMP_DIR_MODE DirMode)//取得演算法群組比較方向文字
{
	CString str;
	switch ( DirMode )
	{
	case ALG_GROUP_CMP_DIR_ANY:	str=m_AlgGroupCompareDirText_Any;	break;
	case ALG_GROUP_CMP_DIR_ONE:	str=m_AlgGroupCompareDirText_One;	break;	
	default:						str=m_UndefinedText;	break;
	}
	return str;	 
}
//-------------------------------------------------------------------------------------//
ALG_GROUP_CMP_DIR_MODE CAOIDataDefine::FindAlgGroupCompareDirByText(LPCTSTR DirText)
{
	CString  strDirText;
	ALG_GROUP_CMP_DIR_MODE DirMode;

	DirMode = ALG_GROUP_CMP_DIR_ANY;
	strDirText = GetAlgGroupCompareDirText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	DirMode = ALG_GROUP_CMP_DIR_ONE;
	strDirText = GetAlgGroupCompareDirText(DirMode);
	if ( strDirText.CompareNoCase(DirText) == 0 ) 
	{	return DirMode; }

	return ALG_GROUP_CMP_DIR_ANY;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlg3DHeightBaseText(ALG_3D_BASE_HEIGHT_MODE BaseMode)//取得演算法3D高度基本模式文字
{
	CString str;
	switch ( BaseMode )
	{
	case ALG_3D_BASE_HEIGHT_MIN:	str=m_Alg3DHeightBaseText_Min;	break;
	case ALG_3D_BASE_HEIGHT_MAX:	str=m_Alg3DHeightBaseText_Max;	break;
	case ALG_3D_BASE_HEIGHT_AVE:	str=m_Alg3DHeightBaseText_Ave;	break;
	case ALG_3D_BASE_HEIGHT_MID:	str=m_Alg3DHeightBaseText_Mid;	break;
	case ALG_3D_BASE_HEIGHT_SQR:	str=m_Alg3DHeightBaseText_SQR;	break;
	default:						str=m_UndefinedText;	break;
	}
	return str;	 
}
//-------------------------------------------------------------------------------------//
ALG_3D_BASE_HEIGHT_MODE CAOIDataDefine::FindAlg3DHeightBaseModeByText(LPCTSTR BaseText) 
{
	CString  strBaseText;
	ALG_3D_BASE_HEIGHT_MODE BaseMode;

	BaseMode = ALG_3D_BASE_HEIGHT_MIN;
	strBaseText = GetAlg3DHeightBaseText(BaseMode);
	if ( strBaseText.CompareNoCase(BaseText) == 0 ) 
	{	return BaseMode; }

	BaseMode = ALG_3D_BASE_HEIGHT_MAX;
	strBaseText = GetAlg3DHeightBaseText(BaseMode);
	if ( strBaseText.CompareNoCase(BaseText) == 0 ) 
	{	return BaseMode; }

	BaseMode = ALG_3D_BASE_HEIGHT_AVE;
	strBaseText = GetAlg3DHeightBaseText(BaseMode);
	if ( strBaseText.CompareNoCase(BaseText) == 0 ) 
	{	return BaseMode; }

	BaseMode = ALG_3D_BASE_HEIGHT_MID;
	strBaseText = GetAlg3DHeightBaseText(BaseMode);
	if ( strBaseText.CompareNoCase(BaseText) == 0 ) 
	{	return BaseMode; }

	BaseMode = ALG_3D_BASE_HEIGHT_SQR;
	strBaseText = GetAlg3DHeightBaseText(BaseMode);
	if ( strBaseText.CompareNoCase(BaseText) == 0 ) 
	{	return BaseMode; }
	return ALG_3D_BASE_HEIGHT_MIN;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgSearchDirectionText(ALG_SEARCH_DIRECTION Direction)//取得演算法搜尋方向文字
{
	CString str;
	switch ( Direction )
	{
	case SEARCH_DIRECTION_FORWARD:	str=m_AlgSearchDirectionText_Forward;	break;
	case SEARCH_DIRECTION_BACKWARD:	str=m_AlgSearchDirectionText_Backward;	break;	
	default:						str=m_UndefinedText;	break;
	}
	return str;	 
}
//-------------------------------------------------------------------------------------//
ALG_SEARCH_DIRECTION  CAOIDataDefine::FindAlgSearchDirectionByText(LPCTSTR DirText)
{
	CString  strDirectText;
	ALG_SEARCH_DIRECTION Direction;

	Direction = SEARCH_DIRECTION_FORWARD;
	strDirectText = GetAlgSearchDirectionText(Direction);
	if ( strDirectText.CompareNoCase(DirText) == 0 ) 
	{	return Direction; }

	Direction = SEARCH_DIRECTION_BACKWARD;
	strDirectText = GetAlgSearchDirectionText(Direction);
	if ( strDirectText.CompareNoCase(DirText) == 0 ) 
	{	return Direction; }

	return SEARCH_DIRECTION_FORWARD;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgEdgeFeatureText(ALG_EDGE_FEATURE_MODE EdgeMode)//取得演算法邊緣特徵文字
{
	CString str;
	switch ( EdgeMode )
	{
	case ALG_EDGE_FEATURE_W2B:	str=m_AlgEdgeFeatureText_W2B;	break;
	case ALG_EDGE_FEATURE_B2W:	str=m_AlgEdgeFeatureText_B2W;	break;	
	default:					str=m_UndefinedText;	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
ALG_EDGE_FEATURE_MODE CAOIDataDefine::FindAlgEdgeFeatureModeByText(LPCTSTR FeatureText)
{
	CString  strFeatureText;
	ALG_EDGE_FEATURE_MODE EdgeMode;

	EdgeMode = ALG_EDGE_FEATURE_W2B;
	strFeatureText = GetAlgEdgeFeatureText(EdgeMode);
	if ( strFeatureText.CompareNoCase(FeatureText) == 0 ) 
	{	return EdgeMode; }

	EdgeMode = ALG_EDGE_FEATURE_B2W;
	strFeatureText = GetAlgEdgeFeatureText(EdgeMode);
	if ( strFeatureText.CompareNoCase(FeatureText) == 0 ) 
	{	return EdgeMode; }

	return ALG_EDGE_FEATURE_W2B;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgHeightDetectionTypeText(int type)//取得演算法
{
	CString str;
	switch (type)
	{
	case 1:	str = m_AlgHeightDetectionType1;	break;
	case 2:	str = m_AlgHeightDetectionType2;	break;
	default:					str = m_AlgHeightDetectionType1;	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindAlgHeightDetectionTypeByText(LPCTSTR FeatureText)
{
	CString  strFeatureText;

	strFeatureText = GetAlgHeightDetectionTypeText(1);
	if (strFeatureText.CompareNoCase(FeatureText) == 0)
	{
		return 1;
	}

	strFeatureText = GetAlgHeightDetectionTypeText(2);
	if (strFeatureText.CompareNoCase(FeatureText) == 0)
	{
		return 2;
	}

	return 1;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgHeightDetectionDirectionText(int type)//取得演算法
{
	CString str;
	switch (type)
	{
	case 1:	str = m_BarcodeDirectionModeText_Ver;	break;
	case 2:	str = m_BarcodeDirectionModeText_Hor;	break;
	case 3:	str = m_BarcodeDirectionModeText_Ver + m_BarcodeDirectionModeText_Hor;	break;
	case 4:	str = m_CalcBasePlaneText_Corner;	break;
	case 5:	str = m_BarcodeDirectionModeText_All;	break;
	default:					str = m_BarcodeDirectionModeText_Ver;	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindAlgHeightDetectionDirectionByText(LPCTSTR FeatureText)
{
	CString  strFeatureText;

	for (int i = 1; i <= 5; i++)
	{
		strFeatureText = GetAlgHeightDetectionDirectionText(i);
		if (strFeatureText.CompareNoCase(FeatureText) == 0)
		{
			return i;
		}
	}

	return 1;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgHeightDetectionMeasureModeText(int mode)//取得演算法
{
	CString str;
	switch (mode)
	{
	case 0:	str = m_AlgHeightDetectionMeasureMode1;	break;
	case 1:	str = m_AlgHeightDetectionMeasureMode2;	break;
	default:					str = m_AlgHeightDetectionMeasureMode1;	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindAlgHeightDetectionMeasureModeByText(LPCTSTR FeatureText)
{
	CString  strFeatureText;

	for (int i = 0; i <= 1; i++)
	{
		strFeatureText = GetAlgHeightDetectionMeasureModeText(i);
		if (strFeatureText.CompareNoCase(FeatureText) == 0)
		{
			return i;
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgHeightDetectionOutputTypeText(int mode)//取得演算法
{
	CString str;
	switch (mode)
	{
	case 1:	str = m_AlgHeightDetectionOutputType1;	break;
	case 2:	str = m_AlgHeightDetectionOutputType2;	break;
	default:					str = m_AlgHeightDetectionOutputType1;	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::FindAlgHeightDetectionOutputTypeByText(LPCTSTR FeatureText)
{
	CString  strFeatureText;

	for (int i = 1; i <= 2; i++)
	{
		strFeatureText = GetAlgHeightDetectionOutputTypeText(i);
		if (strFeatureText.CompareNoCase(FeatureText) == 0)
		{
			return i;
		}
	}

	return 1;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgPatternFolder(int ImageFolderIndex)//取得演算法樣板資料夾
{
	CString Folder;	
	ImageFolderIndex = ImageFolderIndex+1;
	if ( ImageFolderIndex < 10 )
	{	Folder.Format(_T("Image#000%d"), ImageFolderIndex); }
	else if ( ImageFolderIndex < 100 )
	{	Folder.Format(_T("Image#00%d"), ImageFolderIndex); }
	else if ( ImageFolderIndex < 1000 )
	{	Folder.Format(_T("Image#0%d"), ImageFolderIndex); }
	else
	{	Folder.Format(_T("Image#%d"), ImageFolderIndex); }	
	return Folder;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAlgPatternName(int ImaegIndex, BOX_TOWARD Toward)//取得演算法樣板圖檔名稱
{
	CString ImageName;
	ImaegIndex = ImaegIndex+1;
	if ( ImaegIndex < 10 )
	{	ImageName.Format(_T("%s000%d"), _T("IMG"), ImaegIndex); }
	else if ( ImaegIndex < 100 )
	{	ImageName.Format(_T("%s00%d"), _T("IMG"), ImaegIndex); }
	else if ( ImaegIndex < 1000 )
	{	ImageName.Format(_T("%s0%d"), _T("IMG"), ImaegIndex); }
	else
	{	ImageName.Format(_T("%s%d"), _T("IMG"), ImaegIndex); }

	switch ( Toward )
	{
	case BOX_TOWARD_UP:		ImageName += _T("_T");	break;
	case BOX_TOWARD_LEFT:	ImageName += _T("_L");	break;
	case BOX_TOWARD_DOWN:	ImageName += _T("_B");	break;
	case BOX_TOWARD_RIGHT:	ImageName += _T("_R");	break;
	}
	//ImageName += _T(".BMP");
	ImageName += _T(".PNG");
	return ImageName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMarkFullName(unsigned int Index, LPCTSTR Name)//取得標記點全名
{
	CString strFullName;	
	Index = Index+1;//以1為起點
	strFullName.Format(_T("%05d_%s"), Index, Name);
	return strFullName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeFullName(unsigned int Index, LPCTSTR Name)//取得條碼全名
{	
	CString strFullName;	
	Index = Index+1;//以1為起點
	strFullName.Format(_T("%05d_%s"), Index, Name);
	return strFullName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBoxTowardText(BOX_TOWARD Toward)//取得框朝向文字
{
	CString str;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:		str = m_BoxTowardText_Up;	break;
	case BOX_TOWARD_LEFT:	str = m_BoxTowardText_Left;	break;	
	case BOX_TOWARD_DOWN:	str = m_BoxTowardText_Down;	break;
	case BOX_TOWARD_RIGHT:	str = m_BoxTowardText_Right;	break;
	default:				str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
BOX_SHAPE_MODE CAOIDataDefine::FindBoxShapeModeByText(LPCTSTR ModeText)
{
	CString     strBoxShape;
	BOX_SHAPE_MODE ShapeMode=BOX_SHAPE_RETURN;

	ShapeMode = BOX_SHAPE_RECTANGLE;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }
	
	ShapeMode = BOX_SHAPE_ROUND_RECT;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	ShapeMode = BOX_SHAPE_ELLIPSE;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	ShapeMode = BOX_SHAPE_CAPSULE;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	ShapeMode = BOX_SHAPE_BULLET;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	ShapeMode = BOX_SHAPE_HALF_ROUND_RECT;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	ShapeMode = BOX_SHAPE_T_SHAPE;
	strBoxShape = GetBoxShapeModeText(ShapeMode);
	if ( strBoxShape == ModeText ) { return ShapeMode; }

	return BOX_SHAPE_RETURN;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBoxShapeModeText(BOX_SHAPE_MODE ShapeMode)//取得框外形文字
{
	CString str;
	switch ( ShapeMode )
	{
	case BOX_SHAPE_RECTANGLE:	str = m_BoxShapeText_Rect;	break;
	case BOX_SHAPE_ROUND_RECT:	str = m_BoxShapeText_RectRound;	break;	
	case BOX_SHAPE_ELLIPSE:		str = m_BoxShapeText_Ellipse;	break;	
	case BOX_SHAPE_CAPSULE:		str = m_BoxShapeText_Capsule;	break;
	case BOX_SHAPE_BULLET:		str = m_BoxShapeText_Bullet;	break;
	case BOX_SHAPE_HALF_ROUND_RECT:	str = m_BoxShapeText_RectHalfRound;	break;		
	case BOX_SHAPE_T_SHAPE:		str = m_BoxShapeText_TShape;	break;
	default:					str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAITempName(LPCTSTR Name, unsigned int LightIdx)//取得AI暫存檔名
{
	CString strFullName;
	strFullName.Format(_T("%s#%d"), Name, LightIdx+1);
	return strFullName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetComponentFullName(unsigned int PanelIndex, unsigned int BoardIndex, LPCTSTR Name)//取得零件全名
{
	CString strFullName;
	strFullName.Format(_T("%05d_%05d_%s"), PanelIndex+1, BoardIndex+1, Name);
	return strFullName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetComponentFullNameReverse(unsigned int PanelIndex, unsigned int BoardIndex, LPCTSTR Name)//取得零件全名
{
	CString strFullName;
	strFullName.Format(_T("%s_%d_%d"), Name, PanelIndex+1, BoardIndex+1);
	return strFullName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFdSectionText(unsigned int Index)//取得定位點節點文字
{	
	CString str;	
	str.Format(_T("Fd_%05d"), Index+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFdFullName(unsigned int Index, LPCTSTR Name)//取得定位點全名
{	
	CString str;
	str.Format(_T("%s_%05d"), Name, Index+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFdModelFolder(LPCTSTR FdFolder, int FdUniqueID)//取得定位點模組資料夾
{	
	CString str;
	str.Format(_T("%s\\FD_%05d"), FdFolder, FdUniqueID+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFdPatternName(LPCTSTR ProjectFoder, unsigned int FdIdx, unsigned int PatIdx, bool bMask)//取得定位點樣板圖檔
{
	CString str;
	CString strFd;
	CString strPat;
	FdIdx += 1;
	PatIdx += 1;

	if ( FdIdx < 10 ) 
	{	strFd.Format(_T("Fd#00%d"), FdIdx);	}
	else if ( FdIdx < 100 ) 
	{	strFd.Format(_T("Fd#0%d"), FdIdx);	}
	else
	{	strFd.Format(_T("Fd#%d"), FdIdx);	}
	strPat.Format(_T("Pat#%d"), PatIdx);

	if ( true == bMask )
	{	str.Format(_T("%s\\Fiducial\\%s_%s_MSK.BMP"), ProjectFoder, strFd, strPat);	}
	else
	{	str.Format(_T("%s\\Fiducial\\%s_%s_ORG.BMP"), ProjectFoder, strFd, strPat);	}	
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFieldSectionText(unsigned int Index)//取得區域節點文字
{	
	CString str;		
	str.Format(_T("Field_%05d"), Index+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFrameMaskName(LPCTSTR filename)//取得影像遮罩圖像名稱
{
	CString str;
	CString MainName;
	JetAPI::ExtractMainFileName(filename, MainName);
	str.Format(_T("%s.MSK"), MainName);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFrameSectionText(unsigned int Index)//取得影像節點文字
{	
	CString str;
	str.Format(_T("Frame_%05d"), Index+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFrameShortName(unsigned int Index, FRAME_TYPE FrameType, bool bMask, DISTRICT_ID DistrictID)//取得影像短名
{	
	CString str;	
	CString ExtName = _T("PNG");

	if ( true == bMask )
	{	ExtName = _T("MSK");	}
	else
	{
		if ( FRAME_SPACE == FrameType )
		{	ExtName = _T("Z3D");	}
		else
		{	ExtName = _T("PNG");	}
	}
	if ( DISTRICT_ID_A == DistrictID )
	{	str.Format(_T("Frame_%05d.%s"), Index, ExtName); }
	else if ( DISTRICT_ID_B == DistrictID )
	{	str.Format(_T("FrameB_%05d.%s"), Index, ExtName); }
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetLandTypeText(LAND_TYPE Type)//取得特徵框樣式文字
{
	CString str;
	switch ( Type )
	{
	case LAND_TYPE_PAD:			str = m_LandTypeText_Pad;	break;
	case LAND_TYPE_ELECTRODE:	str = m_LandTypeText_Electrode;	break;	
	case LAND_TYPE_IC_LEAD:		str = m_LandTypeText_ICLead;	break;	
	case LAND_TYPE_CON_LEAD:	str = m_LandTypeText_ConLead;	break;			
	case LAND_TYPE_DIP_LEAD:	str = m_LandTypeText_DipLead;	break;		
	default:					str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetModelMaskText(int Mask)//取得模組遮罩文字
{
	CString str;	
	switch ( Mask )
	{
	case MODEL_MASK_NONE:			str = m_DisabeText;	break;
	case MODEL_MASK_PAD:			str = m_ModelMaskPadText;	break;	
	case MODEL_MASK_BODY:			str = m_ModelMaskBodyText;	break;		
	case MODEL_MASK_BODY_NO_LEAD:   str = m_ModelMaskBodyNoLeadText;	break;		
	case MODEL_MASK_LEAD:			str = m_ModelMaskLeadText;	break;	
	case MODEL_MASK_LEAD_TIP:		str = m_ModelMaskLeadTipText;	break;	
	case MODEL_MASK_LEAD_SHOULDER:	str = m_ModelMaskLeadShoulderText;	break;	
	default:						str = m_UndefinedText; 	break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
 LPCTSTR CAOIDataDefine::GetModelPadText() const //取得模組部位文字-焊盤
 {
	 return m_ModelPadText;
 }
 //-------------------------------------------------------------------------------------//
 LPCTSTR CAOIDataDefine::GetModelBodyText() const //取得模組部位文字-本體
 {
	 return m_ModelBodyText;
 }
 //-------------------------------------------------------------------------------------//
 LPCTSTR CAOIDataDefine::GetModelLeadText() const //取得模組部位文字-引腳電極
 {
	 return m_ModelLeadText;
 }
 //-------------------------------------------------------------------------------------//
 LPCTSTR CAOIDataDefine::GetModelLeadTipText() const //取得模組部位文字-引腳前端
 {
	 return m_ModelLeadTipText;
 }
 //-------------------------------------------------------------------------------------//
 LPCTSTR CAOIDataDefine::GetModelLeadShoulderText() const //取得模組部位文字-引腳根部
 {
	 return m_ModelLeadShoulderText;
 }
 //-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetModelGroupAllText()//取得模組全群組的文字
{	
	return m_ModelGroupText_All;//模組群組-全部;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetModelTypeText(MODEL_TYPE Type)//取得模組樣式文字
{
	CString str;
	switch ( Type )
	{
	case MODEL_TYPE_NULL:       str = m_ModelTypeText_Null;	break;
	case MODEL_TYPE_CHIP:		str = m_ModelTypeText_Chip;	break;
	case MODEL_TYPE_CHIP_C:		str = m_ModelTypeText_ChipC;	break;	
	case MODEL_TYPE_CHIP_R:		str = m_ModelTypeText_ChipR;	break;	
	case MODEL_TYPE_CHIP_L:		str = m_ModelTypeText_ChipL;	break;	
	case MODEL_TYPE_CHIP_LED:	str = m_ModelTypeText_ChipLed;	break;	
	case MODEL_TYPE_MELF:		str = m_ModelTypeText_Melf;	break;	

	case MODEL_TYPE_ELECTRODE:			str = m_ModelTypeText_Electrode;	break;	
	case MODEL_TYPE_TANTALUM_CONDENSER:	str = m_ModelTypeText_Tant;	break;	
	case MODEL_TYPE_CAPACITY_ARRAY:		str = m_ModelTypeText_CN;	break;	
	case MODEL_TYPE_RESISTOR_ARRAY:		str = m_ModelTypeText_RN;	break;	
	case MODEL_TYPE_TRANSISTOR:			str = m_ModelTypeText_SOT;	break;	
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:		str = m_ModelTypeText_ElecCap;	break;	
	case MODEL_TYPE_LED_ARRAY:			str = m_ModelTypeText_LedArray;	break;	

	case MODEL_TYPE_NO_LEAD_COMPONENT:	str = m_ModelTypeText_NoLead;	break;	
	case MODEL_TYPE_NO_LEAD_DFN:		str = m_ModelTypeText_NoLeadDN;	break;	
	case MODEL_TYPE_NO_LEAD_QFN:		str = m_ModelTypeText_NoLeadQFN;	break;	
	case MODEL_TYPE_NO_LEAD_OSC:		str = m_ModelTypeText_NoLeadOSC;	break;			

	case MODEL_TYPE_LEAD_COMPONENT:	str = m_ModelTypeText_LeadCom;	break;	
	case MODEL_TYPE_LEAD_SOP:		str = m_ModelTypeText_LeadComSOP;	break;	
	case MODEL_TYPE_LEAD_QFP:		str = m_ModelTypeText_LeadComQFP;	break;	
	case MODEL_TYPE_LEAD_TRANSISTOR:	str = m_ModelTypeText_LeadComSOT;	break;

	case MODEL_TYPE_JLEAD_COMPONENT:	str = m_ModelTypeText_JLeadCom;	break;	
	case MODEL_TYPE_JLEAD_SOJ:			str = m_ModelTypeText_JLeadComSOJ;	break;	
	case MODEL_TYPE_JLEAD_PLCC:			str = m_ModelTypeText_JLeadComPLCC;	break;	

	case MODEL_TYPE_COMPOSITE_COMPONENT:	str = m_ModelTypeText_CompositeCom;	break;	
	case MODEL_TYPE_POWER_TRANSISTOR:		str = m_ModelTypeText_PowerTransistor;	break;	
	case MODEL_TYPE_CONNECTOR:				str = m_ModelTypeText_Connector;	break;	

	case MODEL_TYPE_BGA:			str = m_ModelTypeText_BGA;	break;	
	case MODEL_TYPE_FD:				str = m_ModelTypeText_Fd;	break;			
	case MODEL_TYPE_BARCODE:		str = m_ModelTypeText_Barcode;	break;		

	case MODEL_TYPE_PAD_COMPONENT:	str = m_ModelTypeText_Pad;	break;	
	case MODEL_TYPE_GOLD_FINGER:	str = m_ModelTypeText_GoldFinger;	break;	
	case MODEL_TYPE_DIP_LEAD:		str = m_ModelTypeText_DipLead;	break;	
	case MODEL_TYPE_OTHERS:			str = m_ModelTypeText_Ohters;	break;	
	default:						str = m_UndefinedText; 	break;
	}
	return str;	 
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::GetModelTypePolarity(MODEL_TYPE Type)//取得模組是否有極性
{
	bool bPolarity = true;
	switch ( Type )
	{	
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	//case MODEL_TYPE_CHIP_L:
	//case MODEL_TYPE_ELECTRODE:
	case MODEL_TYPE_CAPACITY_ARRAY:
	case MODEL_TYPE_RESISTOR_ARRAY:	
		bPolarity = false;
		break;
	}
	return bPolarity;	 
}
//-------------------------------------------------------------------------------------//
CWndDefectItem CAOIDataDefine::GetModelBasicDefectItem(MODEL_TYPE Type)
{
	const int Enable = 1;
	CWndDefectItem DefectItems;
	std::vector<WND_DEFECT_ID> List;
	AOIDataCollect.BuildModelBasicDefectIDList(Type, List);
	const size_t Count=List.size();

	DefectItems.SetAll(0);
	for ( size_t i=0; i<Count; i++ )
	{
		WND_DEFECT_ID WndDefectID=List[i];
		DefectItems.SetItemCount(WndDefectID, Enable);
	}
	return DefectItems;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::GetModelDefaultWndParam(MODEL_TYPE Type, CHIP_SIZE_MODE ChipSizeMode, TMODEL_DEFAULT_WND_PARAM &Param)
{
	Param = TMODEL_DEFAULT_WND_PARAM();
	Param.eModelType = Type;
	Param.eChipSizeMode = ChipSizeMode;
	switch ( Type )
	{
	case MODEL_TYPE_NULL:
		Param.SetAll(false);
		break;
	case MODEL_TYPE_CHIP:
		Param.bLeadLifted = false;
		Param.bLeadBended = false;				
		Param.bBridgeUse2D = false;
		if ( CHIP_SIZE_OTHERS==ChipSizeMode || ChipSizeMode<CHIP_SIZE_040_020 )
		{	Param.bDummy = Param.bDummy; }
		else
		{	Param.bPadAlign = false; }
		break;
	case MODEL_TYPE_CHIP_C:		
		Param.bTextWrong = false;
		Param.bPolarity = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		if ( CHIP_SIZE_OTHERS==ChipSizeMode || ChipSizeMode<CHIP_SIZE_040_020 )
		{	Param.bDummy = Param.bDummy; }
		else
		{	Param.bPadAlign = false; }
		break;
	case MODEL_TYPE_CHIP_R:
		Param.bPolarity = false;
		Param.bLeadLifted = false;		
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		if ( CHIP_SIZE_OTHERS==ChipSizeMode || ChipSizeMode<CHIP_SIZE_040_020 )
		{	Param.bBodyMount = false; }
		else
		{	
			Param.bPadAlign = false; 
			Param.bTextWrong = false;
		}
		break;
	case MODEL_TYPE_CHIP_L:
		Param.bTextWrong = false;
		Param.bLeadLifted = false;		
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		if ( CHIP_SIZE_OTHERS==ChipSizeMode || ChipSizeMode<CHIP_SIZE_040_020 )
		{	Param.bDummy = Param.bDummy; }
		else
		{	Param.bPadAlign = false; }
		break;
	case MODEL_TYPE_CHIP_LED:
		Param.bTextWrong = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		break;
	case MODEL_TYPE_MELF:
		Param.bTextWrong = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		Param.bBridgeUse2D = false;
		break;
	case MODEL_TYPE_ELECTRODE:
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		break;
	case MODEL_TYPE_CAPACITY_ARRAY:
		Param.bTextWrong = false;
		Param.bPolarity = false;
		Param.nBodyTiltNum = 4;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		break;
	case MODEL_TYPE_RESISTOR_ARRAY:
		Param.bPolarity = false;
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		break;
	case MODEL_TYPE_TRANSISTOR:
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadAdjust = true;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		break;
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		Param.nBodyTiltNum = 2;
		Param.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_3D;
		Param.bBodyMount = false;
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		Param.bBridgeUse2D = false;
		break;	
	case MODEL_TYPE_LED_ARRAY:		
		Param.nBodyTiltNum = 4;
		Param.bTextWrong = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;		
		break;		
	case MODEL_TYPE_NO_LEAD_COMPONENT:
	case MODEL_TYPE_NO_LEAD_DFN:		
	case MODEL_TYPE_NO_LEAD_QFN:
	case MODEL_TYPE_NO_LEAD_OSC:
		Param.nBodyMountNum = 4;
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		Param.bSolderOpen = false;		
		break;
	case MODEL_TYPE_LEAD_COMPONENT:
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;			
		Param.bBodyMount = false;
		break;
	case MODEL_TYPE_LEAD_SOP:
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;		
		Param.bBodyMount = false;
		break;
	case MODEL_TYPE_LEAD_QFP:
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;		
		Param.bBodyMount = false;
		break;
	case MODEL_TYPE_LEAD_TRANSISTOR:
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadAdjust = true;
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
	case MODEL_TYPE_JLEAD_SOJ:
	case MODEL_TYPE_JLEAD_PLCC:
		Param.nBodyMountNum = 4;
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bLeadLifted = false;		
		Param.bLeadBended = false;
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;
		break;
	case MODEL_TYPE_POWER_TRANSISTOR:		
		Param.nBodyTiltNum = 4;	
		Param.bBodyMount = false;
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		break;
	case MODEL_TYPE_CONNECTOR:
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		Param.bTextWrong = false;
		break;	
	case MODEL_TYPE_BGA:
		Param.bPadAlign = false;
		Param.nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_3D;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;
		Param.bBodyMount = false;
		Param.bBodyDamaged = true;
		Param.bSolderOpen = false;
		Param.bSolderPoor = false;
		Param.bBridge = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		Param.bForeignBody = true;
		break;
	case MODEL_TYPE_BARCODE:
		Param.bPadAlign = false;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;
		Param.bSolderOpen = false;
		Param.bSolderPoor = false;
		Param.bBridge = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		break;
	case MODEL_TYPE_FD:
		Param.bPadAlign = false;
		Param.nBodyMountNum = 4;		
		Param.nBodyTiltNum = 4;
		Param.bSolderOpen = false;
		Param.bSolderPoor = false;
		Param.bBridge = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		break;
	case MODEL_TYPE_PAD_COMPONENT:
		Param.bBodyMount = false;
		Param.bLeadLifted = false;
		Param.bLeadBended = false;
		Param.bSolderOpen = false;
		break;
	case MODEL_TYPE_GOLD_FINGER:
		Param.SetAll(false);
		Param.bPadAlign = true;
		Param.bPadAdjust = true;
		//Param.bSolderPoor = true;
		Param.bPadScratch = true;
		break;
	case MODEL_TYPE_DIP_LEAD:
		Param.SetAll(false);
		Param.bPadAlign = true;
		Param.bPartAlign = true;
		Param.bPadAdjust = true;
		Param.bLeadAdjust = true;
		
		Param.bSolderOpen = true;
		Param.bSolderPoor = true;
		Param.bSolderPadExposed = true;
		Param.bBridge = true;
		break;
	case MODEL_TYPE_OTHERS:
		break;	
	}
	CString ChipSizeText;

	double ChipMin=0;
	double BodyExt=0, PadExt=0;	
	double GapW=0, GapH=0, GapT=0;
	double ChipW=0, ChipH=0, ChipT=0;
	double HeightGap=80;
	double PadOffsetMax=1000;
	double PartOffsetMax=200;
	double LeadOffsetMax=200;		
	double PadAlignExtendRange = 400;
	double PadAdjustExtendRange = 200;
	double PartAlignExtendRange = 200;
	double LeadAdjustExtendRange = 200;
	double PolarityExtendRange = 200;
	double BridgeExtendRange = 400;
	const bool bUseChipSizeLevel = CAOIModel::CheckModelTypUseChipSizeMode(Type);
	if ( true == bUseChipSizeLevel )
	{
		PadOffsetMax=2000;
		ChipSizeText = CAOIModel::GetModelChipSizeModeText(ChipSizeMode);
		if ( CAOIModel::GetModelChipSize(ChipSizeMode, ChipW, ChipH, ChipT) == true &&
			 CAOIModel::GetModelChipSizeGap(ChipSizeMode, GapW, GapH, GapT) == true	)			
		{
			ChipMin = MIN(ChipW, ChipH);
			ChipMin = ChipMin*0.25;

			ChipMin = JetAPI::AdjustValue(ChipMin, 5);
			PartOffsetMax=ChipMin;
			LeadOffsetMax=ChipMin;

			PadExt  = ChipMin*2.25;
			BodyExt = ChipMin*2.0;
			PadExt = JetAPI::AdjustValue(PadExt, 50);
			BodyExt = JetAPI::AdjustValue(BodyExt, 50);

			HeightGap=GapT;
			PadAlignExtendRange  = PadExt;
			PadAdjustExtendRange = BodyExt;
			PartAlignExtendRange = BodyExt;
			LeadAdjustExtendRange= BodyExt;
			PolarityExtendRange  = BodyExt;
			BridgeExtendRange = MIN(400, BodyExt);
		}
		Param.dPartAlignOffsetLimit = PartOffsetMax;		
		Param.dPartAlignHeightTolerance = HeightGap;
		Param.dBodyTiltHeightTolerance = HeightGap;
		Param.dBodyMissingHeightTolerance = HeightGap;
		Param.dPadAlignExtendRange = PadAlignExtendRange;		
		Param.dPadAdjustExtendRange = PadAdjustExtendRange;		
		Param.dPartAlignExtendRange = PartAlignExtendRange;		
		Param.dLeadAdjustExtendRange = LeadAdjustExtendRange;		
		Param.dPolarityExtendRange = PolarityExtendRange;
		Param.dBridgeExtendRange = BridgeExtendRange;
	}

	Param.ePartAlignLinkMode = CAOIModel::ObtainModelDefaultWndRegionLinkMode(Type, WND_DEFECT_PART_ALIGN);
	const bool bBasic = false;
	if ( true == bBasic )
	{
		CWndDefectItem BasicDefectItems = GetModelBasicDefectItem(Type);
		const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
		const size_t Count=List.size();
		for ( size_t i=0; i<Count; i++ )
		{
			WND_DEFECT_ID WndDefectID=List[i];
			if ( 0 == BasicDefectItems.GetItemCount(WndDefectID) )
			{	Param.SetEnable(WndDefectID, false);	}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildModelTypeCombox(CComboBox &Combox)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }

	CString text;
	int      index = 0;
	MODEL_TYPE ModelType = MODEL_TYPE_NULL;
	std::vector<MODEL_TYPE> ModelTypeList;
	CAOIModel::GetModelTypeList(ModelTypeList);	
	const size_t ModelTypeCount=ModelTypeList.size();

	JetAPI::ClearCombox(Combox);
	for ( size_t i=0; i<ModelTypeCount; i++ )
	{
		ModelType = ModelTypeList[i];
		if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		text = GetModelTypeText(ModelType);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, ModelType);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CAOIDataDefine::GetModelTypeIcon(MODEL_TYPE ModelType, bool Small)//取得模組樣式的圖示編號
{
	UINT IconID = 0;
	if ( true == Small )
	{	IconID = IDB_MODEL_EMPTY_S_ICON; }
	else
	{	IconID = IDB_MODEL_EMPTY_M_ICON; }
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP:
		break;
	case MODEL_TYPE_CHIP_C:
		if ( true == Small )
		{	IconID = IDB_MODEL_CHIP_C_S_ICON; }
		else
		{	IconID = IDB_MODEL_CHIP_C_M_ICON; }
		break;
	case MODEL_TYPE_CHIP_R:
		if ( true == Small )
		{	IconID = IDB_MODEL_CHIP_R_S_ICON; }
		else
		{	IconID = IDB_MODEL_CHIP_R_M_ICON; }		
		break;
	case MODEL_TYPE_CHIP_L:
		if ( true == Small )
		{	IconID = IDB_MODEL_CHIP_L_S_ICON; }
		else
		{	IconID = IDB_MODEL_CHIP_L_M_ICON; }		
		break;
	case MODEL_TYPE_CHIP_LED:
		if ( true == Small )
		{	IconID = IDB_MODEL_CHIP_LED_S_ICON; }
		else
		{	IconID = IDB_MODEL_CHIP_LED_M_ICON; }		
		break;
	case MODEL_TYPE_MELF:
		if ( true == Small )
		{	IconID = IDB_MODEL_MELF_S_ICON; }
		else
		{	IconID = IDB_MODEL_MELF_M_ICON; }
		break;
	case MODEL_TYPE_ELECTRODE:
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:
		if ( true == Small )
		{	IconID = IDB_MODEL_TANT_S_ICON; }
		else
		{	IconID = IDB_MODEL_TANT_M_ICON; }		
		break;
	case MODEL_TYPE_CAPACITY_ARRAY:
		if ( true == Small )
		{	IconID = IDB_MODEL_CN_S_ICON; }
		else
		{	IconID = IDB_MODEL_CN_M_ICON; }
		break;
	case MODEL_TYPE_RESISTOR_ARRAY:
		if ( true == Small )
		{	IconID = IDB_MODEL_RN_S_ICON; }
		else
		{	IconID = IDB_MODEL_RN_M_ICON; }
		break;
	case MODEL_TYPE_TRANSISTOR:
	case MODEL_TYPE_LEAD_TRANSISTOR:
		if ( true == Small )
		{	IconID = IDB_MODEL_SOT_S_ICON; }
		else
		{	IconID = IDB_MODEL_SOT_M_ICON; }		
		break;
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		if ( true == Small )
		{	IconID = IDB_MODEL_CAE_S_ICON; }
		else
		{	IconID = IDB_MODEL_CAE_M_ICON; }		
		break;
	case MODEL_TYPE_LED_ARRAY:
		if ( true == Small )
		{	IconID = IDB_MODEL_ARRAY_LED_S_ICON; }
		else
		{	IconID = IDB_MODEL_ARRAY_LED_M_ICON; }				
		break;	
	case MODEL_TYPE_PAD_COMPONENT:
		if ( true == Small )
		{	IconID = IDB_MODEL_PAD_COMPONENT_S_ICON; }
		else
		{	IconID = IDB_MODEL_PAD_COMPONENT_M_ICON; }		
		break;
	case MODEL_TYPE_NO_LEAD_COMPONENT:
		if ( true == Small )
		{	IconID = IDB_MODEL_NO_LEAD_S_ICON; }
		else
		{	IconID = IDB_MODEL_NO_LEAD_M_ICON; }
		break;
	case MODEL_TYPE_NO_LEAD_DFN:
		if ( true == Small )
		{	IconID = IDB_MODEL_DFN_S_ICON; }
		else
		{	IconID = IDB_MODEL_DFN_M_ICON; }
		break;
	case MODEL_TYPE_NO_LEAD_QFN:
		if ( true == Small )
		{	IconID = IDB_MODEL_QFN_S_ICON; }
		else
		{	IconID = IDB_MODEL_QFN_M_ICON; }
		break;	
	case MODEL_TYPE_NO_LEAD_OSC:
		if ( true == Small )
		{	IconID = IDB_MODEL_OSC_S_ICON; }
		else
		{	IconID = IDB_MODEL_OSC_M_ICON; }
		break;	
	case MODEL_TYPE_LEAD_COMPONENT:
		if ( true == Small )
		{	IconID = IDB_MODEL_LEAD_S_ICON; }
		else
		{	IconID = IDB_MODEL_LEAD_M_ICON; }				
		break;
	case MODEL_TYPE_LEAD_SOP:
		if ( true == Small )
		{	IconID = IDB_MODEL_SOP_S_ICON; }
		else
		{	IconID = IDB_MODEL_SOP_M_ICON; }		
		break;
	case MODEL_TYPE_LEAD_QFP:
		if ( true == Small )
		{	IconID = IDB_MODEL_QFP_S_ICON; }
		else
		{	IconID = IDB_MODEL_QFP_M_ICON; }
		break;
	//case MODEL_TYPE_JLEAD_COMPONENT:
	case MODEL_TYPE_JLEAD_SOJ:	
		if ( true == Small )
		{	IconID = IDB_MODEL_SOJ_S_ICON; }
		else
		{	IconID = IDB_MODEL_SOJ_M_ICON; }
		break;	
	case MODEL_TYPE_JLEAD_PLCC:
		if ( true == Small )
		{	IconID = IDB_MODEL_PLCC_S_ICON; }
		else
		{	IconID = IDB_MODEL_PLCC_M_ICON; }
		break;
		break;
	//case MODEL_TYPE_COMPOSITE_COMPONENT:
	case MODEL_TYPE_POWER_TRANSISTOR:
		if ( true == Small )
		{	IconID = IDB_MODEL_SOT_PWR_S_ICON; }
		else
		{	IconID = IDB_MODEL_SOT_PWR_M_ICON; }		
		break;

	case MODEL_TYPE_CONNECTOR:
		if ( true == Small )
		{	IconID = IDB_MODEL_CON_S_ICON; }
		else
		{	IconID = IDB_MODEL_CON_M_ICON; }
		break;	
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_FD:
	case MODEL_TYPE_BARCODE:
		if ( true == Small )
		{	IconID = IDB_MODEL_BGA_S_ICON; }
		else
		{	IconID = IDB_MODEL_BGA_M_ICON; }
		break;
	case MODEL_TYPE_GOLD_FINGER:
		if ( true == Small )
		{	IconID = IDB_MODEL_GOLDEN_FINGER_S_ICON; }
		else
		{	IconID = IDB_MODEL_GOLDEN_FINGER_M_ICON; }		
		break;
	case MODEL_TYPE_DIP_LEAD:
		if ( true == Small )
		{	IconID = IDB_MODEL_DIP_S_ICON; }
		else
		{	IconID = IDB_MODEL_DIP_M_ICON; }		
		break;
	case MODEL_TYPE_OTHERS:
		if ( true == Small )
		{	IconID = IDB_MODEL_OTHERS_S_ICON; }
		else
		{	IconID = IDB_MODEL_OTHERS_M_ICON; }		
		break;	
	default:
		if ( true == Small )
		{	IconID = IDB_MODEL_EMPTY_S_ICON; }
		else
		{	IconID = IDB_MODEL_EMPTY_M_ICON; }
		break;	
	}	
	return IconID;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetModelParamFilename(LPCTSTR LibraryFolder, LPCTSTR ModelName)//命名::模組參數檔名
{
	CString Filename;	
	Filename.Format(_T("%s\\%s.MDL"), LibraryFolder, ModelName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetModelBKImageFilename(LPCTSTR ModelFolder, LPCTSTR ModelName, int UniFrameIdx)//命名::模組底圖名稱
{
	CString BKImage;	
	BKImage.Format(_T("%s\\%s#%d.PNG"), ModelFolder, ModelName, UniFrameIdx+1);
	return BKImage;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildProjectMapIndexCombox(CComboBox &Combox)//專案底圖編號模式
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;	
	const int    MapIndexCount = FRAME_MAX_COUNT;

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	for ( i=0; i<MapIndexCount; i++ )
	{
		Param = i;
		str.Format(_T("%d"), Param+1);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildProjectMapScaleModeCombox(CComboBox &Combox)//專案底圖縮放比例模式
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;	
	const int    ScaleCount = 10;

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	for ( i=0; i<ScaleCount; i++ )
	{	
		Param = i+2;
		str.Format(_T("%d"), Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::GetProjectSpaceToGrayRatioModeCount() const
{
	return 16;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlProjectSpaceToGrayRatioCombox(CComboBox &Combox)//取得高度轉灰階比例
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;	
	const size_t RatioCount = GetProjectSpaceToGrayRatioModeCount();

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	for ( i=0; i<RatioCount; i++ )
	{
		Param += 5;
		str.Format(_T("%d"), Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildProjectColorGroupIDList(CComboBox &Combox)//取得專案彩色群組編號列表
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;	
	const int    ColorGroupCount = MAX_PROJECT_COLOR_COUNT;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Param = -1;
	str = GetEnableDisableText(FN_DISABLE);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	for ( i=0; i<ColorGroupCount; i++ )
	{
		Param = i;				
		str = GetProjectColorGroupText(i);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	

	if ( idx > 0 )
	{	Combox.SetCurSel(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlProjectDlpLedColorCombox(CComboBox &Combox)//取得DLP-LED顏色
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str = GetColorText_Red();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DLP_LED_COLOR_RED);
	idx ++;
	
	str = GetColorText_Green();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DLP_LED_COLOR_GREEN);
	idx ++;

	str = GetColorText_Blue();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DLP_LED_COLOR_BLUE);
	idx ++;

	str = GetColorText_White();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DLP_LED_COLOR_WHITE);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildProjectFieldSizeModeCombox(CComboBox &Combox)//建立專案區域尺寸模式
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str = _T("10 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_010);
	idx ++;

	str = _T("20 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_020);
	idx ++;

	str = _T("30 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_030);
	idx ++;

	str = _T("40 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_040);
	idx ++;

	str = _T("50 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_050);
	idx ++;

	str = _T("60 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_060);
	idx ++;

	str = _T("70 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_070);
	idx ++;

	str = _T("80 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_080);
	idx ++;

	str = _T("90 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_090);
	idx ++;

	str = _T("100 %");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_100);
	idx ++;

	str = _T("Dot");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FIELD_SIZE_DOT);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectTempFilename(LPCTSTR Filename)//形成專案暫存檔案名稱
{
	CString ExtName;	
	CString BaseFolder;
	CString ProjectName;
	CString ProjectTempFilename;
	CString DateTime;

	JetAPI::GetTime(DateTime, CTime::GetCurrentTime());
	BaseFolder = AOIDataCollect.GetAOITempProjectDirectory();
	JetAPI::ExtractExtendFileName(Filename, ExtName);		
	JetAPI::ExtractMainFileNameNoPath(Filename, ProjectName);
	ProjectTempFilename.Format(_T("%s\\%s_%s.%s"), BaseFolder, ProjectName, DateTime, ExtName);
	return ProjectTempFilename;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOfflineFolderName(LPCTSTR Folder)//取得專案離線資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), Folder, _T("Offline"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectMapFileName(LPCTSTR ProjectFolder)//取得專案底圖檔案名稱
{
	CString str;
	str.Format(_T("%s\\%s.INI"), ProjectFolder, _T("ProjectMap"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectGrrSigmaItemFileName(LPCTSTR ProjectFolder)//取得專案Grr標準差項目檔案名稱
{
	CString str;
	str.Format(_T("%s\\%s.SIG"), ProjectFolder, _T("ProjectComponent"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOfflineFdName(LPCTSTR Folder, DISTRICT_ID DistrictID)//取得專案離線定位點檔名
{
	CString str;
	if ( DISTRICT_ID_B == DistrictID )
	{	str.Format(_T("%s\\%s"), Folder, _T("OfflineB_Fd.INI")); }
	else
	{	str.Format(_T("%s\\%s"), Folder, _T("Offline_Fd.INI")); }
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOfflineMapName(LPCTSTR Folder, DISTRICT_ID DistrictID)//取得專案離線底圖檔名
{
	CString str;	
	if ( DISTRICT_ID_B == DistrictID )
	{	str.Format(_T("%s\\%s"), Folder, _T("Offline_B.OPG"));	}
	else
	{	str.Format(_T("%s\\%s"), Folder, _T("Offline.OPG"));	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOfflineFileName(LPCTSTR Folder, DISTRICT_ID DistrictID)//取得專案離線檔案名稱
{
	CString str;
	//str.Format(_T("%s\\%s"), Folder, _T("Offline.INI"));
	if ( DISTRICT_ID_B == DistrictID )
	{	str.Format(_T("%s\\%s"), Folder, _T("Offline_B.TXT"));	}
	else
	{	str.Format(_T("%s\\%s"), Folder, _T("Offline.TXT"));	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOfflineLocalName(LPCTSTR Folder, DISTRICT_ID DistrictID)//取得專案離線模組名稱
{
	CString str;	
	if ( DISTRICT_ID_B == DistrictID )
	{	str.Format(_T("%s\\%s"), Folder, _T("OfflineLocal_B.TXT"));	}
	else
	{	str.Format(_T("%s\\%s"), Folder, _T("OfflineLocal.TXT"));	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectMapImageName(LPCTSTR ProjectFolder, unsigned int MapIndex, bool bSmallMap)//取得專案底圖檔案名稱	 
{
	return GetProjectMapImageName(ProjectFolder, _T("ProjectMap"), MapIndex, bSmallMap, _T("JPG"));	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectMapImageName(LPCTSTR Folder, LPCTSTR MainName, int MapIdx, bool bSmall, LPCTSTR ExtName)//取得專案底圖檔案名稱
{
	CString MapName;	
	if ( false == bSmall )
	{
		if ( 0 == MapIdx )
		{	MapName.Format(_T("%s.%s"), MainName, ExtName);	}
		else
		{	MapName.Format(_T("%s#%d.%s"), MainName, MapIdx+1, ExtName); }
	}
	else
	{
		if ( 0 == MapIdx )
		{	MapName.Format(_T("%sSmall.%s"), MainName, ExtName);	}
		else
		{	MapName.Format(_T("%sSmall#%d.%s"), MainName, MapIdx+1, ExtName);	}		
	}

	CString PathName;	
	if ( NULL==Folder || 0==_tcslen(Folder) )
	{	PathName = MapName;	}
	else
	{	PathName.Format(_T("%s\\%s"), Folder, MapName);	}
	return PathName;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectMarkFileName(LPCTSTR ProjectFolder)//取得專案底標記檔案名稱
{
	CString str;
	str.Format(_T("%s\\%s.INI"), ProjectFolder, _T("ProjectMark"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectMapMaskFileName(LPCTSTR ProjectFolder)//取得專案底圖遮罩檔案名稱
{
	CString str;
	str.Format(_T("%s\\%s.PNG"), ProjectFolder, _T("ProjectMapMask"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectFdFolderName(LPCTSTR ProjectFolder)//取得專案定位點資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("Fiducial"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectSystemFolderName(LPCTSTR ProjectFolder)//取得專案系統資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("System"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectLibraryFolderName(LPCTSTR ProjectFolder)//取得專案資料庫資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("Library"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectOperLogFolderName(LPCTSTR ProjectFolder)//取得專案操錯訊息資料夾	 
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("OperLog"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectFieldMarkFolderName(LPCTSTR ProjectFolder)//取得專案區域定位資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("FieldMark"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectPartLibraryFolderName(LPCTSTR ProjectFolder)//取得專案元件庫資料夾
{
	CString str;
	str.Format(_T("%s\\%s"), ProjectFolder, _T("PartLibrary"));
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetProjectSpcResultFolder(LPCTSTR SpcProjectFolder, SAVE_SPC_FILE_MODE Mode)//取得專案維修站結果資料夾
{
	CString str;
	if ( SAVE_SPC_FILE_JSON_VRS == Mode )
	{	str.Format(_T("%s\\%s"), SpcProjectFolder, _T("VRS_Result"));	}
	else
	{	str.Format(_T("%s\\%s"), SpcProjectFolder, _T("Check_Result"));	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndIndexText(size_t index)//取得檢測框引數名稱 
{
	CString str;	
	str.Format(_T("%05d"), index+1);
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndLogicTypeText(WND_LOGIC_TYPE Type)//取得檢測框邏輯樣式文字
{
	CString str;
	switch ( Type )
	{
	case WND_LOGIC_NONE:		str = m_WndLogText_None;	break;
	case WND_LOGIC_GROUP_ID:	str = m_WndLogText_GroupID;	break;	
	case WND_LOGIC_DEFECT_ID:	str = m_WndLogText_DefectID;	break;	
	default:					str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
WND_LOGIC_TYPE CAOIDataDefine::FindWndLogicTypeByText(LPCTSTR Text)//依據文字尋找檢測框邏輯樣式
{
	CString strLogic;
	WND_LOGIC_TYPE LogicType = WND_LOGIC_NONE;

	LogicType = WND_LOGIC_NONE;
	strLogic = GetWndLogicTypeText(LogicType);
	if ( strLogic.CompareNoCase(Text) == 0 ) 
	{	return LogicType; }

	LogicType = WND_LOGIC_GROUP_ID;
	strLogic = GetWndLogicTypeText(LogicType);
	if ( strLogic.CompareNoCase(Text) == 0 ) 
	{	return LogicType; }

	LogicType = WND_LOGIC_DEFECT_ID;
	strLogic = GetWndLogicTypeText(LogicType);
	if ( strLogic.CompareNoCase(Text) == 0 ) 
	{	return LogicType; }

	return WND_LOGIC_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndFollowModeText(WND_FOLLOW_MODE Mode)//取得檢測框跟隨移動文字
{
	CString str;
	switch ( Mode )
	{
	case WND_FOLLOW_NONE:		str = m_WndFollowText_None;	break;
	case WND_FOLLOW_PAD:		str = m_WndFollowText_Pad;	break;	
	case WND_FOLLOW_PART:		str = m_WndFollowText_Part;	break;	
	case WND_FOLLOW_PAD_BODY:	str = m_WndFollowText_PadBody;	break;	
	case WND_FOLLOW_PAD_LEAD:	str = m_WndFollowText_PadLead;	break;	
	case WND_FOLLOW_PART_BODY:	str = m_WndFollowText_PartBody;	break;	
	case WND_FOLLOW_PART_LEAD:	str = m_WndFollowText_PartLead;	break;	
	default:					str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
WND_FOLLOW_MODE CAOIDataDefine::FindWndFollowModeByLinkModeText(LPCTSTR LinkText)//依據文字取得跟隨移動
{
	CString strDefect;
	WND_FOLLOW_MODE FollowMode = WND_FOLLOW_NONE;

	FollowMode = WND_FOLLOW_NONE;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PAD;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PART;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PAD_BODY;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PAD_LEAD;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PART_BODY;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	FollowMode = WND_FOLLOW_PART_LEAD;
	strDefect = GetWndFollowModeText(FollowMode);
	if ( strDefect.CompareNoCase(LinkText) == 0 ) 
	{	return FollowMode; }

	return WND_FOLLOW_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndRgnLinkModeText(WND_RGN_LINK_MODE Mode)//取得檢測框範圍連動文字
{
	CString str;
	switch ( Mode )
	{
	case WND_RGN_LINK_NONE:	str = m_WndRgnLinkText_None;	break;
	case WND_RGN_LINK_PAD:	str = m_WndRgnLinkText_Pad;	break;	
	case WND_RGN_LINK_BODY:	str = m_WndRgnLinkText_Body;	break;		
	case WND_RGN_LINK_LEAD:	str = m_WndRgnLinkText_Lead;	break;
	case WND_RGN_LINK_PAD_TIP:	str = m_WndRgnLinkText_PadTip;	break;
	case WND_RGN_LINK_PAD_RGN:  str = m_WndRgnLinkText_PadRgn;  break;
	case WND_RGN_LINK_PAD_BODY_RGN:  str = m_WndRgnLinkText_PadBodyRgn;  break;
	case WND_RGN_LINK_PAD_RGN_INNER:  str = m_WndRgnLinkText_PadRgnInner;  break;
	case WND_RGN_LINK_LEAD_TIP:	str = m_WndRgnLinkText_LeadTip;	break;	
	case WND_RGN_LINK_LEAD_SHOULDER:	str = m_WndRgnLinkText_LeadShoulder;	break;	
	case WND_RGN_LINK_LEAD_TIP_SHOULDER:	str = m_WndRgnLinkText_LeadTipShoulder;	break;		
	default:				str = m_UndefinedText; 	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
WND_FOLLOW_MODE CAOIDataDefine::GetWndFollowModeByWndDefectID(WND_DEFECT_ID DefectID)//依據瑕疵代碼取得跟隨移動
{
	WND_FOLLOW_MODE FollowMode = WND_FOLLOW_PAD;
	switch ( DefectID )
	{
	case WND_DEFECT_LEAD_ADJUST:
	case WND_DEFECT_BODY_MISSING:
	case WND_DEFECT_BODY_MOUNT:
	case WND_DEFECT_BODY_TILT:	
	case WND_DEFECT_BODY_POLARITY:
	case WND_DEFECT_BODY_TURNOVER:
	case WND_DEFECT_BODY_WRONG_CODE:
	case WND_DEFECT_BODY_WRONG_TEXT:
	case WND_DEFECT_BODY_TOMBSTONE:
	case WND_DEFECT_BODY_BILLBOARD:
	case WND_DEFECT_BODY_DAMAGED:
	case WND_DEFECT_SOLDER_POOR:
	case WND_DEFECT_SOLDER_OPEN:
	case WND_DEFECT_SOLDER_EXCESS:	
	case WND_DEFECT_LEAD_LIFTED:
	case WND_DEFECT_LEAD_BENDED:
	case WND_DEFECT_LEAD_PROTRUDED:
		FollowMode = WND_FOLLOW_PART;
		break;
	}	
	return FollowMode;
}
//-------------------------------------------------------------------------------------//
WND_RGN_LINK_MODE CAOIDataDefine::FindWndRgnLinkModeByLinkModeText(LPCTSTR LinkText)//取得範圍連動
{
	CString strDefect;
	WND_RGN_LINK_MODE RgnLinkMode = WND_RGN_LINK_NONE;
	std::vector<WND_RGN_LINK_MODE> RgnLinkModeList;
	RgnLinkModeList.push_back(WND_RGN_LINK_NONE);
	RgnLinkModeList.push_back(WND_RGN_LINK_PAD);
	RgnLinkModeList.push_back(WND_RGN_LINK_PAD_TIP);
	RgnLinkModeList.push_back(WND_RGN_LINK_PAD_RGN);
	RgnLinkModeList.push_back(WND_RGN_LINK_PAD_BODY_RGN);
	RgnLinkModeList.push_back(WND_RGN_LINK_PAD_RGN_INNER);
	RgnLinkModeList.push_back(WND_RGN_LINK_BODY);
	RgnLinkModeList.push_back(WND_RGN_LINK_LEAD);
	RgnLinkModeList.push_back(WND_RGN_LINK_LEAD_TIP);
	RgnLinkModeList.push_back(WND_RGN_LINK_LEAD_SHOULDER);
	RgnLinkModeList.push_back(WND_RGN_LINK_LEAD_TIP_SHOULDER);
	const size_t RgnLinkModeCount=RgnLinkModeList.size();

	for ( size_t i=0; i<RgnLinkModeCount; i++ )
	{
		RgnLinkMode = RgnLinkModeList[i];
		strDefect = GetWndRgnLinkModeText(RgnLinkMode);
		if ( strDefect.CompareNoCase(LinkText) == 0 ) 
		{	return RgnLinkMode; }
	}
	return WND_RGN_LINK_NONE;
}
//-------------------------------------------------------------------------------------//
CString  CAOIDataDefine::GetWndSyncMoveModeText(WND_SYNC_MOVE_MODE SyncMoveMode)//取得檢測同動模式
{
	CString str;
	switch ( SyncMoveMode )
	{
	case WND_SYNC_MOVE_ROTATE:	str = m_WndSyncMoveText_Rotate;	break;
	case WND_SYNC_MOVE_MIRROR:	str = m_WndSyncMoveText_Mirror;	break;
	case WND_SYNC_MOVE_SYMMETRY:	str = m_WndSyncMoveText_Symmetry;	break;
	default:					str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
WND_SYNC_MOVE_MODE CAOIDataDefine::FindWndSyncMoveModeByText(LPCTSTR SyncMoveText)//取得檢測框同動模式
{
	CString strSyncMove;
	WND_SYNC_MOVE_MODE SyncMoveMode;

	SyncMoveMode = WND_SYNC_MOVE_ROTATE;
	strSyncMove = GetWndSyncMoveModeText(SyncMoveMode);
	if ( strSyncMove.CompareNoCase(SyncMoveText) == 0 ) 
	{	return SyncMoveMode; }

	SyncMoveMode = WND_SYNC_MOVE_MIRROR;
	strSyncMove = GetWndSyncMoveModeText(SyncMoveMode);
	if ( strSyncMove.CompareNoCase(SyncMoveText) == 0 ) 
	{	return SyncMoveMode; }

	SyncMoveMode = WND_SYNC_MOVE_SYMMETRY;
	strSyncMove = GetWndSyncMoveModeText(SyncMoveMode);
	if ( strSyncMove.CompareNoCase(SyncMoveText) == 0 ) 
	{	return SyncMoveMode; }
	return WND_SYNC_MOVE_ROTATE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndConstrainModeText(WND_CONSTRAIN_MODE ConstrainMode)//取得檢測侷限模式文字
{
	CString str;
	switch ( ConstrainMode )
	{
	case WND_CONSTRAIN_DISABLE:			str = m_WndConstrainText_Disable;	break;
	case WND_CONSTRAIN_PAD_RGN_MOVE:	str = m_WndConstrainText_PadRgnMove;	break;	
	case WND_CONSTRAIN_PAD_RGN_X_MOVE:	str = m_WndConstrainText_PadRgnXMove;	break;
	case WND_CONSTRAIN_PAD_RGN_Y_MOVE:	str = m_WndConstrainText_PadRgnYMove;	break;	
	case WND_CONSTRAIN_PAD_RGN_SCALE:	str = m_WndConstrainText_PadRgnScale;	break;	
	case WND_CONSTRAIN_PAD_RGN_X_SCALE:	str = m_WndConstrainText_PadRgnXScale;	break;
	case WND_CONSTRAIN_PAD_RGN_Y_SCALE:	str = m_WndConstrainText_PadRgnYScale;	break;	
	default:							str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
WND_CONSTRAIN_MODE CAOIDataDefine::FindWndConstrainModeByText(LPCTSTR ConstrainText)//取得檢測框侷限模式
{
	CString strConstrain;
	WND_CONSTRAIN_MODE ConstrainMode = WND_CONSTRAIN_DISABLE;

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_MOVE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_X_MOVE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_Y_MOVE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_SCALE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_X_SCALE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	ConstrainMode = WND_CONSTRAIN_PAD_RGN_Y_SCALE;
	strConstrain = GetWndConstrainModeText(ConstrainMode);
	if ( strConstrain.CompareNoCase(ConstrainText) == 0 ) 
	{	return ConstrainMode; }

	return WND_CONSTRAIN_DISABLE;
}
//-------------------------------------------------------------------------------------//
ALG_AI_MODEL_ID CAOIDataDefine::FindAIModelIDByText(LPCTSTR IDText)//取得AI模型代碼
{
	CString strDefect;
	ALG_AI_MODEL_ID AIModelID = ALG_AI_MODEL_NONE;
	std::vector<ALG_AI_MODEL_ID> AIModelIDList;
	AOIDataCollect.BuildAIModelIDList(AIModelIDList);
	const size_t Count=AIModelIDList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		AIModelID = AIModelIDList[i];
		strDefect = GetAIModelIDText(AIModelID);
		if ( strDefect.CompareNoCase(IDText) == 0 ) 
		{	return AIModelID; }
	}
	return ALG_AI_MODEL_NONE;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAIModelIDText(ALG_AI_MODEL_ID AIModelID)//取得AI模型代碼文字
{
	CString str;
	switch ( AIModelID )
	{
	case ALG_AI_MODEL_NONE:		str = _T("Disable");	break;
	case ALG_AI_MODEL_OCR_01:	str = _T("AI-OCR");	break;	
	default:	str = m_UndefinedText;	break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetWndDefectIDText(WND_DEFECT_ID DefectID)//取得檢測框瑕疵代碼文字
{
	CString str;
	switch ( DefectID )
	{
	case WND_DEFECT_NONE:	str = m_WndDefectText_None;	break;
	case WND_DEFECT_PAD_ALIGN:	str = m_WndDefectText_PadAlign;	break;
	case WND_DEFECT_PART_ALIGN:	str = m_WndDefectText_PartAlign;	break;
	case WND_DEFECT_PAD_ADJUST:	str = m_WndDefectText_PadAdjust;	break;
	case WND_DEFECT_LEAD_ADJUST: str = m_WndDefectText_LeadAdjust;	break;

	case WND_DEFECT_CLASS_CHECK: str = m_WndDefectText_ClassCheck;	break;
	case WND_DEFECT_BASE_VALUE:	 str = m_WndDefectText_BaseValue;	break;

	case WND_DEFECT_BODY_MISSING:	str = m_WndDefectText_BodyMissing;	break;
	case WND_DEFECT_BODY_OFFSET:	str = m_WndDefectText_BodyOffset;	break;
	case WND_DEFECT_BODY_TILT:	str = m_WndDefectText_BodyTilt;	break;
	case WND_DEFECT_BODY_POLARITY:	str = m_WndDefectText_BodyPolarity;	break;
	case WND_DEFECT_BODY_TURNOVER:	str = m_WndDefectText_BodyTurnOver;	break;
	case WND_DEFECT_BODY_MOUNT:	str = m_WndDefectText_BodyMount;	break;
	case WND_DEFECT_BODY_WRONG_CODE:	str = m_WndDefectText_BodyWrongCode;	break;
	case WND_DEFECT_BODY_WRONG_TEXT:	str = m_WndDefectText_BodyWrongText;	break;
	case WND_DEFECT_BODY_TOMBSTONE:	str = m_WndDefectText_BodyTombstone;	break;
	case WND_DEFECT_BODY_BILLBOARD:	str = m_WndDefectText_BodyBillboard;	break;
	case WND_DEFECT_BODY_DAMAGED:	str = m_WndDefectText_BodyDamaged;	break;		

	case WND_DEFECT_SOLDER_POOR:	str = m_WndDefectText_SolderPoor;	break;
	case WND_DEFECT_SOLDER_OPEN:	str = m_WndDefectText_SolderOpen;	break;
	case WND_DEFECT_SOLDER_PAD_EXPOSED:	str = m_WndDefectText_SolderPadExposed;	break;
	case WND_DEFECT_SOLDER_BRIDGE:	str = m_WndDefectText_SolderBridge;	break;
	case WND_DEFECT_SOLDER_BEAD:	str = m_WndDefectText_SolderBead;	break;
	case WND_DEFECT_SOLDER_EXCESS:	str = m_WndDefectText_SolderExcess;	break;

	case WND_DEFECT_LEAD_LIFTED:	str = m_WndDefectText_LeadLifted;	break;
	case WND_DEFECT_LEAD_BENDED:	str = m_WndDefectText_LeadBended;	break;
	case WND_DEFECT_LEAD_PROTRUDED:	str = m_WndDefectText_LeadProtruded;	break;
	case WND_DEFECT_PAD_SCRATCH:	str = m_WndDefectText_PadScratch;	break;
	case WND_DEFECT_FOREIGN_BODY:	str = m_WndDefectText_ForeignBody;	break;
	
	case WND_DEFECT_USER_DEFINE_01:	str = m_WndDefectText_UserDefine_01;	break;
	case WND_DEFECT_USER_DEFINE_02:	str = m_WndDefectText_UserDefine_02;	break;
	case WND_DEFECT_USER_DEFINE_03:	str = m_WndDefectText_UserDefine_03;	break;
	case WND_DEFECT_USER_DEFINE_04:	str = m_WndDefectText_UserDefine_04;	break;
	case WND_DEFECT_USER_DEFINE_05:	str = m_WndDefectText_UserDefine_05;	break;
	case WND_DEFECT_USER_DEFINE_06:	str = m_WndDefectText_UserDefine_06;	break;
	case WND_DEFECT_USER_DEFINE_07:	str = m_WndDefectText_UserDefine_07;	break;
	case WND_DEFECT_USER_DEFINE_08:	str = m_WndDefectText_UserDefine_08;	break;	
	case WND_DEFECT_USER_DEFINE_09:	str = m_WndDefectText_UserDefine_09;	break;	
	case WND_DEFECT_USER_DEFINE_10:	str = m_WndDefectText_UserDefine_10;	break;	

	default:				str = m_UndefinedText; 	break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
WND_DEFECT_ID CAOIDataDefine::FindWndDefectIDByDefectText(LPCTSTR DefectText)//取得檢測框瑕疵代碼	
{
	CString strDefect;
	WND_DEFECT_ID DefectID = WND_DEFECT_NONE;
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		DefectID = List[i];
		strDefect = GetWndDefectIDText(DefectID);
		if ( strDefect.CompareNoCase(DefectText) == 0 ) 
		{	return DefectID; }
	}
	return WND_DEFECT_NONE;
}
//-------------------------------------------------------------------------------------//
int CAOIDataDefine::GetWndDefectIDOrder(WND_DEFECT_ID DefectID)//取得檢測框瑕疵代碼檢測次序
{
	int       nOrder=  0;
	int       nBase =  10000000;//預留可相同10000000的檢測框
	const int nMax =   10000000;//INT_MAX=2147483647
	switch ( DefectID )
	{
	case WND_DEFECT_NONE:
		break;

	//類別確認
	case WND_DEFECT_CLASS_CHECK:
		nOrder = nMax+(1*nBase);
		break;

	//定位瑕疵
	case WND_DEFECT_PAD_ALIGN:
		nOrder = nMax+(2*nBase);
		break;
	case WND_DEFECT_PART_ALIGN:		
		nOrder = nMax+(3*nBase);
		break;
	case WND_DEFECT_PAD_ADJUST:		
		nOrder = nMax+(4*nBase);
		break;
	case WND_DEFECT_LEAD_ADJUST:	
		nOrder = nMax+(5*nBase);
		break;
	case WND_DEFECT_BASE_VALUE:
		nOrder = nMax+(9*nBase);
		break;

	//部品檢測
	case WND_DEFECT_BODY_MISSING:	
		nOrder = nMax+(11*nBase);
		break;
	case WND_DEFECT_BODY_OFFSET:	
		nOrder = nMax+(12*nBase);
		break;
	case WND_DEFECT_BODY_TILT:		
		nOrder = nMax+(13*nBase);
		break;
	case WND_DEFECT_BODY_POLARITY:	
		nOrder = nMax+(14*nBase);
		break;
	case WND_DEFECT_BODY_TURNOVER:	
		nOrder = nMax+(15*nBase);
		break;
	case WND_DEFECT_BODY_MOUNT	: 
		nOrder = nMax+(16*nBase);
		break;
	case WND_DEFECT_BODY_WRONG_CODE: 
		nOrder = nMax+(17*nBase);
		break;
	case WND_DEFECT_BODY_WRONG_TEXT: 
		nOrder = nMax+(18*nBase);
		break;
	case WND_DEFECT_BODY_TOMBSTONE: 
		nOrder = nMax+(19*nBase);
		break;
	case WND_DEFECT_BODY_BILLBOARD: 
		nOrder = nMax+(20*nBase);
		break;
	case WND_DEFECT_BODY_DAMAGED:
		nOrder = nMax+(21*nBase);
		break;

	//焊錫檢測
	case WND_DEFECT_SOLDER_POOR:	
		nOrder = nMax+(51*nBase);
		break;
	case WND_DEFECT_SOLDER_OPEN:	
		nOrder = nMax+(52*nBase);
		break;
	case WND_DEFECT_SOLDER_PAD_EXPOSED: 
		nOrder = nMax+(53*nBase);
		break;
	case WND_DEFECT_SOLDER_BRIDGE:	
		nOrder = nMax+(54*nBase);
		break;
	case WND_DEFECT_SOLDER_BEAD:	
		nOrder = nMax+(55*nBase);
		break;
	case WND_DEFECT_SOLDER_EXCESS:
		nOrder = nMax+(56*nBase);
		break;

	//引腳檢測
	case WND_DEFECT_LEAD_LIFTED:	
		nOrder = nMax+(61*nBase);
		break;
	case WND_DEFECT_LEAD_BENDED:	
		nOrder = nMax+(62*nBase);
		break;
	case WND_DEFECT_LEAD_PROTRUDED: 
		nOrder = nMax+(63*nBase);
		break;
	case WND_DEFECT_PAD_SCRATCH:	
		nOrder = nMax+(64*nBase);
		break;
	case WND_DEFECT_FOREIGN_BODY:	
		nOrder = nMax+(65*nBase);
		break;	

	case WND_DEFECT_USER_DEFINE_01:
		nOrder = nMax+(81*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_02:
		nOrder = nMax+(82*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_03:
		nOrder = nMax+(83*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_04:
		nOrder = nMax+(84*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_05:
		nOrder = nMax+(85*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_06:
		nOrder = nMax+(86*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_07:
		nOrder = nMax+(87*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_08:
		nOrder = nMax+(88*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_09:
		nOrder = nMax+(89*nBase);
		break;
	case WND_DEFECT_USER_DEFINE_10:
		nOrder = nMax+(90*nBase);
		break;
	default:	
		nOrder = nMax+(91*nBase);
		break;
	}
	return nOrder;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::CheckWndDefectIDCanToAlign(WND_DEFECT_ID DefectID)//確認檢測框瑕疵代碼可以補償座標
{
	if ( WND_DEFECT_PAD_ALIGN == DefectID )
	{	return true; }

	if ( WND_DEFECT_PART_ALIGN == DefectID )
	{	return true; }

	if ( WND_DEFECT_PAD_ADJUST == DefectID )
	{	return true; }

	if ( WND_DEFECT_LEAD_ADJUST == DefectID )
	{	return true; }

	return false;	        
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBasePlaneProcTypeText(BASE_PLANE_PROC_TYPE eType)//取得基準面程序樣式文字
{
	CString str;
	switch ( eType )
	{
	case BASE_PLANE_PROC_TYPE_1:	str = m_BasePlaneProcText_1; break;
	case BASE_PLANE_PROC_TYPE_2:	str = m_BasePlaneProcText_2;	break;	
	default:						str = m_UndefinedText;	break;
	}	   	
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBasePlaneProcCombox(CComboBox &Combox)//建立基準面程序樣式列表
{
	CString text;
	int      index = 0;
	BASE_PLANE_PROC_TYPE eType;
	JetAPI::ClearCombox(Combox);
	
	eType = BASE_PLANE_PROC_TYPE_1;
	text = GetBasePlaneProcTypeText(eType);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eType); index ++;

	eType = BASE_PLANE_PROC_TYPE_2;
	text = GetBasePlaneProcTypeText(eType);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eType); index ++;
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBasePlaneAutoRegionText(BASE_PLANE_AUTO_REGION_MODE eMode)//取得基準面自動區域文字
{
	CString str;
	switch ( eMode )
	{
	case BASE_PLANE_AUTO_REGION_DISABLE:str = m_BasePlaneAutoRgnText_Disable; break;
	case BASE_PLANE_AUTO_REGION_GROUP:	str = m_BasePlaneAutoRgnText_Group;	break;	
	case BASE_PLANE_AUTO_REGION_LOWEST:	str = m_BasePlaneAutoRgnText_Lowest;	break;	
	default:							str = m_UndefinedText;	break;
	}	   	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBasePlaneAutoRegionCombox(CComboBox &Combox)//建立基準面自動區域列表
{	
	int      index = 0;
	std::vector<BASE_PLANE_AUTO_REGION_MODE> List;		
	List.push_back(BASE_PLANE_AUTO_REGION_DISABLE);
	//List.push_back(BASE_PLANE_AUTO_REGION_GROUP);
	List.push_back(BASE_PLANE_AUTO_REGION_LOWEST);	

	JetAPI::ClearCombox(Combox);
	for ( size_t i=0; i<List.size(); i++ )
	{
		BASE_PLANE_AUTO_REGION_MODE eMode=List[i];
		CString text = GetBasePlaneAutoRegionText(eMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, eMode); index ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBasePlaneBodyOutsideText(BASE_PLANE_BODY_OUTSIDE_MODE eMode)//取得基準面本體外圍文字
{
	CString str;
	switch ( eMode )
	{
	case BASE_PLANE_BODY_OUTSIDE_DISABLE:str = m_BasePlaneBodyOutsideText_Disable; break;
	case BASE_PLANE_BODY_OUTSIDE_BODY:	str = m_BasePlaneBodyOutsideText_Body;	break;	
	case BASE_PLANE_BODY_OUTSIDE_BODY_LAND:	str = m_BasePlaneBodyOutsideText_BodyLand;	break;	
	default:							str = m_UndefinedText;	break;
	}	   	
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBasePlaneBodyOutsideCombox(CComboBox &Combox)//建立基準面本體外圍列表
{
	int      index = 0;
	std::vector<BASE_PLANE_BODY_OUTSIDE_MODE> List;		
	List.push_back(BASE_PLANE_BODY_OUTSIDE_DISABLE);
	List.push_back(BASE_PLANE_BODY_OUTSIDE_BODY);
	List.push_back(BASE_PLANE_BODY_OUTSIDE_BODY_LAND);	

	JetAPI::ClearCombox(Combox);
	for ( size_t i=0; i<List.size(); i++ )
	{
		BASE_PLANE_BODY_OUTSIDE_MODE eMode=List[i];
		CString text = GetBasePlaneBodyOutsideText(eMode);
		Combox.InsertString(-1, text);//-1表示加在最後面
		Combox.SetItemData(index, eMode); index ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetCalcBasePlaneModeText(CALC_BASE_PLANE_MODE eMode)//取得計算基準面模式文字
{
	CString str;
	switch ( eMode )
	{
	case CALC_BASE_PLANE_DISABLE:	str = m_CalcBasePlaneText_Disable;	break;
	case CALC_BASE_PLANE_AVERAGE:	str = m_CalcBasePlaneText_Ave;	break;
	case CALC_BASE_PLANE_CORNER:	str = m_CalcBasePlaneText_Corner;	break;		
	case CALC_BASE_PLANE_ISO_DATA:	str = m_CalcBasePlaneText_IsoData;	break;		
	case CALC_BASE_PLANE_OTSU:		str = m_CalcBasePlaneText_Ostu;	break;		
	case CALC_BASE_PLANE_CORNER_ONLY:	str = m_CalcBasePlaneText_CornerOnly;	break;		
	case CALC_BASE_PLANE_SURROUND:	str = m_CalcBasePlaneText_Surround;	break;
	case CALC_BASE_PLANE_AUTO_LOWER: str = m_CalcBasePlaneText_AutoLower;	break;
	case CALC_BASE_PLANE_PANEL:		str = m_CalcBasePlaneText_Panel; break;
	case CALC_BASE_PLANE_LOCAL:		str = m_CalcBasePlaneText_Local; break;		
	default:						str = m_UndefinedText;	break;
	}	   	
	;
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildCalcBasePlaneCombox(CComboBox &Combox)//建立基準面平面列表
{
	CString text;
	int      index = 0;
	CALC_BASE_PLANE_MODE eMode;
	JetAPI::ClearCombox(Combox);	
	
	eMode = CALC_BASE_PLANE_DISABLE;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_AVERAGE;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_CORNER;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_ISO_DATA;
	text = GetCalcBasePlaneModeText(eMode);
	//Combox.InsertString(-1, text);//-1表示加在最後面
	//Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_OTSU;
	text = GetCalcBasePlaneModeText(eMode);
	//Combox.InsertString(-1, text);//-1表示加在最後面
	//Combox.SetItemData(index, eMode); index ++;
	  
	eMode = CALC_BASE_PLANE_CORNER_ONLY;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_SURROUND;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;	

	eMode = CALC_BASE_PLANE_AUTO_LOWER;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index++;
	
	eMode = CALC_BASE_PLANE_PANEL;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = CALC_BASE_PLANE_LOCAL;
	text = GetCalcBasePlaneModeText(eMode);
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBasePlaneTowardCombox(CComboBox &Combox)//建立基準面平面朝向
{
	CString text;
	int      index = 0;
	BASE_PLANE_TOWARD_MODE eMode;
	JetAPI::ClearCombox(Combox);	
	
	eMode = BASE_PLANE_TOWARD_ANY;
	text = _T("Any");
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;

	eMode = BASE_PLANE_TOWARD_VERTICAL;
	text = _T("Vertical");
	Combox.InsertString(-1, text);//-1表示加在最後面
	Combox.SetItemData(index, eMode); index ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetLight3DCastIDText(LIGHT_3D_CAST_ID CastID)//取得投光文字
{
	CString Text;
	switch ( CastID )
	{
	case  LIGHT_3D_CAST_00:	Text = _T("Cast00"); break;
	case  LIGHT_3D_CAST_01:	Text = _T("Cast01"); break;
	case  LIGHT_3D_CAST_02:	Text = _T("Cast02"); break;
	case  LIGHT_3D_CAST_03:	Text = _T("Cast03"); break;
	case  LIGHT_3D_CAST_04:	Text = _T("Cast04"); break;
	case  LIGHT_3D_CAST_05:	Text = _T("Cast05"); break;
	case  LIGHT_3D_CAST_06:	Text = _T("Cast06"); break;
	case  LIGHT_3D_CAST_07:	Text = _T("Cast07"); break;
	case  LIGHT_3D_CAST_08:	Text = _T("Cast08"); break;
	default:                Text = _T("Undefined"); break;   
	}
	return Text;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDLPPhaseModeText(int Mode)//取得相位模式的文字
{
	CString str;
	switch ( Mode )
	{			
	case LIGHT3D_PHASE_4_4_1:	 str = _T("4S2P1");	break;
	case LIGHT3D_PHASE_4_4_2:	 str = _T("4S2P2");	break;
	case LIGHT3D_PHASE_2_2_M:	 str = _T("2S2PM");	break;
	case LIGHT3D_PHASE_4_4_M:	 str = _T("4S2PM");	break;	
	case LIGHT3D_PHASE_4_4_M_2:  str = _T("4S2PM-2");	break;	
	case LIGHT3D_PHASE_4_4GC_M:  str = _T("4S4GC2PM");	break;	
	case LIGHT3D_PHASE_4_5GC_M:  str = _T("4S5GC2PM");	break;	
	case LIGHT3D_PHASE_4_6GC_M:  str = _T("4S6GC2PM");	break;	
	case LIGHT3D_PHASE_4_4GC_M_2:  str = _T("4S4GC2PM-2");	break;
	case LIGHT3D_PHASE_4_5GC_M_2:  str = _T("4S5GC2PM-2");	break;
	case LIGHT3D_PHASE_4_6GC_M_2:  str = _T("4S6GC2PM-2");	break;
	default:                     str = _T("Undefined"); break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDLPBitPosText(int index)//取得位元位置文字
{
	CString str;
	if ( index == 24 )
	{	str.Format(_T("Black"));}
	else if ( index < 8 )
	{	str.Format(_T("G:%d"), index);	}
	else if ( index < 16 )
	{	str.Format(_T("R:%d"), index-8);	}
	else 
	{	str.Format(_T("B:%d"), index-16);	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDLPPatternTriggerTypeText(int TriggerType)//取得樣板觸發樣式文字
{
	CString str;
	switch ( TriggerType )
	{
	case DLP_LED_TRIGGER_INTERNAL:	str = _T("Internal");	break;
	case DLP_LED_TRIGGER_EXTERNAL_POS:	str = _T("External Pos.");	break;
	case DLP_LED_TRIGGER_EXTERNAL_NEG:	str = _T("External Neg.");	break;
	case DLP_LED_TRIGGER_NO_INPUT:	str = _T("No Input");	break;
	default:                     str = _T("Undefined"); break;
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetDLPPatternLEDColorText(int LEDIndex)//取得樣版LED編號文字
{
	CString str;
	switch ( LEDIndex )
	{
	case DLP_LED_COLOR_RED:		str = _T("Red");	break;
	case DLP_LED_COLOR_GREEN:	str = _T("Green");	break;
	case DLP_LED_COLOR_YELLOW:	str = _T("Yellow");	break;
	case DLP_LED_COLOR_BLUE:	str = _T("Blue");	break;
	case DLP_LED_COLOR_MAGENTA:	str = _T("Magenta");	break;
	case DLP_LED_COLOR_CYAN:	str = _T("Cyan");	break;
	case DLP_LED_COLOR_WHITE:	str = _T("White");	break;
	case DLP_LED_COLOR_DEBUG:   str = _T("Debug");  break;
	default:                    str = _T("Undefined"); break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::CalcDLPBitPosRange(int BitDepth, int PatNum, int &first, int &end)//計算位元位置與範圍	
{
	int j=0;
	int index = 0;
	
	if ( BitDepth == 5 || BitDepth == 7)
	{	index = PatNum*(BitDepth+1); }
	else
	{	index = PatNum*BitDepth; }

	if ( PatNum == 24 )
	{	first = end = 24;	}
	else
	{
		first = 32;
		for( j=0; j < 32; j++)
		{
			if ( first > 31) //first set bit not found yet
			{
				if( (BitPlanes[index][BitDepth-1] & (1 << j)) == (1<<j))
				{	first = j;	}
			}
			else if((BitPlanes[index][BitDepth-1] & (1 << j)) == 0)
			{	break;	}
		}
		end = j-1;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPLEDCurrentIDCombox(CComboBox &Combox)//建立LED電流編號列表
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("1");
	Param=DLP_LED_CURRENT_ID_01;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("2");
	Param=DLP_LED_CURRENT_ID_02;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPOperationModeCombox(CComboBox &Combox)//建立投射編號視窗	
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("Pattern Seq.");
	Param=DLP_OPERATION_PATTERN;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

#ifndef LIGHT_3D_TI_DLP_USE_V2
	str = _T("Pattern Seq.[Variable Exposure]");
	Param=DLP_OPERATION_PATTERN_EXP;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
#endif//LIGHT_3D_TI_DLP_USE_V2

	str = _T("Video");
	Param=DLP_OPERATION_VIDEO;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Stand By");
	Param=DLP_OPERATION_STANDBY;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPhaseLEDColorCombox(CComboBox &Combox, bool bDebug)//建立相位LED燈源視窗
{
	CString str;
	int     idx=0;	
	int     Item=0;

	JetAPI::ClearCombox(Combox);
	
	Item=DLP_LED_COLOR_RED;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	Item=DLP_LED_COLOR_GREEN;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	Item=DLP_LED_COLOR_BLUE;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	Item=DLP_LED_COLOR_WHITE;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;	
	
	if ( true == bDebug )
	{
		Item=DLP_LED_COLOR_DEBUG;
		str = GetDLPPatternLEDColorText(Item);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Item);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternExpNumCombox(CComboBox &Combox)//建立樣板曝光數量
{
	CString str;
	int     idx=0;	
	int     Item=0;

	JetAPI::ClearCombox(Combox);
	
	Item=DLP_PATTERN_EXP_NUM_01;
	str.Format(_T("%d"), Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_PATTERN_EXP_NUM_02;
	str.Format(_T("%d"), Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternLEDColorCombox(CComboBox &Combox)//建立樣板LED燈源視窗
{
	CString str;
	int     idx=0;	
	int     Item=0;

	JetAPI::ClearCombox(Combox);
	
	Item=DLP_LED_COLOR_RED;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_GREEN;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_YELLOW;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_BLUE;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_MAGENTA;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_CYAN;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item=DLP_LED_COLOR_WHITE;
	str = GetDLPPatternLEDColorText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternFlashIndexCombox(CComboBox &Combox)//建立樣板圖像編號視窗
{
	CString str;
	int     idx=0;		
	int     Param = 0;

	JetAPI::ClearCombox(Combox);
	
	for ( int i=0; i<16; i++ )
	{
		Param = i;
		str.Format(_T("%d"), Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternBitDepthCombox(CComboBox &Combox)//建立樣板位元深度視窗
{
	CString str;
	int     idx=0;		
	int     Param=0;

	JetAPI::ClearCombox(Combox);
	
	str = _T("1 Bit");	
	Param = DLP_BIT_DEPTH_1;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("2 Bit");	
	Param = DLP_BIT_DEPTH_2;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("3 Bit");	
	Param = DLP_BIT_DEPTH_3;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("4 Bit");	
	Param = DLP_BIT_DEPTH_4;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	
	str = _T("5 Bit");	
	Param = DLP_BIT_DEPTH_5;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("6 Bit");	
	Param = DLP_BIT_DEPTH_6;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	
	str = _T("7 Bit");	
	Param = DLP_BIT_DEPTH_7;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("8 Bit");	
	Param = DLP_BIT_DEPTH_8;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternBitRangeCombox(int BitDepth, CComboBox &Combox)//建立樣板位元區間視窗	
{
	CString str;
	int     idx=0;		
	int     Param=0;
	int     i=0;
	const int TotalBits = 24;
	CString strBit[TotalBits];

	for ( i=0; i<TotalBits; i++ )
	{
		if ( i<8 )
		{	strBit[i].Format(_T("G%d"), i);	}
		else if ( i<16 )
		{	strBit[i].Format(_T("R%d"), i-8);	}
		else
		{	strBit[i].Format(_T("B%d"), i-16);	}
	}

	JetAPI::ClearCombox(Combox);
	switch ( BitDepth )
	{
	case 1:
		Param = 0;
		for ( i=0; i<TotalBits; i++ )
		{			
			str = strBit[i];
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param ++;
			idx ++;
		}
		//add white pattern-special case
		str = _T("Black");
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		Param ++;
		idx ++;
		break;
	case 2:
		Param = 0;
		for ( i=0; i<TotalBits; i+=2 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i], strBit[i+1]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=2;
			idx ++;
		}		
		break;
	case 3:
		Param = 0;
		for ( i=0; i<TotalBits; i+=3 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i], strBit[i+2]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=3;
			idx ++;
		}		
		break;
	case 4:
		Param = 0;
		for ( i=0; i<TotalBits; i+=4 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i], strBit[i+3]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=4;
			idx ++;
		}		
		break;
	case 5:
		Param = 0;
		for ( i=0; i<TotalBits; i+=6 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i+1], strBit[i+5]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=6;
			idx ++;
		}		
		break;
	case 6:
		Param = 0;
		for ( i=0; i<TotalBits; i+=6 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i], strBit[i+5]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=6;
			idx ++;
		}		
		break;
	case 7:
		Param = 0;
		for ( i=0; i<TotalBits; i+=8 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i+1], strBit[i+7]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=8;
			idx ++;
		}		
		break;
	case 8:
		Param = 0;
		for ( i=0; i<TotalBits; i+=8 )
		{			
			str.Format(_T("%s ~ %s"), strBit[i], strBit[i+7]);
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			Param +=8;
			idx ++;
		}		
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternSequenceModeCombox(CComboBox &Combox)//建立樣板序列模式視窗
{
	CString str;
	int     idx=0;	
	int     Item=0;

	JetAPI::ClearCombox(Combox);

	
	str = _T("White");	
	Item = DLP_PATTERN_SEQUENCE_WHITE;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	str = _T("RGB");	
	Item = DLP_PATTERN_SEQUENCE_RGB;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	str = _T("1-Phase");	
	Item = DLP_PATTERN_SEQUENCE_4_4_1;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	str = _T("2-Phase");	
	Item = DLP_PATTERN_SEQUENCE_4_4_2;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	str = _T("M-Phase");	
	Item = DLP_PATTERN_SEQUENCE_4_4_M;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;	

	str = _T("Customer");	
	Item = DLP_PATTERN_SEQUENCE_DEBUG;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternTriggerTypeCombox(CComboBox &Combox)//建立樣板觸發樣式視窗	
{
	CString str;
	int     idx=0;	
	int     Item=0;

	JetAPI::ClearCombox(Combox);
	
	Item = DLP_LED_TRIGGER_INTERNAL;
	str = GetDLPPatternTriggerTypeText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item = DLP_LED_TRIGGER_EXTERNAL_POS;	
	str = GetDLPPatternTriggerTypeText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item = DLP_LED_TRIGGER_EXTERNAL_NEG;	
	str = GetDLPPatternTriggerTypeText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	
	Item = DLP_LED_TRIGGER_NO_INPUT;	
	str = GetDLPPatternTriggerTypeText(Item);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Item);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPPatternSouceCombox(CComboBox &Combox)//建立樣板來源視窗
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("Flash");	
	Param=DLP_PATTERN_SOURCE_FLASH;	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Video Port");	
	Param=DLP_PATTERN_SOURCE_VIDEO_PORT;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDLPSequenceTriggerModeCombox(CComboBox &Combox)//建立序列觸發視窗	
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("Internal/External");	
	Param=DLP_SEQUENCE_TRIGGER_INT_EXT;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Vsync");	
	Param=DLP_SEQUENCE_TRIGGER_VSYNC;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetFdText_NG() const//取得定位點文字-瑕疵
{
	return m_FdText_NG;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetBarcodeText_NG() const//取得條碼文字-瑕疵
{
	return m_BarcodeText_NG;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Pad() const//色彩群組文字-銅箔
{
	return m_ProjectColorGroupText_Pad;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Body() const//色彩群組文字-本體
{
	return m_ProjectColorGroupText_Body;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Void() const//色彩群組文字-空焊
{
	return m_ProjectColorGroupText_Void;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Board() const//色彩群組文字-基板
{
	return m_ProjectColorGroupText_Board;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Solder() const//色彩群組文字-焊錫
{
	return m_ProjectColorGroupText_Solder;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataDefine::GetProjectColorGroupText_Others() const//色彩群組文字-其它
{
	return m_ProjectColorGroupText_Ohters;	
}
//-------------------------------------------------------------------------------------//
CString  CAOIDataDefine::GetProjectColorGroupText(size_t Index)//取得專案顏色群組文字
{
	CString str;	
	size_t  ID=0;
	if ( Index >= PROJECT_COLOR_ID_BOARD_BEGIN && Index <=PROJECT_COLOR_ID_BOARD_END )
	{
		ID = Index - PROJECT_COLOR_ID_BOARD_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Board, ID+1);
	}
	if ( Index >= PROJECT_COLOR_ID_PAD_BEGIN && Index <=PROJECT_COLOR_ID_PAD_END )
	{
		ID = Index - PROJECT_COLOR_ID_PAD_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Pad, ID+1);
	}
	if ( Index >= PROJECT_COLOR_ID_SOLDER_BEGIN && Index <=PROJECT_COLOR_ID_SOLDER_END )
	{
		ID = Index - PROJECT_COLOR_ID_SOLDER_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Solder, ID+1);
	}
	if ( Index >= PROJECT_COLOR_ID_VOID_BEGIN && Index <=PROJECT_COLOR_ID_VOID_END )
	{
		ID = Index - PROJECT_COLOR_ID_VOID_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Void, ID+1);
	}
	if ( Index >= PROJECT_COLOR_ID_BODY_BEGIN && Index <=PROJECT_COLOR_ID_BODY_END )
	{
		ID = Index - PROJECT_COLOR_ID_BODY_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Body, ID+1);
	}
	if ( Index >= PROJECT_COLOR_ID_OTHERS_BEGIN && Index <= PROJECT_COLOR_ID_OTHERS_END )
	{
		ID = Index - PROJECT_COLOR_ID_OTHERS_BEGIN;
		str.Format(_T("%s[%d]"), m_ProjectColorGroupText_Ohters, ID+1);
	}	
	if ( Index > PROJECT_COLOR_ID_OTHERS_END )
	{
		ID = Index - PROJECT_COLOR_ID_OTHERS_END;
		str.Format(_T("%s[%d]"), m_UndefinedText, ID);		
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_MODE Mode)//取得軸控加速度調整文字	 
{
	CString Text;
	switch ( Mode )
	{
	case  ACC_TIME_ADJUST_OFF:		Text = m_MotionAccTimeAdjustText_Off; break;
	case  ACC_TIME_ADJUST_FIX_T:	Text = m_MotionAccTimeAdjustText_Fix; break;	
	case  ACC_TIME_ADJUST_MIN_T:	Text = m_MotionAccTimeAdjustText_Min; break;
	case  ACC_TIME_ADJUST_GAMMA:	Text = m_MotionAccTimeAdjustText_Gamma; break;	
	default:
		Text = m_UndefinedText; 
		break;   
	}
	return Text;
}
//-------------------------------------------------------------------------------------// 
bool CAOIDataDefine::BuildLEDCurrentCaliModeCombox(CComboBox &Combox)
{
	int          i=0;
	int          idx=0;	
	CString      str;
	DWORD        Param=0;	

	idx = 0;
	JetAPI::ClearCombox(Combox);

	str = _T("Disable");
	str = GetDisableText();
	Param = LED_CURRENT_CALI_DISABLE;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Average");
	str = GetAveText();
	Param = LED_CURRENT_CALI_AVERAGE;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
	str = _T("Balance");
	//str = GetBalanceText();
	Param = LED_CURRENT_CALI_BALANCE;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------// 
bool CAOIDataDefine::BuildSliceFuncModeCombox(CComboBox &Combox, bool Include2D, bool Include3D)
{
	int          i=0;
	int          idx=0;	
	CString      str;
	DWORD        Param=0;	
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	if ( true == bDisable3D )
	{	Include3D = false;	}

	if ( true == Include2D )
	{
		str = _T("2D Image Gray");
		Param = SLICE_FUNC_2D_IMAGE_GRAY;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	if ( true == Include3D )
	{
		double PeriodRatio=1.0;
		const double P1 = SysParam.m_PhasePeriod1;
		const double P2 = SysParam.m_PhasePeriod2;
		const int PhaseCombinePeriodMode = AOIDataCollect.GetPhaseCombinePeriodMode();
		ImageAPI.CalcCombinePhasePeriodRatio_Public(PhaseCombinePeriodMode, P1, P2, PeriodRatio);

		CString Name=_T("3D Pattern");
		const bool Enable1PhaseA=false;
		const bool UseAliasName=true;//使用別名
		if ( true == Enable1PhaseA )
		{
			str = _T("3D 4Step 1Phase 1Exp A");
			Param = SLICE_FUNC_3D_4STEP_1EXP_A;
			if ( true == UseAliasName )
			{	str.Format(_T("%s [%03d]"), Name, Param);	}
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			idx ++;
		}

		str = _T("3D 4+4Step 2Phase 1Exp");
		Param = SLICE_FUNC_3D_4STEP_4STEP_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x%.0f)"), Name, Param, PeriodRatio);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 4+2Step 2Phase 1Exp");
		Param = SLICE_FUNC_3D_4STEP_2STEP_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d]"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 2+2Step 2Phase 1Exp");
		Param = SLICE_FUNC_3D_2STEP_2STEP_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d]"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 4Step+4GC 2Phase 1Exp");
		Param = SLICE_FUNC_3D_4STEP_4GC_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x08)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 4Step+5GC 2Phase 1Exp");
		Param = SLICE_FUNC_3D_4STEP_5GC_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x16)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 4Step+6GC 2Phase 1Exp");
		Param = SLICE_FUNC_3D_4STEP_6GC_1EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x32)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		if ( true == Enable1PhaseA )
		{
			str = _T("3D 4Step 1Phase 2Exp A");
			Param = SLICE_FUNC_3D_4STEP_2EXP_A;
			if ( true == UseAliasName )
			{	str.Format(_T("%s [%03d]"), Name, Param);	}
			Combox.InsertString(-1, str);
			Combox.SetItemData(idx, Param);
			idx ++;
		}

		str = _T("3D 4+4Step 2Phase 2Light");
		Param = SLICE_FUNC_3D_4STEP_4STEP_2LIGHT;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x%.0f)"), Name, Param, PeriodRatio);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		//v1.01.03.234
		str = _T("3D 4+5GC 2Phase 2Light");
		Param = SLICE_FUNC_3D_4STEP_5GC_2LIGHT;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x16)"), Name, Param);	}
		//Combox.InsertString(-1, str);
		//Combox.SetItemData(idx, Param);
		//idx ++;	

		//v1.01.04.046
		str = _T("3D 4+6GC 2Phase 2Light");
		Param = SLICE_FUNC_3D_4STEP_6GC_2LIGHT;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x32)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	

		str = _T("3D 4+4GC 2Phase 2Light");
		Param = SLICE_FUNC_3D_4STEP_4GC_2LIGHT;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x08)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	

		str = _T("3D 4+4Step 2Phase 2Exp");
		Param = SLICE_FUNC_3D_4STEP_4STEP_2EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x%.0f)"), Name, Param, PeriodRatio);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;

		str = _T("3D 4+2Step 2Phase 2Exp");
		Param = SLICE_FUNC_3D_4STEP_2STEP_2EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d]"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	

		//v1.01.03.234
		str = _T("3D 4+5GC 2Phase 2Exp");
		Param = SLICE_FUNC_3D_4STEP_5GC_2EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x16)"), Name, Param);	}
		//Combox.InsertString(-1, str);
		//Combox.SetItemData(idx, Param);
		//idx ++;	

		str = _T("3D 4+6GC 2Phase 2Exp");
		Param = SLICE_FUNC_3D_4STEP_6GC_2EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x32)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	

		str = _T("3D 4+4GC 2Phase 2Exp");
		Param = SLICE_FUNC_3D_4STEP_4GC_2EXP;
		if ( true == UseAliasName )
		{	str.Format(_T("%s [%03d](x08)"), Name, Param);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildSystemSliceParamCombox(CComboBox &Combox, bool IncludeNone, bool IncludeDLP, bool bUseCaliMode)
{
	int          i=0;
	int          idx=0;	
	CString      str;
	DWORD        Param=0;
	TSliceParam *SliceParamPtr = NULL;	
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	const int SliceParamCount = (int)(AOIDataCollect.GetSystemSliceParamCount());

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	if ( true == bDisable3D )
	{	IncludeDLP = false;	}

	if ( true == IncludeNone )
	{
		str = _T("---");
		Param = SLICE_UNIQUE_ID_NULL;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	for ( i=0; i<SliceParamCount; i++ )
	{
		SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtr(i, false);
		if ( NULL == SliceParamPtr ) { continue; }
		if ( false == bUseCaliMode )
		{
			if ( LED_CURRENT_CALI_DISABLE == SliceParamPtr->SliceLightTable.LEDCurrCaliMode )
			{	continue; }			
		}
		if ( false == IncludeDLP )
		{
			if ( LIGHT_DLP == SliceParamPtr->SliceLightTable.LightType ) { continue; }
		}

		str = SliceParamPtr->SliceName;
		Param = SliceParamPtr->SliceUniqueID;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFrameTypeName(FRAME_TYPE type)
{
	CString str;
	switch ( type )
	{
	case FRAME_GRAY:	str = _T("Gray"); break;
	case FRAME_BAYER:	str = _T("Bayer"); break;
	case FRAME_COLOR:	str = _T("Color"); break;
	case FRAME_SPACE:	str = _T("3D");		break;
	default:			str = m_UndefinedText; break; 
	}
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildSystemFrameParamTypeCombox(CComboBox &Combox)
{
	int          i=0;
	int          idx=0;	
	CString      str;
	FRAME_TYPE   Param=FRAME_NULL;
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	CAMERA_IMAGE_MODE CameraImageMode = AOIDataCollect.GetCameraImageMode(PRIMARY_CAMERA_ID);

	idx = 0;
	JetAPI::ClearCombox(Combox);
	
	if ( CAMERA_IMAGE_GRAY == CameraImageMode )
	{
		Param = FRAME_GRAY;
		str = AOIDataDefine.GetFrameTypeName(Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	if ( CAMERA_IMAGE_BAYER == CameraImageMode )
	{
		Param = FRAME_BAYER;
		str = AOIDataDefine.GetFrameTypeName(Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	if ( CAMERA_IMAGE_GRAY == CameraImageMode )
	{
		Param = FRAME_COLOR;
		str = AOIDataDefine.GetFrameTypeName(Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	if ( false == bDisable3D )
	{
		Param = FRAME_SPACE;
		str = AOIDataDefine.GetFrameTypeName(Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildSystemFrameParamCombox(CComboBox &Combox, bool IncludeNone, bool Include3D)//建立Frame列表		
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;
	TFrameParam *FrameParamPtr = NULL;
	const size_t FrameParamCount = AOIDataCollect.GetSystemFrameParamCount();
	CAMERA_IMAGE_MODE CameraImageMode = AOIDataCollect.GetCameraImageMode(PRIMARY_CAMERA_ID);
	
	idx = 0;
	JetAPI::ClearCombox(Combox);	

	if ( true == IncludeNone )
	{
		str = _T("---");
		Param = FRAME_UNIQUE_ID_NULL;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}

	for ( i=0; i<FrameParamCount; i++ )
	{
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtr(i, false);
		if ( NULL == FrameParamPtr ) { continue; }
		if ( false == Include3D )
		{
			if ( FRAME_SPACE == FrameParamPtr->FrameType ) { continue; }
		}

		if ( CAMERA_IMAGE_GRAY == CameraImageMode )
		{
			if ( FRAME_BAYER == FrameParamPtr->FrameType )
			{	continue; }			
		}
		else if ( CAMERA_IMAGE_BAYER == CameraImageMode )
		{
			if ( FRAME_BAYER != FrameParamPtr->FrameType )
			{	continue; }			
		}
		else if ( CAMERA_IMAGE_COLOR == CameraImageMode )
		{
		}

		str = FrameParamPtr->FrameName;
		Param = FrameParamPtr->FrameUniqueID;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetBarcodeDeviceName(BARCODE_DEVICE_TYPE type)
{
	CString str;
	switch ( type )
	{
	case BARCODE_DEVICE_NULL:					 str = _T("Disabled");	break;

	case BARCODE_DEVICE_MICROSCAN_MS3:	         str = _T("Microscan_MS3");	break;
	case BARCODE_DEVICE_MICROSCAN_MINI:	         str = _T("Microscan_MINI");	break;
	case BARCODE_DEVICE_MICROSCAN_MINI_VELOCITY: str = _T("Microscan_MINI_VELOCITY");	break;		
	case BARCODE_DEVICE_MICROSCAN_MINI3:         str = _T("Microscan_MINI3");	break;		
	case BARCODE_DEVICE_MICROSCAN_MINI_HAWK:     str = _T("Microscan_MINI_HAWK");	break;

	case BARCODE_DEVICE_DATALOGIC_M1000:		str = _T("Datalogic_M1000");	break;		
	case BARCODE_DEVICE_DATALOGIC_MATRIX_200:	str = _T("Datalogic_Matrix200");	break;
	case BARCODE_DEVICE_DATALOGIC_MATRIX_210:	str = _T("Datalogic_Matrix210");	break;
	case BARCODE_DEVICE_DATALOGIC_GFS4400:		str = _T("Datalogic_GFS4400");	break;
	case BARCODE_DEVICE_DATALOGIC_MATRIX_210N:	str = _T("Datalogic_Matrix210N");	break;

	case BARCODE_DEVICE_HONEYWELL_3310GHD:		str = _T("Honeywell_3310GHD");	break;
	case BARCODE_DEVICE_HONEYWELL_1900:			str = _T("Honeywell_1900");	break;	
	
	case BARCODE_DEVICE_KEYENCE_SR2000:         str = _T("Keyence SR2000");	break;	
	case BARCODE_DEVICE_KEYENCE_SR751:			str = _T("Keyence_SR751"); break;

	case BARCODE_DEVICE_SICK_442:				str = _T("Sick_442");	break;	
	case BARCODE_DEVICE_SICK_ICR840:			str = _T("Sick_ICR840");	break;	
	
	case BARCODE_DEVICE_OTHER_AZUREWAVE:		str = _T("Azurewave");	break;	

	case BARCODE_DEVICE_GENERAL_GROUP:  		str = _T("General");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_01:		str = _T("General_01");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_02:		str = _T("General_02");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_03:		str = _T("General_03");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_04:		str = _T("General_04");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_05:		str = _T("General_05");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_06:		str = _T("General_06");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_07:		str = _T("General_07");	break;	
	case BARCODE_DEVICE_GENERAL_DEVICE_08:		str = _T("General_08");	break;	
	default:
		str.Format(_T("Undefined [%d]"), type);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeDeviceIDCombox(CComboBox &Combox, bool IncDisable)
{
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;
	const size_t MaxCount = 8;

	idx = 0;
	JetAPI::ClearCombox(Combox);

	if ( true == IncDisable )
	{
		Param = BARCODE_DEVICE_ID_OFF;
		str = _T("Disable");
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}
	for ( i=0; i<MaxCount; i++ )
	{
		switch ( i )
		{
		case 0:	Param=BARCODE_DEVICE_ID_01;	break;
		case 1:	Param=BARCODE_DEVICE_ID_02;	break;
		case 2:	Param=BARCODE_DEVICE_ID_03;	break;
		case 3:	Param=BARCODE_DEVICE_ID_04;	break;
		case 4:	Param=BARCODE_DEVICE_ID_05;	break;
		case 5:	Param=BARCODE_DEVICE_ID_06;	break;
		case 6:	Param=BARCODE_DEVICE_ID_07;	break;
		case 7:	Param=BARCODE_DEVICE_ID_08;	break;		
		default: Param = 0; break;
		}
		str.Format(_T("%d"), Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildBarcodeDeviceTypeCombox(CComboBox &Combox)
{
	std::vector<BARCODE_DEVICE_TYPE> TypeList;	
	TypeList.push_back(BARCODE_DEVICE_NULL);
#ifndef BARCODE_DEVICE_MODULE
	TypeList.push_back(BARCODE_DEVICE_HONEYWELL_3310GHD);
	TypeList.push_back(BARCODE_DEVICE_DATALOGIC_MATRIX_210);
	TypeList.push_back(BARCODE_DEVICE_DATALOGIC_MATRIX_210N);
	TypeList.push_back(BARCODE_DEVICE_KEYENCE_SR2000);
	TypeList.push_back(BARCODE_DEVICE_KEYENCE_SR751);
	TypeList.push_back(BARCODE_DEVICE_OTHER_AZUREWAVE);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_01);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_02);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_03);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_04);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_05);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_06);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_07);
	TypeList.push_back(BARCODE_DEVICE_GENERAL_DEVICE_08);
	//TypeList.push_back(AAAAAAAAAAAAAA);	
#else
	TypeList.push_back(BARCODE_DEVICE_MICROSCAN_MS3);
	TypeList.push_back(BARCODE_DEVICE_MICROSCAN_MINI);
	TypeList.push_back(BARCODE_DEVICE_MICROSCAN_MINI_VELOCITY);
	TypeList.push_back(BARCODE_DEVICE_MICROSCAN_MINI3);
	TypeList.push_back(BARCODE_DEVICE_MICROSCAN_MINI_HAWK);
	//TypeList.push_back(BARCODE_DEVICE_DATALOGIC_M1000);
	TypeList.push_back(BARCODE_DEVICE_DATALOGIC_MATRIX_200);
	TypeList.push_back(BARCODE_DEVICE_DATALOGIC_MATRIX_210);
	//TypeList.push_back(BARCODE_DEVICE_DATALOGIC_GFS4400);
	TypeList.push_back(BARCODE_DEVICE_HONEYWELL_3310GHD);
	TypeList.push_back(BARCODE_DEVICE_HONEYWELL_1900);
	TypeList.push_back(BARCODE_DEVICE_OTHER_AZUREWAVE);	
	//TypeList.push_back(BARCODE_DEVICE_SICK_442);
	//TypeList.push_back(BARCODE_DEVICE_SICK_ICR840);
	//TypeList.push_back(AAAAAAAAAAAAAA);
#endif//BARCODE_DEVICE_MODULE

	CString      str;	
	int          idx=0;	
	BARCODE_DEVICE_TYPE Type;
	const size_t TypeCount=TypeList.size();

	JetAPI::ClearCombox(Combox);	
	for ( size_t i=0; i<TypeCount; i++ )
	{
		Type = TypeList[i];
		str = GetBarcodeDeviceName(Type);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Type);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::LoadMultiLanguageString_BarcodeDevice(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("BARCODE_DEVICE_BASIC");
	GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::LoadMultiLanguageString_BarcodeHandHeld(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("BARCODE_HANDHELD");
	GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSwitchModelItemCombox(CComboBox &Combox)//切換模組項目列表
{
	size_t       i=0;
	int          idx=0;
	CString      Default;
	CString      KeyName;	
	CString      String;
	CString      Section = _T("SWITCH_MODEL_ITEM_MODE");
	SWITCH_MODEL_ITEM_MODE MoveNextModelType;
	idx = 0;
	JetAPI::ClearCombox(Combox);	
	
	Default = _T("Any");
	KeyName = _T("SWITCH_MODEL_ITEM_ANY");
	MoveNextModelType = SWITCH_MODEL_ITEM_ANY;
	GetUILanguageString(Section, KeyName, Default, String);		
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MoveNextModelType);
	idx ++;
	
	Default = _T("Set");
	KeyName = _T("SWITCH_MODEL_ITEM_SET");
	MoveNextModelType = SWITCH_MODEL_ITEM_SET;
	GetUILanguageString(Section, KeyName, Default, String);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MoveNextModelType);
	idx ++;

	Default = _T("UnSet");
	KeyName = _T("SWITCH_MODEL_ITEM_UNSET");
	MoveNextModelType = SWITCH_MODEL_ITEM_UNSET;
	GetUILanguageString(Section, KeyName, Default, String);	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MoveNextModelType);
	idx ++;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSpaceMergeModeCombox(CComboBox &Combox)//空間合併模式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = _T("Mass");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_MASS);
	idx ++;

	String = _T("Lowest");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_LOWEST);
	idx ++;

	String = _T("Mass2");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_MASS_2);
	idx ++;	

	String = _T("Mass3");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_MASS_3);
	idx ++;	

	String = _T("Mass4");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_MASS_4);
	idx ++;	

	String = _T("Median");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_MEDIAN);
	idx ++;	

	String = _T("Joe1");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_MODE_JOE_1);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSpaceMergeBestModeCombox(CComboBox &Combox)//空間合併最可靠模式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = _T("Best-2");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_BEST_MODE_02);
	idx ++;

	String = _T("Best-3");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_CAST_MERGE_BEST_MODE_03);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSpaceMergeIntensityModeCombox(CComboBox &Combox)//空間合併亮度模式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = _T("Mean");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_INTENSITY_MERGE_MODE_MEAN);
	idx ++;

	String = _T("MaxB");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_INTENSITY_MERGE_MODE_MAXB);
	idx ++;

	String = _T("MaxCV");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, MULTI_INTENSITY_MERGE_MODE_MAXCV);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildCastSpaceFilterModeCombox(CComboBox & Combox)//投光後空間濾波(Cuda)模式列表
{
	size_t       i = 0;
	int          idx = 0;
	CString      String;
	if (Combox.GetSafeHwnd() == NULL) { return false; }

	idx = 0;
	JetAPI::ClearCombox(Combox);

	String = _T("Default");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, CAST_SPACE_FILTER_MODE_DEFAULT);
	idx++;

	String = _T("Spot");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, CAST_SPACE_FILTER_MODE_SPOT);
	idx++;

	String = _T("Small");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, CAST_SPACE_FILTER_MODE_SMALL);
	idx++;

	String = _T("Medium");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, CAST_SPACE_FILTER_MODE_MEDIUM);
	idx++;

	String = _T("Large");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, CAST_SPACE_FILTER_MODE_LARGE);
	idx++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSpaceNoiseDefineModeCombox(CComboBox &Combox)//空間雜訊定義模式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = _T("1");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, PHASE_NOISE_DEF_MODE_1);
	idx ++;

	String = _T("2");
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, PHASE_NOISE_DEF_MODE_2);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuidlSpaceNoiseFilterModeCombox(CComboBox &Combox)//切換空間雜訊過濾模式列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = m_NoiseFilterText_Diable;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_DISABLE);
	idx ++;

	String = m_NoiseFilterText_Smooth;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_AVERAGE);
	idx ++;

	String = m_NoiseFilterText_Median;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_MEDIAN);
	idx ++;

	String = m_NoiseFilterText_Level;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_3LEVEL);
	idx ++;

	String = m_NoiseFilterText_Median2;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_MEDIAN_2);
	idx ++;

	String = m_NoiseFilterText_PyramidMedian;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_PYRAMID_MEDIAN);
	idx++;

	String = m_NoiseFilterText_ContentAware;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_CONTENTAWARE);
	idx++;

	String = m_NoiseFilterText_Fast_Median;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_FAST_MEDIAN);
	idx++;

	String = m_NoiseFilterText_Fast_Average;
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, DATA_NF_FAST_AVERAGE);
	idx++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildPhaseHeightFactorNumCombox(CComboBox &Combox)//相位高度係數第幾個列表
{
	size_t       i=0;
	int          idx=0;	
	CString      String;
	const size_t MaxI=MAX_HEIGHT_TARGET_COUNT;
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	for ( i=0; i<=MaxI; i++ )
	{
		if ( 0 == i )
		{	String = _T("---");	}
		else
		{	String.Format(_T("%d"), i); }
		Combox.InsertString(-1, String);
		Combox.SetItemData(idx, i);
		idx ++;
	}
	Combox.SetCurSel(0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildPhaseConvertHeightModeCombox(CComboBox &Combox)//相位轉高度的模式
{
	size_t       i=0;	
	int          idx=0;	
	int          Data=0;
	CString      String;	
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);			
	
	Data = PHASE_CONVERT_HEIGHT_SCALE;
	String.Format(_T("%02d-Scale"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	/*
	Data = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_3;
	String.Format(_T("%02d-Mapping"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	Data = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_4;
	String.Format(_T("%02d-Mapping"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	Data = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_5;
	String.Format(_T("%02d-Mapping"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	Data = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_6;
	String.Format(_T("%02d-Mapping"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	*/
	Data = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_7;
	String.Format(_T("%02d-Mapping"), Data);
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildSpaceHeightCorrectModelCombox(CComboBox &Combox)//高度校正模式列表
{
	size_t       i=0;	
	int          idx=0;	
	int          Data=0;
	CString      String;	
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }
	
	idx = 0;
	JetAPI::ClearCombox(Combox);			
	
	String = GetDisableText();
	Data = HEIGHT_DATA_CORRECT_NONE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = GetEnableText();
	Data = HEIGHT_DATA_CORRECT_ENABLE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
CString  CAOIDataDefine::GetMESStatusText(int StatusID)
{
	CString str;
	switch ( StatusID )
	{
	case MES_STATAUS_NONE: str = _T("None");	break;

	case MES_STATAUS_MES_SET_CMD: str = _T("MES Set Parameters");	break;
	case MES_STATAUS_MES_SET_RES: str = _T("MES Set Parameters Response");	break;
	case MES_STATAUS_MES_GET_CMD: str = _T("MES Get Parameters");	break;
	case MES_STATAUS_MES_GET_RES: str = _T("MES Get Parameters Response");	break;

	case MES_STATAUS_AOI_SET_CMD: str = _T("AOI Set Parameters");	break;
	case MES_STATAUS_AOI_SET_RES: str = _T("AOI Set Parameters Response");	break;
	case MES_STATAUS_AOI_GET_CMD: str = _T("AOI Get Parameters");	break;
	case MES_STATAUS_AOI_GET_RES: str = _T("AOI Get Parameters Response");	break;

	case MES_STATAUS_VRS_SET_CMD: str = _T("VRS Set Parameters");	break;
	case MES_STATAUS_VRS_SET_RES: str = _T("VRS Set Parameters Response");	break;
	case MES_STATAUS_VRS_GET_CMD: str = _T("VRS Get Parameters");	break;
	case MES_STATAUS_VRS_GET_RES: str = _T("VRS Get Parameters Response");	break;

	case MES_STATAUS_AOI_READY_TO_LOAD_CMD: str = _T("AOI Ready To Load");	break;
	case MES_STATAUS_AOI_READY_TO_LOAD_RES: str = _T("AOI Ready To Load Response");	break;

	case MES_STATAUS_AOI_LOAd_COMPLETE_CMD: str = _T("AOI Load Complete");	break;
	case MES_STATAUS_AOI_LOAd_COMPLETE_RES: str = _T("AOI Load Complete Response");	break;

	case MES_STATAUS_AOI_START_INSPECTION_CMD: str = _T("AOI Start Inspection");	break;
	case MES_STATAUS_AOI_START_INSPECTION_RES: str = _T("AOI Start Inspection Response");	break;

	case MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD: str = _T("AOI Inspection Complete");	break;
	case MES_STATAUS_AOI_INSPECTION_COMPLETE_RES: str = _T("AOI Inspection Complete Response");	break;

	case MES_STATAUS_AOI_READY_TO_UNLOAD_CMD: str = _T("AOI Ready To Unload");	break;
	case MES_STATAUS_AOI_READY_TO_UNLOAD_RES: str = _T("AOI Ready To Unload Response");	break;

	case MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD: str = _T("AOI Unload Complete");	break;
	case MES_STATAUS_AOI_UNLOAd_COMPLETE_RES: str = _T("AOI Unload Complete Response");	break;

	case MES_STATAUS_AOI_LOGIN_OUT_CMD: str = _T("AOI Login/Out Parameters");	break;
	case MES_STATAUS_AOI_LOGIN_OUT_RES: str = _T("AOI Login/Out Parameters Response");	break;		

	case MES_STATAUS_AOI_CHECK_BARCODE_CMD: str = _T("AOI Check Barcode");	break;
	case MES_STATAUS_AOI_CHECK_BARCODE_RES: str = _T("AOI Check Barcode Response");	break;		

	case MES_STATAUS_AOI_INSPECTION_STOP_CMD: str = _T("AOI Inspection Stop");	break;
	case MES_STATAUS_AOI_INSPECTION_STOP_RES: str = _T("AOI Inspection Stop Response");	break;

	case MES_STATAUS_AOI_PARAM_CHANGE_CMD: str = _T("AOI Param Change");	break;
	case MES_STATAUS_AOI_PROJECT_OPEN: str = _T("AOI Project Open");	break;
	case MES_STATAUS_AOI_PROJECT_LOAD_FINISH: str = _T("AOI Project Load Finish");	break;
	case MES_STATAUS_AOI_ALARM_CLEAR: str = _T("AOI Alarm Clear");	break;
	case MES_STATAUS_AOI_EDIT_MODE: str = _T("AOI Edit Mode");	break;
	case MES_STATAUS_AOI_ONLINE_TEST: str = _T("AOI Online Test");	break;

	case MES_STATAUS_VRS_UPLOAD_SFC_CMD: str = _T("VRS Upload SFC");	break;
	case MES_STATAUS_VRS_UPLOAD_SFC_RES: str = _T("VRS Upload SFC Response");	break;		

	default:
		str.Format(_T("Undefined Status ID[%d]"), StatusID);
		break;
	}	          
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPartGroupTargetModeText(PART_GROUP_TARGET_MODE Mode)
{
	CString str;
	switch ( Mode )
	{
	case PART_GROUP_TARGET_AVE: str = _T("Ave"); break;
	case PART_GROUP_TARGET_MIN: str = _T("Min"); break;	
	default:
		str.Format(_T("Undefined %d"), Mode);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetPartGroupColinearityeModeText(PART_GROUP_COLINEARITY_MODE Mode)
{
	CString str;
	switch ( Mode )
	{
	case PART_GROUP_COLINEARITY_X: str = _T("X"); break;
	case PART_GROUP_COLINEARITY_Y: str = _T("Y"); break;
	case PART_GROUP_COLINEARITY_SKEW: str = _T("Skew"); break;	
	case PART_GROUP_COLINEARITY_HEIGHT: str = _T("Height"); break; 
	default:
		str.Format(_T("Undefined %d"), Mode);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetOnlineAutoStopBySpecTimeText(__int64 Time)
{
	CString str;
	if ( 0 == Time )
	{	str = GetDisableText(); }
	else
	{	JetAPI::FormatTime(FORMAT_TIME_HH_MM, CTime(Time), str);	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetMESContactSoftwareName(int nContact)//取得與MES對接軟體名稱
{
	CString str;
	switch ( nContact )
	{
	case MES_CONTACT_IBS: str=_T("IBS");	break;
	case MES_CONTACT_IPS: str=_T("IPS");	break;
	case MES_CONTACT_MTS: str=_T("MTS");	break;
	default:
		str=_T("ITS");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataDefine::BuildDataModeLevelList(std::vector<int> &List)//建立資料模型等級列表
{
	List.clear();
	List.push_back(1);
	List.push_back(2);
	List.push_back(3);
	List.push_back(4);
	List.push_back(5);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAS608StatusText(AS608_STATUS status)
{
	CString str;
	switch (status)
	{
	case(AS608_STATUS_OK):					str = m_AS608StatusText_OK;				break;
	case(AS608_STATUS_FRAME_ERROR):			str = m_AS608StatusText_Error;			break;
	case(AS608_STATUS_NO_FINGERPRINT):		str = m_AS608StatusText_NOFingerprint;	break;
	case(AS608_STATUS_INPUT_ERROR):			str = m_AS608StatusText_InputError;		break;
	case(AS608_STATUS_IMAGE_TOO_DRY):		str = m_AS608StatusText_ImageTooDry;	break;
	case(AS608_STATUS_IMAGE_TOO_WET):		str = m_AS608StatusText_ImageTooWet;	break;
	case(AS608_STATUS_IMAGE_TOO_CLUTTER):	str = m_AS608StatusText_ImageTooClutter;break;
	case(AS608_STATUS_IMAGE_TOO_FEW_FEATURE):	str = m_AS608StatusText_ImageTooFewFeature;		break;
	case(AS608_STATUS_NOT_MATCH):			str = m_AS608StatusText_NotMatch;		break;
	case(AS608_STATUS_NO_MOVE):				str = m_AS608StatusText_NoMove;			break;
	case(AS608_STATUS_FEATURE_COMBINE_ERROR):	str = m_AS608StatusText_FeatureCombineError;	break;
	default:
		str.Format(L"error code: %d", status);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetAS608CommandText(AS608_COMMAND command)
{
	CString str;
	switch (command)
	{
	case(AS608_COMMAND_GETIMAGE):		str = m_AS608CommandText_GetImage;		break;
	case(AS608_COMMAND_GENCHAR):		str = m_AS608CommandText_GenChar;		break;
	case(AS608_COMMAND_MATCH):			str = m_AS608CommandText_Match;			break;
	case(AS608_COMMAND_REGMODEL):		str = m_AS608CommandText_RegMode;		break;
	case(AS608_COMMAND_UPCHAR):			str = m_AS608CommandText_UpChar;		break;
	case(AS608_COMMAND_DOWNCHAR):		str = m_AS608CommandText_DownChar;		break;
	default:
		str.Format(L"command: %d", command);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataDefine::GetFingerPrintConfigFilename() const
{
	CString Folder;
	CString filename;
	Folder = AOIDataCollect.GetAOIDirectory();
	filename.Format(_T("%s\\%s"), Folder, _T("FPSConfig.INI"));
	return filename;
}
//-------------------------------------------------------------------------------------//