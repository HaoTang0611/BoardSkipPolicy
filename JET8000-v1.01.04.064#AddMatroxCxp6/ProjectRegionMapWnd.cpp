// ProjectRegionMapWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectRegionMapWnd.h"
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
// CProjectRegionMapWnd dialog
//-------------------------------------------------------------------------------------//
CProjectRegionMapWnd::CProjectRegionMapWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectRegionMapWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectRegionMapWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	size_t  i=0;
	m_ProjectPtr = NULL;
	m_FrameType = FRAME_COLOR;
	m_FrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;

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
	m_DistrictID = DISTRICT_ID_A;
	m_EnableMultiDistrictMode = false;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectRegionMapWnd)
	DDX_Control(pDX, PROJECT_REGION_MAP_SCALE_COMBO, m_MapScaleCombox);
	DDX_Control(pDX, PROJECT_REGION_HEIGHT_RATIO_COMBO, m_HeightRatioCombox);
	DDX_Control(pDX, PROJECT_REGION_DLP_LED_COLOR_COMBO, m_DlpLEDColorCombox);		
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_08, m_FrameCombox08);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_07, m_FrameCombox07);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_06, m_FrameCombox06);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_05, m_FrameCombox05);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_04, m_FrameCombox04);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_03, m_FrameCombox03);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_02, m_FrameCombox02);
	DDX_Control(pDX, PROJECT_REGION_FRAME_COMBO_01, m_FrameCombox01);
	DDX_Control(pDX, PROJECT_REGION_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectRegionMapWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectRegionMapWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(PROJECT_REGION_MOVE_TO_POS_BTN_A, OnRegionMoveToPosBtnA)
	ON_BN_CLICKED(PROJECT_REGION_MOVE_TO_POS_BTN_B, OnRegionMoveToPosBtnB)
	ON_BN_CLICKED(PROJECT_REGION_SET_POS_BTN_A, OnRegionSetPosBtnA)
	ON_BN_CLICKED(PROJECT_REGION_SET_POS_BTN_B, OnRegionSetPosBtnB)
	ON_BN_CLICKED(PROJECT_REGION_MOVE_TO_SIZE_CORNER_BTN, OnRegionMoveToSizeCornerBtn)
	ON_BN_CLICKED(PROJECT_REGION_MOVE_TO_STOP_BAR_BTN, OnRegionMoveToStopBarBtn)
	ON_BN_CLICKED(PROJECT_REGION_GRAB_REGION_IMAGE_BTN, OnRegionGrabRegionImageBtn)
	ON_BN_CLICKED(PROJECT_REGION_RESET_SYSTEM_BTN, OnRegionResetSystemBtn)
	ON_BN_CLICKED(PROJECT_REGION_LOAD_OFFLINE_BTN, OnRegionLoadOfflineBtn)
	ON_BN_CLICKED(PROJECT_REGION_PCB_IN_BTN, OnRegionPCBInBtn)
	ON_BN_CLICKED(PROJECT_REGION_PCB_OUT_BTN, OnRegionPCBOutBtn)
	ON_BN_CLICKED(PROJECT_REGION_PCB_BACK_BTN, OnRegionPCBBackBtn)
	ON_BN_CLICKED(PROJECT_REGION_PCB_CLAMP_ON_BTN, OnRegionPCBClampOnBtn)
	ON_BN_CLICKED(PROJECT_REGION_VIEW_PROJECT_MAP_WND, OnRegionViewProjectMapWnd)
	ON_BN_CLICKED(PROJECT_REGION_SHOW_RULER_BTN, OnRegionShowRulerBtn)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(PROJECT_REGION_FRAME_SHOW_CHK, OnRegionFrameShowChk)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(PROJECT_REGION_PCB_IN_2ND_BTN, OnRegionPCBIn2ndBtn)
	ON_BN_CLICKED(PROJECT_REGION_PCB_IN_3RD_BTN, OnRegionPCBIn3rdBtn)
	ON_BN_CLICKED(PROJECT_REGION_LANE_ADJUST_WIDTH_BTN, OnRegionLaneAdjustWidthBtn)	
	ON_BN_CLICKED(PROJECT_REGION_DISTRICT_A_BTN, OnRegionDistrictABtn)
	ON_BN_CLICKED(PROJECT_REGION_DISTRICT_B_BTN, OnRegionDistrictBBtn)
	ON_BN_CLICKED(PROJECT_REGION_COMBINE_DISTRICT_BTN, OnRegionCombineDistrictBtn)
	ON_BN_CLICKED(PROJECT_REGION_ALIGN_FD_BTN, OnRegionAlignFdBtn)
	ON_BN_CLICKED(PROJECT_REGION_FRAME_DEFAULT_BTN, OnRegionFrameDefaultBtn)
	ON_BN_CLICKED(PROJECT_REGION_FRAME_CLOSE_ALL_BTN, OnRegionFrameCloseAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectRegionMapWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectRegionMapWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);

	CString str;
	CAOIProject *ProjectPtr = GetActiveProject();
	m_ImageWnd.SetShowLBtnPos(false);		
	m_ImageWnd.SetShowWndCenterLine(true);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);	
#ifdef _DEBUG
	m_ImageWnd.SetShowCursorInfo(true);
