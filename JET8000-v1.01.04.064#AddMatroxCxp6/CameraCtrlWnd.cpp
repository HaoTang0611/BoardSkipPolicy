// CameraCtrlWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CameraCtrlWnd.h"
//-------------------------------------------------------------------------------------//
#include "CameraCtrl.h"
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#include "SpaceBaseParamWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
CCameraCtrlWnd   CameraCtrlWnd;
const int        CameraGrabTimerID = 100;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCameraCtrlWnd dialog
//-------------------------------------------------------------------------------------//
CCameraCtrlWnd::CCameraCtrlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraCtrlWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraCtrlWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ImageW= 0;
	m_ImageH= 0;
	m_ImageStep= 0;
	m_BitCount = 0;
	m_ImageBufferSize = 0;
	m_ImageBufferPtr = NULL;
	m_ImageBufferPtr2 = NULL;	
	m_ShowBufferSize = 0;
	m_ShowBufferPtr = NULL;	

	m_BkColor = 0xE0E0E0;	
	this->m_ImageZoom = 1.0;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;	

	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;

	this->m_ImagePt1.x = -1;
	this->m_ImagePt1.y = -1;
	this->m_ImagePt2.x = -1;
	this->m_ImagePt2.y = -1;
	this->m_CameraID = CAMERA_ID_1;
	this->m_CameraNFramesToGrab = -1;
	this->m_CameraBatchGrabMode = 0;//批次取像模式
	m_DrawRect = false;

	m_IsLocked = false;
	m_StagePosX = 0;
	m_StagePosY = 0;
	m_StagePosZ = 0;
	
	m_AutoFocusPitch = 10;		
	m_AutoFocustBestStd = 0;
	m_AutoFocustMinPosZ = -FLT_MAX;
	m_AutoFocustMaxPosZ =  FLT_MAX;
	m_AutoFocustBestPosZ = 0;
	m_AutoFocustLastPosZ = 0;
	m_AutoFocustScalePosZ = 10;	
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;

#ifndef LIGHT_CTRL_DISABLE
	m_GameraGrabBtn = CAMERACTRL_FRAME_GRAB_BTN;
#else
	m_GameraGrabBtn = CAMERACTRL_GRAB_BTN;
