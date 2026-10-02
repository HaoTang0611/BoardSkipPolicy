// CaliPaneAlign.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CaliPaneAlign.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
#include "InputBoxWnd.h"
#include "MotionCtrlWnd.h"
#include "ImagePhaseWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
#include "SpaceBaseParamWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#define   LED_CURRENT_WARNING_MAX      98
#define   LED_CURRENT_WARNING_MIN      02
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
COLORREF  GridColorList[10]=
{
	0x00FFFF, 0x20FFFF, 0x40FFFF,
	0x60FFFF, 0x80FFFF, 0xA0FFFF,
	0xC0FFFF, 0xD0FFFF, 0xF0FFFF, 
	0xFFFFFF
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneAlign dialog
//-------------------------------------------------------------------------------------//
CCaliPaneAlign::CCaliPaneAlign(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliPaneAlign::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaliPaneAlign)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	this->m_BKColor = 0xE0E0E0;//0xAFFFFF;
	this->m_ZeroPhaseOffsetPosZ = 0.0;	
	this->m_CameraExposureTime_us = 3000;
	this->m_CameraExposureTimeBackup_us = m_CameraExposureTime_us;
	this->m_3DCastCurrent = 50;
	this->m_LightNum = 0;
	this->m_CameraID = PRIMARY_CAMERA_ID;
	this->m_PatternStep = BATCH_GRAB_STEP_1;
	this->m_PhaseID = BATCH_GRAB_PHASE_1;	
	m_SliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;
	this->m_Light3DCastID = LIGHT_3D_CAST_00;	
	this->m_PatternProjectMode = 0;	
	this->m_CameraImageReceieveCount = 0;
	m_ExtraDelayTime = 500;
	m_SaveRawImageTimes = 0;
	m_AveRoiR = 0;
	m_AveRoiG = 0;
	m_AveRoiB = 0;	
	m_AveGrayR = 0;
	m_AveGrayG = 0;
	m_AveGrayB = 0;
	this->m_ShowBuffer = NULL;
	this->m_ShowBuffer1 = NULL;	
	this->m_MaskBuffer = NULL;
	this->m_MaskBuffer1 = NULL;
	this->m_MaskBuffer2 = NULL;
	this->m_PhaseBuffer = NULL;
	this->m_PhaseBuffer1 = NULL;
	this->m_PhaseBuffer2 = NULL;
	this->m_ImageBuffer = NULL;
	this->m_ImageBuffer1 = NULL;
	this->m_ImageBuffer2 = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_SpaceBuffer1 = NULL;
	this->m_SpaceBuffer2 = NULL;
	this->m_ShowBufferSize = 0;
	this->m_ImageBufferSize = 0;
	this->m_SpaceBufferSize = 0;

	this->m_ImgTargetW = 0;
	this->m_ImgTargetH = 0;
	this->m_ImgTargetStep = 0;

	this->m_StagePosX = 0;
	this->m_StagePosY = 0;
	this->m_StagePosZ = 0;

	this->m_ImagePt1.x = -1;
	this->m_ImagePt1.y = -1;
	this->m_ImagePt2.x = -1;
	this->m_ImagePt2.y = -1;
	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;

	this->m_PhaseAngle = 0;//相位角度
	this->m_PhaseRange = 0;//相位範圍
	this->m_PhaseLinePt1.x = -1;
	this->m_PhaseLinePt1.y = -1;
	this->m_PhaseLinePt2.x = -1;
	this->m_PhaseLinePt2.y = -1;
	this->m_PhaseNormPt1 = m_PhaseLinePt1;
	this->m_PhaseNormPt2 = m_PhaseLinePt2;

	this->m_bShowCenterLine = FALSE;
	this->m_bShowHorizontalLine = FALSE;
	this->m_bShowVerticalLine = FALSE;
	m_ModifiedCaliParam = false;

	this->m_ImageZoom = 1.0;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;	

	m_CalibrateAllCannel = false;
	m_CalibrateAllCastID = false;
	this->m_3DCastFocusLT = 0;//3D投光焦距-左上
	this->m_3DCastFocusRT = 0;//3D投光焦距-右上
	this->m_3DCastFocusLB = 0;//3D投光焦距-左下
	this->m_3DCastFocusRB = 0;//3D投光焦距-右下
	this->m_3DCastFocusCC = 0;//3D投光焦距-中央
	this->m_3DCastFocusCCMax = 0;//3D投光焦距-中央最大值

	this->m_ImageGridRows = 20;
	this->m_ImageGridCols = 20;
	this->m_AutoFocusPitch = 10;
	this->m_AutoFocustLastPosZ = 0.0;
	this->m_AutoFocustMinPosZ = 0.0;
	this->m_AutoFocustMaxPosZ = 0.0;
	this->m_AutoFocustScalePosZ = 1.0;

	this->m_PhaseFactorRows = 9;
	this->m_PhaseFactorCols = 9;

	this->m_Multi3DCastID = false;
	this->m_3DCastCurRed = 0;//3D投光電流-Red
	this->m_3DCastCurGrn = 0;//3D投光電流-Grn
	this->m_3DCastCurBlu = 0;//3D投光電流-Blu	
	this->m_3DCastExposureTimeus = 0;

	this->m_ImageToStageScaleX = 1.0;//影像轉機台的比例-X
	this->m_ImageToStageScaleY = 1.0;//影像轉機台的比例-Y	
	this->m_StageToImageScaleX = 1.0;//機台轉影像的比例-X
	this->m_StageToImageScaleY = 1.0;//機台轉影像的比例-Y

	this->m_GrayTarget = 200;//灰階目標
	this->m_GrayMinRatio = 90.0;//灰階誤差範圍		

	m_MultiZeroPlaneGapRatio = 10;
	m_MultiZeroPlaneGapThreshold = 50;

	m_CalibrationMode = CALIBRATION_STOP;
	m_ManipulateMode = CALIALIGN_MANIPULATE_NONE;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaliPaneAlign)
	DDX_Control(pDX, CALIALIGN_NOISE_DEFINE_MODE_COMBO, m_NoiseDefineModeCombox);
	DDX_Control(pDX, CALIALIGN_PARAM_PHASE_TO_HEIGHT_COMBO, m_PhaseToHeightCombox);
	DDX_Control(pDX, CALIALIGN_PHASE_HEIGHT_FACTOR_NUM_COMBO, m_HeightFactorNumCombox);
	DDX_Control(pDX, CALIALIGN_3D_CAST_CURRENT_ID_COMBO, m_3DCastCurrentIDCombox);
	DDX_Control(pDX, CALIALIGN_SAVE_RAW_EXT_NAME_COMBO, m_SaveRawExtNameCombox);
	DDX_Control(pDX, CALIALIGN_MULTI_CAST_MERGE_MODE_COMBO, m_SpaceMergeModeCombox);
	DDX_Control(pDX, CALIALIGN_MULTI_CAST_MERGE_BEST_MODE_COMBO, m_SpaceMergeBestModeCombox);
	DDX_Control(pDX, CALIALIGN_MULTI_INTENSITY_MERGE_MODE_COMBO, m_SpaceMergeIntensityModeCombox);	
	DDX_Control(pDX, CALIALIGN_CAST_FILTER_MODE_COMBO, m_CastSpaceFilterModeCombox);
	DDX_Control(pDX, CALIALIGN_NOISE_FIRST_FILTER_MODE_COMBO, m_FirstFilterModeCombox);
	DDX_Control(pDX, CALIALIGN_NOISE_FILTER_OVER_LOW_MODE_COMBO, m_OverLowModeCombox);
	DDX_Control(pDX, CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_MODE_COMBO, m_HeightVarModeCombox);
	DDX_Control(pDX, CALIALIGN_NOISE_FINAL_FILTER_MODE_COMBO, m_FinalFilterModeCombox);
	DDX_Control(pDX, CALIALIGN_NOISE_FINAL_FILTER_MODE_COMBO2, m_FinalFilterModeCombox2);
	DDX_Control(pDX, CALIALIGN_SLICE_COMBO, m_SliceCombo);
	DDX_Control(pDX, CALIALIGN_FOV_RESOLUTION_COMBO, m_FovResCombox);
	DDX_Control(pDX, CALIALIGN_LOG_LIST_BOX_WND, m_LogListBox);
	DDX_Control(pDX, CALIALIGN_IMAGE_TARGET_WND, m_ImageTargetWnd);
	DDX_Control(pDX, CALIALIGN_PHASE_ID_COMBOX, m_PhaseIDCombox);
	DDX_Control(pDX, CALIALIGN_PHASE_LED_COMBOX, m_PhaseLEDCombox);	
	DDX_Control(pDX, CALIALIGN_MANIPULATE_COMBO, m_ManipulateCombox);
	DDX_Control(pDX, CALIALIGN_3D_CAST_ID_COMBO, m_3DCastIDCombox);
	DDX_Control(pDX, CALIALIGN_LIGHT_COMBO, m_LightCombox);
	DDX_Control(pDX, CALIALIGN_CAMERA_COMBO, m_CameraCombox);
	DDX_Control(pDX, CALIALIGN_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaliPaneAlign, CDialog)
	//{{AFX_MSG_MAP(CCaliPaneAlign)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(CALIALIGN_CAMERA_COMBO, OnSelchangeCameraCombo)
	ON_BN_CLICKED(CALIALIGN_GRAB_BTN, OnGrabBtn)
	ON_BN_CLICKED(CALIALIGN_FOV_WIDTH_BTN, OnFOVWidthBtn)
	ON_BN_CLICKED(CALIALIGN_FOV_HEIGHT_BTN, OnFOVHeightBtn)
	ON_BN_CLICKED(CALIALIGN_CAMERA_ALIGN_HOR_BTN, OnCameraAlignHorBtn)
	ON_BN_CLICKED(CALIALIGN_CAMERA_ALIGN_VER_BTN, OnCameraAlignVerBtn)	
	ON_BN_CLICKED(CALIALIGN_IMAGE_RESOLUTION_CALC_BTN, OnImageResolutionCalcBtn)
	ON_BN_CLICKED(CALIALIGN_IMAGE_FOCUS_AUTO_BTN, OnImageFocusAutoBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(CALIALIGN_LIGHT_ALIGN_BTN, OnLightAlignBtn)
	ON_BN_CLICKED(CALIALIGN_3DCAST_ALIGN_BTN, On3DCastAlignBtn)
	ON_BN_CLICKED(CALIALIGN_3DCAST_FOCUS_BTN, On3DCastFocusBtn)
	ON_WM_MOUSEWHEEL()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(CALIALIGN_MANIPULATE_COMBO, OnSelchangeManipulateCombo)
	ON_BN_CLICKED(CALIALIGN_GRAB_REPEAT_CHK, OnGrabRepeatChk)
	ON_BN_CLICKED(CALIALIGN_LIGHT_CURRENT_BTN, OnLightCurrentBtn)
	ON_BN_CLICKED(CALIALIGN_3D_CAST_CURRENT_BTN, On3DCastCurrentBtn)
	ON_CBN_SELCHANGE(CALIALIGN_LIGHT_COMBO, OnSelchangeLightCombo)
	ON_CBN_SELCHANGE(CALIALIGN_3D_CAST_ID_COMBO, OnSelchange3DCastIDCombo)
	ON_BN_CLICKED(CALIALIGN_PHASE_PERIOD_BTN, OnPhasePeriodBtn)	
	ON_BN_CLICKED(CALIALIGN_PHASE_ZERO_PLANE_BTN, OnPatternZeroPlaneBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_HEIGHT_FACTOR_BTN, OnPatternHeightFactorBtn)
	ON_BN_CLICKED(CALIALIGN_VIEW_RESET_BTN, OnViewResetBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_GRID_GO_BTN, OnTargetGridGoBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_GRID_SET_BTN, OnTargetGridSetBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_GRID_EXP_BTN, OnTargetGridExpBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_WHITE_GO_BTN, OnTargetWhiteGoBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_WHITE_SET_BTN, OnTargetWhiteSetBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_WHITE_EXP_BTN, OnTargetWhiteExpBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_HEIGHT_GO_BTN, OnTargetHeightGoBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_HEIGHT_SET_BTN, OnTargetHeightSetBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_HEIGHT_EXP_BTN, OnTargetHeightExpBtn)
	ON_BN_CLICKED(CALIALIGN_PATTERN_PHASE_MEASURE_CHK, OnPatternPhaseMeasureChk)
	ON_BN_CLICKED(CALIALIGN_SAVE_IMAGE_BTN, OnSaveImageBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_BTN, OnPatternHeightFactorShowBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_IMAGE_BTN, OnPatternImageBtn)
	ON_BN_CLICKED(CALIALIGN_VERIFY_ZERO_PLANE_BTN, OnVerifyZeroPlaneBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_ZERO_PLANE_SHOW_BTN, OnPatternZeroPlaneShowBtn)
	ON_BN_CLICKED(CALIALIGN_MOTION_WND_BTN, OnMotionWndBtn)
	ON_NOTIFY(UDN_DELTAPOS, CALIALIGN_MOTION_Z_PITCH_SPIN, OnDeltaposMotionZPitchSpin)
	ON_BN_CLICKED(CALIALIGN_XYZ_ORG_BTN, OnXYZOrgBtn)
	ON_BN_CLICKED(CALIALIGN_PCB_IN_BTN, OnPCBInBtn)
	ON_BN_CLICKED(CALIALIGN_PCB_OUT_BTN, OnPCBOutBtn)
	ON_BN_CLICKED(CALIALIGN_PCB_BACK_BTN, OnPCBBackBtn)
	ON_BN_CLICKED(CALIALIGN_PCB_CLAMP_ON_BTN, OnPCBClampOnBtn)
	ON_BN_CLICKED(CALIALIGN_PCB_CLAMP_OFF_BTN, OnPCBClampOffBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_CAP_ON_BTN, OnTargetCapOnBtn)
	ON_BN_CLICKED(CALIALIGN_TARGET_CAP_OFF_BTN, OnTargetCapOffBtn)
	ON_CBN_SELCHANGE(CALIALIGN_FOV_RESOLUTION_COMBO, OnSelchangeFovResolutionCombo)
	ON_BN_CLICKED(CALIALIGN_VIEW_ALL_BTN, OnViewAllBtn)
	ON_BN_CLICKED(CALIALIGN_VIEW_1X1_BTN, OnView1x1Btn)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(CALIALIGN_CAMERA_EXPOSURE_TIME_BTN, OnCameraExposureTimeBtn)	
	ON_BN_CLICKED(CALIALIGN_SHOW_CENTER_LINE_CHK, OnShowCenterLineChk)
	ON_BN_CLICKED(CALIALIGN_SHOW_HOR_LINE_CHK, OnShowHorLineChk)
	ON_BN_CLICKED(CALIALIGN_SHOW_VER_LINE_CHK, OnShowVerLineChk)	
	ON_BN_CLICKED(CALIALIGN_HIDE_LINE_BTN, OnHideLineBtn)
	ON_CBN_SELCHANGE(CALIALIGN_SLICE_COMBO, OnSelchangeSliceCombo)
	ON_BN_CLICKED(CALIALIGN_DLP_EXPOSURE_TIME_BTN, OnDLPExposureTimeBtn)
	ON_BN_CLICKED(CALIALIGN_3D_CAST_CURRENT_SET_BTN, On3DCastCurrentSetBtn)
	ON_BN_CLICKED(CALIALIGN_LIGHT_CURRENT_SET_BTN, OnLightCurrentSetBtn)
	ON_CBN_SELCHANGE(CALIALIGN_PHASE_LED_COMBOX, OnSelchangePhaseLedCombox)
	ON_BN_CLICKED(CALIALIGN_BASE_PLANE_PARAM_BTN, OnBasePlaneParamBtn)
	ON_BN_CLICKED(CALIALIGN_SPACE_NOISE_FILTER_BTN, OnSpaceNoiseFilterBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_HEIGHT_FACTOR_SET_BTN, OnPhaseHeightFactorSetBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_HEIGHT_FACTOR_CLEAR_BTN, OnPhaseHeightFactorClearBtn)
	ON_BN_CLICKED(CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_TABLE_BTN, OnPhaseHeightFactorShowTableBtn)	
	ON_BN_CLICKED(CALIALIGN_PARAM_FILE_RELOAD_BTN, OnParamFileReloadBtn)
	ON_CBN_SELCHANGE(CALIALIGN_3D_CAST_CURRENT_ID_COMBO, OnSelchange3DCastCurrentIDCombo)	
	ON_CBN_SELCHANGE(CALIALIGN_PARAM_PHASE_TO_HEIGHT_COMBO, OnSelchangeParamPhaseToHeightCombo)
	ON_BN_CLICKED(CALIALIGN_SHOW_RAW_IMAGE_FOLDER_BTN, OnShowRawImageFolderBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneAlign message handlers
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneAlign::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	const int DLPLEDCurrentID=DLP_LED_CURRENT_ID_01;
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	const double ShowScale = AOIDataCollect.GetSystemParameter().m_ResolutionShowScale;
	AOIDataDefine.BuildSystemSliceParamCombox(m_SliceCombo, false, false, false);
	CLight3DCtrl::BuildCastIDCombox(m_3DCastIDCombox, true);	
	CCameraCtrl::BuildCameraIDCombox(m_CameraCombox);
	AOIDataCollect.BuildFOVResolutionCombox(m_FovResCombox, ShowScale);
	CCameraCtrl::BuildBatchGrabLightNumCombox(m_LightCombox);			
	CCameraCtrl::BuildBatchGrabPhaseIDCombox(m_PhaseIDCombox);	
	AOIDataDefine.BuildDLPPhaseLEDColorCombox(m_PhaseLEDCombox, false);	
	AOIDataDefine.BuildDLPLEDCurrentIDCombox(m_3DCastCurrentIDCombox);
	AOIDataDefine.BuidlSpaceMergeModeCombox(m_SpaceMergeModeCombox);
	AOIDataDefine.BuidlSpaceMergeBestModeCombox(m_SpaceMergeBestModeCombox);
	AOIDataDefine.BuidlSpaceMergeIntensityModeCombox(m_SpaceMergeIntensityModeCombox);	
	AOIDataDefine.BuildCastSpaceFilterModeCombox(m_CastSpaceFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseDefineModeCombox(m_NoiseDefineModeCombox);	
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FirstFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_OverLowModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_HeightVarModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox2);
	AOIDataDefine.BuildPhaseHeightFactorNumCombox(m_HeightFactorNumCombox);
	AOIDataDefine.BuildPhaseConvertHeightModeCombox(m_PhaseToHeightCombox);	

	BuildManipulateCombox(m_ManipulateCombox);		
	BuildSaveRawExtNameCombox(m_SaveRawExtNameCombox);

	this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	this->m_ImageWndMemDC1.CreateMemDC(&m_ImageWnd, m_BKColor);
	this->m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BKColor);

	this->m_ImageTargetWnd.GetClientRect(&m_ImageTargetWndRect);
	m_ImageTargetWndMemDC.CreateMemDC(&m_ImageTargetWnd, 0x000000);

	JetAPI::SetComboxCurSel(m_CameraCombox, CameraID);
	JetAPI::SetComboxCurSel(m_PhaseIDCombox, BATCH_GRAB_PHASE_M);
	JetAPI::SetComboxCurSel(m_PhaseLEDCombox, DLP_LED_COLOR_RED);
	JetAPI::SetComboxCurSel(m_FovResCombox, AOIDataCollect.GetFovResolutionMode());
	JetAPI::SetComboxCurSel(m_SaveRawExtNameCombox, IMAGE_FILE_MODE_BMP);
	JetAPI::SetComboxCurSel(m_3DCastCurrentIDCombox, DLPLEDCurrentID);
	
#ifdef LIGHT_CTRL_DISABLE
	JetAPI::SetComboxCurSel(m_SliceCombo, SLICE_UNIQUE_ID_NULL);
	JetAPI::SetComboxCurSel(m_LightCombox, BATCH_GRAB_LIGHT_00);		
#else
	JetAPI::ShowCtrlWnd(this, CALIALIGN_LIGHT_LABEL, FALSE);
	JetAPI::ShowCtrlWnd(this, CALIALIGN_LIGHT_COMBO, FALSE);
	JetAPI::SetComboxCurSel(m_SliceCombo, SLICE_UNIQUE_ID_DEFAULT);
	JetAPI::SetComboxCurSel(m_LightCombox, BATCH_GRAB_LIGHT_01);	
	Light3DCtrl.SetAllLight3DLEDCurrentID(DLPLEDCurrentID);
#endif//LIGHT_CTRL_DISABLE
		
	TSliceParam *SliceParamPtr_3D = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SLICE_UNIQUE_ID_DLP);
	if ( true == bDisable3D )
	{
		m_SliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;
		JetAPI::SetComboxCurSel(m_3DCastIDCombox, LIGHT_3D_CAST_00);
	}
	else
	{
		if ( NULL == SliceParamPtr_3D )
		{	m_SliceFuncMode = SLICE_FUNC_3D_4STEP_4STEP_1EXP;	}
		else
		{	m_SliceFuncMode = SliceParamPtr_3D->SliceFuncMode;	}
		#ifdef LIGHT_CTRL_DISABLE
			JetAPI::SetComboxCurSel(m_3DCastIDCombox, LIGHT_3D_CAST_01);
		#else
			JetAPI::SetComboxCurSel(m_3DCastIDCombox, LIGHT_3D_CAST_00);		
		#endif//LIGHT_CTRL_DISABLE
	}

	this->SwitchMultiLanguage();	

	CString str;		
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);

	this->m_CameraExposureTime_us = CaliParam.m_ExposureTime_us;
	const int DLPExposureTime_us = CaliParam.m_DLPExposureTime_us;
	const int DLPExposureTime2_us = CaliParam.m_DLPExposureTime2_us;
	this->SetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT, DLPExposureTime_us);
	this->SetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT2, DLPExposureTime2_us);	
#ifdef LIGHT_3D_TI_DLP_USE_V2
	JetAPI::EnableCtrlWnd(this, CALIALIGN_DLP_EXPOSURE_TIME_EDIT2, FALSE);
#endif//LIGHT_3D_TI_DLP_USE_V2		
	JetAPI::EnableEditWnd(this, CALIALIGN_PHASE_PERIOD_EDIT1, FALSE);	
	JetAPI::EnableEditWnd(this, CALIALIGN_PHASE_PERIOD_EDIT2, FALSE);	

	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);	
	this->SetDlgItemInt(CALIALIGN_TARGET_GRID_EXP_EDIT, CaliParam.m_TargetGridExpTime_us);	
	this->SetDlgItemInt(CALIALIGN_TARGET_WHITE_EXP_EDIT, CaliParam.m_TargetWhiteExpTime_us);	
	this->SetDlgItemInt(CALIALIGN_TARGET_HEIGHT_EXP_EDIT, CaliParam.m_TargetHeightExpTime_us);	
	this->SetDlgItemInt(CALIALIGN_IMAGE_FOCUS_AUTO_PITCH_EDIT, (int)(m_AutoFocusPitch));	
	this->SetDlgItemInt(CALIALIGN_GRID_ROWS_EDIT, this->m_ImageGridRows);
	this->SetDlgItemInt(CALIALIGN_GRID_COLS_EDIT, this->m_ImageGridCols);

	this->m_PhaseFactorRows = CaliParam.m_3DCastHeightFactorGridRows;
	this->m_PhaseFactorCols = CaliParam.m_3DCastHeightFactorGridCols;
	this->SetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NROWS_EDIT, this->m_PhaseFactorRows);
	this->SetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NCOLS_EDIT, this->m_PhaseFactorCols);

	str.Format(_T("%.1f"), m_GrayMinRatio);
	this->SetDlgItemText(CALIALIGN_LIGHT_ALIGN_RANGE_EDIT, str);	
	this->SetDlgItemText(CALIALIGN_3DCAST_ALIGN_RANGE_EDIT, str);		
	
	UpdateSliceParamToUI();
	this->SetDlgItemInt(CALIALIGN_PATTERN_CURRENT_GRAY_EDIT, CaliParam.m_3DCastCurrentGray);		
	this->SetDlgItemInt(CALIALIGN_3D_CAST_CURRENT_EXP_EDIT, CaliParam.m_3DCastCurrentExpTime_us);		

	str.Format(_T("%.2f"), CaliParam.m_3DCastMountAngle);
	this->SetDlgItemText(CALIALIGN_3DCAST_MOUNT_ANGLE_EDIT, str);
	str.Format(_T("%.2f"), SysParam.m_PhasePeriod1);
	this->SetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1,  str);
	str.Format(_T("%.2f"), SysParam.m_PhasePeriod2);
	this->SetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, str);	
	JetAPI::SetComboxCurSel(m_PhaseToHeightCombox, SysParam.m_PhaseConvertHeightMode);	
	
	this->SetDlgItemInt(CALIALIGN_PATTERN_PHASE_HIGHT_LEVEL_EDIT, 200);
	this->SetDlgItemInt(CALIALIGN_PATTERN_PHASE_FILTER_COUNT_EDIT, 5);
	this->SetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_FILTER_EDIT, 21);
	//m_EnableBasePhaseCorrect
	//m_BasePhaseCorrectTimes;
	//m_EnableHeightFactorCorrect;//啟用高度係數修正
	//m_HeightFactorCorrectTimes;//高度係數修正次數//v1.01.04.001
	this->SetDlgItemInt(CALIALIGN_PHASE_TARGET_HEIGHT_EDIT, CaliParam.m_TargetHeightThickValue);
	this->SetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT, CaliParam.m_PhaseFactorFovPitch);
	this->SetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_RANGE_EDIT, CaliParam.m_PhaseFactorFovRange);
	CWnd::CheckDlgButton(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_CHK, CaliParam.m_HeightFactorCalibrateWithFOV);	

	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT, 0);
	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT_LT, 0);
	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT_RT, 0);
	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT_LB, 0);
	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT_RB, 0);
	this->SetDlgItemInt(CALIALIGN_3DCAST_FOCUS_EDIT_CC, 0);

	this->m_ZeroPhaseOffsetPosZ = CaliParam.m_PhaseZeroPlaneOffsetPosZ;
	str.Format(_T("%.0f"), m_ZeroPhaseOffsetPosZ);
	CWnd::SetDlgItemText(CALIALIGN_PHASE_PLANE_OFFSET_EDIT, str);		
	
	const int BasePlaneIndex = SysParam.m_DefaultSpaceBasePlaneIndex;
	const int NoiseFilterIndex = SysParam.m_DefaultSpaceNoiseFilterIndex;
	AOIDataCollect.GetSpaceNoiseFilterParam(m_NoiseFilterParam);
	//AOIDataCollect.GetSystemNoiseFilterParam(NoiseFilterIndex, m_NoiseFilterParam);
	//AOIDataCollect.GetSystemBasePlaneParam(BasePlaneIndex, m_NoiseFilterParam.BasePlaneParam);

	UpdatePhaseNoiseParamToUI();
	UpdateSpaceNoiseFilterParamToUI();
	CWnd::SetDlgItemInt(CALIALIGN_MOTION_Z_PITCH_EDIT, 100);	
	
	const double FovW=CaliParam.m_FOVWidth_um;
	const double FovH=CaliParam.m_FOVHeight_um;
	SetFovSize(FovW, FovH);
	CalcFOVSizeResolution();		
	Update3DCastCurrentToUIKernel(LIGHT_3D_CAST_01);

	m_MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this);
	m_Draw3DWnd.Create(IDD_DRAW3D_WND, this);
	m_MaskImageWnd.Create(IDD_IMAGE_MASK_WND, this);
	m_PhaseImageWnd.Create(IDD_IMAGE_PHASE_WND, this);
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, this->m_ImageWndRect, 1.1, this->m_ImageZoom);

	const IMAGE_SIZE ImageW2 = ImageW/2;
	const IMAGE_SIZE ImageH2 = ImageH/2;
	const IMAGE_SIZE GridW = ImageW/4;
	const IMAGE_SIZE GridH = ImageH/4;
	m_ImageRect4D.left   = ImageW2-(GridW/2);
	m_ImageRect4D.top    = ImageH2-(GridH/2);
	m_ImageRect4D.right  = ImageW2+(GridW/2);
	m_ImageRect4D.bottom = ImageH2+(GridH/2);
	SetModifiedCaliParam(false);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->m_ShowBuffer = NULL;
	this->m_ShowBuffer1 = NULL;
	this->m_MaskBuffer = NULL;
	this->m_MaskBuffer1 = NULL;
	this->m_MaskBuffer2 = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_SpaceBuffer1 = NULL;
	this->m_SpaceBuffer2 = NULL;
	this->m_PhaseBuffer = NULL;
	this->m_PhaseBuffer1 = NULL;
	this->m_PhaseBuffer2 = NULL;
	this->m_ImageBuffer = NULL;
	this->m_ImageBuffer1 = NULL;
	this->m_ImageBuffer2 = NULL;
	this->m_ShowBufferSize = 0;
	this->m_ImageBufferSize = 0;
	this->m_SpaceBufferSize = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
	if ( this->m_ImageWnd.GetSafeHwnd() == NULL ) 
	{	return; }
	
	RECT WndRect={0};
	SIZE WndSIze={0};	
	this->m_ImageWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	WndRect.right = cx-4;
	WndRect.bottom = cy-4;
	this->m_ImageWnd.MoveWindow(&WndRect);
	this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	this->m_ImageWndMemDC1.CreateMemDC(&m_ImageWnd, m_BKColor);
	this->m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BKColor);
	
	this->m_ImageTargetWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);	
	WndRect.bottom = cy-4;
	this->m_ImageTargetWnd.MoveWindow(&WndRect);
	this->m_ImageTargetWnd.GetClientRect(&m_ImageTargetWndRect);
	this->m_ImageTargetWndMemDC.CreateMemDC(&m_ImageTargetWnd, 0x000000);
	
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(m_CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(m_CameraID);
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, this->m_ImageWndRect, 1.1, this->m_ImageZoom);

	if ( m_LogListBox.GetSafeHwnd() != NULL )
	{
		m_LogListBox.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-4;
		m_LogListBox.MoveWindow(&WndRect);
	}

	CWnd *pWnd = NULL;
	pWnd = this->GetDlgItem(CALIALIGN_PIXEL_INFO_EDIT);
	if ( (NULL!=pWnd) && (NULL!=pWnd->GetSafeHwnd()) )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-4;	
		pWnd->MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CreateTempFolder()//建立暫存資料夾
{
	CString str;
	CString Folder = AOIDataCollect.GetAOITempDirectory();
	if ( JetAPI::CreateFolder(Folder) == false )
	{
		str.Format(_T("Error, Create Temp Folder Fault [%s]"), Folder);
		JetAPI::ShowMessageBox(str);		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_PANE_ALIGN");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_PANE_ALIGN;
	WndKey = _T("IDD_CALIBRATION_PANE_ALIGN");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_TARGET_GROUP;
	WndKey = _T("CALIALIGN_TARGET_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_GRID_GO_BTN;
	WndKey = _T("CALIALIGN_TARGET_GRID_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_GRID_SET_BTN;
	WndKey = _T("CALIALIGN_TARGET_GRID_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_GRID_EXP_BTN;
	WndKey = _T("CALIALIGN_TARGET_GRID_EXP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_TARGET_WHITE_GO_BTN;
	WndKey = _T("CALIALIGN_TARGET_WHITE_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_WHITE_SET_BTN;
	WndKey = _T("CALIALIGN_TARGET_WHITE_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_WHITE_EXP_BTN;
	WndKey = _T("CALIALIGN_TARGET_WHITE_EXP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_TARGET_HEIGHT_GO_BTN;
	WndKey = _T("CALIALIGN_TARGET_HEIGHT_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_HEIGHT_SET_BTN;
	WndKey = _T("CALIALIGN_TARGET_HEIGHT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_HEIGHT_EXP_BTN;
	WndKey = _T("CALIALIGN_TARGET_HEIGHT_EXP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = CALIALIGN_MANIPULATE_GROUP;
	WndKey = _T("CALIALIGN_MANIPULATE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SLICE_LABEL;
	WndKey = _T("CALIALIGN_SLICE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_LABEL;
	WndKey = _T("CALIALIGN_CAMERA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_LABEL;
	WndKey = _T("CALIALIGN_LIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3DCAST_LABEL;
	WndKey = _T("CALIALIGN_3DCAST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_EXPOSURE_TIME_LABEL;
	WndKey = _T("CALIALIGN_CAMERA_EXPOSURE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_EXPOSURE_TIME_BTN;
	WndKey = _T("CALIALIGN_CAMERA_EXPOSURE_TIME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_DLP_EXPOSURE_TIME_LABEL;
	WndKey = _T("CALIALIGN_DLP_EXPOSURE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_DLP_EXPOSURE_TIME_BTN;
	WndKey = _T("CALIALIGN_DLP_EXPOSURE_TIME_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_MANIPULATE_LABEL;
	WndKey = _T("CALIALIGN_MANIPULATE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_GRID_NUMBER_LABEL;
	WndKey = _T("CALIALIGN_GRID_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_GRAB_BTN;
	WndKey = _T("CALIALIGN_GRAB_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_GRAB_REPEAT_CHK;
	WndKey = _T("CALIALIGN_GRAB_REPEAT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SAVE_IMAGE_BTN;
	WndKey = _T("CALIALIGN_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALIALIGN_MOTION_WND_BTN;
	WndKey = _T("CALIALIGN_MOTION_WND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_XYZ_ORG_BTN;
	WndKey = _T("CALIALIGN_XYZ_ORG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_MOTION_Z_PITCH_LABEL;
	WndKey = _T("CALIALIGN_MOTION_Z_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_VIEW_RESET_BTN;
	WndKey = _T("CALIALIGN_VIEW_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_BASE_PLANE_PARAM_BTN;
	WndKey = _T("CALIALIGN_BASE_PLANE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SPACE_NOISE_FILTER_BTN;
	WndKey = _T("CALIALIGN_SPACE_NOISE_FILTER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_FOV_SIZE_GROUP;
	WndKey = _T("CALIALIGN_FOV_SIZE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_FOV_RESOLUTION_LABEL;
	WndKey = _T("CALIALIGN_FOV_RESOLUTION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_FOV_WIDTH_BTN;
	WndKey = _T("CALIALIGN_FOV_WIDTH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_FOV_HEIGHT_BTN;
	WndKey = _T("CALIALIGN_FOV_HEIGHT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CALIALIGN_CAMERA_ALIGN_GROUP;
	WndKey = _T("CALIALIGN_CAMERA_ALIGN_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_ALIGN_HOR_BTN;
	WndKey = _T("CALIALIGN_CAMERA_ALIGN_HOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_ALIGN_VER_BTN;
	WndKey = _T("CALIALIGN_CAMERA_ALIGN_VER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_CAMERA_ALIGN_3D_CHK;
	WndKey = _T("CALIALIGN_CAMERA_ALIGN_3D_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CALIALIGN_ITEM_ALIGN_GROUP;
	WndKey = _T("CALIALIGN_ITEM_ALIGN_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_ALIGN_BTN;
	WndKey = _T("CALIALIGN_LIGHT_ALIGN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3DCAST_ALIGN_BTN;
	WndKey = _T("CALIALIGN_3DCAST_ALIGN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3DCAST_FOCUS_BTN;
	WndKey = _T("CALIALIGN_3DCAST_FOCUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3DCAST_MOUNT_ANGLE_LABEL;
	WndKey = _T("CALIALIGN_3DCAST_MOUNT_ANGLE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CALIALIGN_IMAGE_FOCUS_AUTO_BTN;
	WndKey = _T("CALIALIGN_IMAGE_FOCUS_AUTO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_IMAGE_RESOLUTION_CALC_BTN;
	WndKey = _T("CALIALIGN_IMAGE_RESOLUTION_CALC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_ITEM_CURRENT_GROUP;
	WndKey = _T("CALIALIGN_ITEM_CURRENT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_CURRENT_BTN;
	WndKey = _T("CALIALIGN_LIGHT_CURRENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_CURRENT_GRAY_LABEL;
	WndKey = _T("CALIALIGN_LIGHT_CURRENT_GRAY_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_CURRENT_GAIN_LABEL;
	WndKey = _T("CALIALIGN_LIGHT_CURRENT_GAIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_CURRENT_EXP_LABEL;
	WndKey = _T("CALIALIGN_LIGHT_CURRENT_EXP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_LIGHT_CURRENT_SET_BTN;
	WndKey = _T("CALIALIGN_LIGHT_CURRENT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_3D_CAST_CURRENT_BTN;
	WndKey = _T("CALIALIGN_3D_CAST_CURRENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3D_CAST_CURRENT_GRAY_LABEL;
	WndKey = _T("CALIALIGN_3D_CAST_CURRENT_GRAY_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3D_CAST_CURRENT_EXP_LABEL;
	WndKey = _T("CALIALIGN_3D_CAST_CURRENT_EXP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3D_CAST_CURRENT_SET_BTN;
	WndKey = _T("CALIALIGN_3D_CAST_CURRENT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_3D_CAST_CURRENT_ID_LABEL;
	WndKey = _T("CALIALIGN_3D_CAST_CURRENT_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_PHASE_CALIBRATION_GROUP;
	WndKey = _T("CALIALIGN_PHASE_CALIBRATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_ID_LABEL;
	WndKey = _T("CALIALIGN_PHASE_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_PERIOD_BTN;
	WndKey = _T("CALIALIGN_PHASE_PERIOD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_ZERO_PLANE_BTN;
	WndKey = _T("CALIALIGN_PHASE_ZERO_PLANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_ZERO_PLANE_SHOW_BTN;
	WndKey = _T("CALIALIGN_PHASE_ZERO_PLANE_SHOW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_BTN;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_BTN;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_LABEL;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_LABEL;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_SET_BTN;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_NUM_LABEL;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_NUM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_CLEAR_BTN;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_TABLE_BTN;
	WndKey = _T("CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_TABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_PATTERN_PHASE_MEASURE_CHK;
	WndKey = _T("CALIALIGN_PATTERN_PHASE_MEASURE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PHASE_IMAGE_BTN;
	WndKey = _T("CALIALIGN_PHASE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TEST_IMAGE_PHASE_CHK;
	WndKey = _T("CALIALIGN_TEST_IMAGE_PHASE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SAVE_RAW_IMAGE_CHK;
	WndKey = _T("CALIALIGN_SAVE_RAW_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALIALIGN_VERIFY_ZERO_PLANE_BTN;
	WndKey = _T("CALIALIGN_VERIFY_ZERO_PLANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_SHOW_RAW_IMAGE_FOLDER_BTN;
	WndKey = _T("CALIALIGN_SHOW_RAW_IMAGE_FOLDER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = CALIALIGN_NOISE_GROUP;
	WndKey = _T("CALIALIGN_NOISE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = CALIALIGN_NOISE_VOID_EXPAND_CHK;
	WndKey = _T("CALIALIGN_NOISE_VOID_EXPAND_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_LOW_CONTRAST_CHK;
	WndKey = _T("CALIALIGN_NOISE_LOW_CONTRAST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_LOW_POTENTIAL_CHK;
	WndKey = _T("CALIALIGN_NOISE_LOW_POTENTIAL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_OVER_SATURATED_CHK;
	WndKey = _T("CALIALIGN_NOISE_OVER_SATURATED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SINGLE_CAST_OVER_LOW_LABEL;
	WndKey = _T("CALIALIGN_SINGLE_CAST_OVER_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_MULTI_CAST_MERGE_LABEL;
	WndKey = _T("CALIALIGN_MULTI_CAST_MERGE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_MULTI_CAST_MERGE_BEST_LABEL;
	WndKey = _T("CALIALIGN_MULTI_CAST_MERGE_BEST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_MULTI_INTENSITY_MERGE_MODE_LABEL;
	WndKey = _T("CALIALIGN_MULTI_INTENSITY_MERGE_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIALIGN_NOISE_SMOOTH_FILTER_CHK;
	WndKey = _T("CALIALIGN_NOISE_SMOOTH_FILTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK;
	WndKey = _T("CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FIRST_FILTER_LABEL;
	WndKey = _T("CALIALIGN_NOISE_FIRST_FILTER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FILTER_OVER_LOW_LABEL;
	WndKey = _T("CALIALIGN_NOISE_FILTER_OVER_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_LABEL;
	WndKey = _T("CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK;
	WndKey = _T("CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FINAL_FILTER_LABEL;
	WndKey = _T("CALIALIGN_NOISE_FINAL_FILTER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_NOISE_FINAL_FILTER_LABEL2;
	WndKey = _T("CALIALIGN_NOISE_FINAL_FILTER_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_PCB_IN_BTN;
	WndKey = _T("CALIALIGN_PCB_IN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PCB_OUT_BTN;
	WndKey = _T("CALIALIGN_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PCB_BACK_BTN;
	WndKey = _T("CALIALIGN_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PCB_CLAMP_ON_BTN;
	WndKey = _T("CALIALIGN_PCB_CLAMP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PCB_CLAMP_OFF_BTN;
	WndKey = _T("CALIALIGN_PCB_CLAMP_OFF_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_CAP_ON_BTN;
	WndKey = _T("CALIALIGN_TARGET_CAP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_TARGET_CAP_OFF_BTN;
	WndKey = _T("CALIALIGN_TARGET_CAP_OFF_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_VIEW_ALL_BTN;
	WndKey = _T("CALIALIGN_VIEW_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_VIEW_1X1_BTN;
	WndKey = _T("CALIALIGN_VIEW_1X1_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_SHOW_CENTER_LINE_CHK;
	WndKey = _T("CALIALIGN_SHOW_CENTER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SHOW_HOR_LINE_CHK;
	WndKey = _T("CALIALIGN_SHOW_HOR_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_SHOW_VER_LINE_CHK;
	WndKey = _T("CALIALIGN_SHOW_VER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_HIDE_LINE_BTN;
	WndKey = _T("CALIALIGN_HIDE_LINE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALIALIGN_PARAM_FILE_GROUP;
	WndKey = _T("CALIALIGN_PARAM_FILE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PARAM_FILE_RELOAD_BTN;
	WndKey = _T("CALIALIGN_PARAM_FILE_RELOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIALIGN_PARAM_PHASE_TO_HEIGHT_LABEL;
	WndKey = _T("CALIALIGN_PARAM_PHASE_TO_HEIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	

	/*
	WndID = AAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneAlign::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CALIBRATION_PANE_ALIGN");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LRESULT CCaliPaneAlign::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:		
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		if ( this->ExecReGrab(wParam) == false )
		{	this->LockUIWnd(false); }
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(false);
		break;
	case MSG_CALIBRATION_ALIGN_WND:
		ExecCaliAlignWndMsg(wParam, lParam);		
		break;	
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1, SPACE_PTR Buffer2)
{
	m_SpaceBuffer = Buffer;
	m_SpaceBuffer1 = Buffer1;
	m_SpaceBuffer2 = Buffer2;
	m_SpaceBufferSize = Size;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1)
{
	m_ShowStep = 0;
	m_ShowBitCount = 8;	

	m_ShowBuffer = Buffer;
	m_ShowBuffer1 = Buffer1;
	m_ShowBufferSize = Size;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer, MASK_PTR MaskBuffer1, MASK_PTR MaskBuffer2)
{
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 8;

	m_MaskBuffer = MaskBuffer;
	m_MaskBuffer1 = MaskBuffer1;
	m_MaskBuffer2 = MaskBuffer2;

	m_ImageBuffer = Buffer;
	m_ImageBuffer1 = Buffer1;
	m_ImageBuffer2 = Buffer2;	
	m_PhaseBuffer = PhaseBuffer;
	m_PhaseBuffer1 = PhaseBuffer1;
	m_PhaseBuffer2 = PhaseBuffer2;
	m_ImageBufferSize = Size;		
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::EnableEditWnd(UINT CtrlID, BOOL bEnable)//啟用編輯控制項
{
	CWnd  *pWnd = NULL;
	CEdit *pEdit = NULL;
	pWnd = this->GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return false; }

//	BOOL bIsEdit = pWnd->IsKindOf(RUNTIME_CLASS(CEdit));
//	if ( FALSE == bIsEdit )
//	{	return false; }

	pEdit = static_cast<CEdit*>(pWnd);	
	if ( NULL == pEdit )
	{	return false; }	

	if ( TRUE == bEnable )
	{	pEdit->SetReadOnly(FALSE); }
	else
	{	pEdit->SetReadOnly(TRUE); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ExtraDelayTime()
{
	//ExtraDelayTimeKernel();//改在取像前延遲
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ExtraDelayTimeKernel()
{
	if ( m_ExtraDelayTime > 0 )
	{	::Sleep(m_ExtraDelayTime); }
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CheckNeedExtraDelayTime()
{
	bool bNeed=false;
	CALIBRATION_MODE CaliMode=GetCalibrationMode();
	switch ( CaliMode )
	{
	case CALIBRATION_STOP:
		bNeed=false;
		break;
	case CALIBRATION_IMAGE_RESOLUTION:

	case CALIBRATION_IMAGE_FOCUS_AUTO_100:
	case CALIBRATION_IMAGE_FOCUS_AUTO_10:
	case CALIBRATION_IMAGE_FOCUS_AUTO:
		bNeed=false;
		break;

	case CALIBRATION_2D_LIGHT_ALIGN:
	case CALIBRATION_3D_CAST_ALIGN:
	case CALIBRATION_3D_CAST_FOCUS:
		bNeed=false;
		break;

	case CALIBRATION_2D_LIGHT_CURRENT:	
	case CALIBRATION_3D_CAST_CURRENT:
	case CALIBRATION_3D_CAST_CURRENT_RED:
	case CALIBRATION_3D_CAST_CURRENT_GRN:
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		bNeed=false;
		break;

	default:
		bNeed = true;
		break;
	}
	return bNeed;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::LockUIWnd(bool bLock)//鎖住視窗
{	
#ifndef OFFLINE_VERSION
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	
	BOOL  bEnable3D = TRUE;	
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }		
	bEnable3D = bEnable;
	if ( true == bDisable3D )
	{	bEnable3D = FALSE;	}
	
	CtrlID = CALIALIGN_TARGET_GRID_EXP_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_GRID_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_GRID_GO_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_TARGET_WHITE_EXP_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_WHITE_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_WHITE_GO_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_TARGET_HEIGHT_EXP_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_HEIGHT_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_HEIGHT_GO_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_SLICE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_CAMERA_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_3D_CAST_ID_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_MANIPULATE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_MOTION_WND_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	CtrlID = CALIALIGN_GRAB_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_SAVE_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	CtrlID = CALIALIGN_FOV_RESOLUTION_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_FOV_WIDTH_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_FOV_HEIGHT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_CAMERA_ALIGN_HOR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_CAMERA_ALIGN_VER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_ALIGN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_3DCAST_ALIGN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_CAMERA_ALIGN_3D_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	

	CtrlID = CALIALIGN_3DCAST_FOCUS_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_IMAGE_FOCUS_AUTO_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		

	CtrlID = CALIALIGN_IMAGE_RESOLUTION_CALC_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_CURRENT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	
	CtrlID = CALIALIGN_PHASE_ID_COMBOX;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_LED_COMBOX;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_PERIOD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_ZERO_PLANE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_ZERO_PLANE_SHOW_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	//CtrlID = CALIALIGN_PATTERN_PHASE_MEASURE_CHK;
	//JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_CLEAR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_SHOW_TABLE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_TEST_IMAGE_PHASE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_SAVE_RAW_IMAGE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_SAVE_RAW_EXT_NAME_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_VERIFY_ZERO_PLANE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);		
	CtrlID = CALIALIGN_SHOW_RAW_IMAGE_FOLDER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_XYZ_ORG_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_MOTION_Z_PITCH_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_PCB_IN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_PCB_OUT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_PCB_BACK_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_PCB_CLAMP_ON_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_PCB_CLAMP_OFF_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_TARGET_CAP_ON_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_TARGET_CAP_OFF_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

//	CtrlID = CALIALIGN_VIEW_ALL_BTN;
//	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
//	CtrlID = CALIALIGN_VIEW_1X1_BTN;
//	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

	CtrlID = CALIALIGN_CAMERA_EXPOSURE_TIME_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

	CtrlID = CALIALIGN_DLP_EXPOSURE_TIME_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	

	//About Edit Ctrl Wnd	
	CtrlID = CALIALIGN_TARGET_GRID_EXP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_WHITE_EXP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_TARGET_HEIGHT_EXP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	

	CtrlID = CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_DLP_EXPOSURE_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_DLP_EXPOSURE_TIME_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	

	CtrlID = CALIALIGN_GRID_ROWS_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_GRID_COLS_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_FOV_WIDTH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_FOV_HEIGHT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_ALIGN_RANGE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_3DCAST_ALIGN_RANGE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_IMAGE_FOCUS_AUTO_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CALIALIGN_3DCAST_MOUNT_ANGLE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_LIGHT_CURRENT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_CURRENT_GRAY_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_CURRENT_GAIN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_CURRENT_EXP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIALIGN_LIGHT_CURRENT_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CALIALIGN_3D_CAST_CURRENT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PATTERN_CURRENT_GRAY_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_3D_CAST_CURRENT_EXP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_3D_CAST_CURRENT_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_3D_CAST_CURRENT_ID_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	
	CtrlID = CALIALIGN_PHASE_PLANE_OFFSET_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_NROWS_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_NCOLS_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_PHASE_TARGET_HEIGHT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_FILTER_EDIT;	
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);		
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT;	
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);		
	CtrlID = CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_RANGE_EDIT;	
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);		

	CtrlID = CALIALIGN_PATTERN_PHASE_HIGHT_LEVEL_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PATTERN_PHASE_FILTER_COUNT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);		

	CtrlID = CALIALIGN_NOISE_DEFINE_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_VOID_EXPAND_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_LOW_CONTRAST_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_LOW_POTENTIAL_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_OVER_SATURATED_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_SMOOTH_FILTER_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	
	CtrlID = CALIALIGN_MULTI_CAST_MERGE_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_MULTI_CAST_MERGE_BEST_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_MULTI_INTENSITY_MERGE_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_CAST_FILTER_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);		
	CtrlID = CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FIRST_FILTER_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_OVER_LOW_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_MODE_COMBO2;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);		
	
	CtrlID = CALIALIGN_PARAM_FILE_RELOAD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_PARAM_PHASE_TO_HEIGHT_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_BASE_PLANE_PARAM_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_SPACE_NOISE_FILTER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_NOISE_LOW_CONTRAST_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_LOW_POTENTIAL_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_OVER_SATURATED_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_LOW_CONTRAST_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_LOW_POTENTIAL_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_OVER_SATURATED_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_SINGLE_CAST_OVER_LOW_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);		
	CtrlID = CALIALIGN_MULTI_CAST_PATCH_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_MULTI_CAST_MIN_VALID_COUNT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_MULTI_CAST_LIMIT_DIFF_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_MULTI_CAST_MAX_DIFF_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_MULTI_CAST_VALID_DIFF_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_VOID_EXPAND_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_SMOOTH_FILTER_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_CAST_MEDIAN_FILTER_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_CAST_MEDIAN_FILTER_USE_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_NOISE_FIRST_FILTER_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FIRST_FILTER_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FIRST_USE_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	

	CtrlID = CALIALIGN_NOISE_FILTER_VOID_EXPAND_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FILTER_OVER_LOW_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FILTER_OVER_LOW_LIMIT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	
	CtrlID = CALIALIGN_NOISE_FILTER_OVER_LOW_KER_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_OVER_LOW_USE_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_KER_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_USE_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);	

	CtrlID = CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);
	CtrlID = CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT2;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable3D);

	CtrlID = CALIALIGN_MOTION_Z_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	
#endif//OFFLINE_VERSION
	/*	
CALIALIGN_GRAB_REPEAT_CHK
	*/
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::MoveToBeforeStagePosition()//移動至機台位置
{
	const double PosX=m_StagePosX;
	const double PosY=m_StagePosY;
	const double PosZ=m_StagePosZ;	
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ) == false )
	{	return false; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{	return false;	}
	return UpdateStagePositionKernel(PosX, PosY, PosZ);	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::RetrieveStagePosition(bool UpdatePos)//取得機台位置
{
	double PosX=0, PosY=0, PosZ=0;
	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ) == false )	
	{	return false; }	
	if ( true == UpdatePos )
	{
		m_StagePosX = PosX;
		m_StagePosY = PosY;
		m_StagePosZ = PosZ;
	}
	return UpdateStagePositionKernel(PosX, PosY, PosZ);	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{	
	CString str;		
	CAMERA_ID CameraID = CCameraCtrl::GetCaemraIDFromWParam(wParam);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));
		this->AddLogListBox(str);
		return true;	
	}
	if ( NULL==m_ShowBuffer || NULL==m_ImageBuffer || NULL==m_ImageBuffer1 || NULL==m_ImageBuffer2) 
	{ 
		return false; 
	}
	
	str.Format(_T("Image Period Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));
	this->AddLogListBox(str);

	CameraCtrl.SetCameraToSendCallback(CameraID, FALSE);
	if ( CameraCtrl.GetCameraImage3(CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer) == false )
	{				
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false); 
		return false;
	}
	m_ShowStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;

	const double ImageOffset = 0.0;
	const double ImageGain = m_SliceParam.SliceGainValue;
	if ( ImageAPI.ImageOffsetGain3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ImageBuffer, ImageOffset, ImageGain) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false); 
		return false;
	}
	CameraCtrl.KeepCameraTempRingBuffer(CameraID);
	CameraCtrl.IncrementCameraCopyToHostCount(CameraID);	
	CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
	CameraCtrl.FreeCameraTempRingBuffer(CameraID);
	if ( BATCH_GRAB_PHASE_M2 == m_PhaseID )
	{
		if ( CameraCtrl.CheckNeedGrabNext3DImage() == true )//2次打光
		{
			bool bFinish = false;
			if ( CameraCtrl.BatchGrabNext3DImage(bFinish) == false )
			{
				JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
				return false; 
			}
			return true;
		}
	}
	
	RECT              Rect={0};
	bool              bFinish = false;
	TPOINT2D          ImagePt1, ImagePt2;
	CALIBRATION_MODE  CalibrationMode = GetCalibrationMode();	
	CAMERA_IMAGE_MODE CameraImageMode=CameraCtrl.GetCameraImageMode(CameraID);

	//更新機台座標
	this->UpdateStagePosition();
	//更新影像平均灰階
	JetAPI::SizeToRect(m_ImageW, m_ImageH, Rect);
	ImageAPI.CalcImageAverage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, Rect, m_AveGrayR, m_AveGrayG, m_AveGrayB);
	GetCurrentDefaultRoi(m_ImageW, m_ImageH, ImagePt1, ImagePt2);
	JetAPI::PointsToRect(ImagePt1, ImagePt2, Rect);		
	ImageAPI.CalcImageAverage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, Rect, m_AveRoiR, m_AveRoiG, m_AveRoiB);
	ShowPixelInfo();

	switch ( CalibrationMode )
	{
	case CALIBRATION_FOV_WIDTH_START:
	case CALIBRATION_FOV_WIDTH_END:
		if ( this->ExecFOVWidth(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }	
		break;
	case CALIBRATION_FOV_HEIGHT_START:
	case CALIBRATION_FOV_HEIGHT_END:
		if ( this->ExecFOVHeight(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }				
		break;
	case CALIBRATION_CAMERA_ALIGN_HOR_START:
	case CALIBRATION_CAMERA_ALIGN_HOR_END:
		if ( this->ExecCameraAlignHor(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }				
		break;	
	case CALIBRATION_CAMERA_ALIGN_VER_START:
	case CALIBRATION_CAMERA_ALIGN_VER_END:
		if ( this->ExecCameraAlignVer(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }				
		break;
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_START:
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_END:
		if ( this->ExecCameraAlignHor3D(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{			
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true;	}
		break;	
	case CALIBRATION_CAMERA_ALIGN_VER_3D_START:
	case CALIBRATION_CAMERA_ALIGN_VER_3D_END:
		if ( this->ExecCameraAlignVer3D(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{			
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }				
		break;	
	case CALIBRATION_IMAGE_RESOLUTION:	
		if ( this->ExecImageResolution(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( bFinish == false )
		{	return true; }
		break;	
	case CALIBRATION_IMAGE_FOCUS_AUTO:
	case CALIBRATION_IMAGE_FOCUS_AUTO_10:
	case CALIBRATION_IMAGE_FOCUS_AUTO_100:
		if ( this->ExecImageFocusAuto(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( bFinish == false )
		{	return true;	}		
		return true;			
		break;
	case CALIBRATION_2D_LIGHT_ALIGN:
		if ( this->Exec2DLightAlign(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_3D_CAST_ALIGN:
		if ( this->Exec3DCastAlign(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_3D_CAST_FOCUS:		
		if ( this->Exec3DCastFocus(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_2D_LIGHT_CURRENT:
		if ( this->Exec2DLightCurrent(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }		
		break;	
	case CALIBRATION_3D_CAST_CURRENT:
	case CALIBRATION_3D_CAST_CURRENT_RED:
	case CALIBRATION_3D_CAST_CURRENT_GRN:
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		if ( this->Exec3DCastCurrent(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		break;	
	case CALIBRATION_PATTERN_ZERO_PLANE:
		if ( this->ExecPhaseZeroPlane(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }				
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT:		
		if ( this->ExecPhaseHeightFactor_DOT(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		return true;
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV:
		if ( this->ExecPhaseHeightFactor_FOV(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		return true;		
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV_Z:
		if ( this->ExecPhaseHeightFactor_FOV_Z(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		return true;				
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV:
		if ( this->ExecPhaseHeightFactor_MultiFOV(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		return true;		
		break;	
	case CALIBRATION_3D_MODEL_TEST:
		if ( this->Exec3DModelTest(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			ExecSelchange3DCastCurrentIDCombo();
			return false;
		}
		if ( false == bFinish )
		{	return true; }		
		else
		{	ExecSelchange3DCastCurrentIDCombo(); }
		break;
	case CALIBRATION_VERIFY_ZERO_PLANE:
		if ( this->ExecVerifyZeroPlane(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			this->LockUIWnd(false);
			return false;
		}
		if ( false == bFinish )
		{	return true; }
		break;
	default:
	#ifdef _DEBUG
		if ( NULL != m_ImageBuffer )
		{
			str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CalibrationImage.PNG"));
			ImageAPI.SavePNGImage(str, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, true);
		}
	#endif//_DEBUG
		::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);		
		if ( CAMERA_IMAGE_BAYER==CameraImageMode )
		{
			IMAGE_SIZE ShowW = m_ImageW;
			IMAGE_SIZE ShowH = m_ImageH;
			IMAGE_SIZE ShowBitCnt = 24;
			IMAGE_SIZE ShowStep = JetAPI::GetBMPImagePixelsPerLine(ShowW, ShowBitCnt, 4);
			const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ShowH);			
			BAYER_PATTERN_MODE BayerPattern = CameraCtrl.GetCameraBayerPattern(CameraID);
			if ( ShowBufferSize <= m_ShowBufferSize )
			{	
				if ( ImageAPI.DebayerColorImage3(ShowW, ShowH, m_ImageStep, m_ImageBuffer, BayerPattern, ShowStep, m_ShowBuffer) == true )
				{
					m_ShowStep = ShowStep;
					m_ShowBitCount = ShowBitCnt;
				}
			}
		}		
		this->DrawImageWndMemDC();
		this->RedrawWnd();	
		break;
	}

	if ( m_ShowBitCount != m_BitCount )//更新影像平均灰階
	{	
		JetAPI::SizeToRect(m_ImageW, m_ImageH, Rect);		
		ImageAPI.CalcImageAverage(m_ImageW, m_ImageH, m_ShowStep, m_ShowBitCount, m_ShowBuffer, Rect, m_AveGrayR, m_AveGrayG, m_AveGrayB);		
		GetCurrentDefaultRoi(m_ImageW, m_ImageH, ImagePt1, ImagePt2);		
		JetAPI::PointsToRect(ImagePt1, ImagePt2, Rect);		
		ImageAPI.CalcImageAverage(m_ImageW, m_ImageH, m_ShowStep, m_ShowBitCount, m_ShowBuffer, Rect, m_AveRoiR, m_AveRoiG, m_AveRoiB);
		ShowPixelInfo();
	}

	CWnd *pWnd = NULL;
	UINT  CtrlID = 0;
	BOOL bRepeat = this->IsDlgButtonChecked(CALIALIGN_GRAB_REPEAT_CHK);
	if ( TRUE == bRepeat )
	{
		SetCalibrationMode(CalibrationMode);
		this->StartReGrab(TRUE);		
	}
	else
	{	
		CameraCtrl.StopCameraGrab(CameraID);
		this->LockUIWnd(false); 
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecFOVWidth(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	int               ImageOffset=0;	
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;
	bFinish = false;
	switch ( CalMode )
	{
	case CALIBRATION_FOV_WIDTH_START:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_WIDTH_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer1, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( true == SignX )
		{	NextPosX = this->m_StagePosX+(CaliParam.m_FOVWidth_um/2);	}
		else
		{	NextPosX = this->m_StagePosX-(CaliParam.m_FOVWidth_um/2);	}
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		SetCalibrationMode(CALIBRATION_FOV_WIDTH_END);		
		this->StartReGrab(FALSE);
		return true;		
		break;	
	case CALIBRATION_FOV_WIDTH_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_WIDTH_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		ImageOffset = static_cast<int>(ImageW);
		ImageOffset = ImageOffset/2;
		::memcpy(m_ImageBuffer2, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer1, m_ImageBuffer2, ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_WIDTH_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		this->DrawImageWndMemDC();
		this->RedrawWnd();
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		ApplyFovResolutionToSystem();
		SetCalibrationMode(CALIBRATION_STOP);
		bFinish = true;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecFOVHeight(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	int               ImageOffset=0;	
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;
	bFinish = false;
	switch ( CalMode )
	{
	case CALIBRATION_FOV_HEIGHT_START:		
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_HEIGHT_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer1, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( true == SignY )
		{	NextPosY = this->m_StagePosY-(CaliParam.m_FOVHeight_um/2);	}
		else
		{	NextPosY = this->m_StagePosY+(CaliParam.m_FOVHeight_um/2);	}
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_FOV_HEIGHT_END);
		this->StartReGrab(FALSE);
		return true;
		break;	
	case CALIBRATION_FOV_HEIGHT_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_HEIGHT_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		ImageOffset = static_cast<int>(ImageH);
		ImageOffset = ImageOffset/2;
		::memcpy(m_ImageBuffer2, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer1, m_ImageBuffer2, ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}	
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FOV_HEIGHT_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		this->DrawImageWndMemDC();
		this->RedrawWnd();	
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		ApplyFovResolutionToSystem();
		SetCalibrationMode(CALIBRATION_STOP);		
		bFinish = true;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecCameraAlignHor(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	int               ImageOffset=0;		
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;
	bFinish = false;
	switch ( CalMode )
	{
	case CALIBRATION_CAMERA_ALIGN_HOR_START:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer1, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( true == SignX )
		{	NextPosX = m_StagePosX+(CaliParam.m_FOVWidth_um); }
		else
		{	NextPosX = m_StagePosX-(CaliParam.m_FOVWidth_um); }
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_CAMERA_ALIGN_HOR_END);
		this->StartReGrab(FALSE);
		return true;
		break;
	case CALIBRATION_CAMERA_ALIGN_HOR_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer2, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		::memset(m_ImageBuffer, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		ImageOffset = static_cast<int>(ImageW);
		ImageOffset = ImageOffset/2;
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer, m_ImageBuffer1, -ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_ShowBuffer, m_ImageBuffer2,  ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}	
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_4.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		this->DrawImageWndMemDC();
		this->RedrawWnd();
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_STOP);
		bFinish = true;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecCameraAlignVer(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	int               ImageOffset=0;	
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;
	bFinish = false;
	switch ( CalMode )
	{
	case CALIBRATION_CAMERA_ALIGN_VER_START:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer1, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		if ( true == SignY )
		{	NextPosY = m_StagePosY-(CaliParam.m_FOVHeight_um); }		
		else
		{	NextPosY = m_StagePosY+(CaliParam.m_FOVHeight_um);	}
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_CAMERA_ALIGN_VER_END);
		this->StartReGrab(FALSE);
		return true;
		break;
	case CALIBRATION_CAMERA_ALIGN_VER_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif		
		::memcpy(m_ImageBuffer2, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
		::memset(m_ImageBuffer, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		ImageOffset = static_cast<int>(ImageH);
		ImageOffset = ImageOffset/2;
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer, m_ImageBuffer1, -ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_ShowBuffer, m_ImageBuffer2,  ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}	
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_4.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		this->DrawImageWndMemDC();
		this->RedrawWnd();
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_STOP);
		bFinish = true;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcCameraAlignHor3D(const TUNI_FRAME &UniFrame, double &HeightDif, double &HeightDifT, double &HeightDifB)
{
	size_t           idx=0;
	size_t           i=0, j=0, k=0;
	size_t           CountL=0;
	size_t           CountR=0;	
	size_t           CountDif=0;
	double           TempDif=0;
	double           HeightL=0;	
	double           HeightR=0;			
	IMAGE_SIZE       RangeW = 50;
	IMAGE_SIZE       RangeH = 50;
	IMAGE_SIZE       ImageW =UniFrame.ImageW;
	IMAGE_SIZE       ImageH =UniFrame.ImageH;
	IMAGE_SIZE       ImageC = (ImageW/2);
	IMAGE_SIZE       ImageL = (ImageC)-RangeW;
	IMAGE_SIZE       ImageR = (ImageC)+RangeW;
	IMAGE_SIZE       ImageStep =UniFrame.ImageStep;
	const MASK_PTR   MarsPtr=UniFrame.MaskPtr;
	const SPACE_PTR  SpacePtr=UniFrame.SpacePtr;	

	//右側高度		
	CountDif = 0;	
	HeightDifT = HeightDifB = HeightDif = 0;
	for ( i=0; i<ImageH; i+=RangeH )
	{
		CountL = 0;
		CountR = 0;
		HeightL = 0;		
		HeightR = 0;
		for ( k=i; k<i+RangeH; k++ )
		{
			if ( k >= ImageH )	{	break; }			
			for ( j=ImageL; j<ImageC-1; j++ )
			{
				idx = (k*ImageStep)+j;
				if ( ImageAPI.CheckSpaceMaskValid(MarsPtr[idx]) == false ) { continue; }			
				CountL ++;
				HeightL += SpacePtr[idx];

			}
			for ( j=ImageC+1; j<ImageR; j++ )
			{
				idx = (k*ImageStep)+j;
				if ( ImageAPI.CheckSpaceMaskValid(MarsPtr[idx]) == false ) { continue; }			
				CountR ++;
				HeightR += SpacePtr[idx];
			}			
		}
		if ( CountL > 0 )
		{	HeightL /= CountL; }
		if ( CountR > 0 )
		{	HeightR /= CountR; }

		TempDif = (HeightR-HeightL);		
		CountDif ++;
		HeightDif += fabs(TempDif);

		if ( i < RangeH )
		{	HeightDifT = TempDif;	}
		if ( i > (ImageH-RangeH) )
		{	HeightDifB = TempDif;	}
	}
	if ( CountDif > 0 )
	{	HeightDif /= CountDif; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcCameraAlignVer3D(const TUNI_FRAME &UniFrame, double &HeightDif, double &HeightDifL, double &HeightDifR)
{
	size_t           idx=0;	
	size_t           i=0, j=0, k=0;	
	size_t           CountT=0;
	size_t           CountB=0;
	size_t           CountDif=0;
	double           HeightT=0;
	double           HeightB=0;
	double           TempDif=0;
	IMAGE_SIZE       RangeH = 50;
	IMAGE_SIZE       RangeW = 50;
	IMAGE_SIZE       ImageW =UniFrame.ImageW;
	IMAGE_SIZE       ImageH =UniFrame.ImageH;
	IMAGE_SIZE       ImageC = (ImageH/2);
	IMAGE_SIZE       ImageT = (ImageC)-RangeH;
	IMAGE_SIZE       ImageB = (ImageC)+RangeH;
	IMAGE_SIZE       ImageStep =UniFrame.ImageStep;
	const MASK_PTR   MarsPtr=UniFrame.MaskPtr;
	const SPACE_PTR  SpacePtr=UniFrame.SpacePtr;
	
	CountDif=0;
	HeightDifR = HeightDifL = HeightDif = 0;
	for ( j=0; j<ImageW; j+=RangeW )
	{		
		CountT = 0;
		CountB = 0;
		HeightT = 0;		
		HeightB = 0;
		for ( k=j; k<j+RangeW; k++ )
		{
			if ( k >= ImageW )	{	break; }
			//上方高度	
			for ( i=ImageT; i<ImageC-1; i++ )
			{		
				idx = (i*ImageStep)+k;
				if ( ImageAPI.CheckSpaceMaskValid(MarsPtr[idx]) == false ) { continue; }			
				CountT ++;
				HeightT += SpacePtr[idx];
			}			

			//下方高度		
			for ( i=ImageC+1; i<ImageB; i++ )
			{
				idx = (i*ImageStep)+k;
				if ( ImageAPI.CheckSpaceMaskValid(MarsPtr[idx]) == false ) { continue; }			
				CountB ++;
				HeightB += SpacePtr[idx];
			}			
		}
		if ( CountT > 0 )
		{	HeightT /= CountT; }
		if ( CountB > 0 )
		{	HeightB /= CountB; }

		TempDif = (HeightB-HeightT);
		CountDif ++;
		HeightDif += fabs(TempDif);

		if ( 0 == j )
		{	HeightDifL = TempDif;	}
		if ( j >  (ImageW-RangeW) )
		{	HeightDifR = TempDif;	}
	}	
	if ( CountDif > 0 )
	{	HeightDif /= CountDif; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecCameraAlignHor3D(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;	
	bool              IsOK=true;	
	double            HeightDif=0;	
	double            HeightDifT=0;	
	double            HeightDifB=0;	
	int               ImageOffset=0;
	const bool        bUseRoi=false;
	TUNI_FRAME        FovUniFrame;
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;	
	const size_t       BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	MOTION_MOVING_MODE MotionMovingMode=MOTION_MOVING_NORMAL;
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();	
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;	
	bFinish = false;	
	switch ( CalMode )
	{
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_START:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif				
		SetSaveRawImageTimes(1);
		if ( true == m_Multi3DCastID )
		{	IsOK = Exec3DModelDataMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame);	}	
		else
		{	IsOK = Exec3DModelDataSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame); }		
		if ( false == IsOK )
		{	return false; }
		//因為底層是以Buffer1為主, 所以先貼上Buffer2
		::memcpy(m_MaskBuffer2, FovUniFrame.MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(m_ImageBuffer2, FovUniFrame.ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
		::memcpy(m_SpaceBuffer2, FovUniFrame.SpacePtr, sizeof(SPACE_DATA)*BufferSize);
		JetAPI::ClearUniFrame(FovUniFrame);
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_11.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer2, true);
	#endif				
		if ( true == SignX )
		{	NextPosX = m_StagePosX+(CaliParam.m_FOVWidth_um); }
		else
		{	NextPosX = m_StagePosX-(CaliParam.m_FOVWidth_um); }
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_CAMERA_ALIGN_HOR_3D_END);
		this->StartReGrab(FALSE);
		return true;
		break;
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif
		SetSaveRawImageTimes(2);
		if ( true == m_Multi3DCastID )
		{	IsOK = Exec3DModelDataMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame);	}	
		else
		{	IsOK = Exec3DModelDataSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame); }		
		if ( false == IsOK )
		{	return false; }
		::memcpy(m_MaskBuffer1, FovUniFrame.MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(m_ImageBuffer1, FovUniFrame.ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
		::memcpy(m_SpaceBuffer1, FovUniFrame.SpacePtr, sizeof(SPACE_DATA)*BufferSize);
		JetAPI::ClearUniFrame(FovUniFrame);
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_11.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer2, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_21.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer1, true);
	#endif
		::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
		::memset(m_ImageBuffer, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		::memset(m_SpaceBuffer, 0x00, sizeof(SPACE_DATA)*BufferSize);
		ImageOffset = static_cast<int>(ImageW);
		ImageOffset = ImageOffset/2;
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_MaskBuffer, m_MaskBuffer2, -ImageOffset, m_MaskBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineHorSpaceImage3(ImageW, ImageH, ImageStep, m_SpaceBuffer, m_SpaceBuffer2, -ImageOffset, m_SpaceBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer, m_ImageBuffer2, -ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_MaskBuffer, m_MaskBuffer1, ImageOffset, m_MaskBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineHorSpaceImage3(ImageW, ImageH, ImageStep, m_SpaceBuffer, m_SpaceBuffer1, ImageOffset, m_SpaceBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineHorGrayImage3(ImageW, ImageH, ImageStep, m_ShowBuffer, m_ImageBuffer1,  ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}	
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_HOR_3D_4.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif
		
		FovUniFrame.ImageW = ImageW;
		FovUniFrame.ImageH = ImageH;
		FovUniFrame.BitCount = BitCount;
		FovUniFrame.ImageStep = ImageStep;
		FovUniFrame.MaskPtr = m_MaskBuffer;		
		FovUniFrame.ImagePtr = m_ShowBuffer;
		FovUniFrame.SpacePtr = m_SpaceBuffer;

		//計算高度值		
		ExecShow3DModelWnd(CalMode, CameraID, FovUniFrame);
		CalcCameraAlignHor3D(FovUniFrame, HeightDif, HeightDifT, HeightDifB);
		str.Format(_T("Dif(ABS)=%.0fum, Dif(T)=%.0fum, Dif(B)=%.0fum"), HeightDif, HeightDifT, HeightDifB);
		JetAPI::ShowMessageBox(str);

		this->DrawImageWndMemDC();
		this->RedrawWnd();
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_STOP);
		bFinish = true;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecCameraAlignVer3D(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	bool              IsOK=true;
	double            HeightDif=0;	
	double            HeightDifR=0;
	double            HeightDifL=0;	
	int               ImageOffset=0;	
	const bool        bUseRoi=false;
	TUNI_FRAME        FovUniFrame;
	double            CurPosX=0, CurPosY=0, CurPosZ=0;
	double            NextPosX=0, NextPosY=0, NextPosZ=0;		
	const size_t       BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
	NextPosX = CurPosX;
	NextPosY = CurPosY;
	NextPosZ = CurPosZ;
	bFinish = false;	
	switch ( CalMode )
	{
	case CALIBRATION_CAMERA_ALIGN_VER_3D_START:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_1.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif
		SetSaveRawImageTimes(1);
		if ( true == m_Multi3DCastID )
		{	IsOK = Exec3DModelDataMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame);	}	
		else
		{	IsOK = Exec3DModelDataSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame); }		
		if ( false == IsOK )
		{	return false; }
		//因為底層是以Buffer1為主, 所以先貼上Buffer2
		::memcpy(m_MaskBuffer2, FovUniFrame.MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(m_ImageBuffer2, FovUniFrame.ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
		::memcpy(m_SpaceBuffer2, FovUniFrame.SpacePtr, sizeof(SPACE_DATA)*BufferSize);
		JetAPI::ClearUniFrame(FovUniFrame);
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_11.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer2, true);
	#endif
		if ( true == SignY )
		{	NextPosY = m_StagePosY-(CaliParam.m_FOVHeight_um); }		
		else
		{	NextPosY = m_StagePosY+(CaliParam.m_FOVHeight_um);	}
		if ( MotionCtrlPtr->XYMoveTo(NextPosX, NextPosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_CAMERA_ALIGN_VER_3D_END);
		this->StartReGrab(FALSE);
		return true;
		break;
	case CALIBRATION_CAMERA_ALIGN_VER_3D_END:
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_2.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	#endif
		SetSaveRawImageTimes(2);
		if ( true == m_Multi3DCastID )
		{	IsOK = Exec3DModelDataMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame);	}	
		else
		{	IsOK = Exec3DModelDataSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, FovUniFrame); }		
		if ( false == IsOK )
		{	return false; }
		::memcpy(m_MaskBuffer1, FovUniFrame.MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(m_ImageBuffer1, FovUniFrame.ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
		::memcpy(m_SpaceBuffer1, FovUniFrame.SpacePtr, sizeof(SPACE_DATA)*BufferSize);
		JetAPI::ClearUniFrame(FovUniFrame);
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_11.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer2, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_21.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer1, true);
	#endif
		::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
		::memset(m_ImageBuffer, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		::memset(m_SpaceBuffer, 0x00, sizeof(SPACE_DATA)*BufferSize);
		ImageOffset = static_cast<int>(ImageH);
		ImageOffset = ImageOffset/2;
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_MaskBuffer, m_MaskBuffer2, -ImageOffset, m_MaskBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineVerSpaceImage3(ImageW, ImageH, ImageStep, m_SpaceBuffer, m_SpaceBuffer2, -ImageOffset, m_SpaceBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_ImageBuffer, m_ImageBuffer2, -ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_3.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif

		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_MaskBuffer, m_MaskBuffer1, ImageOffset, m_MaskBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineVerSpaceImage3(ImageW, ImageH, ImageStep, m_SpaceBuffer, m_SpaceBuffer1, ImageOffset, m_SpaceBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}
		if ( ImageAPI.CombineVerGrayImage3(ImageW, ImageH, ImageStep, m_ShowBuffer, m_ImageBuffer1,  ImageOffset, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
			return false;
		}	
	#ifdef _DEBUG
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_ALIGN_VER_3D_4.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	#endif

		FovUniFrame.ImageW = ImageW;
		FovUniFrame.ImageH = ImageH;
		FovUniFrame.BitCount = BitCount;
		FovUniFrame.ImageStep = ImageStep;
		FovUniFrame.MaskPtr = m_MaskBuffer;		
		FovUniFrame.ImagePtr = m_ShowBuffer;
		FovUniFrame.SpacePtr = m_SpaceBuffer;
		ExecShow3DModelWnd(CalMode, CameraID, FovUniFrame);
		CalcCameraAlignVer3D(FovUniFrame, HeightDif, HeightDifL, HeightDifR);		
		str.Format(_T("Dif(ABS)=%.0fum, Dif(L)=%.0fum, Dif(R)=%.0fum"), HeightDif, HeightDifL, HeightDifR);
		JetAPI::ShowMessageBox(str);

		this->DrawImageWndMemDC();
		this->RedrawWnd();
		if ( MotionCtrlPtr->XYMoveTo(this->m_StagePosX, this->m_StagePosY) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		SetCalibrationMode(CALIBRATION_STOP);
		bFinish = true;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecImageResolution(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString     str;
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_IMAGE_RESOLUTION.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*ImageW*ImageStep);
	this->DrawImageWndMemDC();
	this->RedrawWnd();		
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecImageFocusAuto(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString     str, str1, str2;	
	BOOL        bSave = FALSE;
	RECT        ImageRect;
	TImageStat  Statistics;
	IMAGE_SIZE   GrayStep=0;
	IMAGE_SIZE   GrayBitCount = 8;
	double       CurPosX=0, CurPosY=0, CurPosZ=0;
	double       NextPosX=0, NextPosY=0, NextPosZ=0;	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const double Margin = 50;//um
	const double LimitMinZ = MotionCtrlPtr->GetMotionParameter().m_LimitMinZ + Margin;
	const double LimitMaxZ = MotionCtrlPtr->GetMotionParameter().m_LimitMaxZ - Margin;
	bFinish = false;
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_IMAGE_FOCUS_AUTO.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();	

	JetAPI::Rect4DToRect(m_ImageRect4D, ImageRect);
	IMAGE_SIZE RoiW = ImageRect.right-ImageRect.left;
	IMAGE_SIZE RoiH = ImageRect.bottom-ImageRect.top;
	IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, GrayBitCount, 4);	
	if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, ImageRect, RoiStep, m_ImageBuffer2, false) == false ) 
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		return false;
	}	
	if ( AOIDataCollect.BuildFocusImage(RoiW, RoiH, RoiStep, m_ImageBuffer2, m_ImageBuffer1) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	Statistics.m_Rect = ImageRect;	
	Statistics.m_Rect.left = 0;
	Statistics.m_Rect.right = RoiW;
	Statistics.m_Rect.top = 0;
	Statistics.m_Rect.bottom = RoiH;
	CALC_FOCUS_MODE CalcFocusMode = AOIDataCollect.GetSystemParameter().m_CalcFocusMode;		
	//if ( ImageAPI.CalcGrayImageStatistics(RoiW, RoiH, RoiStep, m_ImageBuffer1, Statistics) == false )
	if ( AOIDataCollect.CalcImageFocusValue(RoiW, RoiH, RoiStep, m_ImageBuffer2, m_ImageBuffer1, CalcFocusMode, Statistics.m_Rect, Statistics.m_Std) == false )
	{	
		//JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	Statistics.m_Std = Statistics.m_Std*3.0;		
	if ( Statistics.m_Std > m_AutoFocustBestStd )
	{
		m_AutoFocustBestPosZ = CurPosZ;
		m_AutoFocustBestStd = Statistics.m_Std;
	}
	str.Format(_T("Reading:%.2f[Z:%.0f]"), Statistics.m_Std, CurPosZ);
	this->SetDlgItemText(CALIALIGN_MOTION_POS_EDIT, str);
	TPOINT2D Reading(Statistics.m_Std, CurPosZ);
	m_AutoFocusReadingList.push_back(Reading);

	double Pitch = this->m_AutoFocusPitch*m_AutoFocustScalePosZ;	
	NextPosZ = CurPosZ + Pitch;
	const double DiffPosZ = abs(m_AutoFocustLastPosZ-CurPosZ);//可能碰到極限, 位置與上一次相同
	m_AutoFocustLastPosZ = CurPosZ;		
	if ( DiffPosZ>5.0 && NextPosZ<m_AutoFocustMaxPosZ )
	{		
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, NextPosZ, MOTION_MOVING_NORMAL) == false )
		{				
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
			return false;
		}	
		this->StartReGrab(FALSE);
		return true;
	}		
	if ( CALIBRATION_IMAGE_FOCUS_AUTO_100 == m_CalibrationMode )
	{
		NextPosZ = m_AutoFocustBestPosZ-Pitch;
		if ( NextPosZ < LimitMinZ )
		{	NextPosZ = LimitMinZ; }
		if ( NextPosZ > LimitMaxZ )
		{	NextPosZ = LimitMaxZ; }		
		m_AutoFocustMinPosZ = m_AutoFocustBestPosZ-(Pitch);
		m_AutoFocustMaxPosZ = m_AutoFocustBestPosZ+(Pitch);
		if ( m_AutoFocustMinPosZ < LimitMinZ )
		{	m_AutoFocustMinPosZ = LimitMinZ; }
		if ( m_AutoFocustMaxPosZ > LimitMaxZ )
		{	m_AutoFocustMaxPosZ = LimitMaxZ; }
		m_AutoFocustBestStd = 0;
		m_AutoFocustBestPosZ = NextPosZ;
		m_AutoFocustLastPosZ = -FLT_MAX;
		m_AutoFocustScalePosZ = 10.0;
		SetCalibrationMode(CALIBRATION_IMAGE_FOCUS_AUTO_10);
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, NextPosZ, MOTION_MOVING_NORMAL) == false )
		{				
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
			return false;
		}	
		this->StartReGrab(FALSE);
		TPOINT2D Reading;		
		m_AutoFocusReadingList.push_back(Reading);
		return true;
	}
	if ( CALIBRATION_IMAGE_FOCUS_AUTO_10 == m_CalibrationMode )
	{
		//Pitch = 2*Pitch;//2倍範圍
		NextPosZ = m_AutoFocustBestPosZ-Pitch;
		if ( NextPosZ < LimitMinZ )
		{	NextPosZ = LimitMinZ; }
		if ( NextPosZ > LimitMaxZ )
		{	NextPosZ = LimitMaxZ; }		
		m_AutoFocustMinPosZ = m_AutoFocustBestPosZ-(Pitch);
		m_AutoFocustMaxPosZ = m_AutoFocustBestPosZ+(Pitch);
		if ( m_AutoFocustMinPosZ < LimitMinZ )
		{	m_AutoFocustMinPosZ = LimitMinZ; }
		if ( m_AutoFocustMaxPosZ > LimitMaxZ )
		{	m_AutoFocustMaxPosZ = LimitMaxZ; }
		m_AutoFocustBestStd = 0;
		m_AutoFocustBestPosZ = NextPosZ;
		m_AutoFocustLastPosZ = -FLT_MAX;
		m_AutoFocustScalePosZ = 1.0;
		SetCalibrationMode(CALIBRATION_IMAGE_FOCUS_AUTO);
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, NextPosZ, MOTION_MOVING_NORMAL) == false )
		{				
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
			return false;
		}	
		this->StartReGrab(FALSE);
		TPOINT2D Reading;		
		m_AutoFocusReadingList.push_back(Reading);
		return true;
	}
	SaveAutoFocusReading(m_AutoFocusReadingList);

	//找出最大的數值
	str1 = _T("Auto Focus Done, Best Focus");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = _T("Do you want to set the value to system?");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s=%.2f, Z:%.0f.\n%s"), str1, m_AutoFocustBestStd, m_AutoFocustBestPosZ, str2);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	
		LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
		MotionCtrlPtr->SetFocusPosZ(m_AutoFocustBestPosZ, LaneID);
		MotionCtrlPtr->SaveMotionParameter();
	}
	
	if ( MotionCtrlPtr->MoveTo(AXIS_Z, m_AutoFocustBestPosZ, MOTION_MOVING_NORMAL) == false )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	str.Format(_T("%.2f %%"), m_AutoFocustBestStd);
	this->SetDlgItemText(CALIALIGN_MOTION_POS_EDIT, str);
	bFinish = true;
	SetCalibrationMode(CALIBRATION_STOP);
	this->StartReGrab(FALSE);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec2DLightAlign(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString str;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_2D_LIGHT_ALIGN.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif
	if ( ImageAPI.CalcGrayImageGridStatistics(ImageW, ImageH, ImageStep, ImagePtr, m_ImageGridList) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DCastAlign(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString str;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_CAST_ALIGN.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif	
	if ( ImageAPI.CalcGrayImageGridStatistics(ImageW, ImageH, ImageStep, ImagePtr, m_ImageGridList) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DCastFocus(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString     str;	
	BOOL        bSave = TRUE;
	RECT        ImageRect;
	TImageStat  Statistics, Grid;
	IMAGE_SIZE  DstW=0, DstH=0, DstStep=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_CAST_FOCUS-1.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif
	JetAPI::Rect4DToRect(m_ImageRect4D, ImageRect);	
	ImageRect.left = 0;
	ImageRect.right = ImageW;
	ImageRect.top = 0;
	ImageRect.bottom = ImageH;
	Statistics.m_Rect = ImageRect;		
	DstW = ImageRect.right-ImageRect.left;
	DstH = ImageRect.bottom-ImageRect.top;
	DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);

	const int Threhold = 10;
	const int Amount = 400;	
	::memset(m_ImageBuffer1, 0x00, sizeof(unsigned char)*DstH*DstStep);
	if ( AOIDataCollect.BuildFocusImage(ImageW, ImageH, ImageStep, ImagePtr, m_ImageBuffer1) == false )	
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_CAST_FOCUS-2.BMP"));
	ImageAPI.SaveBMPImage(str, DstW, DstH, DstStep, BitCount, m_ImageBuffer1, true);
#endif
	const double Ratio = 1;
	Statistics.m_Rect.left = 0;
	Statistics.m_Rect.right = DstW;
	Statistics.m_Rect.top = 0;
	Statistics.m_Rect.bottom = DstH;
	CALC_FOCUS_MODE CalcFocusMode = AOIDataCollect.GetSystemParameter().m_CalcFocusMode;	
	//if ( ImageAPI.CalcGrayImageStatistics(DstW, DstH, DstStep, m_ImageBuffer1, Statistics) == false )
	if ( AOIDataCollect.CalcImageFocusValue(DstW, DstH, DstStep, ImagePtr, m_ImageBuffer1, CalcFocusMode, Statistics.m_Rect, Statistics.m_Std) == false )
	{
		//JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	Statistics.m_Std = Statistics.m_Std*Ratio;
	str.Format(_T("%.2f %%"), Statistics.m_Std);
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT, str);
	
	std::vector<TImageStat> GridList;
	const unsigned int DstW2 = DstW/2;
	const unsigned int DstH2 = DstH/2;
	const unsigned int GridW = DstW/10;
	const unsigned int GridH = DstH/10;
	Grid.m_Rect.left = 0;	Grid.m_Rect.right = GridW;
	Grid.m_Rect.top = 0;    Grid.m_Rect.bottom = GridH;
	GridList.push_back(Grid);
	Grid.m_Rect.left = DstW-GridW;	Grid.m_Rect.right = DstW;
	Grid.m_Rect.top = 0;    Grid.m_Rect.bottom = GridH;
	GridList.push_back(Grid);
	Grid.m_Rect.left = 0;	Grid.m_Rect.right = GridW;
	Grid.m_Rect.top = DstH-GridH;   Grid.m_Rect.bottom = DstH;
	GridList.push_back(Grid);
	Grid.m_Rect.left = DstW-GridW;	Grid.m_Rect.right = DstW;
	Grid.m_Rect.top = DstH-GridH;   Grid.m_Rect.bottom = DstH;
	GridList.push_back(Grid);
	Grid.m_Rect.left = DstW2-(GridW/2);	Grid.m_Rect.right = DstW2+(GridW/2);
	Grid.m_Rect.top = DstH2-(GridH/2);   Grid.m_Rect.bottom = DstH2+(GridH/2);
	GridList.push_back(Grid);
	if ( ImageAPI.CalcGrayImageGridStatistics(DstW, DstH, DstStep, m_ImageBuffer1, GridList) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		return false;
	}
	Grid = GridList[0];
	Grid.m_Std = Grid.m_Std*Ratio;
	m_3DCastFocusLT = Grid.m_Std;
	str.Format(_T("%.2f %%"), Grid.m_Std);
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT_LT, str);
	Grid = GridList[1];
	Grid.m_Std = Grid.m_Std*Ratio;
	m_3DCastFocusRT = Grid.m_Std;
	str.Format(_T("%.2f %%"), Grid.m_Std);
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT_RT, str);
	Grid = GridList[2];
	Grid.m_Std = Grid.m_Std*Ratio;
	m_3DCastFocusLB = Grid.m_Std;
	str.Format(_T("%.2f %%"), Grid.m_Std);
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT_LB, str);
	Grid = GridList[3];
	Grid.m_Std = Grid.m_Std*Ratio;
	m_3DCastFocusRB = Grid.m_Std;
	str.Format(_T("%.2f %%"), Grid.m_Std);
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT_RB, str);
	Grid = GridList[4];
	Grid.m_Std = Grid.m_Std*Ratio;
	str.Format(_T("%.2f %%"), Grid.m_Std);
	m_3DCastFocusCC = Grid.m_Std;
	if ( m_3DCastFocusCCMax < m_3DCastFocusCC ) 
	{	m_3DCastFocusCCMax = m_3DCastFocusCC; }
	this->SetDlgItemText(CALIALIGN_3DCAST_FOCUS_EDIT_CC, str);	
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec2DLightCurrent(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString     str, str2;
	size_t      DstW=0, DstH=0, DstStep=0;
	RECT        ImageRect;
	TImageStat  Statistics;
	int         CurCurrent = 0;
	int         NextCurrent = 0;
	const bool  CalibrateAll = GetCalibrateAllCannel();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_2D_LIGHT_CURRENT.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif	
	JetAPI::Rect4DToRect(m_ImageRect4D, ImageRect);
	Statistics.m_Rect = ImageRect;
	if ( ImageAPI.CalcGrayImageStatistics(ImageW, ImageH, ImageStep, ImagePtr, Statistics) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		return false;
	}	
	CurCurrent = m_2DLEDCurrent;
	NextCurrent = CurCurrent+1;
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();
	int GrayTarget = m_GrayTarget;
	int CurRed=0, CurGrn=0, CurBlu = 0;	
	m_2DLEDCurrentGray = (float)(Statistics.m_Ave);
	T2D_CURRENT Reading(CurCurrent, Statistics.m_Ave);
	Reading.Gain = m_SliceParam.SliceGainValue;
	m_2DLEDCurrentReadingList.push_back(Reading);
	if ( Statistics.m_Ave<this->m_GrayTarget && NextCurrent<101 )
	{	
		this->m_2DLEDCurrent = NextCurrent;		
		for ( int i=0; i<LED_CHANNEL_COUNT; i++ )
		{
			if ( FN_DISABLE == m_SliceParam.SliceLightTable.LEDChannel[i].OnOffState ) { continue; }			
			m_SliceParam.SliceLightTable.LEDChannel[i].PowerValue = (unsigned int)(NextCurrent);
		}

		std::vector<TSliceParam> ParamList;	
		ParamList.push_back(m_SliceParam);	
		if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false;
		}
		
		this->StartReGrab(FALSE);
		return true;
	}
	else
	{
		m_2DLEDCurrent = CurCurrent;			
		SetCalibrationMode(CALIBRATION_STOP);

		T2D_CURRENT BestReading;
		float ResultGray=m_2DLEDCurrentGray;					
		double Gain = m_SliceParam.SliceGainValue;
		const double TargetGray=m_SliceParam.SliceTargetGray;				
		const std::vector<T2D_CURRENT> &ReadingList=m_2DLEDCurrentReadingList;
		const size_t ReadingCount=ReadingList.size();
		if ( CalcBest2DCurrent(ReadingList, TargetGray, BestReading) == true )
		{			
			ResultGray = BestReading.Gray;			
			m_2DLEDCurrent = BestReading.Current;
		}					
		
		if ( m_2DLEDCurrent>LED_CURRENT_WARNING_MAX || m_2DLEDCurrent<LED_CURRENT_WARNING_MIN )
		{
			str.Format(_T("%s Current [%03d] Exception"), m_SliceParam.SliceName, m_2DLEDCurrent);
			m_2DLEDCurrentAlarmList.push_back(str);
		}
		T2D_CURRENT Reading(m_2DLEDCurrent, ResultGray);
		CurCurrent = m_2DLEDCurrent;
		Reading.Gain = m_SliceParam.SliceGainValue;
		m_2DLEDCurrentReadingList.push_back(Reading);		
		Save2DCurrentReading(m_SliceParam, CalMode, m_2DLEDCurrentReadingList);
	}

	int SliceFrameIndex = 0;
	this->GetDlgItemText(CALIALIGN_LIGHT_CURRENT_GAIN_EDIT, str);	
	const double GainValue = ::_ttof(str);
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombo);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);	
	if ( NULL != SliceParamPtr)
	{	
		for ( int i=0; i<LED_CHANNEL_COUNT; i++ )
		{
			if ( FN_DISABLE == SliceParamPtr->SliceLightTable.LEDChannel[i].OnOffState ) { continue; }			
			SliceParamPtr->SliceLightTable.LEDChannel[i].PowerValue = (unsigned int)(CurCurrent);
			SliceParamPtr->SliceCameraExpTimeus = this->m_CameraExposureTime_us;
			SliceParamPtr->SliceGainValue = GainValue;
			SliceParamPtr->SliceTargetGray = m_GrayTarget;			
		}
	}	
	this->m_2DLEDCurrent = 0;	
	this->m_CameraExposureTime_us = m_CameraExposureTimeBackup_us;	
	this->SetDlgItemInt(CALIALIGN_LIGHT_CURRENT_SET_EDIT, CurCurrent);	
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);	
	if ( false == CalibrateAll )
	{
		str = _T("LED Current Tunning Done, Best Current");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s:(%d)"), str, CurCurrent);
		JetAPI::ShowMessageBox(str2);	
		bFinish = true;
		return true;
	}
	if ( true == CalibrateAll )
	{
		const int Count = m_SliceCombo.GetCount();
		const int CurSel = m_SliceCombo.GetCurSel();
		const int NextSel = CurSel+1;
		if ( NextSel < Count )
		{
			//下一個燈源
			m_SliceCombo.SetCurSel(NextSel);			
			UpdateSliceParamToUI();		
			m_2DLEDCurrent = 1;
			m_2DLEDCurrentGray = 0;			
			m_2DLEDCurrentReadingList.clear();
			m_CameraExposureTimeBackup_us = m_CameraExposureTime_us;
			m_GrayTarget = GetDlgItemInt(CALIALIGN_LIGHT_CURRENT_GRAY_EDIT);	
			m_CameraExposureTime_us = GetDlgItemInt(CALIALIGN_LIGHT_CURRENT_EXP_EDIT);
			if ( ConfigGrabParam(CALIBRATION_2D_LIGHT_CURRENT) == false )
			{	return false; }
			SetCalibrationMode(CALIBRATION_2D_LIGHT_CURRENT);	
			if ( this->ExecGrabFirst() == false )
			{	return false; }
			return true;
		}

		if ( m_2DLEDCurrentAlarmList.size() > 0 )
		{
			CString str2;
			const std::vector<CString> &AlarmList=m_2DLEDCurrentAlarmList;
			const size_t AlarmCount=AlarmList.size();
			for ( size_t i=0; i<AlarmCount; i++ )
			{
				const CString &strAlarm=AlarmList[i];
				if ( 0 == i )
				{	str = strAlarm; }
				else
				{
					str2= str;
					str.Format(_T("%s\n%s"), str2, strAlarm);
				}
			}
			JetAPI::ShowMessageBox(str);
		}

		str = _T("Do you want to save calibration parameters?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	AOIDataCollect.SaveSystemSliceParamINI();	}
		bFinish = true;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DCastCurrent(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString     str, str1, str2, str3;
	size_t      DstW=0, DstH=0, DstStep=0;
	RECT        ImageRect;
	TImageStat  Statistics;
	int         CurCurrent = 0;
	int         NextCurrent = 0;	
	const bool  CalibrateAll = GetCalibrateAllCastID();
	LIGHT_3D_CAST_ID  CastID = m_Light3DCastID;	
	LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	bFinish = false;
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_CAST_CURRENT-1.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif	
	JetAPI::Rect4DToRect(m_ImageRect4D, ImageRect);
	Statistics.m_Rect = ImageRect;
	if ( ImageAPI.CalcGrayImageStatistics(ImageW, ImageH, ImageStep, ImagePtr, Statistics) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		return false;
	}
	if ( NULL == Light3DPtr )
	{
		str.Format(_T("Error, Get Pattern Project Exception"));
		JetAPI::ShowMessageBox(str);
		return false;
	}
	const int MaxCurrent = Light3DPtr->GetLEDCurrentMax();
	const int PitchCurrent = CaliParam.m_3DCastCurrentPitch;
	const bool LEDColorUsed_Red = Light3DPtr->GetLEDColorUsed_Red();
	const bool LEDColorUsed_Grn = Light3DPtr->GetLEDColorUsed_Grn();
	const bool LEDColorUsed_Blu = Light3DPtr->GetLEDColorUsed_Blu();
	CurCurrent = m_3DCastCurrent;
	NextCurrent = CurCurrent+PitchCurrent;
	::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	this->DrawImageWndMemDC();
	this->RedrawWnd();
	int CurRed=0, CurGrn=0, CurBlu = 0;	
	const int CurrentID = GetDLPLEDCurrentID();
	m_3DCastCurrentGray = (int)(Statistics.m_Ave);
	if ( Statistics.m_Ave<this->m_GrayTarget && NextCurrent<MaxCurrent)
	{	
		switch ( CalMode )
		{
		case CALIBRATION_3D_CAST_CURRENT_RED:	CurRed = NextCurrent;	break;
		case CALIBRATION_3D_CAST_CURRENT_GRN:	CurGrn = NextCurrent;	break;
		case CALIBRATION_3D_CAST_CURRENT_BLU:	CurBlu = NextCurrent;	break;
		}		
		if ( Light3DCtrl.SetLight3DLEDCurrent(CastID, CurRed, CurGrn, CurBlu, CurrentID) == false )
		{	
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}				
		this->m_3DCastCurrent = NextCurrent;		
		this->StartReGrab(FALSE);
		return true;
	}
	else
	{
		int LEDColor=0;
		CALIBRATION_MODE NextCalMode;
		int NextCur_Red=0, NextCur_Grn=0, NextCur_Blu=0;		
		std::vector<TSliceParam> ParamList;
		ParamList.push_back(m_SliceParam);		
		switch ( CalMode )
		{
		case CALIBRATION_3D_CAST_CURRENT_RED:				
			m_3DCastCurRed = CurCurrent;
			LEDColor = DLP_LED_COLOR_RED;
			if ( true == LEDColorUsed_Grn )
			{
				NextCur_Grn = 1;
				LEDColor = DLP_LED_COLOR_GREEN;
				NextCalMode = CALIBRATION_3D_CAST_CURRENT_GRN;
			}
			else if ( true == LEDColorUsed_Blu )
			{
				NextCur_Blu = 1;
				LEDColor = DLP_LED_COLOR_BLUE;
				NextCalMode = CALIBRATION_3D_CAST_CURRENT_BLU;
			}
			else
			{
				LEDColor = DLP_LED_COLOR_WHITE;
				NextCalMode = CALIBRATION_STOP;	
			}
			SetCalibrationMode(NextCalMode);
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{
				Light3DPtr->GetDLPParam().m_LEDColor = LEDColor;
				if (LightCtrlBoard.BuildLCBTableFromSliceParamList(ParamList) == false)
				{
					JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
					return false;
				}
			}
			if ( CALIBRATION_STOP != NextCalMode )
			{
				if ( Light3DCtrl.SetLight3DLEDCurrent(CastID, NextCur_Red, NextCur_Grn, NextCur_Blu, CurrentID) == false )
				{	
					JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
					return false;
				}
				this->m_3DCastCurrent = 1;
				this->StartReGrab(FALSE);
				return true;
			}
			break;
		case CALIBRATION_3D_CAST_CURRENT_GRN:	
			m_3DCastCurGrn = CurCurrent;
			LEDColor = DLP_LED_COLOR_GREEN;
			if ( true == LEDColorUsed_Blu )
			{
				NextCur_Blu = 1;
				LEDColor = DLP_LED_COLOR_BLUE;
				NextCalMode = CALIBRATION_3D_CAST_CURRENT_BLU;
			}
			else
			{
				LEDColor = DLP_LED_COLOR_WHITE;
				NextCalMode = CALIBRATION_STOP;	
			}
			SetCalibrationMode(NextCalMode);
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{
				Light3DPtr->GetDLPParam().m_LEDColor = LEDColor;
				if (LightCtrlBoard.BuildLCBTableFromSliceParamList(ParamList) == false)
				{
					JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
					return false;
				}
			}
			if ( CALIBRATION_STOP != NextCalMode )
			{
				if ( Light3DCtrl.SetLight3DLEDCurrent(CastID, NextCur_Red, NextCur_Grn, NextCur_Blu, CurrentID) == false )
				{	
					JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
					return false;
				}
				this->m_3DCastCurrent = 1;
				this->StartReGrab(FALSE);
				return true;
			}
			break;
		case CALIBRATION_3D_CAST_CURRENT_BLU:
			m_3DCastCurBlu = CurCurrent;
			LEDColor = DLP_LED_COLOR_WHITE;
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{				
				Light3DPtr->GetDLPParam().m_LEDColor = LEDColor;
				if (LightCtrlBoard.BuildLCBTableFromSliceParamList(ParamList) == false)
				{
					JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
					return false;
				}
			}
			SetCalibrationMode(CALIBRATION_STOP);
			break;
		}
	}
	CurRed = m_3DCastCurRed;
	CurGrn = m_3DCastCurGrn;
	CurBlu = m_3DCastCurBlu;
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	if ( Light3DCtrl.SetLight3DLEDColor(CastID, DLPLEDColor) == false )
	{	JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());	}
	if ( Light3DCtrl.SetLight3DLEDEnabled(CastID, true, true, true, true) == false )
	{
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());		
		return false;
	}
	if ( Light3DCtrl.SetLight3DLEDCurrent(CastID, CurRed, CurGrn, CurBlu, CurrentID) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}		
	this->m_CameraExposureTime_us = m_CameraExposureTimeBackup_us;	
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);	
	this->m_3DCastCurrent = 0;
	str1 = _T("Pattern current tuning finish, result current");
	str1 = LoadMultiLanguageString(str1, str1);
	str2 = _T("Do you want to save calibration parameters?");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s(%d, %d, %d)\n%s"), str1, CurRed, CurGrn, CurBlu, str2);

	if ( false == CalibrateAll )
	{
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			const bool bParamOnly=true;
			if ( Light3DCtrl.SaveLight3DCastParameter(CastID, bParamOnly) == false ) 
			{
				str3 = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str3);
			}
		}
		Update3DCastCurrentToUI();
		bFinish = true;	
		return true;
	}

	if ( true == CalibrateAll )
	{
		const int Count = m_3DCastIDCombox.GetCount()-1;
		const int CurSel = m_3DCastIDCombox.GetCurSel();
		const int NextSel = CurSel+1;
		m_3DCastIDCombox.SetCurSel(NextSel);
		Update3DCastCurrentToUI();
		if ( NextSel < Count )
		{	
			m_3DCastCurRed = 0;//3D投光電流-Red
			m_3DCastCurGrn = 0;//3D投光電流-Grn
			m_3DCastCurBlu = 0;//3D投光電流-Blu	
			m_3DCastCurrent = 1;	
			m_3DCastCurrentGray = 0;
			CALIBRATION_MODE NextCalMode = CALIBRATION_STOP;
			m_CameraExposureTimeBackup_us = m_CameraExposureTime_us;
			m_3DCastExposureTimeus = CWnd::GetDlgItemInt(CALIALIGN_3D_CAST_CURRENT_EXP_EDIT);		
			if ( true == LEDColorUsed_Red ) { NextCalMode = CALIBRATION_3D_CAST_CURRENT_RED; }
			else if ( true == LEDColorUsed_Grn ) { NextCalMode = CALIBRATION_3D_CAST_CURRENT_GRN; }
			else if ( true == LEDColorUsed_Blu ) { NextCalMode = CALIBRATION_3D_CAST_CURRENT_BLU; }
			else {	NextCalMode = CALIBRATION_STOP;	}
			if ( ConfigGrabParam(NextCalMode) == false )
			{	return false; }				
			SetCalibrationMode(NextCalMode);	
			if ( this->ExecGrabFirst() == false )
			{	return false;	}
			return true;
		}
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDYES )
		{	
			if ( Light3DCtrl.SaveAllLight3DCastParameter() == false ) 
			{
				str3 = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str3);
			}
		}		
		bFinish = true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseZeroPlane(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString str;	
	bFinish = false;				
	LIGHT_3D_CAST_ID  CastID = GetLight3DCastID();	
	const bool        bSaveRaw = GetSaveRawImage();		
	const bool CalibrateAll = GetCalibrateAllCastID();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	MotionCtrlPtr->MoveTo(AXIS_Z, m_StagePosZ, MOTION_MOVING_NORMAL);
	MotionCtrlPtr->WaitForDone(AXIS_Z);	
	UpdateStagePosition();
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}		
		break;
	}	
	const TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();		
	if ( FN_ENABLE == CaliParam.m_EnableBasePhaseCorrect )
	{
		CString Keyname;
		CString Filename;		
		CString Folder = GetSaveRawImageFolder();		
		CString ExtName = GetSaveRawImageExtName();
		PHASE_PTR PhasePtr2=NULL;	
		const bool bDebug = true;
		const bool bReverse = true;
		const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const int Times=CaliParam.m_BasePhaseCorrectTimes;
		CString CastName = AOIDataDefine.GetLight3DCastIDText(CastID);
		LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);		
		if ( true==bSaveRaw && NULL!=CastPtr )
		{
			Keyname = _T("PhaseZeroBefore");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, m_PhaseBuffer);
			if ( true == bDebug )
			{					
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);				
				ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer1, false);
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}
		}
		if ( ImageAPI.CorrectPhaseZero_Joe(ImageW, ImageH, ImageStep, m_PhaseBuffer, PhasePtr2, Times) == true )
		{
			if ( NULL != PhasePtr2 );
			{	::memcpy(m_PhaseBuffer, PhasePtr2, sizeof(PHASE_DATA)*BufferSize);	}
			JetMemory.free_func(PhasePtr2);
		}

		if ( true==bSaveRaw && NULL!=CastPtr )
		{
			Keyname = _T("PhaseZeroResult");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, m_PhaseBuffer);
			if ( true == bDebug )
			{					
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
				ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer1, false);
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}
		}
	}
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer, false) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	
	BOOL  bRepeat = CWnd::IsDlgButtonChecked(CALIALIGN_GRAB_REPEAT_CHK);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);		
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_ZERO_PLANE_PERIOD-2.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif
	this->DrawImageWndMemDC();
	this->RedrawWnd();	

	if ( FALSE == bRepeat )
	{	
		DWORD Ret=0;		
		if ( false == CalibrateAll )
		{
			str.Format(_T("Do you want to update phase zero plane ?"));
			str = LoadMultiLanguageString(str, str);
			Ret = JetAPI::ShowMessageBox(str, MB_YESNO);
		}
		else
		{	Ret = IDYES;	}
		
		if ( IDYES == Ret )
		{			
			const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);			
			if ( Light3DCtrl.SetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, m_PhaseBuffer) == false )
			{	
				JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
				return false;
			}
			SetModifiedCaliParam(true);
		}
	}	

	//this->m_CameraExposureTime_us = this->GetDlgItemInt(CALIALIGN_TARGET_WHITE_EXP_EDIT);
	//this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	SetCalibrationMode(CALIBRATION_STOP);
	if ( false == CalibrateAll )
	{
		bFinish = true;
		return true; 
	}
	if ( true == CalibrateAll )
	{	
		if ( CalibrateNext3DCastID(CALIBRATION_PATTERN_ZERO_PLANE) == true )
		{
			if ( MoveToPhaseZeroPlane() == false )
			{	return false;	}
			if ( this->ExecGrabFirst() == false )
			{	return false;	}
			return true;
		}
		UpdateStagePosition();
		str = _T("Do you want to save calibration parameters?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			if ( Light3DCtrl.SaveAllLight3DCastParameter() == false ) 
			{
				str = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str);
			}
		}	
		bFinish = true;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelTest(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	MotionCtrlPtr->MoveTo(AXIS_Z, m_StagePosZ, MOTION_MOVING_NORMAL);
	const BOOL        bPhaseMode = CWnd::IsDlgButtonChecked(CALIALIGN_TEST_IMAGE_PHASE_CHK);
	if ( true == m_Multi3DCastID )
	{	return Exec3DModelTestMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr, bFinish);	}	
	if ( FALSE == bPhaseMode )
	{	return Exec3DModelTestSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr, bFinish); }
	return Exec3DModelTestSingleCastID_2(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr, bFinish);
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelTestMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	size_t            i = 0;	
	RECT              TagRect={0};
	RECT              RoiRect={0};		
	TUNI_FRAME        ModelUniFrame;		
	const bool        bUseRoi = true;	
	bFinish = false;	
	if ( Exec3DModelDataMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, ModelUniFrame) == false )
	{	return false; }

	if ( ExecShow3DModelWnd(CalMode, CameraID, ModelUniFrame) == false )
	{
		JetAPI::ClearUniFrame(ModelUniFrame);
		return false;
	}	
	JetAPI::ClearUniFrame(ModelUniFrame);

	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelTestSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	size_t            i = 0;	
	bool              bIsOK = true;	
	int               nPhase = 0;
	RECT              TagRect={0};
	RECT              RoiRect={0};
	TUNI_FRAME        ModelUniFrame;	
	const bool        bUseRoi = true;	
	if ( Exec3DModelDataSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, ModelUniFrame) == false )
	{	return false;	}
	
	if ( ExecShow3DModelWnd(CalMode, CameraID, ModelUniFrame) == false )
	{
		JetAPI::ClearUniFrame(ModelUniFrame);
		return false;
	}
	JetAPI::ClearUniFrame(ModelUniFrame);

	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelTestSingleCastID_2(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)	
{
	CString           str;
	size_t            i = 0;	
	int               nPhase = 0;
	RECT              TagRect={0};
	RECT              RoiRect={0};	
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	MASK_PTR          MaskPtr = NULL;
	SPACE_PTR         FactorPtr = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;			
	LIGHT_3D_CAST_ID  CastID = m_Light3DCastID;
	const bool        bSaveRaw = GetSaveRawImage();	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int         PhaseConvertHeightMode = SysParam.m_PhaseConvertHeightMode;

	bFinish = false;
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);	
	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}	
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
	{
		str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, FactorPtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==FactorPtr))
	{
		str.Format(_T("Error, Phase Factor size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	const size_t       BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);	
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}		
		break;
	}	
#ifdef _DEBUG
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer, false) == true )
	{
		str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-OriPhase"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	}	
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, ZeroPhasePtr, ImageStep, m_ShowBuffer, false) == true )
	{
		str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-BasePlane"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	}		
#endif	
	for ( i=0; i<BufferSize; i++ )
	{
		nPhase  = m_PhaseBuffer[i];
		nPhase -= ZeroPhasePtr[i];		
		if ( nPhase < 0 ) 
		{	nPhase += PHASE_PERIOD; }
		else if ( nPhase > PHASE_PERIOD ) 
		{	nPhase -= PHASE_PERIOD; }	
		m_PhaseBuffer[i] = static_cast<PHASE_DATA>(nPhase);
	}

	JetAPI::Rect4DToRect(m_ImageRect4D, RoiRect);
	RoiRect.right = RoiRect.left + JetAPI::GetBMPImagePixelsPerLine(RoiRect.right-RoiRect.left, 8, 4);
	DstW = RoiRect.right-RoiRect.left;
	DstH = RoiRect.bottom-RoiRect.top;
	DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);	
	const size_t DstBufferSize = ImageAPI.CalcBufferSize(DstStep, DstH);
	if ( ImageAPI.ExtractPhaseRoiImage3(ImageW, ImageH, ImageStep, BitCount, m_PhaseBuffer, RoiRect, DstStep, m_PhaseBuffer1, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	if ( ImageAPI.ExtractSpaceRoiImage3(ImageW, ImageH, ImageStep, BitCount, FactorPtr, RoiRect, DstStep, m_SpaceBuffer1, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, m_ImageBuffer1, RoiRect, DstStep, m_ImageBuffer2, false ) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, m_MaskBuffer, RoiRect, DstStep, MaskPtr, false ) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	ImageAPI.GrayImageOffsetGain3(DstW, DstH, DstStep, m_ImageBuffer2, m_ImageBuffer2, 0, 2);

	this->m_ImgTargetW = DstW;
	this->m_ImgTargetH = DstH;
	this->m_ImgTargetStep = DstStep;
	if ( ImageAPI.PhaseGrayImageConvertToGray3(DstW, DstH, DstStep, m_PhaseBuffer1, DstStep, m_ShowBuffer1, false) == false )
	{		
		JetMemory.free_func(MaskPtr);
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	this->CreateImageTargetWndMemDC();

	const int LevelMode = 1;
	if ( PHASE_CONVERT_HEIGHT_SCALE == PhaseConvertHeightMode )
	{
		if ( ImageAPI.PhaseImageToSpaceImage3(DstW, DstH, DstStep, m_PhaseBuffer1, m_SpaceBuffer1, MaskPtr, m_SpaceBuffer) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
	}
	else
	{
		double  HeightFactor0[HEIGHT_FACTOR_PARAM_COUNT];
		double  HeightFactor1[HEIGHT_FACTOR_PARAM_COUNT];
		double  HeightFactor2[HEIGHT_FACTOR_PARAM_COUNT];
		if ( Light3DCtrl.GetLight3DHeightFactor(CastID, HeightFactor0, HeightFactor1, HeightFactor2) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}
		if ( ImageAPI.PhaseImageToSpaceImageByMapping3(PhaseConvertHeightMode, DstW, DstH, DstStep, m_PhaseBuffer1, RoiRect.left, RoiRect.top, HeightFactor0, HeightFactor1, HeightFactor2, MaskPtr, m_SpaceBuffer) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
	}
	MASK_PTR Mask2DPtr = NULL;	
	TNoiseFilterParam FilterParam;	
	GetSpaceNoiseFilterParam(FilterParam);			
	const bool bOpenMP = true;
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, DstW*DstH);	
	if ( ImageAPI.BuildSpaceData3(DstW, DstH, DstStep, m_SpaceBuffer, MaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, m_SpaceBuffer1, MaskPtr) == false )
	{
		JetMemory.free_func(MaskPtr);
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	::memcpy(m_SpaceBuffer, m_SpaceBuffer1, sizeof(SPACE_DATA)*DstBufferSize);

	//PHASE_TO_IMAGE_FIXED_SCALE, PHASE_TO_IMAGE_DYNAMIC_SCALE
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_TEST_IMAGE_PHASE_CHK) == TRUE )
	{	m_PhaseImageWnd.SetPhaseBuffer(DstW, DstH, DstStep, MaskPtr, m_PhaseBuffer1, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	}
	else
	{	m_PhaseImageWnd.SetSpaceBuffer(DstW, DstH, DstStep, MaskPtr, m_SpaceBuffer, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	}	
	
	if ( m_PhaseImageWnd.IsWindowVisible() == TRUE )
	{	m_PhaseImageWnd.RedrawWnd();	}
	else
	{	m_PhaseImageWnd.ShowWindow(SW_SHOW);	}

	bool bZeroNoise=true;
	if ( true == bZeroNoise ) 
	{
		for ( i=0; i<(DstStep*DstH); i++ )
		{			
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[i]) == false )
			{	m_SpaceBuffer[i] = 0x00; }
		}
	}
	float PadHeight = 0;
	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;
	if ( m_Draw3DWnd.CheckCalcObject(DstW, DstH) == true )	
	{	CalcObject(DstW, DstH, DstStep, MaskPtr, m_SpaceBuffer, TagRect, PadHeight); }
	else
	{
		::memset(&TagRect, 0x00, sizeof(TagRect));
		::memset(&RoiRect, 0x00, sizeof(RoiRect));
	}
	m_Draw3DWnd.Set3DData(m_SpaceBuffer, m_ImageBuffer2, DstW, DstH, DstStep, false, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, RoiRect, PadHeight);//m_ShowBuffer1	
	m_Draw3DWnd.ShowWindow(SW_SHOW);

	//str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-Mask"));
	//ImageAPI.SaveBMPImage(str, DstW, DstH, DstStep, BitCount, MaskPtr, false);
	//m_MaskImageWnd.ExecLoadMaskFile(str);
	//m_MaskImageWnd.ShowWindow(SW_SHOW);

	JetMemory.free_func(MaskPtr);
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer, false) == false )
	{		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
#ifdef _DEBUG
	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-ResultPhase"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif	

	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelDataMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame)
{
	//return Exec3DModelDataMultiCastID_1(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, DstUniFrame);
	return Exec3DModelDataMultiCastID_2(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, DstUniFrame);
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelDataMultiCastID_1(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame)
{
	CString           str;
	size_t            i = 0;	
	size_t            CastIndex = 0;
	LIGHT_3D_CLS_PTR  CastPtr = NULL;
	const size_t      UniFrameCount = 8;		
	IMAGE_PTR         CameraImage[UniFrameCount]={NULL};
	TUNI_FRAME        RoiUniFrame[UniFrameCount];
	LIGHT_3D_CAST_ID  CastID = LIGHT_3D_CAST_00;		
	CComboBox        &CastIDCombox = m_3DCastIDCombox;	
	const int         CastIDCount = CastIDCombox.GetCount();
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);		
	const int  DLPLightCnt = AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	const bool bReSortCameraImage = AOIDataCollect.CheckReSortCameraImage(SliceFuncMode);

	//Initial Parameters
	JetAPI::InitialUniFrame(DstUniFrame);
	for ( i=0; i<UniFrameCount; i++ )
	{	
		CameraImage[i] = NULL;
		::memset(&RoiUniFrame[i], 0x00, sizeof(RoiUniFrame[i]));	
	}	
	if ( true == bReSortCameraImage )
	{	
		if ( CameraCtrl.ReSortCameraRingBufferImage(CameraID, m_SliceParam) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
	}

	CastIndex = 0;	
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = (LIGHT_3D_CAST_ID)(CastIDCombox.GetItemData(i));	
		if ( LIGHT_3D_CAST_00 == CastID ) { continue; }
		CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL == CastPtr ) { continue; }		
		if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], RoiUniFrame[CastIndex]) == false )
		{
			for ( i=0; i<UniFrameCount; i++ )
			{	JetMemory.free_func(CameraImage[i]); }
			JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);	
			return false;	
		}
		CastIndex ++;
	}

	bool bIsOK = true;	
	const size_t CastUsedCount = CastIndex;	
	switch ( CastUsedCount )
	{
	case 1:	bIsOK = ImageAPI.CloneGrayImage3(ImageW, ImageH, ImageStep, CameraImage[0], m_ShowBuffer, false);	break;
	case 2:	bIsOK = ImageAPI.MergeGrayImage2Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], m_ShowBuffer);	break;
	case 3: bIsOK = ImageAPI.MergeGrayImage3Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], CameraImage[2], m_ShowBuffer);	break;
	case 4:	bIsOK = ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], CameraImage[2], CameraImage[3], m_ShowBuffer);	break;
	}	
	for ( i=0; i<UniFrameCount; i++ )
	{	JetMemory.free_func(CameraImage[i]); }
	if ( false == bIsOK ) 
	{ 	
		JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
		return false; 
	}
#ifdef _DEBUG
	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-ResultPhase"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif	

	//Check Size	
	IMAGE_SIZE        ModelW = 0;
	IMAGE_SIZE        ModelH = 0;
	IMAGE_SIZE        ModelStep = 0;
	MASK_PTR          ModelMaskPtr = NULL;	
	IMAGE_PTR         ModelImagePtr = NULL;
	SPACE_PTR         ModelSpacePtr = NULL;
	PHASE_PTR         ModelPhasePtr = NULL;

	ModelW = RoiUniFrame[0].ImageW;
	ModelH = RoiUniFrame[0].ImageH;
	ModelStep = RoiUniFrame[0].ImageStep;
	for ( i=1; i<UniFrameCount; i++ )
	{
		if ( NULL==RoiUniFrame[i].ImagePtr || NULL==RoiUniFrame[i].MaskPtr || NULL==RoiUniFrame[i].PhasePtr || NULL==RoiUniFrame[i].SpacePtr ) { continue; }

		if ( ModelW!=RoiUniFrame[i].ImageW || ModelH!=RoiUniFrame[i].ImageH || ModelStep!=RoiUniFrame[i].ImageStep )
		{	
			JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
			return false;
		}
	}		

	const bool bOpenMP = false;
	switch ( CastUsedCount )
	{
	case 1: 		
		if ( ImageAPI.CloneGrayImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].MaskPtr, ModelMaskPtr, false) == false ||
		     ImageAPI.CloneSpaceGrayImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].SpacePtr, ModelSpacePtr, false) == false )
		{
			bIsOK = false;
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ModelSpacePtr);
		}		
		break;
	case 2:	
		bIsOK = ImageAPI.Merge2SpaceImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].SpacePtr, RoiUniFrame[0].MaskPtr, RoiUniFrame[1].SpacePtr, RoiUniFrame[1].MaskPtr, bOpenMP, ModelSpacePtr, ModelMaskPtr);
		break;
	case 3:
		bIsOK = ImageAPI.Merge3SpaceImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].SpacePtr, RoiUniFrame[0].MaskPtr, RoiUniFrame[1].SpacePtr, RoiUniFrame[1].MaskPtr, RoiUniFrame[2].SpacePtr, RoiUniFrame[2].MaskPtr, bOpenMP, ModelSpacePtr, ModelMaskPtr);
		break;
	case 4:
		bIsOK = ImageAPI.Merge4SpaceImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].SpacePtr, RoiUniFrame[0].MaskPtr, RoiUniFrame[1].SpacePtr, RoiUniFrame[1].MaskPtr, RoiUniFrame[2].SpacePtr, RoiUniFrame[2].MaskPtr, RoiUniFrame[3].SpacePtr, RoiUniFrame[3].MaskPtr, bOpenMP, ModelSpacePtr, ModelMaskPtr);
		break;
	}
	if ( false == bIsOK ) 
	{ 
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelSpacePtr);
		JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
		return false; 
	}	
	switch ( CastUsedCount )
	{
	case 1:	bIsOK = ImageAPI.CloneGrayImage(ModelW, ModelH, ModelStep, RoiUniFrame[0].ImagePtr, ModelImagePtr, false);	break;
	case 2:	bIsOK = ImageAPI.MergeGrayImage2Frame(ModelW, ModelH, ModelStep, RoiUniFrame[0].ImagePtr, RoiUniFrame[1].ImagePtr, ModelImagePtr);	break;
	case 3: bIsOK = ImageAPI.MergeGrayImage3Frame(ModelW, ModelH, ModelStep, RoiUniFrame[0].ImagePtr, RoiUniFrame[1].ImagePtr, RoiUniFrame[2].ImagePtr, ModelImagePtr);	break;
	case 4:	bIsOK = ImageAPI.MergeGrayImage4Frame(ModelW, ModelH, ModelStep, RoiUniFrame[0].ImagePtr, RoiUniFrame[1].ImagePtr, RoiUniFrame[2].ImagePtr, RoiUniFrame[3].ImagePtr, ModelImagePtr);	break;
	}	
	if ( false == bIsOK ) 
	{ 		
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelImagePtr);
		JetMemory.free_func(ModelSpacePtr);
		JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
		return false; 
	}
	JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);

	DstUniFrame.ImageW = ModelW;
	DstUniFrame.ImageH = ModelH;
	DstUniFrame.ImageStep = ModelStep;
	DstUniFrame.BitCount = BitCount;
	DstUniFrame.ImagePtr = ModelImagePtr;
	DstUniFrame.MaskPtr = ModelMaskPtr;
	DstUniFrame.SpacePtr = ModelSpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelDataMultiCastID_2(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame)
{
	const char fnName[] = "CCaliPaneAlign::Exec3DModelDataMultiCastID_2";
	CString           str;
	size_t            i = 0;
	size_t            CastIndex = 0;	
	LIGHT_3D_CLS_PTR  CastPtr = NULL;
	const size_t      UniFrameCount = 8;		
	TCastParam        CastParam[UniFrameCount];
	IMAGE_PTR         CameraImage[UniFrameCount]={NULL};		
	LIGHT_3D_CAST_ID  CastID = LIGHT_3D_CAST_00;	
	CComboBox        &CastIDCombox = m_3DCastIDCombox;	
	const int         CastIDCount = CastIDCombox.GetCount();
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);		
	const int  DLPLightCnt = AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	const bool bReSortCameraImage = AOIDataCollect.CheckReSortCameraImage(SliceFuncMode);

	//Initial Parameters
	JetAPI::InitialUniFrame(DstUniFrame);
	for ( i=0; i<UniFrameCount; i++ )
	{	CameraImage[i] = NULL;	}	
	if ( true == bReSortCameraImage )
	{	
		if ( CameraCtrl.ReSortCameraRingBufferImage(CameraID, m_SliceParam) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
	}
	
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = (LIGHT_3D_CAST_ID)(CastIDCombox.GetItemData(i));	
		if ( LIGHT_3D_CAST_00 == CastID ) { continue; }
		CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL == CastPtr ) { continue; }		
		if ( Exec3DCastParamByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, CameraImage[CastIndex], CastParam[CastIndex]) == false )
		{
			for ( i=0; i<UniFrameCount; i++ )
			{	JetMemory.free_func(CameraImage[i]); }		
			return false;	
		}
		CastIndex ++;
	}

	bool bIsOK = true;	
	const size_t CastUsedCount = CastIndex;		
	switch ( CastUsedCount )
	{
	case 1:	bIsOK = ImageAPI.CloneGrayImage3(ImageW, ImageH, ImageStep, CameraImage[0], m_ShowBuffer, false);	break;
	case 2:	bIsOK = ImageAPI.MergeGrayImage2Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], m_ShowBuffer);	break;
	case 3: bIsOK = ImageAPI.MergeGrayImage3Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], CameraImage[2], m_ShowBuffer);	break;
	case 4:	bIsOK = ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, CameraImage[0], CameraImage[1], CameraImage[2], CameraImage[3], m_ShowBuffer);	break;
	}	
	for ( i=0; i<UniFrameCount; i++ )
	{	JetMemory.free_func(CameraImage[i]); }
	if ( false == bIsOK ) 
	{	return false;	}
#ifdef _DEBUG
	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-ResultPhase"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif	

	MASK_PTR MaskPtrM = NULL;
	SPACE_PTR SpacePtrM = NULL;
	const bool bOpenMP = true;
	TPhaseNoiseParam NoiseParam;
	GetPhaseNoiseParam(NoiseParam);
	if ( BATCH_GRAB_PHASE_M2 == m_PhaseID )
	{
		switch ( CastUsedCount )
		{
		case 1:	bIsOK = ImageAPI.PatternCast1ToSpace2Exp(ImageW, ImageH, ImageStep, CastParam[0], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;			
		case 2: bIsOK = ImageAPI.PatternCast2ToSpace2Exp(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;
		case 3: bIsOK = ImageAPI.PatternCast3ToSpace2Exp(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;		
		case 4: bIsOK = ImageAPI.PatternCast4ToSpace2Exp(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;
		default:
			bIsOK = false;
			break;
		}
	}
	else
	{
		switch ( CastUsedCount )
		{
		case 1:	bIsOK = ImageAPI.PatternCast1ToSpace(ImageW, ImageH, ImageStep, CastParam[0], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;			
		case 2: bIsOK = ImageAPI.PatternCast2ToSpace(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;
		case 3: bIsOK = ImageAPI.PatternCast3ToSpace(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;		
		case 4: bIsOK = ImageAPI.PatternCast4ToSpace(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	break;
		default:
			bIsOK = false;
			break;
		}
	}	
	
	if ( false == bIsOK )
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);
		return false;
	}
	
	RECT RoiRect;
	if ( false == bUseRoi )
	{	JetAPI::SizeToRect(ImageW, ImageH, RoiRect); }
	else
	{
		JetAPI::Rect4DToRect(m_ImageRect4D, RoiRect); 
		RoiRect.right = RoiRect.left + JetAPI::GetBMPImagePixelsPerLine(RoiRect.right-RoiRect.left, 8, 4);
	}	
	const int nAlign = 4;
	const int DstW = RoiRect.right-RoiRect.left;
	const int DstH = RoiRect.bottom-RoiRect.top;
	const int DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, (int)BitCount, nAlign);	
	const size_t DstBufferSize = ImageAPI.CalcBufferSize(DstStep, DstH);

	//Check Size	
	IMAGE_SIZE        ModelW = 0;
	IMAGE_SIZE        ModelH = 0;
	IMAGE_SIZE        ModelStep = 0;
	MASK_PTR          ModelMaskPtr = NULL;	
	IMAGE_PTR         ModelImagePtr = NULL;
	SPACE_PTR         ModelSpacePtr = NULL;
	PHASE_PTR         ModelPhasePtr = NULL;

	ModelW = (IMAGE_SIZE)(DstW);
	ModelH = (IMAGE_SIZE)(DstH);
	ModelStep = (IMAGE_SIZE)(DstStep);
	if ( JetMemory.alloc_func(DstBufferSize, ModelMaskPtr, fnName, "ModelMaskPtr") == false || 
		 JetMemory.alloc_func(DstBufferSize, ModelImagePtr, fnName, "ModelImagePtr") == false ||
		 JetMemory.alloc_func(DstBufferSize, ModelSpacePtr, fnName, "ModelSpacePtr") == false )
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);

		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelImagePtr);		
		JetMemory.free_func(ModelSpacePtr);
		return false;
	}
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, MaskPtrM, RoiRect, DstStep, ModelMaskPtr, false) == false ||
		 ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, RoiRect, DstStep, ModelImagePtr, false) == false ||		
		 ImageAPI.ExtractSpaceRoiImage3(ImageW, ImageH, ImageStep, BitCount, SpacePtrM, RoiRect, DstStep, ModelSpacePtr, false) == false )
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);

		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelImagePtr);
		JetMemory.free_func(ModelSpacePtr);
		return false;
	}
	JetMemory.free_func(MaskPtrM);
	JetMemory.free_func(SpacePtrM);

	DstUniFrame.ImageW = ModelW;
	DstUniFrame.ImageH = ModelH;
	DstUniFrame.ImageStep = ModelStep;
	DstUniFrame.BitCount = BitCount;
	DstUniFrame.ImagePtr = ModelImagePtr;
	DstUniFrame.MaskPtr = ModelMaskPtr;
	DstUniFrame.SpacePtr = ModelSpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DModelDataSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame)
{
	CString           str;
	size_t            i = 0;
	size_t            CastIndex=0;
	bool              bIsOK = true;	
	const size_t      UniFrameCount = 8;
	IMAGE_PTR         CameraImage[UniFrameCount]={NULL};
	TUNI_FRAME        RoiUniFrame[UniFrameCount];
	const BOOL        bPhaseMode = CWnd::IsDlgButtonChecked(CALIALIGN_TEST_IMAGE_PHASE_CHK);	
	LIGHT_3D_CAST_ID  CastID = LIGHT_3D_CAST_00;

	//Initial Parameters
	for ( i=0; i<UniFrameCount; i++ )
	{	
		CameraImage[i] = NULL;
		::memset(&RoiUniFrame[i], 0x00, sizeof(RoiUniFrame[i]));	
	}	

	CastIndex = 0;
	CastID = m_Light3DCastID;
	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], RoiUniFrame[CastIndex]) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
		return false;	
	}

	bIsOK = ImageAPI.CloneImage3(ImageW, ImageH, ImageStep, BitCount, CameraImage[CastIndex], m_ShowBuffer, false);
	if ( false == bIsOK )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(RoiUniFrame, UniFrameCount);
		return false;	
	}
#ifdef _DEBUG
	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-ResultPhase"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif	
	for ( i=0; i<UniFrameCount; i++ )
	{	JetMemory.free_func(CameraImage[i]); }	

	DstUniFrame = RoiUniFrame[CastIndex];	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DCastParamByCastID(CALIBRATION_MODE &CalMode, LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR &ImagePtr, TCastParam &CastParam)	
{
	const char fnName[]="CCaliPaneAlign::Exec3DCastParamByCastID";
	CString           str;
	size_t            i = 0;	
	int               nPhase = 0;
	RECT              TagRect={0};
	RECT              RoiRect={0};	
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	MASK_PTR          MaskPtr = NULL;
	SPACE_PTR         FactorPtr = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;

	MASK_PTR          FovMaskPtr = m_MaskBuffer;
	IMAGE_PTR         FovImagePtr = m_ImageBuffer1;
	PHASE_PTR         FovPhasePtr = m_PhaseBuffer;
	BOOL              bSaveDebug = FALSE;	
	const bool        bSaveRaw = GetSaveRawImage();	
	const int         SaveTimes = GetSaveRawImageTimes();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int         PhaseConvertHeightMode = SysParam.m_PhaseConvertHeightMode;
	const size_t       BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	JetMemory.free_func(ImagePtr);	
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{	return false; }

	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);
	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}	
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
	{
		str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, FactorPtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==FactorPtr))
	{
		str.Format(_T("Error, Phase Factor size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	if ( Light3DCtrl.GetLight3DHeightFactor(CastID, CastParam.HeightFactor0, CastParam.HeightFactor1, CastParam.HeightFactor2) == false )
	{
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}

	CastParam.ZeroPhasePtr = ZeroPhasePtr;
	CastParam.HeightFactorPtr = FactorPtr;	
	CastParam.HeightBuildMode = PhaseConvertHeightMode;

	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);	
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, &CastParam, SaveTimes) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, &CastParam, SaveTimes) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, &CastParam, SaveTimes) == false )
		{	return false;	}		
		break;
	}		
	//::memcpy(ImagePtr, FovImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	ImageAPI.GrayImageOffsetGain3(ImageW, ImageH, ImageStep, FovImagePtr, ImagePtr, 0, 2);
#ifdef _DEBUG
	if ( TRUE == bSaveDebug )
	{
		if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, FovPhasePtr, ImageStep, m_ShowBuffer, false) == true )
		{
			str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-OriPhase"));
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
		}	
		if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, ZeroPhasePtr, ImageStep, m_ShowBuffer, false) == true )
		{
			str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-BasePlane"));
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
		}		
	}
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Exec3DSpaceByCastID(CALIBRATION_MODE &CalMode, LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, IMAGE_PTR &ImagePtr,TUNI_FRAME &RoiUniFrame)
{
	const char fnName[]="CCaliPaneAlign::Exec3DSpaceByCastID";
	CString           str;
	size_t            i = 0;	
	int               nPhase = 0;
	RECT              TagRect={0};
	RECT              RoiRect={0};	
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	MASK_PTR          MaskPtr = NULL;
	SPACE_PTR         FactorPtr = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;

	MASK_PTR          FovMaskPtr = m_MaskBuffer;
	IMAGE_PTR         FovImagePtr = m_ImageBuffer1;
	PHASE_PTR         FovPhasePtr = m_PhaseBuffer;
	BOOL              bSaveDebug = FALSE;
	const bool        bSaveRaw = GetSaveRawImage();	
	const int         SaveTimes = GetSaveRawImageTimes();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int         PhaseConvertHeightMode = SysParam.m_PhaseConvertHeightMode;
	const size_t       BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	JetMemory.free_func(ImagePtr);	
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{	return false; }

	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);
	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}	
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
	{
		str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, FactorPtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==FactorPtr))
	{
		str.Format(_T("Error, Phase Factor size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);	
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, NULL, FovPhasePtr, FovMaskPtr, FovImagePtr, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}		
		break;
	}		
	//::memcpy(ImagePtr, FovImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	ImageAPI.GrayImageOffsetGain3(ImageW, ImageH, ImageStep, FovImagePtr, ImagePtr, 0, 2);
#ifdef _DEBUG
	if ( TRUE == bSaveDebug )
	{
		if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, FovPhasePtr, ImageStep, m_ShowBuffer, false) == true )
		{
			str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-OriPhase"));
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
		}	
		if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, ZeroPhasePtr, ImageStep, m_ShowBuffer, false) == true )
		{
			str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_3D_MODEL_TEST-BasePlane"));
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
		}		
	}
#endif	
	for ( i=0; i<BufferSize; i++ )
	{
		nPhase  = FovPhasePtr[i];
		nPhase -= ZeroPhasePtr[i];
		if ( nPhase < 0 ) 
		{	nPhase += PHASE_PERIOD; }
		else if ( nPhase > PHASE_PERIOD ) 
		{	nPhase -= PHASE_PERIOD; }	
		FovPhasePtr[i] = static_cast<PHASE_DATA>(nPhase);
	}

	if ( false == bUseRoi )
	{	JetAPI::SizeToRect(ImageW, ImageH, RoiRect); }
	else
	{
		JetAPI::Rect4DToRect(m_ImageRect4D, RoiRect); 
		RoiRect.right = RoiRect.left + JetAPI::GetBMPImagePixelsPerLine(RoiRect.right-RoiRect.left, 8, 4);
	}	
	DstW = RoiRect.right-RoiRect.left;
	DstH = RoiRect.bottom-RoiRect.top;
	DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);	
	const size_t DstBufferSize = ImageAPI.CalcBufferSize(DstStep, DstH);
	
	RoiUniFrame.ImageW = DstW;
	RoiUniFrame.ImageH = DstH;
	RoiUniFrame.ImageStep = DstStep;
	RoiUniFrame.BitCount = 8;
	//高度係數的ROI-已經宣告記憶體
	if ( ImageAPI.ExtractSpaceRoiImage3(ImageW, ImageH, ImageStep, BitCount, FactorPtr, RoiRect, DstStep, m_SpaceBuffer1, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	//相位的ROI	
	if ( ImageAPI.ExtractPhaseRoiImage(ImageW, ImageH, ImageStep, BitCount, FovPhasePtr, RoiRect, DstStep, RoiUniFrame.PhasePtr, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	//影像的ROI
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, FovImagePtr, RoiRect, DstStep, RoiUniFrame.ImagePtr, false ) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	//相位遮罩的ROI
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, FovMaskPtr, RoiRect, DstStep, RoiUniFrame.MaskPtr, false ) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	if ( PHASE_CONVERT_HEIGHT_SCALE == PhaseConvertHeightMode )
	{
		if ( ImageAPI.PhaseImageToSpaceImage(DstW, DstH, DstStep, RoiUniFrame.PhasePtr, m_SpaceBuffer1, RoiUniFrame.MaskPtr, RoiUniFrame.SpacePtr) == false )
		{	
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
	}
	else
	{
		double  HeightFactor0[HEIGHT_FACTOR_PARAM_COUNT];
		double  HeightFactor1[HEIGHT_FACTOR_PARAM_COUNT];
		double  HeightFactor2[HEIGHT_FACTOR_PARAM_COUNT];
		if ( Light3DCtrl.GetLight3DHeightFactor(CastID, HeightFactor0, HeightFactor1, HeightFactor2) == false )
		{
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}
		if ( ImageAPI.PhaseImageToSpaceImageByMapping(PhaseConvertHeightMode, DstW, DstH, DstStep, RoiUniFrame.PhasePtr, RoiRect.left, RoiRect.top, HeightFactor0, HeightFactor1, HeightFactor2, RoiUniFrame.MaskPtr, RoiUniFrame.SpacePtr) == false )
		{	
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
	}
	ImageAPI.GrayImageOffsetGain3(DstW, DstH, DstStep, RoiUniFrame.ImagePtr, RoiUniFrame.ImagePtr, 0, 2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecShow3DModelWnd(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, const TUNI_FRAME &ModelUniFrame)
{
	bool       CaliMode=false;
	IMAGE_SIZE ModelW = ModelUniFrame.ImageW;
	IMAGE_SIZE ModelH = ModelUniFrame.ImageH;
	IMAGE_SIZE ModelStep = ModelUniFrame.ImageStep;
	MASK_PTR   ModelMaskPtr = ModelUniFrame.MaskPtr;	
	IMAGE_PTR  ModelImagePtr = ModelUniFrame.ImagePtr;
	SPACE_PTR  ModelSpacePtr = ModelUniFrame.SpacePtr;
	PHASE_PTR  ModelPhasePtr = ModelUniFrame.PhasePtr;

	MASK_PTR Mask2DPtr = NULL;		
	TNoiseFilterParam FilterParam;		
	GetSpaceNoiseFilterParam(FilterParam);			
	const bool        bOpenMP = true;
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, ModelW*ModelH);	
	if ( CALIBRATION_CAMERA_ALIGN_HOR_3D_END == CalMode || CALIBRATION_CAMERA_ALIGN_VER_3D_END == CalMode )
	{	CaliMode = true;	}
	else
	{	CaliMode = false; }
	if ( true == CaliMode )
	{	FilterParam.BasePlaneParam.CalcBasePlaneMode = CALC_BASE_PLANE_DISABLE; }
	if ( ImageAPI.BuildSpaceData3(ModelW, ModelH, ModelStep, ModelSpacePtr, ModelMaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, m_SpaceBuffer1, ModelMaskPtr) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	::memcpy(ModelSpacePtr, m_SpaceBuffer1, sizeof(SPACE_DATA)*ModelH*ModelStep);	

	size_t i=0;	
	RECT rRect={0};
	this->m_ImgTargetW = ModelW;
	this->m_ImgTargetH = ModelH;
	this->m_ImgTargetStep = ModelStep;	
	const int LevelMode = 1;	
	rRect.left = 0; rRect.right = ModelW;
	rRect.top = 0; rRect.bottom = ModelH;
	if ( ImageAPI.SpaceGrayImageConvertToGray3(ModelW, ModelH, ModelStep, ModelSpacePtr, ModelMaskPtr, rRect, ModelStep, m_ShowBuffer1, -1, false) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	this->CreateImageTargetWndMemDC();

	//PHASE_TO_IMAGE_FIXED_SCALE, PHASE_TO_IMAGE_DYNAMIC_SCALE
	const BOOL   bPhaseMode = CWnd::IsDlgButtonChecked(CALIALIGN_TEST_IMAGE_PHASE_CHK);
	if ( TRUE == bPhaseMode )
	{	m_PhaseImageWnd.SetPhaseBuffer(ModelW, ModelH, ModelStep, ModelMaskPtr, ModelPhasePtr, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	}
	else
	{	m_PhaseImageWnd.SetSpaceBuffer(ModelW, ModelH, ModelStep, ModelMaskPtr, ModelSpacePtr, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	}		
	if ( m_PhaseImageWnd.IsWindowVisible() == TRUE )
	{	m_PhaseImageWnd.RedrawWnd();	}
	else
	{	m_PhaseImageWnd.ShowWindow(SW_SHOW);	}
	bool bZeroNoise=false;
	if ( true == bZeroNoise ) 
	{
		for ( i=0; i<(ModelStep*ModelH); i++ )
		{
			if ( ImageAPI.CheckSpaceMaskValid(ModelMaskPtr[i]) == false )			
			{	ModelSpacePtr[i] = 0x00; }
		}
	}
	RECT  TagRect={0};
	RECT  RoiRect={0};	
	float PadHeight = 0;
	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;	
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	if ( false == CaliMode )
	{
		if ( m_Draw3DWnd.CheckCalcObject(ModelW, ModelH) == true )	
		{	CalcObject(ModelW, ModelH, ModelStep, ModelMaskPtr, ModelSpacePtr, TagRect, PadHeight);  }
		else
		{
			::memset(&TagRect, 0x00, sizeof(TagRect));
			::memset(&RoiRect, 0x00, sizeof(RoiRect));
		}
	}
	m_Draw3DWnd.Set3DData(ModelSpacePtr, ModelImagePtr, ModelW, ModelH, ModelStep, false, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, RoiRect, PadHeight);//m_ShowBuffer1	
	m_Draw3DWnd.ShowWindow(SW_SHOW);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecVerifyZeroPlaneData(int CastID, const TUNI_FRAME &UniFrame, CString &Str)
{
	const IMAGE_SIZE ImageW=UniFrame.ImageW;
	const IMAGE_SIZE ImageH=UniFrame.ImageH;
	const SPACE_PTR  SpacePtr=UniFrame.SpacePtr;	
	const IMAGE_SIZE ImageStep=UniFrame.ImageStep;
	if ( NULL == SpacePtr )
	{	return false; }
	
	double Min=0.0, Max=0.0;
	double Std=0.0, Ave=0.0;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	CalcMeanStdMaxMin(BufferSize, SpacePtr, Ave, Std, Min, Max);	
	Str.Format(_T("Ave:%.1f, STD:%.3f, Min=%.1f, Max=%.1f, Range=%.1f"), Ave, Std, Min, Max, Max-Min);

	bool bSaveRoiHeight=true;
	if ( true == bSaveRoiHeight )
	{
		IMAGE_SIZE Index=0;
		TImageStat Statistics;
		const IMAGE_SIZE RoiW=100;
		const IMAGE_SIZE RoiH=100;
		const IMAGE_SIZE CountW=ImageW/RoiW;
		const IMAGE_SIZE CountH=ImageH/RoiH;
		const IMAGE_SIZE CountAll=CountW*CountH;
		std::vector<TImageStat> StsList(CountAll);		
		for ( IMAGE_SIZE t=0; t<CountH; t++ )
		{
			for ( IMAGE_SIZE s=0; s<CountW; s++ )
			{				
				Statistics.m_Rect.left = (s*RoiW);
				Statistics.m_Rect.top = (t*RoiH);
				Statistics.m_Rect.right = Statistics.m_Rect.left+RoiW;
				Statistics.m_Rect.bottom = Statistics.m_Rect.top+RoiH;
				ImageAPI.CalcFloatGrayImageStatistics(ImageW, ImageH, ImageStep, SpacePtr, Statistics);
				if ( Index < CountAll )
				{
					StsList[Index] = Statistics;
					Index ++;
				}
			}
		}

		CString filename;
		FILE *pfile = NULL;		
		CString strFolder=AOIDataCollect.GetAOITempDirectory();		
		filename.Format(_T("%s\\%s[%02d].CSV"), strFolder, _T("VerifyZeroPlane"), CastID);
		pfile = ::_tfopen(filename, _T("w+"));
		if ( NULL != pfile )
		{
			Index = 0;
			for ( IMAGE_SIZE t=0; t<CountH; t++ )
			{
				for ( IMAGE_SIZE s=0; s<CountW; s++ )
				{				
					const TImageStat &rStatistics=StsList[Index];
					if ( s > 0 )
					{	::_ftprintf(pfile, _T(", "));	}
					::_ftprintf(pfile, _T("%.2f"), rStatistics.m_Ave);
					Index ++;					
				}
				::_ftprintf(pfile, _T("\n"));
			}
			::fclose(pfile); pfile=NULL;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecVerifyZeroPlaneDataList(const TUNI_FRAME UniFrameList[], size_t UniFrameCount, CString &Str)
{
	size_t i=0, j=0;
	const IMAGE_SIZE ImageW=UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH=UniFrameList[0].ImageH;
	const SPACE_PTR  SpacePtr=UniFrameList[0].SpacePtr;	
	const IMAGE_SIZE ImageStep=UniFrameList[0].ImageStep;
	for ( i=0; i<UniFrameCount; i++ )
	{
		if ( NULL == UniFrameList[i].SpacePtr )
		{	return false; }
		if ( ImageW != UniFrameList[i].ImageW )
		{	return false; }
		if ( ImageH != UniFrameList[i].ImageH )
		{	return false; }
		if ( ImageStep != UniFrameList[i].ImageStep )
		{	return false; }
	}

	size_t Cnt=0;
	double Val=0.0;
	double Range=0;
	double Min=0.0, Max=0.0;
	double Std=0.0, Ave=0.0;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const double GapRatio=m_MultiZeroPlaneGapRatio;
	const double GapThreshold=m_MultiZeroPlaneGapThreshold;
	SPACE_PTR TempPtr=m_SpaceBuffer2;

	for ( i=0; i<BufferSize; i++ )
	{
		Min = Max = UniFrameList[0].SpacePtr[i];
		for ( j=1; j<UniFrameCount; j++ )
		{
			Val = UniFrameList[j].SpacePtr[i];
			if ( Min > Val ) { Min = Val; }
			if ( Max < Val ) { Max = Val; }
		}
		Range=Max-Min;
		TempPtr[i] = (SPACE_DATA)(Range);		

		if ( Range < GapThreshold ) { continue; }
		Cnt ++;
	}

	CalcMeanStdMaxMin(BufferSize, TempPtr, Ave, Std, Min, Max);
	Str.Format(_T("Ave:%.1f, STD:%.3f, Min=%.1f, Max=%.1f, Range=%.1f"), Ave, Std, Min, Max, Max-Min);	

	CString strResult;
	double Ratio=Cnt*100.0;
	Ratio /= BufferSize;
	if ( Ratio < GapRatio )
	{	strResult = _T("PASS");	}
	else
	{	strResult = _T("FAILE");	}
	Str.Format(_T("%s, %.2f %% [< %.2f %%] out of range %.2f um"), strResult, Ratio, GapRatio, GapThreshold);	

	bool bSaveImage=true;
	if ( true == bSaveImage )
	{
		CString filename;
		IMAGE_DATA Value=0;
		IMAGE_PTR  ImagePtr=m_ShowBuffer;
		for ( i=0; i<BufferSize; i++ )
		{			
			if ( TempPtr[i] > 255 ) { Value = 255; }
			else { Value = (IMAGE_DATA)(TempPtr[i]); }
			ImagePtr[i] = (IMAGE_DATA)(Value);
		}
		filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("VerifyZeroPlaneResult.PNG"));
		ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, 8, ImagePtr, true);
		m_BitCount = 8;
		m_ImageW = ImageW;
		m_ImageH = ImageH;
		m_ImageStep = ImageStep;
		DrawImageWndMemDC();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcMeanStdMaxMin(size_t Size, const SPACE_PTR Ptr, double &Mean, double &Std, double &Min, double &Max)
{
	if ( NULL == Ptr ) { return false; }

	size_t i=0;
	double Val=0.0, Sum=0.0;
	//Initial 
	Mean = Std = Min = Max = 0.0;

	//找出平均值
	Sum = 0.0;
	for ( i=0; i<Size; i++ )
	{
		Val = Ptr[i];
		if ( 0 == i )
		{	Min = Max = Val; }
		else
		{
			if ( Min > Val ) { Min = Val; }
			if ( Max < Val ) { Max = Val; }
		}
		Sum += Val; 
	}
	Mean = Sum/Size;

	//標準差
	double dSum=0.0, Dif=0.0;
	for ( i=0; i<Size; i++ )
	{	
		Dif = Ptr[i]; 
		Dif = Dif-Mean;
		Dif = Dif*Dif;
		dSum += Dif;
	}

	dSum = dSum/Size;
	Std = ::sqrt(dSum);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecVerifyZeroPlane(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{	
	const BOOL   bPhaseMode = CWnd::IsDlgButtonChecked(CALIALIGN_TEST_IMAGE_PHASE_CHK);
	if ( false == m_Multi3DCastID )
	{	return ExecVerifyZeroPlaneSingleCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr, bFinish);	}	
	else
	{	return ExecVerifyZeroPlaneMultiCastID(CalMode, CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr, bFinish); }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecVerifyZeroPlaneSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;
	size_t            i = 0;	
	bool              bIsOK = true;		
	TUNI_FRAME        UniFrame;
	IMAGE_PTR         CameraImage=NULL;	
	const bool        bUseRoi = false;
	LIGHT_3D_CAST_ID  CastID = m_Light3DCastID;

	bFinish = false;
	CameraImage = NULL;
	::memset(&UniFrame, 0x00, sizeof(UniFrame));	

	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage, UniFrame) == false )	
	{			
		JetAPI::ClearUniFrame(UniFrame);
		JetMemory.free_func(CameraImage);
		return false;	
	}

	if ( ExecVerifyZeroPlaneData(CastID, UniFrame, str) == false )
	{
		JetAPI::ClearUniFrame(UniFrame);
		JetMemory.free_func(CameraImage);
		return false;	
	}

	JetAPI::ShowMessageBox(str);

	JetAPI::ClearUniFrame(UniFrame);
	JetMemory.free_func(CameraImage);

	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecVerifyZeroPlaneMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str;	
	CString           str2, str3;	
	size_t            i = 0;		
	size_t            CastIndex = 0;	
	const size_t      UniFrameCount = 8;	
	CString           ResultStr[UniFrameCount];
	TUNI_FRAME        UniFrameList[UniFrameCount];
	IMAGE_PTR         CameraImage[UniFrameCount]={NULL};	
	LIGHT_3D_CAST_ID  CastID = LIGHT_3D_CAST_00;
	const bool        bUseRoi = false;	
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();	
	const int  DLPLightCnt = AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	const bool bReSortCameraImage = AOIDataCollect.CheckReSortCameraImage(SliceFuncMode);

	bFinish = false;
	//Initial Parameters
	for ( i=0; i<UniFrameCount; i++ )
	{	
		CameraImage[i] = NULL;
		::memset(&UniFrameList[i], 0x00, sizeof(UniFrameList[i]));	
	}	
	if ( true == bReSortCameraImage )
	{	
		if ( CameraCtrl.ReSortCameraRingBufferImage(CameraID, m_SliceParam) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
	}

	CastIndex = 0;
	CastID = LIGHT_3D_CAST_01;
	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], UniFrameList[CastIndex]) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
		return false;	
	}
	CastIndex = 1;
	CastID = LIGHT_3D_CAST_02;
	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], UniFrameList[CastIndex]) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
		return false;	
	}
	CastIndex = 2;
	CastID = LIGHT_3D_CAST_03;
	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], UniFrameList[CastIndex]) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
		return false;	
	}
	CastIndex = 3;
	CastID = LIGHT_3D_CAST_04;
	if ( Exec3DSpaceByCastID(CalMode, CastID, CameraID, ImageW, ImageH, ImageStep, BitCount, bUseRoi, CameraImage[CastIndex], UniFrameList[CastIndex]) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
		return false;		
	}

	const size_t UseCastCount=CastIndex+1;
	for ( i=0; i<UseCastCount; i++ )
	{
		if ( ExecVerifyZeroPlaneData(i+1, UniFrameList[i], ResultStr[i]) == false )
		{
			for ( i=0; i<UniFrameCount; i++ )
			{	JetMemory.free_func(CameraImage[i]); }
			JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
			return false;
		}
		str2 = ResultStr[i];
		ResultStr[i].Format(_T("Cast[%d]=%s"), i+1, str2);
		if ( 0 == i )
		{	str = ResultStr[i]; }
		else
		{	
			str2= str;
			str.Format(_T("%s\n%s"), str2, ResultStr[i]);
		}
	}
	if ( ExecVerifyZeroPlaneDataList(UniFrameList, UseCastCount, str3) == false )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{	JetMemory.free_func(CameraImage[i]); }
		JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);
		return false;
	}
	str2 = str3;
	str3.Format(_T("Pixel Diff.=%s"), str2);
	str2= str;
	str.Format(_T("%s\n%s"), str2, str3);	

	JetAPI::ShowMessageBox(str);

	for ( i=0; i<UniFrameCount; i++ )
	{	JetMemory.free_func(CameraImage[i]); }
	JetAPI::ClearUniFrameList(UniFrameList, UniFrameCount);

	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	bFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_DOT(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str, str1;
	BOOL              bSave = FALSE;
	bool              bSaveRaw = false;
	RECT              RoiRect={0};
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	TImageStat        Statistics;		
	MASK_PTR          MaskPtr = NULL;
	IMAGE_PTR         ImagePtr2 = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;
	TPhaseFactorGrid  *FactorGridPtr = NULL;
	LIGHT_3D_CAST_ID   CastID = GetLight3DCastID();	
	const unsigned int NRows = this->m_PhaseFactorRows;
	const unsigned int NCols = this->m_PhaseFactorCols;
	const bool         CalibrateAll = GetCalibrateAllCastID();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const unsigned int PhaseFactorSize = (unsigned int)(m_PhaseFactorList.size());	
	const unsigned int NextPhaseFactorIndex = m_PhaseFactorIndex+1;	
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);	
	const int TargetNo = JetAPI::GetComboxCurSelData(m_HeightFactorNumCombox);
	const double TargetHeight = this->GetDlgItemInt(CALIALIGN_PHASE_TARGET_HEIGHT_EDIT);//120um
	bFinish = false;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD_DOT"), this->m_PhaseFactorIndex);
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif	
	//::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	//this->DrawImageWndMemDC();
	//this->RedrawWnd();	

	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
	{
		str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer, m_MaskBuffer, m_ImageBuffer1, bSaveRaw) == false )
		{	return false;	}		
		break;
	}
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer, false) == false )
	{		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	this->DrawImageWndMemDC();
	this->RedrawWnd();	

	if ( m_PhaseFactorIndex < PhaseFactorSize )
	{
		FactorGridPtr = &(m_PhaseFactorList[m_PhaseFactorIndex]);	
		RoiRect = FactorGridPtr->m_RectTarget;		
		RoiRect = FactorGridPtr->m_RectLevel;
		DstW = RoiRect.right-RoiRect.left;
		DstH = RoiRect.bottom-RoiRect.top;
		DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);				
		DstW = DstStep;
		RoiRect.right = RoiRect.left+DstW;
		if ( ImageAPI.ExtractPhaseRoiImage3(ImageW, ImageH, ImageStep, BitCount, m_PhaseBuffer, RoiRect, DstStep, m_PhaseBuffer1, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false; 
		}
		if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, ImageStep, m_MaskBuffer, RoiRect, DstStep, MaskPtr, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false; 
		}		
		if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, ImageStep, m_ImageBuffer1, RoiRect, DstStep, ImagePtr2, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			JetMemory.free_func(MaskPtr);
			return false; 
		}	
		this->m_ImgTargetW = DstW;
		this->m_ImgTargetH = DstH;
		this->m_ImgTargetStep = DstStep;
		if ( ImageAPI.PhaseGrayImageConvertToGray3(DstW, DstH, DstStep, m_PhaseBuffer1, DstStep, m_ShowBuffer1, false) == false )
		{		
			JetMemory.free_func(MaskPtr);			
			JetMemory.free_func(ImagePtr2);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
	#ifdef _DEBUG
		if ( TRUE == bSave )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD-PhaseROI-1"), this->m_PhaseFactorIndex);
			ImageAPI.SaveBMPImage(str, DstW, DstH, DstStep, BitCount, m_ShowBuffer1, true);

			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD-MaskROI-1"), this->m_PhaseFactorIndex);
			ImageAPI.SaveBMPImage(str, DstW, DstH, DstStep, BitCount, MaskPtr, true);

			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD-ROI"), this->m_PhaseFactorIndex);
			ImageAPI.SaveBMPImage(str, DstW, DstH, DstStep, BitCount, ImagePtr2, true);
		}		
	#endif
		this->CreateImageTargetWndMemDC();
		if ( ExecHeightFactor(DstW, DstH, DstStep, BitCount, ImagePtr2, MaskPtr, m_PhaseBuffer1, *FactorGridPtr) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(ImagePtr2);
			return false; 
		}
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(ImagePtr2);
		/*
		const double Factor=FactorGridPtr->m_Factor;
		const double FactorMin = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT));
		const double FactorMax = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT));
		if ( Factor < FactorMin )
		{
			str1 = _T("Error, Calculae Value is out of range");
			str1 = LoadMultiLanguageString(str1, str1);
			str.Format(_T("%s (%.0f < %.0f)"), Factor, FactorMin);
			JetAPI::ShowMessageBox(str);
			return false;
		}
		if ( Factor > FactorMax )
		{
			str1 = _T("Error, Calculae Value is out of range");
			str1 = LoadMultiLanguageString(str1, str1);
			str.Format(_T("%s (%.0f > %.0f)"), Factor, FactorMax);
			JetAPI::ShowMessageBox(str);
			return false;
		}
		*/
		FactorGridPtr = FactorGridPtr;
	}

	if ( NextPhaseFactorIndex < PhaseFactorSize )
	{
		FactorGridPtr = &(m_PhaseFactorList[NextPhaseFactorIndex]);		
		if ( MotionCtrlPtr->XYZMoveTo(FactorGridPtr->m_PosX, FactorGridPtr->m_PosY, FactorGridPtr->m_PosZ) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}		
		MotionCtrlPtr->WaitForMotionStop();
		ExtraDelayTime();
		this->m_PhaseFactorIndex = NextPhaseFactorIndex;
		this->StartReGrab(FALSE);
		return true;
	}	
	if ( this->SaveHeightFactorList() == false )
	{	return false; }
	if ( this->CalcImageHeightFactor(ImageW, ImageH, ImageStep, BitCount, m_SpaceBuffer) == false )
	{	return false; }	
	
	if ( Light3DCtrl.SetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, m_SpaceBuffer) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	TPhaseFactorTable GridTable;
	GridTable.nCols = NCols; 
	GridTable.nRows = NRows;
	GridTable.nTargetNo = TargetNo;
	GridTable.dHeight = TargetHeight;	
	GridTable.GridList = m_PhaseFactorList;
	if ( Light3DCtrl.SetLight3DHeightFactorTable(CastID, TargetNo, GridTable) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( Light3DCtrl.BuildLight3DHeightFactorMappingParam(CastID) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}

	if ( MotionCtrlPtr->XYZMoveTo(this->m_StagePosX, this->m_StagePosY, this->m_StagePosZ) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}

#ifdef _DEBUG
//	str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD-4"), this->m_PhaseFactorIndex);
//	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
#endif

	SetModifiedCaliParam(true);
	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	this->m_PhaseFactorIndex = PhaseFactorSize/2;
	SetCalibrationMode(CALIBRATION_STOP);

	if ( false == CalibrateAll )
	{
		bFinish = true;
		StartReGrab(TRUE);
		return true;
	}
	if ( true == CalibrateAll )
	{	
		if ( CalibrateNext3DCastID(CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT) == true )
		{
			ClearLogListBox();
			if ( ExecPhaseHeightFactor_DOT(m_Light3DCastID) == false )
			{	return false;	}
			return true;
		}	
		UpdateStagePosition();
		if ( CalibrateNext3DCastID(CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT) == true )
		{
			ClearLogListBox();
			if ( ExecPhaseHeightFactor_DOT(m_Light3DCastID) == false )
			{	return false;	}
			return true;
		}	
		UpdateStagePosition();
		Light3DCtrl.AdjustLight3DHeightFactorMappingParam();
		str = _T("Do you want to save calibration parameters?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			if ( Light3DCtrl.SaveAllLight3DCastParameter() == false ) 
			{
				str = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str);
			}
		}	
		JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
		bFinish = true;			
		StartReGrab(TRUE);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_FOV(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	CString           str, str1;
	BOOL              bSave = FALSE;	
	RECT              RoiRect={0};
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	TImageStat        Statistics;		
	MASK_PTR          MaskPtr = NULL;
	IMAGE_PTR         ImagePtr2 = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;		
	const int         SaveTimes = 1;
	LIGHT_3D_CAST_ID  CastID = m_Light3DCastID;
	const bool        bSaveRaw = GetSaveRawImage();	
	const bool bUseRawPhase = CheckUseRawPhaseData();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);	
	bFinish = false;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD_FOV"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif	
	//::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	//this->DrawImageWndMemDC();
	//this->RedrawWnd();	

	if ( true == bUseRawPhase )
	{	ZeroPhasePtr = NULL;	}
	else
	{
		if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
		{	
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}
		if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
		{
			str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}		
		break;
	}
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer1, ImageStep, m_ShowBuffer, false) == false )
	{		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	this->DrawImageWndMemDC();
	this->RedrawWnd();		

	double PosZ_Next=0;
	const double PosX = m_StagePosX;
	const double PosY = m_StagePosY;
	const double PosZ = m_StagePosZ;
	const int    PitchZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT));
	const bool   SignZ = AOIDataCollect.GetStageSignPositiveZ();
	if ( true == SignZ )
	{	PosZ_Next = PosZ-PitchZ;	}
	else
	{	PosZ_Next = PosZ+PitchZ;	}
	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ_Next) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}		
	MotionCtrlPtr->WaitForMotionStop();
	RetrieveStagePosition(false);
	ExtraDelayTime();	
	SetCalibrationMode(CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV_Z);
	this->StartReGrab(FALSE);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_FOV_Z(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{	
	CString           str, str1;
	BOOL              bSave = FALSE;	
	RECT              RoiRect={0};
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	TImageStat        Statistics;		
	MASK_PTR          MaskPtr = NULL;
	IMAGE_PTR         ImagePtr2 = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;		
	const int         SaveTimes = 2;	
	LIGHT_3D_CAST_ID  CastID = GetLight3DCastID();
	const bool        bSaveRaw = GetSaveRawImage();	
	const bool bUseRawPhase = CheckUseRawPhaseData();
	const bool CalibrateAll = GetCalibrateAllCastID();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);	
	const TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();
	const int EnableHeightFactorCorrect = CaliParam.m_EnableHeightFactorCorrect;
	bFinish = false;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD_FOV_Z"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif	
	//::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	//this->DrawImageWndMemDC();
	//this->RedrawWnd();	

	if ( true == bUseRawPhase )
	{	ZeroPhasePtr = NULL; }
	else
	{
		if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
		{	
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}
		if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
		{
			str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}

	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer2, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer2, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer2, m_MaskBuffer, NULL, bSaveRaw, NULL, SaveTimes) == false )
		{	return false;	}		
		break;
	}
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer2, ImageStep, m_ShowBuffer, false) == false )
	{		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	this->DrawImageWndMemDC();
	this->RedrawWnd();

	//計算兩個相位差值	
	size_t       i=0;	
	int          v1=0, v2=0, val=0;
	double       d1=0, d2=0;
	double       Ave1=0, Ave2=0;
	double       Max1=0, Max2=0;
	double       Min1=0, Min2=0;
	double       factor=0.0;	
	double       SaveAve=0.0;
	int          SaveCount=0;
	float        ScaleMax=-FLT_MAX;
	float        ScaleMin= FLT_MAX;
	const float  KScale = PHASE_HEIGHT_FACTOR_SCALE;	
	const int    PitchZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT));

	//平均5x5平均	
	const int KenSize  = 5;		
	const int MinValue = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT));
	const int MaxValue = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT));
	int KenSize2 = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_FILTER_EDIT));
	if ( KenSize2 < 1 ) { KenSize2 = 1; }

	if ( FN_ENABLE != EnableHeightFactorCorrect )
	{
		ImageAPI.MedianShortGrayImage3(ImageW, ImageH, ImageStep, m_PhaseBuffer1, KenSize, m_PhaseBuffer);//剃除特殊點
		if ( 1 == KenSize2 )
		{	::memcpy(m_PhaseBuffer1, m_PhaseBuffer, sizeof(PHASE_DATA)*BufferSize);	}
		else//穩定資料	
		{	ImageAPI.SmoothShortGrayImage3(ImageW, ImageH, ImageStep, m_PhaseBuffer, KenSize2, m_PhaseBuffer1); }
	
		ImageAPI.MedianShortGrayImage3(ImageW, ImageH, ImageStep, m_PhaseBuffer2, KenSize, m_PhaseBuffer);//剃除特殊點
		if ( 1 == KenSize2 )
		{	::memcpy(m_PhaseBuffer2, m_PhaseBuffer, sizeof(PHASE_DATA)*BufferSize);	}
		else//穩定資料	
		{	ImageAPI.SmoothShortGrayImage3(ImageW, ImageH, ImageStep, m_PhaseBuffer, KenSize2, m_PhaseBuffer2); }
	}

	Ave1 = Ave2 = 0;
	Max1 = Max2 = 0;
	Min1 = Min2 = DBL_MAX;
	if ( false == bUseRawPhase )
	{
		for ( i=0; i<BufferSize; i++ )
		{
			d1 = m_PhaseBuffer1[i];
			d2 = m_PhaseBuffer2[i];
			Ave1 += d1;
			Ave2 += d2;
			if ( Max1 < d1 ) { Max1 = d1; }
			if ( Max2 < d2 ) { Max2 = d2; }
			if ( Min1 > d1 ) { Min1 = d1; }
			if ( Min2 > d2 ) { Min2 = d2; }
		}
		if ( 0 != BufferSize )
		{
			Ave1 /= BufferSize;
			Ave2 /= BufferSize;
		}
	}
	//ImageAPI.SaveSpaceGrayImage
	//str.Format(_T("Phase Average(%.2f, %.2f), Max(%.2f, %.2f), Min(%.2f, %.2f)"), Ave1, Ave2, Max1, Max2, Min1, Min2);
	//JetAPI::ShowMessageBox(str);
	//return false;	
	SaveCount = 0;
	for ( i=0; i<BufferSize; i++  )
	{		
		v1 = m_PhaseBuffer1[i];
		v2 = m_PhaseBuffer2[i];
		if ( false ==  bUseRawPhase )
		{
			if ( v2 < v1)
			{	
				str = _T("Error, Phase Value Exception");
				JetAPI::ShowMessageBox(str);
				return false;
			}		
			val = v2-v1;
		}
		else
		{
			val = v2;
			ImageAPI.DeductBasePhase_Public(m_PhaseBuffer1[i], val);
		}
		if ( 0 == val )//壞點
		{	factor = 0.0f;	}
		else
		{
			factor = PitchZ*KScale/val;
			SaveAve += factor;
			SaveCount ++;
			if ( factor > ScaleMax ) { ScaleMax = factor; }
			if ( factor < ScaleMin ) { ScaleMin = factor; }
		}
		m_PhaseBuffer[i] = static_cast<PHASE_DATA>(val);		
		m_SpaceBuffer[i] = static_cast<SPACE_DATA>(factor);		

		m_ShowBuffer[i] = 0;
		if ( factor < MinValue )	{	m_ShowBuffer[i] = 0xFF; }
		if ( factor > MaxValue )	{	m_ShowBuffer[i] = 0xFF; }
	}
	if ( SaveCount > 0 )
	{	SaveAve /= SaveCount;	}

	if ( SaveCount != BufferSize )//有壞點時
	{			
		const int nImgW=(int)(ImageW);
		const int nImgH=(int)(ImageH);
		const int nImgStep=(int)(ImageStep);
		for ( i=0; i<BufferSize; i++  )
		{	
			factor = m_SpaceBuffer[i];		
			if ( factor > 0.0001 ) { continue; }
			int nCnt=0;
			double fSum=0.0;
			const int nHalfSize=2;
			const int nX=i%nImgStep;
			const int nY=i/nImgStep;
			for ( int s=nY-nHalfSize; s<=nY+nHalfSize; s++ )
			{
				if ( s<0 || s>=nImgH ) { continue; }
				for ( int t=nX-nHalfSize; t<=nX+nHalfSize; t++ )
				{
					if ( t<0 || t>=nImgW ) { continue; }
					const int idx=(s*nImgStep)+t;
					if ( idx == i ) { continue; }
					nCnt ++;
					fSum += m_SpaceBuffer[idx];
				}
			}
			if ( 0 == nCnt )
			{	factor = SaveAve;	}
			else
			{	factor = fSum/nCnt;	}
			m_SpaceBuffer[i] = static_cast<SPACE_DATA>(factor);		
		}	
	}

	if ( FN_ENABLE == EnableHeightFactorCorrect )
	{
		const int Times = CaliParam.m_HeightFactorCorrectTimes;
		::memcpy(m_SpaceBuffer1, m_SpaceBuffer, sizeof(SPACE_DATA)*BufferSize);		
		ImageAPI.CorrectHeightFactor_Joe3(ImageW, ImageH, ImageStep, m_SpaceBuffer1, m_SpaceBuffer2, Times);
		::memcpy(m_SpaceBuffer, m_SpaceBuffer2, sizeof(SPACE_DATA)*BufferSize);		
		
		SaveAve = 0.0;
		ScaleMax=-FLT_MAX;
		ScaleMin= FLT_MAX;
		for ( i=0; i<BufferSize; i++  )
		{	
			factor = m_SpaceBuffer[i];
			SaveAve += factor;
			if ( factor > ScaleMax ) { ScaleMax = factor; }
			if ( factor < ScaleMin ) { ScaleMin = factor; }			
			m_ShowBuffer[i] = 0;
			if ( factor < MinValue )	{	m_ShowBuffer[i] = 0xFF; }
			if ( factor > MaxValue )	{	m_ShowBuffer[i] = 0xFF; }
		}
		if ( BufferSize > 0 )
		{	SaveAve /= BufferSize;	}		
	}
	//Save For Debug	
	if ( true == bSaveRaw )		
	{			
		CString Keyname;
		CString Filename;		
		const bool bDebug=true;
		const bool bReverse=true;
		CString Folder = GetSaveRawImageFolder();	
		CString ExtName = GetSaveRawImageExtName();
		CString CastName = AOIDataDefine.GetLight3DCastIDText(CastID);
		LIGHT_3D_CLS_PTR CastPtr=Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL != CastPtr )
		{	
			Keyname = _T("HeightFactorPhaseLow");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, m_PhaseBuffer1);		
			if ( true == bDebug )
			{	
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
				ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer1, ImageStep, m_ShowBuffer1, false);				
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}

			Keyname = _T("HeightFactorPhaseHigh");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, m_PhaseBuffer2);
			if ( true == bDebug )
			{		
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
				ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer2, ImageStep, m_ShowBuffer1, false);				
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}

			Keyname = _T("HeightFactorPhaseDif");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, m_PhaseBuffer);
			if ( true == bDebug )
			{	
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
				ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer, ImageStep, m_ShowBuffer1, false);				
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}

			if ( FN_ENABLE == EnableHeightFactorCorrect )
			{
				Keyname = _T("HeightFactorBefore");
				Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
				CastPtr->SaveDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, m_SpaceBuffer1);
				if ( true == bDebug )
				{					
					Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
					ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_SpaceBuffer1, m_ShowBuffer1);
					ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
				}
			}

			Keyname = _T("HeightFactorResult");
			Filename.Format(_T("%s\\%s_%s.BIN"), Folder, CastName, Keyname);
			CastPtr->SaveDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, m_SpaceBuffer);
			if ( true == bDebug )
			{		
				Filename.Format(_T("%s\\%s_%s.%s"), Folder, CastName, Keyname, ExtName);
				ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_SpaceBuffer, m_ShowBuffer1);
				ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, 8, m_ShowBuffer1, bReverse);
			}
		}
	}
	this->DrawImageWndMemDC();
	this->RedrawWnd();

	CString strAve = AOIDataDefine.GetAveText();
	CString strMax = AOIDataDefine.GetMaxText();
	CString strMin = AOIDataDefine.GetMinText();
	CString strResult = AOIDataDefine.GetResultText();
	CString strWarning = AOIDataDefine.GetWarningText();	

	if ( ScaleMin<MinValue || ScaleMax>MaxValue )
	{	str.Format(_T("Warrning!!!, some pixels is out of range (Max:%.0f, Min:%.0f, Ave:%.0f)"), ScaleMax, ScaleMin, SaveAve);	}
	else
	{	str.Format(_T("%s: %s:%.0f, %s:%.0f, %s:%.0f"), strResult, strMax, ScaleMax, strMin, ScaleMin, strAve, SaveAve);	}
	UINT Res = 0;
	if ( true == CalibrateAll )
	{	Res = IDYES;	}
	else
	{	Res = JetAPI::ShowMessageBox(str, MB_YESNO); }	
#ifdef _DEBUG
	float        Offset=0.0f;
	float        OffsetMin=FLT_MAX;
	float        OffsetMax=-FLT_MAX;
	SPACE_PTR    SpacePtr=NULL;
	IMAGE_SIZE   SpaceW=0, SpaceH=0, SpaceStep=0;
	Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, SpaceW, SpaceH, SpaceStep, SpacePtr);
	if ( NULL != SpacePtr )
	{
		for ( i=0; i<BufferSize; i++  )
		{
			Offset = SpacePtr[i]-m_SpaceBuffer[i];
			if ( Offset < OffsetMin ) { OffsetMin = Offset; }
			if ( Offset > OffsetMax ) { OffsetMax = Offset; }
		}		
	}
#endif//_DEBUG
	if ( IDYES == Res )
	{
		if ( Light3DCtrl.SetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, m_SpaceBuffer) == false )
		{	
			JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
			return false;
		}
	}

	MoveToBeforeStagePosition();	
	ExtraDelayTime();
	SetModifiedCaliParam(true);
	this->DrawImageWndMemDC();
	this->RedrawWnd();	
	SetCalibrationMode(CALIBRATION_STOP);
	if ( false == CalibrateAll )
	{	bFinish = true;	 }
	else
	{	
		if ( CalibrateNext3DCastID(CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV) == true )
		{
			if ( this->ExecGrabFirst() == false )
			{	return false;	}
			return true;
		}
		UpdateStagePosition();
		str = _T("Do you want to save calibration parameters?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			if ( Light3DCtrl.SaveAllLight3DCastParameter() == false ) 
			{
				str = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str);
			}
		}		
		bFinish = true;
	}
	this->StartReGrab(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_MultiFOV(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	int               i=0;
	CString           str, str1;
	BOOL              bSave = FALSE;
	bool              bSaveRaw = false;
	RECT              RoiRect={0};
	IMAGE_SIZE        DstW = 0;
	IMAGE_SIZE        DstH = 0;
	IMAGE_SIZE        DstStep = 0;
	TImageStat        Statistics;		
	MASK_PTR          MaskPtr = NULL;
	IMAGE_PTR         ImagePtr2 = NULL;
	PHASE_PTR         ZeroPhasePtr = NULL;		
	LIGHT_3D_CAST_ID  CastID = GetLight3DCastID();	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);	
	bFinish = false;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD_FOV"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif	
	//::memcpy(m_ShowBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	//this->DrawImageWndMemDC();
	//this->RedrawWnd();	

	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, DstW, DstH, DstStep, ZeroPhasePtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	if ( (DstW!=ImageW) || (DstH!=ImageH) || (DstStep!=ImageStep) || (NULL==ZeroPhasePtr))
	{
		str.Format(_T("Error, Zero Phase size Exception(W:%d, H:%d, Step:%d)"), DstW, DstH, DstStep);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	::memset(m_MaskBuffer, 0x00, sizeof(MASK_DATA)*BufferSize);
	switch ( m_PhaseID )
	{
	case BATCH_GRAB_PHASE_1:
	case BATCH_GRAB_PHASE_2:
		if ( this->CalcPhasePeriod1(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}
		break;
	case BATCH_GRAB_PHASE_M:
		if ( this->CalcPhasePeriod2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}
		break;	
	case BATCH_GRAB_PHASE_M2:
		if ( this->CalcPhasePeriod2Exp2(CastID, CameraID, m_PatternStep, ImageW, ImageH, ImageStep, ZeroPhasePtr, m_PhaseBuffer1, m_MaskBuffer, NULL, bSaveRaw) == false )
		{	return false;	}		
		break;
	}
	if ( ImageAPI.PhaseGrayImageConvertToGray3(ImageW, ImageH, ImageStep, m_PhaseBuffer1, ImageStep, m_ShowBuffer, false) == false )
	{		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	m_PhaseFactorID_Z ++;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s#%02d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("CALIBRATION_PATTERN_HEIGHT_FACTOR_PERIOD_FOV"), m_PhaseFactorID_Z);
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, true);
	}
#endif	
	
	const bool bTemp = true;
	str = AOIDataCollect.GetPhaseFactorFovZFilename(CastID, m_PhaseFactorID_Z, bTemp);
	JetAPI::ExtractLastPath(str, str1);
	JetAPI::CreateFolder(str1);
	if ( ImageAPI.SavePhaseBinFile(str, ImageW, ImageH, ImageStep, m_PhaseBuffer1) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}	
	this->DrawImageWndMemDC();
	this->RedrawWnd();		

	double PosZ_Next=0;	
	const double PosX = m_StagePosX;
	const double PosY = m_StagePosY;
	const double PosZ = m_StagePosZ;	
	const int    MaxFileNo = MAX_HEIGHT_TARGET_COUNT;
	const bool   SignZ = AOIDataCollect.GetStageSignPositiveZ();
	const int    PitchZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT));
	const int    RangeZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_RANGE_EDIT));	
	const int    PitchZ2=PitchZ*m_PhaseFactorID_Z;	
	if ( true == SignZ )
	{	PosZ_Next = PosZ-PitchZ2;	}
	else
	{	PosZ_Next = PosZ+PitchZ2;	}

	bool TempFinish=false;
	if ( m_PhaseFactorID_Z>MaxFileNo || PitchZ2>RangeZ )
	{ 
		TempFinish = true; 
		PosZ_Next = PosZ;
	}

	if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ_Next) == false )
	{	
		TempFinish = true; 
		PosZ_Next = PosZ;
		if ( MotionCtrlPtr->XYZMoveTo(PosX, PosY, PosZ_Next) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
	}	
	if ( false == TempFinish )
	{
		MotionCtrlPtr->WaitForMotionStop();
		RetrieveStagePosition(false);
		ExtraDelayTime();
		this->StartReGrab(FALSE);
		return true;
	}
	
	if ( ExecPhaseHeightFactor_MultiFOVFunc() == false )
	{	return false;	}			

	const bool CalibrateAll = GetCalibrateAllCastID();
	if ( false == CalibrateAll )
	{
		bFinish = true;
		SetCalibrationMode(CALIBRATION_STOP);
		StartReGrab(TRUE);
		return true;
	}
	if ( true == CalibrateAll )
	{	
		if ( CalibrateNext3DCastID(CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV) == true )
		{
			ClearLogListBox();
			if ( ExecPhaseHeightFactor_MultiFOV(m_Light3DCastID) == false )
			{	return false;	}
			return true;
		}
		UpdateStagePosition();

		int PhaseConvertHeightMode = PHASE_CONVERT_HEIGHT_MAPPING_FUNC_7;
		TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
		SysParam.m_PhaseConvertHeightMode = PhaseConvertHeightMode;	
		JetAPI::SetComboxCurSel(m_PhaseToHeightCombox, PhaseConvertHeightMode);				

		Light3DCtrl.AdjustLight3DHeightFactorMappingParam();
		str = _T("Do you want to save calibration parameters?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			if ( Light3DCtrl.SaveAllLight3DCastParameter() == false ) 
			{
				str = Light3DCtrl.GetErrorString();
				JetAPI::ShowMessageBox(str);
			}
		}			
		bFinish = true;			
		SetCalibrationMode(CALIBRATION_STOP);			
		StartReGrab(TRUE);		
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_MultiFOVFunc()
{	
	int        i=0,j=0,k=0;
	int        IdxX=0, IdxY=0;
	CString    str;
	size_t     Index=0;
	size_t     BufferSize=0;	
	int        TagetNo=0;
	IMAGE_SIZE PhaseW=0;
	IMAGE_SIZE PhaseH=0;
	IMAGE_SIZE PhaseStep=0;
	PHASE_PTR  PhasePtr = NULL;	
	double     PhaseBase=0;
	double     PhaseTarget=0;	
	double     PhaseOffset=0;	
	double     HeightBase=0;
	double     HeightTarget=0;
	double     HeightOffset=0;
	const bool bTemp = true;
	const double PosX = m_StagePosX;
	const double PosY = m_StagePosY;
	const double PosZ = m_StagePosZ;	
	const int PhaseCount = m_PhaseFactorID_Z;	
	LIGHT_3D_CAST_ID  CastID = m_Light3DCastID;
	int KenSize = 3;
	int KenSize2 = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_FILTER_EDIT));
	if ( KenSize2 < 1 ) { KenSize2 = 1; }	
	
	TPhaseFactorGrid FactorGrid;	
	TPhaseFactorTable GridTable;
	int ImagePitchX=0, ImagePitchY=0;		
	const int Cols = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NCOLS_EDIT);
	const int Rows = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NROWS_EDIT);		
	const int PitchZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT));
	const int PlaneOffset = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_PLANE_OFFSET_EDIT));
	Light3DCtrl.ClearLight3DHeightFactorTableList(CastID);	
	for ( k=0; k<PhaseCount; k++ )
	{		
		str = AOIDataCollect.GetPhaseFactorFovZFilename(CastID, k+1, bTemp);
		if ( ImageAPI.LoadPhaseBinFile(str, PhaseW, PhaseH, PhaseStep, PhasePtr) == false )
		{	continue; }
		BufferSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
		if ( 0==BufferSize || NULL==PhasePtr ) 
		{	continue; }

		ImageAPI.MedianShortGrayImage3(PhaseW, PhaseH, PhaseStep, PhasePtr, KenSize, m_PhaseBuffer);//剃除特殊點
		if ( 1 == KenSize2 )
		{	::memcpy(m_PhaseBuffer1, m_PhaseBuffer, sizeof(PHASE_DATA)*BufferSize);	}
		else//穩定資料	
		{	ImageAPI.SmoothShortGrayImage3(PhaseW, PhaseH, PhaseStep, m_PhaseBuffer, KenSize2, m_PhaseBuffer1); }
		if ( 0 == k )
		{	::memcpy(m_PhaseBuffer2, m_PhaseBuffer1, sizeof(PHASE_DATA)*BufferSize);	}
		JetMemory.free_func(PhasePtr);
		if ( k == 0 )
		{ continue; }				
		
		TagetNo = k;
		HeightBase = PlaneOffset;
		HeightTarget = PlaneOffset+(PitchZ*k);
		HeightOffset = HeightTarget-HeightBase;
		ImagePitchX = PhaseW/(Cols+1);
		ImagePitchY = PhaseH/(Rows+1);
		
		GridTable.nRows = Rows;
		GridTable.nCols = Cols;
		GridTable.GridList.clear();
		GridTable.nTargetNo = TagetNo;
		GridTable.dHeight = HeightOffset;

		FactorGrid.m_TargetNo = TagetNo;		
		for ( i=0; i<Rows; i++ )
		{
			for ( j=0; j<Cols; j++ )
			{	
				IdxX = (j+1)*ImagePitchX;
				IdxY = (i+1)*ImagePitchY;
				Index = (IdxY*PhaseStep)+IdxX;				
				PhaseBase = m_PhaseBuffer2[Index];
				PhaseTarget = m_PhaseBuffer1[Index];
				PhaseOffset = PhaseTarget-PhaseBase;

				FactorGrid.m_ImgX = IdxX;
				FactorGrid.m_ImgY = IdxY;
				FactorGrid.m_PhaseBase = PhaseBase;
				FactorGrid.m_PhaseTarget = PhaseTarget;
				FactorGrid.m_PhaseOffset = PhaseOffset;
				FactorGrid.m_HeightBase = HeightBase;
				FactorGrid.m_HeightTarget = HeightTarget;
				FactorGrid.m_HeightOffset = HeightOffset;

				FactorGrid.m_Phase = PhaseTarget;
				FactorGrid.m_Height = HeightTarget;				
				GridTable.GridList.push_back(FactorGrid);
			}
		}
		str = AOIDataCollect.GetPhaseFactorFilename(CastID, HeightOffset, bTemp);
		AOIDataCollect.SavePhaseFactorTableFile(str, GridTable);
		Light3DCtrl.SetLight3DHeightFactorTable(CastID, TagetNo, GridTable);
	}	
	if ( Light3DCtrl.BuildLight3DHeightFactorMappingParam(CastID) == false )
	{
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CheckHeightFactor(const TPhaseFactorGrid &Grid)//計算單格的局部高度比例平均
{
	const int Index_X = (int)(Grid.m_IdxX);
	const int Index_Y = (int)(Grid.m_IdxY);
	const int IndexMin_X=MAX(0, Index_X-1);
	const int IndexMin_Y=MAX(0, Index_Y-1);
	const int IndexMax_X=MIN(m_PhaseFactorCols, Index_X+1);
	const int IndexMax_Y=MIN(m_PhaseFactorRows, Index_Y+1);
	const size_t GridCount=m_PhaseFactorList.size();
	for ( size_t i=0; i<GridCount; i++ )
	{
		TPhaseFactorGrid *FactorGridPtr = &(m_PhaseFactorList[i]);	
		if ( NULL == FactorGridPtr ) { continue; }
		if ( FactorGridPtr->m_IdxX < IndexMin_X ) { continue; }
		if ( FactorGridPtr->m_IdxY < IndexMin_Y ) { continue; }
		if ( FactorGridPtr->m_IdxX > IndexMax_X ) { continue; }
		if ( FactorGridPtr->m_IdxY > IndexMax_Y ) { continue; }
		if ( ::fabs(FactorGridPtr->m_Factor-1.0)<0.1 ) { continue; }

		double Sum=0.0;
		int    Count=0;
		const int LocIdx_X = (int)(FactorGridPtr->m_IdxX);
		const int LocIdx_Y = (int)(FactorGridPtr->m_IdxY);
		const int LocIdxMin_X=MAX(0, LocIdx_X-1);
		const int LocIdxMin_Y=MAX(0, LocIdx_Y-1);
		const int LocIdxMax_X=MIN(m_PhaseFactorCols, LocIdx_X+1);
		const int LocIdxMax_Y=MIN(m_PhaseFactorRows, LocIdx_Y+1);
		FactorGridPtr->m_FactorAve = FactorGridPtr->m_Factor;		
		for ( size_t j=0; j<GridCount; j++ )
		{
			TPhaseFactorGrid *LocalGridPtr = &(m_PhaseFactorList[j]);	
			if ( NULL == LocalGridPtr ) { continue; }
			//if ( LocalGridPtr == FactorGridPtr ) { continue; }//跳過本身
			if ( LocalGridPtr->m_IdxX < LocIdxMin_X ) { continue; }
			if ( LocalGridPtr->m_IdxY < LocIdxMin_Y ) { continue; }
			if ( LocalGridPtr->m_IdxX > LocIdxMax_X ) { continue; }
			if ( LocalGridPtr->m_IdxY > LocIdxMax_Y ) { continue; }
			if ( ::fabs(LocalGridPtr->m_Factor-1.0)<0.1 ) { continue; }
			Sum += LocalGridPtr->m_Factor;
			Count ++;
		}
		if ( Count > 0 )
		{	FactorGridPtr->m_FactorAve = Sum/Count;	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecHeightFactor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, const PHASE_PTR Ptr, TPhaseFactorGrid &Grid)//計算單格的高度比例
{
	const char fnName[] = "CCaliPaneAlign::ExecHeightFactor";
	if ( 0==ImageW || 0==ImageH || 0==ImageStep || NULL==MaskPtr || NULL==Ptr ) { return false; }	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	
	const int ExtendL = Grid.m_RectTarget.left - Grid.m_RectLevel.left;
	const int ExtendR = Grid.m_RectLevel.right - Grid.m_RectTarget.right;
	const int ExtendT = Grid.m_RectTarget.top - Grid.m_RectLevel.top;
	const int ExtendB = Grid.m_RectLevel.bottom - Grid.m_RectTarget.bottom;
	if ( (ExtendL<0) || (ExtendR<0) || (ExtendT<0) || (ExtendB<0) ) { return false; }	

	CString str;
	size_t idx=0;
	size_t i=0, j=0;
	size_t TargetCount=0;	
	RECT rcTarget={0};
	RECT rcTarget2={0};
	RECT rcLevel2={0};
	TImageStat TargetState;
	rcTarget.left = ExtendL;
	rcTarget.top = ExtendT;
	rcTarget.right = ImageW-ExtendR;
	rcTarget.bottom = ImageH-ExtendB;
	int TargetW = rcTarget.right-rcTarget.left;
	int TargetH = rcTarget.bottom-rcTarget.top;
	if ( (TargetW<0) || (TargetH<0) ) { return false; }

	double RoiAve=0.0;
	int    RoiCount=0;
	double LevelAve=0.0;	
	int    LevelCnt=0;
	double TargetAve=0.0;
	int    TargetCnt=0;

	//為了避免陰影雜訊造成基準面高度失真, 雜訊外擴2個像素
	MASK_PTR NoiseMaskPtr=NULL;	
	if ( JetMemory.alloc_func(BufferSize, NoiseMaskPtr, fnName, "NoiseMaskPtr") == true )
	{
		CString str2;
		::memset(NoiseMaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
		for ( i=0; i<BufferSize; i++ )
		{
			idx = i;
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == true ) 
			{	continue; }
			NoiseMaskPtr[idx] = 0xFF;
		}
		IMAGE_PTR MaskTempPtr=NULL;
		if ( ImageAPI.DilateGrayImage(ImageW, ImageH, ImageStep, NoiseMaskPtr, 5, 1, MaskTempPtr)  == true )//外擴雜訊範圍
		{
			for ( i=0; i<BufferSize; i++ )
			{
				idx = i;	
				if ( 0 == MaskTempPtr[idx] ) { continue; }
				if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == true ) 
				{	MaskPtr[idx] = PHASE_MASK_EXTEND_VOID;	}
			}
			JetMemory.free_func(MaskTempPtr);		
		}	
		JetMemory.free_func(NoiseMaskPtr);		
	}

	//要改成以下程序, 否則會因為鏡頭組裝偏差造成計算誤差
	//1.先找出有效點的平均高度, 做為基準面與階高的閥值, 並做2值化
	for ( i=0; i<ImageH; i++ )
	{	
		for ( j=0; j<ImageW; j++ )
		{
			idx = (i*ImageStep)+j;
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
			{	continue; }
			if ( i<rcTarget.top || i>rcTarget.bottom || j<rcTarget.left || j>rcTarget.right)
			{
				LevelAve += Ptr[idx];
				LevelCnt ++;
			}
			else
			{
				TargetAve += Ptr[idx];
				TargetCnt ++;
			}
			RoiAve += Ptr[idx];
			RoiCount ++;
		}
	}
	if ( RoiCount > 0 ) 
	{	RoiAve /= RoiCount;	}
	if ( LevelCnt > 0 ) 
	{	LevelAve /= LevelCnt;	}
	if ( TargetCnt > 0 ) 
	{	TargetAve /= TargetCnt;	}

	//const double HeightThreshold = RoiAve*1.2;//Small Target
	//const double HeightThreshold = RoiAve*0.75;//Big Target
	//const double HeightThreshold = RoiAve*(1.0-Weighting2);
	const double HeightThreshold = (TargetAve*0.6)+(LevelAve*0.4);

	IMAGE_PTR  BinaryPtr=NULL;
	if ( JetMemory.alloc_func(BufferSize, BinaryPtr, fnName, ("BinaryPtr")) == false )
	{	return false;	}
	::memset(BinaryPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);	
	for ( i=0; i<ImageH; i++ )
	{	
		for ( j=0; j<ImageW; j++ )
		{
			idx = (i*ImageStep)+j;			
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
			{	continue; }
			RoiAve = Ptr[idx];
			if ( RoiAve < HeightThreshold ) { continue; }			
			BinaryPtr[idx] = 0xFF;			
		}		
	}	
#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("HeightBinary.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, BinaryPtr, true);
#endif//_DEBUG

	//2.找出最大的區塊, 將區塊內部侵蝕最為階高範圍, 將缺塊外部膨脹做為基準面範圍	
	RECT     TmpRect={0};
	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
	JetAPI::SizeToRect(ImageW, ImageH, TmpRect);
	if ( BlobDetector.GrayImageRoiBlobDetect(ImageW, ImageH, ImageStep, BinaryPtr, TmpRect, 128, 255) == false )
	{	
		JetMemory.free_func(BinaryPtr);				
		return false;
	}
	JetMemory.free_func(BinaryPtr);		
	TBlobResult *pBlobResult=BlobDetector.GetBlobPtr(0, true);			
	if ( NULL == pBlobResult ) { return true; }
	rcTarget = pBlobResult->m_BlobRect;
	TargetW = rcTarget.right-rcTarget.left;
	TargetH = rcTarget.bottom-rcTarget.top;
	if ( (TargetW<0) || (TargetH<0) ) { return false; }
	
	//3.計算出階高與基面的高度差

	//內縮20%, 塊規只使用80%的範圍
	const double Ratio = 0.1;
	const int ExtTargetW = (int)(TargetW*Ratio);
	const int ExtTargetH = (int)(TargetH*Ratio);
	rcTarget2.left  = rcTarget.left  + ExtTargetW;
	rcTarget2.right = rcTarget.right - ExtTargetW;
	rcTarget2.top   = rcTarget.top  + ExtTargetH;
	rcTarget2.bottom= rcTarget.bottom - ExtTargetH;

	TargetCount=0;
	TargetState.m_Ave = 0;	
	TargetState.m_Rect = rcTarget2;
	for ( i=TargetState.m_Rect.top; i<TargetState.m_Rect.bottom; i++ )
	{	
		for ( j=TargetState.m_Rect.left; j<TargetState.m_Rect.right; j++ )
		{
			idx = (i*ImageStep)+j;			
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
			{	continue; }
			RoiAve = Ptr[idx];//剔除內部較低的雜訊
			if ( RoiAve < HeightThreshold ) { continue; }		
			TargetState.m_Ave += Ptr[idx];
			TargetCount ++;
		}
	}
	if ( TargetCount > 0 ) 
	{	TargetState.m_Ave /= TargetCount;	}

//	if ( ImageAPI.CalcShortGrayImageStatistics(ImageW, ImageH, ImageStep, Ptr, TargetState) == false )
//	{
//		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
//		return false;
//	}

	//外擴20%, 基準面只使用80%的範圍	
	double LevelSum   = 0;
	size_t LevelCount = 0;
	rcLevel2.left  = rcTarget.left  - (int)(ExtendL*Ratio);
	rcLevel2.right = rcTarget.right + (int)(ExtendR*Ratio);
	rcLevel2.top   = rcTarget.top   - (int)(ExtendT*Ratio);
	rcLevel2.bottom= rcTarget.bottom + (int)(ExtendB*Ratio);

	LevelSum   = 0;
	LevelCount = 0;
	for ( i=0; i<ImageH; i++ )
	{
		if ( (i>rcLevel2.top) && (i<rcLevel2.bottom) ) 
		{	continue; }

		for ( j=0; j<ImageW; j++ )
		{
			if ( (j>rcLevel2.left) && (i<rcLevel2.right) ) 
			{	continue; }

			idx = (i*ImageStep)+j;			
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
			{	continue; }
			RoiAve = Ptr[idx];//剔除周圍較高的雜訊
			if ( RoiAve > HeightThreshold ) { continue; }		
			LevelSum += Ptr[idx];
			LevelCount ++;
		}
	}
	double LevelAve2 = 0;
	if ( LevelCount > 0 ) 
	{	LevelAve2 = LevelSum/LevelCount;	}

	const float KScale = PHASE_HEIGHT_FACTOR_SCALE;
	const double LevelHeight = GetDlgItemInt(CALIALIGN_PHASE_PLANE_OFFSET_EDIT);//基準面高度值-自動對焦後是否會相同高度呢?
	const double TargetHeight = GetDlgItemInt(CALIALIGN_PHASE_TARGET_HEIGHT_EDIT);//塊規階高
	const double LevelPhase = LevelAve2;
	const double TargetPhase = TargetState.m_Ave;
	const double dGray = TargetPhase-LevelPhase;
	const double KValue = TargetHeight/dGray;
	const double LevelHeight2 = LevelPhase*KValue;//計算出目前基準面相對相平面高度

	Grid.m_Phase = dGray;
	Grid.m_Height = TargetHeight;
	Grid.m_PhaseOffset = dGray;//階高塊相對基準面的相位差
	Grid.m_HeightOffset = TargetHeight;//階高塊相對基準面的高度差
	Grid.m_PhaseBase = LevelPhase;//基準面相對相平面的相位值
	Grid.m_PhaseTarget = TargetPhase;//階高塊相對相平面的相位值
	Grid.m_HeightBase = LevelHeight;//基準面相對相平面的高度值	
	Grid.m_HeightTarget = LevelHeight+TargetHeight;//階高塊相對相平面的高度值

	Grid.m_PhaseBaseCalc = Grid.m_PhaseBase;
	Grid.m_PhaseTargetCalc = Grid.m_PhaseTarget;
	Grid.m_HeightBaseCalc = Grid.m_HeightBase;
	Grid.m_HeightTargetCalc = Grid.m_HeightTarget;

	Grid.m_Factor = KValue;//數字多為0.0xxx, 所以乘上一個係數比
	Grid.m_Factor *= KScale;	
	if ( Grid.m_Factor < 0 ) 
	{	Grid.m_Factor = Grid.m_Factor;	}	
	CheckHeightFactor(Grid);

	const bool bOpenMP = true;
	const double SpaceRatio = -1;//AOIDataCollect.GetSpaceToGrayRatio();
	const SPACE_DATA HeightFactor = (SPACE_DATA)(Grid.m_Factor/KScale);	
	SPACE_PTR SpacePtr = this->m_SpaceBuffer;	
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, ImageW*ImageH);
	if ( NULL != SpacePtr )
	{	
		RECT TagRect={0};
		RECT RoiRect={0};	
		TNoiseFilterParam FilterParam;
		MASK_PTR  MaskDstPtr = NULL;
		MASK_PTR  Mask2DPtr = NULL;
		IMAGE_PTR ImageMaskPtr = NULL;
		SPACE_PTR SpaceDstPtr = NULL;		
		
		TagRect = rcTarget;
		TagRect.top = ImageH-rcTarget.bottom;
		TagRect.bottom = ImageH-rcTarget.top;
		JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
		for ( i=0; i<BufferSize; i++ )
		{	
			SpacePtr[i] = Ptr[i];
			SpacePtr[i] = SpacePtr[i]*HeightFactor;			
		}
		AOIDataCollect.GetSpaceNoiseFilterParam(FilterParam);
		//FilterParam.DataHeightFTMode = DATA_NF_DISABLE;		
		FilterParam.DataFirstFilterMode = DATA_NF_DISABLE;
		FilterParam.DataVoidExpandEnabled = false;
		FilterParam.DataOverLowFTMode = DATA_NF_DISABLE;
		FilterParam.DataHeightFTMode = DATA_NF_DISABLE;//為了加速顯示
		FilterParam.DataVoidReContructed = false;		
		//FilterParam.DataFinalFilterMode = NOISE_FILTER_DISABLE;
		//FilterParam.DataFinalFilterMode2 = NOISE_FILTER_DISABLE;
		IMAGE_PTR GuidedImagePtr = NULL;
		ImageAPI.GetGuidedImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, GuidedImagePtr);//20191126, Joe
		if ( ImageAPI.BuildSpaceData(ImageW, ImageH, ImageStep, SpacePtr, MaskPtr, Mask2DPtr, nOpenMPCnt, FilterParam, SpaceDstPtr, MaskDstPtr, GuidedImagePtr) == true )
		{	
			//根據實際切平面來計算高度						
			double TargetAve=0;			
			double CalScale = 1.0;
			double NewFactor = Grid.m_Factor;
			TargetCount = 0;
			for ( i=TargetState.m_Rect.top; i<TargetState.m_Rect.bottom; i++ )
			{	
				for ( j=TargetState.m_Rect.left; j<TargetState.m_Rect.right; j++ )
				{
					idx = (i*ImageStep)+j;
					if ( ImageAPI.CheckSpaceMaskValid(MaskDstPtr[idx]) == false ) 
					{	continue; }

					TargetAve += SpaceDstPtr[idx];
					TargetCount ++;
				}
			}
			if ( TargetCount > 0 ) 
			{	
				TargetAve /= TargetCount;	
				CalScale   = TargetHeight/TargetAve;				
				if ( CalScale<0.9 || CalScale>1.1 )
				{	NewFactor = Grid.m_Factor;	}
				else
				{
					NewFactor = Grid.m_Factor*CalScale;
					Grid.m_Factor = NewFactor;
					for ( i=0; i<ImageH; i++ )
					{	
						for ( j=0; j<ImageW; j++ )
						{	
							idx = (i*ImageStep)+j;
							if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
							{	continue; }
							SpaceDstPtr[idx] *= CalScale;						
						}
					}
				}
			}

			for ( i=0; i<BufferSize; i++ )
			{	
				if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[i]) == false )					
				{	SpaceDstPtr[i] = 0; }
			}
			float ShowMinH = -1;
			float ShowMaxH = -1;
			float RuleMinH = -1;
			float RuleMaxH = -1;
			IMAGE_SIZE ImageMaskBitCount = 24;
			IMAGE_SIZE ImageMaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ImageMaskBitCount, 4);
			if ( ImageAPI.SpaceGrayImageConvertToColor(ImageW, ImageH, ImageStep, SpaceDstPtr, MaskDstPtr, ImageMaskStep, ImageMaskPtr, SpaceRatio, false) == true )
			{	m_Draw3DWnd.Set3DData(SpaceDstPtr, ImageMaskPtr, ImageW, ImageH, ImageStep, true, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, RoiRect, TargetHeight);	}
			else
			{	m_Draw3DWnd.Set3DData(SpaceDstPtr, ImagePtr, ImageW, ImageH, ImageStep, false, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, RoiRect, TargetHeight); }						
			m_Draw3DWnd.SetBoundPadOpen(true);
			m_Draw3DWnd.ShowWindow(SW_SHOW);
			JetMemory.free_func(MaskDstPtr);	MaskDstPtr = NULL;
			JetMemory.free_func(SpaceDstPtr);	SpaceDstPtr = NULL;
			JetMemory.free_func(ImageMaskPtr);	ImageMaskPtr = NULL;
		}		
		JetMemory.free_func(GuidedImagePtr); GuidedImagePtr = NULL;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::SaveHeightFactorList()
{
	FILE *pfile = NULL;
	CString filename;
	TCHAR   TMode[32] = _T("");
	LIGHT_3D_CAST_ID CastID = m_Light3DCastID;
	const unsigned int NRows = this->m_PhaseFactorRows;
	const unsigned int NCols = this->m_PhaseFactorCols;
	const double TargetHeight = this->GetDlgItemInt(CALIALIGN_PHASE_TARGET_HEIGHT_EDIT);//120um
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("HeightFactor.TXT"));
	pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile ) 
	{	return false; }

	size_t i = 0;
	TPhaseFactorGrid  *FactorGridPtr = NULL;	
	const size_t PhaseFactorListSize = this->m_PhaseFactorList.size();

	::_ftprintf(pfile, _T("idx, x, y, factor\n"));
	for ( i=0; i<PhaseFactorListSize; i++ )
	{
		FactorGridPtr = &(m_PhaseFactorList[i]);
		if ( NULL == FactorGridPtr ) { continue; }
		::_ftprintf(pfile, _T("%d, %.2f, %.2f, %.4f\n"), i+1, FactorGridPtr->m_ImgX, FactorGridPtr->m_ImgY, FactorGridPtr->m_Factor);
	}

	::fclose(pfile);
	pfile = NULL;
	
	CString Folder;	
	const bool bTemp = true;	
	Folder = AOIDataCollect.GetPhaseFactorFolder(bTemp);
	JetAPI::CreateFolder(Folder);

	TPhaseFactorTable GridTable;
	GridTable.nCols = NCols;
	GridTable.nRows = NRows;
	GridTable.dHeight = TargetHeight;
	GridTable.nTargetNo = JetAPI::GetComboxCurSelData(m_HeightFactorNumCombox);
	GridTable.GridList = m_PhaseFactorList;
	filename = AOIDataCollect.GetPhaseFactorFilename(CastID, TargetHeight, bTemp);		
	AOIDataCollect.SavePhaseFactorTableFile(filename, GridTable);//儲存相位高度比例參數檔案
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcImageHeightFactor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, SPACE_PTR Ptr)//計算整張的圖片參數
{
	if ( 0==ImageW || 0==ImageH || 0==ImageStep || NULL==Ptr ) { return false; }
	
	unsigned int u=0, v=0, s=0;
	unsigned int i=0, j=0, k=0;
	IMAGE_SIZE   L=0, T=0, R=0, B=0;
	TPhaseFactorGrid  *Grid11 = NULL;
	TPhaseFactorGrid  *Grid21 = NULL;
	TPhaseFactorGrid  *Grid12 = NULL;
	TPhaseFactorGrid  *Grid22 = NULL;	
	const unsigned int NRows = this->m_PhaseFactorRows;
	const unsigned int NCols = this->m_PhaseFactorCols;
	const unsigned int PhaseFactorListSize = (int)(this->m_PhaseFactorList.size());

	double x=0, y=0;
	double x1=0, x2=0;
	double y1=0, y2=0;
	double dx2x1=0;
	double dy2y1=0;
	double dx2x1dy2y1=0;
	double p11=0, p21=0, p12=0, p22=0, p=0;	

	for ( i=0; i<NRows-1; i++ )
	{
		for ( j=0; j<NCols-1; j++ )
		{
			k = i*NCols+j;
			Grid11 = &(m_PhaseFactorList[k]);

			k = i*NCols+j+1;
			Grid21 = &(m_PhaseFactorList[k]);

			k = (i+1)*NCols+j;
			Grid12 = &(m_PhaseFactorList[k]);

			k = (i+1)*NCols+j+1;
			Grid22 = &(m_PhaseFactorList[k]);

			x1 = Grid11->m_ImgX;
			y1 = Grid11->m_ImgY;
			x2 = Grid22->m_ImgX;
			y2 = Grid22->m_ImgY;
			dx2x1 = x2-x1;
			dy2y1 = y2-y1;
			dx2x1dy2y1 = dx2x1*dy2y1;

			if ( i ==0 )
			{
				if ( j==0 ) 
				{	//Left & Top
					L = 0;
					T = 0;
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
				}
				else if ( j == (NCols-2) )
				{
					//Right & Top
					T = 0;
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
					R = ImageW;
				}
				else
				{
					// Top
					T = 0;
					L = (IMAGE_SIZE)(Grid11->m_ImgX);					
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
				}
				
			}
			else if ( i == (NRows-2) )
			{
				if ( j==0 ) 
				{
					//Left Bottom
					L = 0;
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
				}
				else if ( j == (NCols-2) )
				{
					//Right Bottom
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
					R = ImageW;
				}
				else
				{
					//Bottom
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
				}
			}
			else if ( j == 0 )
			{
				//Left
				L = 0;
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				R = (IMAGE_SIZE)(Grid22->m_ImgX);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
			}
			else if ( j == (NCols-2) )
			{
				//Right				
				L = (IMAGE_SIZE)(Grid11->m_ImgX);
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
				R = ImageW;
			}
			else
			{
				L = (IMAGE_SIZE)(Grid11->m_ImgX);
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				R = (IMAGE_SIZE)(Grid22->m_ImgX);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
			}
			for ( u=T; u<B; u++ )
			{
				for ( v=L; v<R; v++ )
				{
					x = v;
					y = u;

					p11 = (x2-x)*(y2-y)*Grid11->m_Factor;
					p21 = (x-x1)*(y2-y)*Grid21->m_Factor;
					p12 = (x2-x)*(y-y1)*Grid12->m_Factor;
					p22 = (x-x1)*(y-y1)*Grid22->m_Factor;
					p = (p11+p21+p12+p22)/dx2x1dy2y1;
					//if ( p >= PHASE_MAX ) 
					//{	p = PHASE_MAX-1; }
					//else if ( p < 0 ) 
					//{	p = 0; }
					Ptr[u*ImageStep+v] = static_cast<SPACE_DATA>(p);
					k = k;
				}
			}

			k = k;
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::TestImageHeightFactor2()
{
	//Test K-Factor Function
	int    idx=0;
	float *PhasePtr1 = NULL;
	float *PhasePtr2 = NULL;
	float *FactorPtr1 = NULL;
	float *FactorPtr2 = NULL;
	float  Target1 = 5000.0f;
	float  Target2 =10000.0f;	
	const unsigned int nW = 4096;
	const unsigned int nH = 3072;
	const unsigned int nS = nW;
	const unsigned int nB = 8;
	const unsigned int nSize=nS*nH;
	TPhaseFactorGrid Grid;
	std::vector<TPhaseFactorGrid> GridList1;
	std::vector<TPhaseFactorGrid> GridList2;	
	const char fnName[] = "CCaliPaneAlign::CCaliPaneAlign";
	if ( JetMemory.alloc_func(nSize, PhasePtr1, fnName, "PhasePtr1") == false || 
		 JetMemory.alloc_func(nSize, PhasePtr2, fnName, "PhasePtr2") == false ||
		 JetMemory.alloc_func(nSize, FactorPtr1, fnName, "FactorPtr1") == false ||
		 JetMemory.alloc_func(nSize, FactorPtr2, fnName, "FactorPtr2") == false 
		)
	{
		JetMemory.free_func(PhasePtr1);
		JetMemory.free_func(PhasePtr2);
		JetMemory.free_func(FactorPtr1);
		JetMemory.free_func(FactorPtr2);
		return false;
	}

	GridList1.clear();//Target1 - 5000
	Grid.m_Height = 5000;
	Grid.m_IdxX = 0;	Grid.m_IdxY = 0;	Grid.m_ImgX =  611.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 8862.47;	GridList1.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 0;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 7931.79;	GridList1.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 0;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 6752.77;	GridList1.push_back(Grid);
	Grid.m_IdxX = 0;	Grid.m_IdxY = 1;	Grid.m_ImgX =  611.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 8852.22;	GridList1.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 1;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 7917.72;	GridList1.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 1;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 6730.34;	GridList1.push_back(Grid);
	Grid.m_IdxX = 0;	Grid.m_IdxY = 2;	Grid.m_ImgX =  611.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 8873.72;	GridList1.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 2;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 7931.68;	GridList1.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 2;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 6742.08;	GridList1.push_back(Grid);

	GridList2.clear();//Target2 - 1000
	Grid.m_Height = 10000;
	Grid.m_IdxX = 0;	Grid.m_IdxY = 0;	Grid.m_ImgX =  611.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 18180.73;	GridList2.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 0;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 16344.25;	GridList2.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 0;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY =  602.5;	Grid.m_Phase = 13937.73;	GridList2.push_back(Grid);
	Grid.m_IdxX = 0;	Grid.m_IdxY = 1;	Grid.m_ImgX =  611.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 18164.89;	GridList2.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 1;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 16300.02;	GridList2.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 1;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY = 1535.5;	Grid.m_Phase = 13880.05;	GridList2.push_back(Grid);
	Grid.m_IdxX = 0;	Grid.m_IdxY = 2;	Grid.m_ImgX =  611.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 18239.50;	GridList2.push_back(Grid);
	Grid.m_IdxX = 1;	Grid.m_IdxY = 2;	Grid.m_ImgX = 2047.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 16427.85;	GridList2.push_back(Grid);
	Grid.m_IdxX = 2;	Grid.m_IdxY = 2;	Grid.m_ImgX = 3483.5;	Grid.m_ImgY = 2468.5;	Grid.m_Phase = 14017.36;	GridList2.push_back(Grid);
	

	idx=0;	
	//PhasePtr1[idx++] = 8862.47;	PhasePtr1[idx++] = 7931.79;	PhasePtr1[idx++] = 6752.77;
	//PhasePtr1[idx++] = 8852.22;	PhasePtr1[idx++] = 7917.72;	PhasePtr1[idx++] = 6730.34;
	//PhasePtr1[idx++] = 8873.72;	PhasePtr1[idx++] = 7931.68;	PhasePtr1[idx++] = 6742.08;	
	idx=0;
	//PhasePtr2[idx++] = 18180.73;	PhasePtr2[idx++] = 16344.25;	PhasePtr2[idx++] = 13937.73;
	//PhasePtr2[idx++] = 18164.89;	PhasePtr2[idx++] = 16300.02;	PhasePtr2[idx++] = 13880.05;
	//PhasePtr2[idx++] = 18239.50;	PhasePtr2[idx++] = 16427.85;	PhasePtr2[idx++] = 14017.36;
	::memset(FactorPtr1, 0x00, sizeof(float)*nSize);
	::memset(FactorPtr2, 0x00, sizeof(float)*nSize);
	MapGridPhaseToImage(GridList1, nW, nH, nS, nB, PhasePtr1);
	MapGridPhaseToImage(GridList2, nW, nH, nS, nB, PhasePtr2);
	//CalcImageHeightFactor2(nW, nH, nS, nB, PhasePtr1, NULL, Target1, Target2, FactorPtr1, NULL);	
	//CalcImageHeightFactor2(nW, nH, nS, nB, NULL, PhasePtr2, Target1, Target2, NULL, FactorPtr2);	
	//CalcImageHeightFactor2(nW, nH, nS, nB, PhasePtr1, PhasePtr2, Target1, Target2, FactorPtr1, FactorPtr2);	
	JetMemory.free_func(PhasePtr1);
	JetMemory.free_func(PhasePtr2);
	JetMemory.free_func(FactorPtr1);
	JetMemory.free_func(FactorPtr2);
	
	CString filename;	
	double Target1d=0;
	double Target2d=0;
	int NCols2 = 0;
	int NRows2 = 0;
	const unsigned int NCols = 3;
	const unsigned int NRows = 3;
	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_01;
	std::vector<TPhaseFactorGrid> GridList1R;
	std::vector<TPhaseFactorGrid> GridList2R;

	CString Folder;
	const bool bTemp = true;
	Folder = AOIDataCollect.GetPhaseFactorFolder(bTemp);	
	JetAPI::CreateFolder(Folder);

	TPhaseFactorTable GridTableIn;
	TPhaseFactorTable GridTableOut;
	GridTableOut.nCols = NCols;
	GridTableOut.nRows = NRows;
	GridTableOut.dHeight = Target1;
	GridTableOut.nTargetNo = 1;
	GridTableOut.GridList = GridList1;
	filename = AOIDataCollect.GetPhaseFactorFilename(CastID, Target1, bTemp);
	AOIDataCollect.SavePhaseFactorTableFile(filename, GridTableOut);//儲存相位高度比例參數檔案
	AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTableIn);//載入相位高度比例參數檔案

	GridTableOut.nCols = NCols;
	GridTableOut.nRows = NRows;
	GridTableOut.dHeight = Target2;
	GridTableOut.nTargetNo = 2;
	GridTableOut.GridList = GridList2;
	filename = AOIDataCollect.GetPhaseFactorFilename(CastID, Target2, bTemp);	
	AOIDataCollect.SavePhaseFactorTableFile(filename, GridTableOut);//儲存相位高度比例參數檔案
	AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTableIn);//載入相位高度比例參數檔案	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::MapGridPhaseToImage(const std::vector<TPhaseFactorGrid> &GridList, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, SPACE_PTR Ptr)//將Grid內插出影像數值(相位)	
{
	if ( 0==ImageW || 0==ImageH || 0==ImageStep || NULL==Ptr ) { return false; }	
	
	unsigned int u=0, v=0, s=0;
	unsigned int i=0, j=0, k=0;
	unsigned int IdxX=0, IdxY=0;
	IMAGE_SIZE   L=0, T=0, R=0, B=0;	
	TPhaseFactorGrid  *Grid11 = NULL;
	TPhaseFactorGrid  *Grid21 = NULL;
	TPhaseFactorGrid  *Grid12 = NULL;
	TPhaseFactorGrid  *Grid22 = NULL;	
	TPhaseFactorGrid  *GridPtr = NULL;	
	const unsigned int PhaseFactorListSize = (unsigned int)(GridList.size());

	IdxX = IdxY = 0;
	for ( i=0; i<PhaseFactorListSize; i++ )
	{
		GridPtr = (TPhaseFactorGrid*)(&(GridList[i]));
		if ( IdxX < GridPtr->m_IdxX ) { IdxX = GridPtr->m_IdxX; }
		if ( IdxY < GridPtr->m_IdxY ) { IdxY = GridPtr->m_IdxY; }
	}
			

	double x=0, y=0;
	double x1=0, x2=0;
	double y1=0, y2=0;
	double dx2x1=0;
	double dy2y1=0;
	double dx2x1dy2y1=0;
	double p11=0, p21=0, p12=0, p22=0, p=0;	
	const unsigned int NRows = IdxY+1;
	const unsigned int NCols = IdxX+1;

	for ( i=0; i<NRows-1; i++ )
	{
		for ( j=0; j<NCols-1; j++ )
		{
			k = i*NCols+j;
			if ( k >= PhaseFactorListSize) { continue; }
			Grid11 = (TPhaseFactorGrid*)(&(GridList[k]));

			k = i*NCols+j+1;
			if ( k >= PhaseFactorListSize) { continue; }
			Grid21 = (TPhaseFactorGrid*)(&(GridList[k]));

			k = (i+1)*NCols+j;
			if ( k >= PhaseFactorListSize) { continue; }
			Grid12 = (TPhaseFactorGrid*)(&(GridList[k]));

			k = (i+1)*NCols+j+1;
			if ( k >= PhaseFactorListSize) { continue; }
			Grid22 = (TPhaseFactorGrid*)(&(GridList[k]));

			x1 = Grid11->m_ImgX;
			y1 = Grid11->m_ImgY;
			x2 = Grid22->m_ImgX;
			y2 = Grid22->m_ImgY;
			dx2x1 = x2-x1;
			dy2y1 = y2-y1;
			dx2x1dy2y1 = dx2x1*dy2y1;

			if ( i ==0 )
			{
				if ( j==0 ) 
				{	//Left & Top
					L = 0;
					T = 0;
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
				}
				else if ( j == (NCols-2) )
				{
					//Right & Top
					T = 0;
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
					R = ImageW;
				}
				else
				{
					// Top
					T = 0;
					L = (IMAGE_SIZE)(Grid11->m_ImgX);					
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(Grid22->m_ImgY);
				}
				
			}
			else if ( i == (NRows-2) )
			{
				if ( j==0 ) 
				{
					//Left Bottom
					L = 0;
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
				}
				else if ( j == (NCols-2) )
				{
					//Right Bottom
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
					R = ImageW;
				}
				else
				{
					//Bottom
					T = (IMAGE_SIZE)(Grid11->m_ImgY);
					L = (IMAGE_SIZE)(Grid11->m_ImgX);
					R = (IMAGE_SIZE)(Grid22->m_ImgX);
					B = (IMAGE_SIZE)(ImageH);
				}
			}
			else if ( j == 0 )
			{
				//Left
				L = 0;
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				R = (IMAGE_SIZE)(Grid22->m_ImgX);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
			}
			else if ( j == (NCols-2) )
			{
				//Right				
				L = (IMAGE_SIZE)(Grid11->m_ImgX);
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
				R = ImageW;
			}
			else
			{
				L = (IMAGE_SIZE)(Grid11->m_ImgX);
				T = (IMAGE_SIZE)(Grid11->m_ImgY);
				R = (IMAGE_SIZE)(Grid22->m_ImgX);
				B = (IMAGE_SIZE)(Grid22->m_ImgY);
			}
			for ( u=T; u<B; u++ )
			{
				for ( v=L; v<R; v++ )
				{
					x = v;
					y = u;

					p11 = (x2-x)*(y2-y)*Grid11->m_Phase;
					p21 = (x-x1)*(y2-y)*Grid21->m_Phase;
					p12 = (x2-x)*(y-y1)*Grid12->m_Phase;
					p22 = (x-x1)*(y-y1)*Grid22->m_Phase;
					p = (p11+p21+p12+p22)/dx2x1dy2y1;
					//if ( p >= PHASE_MAX ) 
					//{	p = PHASE_MAX-1; }
					//else if ( p < 0 ) 
					//{	p = 0; }
					Ptr[u*ImageStep+v] = static_cast<SPACE_DATA>(p);
					k = k;
				}
			}

			k = k;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeCameraCombo() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	CalcFOVSizeResolution();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::RedrawWnd()//重繪視窗
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CString  str;	
	CClientDC dc(&m_ImageWnd);	
	HDC hDC = dc.GetSafeHdc();	
	HDC hBKDC1 = m_ImageWndMemDC1.GetSafeHdc();	
	HDC hBKDC2 = m_ImageWndMemDC2.GetSafeHdc();		
	
	::IntersectClipRect(hDC, this->m_ImageWndRect.left, this->m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);	
	::IntersectClipRect(hBKDC1, this->m_ImageWndRect.left, this->m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);	
	::IntersectClipRect(hBKDC2, this->m_ImageWndRect.left, this->m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);	

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hBKDC2, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hBKDC1, 0, 0, SRCCOPY );
	if ( ::fabs(this->m_ImageZoom) > 0.0001 )
	{	str.Format(_T("Zoom:%.2f%%"), 100.0/this->m_ImageZoom); }
	else
	{	str.Format(_T("Zoom:%.2f%%"), 0.00); }
	int BKMode = ::SetBkMode(hBKDC2, TRANSPARENT);
	COLORREF clrText = ::SetTextColor(hBKDC2, 0x8F2F8F);
	::TextOut(hBKDC2, 4, 8, str, str.GetLength());	
	::SetBkMode(hBKDC2, BKMode);
	::SetTextColor(hBKDC2, clrText);	

	int     idx=0;	
	CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);
	switch ( ManipulateMode )
	{	
	case CALIALIGN_MANIPULATE_RECT:
		this->DrawRectLine(hBKDC2);
		break;
	case CALIALIGN_MANIPULATE_GRID:
		this->DrawGridLine(hBKDC2);
		break;
	case CALIALIGN_MANIPULATE_PHASE_LINE:
		this->DrawPhaseLine(hBKDC2);
		break;
	case CALIALIGN_MANIPULATE_FACTOR_GRID:
		this->DrawHFactorLine(hBKDC2);
		break;
	}

	if ( TRUE == this->m_bShowCenterLine )
	{	this->DrawCrossLine(hBKDC2);	}

	if ( TRUE == this->m_bShowHorizontalLine )
	{	this->DrawHorLine(hBKDC2);	}

	if ( TRUE == this->m_bShowVerticalLine )
	{	this->DrawVerLine(hBKDC2);	}	

	if ( CALIBRATION_3D_CAST_FOCUS == m_CalibrationMode )
	{	this->Draw3DCastFocus(hBKDC2);	}
	this->DrawImageFrame(hBKDC2);
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hBKDC2, 0, 0, SRCCOPY );

	this->DrawBigInfoWnd();//繪製大訊息視窗
	this->DrawImageTargetWnd();//繪製目標影像視窗
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawHorLine(HDC hDC)//繪製水平線
{	
	POINT    WndPti;
	TPOINT2D WndPtd;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageHorPt, WndPtd);	
	
	JetAPI::Point2DToPoint(WndPtd, WndPti);

	HPEN hPen = ::CreatePen(PS_DOT, 1, 0x00FFFF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::MoveToEx(hDC, m_ImageWndRect.left, WndPti.y, NULL);
	::LineTo(hDC, m_ImageWndRect.right, WndPti.y);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawVerLine(HDC hDC)//繪製垂直線
{
	POINT    WndPti;
	TPOINT2D WndPtd;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageVerPt, WndPtd);	
	
	JetAPI::Point2DToPoint(WndPtd, WndPti);

	HPEN hPen = ::CreatePen(PS_DOT, 1, 0x00FFFF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::MoveToEx(hDC, WndPti.x, m_ImageWndRect.top, NULL);
	::LineTo(hDC, WndPti.x, m_ImageWndRect.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawRectLine(HDC hDC)//繪區域直線
{
	//TRECT4D                    m_ImageRect4D;
	POINT    WndPt1i, WndPt2i;
	TPOINT2D WndPt1d, WndPt2d;
	TPOINT2D ImagePt1d, ImagePt2d;

	ImagePt1d.x = m_ImageRect4D.left;
	ImagePt1d.y = m_ImageRect4D.top;
	ImagePt2d.x = m_ImageRect4D.right;
	ImagePt2d.y = m_ImageRect4D.bottom;

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt1d, WndPt1d);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt2d, WndPt2d);

	JetAPI::Point2DToPoint(WndPt1d, WndPt1i);
	JetAPI::Point2DToPoint(WndPt2d, WndPt2i);	

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	ImageAPI.DrawRectLine(hDC, WndPt1i, WndPt2i);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawGridLine(HDC hDC)//繪區域直線
{
	size_t  i=0;	
	CString str;
	int     ClrIdx=0;
	int     TextX=0, TextY=0;
	COLORREF clrText=0x808000;
	COLORREF clrOldText=0x00;
	COLORREF clrRed = 0x0000FF;
	COLORREF clrGrn = 0x00AF00;
	COLORREF clrBlu = 0xFF0000;
	COLORREF clrBK = 0x000000;
	COLORREF clrOldBK = 0x000000;
	RECT    GridRect={0};
	int     BkMode = 0;
	double  clrRatio = 0;
	double  MinGray=0;
	double  MaxGray=0;
	bool    bDrawText=false;
	const int     FontH = 16;
	const int     FontW = 16;
	TImageStat  *ImageGridPtr=NULL;
	POINT    WndPt1i, WndPt2i;
	TPOINT2D WndPt1d, WndPt2d;
	TPOINT2D ImagePt1d, ImagePt2d;	
	const size_t GridCount = m_ImageGridRows*m_ImageGridCols;

	HBRUSH hBrush = NULL;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = 10;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);	
	HPEN redPen = ::CreatePen(PS_SOLID, 2, 0x0000F0);
	HPEN grnPen = ::CreatePen(PS_SOLID, 2, 0x00F000);
	HPEN grdPen = ::CreatePen(PS_DOT, 1, 0x802080);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, grdPen);
	clrOldText = ::SetTextColor(hDC, clrText);
	BkMode = ::SetBkMode(hDC, OPAQUE);
	clrOldBK = ::SetBkColor(hDC, clrBK);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	int cntNG=0, cntOK=0;
	for ( i=0; i<GridCount; i++ )
	{
		ImageGridPtr = &(m_ImageGridList[i]);
		if ( NULL == ImageGridPtr ) { continue; }

		bDrawText=false;
		ImagePt1d.x = ImageGridPtr->m_Rect.left;
		ImagePt1d.y = ImageGridPtr->m_Rect.top;
		ImagePt2d.x = ImageGridPtr->m_Rect.right;
		ImagePt2d.y = ImageGridPtr->m_Rect.bottom;
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt1d, WndPt1d);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt2d, WndPt2d);		
		JetAPI::Point2DToPoint(WndPt1d, WndPt1i);
		JetAPI::Point2DToPoint(WndPt2d, WndPt2i);
		GridRect.left = WndPt1i.x;
		GridRect.top = WndPt1i.y;
		GridRect.right = WndPt2i.x;
		GridRect.bottom = WndPt2i.y;
		
		::MoveToEx(hDC, WndPt1i.x, WndPt1i.y, NULL);
		::LineTo(hDC, WndPt2i.x, WndPt1i.y);
		::LineTo(hDC, WndPt2i.x, WndPt2i.y);
		::LineTo(hDC, WndPt1i.x, WndPt2i.y);
		::LineTo(hDC, WndPt1i.x, WndPt1i.y);						
		
		if ( ImageGridPtr->m_Ratio < 50 )
		{	ClrIdx = 0;	}
		else
		{	ClrIdx=static_cast<int>((ImageGridPtr->m_Ratio-50)/5.0);	}
		if ( ClrIdx < 0 ) { ClrIdx = 0; }
		else if ( ClrIdx > 9 ) { ClrIdx = 9; }
		clrBK = GridColorList[ClrIdx];
		::SetBkColor(hDC, clrBK);
	//	clrText = ImageAPI.InterpolateColorValue(0x40AF00, 0x4000AF, clrRatio);
	//	::SetTextColor(hDC, clrText);

		TextX=WndPt1i.x+8;
		TextY=WndPt1i.y+8;

		str.Format(_T("%.0f "), ImageGridPtr->m_Ave);
		if ( (TextY+FontH)<WndPt2i.y && (FontW*str.GetLength())<WndPt2i.x)
		{				
			::SetTextColor(hDC, clrBlu);
			::TextOut(hDC, TextX, TextY, str, str.GetLength());
			TextY += FontH;
			bDrawText = true;
		}
		str.Format(_T("%.0f%%"), ImageGridPtr->m_Ratio);
		if ( (TextY+FontH)<WndPt2i.y && (FontW*str.GetLength())<WndPt2i.x)
		{	
			if (ImageGridPtr->m_Ratio < m_GrayMinRatio )
			{	::SetTextColor(hDC, clrRed); }
			else
			{	::SetTextColor(hDC, clrGrn); }
			::TextOut(hDC, TextX, TextY, str, str.GetLength());
			TextY += FontH;
			bDrawText = true;
		}
		else
		{	::SetTextColor(hDC, 0x000000);	}

		if ( (ImageGridPtr->m_State&OBJECT_STATE_MAXIMUM)==OBJECT_STATE_MAXIMUM )//最大值
		{	
			::SelectObject(hDC, grnPen);
			ImageAPI.DrawRectLine(hDC, GridRect);
			cntOK ++;
		}
		else if ( (ImageGridPtr->m_State&OBJECT_STATE_MINIMUM)==OBJECT_STATE_MINIMUM )//最小值
		{	
			::SelectObject(hDC, redPen);
			ImageAPI.DrawRectLine(hDC, GridRect);
			cntNG ++;
		}		
		::SelectObject(hDC, grdPen);

		if ( false == bDrawText )
		{
			hBrush = ::CreateSolidBrush(clrBK);
			::InflateRect(&GridRect, -2, -2);
			::FillRect(hDC, &GridRect, hBrush);
			::DeleteObject(hBrush);	hBrush = NULL;
		}
	}
	 
	::SetBkMode(hDC, BkMode);
	::SetBkColor(hDC, clrOldBK);
	::SetTextColor(hDC, clrOldText);
	::SelectObject(hDC, hOldFont);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(grdPen);	grdPen=NULL;
	::DeleteObject(redPen);	redPen=NULL;
	::DeleteObject(grnPen);	grnPen=NULL;
	::DeleteObject(hFont);	hFont=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawHFactorLine(HDC hDC)//繪區高度係數
{
	size_t  i=0;	
	CString str;
	int     ClrIdx=0;
	int     TextX=0, TextY=0;
	COLORREF clrText=0x808000;
	COLORREF clrOldText=0x00;
	COLORREF clrRed = 0x0000AF;
	COLORREF clrGrn = 0x00AF00;
	COLORREF clrBlu = 0xFF0000;
	COLORREF clrBK = 0x000000;
	COLORREF clrOldBK = 0x000000;
	RECT    GridRect={0};
	RECT    GridRect2={0};
	RECT    TempRect={0};
	int     BkMode = 0;
	double  clrRatio = 0;
	double  MinGray=0;
	double  MaxGray=0;
	bool    bDrawText=false;
	const int     FontH = 24;
	const int     FontW = 10;
	const int     ShowSizeW = 32;
	const int     ShowSizeH = 32;
	TPhaseFactorGrid  *GridPtr=NULL;
	POINT    GridCP={0};
	POINT    WndPt1i={0}, WndPt2i={0};
	TPOINT2D WndPt1d, WndPt2d;
	TPOINT2D ImagePt1d, ImagePt2d;	
	const bool  CheckByLocalAve=true;
	const size_t GridCount = m_PhaseFactorList.size();//m_PhaseFactorRows*m_PhaseFactorCols;	
	const double FactorMin = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT));
	const double FactorMax = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT));

	HBRUSH hBrush = NULL;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = 24;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);	
	HPEN redPen = ::CreatePen(PS_SOLID, 2, 0x0000F0);
	HPEN grnPen = ::CreatePen(PS_SOLID, 2, 0x00F000);
	HPEN grdPen = ::CreatePen(PS_DOT, 1, 0x80F0F0);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, grdPen);
	clrOldText = ::SetTextColor(hDC, clrText);
	BkMode = ::SetBkMode(hDC, TRANSPARENT);//OPAQUE
	clrOldBK = ::SetBkColor(hDC, clrBK);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	int cntNG=0, cntOK=0;

	for ( i=0; i<GridCount; i++ )
	{
		GridPtr = &(m_PhaseFactorList[i]);
		if ( NULL == GridPtr ) { continue; }

		bDrawText=false;
		ImagePt1d.x = GridPtr->m_RectTarget.left;
		ImagePt1d.y = GridPtr->m_RectTarget.top;
		ImagePt2d.x = GridPtr->m_RectTarget.right;
		ImagePt2d.y = GridPtr->m_RectTarget.bottom;
		//ImagePt1d.x = GridPtr->m_RectLevel.left;
		//ImagePt1d.y = GridPtr->m_RectLevel.top;
		//ImagePt2d.x = GridPtr->m_RectLevel.right;
		//ImagePt2d.y = GridPtr->m_RectLevel.bottom;
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt1d, WndPt1d);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt2d, WndPt2d);		
		JetAPI::Point2DToPoint(WndPt1d, WndPt1i);
		JetAPI::Point2DToPoint(WndPt2d, WndPt2i);
		GridRect.left = WndPt1i.x;
		GridRect.top = WndPt1i.y;
		GridRect.right = WndPt2i.x;
		GridRect.bottom = WndPt2i.y;
		GridCP.x = (GridRect.left+GridRect.right)/2;
		GridCP.y = (GridRect.top+GridRect.bottom)/2;

		GridRect2.left  = GridCP.x - ShowSizeW;
		GridRect2.right = GridCP.x + ShowSizeW;
		GridRect2.top   = GridCP.y - ShowSizeH;
		GridRect2.bottom= GridCP.y + ShowSizeH;
		ImageAPI.DrawRectLine(hDC, GridRect2);

		if ( i == this->m_PhaseFactorIndex )
		{
			TempRect = GridRect;
			::InflateRect(&TempRect, -2, -2);
			::SelectObject(hDC, redPen);
			ImageAPI.DrawRectLine(hDC, TempRect);
			::SelectObject(hDC, grdPen);
		}
		
		str.Format(_T("%.0f "), GridPtr->m_Factor);
		TextX=GridCP.x-(FontW*str.GetLength()/2);
		TextY=GridCP.y-(FontH/2);
		if ( ((TextY+FontH)/2)<WndPt2i.y && (FontW*str.GetLength()/2)<WndPt2i.x)
		{	
			if ( ::fabs(GridPtr->m_Factor-1.0)<0.1 )
			{	::SetTextColor(hDC, clrBlu); }
			else
			{
				bool bNG=false;
				if ( true == CheckByLocalAve )
				{
					if ( fabs(GridPtr->m_FactorAve) > 0.1 )
					{
						double LocRatio=100.0*GridPtr->m_Factor/GridPtr->m_FactorAve;
						double LocDif=fabs(LocRatio-100.0);
						if ( LocDif > 10 )
						{	bNG = true;	}
					}
				}
				else
				{
					if ( GridPtr->m_Factor<FactorMin || GridPtr->m_Factor>FactorMax )
					{	bNG = true;	}
				}
				if ( true == bNG )
				{	::SetTextColor(hDC, clrRed);	}
				else
				{	::SetTextColor(hDC, clrGrn); }
			}
			::TextOut(hDC, TextX, TextY, str, str.GetLength());
			TextY += FontH;
			bDrawText = true;
		}
		
		/*
		if ( ImageGridPtr->m_Ratio < 50 )
		{	ClrIdx = 0;	}
		else
		{	ClrIdx=static_cast<int>((ImageGridPtr->m_Ratio-50)/5.0);	}
		if ( ClrIdx < 0 ) { ClrIdx = 0; }
		else if ( ClrIdx > 9 ) { ClrIdx = 9; }
		clrBK = GridColorList[ClrIdx];
		::SetBkColor(hDC, clrBK);
	//	clrText = ImageAPI.InterpolateColorValue(0x40AF00, 0x4000AF, clrRatio);
	//	::SetTextColor(hDC, clrText);

		

		str.Format(_T("%.0f "), ImageGridPtr->m_Ave);
		if ( (TextY+FontH)<WndPt2i.y && (FontW*str.GetLength())<WndPt2i.x)
		{				
			::SetTextColor(hDC, clrBlu);
			::TextOut(hDC, TextX, TextY, str, str.GetLength());
			TextY += FontH;
			bDrawText = true;
		}
		str.Format(_T("%.0f%%"), ImageGridPtr->m_Ratio);
		if ( (TextY+FontH)<WndPt2i.y && (FontW*str.GetLength())<WndPt2i.x)
		{	
			if (ImageGridPtr->m_Ratio < m_GrayMinRatio )
			{	::SetTextColor(hDC, clrRed); }
			else
			{	::SetTextColor(hDC, clrGrn); }
			::TextOut(hDC, TextX, TextY, str, str.GetLength());
			TextY += FontH;
			bDrawText = true;
		}
		else
		{	::SetTextColor(hDC, 0x000000);	}

		if ( (ImageGridPtr->m_State&OBJECT_STATE_MAXIMUM)==OBJECT_STATE_MAXIMUM )//最大值
		{	
			::SelectObject(hDC, grnPen);
			ImageAPI.DrawRectLine(hDC, GridRect);
			cntOK ++;
		}
		else if ( (ImageGridPtr->m_State&OBJECT_STATE_MINIMUM)==OBJECT_STATE_MINIMUM )//最小值
		{	
			::SelectObject(hDC, redPen);
			ImageAPI.DrawRectLine(hDC, GridRect);
			cntNG ++;
		}		
		::SelectObject(hDC, grdPen);

		if ( false == bDrawText )
		{
			hBrush = ::CreateSolidBrush(clrBK);
			::InflateRect(&GridRect, -2, -2);
			::FillRect(hDC, &GridRect, hBrush);
			::DeleteObject(hBrush);	hBrush = NULL;
		}
		*/
	}
	 
	::SetBkMode(hDC, BkMode);
	::SetBkColor(hDC, clrOldBK);
	::SetTextColor(hDC, clrOldText);
	::SelectObject(hDC, hOldFont);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(grdPen);	grdPen=NULL;
	::DeleteObject(redPen);	redPen=NULL;
	::DeleteObject(grnPen);	grnPen=NULL;
	::DeleteObject(hFont);	hFont=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawCrossLine(HDC hDC)//繪製十字線
{
	POINT    WndPti;	
	TPOINT2D WndPtd;
	TPOINT2D ImagePtd;
	ImagePtd.x = (double)(m_ImageW/2.0);
	ImagePtd.y = (double)(m_ImageH/2.0);

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);	
	JetAPI::Point2DToPoint(WndPtd, WndPti);
	HPEN hPen = ::CreatePen(PS_DOT, 1, 0x00FF00);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	::MoveToEx(hDC, m_ImageWndRect.left, WndPti.y, NULL);
	::LineTo(hDC, m_ImageWndRect.right, WndPti.y);

	::MoveToEx(hDC, WndPti.x, m_ImageWndRect.top, NULL);
	::LineTo(hDC, WndPti.x, m_ImageWndRect.bottom);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawPhaseLine(HDC hDC)//繪製相位線
{
	CalcPhaseNormLine();

	CString  str;
	size_t   i = 0;
	RECT     Rect={0};
	POINT    WndPti1;	
	POINT    WndPti2;		
	TPOINT2D WndPtd1;
	TPOINT2D WndPtd2;
	TPOINT2D ImagePtd1;
	TPOINT2D ImagePtd2;	
	TPOINT2D ImagePtdc;
	TPIXEL_GRY  *GryPixelPtr = NULL;
	const size_t PhasePtListSize = m_PhasePtList.size();
	
	HBRUSH hBrush = NULL;
	HPEN hPenRed = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hPenGrn = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPenRed);

	hBrush = ::CreateSolidBrush(0x4040FF);
	for ( i=0; i<PhasePtListSize; i++ )
	{
		GryPixelPtr = &(m_PhasePtList[i]);
		ImagePtd1.x = GryPixelPtr->x;
		ImagePtd1.y = GryPixelPtr->y;
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd1, WndPtd1);			
		JetAPI::Point2DToPoint(WndPtd1, WndPti1);
		Rect.left  = WndPti1.x - 4;
		Rect.right = WndPti1.x + 4;
		Rect.top   = WndPti1.y - 4;
		Rect.bottom= WndPti1.y + 4;
		::FillRect(hDC, &Rect, hBrush);
	}
	::DeleteObject(hBrush); hBrush= NULL;

	ImagePtdc.x = (m_PhaseLinePt1.x+m_PhaseLinePt2.x)/2;
	ImagePtdc.y = (m_PhaseLinePt1.y+m_PhaseLinePt2.y)/2;
	//繪製斜線
	ImagePtd1 = m_PhaseLinePt1;
	ImagePtd2 = m_PhaseLinePt2;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd1, WndPtd1);	
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd2, WndPtd2);	
	
	JetAPI::Point2DToPoint(WndPtd1, WndPti1);
	JetAPI::Point2DToPoint(WndPtd2, WndPti2);

	::SelectObject(hDC, hPenRed);
	::MoveToEx(hDC, WndPti1.x, WndPti1.y, NULL);
	::LineTo(hDC, WndPti2.x, WndPti2.y);

	//繪製法線
	ImagePtd1 = m_PhaseNormPt1;
	ImagePtd2 = m_PhaseNormPt2;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd1, WndPtd1);	
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd2, WndPtd2);	

	JetAPI::Point2DToPoint(WndPtd1, WndPti1);
	JetAPI::Point2DToPoint(WndPtd2, WndPti2);	

	::SelectObject(hDC, hPenGrn);
	::MoveToEx(hDC, WndPti1.x, WndPti1.y, NULL);
	::LineTo(hDC, WndPti2.x, WndPti2.y);

	//中央
	::SetTextColor(hDC, 0xFFFFFF);
	ImagePtd1 = ImagePtdc;	
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd1, WndPtd1);		
	JetAPI::Point2DToPoint(WndPtd1, WndPti1);
	
	str.Format(_T("Count:%d"), this->m_PhaseCount);
	::TextOut(hDC, WndPti1.x, WndPti1.y, str, str.GetLength());
	str.Format(_T("Range:%.2f Pixels"), this->m_PhaseRange);
	::TextOut(hDC, WndPti1.x, WndPti1.y+16, str, str.GetLength());
	str.Format(_T("Tile:%.2f Degree"), this->m_PhaseAngle);
	::TextOut(hDC, WndPti1.x, WndPti1.y+32, str, str.GetLength());
	str.Format(_T("Height:%.2f um"), this->m_PhaseHeight);
	::TextOut(hDC, WndPti1.x, WndPti1.y+48, str, str.GetLength());	

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenRed);	hPenRed=NULL;
	::DeleteObject(hPenGrn);	hPenGrn=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::Draw3DCastFocus(HDC hDC)//繪製3D投光焦距數值
{		
	CString str;
	POINT    WndPti;
	TPOINT2D ImagePtd, WndPtd;
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;
	const IMAGE_SIZE ImageW2 = ImageW/2;
	const IMAGE_SIZE ImageH2 = ImageH/2;
	const IMAGE_SIZE GridCount = 10;
	const IMAGE_SIZE GridW = ImageW/GridCount;
	const IMAGE_SIZE GridH = ImageH/GridCount;
	const IMAGE_SIZE GridW2 = GridW/2;
	const IMAGE_SIZE GridH2 = GridH/2;
	const int FontHeight = 96;
	const double WordNum = 2.3;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontHeight;
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);	
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);
	COLORREF ClrBK = ::SetBkColor(hDC, 0xFFFFFF);
	COLORREF ClrTxet= ::SetTextColor(hDC, 0x0000FF);

	//Left Top
	ImagePtd.x = GridW2;
	ImagePtd.y = GridH2;	
	ImagePtd.x = 0;
	ImagePtd.y = 0;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);
	JetAPI::Point2DToPoint(WndPtd, WndPti);	
	str.Format(_T("%.2f"), m_3DCastFocusLT);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());

	//Right Top
	ImagePtd.x = ImageW-GridW2;
	ImagePtd.y = GridH2;
	ImagePtd.x = (int)(ImageW-(FontHeight*m_ImageZoom)*WordNum);
	ImagePtd.y = 0;	
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);
	JetAPI::Point2DToPoint(WndPtd, WndPti);
	str.Format(_T("%.2f"), m_3DCastFocusRT);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());

	//Left Bottom
	ImagePtd.x = GridW2;
	ImagePtd.y = ImageH-GridH2;	
	ImagePtd.x = 0;
	ImagePtd.y = (int)(ImageH-(FontHeight*m_ImageZoom));
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);
	JetAPI::Point2DToPoint(WndPtd, WndPti);
	str.Format(_T("%.2f"), m_3DCastFocusLB);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());

	//Right Bottom
	ImagePtd.x = ImageW-GridW2;
	ImagePtd.y = ImageH-GridH2;	
	ImagePtd.x = (int)(ImageW-(FontHeight*m_ImageZoom)*WordNum);
	ImagePtd.y = (int)(ImageH-(FontHeight*m_ImageZoom));
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);
	JetAPI::Point2DToPoint(WndPtd, WndPti);
	str.Format(_T("%.2f"), m_3DCastFocusRB);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());

	//Center
	ImagePtd.x = ImageW2;
	ImagePtd.y = ImageH2;	
	ImagePtd.x = (int)(ImageW2-(FontHeight*m_ImageZoom*0.5)*WordNum);
	ImagePtd.y = (int)(ImageH2-(FontHeight*m_ImageZoom*0.5));
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);
	JetAPI::Point2DToPoint(WndPtd, WndPti);
	str.Format(_T("%.2f"), m_3DCastFocusCC);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());

	str.Format(_T("Max:%.2f"), m_3DCastFocusCCMax);
	WndPti.x = WndPti.x-FontHeight;
	WndPti.y = WndPti.y+(FontHeight*0.75);
	::TextOut(hDC, WndPti.x, WndPti.y, str, str.GetLength());	

	::SetBkColor(hDC, ClrBK);
	::SetTextColor(hDC, ClrTxet);	
	::SelectObject(hDC, hOldFont);
	::SetBkMode(hDC, BKMode);
	::DeleteObject(hFont);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawImageFrame(HDC hDC)//繪製影像外框
{	
	RECT     Rect;
	TPOINT2D ImagePtd[2], WndPtd[2];
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;

	//Left Top
	ImagePtd[0].x = 0;	ImagePtd[0].y = 0;
	ImagePtd[1].x = ImageW;	ImagePtd[1].y = ImageH;
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd[0], WndPtd[0]);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd[1], WndPtd[1]);
	Rect.left   = JetAPI::Floor(WndPtd[0].x);
	Rect.top    = JetAPI::Floor(WndPtd[0].y);
	Rect.right  = JetAPI::Floor(WndPtd[1].x);
	Rect.bottom = JetAPI::Floor(WndPtd[1].y);

	HPEN hPen = ::CreatePen(PS_DOT, 1, 0xEAD999);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2)
{
	return JetAPI::CheckPtInCtrlWnd(this, pt, ControlID, &pt2);

//	CWnd *pWnd = this->GetDlgItem(ControlID);
//	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) )
//	{	return false; }

//	pt2 = pt;
//	this->MapWindowPoints(pWnd, &pt2, 1);
	
//	RECT Rect={0};
//	pWnd->GetClientRect(&Rect);
//	if ( ::PtInRect(&Rect, pt2) == FALSE )
//	{	return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawImageWndMemDC()//建立影像的記憶體圖像	
{	
	HDC hMemDC = this->m_ImageWndMemDC1.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	COLORREF clrBK = m_BKColor;		
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	if ( NULL == m_ShowBuffer ) { return; }
	//if ( ImageAPI.DrawImageToDC(hMemDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	if ( ImageAPI.DrawImageToDC(hMemDC, m_ImageW, m_ImageH, m_ShowStep, m_ShowBitCount, m_ShowBuffer, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	{	return ; }	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Check3DCastID(bool bShowMsg)//確認3D投光編號
{
	CString str;
	const int CastID = (int)(JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox));
	if ( 0 == CastID )
	{
		if ( true == bShowMsg )
		{
			str = _T("Please Select 3D Cast First!");
			str = this->LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
		}
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CheckHeightFactorPhaseID()//確認高度比例的平面編號
{
	CString str;
	const int PhaseID = (int)(JetAPI::GetComboxCurSelData(this->m_PhaseIDCombox));
	if ( BATCH_GRAB_PHASE_M != PhaseID )//BATCH_GRAB_PHASE_M2
	{
		str = _T("Please Select Phase M !");
		str = this->LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ConfigGrabParam(CALIBRATION_MODE Mode)//組態取像參數
{	
	CString str;	
	unsigned int i=0, j=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(m_CameraCombox));	
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	const IMAGE_SIZE ImageWHalf = ImageW/2;
	const IMAGE_SIZE ImageHHalf = ImageH/2;

	CWnd::GetDlgItemText(CALIALIGN_GRID_ROWS_EDIT, str);
	m_ImageGridRows = ::_ttoi(str);
	CWnd::GetDlgItemText(CALIALIGN_GRID_COLS_EDIT, str);
	m_ImageGridCols = ::_ttoi(str);
	if ( m_ImageGridRows<=0 || m_ImageGridCols<=0 )
	{	return false; }

	GetFovSize(CaliParam.m_FOVWidth_um, CaliParam.m_FOVHeight_um);	
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombo);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);		
	if ( NULL != SliceParamPtr)
	{	m_SliceParam = *SliceParamPtr;	}
	else
	{	m_SliceParam = TSliceParam(); }

	m_PhaseFactorRows = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NROWS_EDIT);
	m_PhaseFactorCols = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_NCOLS_EDIT);
	if ( m_PhaseFactorRows<=0 || m_PhaseFactorCols<=0 )
	{	return false; }	
	this->m_ZeroPhaseOffsetPosZ = (int)(CWnd::GetDlgItemInt(CALIALIGN_PHASE_PLANE_OFFSET_EDIT));
	const int TargetHeightValue = (int)CWnd::GetDlgItemInt(CALIALIGN_PHASE_TARGET_HEIGHT_EDIT);
	const int PhaseFovPitch = (int)CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_PITCH_EDIT);
	const int PhaseFovRange = (int)CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_RANGE_EDIT);	
	CWnd::GetDlgItemText(CALIALIGN_3DCAST_MOUNT_ANGLE_EDIT, str);	
	CaliParam.m_3DCastMountAngle = ::_tcstod(str, NULL);

	//m_EnableBasePhaseCorrect
	//m_BasePhaseCorrectTimes;
	//m_EnableHeightFactorCorrect;//啟用高度係數修正
	//m_HeightFactorCorrectTimes;//高度係數修正次數//v1.01.04.001
	CaliParam.m_PhaseZeroPlaneOffsetPosZ = this->m_ZeroPhaseOffsetPosZ;
	CaliParam.m_TargetHeightThickValue = TargetHeightValue;
	CaliParam.m_PhaseFactorFovPitch = PhaseFovPitch;
	CaliParam.m_PhaseFactorFovRange = PhaseFovRange;
	
	CaliParam.m_3DCastHeightFactorGridRows = m_PhaseFactorRows;
	CaliParam.m_3DCastHeightFactorGridCols = m_PhaseFactorCols;
	
	CalcFOVSizeResolution();
	UpdatePhaseNoiseParamFromUI();
	CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);
	
	int DLPIndex = -1;
	int SliceFrameIndex = -1;
	bool bUsePhaseLED=false;
	const int CurrentID = GetDLPLEDCurrentID();
	const double ResX = m_ImageToStageScaleX;
	const double ResY = m_ImageToStageScaleY;
	const double FOVW = CaliParam.m_FOVWidth_um;
	const double FOVH = CaliParam.m_FOVHeight_um;		
	const int TargetNum = JetAPI::GetComboxCurSelData(m_HeightFactorNumCombox);

	DWORD BatchGrabProj = 0;
	BOOL  bBuildPhaseFactorList = FALSE;
	this->m_PhaseID = JetAPI::GetComboxCurSelData(this->m_PhaseIDCombox);	
	this->m_LightNum = JetAPI::GetComboxCurSelData(this->m_LightCombox);		
	this->m_Light3DCastID = (LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox);
	LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(m_Light3DCastID);
	
	m_Multi3DCastID = false;
	m_PhaseFactorID_Z = 0;
	SetSaveRawImageTimes(0);
	m_CameraImageReceieveCount = 0;	
	switch ( this->m_Light3DCastID )
	{
	case LIGHT_3D_CAST_01:	
		DLPIndex = 0;
		m_PatternProjectMode = BATCH_GRAB_3D_CAST_01;	
		break;
	case LIGHT_3D_CAST_02:	
		DLPIndex = 1;
		m_PatternProjectMode = BATCH_GRAB_3D_CAST_02;	
		break;
	case LIGHT_3D_CAST_03:	
		DLPIndex = 2;
		m_PatternProjectMode = BATCH_GRAB_3D_CAST_03;	
		break;
	case LIGHT_3D_CAST_04:	
		DLPIndex = 3;
		m_PatternProjectMode = BATCH_GRAB_3D_CAST_04;	
		break;
	default:
		DLPIndex = -1;
		m_PatternProjectMode = 0; 
		break;
	}	
	switch ( Mode )
	{
	case CALIBRATION_FOV_WIDTH_START:		
	case CALIBRATION_FOV_HEIGHT_START:
	case CALIBRATION_CAMERA_ALIGN_HOR_START:
	case CALIBRATION_CAMERA_ALIGN_VER_START:	
	case CALIBRATION_IMAGE_RESOLUTION:
		switch ( ManipulateMode )
		{
		case CALIALIGN_MANIPULATE_PHASE_LINE:
		case CALIALIGN_MANIPULATE_FACTOR_GRID:			
			JetAPI::SetComboxCurSel(this->m_ManipulateCombox, CALIALIGN_MANIPULATE_NONE);
			break;
		}
		if ( 0==m_LightNum || LIGHT_3D_CAST_00!=m_Light3DCastID )
		{				
			m_PatternStep = BATCH_GRAB_STEP_1;
			m_PhaseID = BATCH_GRAB_PHASE_1;			
			//m_CameraExposureTime_us = Param->m_TargetGridExpTime_us;						
		}
		else
		{	
			DLPIndex = -1;
			m_PatternStep = 0;
			m_PhaseID = 0;			
			m_PatternProjectMode = 0;			
		}
		break;	
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_START:
	case CALIBRATION_CAMERA_ALIGN_HOR_3D_END:
	case CALIBRATION_CAMERA_ALIGN_VER_3D_START:
	case CALIBRATION_CAMERA_ALIGN_VER_3D_END:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;
		if ( DLPIndex < 0 )
		{
			DLPIndex = 0;
			m_Multi3DCastID = true;		
		}	
		break;
	case CALIBRATION_IMAGE_FOCUS_AUTO:
	case CALIBRATION_IMAGE_FOCUS_AUTO_10:
	case CALIBRATION_IMAGE_FOCUS_AUTO_100:
		if ( 0==m_LightNum || LIGHT_3D_CAST_00!=m_Light3DCastID )
		{				
			m_PatternStep = BATCH_GRAB_STEP_1;
			m_PhaseID = BATCH_GRAB_PHASE_1;			
			//m_CameraExposureTime_us = Param->m_TargetGridExpTime_us;			
		}
		else
		{
			DLPIndex = -1;
			m_PatternStep = 0;
			m_PhaseID = 0;			
			m_PatternProjectMode = 0;			
		}
		break;
	case CALIBRATION_2D_LIGHT_ALIGN:
		DLPIndex = -1;
		//JetAPI::SetComboxCurSel(m_3DCastIDCombox, 0);
		break;
	case CALIBRATION_3D_CAST_ALIGN:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_1;
		this->m_PhaseID = BATCH_GRAB_PHASE_1;		
		//this->m_CameraExposureTime_us = Param->m_TargetWhiteExpTime_us;		
		break;
	case CALIBRATION_3D_CAST_FOCUS:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;

		if ( BATCH_GRAB_PHASE_M == m_PhaseID )//BATCH_GRAB_PHASE_M2
		{	m_PhaseID = BATCH_GRAB_PHASE_1;		}

		//this->m_CameraExposureTime_us = Param->m_TargetWhiteExpTime_us;		
		break;	
	case CALIBRATION_2D_LIGHT_CURRENT:
		DLPIndex = -1;		
		for ( i=0; i<LED_CHANNEL_COUNT; i++ )
		{
			if ( FN_DISABLE == m_SliceParam.SliceLightTable.LEDChannel[i].OnOffState ) { continue; }
			m_SliceParam.SliceLightTable.LEDChannel[i].PowerValue = (unsigned int)(m_2DLEDCurrent); 
		}		

		CWnd::GetDlgItemText(CALIALIGN_LIGHT_CURRENT_GAIN_EDIT, str);		
		m_SliceParam.SliceGainValue = ::_ttof(str);		
		SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);		
		if ( NULL != SliceParamPtr)
		{	SliceParamPtr->SliceGainValue = ::_ttof(str);	}		
		CWnd::SetDlgItemInt(CALIALIGN_LIGHT_CURRENT_EXP_EDIT, m_CameraExposureTime_us);			
		break;
	case CALIBRATION_3D_CAST_CURRENT:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_1;
		this->m_PhaseID = BATCH_GRAB_PHASE_1;		
		this->m_CameraExposureTime_us = this->m_3DCastExposureTimeus*1.0;//2.5->1.0		
		JetAPI::SetComboxCurSel(m_PhaseLEDCombox, DLP_LED_COLOR_WHITE);
		break;
	case CALIBRATION_3D_CAST_CURRENT_RED:		
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_1;
		this->m_PhaseID = BATCH_GRAB_PHASE_1;				
		this->m_CameraExposureTime_us = this->m_3DCastExposureTimeus*1.0;//2.5->1.0		
		if ( NULL != Light3DPtr )
		{
			Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_WHITE;
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{	Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_RED;	}
			Light3DCtrl.SetLight3DLEDCurrent(m_Light3DCastID, m_3DCastCurrent, 0, 0, CurrentID);	
		}		
		break;
	case CALIBRATION_3D_CAST_CURRENT_GRN:
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_1;
		this->m_PhaseID = BATCH_GRAB_PHASE_1;		
		this->m_CameraExposureTime_us = this->m_3DCastExposureTimeus*1.0;//2.5->1.0		
		if ( NULL != Light3DPtr )
		{
			Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_WHITE;
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{	Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_GREEN;	}
			Light3DCtrl.SetLight3DLEDCurrent(m_Light3DCastID, 0, m_3DCastCurrent, 0, CurrentID);	
		}		
		break;
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_1;
		this->m_PhaseID = BATCH_GRAB_PHASE_1;		
		this->m_CameraExposureTime_us = this->m_3DCastExposureTimeus*1.0;//2.5->1.0		
		if ( NULL != Light3DPtr )
		{
			Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_WHITE;
			if (LIGHT_3D_DEVICE_DLP4710 == Light3DPtr->GetDeviceType())
			{	Light3DPtr->GetDLPParam().m_LEDColor = DLP_LED_COLOR_BLUE;	}
			Light3DCtrl.SetLight3DLEDCurrent(m_Light3DCastID, 0, 0, m_3DCastCurrent, CurrentID);	
		}		
		break;
		
	case CALIBRATION_PATTERN_ZERO_PLANE:	
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;		
		//this->m_CameraExposureTime_us = Param->m_TargetWhiteExpTime_us;		
		break;	
	case CALIBRATION_3D_MODEL_TEST:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;
		if ( DLPIndex < 0 )
		{
			DLPIndex = 0;
			m_Multi3DCastID = true;		
		}		
		break;
	case CALIBRATION_VERIFY_ZERO_PLANE:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;
		if ( DLPIndex < 0 )
		{
			DLPIndex = 0;
			m_Multi3DCastID = true;		
		}
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT:	
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;
		//this->m_CameraExposureTime_us = Param->m_TargetHeightExpTime_us;
		if ( NULL != Light3DPtr )
		{
			GetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT, str);
			Light3DPtr->SetPhaseFactorMin(::_ttof(str));
			GetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT, str);
			Light3DPtr->SetPhaseFactorMax(::_ttof(str));
		}		
		bBuildPhaseFactorList = TRUE;
		break;	
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV:
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV:
		bUsePhaseLED = true;
		this->m_LightNum = 0;
		this->m_PatternStep = BATCH_GRAB_STEP_4;
		//this->m_CameraExposureTime_us = Param->m_TargetHeightExpTime_us;
		if ( NULL != Light3DPtr )
		{
			GetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT, str);
			Light3DPtr->SetPhaseFactorMin(::_ttof(str));
			GetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT, str);
			Light3DPtr->SetPhaseFactorMax(::_ttof(str));
		}		
		bBuildPhaseFactorList = FALSE;
		break;
	default:
		if ( 0==m_LightNum || LIGHT_3D_CAST_00!=m_Light3DCastID )
		{	
			m_PatternStep = BATCH_GRAB_STEP_1;
			m_PhaseID = BATCH_GRAB_PHASE_1;
		}
		else
		{
			DLPIndex = -1;
			m_PatternStep = 0;
			m_PhaseID = 0;			
			m_PatternProjectMode = 0;;
		}
		break;
	}
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);	
	if ( DLPIndex < 0 ) 
	{	
		m_SliceParam.SliceCameraID = CameraID;
		m_SliceParam.SliceCameraExpTimeus = m_CameraExposureTime_us;
		m_SliceParam.SliceCameraFrames = 1;
		m_SliceParam.SliceLightTable.LightType = LIGHT_LED;		
		m_SliceParam.SliceCameraDelayTimeus = 0;
	}
	else
	{	
		int   NFrames = 1;
		int   TotalNFrames = 0;
		int   PhasePatMode = DLP_PATTERN_SEQUENCE_NONE;
		SLICE_FUNC_MODE SliceFuncMode = m_SliceFuncMode;
		if ( BATCH_GRAB_STEP_1 == m_PatternStep )
		{
			NFrames = 1;
			SliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;
			PhasePatMode = DLP_PATTERN_SEQUENCE_WHITE;
		}
		else
		{			
			switch ( m_PhaseID )
			{
			case BATCH_GRAB_PHASE_1:	
				NFrames = 4;
				PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_1;
				break;
			case BATCH_GRAB_PHASE_2:	
				NFrames = 4;
				PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_2;
				break;
			case BATCH_GRAB_PHASE_M:
				SliceFuncMode=CheckSliceFuncModeByBatchGrabPhaseMode(m_SliceFuncMode, m_PhaseID);
				NFrames=AOIDataCollect.CheckSliceFuncModeDlpPatternCountOnce(SliceFuncMode);
				PhasePatMode=AOIDataCollect.CheckSliceFuncModeDlpPatternMode(SliceFuncMode);
				break;
			case BATCH_GRAB_PHASE_M2:
				SliceFuncMode=CheckSliceFuncModeByBatchGrabPhaseMode(m_SliceFuncMode, m_PhaseID);
				NFrames=AOIDataCollect.CheckSliceFuncModeDlpPatternCountOnce(SliceFuncMode);
				PhasePatMode=AOIDataCollect.CheckSliceFuncModeDlpPatternMode(SliceFuncMode);
				JetAPI::SetComboxCurSel(m_3DCastCurrentIDCombox, DLP_LED_CURRENT_ID_01);
				break;
			}			
		}
		m_SliceParam = TSliceParam();		
		if ( false == m_Multi3DCastID )
		{
			TotalNFrames = NFrames;
			m_SliceParam.SliceLightTable.DLPCast[DLPIndex].OnOffState = FN_ENABLE;
			m_SliceParam.SliceLightTable.DLPCast[DLPIndex].TriggerCount = NFrames;
			m_SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode = PhasePatMode;
		}
		else
		{
			for ( i=0; i<DLP_CAST_COUNT; i++ )
			{
				m_SliceParam.SliceLightTable.DLPCast[i].OnOffState = FN_DISABLE;
				m_SliceParam.SliceLightTable.DLPCast[i].TriggerCount = 0;
				m_SliceParam.SliceLightTable.DLPCast[i].PhasePatMode = PhasePatMode;
			}

			const size_t CastIDCount = m_3DCastIDCombox.GetCount();	
			for ( i=0; i<CastIDCount; i++ )
			{
				LIGHT_3D_CAST_ID CastID = (LIGHT_3D_CAST_ID)(m_3DCastIDCombox.GetItemData(i));
				if ( LIGHT_3D_CAST_00 == CastID ) { continue; }
				LIGHT_3D_CLS_PTR CastPtr2 = Light3DCtrl.GetLight3DCastPtr(CastID);
				if ( NULL == CastPtr2 ) { continue; }				
				DLPIndex = CLight3DCtrl::GetLight3DIndexByCastID(CastID);

				TotalNFrames += NFrames;
				m_SliceParam.SliceLightTable.DLPCast[DLPIndex].OnOffState = FN_ENABLE;
				m_SliceParam.SliceLightTable.DLPCast[DLPIndex].TriggerCount = NFrames;
				m_SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode = PhasePatMode;
			}	
		}
		m_SliceParam.SliceCameraID = CameraID;
		m_SliceParam.SliceCameraExpTimeus = m_CameraExposureTime_us;		
		m_SliceParam.SliceCameraFrames = TotalNFrames;
		m_SliceParam.SliceLightTable.LightType = LIGHT_DLP;		
		m_SliceParam.SliceLightTable.DLPTurnOnTimeus = m_CameraExposureTime_us;		
		m_SliceParam.SliceCameraDelayTimeus = 0;
		m_SliceParam.SliceFuncMode = SliceFuncMode;

		if ( true == bUsePhaseLED )
		{
			const bool bUpdate = true;
			int LEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);			
			if ( NULL != Light3DPtr )
			{	Light3DPtr->GetDLPParam().m_LEDColor = LEDColor;	}
			else
			{	Light3DCtrl.SetAllLight3DLEDColor(LEDColor);	}
		}
	}	

	SIZE  GridSize={0};
	TImageStat ImageGrid;		
	GridSize.cx = ImageW/m_ImageGridCols;
	GridSize.cy = ImageH/m_ImageGridRows;	

	this->m_ImageGridList.clear();
	for ( i=0; i<m_ImageGridRows; i++ )
	{
		for ( j=0; j<m_ImageGridCols; j++ )
		{
			ImageGrid.m_Rect.left = j*GridSize.cx;
			ImageGrid.m_Rect.top = i*GridSize.cy;
			ImageGrid.m_Rect.right = ImageGrid.m_Rect.left+GridSize.cx;
			ImageGrid.m_Rect.bottom = ImageGrid.m_Rect.top+GridSize.cy;
			this->m_ImageGridList.push_back(ImageGrid);
		}
	}	

	if ( TRUE == bBuildPhaseFactorList )
	{		
		const int    GapImg=2;//
		const double ExtendPos = 1000;//1mm
		const int    ExtendImg = JetAPI::Ceil(ExtendPos/ResX);
		TPhaseFactorGrid FactorGrid;
		m_PhaseFactorIndex = 0;
		TPOINT3D StageOffset;
		GridSize.cx = ImageW/m_PhaseFactorCols;
		GridSize.cy = ImageH/m_PhaseFactorRows;	
		GridSize.cx = JetAPI::Floor(m_ImageRect4D.right-m_ImageRect4D.left)+ExtendImg+ExtendImg;
		GridSize.cy = JetAPI::Floor(m_ImageRect4D.bottom-m_ImageRect4D.top)+ExtendImg+ExtendImg;	
		const IMAGE_SIZE RegionW = ImageW-GridSize.cx-GapImg-GapImg;
		const IMAGE_SIZE RegionH = ImageH-GridSize.cy-GapImg-GapImg;
		const int ImagePitchX = RegionW/(m_PhaseFactorCols-1);
		const int ImagePitchY = RegionH/(m_PhaseFactorRows-1);
		const bool SignX = AOIDataCollect.GetStageSignPositiveX();
		const bool SignY = AOIDataCollect.GetStageSignPositiveY();		
		m_PhaseFactorList.clear();		
		FactorGrid.m_TargetNo = TargetNum;
		for ( i=0; i<m_PhaseFactorRows; i++ )
		{
			for ( j=0; j<m_PhaseFactorCols; j++ )
			{
				FactorGrid.m_IdxX = (unsigned int)(j);
				FactorGrid.m_IdxY = (unsigned int)(i);
				FactorGrid.m_RectLevel.left = (j*ImagePitchX)+GapImg;
				FactorGrid.m_RectLevel.top = (i*ImagePitchY)+GapImg;
				FactorGrid.m_RectLevel.right = FactorGrid.m_RectLevel.left+GridSize.cx;
				FactorGrid.m_RectLevel.bottom = FactorGrid.m_RectLevel.top+GridSize.cy;
				::InflateRect(&FactorGrid.m_RectLevel, -GapImg, -GapImg);
				if ( FactorGrid.m_RectLevel.left<0 || FactorGrid.m_RectLevel.top<0 ||FactorGrid.m_RectLevel.right>ImageW || FactorGrid.m_RectLevel.bottom>ImageH )
				{
					str.Format(_T("Error, Phase Factor Grid Fault[L:%d, T:%d, R:%d, B:%d]"), FactorGrid.m_RectLevel.left, FactorGrid.m_RectLevel.top, FactorGrid.m_RectLevel.right, FactorGrid.m_RectLevel.bottom);
					JetAPI::ShowMessageBox(str);
					return false;
				}
				::InflateRect(&FactorGrid.m_RectLevel, GapImg, GapImg);//恢復原來尺寸
				FactorGrid.m_RectTarget = FactorGrid.m_RectLevel;
				::InflateRect(&FactorGrid.m_RectTarget, -ExtendImg, -ExtendImg);
				FactorGrid.m_ImgX = FactorGrid.m_RectTarget.left+FactorGrid.m_RectTarget.right;
				FactorGrid.m_ImgY = FactorGrid.m_RectTarget.top+FactorGrid.m_RectTarget.bottom;
				FactorGrid.m_ImgX = FactorGrid.m_ImgX/2.0;
				FactorGrid.m_ImgY = FactorGrid.m_ImgY/2.0;

				if ( true == SignX )
				{	FactorGrid.m_PosX = m_StagePosX-((FactorGrid.m_ImgX-ImageWHalf)*ResX);	}
				else
				{	FactorGrid.m_PosX = m_StagePosX+((FactorGrid.m_ImgX-ImageWHalf)*ResX);	}

				if ( true == SignY )
				{	FactorGrid.m_PosY = m_StagePosY+((FactorGrid.m_ImgY-ImageHHalf)*ResY);	}
				else
				{	FactorGrid.m_PosY = m_StagePosY-((FactorGrid.m_ImgY-ImageHHalf)*ResY);	}				
				FactorGrid.m_PosZ = m_StagePosZ;
				FactorGrid.m_Height = TargetHeightValue;				
				m_PhaseFactorList.push_back(FactorGrid);
			}
		}		
	}
	//this->ClearLogListBox();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdatePhaseNoiseParamToUI()//更新相位雜訊定義	
{
	CString str;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int NoiseDefine = SysParam.m_PhaseNoiseDefine;

	JetAPI::SetComboxCurSel(m_NoiseDefineModeCombox, SysParam.m_PhaseNoiseDefineMode);
	if ( (NoiseDefine&PHASE_NOSIE_VOID_EXPAND) == 0 )
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_VOID_EXPAND_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_VOID_EXPAND_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_LOW_CONTRAST) == 0 )
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_LOW_CONTRAST_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_LOW_CONTRAST_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_LOW_POTENTIAL) == 0 )
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_LOW_POTENTIAL_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_LOW_POTENTIAL_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_OVER_SATURATED) == 0 )
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_OVER_SATURATED_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_OVER_SATURATED_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_SMOOTH_FILTER) == 0 )
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_SMOOTH_FILTER_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(CALIALIGN_NOISE_SMOOTH_FILTER_CHK, TRUE);	}

	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT, SysParam.m_PhaseNoiseLowContrastA);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT, SysParam.m_PhaseNoiseLowPotentialA);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT, SysParam.m_PhaseNoiseOverSaturatedA);

	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT2, SysParam.m_PhaseNoiseLowContrastB);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT2, SysParam.m_PhaseNoiseLowPotentialB);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT2, SysParam.m_PhaseNoiseOverSaturatedB);

	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT, SysParam.m_PhaseNoiseLowContrastC);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT, SysParam.m_PhaseNoiseLowPotentialC);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT, SysParam.m_PhaseNoiseOverSaturatedC);

	//m_SpaceNoiseMultiCastPatchSize = 0;//高度雜訊多投光合併Patch尺寸
	JetAPI::SetComboxCurSel(m_SpaceMergeModeCombox, SysParam.m_SpaceNoiseMultiCastMergeMode);
	JetAPI::SetComboxCurSel(m_SpaceMergeBestModeCombox, SysParam.m_SpaceNoiseMultiCastMergeBestMode);
	JetAPI::SetComboxCurSel(m_SpaceMergeIntensityModeCombox, SysParam.m_SpaceNoiseMultiIntensityMergeMode);	
	CWnd::SetDlgItemInt(CALIALIGN_MULTI_CAST_PATCH_SIZE_EDIT, SysParam.m_SpaceNoiseMultiCastPatchSize);
	CWnd::SetDlgItemInt(CALIALIGN_MULTI_CAST_MIN_VALID_COUNT_EDIT, SysParam.m_SpaceNoiseMultiCastMinValidCount);
	str.Format(_T("%.0f"), SysParam.m_SpaceNoiseMultiCastMaxDifference);
	CWnd::SetDlgItemText(CALIALIGN_MULTI_CAST_MAX_DIFF_EDIT, str);
	str.Format(_T("%.0f"), SysParam.m_SpaceNoiseMultiCastLimitDifference);
	CWnd::SetDlgItemText(CALIALIGN_MULTI_CAST_LIMIT_DIFF_EDIT, str);	
	//m_SpaceNoiseMultiCastValidBestRatio;//高度雜訊多投光高度最好比例-um	
	str.Format(_T("%.0f"), SysParam.m_SpaceNoiseMultiCastValidDifference);
	CWnd::SetDlgItemText(CALIALIGN_MULTI_CAST_VALID_DIFF_EDIT, str);	
	//m_SpaceNoiseMultiCastOppositeMaxGray;//高度雜訊多投光合併對邊灰階上限-gray

	JetAPI::SetComboxCurSel(m_CastSpaceFilterModeCombox, SysParam.m_SpaceNoiseCastFilterMode);
	str.Format(_T("%d"), SysParam.m_SpaceNoiseCastMedianFilterSize);
	CWnd::SetDlgItemText(CALIALIGN_CAST_MEDIAN_FILTER_SIZE_EDIT, str);	
	str.Format(_T("%d"), SysParam.m_SpaceNoiseCastMedianFilterUseSize);
	CWnd::SetDlgItemText(CALIALIGN_CAST_MEDIAN_FILTER_USE_SIZE_EDIT, str);

	str.Format(_T("%.0f"), SysParam.m_SpaceNoiseSingleCastLowLimit);
	CWnd::SetDlgItemText(CALIALIGN_SINGLE_CAST_OVER_LOW_EDIT, str);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_VOID_EXPAND_EDIT, SysParam.m_PhaseNoiseExtendVoid);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_SMOOTH_FILTER_EDIT, SysParam.m_PhaseNoiseSmoothFilter);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdatePhaseNoiseParamFromUI()//更新相位雜訊定義	
{
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	int PhaseNoiseDef = 0;	
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_LOW_CONTRAST_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_CONTRAST; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_LOW_POTENTIAL_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_POTENTIAL; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_OVER_SATURATED_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_OVER_SATURATED; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_VOID_EXPAND_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_VOID_EXPAND; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_SMOOTH_FILTER_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_SMOOTH_FILTER; }	

	SysParam.m_PhaseNoiseDefine = PhaseNoiseDef;	
	SysParam.m_PhaseNoiseDefineMode = JetAPI::GetComboxCurSelData(m_NoiseDefineModeCombox);

	SysParam.m_PhaseNoiseLowContrastA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT);	
	SysParam.m_PhaseNoiseLowPotentialA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT);
	SysParam.m_PhaseNoiseOverSaturatedA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT);

	SysParam.m_PhaseNoiseLowContrastB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT2);	
	SysParam.m_PhaseNoiseLowPotentialB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT2);
	SysParam.m_PhaseNoiseOverSaturatedB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT2);

	SysParam.m_PhaseNoiseLowContrastC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT);	
	SysParam.m_PhaseNoiseLowPotentialC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT);
	SysParam.m_PhaseNoiseOverSaturatedC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT);

	SysParam.m_SpaceNoiseSingleCastLowLimit = (int)(CWnd::GetDlgItemInt(CALIALIGN_SINGLE_CAST_OVER_LOW_EDIT));	
	SysParam.m_SpaceNoiseMultiCastMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeModeCombox);
	SysParam.m_SpaceNoiseMultiCastMergeBestMode = JetAPI::GetComboxCurSelData(m_SpaceMergeBestModeCombox);	
	SysParam.m_SpaceNoiseMultiIntensityMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeIntensityModeCombox);		
	SysParam.m_SpaceNoiseMultiCastPatchSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_PATCH_SIZE_EDIT));
	SysParam.m_SpaceNoiseMultiCastMinValidCount = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_MIN_VALID_COUNT_EDIT));
	SysParam.m_SpaceNoiseMultiCastMaxDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_MAX_DIFF_EDIT));
	SysParam.m_SpaceNoiseMultiCastLimitDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_LIMIT_DIFF_EDIT));	
	//m_SpaceNoiseMultiCastValidBestRatio;//高度雜訊多投光高度最好比例-um	
	SysParam.m_SpaceNoiseMultiCastValidDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_VALID_DIFF_EDIT));
	//m_SpaceNoiseMultiCastOppositeMaxGray;//高度雜訊多投光合併對邊灰階上限-gray

	SysParam.m_SpaceNoiseCastFilterMode = JetAPI::GetComboxCurSelData(m_CastSpaceFilterModeCombox);
	SysParam.m_SpaceNoiseCastMedianFilterSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_CAST_MEDIAN_FILTER_SIZE_EDIT));	
	SysParam.m_SpaceNoiseCastMedianFilterUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_CAST_MEDIAN_FILTER_USE_SIZE_EDIT));		

	SysParam.m_PhaseNoiseExtendVoid = CWnd::GetDlgItemInt(CALIALIGN_NOISE_VOID_EXPAND_EDIT);
	SysParam.m_PhaseNoiseSmoothFilter = CWnd::GetDlgItemInt(CALIALIGN_NOISE_SMOOTH_FILTER_EDIT);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalibrateNext3DCastID(CALIBRATION_MODE Mode)//校正下一個3D投光
{
	const int Count = m_3DCastIDCombox.GetCount()-1;
	const int CurSel = m_3DCastIDCombox.GetCurSel();
	const int NextSel = CurSel+1;		
	m_3DCastIDCombox.SetCurSel(NextSel);
	Update3DCastCurrentToUI();
	if ( NextSel >= Count )
	{	return false; }

	LIGHT_3D_CAST_ID CastID=(LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(m_3DCastIDCombox);			
	SetLight3DCastID(CastID);
	if ( ConfigGrabParam(Mode) == false )
	{	return false; }	
	SetCalibrationMode(Mode);		
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetFirstCali3DCastID(LIGHT_3D_CAST_ID &CastID)//取得第1個校正的3D投光
{	
	CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox));
	if ( LIGHT_3D_CAST_00 != CastID )
	{	
		SetCalibrateAllCastID(false);
		SetLight3DCastID(CastID);				
		return true;
	}
	SetCalibrateAllCastID(true);
	CastID = LIGHT_3D_CAST_01;
	SetLight3DCastID(CastID);
	CWnd::CheckDlgButton(CALIALIGN_GRAB_REPEAT_CHK, FALSE);
	JetAPI::SetComboxCurSel(m_PhaseIDCombox, BATCH_GRAB_PHASE_M);		
	JetAPI::SetComboxCurSel(m_3DCastIDCombox, CastID);
	Update3DCastCurrentToUIKernel(CastID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetPhaseNoiseParam(TPhaseNoiseParam &NoiseParam)//更新雜訊定義
{	
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);

	NoiseParam.PhaseNoiseDef = 0;//AOIDataCollect.GetSystemParameter().m_PhaseNoiseDefine;	
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_LOW_CONTRAST_CHK) == TRUE )
	{	NoiseParam.PhaseNoiseDef |= PHASE_NOSIE_LOW_CONTRAST; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_LOW_POTENTIAL_CHK) == TRUE )
	{	NoiseParam.PhaseNoiseDef |= PHASE_NOSIE_LOW_POTENTIAL; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_OVER_SATURATED_CHK) == TRUE )
	{	NoiseParam.PhaseNoiseDef |= PHASE_NOSIE_OVER_SATURATED; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_VOID_EXPAND_CHK) == TRUE )
	{	NoiseParam.PhaseNoiseDef |= PHASE_NOSIE_VOID_EXPAND; }
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_SMOOTH_FILTER_CHK) == TRUE )
	{	NoiseParam.PhaseNoiseDef |= PHASE_NOSIE_SMOOTH_FILTER; }	

	NoiseParam.PhaseNoiseDefMode = JetAPI::GetComboxCurSelData(m_NoiseDefineModeCombox);	
	NoiseParam.PhaseLowContrastA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT);
	NoiseParam.PhaseLowPotentialA = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT);

	NoiseParam.PhaseLowContrastB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT2);
	NoiseParam.PhaseOverSaturatedB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT2);
	NoiseParam.PhaseLowPotentialB = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT2);

	NoiseParam.PhaseLowContrastC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_OVER_SATURATED_EDIT);
	NoiseParam.PhaseLowPotentialC = CWnd::GetDlgItemInt(CALIALIGN_NOISE_LOW_POTENTIAL_EDIT);
	
	//SpaceMultiCastMinValidCount;//多投光最少多少有效值
	//m_SpaceNoiseMultiCastMinValidCount;//高度雜訊多投光最少有效值數		
	NoiseParam.SpaceSingleCastLowLimit = (int)(CWnd::GetDlgItemInt(CALIALIGN_SINGLE_CAST_OVER_LOW_EDIT));
	NoiseParam.SpaceMultiCastMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeModeCombox);
	NoiseParam.SpaceMultiCastMergeBestMode = JetAPI::GetComboxCurSelData(m_SpaceMergeBestModeCombox);
	NoiseParam.SpaceMultiIntensityMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeIntensityModeCombox);	
	NoiseParam.SpaceMultiCastPatchSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_PATCH_SIZE_EDIT));
	NoiseParam.SpaceMultiCastMinValidCount = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_MIN_VALID_COUNT_EDIT));
	NoiseParam.SpaceMultiCastMaxDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_MAX_DIFF_EDIT));
	NoiseParam.SpaceMultiCastLimitDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_LIMIT_DIFF_EDIT));
	//SpaceMultiCastValidBestRatio;//多投光最好高度比例um-不做濾波
	NoiseParam.SpaceMultiCastValidDifference = (int)(CWnd::GetDlgItemInt(CALIALIGN_MULTI_CAST_VALID_DIFF_EDIT));	

	NoiseParam.SpaceCastFilterMode = JetAPI::GetComboxCurSelData(m_CastSpaceFilterModeCombox);
	NoiseParam.SpaceCastMedianFilterSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_CAST_MEDIAN_FILTER_SIZE_EDIT));	
	NoiseParam.SpaceCastMedianFilterUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_CAST_MEDIAN_FILTER_USE_SIZE_EDIT));

	NoiseParam.PhaseExtendVoid = CWnd::GetDlgItemInt(CALIALIGN_NOISE_VOID_EXPAND_EDIT);
	NoiseParam.PhaseSmoothFilter = CWnd::GetDlgItemInt(CALIALIGN_NOISE_SMOOTH_FILTER_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
SLICE_FUNC_MODE CCaliPaneAlign::CheckSliceFuncModeByBatchGrabPhaseMode(SLICE_FUNC_MODE RefSlicFuncMode, int BatchGrabPhaseMode) const//依據SliceFuncMode與相位模式來取得對應的SliceFuncMode
{
	SLICE_FUNC_MODE SliceFuncMode=RefSlicFuncMode;
	switch ( BatchGrabPhaseMode )
	{
	case BATCH_GRAB_PHASE_1:
		break;
	case BATCH_GRAB_PHASE_2:
		break;
	case BATCH_GRAB_PHASE_M:
		switch ( RefSlicFuncMode )
		{
		case SLICE_FUNC_3D_2STEP_2STEP_1EXP:			
			SliceFuncMode= SLICE_FUNC_3D_2STEP_2STEP_1EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_2STEP_1EXP:
		case SLICE_FUNC_3D_4STEP_2STEP_2EXP:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_2STEP_1EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_4GC_1EXP:
		case SLICE_FUNC_3D_4STEP_4GC_2EXP:
		case SLICE_FUNC_3D_4STEP_4GC_2LIGHT:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4GC_1EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_5GC_1EXP:
		case SLICE_FUNC_3D_4STEP_5GC_2EXP:
		case SLICE_FUNC_3D_4STEP_5GC_2LIGHT:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_5GC_1EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_6GC_1EXP:
		case SLICE_FUNC_3D_4STEP_6GC_2EXP:
		case SLICE_FUNC_3D_4STEP_6GC_2LIGHT:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_6GC_1EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_4STEP_2EXP:
		case SLICE_FUNC_3D_4STEP_4STEP_2LIGHT:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4STEP_1EXP;			
			break;
		default:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4STEP_1EXP;
			break;
		}
		break;
	case BATCH_GRAB_PHASE_M2:
		switch ( RefSlicFuncMode )
		{
		case SLICE_FUNC_3D_2STEP_2STEP_1EXP:			
			SliceFuncMode= SLICE_FUNC_3D_2STEP_2STEP_1EXP;
			break;
		case SLICE_FUNC_3D_4STEP_2STEP_1EXP:
		case SLICE_FUNC_3D_4STEP_2STEP_2EXP:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_2STEP_2EXP;			
			break;		
		case SLICE_FUNC_3D_4STEP_4GC_2EXP:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4GC_2EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_4GC_1EXP:
		case SLICE_FUNC_3D_4STEP_4GC_2LIGHT:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4GC_2LIGHT;			
			break;
		case SLICE_FUNC_3D_4STEP_5GC_2EXP:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_5GC_2EXP;
			break;
		case SLICE_FUNC_3D_4STEP_5GC_1EXP:
		case SLICE_FUNC_3D_4STEP_5GC_2LIGHT:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_5GC_2LIGHT;			
			break;
		case SLICE_FUNC_3D_4STEP_6GC_2EXP:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_6GC_2EXP;
			break;
		case SLICE_FUNC_3D_4STEP_6GC_1EXP:
		case SLICE_FUNC_3D_4STEP_6GC_2LIGHT:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_6GC_2LIGHT;
			break;		
		case SLICE_FUNC_3D_4STEP_4STEP_2EXP:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4STEP_2EXP;			
			break;
		case SLICE_FUNC_3D_4STEP_4STEP_1EXP:
		case SLICE_FUNC_3D_4STEP_4STEP_2LIGHT:			
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4STEP_2LIGHT;			
			break;
		default:
			SliceFuncMode= SLICE_FUNC_3D_4STEP_4STEP_2EXP;
			break;
		}
		break;
	}
	return SliceFuncMode;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdateSpaceNoiseFilterParamToUI()//更新雜訊過濾參數
{
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	//無效點外擴
	
	CWnd::CheckDlgButton(CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK, SysParam.m_SpaceNoiseDataVoidExpandEnabed);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_EXPAND_EDIT, SysParam.m_SpaceNoiseDataVoidExpandSize);	

	//初次處理
	JetAPI::SetComboxCurSel(m_FirstFilterModeCombox, SysParam.m_SpaceNoiseFirstFilterMode);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_PITCH_EDIT, SysParam.m_SpaceNoiseFirstFilterPitch);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_SIZE_EDIT, SysParam.m_SpaceNoiseFirstKerSize);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FIRST_USE_SIZE_EDIT, SysParam.m_SpaceNoiseFirstUseSize);

	//高度過低判定
	JetAPI::SetComboxCurSel(m_OverLowModeCombox, SysParam.m_SpaceNoiseOverLowerMode);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_EDIT, SysParam.m_SpaceNoiseOverLowerRange);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_LIMIT_EDIT, SysParam.m_SpaceNoiseOverLowerLimit);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_KER_SIZE_EDIT, SysParam.m_SpaceNoiseOverLowerKerSize);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_USE_SIZE_EDIT, SysParam.m_SpaceNoiseOverLowerUseSize);	
	//高度異常判定
	JetAPI::SetComboxCurSel(m_HeightVarModeCombox, SysParam.m_SpaceNoiseHeightAbnormalMode);		
	//m_SpaceNoiseHeightAbnormalPitch;//高度雜訊高度異常步長
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_EDIT, SysParam.m_SpaceNoiseHeightAbnormalRange);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_PITCH_EDIT, SysParam.m_SpaceNoiseHeightAbnormalPitch);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT, SysParam.m_SpaceNoiseHeightAbnormalChkSize);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_KER_SIZE_EDIT, SysParam.m_SpaceNoiseHeightAbnormalKerSize);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_USE_SIZE_EDIT, SysParam.m_SpaceNoiseHeightAbnormalUseSize);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT, SysParam.m_SpaceNoiseHeightAbnormalRepeatCnt);
	
	//雜訊重建
	CWnd::CheckDlgButton(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK, SysParam.m_SpaceNoiseDataVoidReContructedEnabled);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_EDIT, SysParam.m_SpaceNoiseDataVoidReContructedExtSize);
	//最末處理
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox, SysParam.m_SpaceNoiseFinalFilterMode);					
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT, SysParam.m_SpaceNoiseFinalPitch);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT, SysParam.m_SpaceNoiseFinalKerSize);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT, SysParam.m_SpaceNoiseFinalUseSize);

	//最末處理-2
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox2, SysParam.m_SpaceNoiseFinalFilterMode2);	
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT2, SysParam.m_SpaceNoiseFinalPitch2);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT2, SysParam.m_SpaceNoiseFinalKerSize2);
	CWnd::SetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT2, SysParam.m_SpaceNoiseFinalUseSize2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdateSpaceNoiseFilterParamFromUI()//更新雜訊過濾參數
{
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	//無效點外擴
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK) == TRUE )
	{	SysParam.m_SpaceNoiseDataVoidExpandEnabed = FN_ENABLE; }
	else
	{	SysParam.m_SpaceNoiseDataVoidExpandEnabed = FN_DISABLE; }	
	SysParam.m_SpaceNoiseDataVoidExpandSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_EXPAND_EDIT);	

	//後製平滑處理		
	SysParam.m_SpaceNoiseFirstFilterMode = JetAPI::GetComboxCurSelData(m_FirstFilterModeCombox);
	SysParam.m_SpaceNoiseFirstFilterPitch = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_PITCH_EDIT);
	SysParam.m_SpaceNoiseFirstKerSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_SIZE_EDIT);
	SysParam.m_SpaceNoiseFirstUseSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_USE_SIZE_EDIT);

	//高度過低判定	
	SysParam.m_SpaceNoiseOverLowerMode = JetAPI::GetComboxCurSelData(m_OverLowModeCombox);
	SysParam.m_SpaceNoiseOverLowerRange = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_EDIT));
	SysParam.m_SpaceNoiseOverLowerLimit = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_LIMIT_EDIT));	
	SysParam.m_SpaceNoiseOverLowerKerSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_KER_SIZE_EDIT));	
	SysParam.m_SpaceNoiseOverLowerUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_USE_SIZE_EDIT));		

	//高度異常濾除	
	SysParam.m_SpaceNoiseHeightAbnormalMode = JetAPI::GetComboxCurSelData(m_HeightVarModeCombox);	
	SysParam.m_SpaceNoiseHeightAbnormalPitch = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_PITCH_EDIT));
	SysParam.m_SpaceNoiseHeightAbnormalRange = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_EDIT));
	SysParam.m_SpaceNoiseHeightAbnormalChkSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT));
	SysParam.m_SpaceNoiseHeightAbnormalKerSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_KER_SIZE_EDIT));
	SysParam.m_SpaceNoiseHeightAbnormalUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_USE_SIZE_EDIT));
	SysParam.m_SpaceNoiseHeightAbnormalRepeatCnt = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT));	
	
	//雜訊重建
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK) == TRUE )
	{	SysParam.m_SpaceNoiseDataVoidReContructedEnabled = FN_ENABLE; }
	else
	{	SysParam.m_SpaceNoiseDataVoidReContructedEnabled = FN_DISABLE; }	
	SysParam.m_SpaceNoiseDataVoidReContructedExtSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_EDIT);

	//後製平滑處理		
	SysParam.m_SpaceNoiseFinalFilterMode = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox);		
	SysParam.m_SpaceNoiseFinalPitch = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT);
	SysParam.m_SpaceNoiseFinalKerSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT);
	SysParam.m_SpaceNoiseFinalUseSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT);

	//後製平滑處理-2
	SysParam.m_SpaceNoiseFinalFilterMode2 = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox2);	
	SysParam.m_SpaceNoiseFinalPitch2 = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_PITCH_EDIT2);
	SysParam.m_SpaceNoiseFinalKerSize2 = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT2);
	SysParam.m_SpaceNoiseFinalUseSize2 = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetSpaceNoiseFilterParam(TNoiseFilterParam &FilterParam)//更新雜訊過濾參數
{	
	AOIDataCollect.GetSpaceNoiseFilterParam(FilterParam);	
	//無效點外擴
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_FILTER_VOID_EXPAND_CHK) == TRUE )
	{	FilterParam.DataVoidExpandEnabled = true; }
	else
	{	FilterParam.DataVoidExpandEnabled = false; }	
	FilterParam.DataVoidExpandSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_EXPAND_EDIT);
	FilterParam.DataVoidExpandIterCount = 1;

	//首次處理		
	FilterParam.DataFirstFilterMode = JetAPI::GetComboxCurSelData(m_FirstFilterModeCombox);
	FilterParam.DataFirstFilterPitch = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_PITCH_EDIT);
	FilterParam.DataFirstFilterKerSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_FILTER_SIZE_EDIT);
	FilterParam.DataFirstFilterUseSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FIRST_USE_SIZE_EDIT);

	//高度過低判定	
	FilterParam.DataOverLowFTMode = JetAPI::GetComboxCurSelData(m_OverLowModeCombox);
	FilterParam.DataOverLowFTRange = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_EDIT));
	FilterParam.DataOverLowFTLimit = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_LIMIT_EDIT));	
	FilterParam.DataOverLowFTKerSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_KER_SIZE_EDIT));
	FilterParam.DataOverLowFTUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_OVER_LOW_USE_SIZE_EDIT));		

	//高度異常判定	
	FilterParam.DataHeightFTMode = JetAPI::GetComboxCurSelData(m_HeightVarModeCombox);
	FilterParam.DataHeightFTRange = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_EDIT));
	FilterParam.DataHeightFTPitch = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_PITCH_EDIT));
	FilterParam.DataHeightFTChkSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT));	
	FilterParam.DataHeightFTKerSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_KER_SIZE_EDIT));
	FilterParam.DataHeightFTUseSize = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_USE_SIZE_EDIT));
	FilterParam.DataHeightFTRepeatCnt = (int)(CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT));	

	//雜訊重建
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_CHK) == TRUE )
	{	FilterParam.DataVoidReContructed = true; }
	else
	{	FilterParam.DataVoidReContructed = false; }	
	FilterParam.DataVoidReContructedExtSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FILTER_VOID_RECONTRUCT_EDIT);

	//最末處理		
	FilterParam.DataFinalFilterMode = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox);
	FilterParam.DataFinalFilterKerSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT);
	FilterParam.DataFinalFilterUseSize = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT);	
	//DataFinalFilterPitch = 4;//平滑滑移步進

	//最末處理-2
	FilterParam.DataFinalFilterMode2 = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox2);
	FilterParam.DataFinalFilterKerSize2 = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_FILTER_SIZE_EDIT2);
	FilterParam.DataFinalFilterUseSize2 = CWnd::GetDlgItemInt(CALIALIGN_NOISE_FINAL_USE_SIZE_EDIT2);	
	//DataFinalFilterPitch2;//平滑滑移步進

	FilterParam.DataCorrectMode = m_NoiseFilterParam.DataCorrectMode;
	FilterParam.BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;
	m_NoiseFilterParam = FilterParam;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::SaveAutoFocusReading(const std::vector<TPOINT2D> &ReadingList)
{
	const size_t Count=ReadingList.size();
	if ( 0 == Count ) { return true; }

	FILE *pfile=NULL;	
	CString Filename;
	Filename.Format(_T("%s\\%s.TXT"), AOIDataCollect.GetAOITempDirectory(), _T("AutoFocus"));		
	pfile = ::_tfopen(Filename, _T("w+"));
	if ( NULL == pfile ) { return false; }
	::_ftprintf(pfile, _T("Reading, ZPos\n"));
	for ( size_t i=0; i<Count; i++ )
	{
		const TPOINT2D &Reading=ReadingList[i];
		if ( fabs(Reading.x) < 0.00001 )
		{	::_ftprintf(pfile, _T("\n"));	}
		else
		{	::_ftprintf(pfile, _T("%.2f, %.2f\n"), Reading.x, Reading.y);		}		
	}		
	::fclose(pfile); pfile=NULL;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcBest2DCurrent(const std::vector<T2D_CURRENT> &ReadingList, double TargetGray, T2D_CURRENT &Best) const
{
	Best = T2D_CURRENT();
	double MinErr=DBL_MAX;
	const size_t Count=ReadingList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		const T2D_CURRENT &ReadingRef=ReadingList[i];
		double Err=::fabs(ReadingRef.Gray-TargetGray);
		if ( Err < MinErr )
		{	
			MinErr = Err;
			Best = ReadingRef;
		}
	}
	if ( DBL_MAX == MinErr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Save2DCurrentReading(const TSliceParam &SliceParam, CALIBRATION_MODE CalMode, const std::vector<T2D_CURRENT> &ReadingList)
{
	const size_t Count=ReadingList.size();
	if ( 0 == Count ) { return true; }

	FILE *pfile=NULL;
	CString Filename;
	CString strFolder;
	CString SliceName=SliceParam.SliceName; 
	const double GrayGain=SliceParam.SliceGainValue;
	strFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("2DCurrent"));
	Filename.Format(_T("%s\\%s.TXT"), strFolder, SliceName);

	::CreateDirectory(strFolder, NULL);
	pfile = ::_tfopen(Filename, _T("w+"));
	if ( NULL == pfile ) { return false; }

	::_ftprintf(pfile, _T("Current, Reading, Target, Gain\n"));
	for ( size_t i=0; i<Count; i++ )
	{
		const T2D_CURRENT &Reading=ReadingList[i];
		::_ftprintf(pfile, _T("%d, %.2f, %d, %.2f\n"), Reading.Current, Reading.Gray, SliceParam.SliceTargetGray, Reading.Gain);	
	}
	::fclose(pfile); pfile=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcPhaseNormLine()//計算相位法向量
{
	double a1=0, b1=0;
	double a2=0, b2=0;
	double Thita1 = 0;
	double Thita2 = 0;
	const double len = 1200;
	TPOINT2D CP;	
	CP.x = (m_PhaseLinePt1.x+m_PhaseLinePt2.x)/2;
	CP.y = (m_PhaseLinePt1.y+m_PhaseLinePt2.y)/2;
	if ( fabs(m_PhaseLinePt2.x-m_PhaseLinePt1.x) < 0.001 )//垂直線
	{
		Thita1 = 90.0;
		Thita2 = 00.0;
		m_PhaseNormPt1.x = CP.x + len;
		m_PhaseNormPt1.y = CP.y;
		m_PhaseNormPt2.x = CP.x - len;
		m_PhaseNormPt2.y = CP.y;		
	}	
	else if ( fabs(m_PhaseLinePt2.y-m_PhaseLinePt1.y) < 0.001 )//水平線
	{		
		Thita1 = 00.0;
		Thita2 = 90.0;

		m_PhaseNormPt1.x = CP.x;
		m_PhaseNormPt1.y = CP.y + len;
		m_PhaseNormPt2.x = CP.x;
		m_PhaseNormPt2.y = CP.y - len;		
	}
	else
	{
		//y = ax + b
		// a1 = (y2-y1)/(x2-x1);
		// b1 = y-ax
		a1 = (m_PhaseLinePt2.y-m_PhaseLinePt1.y)/(m_PhaseLinePt2.x-m_PhaseLinePt1.x);
		b1 = m_PhaseLinePt1.y - (a1*m_PhaseLinePt1.x);
		Thita1 = ::atan(a1);
		Thita1 = Thita1*RAD_TO_DEG_DBL;

		//a2 = -1xa1
		a2 = -1/a1;
		b2 = CP.y - (a2*CP.x);

		Thita2 = ::atan(a2);
		const double dx = len*cos(Thita2);
		Thita2 = Thita2*RAD_TO_DEG_DBL;

		m_PhaseNormPt1.x = CP.x + dx;
		m_PhaseNormPt1.y = (m_PhaseNormPt1.x*a2)+b2;
		m_PhaseNormPt2.x = CP.x - dx;
		m_PhaseNormPt2.y = (m_PhaseNormPt2.x*a2)+b2;
	}

	this->m_PhaseCount = 0;
	this->m_PhaseRange = 0;
	this->m_PhaseHeight = 0.0;
	this->m_PhaseAngle = Thita1;//相位角度
	this->m_PhasePtList.clear();	

	CString str;
	FILE *pfile = NULL;
	size_t i=0, j=0;
	POINT ImagePt1={0}, ImagePt2={0};
	TPIXEL_GRY              GryPixel;
	TPIXEL_GRY             *GryPixelPtr = NULL;
	std::vector<TPIXEL_GRY> ProfileGry1;	
	std::vector<TPIXEL_GRY> ProfileGry2;	
	std::vector<TPIXEL_GRY> ProfileGry3;	

	JetAPI::Point2DToPoint(m_PhaseNormPt1, ImagePt1);
	JetAPI::Point2DToPoint(m_PhaseNormPt2, ImagePt2);
	ImageAPI.SobelImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, m_ImageBuffer1); 
	ImageAPI.CalcImageProfile(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer1, ImagePt1, ImagePt2, ProfileGry1);
	
	CWnd::GetDlgItemText(CALIALIGN_3DCAST_MOUNT_ANGLE_EDIT, str);
	const double MountAngleDeg = ::_tcstod(str, NULL);
	const double MountAngleRad = MountAngleDeg*DEG_TO_RAD_DBL;
	const int Range = this->GetDlgItemInt(CALIALIGN_PATTERN_PHASE_FILTER_COUNT_EDIT); 
	const int HightLevel = this->GetDlgItemInt(CALIALIGN_PATTERN_PHASE_HIGHT_LEVEL_EDIT); ;
	const size_t ProfileSize1 = ProfileGry1.size();		
	for ( i=0; i<ProfileSize1; i++ )
	{
		if ( ProfileGry1[i].gray < HightLevel ) { continue; }
		ProfileGry2.push_back(ProfileGry1[i]);
	}	

	const size_t ProfileSize2 = ProfileGry2.size();		
	if ( ProfileSize2 > 1 )
	{
		int dx=0, dy=0, dl=0, count=0;
		count=0;		
		GryPixel=TPIXEL_GRY();
		for ( i=0; i<ProfileSize2-1; i++ )
		{
			dx = ProfileGry2[i+1].x-ProfileGry2[i].x;
			dy = ProfileGry2[i+1].y-ProfileGry2[i].y;
			if ( dx < 0 ) { dx = -dx; }
			if ( dy < 0 ) { dy = -dy; }
			dl = (dx+dy)>>1;
			if ( dl < Range )
			{
				GryPixel.x += ProfileGry2[i].x;
				GryPixel.y += ProfileGry2[i].y;
				GryPixel.gray += ProfileGry2[i].gray;
				count ++;
				continue;
			}
			if ( count == 0 ) { continue; }
			GryPixel.x    /= count;
			GryPixel.y    /= count;
			GryPixel.gray /= count;
			ProfileGry3.push_back(GryPixel);
			count = 0;
			GryPixel=TPIXEL_GRY();			
		}
		if ( count > 0 )
		{
			GryPixel.x    /= count;
			GryPixel.y    /= count;
			GryPixel.gray /= count;
			ProfileGry3.push_back(GryPixel);
		}
	}
	const size_t ProfileSize3 = ProfileGry3.size();		
	if ( ProfileSize3 > 1 )
	{
		m_PhaseCount=0;
		m_PhaseRange=0;
		m_PhaseHeight = 0;
		double dx=0, dy=0, dl=0;
		for ( i=0; i<ProfileSize3-1; i++ )
		{
			dx = ProfileGry3[i+1].x-ProfileGry3[i].x;
			dy = ProfileGry3[i+1].y-ProfileGry3[i].y;
			dx *= dx;
			dy *= dy;
			dl = sqrt(dx+dy);

			m_PhaseRange += dl;
			m_PhaseCount ++;			
		}
		if ( m_PhaseCount > 0 ) 
		{	m_PhaseRange /= m_PhaseCount;	}
	}
	m_PhasePtList = ProfileGry3;

	//m_PhaseHeight
	double Res = 0.0, ResX = 0.0, ResY = 0.0;	
	CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(m_CameraCombox));	
	const size_t ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const size_t ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	const double tanAngle = ::tan(MountAngleRad);
	const double FOVW = AOIDataCollect.GetFovSizeRealW();
	const double FOVH = AOIDataCollect.GetFovSizeRealH();
	if ( ImageW > 0 ) 
	{	ResX = FOVW/ImageW;	}
	if ( ImageH > 0 ) 
	{	ResY = FOVH/ImageH;	}
	Res = (ResX+ResY)*0.5;
	this->m_PhaseHeight = m_PhaseRange*Res/tanAngle;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::SetFovSize(double FOVW, double FOVH)//設定視野大小
{
	CString strW, strH;
	const double ShowScale=AOIDataCollect.GetSystemParameter().m_ResolutionShowScale;
	const double FovW=FOVW*ShowScale/100.0;
	const double FovH=FOVH*ShowScale/100.0;
	strW.Format(_T("%.0f"), FovW);
	strH.Format(_T("%.0f"), FovH);
	CWnd::SetDlgItemText(CALIALIGN_FOV_WIDTH_EDIT, strW);
	CWnd::SetDlgItemText(CALIALIGN_FOV_HEIGHT_EDIT, strH);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetFovSize(double &FOVW, double &FOVH)//取得視野大小
{
	CString strW, strH;
	const double ShowScale=AOIDataCollect.GetSystemParameter().m_ResolutionShowScale;
	CWnd::GetDlgItemText(CALIALIGN_FOV_WIDTH_EDIT, strW);
	CWnd::GetDlgItemText(CALIALIGN_FOV_HEIGHT_EDIT, strH);
	const double FovW = ::_tcstod(strW, NULL);	
	const double FovH = ::_tcstod(strH, NULL);
	FOVW = FovW*100.0/ShowScale;
	FOVH = FovH*100.0/ShowScale;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcFOVSizeResolution()//計算FOV尺寸的解析度
{
	CString str;
	double FOVW = 0, FOVH = 0;
	double ResX = 0, ResY = 0;	
	CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(m_CameraCombox));	
	const double ShowScale=AOIDataCollect.GetSystemParameter().m_ResolutionShowScale;
	size_t ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	size_t ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);

	GetFovSize(FOVW, FOVH);	
	if ( ImageW > 0 ) 
	{	ResX = FOVW/ImageW;	}
	if ( ImageH > 0 ) 
	{	ResY = FOVH/ImageH;	}
	const double ShowResX=ResX*ShowScale/100.0;
	const double ShowResY=ResY*ShowScale/100.0;
	str.Format(_T("%.2f"), ShowResX);
	CWnd::SetDlgItemText(CALIALIGN_FOV_RESOLUTION_X_EDIT, str);
	str.Format(_T("%.2f"), ShowResY);
	CWnd::SetDlgItemText(CALIALIGN_FOV_RESOLUTION_Y_EDIT, str);	

	m_ImageToStageScaleX = ResX;//影像轉機台的比例-X
	m_ImageToStageScaleY = ResY;//影像轉機台的比例-Y	
	m_StageToImageScaleX = 1/ResX;//機台轉影像的比例-X
	m_StageToImageScaleY = 1/ResY;//機台轉影像的比例-Y
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ApplyFovResolutionToSystem()
{
	AOIDataCollect.ApplyCalibrationParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecGrabFirst()//執行第一次取像
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	if ( CheckNeedExtraDelayTime() == true )
	{	ExtraDelayTimeKernel(); }	

	const int DLPLEDCurrentID = GetDLPLEDCurrentID();
	CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraCombox));			
	this->m_CameraBatchGrabMode = m_LightNum+m_PatternStep+m_PhaseID+m_PatternProjectMode;//批次取像模式	

	size_t i=0;
	LIGHT_3D_CAST_ID CastID;		
	LIGHT_3D_CLS_PTR Light3DPtr = NULL;
	std::vector<LIGHT_3D_CAST_ID> CastIDList;	
	if ( false == m_Multi3DCastID )
	{	CastIDList.push_back(m_Light3DCastID);	}
	else
	{	Light3DCtrl.GetLight3DCastIDList(CastIDList); }

	const size_t CastIDCount = CastIDList.size();
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		Light3DPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL == Light3DPtr ) { continue; }		
		if ( Light3DPtr->ExecDLPLightSetting(DLPLEDCurrentID) == false )
		{
			JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());		
			return false;
		}
	}	
	std::vector<TSliceParam> ParamList;	
	ParamList.push_back(m_SliceParam);	
	if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	m_CameraID = CameraID;
	bool bFinish = false;		
	this->LockUIWnd(true);			
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	
	if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		this->LockUIWnd(false);
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecGrabNext()//執行下一次取像
{
	if ( CheckNeedWaitForMoveDone() == true )
	{
		if ( MotionCtrlPtr->WaitForMotionStop() == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		if (CheckNeedExtraDelayTime() == true)
		{	ExtraDelayTimeKernel();	}
	}	
	
	bool bFinish = false;
	m_CameraImageReceieveCount = 0;
	CAMERA_ID CameraID = m_CameraID;	
	DWORD BatchGrabMode = CameraCtrl.GetBatchGrabMode();
	BATCH_GRAB_STEP GrabStep = CameraCtrl.GetBatchGrabFirstStep(BatchGrabMode);
	CameraCtrl.ClearCameraCount(CameraID);
	CameraCtrl.ResetAllCameraRingBuffer();
	CameraCtrl.SetBatchGrabStep(GrabStep);	
	CameraCtrl.BatchIndexReset();
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	
	if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false);
		return false;
	}
	if ( false == bFinish )
	{	return true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CheckNeedWaitForMoveDone()
{
	bool bWait=true;
	CALIBRATION_MODE CaliMode=GetCalibrationMode();
	switch ( CaliMode )
	{
	case CALIBRATION_2D_LIGHT_ALIGN:
		bWait = false;
		break;
	case CALIBRATION_3D_CAST_ALIGN:
	case CALIBRATION_3D_CAST_FOCUS:
		bWait = false;
		break;		
	case CALIBRATION_2D_LIGHT_CURRENT:
	case CALIBRATION_3D_CAST_CURRENT:
	case CALIBRATION_3D_CAST_CURRENT_RED:
	case CALIBRATION_3D_CAST_CURRENT_GRN:
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		bWait = false;
		break;
	}
	return bWait;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	this->RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::BuildManipulateCombox(CComboBox &Combox)
{
	int     idx=0;
	CString str;
	CALIALIGN_MANIPULATE_MODE ManipulateMode = CALIALIGN_MANIPULATE_NONE;
	JetAPI::ClearCombox(Combox);
	idx = 0;

	str = _T("None");
	ManipulateMode = CALIALIGN_MANIPULATE_NONE;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ManipulateMode);
	idx ++;

	str = _T("Rect");
	ManipulateMode = CALIALIGN_MANIPULATE_RECT;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ManipulateMode);
	idx ++;

	str = _T("Grid");
	ManipulateMode = CALIALIGN_MANIPULATE_GRID;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ManipulateMode);
	idx ++;

#ifndef DISABLE_3D
	str = _T("Phase Line");
	ManipulateMode = CALIALIGN_MANIPULATE_PHASE_LINE;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ManipulateMode);
	idx ++;	

	str = _T("H-Factor");
	ManipulateMode = CALIALIGN_MANIPULATE_FACTOR_GRID;		
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, ManipulateMode);
	idx ++;
#endif//DISABLE_3D
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::BuildSaveRawExtNameCombox(CComboBox &Combox)
{
	int     idx=0;
	CString str;	
	JetAPI::ClearCombox(Combox);
	idx = 0;

	str = _T("BMP");	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, IMAGE_FILE_MODE_BMP);
	idx ++;

	str = _T("PNG");	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, IMAGE_FILE_MODE_PNG);
	idx ++;
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnGrabBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->ConfigGrabParam(CALIBRATION_STOP) == false )
	{	return; }
	SetCalibrationMode(CALIBRATION_STOP);	
	this->ExecGrabFirst();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnFOVWidthBtn() 
{
	// TODO: Add your control notification handler code here		
	if ( this->ConfigGrabParam(CALIBRATION_FOV_WIDTH_START) == false )
	{	return; }	
	SetModifiedCaliParam(true);
	SetCalibrationMode(CALIBRATION_FOV_WIDTH_START);	
	if ( this->RetrieveStagePosition() == false )
	{		
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnFOVHeightBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( this->ConfigGrabParam(CALIBRATION_FOV_HEIGHT_START) == false )
	{	return; }	
	SetCalibrationMode(CALIBRATION_FOV_HEIGHT_START);	
	if ( this->RetrieveStagePosition() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnCameraAlignHorBtn() 
{
	// TODO: Add your control notification handler code here		
	CALIBRATION_MODE CalibrationMode;
	BOOL bCheck3D = CWnd::IsDlgButtonChecked(CALIALIGN_CAMERA_ALIGN_3D_CHK);
	if ( FALSE == bCheck3D ) { CalibrationMode = CALIBRATION_CAMERA_ALIGN_HOR_START; }
	else { CalibrationMode = CALIBRATION_CAMERA_ALIGN_HOR_3D_START; }

	if ( ConfigGrabParam(CalibrationMode) == false )
	{	return; }
	SetCalibrationMode(CalibrationMode);	
	if ( this->RetrieveStagePosition() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnCameraAlignVerBtn() 
{
	// TODO: Add your control notification handler code here	
	CALIBRATION_MODE CalibrationMode;
	BOOL bCheck3D = CWnd::IsDlgButtonChecked(CALIALIGN_CAMERA_ALIGN_3D_CHK);
	if ( FALSE == bCheck3D ) { CalibrationMode = CALIBRATION_CAMERA_ALIGN_VER_START; }
	else { CalibrationMode = CALIBRATION_CAMERA_ALIGN_VER_3D_START; }

	if ( ConfigGrabParam(CalibrationMode) == false )
	{	return; }	
	SetCalibrationMode(CalibrationMode);	
	if ( this->RetrieveStagePosition() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnImageResolutionCalcBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( this->ConfigGrabParam(CALIBRATION_IMAGE_RESOLUTION) == false )
	{	return; }
	SetCalibrationMode(CALIBRATION_IMAGE_RESOLUTION);	
	if ( this->RetrieveStagePosition() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnImageFocusAutoBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("Do you want to Execute Auto-Focus ?"));
//	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
//	{	return; }

	//更新Pitch	
	//CALIBRATION_IMAGE_FOCUS_AUTO, CALIBRATION_IMAGE_FOCUS_AUTO_10, CALIBRATION_IMAGE_FOCUS_AUTO_100
	CALIBRATION_MODE CalibrationMode = CALIBRATION_IMAGE_FOCUS_AUTO_100;
	this->GetDlgItemText(CALIALIGN_IMAGE_FOCUS_AUTO_PITCH_EDIT, str);	
	this->m_AutoFocusPitch = ::_tcstod(str, NULL);		
	if ( this->m_AutoFocusPitch < 5 ) 
	{	m_AutoFocusPitch = 5; }
	this->m_AutoFocustBestStd = 0;
	this->m_AutoFocusReadingList.clear();
	this->m_AutoFocustBestPosZ = MotionCtrlPtr->GetMotionParameter().m_StageStartPosZ;	
	const double MinZ = MotionCtrlPtr->GetMotionParameter().m_LimitMinZ+m_AutoFocusPitch;
	const double MaxZ = MotionCtrlPtr->GetMotionParameter().m_LimitMaxZ-m_AutoFocusPitch;	
	switch ( CalibrationMode )
	{
	case CALIBRATION_IMAGE_FOCUS_AUTO_100:	this->m_AutoFocustScalePosZ = 100.0;	break;
	case CALIBRATION_IMAGE_FOCUS_AUTO_10:	this->m_AutoFocustScalePosZ = 10.0;	break;
	default:	this->m_AutoFocustScalePosZ = 1.0;	break;
	}
	
	this->m_AutoFocustMaxPosZ = MaxZ;
	this->m_AutoFocustMinPosZ = MinZ;
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return; }	

	SetCalibrationMode(CalibrationMode);	
	if ( this->RetrieveStagePosition() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	this->m_AutoFocustLastPosZ = -FLT_MAX;
	if ( MotionCtrlPtr->XYZMoveTo(m_StagePosX, m_StagePosY, MinZ) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	FocusToEditCtrl();
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	m_LBtnUpPos = point;
	m_LBtnDownPos = m_MovingPos = m_LBtnUpPos;	
	SetCapture();
	
	if ( PtInControlWnd(pt, CALIALIGN_IMAGE_WND, pt) == true )
	{	
		this->m_ImageWndPt1 = pt;
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);

		if ( TRUE == m_bShowHorizontalLine )
		{	this->m_ImageHorPt = m_ImagePt1;	}
		if ( TRUE == m_bShowVerticalLine )
		{	this->m_ImageVerPt = m_ImagePt1;	}

		CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);		
		switch ( ManipulateMode )
		{
		case CALIALIGN_MANIPULATE_PHASE_LINE:
			this->m_PhaseLinePt1 = m_ImagePt1;
			this->m_PhaseLinePt2 = m_ImagePt1;
			this->m_PhaseNormPt1 = m_PhaseLinePt1;
			this->m_PhaseNormPt2 = m_PhaseLinePt2;			
			break;
		case CALIALIGN_MANIPULATE_RECT:
			this->m_ImageRect4D.left = m_ImagePt1.x;
			this->m_ImageRect4D.top = m_ImagePt1.y;
			this->m_ImageRect4D.right = m_ImagePt1.x;
			this->m_ImageRect4D.bottom = m_ImagePt1.y;
			break;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	if ( PtInControlWnd(pt, CALIALIGN_IMAGE_WND, pt) == true )
	{	
		this->m_ImageWndPt2 = pt;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
		if ( TRUE == m_bShowHorizontalLine )
		{	this->m_ImageHorPt = m_ImagePt2;	}
		if ( TRUE == m_bShowVerticalLine )
		{	this->m_ImageVerPt = m_ImagePt2;	}

		CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);		
		switch ( ManipulateMode )
		{		
		case CALIALIGN_MANIPULATE_PHASE_LINE:				
			this->m_PhaseLinePt2 = m_ImagePt2;
			this->CalcPhaseNormLine();
			break;
		case CALIALIGN_MANIPULATE_RECT:
			m_ImageRect4D.left  = MIN(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRect4D.right = MAX(m_ImagePt1.x, m_ImagePt2.x);
			m_ImageRect4D.top  = MIN(m_ImagePt1.y, m_ImagePt2.y);
			m_ImageRect4D.bottom = MAX(m_ImagePt1.y, m_ImagePt2.y);			
			break;
		}		
	}

	m_LBtnUpPos = m_MovingPos = point;
	m_MovingPos.x = m_MovingPos.y = -1;
	m_LBtnUpPos = m_LBtnDownPos = m_MovingPos;
	RedrawWnd();

	CDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT WndPt = point;
	if ( PtInControlWnd(WndPt, CALIALIGN_IMAGE_WND, WndPt) == true )
	{	ShowPixelInfo(WndPt);	}
	if ( this != GetCapture() ) 
	{	
		CDialog::OnMouseMove(nFlags, point);
		return; 
	}	

	POINT pt  = point;
	if ( nFlags&MK_LBUTTON )
	{
		CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);
		if ( PtInControlWnd(pt, CALIALIGN_IMAGE_WND, pt) == true )
		{	
			this->m_ImageWndPt2 = (pt);				
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
			if ( TRUE == m_bShowHorizontalLine )
			{	this->m_ImageHorPt = m_ImagePt2;	}
			if ( TRUE == m_bShowVerticalLine )
			{	this->m_ImageVerPt = m_ImagePt2;	}

			switch ( ManipulateMode )
			{			
			case CALIALIGN_MANIPULATE_PHASE_LINE:				
				this->m_PhaseLinePt2 = m_ImagePt2;
				this->CalcPhaseNormLine();
				break;
			case CALIALIGN_MANIPULATE_RECT:
				m_ImageRect4D.left  = MIN(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRect4D.right = MAX(m_ImagePt1.x, m_ImagePt2.x);
				m_ImageRect4D.top  = MIN(m_ImagePt1.y, m_ImagePt2.y);
				m_ImageRect4D.bottom = MAX(m_ImagePt1.y, m_ImagePt2.y);
				break;
			}			
		}
	}
	else if ( nFlags&MK_RBUTTON )
	{
		m_ImageOffset.x += point.x-m_MovingPos.x;
		m_ImageOffset.y += point.y-m_MovingPos.y;
		m_MovingPos = point;
		this->DrawImageWndMemDC();
	}
	this->RedrawWnd();

	CDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;
	this->SetCapture();

	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	this->m_RBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	

	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnLightAlignBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_GRID);	
	this->GetDlgItemText(CALIALIGN_LIGHT_ALIGN_RANGE_EDIT, str);	
	this->m_GrayMinRatio = ::_tcstod(str, NULL);
	if ( this->ConfigGrabParam(CALIBRATION_2D_LIGHT_ALIGN) == false )
	{	return; }	
	FocusToEditCtrl();
	SetCalibrationMode(CALIBRATION_2D_LIGHT_ALIGN);	
	CWnd::CheckDlgButton(CALIALIGN_GRAB_REPEAT_CHK, TRUE);
	this->ExecGrabFirst();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::On3DCastAlignBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bShowMsg = true;
	if ( Check3DCastID(bShowMsg) == false ) { return; }
	CString str;	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_GRID);	
	this->GetDlgItemText(CALIALIGN_3DCAST_ALIGN_RANGE_EDIT, str);	
	this->m_GrayMinRatio = ::_tcstod(str, NULL);
	if ( this->ConfigGrabParam(CALIBRATION_3D_CAST_ALIGN) == false )
	{	return; }
	FocusToEditCtrl();
	SetCalibrationMode(CALIBRATION_3D_CAST_ALIGN);	
	CWnd::CheckDlgButton(CALIALIGN_GRAB_REPEAT_CHK, TRUE);	
	this->ExecGrabFirst();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::On3DCastFocusBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bShowMsg = true;
	if ( Check3DCastID(bShowMsg) == false ) { return; }
	m_3DCastFocusLT = 0;//3D投光焦距-左上
	m_3DCastFocusRT = 0;//3D投光焦距-右上
	m_3DCastFocusLB = 0;//3D投光焦距-左下
	m_3DCastFocusRB = 0;//3D投光焦距-右下
	m_3DCastFocusCC = 0;//3D投光焦距-中央
	m_3DCastFocusCCMax = 0;
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_NONE);	
	if ( this->ConfigGrabParam(CALIBRATION_3D_CAST_FOCUS) == false )
	{	return; }
	FocusToEditCtrl();
	SetCalibrationMode(CALIBRATION_3D_CAST_FOCUS);	
	CWnd::CheckDlgButton(CALIALIGN_GRAB_REPEAT_CHK, TRUE);
	this->ExecGrabFirst();
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ShowPixelInfo()
{
	POINT WndPt;
	::GetCursorPos(&WndPt);
	CWnd::ScreenToClient(&WndPt);
	CWnd *pWnd = CWnd::GetDlgItem(CALIALIGN_IMAGE_WND);
	if ( NULL == pWnd ) { return false; }
	CWnd::MapWindowPoints(pWnd, &WndPt, 1);	
	ShowPixelInfo(WndPt);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ShowPixelInfo(const POINT &WndPt)
{
	int      R=0, G=0, B=0, idx=0;		
	POINT    ImagePos={0};
	CString  strPixel;
	TPOINT2D ImagePt=WndPt;
	TPOINT2D WndPt2 =WndPt;
	const IMAGE_SIZE ImageW = this->m_ImageW;
	const IMAGE_SIZE ImageH = this->m_ImageH;
	const IMAGE_SIZE ImageStep = this->m_ShowStep;
	const IMAGE_SIZE BitCount = this->m_ShowBitCount;
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt2);
		
	JetAPI::Point2DToPoint(ImagePt, ImagePos);		
	if ( NULL==m_ShowBuffer || ImagePos.x<0 || ImagePos.y<0 || ImagePos.x>=nImageW || ImagePos.y>=nImageH )
	{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), Ave(%.0f, %.0f, %.0f), ROI(%.0f, %.0f, %.0f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, m_AveGrayR, m_AveGrayG, m_AveGrayB, m_AveRoiR, m_AveRoiG, m_AveRoiB); }
	else if ( 8 == BitCount )
	{	
		idx = (ImagePos.y*ImageStep)+(ImagePos.x);
		R = G = B = m_ShowBuffer[idx];
		strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), RGB=(%d, %d, %d), Ave(%.0f, %.0f, %.0f), ROI(%.0f, %.0f, %.0f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R, G, B, m_AveGrayR, m_AveGrayG, m_AveGrayB, m_AveRoiR, m_AveRoiG, m_AveRoiB);
	}
	else if ( 24 == BitCount )
	{	
		idx = (ImagePos.y*ImageStep)+(ImagePos.x*3);
		B = m_ShowBuffer[idx]; 
		G = m_ShowBuffer[idx+1]; 
		R = m_ShowBuffer[idx+2]; 
		strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), RGB=(%d, %d, %d), Ave(%.0f, %.0f, %.0f), ROI(%.0f, %.0f, %.0f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R, G, B, m_AveGrayR, m_AveGrayG, m_AveGrayB, m_AveRoiR, m_AveRoiG, m_AveRoiB);
	}
	else
	{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), Ave(%.0f, %.0f, %.0f), ROI(%.0f, %.0f, %.0f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, m_AveGrayR, m_AveGrayG, m_AveGrayB, m_AveRoiR, m_AveRoiG, m_AveRoiB); }
	this->SetDlgItemText(CALIALIGN_PIXEL_INFO_EDIT, strPixel);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetCurrentDefaultRoi(int ImageW, int ImageH, TPOINT2D &Pt1, TPOINT2D &Pt2) const
{
	const int W2 = ImageW / 2;
	const int H2 = ImageH / 2;
	const int CpX = ImageW / 2;
	const int CpY = ImageH / 2;
	Pt1.x = CpX - (W2 / 2);
	Pt1.y = CpY - (H2 / 2);
	Pt2.x = CpX + (W2 / 2);
	Pt2.y = CpY + (H2 / 2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( WM_MOUSEWHEEL != message ) { return false; }
	CWnd *pWnd = GetFocus();
	if ( NULL == pWnd ) { return false; }
	if ( this != pWnd )
	{	pWnd = pWnd->GetParent();	}	
	if ( this != pWnd ) { return false; }

	CPoint pt, point;
	point.x = pt.x = GET_X_LPARAM(lParam); 
	point.y = pt.y = GET_Y_LPARAM(lParam); 
	this->ScreenToClient(&point);
	if ( JetAPI::CheckPtInCtrlWnd(this, point, CALIALIGN_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	double NextImageZoom = this->m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;	

	this->DrawImageWndMemDC();
	this->RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneAlign::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneAlign::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		AOIDataCollect.SetCallbackWnd(GetSafeHwnd());	
		FocusToEditCtrl();
		SetCalibrationMode(CALIBRATION_STOP);
		this->StartReGrab(TRUE);		
	}
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::StartReGrab(BOOL ReStart)//開始取下一個像
{
	::Sleep(10);
	if ( TRUE == ReStart ) 
	{	ReStart = ReStart; }
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, ReStart, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecReGrab(WPARAM wParam)
{
	BOOL   bReGrabAll = (BOOL)(wParam);	
	if ( FALSE == bReGrabAll )
	{
		if ( this->ExecGrabNext() == false )
		{
			SetCalibrationMode(CALIBRATION_STOP);
			return false;
		}
		return true;
	}	
	CAMERA_ID CameraID = this->m_CameraID;
	CALIBRATION_MODE CalibrationMode = GetCalibrationMode();	
	switch ( CalibrationMode )
	{
	case CALIBRATION_FOV_WIDTH_END:		
		this->OnFOVWidthBtn();		
		break;
	case CALIBRATION_FOV_HEIGHT_END:
		this->OnFOVHeightBtn();		
		break;
	case CALIBRATION_CAMERA_ALIGN_HOR_END:
		this->OnCameraAlignHorBtn();		
		break;
	case CALIBRATION_CAMERA_ALIGN_VER_END:
		this->OnCameraAlignVerBtn();		
		break;	
	case CALIBRATION_IMAGE_RESOLUTION:
		this->OnImageResolutionCalcBtn();		
		break;		
	case CALIBRATION_2D_LIGHT_ALIGN:
		//this->OnLightAlignBtn();		
		this->ExecGrabNext();
		break;
	case CALIBRATION_3D_CAST_ALIGN:
		//this->On3DCastAlignBtn();		
		this->ExecGrabNext();
		break;
	case CALIBRATION_3D_CAST_FOCUS:			
		//this->On3DCastFocusBtn();		
		this->ExecGrabNext();
		break;
	case CALIBRATION_PATTERN_ZERO_PLANE:	
		this->ExecPhaseZeroPlaneBtn();
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT:		
		this->ExecGrabNext();
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV:
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV_Z:
		this->ExecGrabNext();
		break;
	case CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV:
		break;
	case CALIBRATION_STOP:
		this->OnGrabBtn();		
		break;
	default:
		CameraCtrl.StopCameraGrab(CameraID);
		this->LockUIWnd(false); 
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeManipulateCombo() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();	
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnGrabRepeatChk() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnLightCurrentBtn() 
{
	// TODO: Add your control notification handler code here	
	CString str;	
	DWORD   Res=0;
	str.Format(_T("Do you want to Execute Each 2D Light current tunning ?"));
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Res  )
	{	return; }	
	
	if ( IDYES != Res )
	{	SetCalibrateAllCannel(false); }
	else
	{	
		const int Count = m_SliceCombo.GetCount();
		if ( Count > 0 )
		{	
			m_SliceCombo.SetCurSel(0); 
			UpdateSliceParamToUI();
		}
		SetCalibrateAllCannel(true);
	}

	str.Format(_T("Do you want to reset ROI ?"));
	str = LoadMultiLanguageString(str, str);
	if (JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES)
	{
		GetCurrentDefaultRoi(m_ImageW, m_ImageH, m_ImagePt1, m_ImagePt2);
		JetAPI::PointsToRect(m_ImagePt1, m_ImagePt2, m_ImageRect4D);		
		RedrawWnd();
	}

	this->m_2DLEDCurrent = 1;	
	this->m_2DLEDCurrentGray = 0;	
	this->m_2DLEDCurrentAlarmList.clear();
	this->m_2DLEDCurrentReadingList.clear();
	this->m_CameraExposureTimeBackup_us = m_CameraExposureTime_us;
	this->m_GrayTarget = this->GetDlgItemInt(CALIALIGN_LIGHT_CURRENT_GRAY_EDIT);	
	this->m_CameraExposureTime_us = this->GetDlgItemInt(CALIALIGN_LIGHT_CURRENT_EXP_EDIT);		
	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
	const CALIBRATION_MODE CaliMode = CALIBRATION_2D_LIGHT_CURRENT;	
	if ( this->ConfigGrabParam(CaliMode) == false )
	{	return; }	
	FocusToEditCtrl();
	SetCalibrationMode(CaliMode);	
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::On3DCastCurrentBtn() 
{
	// TODO: Add your control notification handler code here
	DWORD   Res;
	CString str;
	str.Format(_T("Do you want to Execute 3D Cast current tunning ?"));
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( IDNO == Res )
	{	return ; }

	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_00;
	LIGHT_3D_CAST_ID Light3DID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(m_3DCastIDCombox));
	if ( LIGHT_3D_CAST_00 != Light3DID )
	{	
		CastID = Light3DID;
		SetCalibrateAllCastID(false);	
	}
	else
	{			
		SetCalibrateAllCastID(true);		
		m_3DCastIDCombox.SetCurSel(0);
		CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(m_3DCastIDCombox));
	}
	const bool bShowMsg = true;
	if ( Check3DCastID(bShowMsg) == false ) { return; }

	bool LEDColorUsed_Red = true;
	bool LEDColorUsed_Grn = true;
	bool LEDColorUsed_Blu = true;
	LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
	if ( NULL != Light3DPtr )
	{
		LEDColorUsed_Red = Light3DPtr->GetLEDColorUsed_Red();
		LEDColorUsed_Grn = Light3DPtr->GetLEDColorUsed_Grn();
		LEDColorUsed_Blu = Light3DPtr->GetLEDColorUsed_Blu();
	}

	str.Format(_T("Do you want to reset ROI ?"));
	str = LoadMultiLanguageString(str, str);
	if (JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES)
	{
		GetCurrentDefaultRoi(m_ImageW, m_ImageH, m_ImagePt1, m_ImagePt2);
		JetAPI::PointsToRect(m_ImagePt1, m_ImagePt2, m_ImageRect4D);
		RedrawWnd();
	}		
	
	m_3DCastCurRed = 0;//3D投光電流-Red
	m_3DCastCurGrn = 0;//3D投光電流-Grn
	m_3DCastCurBlu = 0;//3D投光電流-Blu	
	m_3DCastCurrent = 1;	
	m_3DCastCurrentGray = 0;	
	m_CameraExposureTimeBackup_us = m_CameraExposureTime_us;
	m_GrayTarget = this->GetDlgItemInt(CALIALIGN_PATTERN_CURRENT_GRAY_EDIT);
	m_3DCastExposureTimeus = CWnd::GetDlgItemInt(CALIALIGN_3D_CAST_CURRENT_EXP_EDIT);		
	const int Cast3DGrayValue = (int)CWnd::GetDlgItemInt(CALIALIGN_PATTERN_CURRENT_GRAY_EDIT);	
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CaliParam.m_3DCastCurrentExpTime_us = m_3DCastExposureTimeus;
	CaliParam.m_3DCastCurrentGray = Cast3DGrayValue;	

	const unsigned int SliceUniqueID = SLICE_UNIQUE_ID_DLP;
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);
	if ( NULL != SliceParamPtr )
	{
		SliceParamPtr->SliceTargetGray = Cast3DGrayValue;
		SliceParamPtr->SliceCameraExpTimeus = m_3DCastExposureTimeus;		
	}	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	

	CALIBRATION_MODE Mode=CALIBRATION_STOP;
	if ( true == LEDColorUsed_Red ) { Mode = CALIBRATION_3D_CAST_CURRENT_RED; }
	else if ( true == LEDColorUsed_Grn ) { Mode = CALIBRATION_3D_CAST_CURRENT_GRN; }
	else if ( true == LEDColorUsed_Blu ) { Mode = CALIBRATION_3D_CAST_CURRENT_BLU; }
	else { return; }

	if ( this->ConfigGrabParam(Mode) == false )
	{	return; }	
	FocusToEditCtrl();
	SetCalibrationMode(Mode);	
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeLightCombo() 
{
	// TODO: Add your control notification handler code here	
	FocusToEditCtrl();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchange3DCastIDCombo() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();	
	Update3DCastCurrentToUI();
	OnGrabBtn();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPhasePeriodBtn()
{
	// TODO: Add your control notification handler code here	
	CString strPeriod1, strPeriod2;
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1, strPeriod1);
	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, strPeriod2);
	CString str;	
	int   Index1=-1;
	int   Index2=-1;	
	double Period1 = 0;
	double Period2 = 0;
	CInputBoxWnd InputBox;		
	CString strLabel1 = _T("Period 1");
	CString strLabel2 = _T("Period 2");
	int BitMode = Light3DCtrl.GetPatternBitCount();
	if ( 8 != BitMode )
	{	BitMode = 6; }

	Period1 = ::_ttof(strPeriod1);
	Period2 = ::_ttof(strPeriod2);	
	Light3DCtrl.FindPatternIndex(BitMode, Period1, Index1);
	Light3DCtrl.FindPatternIndex(BitMode, Period2, Index2);
	strLabel1.Format(_T("%s [ID:%d]"), _T("Period 1"), Index1+1);
	strLabel2.Format(_T("%s [ID:%d]"), _T("Period 2"), Index2+1);
	InputBox.SetParam2(_T("Phase Period"), strLabel1, strPeriod1, strLabel2, strPeriod2);
	if ( InputBox.DoModal() != IDOK )
	{	return; }

	Index1=-1;
	Index2=-1;	
	strPeriod1 = InputBox.m_DataEdit1;
	strPeriod2 = InputBox.m_DataEdit2;
	Period1 = ::_ttof(strPeriod1);
	Period2 = ::_ttof(strPeriod2);	
	if ( strPeriod1.CompareNoCase(strPeriod2) == 0 )
	{
		str = _T("Error, Period 1 can not equal to Period2");
		JetAPI::ShowMessageBox(str);
		return ;
	}	
	Light3DCtrl.LoadPatternIndexIniFile();
	if ( Light3DCtrl.FindPatternIndex(BitMode, Period1, Index1) == false )
	{
		str = Light3DCtrl.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
	if ( Light3DCtrl.FindPatternIndex(BitMode, Period2, Index2) == false )
	{
		str = Light3DCtrl.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}

	str.Format(_T("New Phase Period (%.2f [%d], %.2f [%d])"), Period1, Index1+1, Period2, Index2+1);
	JetAPI::ShowMessageBox(str);

	double PeriodMin = MIN(Period1, Period2);
	double PeriodMax = MAX(Period1, Period2);
	strPeriod1.Format(_T("%.2f"), PeriodMin);
	strPeriod2.Format(_T("%.2f"), PeriodMax);
	SysParam.m_PhasePeriod1 = PeriodMin;
	SysParam.m_PhasePeriod2 = PeriodMax;
	CWnd::SetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1, strPeriod1);
	CWnd::SetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, strPeriod2);
	if ( Light3DCtrl.SetAllLight3DCastPatternIndexByPeriod(PeriodMin, PeriodMax) == false )
	{
		str = Light3DCtrl.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternZeroPlaneBtn() 
{
	// TODO: Add your control notification handler code here	
	FocusToEditCtrl();
	if ( CreateRawImageFolder() == false ) { return; }
	const int CurrentID = DLP_LED_CURRENT_ID_01;
	JetAPI::SetComboxCurSel(m_3DCastCurrentIDCombox, CurrentID);	
	this->ExecPhaseZeroPlaneBtn();	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::MoveToPhaseZeroPlane()//移動至相平面高度
{
	CString str;
	this->GetDlgItemText(CALIALIGN_PHASE_PLANE_OFFSET_EDIT, str);
	m_ZeroPhaseOffsetPosZ = ::_ttoi(str);
	double NewPosZ = 0;
	const bool SignZ = AOIDataCollect.GetStageSignPositiveZ();
	if ( true == SignZ )
	{	NewPosZ = this->m_StagePosZ + m_ZeroPhaseOffsetPosZ;	}
	else
	{	NewPosZ = this->m_StagePosZ - m_ZeroPhaseOffsetPosZ;	}
	if ( MotionCtrlPtr->MoveTo(AXIS_Z, NewPosZ, MOTION_MOVING_NORMAL) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}	
	if ( MotionCtrlPtr->WaitForDone(AXIS_Z) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}
	ExtraDelayTime();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseZeroPlaneBtn()
{
	CString str;		
	if ( this->RetrieveStagePosition() == false )
	{	return false;	}
	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_00;
	CALIBRATION_MODE CalibrationMode = CALIBRATION_PATTERN_ZERO_PLANE;	
	str.Format(_T("Do you want to Execute 3D Cast phase base?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return false; }		
	if ( InputPhaseZeroPlaneCorrect() == false )
	{	return false; }
	if ( GetFirstCali3DCastID(CastID) == false )
	{	return false; }		
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return false; }	
	if ( MoveToPhaseZeroPlane() == false )
	{	return false; }
	SetCalibrationMode(CalibrationMode);	
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CheckUseRawPhaseData() const//確認使用原始相位值
{	
	const TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();
	if ( FN_ENABLE != CaliParam.m_EnableHeightFactorCorrect )
	{	return false; }
	//return false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::InputPhaseZeroPlaneCorrect()//輸入相位-相平面修正
{
	CString str;
	DWORD DefaultBtn=0;	
	TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();		
	str.Format(_T("Do you want to correct base phase?"));
	str = LoadMultiLanguageString(str, str);
	switch ( CaliParam.m_EnableBasePhaseCorrect )
	{
	case FN_ENABLE: DefaultBtn=MB_DEFBUTTON1; break;
	default:
	case FN_DISABLE: DefaultBtn=MB_DEFBUTTON2; break;
	}
	DWORD Res=JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|DefaultBtn);
	if ( IDCANCEL == Res ) { return false; }		
	if ( IDYES == Res ) { CaliParam.m_EnableBasePhaseCorrect = FN_ENABLE; }
	if ( IDNO == Res ) { CaliParam.m_EnableBasePhaseCorrect = FN_DISABLE; }

	//v1.01.03.234
	if ( FN_ENABLE == CaliParam.m_EnableBasePhaseCorrect)
	{			
		CInputBoxWnd InputBox;
		CString strCaption, strLabel;
		strLabel = _T("Correct Times");
		strCaption = _T("Input Correct Times");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		str.Format(_T("%d"), CaliParam.m_BasePhaseCorrectTimes);
		InputBox.SetParam1(strCaption, strLabel, str);
		if (InputBox.DoModal() == IDCANCEL)
		{	return false;	}
		const int nVal=::_ttoi(InputBox.m_DataEdit1);
		if ( nVal>1 && nVal<100 )
		{	CaliParam.m_BasePhaseCorrectTimes = nVal; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::InputPhaseHeightFactorCorrect()//輸入相位高度係數修正
{
	CString str;
	DWORD DefaultBtn=0;	
	TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();
	str.Format(_T("Do you want to correct height factor?"));
	str = LoadMultiLanguageString(str, str);
	switch ( CaliParam.m_EnableHeightFactorCorrect )
	{
	case FN_ENABLE: DefaultBtn=MB_DEFBUTTON1; break;
	default:
	case FN_DISABLE: DefaultBtn=MB_DEFBUTTON2; break;
	}
	DWORD Res=JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|DefaultBtn);
	if ( IDCANCEL == Res ) { return false; }		
	if ( IDYES == Res ) { CaliParam.m_EnableHeightFactorCorrect = FN_ENABLE; }
	if ( IDNO == Res ) { CaliParam.m_EnableHeightFactorCorrect = FN_DISABLE; }

	//v1.01.04.001
	if ( FN_ENABLE == CaliParam.m_EnableHeightFactorCorrect)
	{			
		CInputBoxWnd InputBox;
		CString strCaption, strLabel;
		strLabel = _T("Correct Times");
		strCaption = _T("Input Correct Times");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		str.Format(_T("%d"), CaliParam.m_HeightFactorCorrectTimes);
		InputBox.SetParam1(strCaption, strLabel, str);
		if (InputBox.DoModal() == IDCANCEL)
		{	return false;	}
		const int nVal=::_ttoi(InputBox.m_DataEdit1);
		if ( nVal>1 && nVal<100 )
		{	CaliParam.m_HeightFactorCorrectTimes = nVal; }
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ExecPhaseHeightFactor_DOT()
{	
	//if ( InputPhaseHeightFactorCorrect() == false )
	//{	return; }
	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_00;
	CALIBRATION_MODE CalibrationMode = CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT;	
	if ( GetFirstCali3DCastID(CastID) == false )
	{	return; }
	FocusToEditCtrl();
	SetCalibrationMode(CalibrationMode);
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_FACTOR_GRID);	
	if ( ExecPhaseHeightFactor_DOT(CastID) == false ) 
	{
		JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_DOT(LIGHT_3D_CAST_ID CastID)
{
	CAMERA_ID CameraID = this->m_CameraID;	
	const IMAGE_SIZE ImageWFull = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageHFull = CameraCtrl.GetCameraImageSizeH(CameraID);
	const IMAGE_SIZE ImageWHalf = ImageWFull/2;
	const IMAGE_SIZE ImageHHalf = ImageHFull/2;
	TPOINT2D ImagePt, StageCP, StagePt;
	MotionCtrlPtr->GetCurrentPos(StageCP.x, StageCP.y);	
	ImagePt.x = (m_ImageRect4D.left+m_ImageRect4D.right)/2.0;
	ImagePt.y = (m_ImageRect4D.top+m_ImageRect4D.bottom)/2.0;		
	AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt);	
	if ( MotionCtrlPtr->XYMoveTo(StagePt.x, StagePt.y) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	MotionCtrlPtr->WaitForMotionStop();
	ImagePt.x = ImagePt.x-ImageWHalf;
	ImagePt.y = ImagePt.y-ImageHHalf;
	m_ImageRect4D.left  -= ImagePt.x;
	m_ImageRect4D.right -= ImagePt.x;
	m_ImageRect4D.top   -= ImagePt.y;
	m_ImageRect4D.bottom-= ImagePt.y;

	CALIBRATION_MODE CalibrationMode = GetCalibrationMode();	
	if ( this->RetrieveStagePosition() == false )
	{	return false;	}	
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return false; }		
	
	const size_t FactorSize = this->m_PhaseFactorList.size();	
	if ( this->m_PhaseFactorIndex >= FactorSize )
	{	return false; }
	TPhaseFactorGrid *GridPtr = &(m_PhaseFactorList[m_PhaseFactorIndex]);
	if ( MotionCtrlPtr->XYZMoveTo(GridPtr->m_PosX, GridPtr->m_PosY, GridPtr->m_PosZ) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	ExtraDelayTime();
	const unsigned int ImageW = this->m_ImageW;
	const unsigned int ImageH = this->m_ImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;		
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ExecPhaseHeightFactor_FOV()
{	
	if ( InputPhaseHeightFactorCorrect() == false )
	{	return; }
	CAMERA_ID CameraID = this->m_CameraID;	
	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_00;	
	CALIBRATION_MODE CalibrationMode = CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV;	
	const IMAGE_SIZE ImageWFull = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageHFull = CameraCtrl.GetCameraImageSizeH(CameraID);
	const IMAGE_SIZE ImageWHalf = ImageWFull/2;
	const IMAGE_SIZE ImageHHalf = ImageHFull/2;	
	MotionCtrlPtr->WaitForMotionStop();		
	if ( this->RetrieveStagePosition() == false )
	{	return;	}		
	if ( GetFirstCali3DCastID(CastID) == false )
	{	return; }
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return; }
	const unsigned int ImageW = this->m_ImageW;
	const unsigned int ImageH = this->m_ImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	FocusToEditCtrl();
	SetCalibrationMode(CalibrationMode);
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ExecPhaseHeightFactor_MultiFOV()
{		
	LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_00;
	CALIBRATION_MODE CalibrationMode = CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV;	
	if ( GetFirstCali3DCastID(CastID) == false )
	{	return ; }	
	FocusToEditCtrl();
	SetCalibrationMode(CalibrationMode);	
	if ( ExecPhaseHeightFactor_MultiFOV(CastID) == false ) 
	{		
		SetCalibrationMode(CALIBRATION_STOP);
		return;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecPhaseHeightFactor_MultiFOV(LIGHT_3D_CAST_ID CastID)
{	
	CAMERA_ID CameraID = this->m_CameraID;	
	CALIBRATION_MODE CalibrationMode = CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV;	
	const IMAGE_SIZE ImageWFull = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageHFull = CameraCtrl.GetCameraImageSizeH(CameraID);
	const IMAGE_SIZE ImageWHalf = ImageWFull/2;
	const IMAGE_SIZE ImageHHalf = ImageHFull/2;	
	MotionCtrlPtr->WaitForMotionStop();		
	if ( this->RetrieveStagePosition() == false )
	{	return false;	}	
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return false; }

	const unsigned int ImageW = this->m_ImageW;
	const unsigned int ImageH = this->m_ImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	FocusToEditCtrl();
	SetCalibrationMode(CalibrationMode);
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternHeightFactorBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;	
	if ( CreateRawImageFolder() == false ) { return; }	
	if ( CheckHeightFactorPhaseID() == false ) { return ; }	
	str.Format(_T("Do you want to Execute 3D Cast height factor ?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }

	const int CurrentID = DLP_LED_CURRENT_ID_01;
	TCalibrationParameter &CaliParam=AOIDataCollect.GetCalibrationParameter();
	BOOL bFOVChk = CWnd::IsDlgButtonChecked(CALIALIGN_PHASE_HEIGHT_FACTOR_FOV_CHK);
	const int PhaseConvertHeightMode = JetAPI::GetComboxCurSelData(m_PhaseToHeightCombox);
	JetAPI::SetComboxCurSel(m_3DCastCurrentIDCombox, CurrentID);
	if ( TRUE == bFOVChk )
	{			
		CaliParam.m_HeightFactorCalibrateWithFOV = FN_ENABLE;
		if ( PHASE_CONVERT_HEIGHT_SCALE == PhaseConvertHeightMode )
		{	ExecPhaseHeightFactor_FOV();	}
		else
		{	ExecPhaseHeightFactor_MultiFOV();		}
	}
	else
	{
		CaliParam.m_HeightFactorCalibrateWithFOV = FN_DISABLE;
		ExecPhaseHeightFactor_DOT();	
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::SaveRawImageList(LPCTSTR Name, LIGHT_3D_CAST_ID CastID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtrList[], int ImageCount, int SaveTimes)
{	
	int     i=0;	
	int     u=0, v=0;
	CString str;
	CString strCount;
	CString strPeriod;	
	CString RawName;	
	const int MaxN=6;
	std::vector<CString> 	RawFileList;
	CString Folder = GetSaveRawImageFolder();	
	CString ExtName = GetSaveRawImageExtName();	
	CString CastName = AOIDataDefine.GetLight3DCastIDText(CastID);
	::CreateDirectory(Folder, NULL);	::Sleep(0);

	for ( i=0; i<ImageCount; i++ )
	{
		u = i%MaxN;
		v = i/MaxN;
		if ( NULL == ImagePtrList[i] ) { continue; }
		strCount.Format(_T("%d"), u+1);
		strPeriod.Format(_T("%c"), _T('A')+v);		
		if ( 0 == SaveTimes )
		{	RawName.Format(_T("%s%s_%s%s.%s"), CastName, Name, strPeriod, strCount, ExtName);	}
		else
		{	RawName.Format(_T("%s[%02d]%s_%s%s.%s"), CastName, SaveTimes, Name, strPeriod, strCount, ExtName);	}		
		RawFileList.push_back(RawName);		
		str.Format(_T("%s\\%s"), Folder, RawName);
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtrList[i], true);		
	}

	FILE  *pfile = NULL;
	TCHAR  TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	if ( 0 == SaveTimes )
	{	str.Format(_T("%s\\%s%s"), Folder, CastName, _T("RawFileList.TXT")); }
	else
	{	str.Format(_T("%s\\%s[%02d]%s"), Folder, CastName, SaveTimes, _T("RawFileList.TXT")); }
	pfile = ::_tfopen(str, TMode);
	if ( NULL != pfile )
	{
		const size_t RawFileCount = RawFileList.size();
		for ( i=0; i<RawFileCount; i++ )
		{	::_ftprintf(pfile, _T("%s\n"), RawFileList[i]);	}
		::_ftprintf(pfile, _T("\n"));
		::fclose(pfile); pfile = NULL;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcPhasePeriod1(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr, int SaveTimes)//計算相機平面
{
	if ( NULL == PhasePtr ) { return false; }
	const char fnName[] = "CCaliPaneAlign::CalcPhasePeriod1";
	CString str;	
	long   idx=0;
	size_t i=0, j=0;	
	IMAGE_PTR PatternPtr1=NULL;	
	IMAGE_PTR PatternPtr2=NULL;	
	IMAGE_PTR PatternPtr3=NULL;	
	IMAGE_PTR PatternPtr4=NULL;	
	IMAGE_PTR PatternPtr5=NULL;
	IMAGE_PTR PatternPtr6=NULL;
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
	const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));	

	TPhaseNoiseParam NoiseParam;	
	GetPhaseNoiseParam(NoiseParam);
	
	idx = RingIndex;
	idx = idx-ImageCount;
	idx = idx+m_CameraImageReceieveCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	const size_t IdxBegin=idx;
	if ( PatternStep >= BATCH_GRAB_STEP_4 )
	{
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr1) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr2) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr3) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr4) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;
	}	
	const size_t IdxEnd=idx;
	const size_t IdxCount=IdxEnd-IdxBegin;

	//將圖像次序相反-20161011-改在DLP的樣板圖片次序顛倒
	//ImageAPI.SwapImagePtr5(PatternPtr1, PatternPtr2, PatternPtr3, PatternPtr4, PatternPtr5, true);	
	if ( true == SaveRaw )
	{	 
		int       ImagePtrCnt=0;
		IMAGE_SIZE BitCount = 8;
		IMAGE_PTR ImagePtrList[64];
		CString Name = _T("Pattern4S1P");
		::memset(ImagePtrList, 0x00, sizeof(ImagePtrList));
		i = 0;
		ImagePtrList[i++] = PatternPtr1;	ImagePtrList[i++] = PatternPtr2;	ImagePtrList[i++] = PatternPtr3;	ImagePtrList[i++] = PatternPtr4;	ImagePtrList[i++] = PatternPtr5;	ImagePtrList[i++] = PatternPtr6;
		ImagePtrCnt = i;
		SaveRawImageList(Name, CastID, ImageW, ImageH, ImageStep, BitCount, ImagePtrList, ImagePtrCnt, SaveTimes);		
	}

	int         PatternID = 0; 
	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;	
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	if ( FALSE!=bCheckSmoothFilter && nSmoothFilterSize>0 && NULL!=ImagePtr )//NULL!=ImagePtr->非校正時
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PatternStep )
		{		
		case BATCH_GRAB_STEP_4:
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr1, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr1, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr2, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr2, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr3, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr3, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr4, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr4, BufferPtr, BufferSize);	}
				JetMemory.free_func(BufferPtr);
			}			
			break;
		}
	}		

	double Gamma=1.0f;
	bool   bCalcPhase = true;
	Light3DCtrl.GetLight3DImageGamma(CastID, Gamma);
	if ( NULL == CastParamPtr )	{	bCalcPhase = true; }
	else{	bCalcPhase = false; }	
	switch ( PatternStep )
	{	
	case BATCH_GRAB_STEP_4:
		PatternID = PHASE_PATTERN_A;
		if ( true == bCalcPhase )
		{
			if ( ImageAPI.GrayImage4FrameToPhase3(ImageW, ImageH, ImageStep, PatternPtr1, PatternPtr2, PatternPtr3, PatternPtr4, PatternID, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr) == false )		
			{
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return false;
			}
		}
		if ( NULL != ImagePtr )
		{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr1, PatternPtr2, PatternPtr3, PatternPtr4, ImagePtr);	}
		break;	
	}
	if ( NULL != CastParamPtr )
	{	
		CastParamPtr->ExpCount = 1;
		//CastParamPtr->PerA = Per1;
		//CastParamPtr->PerB = Per2;	
		CastParamPtr->ImageW = ImageW;
		CastParamPtr->ImageH = ImageH;
		CastParamPtr->ImageStep = ImageStep;
		CastParamPtr->ImageCount = IdxCount;		
		//CastParamPtr->ExpTimeA = ExpTime1;
		//CastParamPtr->ExpTimeB = ExpTime2;
		CastParamPtr->Gamma = Gamma;
		//CastParamPtr->DecodeMode = SliceFuncMode;	

		CastParamPtr->PtrA1 = PatternPtr1;
		CastParamPtr->PtrA2 = PatternPtr2;
		CastParamPtr->PtrA3 = PatternPtr3;
		CastParamPtr->PtrA4 = PatternPtr4;
		CastParamPtr->PtrA5 = PatternPtr5;
		return true;
	}
	const int   nVoidExpandSize = NoiseParam.PhaseExtendVoid;
	const BOOL  bCheckVoidExpaned = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND);
	//NoiseParam.PhaseExtendVoid = 0;//因為Cuda尚未撰寫, 先行關閉
	if ( FALSE!=bCheckVoidExpaned && nVoidExpandSize > 0 && NULL!=ImagePtr  )
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nVoidExpandSize*2+1;
		const int    nVoidExpandIterCount = 1;		
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
		{	
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, ImageStep, MaskPtr, nVoidExpandSize, nVoidExpandIterCount, BufferPtr) == true )
			{	::memcpy(MaskPtr, BufferPtr, BufferSize);	}
			JetMemory.free_func(BufferPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcPhasePeriod2(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr, int SaveTimes)//計算相機平面
{
	if ( NULL == PhasePtr ) { return false; }
	const char fnName[] = "CCaliPaneAlign::CalcPhasePeriod2";
	CString       str;	
	size_t        i=0;
	long          idx=0;	
	double        Per1=8, Per2=12, PerN=0;	
	IMAGE_PTR PatternPtr11=NULL;	
	IMAGE_PTR PatternPtr12=NULL;	
	IMAGE_PTR PatternPtr13=NULL;	
	IMAGE_PTR PatternPtr14=NULL;	
	IMAGE_PTR PatternPtr15=NULL;
	IMAGE_PTR PatternPtr16=NULL;
	IMAGE_PTR PatternPtr21=NULL;	
	IMAGE_PTR PatternPtr22=NULL;	
	IMAGE_PTR PatternPtr23=NULL;	
	IMAGE_PTR PatternPtr24=NULL;	
	IMAGE_PTR PatternPtr25=NULL;
	IMAGE_PTR PatternPtr26=NULL;
	std::vector<IMAGE_PTR*> PtrList;
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	

	TPhaseNoiseParam NoiseParam;	
	GetPhaseNoiseParam(NoiseParam);

	idx = RingIndex;
	idx = idx-ImageCount;
	idx = idx+m_CameraImageReceieveCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1, str);	
	Per1 = ::_tcstod(str, NULL);
	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, str);	
	Per2 = ::_tcstod(str, NULL);

	PtrList.clear();
	const size_t IdxBegin=idx;
	const int ExpTime1 = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT);
	const int ExpTime2 = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT2);		
	if ( PatternStep >= BATCH_GRAB_STEP_4 )
	{
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP==SliceFuncMode )//SLICE_FUNC_3D_2_2STEP_2EXP
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
		}
		else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
		}		
		else if ( SLICE_FUNC_3D_4STEP_4GC_1EXP==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
		}
		else if ( SLICE_FUNC_3D_4STEP_5GC_1EXP==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			PtrList.push_back(&PatternPtr25);
		}			
		else if ( SLICE_FUNC_3D_4STEP_6GC_1EXP==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			PtrList.push_back(&PatternPtr25);
			PtrList.push_back(&PatternPtr26);
		}
		else
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
		}
	}
	const size_t PtrCount=PtrList.size();
	for ( i=0; i<PtrCount; i++ )
	{
		IMAGE_PTR *Ptr=PtrList[i];
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, *Ptr) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;		
		m_CameraImageReceieveCount ++;			
	}
	const size_t IdxEnd=idx;
	const size_t IdxCount=IdxEnd-IdxBegin;
	//將圖像次序相反-20161011-改在DLP的樣板圖片次序顛倒
	//ImageAPI.SwapImagePtr5(PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr15, true);
	//ImageAPI.SwapImagePtr5(PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, true);

	if ( true == SaveRaw )
	{
		int       ImagePtrCnt=0;
		IMAGE_SIZE BitCount = 8;
		IMAGE_PTR ImagePtrList[64];
		CString Name = _T("Pattern4S2P");
		::memset(ImagePtrList, 0x00, sizeof(ImagePtrList));
		i = 0;
		ImagePtrList[i++] = PatternPtr11;	ImagePtrList[i++] = PatternPtr12;	ImagePtrList[i++] = PatternPtr13;	ImagePtrList[i++] = PatternPtr14;	ImagePtrList[i++] = PatternPtr15;	ImagePtrList[i++] = PatternPtr16;
		ImagePtrList[i++] = PatternPtr21;	ImagePtrList[i++] = PatternPtr22;	ImagePtrList[i++] = PatternPtr23;	ImagePtrList[i++] = PatternPtr24;	ImagePtrList[i++] = PatternPtr25;	ImagePtrList[i++] = PatternPtr26;
		ImagePtrCnt = i;
		SaveRawImageList(Name, CastID, ImageW, ImageH, ImageStep, BitCount, ImagePtrList, ImagePtrCnt, SaveTimes);		
		/*
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		CString      Folder = AOIDataCollect.GetAOITempDirectory();
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
		{
			ImageAPI.AverageGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, BufferPtr);
			str.Format(_T("%s\\%s"), Folder, _T("AverageP1.PNG"));
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, BufferPtr, true);

			ImageAPI.AverageGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, BufferPtr);
			str.Format(_T("%s\\%s"), Folder, _T("AverageP2.PNG"));
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, BufferPtr, true);

			JetMemory.free_func(BufferPtr);
		}
		*/
	}	

	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;	
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	//if ( FALSE!=bCheckSmoothFilter && NULL!=ImagePtr && nSmoothFilterSize>0 )//NULL!=ImagePtr->非校正時
	if ( FALSE!=bCheckSmoothFilter && nSmoothFilterSize>0 )//連校正也平均化
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PatternStep )
		{		
		case BATCH_GRAB_STEP_4:			
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
				{	
				}
				else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22, BufferPtr, BufferSize);	}
				}
				else
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr23, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr23, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr24, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr24, BufferPtr, BufferSize);	}					
				}
				JetMemory.free_func(BufferPtr);
			}
			break;		
		}
	}		

	double Gamma=1.0f;
	bool   bCalcPhase = true;
	Light3DCtrl.GetLight3DImageGamma(CastID, Gamma);
	if ( NULL == CastParamPtr )	{	bCalcPhase = true; }
	else{	bCalcPhase = false; }	
	switch ( PatternStep )
	{	
	case BATCH_GRAB_STEP_4:
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage221FrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, Per1, ExpTime1, PatternPtr21, PatternPtr22, PatternPtr23, Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.CloneGrayImage3(ImageW, ImageH, ImageStep, PatternPtr23, ImagePtr, false);	}
		}
		else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage42FrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, ExpTime1, PatternPtr21, PatternPtr22, Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		}		
		else if ( SLICE_FUNC_3D_4STEP_4GC_1EXP==SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage44GCFrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, ExpTime1, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		}
		else if ( SLICE_FUNC_3D_4STEP_5GC_1EXP == SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage45GCFrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, ExpTime1, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		}
		else if ( SLICE_FUNC_3D_4STEP_6GC_1EXP == SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage46GCFrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, ExpTime1, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, PatternPtr26, Per2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		}
		else
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage4FrameTo2Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, Per2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
			}
			if ( NULL != ImagePtr )
			{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		}		
		break;	
	}
	if ( NULL != CastParamPtr )
	{	
		CastParamPtr->ExpCount = 1;
		CastParamPtr->PerA = Per1;
		CastParamPtr->PerB = Per2;	
		CastParamPtr->ImageW = ImageW;
		CastParamPtr->ImageH = ImageH;
		CastParamPtr->ImageStep = ImageStep;
		CastParamPtr->ImageCount = IdxCount;		
		CastParamPtr->ExpTimeA = ExpTime1;
		CastParamPtr->ExpTimeB = ExpTime2;
		CastParamPtr->Gamma = Gamma;
		AOIDataCollect.CheckSliceFuncModeDecodePhaseMode(IdxCount, SliceFuncMode, CastParamPtr->DecodeMode);

		CastParamPtr->PtrA1 = PatternPtr11;
		CastParamPtr->PtrA2 = PatternPtr12;
		CastParamPtr->PtrA3 = PatternPtr13;
		CastParamPtr->PtrA4 = PatternPtr14;
		CastParamPtr->PtrA5 = PatternPtr15;
		CastParamPtr->PtrB1 = PatternPtr21;
		CastParamPtr->PtrB2 = PatternPtr22;
		CastParamPtr->PtrB3 = PatternPtr23;
		CastParamPtr->PtrB4 = PatternPtr24;
		CastParamPtr->PtrB5 = PatternPtr25;
		CastParamPtr->PtrB6 = PatternPtr26;
		return true;
	}
	const int   nVoidExpandSize = NoiseParam.PhaseExtendVoid;
	const BOOL  bCheckVoidExpaned = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND);
	//NoiseParam.PhaseExtendVoid = 0;//因為Cuda尚未撰寫, 先行關閉
	if ( FALSE!=bCheckVoidExpaned && nVoidExpandSize > 0 && NULL!=ImagePtr  )
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nVoidExpandSize*2+1;
		const int    nVoidExpandIterCount = 1;		
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
		{	
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, ImageStep, MaskPtr, nVoidExpandSize, nVoidExpandIterCount, BufferPtr) == true )
			{	::memcpy(MaskPtr, BufferPtr, BufferSize);	}
			JetMemory.free_func(BufferPtr);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcPhasePeriod3(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, int SaveTimes)//計算相機平面
{
	if ( NULL == PhasePtr ) { return false; }
	const char fnName[] = "CCaliPaneAlign::CalcPhasePeriod3";
	CString str;	
	size_t    i=0;
	long      idx=0;
	double    Per1=8, Per2=12, Per3=24, PerN=0;			
	IMAGE_PTR PatternPtr11=NULL;	
	IMAGE_PTR PatternPtr12=NULL;	
	IMAGE_PTR PatternPtr13=NULL;	
	IMAGE_PTR PatternPtr14=NULL;	
	IMAGE_PTR PatternPtr15=NULL;
	IMAGE_PTR PatternPtr16=NULL;
	IMAGE_PTR PatternPtr21=NULL;	
	IMAGE_PTR PatternPtr22=NULL;	
	IMAGE_PTR PatternPtr23=NULL;	
	IMAGE_PTR PatternPtr24=NULL;	
	IMAGE_PTR PatternPtr25=NULL;
	IMAGE_PTR PatternPtr26=NULL;
	IMAGE_PTR PatternPtr31=NULL;	
	IMAGE_PTR PatternPtr32=NULL;	
	IMAGE_PTR PatternPtr33=NULL;	
	IMAGE_PTR PatternPtr34=NULL;	
	IMAGE_PTR PatternPtr35=NULL;
	IMAGE_PTR PatternPtr36=NULL;
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);	

	TPhaseNoiseParam NoiseParam;	
	GetPhaseNoiseParam(NoiseParam);

	idx = RingIndex;
	idx = idx-ImageCount;
	idx = idx+m_CameraImageReceieveCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1, str);	
	Per1 = ::_tcstod(str, NULL);
	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, str);	
	Per2 = ::_tcstod(str, NULL);	

	const size_t IdxBegin=idx;
	if ( PatternStep >= BATCH_GRAB_STEP_4 )
	{
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr11) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr12) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr13) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr14) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;		
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr21) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr22) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr23) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;				
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr24) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;		
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr31) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr32) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr33) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;	
		m_CameraImageReceieveCount ++;

		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, PatternPtr34) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;
	}
	const size_t IdxEnd=idx;
	const size_t IdxCount=IdxEnd-IdxBegin;
	//將圖像次序相反-20161011-改在DLP的樣板圖片次序顛倒
	//ImageAPI.SwapImagePtr5(PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr15, true);
	//ImageAPI.SwapImagePtr5(PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, true);
	//ImageAPI.SwapImagePtr5(PatternPtr31, PatternPtr32, PatternPtr33, PatternPtr34, PatternPtr35, true);

	if ( true == SaveRaw )
	{
		int       ImagePtrCnt=0;
		IMAGE_SIZE BitCount = 8;
		IMAGE_PTR ImagePtrList[64];
		CString Name = _T("Pattern4S3P");
		::memset(ImagePtrList, 0x00, sizeof(ImagePtrList));
		i = 0;
		ImagePtrList[i++] = PatternPtr11;	ImagePtrList[i++] = PatternPtr12;	ImagePtrList[i++] = PatternPtr13;	ImagePtrList[i++] = PatternPtr14;	ImagePtrList[i++] = PatternPtr15;	ImagePtrList[i++] = PatternPtr16;
		ImagePtrList[i++] = PatternPtr21;	ImagePtrList[i++] = PatternPtr22;	ImagePtrList[i++] = PatternPtr23;	ImagePtrList[i++] = PatternPtr24;	ImagePtrList[i++] = PatternPtr25;	ImagePtrList[i++] = PatternPtr26;
		ImagePtrList[i++] = PatternPtr31;	ImagePtrList[i++] = PatternPtr32;	ImagePtrList[i++] = PatternPtr33;	ImagePtrList[i++] = PatternPtr34;	ImagePtrList[i++] = PatternPtr35;	ImagePtrList[i++] = PatternPtr36;
		ImagePtrCnt = i;
		SaveRawImageList(Name, CastID, ImageW, ImageH, ImageStep, BitCount, ImagePtrList, ImagePtrCnt, SaveTimes);		
	}
	
	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;	
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	if ( FALSE!=bCheckSmoothFilter && NULL!=ImagePtr && nSmoothFilterSize>0 )//NULL!=ImagePtr->非校正時
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PatternStep )
		{		
		case BATCH_GRAB_STEP_4:
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr31, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr31, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr32, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr32, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr33, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr33, BufferPtr, BufferSize);	}
				if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr34, KerSize, BufferPtr) == true )
				{	::memcpy(PatternPtr34, BufferPtr, BufferSize);	}
				JetMemory.free_func(BufferPtr);
			}			
			break;		
		}
	}		

	double Gamma=1.0f;
	Light3DCtrl.GetLight3DImageGamma(CastID, Gamma);
	switch ( PatternStep )
	{	
	case BATCH_GRAB_STEP_4:
		if ( ImageAPI.GrayImage4FrameTo3Phase3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, Per1, PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, Per2, PatternPtr31, PatternPtr32, PatternPtr33, PatternPtr34, Per3, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		if ( NULL != ImagePtr )
		{	ImageAPI.MergeGrayImage4Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, ImagePtr);	}
		break;	
	}

	const int   nVoidExpandSize = NoiseParam.PhaseExtendVoid;
	const BOOL  bCheckVoidExpaned = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND);
	//NoiseParam.PhaseExtendVoid = 0;//因為Cuda尚未撰寫, 先行關閉
	if ( FALSE!=bCheckVoidExpaned && nVoidExpandSize > 0 && NULL!=ImagePtr  )
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nVoidExpandSize*2+1;
		const int    nVoidExpandIterCount = 1;		
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
		{	
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, ImageStep, MaskPtr, nVoidExpandSize, nVoidExpandIterCount, BufferPtr) == true )
			{	::memcpy(MaskPtr, BufferPtr, BufferSize);	}
			JetMemory.free_func(BufferPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcPhasePeriod2Exp2(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr, int SaveTimes)//計算相位-周期x2 曝光x2
{
	if ( NULL == PhasePtr ) { return false; }
	const char fnName[] = "CCaliPaneAlign::CalcPhasePeriod2Exp2";
	CString       str;	
	size_t        i=0;
	long          idx=0;
	double        Per1=8, Per2=12, PerN=0;
	IMAGE_PTR PatternPtr11=NULL;	
	IMAGE_PTR PatternPtr12=NULL;	
	IMAGE_PTR PatternPtr13=NULL;	
	IMAGE_PTR PatternPtr14=NULL;	
	IMAGE_PTR PatternPtr15=NULL;
	IMAGE_PTR PatternPtr16=NULL;
	IMAGE_PTR PatternPtr21=NULL;	
	IMAGE_PTR PatternPtr22=NULL;	
	IMAGE_PTR PatternPtr23=NULL;	
	IMAGE_PTR PatternPtr24=NULL;	
	IMAGE_PTR PatternPtr25=NULL;
	IMAGE_PTR PatternPtr26=NULL;
	IMAGE_PTR PatternPtr11_2=NULL;	
	IMAGE_PTR PatternPtr12_2=NULL;	
	IMAGE_PTR PatternPtr13_2=NULL;	
	IMAGE_PTR PatternPtr14_2=NULL;	
	IMAGE_PTR PatternPtr15_2=NULL;
	IMAGE_PTR PatternPtr16_2=NULL;
	IMAGE_PTR PatternPtr21_2=NULL;	
	IMAGE_PTR PatternPtr22_2=NULL;	
	IMAGE_PTR PatternPtr23_2=NULL;	
	IMAGE_PTR PatternPtr24_2=NULL;	
	IMAGE_PTR PatternPtr25_2=NULL;
	IMAGE_PTR PatternPtr26_2=NULL;
	std::vector<IMAGE_PTR*> PtrList;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const SLICE_FUNC_MODE SliceFuncMode = GetGrabSliceFuncMode();
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);		

	TPhaseNoiseParam NoiseParam;	
	GetPhaseNoiseParam(NoiseParam);

	idx = RingIndex;
	idx = idx-ImageCount;
	idx = idx+m_CameraImageReceieveCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT1, str);	
	Per1 = ::_tcstod(str, NULL);
	CWnd::GetDlgItemText(CALIALIGN_PHASE_PERIOD_EDIT2, str);	
	Per2 = ::_tcstod(str, NULL);

	PtrList.clear();
	const size_t IdxBegin=idx;
	const int ExpTime1 = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT);
	const int ExpTime2 = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT2);
	if ( PatternStep >= BATCH_GRAB_STEP_4 )
	{
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )//SLICE_FUNC_3D_2_2STEP_2EXP
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
			PtrList.push_back(&PatternPtr23_2);
		}
		else if ( SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr13_2);
			PtrList.push_back(&PatternPtr14_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
			PtrList.push_back(&PatternPtr23_2);
			PtrList.push_back(&PatternPtr24_2);
		}
		else if ( SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			PtrList.push_back(&PatternPtr25);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr13_2);
			PtrList.push_back(&PatternPtr14_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
			PtrList.push_back(&PatternPtr23_2);
			PtrList.push_back(&PatternPtr24_2);
			PtrList.push_back(&PatternPtr25_2);	
		}		
		else if ( SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			PtrList.push_back(&PatternPtr25);
			PtrList.push_back(&PatternPtr26);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr13_2);
			PtrList.push_back(&PatternPtr14_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
			PtrList.push_back(&PatternPtr23_2);
			PtrList.push_back(&PatternPtr24_2);
			PtrList.push_back(&PatternPtr25_2);	
			PtrList.push_back(&PatternPtr26_2);
		}	
		else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr13_2);
			PtrList.push_back(&PatternPtr14_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
		}
		else
		{
			PtrList.push_back(&PatternPtr11);
			PtrList.push_back(&PatternPtr12);
			PtrList.push_back(&PatternPtr13);
			PtrList.push_back(&PatternPtr14);
			PtrList.push_back(&PatternPtr21);
			PtrList.push_back(&PatternPtr22);
			PtrList.push_back(&PatternPtr23);
			PtrList.push_back(&PatternPtr24);
			//Second Exposure
			PtrList.push_back(&PatternPtr11_2);
			PtrList.push_back(&PatternPtr12_2);
			PtrList.push_back(&PatternPtr13_2);
			PtrList.push_back(&PatternPtr14_2);
			PtrList.push_back(&PatternPtr21_2);
			PtrList.push_back(&PatternPtr22_2);
			PtrList.push_back(&PatternPtr23_2);
			PtrList.push_back(&PatternPtr24_2);
		}
	}
	const size_t PtrCount=PtrList.size();
	for ( i=0; i<PtrCount; i++ )
	{
		IMAGE_PTR *Ptr=PtrList[i];
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, *Ptr) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;		
		m_CameraImageReceieveCount ++;			
	}
	const size_t IdxEnd=idx;
	const size_t IdxCount=IdxEnd-IdxBegin;
	//將圖像次序相反-20161011-改在DLP的樣板圖片次序顛倒
	//ImageAPI.SwapImagePtr5(PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr15, true);
	//ImageAPI.SwapImagePtr5(PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, true);
	//ImageAPI.SwapImagePtr5(PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, PatternPtr15_2, true);
	//ImageAPI.SwapImagePtr5(PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, PatternPtr24_2, PatternPtr25_2, true);

	if ( true == SaveRaw )
	{
		int       ImagePtrCnt=0;
		IMAGE_SIZE BitCount = 8;
		IMAGE_PTR ImagePtrList[64];
		CString Name = _T("Pattern4S2P2Exp");
		::memset(ImagePtrList, 0x00, sizeof(ImagePtrList));
		i = 0;
		ImagePtrList[i++] = PatternPtr11;	ImagePtrList[i++] = PatternPtr12;	ImagePtrList[i++] = PatternPtr13;	ImagePtrList[i++] = PatternPtr14;	ImagePtrList[i++] = PatternPtr15;	ImagePtrList[i++] = PatternPtr16;
		ImagePtrList[i++] = PatternPtr21;	ImagePtrList[i++] = PatternPtr22;	ImagePtrList[i++] = PatternPtr23;	ImagePtrList[i++] = PatternPtr24;	ImagePtrList[i++] = PatternPtr25;	ImagePtrList[i++] = PatternPtr26;
		ImagePtrList[i++] = PatternPtr11_2;	ImagePtrList[i++] = PatternPtr12_2;	ImagePtrList[i++] = PatternPtr13_2;	ImagePtrList[i++] = PatternPtr14_2;	ImagePtrList[i++] = PatternPtr15_2;	ImagePtrList[i++] = PatternPtr16_2;
		ImagePtrList[i++] = PatternPtr21_2;	ImagePtrList[i++] = PatternPtr22_2;	ImagePtrList[i++] = PatternPtr23_2;	ImagePtrList[i++] = PatternPtr24_2;	ImagePtrList[i++] = PatternPtr25_2;	ImagePtrList[i++] = PatternPtr26_2;
		ImagePtrCnt = i;
		SaveRawImageList(Name, CastID, ImageW, ImageH, ImageStep, BitCount, ImagePtrList, ImagePtrCnt, SaveTimes);		
	}	

	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;	
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	//if ( FALSE!=bCheckSmoothFilter && NULL!=ImagePtr && nSmoothFilterSize>0 )//NULL!=ImagePtr->非校正時
	if ( FALSE!=bCheckSmoothFilter && nSmoothFilterSize>0 )//連校正也平均化
	{
		IMAGE_PTR    BufferPtr = NULL;		
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PatternStep )
		{		
		case BATCH_GRAB_STEP_4:
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
				{
				}
				else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22, BufferPtr, BufferSize);	}					

					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21_2, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22_2, BufferPtr, BufferSize);	}					
				}
				else
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr23, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr23, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr24, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr24, BufferPtr, BufferSize);	}

					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr21_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr21_2, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr22_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr22_2, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr23_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr23_2, BufferPtr, BufferSize);	}
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PatternPtr24_2, KerSize, BufferPtr) == true )
					{	::memcpy(PatternPtr24_2, BufferPtr, BufferSize);	}
				}
				JetMemory.free_func(BufferPtr);
			}			
			break;		
		}
	}		
	
	double Gamma=1.0f;
	bool   bCalcPhase=true;
	const bool bOpenMP = true;
	Light3DCtrl.GetLight3DImageGamma(CastID, Gamma);		
	if ( NULL == CastParamPtr )	{	bCalcPhase = true; }
	else{	bCalcPhase = false; }
	switch ( PatternStep )
	{	
	case BATCH_GRAB_STEP_4:			
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
		{
			if ( true == bCalcPhase )
			{
				if (ImageAPI.GrayImage221FrameTo2Phase2Exp3(ImageW, ImageH, ImageStep,
					PatternPtr11, PatternPtr12, PatternPtr11_2, PatternPtr12_2, Per1, ExpTime1, ExpTime1,
					PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, Per2, ExpTime2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false)
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}			
			}
		}
		else if ( SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode )
		{			
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage44GCFrameTo2Phase2Exp3(ImageW, ImageH, ImageStep, 
					PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, Per1, ExpTime1, ExpTime1,
					PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, PatternPtr24_2, Per2, ExpTime2, ExpTime2,
					Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
					return false;
				}
			}
		}
		else if ( SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode )
		{			
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage45GCFrameTo2Phase2Exp3(ImageW, ImageH, ImageStep, 
					PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, Per1, ExpTime1, ExpTime1,
					PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, PatternPtr24_2, PatternPtr25_2, Per2, ExpTime2, ExpTime2,
					Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
					return false;
				}
			}
		}
		else if ( SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode )
		{			
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage46GCFrameTo2Phase2Exp3(ImageW, ImageH, ImageStep, 
					PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, Per1, ExpTime1, ExpTime1,
					PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr25, PatternPtr26, PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, PatternPtr24_2, PatternPtr25_2, PatternPtr26_2, Per2, ExpTime2, ExpTime2,
					Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
					return false;
				}
			}
		}
		else if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_2STEP_2EXP==SliceFuncMode )
		{	
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage42FrameTo2Phase2Exp3(ImageW, ImageH, ImageStep, 
					PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, Per1, ExpTime1, ExpTime1,
					PatternPtr21, PatternPtr22, PatternPtr21_2, PatternPtr22_2, Per2, ExpTime2, ExpTime2, Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
					return false;
				}
			}
		}
		else
		{
			if ( true == bCalcPhase )
			{
				if ( ImageAPI.GrayImage4FrameTo2Phase2Exp3(ImageW, ImageH, ImageStep, 
					PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, Per1, 
					PatternPtr21, PatternPtr22, PatternPtr23, PatternPtr24, PatternPtr21_2, PatternPtr22_2, PatternPtr23_2, PatternPtr24_2, Per2, 
					Gamma, ZeroPhasePtr, NoiseParam, MaskPtr, PhasePtr, PerN) == false )		
				{
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());			
					return false;
				}		
			}
		}
		if ( NULL != ImagePtr )
		{	ImageAPI.MergeGrayImage8Frame3(ImageW, ImageH, ImageStep, PatternPtr11, PatternPtr12, PatternPtr13, PatternPtr14, PatternPtr11_2, PatternPtr12_2, PatternPtr13_2, PatternPtr14_2, ImagePtr);	}
		break;	
	}

	if ( NULL != CastParamPtr )
	{		
		CastParamPtr->ExpCount = 2;
		CastParamPtr->PerA = Per1;
		CastParamPtr->PerB = Per2;	
		CastParamPtr->ImageW = ImageW;
		CastParamPtr->ImageH = ImageH;
		CastParamPtr->ImageStep = ImageStep;
		CastParamPtr->ImageCount = IdxCount;		
		CastParamPtr->ExpTimeA = ExpTime1;
		CastParamPtr->ExpTimeB = ExpTime2;
		CastParamPtr->ExpTimeC = ExpTime1;
		CastParamPtr->ExpTimeD = ExpTime2;
		CastParamPtr->Gamma = Gamma;
		AOIDataCollect.CheckSliceFuncModeDecodePhaseModeExp2(IdxCount, SliceFuncMode, CastParamPtr->DecodeMode);

		CastParamPtr->PtrA1 = PatternPtr11;
		CastParamPtr->PtrA2 = PatternPtr12;
		CastParamPtr->PtrA3 = PatternPtr13;
		CastParamPtr->PtrA4 = PatternPtr14;
		CastParamPtr->PtrA5 = PatternPtr15;
		CastParamPtr->PtrB1 = PatternPtr21;
		CastParamPtr->PtrB2 = PatternPtr22;
		CastParamPtr->PtrB3 = PatternPtr23;
		CastParamPtr->PtrB4 = PatternPtr24;
		CastParamPtr->PtrB5 = PatternPtr25;
		CastParamPtr->PtrB6 = PatternPtr26;

		CastParamPtr->PtrC1 = PatternPtr11_2;
		CastParamPtr->PtrC2 = PatternPtr12_2;
		CastParamPtr->PtrC3 = PatternPtr13_2;
		CastParamPtr->PtrC4 = PatternPtr14_2;
		CastParamPtr->PtrC5 = PatternPtr15_2;
		CastParamPtr->PtrD1 = PatternPtr21_2;
		CastParamPtr->PtrD2 = PatternPtr22_2;
		CastParamPtr->PtrD3 = PatternPtr23_2;
		CastParamPtr->PtrD4 = PatternPtr24_2;
		CastParamPtr->PtrD5 = PatternPtr25_2;		
		CastParamPtr->PtrD6 = PatternPtr26_2;
		return true;
	}

	const int   nVoidExpandSize = NoiseParam.PhaseExtendVoid;
	const BOOL  bCheckVoidExpaned = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND);
	//NoiseParam.PhaseExtendVoid = 0;//因為Cuda尚未撰寫, 先行關閉
	if ( FALSE!=bCheckVoidExpaned && nVoidExpandSize > 0 && NULL!=ImagePtr  )
	{
		IMAGE_PTR    BufferPtr = NULL;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
		const int    KerSize = nVoidExpandSize*2+1;
		const int    nVoidExpandIterCount = 1;		
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
		{	
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, ImageStep, MaskPtr, nVoidExpandSize, nVoidExpandIterCount, BufferPtr) == true )
			{	::memcpy(MaskPtr, BufferPtr, BufferSize);	}
			JetMemory.free_func(BufferPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnViewResetBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsError=false;
	CString CtrlBoardErr;	
	LightCtrlBoard.CheckLightCtrlBoardError(IsError, CtrlBoardErr);
	if (true == IsError)
	{
		CString str=CString(_T("Light Ctrl Board Error\n"))+CtrlBoardErr; 
		JetAPI::ShowMessageBox(str);
	}

	LockUIWnd(false);
	ClearLogListBox();
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
	CameraCtrl.StopAllCameraGrab();
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(m_CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(m_CameraID);
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawBigInfoWnd()//繪製大訊息視窗
{
	CWnd *pWnd = this->GetDlgItem(CALIALIGN_BIG_INFO_WND);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
	{	return ;	}
	
	CClientDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( hDC == NULL ) { return ; }

	CALIALIGN_MANIPULATE_MODE ManipulateMode = (CALIALIGN_MANIPULATE_MODE)JetAPI::GetComboxCurSelData(this->m_ManipulateCombox);
	switch ( ManipulateMode )
	{	
	case CALIALIGN_MANIPULATE_RECT:		
		break;
	case CALIALIGN_MANIPULATE_GRID:
		this->DrawWorstGridLine(pWnd, hDC);
		break;
	case CALIALIGN_MANIPULATE_FACTOR_GRID:
		break;
	}

	CALIBRATION_MODE CalibrationMode = GetCalibrationMode();
	switch ( CalibrationMode )
	{
	case CALIBRATION_2D_LIGHT_CURRENT:	
		Draw3DCastCurrentInfo(pWnd, hDC);				
		break;
	case CALIBRATION_3D_CAST_CURRENT:
	case CALIBRATION_3D_CAST_CURRENT_RED:
	case CALIBRATION_3D_CAST_CURRENT_GRN:
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		Draw3DCastCurrentInfo(pWnd, hDC);				
		break;
	}	
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::CreateImageTargetWndMemDC()//繪製目標影像的記憶體圖像	
{
	if ( NULL == m_ShowBuffer ) { return; }
	HDC hMemDC = this->m_ImageTargetWndMemDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	COLORREF clrBK = 0xAFFFFF;
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageTargetWndRect, hBrush);
		::DeleteObject(hBrush);
	}
	if ( ImageAPI.DrawImageToDC(hMemDC, m_ImgTargetW, m_ImgTargetH, m_ImgTargetStep, m_BitCount, m_ShowBuffer1, m_ImageTargetWndRect, m_ImageTargetOffset, m_ImageTargetZoom, clrBK) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawImageTargetWnd()//繪製目標影像視窗
{
	if ( m_ImageTargetWnd.GetSafeHwnd() == NULL ) { return; }
	CString  str;	
	CClientDC dc(&m_ImageTargetWnd);	
	HDC hDC = dc.GetSafeHdc();	
	HDC hBKDC = m_ImageTargetWndMemDC.GetSafeHdc();	
	::IntersectClipRect(hDC, this->m_ImageTargetWndRect.left, this->m_ImageTargetWndRect.top, m_ImageTargetWndRect.right, m_ImageTargetWndRect.bottom);	

	::BitBlt(hDC, 0, 0, m_ImageTargetWndRect.right, m_ImageTargetWndRect.bottom, hBKDC, 0, 0, SRCCOPY );
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawWorstGridLine(CWnd *pWnd, HDC hDC)//繪區域格子
{
	size_t  i=0, idx=0;	
	CString str;
	int     ClrIdx=0;
	int     TextX=0, TextY=0;
	COLORREF clrText=0x808000;
	COLORREF clrOldText=0x00;
	COLORREF clrRed = 0x0000FF;
	COLORREF clrGrn = 0x00AF00;
	COLORREF clrBlu = 0xFF0000;
	COLORREF clrBK = 0xFFFFFF;
	COLORREF clrOldBK = 0x000000;
	RECT    GridRect={0};
	double  Ave = 0;	
	const int     FontH = 48;
	const int     FontW = 48;
	TImageStat  *ImageGridPtr=NULL;	
	const size_t GridCount = m_ImageGridList.size();//m_ImageGridRows*m_ImageGridCols;

	HBRUSH  hBrush = NULL;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = 56;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);		
	clrOldText = ::SetTextColor(hDC, clrText);
	
	clrOldBK = ::SetBkColor(hDC, clrBK);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	pWnd->GetClientRect(&GridRect);
	hBrush = ::CreateSolidBrush(clrBK);
	::FillRect(hDC, &GridRect, hBrush);
	::DeleteObject(hBrush); hBrush = NULL;	
	
	Ave = 0;
	idx = GridCount;	
	for ( i=0; i<GridCount; i++ )
	{
		ImageGridPtr = &(m_ImageGridList[i]);
		if ( NULL == ImageGridPtr ) { continue; }
		Ave += ImageGridPtr->m_Ave;
		if ( (ImageGridPtr->m_State&OBJECT_STATE_MINIMUM)==OBJECT_STATE_MINIMUM )//最小值
		{	idx = i;	}
	}
	if ( GridCount > 0 ) 
	{	Ave = Ave/GridCount; }
	
	if ( idx != GridCount )
	{
		ImageGridPtr = &(m_ImageGridList[idx]);
		ClrIdx=static_cast<int>(ImageGridPtr->m_Ratio/10.0);		
		if ( ClrIdx < 0 ) { ClrIdx = 0; }
		else if ( ClrIdx > 9 ) { ClrIdx = 9; }
		clrBK = GridColorList[ClrIdx];
		//::SetBkColor(hDC, clrBK);

		TextX=GridRect.left+FontW/4;
		TextY=GridRect.top +FontH/4;

		//str.Format(_T("%.2f "), ImageGridPtr->m_Ave);	
		str.Format(_T("%.2f "), Ave);	
		::SetTextColor(hDC, clrBlu);
		::TextOut(hDC, TextX, TextY, str, str.GetLength());
		TextY += FontH;			
		
		str.Format(_T("%.2f%%"), ImageGridPtr->m_Ratio);	
		if (ImageGridPtr->m_Ratio < m_GrayMinRatio )
		{	::SetTextColor(hDC, clrRed); }
		else
		{	::SetTextColor(hDC, clrGrn); }
		::TextOut(hDC, TextX, TextY, str, str.GetLength());
		TextY += FontH;
	}	 
	
	::SetBkColor(hDC, clrOldBK);
	::SetTextColor(hDC, clrOldText);
	::SelectObject(hDC, hOldFont);	
	::DeleteObject(hFont);	hFont=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::DrawBigInfoText(CWnd *pWnd, HDC hDC, CString str)//影像灰階
{
	size_t  i=0, idx=0;	
	int     ClrIdx=0;
	int     TextX=0, TextY=0;
	COLORREF clrText=0x808000;
	COLORREF clrOldText=0x00;
	COLORREF clrRed = 0x0000FF;
	COLORREF clrGrn = 0x00AF00;
	COLORREF clrBlu = 0xFF0000;
	COLORREF clrBK = 0xFFFFFF;
	COLORREF clrOldBK = 0x000000;
	RECT    GridRect={0};
	double  Ave = 0;	
	const int     FontH = 48;
	const int     FontW = 48;
	
	HBRUSH  hBrush = NULL;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = 56;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);		
	clrOldText = ::SetTextColor(hDC, clrText);
	
	clrOldBK = ::SetBkColor(hDC, clrBK);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	pWnd->GetClientRect(&GridRect);
	hBrush = ::CreateSolidBrush(clrBK);
	::FillRect(hDC, &GridRect, hBrush);
	::DeleteObject(hBrush); hBrush = NULL;		
	
	::SetTextColor(hDC, clrBlu);
	::TextOut(hDC, TextX, TextY, str, str.GetLength());	
	
	::SetBkColor(hDC, clrOldBK);
	::SetTextColor(hDC, clrOldText);
	::SelectObject(hDC, hOldFont);	
	::DeleteObject(hFont);	hFont=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::Draw3DCastCurrentInfo(CWnd *pWnd, HDC hDC)//3D投光電流訊息
{
	size_t  i=0, idx=0;	
	CString str;
	int     ClrIdx=0;	
	int     TextX=0, TextY=0;
	COLORREF clrText=0x808000;
	COLORREF clrOldText=0x00;
	COLORREF clrRed = 0x0000AF;
	COLORREF clrGrn = 0x00AF00;
	COLORREF clrBlu = 0xAF0000;
	COLORREF clrGry = 0x808080;
	COLORREF clrBK = 0xFFFFFF;
	COLORREF clrOldBK = 0x000000;
	RECT    GridRect={0};
	double  Ave = 0;	
	int     Current = 0;
	int     CurrentGray = 0;
	const int     FontH = 48;
	const int     FontW = 48;
	
	HBRUSH  hBrush = NULL;
	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = 56;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	

	hFont = CreateFontIndirect(&LogFont);		
	clrOldText = ::SetTextColor(hDC, clrText);
	
	clrOldBK = ::SetBkColor(hDC, clrBK);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	pWnd->GetClientRect(&GridRect);
	hBrush = ::CreateSolidBrush(clrBK);
	::FillRect(hDC, &GridRect, hBrush);
	::DeleteObject(hBrush); hBrush = NULL;		
	

	CALIBRATION_MODE CalibrationMode = GetCalibrationMode();
	switch ( CalibrationMode )
	{
	case CALIBRATION_3D_CAST_CURRENT:
		Current = m_3DCastCurrent;
		CurrentGray = m_3DCastCurrentGray;
		::SetTextColor(hDC, clrGry);
		break;
	case CALIBRATION_3D_CAST_CURRENT_RED:
		Current = m_3DCastCurrent;
		CurrentGray = m_3DCastCurrentGray;
		::SetTextColor(hDC, clrRed);
		break;
	case CALIBRATION_3D_CAST_CURRENT_GRN:
		Current = m_3DCastCurrent;
		CurrentGray = m_3DCastCurrentGray;
		::SetTextColor(hDC, clrGrn);
		break;
	case CALIBRATION_3D_CAST_CURRENT_BLU:
		Current = m_3DCastCurrent;
		CurrentGray = m_3DCastCurrentGray;
		::SetTextColor(hDC, clrBlu);
		break;
	default:
		Current = m_2DLEDCurrent;
		CurrentGray = m_2DLEDCurrentGray;
		::SetTextColor(hDC, clrGry);
		break;
	}		
	str.Format(_T("PWR:%d"), Current);
	::TextOut(hDC, TextX, TextY, str, str.GetLength());	

	TextY += FontH;			
	str.Format(_T("Gray:%d"),CurrentGray);
	::SetTextColor(hDC, clrGry);
	::TextOut(hDC, TextX, TextY, str, str.GetLength());	

	::SetBkColor(hDC, clrOldBK);
	::SetTextColor(hDC, clrOldText);
	::SelectObject(hDC, hOldFont);	
	::DeleteObject(hFont);	hFont=NULL;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetGridGoBtn() 
{
	// TODO: Add your control notification handler code here
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetGridPosX, CaliParam.m_TargetGridPosY, CaliParam.m_TargetGridPosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	this->m_CameraExposureTime_us = CaliParam.m_TargetGridExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetGridSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("Do you want to set current position as grid target position?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetGridPosX = PosX;
	CaliParam.m_TargetGridPosY = PosY;
	CaliParam.m_TargetGridPosZ = PosZ;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetGridExpBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CaliParam.m_TargetGridExpTime_us = this->GetDlgItemInt(CALIALIGN_TARGET_GRID_EXP_EDIT);
	this->m_CameraExposureTime_us = CaliParam.m_TargetGridExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetWhiteGoBtn() 
{
	// TODO: Add your control notification handler code here	
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetWhitePosX, CaliParam.m_TargetWhitePosY, CaliParam.m_TargetWhitePosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	this->m_CameraExposureTime_us = CaliParam.m_TargetWhiteExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetWhiteSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("Do you want to set current position as white target position?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetWhitePosX = PosX;
	CaliParam.m_TargetWhitePosY = PosY;
	CaliParam.m_TargetWhitePosZ = PosZ;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetWhiteExpBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CaliParam.m_TargetWhiteExpTime_us = this->GetDlgItemInt(CALIALIGN_TARGET_WHITE_EXP_EDIT);
	this->m_CameraExposureTime_us = CaliParam.m_TargetWhiteExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetHeightGoBtn() 
{
	// TODO: Add your control notification handler code here	
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetHeightPosX, CaliParam.m_TargetHeightPosY, CaliParam.m_TargetHeightPosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
	this->m_CameraExposureTime_us = CaliParam.m_TargetHeightExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetHeightSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("Do you want to set current position as height target position?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetHeightPosX = PosX;
	CaliParam.m_TargetHeightPosY = PosY;
	CaliParam.m_TargetHeightPosZ = PosZ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetHeightExpBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CaliParam.m_TargetHeightExpTime_us = this->GetDlgItemInt(CALIALIGN_TARGET_HEIGHT_EXP_EDIT);
	this->m_CameraExposureTime_us = CaliParam.m_TargetHeightExpTime_us;
	this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternPhaseMeasureChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = this->IsDlgButtonChecked(CALIALIGN_PATTERN_PHASE_MEASURE_CHK);
	if ( FALSE==bCheck )
	{		
		SetCalibrationMode(CALIBRATION_STOP);		
		this->LockUIWnd(false);
		this->RedrawWnd();
		return ;
	}
	JetAPI::SetComboxCurSel(this->m_ManipulateCombox, CALIALIGN_MANIPULATE_PHASE_LINE);
	SetCalibrationMode(CALIBRATION_PATTERN_PHASE_MEASURE);	
	this->LockUIWnd(true);
	this->RedrawWnd();
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSaveImageBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	

	CString filename = dialog.GetPathName();
	if ( ImageAPI.SaveBMPImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, true) == false )
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternHeightFactorShowBtn() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseWnd   Wnd;
	SPACE_PTR  PhaseFactorPtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);

	m_Light3DCastID = (LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox);
	if ( Light3DCtrl.GetLight3DPhaseFactor(m_Light3DCastID, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhaseFactorPtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return ;
	}

	if ( NULL == PhaseFactorPtr ) 
	{	return; }
	Wnd.SetSpaceBuffer(ImageW, ImageH, ImageStep, NULL, PhaseFactorPtr, PHASE_TO_IMAGE_DYNAMIC_SCALE, FALSE);
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternImageBtn() 
{
	// TODO: Add your control notification handler code here	
	//const bool bShowMsg = true;
	//if ( Check3DCastID(bShowMsg) == false ) { return ; }

	CString str;
	if ( CreateRawImageFolder() == false ) { return; }
	CALIBRATION_MODE CalibrationMode = CALIBRATION_3D_MODEL_TEST;	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
	if ( this->RetrieveStagePosition() == false )
	{	return;	}
	Light3DCtrl.ClearAllLight3DPatternList();//確保每次都重新設定樣板
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return ; }	

	SetCalibrationMode(CalibrationMode);	
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return ;
	}
	FocusToEditCtrl();
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnVerifyZeroPlaneBtn()
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( CreateRawImageFolder() == false ) { return; }
	CALIBRATION_MODE CalibrationMode = CALIBRATION_VERIFY_ZERO_PLANE;	
	JetAPI::SetComboxCurSel(m_ManipulateCombox, CALIALIGN_MANIPULATE_RECT);	
	if ( this->RetrieveStagePosition() == false )
	{	return;	}
	Light3DCtrl.ClearAllLight3DPatternList();//確保每次都重新設定樣板
	if ( this->ConfigGrabParam(CalibrationMode) == false )
	{	return ; }	

	if ( true == m_Multi3DCastID )
	{
		CString      strValue;
		CString      strLabel;
		CString      strCaption;	
		TBarcodeInfo BarcodeInfo;
		CInputBoxWnd InputBox;
		TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();

		m_MultiZeroPlaneGapRatio = SysParam.m_SpaceVerifyMultiCastGapRatio;
		m_MultiZeroPlaneGapThreshold = SysParam.m_SpaceVerifyMultiCastGapThreshold;		

		strCaption = _T("Set Multi Zero Plane Gap Threshold");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strLabel = _T("Gap Threshold");
		strValue.Format(_T("%.2f"), m_MultiZeroPlaneGapThreshold);
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return ;  }
		m_MultiZeroPlaneGapThreshold = ::_ttof(InputBox.m_DataEdit1);
		if ( m_MultiZeroPlaneGapThreshold < 0 ) { m_MultiZeroPlaneGapThreshold = 0; }
		SysParam.m_SpaceVerifyMultiCastGapThreshold=m_MultiZeroPlaneGapThreshold;

		strCaption = _T("Set Multi Zero Plane Gap Ratio");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strLabel = _T("Gap Ratio");
		strValue.Format(_T("%.2f"), m_MultiZeroPlaneGapRatio);
		InputBox.SetParam1(strCaption, strLabel, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return ;  }
		m_MultiZeroPlaneGapRatio = ::_ttof(InputBox.m_DataEdit1);
		if ( m_MultiZeroPlaneGapRatio < 0 ) { m_MultiZeroPlaneGapRatio = 0; }
		SysParam.m_SpaceVerifyMultiCastGapRatio=m_MultiZeroPlaneGapRatio;
	}

	SetCalibrationMode(CalibrationMode);	
	if ( this->ExecGrabFirst() == false )
	{
		SetCalibrationMode(CALIBRATION_STOP);
		return ;
	}
	FocusToEditCtrl();
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPatternZeroPlaneShowBtn() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseWnd   Wnd;
	PHASE_PTR  PhaseFactorPtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);
	const int PhaseMode = CCameraCtrl::GetPhaseMode(m_PatternStep, m_PhaseID);
	this->m_Light3DCastID = (LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox);
	if ( Light3DCtrl.GetLight3DPhaseZero(m_Light3DCastID, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhaseFactorPtr) == false )
	{	
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return ;
	}

	if ( NULL == PhaseFactorPtr ) 
	{	return; }
	Wnd.SetPhaseBuffer(ImageW, ImageH, ImageStep, NULL, PhaseFactorPtr, PHASE_TO_IMAGE_DYNAMIC_SCALE, FALSE);
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnMotionWndBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( m_MotionCtrlWnd.GetSafeHwnd() == NULL  ) { return; }
//	MotionCtrlWnd.ShowWindow(SW_SHOW);
	BOOL bVisible = m_MotionCtrlWnd.IsWindowVisible();
	if ( FALSE == bVisible )
	{	m_MotionCtrlWnd.ShowWindow(SW_SHOW); }
	else
	{	m_MotionCtrlWnd.ShowWindow(SW_HIDE); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::AddLogListBox(LPCTSTR str)//加入紀錄列表視窗
{
	//this->m_LogListBox.AddString(str);
	int i = 0;
	const int ListBoxCount = this->m_LogListBox.GetCount();		
	if ( ListBoxCount > 1024 )	
	{	
		m_LogListBox.SetRedraw(FALSE);	
		for (i=ListBoxCount-1; i>=8; i--)
		{	m_LogListBox.DeleteString(i);	}
		m_LogListBox.SetRedraw(TRUE);		
	}
	this->m_LogListBox.InsertString(0, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::ClearLogListBox()//清除紀錄列表視窗
{
	m_LogListBox.ResetContent();
//	int i = 0;
//	const int ListBoxCount = this->m_LogListBox.GetCount();		
//	for (i=ListBoxCount-1; i>=0; i--)
//	{	m_LogListBox.DeleteString(i);	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnDeltaposMotionZPitchSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here	
	double ZPos = 0;
	double PosX=0, PosY=0, PosZ=0;
	double ZPitch = CWnd::GetDlgItemInt(CALIALIGN_MOTION_Z_PITCH_EDIT);	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( pNMUpDown->iDelta > 0 ) 
	{	ZPos = PosZ+ZPitch;	}
	else
	{	ZPos = PosZ-ZPitch;	}
	if ( MotionCtrlPtr->MoveTo(AXIS_Z, ZPos, MOTION_MOVING_NORMAL) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}	
	this->StartReGrab(TRUE);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnXYZOrgBtn() 
{
	// TODO: Add your control notification handler code here
	if ( MotionCtrlPtr->XYZMoveTo(0, 0, 0) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}	
	FocusToEditCtrl();
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPCBInBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->ExecPCBIn_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPCBOutBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->ExecPCBOut_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPCBBackBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->ExecPCBBack_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPCBClampOnBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->WriteConveryerClamp_LA(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPCBClampOffBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->WriteConveryerClamp_LA(false) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetCapOnBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	if ( PlcCtrlPtr->TurnOnOffTargetCap(true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnTargetCapOffBtn() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();	
	if ( PlcCtrlPtr->TurnOnOffTargetCap(false) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeFovResolutionCombo() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CAMERA_ID CameraID = (CAMERA_ID)JetAPI::GetComboxCurSelData(m_CameraCombox);
	const int ResMode = JetAPI::GetComboxCurSelData(m_FovResCombox);	

	const double ResScale = AOIDataCollect.GetFovResolutionValueFn(ResMode);	
	const unsigned int ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const unsigned int ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	const double FOVW = ImageW*ResScale;
	const double FOVH = ImageH*ResScale;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();

	str = _T("Do you want to reset Fov Size?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )	
	{
		SetFovSize(FOVW, FOVH);		
		CalcFOVSizeResolution();
		CaliParam.m_FOVWidth_um = FOVW;
		CaliParam.m_FOVHeight_um = FOVH;		
	}
	AOIDataCollect.SetFovResolutionMode(ResMode);
	FocusToEditCtrl();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnViewAllBtn() 
{
	// TODO: Add your control notification handler code here	
	const unsigned int ImageW = this->m_ImageW;
	const unsigned int ImageH = this->m_ImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	this->DrawImageWndMemDC();
	this->RedrawWnd();
	FocusToEditCtrl();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnView1x1Btn() 
{
	// TODO: Add your control notification handler code here
	m_ImageZoom = 1.0;
	this->m_ImageOffset.x = 0;
	this->m_ImageOffset.y = 0;
	this->DrawImageWndMemDC();
	this->RedrawWnd();
	FocusToEditCtrl();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	POINT    WndPt = point;	
	if ( PtInControlWnd(WndPt, CALIALIGN_IMAGE_WND, WndPt) == true )
	{
		TPOINT2D ImagePt=WndPt;	
		TPOINT2D Res, StageCp, StagePt;	
		const IMAGE_SIZE ImageW = this->m_ImageW;
		const IMAGE_SIZE ImageH = this->m_ImageH;	

		Res.x = AOIDataCollect.GetCameraResolutionX(m_CameraID);
		Res.y = AOIDataCollect.GetCameraResolutionY(m_CameraID);
		MotionCtrlPtr->GetCurrentPos(StageCp.x, StageCp.y);
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, Res, ImagePt, StageCp, StagePt);

		if ( MotionCtrlPtr->XYMoveTo(StagePt.x, StagePt.y) == false )
		{	
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
			CDialog::OnRButtonDblClk(nFlags, point);
			return;
		}
		this->m_ImageOffset.x = 0;
		this->m_ImageOffset.y = 0;
		this->StartReGrab(TRUE);
	}
	
	CDialog::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnCameraExposureTimeBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	this->m_CameraExposureTime_us = this->GetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT);	
	FocusToEditCtrl();
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnShowCenterLineChk() 
{
	// TODO: Add your control notification handler code here
	this->m_bShowHorizontalLine = FALSE;
	this->m_bShowVerticalLine = FALSE;	
	CWnd::CheckDlgButton(CALIALIGN_SHOW_HOR_LINE_CHK, m_bShowHorizontalLine);
	CWnd::CheckDlgButton(CALIALIGN_SHOW_VER_LINE_CHK, m_bShowVerticalLine);
	this->m_bShowCenterLine = CWnd::IsDlgButtonChecked(CALIALIGN_SHOW_CENTER_LINE_CHK);
	FocusToEditCtrl();
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnShowHorLineChk() 
{
	// TODO: Add your control notification handler code here
	this->m_bShowCenterLine = FALSE;
	CWnd::CheckDlgButton(CALIALIGN_SHOW_CENTER_LINE_CHK, m_bShowCenterLine);
	this->m_bShowHorizontalLine = CWnd::IsDlgButtonChecked(CALIALIGN_SHOW_HOR_LINE_CHK);
	FocusToEditCtrl();
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnShowVerLineChk() 
{
	// TODO: Add your control notification handler code here
	this->m_bShowCenterLine = FALSE;
	CWnd::CheckDlgButton(CALIALIGN_SHOW_CENTER_LINE_CHK, m_bShowCenterLine);
	this->m_bShowVerticalLine = CWnd::IsDlgButtonChecked(CALIALIGN_SHOW_VER_LINE_CHK);
	FocusToEditCtrl();
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnHideLineBtn() 
{
	// TODO: Add your control notification handler code here
	this->m_bShowCenterLine = FALSE;
	this->m_bShowHorizontalLine = FALSE;
	this->m_bShowVerticalLine = FALSE;
	CWnd::CheckDlgButton(CALIALIGN_SHOW_CENTER_LINE_CHK, m_bShowCenterLine);
	CWnd::CheckDlgButton(CALIALIGN_SHOW_HOR_LINE_CHK, m_bShowHorizontalLine);
	CWnd::CheckDlgButton(CALIALIGN_SHOW_VER_LINE_CHK, m_bShowVerticalLine);
	FocusToEditCtrl();
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdateStagePosition()//更新機台座標
{	
	double PosX=0, PosY=0, PosZ=0;
	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ) == false )	
	{	return false; }	
	return UpdateStagePositionKernel(PosX, PosY, PosZ);	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::UpdateStagePositionKernel(double PosX, double PosY, double PosZ)//更新機台座標
{
	CString str;
	str.Format(_T("(%.0f, %.0f, %.0f)"), PosX, PosY, PosZ);
	CWnd::SetDlgItemText(CALIALIGN_MOTION_POS_EDIT, str);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Update3DCastCurrentToUI()//更新3D投光的電流值
{	
	LIGHT_3D_CAST_ID Light3DID = (LIGHT_3D_CAST_ID)JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox);
	Update3DCastCurrentToUIKernel(Light3DID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::Update3DCastCurrentToUIKernel(LIGHT_3D_CAST_ID Light3DID)//更新3D投光的電流值
{
	CString str;
	CString str1;
	LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr ) 
	{
		SetDlgItemText(CALIALIGN_3D_CAST_CURRENT_VALUE_EDIT, str);	
		SetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT, str);
		SetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT, str);			
		return false;
	}
	
	int LEDColor=-1;
	int Red=0, Grn=0, Blu=0;
	double FactorMin=0;
	double FactorMax=0;	
	//LEDColor = Light3DPtr->GetLEDColor();
	LEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
	FactorMin = Light3DPtr->GetDLPParam().m_PhaseFactorMin;
	FactorMax = Light3DPtr->GetDLPParam().m_PhaseFactorMax;

	Light3DPtr->GetDLPLEDCurrent(Red, Grn, Blu);
	str1 = _T("Current");
	str1 = LoadMultiLanguageString(str1, str1);
	str.Format(_T("%s: (%d, %d, %d)"), str1, Red, Grn, Blu);	
	SetDlgItemText(CALIALIGN_3D_CAST_CURRENT_VALUE_EDIT, str);

	str.Format(_T("%.0f"), FactorMin);
	SetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT, str);
	str.Format(_T("%.0f"), FactorMax);
	SetDlgItemText(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT, str);
	if ( LEDColor > 0 ) 
	{	JetAPI::SetComboxCurSel(m_PhaseLEDCombox, LEDColor);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::UpdateSliceParamToUI()
{
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombo);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);
	if ( NULL == SliceParamPtr ) { return ; }

	size_t       i=0;
	CString      str;
	unsigned int CurCurrent=0;
	CAMERA_ID    CameraID = SliceParamPtr->SliceCameraID;
	unsigned int ExpTimeus = SliceParamPtr->SliceCameraExpTimeus;
	unsigned int GrayValue = SliceParamPtr->SliceTargetGray;	
	double       GainValue = SliceParamPtr->SliceGainValue;	

	CurCurrent = 0;
	for ( int i=0; i<LED_CHANNEL_COUNT; i++ )
	{
		if ( FN_DISABLE == SliceParamPtr->SliceLightTable.LEDChannel[i].OnOffState ) { continue; }			
		CurCurrent = SliceParamPtr->SliceLightTable.LEDChannel[i].PowerValue;
		break;
	}	

	JetAPI::SetComboxCurSel(m_CameraCombox, CameraID);	
	//this->m_CameraExposureTime_us = ExpTimeus;	
	//this->SetDlgItemInt(CALIALIGN_CAMERA_EXPOSURE_TIME_EDIT, m_CameraExposureTime_us);	
	str.Format(_T("%.2f"), GainValue);
	CWnd::SetDlgItemText(CALIALIGN_LIGHT_CURRENT_GAIN_EDIT, str);
	CWnd::SetDlgItemInt(CALIALIGN_LIGHT_CURRENT_GRAY_EDIT, GrayValue);	
	CWnd::SetDlgItemInt(CALIALIGN_LIGHT_CURRENT_EXP_EDIT, ExpTimeus);
	CWnd::SetDlgItemInt(CALIALIGN_LIGHT_CURRENT_SET_EDIT, CurCurrent);
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeSliceCombo() 
{
	// TODO: Add your control notification handler code here
	FocusToEditCtrl();
	UpdateSliceParamToUI();
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::FocusToEditCtrl()
{
	JetAPI::FocusCtrlWnd(this, CALIALIGN_IMAGE_FOCUS_AUTO_PITCH_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnDLPExposureTimeBtn() 
{
	// TODO: Add your control notification handler code here	
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const int DLPExposureTime = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT);
	const int DLPExposureTime2 = CWnd::GetDlgItemInt(CALIALIGN_DLP_EXPOSURE_TIME_EDIT2);
	const int MinExposureTime = CLight3DCtrl::GetDLPExposureMinTime(false, true, 1);
	if ( DLPExposureTime < MinExposureTime ) { return ; }
	if ( DLPExposureTime2 < MinExposureTime ) { return ; }	

	CaliParam.m_DLPExposureTime_us = DLPExposureTime;
	CaliParam.m_DLPExposureTime2_us = DLPExposureTime2;
	PhasePtr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_01);	
	if ( NULL != PhasePtr )
	{
		//const int PaddingTime_us = PhasePtr->GetPeriodPaddingTime();
		const int PaddingTime_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime);
		const int PaddingTime2_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime2);
		const int DLPPeriodTime = DLPExposureTime + PaddingTime_us;
		const int DLPPeriodTime2 = DLPExposureTime2 + PaddingTime2_us;
		PhasePtr->GetDLPParam().m_PeriodTime_us = DLPPeriodTime;
		PhasePtr->GetDLPParam().m_ExposureTime_us = DLPExposureTime;
		PhasePtr->GetDLPParam().m_PeriodTime2_us = DLPPeriodTime2;
		PhasePtr->GetDLPParam().m_ExposureTime2_us = DLPExposureTime2;
	//	PhasePtr->SaveDLPParameter();
	}
	PhasePtr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_02);	
	if ( NULL != PhasePtr )
	{
		//const int PaddingTime_us = PhasePtr->GetPeriodPaddingTime();
		const int PaddingTime_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime);
		const int PaddingTime2_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime2);
		const int DLPPeriodTime = DLPExposureTime + PaddingTime_us;
		const int DLPPeriodTime2 = DLPExposureTime2 + PaddingTime2_us;
		PhasePtr->GetDLPParam().m_PeriodTime_us = DLPPeriodTime;
		PhasePtr->GetDLPParam().m_ExposureTime_us = DLPExposureTime;
		PhasePtr->GetDLPParam().m_PeriodTime2_us = DLPPeriodTime2;
		PhasePtr->GetDLPParam().m_ExposureTime2_us = DLPExposureTime2;
	//	PhasePtr->SaveDLPParameter();
	}
	PhasePtr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_03);	
	if ( NULL != PhasePtr )
	{
		//const int PaddingTime_us = PhasePtr->GetPeriodPaddingTime();
		const int PaddingTime_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime);
		const int PaddingTime2_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime2);
		const int DLPPeriodTime = DLPExposureTime + PaddingTime_us;
		const int DLPPeriodTime2 = DLPExposureTime2 + PaddingTime2_us;
		PhasePtr->GetDLPParam().m_PeriodTime_us = DLPPeriodTime;
		PhasePtr->GetDLPParam().m_ExposureTime_us = DLPExposureTime;
		PhasePtr->GetDLPParam().m_PeriodTime2_us = DLPPeriodTime2;
		PhasePtr->GetDLPParam().m_ExposureTime2_us = DLPExposureTime2;
	//	PhasePtr->SaveDLPParameter();
	}
	PhasePtr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_04);	
	if ( NULL != PhasePtr )
	{
		//const int PaddingTime_us = PhasePtr->GetPeriodPaddingTime();
		const int PaddingTime_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime);
		const int PaddingTime2_us = PhasePtr->CalcPeriodPaddingTime(DLPExposureTime2);
		const int DLPPeriodTime = DLPExposureTime + PaddingTime_us;
		const int DLPPeriodTime2 = DLPExposureTime2 + PaddingTime2_us;
		PhasePtr->GetDLPParam().m_PeriodTime_us = DLPPeriodTime;
		PhasePtr->GetDLPParam().m_ExposureTime_us = DLPExposureTime;
		PhasePtr->GetDLPParam().m_PeriodTime2_us = DLPPeriodTime2;
		PhasePtr->GetDLPParam().m_ExposureTime2_us = DLPExposureTime2;
	//	PhasePtr->SaveDLPParameter();
	}
	FocusToEditCtrl();
	this->StartReGrab(TRUE);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::On3DCastCurrentSetBtn() 
{
	// TODO: Add your control notification handler code here
	bool bSetAll=false;
	//const bool bShowMsg = true;
	//if ( Check3DCastID(bShowMsg) == false ) { return; }	
	CString str;
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	LIGHT_3D_CAST_ID CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(this->m_3DCastIDCombox));
	PhasePtr = Light3DCtrl.GetLight3DCastPtr(CastID);	
	if ( NULL == PhasePtr ) 
	{	
		str = _T("Do you want to set all DLP LED Current?");
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return; }
		bSetAll = true; 
		CastID = LIGHT_3D_CAST_01;
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(CastID);	
		if ( NULL == PhasePtr ) 
		{	return ; }
	}
	else
	{	bSetAll = false; }			

	CString WndTxt, Title, Default;
	CInputBoxWnd InputWnd;
	int CurrentRed=0, CurrentGrn=0, CurrentBlu=0;
	const int DLPLEDCurrentID = GetDLPLEDCurrentID();
	PhasePtr->GetDLPLEDCurrent(CurrentRed, CurrentGrn, CurrentBlu);

	WndTxt = _T("Set DLP LED Current");
	WndTxt = LoadMultiLanguageString(WndTxt, WndTxt);

	Title  = _T("Red Current");
	Title = LoadMultiLanguageString(Title, Title);
	Default.Format(_T("%d"), CurrentRed);
	InputWnd.SetParam1(WndTxt, Title, Default);
	if ( InputWnd.DoModal() == IDCANCEL ) { return; }
	CurrentRed = ::_ttoi(InputWnd.m_DataEdit1);
	if ( CurrentRed < 0 ) { return ; }

	Title  = _T("Green Current");
	Title = LoadMultiLanguageString(Title, Title);
	Default.Format(_T("%d"), CurrentGrn);
	InputWnd.SetParam1(WndTxt, Title, Default);
	if ( InputWnd.DoModal() == IDCANCEL ) { return; }
	CurrentGrn = ::_ttoi(InputWnd.m_DataEdit1);
	if ( CurrentGrn < 0 ) { return ; }

	Title  = _T("Blue Current");
	Title = LoadMultiLanguageString(Title, Title);
	Default.Format(_T("%d"), CurrentBlu);
	InputWnd.SetParam1(WndTxt, Title, Default);
	if ( InputWnd.DoModal() == IDCANCEL ) { return; }
	CurrentBlu = ::_ttoi(InputWnd.m_DataEdit1);
	if ( CurrentBlu < 0 ) { return ; }

	if ( false == bSetAll )
	{	PhasePtr->SetDLPLEDCurrent(CurrentRed, CurrentGrn, CurrentBlu, true, DLPLEDCurrentID);		}
	else
	{	Light3DCtrl.SetAllLight3DLEDCurrent(CurrentRed, CurrentGrn, CurrentBlu, DLPLEDCurrentID);	}
	
	FocusToEditCtrl();
	this->Update3DCastCurrentToUI();
	this->StartReGrab(TRUE);
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnLightCurrentSetBtn() 
{
	// TODO: Add your control notification handler code here
	size_t i=0;
	int    SliceFrameIndex = 0;	
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombo);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);	
	if ( NULL == SliceParamPtr) { return ; }		

	CString str;
	unsigned int CurCurrent=0;
	CInputBoxWnd InputWnd;
	CString WndTxt, Title, Default;	

	CurCurrent = 0;
	for ( int i=0; i<LED_CHANNEL_COUNT; i++ )
	{
		if ( FN_DISABLE == SliceParamPtr->SliceLightTable.LEDChannel[i].OnOffState ) { continue; }			
		CurCurrent = SliceParamPtr->SliceLightTable.LEDChannel[i].PowerValue;
		break;
	}	

	WndTxt = _T("Set LED Current");
	WndTxt = LoadMultiLanguageString(WndTxt, WndTxt);

	Title  = _T("Current");
	Title = LoadMultiLanguageString(Title, Title);
	Default.Format(_T("%d"), CurCurrent);
	InputWnd.SetParam1(WndTxt, Title, Default);
	if ( InputWnd.DoModal() == IDCANCEL ) { return; }
	CurCurrent = ::_ttoi(InputWnd.m_DataEdit1);
	if ( CurCurrent < 0 ) { return ; }

	CWnd::GetDlgItemText(CALIALIGN_LIGHT_CURRENT_GAIN_EDIT, str);
	const double GainValue = ::_ttof(str);

	for ( int i=0; i<LED_CHANNEL_COUNT; i++ )
	{
		if ( FN_DISABLE == SliceParamPtr->SliceLightTable.LEDChannel[i].OnOffState ) { continue; }			
		SliceParamPtr->SliceLightTable.LEDChannel[i].PowerValue = (unsigned int)(CurCurrent);		
		SliceParamPtr->SliceGainValue = GainValue;
	}

	FocusToEditCtrl();
	UpdateSliceParamToUI();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangePhaseLedCombox() 
{
	// TODO: Add your control notification handler code here
	CString str;
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const bool ModifiedCaliParam = GetModifiedCaliParam();
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_PhaseLEDCombox);

	if ( true == ModifiedCaliParam )
	{		
		str = _T("Do you wanto to save calibration parameter?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{
			if ( AOIDataCollect.SaveAllCalibrationParameter() == false )
			{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
			SetModifiedCaliParam(false);
		}
	}

	FocusToEditCtrl();
	AOIDataCollect.SetSystemDlpLedColor(DLPLEDColor);
	if ( Light3DCtrl.SetAllLight3DLEDColor(DLPLEDColor) == false )
	{	JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecOnOK()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return true; }

	UpdatePhaseNoiseParamFromUI();
	UpdateSpaceNoiseFilterParamFromUI();
	return true;
}
//-------------------------------------------------------------------------------------//
SLICE_FUNC_MODE CCaliPaneAlign::GetGrabSliceFuncMode() const//取得實際取像的函式模式
{
	return m_SliceParam.SliceFuncMode;
}
//-------------------------------------------------------------------------------------//
CALIBRATION_MODE CCaliPaneAlign::GetCalibrationMode() const
{
	return m_CalibrationMode;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetCalibrationMode(CALIBRATION_MODE Mode)
{
	m_CalibrationMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetCalibrateAllCannel() const
{
	return m_CalibrateAllCannel;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetCalibrateAllCannel(bool bAll)
{
	m_CalibrateAllCannel = bAll;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetCalibrateAllCastID() const
{
	return m_CalibrateAllCastID;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetCalibrateAllCastID(bool bAll)
{
	m_CalibrateAllCastID = bAll;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CCaliPaneAlign::GetLight3DCastID() const
{
	return m_Light3DCastID;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetLight3DCastID(LIGHT_3D_CAST_ID CastID)
{
	m_Light3DCastID = CastID;
}
//-------------------------------------------------------------------------------------//
int CCaliPaneAlign::GetDLPLEDCurrentID() const
{
	int val = JetAPI::GetComboxCurSelData(m_3DCastCurrentIDCombox);
	return val;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetModifiedCaliParam(bool val)
{
	m_ModifiedCaliParam = val;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetModifiedCaliParam() const
{
	return m_ModifiedCaliParam;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::GetSaveRawImage() const
{
	if ( CWnd::IsDlgButtonChecked(CALIALIGN_SAVE_RAW_IMAGE_CHK) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CreateRawImageFolder()//建立原圖資料夾
{
	if ( CreateTempFolder() == false )
	{	return false; }

	CString str;
	CString Folder = GetSaveRawImageFolder();
	if ( JetAPI::CreateFolder(Folder) == false )
	{
		str.Format(_T("Error, Create Raw Image Folder Fault [%s]"), Folder);
		JetAPI::ShowMessageBox(str);		
		return false; 
	}
	JetAPI::ClearFolder(Folder);
	return true;
}
//-------------------------------------------------------------------------------------//
int CCaliPaneAlign::GetSaveRawImageTimes() const
{
	return m_SaveRawImageTimes;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::SetSaveRawImageTimes(int val)
{
	m_SaveRawImageTimes = val;
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneAlign::GetSaveRawImageFolder() const
{
	CString Folder;	
	Folder.Format(_T("%s\\RawImage"), AOIDataCollect.GetAOITempDirectory());
	return Folder;
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneAlign::GetSaveRawImageExtName()
{
	CString ExtName;
	const int ExtNameMode = JetAPI::GetComboxCurSelData(m_SaveRawExtNameCombox);
	switch ( ExtNameMode )
	{
	case IMAGE_FILE_MODE_JPG:	ExtName = _T("JPG");	break;
	case IMAGE_FILE_MODE_PNG:	ExtName = _T("PNG");	break;
	case IMAGE_FILE_MODE_BMP:
	default:
		ExtName = _T("BMP");
		break;
	}
	return ExtName;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecCaliAlignWndMsg(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch ( wParam )
	{
	case WPARAM_SAVE_SYSTEM_PARAM:
		str = _T("Save System Param");
		str = LoadMultiLanguageString(str, str);
		AddLogListBox(str);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::CalcObject(IMAGE_SIZE ModelW, IMAGE_SIZE ModelH, IMAGE_SIZE ModelStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, RECT &ObjRect, float &ObjH)//求得物體的資料
{
	const int KenSize = 5;
	std::vector<RECT> ObjList;
	std::vector<float> ObjHList;
	if ( ImageAPI.BuildSpaceObjectList(ModelW, ModelH, ModelStep, SpacePtr, MaskPtr, KenSize, ObjList, ObjHList) == false )
	{	return false; }
	const size_t ObjCount=ObjList.size();
	const size_t ObjHCount=ObjHList.size();
	if ( ObjCount != ObjHCount )
	{	return false; }
	if ( 0 == ObjCount )
	{	return true; }

	ObjRect = ObjList[0];
	ObjH    = ObjHList[0];	
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnBasePlaneParamBtn() 
{
	// TODO: Add your control notification handler code here
	int  nBaseColoeIndex=0;	
	bool bBaseColorEnabled=false;	
	TBasePlaneParam BasePlaneParam;
	CSpaceBaseParamWnd ParamWnd;
	
	bBaseColorEnabled = false;
	nBaseColoeIndex   = -1;
	BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;
	if ( nBaseColoeIndex >= 0 ) { nBaseColoeIndex -= PROJECT_COLOR_ID_BOARD_BEGIN; }

	//ParamWnd.SetProjectPtr(ProjectPtr);
	ParamWnd.SetBasePlaneParam(BasePlaneParam);
	ParamWnd.SetBaseColorIndex(nBaseColoeIndex);
	ParamWnd.SetBaseColorEnabled(bBaseColorEnabled);	
	if ( ParamWnd.DoModal() == IDCANCEL )
	{	return ; }	

	ParamWnd.GetBasePlaneParam(BasePlaneParam);
	nBaseColoeIndex = ParamWnd.GetBaseColorIndex();
	bBaseColorEnabled = ParamWnd.GetBaseColorEnabled();
	const bool bParamSetting = ParamWnd.GetBasePlaneParamSetting();
	const bool bColorSetting = ParamWnd.GetBasePlaneColorSetting();

	if ( true == bParamSetting )
	{	m_NoiseFilterParam.BasePlaneParam = BasePlaneParam; }

	if ( true == bColorSetting ) 
	{
		//if ( false==bBaseColorEnabled || -1==nBaseColoeIndex )
		//{	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(false);	}
		//else
		//{
		//	nBaseColoeIndex += PROJECT_COLOR_ID_BOARD_BEGIN;
		//	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(true);
		//	ProjectPtr->SetProjectComponentSelectedMaskColorIndex_Base(nBaseColoeIndex); 
		//}
	}
	AOIDataCollect.UpdateSystemBasePlaneParamToProject();
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSpaceNoiseFilterBtn() 
{
	// TODO: Add your control notification handler code here
	CSpaceNoiseFilterParamWnd Wnd;	
	TNoiseFilterParam NoiseFilterParam;
	
	NoiseFilterParam = m_NoiseFilterParam;
	Wnd.SetNoiseFilterParam(NoiseFilterParam);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }
	Wnd.GetNoiseFilterParam(NoiseFilterParam);

	NoiseFilterParam.BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;	
	m_NoiseFilterParam = NoiseFilterParam;
	AOIDataCollect.SetSpaceNoiseFilterParam(NoiseFilterParam);
	AOIDataCollect.UpdateSystemNoiseFilterParamToProject();
	UpdateSpaceNoiseFilterParamToUI();
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPhaseHeightFactorSetBtn() 
{
	// TODO: Add your control notification handler code here	
	size_t   i=0;	
	LIGHT_3D_CAST_ID CastID;
	const bool bShowMsg = false;
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	const int MinVal = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MIN_EDIT);
	const int MaxVal = CWnd::GetDlgItemInt(CALIALIGN_PHASE_HEIGHT_FACTOR_MAX_EDIT);
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	if ( Check3DCastID(bShowMsg) == true )
	{ 
		CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(m_3DCastIDCombox));
		CastIDList.push_back(CastID);
	}	
	else
	{	Light3DCtrl.GetLight3DCastIDList(CastIDList);	}
	const size_t CastIDCount = CastIDList.size();
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(CastID);	
		if ( NULL == PhasePtr ) { continue; }
		PhasePtr->SetPhaseFactorMin(MinVal);
		PhasePtr->SetPhaseFactorMax(MaxVal);
		PhasePtr->SaveDLPParameter();
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPhaseHeightFactorClearBtn()
{
	// TODO: Add your control notification handler code here	
	size_t   i=0;
	CString  str;
	LIGHT_3D_CAST_ID CastID;
	const bool bShowMsg = false;
	LIGHT_3D_CLS_PTR PhasePtr = NULL;

	str = _T("Do you want to clear 3D Cast Height Factor File ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return ; }

	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	if ( Check3DCastID(bShowMsg) == true )
	{ 
		CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(m_3DCastIDCombox));
		CastIDList.push_back(CastID);
	}	
	else
	{	Light3DCtrl.GetLight3DCastIDList(CastIDList);	}

	bool bIncludeFiles = true;
	const size_t CastIDCount = CastIDList.size();
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(CastID);	
		if ( NULL == PhasePtr ) { continue; }		
		PhasePtr->ClearHeightFactorTableList(bIncludeFiles);
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnPhaseHeightFactorShowTableBtn()
{
	// TODO: Add your control notification handler code here		
	CString  str;
	CString  strLabel;
	CString  strCaption;
	int      TargetNo=0;
	size_t   i=0, j=0, k=0;
	LIGHT_3D_CAST_ID CastID;
	const bool bShowMsg = false;
	const int  MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;
	LIGHT_3D_CLS_PTR PhasePtr = NULL;	
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	if ( Check3DCastID(bShowMsg) == true )
	{ 
		CastID = (LIGHT_3D_CAST_ID)(JetAPI::GetComboxCurSelData(m_3DCastIDCombox));
		CastIDList.push_back(CastID);
	}	
	else
	{	Light3DCtrl.GetLight3DCastIDList(CastIDList); }
	const size_t CastIDCount = CastIDList.size();

	TListNode       Node;	
	DWORD_PTR       OldIndex=0;
	std::vector<TListNode> NodelList;
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(CastID);	
		if ( NULL == PhasePtr ) { continue; }		

		TPhaseFactorTable GridTable;
		for ( j=0; j<MaxTargetNo; j++ )
		{
			TargetNo = (int)(j+1);
			if ( PhasePtr->CloneHeightFactorTable(TargetNo, GridTable) == false ) { continue; }
			str.Format(_T("3D-%02d, No:%d, Rows:%d, Cols:%d, Height:%.0f"), CastID, GridTable.nTargetNo, GridTable.nRows, GridTable.nCols, GridTable.dHeight);

			Node.Data = k;
			Node.Text = str;
			NodelList.push_back(Node);
			k ++;
		}
	}		

	POINT Point;
	CInputListWnd   EnumWnd;
	::GetCursorPos(&Point);
	EnumWnd.SetWndPos(Point);
	strCaption = _T("Height Factor Grid Infomation Wnd");
	strLabel = _T("Height Factor Info.");
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnParamFileReloadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to reload calibration parameters ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return ; }

	if ( AOIDataCollect.LoadCalibrationParameter() == false ) 
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}

	if ( Light3DCtrl.LoadAllLight3DCastParameter(true) == false ) 
	{
		str = Light3DCtrl.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}

	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneAlign::ExecSelchange3DCastCurrentIDCombo()
{
	const int CurrentID = GetDLPLEDCurrentID();
	Light3DCtrl.SetAllLight3DLEDCurrentID(CurrentID);
	Update3DCastCurrentToUI();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchange3DCastCurrentIDCombo()
{	
	FocusToEditCtrl();	
	ExecSelchange3DCastCurrentIDCombo();
	OnGrabBtn();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnSelchangeParamPhaseToHeightCombo() 
{
	// TODO: Add your control notification handler code here
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	SysParam.m_PhaseConvertHeightMode = JetAPI::GetComboxCurSelData(m_PhaseToHeightCombox);	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneAlign::OnShowRawImageFolderBtn()
{
	// TODO: Add your control notification handler code here
	CString Folder = GetSaveRawImageFolder();
	::CreateDirectory(Folder, NULL);	::Sleep(0);		
	::ShellExecute(NULL, _T("open"), Folder, NULL, NULL, SW_SHOW);
}
//-------------------------------------------------------------------------------------//