#endif

	UpdateParamToUI();	
	SwitchMultiLanguage();
	if ( CreateImageBuffer() == false )
	{	return FALSE; }

	CString     TempFolder;
	CString     OfflineFolder;
	TSIZE2D     MapSizeDA;
	TSIZE2D     MapSizeDB;
	TREGION4D   MapStageRgnDA;
	TREGION4D   MapStageRgnDB;	
	DISTRICT_ID DistrictID = GetDistrictID();
	const bool  bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();

	m_EnableMultiDistrictMode = bMultiDistrictMode;
	m_ProjectMapWnd.SetProjectPtr(ProjectPtr, false);	
	m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer, false, true);	
	
	TempFolder = AOIDataCollect.GetAOITempDirectory();	
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(TempFolder);
	ProjectPtr->SetProjectOfflineFolder(OfflineFolder);
	JetAPI::ClearFolder(OfflineFolder);

	AOIDataDefine.BuildProjectMapScaleModeCombox(m_MapScaleCombox);
	CWnd::CheckDlgButton(PROJECT_SAVE_OFFLINE_IMAGE_CHK, SysParam.m_DefaultProjectSaveOfflineImageFiles);
	if ( NULL != ProjectPtr )
	{
		const int ScaleMode = ProjectPtr->GetProjectMapScaleMode();
		JetAPI::SetComboxCurSel(m_MapScaleCombox, ScaleMode);
		ProjectPtr->GetProjectMapLocSize_DA(MapSizeDA);		
		ProjectPtr->GetProjectMapLocSize_DB(MapSizeDB);		
		ProjectPtr->GetProjectMapLocStageRgn_DA(MapStageRgnDA);		
		ProjectPtr->GetProjectMapLocStageRgn_DB(MapStageRgnDB);
	}
	
	m_RegionPosXA = MIN(MapStageRgnDA.maxX, MapStageRgnDA.minX);
	m_RegionPosYA = MIN(MapStageRgnDA.maxY, MapStageRgnDA.minY);
	m_RegionPosZA = ProjectPtr->GetProjectFocusPos();
	m_RegionPosXB = MAX(MapStageRgnDA.maxX, MapStageRgnDA.minX);
	m_RegionPosYB = MAX(MapStageRgnDA.maxY, MapStageRgnDA.minY);
	m_RegionPosZB = ProjectPtr->GetProjectFocusPos();
	m_RegionWidth = ::fabs(MapSizeDA.cx);
	m_RegionLength = ::fabs(MapSizeDA.cy);
	m_RegionWidth = JetAPI::Unit_UmtoMM(m_RegionWidth);
	m_RegionLength = JetAPI::Unit_UmtoMM(m_RegionLength);	

	m_RegionPosXA_DB = MIN(MapStageRgnDB.maxX, MapStageRgnDB.minX);
	m_RegionPosYA_DB = MIN(MapStageRgnDB.maxY, MapStageRgnDB.minY);
	m_RegionPosZA_DB = ProjectPtr->GetProjectFocusPos();
	m_RegionPosXB_DB = MAX(MapStageRgnDB.maxX, MapStageRgnDB.minX);
	m_RegionPosYB_DB = MAX(MapStageRgnDB.maxY, MapStageRgnDB.minY);
	m_RegionPosZB_DB = ProjectPtr->GetProjectFocusPos();
	m_RegionWidth_DB = ::fabs(MapSizeDB.cx);
	m_RegionLength_DB = ::fabs(MapSizeDB.cy);
	m_RegionWidth_DB = JetAPI::Unit_UmtoMM(m_RegionWidth_DB);
	m_RegionLength_DB = JetAPI::Unit_UmtoMM(m_RegionLength_DB);	
	
	InitProjectFrameCombox();
	BuildProjectFrameCombox();
	UpdateParamToUI();	
	OnRegionFrameShowChk();
	AOIDataCollect.SetCallbackWnd(GetSafeHwnd()); 
	m_BackupJogMode = MotionCtrlPtr->GetIsJogMode();
	MotionCtrlPtr->SetIsJogMode(true);
	AOIDataCollect.SetOfflineMode(false);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	ExecGrabImage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	LockUIWnd(false);
	ReleaseImageBuffer();	
	ClearMapBufferSet(DISTRICT_ID_A);
	ClearMapBufferSet(DISTRICT_ID_B);
	MotionCtrlPtr->SetIsJogMode(m_BackupJogMode);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		m_ImageWnd.MoveWindow(&WndRect);
		//this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
		//this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
	}
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectRegionMapWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	HWND hWnd = NULL;
	const bool InspectionDrawing = AOIDataCollect.GetInspectionDrawing();	
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:
		if ( this->UpdateFovImage(wParam, lParam, true) == false )
		{	this->LockUIWnd(FALSE); }				
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecGrabImage();
		break;
	case MSG_INSPECTION_CALLBACK:
		switch ( wParam )
		{		
		case WPARAM_INSPECTION_FINISH://檢測狀態-檢測結束			
		case WPARAM_INSPECTION_ONLINE_FINISH:
			ExecFinish(lParam);
			break;			
		}		
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(FALSE);		
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:
		break;	
	case MSG_MOTION_CALLBACK:		
		break;	
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			if ( this->UpdateFovImage(PRIMARY_CAMERA_ID, lParam, false) == false )
			{	this->LockUIWnd(FALSE); }
			break;
		case WPARAM_SHOW_PROJECT_MAP_WND:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL )
			{				
				if ( FALSE == lParam )
				{	m_ProjectMapWnd.ShowWindow(SW_HIDE);	}
				else
				{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
			}
			break;
		case WPARAM_REDRAW_PROJECT_MAP:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL && m_ProjectMapWnd.IsWindowVisible() == TRUE )
			{
				if ( true == InspectionDrawing )
				{
					CAOIProject *ProjectPtr = GetActiveProject();
					if ( NULL != ProjectPtr )
					{	ProjectPtr->UpdateProjectMapShowPtr(); }
				}
				if ( NULL == lParam )
				{	m_ProjectMapWnd.RedrawWnd(TRUE);	}
				else if ( true == InspectionDrawing )
				{	m_ProjectMapWnd.RedrawWnd(TRUE);	}
			}
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			switch ( lParam )
			{
			case LPARAM_DRAW_PROJECT_MODE_NORMAL:
				m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
				break;
			case LPARAM_DRAW_PROJECT_MODE_INSPECTING:
				m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
				break;
			}
			break;		
		}
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		OnImageWndNotify(wParam, lParam);		
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CProjectRegionMapWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_REGION_MAP_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_REGION_MAP_WND;
	WndKey = _T("IDD_PROJECT_REGION_MAP_WND");
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
	WndID = PROJECT_REGION_REGION_CORNER_GROUP;
	WndKey = _T("PROJECT_REGION_REGION_CORNER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_CORNER_UNIT_LABEL;
	WndKey = _T("PROJECT_REGION_CORNER_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_CORNER_X_LABEL;
	WndKey = _T("PROJECT_REGION_CORNER_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_CORNER_Y_LABEL;
	WndKey = _T("PROJECT_REGION_CORNER_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_CORNER_Z_LABEL;
	WndKey = _T("PROJECT_REGION_CORNER_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_MOVE_TO_POS_BTN_A;
	WndKey = _T("PROJECT_REGION_MOVE_TO_POS_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_SET_POS_BTN_A;
	WndKey = _T("PROJECT_REGION_SET_POS_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_MOVE_TO_POS_BTN_B;
	WndKey = _T("PROJECT_REGION_MOVE_TO_POS_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_SET_POS_BTN_B;
	WndKey = _T("PROJECT_REGION_SET_POS_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_REGION_SIZE_GROUP;
	WndKey = _T("PROJECT_REGION_REGION_SIZE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_REGION_UNIT_LABEL;
	WndKey = _T("PROJECT_REGION_REGION_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_REGION_WIDTH_LABEL;
	WndKey = _T("PROJECT_REGION_REGION_WIDTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_REGION_LENGTH_LABEL;
	WndKey = _T("PROJECT_REGION_REGION_LENGTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_MOVE_TO_SIZE_CORNER_BTN;
	WndKey = _T("PROJECT_REGION_MOVE_TO_SIZE_CORNER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_MOVE_TO_STOP_BAR_BTN;
	WndKey = _T("PROJECT_REGION_MOVE_TO_STOP_BAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_DLP_LED_COLOR_LABEL;
	WndKey = _T("PROJECT_REGION_DLP_LED_COLOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_REGION_HEIGHT_RATIO_LABEL;
	WndKey = _T("PROJECT_REGION_HEIGHT_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_REGION_MAP_SCALE_LABEL;
	WndKey = _T("PROJECT_REGION_MAP_SCALE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_GRAB_LIVE_CHK;
	WndKey = _T("PROJECT_REGION_GRAB_LIVE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_SAVE_OFFLINE_IMAGE_CHK;
	WndKey = _T("PROJECT_SAVE_OFFLINE_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_GRAB_REGION_IMAGE_BTN;
	WndKey = _T("PROJECT_REGION_GRAB_REGION_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_RESET_SYSTEM_BTN;
	WndKey = _T("PROJECT_REGION_RESET_SYSTEM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_LOAD_OFFLINE_BTN;
	WndKey = _T("PROJECT_REGION_LOAD_OFFLINE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PROJECT_REGION_DISTRICT_A_BTN;
	WndKey = _T("PROJECT_REGION_DISTRICT_A_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_DISTRICT_B_BTN;
	WndKey = _T("PROJECT_REGION_DISTRICT_B_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_COMBINE_DISTRICT_BTN;
	WndKey = _T("PROJECT_REGION_COMBINE_DISTRICT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_REGION_ALIGN_FD_BTN;
	WndKey = _T("PROJECT_REGION_ALIGN_FD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_REGION_PCB_CTRL_GROUP;
	WndKey = _T("PROJECT_REGION_PCB_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_PCB_IN_BTN;
	WndKey = _T("PROJECT_REGION_PCB_IN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_PCB_IN_2ND_BTN;
	WndKey = _T("PROJECT_REGION_PCB_IN_2ND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_PCB_IN_3RD_BTN;
	WndKey = _T("PROJECT_REGION_PCB_IN_3RD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_PCB_OUT_BTN;
	WndKey = _T("PROJECT_REGION_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_PCB_BACK_BTN;
	WndKey = _T("PROJECT_REGION_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_PCB_CLAMP_ON_BTN;
	WndKey = _T("PROJECT_REGION_PCB_CLAMP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PROJECT_REGION_LANE_ADJUST_WIDTH_BTN;
	WndKey = _T("PROJECT_REGION_LANE_ADJUST_WIDTH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_VIEW_PROJECT_MAP_WND;
	WndKey = _T("PROJECT_REGION_VIEW_PROJECT_MAP_WND");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROJECT_REGION_SHOW_RULER_BTN;
	WndKey = _T("PROJECT_REGION_SHOW_RULER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PROJECT_REGION_FRAME_SHOW_CHK;
	WndKey = _T("PROJECT_REGION_FRAME_SHOW_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_CONFIG_GROUP;
	WndKey = _T("PROJECT_REGION_FRAME_CONFIG_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_01;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_01");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_02;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_02");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_03;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_03");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_04;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_04");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_05;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_05");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_06;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_06");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_07;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_07");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROJECT_REGION_FRAME_LABEL_08;
	WndKey = _T("PROJECT_REGION_FRAME_LABEL_08");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_FRAME_DEFAULT_BTN;
	WndKey = _T("PROJECT_REGION_FRAME_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROJECT_REGION_FRAME_CLOSE_ALL_BTN;
	WndKey = _T("PROJECT_REGION_FRAME_CLOSE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectRegionMapWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_REGION_MAP_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::SetProjectPtr(CAOIProject* Ptr)
{
	m_ProjectPtr = Ptr;
	m_ProjectFrameUniqueIDList.clear();
	if ( NULL != Ptr )
	{
		m_DistrictID = Ptr->GetProjectActDistrictID();
		Ptr->CloneProjectFrameUniqueIDList(m_ProjectFrameUniqueIDList);	
	}
	//m_ProjectMapWnd.SetProjectPtr(Ptr);//這時呼叫會有蟲
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CProjectRegionMapWnd::GetDistrictID() const
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::SetDistrictID(DISTRICT_ID val)
{
	m_DistrictID = val;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::InitProjectFrameCombox()
{	
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox01, true, true);	
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox02, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox03, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox04, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox05, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox06, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox07, true, true);
	AOIDataDefine.BuildSystemFrameParamCombox(m_FrameCombox08, true, true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::BuildProjectFrameCombox()
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }

	size_t i = 0;
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	std::vector<unsigned int> FrameUniqueIDList = m_ProjectFrameUniqueIDList;	
	const size_t FrameUniqueIDCount = FrameUniqueIDList.size();

	CloaseAllProjectFrameCombox();
	for ( i=0; i<FrameUniqueIDCount; i++ )
	{
		if ( i >= FRAME_MAX_COUNT ) { continue; }
		FrameUniqueID = FrameUniqueIDList[i];
		switch ( i )
		{
		case 0:	JetAPI::SetComboxCurSel(m_FrameCombox01, FrameUniqueID);	break;
		case 1:	JetAPI::SetComboxCurSel(m_FrameCombox02, FrameUniqueID);	break;
		case 2:	JetAPI::SetComboxCurSel(m_FrameCombox03, FrameUniqueID);	break;
		case 3:	JetAPI::SetComboxCurSel(m_FrameCombox04, FrameUniqueID);	break;
		case 4:	JetAPI::SetComboxCurSel(m_FrameCombox05, FrameUniqueID);	break;
		case 5:	JetAPI::SetComboxCurSel(m_FrameCombox06, FrameUniqueID);	break;
		case 6:	JetAPI::SetComboxCurSel(m_FrameCombox07, FrameUniqueID);	break;
		case 7:	JetAPI::SetComboxCurSel(m_FrameCombox08, FrameUniqueID);	break;
		}
	}	
	
	AOIDataDefine.BuidlProjectSpaceToGrayRatioCombox(m_HeightRatioCombox);
	const int SpaceRatioMode = ProjectPtr->GetProjectSpaceToGrayRatioMode();
	JetAPI::SetComboxCurSel(m_HeightRatioCombox, SpaceRatioMode);

	AOIDataDefine.BuidlProjectDlpLedColorCombox(m_DlpLEDColorCombox);
	const int DlpLedColor = ProjectPtr->GetProjectDlpLedColorMode();
	JetAPI::SetComboxCurSel(m_DlpLEDColorCombox, DlpLedColor);	

	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( true == OfflineMode )
	{
		m_HeightRatioCombox.EnableWindow(FALSE);
		m_DlpLEDColorCombox.EnableWindow(FALSE);
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::CloaseAllProjectFrameCombox()
{
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	JetAPI::SetComboxCurSel(m_FrameCombox01, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox02, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox03, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox04, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox05, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox06, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox07, FrameUniqueID);
	JetAPI::SetComboxCurSel(m_FrameCombox08, FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::BuildFrameUniqueIDList(std::vector<unsigned int> &FrameUniqueIDList)
{
	bool b3DMode = false;
	FrameUniqueIDList.clear();

	b3DMode = false;
	if ( AddFrameUniqueIDList(m_FrameCombox01, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox02, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox03, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox04, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox05, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox06, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox07, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox08, FrameUniqueIDList, b3DMode) == false ) { return false; }

	b3DMode = true;
	if ( AddFrameUniqueIDList(m_FrameCombox01, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox02, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox03, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox04, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox05, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox06, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox07, FrameUniqueIDList, b3DMode) == false ) { return false; }
	if ( AddFrameUniqueIDList(m_FrameCombox08, FrameUniqueIDList, b3DMode) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::AddFrameUniqueIDList(CComboBox &Combox, std::vector<unsigned int> &FrameUniqueIDList, bool b3DMode)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return false; }

	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	FrameUniqueID = (unsigned int)JetAPI::GetComboxCurSelData(Combox);
	if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID )
	{	return true; }

	if ( true == b3DMode )
	{
		if ( FRAME_UNIQUE_ID_DLP != FrameUniqueID ) { return true; }		
	}
	else
	{		
		if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID ) { return true; }
	}
	FrameUniqueIDList.push_back(FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::CreateImageBuffer()
{
	ReleaseImageBuffer();

	const char fnName[] = "CProjectRegionMapWnd::CreateImageBuffer";
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	IMAGE_PTR  BufferPtr = NULL;
	IMAGE_SIZE BufferW = CameraCtrl.GetCameraImageSizeW(CameraID);
	IMAGE_SIZE BufferH = CameraCtrl.GetCameraImageSizeH(CameraID);	
	IMAGE_SIZE BitCount = 24;
	IMAGE_SIZE BufferStep = JetAPI::GetBMPImagePixelsPerLine(BufferW, BitCount, 4);
	size_t     BufferSize = ImageAPI.CalcBufferSize(BufferStep, BufferH);
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr-1") == false )
	{
		ReleaseImageBuffer();
		return false;
	}
	m_ImageW = BufferW;
	m_ImageH = BufferH;
	m_BitCount = BitCount;
	m_ImageStep = BufferStep;
	m_ImageBuffer = BufferPtr;
	m_ImageBufferSize = BufferSize;
	::memset(BufferPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr-2") == false )
	{
		ReleaseImageBuffer();
		return false;
	}
	m_ShowImageW = BufferW;
	m_ShowImageH = BufferH;
	m_ShowBitCount = BitCount;
	m_ShowImageStep = BufferStep;	
	m_ShowBuffer = BufferPtr;
	m_ShowBufferSize = BufferSize;	
	::memset(BufferPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ReleaseImageBuffer()
{
	if ( NULL != m_ImageBuffer )
	{	JetMemory.free_func(m_ImageBuffer);	}
	m_ImageBufferSize = 0;

	if ( NULL != m_ShowBuffer )
	{	JetMemory.free_func(m_ShowBuffer);	}
	m_ShowBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::UpdateParamToUI()//將參數更新至介面
{
	CString str;
	if ( NULL != m_ProjectPtr )
	{
		const int ScaleMode = m_ProjectPtr->GetProjectMapScaleMode();
		JetAPI::SetComboxCurSel(m_MapScaleCombox, ScaleMode);		
	}

	DISTRICT_ID  DistrictID = GetDistrictID();	
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		str.Format(_T("%.0f"), m_RegionPosXA_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_X_EDIT_A, str);
		str.Format(_T("%.0f"), m_RegionPosYA_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Y_EDIT_A, str);
		str.Format(_T("%.0f"), m_RegionPosZA_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Z_EDIT_A, str);

		str.Format(_T("%.0f"), m_RegionPosXB_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_X_EDIT_B, str);
		str.Format(_T("%.0f"), m_RegionPosYB_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Y_EDIT_B, str);
		str.Format(_T("%.0f"), m_RegionPosZB_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Z_EDIT_B, str);

		str.Format(_T("%.3f"), m_RegionWidth_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_WIDTH_EDIT, str);
		str.Format(_T("%.3f"), m_RegionLength_DB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_LENGTH_EDIT, str);
	}
	else
	{
		str.Format(_T("%.0f"), m_RegionPosXA);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_X_EDIT_A, str);
		str.Format(_T("%.0f"), m_RegionPosYA);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Y_EDIT_A, str);
		str.Format(_T("%.0f"), m_RegionPosZA);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Z_EDIT_A, str);

		str.Format(_T("%.0f"), m_RegionPosXB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_X_EDIT_B, str);
		str.Format(_T("%.0f"), m_RegionPosYB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Y_EDIT_B, str);
		str.Format(_T("%.0f"), m_RegionPosZB);
		this->SetDlgItemText(PROJECT_REGION_CORNER_POS_Z_EDIT_B, str);

		str.Format(_T("%.3f"), m_RegionWidth);
		this->SetDlgItemText(PROJECT_REGION_CORNER_WIDTH_EDIT, str);
		str.Format(_T("%.3f"), m_RegionLength);
		this->SetDlgItemText(PROJECT_REGION_CORNER_LENGTH_EDIT, str);
	}
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(PROJECT_REGION_DISTRICT_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::UpdateUIToParam()//將介面更新至參數
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionMoveToPosBtnA() 
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
void CProjectRegionMapWnd::OnRegionMoveToPosBtnB() 
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
	if ( MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionSetPosBtnA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	DISTRICT_ID  DistrictID = GetDistrictID();		
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		this->m_RegionPosXA_DB = PosX;
		this->m_RegionPosYA_DB = PosY;	
		this->m_RegionPosZA_DB = PosZ;	
		this->m_RegionWidth_DB = ::fabs(m_RegionPosXA_DB-m_RegionPosXB_DB);
		this->m_RegionLength_DB = ::fabs(m_RegionPosYA_DB-m_RegionPosYB_DB);
		this->m_RegionWidth_DB = JetAPI::Unit_UmtoMM(m_RegionWidth);
		this->m_RegionLength_DB = JetAPI::Unit_UmtoMM(m_RegionLength);
	}
	else
	{
		this->m_RegionPosXA = PosX;
		this->m_RegionPosYA = PosY;	
		this->m_RegionPosZA = PosZ;	
		this->m_RegionWidth = ::fabs(m_RegionPosXA-m_RegionPosXB);
		this->m_RegionLength = ::fabs(m_RegionPosYA-m_RegionPosYB);
		this->m_RegionWidth = JetAPI::Unit_UmtoMM(m_RegionWidth);
		this->m_RegionLength = JetAPI::Unit_UmtoMM(m_RegionLength);
	}

	this->UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionSetPosBtnB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	DISTRICT_ID  DistrictID = GetDistrictID();		
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		this->m_RegionPosXB_DB = PosX;
		this->m_RegionPosYB_DB = PosY;	
		this->m_RegionPosZB_DB = PosZ;	
		this->m_RegionWidth_DB = ::fabs(m_RegionPosXA_DB-m_RegionPosXB_DB);
		this->m_RegionLength_DB = ::fabs(m_RegionPosYA_DB-m_RegionPosYB_DB);
		this->m_RegionWidth_DB = JetAPI::Unit_UmtoMM(m_RegionWidth_DB);
		this->m_RegionLength_DB = JetAPI::Unit_UmtoMM(m_RegionLength_DB);	
	}
	else
	{
		this->m_RegionPosXB = PosX;
		this->m_RegionPosYB = PosY;	
		this->m_RegionPosZB = PosZ;	
		this->m_RegionWidth = ::fabs(m_RegionPosXA-m_RegionPosXB);
		this->m_RegionLength = ::fabs(m_RegionPosYA-m_RegionPosYB);
		this->m_RegionWidth = JetAPI::Unit_UmtoMM(m_RegionWidth);
		this->m_RegionLength = JetAPI::Unit_UmtoMM(m_RegionLength);	
	}
	this->UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionMoveToSizeCornerBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PCBW = 0;
	double PCBH = 0;	
	DISTRICT_ID  DistrictID = GetDistrictID();	
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		this->GetDlgItemText(PROJECT_REGION_CORNER_WIDTH_EDIT, str);
		this->m_RegionWidth_DB = ::_tcstod(str, NULL);
		this->GetDlgItemText(PROJECT_REGION_CORNER_LENGTH_EDIT, str);
		this->m_RegionLength_DB = ::_tcstod(str, NULL);	
		PCBW = JetAPI::Unit_MMtoUM(m_RegionWidth_DB);
		PCBH = JetAPI::Unit_MMtoUM(m_RegionLength_DB);	
	}
	else
	{
		this->GetDlgItemText(PROJECT_REGION_CORNER_WIDTH_EDIT, str);
		this->m_RegionWidth = ::_tcstod(str, NULL);
		this->GetDlgItemText(PROJECT_REGION_CORNER_LENGTH_EDIT, str);
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
	this->UpdateParamToUI();	
	if ( MotionCtrlPtr->XYMoveTo(PCBPosX, PCBPosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	ExecGrabImage();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionMoveToStopBarBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID      LaneID = AOIDataCollect.GetActiveLaneID();
	DISTRICT_ID  DistrictID = GetDistrictID();
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
void CProjectRegionMapWnd::OnRegionGrabRegionImageBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	CString  str;
	TPOINT3D StagePosA;
	TPOINT3D StagePosB;
	DISTRICT_ID DistrictID = GetDistrictID();
	LANE_ID     LaneID = AOIDataCollect.GetActiveLaneID();			
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	BOOL bSaveOffline=CWnd::IsDlgButtonChecked(PROJECT_SAVE_OFFLINE_IMAGE_CHK);

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
	const bool bPanelFdOnlye = true;
	CString ProjectName = ProjectPtr->GetProjectFileName();		
	size_t  FdCount = ProjectPtr->GetProjectFdCount(DistrictID, bPanelFdOnlye);
	const bool  IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial(LaneID);
	if ( true==IsNeedGrabFiducial && FdCount>0 ) 
	{
		str = _T("Align board first, please");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}

	JetAPI::ExtractMainFileName(ProjectName, ProjectFolder);
	OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(ProjectFolder);	
	OfflineFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Offline"));
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
	if ( DISTRICT_ID_A == DistrictID )
	{
		m_Finish_DA = false;
		JetAPI::ClearFolder(OfflineFolder); 
	}
	else
	{	m_Finish_DB = false;	}
	ClearMapBufferSet(DistrictID);

	std::vector<unsigned int> FrameUniqueIDList;
	unsigned int FrameUniqueID=FRAME_UNIQUE_ID_NULL;
	BuildFrameUniqueIDList(FrameUniqueIDList);	
	const size_t FrameUniqueIDCount = FrameUniqueIDList.size();
	if ( 0 == FrameUniqueIDCount )
	{
		str.Format(_T("Error, there is no any one frame image in the project!"));
		JetAPI::ShowMessageBox(str);
		return ;
	}
	if ( ProjectPtr->BuildProjectImageConfig(FrameUniqueIDList) == false )
	{
		str = ProjectPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return ;
	}		
	
	const TASK_MODE TaskMode = TASK_PROJECT_MAP;
	const int ScaleMode = (int)(JetAPI::GetComboxCurSelData(m_MapScaleCombox));		
	const int DlpLedColor = (int)(JetAPI::GetComboxCurSelData(m_DlpLEDColorCombox));
	const int SpaceRatioMode = (int)(JetAPI::GetComboxCurSelData(m_HeightRatioCombox));			

	ProjectPtr->SetProjectActTaskMode(TaskMode);
	ProjectPtr->SetProjectMapScaleMode(ScaleMode);
	ProjectPtr->SetProjectDlpLedColorMode(DlpLedColor);	
	ProjectPtr->SetProjectSpaceToGrayRatioMode(SpaceRatioMode);	
	if ( ProjectPtr->CalcProjectMapBuffer(StagePosA, StagePosB) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetProjectErrorString());
		return;
	}	

	this->m_LiveGrab = FALSE;
	AOIDataCollect.SetTaskMode(TaskMode);
	AOIDataCollect.SetOfflineMode(false);
	AOIDataCollect.SetIsNeedResetLightCtrlDLP(true);
	AOIDataCollect.SetProjectLightSetting(ProjectPtr);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	

	m_ProjectMapWnd.SetProjectPtr(ProjectPtr, true);	
	m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	
	m_ProjectMapWnd.ShowWindow(SW_SHOW);
	if ( AOIDataCollect.StartThreadSequenceThread(true) == false )
	{	
		m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->m_LiveGrab = this->IsDlgButtonChecked(PROJECT_REGION_GRAB_LIVE_CHK);
		return ;
	}
	this->LockUIWnd(true);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionResetSystemBtn() 
{
	// TODO: Add your control notification handler code here
	AOIDataCollect.IdleAllThread(false);
	AOIDataCollect.SetTaskMode(TASK_NONE);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
	CameraCtrl.ResetBatchGrabbing();
	this->LockUIWnd(false);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionLoadOfflineBtn() 
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
	this->m_LiveGrab = this->IsDlgButtonChecked(PROJECT_REGION_GRAB_LIVE_CHK);		
	m_ProjectMapWnd.SetProjectPtr(ProjectPtr, true);	
	m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionPCBInBtn() 
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
void CProjectRegionMapWnd::OnRegionPCBOutBtn() 
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
void CProjectRegionMapWnd::OnRegionPCBBackBtn() 
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
void CProjectRegionMapWnd::OnRegionPCBClampOnBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOnProc(LaneID) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	ExecGrabImage();
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::RedrawWnd()
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
BOOL CProjectRegionMapWnd::CreateBKDC(bool bResetView)//建立背景DC	
{
	if ( NULL==m_ShowBuffer ) { return FALSE; }	
	TPOINT2D   ImageRes;
	TPOINT2D   StagePos;
	TREGION4D  StageRgn;
	IMAGE_SIZE ImageW=0, ImageH=0;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	const double FOVWum = AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = AOIDataCollect.GetFovSizeRealH();	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();

	MotionCtrlPtr->GetCurrentPos(StagePos.x, StagePos.y, OfflineMode);	
	StageRgn.minX = StagePos.x-(FOVWum*0.5);
	StageRgn.maxX = StagePos.x+(FOVWum*0.5);
	StageRgn.minY = StagePos.y-(FOVHum*0.5);
	StageRgn.maxY = StagePos.y+(FOVHum*0.5);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);
	
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer, false, bResetView);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecGrabImage()
{	
	CString str;
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == this->m_ProjectPtr ) { return false; }		
#ifndef OFFLINE_VERSION
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
		return false;
	}	
#else
	if ( AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		this->LockUIWnd(false);
		return false;
	}	
#endif//LIGHT_CTRL_DISABLE	
	
#else
	PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::LockUIWnd(bool bLock)
{
	AOIDataCollect.SetIsLockUIWnd(bLock);	
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	
	BOOL  bEnable2 = TRUE;	
	DISTRICT_ID  DistrictID = GetDistrictID();

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }
	bEnable2 = bEnable;
#ifdef OFFLINE_VERSION
	bEnable = FALSE;

	CtrlID = PROJECT_REGION_GRAB_LIVE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_RESET_SYSTEM_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_SAVE_OFFLINE_IMAGE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PROJECT_REGION_PCB_IN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_REGION_PCB_OUT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_REGION_PCB_BACK_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_REGION_PCB_CLAMP_ON_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_PCB_IN_2ND_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_REGION_PCB_IN_3RD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = PROJECT_REGION_LANE_ADJUST_WIDTH_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
#endif//OFFLINE_VERSION
	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_PCB_OUT_BTN, FALSE);	}

	CtrlID = PROJECT_REGION_MOVE_TO_POS_BTN_A;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_MOVE_TO_POS_BTN_B;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_SET_POS_BTN_A;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_SET_POS_BTN_B;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PROJECT_REGION_MOVE_TO_SIZE_CORNER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_REGION_MOVE_TO_STOP_BAR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PROJECT_REGION_GRAB_LIVE_CHK;
//	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PROJECT_SAVE_OFFLINE_IMAGE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PROJECT_REGION_MAP_SCALE_COMBO;
	if ( DISTRICT_ID_B == DistrictID )
	{	JetAPI::EnableCtrlWnd(this, CtrlID, FALSE); }
	else
	{	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable); }

	CtrlID = PROJECT_REGION_GRAB_REGION_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PROJECT_REGION_ALIGN_FD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	//Edit Control
	CtrlID = PROJECT_REGION_CORNER_WIDTH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = PROJECT_REGION_CORNER_LENGTH_EDIT;
	if ( DISTRICT_ID_B == DistrictID )
	{	JetAPI::EnableEditWnd(this, CtrlID, FALSE); }
	else
	{	JetAPI::EnableEditWnd(this, CtrlID, bEnable); }

	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_01, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_02, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_03, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_04, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_05, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_06, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_07, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_COMBO_08, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_DEFAULT_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_FRAME_CLOSE_ALL_BTN, bEnable);

#ifndef OFFLINE_VERSION
	bEnable = FALSE;
#else
	bEnable = TRUE;
#endif//OFFLINE_VERSION
	CtrlID = PROJECT_REGION_LOAD_OFFLINE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	BOOL bEnabled=FALSE;
	if ( false == m_EnableMultiDistrictMode  )
	{
		JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DISTRICT_A_BTN, bEnabled);
		JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DISTRICT_B_BTN, bEnabled);
		JetAPI::EnableCtrlWnd(this, PROJECT_REGION_COMBINE_DISTRICT_BTN, bEnabled);
	}
	else
	{
		if ( true == m_Finish_DA ) 
		{	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DISTRICT_B_BTN, TRUE);	}
		else
		{	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DISTRICT_B_BTN, FALSE); }

		if ( false==m_Finish_DA || false==m_Finish_DB ) 
		{	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_COMBINE_DISTRICT_BTN, FALSE);	}
		else
		{	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_COMBINE_DISTRICT_BTN, TRUE); }

		JetAPI::EnableCtrlWnd(this, PROJECT_REGION_SET_POS_BTN_A, bEnabled);
		JetAPI::EnableCtrlWnd(this, PROJECT_REGION_SET_POS_BTN_B, bEnabled);
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecFinish(LPARAM lParam)
{
	bool     bIsOK=true;
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode();
	
	switch ( TaskMode )
	{
	case TASK_PROJECT_MAP:
		bIsOK = ExecFinishProjectMap();
		break;
	case TASK_ALIGN_PROJECT:
		bIsOK = ExecAlignFd_Finish();
		break;
	default:
		LockUIWnd(false);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecFinishProjectMap()
{
	CString      str;	
	CString      OfflineFolder;
	TPOINT3D     StagePos;			
	CAOIPanel   *PanelPtr = NULL;
	CAOIProject *ProjectPtr = NULL;
	DISTRICT_ID  DistrictID = GetDistrictID();
	ProjectPtr = GetActiveProject();
	m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);	
	if ( NULL != ProjectPtr )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
		if ( NULL != PanelPtr )
		{
			CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad						
			ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);			
			ProjectPtr->MapProjectMapLocStageToCadPos(*STCPtr);
		}
		OfflineFolder = ProjectPtr->GetProjectProgramOfflineFolder();
		str = AOIDataDefine.GetProjectOfflineMapName(OfflineFolder, DistrictID);
		ProjectPtr->SaveProjectMapFile(str);
		str = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
		ProjectPtr->SaveProjectOfflineFdFile(str);
		m_ProjectMapWnd.SetProjectPtr(ProjectPtr, true);
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
	this->LockUIWnd(false);
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
	m_LiveGrab = this->IsDlgButtonChecked(PROJECT_REGION_GRAB_LIVE_CHK);
	ExecGrabImage();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecLoadOfflineProgram(LPCTSTR filename)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }

	DWORD    res=0;
	CString  str;
	CString  MapFile;
	CString  MapFolder;
	CString  OffelineFile;	
	DISTRICT_ID DistrictID = GetDistrictID();
	TProjectParameter &Param = ProjectPtr->GetProjectParameter();

	MapFile = filename;
	JetAPI::ExtractFolder(MapFile, MapFolder);	
	OffelineFile = AOIDataDefine.GetProjectOfflineFileName(MapFolder, DistrictID);
	if ( ProjectPtr->LoadProjectMapFile(MapFile) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}
	if ( ProjectPtr->LoadProjectProgramOfflineFile(OffelineFile) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}

	TREGION4D StageRgn;	
	ProjectPtr->GetProjectMapTeachRgn(StageRgn);
	ProjectPtr->ResetProjectMapTeachRgn(StageRgn);
	AOIDataCollect.SetOfflineMode(true);
	AOIDataCollect.SetOfflineFileName(OffelineFile);

	const double StageCpX = StageRgn.GetCpX();
	const double StageCpY = StageRgn.GetCpY();	
	const double Width = StageRgn.GetWidth();
	const double Height = StageRgn.GetHeight();
	const double FocusOffsetZ = ProjectPtr->GetProjectProgramOfflineFocusOffsetZ();
	ProjectPtr->SetProjectFocusPosOffset(FocusOffsetZ);	

	Param.m_TestSizeWidth = JetAPI::Unit_UmtoMM(Width);
	Param.m_TestSizeHeight = JetAPI::Unit_UmtoMM(Height);

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
	
	if ( AOIDataCollect.GetPreLoadProjectProgramImage() == false ) { return true; }

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

	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StageCpX, StageCpY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	ExecGrabImage();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
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
		if ( LoadProgramOfflineImage(wParam, lParam, bGetImage) == FALSE )
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
	m_ResetView = true;

	if ( m_ImageWnd.IsWindowVisible() == TRUE )
	{	m_ImageWnd.RedrawWnd(TRUE);	}

	if ( TRUE == this->m_LiveGrab )
	{	ExecGrabImage(); }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像	
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
	{	return true;	}

	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	LockUIWnd(false);
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
bool CProjectRegionMapWnd::RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像	
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
bool CProjectRegionMapWnd::LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage)
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
void CProjectRegionMapWnd::OnRegionViewProjectMapWnd() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL )
	{	return; }
	BOOL bCheck = CWnd::IsDlgButtonChecked(PROJECT_REGION_VIEW_PROJECT_MAP_WND);
	if ( TRUE == bCheck ) 
	{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
	else
	{	m_ProjectMapWnd.ShowWindow(SW_HIDE);	}
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::CheckFinish()
{
	CString str;
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
			str.Format(_T("Error, Grab Project Map First DB "));
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
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnOK() 
{
	// TODO: Add extra validation here
	if ( CheckFinish() == false )
	{	return ; }
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	if ( GetLockUIWnd() == true )	
	{	return ; }
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CProjectRegionMapWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionShowRulerBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	CRulerWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here	
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{	
	if ( WPARAM_CONTEXT_MENU == wParam )
	{
		const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();
		if ( true == SwitchFrameMode )
		{	SwitchFrameImage(); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::SwitchFrameImage()
{
	return;

	CWnd::PostMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, 0);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionFrameShowChk() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	UINT  CtrlID = 0;
	BOOL  bShow = TRUE;
	BOOL  bChecked = CWnd::IsDlgButtonChecked(PROJECT_REGION_FRAME_SHOW_CHK);
	if ( TRUE == bChecked ) { bShow = TRUE; }
	else { bShow = FALSE; }
	
	CtrlID = PROJECT_REGION_FRAME_CONFIG_GROUP;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = PROJECT_REGION_FRAME_LABEL_01;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_01;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_02;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_02;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_03;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_03;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_04;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_04;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_05;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_05;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_06;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_06;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_07;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_07;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_LABEL_08;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_COMBO_08;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);

	CtrlID = PROJECT_REGION_FRAME_DEFAULT_BTN;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
	CtrlID = PROJECT_REGION_FRAME_CLOSE_ALL_BTN;
	JetAPI::ShowCtrlWnd(this, CtrlID, bShow);
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  768;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionPCBIn2ndBtn() 
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
void CProjectRegionMapWnd::OnRegionPCBIn3rdBtn() 
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
void CProjectRegionMapWnd::OnRegionLaneAdjustWidthBtn()
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
bool CProjectRegionMapWnd::ChangeDistrictID(DISTRICT_ID DistrictID)
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
void CProjectRegionMapWnd::OnRegionDistrictABtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnabled=TRUE;
	BOOL bEnabled3D=TRUE;	
	const bool bDisable3D = AOIDataCollect.GetDisable3D();	
	if ( true == bDisable3D )
	{	bEnabled3D=FALSE;	}
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DLP_LED_COLOR_COMBO, bEnabled3D);
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_HEIGHT_RATIO_COMBO, bEnabled3D);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_MAP_SCALE_COMBO, bEnabled);
	JetAPI::EnableEditWnd(this, PROJECT_REGION_CORNER_LENGTH_EDIT, bEnabled);
	ChangeDistrictID(DISTRICT_ID_A);	
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionDistrictBBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnabled=FALSE;
	m_RegionLength_DB = m_RegionLength;
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_DLP_LED_COLOR_COMBO, bEnabled);
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_HEIGHT_RATIO_COMBO, bEnabled);	
	JetAPI::EnableCtrlWnd(this, PROJECT_REGION_MAP_SCALE_COMBO, bEnabled);
	JetAPI::EnableEditWnd(this, PROJECT_REGION_CORNER_LENGTH_EDIT, bEnabled);
	ChangeDistrictID(DISTRICT_ID_B);	
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::ClearMapBufferSet(DISTRICT_ID DistrictID)
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
void CProjectRegionMapWnd::ClearMapBuffer(size_t index, DISTRICT_ID DistrictID)
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
bool CProjectRegionMapWnd::SetMapBuffer(size_t index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, DISTRICT_ID DistrictID)
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
bool CProjectRegionMapWnd::ExecCombinMapBuffer()//合併兩張底圖
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

	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
	if ( NULL != PanelPtr )
	{	//整板的座標轉換-Stage to Cad
		CMapCoordinate *STCPtrDA = PanelPtr->GetPanelMapSTCPtr(DISTRICT_ID_A);
		CMapCoordinate *STCPtrDB = PanelPtr->GetPanelMapSTCPtr(DISTRICT_ID_B);
		ProjectPtr->MapProjectMapStageToCadPos(DISTRICT_ID_A, *STCPtrDA);
		ProjectPtr->MapProjectMapStageToCadPos(DISTRICT_ID_B, *STCPtrDB);
	}

	CombineWnd.ClearMapBuffer();
	m_Finish = true;
	//MovePCBToDistrict(DISTRICT_ID_A);
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionCombineDistrictBtn() 
{
	// TODO: Add your control notification handler code here
	ExecCombinMapBuffer();
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecAlignFd()//對齊定位點
{
	CString str;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetIsNeedGrabFiducial(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetTaskMode(TASK_ALIGN_PROJECT);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	LockUIWnd(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectRegionMapWnd::ExecAlignFd_Finish()
{
	LockUIWnd(false);
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }		
	
	CString      OfflineFolder;
	CString      OfflineFdName;
	DISTRICT_ID  DistrictID = GetDistrictID();

	OfflineFolder = ProjectPtr->GetProjectOfflineFolder();
	OfflineFdName = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionAlignFdBtn() 
{
	// TODO: Add your control notification handler code here
	ExecAlignFd();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionFrameDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	BuildProjectFrameCombox();
}
//-------------------------------------------------------------------------------------//
void CProjectRegionMapWnd::OnRegionFrameCloseAllBtn() 
{
	// TODO: Add your control notification handler code here
	CloaseAllProjectFrameCombox();
}
//-------------------------------------------------------------------------------------//