#endif//LIGHT_CTRL_DISABLE
	QueryPerformanceCounter(&m_CameraGrabStartTime);//相機取像的起始時間
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraCtrlWnd)
	DDX_Control(pDX, CAMERACTRL_DLP_LED_COLOR_COMBO, m_DlpLEDColorCombox);	
	DDX_Control(pDX, CAMERACTRL_IMAGE_SCALE_COMBO, m_ImageScaleCombox);
	DDX_Control(pDX, CAMERACTRL_IMAGE_SOURCE_COMBO, m_ImageSouceCombox);
	DDX_Control(pDX, CAMERACTRL_FRAME_2D_COMBOX, m_Frame2DCombox);
	DDX_Control(pDX, CAMERACTRL_PHASE_STEP_COMBO, m_PhaseStepCombox);
	DDX_Control(pDX, CAMERACTRL_PHASE_PERIOD_COMBO, m_PhasePeriodCombox);
	DDX_Control(pDX, CAMERACTRL_PHASE_PROJECT_COMBO, m_PhaseCastCombox);
	DDX_Control(pDX, CAMERACTRL_LIGHT_NUMBER_COMBO, m_LightNumCombox);
	DDX_Control(pDX, CAMERACTRL_CALLBACK_TIMMING_COMBO, m_CallbackTimmingCombox);
	DDX_Control(pDX, CAMERACTRL_GRAB_MODE_COMBO, m_GrabModeCombox);
	DDX_Control(pDX, CAMERACTRL_CAMERA_ID_COMBO, m_CameraIdCombox);
	DDX_Control(pDX, CAMERACTRL_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCameraCtrlWnd, CDialog)
	//{{AFX_MSG_MAP(CCameraCtrlWnd)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(CAMERACTRL_GRAB_BTN, OnGrabBtn)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(CAMERACTRL_FIRE_TRIGGER_BTN, OnFireTriggerBtn)
	ON_BN_CLICKED(CAMERACTRL_CANCEL_LOCK_BTN, OnCancelLockBtn)
	ON_BN_CLICKED(CAMERACTRL_SAVE_IMAGE_BTN, OnSaveImageBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(CAMERACTRL_GRAB_CONTINUE_CHK, OnGrabContinueChk)
	ON_CBN_SELCHANGE(CAMERACTRL_CAMERA_ID_COMBO, OnSelchangeCameraIDCombo)
	ON_BN_CLICKED(CAMERACTRL_BATCH_GRAB_BTN, OnBatchGrabBtn)
	ON_BN_CLICKED(CAMERACTRL_FRAME_GRAB_BTN, OnFrameGrabBtn)
	ON_BN_CLICKED(CAMERACTRL_RESET_LIGHT_CTRL_BTN, OnResetLightCtrlBtn)
	ON_CBN_SELCHANGE(CAMERACTRL_FRAME_2D_COMBOX, OnSelchangeFrame2dCombox)
	ON_BN_CLICKED(CAMERACTRL_FOCUS_AUTO_BTN, OnFocusAutoBtn)
	ON_BN_CLICKED(CAMERACTRL_BUILD_3D_OBJ_BTN, OnBuild3dObjBtn)
	ON_BN_CLICKED(CAMERACTRL_VIEW_ALL_BTN, OnViewAllBtn)
	ON_BN_CLICKED(CAMERACTRL_VIEW_1X1_BTN, OnView1x1Btn)
	ON_BN_CLICKED(CAMERACTRL_VIEW_CENTER_LINE_CHK, OnViewCenterLineChk)
	ON_BN_CLICKED(CAMERACTRL_FRAME_3D_CHK, OnFrame3dChk)
	ON_CBN_SELCHANGE(CAMERACTRL_IMAGE_SOURCE_COMBO, OnSelchangeImageSourceCombo)
	ON_CBN_SELCHANGE(CAMERACTRL_DLP_LED_COLOR_COMBO, OnSelchangeDlpLedColorCombo)
	ON_BN_CLICKED(CAMERACTRL_RESET_CAMERA_BTN, OnResetCameraBtn)
	ON_BN_CLICKED(CAMERACTRL_BASE_PLANE_PARAM_BTN, OnBasePlaneParamBtn)
	ON_BN_CLICKED(CAMERACTRL_SPACE_NOISE_FILTER_BTN, OnSpaceNoiseFilterBtn)
	ON_BN_CLICKED(CAMERACTRL_WHITE_BALANCE_SET_BTN, OnWhiteBalanceSetBtn)
	ON_BN_CLICKED(CAMERACTRL_WHITE_BALANCE_READ_BTN, OnWhiteBalanceReadBtn)	
	ON_BN_CLICKED(CAMERACTRL_WHITE_BALANCE_CALC_BTN, OnWhiteBalanceCalcBtn)	
	ON_BN_CLICKED(CAMERACTRL_READ_TEMPERATURE_BTN, OnReadTemperatureBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCameraCtrlWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CCameraCtrlWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CWnd::ShowWindow(SW_SHOWMAXIMIZED);

	CString str;
	bool    bInclude3D=true;
	bool    bIncludeNone=false;

#ifdef DISABLE_3D
	bInclude3D = false;
#else
	bInclude3D = true;
	m_PhaseWnd.Create(IDD_IMAGE_PHASE_WND, this);
	m_Draw3DWnd.Create(IDD_DRAW3D_WND, this);
#endif //DISABLE_3D	

	this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);		
	
	BuildImageScaleModeCombox(m_ImageScaleCombox);
	CCameraCtrl::BuildCameraIDCombox(m_CameraIdCombox);
	CCameraCtrl::BuildCameraGrabModeCombox(m_GrabModeCombox);
	CCameraCtrl::BuildCameraCallbackTimmingCombox(m_CallbackTimmingCombox);

	CCameraCtrl::BuildBatchGrabLightNumCombox(m_LightNumCombox);
	CCameraCtrl::BuildBatchGrabPhaseStepCombox(m_PhaseStepCombox);
	CCameraCtrl::BuildBatchGrabPhasePeriodCombox(m_PhasePeriodCombox);
	CCameraCtrl::BuildBatchGrabPhaseCastCombox(m_PhaseCastCombox);
	
	const int BasePlaneIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceBasePlaneIndex;
	const int NoiseFilterIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceNoiseFilterIndex;
	AOIDataCollect.GetSpaceNoiseFilterParam(m_NoiseFilterParam);
	AOIDataDefine.BuidlProjectDlpLedColorCombox(m_DlpLEDColorCombox);
	AOIDataDefine.BuildSystemFrameParamCombox(m_Frame2DCombox, bIncludeNone, bInclude3D);	
	AOIDataDefine.BuildImageSourceModeCombox(m_ImageSouceCombox, FRAME_COLOR);	
	//AOIDataCollect.GetSystemNoiseFilterParam(NoiseFilterIndex, m_NoiseFilterParam);
	//AOIDataCollect.GetSystemBasePlaneParam(BasePlaneIndex, m_NoiseFilterParam.BasePlaneParam);	
	
	JetAPI::SetComboxCurSel(m_CameraIdCombox, PRIMARY_CAMERA_ID);
	JetAPI::SetComboxCurSel(m_GrabModeCombox, CAMERA_GRAB_SOFTWARE_TRIGGER);
	JetAPI::SetComboxCurSel(m_CallbackTimmingCombox, CAMERA_CALLBACK_EACH_FRAME);
	JetAPI::SetComboxCurSel(m_ImageScaleCombox, FN_DISABLE);
	JetAPI::SetComboxCurSel(m_ImageSouceCombox, IMAGE_SRC_COLOR);	
	JetAPI::SetComboxCurSel(m_Frame2DCombox, FRAME_UNIQUE_ID_DEFAULT);	
	//m_Frame2DCombox.SetCurSel(0);

#ifdef LIGHT_CTRL_DISABLE
	JetAPI::SetComboxCurSel(m_LightNumCombox, BATCH_GRAB_LIGHT_00);
#else
	JetAPI::SetComboxCurSel(m_LightNumCombox, BATCH_GRAB_LIGHT_01);
#endif//LIGHT_CTRL_DISABLE

	JetAPI::SetComboxCurSel(m_PhaseStepCombox, BATCH_GRAB_STEP_4);	
#ifdef DISABLE_3D
	JetAPI::SetComboxCurSel(m_PhaseCastCombox, NULL);	
	JetAPI::EnableCtrlWnd(this, CAMERACTRL_FRAME_3D_CHK, FALSE);
	JetAPI::EnableCtrlWnd(this, CAMERACTRL_DLP_LED_COLOR_COMBO, FALSE);	
	JetAPI::EnableCtrlWnd(this, CAMERACTRL_BUILD_3D_OBJ_BTN, FALSE);	
#else
	JetAPI::SetComboxCurSel(m_PhaseCastCombox, BATCH_GRAB_3D_CAST_01);	
#endif//DISABLE_3D
	
	JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DLP_LED_COLOR_RED);	
	this->SetDlgItemInt(CAMERACTRL_PERIOD_TIME_EDIT, -1);
	this->SetDlgItemInt(CAMERACTRL_EXPOSURE_TIME_EDIT, 1000);
	this->SetDlgItemInt(CAMERACTRL_NFRAMES_GRAB_EDIT, -1);
	this->SetDlgItemInt(CAMERACTRL_LOOP_COUNT_EDIT, 1);
	this->SetDlgItemInt(CAMERACTRL_PHASE_CURRENT_EDIT, 100);

	str.Format(_T("%.0f"), m_AutoFocusPitch);
	CWnd::SetDlgItemText(CAMERACTRL_FOCUS_AUTO_PITCH_EDIT, str);

	this->CreatImageBuffer();
	this->SwitchMultiLanguage();

	this->OnSelchangeCameraIDCombo();	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CAMERA_CTRL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CAMERA_CTRL_WND;
	WndKey = _T("IDD_CAMERA_CTRL_WND");
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
	WndID = CAMERACTRL_GROUP_LABEL;
	WndKey = _T("CAMERACTRL_GROUP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_CAMERA_ID_LABEL;
	WndKey = _T("CAMERACTRL_CAMERA_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_GRAB_MODE_LABEL;
	WndKey = _T("CAMERACTRL_GRAB_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_CALLBACK_TIMMING_LABEL;
	WndKey = _T("CAMERACTRL_CALLBACK_TIMMING_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_GRAB_BTN;
	WndKey = _T("CAMERACTRL_GRAB_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_GRAB_CONTINUE_CHK;
	WndKey = _T("CAMERACTRL_GRAB_CONTINUE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_PERIOD_TIME_LABEL;
	WndKey = _T("CAMERACTRL_PERIOD_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_EXPOSURE_TIME_LABEL;
	WndKey = _T("CAMERACTRL_EXPOSURE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_LOOP_COUNT_LABEL;
	WndKey = _T("CAMERACTRL_LOOP_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_CALLBACK_TIMMING_LABEL;
	WndKey = _T("CAMERACTRL_CALLBACK_TIMMING_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_SAVE_IMAGE_BTN;
	WndKey = _T("CAMERACTRL_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_IMAGE_SCALE_LABEL;
	WndKey = _T("CAMERACTRL_IMAGE_SCALE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CAMERACTRL_CANCEL_LOCK_BTN;
	WndKey = _T("CAMERACTRL_CANCEL_LOCK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CAMERACTRL_LIGHT_NUMBER_LABEL;
	WndKey = _T("CAMERACTRL_LIGHT_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_PHASE_PROJECT_LABE;
	WndKey = _T("CAMERACTRL_PHASE_PROJECT_LABE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_PHASE_STEP_LABE;
	WndKey = _T("CAMERACTRL_PHASE_STEP_LABE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_PHASE_PERIOD_LABE;
	WndKey = _T("CAMERACTRL_PHASE_PERIOD_LABE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = CAMERACTRL_FRAME_GROUP_LABEL;
	WndKey = _T("CAMERACTRL_FRAME_GROUP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_FRAME_2D_LABEL;
	WndKey = _T("CAMERACTRL_FRAME_2D_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_FRAME_3D_CHK;
	WndKey = _T("CAMERACTRL_FRAME_3D_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_FRAME_GRAB_BTN;
	WndKey = _T("CAMERACTRL_FRAME_GRAB_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_RESET_LIGHT_CTRL_BTN;
	WndKey = _T("CAMERACTRL_RESET_LIGHT_CTRL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = CAMERACTRL_FOCUS_AUTO_BTN;
	WndKey = _T("CAMERACTRL_FOCUS_AUTO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CAMERACTRL_BUILD_3D_OBJ_BTN;
	WndKey = _T("CAMERACTRL_BUILD_3D_OBJ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_RESET_CAMERA_BTN;
	WndKey = _T("CAMERACTRL_RESET_CAMERA_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_IMAGE_SOURCE_LABEL;
	WndKey = _T("CAMERACTRL_IMAGE_SOURCE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//			
	WndID = CAMERACTRL_VIEW_ALL_BTN;
	WndKey = _T("CAMERACTRL_VIEW_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_VIEW_1X1_BTN;
	WndKey = _T("CAMERACTRL_VIEW_1X1_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_VIEW_CENTER_LINE_CHK;
	WndKey = _T("CAMERACTRL_VIEW_CENTER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = CAMERACTRL_BASE_PLANE_PARAM_BTN;
	WndKey = _T("CAMERACTRL_BASE_PLANE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_SPACE_NOISE_FILTER_BTN;
	WndKey = _T("CAMERACTRL_SPACE_NOISE_FILTER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//White Balance	
	WndID = CAMERACTRL_WHITE_BALANCE_GROUP;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_RED_LABEL;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_RED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_GREEN_LABEL;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_GREEN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_BLUE_LABEL;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_BLUE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_SET_BTN;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_READ_BTN;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CAMERACTRL_WHITE_BALANCE_CALC_BTN;
	WndKey = _T("CAMERACTRL_WHITE_BALANCE_CALC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = CAMERACTRL_READ_TEMPERATURE_BTN;
	WndKey = _T("CAMERACTRL_READ_TEMPERATURE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	*/
}
//-------------------------------------------------------------------------------------//
CString CCameraCtrlWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CAMERA_CTRL_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	//return ;
	if ( m_PhaseWnd.GetSafeHwnd() != NULL )
	{	m_PhaseWnd.ShowWindow(SW_HIDE);	}
	if ( m_Draw3DWnd.GetSafeHwnd() != NULL )
	{
		m_Draw3DWnd.ShowWindow(SW_HIDE);
		m_Draw3DWnd.ReleaseData();
	}		
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_RESET_CALLBACK_HWND, TRUE);
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CCameraCtrlWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->GetIsJogMode() == true ) 
	{
		BOOL bLive = CWnd::IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
		if ( FALSE == bLive )
		{
			if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
			{	return TRUE; }
		}
	}

	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }

	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			m_DrawRect = false;
			return TRUE;
			break;
		case VK_ENHANCE_IMAGE:
			AOIDataCollect.ToggleIsEnhanceDisplayImage();
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return ; }

	CWnd *pWnd = NULL;
	RECT  WndRect = {0};
	SIZE  EditWndSize = {0};	
	pWnd = this->GetDlgItem(CAMERACTRL_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		EditWndSize.cx = WndRect.right-WndRect.left;
		EditWndSize.cy = WndRect.bottom-WndRect.top;
		this->ScreenToClient(&WndRect);		
		WndRect.bottom = cy-8;
		WndRect.right  = cx-4;
		WndRect.top = WndRect.bottom-EditWndSize.cy;
		pWnd->MoveWindow(&WndRect);		
	}	
	pWnd = this->GetDlgItem(CAMERACTRL_COUNT_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-4;		
		pWnd->MoveWindow(&WndRect);
	}
	pWnd = this->GetDlgItem(CAMERACTRL_PIXEL_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-4;		
		pWnd->MoveWindow(&WndRect);
	}
	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-8-EditWndSize.cy-4;
		this->m_ImageWnd.MoveWindow(&WndRect);
		this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
		this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
		DrawImageWndMemDC();	
		RedrawWnd();		
	}
	pWnd = this->GetDlgItem(CAMERACTRL_GROUP_LABEL);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.bottom = cy-8;				
		pWnd->MoveWindow(&WndRect);		
	}
	
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( 0 != nStatus ) { return; }
	if ( TRUE == bShow )
	{	
		CreatImageBuffer();
		bool  bInclude3D = true;
		bool  bIncludeNode=false;		
		const int DlpLedColor = AOIDataCollect.GetSystemDlpLedColor();
	#ifdef DISABLE_3D
		bInclude3D = false;
	#endif //DISABLE_3D
		MotionCtrlPtr->SetIsJogMode(true);
		AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());
		AOIDataDefine.BuildSystemFrameParamCombox(m_Frame2DCombox, bIncludeNode, bInclude3D);		
		JetAPI::SetComboxCurSel(m_Frame2DCombox, FRAME_UNIQUE_ID_DEFAULT);
		JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DlpLedColor);	
		if ( m_Frame2DCombox.GetCount() > 0 )
		{
			if ( m_Frame2DCombox.GetCurSel() == -1 )
			{	m_Frame2DCombox.SetCurSel(0);	}
		}
	#ifndef PHASE_CTRL_DISABLE	
		Light3DCtrl.SetAllLight3DLEDColor(DlpLedColor);		
	#endif//PHASE_CTRL_DISABLE
		OnWhiteBalanceReadBtn();
		OnReadTemperatureBtn();
		CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
	}
	else
	{	
		//CameraCtrl.StopAllCameraGrab();
		DestroyImageBuffer();
		JetAPI::ClearUniFrameList(m_UniFrameList);
	}
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 600;
	lpMMI->ptMinTrackSize.y = 400;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::UpdateShowBufferImage()//更新顯示影像
{
	if ( NULL == m_ImageBufferPtr ) { return; }
	if ( NULL == m_ImageBufferPtr2 ) { return; }	
	if ( NULL == m_ShowBufferPtr ) { return; }
	
	if ( AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBufferPtr2, m_ShowBufferPtr) == false )
	{
		const size_t ImageBuffer = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
		::memset(m_ShowBufferPtr, 0x00, sizeof(IMAGE_DATA)*ImageBuffer);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::DrawImageWndMemDC()//建立影像的記憶體圖像
{	
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }
	COLORREF clrBK = m_BkColor;	
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}

	if ( NULL == m_ImageBufferPtr ) { return; }
	if ( NULL == m_ImageBufferPtr2 ) { return; }	
	if ( NULL == m_ShowBufferPtr ) { return; }
	//UpdateShowBufferImage();	
	if ( ImageAPI.DrawImageToDC(hMemDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBufferPtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::CreateImageSourceImage()//建立影像來源的圖像
{
	if ( NULL == m_ImageBufferPtr ) { return; }
	if ( NULL == m_ImageBufferPtr2 ) { return; }	
	bool CloneImage = true;
	IMAGE_SRC_MODE ImageSourceMode = (IMAGE_SRC_MODE)(JetAPI::GetComboxCurSelData(m_ImageSouceCombox));

	CloneImage = true;
	if ( 24 == m_BitCount )
	{
		if ( IMAGE_SRC_COLOR != ImageSourceMode )				
		{
			RECT RoiRect;
			IMAGE_PTR GrayPtr=NULL;
			const int nAlign = 4;
			const IMAGE_SIZE GrayBit  = 8;
			const IMAGE_SIZE StepGray = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, GrayBit, nAlign);			
			const int WR = 100;
			const int WG = 100;
			const int WB = 100;
			RoiRect.left = 0; 
			RoiRect.top = 0;
			RoiRect.right = (int)(m_ImageW);
			RoiRect.bottom = (int)(m_ImageH);
			if ( ImageAPI.ColorImageToGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_ImageBufferPtr, RoiRect, StepGray, GrayPtr, ImageSourceMode, WR, WG, WB, false) == true )
			{	
				if ( ImageAPI.RGBImageToColorImage3(m_ImageW, m_ImageH, StepGray, GrayPtr, GrayPtr, GrayPtr, m_ImageStep, m_ImageBufferPtr2, false) == true )
				{	CloneImage = false; }
				JetMemory.free_func(GrayPtr);
			}			
		}
	}
	if ( true == CloneImage )
	{
		const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
		::memcpy(m_ImageBufferPtr2, m_ImageBufferPtr, sizeof(IMAGE_DATA)*BufferSize);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::BuildImageScaleModeCombox(CComboBox &Combox)
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;	
	const int    ScaleCount = 10;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str = AOIDataDefine.GetDisableText();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, FN_DISABLE);
	idx ++;

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
void CCameraCtrlWnd::RedrawWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( hDC == NULL ) { return; }
	HDC MemDC = this->m_ImageWndMemDC.GetSafeHdc();
	if ( MemDC == NULL ) { return; }
	RECT Rect = this->m_ImageWndRect;
	::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
	
	//HBRUSH hBrush = ::CreateSolidBrush(0x000000);
	//::FillRect(MemDC, &Rect, hBrush);
	//::DeleteObject(hBrush);

	::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, MemDC, 0, 0, SRCCOPY );

	DrawCenterLine(hDC);
	DrawRect(hDC);	

	if ( this->m_strPixel.GetLength() > 0 ) 
	{	::TextOut(hDC, 0, 0, m_strPixel, m_strPixel.GetLength());	}
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::DrawRect(HDC hDC)//繪製矩形
{
	if ( false == m_DrawRect ) { return; }

	TPOINT2D WndPt1;
	TPOINT2D WndPt2;
	RECT rectWnd={0};
	RECT rectImage={0};

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImagePt1, WndPt1);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImagePt2, WndPt2);	

	rectWnd.left   = JetAPI::Floor(MIN(WndPt1.x, WndPt2.x));
	rectWnd.top    = JetAPI::Floor(MIN(WndPt1.y, WndPt2.y));
	rectWnd.right  = JetAPI::Floor(MAX(WndPt1.x, WndPt2.x));
	rectWnd.bottom = JetAPI::Floor(MAX(WndPt1.y, WndPt2.y));

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	::MoveToEx(hDC, rectWnd.left, rectWnd.top, NULL);
	::LineTo(hDC, rectWnd.right, rectWnd.top);
	::LineTo(hDC, rectWnd.right, rectWnd.bottom);
	::LineTo(hDC, rectWnd.left, rectWnd.bottom);
	::LineTo(hDC, rectWnd.left, rectWnd.top);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::DrawCenterLine(HDC hDC)//繪製中心線
{
	BOOL bShow = CWnd::IsDlgButtonChecked(CAMERACTRL_VIEW_CENTER_LINE_CHK);
	if ( FALSE == bShow ) { return; }

	POINT    Pt1={0}, Pt2={0};
	TPOINT2D WndHorPtL,WndHorPtR;
	TPOINT2D WndVerPtT,WndVerPtB;
	TPOINT2D ImageHorPtL,ImageHorPtR;
	TPOINT2D ImageVerPtT,ImageVerPtB;

	ImageHorPtL.x = 0;
	ImageHorPtL.y = m_ImageH/2;
	ImageHorPtR.x = m_ImageW;
	ImageHorPtR.y = m_ImageH/2;

	ImageVerPtT.x = m_ImageW/2;
	ImageVerPtT.y = 0;
	ImageVerPtB.x = m_ImageW/2;
	ImageVerPtB.y = m_ImageH;

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImageHorPtL, WndHorPtL);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImageHorPtR, WndHorPtR);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImageVerPtT, WndVerPtT);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImageVerPtB, WndVerPtB);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFF00FF);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));

	JetAPI::Point2DToPoint(WndHorPtL, Pt1);
	JetAPI::Point2DToPoint(WndHorPtR, Pt2);	
	::MoveToEx(hDC, Pt1.x, Pt1.y, NULL);
	::LineTo(hDC, Pt2.x, Pt2.y);	
	
	JetAPI::Point2DToPoint(WndVerPtT, Pt1);
	JetAPI::Point2DToPoint(WndVerPtB, Pt2);
	::MoveToEx(hDC, Pt1.x, Pt1.y, NULL);
	::LineTo(hDC, Pt2.x, Pt2.y);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::UpdateCursorInfo(POINT WndPt)//更新鼠標資訊
{
	int      R=0, G=0, B=0, idx=0;		
	POINT    ImagePos={0};
	CString  strPixel;
	TPOINT2D ImagePt=WndPt;
	TPOINT2D WndPt2 =WndPt;
	const IMAGE_SIZE ImageW = this->m_ImageW;
	const IMAGE_SIZE ImageH = this->m_ImageH;
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt2);		
	
	JetAPI::Point2DToPoint(ImagePt, ImagePos);
	if ( NULL==m_ImageBufferPtr || ImagePos.x<0 || ImagePos.y<0 || ImagePos.x>=nImageW || ImagePos.y>=nImageH )
	{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y); }
	else if ( 8 == m_BitCount )
	{	
		idx = (ImagePos.y*m_ImageStep)+(ImagePos.x);
		R = G = B = m_ImageBufferPtr[idx];
		strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R, G, B);
	}
	else if ( 24 == m_BitCount )
	{	
		idx = (ImagePos.y*m_ImageStep)+(ImagePos.x*3);
		B = m_ImageBufferPtr[idx]; 
		G = m_ImageBufferPtr[idx+1]; 
		R = m_ImageBufferPtr[idx+2]; 
		strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R, G, B);
	}
	else
	{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y); }
	this->SetDlgItemText(CAMERACTRL_PIXEL_INFO_EDIT, strPixel);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::CreatImageBuffer()
{
	const char fnName[] = "CCameraCtrlWnd::CreatImageBuffer";
	this->DestroyImageBuffer();

	this->m_ImageBufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(m_ImageBufferSize, m_ImageBufferPtr, fnName, "m_ImageBufferPtr") == false )
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());
		return false;
	}
	if ( JetMemory.alloc_func(m_ImageBufferSize, m_ImageBufferPtr2, fnName, "m_ImageBufferPtr2") == false )
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());
		return false;
	}
	m_ShowBufferSize = m_ImageBufferSize;
	if ( JetMemory.alloc_func(m_ShowBufferSize, m_ShowBufferPtr, fnName, "m_ShowBufferPtr") == false )
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::DestroyImageBuffer()
{
	if ( NULL != m_ImageBufferPtr )
	{	JetMemory.free_func(m_ImageBufferPtr); }
	if ( NULL != m_ImageBufferPtr2 )
	{	JetMemory.free_func(m_ImageBufferPtr2); }		
	m_ImageBufferSize = 0;

	if ( NULL != m_ShowBufferPtr )
	{	JetMemory.free_func(m_ShowBufferPtr); }
	m_ShowBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnGrabBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bLock = GetLockUIWnd();
	if ( true == bLock ) 
	{	return; }

	ResetShowImageBuffer();
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;
	ExecGrabTest();	
}
//-------------------------------------------------------------------------------------//
LRESULT CCameraCtrlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	BOOL bLive = NULL;
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
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(false);
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		if ( TRUE == wParam )
		{
			const bool bLock = GetLockUIWnd();
			if ( true == bLock ) { return TRUE; }			
			bLive = CWnd::IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
			if ( TRUE == bLive ) { return TRUE; }
		}
		ExecGrabFunc();
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE:
			const bool bLock = GetLockUIWnd();
			if ( true == bLock ) { return TRUE; }			
			bLive = CWnd::IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
			if ( TRUE == bLive ) { return TRUE; }
			ExecGrabFunc();
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyImageBuffer();
	JetAPI::ClearUniFrameList(m_UniFrameList);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	if ( CAMERACTRL_BATCH_GRAB_BTN == this->m_GameraGrabBtn )
	{
		return RetrieveCameraImage_BatchGrab(wParam, lParam, bCameraCallBack);
	}
	if ( CAMERACTRL_FRAME_GRAB_BTN == this->m_GameraGrabBtn )
	{
		return RetrieveCameraImage_UniFrame(wParam, lParam, bCameraCallBack);
	}	
	return RetrieveCameraImage_Grab(wParam, lParam, bCameraCallBack);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::RetrieveCameraImage_Grab(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	if ( NULL == m_ImageBufferPtr ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		this->m_cntImageBypass ++;
		this->StartTimerCameraGrab();
		return true;
	}
	CString str;
	long cntCameraBak=0, cntExpBak=0, cntImageBak=0, cntImageCpy=0;	
	CAMERA_ID CameraID = CCameraCtrl::GetCaemraIDFromWParam(wParam);
	CameraCtrl.SetCameraToSendCallback(CameraID, FALSE);
	if ( CameraCtrl.GetCameraImage3(CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBufferPtr) == false )
	{				
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false); 
		return false;
	}
	CameraCtrl.KeepCameraTempRingBuffer(CameraID);
	CameraCtrl.IncrementCameraCopyToHostCount(CameraID);
	CameraCtrl.GetCameraCount(CameraID, cntCameraBak, cntExpBak, cntImageBak, cntImageCpy);
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間	
	CreateImageSourceImage();
	UpdateShowBufferImage();
	DrawImageWndMemDC();
	CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
	CameraCtrl.FreeCameraTempRingBuffer(CameraID);
	
	RECT    AveRoi;
	CString AveStr;
	double  AveR=0, AveG=0, AveB=0, Ave=0;
	AveRoi.left   = (m_ImageW/4);
	AveRoi.right  = (m_ImageW*3/4);
	AveRoi.top    = (m_ImageH/4);
	AveRoi.bottom = (m_ImageH*3/4);
	switch ( m_BitCount )
	{
	case 8:
		ImageAPI.CalcGrayImageAverage(m_ImageW, m_ImageH, m_ImageStep, m_ImageBufferPtr, AveRoi, Ave);
		AveStr.Format(_T("Gray=%.2f"), Ave);
		break;
	case 24:
		ImageAPI.CalcColorImageAverage(m_ImageW, m_ImageH, m_ImageStep, m_ImageBufferPtr, AveRoi, AveR, AveG,AveG);
		AveStr.Format(_T("(R,G,B)=(%.2f,%.2f,%.2f)"), AveR, AveG, AveB);
		break;
	}
	double Time = (double)((m_CameraGrabEndTime.QuadPart - m_CameraGrabStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart);//ms
	double CameraFPS = cntImageBak*1000.0/Time;
	double ViewFPS = cntImageCpy*1000.0/Time;
	str.Format(_T("Cam:%u, Exp:%u, Img:%u, Cpy:%u, Camera FPS=%.2f, View FPS=%.2f, Tim=%.2fms, %s"), cntCameraBak, cntExpBak, cntImageBak, cntImageCpy, CameraFPS, ViewFPS, Time, AveStr);
	this->SetDlgItemText(CAMERACTRL_COUNT_INFO_EDIT, str);

	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	str.Format(_T("Z:%.0f um"), PosZ);
	this->SetDlgItemText(CAMERACTRL_MOTION_POS_EDIT, str);

	this->RedrawWnd();	
	OnReadTemperatureBtn();

	if ( ExecAutoFocusNext() == false )
	{
		LockUIWnd(false);
		return false;
	}
	if ( CAMERA_CTRL_GRAB_NORMAL == m_CameraCtrlGrabMode )
	{
		BOOL bLive = this->IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
		if ( TRUE == bLive )
		{
			if ( (m_CameraNFramesToGrab==-1) || (cntImageBak<m_CameraNFramesToGrab) )
			{	this->StartTimerCameraGrab(); }
			else 
			{
				CameraCtrl.StopCameraGrab(CameraID);			
				this->LockUIWnd(false); 
				this->CheckDlgButton(CAMERACTRL_GRAB_CONTINUE_CHK, FALSE);
			}
		}
		else
		{	
			CameraCtrl.StopCameraGrab(CameraID);
			this->LockUIWnd(false); 
		}
	}
	else
	{
		CameraCtrl.StopCameraGrab(CameraID);
		this->LockUIWnd(false); 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::RetrieveCameraImage_BatchGrab(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	if ( NULL == m_ImageBufferPtr ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		this->m_cntImageBypass ++;
		return true;	
	}

	CString str;
	bool bFinish=false;
	CAMERA_ID CameraID = CCameraCtrl::GetCaemraIDFromWParam(wParam);
	long cntCameraBak=0, cntExpBak=0, cntImageBak=0, cntImageCpy=0;	
	CameraCtrl.GetCameraCount(CameraID, cntCameraBak, cntExpBak, cntImageBak, cntImageCpy);
	this->m_cntCameraBak += cntCameraBak;
	this->m_cntExpBak += cntExpBak;
	this->m_cntImageBak += cntImageBak;
	this->m_cntImageCpy += cntImageCpy;	
	if ( CameraCtrl.BatchGrabStart(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false);
		return false;
	}
	if ( false == bFinish )
	{	return true; }
	
	//CameraCtrl.SetCameraToSendCallback(CameraID, FALSE);
	if ( CameraCtrl.GetCameraImage3(CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBufferPtr) == false )
	{				
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		return false;
	}
	CameraCtrl.KeepCameraTempRingBuffer(CameraID);
	CameraCtrl.IncrementCameraCopyToHostCount(CameraID);
	
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間	
	CreateImageSourceImage();
	UpdateShowBufferImage();
	DrawImageWndMemDC();
	CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
	//CameraCtrl.FreeCameraTempRingBuffer(CameraID);
	
	double Time = (double)((m_CameraGrabEndTime.QuadPart - m_CameraGrabStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart);//ms
	double CameraFPS = m_cntImageBak*1000.0/Time;
	double ViewFPS = m_cntImageCpy*1000.0/Time;
	str.Format(_T("Cam:%u, Exp:%u, Img:%u, Cpy:%u, Camera FPS=%.2f, View FPS=%.2f, Tim=%.2fms"), m_cntCameraBak, m_cntExpBak, m_cntImageBak, m_cntImageCpy, CameraFPS, ViewFPS, Time);
	this->SetDlgItemText(CAMERACTRL_COUNT_INFO_EDIT, str);
	
	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	str.Format(_T("Z:%.0f um"), PosZ);
	this->SetDlgItemText(CAMERACTRL_MOTION_POS_EDIT, str);

	this->RedrawWnd();	
	OnReadTemperatureBtn();

	if ( ExecAutoFocusNext() == false )
	{
		LockUIWnd(false);
		return false;
	}
	if ( CAMERA_CTRL_GRAB_NORMAL == m_CameraCtrlGrabMode )
	{
		BOOL bLive = this->IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
		if ( TRUE == bLive )
		{
			::Sleep(30);
			if ( ExecGrabBatchNext() == false )
			{	return false; }
		}
		else
		{	this->LockUIWnd(false); }
	}
	else
	{	this->LockUIWnd(false); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::LockUIWnd(bool bLock)//鎖住視窗
{	
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	
	BOOL  bEnable3D = TRUE;
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }
	bEnable3D = bEnable;
	if ( true == bDisable3D )
	{	bEnable3D = FALSE;	}

	m_IsLocked = bLock;
	CtrlID = CAMERACTRL_CAMERA_ID_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CAMERACTRL_GRAB_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_CALLBACK_TIMMING_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CAMERACTRL_GRAB_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CAMERACTRL_LIGHT_NUMBER_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_PHASE_PROJECT_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_PHASE_STEP_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_PHASE_PERIOD_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CAMERACTRL_BATCH_GRAB_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_SAVE_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = CAMERACTRL_FRAME_2D_COMBOX;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_FRAME_3D_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CAMERACTRL_DLP_LED_COLOR_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);	
	CtrlID = CAMERACTRL_FRAME_GRAB_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_RESET_LIGHT_CTRL_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CAMERACTRL_FOCUS_AUTO_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	//CtrlID = CAMERACTRL_BUILD_3D_OBJ_BTN;
	//JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CAMERACTRL_BASE_PLANE_PARAM_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);
	CtrlID = CAMERACTRL_SPACE_NOISE_FILTER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable3D);		
	
	//About Edit
	CtrlID = CAMERACTRL_NFRAMES_GRAB_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_PERIOD_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_EXPOSURE_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_LOOP_COUNT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_PHASE_CURRENT_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	

	CtrlID = CAMERACTRL_FOCUS_AUTO_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	

	//About White Balance
	CtrlID = CAMERACTRL_WHITE_BALANCE_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CAMERACTRL_WHITE_BALANCE_READ_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CAMERACTRL_WHITE_BALANCE_CALC_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		

	CtrlID = CAMERACTRL_WHITE_BALANCE_RED_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CAMERACTRL_WHITE_BALANCE_GREEN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CAMERACTRL_WHITE_BALANCE_BLUE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::GetLockUIWnd() const
{
	return m_IsLocked;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case CameraGrabTimerID:
		this->KillTimerCameraGrab();
		this->ExecFireTrigger();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::StartTimerCameraGrab()//開始相機取像
{
	//this->SetTimer(CameraGrabTimerID, 10, NULL);
	this->SetTimer(CameraGrabTimerID, 0, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::KillTimerCameraGrab()//停止相機取像
{
	this->KillTimer(CameraGrabTimerID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ResetShowImageBuffer()
{
	if ( NULL == m_ShowBufferPtr ) { return true; }
	::memset(m_ShowBufferPtr, 0x00, sizeof(IMAGE_DATA)*m_ShowBufferSize);
	DrawImageWndMemDC();	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnFireTriggerBtn() 
{
	// TODO: Add your control notification handler code here
	this->ExecFireTrigger();
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecFireTrigger()
{
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));
	const CAMERA_GRAB_MODE GrabMode = (CAMERA_GRAB_MODE)(JetAPI::GetComboxCurSelData(this->m_GrabModeCombox));
	switch ( GrabMode )
	{
	case CAMERA_GRAB_FREE_RUN:
		break;
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		CameraCtrl.FireCameraSoftwareTrigger(CameraID);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::RetrieveStagePosition()
{
	MotionCtrlPtr->GetCurrentPos(m_StagePosX, m_StagePosY, m_StagePosZ);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnCancelLockBtn() 
{
	// TODO: Add your control notification handler code here
	this->LockUIWnd(false);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSaveImageBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ImageBufferPtr ) { return; }	
	if ( (24!=m_BitCount) && (8!=m_BitCount) ) { return ; }

	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString filenameBase;
	CString filenameBase2;
	CString filename = dialog.GetPathName();	
	CString extname = dialog.GetFileExt();	
	std::vector<CString> filenameList;
	const CAMERA_ID CameraID = this->m_CameraID;
	int    idx = 0;
	int    len = 0;

	extname.MakeUpper();	
	idx = filename.ReverseFind(_T('.'));
	filenameBase =  filename.Left(idx);	
	len = filenameBase.GetLength();
	idx = filenameBase.ReverseFind(_T('\\'));
	filenameBase2 = filenameBase.Right(len-idx-1);

	if ( CAMERACTRL_BATCH_GRAB_BTN == this->m_GameraGrabBtn )
	{
		long i=0, idx=0;
		IMAGE_PTR pImage=NULL;
		IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
		const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
		const long RingIndex  = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
		const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));
		idx = RingIndex;
		idx = idx - ImageCount;
		if ( idx < 0 ) { idx += RingListSize;  }
		BitCount = 8;
		for ( i=0; i<ImageCount; i++ )
		{
			if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, pImage) == false )
			{	continue; }

			filename.Format(_T("%s#%d.%s"), filenameBase, i+1, extname);
			if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, pImage, true) == false )
			{
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			filename.Format(_T("%s#%d.%s"), filenameBase2, i+1, extname);
			filenameList.push_back(filename);

			idx ++;
		}

		FILE *pfile = NULL;
		TCHAR  TMode[32] = _T("");
		_tcscpy(TMode, _T("w+"));
		JetAPI::ModifyOpenFileMode_Write(TMode);
		filename.Format(_T("%s.%s"), filenameBase, _T("TXT"));		
		pfile = _tfopen(filename, TMode);
		if ( NULL != pfile )
		{
			size_t NFiles = filenameList.size();
			for ( i=0; i<NFiles; i++ )
			{
				filename = filenameList[i];
				::_ftprintf(pfile, _T("%s\n"), (LPCTSTR)filename);
			}
			::fclose(pfile);
			pfile = NULL;
		}
	}
	else
	{
		TRECT4D  Rect4D;
		RECT     RoiRect = { 0 };
		TPOINT2D ImagePt1, ImagePt2;
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);


		Rect4D.left = MIN(ImagePt1.x, ImagePt2.x);
		Rect4D.top = MIN(ImagePt1.y, ImagePt2.y);
		Rect4D.right = MAX(ImagePt1.x, ImagePt2.x);
		Rect4D.bottom = MAX(ImagePt1.y, ImagePt2.y);
		JetAPI::Rect4DToRect(Rect4D, RoiRect);

		if (RoiRect.left< 0)
		{
			RoiRect.left = 0;
		}
		if (RoiRect.top < 0)
		{
			RoiRect.top = 0;
		}
		if (RoiRect.right > m_ImageW)
		{
			RoiRect.right = m_ImageW;
		}
		if (RoiRect.bottom > m_ImageH)
		{
			RoiRect.bottom = m_ImageH;
		}
		IMAGE_SIZE RoiRectW = RoiRect.right - RoiRect.left;
		IMAGE_SIZE RoiRectStep = JetAPI::GetBMPImagePixelsPerLine(RoiRectW, 4);
		RoiRect.right = RoiRect.left + RoiRectStep;
		if (RoiRect.right > m_ImageW)//為了與繪圖同步, 因此調整ROI的寬度至4的倍數
		{
			RoiRect.right = m_ImageW;
			RoiRect.left = RoiRect.right - RoiRectStep;
		}

		IMAGE_SIZE RoiW = RoiRect.right - RoiRect.left;
		IMAGE_SIZE RoiH = RoiRect.bottom - RoiRect.top;
		IMAGE_SIZE RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, m_BitCount, 1);
		if (RoiW>m_ImageW || RoiH>m_ImageH) {return;}

		IMAGE_PTR BufferPtr=NULL;
		const size_t BufferSize=ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
		if ( JetMemory.alloc_func(BufferSize, BufferPtr, "CCameraCtrlWnd::OnSaveImageBtn", "BufferPtr") == false )
		{	return; }
		::memcpy(BufferPtr, m_ImageBufferPtr, sizeof(IMAGE_DATA)*BufferSize);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBufferPtr, BufferPtr);

		if (RoiW != 0 && RoiH != 0)
		{
			IMAGE_PTR  RoiImagePtr = NULL;
			if (ImageAPI.ExtractRoiImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, BufferPtr, RoiRect, RoiImageStep, RoiImagePtr, false) == false)
			{
				JetMemory.free_func(BufferPtr);
				JetMemory.free_func(RoiImagePtr);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			
			if (ImageAPI.SaveImage(filename, RoiW, RoiH, RoiImageStep, m_BitCount, RoiImagePtr, true) == false)
			{
				JetMemory.free_func(BufferPtr);
				JetMemory.free_func(RoiImagePtr);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			JetMemory.free_func(BufferPtr);
			JetMemory.free_func(RoiImagePtr);
		}
		else 
		{	
			if (ImageAPI.SaveImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, BufferPtr, true) == false)
			{
				JetMemory.free_func(BufferPtr);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			JetMemory.free_func(BufferPtr);
		}
	}
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2)
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
void CCameraCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	this->m_LBtnUpPos = point;
	this->m_LBtnDownPos = this->m_MovingPos = this->m_LBtnUpPos;
	this->SetCapture();
	
	if ( PtInControlWnd(pt, CAMERACTRL_IMAGE_WND, pt) == true )
	{
		m_DrawRect = true;
		m_ImageWndPt1 = pt;
		m_ImageWndPt2 = pt;
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}
	RedrawWnd();
	CDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( GetCapture() != this )	{	return; }
	::ReleaseCapture();
	POINT pt = point;
	if ( PtInControlWnd(pt, CAMERACTRL_IMAGE_WND, pt) == true )
	{	
		this->m_ImageWndPt2 = pt;
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}

	//m_DrawRect = false;
	this->m_LBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	this->RedrawWnd();

	CDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;
	this->SetCapture();

	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( GetCapture() != this )	{	return; }
	::ReleaseCapture();
	this->m_RBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	

	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT WndPt = point;
	if ( PtInControlWnd(WndPt, CAMERACTRL_IMAGE_WND, WndPt) == true )
	{	UpdateCursorInfo(WndPt);		}

	if ( this != GetCapture() ) 
	{	
		CDialog::OnMouseMove(nFlags, point);
		return; 
	}	

	POINT pt  = point;
	if ( nFlags&MK_LBUTTON )
	{
		if ( PtInControlWnd(pt, CAMERACTRL_IMAGE_WND, pt) == true )
		{	
			this->m_ImageWndPt2 = (pt);	
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
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
bool CCameraCtrlWnd::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
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
	if ( JetAPI::CheckPtInCtrlWnd(this, point, CAMERACTRL_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	double NextImageZoom = m_ImageZoom;
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
BOOL CCameraCtrlWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	POINT pt = point;
	if ( PtInControlWnd(pt, CAMERACTRL_IMAGE_WND, pt) == true )
	{	ExecStageMoveTo(pt);	}
	return CDialog::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//CCameraCtrlWnd::ExecStageMoveTo(point);

	return CDialog::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnGrabContinueChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = this->IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
	if ( bCheck == FALSE )
	{	this->LockUIWnd(FALSE); }
	else
	{	this->ExecGrabFunc();	}
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSelchangeCameraIDCombo() 
{
	// TODO: Add your control notification handler code here
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	const CAMERA_GRAB_MODE GrabMode = CameraCtrl.GetCameraGrabMode(CameraID);
	const CAMERA_CALLBACK_TIMMING CalbTimm = CameraCtrl.GetCameraCallbackTimming(CameraID);
	const int ExposureTime = CameraCtrl.GetCameraExposureTime(CameraID);
	const long NFramesToGrab = CameraCtrl.GetCameraNFramesToGrab(CameraID);
	const long GrabLoop = CameraCtrl.GetCameraBatchGrabCount(CameraID);

	JetAPI::SetComboxCurSel(m_GrabModeCombox, GrabMode);
	JetAPI::SetComboxCurSel(m_CallbackTimmingCombox, CalbTimm);
	this->SetDlgItemInt(CAMERACTRL_EXPOSURE_TIME_EDIT, ExposureTime);
	this->SetDlgItemInt(CAMERACTRL_NFRAMES_GRAB_EDIT, NFramesToGrab);
	this->SetDlgItemInt(CAMERACTRL_LOOP_COUNT_EDIT, GrabLoop);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnBatchGrabBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bLock = GetLockUIWnd();
	if ( true == bLock ) 
	{	return; }

	ResetShowImageBuffer();
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;
	ExecGrabBatch();	
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnFrameGrabBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bLock = GetLockUIWnd();
	if ( true == bLock ) 
	{	return; }

	CString str;
	str.Format(_T("CCameraCtrlWnd::OnFrameGrabBtn Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	ResetShowImageBuffer();		
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;		
	ExecGrabFrame();
	str.Format(_T("CCameraCtrlWnd::OnFrameGrabBtn End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabFunc()
{
	bool IsOK = true;	
	switch ( m_GameraGrabBtn )
	{
	case CAMERACTRL_GRAB_BTN:
		IsOK = ExecGrabTest();
		break;
	case CAMERACTRL_BATCH_GRAB_BTN:
		IsOK = ExecGrabBatch();
		break;
	case CAMERACTRL_FRAME_GRAB_BTN:
		IsOK = ExecGrabFrame();		
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabTest()
{
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	const CAMERA_GRAB_MODE GrabMode = (CAMERA_GRAB_MODE)(JetAPI::GetComboxCurSelData(this->m_GrabModeCombox));
	const CAMERA_CALLBACK_TIMMING CallbackTimming = (CAMERA_CALLBACK_TIMMING)(JetAPI::GetComboxCurSelData(this->m_CallbackTimmingCombox));
	const int ExposureTime = this->GetDlgItemInt(CAMERACTRL_EXPOSURE_TIME_EDIT);
	const int NFrames = this->GetDlgItemInt(CAMERACTRL_NFRAMES_GRAB_EDIT);
	const int GrabLoop = this->GetDlgItemInt(CAMERACTRL_LOOP_COUNT_EDIT);
	if ( NFrames <= 0 ) 
	{	this->m_CameraNFramesToGrab = -1; }
	else
	{	this->m_CameraNFramesToGrab = NFrames; }

	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if ( CameraCtrl.ResetCameraRingBuffer(CameraID) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}

	if ( CameraCtrl.SetCameraExposureTime(CameraID, ExposureTime) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}

	if (CameraCtrl.SetCameraGrabMode(CameraID, GrabMode) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}

	if ( CameraCtrl.SetCameraCallbackTimming(CameraID, CallbackTimming) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	if ( CameraCtrl.SetCameraNFramesToGrab(CameraID, m_CameraNFramesToGrab) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	if ( CameraCtrl.SetCameraBatchGrabCount(CameraID, GrabLoop) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	CameraCtrl.ClearCameraCount(CameraID);	
	this->LockUIWnd(true);	
	this->m_CameraID = CameraID;
	this->m_GameraGrabBtn = CAMERACTRL_GRAB_BTN;
	ResetCameraCount();	
	QueryPerformanceCounter(&m_CameraGrabStartTime);//相機取像的起始時間
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間
	if ( CameraCtrl.StartCameraGrab(CameraID) == false)
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		this->LockUIWnd(false);
		return false;
	}	
	this->ExecFireTrigger();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabBatch()
{
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	const int PeriodTime = this->GetDlgItemInt(CAMERACTRL_PERIOD_TIME_EDIT);
	const int ExposureTime = this->GetDlgItemInt(CAMERACTRL_EXPOSURE_TIME_EDIT);	
	const int LEDCurrent = this->GetDlgItemInt(CAMERACTRL_PHASE_CURRENT_EDIT);

	const DWORD  LightNum     = (DWORD)(JetAPI::GetComboxCurSelData(m_LightNumCombox));	
	const DWORD  PhaseStep    = (DWORD)(JetAPI::GetComboxCurSelData(m_PhaseStepCombox));
	const DWORD  PhasePeriod  = (DWORD)(JetAPI::GetComboxCurSelData(m_PhasePeriodCombox));
	const DWORD  PhaseCast    = (DWORD)(JetAPI::GetComboxCurSelData(m_PhaseCastCombox));

	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}

	this->m_CameraBatchGrabMode = LightNum+PhaseStep+PhasePeriod+PhaseCast;//批次取像模式	
	//const int LEDCurrentID=0;
	//Light3DCtrl.SetAllLight3DLEDCurrent(LEDCurrent,LEDCurrent, LEDCurrent);
	if ( CameraCtrl.BatchGrabPrepare(CameraID, m_CameraBatchGrabMode, PeriodTime, ExposureTime) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	bool bFinish = false;		
	this->LockUIWnd(true);	
	this->m_CameraID = CameraID;
	this->m_GameraGrabBtn = CAMERACTRL_BATCH_GRAB_BTN;
	ResetCameraCount();	
	QueryPerformanceCounter(&m_CameraGrabStartTime);//相機取像的起始時間
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間
	if ( CameraCtrl.BatchGrabStart(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		this->LockUIWnd(false);
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabFrame()
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}

#ifndef LIGHT_CTRL_DISABLE
	BOOL     bUse3D = CWnd::IsDlgButtonChecked(CAMERACTRL_FRAME_3D_CHK);
	const int DlpLedColor = JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox);	
	unsigned int FrameUniqueID = JetAPI::GetComboxCurSelData(m_Frame2DCombox);	
	if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID ) { return false; }
	TFrameParam *Frame2DParamPtr = NULL;
	TFrameParam *Frame3DParamPtr = NULL;
	TFrameParam              FrameParam;
	std::vector<TFrameParam> FrameParamList;
	const unsigned int FrameUniqueID3D = FRAME_UNIQUE_ID_DLP;//FRAME_UNIQUE_ID_DLP

	Frame2DParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);//依據Frame ID來取得Frame Param指標
	if ( NULL == Frame2DParamPtr ) { return false; }

	if ( TRUE==bUse3D && FrameUniqueID3D!=FrameUniqueID )
	{
		Frame3DParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID3D);//依據Frame ID來取得Frame Param指標
		if ( NULL == Frame3DParamPtr ) { return false; }
		AOIDataCollect.SetSystemDlpLedColor(DlpLedColor);
	}

	FrameParam = *Frame2DParamPtr;
	FrameParamList.push_back(FrameParam);
	if ( NULL != Frame3DParamPtr )
	{	
		FrameParam = *Frame3DParamPtr;
		FrameParamList.push_back(FrameParam);	
	}			
//	if ( CameraCtrl.SetCameraCallbackTimming(CameraID, CAMERA_CALLBACK_BATCH_GRAB_DONE) == false )
//	{
//	JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
//		return false;
//	}

	bool ResetLight = false;
	if ( AOIDataCollect.ExecPrepareFrameImageSetting(FrameParamList, ResetLight) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}

	JetAPI::ClearUniFrameList(m_UniFrameList);
	LockUIWnd(true);
	m_GameraGrabBtn = CAMERACTRL_FRAME_GRAB_BTN;
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	
	ResetCameraCount();
	QueryPerformanceCounter(&m_CameraGrabStartTime);//相機取像的起始時間
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		LockUIWnd(false);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabNextFunc()
{
	bool IsOK = true;
	switch ( m_GameraGrabBtn )
	{
	case CAMERACTRL_GRAB_BTN:
		IsOK = ExecGrabTest();		
		break;
	case CAMERACTRL_BATCH_GRAB_BTN:		
		IsOK = ExecGrabBatchNext();
		break;
	case CAMERACTRL_FRAME_GRAB_BTN:
		IsOK = ExecGrabFrameNext();		
		break;
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabFrameTest()
{
	if ( this->ExecFireTrigger() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabBatchNext()
{
	bool bFinish = false;
	DWORD BatchGrabMode = CameraCtrl.GetBatchGrabMode();
	BATCH_GRAB_STEP GrabStep = CameraCtrl.GetBatchGrabFirstStep(BatchGrabMode);
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	

	CameraCtrl.ClearCameraCount(CameraID);
	CameraCtrl.SetBatchGrabStep(GrabStep);
	ResetCameraCount();		
	QueryPerformanceCounter(&m_CameraGrabStartTime);//相機取像的起始時間
	QueryPerformanceCounter(& m_CameraGrabEndTime);//相機取像的結束時間		
	if ( CameraCtrl.BatchGrabStart(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecGrabFrameNext()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::RetrieveCameraImage_UniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{	
	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}	

	CString str;	
	const unsigned int MapIndex = 0;	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	long cntCameraBak=0, cntExpBak=0, cntImageBak=0, cntImageCpy=0;	
	CameraCtrl.GetCameraCount(CameraID, cntCameraBak, cntExpBak, cntImageBak, cntImageCpy);
	this->m_cntCameraBak += cntCameraBak;
	this->m_cntExpBak += cntExpBak;
	this->m_cntImageBak += cntImageBak;
	this->m_cntImageCpy += cntImageCpy;	

	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }	
	QueryPerformanceCounter(&m_CameraGrabEndTime);//相機取像的結束時間	

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	const size_t GrabFrameParamCount = GrabFrameParamList.size();
	if ( 0 == GrabFrameParamCount ) { return false; }
	JetAPI::ClearUniFrameList(m_UniFrameList);

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }

	double Time = (double)((m_CameraGrabEndTime.QuadPart - m_CameraGrabStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart);//ms
	double CameraFPS = cntImageBak*1000.0/Time;
	double ViewFPS = cntImageCpy*1000.0/Time;
	str.Format(_T("Cam:%u, Exp:%u, Img:%u, Cpy:%u, Camera FPS=%.2f, View FPS=%.2f, Tim=%.2fms"), m_cntCameraBak, m_cntExpBak, m_cntImageBak, m_cntImageCpy, CameraFPS, ViewFPS, Time);
	this->SetDlgItemText(CAMERACTRL_COUNT_INFO_EDIT, str);

	double PosX=0, PosY=0, PosZ=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	str.Format(_T("Z:%.0f um"), PosZ);
	this->SetDlgItemText(CAMERACTRL_MOTION_POS_EDIT, str);

	TUNI_FRAME UniFrame = UniFrameList[0];
	FRAME_TYPE FrameType = GrabFrameParamList[0].FrameType;
	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(FrameType);	
	
	const size_t MaxFrames = UniFrameList.size();
	if ( MapIndex>=0 && MapIndex<MaxFrames )
	{	UniFrame = UniFrameList[MapIndex]; }
	else
	{	UniFrame = UniFrameList[0]; }
	
	bool bGrab3D = false;
	for ( i=0; i<GrabFrameParamCount; i++ )
	{
		const TFrameParam &Ref=GrabFrameParamList[i];
		if ( FRAME_SPACE != Ref.FrameType ) { continue; }
		bGrab3D = true;
		break;
	}
	JetAPI::EnableCtrlWnd(this, CAMERACTRL_BUILD_3D_OBJ_BTN, bGrab3D);


	BuffserSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
	if ( NULL!=UniFrame.ImagePtr && BuffserSize <= m_ImageBufferSize )
	{	
		m_ImageW = UniFrame.ImageW;
		m_ImageH = UniFrame.ImageH;
		m_ImageStep = UniFrame.ImageStep;
		m_BitCount = UniFrame.BitCount;		
		::memcpy(m_ImageBufferPtr, UniFrame.ImagePtr, sizeof(unsigned char)*BuffserSize);
	}	
	else
	{	::memset(m_ImageBufferPtr, 0x00, sizeof(unsigned char)*m_ImageBufferSize);	}	

	CreateImageSourceImage();
	UpdateShowBufferImage();
	DrawImageWndMemDC();
	m_UniFrameList = UniFrameList;		
	//JetAPI::ClearUniFrameList(UniFrameList);
	RedrawWnd();
	OnReadTemperatureBtn();

	if ( ExecAutoFocusNext() == false )
	{
		LockUIWnd(false);
		return false;
	}
	if ( CAMERA_CTRL_GRAB_NORMAL == m_CameraCtrlGrabMode )
	{
		BOOL bLive = this->IsDlgButtonChecked(CAMERACTRL_GRAB_CONTINUE_CHK);
		if ( bLive )
		{	
			if ( ExecGrabFrameNext() == false )
			{	return false; }
			return true;
		}
	}
	LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnResetLightCtrlBtn() 
{
	// TODO: Add your control notification handler code here	
	CString str;	
	str.Format(_T("CCameraCtrlWnd::OnResetLightCtrlBtn Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}	
	str.Format(_T("CCameraCtrlWnd::OnResetLightCtrlBtn End"));
	AOIDataCollect.SaveMovingTimeMsg(str);
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSelchangeFrame2dCombox() 
{
	// TODO: Add your control notification handler code here
	CWnd *WndPtr = CWnd::GetDlgItem(CAMERACTRL_COUNT_INFO_EDIT);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{	WndPtr->SetFocus();	}

	ExecGrabFrame();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::ResetCameraCount()
{
	this->m_cntCameraBak = 0;
	this->m_cntExpBak = 0;
	this->m_cntImageBak = 0;
	this->m_cntImageCpy = 0;	
	this->m_cntImageBypass = 0;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnFocusAutoBtn() 
{
	// TODO: Add your control notification handler code here	
	CString str;
	str = _T("Do you want to exec Auto-Focus process?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }

	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;
	CWnd::GetDlgItemText(CAMERACTRL_FOCUS_AUTO_PITCH_EDIT, str);	
	m_AutoFocusPitch = ::_tcstod(str, NULL);		
	if ( m_AutoFocusPitch < 5 ) 
	{	m_AutoFocusPitch = 5; }
	this->m_AutoFocustBestStd = 0;
	this->m_AutoFocusReadingList.clear();
	this->m_AutoFocustBestPosZ = MotionCtrlPtr->GetMotionParameter().m_StageStartPosZ;	
	const double MinZ = MotionCtrlPtr->GetMotionParameter().m_LimitMinZ+m_AutoFocusPitch;
	const double MaxZ = MotionCtrlPtr->GetMotionParameter().m_LimitMaxZ-m_AutoFocusPitch;	
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	TPOINT2D ImagePt1, ImagePt2;
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);
	
	m_AutoFrcusRect.left   = MIN(ImagePt1.x, ImagePt2.x);
	m_AutoFrcusRect.top    = MIN(ImagePt1.y, ImagePt2.y);
	m_AutoFrcusRect.right  = MAX(ImagePt1.x, ImagePt2.x);
	m_AutoFrcusRect.bottom = MAX(ImagePt1.y, ImagePt2.y);
	if ( m_AutoFrcusRect.left < 0 ) { m_AutoFrcusRect.left = 0; }
	if ( m_AutoFrcusRect.right > ImageW ) { m_AutoFrcusRect.right = ImageW; }
	if ( m_AutoFrcusRect.top < 0 ) { m_AutoFrcusRect.top = 0; }
	if ( m_AutoFrcusRect.bottom > ImageH ) { m_AutoFrcusRect.bottom = ImageH; }
	const double RectW = m_AutoFrcusRect.right-m_AutoFrcusRect.left;
	const double RectH = m_AutoFrcusRect.bottom-m_AutoFrcusRect.top;
	if ( RectH<1.0 || RectW<1.0 )
	{
		str = _T("Select the region First");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}

	m_AutoFocustScalePosZ = 100.0;
	m_AutoFocustMaxPosZ = MaxZ;
	m_AutoFocustMinPosZ = MinZ;	
	m_AutoFocustLastPosZ = -FLT_MAX;
	RetrieveStagePosition();
	if ( MotionCtrlPtr->XYZMoveTo(m_StagePosX, m_StagePosY, MinZ) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
		return;
	}

	CWnd::CheckDlgButton(CAMERACTRL_FRAME_3D_CHK, FALSE);
	CWnd::CheckDlgButton(CAMERACTRL_GRAB_CONTINUE_CHK, FALSE);
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_AUTO_FOCUS_100;
	LockUIWnd(true);
	if ( ExecGrabFunc() == false )
	{	
		m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;
		LockUIWnd(false);
		return ; 
	}
//	switch 
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecAutoFocusNext()
{
	bool UseAutoFocus=false;
	switch ( m_CameraCtrlGrabMode )
	{
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_1:
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_10:
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_100:
		UseAutoFocus = true;
		break;
	default:
		UseAutoFocus = false;
		break;
	}
	if ( false == UseAutoFocus ) { return true; }
	
	const char fnName[] = "CCameraCtrlWnd::ExecAutoFocusNext";
	bool        bFinish = false;
	CString     str, str1, str2;	
	BOOL        bSave = FALSE;
	RECT        ImageRect;
	TImageStat  Statistics;	
	double       CurPosX=0, CurPosY=0, CurPosZ=0;
	double       NextPosX=0, NextPosY=0, NextPosZ=0;	
	const double Margin = 50;//um
	const double LimitMinZ = MotionCtrlPtr->GetMotionParameter().m_LimitMinZ + Margin;
	const double LimitMaxZ = MotionCtrlPtr->GetMotionParameter().m_LimitMaxZ - Margin;
	
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	IMAGE_SIZE   ImageStep = m_ImageStep;
	IMAGE_SIZE   BitCount = m_BitCount;
	IMAGE_PTR    ImagePtr = m_ImageBufferPtr;
	IMAGE_SIZE   GrayBitCount = 8;
	IMAGE_SIZE   GrayStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBitCount, 4);
	IMAGE_PTR    GrayBuffer = NULL;
	IMAGE_PTR    ImageBuffer = NULL;		
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, GrayBuffer, fnName, "GrayBuffer") == false || 
		 JetMemory.alloc_func(BufferSize, ImageBuffer, fnName, "ImageBuffer") == false )
	{
		JetMemory.free_func(GrayBuffer);
		JetMemory.free_func(ImageBuffer);
		return false; 
	}

	bFinish = false;
	MotionCtrlPtr->GetCurrentPos(CurPosX, CurPosY, CurPosZ);
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_FOCUS_AUTO.PNG"));
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif

	switch ( m_CameraCtrlGrabMode )
	{
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_1:
		break;
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_10:
		break;
	case CAMERA_CTRL_GRAB_AUTO_FOCUS_100:
		break;
	}
	
	if ( GrayStep == ImageStep )
	{	::memcpy(GrayBuffer, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
	else
	{
		ImageRect.left = 0;
		ImageRect.top = 0;
		ImageRect.right = ImageW;
		ImageRect.bottom = ImageH;
		if ( 24 == BitCount )
		{
			if ( ImageAPI.ColorImageToGrayImage3(ImageW, ImageH, ImageStep, ImagePtr, ImageRect, GrayStep, GrayBuffer, IMAGE_SRC_GRAY, 100, 100, 100, false) == false )
			{
				JetMemory.free_func(GrayBuffer);
				JetMemory.free_func(ImageBuffer);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return false;
			}
		}
		else
		{
			if ( ImageAPI.AlignGrayImageBuffer3(ImageW, ImageH, ImageStep, ImagePtr, GrayStep, GrayBuffer, false) == false )
			{
				JetMemory.free_func(GrayBuffer);
				JetMemory.free_func(ImageBuffer);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return false;
			}
		}
	}
	JetAPI::Rect4DToRect(m_AutoFrcusRect, ImageRect);
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CAMERA_FOCUS_AUTO_GRAY.PNG"));
		ImageAPI.SaveImage(str, ImageW, ImageH, GrayStep, GrayBitCount, GrayBuffer, true);
	}
#endif
	
	IMAGE_SIZE RoiW = ImageRect.right-ImageRect.left;
	IMAGE_SIZE RoiH = ImageRect.bottom-ImageRect.top;
	IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, GrayBitCount, 4);	
	if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, GrayStep, GrayBuffer, ImageRect, RoiStep, ImageBuffer, false) == false ) 
	{
		JetMemory.free_func(GrayBuffer);
		JetMemory.free_func(ImageBuffer);
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		return false;
	}
	::memcpy(GrayBuffer, ImageBuffer, sizeof(IMAGE_DATA)*RoiStep*RoiH);
	if ( AOIDataCollect.BuildFocusImage(RoiW, RoiH, RoiStep, GrayBuffer, ImageBuffer) == false )
	{
		JetMemory.free_func(GrayBuffer);
		JetMemory.free_func(ImageBuffer);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}	
	Statistics.m_Rect.left = 0;
	Statistics.m_Rect.right = RoiW;
	Statistics.m_Rect.top = 0;
	Statistics.m_Rect.bottom = RoiH;
	CALC_FOCUS_MODE CalcFocusMode = AOIDataCollect.GetSystemParameter().m_CalcFocusMode;	
	//if ( ImageAPI.CalcGrayImageStatistics(RoiW, RoiH, RoiStep, ImageBuffer, Statistics) == false )
	if ( AOIDataCollect.CalcImageFocusValue(RoiW, RoiH, RoiStep, GrayBuffer, ImageBuffer, CalcFocusMode, Statistics.m_Rect, Statistics.m_Std) == false )
	{	
		JetMemory.free_func(GrayBuffer);
		JetMemory.free_func(ImageBuffer);
		//JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	JetMemory.free_func(GrayBuffer);
	JetMemory.free_func(ImageBuffer);
	//Statistics.m_Std = Statistics.m_Std*3.0;		
	if ( Statistics.m_Std > m_AutoFocustBestStd )
	{
		m_AutoFocustBestPosZ = CurPosZ;
		m_AutoFocustBestStd = Statistics.m_Std;
	}
	str.Format(_T("Reading:%.2f[Z:%.0f]"), Statistics.m_Std, CurPosZ);
	CWnd::SetDlgItemText(CAMERACTRL_MOTION_POS_EDIT, str);
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
		if ( ExecGrabNextFunc() == false )
		{	return false; }
		return true;
	}		
	if ( CAMERA_CTRL_GRAB_AUTO_FOCUS_100 == m_CameraCtrlGrabMode )
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
		m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_AUTO_FOCUS_10;
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, NextPosZ, MOTION_MOVING_NORMAL) == false )
		{				
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
			return false;
		}	
		if ( ExecGrabNextFunc() == false )
		{	return false; }
		TPOINT2D Reading;
		m_AutoFocusReadingList.push_back(Reading);
		return true;
	}
	if ( CAMERA_CTRL_GRAB_AUTO_FOCUS_10 == m_CameraCtrlGrabMode )
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
		m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_AUTO_FOCUS_1;		
		if ( MotionCtrlPtr->MoveTo(AXIS_Z, NextPosZ, MOTION_MOVING_NORMAL) == false )
		{				
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());		
			return false;
		}	
		if ( ExecGrabNextFunc() == false )
		{	return false; }
		TPOINT2D Reading;
		m_AutoFocusReadingList.push_back(Reading);
		return true;
	}
	SaveAutoFocusReading(m_AutoFocusReadingList);

	//找出最大的數值
	const bool UpdateFocus=false;
	str1 = _T("Auto Focus Done, Best Focus");
	str1 = LoadMultiLanguageString(str1, str1);	
	if ( true == UpdateFocus )
	{
		str2 = _T("Do you want to set the value to system?");
		str2 = LoadMultiLanguageString(str2, str2);
		str.Format(_T("%s=%.2f, Z:%.0f.\n%s"), str1, m_AutoFocustBestStd, m_AutoFocustBestPosZ, str2);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{			
			LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
			MotionCtrlPtr->SetFocusPosZ(m_AutoFocustBestPosZ, LaneID);
			MotionCtrlPtr->SaveMotionParameter();
		}
	}
	else
	{
		str.Format(_T("%s=%.2f, Z:%.0f"), str1, m_AutoFocustBestStd, m_AutoFocustBestPosZ);
		JetAPI::ShowMessageBox(str);
	}
	
	if ( MotionCtrlPtr->MoveTo(AXIS_Z, m_AutoFocustBestPosZ, MOTION_MOVING_NORMAL) == false )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	str.Format(_T("Z:%.0f um"), m_AutoFocustBestPosZ);
	this->SetDlgItemText(CAMERACTRL_MOTION_POS_EDIT, str);
	m_CameraCtrlGrabMode = CAMERA_CTRL_GRAB_NORMAL;
	bFinish = true;
	if ( ExecGrabNextFunc() == false )
	{	return false; }
	LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnBuild3dObjBtn() 
{
	// TODO: Add your control notification handler code here
	ExecBuild3DObject();
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecBuild3DObject()
{	
	size_t       i=0;
	bool         get3DData=false;
	TUNI_FRAME   UniFrame;
	const size_t UniFrameCount = m_UniFrameList.size();
	if ( 0 == UniFrameCount ) { return true; }

	get3DData=false;
	for ( i=0; i<UniFrameCount; i++ )
	{
		UniFrame = m_UniFrameList[i];
		if ( NULL==UniFrame.MaskPtr || NULL==UniFrame.SpacePtr ) 
		{	continue; }
		get3DData = true;
		break;
	}
	if ( false == get3DData ) { return false; }
	IMAGE_SIZE ImageW = m_UniFrameList[0].ImageW;
	IMAGE_SIZE ImageH = m_UniFrameList[0].ImageH;
	IMAGE_SIZE ImageStep = m_UniFrameList[0].ImageStep;
	IMAGE_SIZE BitCount  = m_UniFrameList[0].BitCount;
	IMAGE_PTR  ImagePtr = m_UniFrameList[0].ImagePtr;

	IMAGE_SIZE SpaceStep = UniFrame.ImageStep;
	MASK_PTR   MaskPtr   = UniFrame.MaskPtr;
	SPACE_PTR  SpacePtr  = UniFrame.SpacePtr;	
	const int  ScaleMode = JetAPI::GetComboxCurSelData(m_ImageScaleCombox);	
	
	TRECT4D  Rect4D;
	RECT     RoiRect={0};
	TPOINT2D ImagePt1, ImagePt2;
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);
	
	Rect4D.left   = MIN(ImagePt1.x, ImagePt2.x);
	Rect4D.top    = MIN(ImagePt1.y, ImagePt2.y);
	Rect4D.right  = MAX(ImagePt1.x, ImagePt2.x);
	Rect4D.bottom = MAX(ImagePt1.y, ImagePt2.y);	
	JetAPI::Rect4DToRect(Rect4D, RoiRect);
	if ( RoiRect.left< 0 )
	{	RoiRect.left = 0; }
	if ( RoiRect.top < 0 )
	{	RoiRect.top = 0; }
	if ( RoiRect.right > ImageW )
	{	RoiRect.right = ImageW; }
	if ( RoiRect.bottom > ImageH )
	{	RoiRect.bottom = ImageH; }
	IMAGE_SIZE RoiRectW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiRectH=RoiRect.bottom-RoiRect.top;
	IMAGE_SIZE RoiRectStep = JetAPI::GetBMPImagePixelsPerLine(RoiRectW, 4);
	RoiRect.right = RoiRect.left+RoiRectStep;
	if ( RoiRect.right > ImageW )//為了與繪圖同步, 因此調整ROI的寬度至4的倍數
	{	
		RoiRect.right = ImageW;
		RoiRect.left = RoiRect.right-RoiRectStep;
	}

	const int  nAlign = 1;
	MASK_PTR   RoiMaskPtr   = NULL;
	MASK_PTR   ResMaskPtr   = NULL;
	IMAGE_PTR  RoiImagePtr  = NULL;
	SPACE_PTR  RoiSpacePtr  = NULL;	
	SPACE_PTR  ResSpacePtr  = NULL;
	IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;	
	IMAGE_SIZE RoiBitCount = 8;	
	IMAGE_SIZE RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	IMAGE_SIZE RoiPhaseStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCount, nAlign);
	if ( RoiW>ImageW || RoiH>ImageH ) 
	{	return false; }

	if ( FN_DISABLE == ScaleMode )
	{
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiImageStep, RoiImagePtr, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}

		if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, SpaceStep, SpacePtr, RoiRect, RoiPhaseStep, RoiSpacePtr, false) == false )
		{		
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}

		if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, SpaceStep, MaskPtr, RoiRect, RoiPhaseStep, RoiMaskPtr, false) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}	
	}
	else
	{
		IMAGE_SIZE DstW=0;
		IMAGE_SIZE DstH=0;
		IMAGE_SIZE DstStep=0;
		double Scale = ScaleMode;
		Scale = 1.0/Scale;
		if ( ImageAPI.ScaleImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Scale, DstW, DstH, DstStep, RoiImagePtr) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		RoiImageStep = DstStep;
		if ( ImageAPI.ScaleSpace(ImageW, ImageH, SpaceStep, SpacePtr, Scale, DstW, DstH, DstStep, RoiSpacePtr) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		if ( ImageAPI.ScaleMask(ImageW, ImageH, SpaceStep, MaskPtr, Scale, DstW, DstH, DstStep, RoiMaskPtr) == false )
		{
			JetMemory.free_func(RoiMaskPtr);
			JetMemory.free_func(RoiImagePtr);
			JetMemory.free_func(RoiSpacePtr);
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		RoiW = DstW;
		RoiH = DstH;		
		RoiPhaseStep = DstStep;
		if ( DstW != DstStep)
		{
			IMAGE_PTR  TempMskPtr=NULL;
			IMAGE_SIZE TempMskStep=DstW;
			if ( ImageAPI.AlignGrayImageBuffer(DstW, DstH, DstStep, RoiMaskPtr, TempMskStep, TempMskPtr, false) == false )
			{
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(RoiImagePtr);
				JetMemory.free_func(RoiSpacePtr);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return false;
			}
			JetMemory.free_func(RoiMaskPtr);
			RoiMaskPtr = TempMskPtr;

			SPACE_PTR  TempSpcPtr=NULL;
			IMAGE_SIZE TempSpcStep=DstW;
			if ( ImageAPI.AlignSpaceImageBuffer(DstW, DstH, DstStep, RoiSpacePtr, TempSpcStep, TempSpcPtr, false) == false )
			{
				JetMemory.free_func(RoiMaskPtr);
				JetMemory.free_func(RoiImagePtr);
				JetMemory.free_func(RoiSpacePtr);
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return false;
			}
			JetMemory.free_func(RoiSpacePtr);
			RoiSpacePtr = TempSpcPtr;
			RoiPhaseStep = DstW;
		}		
		if ( 8 == BitCount )
		{
			if ( RoiImageStep != (DstW) )
			{
				IMAGE_PTR  TempPtr=NULL;
				IMAGE_SIZE TempStep=DstW;
				if ( ImageAPI.AlignImageBuffer(DstW, DstH, RoiImageStep, BitCount, RoiImagePtr, TempStep, TempPtr, false) == false )
				{
					JetMemory.free_func(RoiMaskPtr);
					JetMemory.free_func(RoiImagePtr);
					JetMemory.free_func(RoiSpacePtr);
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
				JetMemory.free_func(RoiImagePtr);
				RoiImagePtr = TempPtr;
				RoiImageStep = TempStep;
			}
		}
		if ( 24 == BitCount )
		{
			if ( RoiImageStep != (3*DstW) )
			{
				IMAGE_PTR  TempPtr=NULL;
				IMAGE_SIZE TempStep=3*DstW;
				if ( ImageAPI.AlignImageBuffer(DstW, DstH, RoiImageStep, BitCount, RoiImagePtr, TempStep, TempPtr, false) == false )
				{
					JetMemory.free_func(RoiMaskPtr);
					JetMemory.free_func(RoiImagePtr);
					JetMemory.free_func(RoiSpacePtr);
					JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
					return false;
				}
				JetMemory.free_func(RoiImagePtr);
				RoiImagePtr = TempPtr;
				RoiImageStep = TempStep;
			}
		}
	}

	RECT              TagRect={0};
	MASK_PTR          Mask2DPtr=NULL;
	IMAGE_PTR		  GuidedImagePtr = NULL;
	ImageAPI.GetGuidedImage(RoiW, RoiH, RoiImageStep, BitCount, RoiImagePtr, GuidedImagePtr);//20191126, Joe
	const bool        bOpenMP = true;
	TNoiseFilterParam NoiseFilterParam = m_NoiseFilterParam;	
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, RoiW*RoiH);
	if ( ImageAPI.BuildSpaceData(RoiW, RoiH, RoiPhaseStep, RoiSpacePtr, RoiMaskPtr, Mask2DPtr, nOpenMPCnt, NoiseFilterParam, ResSpacePtr, ResMaskPtr, GuidedImagePtr) == false )
	{
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiSpacePtr);
		JetMemory.free_func(GuidedImagePtr);

		JetMemory.free_func(ResMaskPtr);		
		JetMemory.free_func(RoiImagePtr);		
		JetMemory.free_func(ResSpacePtr);		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	JetMemory.free_func(RoiMaskPtr);
	JetMemory.free_func(RoiSpacePtr);
	JetMemory.free_func(GuidedImagePtr);

	if ( AOIDataCollect.ExecEnhanceDisplayImage(RoiW, RoiH, RoiImageStep, BitCount, RoiImagePtr, RoiImagePtr) == false )
	{
		JetMemory.free_func(RoiImagePtr);
		JetMemory.free_func(ResMaskPtr);		
		JetMemory.free_func(ResSpacePtr);		
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	float PadHeight = 0;
	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;
	bool IsColor = ImageAPI.CheckIsColor(BitCount);			
	if ( m_Draw3DWnd.CheckCalcObject(RoiW, RoiH) == true )	
	{	CalcObject(RoiW, RoiH, RoiPhaseStep, ResMaskPtr, ResSpacePtr, TagRect, PadHeight); }
	else
	{	::memset(&TagRect, 0x00, sizeof(TagRect));	}
	m_Draw3DWnd.Set3DData(ResSpacePtr, RoiImagePtr, RoiW, RoiH, RoiImageStep, IsColor, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, TagRect, PadHeight);	
	JetMemory.free_func(RoiImagePtr);
	JetMemory.free_func(ResMaskPtr);		
	JetMemory.free_func(ResSpacePtr);		

	m_Draw3DWnd.ShowWindow(SW_SHOW);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::SaveAutoFocusReading(const std::vector<TPOINT2D> &ReadingList)
{
	const size_t Count=ReadingList.size();
	if ( 0 == Count ) { return true; }

	FILE *pfile=NULL;	
	CString Filename;
	Filename.Format(_T("%s\\%s.TXT"), AOIDataCollect.GetAOITempDirectory(), _T("CameraAutoFocus"));		
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
bool CCameraCtrlWnd::CalcObject(IMAGE_SIZE ModelW, IMAGE_SIZE ModelH, IMAGE_SIZE ModelStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, RECT &ObjRect, float &ObjH)//求得物體的資料
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
void CCameraCtrlWnd::OnViewAllBtn() 
{
	// TODO: Add your control notification handler code here
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnView1x1Btn() 
{
	// TODO: Add your control notification handler code here	
	m_ImageZoom = 1.0;	
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrlWnd::ExecStageMoveTo(POINT point)
{	
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	TPOINT2D StagePt;
	TPOINT2D StageRgnCp;
	TPOINT2D ImagePt;
	TPOINT2D WndPt = point;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	StageRgnCp.x = PosX;
	StageRgnCp.y = PosY;
	ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	if ( AOIDataCollect.MapCameraPtToStage(m_CameraID, ImagePt, StageRgnCp, StagePt) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}	
	if ( MotionCtrlPtr->XYMoveTo(StagePt.x, StagePt.y, false) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	m_DrawRect = false; 
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
	PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnViewCenterLineChk() 
{
	// TODO: Add your control notification handler code here	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnFrame3dChk() 
{
	// TODO: Add your control notification handler code here
	ExecGrabFrame();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSelchangeImageSourceCombo() 
{
	// TODO: Add your control notification handler code here
	CreateImageSourceImage();
	UpdateShowBufferImage();
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnSelchangeDlpLedColorCombo()
{	
	const int DlpLedColor = JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox);	
#ifndef PHASE_CTRL_DISABLE	
	Light3DCtrl.SetAllLight3DLEDColor(DlpLedColor);	
	JetAPI::FocusCtrlWnd(this, CAMERACTRL_FOCUS_AUTO_PITCH_EDIT);
	ExecGrabFunc();
#endif//PHASE_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnResetCameraBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef CAMERA_OBJ_DISABLE
	bool IsOK = true;
	DWORD   ret;
	CString str;
	CString strFinish = AOIDataDefine.GetFinishText();
	str = _T("Do you want to re-initial Camera ?");
	str = LoadMultiLanguageString(str, str);
	ret = JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON2);
	if ( IDCANCEL == ret )
	{	return; }
	if (IDNO == ret)
	{
		LockUIWnd(false);
		CameraCtrl.ResetBatchGrabbing();
		return;	
	}
	LockUIWnd(true);
	if ( IDYES == ret )
	{	IsOK = CameraCtrl.InitialAllCamera();	}
	else//ResetCamera 有問題
	{	IsOK = CameraCtrl.ResetAllCamera();	}	
	LockUIWnd(false);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());	
		return;
	}	
#ifndef	LIGHT_CTRL_DISABLE
	CameraCtrl.SetAllCameraExternalTrigger();
#else
	CameraCtrl.SetAllCameraInternalTrigger();
#endif//LIGHT_CTRL_DISABLE
	JetAPI::ShowMessageBox(strFinish);		
#endif//CAMERA_OBJ_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnBasePlaneParamBtn() 
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
void CCameraCtrlWnd::OnSpaceNoiseFilterBtn() 
{
	// TODO: Add your control notification handler code here
	CSpaceNoiseFilterParamWnd Wnd;	
	TNoiseFilterParam NoiseFilterParam;
	
	NoiseFilterParam = m_NoiseFilterParam;
	Wnd.SetNoiseFilterParam(NoiseFilterParam);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }
	Wnd.GetNoiseFilterParam(NoiseFilterParam);

	TBasePlaneParam BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;
	m_NoiseFilterParam = NoiseFilterParam;
	m_NoiseFilterParam.BasePlaneParam = BasePlaneParam;
	AOIDataCollect.UpdateSystemNoiseFilterParamToProject();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnWhiteBalanceSetBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef CAMERA_OBJ_DISABLE	
	CString strWBR, strWBG, strWBB;
	double  WBR=0.0, WBG=0.0, WBB=0.0;	
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	
	CWnd::GetDlgItemText(CAMERACTRL_WHITE_BALANCE_RED_EDIT, strWBR);
	CWnd::GetDlgItemText(CAMERACTRL_WHITE_BALANCE_GREEN_EDIT, strWBG);
	CWnd::GetDlgItemText(CAMERACTRL_WHITE_BALANCE_BLUE_EDIT, strWBB);

	WBR = ::_ttof(strWBR);
	WBG = ::_ttof(strWBG);
	WBB = ::_ttof(strWBB);

	if ( CameraCtrl.SetCameraWhiteBalance(CameraID, WBR, WBG, WBB) == false )
	{	JetAPI::ShowMessageBox(CameraCtrl.GetErrorString()); }
	else
	{	CameraCtrl.SaveCameraINIFile(CameraID);	}
#endif//CAMERA_OBJ_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnWhiteBalanceReadBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef CAMERA_OBJ_DISABLE	
	CString strWBR, strWBG, strWBB;
	double  WBR=0.0, WBG=0.0, WBB=0.0;
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));	
	
	if ( CameraCtrl.GetCameraWhiteBalance(CameraID, WBR, WBG, WBB) == false )
	{	
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString()); 
		return ;
	}
	strWBR.Format(_T("%.3f"), WBR);
	strWBG.Format(_T("%.3f"), WBG);
	strWBB.Format(_T("%.3f"), WBB);
	CWnd::SetDlgItemText(CAMERACTRL_WHITE_BALANCE_RED_EDIT, strWBR);
	CWnd::SetDlgItemText(CAMERACTRL_WHITE_BALANCE_GREEN_EDIT, strWBG);
	CWnd::SetDlgItemText(CAMERACTRL_WHITE_BALANCE_BLUE_EDIT, strWBB);
#endif//CAMERA_OBJ_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnWhiteBalanceCalcBtn() 
{
	// TODO: Add your control notification handler code here
#ifndef CAMERA_OBJ_DISABLE			
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));		
	if ( CameraCtrl.CalcCameraWhiteBalance(CameraID) == false )
	{	JetAPI::ShowMessageBox(CameraCtrl.GetErrorString()); }
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);	
#endif//CAMERA_OBJ_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrlWnd::OnReadTemperatureBtn()
{
#ifndef CAMERA_OBJ_DISABLE	
	CString str;
	CString str2;		
	const UINT CtrlID = CAMERACTRL_READ_TEMPERATURE_EDIT;
	const CAMERA_ID CameraID = (CAMERA_ID)(JetAPI::GetComboxCurSelData(this->m_CameraIdCombox));
	const double Temperature=CameraCtrl.ReadCameraTemperature(CameraID);	
	str.Format(_T("%.2f"), Temperature);
	CWnd::GetDlgItemText(CtrlID, str2);
	if ( str2.CompareNoCase(str) != 0 )
	{	CWnd::SetDlgItemText(CtrlID, str);	}
#endif//CAMERA_OBJ_DISABLE
	return;
}
//-------------------------------------------------------------------------------------//