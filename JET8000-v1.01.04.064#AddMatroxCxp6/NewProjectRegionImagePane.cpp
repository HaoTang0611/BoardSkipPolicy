// NewProjectRegionImagePane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectRegionImagePane.h"
//-------------------------------------------------------------------------------------//
#include "RulerWnd.h"
#include "InputBoxWnd.h"
#include "ImageCombineWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneRegionImage dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneRegionImage::CNewProjectPaneRegionImage(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneRegionImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneRegionImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	size_t   i=0;
	m_RegionPosXA = 0;
	m_RegionPosYA = 0;
	m_RegionPosZA = 0;
	m_RegionPosXB = 0;
	m_RegionPosYB = 0;
	m_RegionPosZB = 0;
	m_RegionWidth = 0;
	m_RegionLength = 0;	

	m_RegionPosXA_DB = 0;
	m_RegionPosYA_DB = 0;
	m_RegionPosZA_DB = 0;
	m_RegionPosXB_DB = 0;
	m_RegionPosYB_DB = 0;
	m_RegionPosZB_DB = 0;
	m_RegionWidth_DB = 0;
	m_RegionLength_DB = 0;		

	m_ProjectPtr = NULL;	
	m_FrameType = FRAME_COLOR;
	m_FrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;
	m_NewProjectMode = NEW_PROJECT_ONLINE;	

	m_ResetView = true;
	m_FovStageX = 0.0;
	m_FovStageY = 0.0;

	m_ImageW = 0;
	m_ImageH = 0;
	m_BitCount = 0;
	m_ImageStep = 0;
	m_ImageBuffer = NULL;	
	m_ImageBufferSize = 0;

	m_ShowBuffer = NULL;
	m_ShowBufferSize = NULL;	

	m_MapResolution_DA.x = m_MapResolution_DA.y = 10.0;
	m_MapResolution_DB.x = m_MapResolution_DB.y = 10.0;
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		m_MapW_DA[i] = 0;
		m_MapH_DA[i] = 0;	
		m_MapStep_DA[i] = 0;
		m_BitCount_DA[i] = 8;	
		m_MapBuffer_DA[i] = NULL;
		m_MapSize_DA[i] = 0;	

		m_MapW_DB[i] = 0;
		m_MapH_DB[i] = 0;	
		m_MapStep_DB[i] = 0;
		m_BitCount_DB[i] = 8;	
		m_MapBuffer_DB[i] = NULL;
		m_MapSize_DB[i] = 0;		
	}

	m_Finish = false;
	m_Finish_DA = false;
	m_Finish_DB = false;
	m_LiveGrab = FALSE;	
	m_DistrictID=DISTRICT_ID_A;
	m_EnableMultiDistrictMode = false;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneRegionImage)
	DDX_Control(pDX, REGION_HEIGHT_RATIO_COMBO, m_HeightRatioCombox);
	DDX_Control(pDX, REGION_DLP_LED_COLOR_COMBO, m_DlpLEDColorCombox);
	DDX_Control(pDX, REGION_MAP_SCALE_COMBO, m_MapScaleCombox);
	DDX_Control(pDX, REGION_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneRegionImage, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneRegionImage)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(REGION_SET_POS_BTN_A, OnSetPosBtnA)
	ON_BN_CLICKED(REGION_SET_POS_BTN_B, OnSetPosBtnB)
	ON_BN_CLICKED(REGION_MOVE_TO_POS_BTN_A, OnMoveToPosBtnA)
	ON_BN_CLICKED(REGION_MOVE_TO_POS_BTN_B, OnMoveToPosBtnB)
	ON_BN_CLICKED(REGION_MOVE_TO_SIZE_CORNER_BTN, OnMoveToSizeCornerBtn)
	ON_BN_CLICKED(REGION_MOVE_TO_STOP_BAR_BTN, OnMoveToStopBarBtn)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(REGION_GRAB_REGION_IMAGE_BTN, OnGrabRegionImageBtn)
	ON_BN_CLICKED(REGION_RESET_SYSTEM_BTN, OnResetSystemBtn)
	ON_BN_CLICKED(REGION_GRAB_LIVE_CHK, OnGrabLiveChk)
	ON_BN_CLICKED(REGION_LOAD_OFFLINE_BTN, OnLoadOfflineBtn)
	ON_BN_CLICKED(REGION_PCB_IN_BTN, OnPCBInBtn)
	ON_BN_CLICKED(REGION_PCB_OUT_BTN, OnPCBOutBtn)
	ON_BN_CLICKED(REGION_PCB_BACK_BTN, OnPCBBackBtn)
	ON_BN_CLICKED(REGION_PCB_CLAMP_ON_BTN, OnPCBClampOnBtn)	
	ON_BN_CLICKED(REGION_SHOW_RULER_BTN, OnShowRulerBtn)
	ON_CBN_SELCHANGE(REGION_MAP_SCALE_COMBO, OnSelchangeMapScaleCombo)
	ON_CBN_SELCHANGE(REGION_DLP_LED_COLOR_COMBO, OnSelchangeDlpLedColorCombo)
	ON_BN_CLICKED(REGION_PCB_IN_2ND_BTN, OnPCBIn2ndBtn)
	ON_BN_CLICKED(REGION_PCB_IN_3RD_BTN, OnPCBIn3rdBtn)
	ON_BN_CLICKED(REGION_LANE_ADJUST_WIDTH_BTN, OnLaneAdjustWidthBtn)
	ON_BN_CLICKED(REGION_DISTRICT_A_BTN, OnDistrictABtn)
	ON_BN_CLICKED(REGION_DISTRICT_B_BTN, OnDistrictBBtn)
	ON_BN_CLICKED(REGION_COMBINE_DISTRICT_BTN, OnCombineDistrictBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneRegionImage message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	CAOIProject *ProjectPtr = this->m_ProjectPtr;
//	m_ImageWnd.GetClientRect(&m_ImageWndRect);
//	m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);	
	m_ImageWnd.SetShowLBtnPos(false);			
	m_ImageWnd.SetShowWndCenterLine(true);
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);	
#ifdef _DEBUG
	m_ImageWnd.SetShowCursorInfo(true);
#endif

	double     LaneWidth = 0;
	LANE_ID    LaneID = AOIDataCollect.GetActiveLaneID();
	const bool IsRightIn = PlcCtrlPtr->GetIsPCBRightInDirection();	
	const bool LaneAdjustCanMove = PlcCtrlPtr->GetLaneAdjustCanMove(LaneID);
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
	if ( true == LaneAdjustCanMove )
	{
		LaneWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
		m_RegionLength_DB = m_RegionLength = LaneWidth + 2;
	}

	SwitchMultiLanguage();	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_BitCount = 8;
	m_ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	m_ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	m_ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, m_BitCount, 4);
	m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, false, true);

	AOIDataDefine.BuildProjectMapScaleModeCombox(m_MapScaleCombox);
	AOIDataDefine.BuidlProjectDlpLedColorCombox(m_DlpLEDColorCombox);
	AOIDataDefine.BuidlProjectSpaceToGrayRatioCombox(m_HeightRatioCombox);		

	if ( GetEnableMultiDistrictMode()==true || NEW_PROJECT_OFFLINE==GetNewProjectMode() )
	{
		CWnd::CheckDlgButton(REGION_SAVE_OFFLINE_IMAGE_CHK, TRUE);
		JetAPI::EnableCtrlWnd(this, REGION_SAVE_OFFLINE_IMAGE_CHK, FALSE);
	}
	else
	{	CWnd::CheckDlgButton(REGION_SAVE_OFFLINE_IMAGE_CHK, SysParam.m_DefaultProjectSaveOfflineImageFiles);	}

	bool RightSide = true;
	DISTRICT_ID DistrictID = GetDistrictID();		
	RightSide = AOIDataCollect.CheckPCBStopAtRightSide(IsRightIn, DISTRICT_ID_A);
	m_RegionPosXA = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	m_RegionPosXB = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	m_RegionPosYA = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	m_RegionPosYB = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	m_RegionPosZA = MotionCtrlPtr->GetMotionPCBStopPosZ(LaneID, RightSide);
	m_RegionPosZB = MotionCtrlPtr->GetMotionPCBStopPosZ(LaneID, RightSide);

	RightSide = AOIDataCollect.CheckPCBStopAtRightSide(IsRightIn, DISTRICT_ID_B);
	m_RegionPosXA_DB = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	m_RegionPosXB_DB = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	m_RegionPosYA_DB = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	m_RegionPosYB_DB = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	m_RegionPosZA_DB = MotionCtrlPtr->GetMotionPCBStopPosZ(LaneID, RightSide);
	m_RegionPosZB_DB = MotionCtrlPtr->GetMotionPCBStopPosZ(LaneID, RightSide);	

	if ( NULL != ProjectPtr )
	{		
		const int ScaleMode = ProjectPtr->GetProjectMapScaleMode();
		const int DlpLedColor = ProjectPtr->GetProjectDlpLedColorMode();		
		const int SpaceRatioMode = ProjectPtr->GetProjectSpaceToGrayRatioMode();	
		JetAPI::SetComboxCurSel(m_MapScaleCombox, ScaleMode);
		JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DlpLedColor);
		JetAPI::SetComboxCurSel(m_HeightRatioCombox, SpaceRatioMode);
		m_RegionPosZA = ProjectPtr->GetProjectFocusPos();
		m_RegionPosZB = ProjectPtr->GetProjectFocusPos();

		if ( true == m_Finish )
		{
			TSIZE2D MapLocSize_DA, MapLocSize_DB;
			ProjectPtr->GetProjectMapLocSize_DA(MapLocSize_DA);	
			ProjectPtr->GetProjectMapLocSize_DB(MapLocSize_DB);	
			const double MapSizeX_DA=MapLocSize_DA.cx;
			const double MapSizeY_DA=MapLocSize_DA.cy;
			const double MapSizeX_DB=MapLocSize_DB.cx;
			const double MapSizeY_DB=MapLocSize_DB.cy;
			const double MapSizeX_DA_mm=JetAPI::Unit_UmtoMM(MapSizeX_DA);
			const double MapSizeY_DA_mm=JetAPI::Unit_UmtoMM(MapSizeY_DA);
			const double MapSizeX_DB_mm=JetAPI::Unit_UmtoMM(MapSizeX_DB);
			const double MapSizeY_DB_mm=JetAPI::Unit_UmtoMM(MapSizeY_DB);
			m_RegionWidth = MapSizeX_DA_mm;
			m_RegionLength = MapSizeY_DA_mm;
			m_RegionWidth_DB = MapSizeX_DB_mm;
			m_RegionLength_DB = MapSizeY_DB_mm;
		}
	}
	UpdateParamToUI();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
	this->m_ImageBuffer = NULL;
	this->m_ImageBufferSize = 0;

	this->m_ShowBuffer = NULL;
	this->m_ShowBufferSize = 0;	

	ClearMapBufferSet(DISTRICT_ID_A);
	ClearMapBufferSet(DISTRICT_ID_B);	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		this->m_ImageWnd.MoveWindow(&WndRect);
		//this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
		//this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_REGION_IMAGE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_REGION_IMAGE;
	WndKey = _T("IDD_NEW_PROJECT_PANE_REGION_IMAGE");
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
	WndID = REGION_REGION_CORNER_GROUP;
	WndKey = _T("REGION_REGION_CORNER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_CORNER_UNIT_LABEL;
	WndKey = _T("REGION_CORNER_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_CORNER_X_LABEL;
	WndKey = _T("REGION_CORNER_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_CORNER_Y_LABEL;
	WndKey = _T("REGION_CORNER_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_CORNER_Z_LABEL;
	WndKey = _T("REGION_CORNER_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_MOVE_TO_POS_BTN_A;
	WndKey = _T("REGION_MOVE_TO_POS_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_SET_POS_BTN_A;
	WndKey = _T("REGION_SET_POS_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_MOVE_TO_POS_BTN_B;
	WndKey = _T("REGION_MOVE_TO_POS_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_SET_POS_BTN_B;
	WndKey = _T("REGION_SET_POS_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_REGION_SIZE_GROUP;
	WndKey = _T("REGION_REGION_SIZE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	m_SizeGroupText = NewLabelText;

	WndID = REGION_REGION_UNIT_LABEL;
	WndKey = _T("REGION_REGION_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_REGION_WIDTH_LABEL;
	WndKey = _T("REGION_REGION_WIDTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_REGION_LENGTH_LABEL;
	WndKey = _T("REGION_REGION_LENGTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_MOVE_TO_SIZE_CORNER_BTN;
	WndKey = _T("REGION_MOVE_TO_SIZE_CORNER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_MOVE_TO_STOP_BAR_BTN;
	WndKey = _T("REGION_MOVE_TO_STOP_BAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_DLP_LED_COLOR_LABEL;
	WndKey = _T("REGION_DLP_LED_COLOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = REGION_HEIGHT_RATIO_LABEL;
	WndKey = _T("REGION_HEIGHT_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = REGION_MAP_SCALE_LABEL;
	WndKey = _T("REGION_MAP_SCALE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = REGION_GRAB_LIVE_CHK;
	WndKey = _T("REGION_GRAB_LIVE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = REGION_SAVE_OFFLINE_IMAGE_CHK;
	WndKey = _T("REGION_SAVE_OFFLINE_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	
	WndID = REGION_GRAB_REGION_IMAGE_BTN;
	WndKey = _T("REGION_GRAB_REGION_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_RESET_SYSTEM_BTN;
	WndKey = _T("REGION_RESET_SYSTEM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_LOAD_OFFLINE_BTN;
	WndKey = _T("REGION_LOAD_OFFLINE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REGION_PCB_CTRL_GROUP;
	WndKey = _T("REGION_PCB_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_IN_BTN;
	WndKey = _T("REGION_PCB_IN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_OUT_BTN;
	WndKey = _T("REGION_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_BACK_BTN;
	WndKey = _T("REGION_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_CLAMP_ON_BTN;
	WndKey = _T("REGION_PCB_CLAMP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_IN_2ND_BTN;
	WndKey = _T("REGION_PCB_IN_2ND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_PCB_IN_3RD_BTN;
	WndKey = _T("REGION_PCB_IN_3RD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_LANE_ADJUST_WIDTH_BTN;
	WndKey = _T("REGION_LANE_ADJUST_WIDTH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = REGION_SHOW_RULER_BTN;
	WndKey = _T("REGION_SHOW_RULER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = REGION_DISTRICT_A_BTN;
	WndKey = _T("REGION_DISTRICT_A_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_DISTRICT_B_BTN;
	WndKey = _T("REGION_DISTRICT_B_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REGION_COMBINE_DISTRICT_BTN;
	WndKey = _T("REGION_COMBINE_DISTRICT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	

	/*
	WndID = AAAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
	
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneRegionImage::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_REGION_IMAGE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CNewProjectPaneRegionImage::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetShowBuffer(size_t BufferSize, IMAGE_PTR Ptr)
{
	this->m_ShowBuffer = Ptr;	
	this->m_ShowBufferSize = BufferSize;	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetImageBuffer(size_t BufferSize, IMAGE_PTR Ptr)
{
	this->m_ImageBuffer = Ptr;	
	this->m_ImageBufferSize = BufferSize;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnSetPosBtnA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	DISTRICT_ID DistrictID = GetDistrictID();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_RegionPosXA_DB = PosX;
		m_RegionPosYA_DB = PosY;	
		m_RegionPosZA_DB = PosZ;	
		m_RegionWidth_DB = ::fabs(m_RegionPosXA_DB-m_RegionPosXB_DB);
		m_RegionLength_DB = ::fabs(m_RegionPosYA_DB-m_RegionPosYB_DB);
		m_RegionWidth_DB = JetAPI::Unit_UmtoMM(m_RegionWidth_DB);
		m_RegionLength_DB = JetAPI::Unit_UmtoMM(m_RegionLength_DB);
	}
	else
	{
		m_RegionPosXA = PosX;
		m_RegionPosYA = PosY;	
		m_RegionPosZA = PosZ;	
		m_RegionWidth = ::fabs(m_RegionPosXA-m_RegionPosXB);
		m_RegionLength = ::fabs(m_RegionPosYA-m_RegionPosYB);
		m_RegionWidth = JetAPI::Unit_UmtoMM(m_RegionWidth);
		m_RegionLength = JetAPI::Unit_UmtoMM(m_RegionLength);
	}	
	
	UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnSetPosBtnB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	DISTRICT_ID DistrictID = GetDistrictID();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_RegionPosXB_DB = PosX;
		m_RegionPosYB_DB = PosY;	
		m_RegionPosZB_DB = PosZ;	
		m_RegionWidth_DB = ::fabs(m_RegionPosXA_DB-m_RegionPosXB_DB);
		m_RegionLength_DB = ::fabs(m_RegionPosYA_DB-m_RegionPosYB_DB);
		m_RegionWidth_DB = JetAPI::Unit_UmtoMM(m_RegionWidth_DB);
		m_RegionLength_DB = JetAPI::Unit_UmtoMM(m_RegionLength_DB);	
	}
	else
	{
		m_RegionPosXB = PosX;
		m_RegionPosYB = PosY;	
		m_RegionPosZB = PosZ;	
		m_RegionWidth = ::fabs(m_RegionPosXA-m_RegionPosXB);
		m_RegionLength = ::fabs(m_RegionPosYA-m_RegionPosYB);
		m_RegionWidth = JetAPI::Unit_UmtoMM(m_RegionWidth);
		m_RegionLength = JetAPI::Unit_UmtoMM(m_RegionLength);	
	}		
	UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnMoveToPosBtnA() 
{
	// TODO: Add your control notification handler code here
	double StagePosX=0, StagePosY=0;
	DISTRICT_ID DistrictID = GetDistrictID();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		StagePosX = m_RegionPosXA_DB;
		StagePosY = m_RegionPosYA_DB;
	}
	else
	{
		StagePosX = m_RegionPosXA;
		StagePosY = m_RegionPosYA;
	}
	if ( MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnMoveToPosBtnB() 
{
	// TODO: Add your control notification handler code here
	double StagePosX=0, StagePosY=0;
	DISTRICT_ID DistrictID = GetDistrictID();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		StagePosX = m_RegionPosXB_DB;
		StagePosY = m_RegionPosYB_DB;
	}
	else
	{
		StagePosX = m_RegionPosXB;
		StagePosY = m_RegionPosYB;
	}
	if ( MotionCtrlPtr->XYMoveTo(m_RegionPosXB, m_RegionPosYB, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnMoveToSizeCornerBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PCBW = 0;
	double PCBH = 0;	
	DISTRICT_ID  DistrictID = GetDistrictID();		
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		this->GetDlgItemText(REGION_CORNER_WIDTH_EDIT, str);
		this->m_RegionWidth_DB = ::_tcstod(str, NULL);
		this->GetDlgItemText(REGION_CORNER_LENGTH_EDIT, str);
		this->m_RegionLength_DB = ::_tcstod(str, NULL);
		PCBW = JetAPI::Unit_MMtoUM(m_RegionWidth_DB);
		PCBH = JetAPI::Unit_MMtoUM(m_RegionLength_DB);
	}
	else
	{
		this->GetDlgItemText(REGION_CORNER_WIDTH_EDIT, str);
		this->m_RegionWidth = ::_tcstod(str, NULL);
		this->GetDlgItemText(REGION_CORNER_LENGTH_EDIT, str);
		this->m_RegionLength = ::_tcstod(str, NULL);
		PCBW = JetAPI::Unit_MMtoUM(m_RegionWidth);
		PCBH = JetAPI::Unit_MMtoUM(m_RegionLength);
	}	
	
	LANE_ID      LaneID = AOIDataCollect.GetActiveLaneID();
	const bool   IsRightIn = PlcCtrlPtr->GetIsPCBRightInDirection();		
	const bool   SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   SignY = AOIDataCollect.GetStageSignPositiveY();	
	const bool   RightSide = AOIDataCollect.CheckPCBStopAtRightSide(IsRightIn, DistrictID);	
	const double PCBStopX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	const double PCBStopY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);		
	MACHINE_CAMERA_SIDE CameraSide = AOIDataCollect.GetMachineCameraSide();
	double PCBPosX = 0;
	double PCBPosY = 0;		
	if ( false == RightSide ) //右進料
	{
		if ( MACHINE_CAMERA_BOT == CameraSide )//下照式
		{
			if ( false == SignX )
			{	PCBPosX = PCBStopX+(PCBW); }
			else
			{	PCBPosX = PCBStopX-(PCBW); }
		}
		else
		{
			if ( true == SignX )
			{	PCBPosX = PCBStopX+(PCBW); }
			else
			{	PCBPosX = PCBStopX-(PCBW); }			
		}
		if ( true == SignY )
		{	PCBPosY = PCBStopY+(PCBH); }
		else
		{	PCBPosY = PCBStopY-(PCBH); }
	}
	else
	{
		if ( MACHINE_CAMERA_BOT == CameraSide )//下照式
		{
			if ( false == SignX )
			{	PCBPosX = PCBStopX-(PCBW); }
			else
			{	PCBPosX = PCBStopX+(PCBW); }
		}
		else
		{
			if ( true == SignX )
			{	PCBPosX = PCBStopX-(PCBW); }
			else
			{	PCBPosX = PCBStopX+(PCBW); }
		}
		if ( true == SignY )
		{	PCBPosY = PCBStopY+(PCBH); }
		else
		{	PCBPosY = PCBStopY-(PCBH); }
	}	

	double PosX1=0, PosY1=0;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_RegionPosXA_DB = PCBStopX;
		m_RegionPosYA_DB = PCBStopY;	
		m_RegionPosZA_DB = PosZ;

		m_RegionPosXB_DB = PCBPosX;
		m_RegionPosYB_DB = PCBPosY;	
		m_RegionPosZB_DB = PosZ;
	}
	else
	{
		m_RegionPosXA = PCBStopX;
		m_RegionPosYA = PCBStopY;	
		m_RegionPosZA = PosZ;

		m_RegionPosXB = PCBPosX;
		m_RegionPosYB = PCBPosY;	
		m_RegionPosZB = PosZ;
	}
	UpdateParamToUI();	
	if ( MotionCtrlPtr->XYMoveTo(PCBPosX, PCBPosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnMoveToStopBarBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID      LaneID = AOIDataCollect.GetActiveLaneID();
	DISTRICT_ID  DistrictID=GetDistrictID();	
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();
	const bool   IsRightIn = PlcCtrlPtr->GetIsPCBRightInDirection();	
	const bool   RightSide = AOIDataCollect.CheckPCBStopAtRightSide(IsRightIn, DistrictID);
	const double StageX = MotionCtrlPtr->GetMotionPCBStopPosX(LaneID, RightSide);
	const double StageY = MotionCtrlPtr->GetMotionPCBStopPosY(LaneID, RightSide);
	if ( MotionCtrlPtr->XYMoveTo(StageX, StageY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::RedrawWnd()
{
	/*
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	HDC hBKDC = m_ImageWndMemDC.GetSafeHdc();	
	CString  str;
	CClientDC dc(&m_ImageWnd);	
	HDC hDC = dc.GetSafeHdc();	

	::IntersectClipRect(hDC, this->m_ImageWndRect.left, this->m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);	

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hBKDC, 0, 0, SRCCOPY ); 	
	*/
}
//-------------------------------------------------------------------------------------//
BOOL  CNewProjectPaneRegionImage::DrawCtrlWnd(WPARAM wParam, LPARAM lParam)//在控制像繪圖後重新繪圖
{
	UINT CtrlID = (UINT)(wParam);
	HDC  hDC = (HDC)(lParam);
	switch ( CtrlID )
	{
	case REGION_IMAGE_WND:
		break;
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		CString str;						
		CAOIProject *ProjectPtr = GetActiveProject();		
		LANE_ID      LaneID = AOIDataCollect.GetActiveLaneID();		
		if ( NULL != ProjectPtr )
		{	
			const int ScaleMode = ProjectPtr->GetProjectMapScaleMode();
			const int DlpLedColor = ProjectPtr->GetProjectDlpLedColorMode();		
			const int SpaceRatioMode = ProjectPtr->GetProjectSpaceToGrayRatioMode();	
			unsigned int FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FRAME_UNIQUE_ID_DLP);

			JetAPI::SetComboxCurSel(m_MapScaleCombox, ScaleMode);
			JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DlpLedColor);
			JetAPI::SetComboxCurSel(m_HeightRatioCombox, SpaceRatioMode);
			m_RegionPosZA = ProjectPtr->GetProjectFocusPos();
			m_RegionPosZB = ProjectPtr->GetProjectFocusPos();

			BOOL bEnable3DUI=TRUE;
			if ( -1 == FrameIndex )
			{	bEnable3DUI = FALSE; }			
			m_DlpLEDColorCombox.EnableWindow(bEnable3DUI);	
			m_HeightRatioCombox.EnableWindow(bEnable3DUI);
		}		
	#ifndef OFFLINE_VERSION
		AOIDataCollect.SetOfflineMode(false);
	#endif//OFFLINE_VERSION

		UpdateParamToUI();
		str = AOIDataDefine.GetLaneIDText(LaneID);
		CWnd::SetDlgItemText(REGION_LAN_ID_EDIT, str);
		if ( m_SizeGroupText.GetLength() > 0 )
		{
			double LaneLength=0.0;
			const TMotionParameter &MotionSystem=MotionCtrlPtr->GetMotionParameter();
			switch ( LaneID )
			{
			case LANE_ID_B:	LaneLength = ::fabs(MotionSystem.m_PCBStopRPosX_LB-MotionSystem.m_PCBStopLPosX_LB);	break;
			default:
			case LANE_ID_A:	LaneLength = ::fabs(MotionSystem.m_PCBStopRPosX_LA-MotionSystem.m_PCBStopLPosX_LA);	break;				
			}
			str.Format(_T("%s [X:%.0fmm]"), m_SizeGroupText, JetAPI::Unit_UmtoMM(LaneLength));
			CWnd::SetDlgItemText(REGION_REGION_SIZE_GROUP, str);
		}
		AOIDataCollect.SetProjectLightSetting(ProjectPtr);
		m_ImageWnd.ShowFittedZoom();		
		AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd()); 
		
		WPARAM wParam=0;
		if ( true == m_EnableMultiDistrictMode )
		{	
			if ( false == m_Finish_DA )
			{	wParam=MAKEWPARAM(REGION_DISTRICT_A_BTN, BN_CLICKED);	}
			else if ( false == m_Finish_DB )
			{	wParam=MAKEWPARAM(REGION_DISTRICT_B_BTN, BN_CLICKED);	}
			if ( 0 != wParam )
			{	CWnd::PostMessage(WM_COMMAND, wParam, NULL);	}
		}
		if ( 0 == wParam )
		{	ExecGrabImage();	}

		CWnd *ParentWnd=GetParent();	
		if ( NULL != ParentWnd )
		{	::PostMessage(ParentWnd->GetSafeHwnd(), MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, TRUE);	}
	}
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneRegionImage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	HWND hWnd = NULL;
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:
		m_ResetView = true;
		if ( this->UpdateFovImage(wParam, lParam, true) == false )
		{	this->LockUIWnd(false); }				
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecGrabImage();
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);	
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(false);		
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:		
		DrawCtrlWnd(wParam, lParam);
		break;	
	case MSG_MOTION_CALLBACK:		
		break;
	case MSG_INSPECTION_CALLBACK:
		if ( WPARAM_INSPECTION_FINISH == wParam )
		{	ExecFinishProjectMap();	}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			m_ResetView = false;
			if ( this->UpdateFovImage(PRIMARY_CAMERA_ID, lParam, false) == false )
			{	this->LockUIWnd(false); }
			break;
		case WPARAM_SHOW_PROJECT_MAP_WND:
			::SendMessage(CWnd::GetParent()->GetSafeHwnd(), message, wParam, lParam);
			break;
		case WPARAM_REDRAW_PROJECT_MAP:
			::SendMessage(CWnd::GetParent()->GetSafeHwnd(), message, wParam, lParam);
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			::SendMessage(CWnd::GetParent()->GetSafeHwnd(), message, wParam, lParam);
			break;		
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetProjectPtr(CAOIProject *ProjectPtr)
{
	bool GarbMapDone = false;
	this->m_ProjectPtr = ProjectPtr;
	if ( NULL != ProjectPtr )
	{	
		for ( int i=0; i<FRAME_MAX_COUNT; i++ )
		{
			if ( false == ProjectPtr->CheckProjectMapPtr(i) ) { continue; }
			GarbMapDone = true;
			break;
		}		
	}	
	m_Finish = GarbMapDone;	
	m_Finish_DA = m_Finish_DB = GarbMapDone;
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CNewProjectPaneRegionImage::GetDistrictID() const
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetDistrictID(DISTRICT_ID Mode)
{
	m_DistrictID = Mode;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneRegionImage::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
{	
	bool bGetImage = false;	
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	
	#ifndef LIGHT_CTRL_DISABLE
		if ( RetrieveCameraUniFrame(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#else
		if ( RetrieveCameraImage(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#endif//LIGHT_CTRL_DISABLE
		if ( false == bGetImage ) { return true; }
	}
	else
	{
		if ( LoadProgramOfflineImage(wParam, lParam, bGetImage) == false )
		{	return false; }		
	}

	//無適合的資料	
	if ( false == bGetImage )
	{	::memset(m_ShowBuffer, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	else
	{		
		if ( AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}		
	}
	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;
	this->CreateBKDC(m_ResetView); 
	this->LockUIWnd(false);

	if ( TRUE == this->m_LiveGrab )
	{	ExecGrabImage(); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return true;	
	}	
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);	
	if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	bGetImage = true;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像	
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();		
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}	
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return true; }

	TUNI_FRAME UniFrame = UniFrameList[0];
	const size_t MaxFrames = UniFrameList.size();
	if ( MapIndex>=0 && MapIndex<MaxFrames )
	{	UniFrame = UniFrameList[MapIndex]; }
	else
	{	UniFrame = UniFrameList[0]; }
	
	BuffserSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
	if ( NULL!=UniFrame.ImagePtr && BuffserSize <= m_ImageBufferSize )
	{	
		m_ImageW = UniFrame.ImageW;
		m_ImageH = UniFrame.ImageH;
		m_ImageStep = UniFrame.ImageStep;
		m_BitCount = UniFrame.BitCount;		
		::memcpy(m_ImageBuffer, UniFrame.ImagePtr, sizeof(unsigned char)*BuffserSize);
	}	
	else
	{	::memset(m_ImageBuffer, 0x00, sizeof(unsigned char)*m_ImageBufferSize);	}
	JetAPI::ClearUniFrameList(UniFrameList);
	bGetImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	
	double PosX=0, PosY=0, PosZ=0;
	const int  MaxFrames = FRAME_MAX_COUNT;
	TUNI_FRAME UniFrameList[FRAME_MAX_COUNT];
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	Pos.x = PosX;
	Pos.y = PosY;
	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	m_ResetView = JetAPI::CheckMoved(m_FovStageX, m_FovStageY, PosX, PosY);	
	m_FovStageX = PosX;
	m_FovStageY = PosY;	

	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_PROGRAM;
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	else
	{
		if ( MapIndex>=0 && MapIndex<MaxFrames )
		{
			BitCount = UniFrameList[MapIndex].BitCount;
			ImageStep = UniFrameList[MapIndex].ImageStep;	
			BuffserSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( NULL!=UniFrameList[MapIndex].ImagePtr && BuffserSize <= m_ShowBufferSize )
			{
				bGetImage = true;
				m_ImageW = ImageW;
				m_ImageH = ImageH;
				m_ImageStep = ImageStep;
				m_BitCount = BitCount;
				::memcpy(m_ImageBuffer, UniFrameList[MapIndex].ImagePtr, sizeof(unsigned char)*BuffserSize);
			}
		}
	}	
	JetAPI::ClearUniFrameList(UniFrameList, FRAME_MAX_COUNT);	
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::CreateBKDC(bool bResetView)
{	
	if ( NULL==m_ShowBuffer ) { return FALSE; }	
	TPOINT2D   ImageRes;
	TPOINT2D   StagePos;
	TREGION4D  StageRgn;
	IMAGE_SIZE ImageW=0, ImageH=0;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;	
	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer, false, bResetView);
	CNewProjectPaneRegionImage::RedrawProjectImageWnd();		
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::CheckFinish()
{
	CString str;	
#ifndef _DEBUG
	if ( true == m_EnableMultiDistrictMode )
	{	
		if ( false == m_Finish_DA )
		{
			str.Format(_T("Error, Grab Project Map First DA"));
			str = this->LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return false;
		}
		if ( false == m_Finish_DB )
		{
			str.Format(_T("Error, Grab Project Map First DB"));
			str = this->LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return false;
		}		
		if ( false == m_Finish )
		{
			str.Format(_T("Error, Combine Project Map First"));
			str = this->LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return false;
		}		
	}	
	else
	{
		if ( false == m_Finish )
		{
			str.Format(_T("Error, Grab Project Map First"));
			str = this->LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::LockUIWnd(bool bLock)
{
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	
	BOOL  bEnable2 = TRUE;	
	DISTRICT_ID DistrictID = GetDistrictID();

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }
	bEnable2 = bEnable;
#ifdef OFFLINE_VERSION
	bEnable = FALSE;

	CtrlID = REGION_GRAB_LIVE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_RESET_SYSTEM_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_SAVE_OFFLINE_IMAGE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = REGION_PCB_IN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_PCB_OUT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_PCB_BACK_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_PCB_CLAMP_ON_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = REGION_PCB_IN_2ND_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_PCB_IN_3RD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = REGION_LANE_ADJUST_WIDTH_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
#endif//OFFLINE_VERSION
	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	JetAPI::EnableCtrlWnd(this, REGION_PCB_OUT_BTN, FALSE);	}

	CtrlID = REGION_MOVE_TO_POS_BTN_A;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = REGION_MOVE_TO_POS_BTN_B;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = REGION_SET_POS_BTN_A;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = REGION_SET_POS_BTN_B;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = REGION_MOVE_TO_SIZE_CORNER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = REGION_MOVE_TO_STOP_BAR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = REGION_GRAB_LIVE_CHK;
//	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	if ( GetEnableMultiDistrictMode()==true || NEW_PROJECT_OFFLINE==GetNewProjectMode() )
	{	CtrlID = REGION_SAVE_OFFLINE_IMAGE_CHK;	}
	else
	{
		CtrlID = REGION_SAVE_OFFLINE_IMAGE_CHK;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	}

	CtrlID = REGION_MAP_SCALE_COMBO;
	if ( DISTRICT_ID_B == DistrictID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, FALSE);	}
	else
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	}

	CtrlID = REGION_GRAB_REGION_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	//Edit Control
	CtrlID = REGION_CORNER_WIDTH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = REGION_CORNER_LENGTH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	

#ifndef OFFLINE_VERSION
	bEnable = FALSE;
#else
	bEnable = TRUE;
#endif//OFFLINE_VERSION
	CtrlID = REGION_LOAD_OFFLINE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	if ( false == m_EnableMultiDistrictMode )
	{
		bEnable2 = FALSE;
		CtrlID = REGION_DISTRICT_A_BTN;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
		CtrlID = REGION_DISTRICT_B_BTN;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
		CtrlID = REGION_COMBINE_DISTRICT_BTN;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);		
	}
	else
	{
		bEnable2 = FALSE;
		if ( true == m_Finish_DA )
		{	JetAPI::EnableCtrlWnd(this, REGION_DISTRICT_B_BTN, TRUE);	}
		else
		{	JetAPI::EnableCtrlWnd(this, REGION_DISTRICT_B_BTN, FALSE);	}
		if ( false==m_Finish_DB || false==m_Finish_DA )
		{	JetAPI::EnableCtrlWnd(this, REGION_COMBINE_DISTRICT_BTN, FALSE);	}
		else
		{	JetAPI::EnableCtrlWnd(this, REGION_COMBINE_DISTRICT_BTN, TRUE);	}

		CtrlID = REGION_SET_POS_BTN_A;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
		CtrlID = REGION_SET_POS_BTN_B;
		JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);		
	}

	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::ExecGrabImage()
{	
	CString str;
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == this->m_ProjectPtr ) { return FALSE; }		
#ifndef OFFLINE_VERSION
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( true == OfflineMode )
	{
		PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
		return TRUE;
	}
	if ( AOIDataCollect.CheckCanGrabNextUniFrameImage() == false )
	{	return TRUE;	}
	bool  bCameraFinish = false;
	this->LockUIWnd(true);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
#else
	if ( AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		this->LockUIWnd(false);
		return FALSE;
	}	
#endif//LIGHT_CTRL_DISABLE	
	
#else
	PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
#endif//OFFLINE_VERSION
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnGrabRegionImageBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return ; }
	
	CString  str;
	TPOINT3D StagePosA;
	TPOINT3D StagePosB;
	DISTRICT_ID DistrictID = GetDistrictID();
	if ( DISTRICT_ID_B == DistrictID )
	{
		StagePosA.x = m_RegionPosXA_DB;
		StagePosA.y = m_RegionPosYA_DB;
		StagePosA.z = m_RegionPosZA_DB;
		StagePosB.x = m_RegionPosXB_DB;
		StagePosB.y = m_RegionPosYB_DB;	
		StagePosB.z = m_RegionPosZB_DB;	
	}
	else
	{
		StagePosA.x = m_RegionPosXA;
		StagePosA.y = m_RegionPosYA;
		StagePosA.z = m_RegionPosZA;
		StagePosB.x = m_RegionPosXB;
		StagePosB.y = m_RegionPosYB;	
		StagePosB.z = m_RegionPosZB;	
	}

	CString OfflineFolder;
	CString ProjectFolder;	
	CString ProjectName = ProjectPtr->GetProjectFileName();	
	BOOL bSaveOffline=CWnd::IsDlgButtonChecked(REGION_SAVE_OFFLINE_IMAGE_CHK);
	if ( GetEnableMultiDistrictMode() == true )
	{	bSaveOffline = true;	}

	JetAPI::ExtractMainFileName(ProjectName, ProjectFolder);
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(ProjectFolder);
	ProjectPtr->SetProjectOfflineFolder(OfflineFolder);
	ProjectPtr->SetProjectProgramOfflineFolder(OfflineFolder);	
	ProjectPtr->SetProjectSaveOfflineImageFiles((bool)(bSaveOffline));
	if ( JetAPI::CreateFolder(OfflineFolder) == false )
	{
		str.Format(_T("Erroe, Create Folder Fault (%s)"), OfflineFolder);
		JetAPI::ShowMessageBox(str);
		return;
	}
	m_Finish = false;
	if ( DISTRICT_ID_A==DistrictID )
	{
		m_Finish_DA = false;
		JetAPI::ClearFolder(OfflineFolder); 
	}
	else
	{	m_Finish_DB = false;	}	
	ClearMapBufferSet(DistrictID);

	const TASK_MODE TaskMode = TASK_PROJECT_MAP;
	const int ScaleMode = (int)(JetAPI::GetComboxCurSelData(m_MapScaleCombox));		
	const int DlpLedColor = (int)(JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox));
	const int SpaceRatioMode = (int)(JetAPI::GetComboxCurSelData(m_HeightRatioCombox));			

	ProjectPtr->SetProjectActTaskMode(TaskMode);
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);	
	ProjectPtr->SetProjectMapScaleMode(ScaleMode);
	ProjectPtr->SetProjectTestMapScaleMode(ScaleMode);
	ProjectPtr->SetProjectDlpLedColorMode(DlpLedColor);
	ProjectPtr->SetProjectSpaceToGrayRatioMode(SpaceRatioMode);	
	AOIDataCollect.SetSystemDlpLedColor(DlpLedColor);
	if ( ProjectPtr->CalcProjectMapBuffer(StagePosA, StagePosB) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetProjectErrorString());
		return;
	}	

	this->m_LiveGrab = FALSE;
	AOIDataCollect.SetTaskMode(TaskMode);
	AOIDataCollect.SetOfflineMode(false);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	

	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_INSPECTING);
	if ( AOIDataCollect.StartThreadSequenceThread(true) == false )
	{			
		SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->m_LiveGrab = this->IsDlgButtonChecked(REGION_GRAB_LIVE_CHK);
		return ;
	}
	this->LockUIWnd(true);
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::RedrawProjectImageWnd()
{	
	PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, NULL);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnResetSystemBtn() 
{
	// TODO: Add your control notification handler code here
	AOIDataCollect.IdleAllThread(false);
	AOIDataCollect.SetTaskMode(TASK_NONE);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
	CameraCtrl.ResetBatchGrabbing();
	LockUIWnd(false);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnGrabLiveChk() 
{
	// TODO: Add your control notification handler code here
	BOOL Check = this->IsDlgButtonChecked(REGION_GRAB_LIVE_CHK);
//	this->m_LiveGrab = Check;
	this->m_LiveGrab = FALSE;
	if ( TRUE == this->m_LiveGrab )
	{	ExecGrabImage(); }
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecNextPane()
{
	if ( CheckFinish() == false )
	{	return false; }
	UpdateProjectLandWidth();
	if ( true == m_EnableMultiDistrictMode )
	{	AOIDataCollect.SetOfflineMode(true);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecPrevPane()
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecFinishPane()
{
	if ( CheckFinish() == false )
	{	return false; }
	UpdateProjectLandWidth();
	//if ( true==m_EnableMultiDistrictMode && NEW_PROJECT_ONLINE==m_NewProjectMode )
	//{	AOIDataCollect.SetOfflineMode(true);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ReInitialPane()
{
	UpdateParamToUI();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::UpdateParamToUI()//將參數更新至介面
{
	CString str;
	DISTRICT_ID DistrictID = GetDistrictID();
	if ( NULL != m_ProjectPtr )
	{
		const int ScaleMode = m_ProjectPtr->GetProjectMapScaleMode();
		JetAPI::SetComboxCurSel(m_MapScaleCombox, ScaleMode);
	}
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(REGION_DISTRICT_EDIT, str);
	m_ImageWnd.SetImageText(str, true);
	if ( DISTRICT_ID_B == DistrictID )
	{
		str.Format(_T("%.0f"), this->m_RegionPosXA_DB);
		this->SetDlgItemText(REGION_CORNER_POS_X_EDIT_A, str);
		str.Format(_T("%.0f"), this->m_RegionPosYA_DB);
		this->SetDlgItemText(REGION_CORNER_POS_Y_EDIT_A, str);
		str.Format(_T("%.0f"), this->m_RegionPosZA_DB);
		this->SetDlgItemText(REGION_CORNER_POS_Z_EDIT_A, str);

		str.Format(_T("%.0f"), this->m_RegionPosXB_DB);
		this->SetDlgItemText(REGION_CORNER_POS_X_EDIT_B, str);
		str.Format(_T("%.0f"), this->m_RegionPosYB_DB);
		this->SetDlgItemText(REGION_CORNER_POS_Y_EDIT_B, str);
		str.Format(_T("%.0f"), this->m_RegionPosZB_DB);
		this->SetDlgItemText(REGION_CORNER_POS_Z_EDIT_B, str);

		str.Format(_T("%.3f"), this->m_RegionWidth_DB);
		this->SetDlgItemText(REGION_CORNER_WIDTH_EDIT, str);
		str.Format(_T("%.3f"), this->m_RegionLength_DB);
		this->SetDlgItemText(REGION_CORNER_LENGTH_EDIT, str);
	}
	else
	{
		str.Format(_T("%.0f"), this->m_RegionPosXA);
		this->SetDlgItemText(REGION_CORNER_POS_X_EDIT_A, str);
		str.Format(_T("%.0f"), this->m_RegionPosYA);
		this->SetDlgItemText(REGION_CORNER_POS_Y_EDIT_A, str);
		str.Format(_T("%.0f"), this->m_RegionPosZA);
		this->SetDlgItemText(REGION_CORNER_POS_Z_EDIT_A, str);

		str.Format(_T("%.0f"), this->m_RegionPosXB);
		this->SetDlgItemText(REGION_CORNER_POS_X_EDIT_B, str);
		str.Format(_T("%.0f"), this->m_RegionPosYB);
		this->SetDlgItemText(REGION_CORNER_POS_Y_EDIT_B, str);
		str.Format(_T("%.0f"), this->m_RegionPosZB);
		this->SetDlgItemText(REGION_CORNER_POS_Z_EDIT_B, str);

		str.Format(_T("%.3f"), this->m_RegionWidth);
		this->SetDlgItemText(REGION_CORNER_WIDTH_EDIT, str);
		str.Format(_T("%.3f"), this->m_RegionLength);
		this->SetDlgItemText(REGION_CORNER_LENGTH_EDIT, str);
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneRegionImage::UpdateUIToParam()//將介面更新至參數
{
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnLoadOfflineBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return ; }

	CString  str;
	CString  filename;	
	DISTRICT_ID DistrictID = GetDistrictID();
	//TCHAR szFilters[]=_T("Offline Files (*.INI)|*.INI|All Files (*.*)|*.*||");
	TCHAR szFilters[]=_T("Offline Files (*.OPG)|*.INI|All Files (*.*)|*.*||");
	CFileDialog dialog(TRUE, _T("OPG"), _T("*.OPG"), OFN_FILEMUSTEXIST, szFilters, this);
	if ( dialog.DoModal() == IDCANCEL ) 
	{	return ; }

	filename = dialog.GetPathName();
	if ( ExecLoadOfflineProgram(filename) == false )
	{	return; }

	if ( true == m_EnableMultiDistrictMode )
	{
		bool       bOk=true;
		size_t     i=0;
		size_t     MapSize=0;
		IMAGE_PTR  MapPtr=NULL;
		IMAGE_SIZE MapW=0, MapH=0, MapStep=0, MapBitCount=0;

		bOk=true;
		ClearMapBufferSet(DistrictID);
		if ( DISTRICT_ID_A == DistrictID ) 
		{	
			ProjectPtr->GetProjectMapTeachRgn(m_MapStageRgn_DA);	
			ProjectPtr->GetProjectMapResolution(m_MapResolution_DA.x, m_MapResolution_DA.y);
		}
		if ( DISTRICT_ID_B == DistrictID ) 
		{	
			ProjectPtr->GetProjectMapTeachRgn(m_MapStageRgn_DB);	
			ProjectPtr->GetProjectMapResolution(m_MapResolution_DB.x, m_MapResolution_DB.y);
		}
		for ( i=0; i<FRAME_MAX_COUNT; i++ )
		{	
			if ( ProjectPtr->GetProjectMapPtr(i, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
			{	continue; }
			if ( SetMapBuffer(i, MapW, MapH, MapStep, MapBitCount, MapPtr, DistrictID) == false ) 
			{	bOk = false;	}			
		}

		m_Finish = false;
		if ( true == bOk ) 
		{
			if ( DISTRICT_ID_A==DistrictID )
			{	m_Finish_DA = true;		}
			else
			{	m_Finish_DB = true;		}
		}		
	}
	else
	{	m_Finish = true;	}	
	m_LiveGrab = this->IsDlgButtonChecked(REGION_GRAB_LIVE_CHK);		
	
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecLoadOfflineProgram(LPCTSTR filename)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }

	DWORD    res;
	CString  str;
	CString  DstMapFile;
	CString  SrcMapFile;	
	CString  DstMapFolder;
	CString  SrcMapFolder;
	CString  ProjectFolder;
	CString  OfflineFolder;
	CString  DstOffelineFile;	
	CString  SrcOffelineFile;		
	DISTRICT_ID DistrictID = GetDistrictID();
	std::vector<unsigned int> FrameUniqueIDList;
	CString ProjectName = ProjectPtr->GetProjectFileName();	
	TProjectParameter &Param = ProjectPtr->GetProjectParameter();
	
	JetAPI::ExtractMainFileName(ProjectName, ProjectFolder);
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(ProjectFolder);
	ProjectPtr->SetProjectOfflineFolder(OfflineFolder);
	ProjectPtr->SetProjectProgramOfflineFolder(OfflineFolder);	
	if ( JetAPI::CreateFolder(OfflineFolder) == false )
	{
		str.Format(_T("Erroe, Create Folder Fault (%s)"), OfflineFolder);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	SrcMapFile = filename;
	JetAPI::ExtractFolder(SrcMapFile, SrcMapFolder);	
	DstMapFolder = ProjectPtr->GetProjectProgramOfflineFolder();			
	DstOffelineFile = AOIDataDefine.GetProjectOfflineFileName(DstMapFolder, DistrictID);
	SrcOffelineFile = AOIDataDefine.GetProjectOfflineFileName(SrcMapFolder, DistrictID);
	if ( DstMapFolder.CompareNoCase(SrcMapFolder) != 0 ) 
	{
		if ( JetAPI::CopyFolderAToFolderB(SrcMapFolder, DstMapFolder, false, true, _T(""), -1, -1) == false )
		{
			str.Format(_T("Error, Copy Folder Fault [%s ---> %s]"), SrcMapFolder, DstMapFolder);
			return false;
		}
	}		
	//DstMapFile.Format(_T("%s\\%s"), DstMapFolder, _T("Offline.OPG"));
	DstMapFile = AOIDataDefine.GetProjectOfflineMapName(DstMapFolder, DistrictID);
	if ( ProjectPtr->LoadProjectMapFile(DstMapFile) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	if ( ProjectPtr->LoadProjectProgramOfflineFile(DstOffelineFile, FrameUniqueIDList) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	int ScaleMode = ProjectPtr->GetProjectMapScaleMode();
	ProjectPtr->SetProjectTestMapScaleMode(ScaleMode);
	const size_t FrameUniqueIDCount = FrameUniqueIDList.size();
	if ( 0 != FrameUniqueIDCount )
	{
		if ( ProjectPtr->BuildProjectImageConfig(FrameUniqueIDList) == false )
		{
			JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
			return false;
		}	
	}

	
	TREGION4D StageRgn;
	ProjectPtr->GetProjectMapTeachRgn(StageRgn);
	ProjectPtr->ResetProjectMapTeachRgn(StageRgn);	

	AOIDataCollect.SetOfflineMode(true);
	AOIDataCollect.SetOfflineFileName(DstOffelineFile);
	const double StageCpX = StageRgn.GetCpX();
	const double StageCpY = StageRgn.GetCpY();	
	const double Width = StageRgn.GetWidth();
	const double Height = StageRgn.GetHeight();
	const double FocusOffsetZ = ProjectPtr->GetProjectProgramOfflineFocusOffsetZ();
	ProjectPtr->SetProjectFocusPosOffset(FocusOffsetZ);	

	Param.m_TestSizeWidth = JetAPI::Unit_UmtoMM(Width);
	Param.m_TestSizeHeight = JetAPI::Unit_UmtoMM(Height);	;

	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_RegionPosXA_DB = StageRgn.minX;
		m_RegionPosYA_DB = StageRgn.minY;
		m_RegionPosZA_DB = ProjectPtr->GetProjectFocusPos();
		m_RegionPosXB_DB = StageRgn.maxX;
		m_RegionPosYB_DB = StageRgn.maxY;
		m_RegionPosZB_DB = ProjectPtr->GetProjectFocusPos();
		m_RegionWidth_DB = Width;
		m_RegionLength_DB = Height;
		m_RegionWidth_DB = JetAPI::Unit_UmtoMM(Width);
		m_RegionLength_DB = JetAPI::Unit_UmtoMM(Height);		
		ProjectPtr->GetProjectMapTeachRgn_DB(m_MapStageRgn_DB);
		ProjectPtr->GetProjectMapResolution(m_MapResolution_DB.x, m_MapResolution_DB.y);
	}
	else
	{
		m_RegionPosXA = StageRgn.minX;
		m_RegionPosYA = StageRgn.minY;
		m_RegionPosZA = ProjectPtr->GetProjectFocusPos();
		m_RegionPosXB = StageRgn.maxX;
		m_RegionPosYB = StageRgn.maxY;
		m_RegionPosZB = ProjectPtr->GetProjectFocusPos();
		m_RegionWidth = Width;
		m_RegionLength = Height;
		m_RegionWidth = JetAPI::Unit_UmtoMM(Width);
		m_RegionLength = JetAPI::Unit_UmtoMM(Height);		
		ProjectPtr->GetProjectMapTeachRgn_DA(m_MapStageRgn_DA);
		ProjectPtr->GetProjectMapResolution(m_MapResolution_DA.x, m_MapResolution_DA.y);
	}
	this->UpdateParamToUI();	
	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StageCpX, StageCpY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();

	if ( AOIDataCollect.GetPreLoadProjectProgramImage() == false )
	{
		ExecGrabImage();
		return true; 
	}

#ifdef _DEBUG
	str.Format(_T("Do you want to pre load all program images?"));
	str = LoadMultiLanguageString(str, str);
	res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( IDNO == res ) { return true; }
#endif//_DEBUG

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);
	if ( ProjectPtr->ExecProjectPreLoadProgramImage() == false )
	{
		::SetCursor(hOldCursor);	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	::SetCursor(hOldCursor);	
	ExecGrabImage();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecFinishProjectMap()
{
	CString      str;	
	CString      OfflineFolder;
	TPOINT3D     StagePos;			
	CAOIProject *ProjectPtr = NULL;
	DISTRICT_ID  DistrictID = GetDistrictID();
	ProjectPtr = GetActiveProject();	
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
	if ( NULL != ProjectPtr )
	{		
		OfflineFolder = ProjectPtr->GetProjectProgramOfflineFolder();
		str = AOIDataDefine.GetProjectOfflineMapName(OfflineFolder, DistrictID);
		ProjectPtr->SaveProjectMapFile(str);
	}
	switch ( DistrictID )
	{
	case DISTRICT_ID_B:
		StagePos.x = (m_RegionPosXA_DB+m_RegionPosXB_DB)*0.5;
		StagePos.y = (m_RegionPosYA_DB+m_RegionPosYB_DB)*0.5;
		StagePos.z = (m_RegionPosZA_DB+m_RegionPosZB_DB)*0.5;
		break;
	case DISTRICT_ID_A:
		StagePos.x = (m_RegionPosXA+m_RegionPosXB)*0.5;
		StagePos.y = (m_RegionPosYA+m_RegionPosYB)*0.5;
		StagePos.z = (m_RegionPosZA+m_RegionPosZB)*0.5;
		break;
	}	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->XYZMoveTo(StagePos.x, StagePos.y, StagePos.z, OfflineMode);	
	LockUIWnd(false);
	if ( true == m_EnableMultiDistrictMode )
	{
		bool       bOk=true;
		size_t     i=0;
		size_t     MapSize=0;
		IMAGE_PTR  MapPtr=NULL;
		IMAGE_SIZE MapW=0, MapH=0, MapStep=0, MapBitCount=0;

		bOk=true;
		ClearMapBufferSet(DistrictID);
		if ( DISTRICT_ID_A == DistrictID ) 
		{	
			ProjectPtr->GetProjectMapTeachRgn(m_MapStageRgn_DA);	
			ProjectPtr->GetProjectMapResolution(m_MapResolution_DA.x, m_MapResolution_DA.y);
		}
		if ( DISTRICT_ID_B == DistrictID ) 
		{	
			ProjectPtr->GetProjectMapTeachRgn(m_MapStageRgn_DB);	
			ProjectPtr->GetProjectMapResolution(m_MapResolution_DB.x, m_MapResolution_DB.y);
		}
		for ( i=0; i<FRAME_MAX_COUNT; i++ )
		{	
			if ( ProjectPtr->GetProjectMapPtr(i, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
			{	continue; }
			if ( SetMapBuffer(i, MapW, MapH, MapStep, MapBitCount, MapPtr, DistrictID) == false ) 
			{	bOk = false;	}			
		}
		m_Finish = false;
		if ( true == bOk ) 
		{
			if ( DISTRICT_ID_A==DistrictID )
			{	m_Finish_DA = true;	}
			else
			{	m_Finish_DB = true;	}	
		}
	}
	else
	{	m_Finish = true;	}	
	m_LiveGrab = this->IsDlgButtonChecked(REGION_GRAB_LIVE_CHK);
	ExecGrabImage();
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBInBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBOutBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBBackBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBClampOnBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOnProc(LaneID) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnShowRulerBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CRulerWnd Wnd;
	Wnd.DoModal();	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnSelchangeMapScaleCombo()
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	const int ScaleMode = (int)(JetAPI::GetComboxCurSelData(m_MapScaleCombox));			
	ProjectPtr->SetProjectMapScaleMode(ScaleMode);
	ProjectPtr->SetProjectTestMapScaleMode(ScaleMode);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnSelchangeDlpLedColorCombo() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	const int DlpLedColor = JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox);	
	ProjectPtr->SetProjectDlpLedColorMode(DlpLedColor);
	AOIDataCollect.SetSystemDlpLedColor(DlpLedColor);
	AOIDataCollect.SetProjectLightSetting(ProjectPtr);
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBIn2ndBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBIn2ndProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnPCBIn3rdBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBIn3rdProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnLaneAdjustWidthBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	if ( PlcCtrlPtr->CheckLaneAdjustCanMove(LaneID) == false )
	{
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
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
	{	return;  }
	
	double NewPos = JetAPI::StrToDbl(InputBox.m_DataEdit1);
	if ( AOIDataCollect.ExecLaneAdjustWidth(LaneID, NewPos) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}
	ExecGrabImage();
	return;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ChangeDistrictID(DISTRICT_ID DistrictID)
{
	CString       str;	
	CAOIProject  *ProjectPtr = GetActiveProject();
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.MovePCBToDistrictID(LaneID, DistrictID) == false )
	{
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	SetDistrictID(DistrictID);
	AOIDataCollect.SetActiveDistrictID(DistrictID);
	if ( NULL != ProjectPtr )
	{	ProjectPtr->SetProjectActDistrictID(DistrictID, true);	}
	UpdateParamToUI();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::ClearMapBufferSet(DISTRICT_ID DistrictID)
{
	size_t   i=0;
	if ( DISTRICT_ID_A == DistrictID ) 
	{	
		m_MapStageRgn_DA = TREGION4D(); 
		m_MapResolution_DA.x = m_MapResolution_DA.y = 10.0;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	
		m_MapStageRgn_DB = TREGION4D();	
		m_MapResolution_DB.x = m_MapResolution_DB.y = 10.0;
	}	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	ClearMapBuffer(i, DistrictID);	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::ClearMapBuffer(size_t index, DISTRICT_ID DistrictID)
{
	if ( index >= FRAME_MAX_COUNT ) { return; }
	if ( DISTRICT_ID_B == DistrictID )
	{	
		m_MapW_DB[index] = 0;
		m_MapH_DB[index] = 0;
		m_MapStep_DB[index] = 0;
		m_MapSize_DB[index] = 0;
		m_BitCount_DB[index] = 8;
		JetMemory.free_func(m_MapBuffer_DB[index]);		
		m_MapBuffer_DB[index] = NULL;
	}
	if ( DISTRICT_ID_A == DistrictID )
	{	
		m_MapW_DA[index] = 0;
		m_MapH_DA[index] = 0;
		m_MapStep_DA[index] = 0;
		m_MapSize_DA[index] = 0;
		m_BitCount_DA[index] = 8;
		JetMemory.free_func(m_MapBuffer_DA[index]);		
		m_MapBuffer_DA[index] = NULL;
	}
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::SetMapBuffer(size_t index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, DISTRICT_ID DistrictID)
{
	if ( index >= FRAME_MAX_COUNT ) { return false; }
	const char fnName[] = "CNewProjectPaneRegionImage::SetMapBuffer";
	IMAGE_SIZE   MapW=ImageW;
	IMAGE_SIZE   MapH=ImageH;
	IMAGE_SIZE   MapBitCnt=BitCount;	
	IMAGE_SIZE   MapStep=ImageStep;
	IMAGE_PTR    MapBuffer=NULL;
	const size_t MapSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);

	ClearMapBuffer(index, DistrictID);
	if ( NULL == Ptr ) { return false; }
	switch ( DistrictID ) 
	{
	case DISTRICT_ID_A:
	case DISTRICT_ID_B:
		break;
	default:
		return false;		
	}

	if ( JetMemory.alloc_func(MapSize, MapBuffer, fnName, "MapBuffer") == false ) 
	{	return false;	}
	::memcpy(MapBuffer, Ptr, sizeof(IMAGE_DATA)*MapSize);
	if ( DISTRICT_ID_B == DistrictID )
	{	
		m_MapW_DB[index] = MapW;
		m_MapH_DB[index] = MapH;
		m_MapStep_DB[index] = MapStep;
		m_MapSize_DB[index] = MapSize;
		m_BitCount_DB[index] = BitCount;
		m_MapBuffer_DB[index] = MapBuffer;		
	}
	if ( DISTRICT_ID_A == DistrictID )
	{	
		m_MapW_DA[index] = MapW;
		m_MapH_DA[index] = MapH;
		m_MapStep_DA[index] = MapStep;
		m_MapSize_DA[index] = MapSize;
		m_BitCount_DA[index] = BitCount;
		m_MapBuffer_DA[index] = MapBuffer;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::ExecCombinMapBuffer()//合併兩張底圖
{
	CAOIProject  *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	CImageCombineWnd CombineWnd;
	const bool bIsRightIn = PlcCtrlPtr->GetIsPCBRightInDirection();	
	const MACHINE_CAMERA_SIDE CameraSide=AOIDataCollect.GetMachineCameraSide();	

	bool IsRightIn = bIsRightIn;
	if ( MACHINE_CAMERA_BOT == CameraSide )
	{	IsRightIn = !bIsRightIn;	}
	if ( false == IsRightIn )
	{
		CombineWnd.SetCombineMapMode(COMBINE_MAP_BY_RIGHT);
		CombineWnd.SetMapInfo_R(m_MapStageRgn_DA, m_MapResolution_DA, m_MapW_DA, m_MapH_DA, m_MapStep_DA, m_BitCount_DA, m_MapBuffer_DA, FRAME_MAX_COUNT);;
		CombineWnd.SetMapInfo_L(m_MapStageRgn_DB, m_MapResolution_DB, m_MapW_DB, m_MapH_DB, m_MapStep_DB, m_BitCount_DB, m_MapBuffer_DB, FRAME_MAX_COUNT);;
	}
	else
	{
		CombineWnd.SetCombineMapMode(COMBINE_MAP_BY_LEFT);
		CombineWnd.SetMapInfo_L(m_MapStageRgn_DA, m_MapResolution_DA, m_MapW_DA, m_MapH_DA, m_MapStep_DA, m_BitCount_DA, m_MapBuffer_DA, FRAME_MAX_COUNT);;
		CombineWnd.SetMapInfo_R(m_MapStageRgn_DB, m_MapResolution_DB, m_MapW_DB, m_MapH_DB, m_MapStep_DB, m_BitCount_DB, m_MapBuffer_DB, FRAME_MAX_COUNT);;
	}
	if ( CombineWnd.BuildMapBuffer() == false ) 
	{	return false;	}	
	if ( CombineWnd.DoModal() == IDCANCEL )
	{
		CombineWnd.ClearMapBuffer();
		return false; 
	}
	
	size_t       i=0;
	RECT         MapRect;
	RECT         MapRectR;
	RECT         MapRectL;
	TREGION4D    MapStageRgn_DA=m_MapStageRgn_DA;	
	TREGION4D    MapStageRgn_DB=m_MapStageRgn_DB;	
	const size_t MaxCount=FRAME_MAX_COUNT;
	IMAGE_SIZE MapW[MaxCount]={0};
	IMAGE_SIZE MapH[MaxCount]={0};
	IMAGE_SIZE MapBit[MaxCount]={0};
	IMAGE_SIZE MapStep[MaxCount]={0};
	IMAGE_PTR  MapPtr[MaxCount]={NULL};
	SIZE_T     MapSize[MaxCount]={0};

	IMAGE_SIZE MapWFull[MaxCount]={0};
	IMAGE_SIZE MapHFull[MaxCount]={0};
	IMAGE_SIZE MapBitFull[MaxCount]={0};
	IMAGE_SIZE MapStepFull[MaxCount]={0};
	IMAGE_PTR  MapPtrFull[MaxCount]={NULL};
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();	
	
	CombineWnd.GetMapResultRect(MapRect);
	CombineWnd.GetMapResultRect_R(MapRectR);
	CombineWnd.GetMapResultRect_L(MapRectL);
	CombineWnd.GetMapBuffer(MapWFull, MapHFull, MapStepFull, MapBitFull, MapPtrFull, MaxCount);
	
	const double MapSizeW = (MapRect.right-MapRect.left)*m_MapResolution_DA.x;
	const double MapSizeH = (MapRect.bottom-MapRect.top)*m_MapResolution_DA.y;

	OffsetRect(&MapRectR, -MapRect.left, -MapRect.top);
	OffsetRect(&MapRectL, -MapRect.left, -MapRect.top);	
	if ( IsRightIn == true ) 
	{		
		if ( true == SignX ) 				
		{	
			MapStageRgn_DA.maxX = MapStageRgn_DA.minX+MapSizeW;	
			MapStageRgn_DB.minX = MapStageRgn_DB.maxX-MapSizeW;	
			
		}
		else
		{	
			MapStageRgn_DA.minX = MapStageRgn_DA.maxX-MapSizeW;	
			MapStageRgn_DB.maxX = MapStageRgn_DB.minX+MapSizeW;	
		}
		if ( true == SignY )
		{	
			MapStageRgn_DA.minY = MapStageRgn_DA.maxY-MapSizeH;	
			MapStageRgn_DB.minY = MapStageRgn_DB.maxY-MapSizeH;	
		}
		else
		{	
			MapStageRgn_DA.maxY = MapStageRgn_DA.minY-MapSizeH;	
			MapStageRgn_DB.maxY = MapStageRgn_DB.minY-MapSizeH;	
		}
	}
	else
	{
		if ( true == SignX ) 		
		{	
			MapStageRgn_DA.minX = MapStageRgn_DA.maxX-MapSizeW;	
			MapStageRgn_DB.maxX = MapStageRgn_DB.minX+MapSizeW;	
		}
		else
		{	
			MapStageRgn_DA.maxX = MapStageRgn_DA.minX+MapSizeW;	
			MapStageRgn_DB.minX = MapStageRgn_DB.maxX-MapSizeW;	
		}
		if ( true == SignY )
		{	
			MapStageRgn_DA.minY = MapStageRgn_DA.maxY-MapSizeH;	
			MapStageRgn_DB.minY = MapStageRgn_DB.maxY-MapSizeH;	
		}
		else
		{	
			MapStageRgn_DA.maxY = MapStageRgn_DA.minY-MapSizeH;	
			MapStageRgn_DB.maxY = MapStageRgn_DB.minY-MapSizeH;	
		}
	}
	const char fnName[] = "CNewProjectPaneRegionImage::ExecCombinMapBuffer()";
	ProjectPtr->ClearProjectMapBuffer();
	for ( i=0; i<MaxCount; i++ )
	{
		if ( NULL == MapPtrFull[i] ) { continue; }
		MapW[i] = MapRect.right-MapRect.left;
		MapH[i] = MapRect.bottom-MapRect.top;
		MapBit[i] = MapBitFull[i];
		MapStep[i] = JetAPI::GetBMPImagePixelsPerLine(MapW[i], MapBit[i], 4);
		MapSize[i] = ImageAPI.CalcBufferSize(MapStep[i], MapH[i]);
		if ( JetMemory.alloc_func(MapSize[i], MapPtr[i], fnName, "MapPtr") == false ) 
		{	continue; }
		if ( ImageAPI.ExtractRoiImage3(MapWFull[i], MapHFull[i], MapStepFull[i], MapBitFull[i], MapPtrFull[i], MapRect, MapStep[i], MapPtr[i], false) == false ) 
		{
			JetMemory.free_func(MapPtr[i]); MapPtr[i] = NULL;
			continue; 
		}
		ProjectPtr->SetProjectMapPtr(i, MapW[i], MapH[i], MapStep[i], MapBit[i], MapPtr[i]);
	}
	
	if ( false == IsRightIn )
	{	
		ProjectPtr->SetProjectMapLocRect_DA(MapRectR);
		ProjectPtr->SetProjectMapLocRect_DB(MapRectL);
	}
	else
	{
		ProjectPtr->SetProjectMapLocRect_DA(MapRectL);
		ProjectPtr->SetProjectMapLocRect_DB(MapRectR);
	}
	ProjectPtr->ResetProjectMapTeachRgn(MapStageRgn_DA);
	ProjectPtr->ResetProjectMapTeachRgn_DA(MapStageRgn_DA);
	ProjectPtr->ResetProjectMapTeachRgn_DB(MapStageRgn_DB);
	CombineWnd.ClearMapBuffer();
	m_Finish = true;
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	

	//MovePCBToDistrict(DISTRICT_ID_A);
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnDistrictABtn() 
{
	// TODO: Add your control notification handler code here	
	BOOL bEnabled=TRUE;
	JetAPI::EnableCtrlWnd(this, REGION_DLP_LED_COLOR_COMBO, bEnabled);
	JetAPI::EnableCtrlWnd(this, REGION_HEIGHT_RATIO_COMBO, bEnabled);	
	JetAPI::EnableCtrlWnd(this, REGION_MAP_SCALE_COMBO, bEnabled);
	JetAPI::EnableEditWnd(this, REGION_CORNER_LENGTH_EDIT, bEnabled);
	ChangeDistrictID(DISTRICT_ID_A);	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnDistrictBBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnabled=FALSE;
	m_RegionLength_DB = m_RegionLength;
	JetAPI::EnableCtrlWnd(this, REGION_DLP_LED_COLOR_COMBO, bEnabled);
	JetAPI::EnableCtrlWnd(this, REGION_HEIGHT_RATIO_COMBO, bEnabled);
	JetAPI::EnableCtrlWnd(this, REGION_MAP_SCALE_COMBO, bEnabled);
	JetAPI::EnableEditWnd(this, REGION_CORNER_LENGTH_EDIT, bEnabled);
	ChangeDistrictID(DISTRICT_ID_B);		
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneRegionImage::OnCombineDistrictBtn() 
{
	// TODO: Add your control notification handler code here
	//
	ExecCombinMapBuffer();
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneRegionImage::UpdateProjectLandWidth()
{	
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();	
	bool LaneAdjustCanMove = PlcCtrlPtr->CheckLaneAdjustCanMove(LaneID);	
	if ( true == LaneAdjustCanMove )
	{
		const double LandWidth = PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
		ProjectPtr->SetProjectLaneWidth(LandWidth);
	}
	else
	{	
		TREGION4D  MapCadRgn_DA, MapCadRgn_DB;	
		ProjectPtr->GetProjectMapStageRgn_DA(MapCadRgn_DA);
		ProjectPtr->GetProjectMapStageRgn_DB(MapCadRgn_DB);	
		const double MapSizeY_DA=MapCadRgn_DA.GetHeight();	
		const double MapSizeY_DB=MapCadRgn_DB.GetHeight();	
		const double MapSizeY_DA_mm=JetAPI::Unit_UmtoMM(MapSizeY_DA);	
		const double MapSizeY_DB_mm=JetAPI::Unit_UmtoMM(MapSizeY_DB);
		const double LandWidth = MAX(MapSizeY_DA_mm, MapSizeY_DB_mm);
		ProjectPtr->SetProjectLaneWidth(LandWidth);
	}
	return true;
}
//-------------------------------------------------------------------------------------//