// CaliPaneStage.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CaliPaneStage.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneStage dialog
//-------------------------------------------------------------------------------------//
CCaliPaneStage::CCaliPaneStage(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliPaneStage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaliPaneStage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LaneID = LANE_ID_A;
	m_CameraID = PRIMARY_CAMERA_ID;	
	m_CaliStageMode = CALIBRATION_STAGE_STOP;

	m_BKColor = 0x000000;
	m_ImageZoom = 1.0;
	::memset(&m_ImageWndRect, 0x00, sizeof(m_ImageWndRect));	

	m_DotBKColor = 0xB0E4EF;	
	m_DotBKColor = 0xE7BFC8;
	m_DotBKColor = 0xC3C3C3;
	m_DotImageZoom = 1.0;
	::memset(&m_DotImageWndRect, 0x00, sizeof(m_DotImageWndRect));	

	m_DotImagePtr = NULL;
	m_DotImageW = 0;
	m_DotImageH = 0;
	m_DotImageStep = 0;
	m_DotImageBitCount = 8;	

	m_Threshold = 36;
	m_ImageFontHeight = 16;
	m_DotWidth = 50;
	m_DotHeight = 50;	
	m_DotDateTime = CTime::GetCurrentTime();//校正時間
	m_DotRoiSize.cx = m_DotRoiSize.cy = 0;

	m_ShowCaliStep = false;
	m_ShowRoiRect = false;
	m_ShowCrossLine = false;
	m_ShowCursorLine = false;
	m_ShowDotMatched = false;

	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_BitCount = 8;

	this->m_ShowBuffer = NULL;
	this->m_ShowBuffer1 = NULL;	
	this->m_MaskBuffer = NULL;
	this->m_PhaseBuffer = NULL;
	this->m_PhaseBuffer1 = NULL;
	this->m_PhaseBuffer2 = NULL;
	this->m_ImageBuffer = NULL;
	this->m_ImageBuffer1 = NULL;
	this->m_ImageBuffer2 = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_SpaceBuffer1 = NULL;
	this->m_ShowBufferSize = 0;
	this->m_ImageBufferSize = 0;
	this->m_SpaceBufferSize = 0;
	
	m_ErrorOffsetX = 30;
	m_ErrorOffsetY = 30;
	m_AlignTolerance_DOT = 5;
	m_AlignTolerance_HOR = 10;
	m_AlignTolerance_Ver = 100;
	m_DotAlignMode = DOT_ALIGN_MATCH;

	m_LockUIWnd = false;
	m_LockUIAlign=true;//鎖住對齊
	m_LockUIAlignHor=true;//鎖住水平調整
	m_LockUIAlignVer=true;//鎖住垂直確認
	m_LockUIAlignCorner=true;//鎖住四角落確認
	m_LockUIAlignDotNode=true;//鎖住玻璃點校正
	m_DotNodeCanBeXYTable = false;	

	POINT pt={0,0};
	m_MovingPos=pt;
	m_RBtnUpPos=pt;
	m_RBtnDownPos=pt;
	m_LBtnUpPos=pt;
	m_LBtnDownPos=pt;

	m_AlignedCount=2;
	m_AlignedMapUsed=false;
	m_AlignedMap.Identity();

	ResetDotSearch();
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaliPaneStage)	
	DDX_Control(pDX, CALISTAGE_LANE_ID_COMBO, m_LaneIDCombox);	
	DDX_Control(pDX, CALISTAGE_ALIGN_MODE_COMBO, m_AlignModeCombox);
	DDX_Control(pDX, CALISTAGE_ALIGN_COUNT_COMBO, m_AlignCountCombox);
	DDX_Control(pDX, CALISTAGE_DOT_LIST_WND, m_DotListCtrl);
	DDX_Control(pDX, CALISTAGE_SLICE_COMBO, m_SliceCombox);
	DDX_Control(pDX, CALISTAGE_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, CALISTAGE_DOT_IMAGE_WND, m_DotImageWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaliPaneStage, CDialog)
	//{{AFX_MSG_MAP(CCaliPaneStage)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(CALISTAGE_DOT_BUILD_BTN, OnDotBuildBtn)
	ON_BN_CLICKED(CALISTAGE_GRAB_BTN, OnGrabBtn)
	ON_CBN_SELCHANGE(CALISTAGE_SLICE_COMBO, OnSelchangeSliceCombo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(CALISTAGE_DOT_MATCH_BTN, OnDotMatchBtn)
	ON_BN_CLICKED(CALISTAGE_SHOW_CENTER_LINE_CHK, OnShowCenterLineChk)
	ON_BN_CLICKED(CALISTAGE_SHOW_CURSOR_LINE_CHK, OnShowCursorLineChk)
	ON_BN_CLICKED(CALISTAGE_MOTION_WND_BTN, OnMotionWndBtn)
	ON_BN_CLICKED(CALISTAGE_DOT_ALIGN_BTN, OnDotAlignBtn)
	ON_BN_CLICKED(CALISTAGE_ALIGN_X12_BTN, OnAlignX12Btn)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_X1_BTN, OnMoveToX1Btn)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_X2_BTN, OnMoveToX2Btn)
	ON_BN_CLICKED(CALISTAGE_VERIFY_X_BTN, OnVerifyXBtn)
	ON_BN_CLICKED(CALISTAGE_ALIGN_Y12_BTN, OnAlignY12Btn)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_Y1_BTN, OnMoveToY1Btn)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_Y2_BTN, OnMoveToY2Btn)
	ON_BN_CLICKED(CALISTAGE_VERIFY_Y_BTN, OnVerifyYBtn)
	ON_BN_CLICKED(CALISTAGE_ALIGN_CORNER_BTN, OnAlignCornerBtn)
	ON_BN_CLICKED(CALISTAGE_CALIBRATE_DOT_BTN, OnCalibrateDotBtn)
	ON_BN_CLICKED(CALISTAGE_BUILD_XYDOT_TABLE_BTN, OnBuildXYDotTableBtn)
	ON_BN_CLICKED(CALISTAGE_ENABLE_XYCALI_CHK, OnEnableXYCaliChk)
	ON_NOTIFY(HDN_ITEMCHANGED, CALISTAGE_DOT_LIST_WND, OnItemchangedDotListWnd)
	ON_NOTIFY(NM_CLICK, CALISTAGE_DOT_LIST_WND, OnClickDotListWnd)
	ON_NOTIFY(NM_DBLCLK, CALISTAGE_DOT_LIST_WND, OnDblclkDotListWnd)
	ON_BN_CLICKED(CALIBRATION_SET_TOL_BTN, OnSetTolBtn)
	ON_BN_CLICKED(CALISTAGE_SHOW_DOT_MAP_CHK, OnShowDotMapChk)
	ON_BN_CLICKED(CALISTAGE_DOT_ERR_SET_BTN, OnDotErrSetBtn)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_CORNER_POS_BTN1, OnMoveToCornerPosBtn1)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_CORNER_POS_BTN2, OnMoveToCornerPosBtn2)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_CORNER_POS_BTN3, OnMoveToCornerPosBtn3)
	ON_BN_CLICKED(CALISTAGE_MOVE_TO_CORNER_POS_BTN4, OnMoveToCornerPosBtn4)
	ON_BN_CLICKED(CALISTAGE_ANALZE_CORNER_BTN, OnAnalzeCornerBtn)
	ON_BN_CLICKED(CALISTAGE_DOT_SAVE_BTN, OnDotSaveBtn)
	ON_BN_CLICKED(CALISTAGE_SAVE_IMAGE_BTN, OnSaveImageBtn)	
	ON_BN_CLICKED(CALISTAGE_GANTRY_FETCH_OFFSET_BTN, OnGantryFetchOffsetBtn)
	ON_BN_CLICKED(CALISTAGE_GANTRY_SET_STD_OFFSET_BTN, OnGantrySetStdOffsetBtn)
	ON_BN_CLICKED(CALISTAGE_ALIGN_CAMERA_BTN, OnAlignCameraBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneStage message handlers
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneStage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_DotListCtrl);	
	m_DotImageWnd.GetClientRect(&m_DotImageWndRect);
	m_DotImageWndMemDC1.CreateMemDC(&m_DotImageWnd, m_DotBKColor);
	m_MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this);	
	BuildDotListWndHeader();
	BuildDotAlignModeCombox();	
	BuildDotAlignCountCombox();
	LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	AOIDataDefine.BuildLaneIDCombox(m_LaneIDCombox);
	AOIDataDefine.BuildSystemSliceParamCombox(m_SliceCombox, false, false, false);
	JetAPI::SetComboxCurSel(m_AlignModeCombox, m_DotAlignMode);
	JetAPI::SetComboxCurSel(m_AlignCountCombox, m_AlignedCount);
	JetAPI::SetComboxCurSel(m_LaneIDCombox, LaneID);
	JetAPI::SetComboxCurSel(m_SliceCombox, SLICE_UNIQUE_ID_DEFAULT);	
	SetActiveLaneID(LaneID);
	SwitchMultiLanguage();
	if ( MULTI_LANE_2 != MultiLaneMode )
	{	JetAPI::EnableCtrlWnd(this, CALISTAGE_USE_DUAL_LANE_CHK, FALSE);	}

	CString str;
	const double SizeW = 1.300;//0.05;
	const double SizeH = 1.300;//0.05;
	const double PitchX = 6.35;//0.25;
	const double PitchY = 6.35;//0.25;
	const int    SkipX = 0;
	const int    SkipY = 0;
	const int    CountX = 59;
	const int    CountY = 55;	
	const int    MarginX= 32;
	const int    MarginY= 32;
	const int    ExtendX = 256;
	const int    ExtendY = 256;
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();

	str.Format(_T("%.2f"), SizeW);
	CWnd::SetDlgItemText(CALISTAGE_DOT_SIZE_W_EDIT, str);
	str.Format(_T("%.2f"), SizeH);
	CWnd::SetDlgItemText(CALISTAGE_DOT_SIZE_H_EDIT, str);	
	str.Format(_T("%.2f"), PitchX);
	CWnd::SetDlgItemText(CALISTAGE_DOT_PITCH_X_EDIT, str);
	str.Format(_T("%.2f"), PitchY);
	CWnd::SetDlgItemText(CALISTAGE_DOT_PITCH_Y_EDIT, str);	
	CWnd::SetDlgItemInt(CALISTAGE_DOT_SKIP_X_EDIT, SkipX);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_SKIP_Y_EDIT, SkipY);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_COUNT_X_EDIT, CountX);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_COUNT_Y_EDIT, CountY);	
	CWnd::SetDlgItemInt(CALISTAGE_DOT_MARGIN_X_EDIT, MarginX);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_MARGIN_Y_EDIT, MarginY);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT, ExtendX);
	CWnd::SetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT, ExtendY);	
	CWnd::SetDlgItemInt(CALISTAGE_DOT_MATCH_SCORE_EDIT, 85);
	CWnd::SetDlgItemInt(CALISTAGE_THRESHOLD_EDIT, m_Threshold);	

	str.Format(_T("%.0f"), m_AlignTolerance_DOT);
	CWnd::SetDlgItemText(CALIBRATION_ALIGN_TOL_EDIT, str);	
	str.Format(_T("%.0f"), m_AlignTolerance_HOR);
	CWnd::SetDlgItemText(CALIBRATION_ALIGN_HOR_TOL_EDIT, str);	
	str.Format(_T("%.0f"), m_AlignTolerance_Ver);
	CWnd::SetDlgItemText(CALIBRATION_ALIGN_VER_TOL_EDIT, str);	

	str.Format(_T("%.0f"), m_ErrorOffsetX);
	CWnd::SetDlgItemText(CALISTAGE_DOT_ERR_X_EDIT, str);	
	str.Format(_T("%.0f"), m_ErrorOffsetY);
	CWnd::SetDlgItemText(CALISTAGE_DOT_ERR_Y_EDIT, str);		

	CWnd::SetDlgItemInt(CALISTAGE_AUTO_FOCUS_PITCH_EDIT, 10);
	CWnd::SetDlgItemInt(CALISTAGE_MOVE_DELAY_TIME_EDIT, 50);	
	CWnd::SetDlgItemInt(CALISTAGE_MAX_XYCALI_COUNT_EDIT, 5);

	double GantryStdOffset=0;
	const int Axis=GetGantryAxis();	
	CWnd::SetDlgItemInt(CALISTAGE_GANTRY_FETCH_OFFSET_EDIT, 0);
	CWnd::SetDlgItemInt(CALISTAGE_GANTRY_SET_STD_OFFSET_EDIT, 0);	
	if ( Axis >= 0 )
	{
		if ( MotionCtrlPtr->GetGantryStdOffset(Axis, GantryStdOffset) == true )
		{	CWnd::SetDlgItemInt(CALISTAGE_GANTRY_SET_STD_OFFSET_EDIT, (int)(GantryStdOffset)); }
	}

	MotionCtrlPtr->SetStopXYCalibration(false);	
	if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
	{	CWnd::CheckDlgButton(CALISTAGE_ENABLE_XYCALI_CHK, TRUE); }
	else
	{	CWnd::CheckDlgButton(CALISTAGE_ENABLE_XYCALI_CHK, FALSE); }

	m_AlignedMap.Identity();	

	LoadParamFile();	
	OnDotBuildBtn();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseDotImage();

	this->m_ShowBuffer = NULL;
	this->m_ShowBuffer1 = NULL;
	this->m_MaskBuffer = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_SpaceBuffer1 = NULL;
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
void CCaliPaneStage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL )
	{	return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) 
	{	return; }
	
	CWnd *WndPtr=NULL;
	RECT  WndRect={0};
	SIZE  WndSIze={0};	
	if ( m_DotListCtrl.GetSafeHwnd() != NULL  )
	{
		m_DotListCtrl.GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);		
		WndRect.bottom = cy-4;
		m_DotListCtrl.MoveWindow(&WndRect);
	}

	WndPtr = CWnd::GetDlgItem(CALISTAGE_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);		
		WndRect.right = cx-4;
		WndPtr->MoveWindow(&WndRect);		
	}

	m_ImageWnd.GetWindowRect(&WndRect);
	ScreenToClient(&WndRect);
	WndRect.right = cx-4;
	WndRect.bottom = cy-4;
	m_ImageWnd.MoveWindow(&WndRect);
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC1.CreateMemDC(&m_ImageWnd, m_BKColor);
	m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_BKColor);
	return;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{			
		AOIDataCollect.SetCallbackWnd(GetSafeHwnd());	
		FocusToEditCtrl();
		MotionCtrlPtr->SetStopXYCalibration(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);

		const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
		if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
		{	CWnd::CheckDlgButton(CALISTAGE_ENABLE_XYCALI_CHK, TRUE); }
		else
		{	CWnd::CheckDlgButton(CALISTAGE_ENABLE_XYCALI_CHK, FALSE); }
		this->StartReGrab(TRUE);		
	}
	else
	{	MotionCtrlPtr->SetStopXYCalibration(true); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	// Do not call CDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneStage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CCaliPaneStage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			if ( RetrieveCameraImage(wParam, lParam, true) == false )
			{				
				LockUIWnd(false);	
				CWnd::CheckDlgButton(CALISTAGE_GRAB_REPEAT_CHK, FALSE);
			}
			break;
		}
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		if ( ExecReGrab(wParam) == false )
		{	LockUIWnd(false); }
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		LockUIWnd(false);
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveParamFile()//儲存參數檔案
{	
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString Default;	
	CString Section=_T("IDD_CALIBRATION_PANE_STAGE");	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SIZE_W_EDIT;
	WndKey = _T("CALISTAGE_DOT_SIZE_W_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SIZE_H_EDIT;
	WndKey = _T("CALISTAGE_DOT_SIZE_H_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_PITCH_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_PITCH_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_PITCH_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_PITCH_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SKIP_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_SKIP_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SKIP_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_SKIP_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_COUNT_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_COUNT_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_COUNT_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_COUNT_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_DOT_MARGIN_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_MARGIN_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_MARGIN_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_MARGIN_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_EXTEND_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_EXTEND_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_EXTEND_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_EXTEND_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	////=============================
	WndID = CALIBRATION_ALIGN_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALIBRATION_ALIGN_HOR_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_HOR_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALIBRATION_ALIGN_VER_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_VER_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_THRESHOLD_EDIT;
	WndKey = _T("CALISTAGE_THRESHOLD_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_MATCH_SCORE_EDIT;
	WndKey = _T("CALISTAGE_DOT_MATCH_SCORE_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_ERR_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_ERR_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_ERR_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_ERR_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	//---------------------------------------------------------------------------------//
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::LoadParamFile()//載入參數檔案	
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString String;
	CString Default;
	CString Section=_T("IDD_CALIBRATION_PANE_STAGE");	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SIZE_W_EDIT;
	WndKey = _T("CALISTAGE_DOT_SIZE_W_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SIZE_H_EDIT;
	WndKey = _T("CALISTAGE_DOT_SIZE_H_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_PITCH_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_PITCH_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_PITCH_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_PITCH_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SKIP_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_SKIP_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_SKIP_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_SKIP_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_COUNT_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_COUNT_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_COUNT_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_COUNT_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);	
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_DOT_MARGIN_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_MARGIN_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_MARGIN_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_MARGIN_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_EXTEND_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_EXTEND_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_EXTEND_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_EXTEND_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//	
	WndID = CALIBRATION_ALIGN_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//	
	WndID = CALIBRATION_ALIGN_HOR_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_HOR_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALIBRATION_ALIGN_VER_TOL_EDIT;
	WndKey = _T("CALIBRATION_ALIGN_VER_TOL_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_THRESHOLD_EDIT;
	WndKey = _T("CALISTAGE_THRESHOLD_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_MATCH_SCORE_EDIT;
	WndKey = _T("CALISTAGE_DOT_MATCH_SCORE_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_ERR_X_EDIT;
	WndKey = _T("CALISTAGE_DOT_ERR_X_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_DOT_ERR_Y_EDIT;
	WndKey = _T("CALISTAGE_DOT_ERR_Y_EDIT");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	//---------------------------------------------------------------------------------//	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveImageFile()//存圖
{	
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	CString Filename=dialog.GetPathName();
	if ( ImageAPI.SaveImage(Filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, true) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_PANE_STAGE");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_PANE_STAGE;
	WndKey = _T("IDD_CALIBRATION_PANE_STAGE");
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
	WndID = CALISTAGE_DOT_INFO_GROUP;
	WndKey = _T("CALISTAGE_DOT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_SIZE_W_LABE;
	WndKey = _T("CALISTAGE_DOT_SIZE_W_LABE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_SIZE_H_LABEL;
	WndKey = _T("CALISTAGE_DOT_SIZE_H_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_PITCH_X_LABEL;
	WndKey = _T("CALISTAGE_DOT_PITCH_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_PITCH_Y_LABEL;
	WndKey = _T("CALISTAGE_DOT_PITCH_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_COUNT_X_LABEL;
	WndKey = _T("CALISTAGE_DOT_COUNT_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_COUNT_Y_LABEL;
	WndKey = _T("CALISTAGE_DOT_COUNT_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_SKIP_LABEL;
	WndKey = _T("CALISTAGE_DOT_SKIP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_MARGIN_X_LABEL;
	WndKey = _T("CALISTAGE_DOT_MARGIN_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_MARGIN_Y_LABEL;
	WndKey = _T("CALISTAGE_DOT_MARGIN_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_EXTEND_X_LABEL;
	WndKey = _T("CALISTAGE_DOT_EXTEND_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_EXTEND_Y_LABEL;
	WndKey = _T("CALISTAGE_DOT_EXTEND_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_WHITE_CHK;
	WndKey = _T("CALISTAGE_DOT_WHITE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_REVERSE_X_CHK;
	WndKey = _T("CALISTAGE_REVERSE_X_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_MATCH_SCORE_LABEL;
	WndKey = _T("CALISTAGE_DOT_MATCH_SCORE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_BUILD_BTN;
	WndKey = _T("CALISTAGE_DOT_BUILD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_SAVE_BTN;
	WndKey = _T("CALISTAGE_DOT_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_SAVE_IMAGE_BTN;
	WndKey = _T("CALISTAGE_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_MATCH_BTN;
	WndKey = _T("CALISTAGE_DOT_MATCH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_THRESHOLD_LABEL;
	WndKey = _T("CALISTAGE_THRESHOLD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_SLICE_LABEL;
	WndKey = _T("CALISTAGE_SLICE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_AUTO_FOCUS_PITCH_LABEL;
	WndKey = _T("CALISTAGE_AUTO_FOCUS_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_AUTO_FOCUS_BTN;
	WndKey = _T("CALISTAGE_AUTO_FOCUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_GRAB_BTN;
	WndKey = _T("CALISTAGE_GRAB_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_GRAB_REPEAT_CHK;
	WndKey = _T("CALISTAGE_GRAB_REPEAT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_SHOW_DOT_MAP_CHK;
	WndKey = _T("CALISTAGE_SHOW_DOT_MAP_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_SHOW_CENTER_LINE_CHK;
	WndKey = _T("CALISTAGE_SHOW_CENTER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_SHOW_CURSOR_LINE_CHK;
	WndKey = _T("CALISTAGE_SHOW_CURSOR_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_MOVE_DELAY_TIME_LABEL;
	WndKey = _T("CALISTAGE_MOVE_DELAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_MOTION_WND_BTN;
	WndKey = _T("CALISTAGE_MOTION_WND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_LANE_ID_LABEL;
	WndKey = _T("CALISTAGE_LANE_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_USE_DUAL_LANE_CHK;
	WndKey = _T("CALISTAGE_USE_DUAL_LANE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALIBRATION_SET_TOL_BTN;
	WndKey = _T("CALIBRATION_SET_TOL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_DOT_ALIGN_BTN;
	WndKey = _T("CALISTAGE_DOT_ALIGN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_ALIGN_MODE_LABEL;
	WndKey = _T("CALISTAGE_ALIGN_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIBRATION_ALIGN_TOL_LABEL;
	WndKey = _T("CALIBRATION_ALIGN_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_ALIGN_X12_BTN;
	WndKey = _T("CALISTAGE_ALIGN_X12_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIBRATION_ALIGN_HOR_TOL_LABEL;
	WndKey = _T("CALIBRATION_ALIGN_HOR_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_MOVE_TO_X1_BTN;
	WndKey = _T("CALISTAGE_MOVE_TO_X1_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_MOVE_TO_X2_BTN;
	WndKey = _T("CALISTAGE_MOVE_TO_X2_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_VERIFY_X_BTN;
	WndKey = _T("CALISTAGE_VERIFY_X_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_ALIGN_Y12_BTN;
	WndKey = _T("CALISTAGE_ALIGN_Y12_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIBRATION_ALIGN_VER_TOL_LABEL;
	WndKey = _T("CALIBRATION_ALIGN_VER_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_MOVE_TO_Y1_BTN;
	WndKey = _T("CALISTAGE_MOVE_TO_Y1_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_MOVE_TO_Y2_BTN;
	WndKey = _T("CALISTAGE_MOVE_TO_Y2_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_VERIFY_Y_BTN;
	WndKey = _T("CALISTAGE_VERIFY_Y_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_ALIGN_CORNER_BTN;
	WndKey = _T("CALISTAGE_ALIGN_CORNER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_ANALZE_CORNER_BTN;
	WndKey = _T("CALISTAGE_ANALZE_CORNER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALISTAGE_MOVE_TO_CORNER_POS_BTN1;
	WndKey = _T("CALISTAGE_MOVE_TO_CORNER_POS_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_MOVE_TO_CORNER_POS_BTN2;
	WndKey = _T("CALISTAGE_MOVE_TO_CORNER_POS_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_MOVE_TO_CORNER_POS_BTN3;
	WndKey = _T("CALISTAGE_MOVE_TO_CORNER_POS_BTN3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_MOVE_TO_CORNER_POS_BTN4;
	WndKey = _T("CALISTAGE_MOVE_TO_CORNER_POS_BTN4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_CALIBRATE_DOT_BTN;
	WndKey = _T("CALISTAGE_CALIBRATE_DOT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_BUILD_XYDOT_TABLE_BTN;
	WndKey = _T("CALISTAGE_BUILD_XYDOT_TABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = CALISTAGE_ENABLE_XYCALI_CHK;
	WndKey = _T("CALISTAGE_ENABLE_XYCALI_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_DOT_ERR_SET_BTN;
	WndKey = _T("CALISTAGE_DOT_ERR_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_LIST_LABEL;
	WndKey = _T("CALISTAGE_DOT_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_ERR_X_LABEL;
	WndKey = _T("CALISTAGE_DOT_ERR_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_DOT_ERR_Y_LABEL;
	WndKey = _T("CALISTAGE_DOT_ERR_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_MAX_XYCALI_COUNT_LABEL;
	WndKey = _T("CALISTAGE_MAX_XYCALI_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_ALIGN_COUNT_LABEL;
	WndKey = _T("CALISTAGE_ALIGN_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALISTAGE_SAVE_DOT_ROI_IMG_CHK;
	WndKey = _T("CALISTAGE_SAVE_DOT_ROI_IMG_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALISTAGE_GANTRY_FETCH_OFFSET_BTN;
	WndKey = _T("CALISTAGE_GANTRY_FETCH_OFFSET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALISTAGE_GANTRY_SET_STD_OFFSET_BTN;
	WndKey = _T("CALISTAGE_GANTRY_SET_STD_OFFSET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = CALISTAGE_ALIGN_CAMERA_BTN;
	WndKey = _T("CALISTAGE_ALIGN_CAMERA_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	//WndID = AAAAAAAAAAAAAAAAAAAAA;
	//WndKey = _T("AAAAAAAAAAAAAAAAAAAAA");
	//this->GetDlgItemText(WndID, LabelText);
	//AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	//this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	return;
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneStage::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CALIBRATION_PANE_STAGE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1)
{
	m_SpaceBuffer = Buffer;
	m_SpaceBuffer1 = Buffer1;
	m_SpaceBufferSize = Size;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1)
{
	m_ShowBuffer = Buffer;
	m_ShowBuffer1 = Buffer1;
	m_ShowBufferSize = Size;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer)
{
	m_MaskBuffer = MaskBuffer;
	m_ImageBuffer = Buffer;
	m_ImageBuffer1 = Buffer1;
	m_ImageBuffer2 = Buffer2;	
	m_PhaseBuffer = PhaseBuffer;
	m_PhaseBuffer1 = PhaseBuffer1;
	m_PhaseBuffer2 = PhaseBuffer2;
	m_ImageBufferSize = Size;		
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDotBuildBtn() 
{
	// TODO: Add your control notification handler code here
	BuildDotImage();
	HDC hMemDC = m_DotImageWndMemDC1.GetSafeHdc();
	if ( NULL == hMemDC ) { return ; }
	DrawDotImage(hMemDC);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::ClearAlignedFdList()
{
	m_AlignedFdCadList.clear();
	m_AlignedFdResList.clear();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetAlignedMapUsed(bool val)//使用玻璃板座標對齊轉換
{
	m_AlignedMapUsed = val;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::GetAlignedMapUsed() const//使用玻璃板座標對齊轉換
{
	return m_AlignedMapUsed;
}
//-------------------------------------------------------------------------------------//
CAMERA_ID CCaliPaneStage::GetCameraID()
{
	return m_CameraID;
}
//-------------------------------------------------------------------------------------//
COLORREF CCaliPaneStage::GetColorNG()//取得NG顏色
{
	return (COLORREF)(0x0000FF);
}
//-------------------------------------------------------------------------------------//
COLORREF CCaliPaneStage::GetColorOK()//取得OK顏色
{
	return (COLORREF)(0x00FF00);
}
//-------------------------------------------------------------------------------------//
COLORREF CCaliPaneStage::GetColorSel()//取得選取顏色
{
	return (COLORREF)(0xFFFFFF);
}
//-------------------------------------------------------------------------------------//
COLORREF CCaliPaneStage::GetColorNone()//取得未處理顏色
{
	return (COLORREF)(0x808080);
}
//-------------------------------------------------------------------------------------//
int CCaliPaneStage::GetThreshold()//取得2值化閥值
{
	return m_Threshold;	
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetUnitRatio()//取得單位比例
{
	//return 2.54*10*1000;//inch to um;
	return 1000.0;//mm to um;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetErrorOffsetX()
{
	return m_ErrorOffsetX;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetErrorOffsetY()
{
	return m_ErrorOffsetY;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetHorTolerance()//取得水平公差範圍
{
	return m_AlignTolerance_HOR;//um
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetVerTolerance()//取得水平公差範圍
{
	return m_AlignTolerance_Ver;//um
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetAlignTolerance()//取得對齊公差範圍
{
	return m_AlignTolerance_DOT;//um	 
}
//-------------------------------------------------------------------------------------//
BOX_SHAPE_MODE CCaliPaneStage::GetDotShapeMode()
{
	return BOX_SHAPE_ELLIPSE;
	return BOX_SHAPE_RECTANGLE;	
}
//-------------------------------------------------------------------------------------//
DWORD CCaliPaneStage::GetMoveDownDelyTime()//移動完成延遲時間
{
	DWORD DelyTime = (DWORD)(CWnd::GetDlgItemInt(CALISTAGE_MOVE_DELAY_TIME_EDIT));
	return DelyTime;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::FocusToEditCtrl()
{
	JetAPI::FocusCtrlWnd(this, CALISTAGE_AUTO_FOCUS_PITCH_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotAlignModeCombox()
{
	CString    str;
	int        idx=0;	
	CComboBox &Combox = m_AlignModeCombox;

	idx=0;
	JetAPI::ClearCombox(Combox);
	
	str = _T("Blob");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DOT_ALIGN_BLOB);
	idx ++;

	str = _T("Match");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DOT_ALIGN_MATCH);
	idx ++;	

	return true;
	str = _T("Hough Circle");
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DOT_ALIGN_HOUGH_CIRCLE);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotAlignCountCombox()
{
	CString    str;
	int        idx=0;	
	CComboBox &Combox = m_AlignCountCombox;

	idx=0;
	JetAPI::ClearCombox(Combox);

	for ( int i=1; i<5; i++ )
	{
		str.Format(_T("%d"), i);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, i);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	CThisListCtrl &ListCtrl = m_DotListCtrl;
	ListCtrl.GetClientRect(&Rect);	
	width2 = 36;
	str = _T("Y");
	//str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("X");
	//str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width = (Rect.right-Rect.left-width-16)/3;
	str = _T("dx");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;		

	str = _T("dy");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;		

	str = _T("dL");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotListWnd()
{
	CThisListCtrl &ListCtrl = m_DotListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	
	size_t       i=0;
	CString      str;
	int          nItem=0;
	int          nSubItem=0;		
	double       dOffsetL=0.0;
	TDotNode     DotNode;	
	COLORREF     ColorNG=GetColorNG();
	COLORREF     ColorOK=0x000000;
	COLORREF     ColorNone=0x000000;
	const size_t DotNodeCount = m_DotNodeList.size();

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<DotNodeCount; i++  )
	{
		DotNode = m_DotNodeList[i];
		nSubItem = 0;

		str.Format(_T("%d"), DotNode.nIndexY+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);
		switch ( DotNode.eResultID )
		{
		case RESULT_ID_NONE:
		case RESULT_ID_SKIP:
		case RESULT_ID_BYPASS:		
			ListCtrl.SetItemTextColor(nItem, ColorNone);
			break;
		case RESULT_ID_OK:
			ListCtrl.SetItemTextColor(nItem, ColorOK);
			break;
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			ListCtrl.SetItemTextColor(nItem, ColorNG);
			break;
		}
		str.Format(_T("%d"), DotNode.nIndexY+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%d"), DotNode.nIndexX+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%.2f"), DotNode.dOffsetX);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%.2f"), DotNode.dOffsetY);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		dOffsetL = ::sqrt((DotNode.dOffsetX*DotNode.dOffsetX)+(DotNode.dOffsetY*DotNode.dOffsetY));
		str.Format(_T("%.2f"), dOffsetL);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::UpdateDotListWnd()
{
	CThisListCtrl &ListCtrl = m_DotListCtrl;	
	
	int          i=0;
	int          Index=0;
	CString      str;
	int          nItem=0;
	int          nSubItem=0;
	int          nItemSel=-1;
	double       dOffsetL=0.0;
	TDotNode     DotNode;
	COLORREF     ColorNG=GetColorNG();
	COLORREF     ColorOK=0x000000;
	COLORREF     ColorNone=0x000000;
	const int    ItemCount=ListCtrl.GetItemCount();
	const size_t DotNodeCount = m_DotNodeList.size();

	nItem=0;
	//ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ItemCount; i++  )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index<0 || Index>=DotNodeCount ) { continue; }
		
		nItem = i;
		nSubItem = 0;
		DotNode = m_DotNodeList[Index];
		if ( m_DotNodeIndex == Index )
		{	nItemSel = nItem; }
		switch ( DotNode.eResultID )
		{
		case RESULT_ID_NONE:
		case RESULT_ID_SKIP:
		case RESULT_ID_BYPASS:
			ListCtrl.SetItemTextColor(nItem, ColorNone);
			break;
		case RESULT_ID_OK:
			ListCtrl.SetItemTextColor(nItem, ColorOK);
			break;
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			ListCtrl.SetItemTextColor(nItem, ColorNG);
			break;
		}
		
		str.Format(_T("%d"), DotNode.nIndexY+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%d"), DotNode.nIndexX+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%.2f"), DotNode.dOffsetX);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		str.Format(_T("%.2f"), DotNode.dOffsetY);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;

		dOffsetL = ::sqrt((DotNode.dOffsetX*DotNode.dOffsetX)+(DotNode.dOffsetY*DotNode.dOffsetY));
		str.Format(_T("%.2f"), dOffsetL);
		ListCtrl.SetItemText(nItem, nSubItem, str);	nSubItem ++;
	}

	if ( nItemSel > 0 )
	{
		const int nItemShow = MIN(ItemCount-1, nItemSel+3);
		//ListCtrl.EnsureVisible(nItemShow, FALSE);
		ListCtrl.SetItemState(nItemSel, LVIS_SELECTED, LVIS_SELECTED);
		ListCtrl.SetFocus();
	}
	//ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::EnsureVisibleDotListData(int nData)
{
	if ( nData < 0 ) { return true; }
	CThisListCtrl &ListCtrl = m_DotListCtrl;	
	const int ItemCount = ListCtrl.GetItemCount();
	if ( 0 == ItemCount ) { return true; }
	
	int i=0;
	int Index=0;
	int nItem = -1;
	for ( i=0; i<ItemCount; i++ )
	{
		Index = ListCtrl.GetItemData(i);
		if ( Index != nData) { continue; }
		nItem = i;
		break;
	}
	if ( nItem < 0 ) { return true; }
	const int nItemShow = MIN(ItemCount-1, nItem+3);
	if ( nItem != nItemShow )
	{	ListCtrl.EnsureVisible(nItem, FALSE);	}
	ListCtrl.EnsureVisible(nItemShow, FALSE);
	ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	ListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotImage()
{
	CString str;
	const char fnName[] = "CCaliPaneStage::BuildDotImage";	
	if ( UpdateDotParamFromUI() == false ) { return false; }

	const int DotPatW = m_DotSizeW;
	const int DotPatH = m_DotSizeH;	
	const int MarginW = m_DotMarginW;
	const int MarginH = m_DotMarginH;
	const int PatternW = m_DotPatternW;
	const int PatternH = m_DotPatternH;
	const int PatternCpX = PatternW/2;
	const int PatternCpY = PatternH/2;
	const bool bWhiteDot = m_DotWhiteMode;

	ReleaseDotImage();
	CvSize Size;
	Size.width  = PatternW;
	Size.height = PatternH;	
	const int nAlign = 4;
	const int PatBitCount = 8;
	const int ImageChannel = 1;
	const int SrcWStripp = JetAPI::GetBMPImagePixelsPerLine((Size.width), PatBitCount, nAlign);	
	const int ImageSize = SrcWStripp*Size.height;	

	IplImage * pImg = ::cvCreateImage(Size, PatBitCount, 1);
	if ( NULL == pImg )
	{
		str = _T("Error, Create Image Fault");
		JetAPI::ShowMessageBox(str);
		return false;	
	}	
	
	int   Radius=0;
	int   RectW=0, RectH=0;	
	unsigned char BKClr   = 0x00;
	unsigned char FillClr = 0xFF;
	RECT  MainRect={0,0,0,0};
	RECT  MaskRect={0,0,0,0};
	RECT  TempRect={0,0,0,0};	
	RECT  CircleRect={0,0,0,0};		
	
	CvBox2D cvbox;
	CvPoint PA1, PA2;	
	CvScalar color;
	CvScalar bkcolor;
	
	MainRect.left   = MarginW;
	//MainRect.right  = PatternW-MarginW-1;
	MainRect.right  = PatternW-MarginW;//改成左右對稱
	MainRect.top    = MarginH;
	//MainRect.bottom = PatternH-MarginH-1;
	MainRect.bottom = PatternH-MarginH;//改成上下對稱

	//Bk Image	
	if ( true == bWhiteDot)
	{	
		BKClr   = 0x00; 
		FillClr = 0xFF;
	}
	else
	{	
		BKClr   = 0xFF; 
		FillClr = 0x00;
	}

	bkcolor.val[0] = BKClr;
	bkcolor.val[1] = BKClr;
	bkcolor.val[2] = BKClr;
	bkcolor.val[3] =   0;	
	::memset(pImg->imageData, BKClr, pImg->imageSize);	

	color.val[0] = FillClr;
	color.val[1] = FillClr;
	color.val[2] = FillClr;
	color.val[3] =   0;						
	
	MaskRect.left = PatternCpX-(DotPatW/2);
	MaskRect.top  = PatternCpY-(DotPatH/2);
	MaskRect.right = MaskRect.left+DotPatW;
	MaskRect.bottom = MaskRect.top+DotPatH;
	MaskRect.right = MaskRect.right - 1;
	MaskRect.bottom = MaskRect.bottom - 1;
	BOX_SHAPE_MODE BoxShapeMode = GetDotShapeMode();	
	switch ( BoxShapeMode )
	{	
	case BOX_SHAPE_ELLIPSE:
		cvbox.center.x = (MaskRect.left+MaskRect.right)/2.0f;
		cvbox.center.y = (MaskRect.top+MaskRect.bottom)/2.0f;
		cvbox.size.height = (float)(MaskRect.right-MaskRect.left);
		cvbox.size.width = (float)(MaskRect.bottom-MaskRect.top);
		cvbox.angle = 90; 
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);				
		break;	
	default://BOX_SHAPE_RECTANGLE
		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		break;
	}		
	const bool bColorCamera=false;
	const size_t BufferSize=PatternW*PatternH*4;//直接取最大
	if ( JetMemory.alloc_func(BufferSize, m_DotImagePtr, fnName, "m_DotImagePtr") == false )
	{
		str = _T("Error, Create Dot Image Buffer Fault");
		JetAPI::ShowMessageBox(str);
		::cvReleaseImage(&pImg);
		return false;
	}

	if ( false == bColorCamera )
	{	
		m_DotImageW = PatternW;
		m_DotImageH = PatternH;
		m_DotImageBitCount = 8;	
		m_DotImageStep = SrcWStripp;		
		::memcpy(m_DotImagePtr, pImg->imageData, sizeof(unsigned char)*m_DotImageStep*m_DotImageH);
	}
	else
	{
		m_DotImageW = PatternW;
		m_DotImageH = PatternH;
		m_DotImageBitCount = 24;	
		m_DotImageStep = JetAPI::GetBMPImagePixelsPerLine(m_DotImageW, m_DotImageBitCount, nAlign);		
		
		size_t i=0, j=0;
		size_t SrcIdx=0, DstIdx=0;
		const size_t CopyLen = SrcWStripp;
		for ( i=0; i<PatternH; i++ )
		{
			for ( j=0; j<PatternW; j++ )
			{
				SrcIdx = (i*SrcWStripp)+(j);
				DstIdx = (i*m_DotImageStep)+(j*3);
				m_DotImagePtr[DstIdx] = pImg->imageData[SrcIdx];
				m_DotImagePtr[DstIdx+1] = pImg->imageData[SrcIdx];
				m_DotImagePtr[DstIdx+2] = pImg->imageData[SrcIdx];
			}
		}		
	}
	::cvReleaseImage(&pImg);	
	m_ShowRoiRect = true;

	ImageAPI.CalcImageWndFitZoom(m_DotImageW, m_DotImageH, m_DotImageWndRect, 1.1, m_DotImageZoom);
#ifdef _DEBUG
	BOOL bSaved = TRUE;
	if ( TRUE == bSaved )
	{	
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DotPattern.PNG"));
		ImageAPI.SavePNGImage(str, m_DotImageW, m_DotImageH, m_DotImageStep, m_DotImageBitCount, m_DotImagePtr, true);
	}
#endif//_DEBUG	

	SetLockUIAlign(false);//鎖住對齊			
	SetLockUIAlignHor(true);//鎖住水平調整
	SetLockUIAlignVer(true);//鎖住垂直確認
	SetLockUIAlignCorner(true);//鎖住四角落確認
	SetLockUIAlignDotNode(true);//鎖住玻璃點校正	
	LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ReleaseDotImage()
{
	if ( NULL != m_DotImagePtr )
	{	JetMemory.free_func(m_DotImagePtr);	}
	m_DotImageZoom = 1.0;
	m_DotImagePtr = NULL;
	m_DotImageW = 256;
	m_DotImageH = 256;
	m_DotImageStep = 256*3;
	m_DotImageBitCount = 24;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::RedrawWnd()
{
	DrawImageWnd();
	DrawDotImageWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawDotImageWnd()
{
	if ( m_DotImageWnd.GetSafeHwnd() == NULL  ) { return; }
	CClientDC dc(&m_DotImageWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( NULL == hDC ) { return ; }
	HDC hBKDC1 = m_DotImageWndMemDC1.GetSafeHdc();
	if ( NULL == hBKDC1 ) { return ; }
	RECT WndRect=m_DotImageWndRect;
	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hBKDC1, 0, 0, SRCCOPY );
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawDotImage(HDC hDC)
{
	if ( NULL == hDC ) { return; }	
	COLORREF BKClr = m_DotBKColor;
	RECT WndRect = m_DotImageWndRect;	
	HBRUSH hBrush = ::CreateSolidBrush(BKClr);
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &WndRect, hBrush);
		::DeleteObject(hBrush);
	}	
	if ( NULL == m_DotImagePtr )
	{	return; }
	ImageAPI.DrawImageToDC(hDC, m_DotImageW, m_DotImageH, m_DotImageStep, m_DotImageBitCount, m_DotImagePtr, WndRect, m_DotImageOffset, m_DotImageZoom, BKClr);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawImageWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL  ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hBKDC1 = m_ImageWndMemDC1.GetSafeHdc();
	HDC hBKDC2 = m_ImageWndMemDC2.GetSafeHdc();
	if ( NULL == hDC ) { return ; }	
	if ( NULL == hBKDC1 ) { return ; }
	if ( NULL == hBKDC2 ) { return ; }
	RECT WndRect=m_ImageWndRect;
	BOOL bShowDotMap = CWnd::IsDlgButtonChecked(CALISTAGE_SHOW_DOT_MAP_CHK);

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	if ( FALSE == bShowDotMap )
	{	::BitBlt(hBKDC2, 0, 0, WndRect.right, WndRect.bottom, hBKDC1, 0, 0, SRCCOPY ); }
	else
	{	DrawDotMap(hBKDC2); }
	
	//Draw something on hBKDC2
	if ( FALSE == bShowDotMap )
	{		
		CString str;
		POINT   TextPt;		
		LOGFONT LogFont;
		HFONT   hFont = NULL;
		HFONT   hOldFont = NULL;
		const int FontH = m_ImageFontHeight;
		const int TextPitchY = FontH+2;		

		::memset(&LogFont, 0x00, sizeof(LogFont));
		LogFont.lfHeight = FontH;	
		LogFont.lfWeight = FW_BOLD;
		LogFont.lfCharSet = DEFAULT_CHARSET;
		::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
		hFont = CreateFontIndirect(&LogFont);	
		hOldFont = (HFONT)::SelectObject(hBKDC2, hFont);

		TextPt.x = 4;
		TextPt.y = WndRect.bottom-TextPitchY;
		str = m_strImageValue;
		COLORREF OldTextColor = ::SetTextColor(hBKDC2, 0xFFFFFF);
		::TextOut(hBKDC2, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y -= TextPitchY;	 
		::SetTextColor(hBKDC2, OldTextColor);

		if ( true == m_ShowRoiRect )
		{	DrawRoiRect(hBKDC2);	}

		if ( true == m_ShowCrossLine )
		{	DrawCrossLine(hBKDC2);	}

		if ( true == m_ShowCursorLine )
		{	DrawCursorLine(hBKDC2);	}

		if ( true == m_ShowDotMatched )
		{	DrawDotMatched(hBKDC2);	}	

		::SelectObject(hBKDC2, hOldFont);
		::DeleteObject(hFont);	hFont=NULL;
	}
	
	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hBKDC2, 0, 0, SRCCOPY );
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawImageWndMemDC()
{
	HDC hMemDC = m_ImageWndMemDC1.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	COLORREF clrBK = m_BKColor;		
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	if ( NULL == m_ShowBuffer ) { return; }
	if ( ImageAPI.DrawImageToDC(hMemDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawRoiRect(HDC hDC)//繪製搜尋範圍
{
	;
	CString  str;
	RECT     Rect;
	SIZE     WndSzi;
	POINT    WndPti;
	TSIZE2D  WndSzd;
	TPOINT2D WndPtd;
	TPOINT2D ImagePtd;
	BOX_SHAPE_MODE BoxShapeMode = GetDotShapeMode();	
	ImagePtd.x = (double)(m_ImageW/2.0);
	ImagePtd.y = (double)(m_ImageH/2.0);

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);	
	WndSzd.cx = m_DotRoiSize.cx/m_ImageZoom;
	WndSzd.cy = m_DotRoiSize.cy/m_ImageZoom;

	JetAPI::Size2DToSize(WndSzd, WndSzi);	
	JetAPI::Point2DToPoint(WndPtd, WndPti);	
	Rect.left = WndPti.x-(WndSzi.cx/2);
	Rect.top  = WndPti.y-(WndSzi.cy/2);
	Rect.right = Rect.left + WndSzi.cx;
	Rect.bottom = Rect.top + WndSzi.cy;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);	
	ImageAPI.DrawRectLine(hDC, Rect);			
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawCrossLine(HDC hDC)//繪製十字線
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
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawCursorLine(HDC hDC)//繪製鼠標線
{
	POINT WndPt;
	RECT WndRect=m_ImageWndRect;
	::GetCursorPos(&WndPt);
	m_ImageWnd.ScreenToClient(&WndPt);
	if ( ::PtInRect(&WndRect, WndPt) == FALSE )
	{	return; }

	HPEN hPen = ::CreatePen(PS_DOT, 1, 0x00F0FF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	::MoveToEx(hDC, WndRect.left, WndPt.y, NULL);
	::LineTo(hDC, WndRect.right, WndPt.y);

	::MoveToEx(hDC, WndPt.x, WndRect.top, NULL);
	::LineTo(hDC, WndPt.x, WndRect.bottom);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawDotMatched(HDC hDC, double ImgPosX, double ImgPosY)//繪製點找到的位置
{
	if ( NULL == hDC ) { return; }
	RECT     Rect;	
	TSIZE2D  WndSzd;
	TPOINT2D WndPtd;	
	const TPOINT2D ImagePtd(ImgPosX, ImgPosY);
	BOX_SHAPE_MODE BoxShapeMode = GetDotShapeMode();

	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePtd, WndPtd);	
	WndSzd.cx = m_DotMatchedSz.cx/m_ImageZoom;
	WndSzd.cy = m_DotMatchedSz.cy/m_ImageZoom;

	double fL=WndPtd.x-(WndSzd.cx/2);
	double fT=WndPtd.y-(WndSzd.cy/2);
	double fR=fL+WndSzd.cx;
	double fB=fT+WndSzd.cy;
	Rect.left = (int)(fL+0.5);
	Rect.top = (int)(fT+0.5);
	Rect.right = (int)(fR+0.5);
	Rect.bottom = (int)(fB+0.5);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	switch ( BoxShapeMode )
	{
	case BOX_SHAPE_ELLIPSE:		
		ImageAPI.DrawEllipseLine(hDC, Rect);		
		break;
	default://BOX_SHAPE_RECTANGLE
		ImageAPI.DrawRectLine(hDC, Rect);		
		break;
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen=NULL;
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawDotMatched(HDC hDC)//繪製點找到的位置
{
	//if ( false == m_DotMatched ) { return ; }
	CString  str;
	const size_t DotMatchedPtCount=m_DotMatchedPtList.size();
	if ( 0 == DotMatchedPtCount )
	{
		DrawDotMatched(hDC, m_DotMatchedPt.x, m_DotMatchedPt.y);
	}
	else
	{
		for ( size_t i=0; i<DotMatchedPtCount; i++ )
		{	DrawDotMatched(hDC, m_DotMatchedPtList[i].x, m_DotMatchedPtList[i].y);	}
	}

	POINT TextPt={0,0};		
	COLORREF ColorNG=0x0000FF;
	COLORREF ColorOK=0x00FF00;
	COLORREF ColorText=0x4FFFFF;
	COLORREF OldTextColor = ::SetTextColor(hDC, ColorText);
	CString  strLine = AOIDataDefine.GetLineText();
	CString  strScore = AOIDataDefine.GetScoreText();
	CString  strScale = AOIDataDefine.GetScaleText();
	CString  strOffset = AOIDataDefine.GetOffsetText();
	CString  strVertical = AOIDataDefine.GetVerticalText();
	CString  strHorizontal = AOIDataDefine.GetHorizontalText();
	const int TextPitchY=m_ImageFontHeight+2;
	const double ScoreLSL=50.0;
	const double HorTol = GetHorTolerance();
	const double VerTol = GetVerTolerance();	

	TextPt.x += 4;
	if ( true == m_ShowCaliStep )
	{
		str = m_strCaliStep;
		::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;	 
	}

	if ( m_DotMatchedScore < ScoreLSL )
	{	::SetTextColor(hDC, ColorNG); }
	else
	{	::SetTextColor(hDC, ColorOK);	}
	str.Format(_T("%s(%.2f)"), strScore, m_DotMatchedScore);
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;	 

	::SetTextColor(hDC, ColorText);
	str.Format(_T("%s(%.2f, %.2f) um"), strOffset, m_DotMatchedOffset.x, m_DotMatchedOffset.y);
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;
	str.Format(_T("%s(%.2f, %.2f)"), strScale, m_DotMatchedScale.x, m_DotMatchedScale.y);
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;	

	if ( fabs(m_DotHorGapY) > HorTol )
	{	::SetTextColor(hDC, ColorNG); }
	else
	{	::SetTextColor(hDC, ColorOK); }
	str.Format(_T("%s %s %s(%.2f) um"), strHorizontal, strLine, strOffset, m_DotHorGapY);
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;	

	if ( fabs(m_DotVerGapX) > VerTol )
	{	::SetTextColor(hDC, ColorNG); }
	else
	{	::SetTextColor(hDC, ColorOK); }
	str.Format(_T("%s %s %s(%.2f) um"), strVertical, strLine, strOffset, m_DotVerGapX);
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());	TextPt.y += TextPitchY;	
	::SetTextColor(hDC, OldTextColor);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::DrawDotMap(HDC hDC)//繪製點結果	
{	
	COLORREF BKColor = m_BKColor;
	RECT  WndRect = m_ImageWndRect;
	HBRUSH hBrush = ::CreateSolidBrush(BKColor);
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &WndRect, hBrush);
		::DeleteObject(hBrush);
		hBrush = NULL;
	}

	int      i=0;
	COLORREF Color=0;
	COLORREF ColorNG=GetColorNG();
	COLORREF ColorOK=GetColorOK();
	COLORREF ColorSel=GetColorSel();
	COLORREF ColorNone=GetColorNone();	
	HPEN     hPen=NULL;
	HPEN     hPenOld=NULL;
	HPEN     hPenNG = ::CreatePen(PS_SOLID, 1, ColorNG);
	HPEN     hPenOK = ::CreatePen(PS_SOLID, 1, ColorOK);
	HPEN     hPenSel = ::CreatePen(PS_SOLID, 2, ColorSel);	
	HPEN     hPenNone = ::CreatePen(PS_SOLID, 1, ColorNone);	
	const int DotNodeCount = (int)(m_DotNodeList.size());

	hPenOld=(HPEN)(::SelectObject(hDC, hPenNG));
	for ( i=0; i<DotNodeCount; i++ )
	{
		TDotNode &DotNode = m_DotNodeList[i];
		
		if ( m_DotNodeIndex == i)
		{			
			hPen = hPenSel;
			Color = ColorSel; 			
		}
		else
		{
			switch ( DotNode.eResultID )
			{
			case RESULT_ID_NONE:
			case RESULT_ID_SKIP:
			case RESULT_ID_BYPASS:
				hPen = hPenNone;
				Color = ColorNone;	
				break;
			case RESULT_ID_OK:
				hPen = hPenOK;
				Color = ColorOK;	
				break;
			case RESULT_ID_NG:			
			case RESULT_ID_EXCEPTION:
				hPen = hPenNG;
				Color = ColorNG;
				break;
			}			
		}
		::SelectObject(hDC, hPen);

		switch ( DotNode.eShapeMode )
		{
		case BOX_SHAPE_ELLIPSE:
			ImageAPI.DrawEllipseLine(hDC, DotNode.rcDot);
			break;
		default:
			ImageAPI.DrawRectLine(hDC, DotNode.rcDot);
			break;
		}		
		//::FillRect(hDC, &(DotNode.rcDot), hBrush);
	}
	::SelectObject(hDC, hPenOld);
	::DeleteObject(hPenNG);	hPenNG = NULL;
	::DeleteObject(hPenOK);	hPenOK = NULL;
	::DeleteObject(hPenSel);	hPenSel = NULL;
	::DeleteObject(hPenNone);	hPenNone = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::StartReGrab(BOOL ReStart)//開始取下一個像
{
	//::Sleep(10);
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, ReStart, NULL);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecReGrab(WPARAM wParam)//取下一個像
{
	BOOL   bReGrabAll = (BOOL)(wParam);	
	if ( FALSE == bReGrabAll )
	{
		if ( this->ExecGrabNext() == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			return false;
		}
		return true;
	}	
	CAMERA_ID CameraID = GetCameraID();
	CALIBRATION_STAGE_MODE CalibrationMode = GetCalibrationMode();	
	switch ( CalibrationMode )
	{	
	case CALIBRATION_STAGE_STOP:
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
bool CCaliPaneStage::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
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
	if ( JetAPI::CheckPtInCtrlWnd(this, point, CALISTAGE_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
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
bool CCaliPaneStage::ExecGrabFirst()//執行第一次取像
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	DWORD DelayTime = GetMoveDownDelyTime();
	if ( DelayTime > 0 )
	{	Sleep(DelayTime); }

	CAMERA_ID CameraID = GetCameraID();
	std::vector<TSliceParam> ParamList;	
	ParamList.push_back(m_SliceParam);	
	if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}	
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
bool CCaliPaneStage::ExecGrabNext()//執行下一次取像
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	DWORD DelayTime = GetMoveDownDelyTime();
	if ( DelayTime > 0 )
	{	Sleep(DelayTime); }

	bool bFinish = false;	
	CAMERA_ID CameraID = GetCameraID();
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
void CCaliPaneStage::SetShowCaliStep(bool bShow)
{
	m_ShowCaliStep = bShow;
}
//-------------------------------------------------------------------------------------//
LANE_ID CCaliPaneStage::GetActiveLaneID() const
{
	return m_LaneID;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetActiveLaneID(LANE_ID LaneID)
{
	m_LaneID = LaneID;
}
//-------------------------------------------------------------------------------------//
CALIBRATION_STAGE_MODE CCaliPaneStage::GetCalibrationMode() const
{
	return m_CaliStageMode;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetCalibrationMode(CALIBRATION_STAGE_MODE Mode)
{
	m_CaliStageMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXYMoveTo(CALIBRATION_STAGE_MODE Mode, double PosX, double PosY)//2軸移動，但非同動唷	
{
	MOTION_MOVING_MODE MotionMovingMode=MOTION_MOVING_NORMAL;
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	if ( CALIBRATION_STAGE_CALIBRATE_DOT_NODE == Mode )
	{
		if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
		{	MotionMovingMode = MOTION_MOVING_GO_STOP;	}
	}
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, false, MotionMovingMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetLockUIAlign(bool bLock)//鎖住對齊
{
	m_LockUIAlign=bLock;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetLockUIAlignHor(bool bLock)//鎖住水平調整
{
	m_LockUIAlignHor=bLock;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetLockUIAlignVer(bool bLock)//鎖住垂直確認
{
	m_LockUIAlignVer=bLock;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetLockUIAlignCorner(bool bLock)//鎖住四角落確認
{
	m_LockUIAlignCorner=bLock;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetLockUIAlignDotNode(bool bLock)//鎖住玻璃點校正
{
	m_LockUIAlignDotNode=bLock;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::GetLockUIWnd() const
{
	return m_LockUIWnd;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::LockUIWnd(bool bLock)//鎖住視窗		
{
	UINT CtrlID=0;
	BOOL bEnable = FALSE;
	BOOL bEnable2 = FALSE;
	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }

	m_LockUIWnd = bLock;
	//Dot Param
	CtrlID = CALISTAGE_DOT_WHITE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_REVERSE_X_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_BUILD_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_SAVE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_SAVE_IMAGE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_MATCH_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_SIZE_W_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_SIZE_H_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_PITCH_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_PITCH_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_SKIP_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_SKIP_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_COUNT_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_COUNT_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CALISTAGE_DOT_MARGIN_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_MARGIN_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_EXTEND_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_EXTEND_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALIBRATION_SET_TOL_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CALISTAGE_DOT_MATCH_SCORE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);	
	CtrlID = CALISTAGE_THRESHOLD_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);		

	CtrlID = CALISTAGE_DOT_ERR_SET_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CALISTAGE_DOT_ERR_X_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_ERR_Y_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	//Other Setting
	CtrlID = CALISTAGE_SLICE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_AUTO_FOCUS_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_GRAB_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_MOTION_WND_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);		
	CtrlID = CALISTAGE_MOVE_DELAY_TIME_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_AUTO_FOCUS_PITCH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = CALISTAGE_DOT_LIST_WND;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	CtrlID = CALISTAGE_LANE_ID_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	

	//Function
	const bool bLockAlign = m_LockUIAlign;	
	if ( true == bLockAlign )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = CALISTAGE_DOT_ALIGN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALIBRATION_ALIGN_TOL_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	const bool bLockAlignHor = m_LockUIAlignHor;	
	if ( true == bLockAlignHor )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = CALISTAGE_ALIGN_X12_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_MOVE_TO_X1_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_MOVE_TO_X2_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_VERIFY_X_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALIBRATION_ALIGN_HOR_TOL_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	const bool bLockAlignVer = m_LockUIAlignVer;	
	if ( true == bLockAlignVer )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = CALISTAGE_ALIGN_Y12_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_MOVE_TO_Y1_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_MOVE_TO_Y2_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_VERIFY_Y_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALIBRATION_ALIGN_VER_TOL_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);

	const bool bLockAlignCorner = m_LockUIAlignCorner;	
	if ( true == bLockAlignCorner )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = CALISTAGE_ALIGN_CORNER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_ANALZE_CORNER_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);	
	CtrlID = CALISTAGE_VERIFY_CORNER_POS_BTN1;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_VERIFY_CORNER_POS_BTN2;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_VERIFY_CORNER_POS_BTN3;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	CtrlID = CALISTAGE_VERIFY_CORNER_POS_BTN4;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);

	const bool bLockAlignDotNode = m_LockUIAlignDotNode;	
	if ( true == bLockAlignDotNode )
	{	bEnable2 = FALSE; }
	else
	{	bEnable2 = bEnable; }
	CtrlID = CALISTAGE_CALIBRATE_DOT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);	
	CtrlID = CALISTAGE_BUILD_XYDOT_TABLE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);	
	CtrlID = CALISTAGE_SAVE_DOT_ROI_IMG_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable2);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetRepeatGrabBtnCheck(bool bCheck)//啟用重複取像
{
	CWnd::CheckDlgButton(CALISTAGE_GRAB_REPEAT_CHK, bCheck);
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::GetRepeatGrabBtnChecked()
{
	BOOL bRepeat = CWnd::IsDlgButtonChecked(CALISTAGE_GRAB_REPEAT_CHK);
	if ( FALSE ==bRepeat ) { return false;}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::RetrieveStagePosition(bool UpdatePos)//取得機台位置
{
	double PosX=0, PosY=0, PosZ=0;
	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ) == false )	
	{	return false; }	
	if ( true == UpdatePos )
	{
		m_StagePos.x = PosX;
		m_StagePos.y = PosY;
		m_StagePos.z = PosZ;
	}
	CString str;
	str.Format(_T("(%.0f, %.0f, %.0f)"), PosX, PosY, PosZ);
	CWnd::SetDlgItemText(CALISTAGE_MOTION_POS_EDIT, str);
	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像	
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
	//更新機台座標
	RetrieveStagePosition(false);
	
	bool              bFinish = false;
	CALIBRATION_STAGE_MODE  CalibrationMode = GetCalibrationMode();	
	switch ( CalibrationMode )
	{
	case CALIBRATION_STAGE_DOT_ALIGN:
		if ( ExecOrgDotAlign(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }			
		break;
	case CALIBRATION_STAGE_DOT_CAMERA:
		if ( ExecDotCameraAlign(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }			
		break;
	case CALIBRATION_STAGE_X_POS_ALIGN:		
	case CALIBRATION_STAGE_X_POS_ALIGN_1:
		if ( ExecXAlignHorLineX1(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_X_POS_ALIGN_2:
		if ( ExecXAlignHorLineX2(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_X_POS_VERIFY_1:
		if ( ExecXVerifyX1(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }	
		break;
	case CALIBRATION_STAGE_X_POS_VERIFY_2:
		if ( ExecXVerifyX2(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;	
	case CALIBRATION_STAGE_X_POS_VERIFY_EACH:
		if ( ExecXVerifyXEach(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }		
		break;	
	case CALIBRATION_STAGE_Y_POS_ALIGN:
	case CALIBRATION_STAGE_Y_POS_ALIGN_1:
		if ( ExecYAlignVerLineY1(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_Y_POS_ALIGN_2:
		if ( ExecYAlignVerLineY2(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_Y_POS_VERIFY_1:
		if ( ExecYVerifyY1(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }	
		break;
	case CALIBRATION_STAGE_Y_POS_VERIFY_2:		
		if ( ExecYVerifyY2(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }	
		break;		
	case CALIBRATION_STAGE_Y_POS_VERIFY_EACH:
		if ( ExecYVerifyYEach(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }		
		break;
	case CALIBRATION_STAGE_ALIGN_CORNER:
	case CALIBRATION_STAGE_ALIGN_CORNER_1:
		if ( ExecAlignCornerPos1(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_ALIGN_CORNER_2:
		if ( ExecAlignCornerPos2(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_ALIGN_CORNER_3:
		if ( ExecAlignCornerPos3(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
		{	return true; }
		break;
	case CALIBRATION_STAGE_ALIGN_CORNER_4:
		if ( ExecAlignCornerPos4(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}				
		if ( bFinish == false )
		{	return true; }
		if ( true == GetAlignedMapUsed() )	
		{
			AnalyzeAlignedMap();	
			ExecCalibrateDotBtn();		
			return true;
		}
		break;
	case CALIBRATION_STAGE_CALIBRATE_DOT_NODE:
		if ( ExecCalibrateDotNode(CalibrationMode, CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, bFinish) == false )
		{
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			LockUIWnd(false);
			return false;
		}		
		if ( bFinish == false )
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
		DrawImageWndMemDC();
		RedrawWnd();	
		break;
	}

	CWnd *pWnd = NULL;
	UINT  CtrlID = 0;
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( true == bRepeat )
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
void CCaliPaneStage::ClearLogListBox()//清除紀錄列表視窗
{
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::AddLogListBox(LPCTSTR str)//加入紀錄列表視窗
{
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetInfoText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(CALISTAGE_INFO_EDIT, Text);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnGrabBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->ConfigGrabParam(CALIBRATION_STAGE_STOP) == false )
	{	return; }
	SetCalibrationMode(CALIBRATION_STAGE_STOP);	
	ExecGrabFirst();
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ConfigGrabParam(CALIBRATION_STAGE_MODE CaliMode)//組態取像參數
{	
	CString str;	
	unsigned int i=0, j=0;
	bool bGrabRepeat=false;	
	double FOVW = 0, FOVH = 0;
	double ResX = 0, ResY = 0;
	LANE_ID  LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneIDCombox));
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CAMERA_ID CameraID = GetCameraID();
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	const IMAGE_SIZE ImageWHalf = ImageW/2;
	const IMAGE_SIZE ImageHHalf = ImageH/2;
	
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombox);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);		
	if ( NULL != SliceParamPtr)
	{	m_SliceParam = *SliceParamPtr;	}
	else
	{	m_SliceParam = TSliceParam(); }
	
	SetActiveLaneID(LaneID);
	RetrieveStagePosition(true);	
	//Camera Exposure
	switch ( CaliMode )
	{
	case CALIBRATION_STAGE_DOT_ALIGN:		
		bGrabRepeat=true;
		m_DotHorGapY = 0.0;
		m_DotVerGapX = 0.0;
		m_CaliRepeatCount = 0;		
		SetDotOrgStagePos(m_StagePos.x, m_StagePos.y);		
		SetRepeatGrabBtnCheck(true);		
		break;
	case CALIBRATION_STAGE_DOT_CAMERA:
		bGrabRepeat=true;
		m_CaliRepeatCount = 0;
		UpdateDotHorVerPos();//計算點的水平與垂直位置
		break;
	case CALIBRATION_STAGE_X_POS_ALIGN:
		bGrabRepeat=true;
		m_CaliRepeatCount = 0;
		UpdateDotHorVerPos();//計算點的水平與垂直位置				
		break;
	case CALIBRATION_STAGE_X_POS_VERIFY_1://驗證第1點		
		bGrabRepeat=true;
		break;
	case CALIBRATION_STAGE_X_POS_VERIFY_2://驗證第2點		
		bGrabRepeat=true;
		break;
	case CALIBRATION_STAGE_X_POS_VERIFY_EACH:		
		bGrabRepeat=true;
		m_DotHorLineList.clear();
		m_CaliRepeatCount = 0;		
		UpdateDotHorVerPos();//計算點的水平與垂直位置
		break;
	case CALIBRATION_STAGE_Y_POS_ALIGN:
		bGrabRepeat=true;
		m_CaliRepeatCount = 0;
		UpdateDotHorVerPos();//計算點的水平與垂直位置				
		break;
	case CALIBRATION_STAGE_Y_POS_VERIFY_1:
		bGrabRepeat=true;
		break;
	case CALIBRATION_STAGE_Y_POS_VERIFY_2:
		bGrabRepeat=true;
		break;	
	case CALIBRATION_STAGE_Y_POS_VERIFY_EACH:
		bGrabRepeat=true;
		m_DotVerLineList.clear();
		m_CaliRepeatCount = 0;		
		UpdateDotHorVerPos();//計算點的水平與垂直位置
		break;
	case CALIBRATION_STAGE_ALIGN_CORNER:
		bGrabRepeat=true;
		m_CaliRepeatCount = 0;		
		UpdateDotHorVerPos();//計算點的水平與垂直位置
		break;
	case CALIBRATION_STAGE_CALIBRATE_DOT_NODE:
		bGrabRepeat=true;
		m_DotNodeIndex=0;
		m_CaliRepeatCount = 0;		
		if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
		{	m_DotNodeCanBeXYTable = false;	}
		else
		{	m_DotNodeCanBeXYTable = true;	}
		BuildDotNodeList();		
		break;
	}
	SetInfoText(_T(""));
	if ( true == bGrabRepeat )
	{	SetRepeatGrabBtnCheck(true);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::CreateTempFolder()//建立暫存資料夾
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
int CCaliPaneStage::GetDotCountX() const
{
	return CalcDotCount(m_DotSkipX, m_DotCountX);	
}
//-------------------------------------------------------------------------------------//
int CCaliPaneStage::GetDotCountY() const
{
	return CalcDotCount(m_DotSkipY, m_DotCountY);	
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotPitchX() const
{
	const int DotPeriod=GetDotPeriod(m_DotSkipX);
	const double Pitch = m_DotPitchX*DotPeriod;
	return Pitch;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotPitchY() const
{
	const int DotPeriod=GetDotPeriod(m_DotSkipY);
	const double Pitch = m_DotPitchY*DotPeriod;
	return Pitch;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::GetDotRoiSize(int &W, int &H)
{
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int ExtendX = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT);
	const int ExtendY = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT);	
	const int RoiW = (PatW+ExtendX+ExtendX);
	const int RoiH = (PatH+ExtendY+ExtendY);
	W = RoiW;
	H = RoiH;
	return true;
}
//-------------------------------------------------------------------------------------//
int CCaliPaneStage::GetDotPeriod(int DotSkip) const
{
	if ( DotSkip <= 0 ) { return 1; }
	return (DotSkip+1);
}
//-------------------------------------------------------------------------------------//
int CCaliPaneStage::CalcDotCount(int DotSkip, int DotCount) const
{
	const int DotPeriod=GetDotPeriod(DotSkip);
	const int Count = DotCount/DotPeriod;
	const int CountAll = Count*DotPeriod;
	if ( CountAll < DotCount )
	{	return (Count+1); }
	return Count;
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneStage::GetDebugFolder() const
{
	CString str;
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DotNodeImage"));
	return str;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::ResetDotSearch()//重置點搜尋
{
	m_DotMatched = false;
	m_DotMatchedPt.x = m_ImageW/2;
	m_DotMatchedPt.y = m_ImageH/2;
	m_DotMatchedSz.cx = m_DotSizeW;
	m_DotMatchedSz.cy = m_DotSizeH;			
	m_DotMatchedScale.x = 100.0;
	m_DotMatchedScale.y = 100.0;
	m_DotMatchedOffset.x = 0;
	m_DotMatchedOffset.y = 0;
	m_DotMatchedScore = 0;
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSearch()//執行點搜尋
{
	double DotX = m_ImageW*0.5;
	double DotY = m_ImageH*0.5;
	if ( ExecDotSearch(DotX, DotY) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSearch(double DotX, double DotY)//執行點搜尋
{
	bool IsOK = true;	
	DOT_ALIGN_MODE AlignMode = m_DotAlignMode;
	switch ( AlignMode )
	{
	case DOT_ALIGN_BLOB:
		IsOK = ExecDotSearch_Blob(DotX, DotY);
		break;
	case DOT_ALIGN_MATCH:
		IsOK = ExecDotSearch_Match(DotX, DotY);
		break;
	case DOT_ALIGN_HOUGH_CIRCLE:
		IsOK = ExecDotSearch_HoughCircle(DotX, DotY);
		break;
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSearch_Blob(double DotX, double DotY)//執行點區塊搜尋
{
	CString str;
	ResetDotSearch();
	const char fnName[] = "CCaliPaneStage::ExecDotSearch_Blob";	
	IMAGE_PTR ImagePtr = m_ImageBuffer;
	const int  nAlign = 4;
	const int ImageW = (int)(m_ImageW);
	const int ImageH = (int)(m_ImageH);	
	const int ImageCpX=ImageW/2;
	const int ImageCpY=ImageH/2;
	int BitCount = (int)(m_BitCount);
	CAMERA_ID  CameraID=GetCameraID();
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	
	//Dot Pattern	
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int PatStep = (int)(m_DotImageStep);
	const int PatBitCount=(int)(m_DotImageBitCount);
	unsigned char *PatPtr = m_DotImagePtr;

	//Roi		
	RECT RoiRect={0,0,0,0};
	const int ExtendX = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT);
	const int ExtendY = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT);
	unsigned char *RoiPtr=NULL;		
	const int RoiW = (PatW+ExtendX+ExtendX);
	const int RoiH = (PatH+ExtendY+ExtendY);
	const int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, 8, nAlign);	
	const size_t RoiSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	if ( JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false )
	{
		JetMemory.free_func(RoiPtr);		
		SetInfoText(_T("Error, Create Roi Memory Fault"));		
		return false; 
	}

	const int RoiX = (int)(DotX);
	const int RoiY = (int)(DotY);
	const int RoiLX = RoiX-(RoiW/2);
	const int RoiTY = RoiY-(RoiH/2);
	//const int RoiLX = ImageCpX-(RoiW/2);
	//const int RoiTY = ImageCpY-(RoiH/2);
	RoiRect.left = RoiLX;
	RoiRect.top  = RoiTY;
	RoiRect.right = RoiRect.left+RoiW;
	RoiRect.bottom = RoiRect.top+RoiH;

	// Convert it to gray
	//cvtColor( src, src_gray, CV_BGR2GRAY );
	if ( 24 == BitCount )
	{
		int WR=100, WG=100, WB=100;
		if ( ImageAPI.ColorImageToGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_ImageBuffer, RoiRect, RoiStep, RoiPtr, IMAGE_SRC_GRAY, WR, WG, WB, false) == false )
		{	
			JetMemory.free_func(RoiPtr);
			SetInfoText(_T("Error, Extract Roi Image Fault"));		
			return false; 
		}
	}
	else
	{
		if ( ImageAPI.ExtractRoiImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, RoiRect, RoiStep, RoiPtr, false) == false )
		{
			JetMemory.free_func(RoiPtr);			
			SetInfoText(_T("Error, Extract Roi Image Fault"));		
			return false;
		}
	}
	BitCount = 8;

	const int Threshold=GetThreshold();
	JetAPI::SizeToRect(RoiW, RoiH, RoiRect);	
#ifdef _DEBUG
	str.Format(_T("%s\\DotBlobGray.PNG"), AOIDataCollect.GetAOITempDirectory());
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
#endif//_DEBUG

	if ( false == m_DotWhiteMode )
	{	ImageAPI.BinaryGrayImage3(RoiW, RoiH, RoiStep, RoiPtr, RoiRect, RoiStep, RoiPtr, 0, Threshold);	}
	else
	{	ImageAPI.BinaryGrayImage3(RoiW, RoiH, RoiStep, RoiPtr, RoiRect, RoiStep, RoiPtr, Threshold, 255);	}	
#ifdef _DEBUG
	str.Format(_T("%s\\DotBlobBinary.PNG"), AOIDataCollect.GetAOITempDirectory());
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
#endif//_DEBUG

	//區塊分析
	RECT     BlobRoiRect;
	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
	JetAPI::SizeToRect(RoiW, RoiH, BlobRoiRect);	
	if ( BlobDetector.GrayImageRoiBlobDetect(RoiW, RoiH, RoiStep, RoiPtr, BlobRoiRect, 128, 255) == false )
	{
		JetMemory.free_func(RoiPtr);
		SetInfoText(_T("Error, Blob Detect"));				
		return false; 
	}
	ExecDotSaveRoiImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr);
	JetMemory.free_func(RoiPtr);

	size_t       i=0;	
	RECT         BlobRect={0,0,0,0};
	double       BlobCGX=0.0, BlobCGY=0.0;
	int          BlobW=0, BlobH=0, BlobArea=0;
	TBlobResult *BlobPtr=NULL;	
	int          DotArea=0;
	const int    DotSizeW=m_DotSizeW;
	const int    DotSizeH=m_DotSizeH;
	BOX_SHAPE_MODE DotShapeMode = GetDotShapeMode();	
	const size_t BlobResCount = BlobDetector.GetBlobCount();
	std::vector<TBlobResult> BlobList(BlobResCount);		
	
	if ( BOX_SHAPE_ELLIPSE == DotShapeMode  )
	{	DotArea = (int)(3.14159*(DotSizeW/2)*(DotSizeH/2));	}
	else
	{	DotArea = DotSizeW*DotSizeH;	}
	BlobList.clear();
	for ( i=0; i<BlobResCount; i++ )
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if ( NULL == BlobPtr ) { continue; }		
		BlobRect = BlobPtr->m_BlobRect;
		BlobCGX = BlobPtr->m_BlobGCPosX;
		BlobCGY = BlobPtr->m_BlobGCPosY;
		BlobArea = BlobPtr->m_BlobPixels;
		BlobW = BlobRect.right-BlobRect.left;
		BlobH = BlobRect.bottom-BlobRect.top;
		break;
	}	
	JetMemory.free_func(RoiPtr);
	const double AreaErrRatio = labs(BlobArea-DotArea)*100.0/DotArea;

	m_DotMatchedPt.x = BlobCGX+RoiLX;
	m_DotMatchedPt.y = BlobCGY+RoiTY;
	m_DotMatchedSz.cx = BlobW;
	m_DotMatchedSz.cy = BlobH;
	m_DotMatchedScale.x = 100.0*BlobW/m_DotSizeW;
	m_DotMatchedScale.y = 100.0*BlobH/m_DotSizeH;
	m_DotMatchedScore = 100.0-AreaErrRatio;
	m_DotMatched = true;
	m_ShowDotMatched = true;	
	
	TPOINT2D ImaggeRes;
	TPOINT2D CadOffset;	
	TPOINT2D ImageOffset;
	TPOINT2D StageOffset;
	ImaggeRes.x = ResX;
	ImaggeRes.y = ResY;
	ImageOffset.x = (BlobCGX-(RoiW*0.5));
	ImageOffset.y = (BlobCGY-(RoiH*0.5));
	AOIDataCollect.MapImageOffsetToCad(ImageOffset, ImaggeRes, CadOffset);
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	m_DotMatchedOffset.x = StageOffset.x;
	m_DotMatchedOffset.y = StageOffset.y;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSearch_Match(double DotX, double DotY)//執行點匹配
{
	CString str;
	ResetDotSearch();
	const char fnName[] = "CCaliPaneStage::ExecDotSearch_Match";
	if ( NULL == m_DotImagePtr ) 
	{	return false; }
	if ( NULL == m_ImageBuffer ) 
	{	return false; }

	CJetMatch Match;
	const int  nAlign = 4;
	const bool bRobustness = true;
	const int  nMinReduceArea = 256;
	CAMERA_ID  CameraID=GetCameraID();
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const int    nMinScore = (int)(CWnd::GetDlgItemInt(CALISTAGE_DOT_MATCH_SCORE_EDIT));
	const float  fMinScore = nMinScore*0.01f;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();	
	
	//Dot Pattern
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int PatStep = (int)(m_DotImageStep);
	const int PatBitCount=(int)(m_DotImageBitCount);
	unsigned char *PatPtr = m_DotImagePtr;

	const int ImageW = (int)(m_ImageW);
	const int ImageH = (int)(m_ImageH);
	const int ImageCpX=ImageW/2;
	const int ImageCpY=ImageH/2;
	const int BitCount = (int)(m_BitCount);
	if (  BitCount != PatBitCount )
	{
		SetInfoText(_T("Error, Dot Image BitCount Exception"));
		return false; 
	}

	//Roi		
	RECT RoiRect={0,0,0,0};
	const int ExtendX = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT);
	const int ExtendY = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT);
	unsigned char *RoiPtr=NULL;	
	const int RoiW = (PatW+ExtendX+ExtendX);
	const int RoiH = (PatH+ExtendY+ExtendY);
	const int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	const size_t RoiSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	if ( JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false )
	{
		SetInfoText(_T("Error, Create Roi Memory Fault"));
		return false; 
	}

	const int RoiX = (int)(DotX);
	const int RoiY = (int)(DotY);
	const int RoiLX = RoiX-(RoiW/2);
	const int RoiTY = RoiY-(RoiH/2);
	//const int RoiLX = ImageCpX-(RoiW/2);
	//const int RoiTY = ImageCpY-(RoiH/2);
	RoiRect.left = RoiLX;
	RoiRect.top  = RoiTY;
	RoiRect.right = RoiRect.left+RoiW;
	RoiRect.bottom = RoiRect.top+RoiH;
	if ( ImageAPI.ExtractRoiImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, RoiRect, RoiStep, RoiPtr, false) == false )
	{
		SetInfoText(_T("Error, Extract Roi Image Fault"));
		JetMemory.free_func(RoiPtr);
		return false;
	}	

#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DotPat.PNG"));
	ImageAPI.SaveImage(str, PatW, PatH, PatStep, PatBitCount, PatPtr, true);

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DotRoi.PNG"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);

	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DotFov.PNG"));
	//ImageAPI.SaveImage(str, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, true);
#endif//_DEBUG

	//initial eMatch	
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{
		str = Match.GetErrorString();
		SetInfoText(str);
		JetMemory.free_func(RoiPtr);		
		return false;
	}
	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);	
	Match.SetUseDontCareArea(false);
	Match.SetDontCareThreshold(0); 		
	if ( Match.LearnPattern(PatW, PatH, PatStep, PatBitCount, PatPtr, true) == false )
	{
		SetInfoText(_T("Error, Learn Dot Image Fault"));
		JetMemory.free_func(RoiPtr);
		return false; 
	}
	
	const bool UseScale = false;
	const float Scale=0.1f;
	if ( true == UseScale )
	{
		Match.SetMinScale(1.0f-Scale);
		Match.SetMaxScale(1.0f+Scale);
		Match.SetUseScale(true);	
	}

	Match.SetInterpolate(true);
	Match.SetMinScore(fMinScore);//fMinScore
	Match.SetMaxPositions(1);
	if ( Match.Match(RoiW, RoiH, RoiStep, BitCount, RoiPtr, true) == false )
	{
		SetInfoText(_T("Error, Match Dot Image Fault"));
		JetMemory.free_func(RoiPtr);
		return false; 
	}
	ExecDotSaveRoiImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr);
	JetMemory.free_func(RoiPtr);

	const int NResults = Match.GetNumPositions();	
	if ( 0 == NResults )
	{
		SetInfoText(_T("Error, Match Dot Image Fault (n=0)"));
		return false; 
	}

	const int idx = 0;
	double ResultX = Match.GetResultPosX(idx);
	double ResultY = Match.GetResultPosY(idx);
	double ResultA = Match.GetResultAngle(idx);
	double ResultS = Match.GetResultScore(idx)*100.0;
	double ResultSX = Match.GetResultScaleX(idx);
	double ResultSY = Match.GetResultScaleY(idx);

	double ResultCX = ResultX+RoiLX;
	double ResultCY = ResultY+RoiTY;
	/*
	double RoiOffsetX = ResultCX-dRoiCpx;
	double RoiOffsetY = dRoiCpy-ResultCY;
	double CadSkew = -ResultA;
	double CadOffsetX = RoiOffsetX/ImageScale.x;
	double CadOffsetY = RoiOffsetY/ImageScale.y;

	double CadScaleX = ResultSX*100.0;
	double CadScaleY = ResultSY*100.0;		
	if ( ResultS < 0.0 ) { ResultS = 0.0; }
	*/	
	m_DotMatchedPt.x = ResultCX;
	m_DotMatchedPt.y = ResultCY;
	m_DotMatchedSz.cx = m_DotSizeW*ResultSX;
	m_DotMatchedSz.cy = m_DotSizeH*ResultSY;			
	m_DotMatchedScale.x = ResultSX*100.0;
	m_DotMatchedScale.y = ResultSY*100.0;
	m_DotMatchedScore = ResultS;
	m_DotMatched = true;
	m_ShowDotMatched = true;	
	
	TPOINT2D ImaggeRes;
	TPOINT2D CadOffset;	
	TPOINT2D ImageOffset;
	TPOINT2D StageOffset;
	ImaggeRes.x = ResX;
	ImaggeRes.y = ResY;
	ImageOffset.x = (ResultX-(RoiW*0.5));
	ImageOffset.y = (ResultY-(RoiH*0.5));
	AOIDataCollect.MapImageOffsetToCad(ImageOffset, ImaggeRes, CadOffset);
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	m_DotMatchedOffset.x = StageOffset.x;
	m_DotMatchedOffset.y = StageOffset.y;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSearch_HoughCircle(double DotX, double DotY)//執行點搜尋霍夫圓形
{	
#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
	return false;
#else
	CString str;
	ResetDotSearch();
	const char fnName[] = "CCaliPaneStage::ExecDotSearch_HoughCircle";
	IMAGE_PTR GrayPtr=NULL;
	IMAGE_PTR ImagePtr = m_ImageBuffer;
	const int  nAlign = 4;
	const int ImageW = (int)(m_ImageW);
	const int ImageH = (int)(m_ImageH);	
	const int ImageCpX=ImageW/2;
	const int ImageCpY=ImageH/2;
	CAMERA_ID  CameraID=GetCameraID();
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	int BitCount = (int)(m_BitCount);
	BOX_SHAPE_MODE BoxShapeMode = GetDotShapeMode();	
	if ( BOX_SHAPE_ELLIPSE != BoxShapeMode )	
	{
		SetInfoText(_T("Error, Circle Mode Only"));		
		return false;
	}
	
	//Dot Pattern	
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int PatStep = (int)(m_DotImageStep);
	const int PatBitCount=(int)(m_DotImageBitCount);
	unsigned char *PatPtr = m_DotImagePtr;

	//Roi		
	RECT RoiRect={0,0,0,0};
	const int ExtendX = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT);
	const int ExtendY = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT);
	unsigned char *RoiPtr=NULL;		
	const int RoiW = (PatW+ExtendX+ExtendX);
	const int RoiH = (PatH+ExtendY+ExtendY);
	const int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, 8, nAlign);	
	const size_t RoiSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	if ( JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false ||
		 JetMemory.alloc_func(RoiSize, GrayPtr, fnName, "GrayPtr") == false  )
	{
		JetMemory.free_func(RoiPtr);		
		SetInfoText(_T("Error, Create Roi Memory Fault"));		
		return false; 
	}

	const int RoiX = (int)(DotX);
	const int RoiY = (int)(DotY);
	const int RoiLX = RoiX-(RoiW/2);
	const int RoiTY = RoiY-(RoiH/2);
	//const int RoiLX = ImageCpX-(RoiW/2);
	//const int RoiTY = ImageCpY-(RoiH/2);
	RoiRect.left = RoiLX;
	RoiRect.top  = RoiTY;
	RoiRect.right = RoiRect.left+RoiW;
	RoiRect.bottom = RoiRect.top+RoiH;

	// Convert it to gray
	//cvtColor( src, src_gray, CV_BGR2GRAY );
	if ( 24 == BitCount )
	{
		int WR=100, WG=100, WB=100;
		if ( ImageAPI.ColorImageToGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_ImageBuffer, RoiRect, RoiStep, RoiPtr, IMAGE_SRC_GRAY, WR, WG, WB, false) == false )
		{	
			JetMemory.free_func(RoiPtr);		
			SetInfoText(_T("Error, Extract Roi Image Fault"));		
			return false; 
		}
	}
	else
	{
		if ( ImageAPI.ExtractRoiImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, RoiRect, RoiStep, RoiPtr, false) == false )
		{
			JetMemory.free_func(RoiPtr);			
			SetInfoText(_T("Error, Extract Roi Image Fault"));		
			return false;
		}
	}
	BitCount = 8;
	if ( false == m_DotWhiteMode )
	{	ImageAPI.InvertGrayImage3(RoiW, RoiH, RoiStep, RoiPtr, RoiPtr);	}
	
	str.Format(_T("%s\\DotHoughCircleRaw.PNG"), AOIDataCollect.GetAOITempDirectory());
	//if ( ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true) == false )
	//{
	//	JetMemory.free_func(RoiPtr);			
	//	SetInfoText(_T("Error, Save Temp File Fault"));		
	//	return false;
	//}

	
	CvMat  *cvMat=NULL;		
	TIMAGE tmpImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr);	
	cvMat = ImageAPI.ImageToCVMat(tmpImage, false, fnName);
	if ( NULL == cvMat )
	{
		JetMemory.free_func(RoiPtr);			
		SetInfoText(_T("Error, Image to cvMat Fault"));		
		return false;
	}
	ExecDotSaveRoiImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr);
	JetMemory.free_func(RoiPtr);
	

	//cv::Mat src, src_gray;	
	std::string folder;
	std::string filename;		
	cv::vector<cv::Vec3f> circles(1000);
	cv::Mat src_gray(cvMat, false);//clone one
	CString TempFolder = AOIDataCollect.GetAOITempDirectory();
	//::cvReleaseMat(&cvMat);	

	JetAPI::TCHAR2string(TempFolder, folder);
	filename = folder+"\\DotHoughCircleRaw.PNG";
	//src = cv::imread(filename);
#ifdef _DEBUG
	filename = folder+"\\DotHoughCircle.PNG";
	//cv::imwrite(filename, src);
#endif//_DEBUG

	// Convert it to gray
	//cv::cvtColor(src, src_gray, CV_BGR2GRAY );

	// Reduce the noise so we avoid false circle detection
	//cv::GaussianBlur(src_gray, src_gray, cv::Size(9, 9), 2, 2 );
#ifdef _DEBUG
	filename = folder+"\\DotHoughCircleBlur.PNG";
	cv::imwrite(filename, src_gray);
#endif//_DEBUG

	const int    DotSizeW=m_DotSizeW;
	const int    DotSizeH=m_DotSizeH;
	const int    RadiusStd=(DotSizeW+DotSizeH)/4;
	const int    RadiusMin=(int)(RadiusStd*0.8);
	const int    RadiusMax=(int)(RadiusStd*1.2);
	const int    RadiusMax2=(int)(RadiusStd*1.5);
	// Apply the Hough Transform to find the circles
	//CV_EXPORTS_W void HoughCircles( InputArray image, OutputArray circles, int method, double dp, double minDist, double param1=100, double param2=100, int minRadius=0, int maxRadius=0 );
	try
	{
		cv::HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, RadiusMin, 50, 10, RadiusMin, RadiusMax);
		::cvReleaseMat(&cvMat);	
	}
	catch (cv::Exception &e)
	{
		::cvReleaseMat(&cvMat);	
		const char *msg = e.what();
		str = msg;
		SetInfoText(str);				
		return false;
	}

	// Draw the circles detected
	size_t i=0;
	float     radius=0;
	cv::Point2f center;
	const size_t CircleCount=circles.size();
	if ( 0 == CircleCount )
	{
		SetInfoText(_T("Error, Can not find circles"));
		return false;
	}
	//找最接近半徑的點
	bool   bFirst=true;
	double Error=0.0;
	double MatchErr = 0.0;
	size_t MatchID = 0;
	for( i = 0; i<CircleCount; i++ )
	{
		center.x = (circles[i][0]);
		center.y = (circles[i][1]);
		radius = (circles[i][2]);

		Error = fabs(radius-RadiusStd);
		if ( true == bFirst  )
		{
			bFirst = false;
			MatchID = i;
			MatchErr = Error;	
		}
		else if ( Error < MatchErr )
		{
			MatchID = i;
			MatchErr = Error;	
		}		
		
   }
	
	if ( MatchID < CircleCount )
	{
		i = MatchID;
		center.x = (circles[i][0]);
		center.y = (circles[i][1]);
		radius = (circles[i][2]);
	}

	m_DotMatchedPt.x = center.x+RoiLX;
	m_DotMatchedPt.y = center.y+RoiTY;
	m_DotMatchedSz.cx = 2*radius;//m_DotSizeW;
	m_DotMatchedSz.cy = 2*radius;//m_DotSizeH;
	m_DotMatchedScale.x = 200.0*radius/m_DotSizeW;
	m_DotMatchedScale.y = 200.0*radius/m_DotSizeH;
	m_DotMatchedScore = 100;
	m_DotMatched = true;
	m_ShowDotMatched = true;	
	
	TPOINT2D ImaggeRes;
	TPOINT2D CadOffset;	
	TPOINT2D ImageOffset;
	TPOINT2D StageOffset;
	ImaggeRes.x = ResX;
	ImaggeRes.y = ResY;
	ImageOffset.x = (center.x-(RoiW*0.5));
	ImageOffset.y = (center.y-(RoiH*0.5));
	AOIDataCollect.MapImageOffsetToCad(ImageOffset, ImaggeRes, CadOffset);
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	m_DotMatchedOffset.x = StageOffset.x;
	m_DotMatchedOffset.y = StageOffset.y;	
#endif//OPEN_CV_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SavePatImage()
{	
	BOOL bSave=CWnd::IsDlgButtonChecked(CALISTAGE_SAVE_DOT_ROI_IMG_CHK);
	if ( FALSE == bSave ) { return true; }

	CString str;
	CString strFolder = GetDebugFolder();
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int PatStep = (int)(m_DotImageStep);
	const int PatBitCount=(int)(m_DotImageBitCount);
	unsigned char *PatPtr = m_DotImagePtr;
	if ( NULL == PatPtr ) { return true; }
	::CreateDirectory(strFolder, NULL);
	JetAPI::ClearFolder(strFolder);
	str.Format(_T("%s\\DotPat.PNG"), strFolder);
	ImageAPI.SaveImage(str, PatW, PatH, PatStep, PatBitCount, PatPtr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotSaveRoiImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)//執行儲存區域影像
{
	if ( NULL == Ptr ) { return true; }
	CALIBRATION_STAGE_MODE CaliMode = GetCalibrationMode();	
	if ( CALIBRATION_STAGE_CALIBRATE_DOT_NODE != CaliMode ) { return false; }	
	BOOL bSave=CWnd::IsDlgButtonChecked(CALISTAGE_SAVE_DOT_ROI_IMG_CHK);
	if ( FALSE == bSave ) { return true; }
	const int Index = m_DotNodeIndex;	
	const int Count = (int)(m_DotNodeList.size());	
	if ( Index<0 || Index>=Count ) { return true; }
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	if ( FN_DISABLE == MotionParam.m_XYCaliEnable ) { return true; }

	CString str;
	CString strFolder = GetDebugFolder();
	const TDotNode &DotNode=m_DotNodeList[Index];
	const int IndexX=DotNode.nIndexX;
	const int IndexY=DotNode.nIndexY;	
	::CreateDirectory(strFolder, NULL);
	str.Format(_T("%s\\DotRoi_Y%02d_X%02d.PNG"), strFolder, IndexY+1, IndexX+1);
	ImageAPI.SaveImage(str, W, H, Step, BitCount, Ptr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecOtherDotSearch()//執行其他點搜尋
{	return true;
	const int NodeIndex = m_DotNodeIndex;
	const int Count = (int)(m_DotNodeList.size());	
	if ( NodeIndex >= Count ) { return true; }
	const TDotNode &DotNodeRef = m_DotNodeList[NodeIndex];

	TPOINT2D StageCp;
	TPOINT2D ImagePos;	
	TPOINT2D StagePos;	
	TREGION4D FovRegion;
	CAMERA_ID CameraID = GetCameraID();	
	const int ImageW = (int)(m_ImageW);
	const int ImageH = (int)(m_ImageH);		
	const int PatW = (int)(m_DotImageW);
	const int PatH = (int)(m_DotImageH);
	const int ExtendX = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT);
	const int ExtendY = CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT);	
	const int RoiW = (PatW+ExtendX+ExtendX);
	const int RoiH = (PatH+ExtendY+ExtendY);	
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);	
	MotionCtrlPtr->GetCurrentPos(StageCp.x, StageCp.y);
	AOIDataCollect.GetFovStageRegionReal(FovRegion);//取得視野實際範圍

	std::vector<TDotNode> OtherList;
	for ( size_t i=0; i<Count; i++ )
	{		
		const TDotNode &rDotNode = m_DotNodeList[i];
		if ( NodeIndex == i )
		{
			OtherList.push_back(rDotNode);
			continue; 
		}

		StagePos.x = rDotNode.dAlignedX;
		StagePos.y = rDotNode.dAlignedY;
		const int DotIndexX=rDotNode.nIndexX;
		const int DotIndexY=rDotNode.nIndexY;
		
		if ( rDotNode.dAlignedX < FovRegion.minX ) { continue; }
		if ( rDotNode.dAlignedY < FovRegion.minY ) { continue; }
		if ( rDotNode.dAlignedX > FovRegion.maxX ) { continue; }
		if ( rDotNode.dAlignedY > FovRegion.maxY ) { continue; }		
		AOIDataCollect.MapStagePtToCamera(CameraID, StagePos, StageCp, ImagePos);

		RECT RoiRect;
		const int RoiX=(int)(ImagePos.x);
		const int RoiY=(int)(ImagePos.y);
		const int RoiLX = RoiX-(RoiW/2);
		const int RoiTY = RoiY-(RoiH/2);
		RoiRect.left = RoiLX;
		RoiRect.top  = RoiTY;
		RoiRect.right = RoiRect.left+RoiW;
		RoiRect.bottom = RoiRect.top+RoiH;
		if ( RoiRect.left   < 0 ) { continue; }
		if ( RoiRect.top    < 0 ) { continue; }
		if ( RoiRect.right  > ImageW ) { continue; }
		if ( RoiRect.bottom > ImageH ) { continue; }		
		if ( ExecDotSearch(ImagePos.x, ImagePos.y) == false )
		{	continue; }

		TDotNode DotNode=rDotNode;
		const double StageOffsetX = GetDotMatchedOffsetX();
		const double StageOffsetY = GetDotMatchedOffsetY();	

		DotNode.dCaliPosX = DotNode.dAlignedX+StageOffsetX;
		DotNode.dCaliPosY = DotNode.dAlignedY+StageOffsetY;
		DotNode.dOffsetX = DotNode.dCaliPosX-DotNode.dAlignedX;
		DotNode.dOffsetY = DotNode.dCaliPosY-DotNode.dAlignedY;
		OtherList.push_back(DotNode);
	}

	CString filename;	
	CString strFolder;	
	strFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("OtherDot"));
	::CreateDirectory(strFolder, NULL);
	filename.Format(_T("%s\\%03d_%03d_OtherDot.TXT"), strFolder, DotNodeRef.nIndexX+1, DotNodeRef.nIndexY+1);
	SaveDotNodeFile(filename, OtherList, false, false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecOrgDotAlign(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Dot Position");
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	const double Tolerance = GetAlignTolerance();	
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const double NextPosX = StageOffsetX+OrgPosX;
	const double NextPosY = StageOffsetY+OrgPosY;
	SetDotOrgStagePos(NextPosX, NextPosY);
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	const int MaxCaliRepeatCount=1;
	if ( m_CaliRepeatCount<MaxCaliRepeatCount )
	{
		m_CaliRepeatCount ++;
		StartReGrab(FALSE);
		return true;
	}

	bFinish = true;
	m_DotHorGapY=0.0;
	m_DotVerGapX=0.0;
	UpdateDotHorVerPos();	
	SetShowCaliStep(false);
	SetLockUIAlignHor(false);//鎖住水平調整
	SetLockUIAlignVer(false);//鎖住垂直確認
	SetLockUIAlignCorner(false);//鎖住四角落確認
	SetLockUIAlignDotNode(false);//鎖住玻璃點校正		
	SetDotOrgOffsetPos(0, 0);	
	SetCalibrationMode(CALIBRATION_STAGE_STOP);	
	SetRepeatGrabBtnCheck(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecDotCameraAlign(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{		
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);

	CString str;
	int DotRectW=0, DotRectH=0;	
	std::vector<CString> TextOutList;
	const double PitchX=GetDotPitchX();
	const double PitchY=GetDotPitchY();
	GetDotRoiSize(DotRectW, DotRectH);
	const int DotRectWHalf=DotRectW/2;
	const int DotRectHHalf=DotRectH/2;	

	const double FovWidth=AOIDataCollect.GetFovSizeRealW();
	const double FovHeight=AOIDataCollect.GetFovSizeRealH();
	const double FovWidthHalf=FovWidth*0.5;
	const double FovHeightHalf=FovHeight*0.5;	
	int NDotCountX=MAX((int)(FovWidthHalf/PitchX), 0);
	int NDotCountY=MAX((int)(FovHeightHalf/PitchY), 0);	
	
	TPOINT2D StagePos;	
	TPOINT2D ImgPosL, ImgPosT, ImgPosR, ImgPosB;	
	TPOINT2D ImgResL, ImgResT, ImgResR, ImgResB;
	TPOINT2D DotPosL, DotPosT, DotPosR, DotPosB;
	TPOINT2D DotGapL, DotGapT, DotGapR, DotGapB;
	TPOINT2D DotResL, DotResT, DotResR, DotResB;
	MotionCtrlPtr->GetCurrentPos(StagePos.x, StagePos.y);
	DotPosL = DotPosT = DotPosR = DotPosB = StagePos;
	DotPosL.x = StagePos.x-(NDotCountX*PitchX);
	DotPosR.x = StagePos.x+(NDotCountX*PitchX);
	DotPosB.y = StagePos.y-(NDotCountY*PitchY);//Y軸方向相反
	DotPosT.y = StagePos.y+(NDotCountY*PitchY);//Y軸方向相反	
	AOIDataCollect.MapStagePtToCamera(CameraID, DotPosL, StagePos, ImgPosL);
	AOIDataCollect.MapStagePtToCamera(CameraID, DotPosT, StagePos, ImgPosT);
	AOIDataCollect.MapStagePtToCamera(CameraID, DotPosR, StagePos, ImgPosR);
	AOIDataCollect.MapStagePtToCamera(CameraID, DotPosB, StagePos, ImgPosB);	

	const int nLeft=(int)(ImgPosL.x-DotRectWHalf);
	const int nTop=(int)(ImgPosT.y-DotRectHHalf);
	const int nRight=(int)(ImgPosR.x+DotRectWHalf);
	const int nBottom=(int)(ImgPosB.y+DotRectHHalf);
	if ( nLeft<0 || nRight>ImageW )
	{
		NDotCountX -= 1;
		DotPosL.x = StagePos.x-(NDotCountX*PitchX);
		DotPosR.x = StagePos.x+(NDotCountX*PitchX);	
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosL, StagePos, ImgPosL);
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosR, StagePos, ImgPosR);
	}
	if ( nTop<0 || nBottom>ImageH )
	{
		NDotCountY -= 1;
		DotPosB.y = StagePos.y-(NDotCountY*PitchY);//Y軸方向相反
		DotPosT.y = StagePos.y+(NDotCountY*PitchY);	//Y軸方向相反
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosT, StagePos, ImgPosT);		
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosB, StagePos, ImgPosB);
	}	

	DotResL = DotPosL;
	DotResT = DotPosT;
	DotResR = DotPosR;
	DotResB = DotPosB;
	m_strCaliStep = _T("");
	m_DotMatchedPtList.clear();
	str = _T("Pos, ImgPosX, ImgPoxY, ResultX, ResultY");
	TextOutList.push_back(str);
	if ( ExecDotSearch(ImgPosL.x, ImgPosL.y)==true )
	{
		ImgResL.x = GetDotMatchedPtX();
		ImgResL.y = GetDotMatchedPtY();
		DotGapL.x = GetDotMatchedOffsetX();
		DotGapL.y = GetDotMatchedOffsetY();		
		DotResL.x += GetDotMatchedOffsetX();
		DotResL.y += GetDotMatchedOffsetY();	
		m_DotMatchedPtList.push_back(m_DotMatchedPt);
		str.Format(_T("Left Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosL.x, ImgPosL.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
		TextOutList.push_back(str);
	}
	if ( ExecDotSearch(ImgPosR.x, ImgPosR.y)==true )
	{
		ImgResR.x = GetDotMatchedPtX();
		ImgResR.y = GetDotMatchedPtY();
		DotGapR.x = GetDotMatchedOffsetX();
		DotGapR.y = GetDotMatchedOffsetY();		
		DotResR.x += GetDotMatchedOffsetX();
		DotResR.y += GetDotMatchedOffsetY();		
		m_DotMatchedPtList.push_back(m_DotMatchedPt);
		str.Format(_T("Right Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosR.x, ImgPosR.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
		TextOutList.push_back(str);
	}

	if ( ExecDotSearch(ImgPosT.x, ImgPosT.y)==true )
	{
		ImgResT.x = GetDotMatchedPtX();
		ImgResT.y = GetDotMatchedPtY();
		DotGapT.x = GetDotMatchedOffsetX();
		DotGapT.y = GetDotMatchedOffsetY();		
		DotResT.x += GetDotMatchedOffsetX();
		DotResT.y += GetDotMatchedOffsetY();	
		m_DotMatchedPtList.push_back(m_DotMatchedPt);
		str.Format(_T("Top Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosT.x, ImgPosT.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
		TextOutList.push_back(str);
	}
	if ( ExecDotSearch(ImgPosB.x, ImgPosB.y)==true )
	{
		ImgResB.x = GetDotMatchedPtX();
		ImgResB.y = GetDotMatchedPtY();
		DotGapB.x = GetDotMatchedOffsetX();
		DotGapB.y = GetDotMatchedOffsetY();		
		DotResB.x += GetDotMatchedOffsetX();
		DotResB.y += GetDotMatchedOffsetY();		
		m_DotMatchedPtList.push_back(m_DotMatchedPt);
		str.Format(_T("Bottom Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosB.x, ImgPosB.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
		TextOutList.push_back(str);
	}

	TPOINT2D HorGap, VerGap;	
	const size_t DotMatchedCount=m_DotMatchedPtList.size();
	HorGap.x = DotResR.x-DotResL.x;
	HorGap.y = DotResR.y-DotResL.y;
	VerGap.x = DotResB.x-DotResT.x;
	VerGap.y = DotResB.y-DotResT.y;
	//m_strCaliStep.Format(_T("Center Hor-Y %.2fum, Center Ver-X %.2fum"), HorGap.y, VerGap.x);

	if ( 4 == DotMatchedCount )//計算4個端點的位置
	{
		TPOINT2D ImgPosLT, ImgPosRT, ImgPosLB, ImgPosRB;	
		TPOINT2D DotPosLT, DotPosRT, DotPosLB, DotPosRB;		
		TPOINT2D DotResLT, DotResRT, DotResLB, DotResRB;
		DotPosLT.x = DotPosL.x;	DotPosLT.y = DotPosT.y;
		DotPosRT.x = DotPosR.x;	DotPosRT.y = DotPosT.y;
		DotPosLB.x = DotPosL.x;	DotPosLB.y = DotPosB.y;
		DotPosRB.x = DotPosR.x;	DotPosRB.y = DotPosB.y;
		DotResLT = DotPosLT;
		DotResRT = DotPosRT;
		DotResLB = DotPosLB;
		DotResRB = DotPosRB;
		
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosLT, StagePos, ImgPosLT);
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosRT, StagePos, ImgPosRT);
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosLB, StagePos, ImgPosLB);
		AOIDataCollect.MapStagePtToCamera(CameraID, DotPosRB, StagePos, ImgPosRB);
		if ( ExecDotSearch(ImgPosLT.x, ImgPosLT.y)==true )
		{			
			DotResLT.x += GetDotMatchedOffsetX();
			DotResLT.y += GetDotMatchedOffsetY();
			m_DotMatchedPtList.push_back(m_DotMatchedPt);
			str.Format(_T("Left-Top Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosLT.x, ImgPosLT.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
			TextOutList.push_back(str);
		}
		if ( ExecDotSearch(ImgPosRT.x, ImgPosRT.y)==true )
		{			
			DotResRT.x += GetDotMatchedOffsetX();
			DotResRT.y += GetDotMatchedOffsetY();
			m_DotMatchedPtList.push_back(m_DotMatchedPt);
			str.Format(_T("Right-Top Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosRT.x, ImgPosRT.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
			TextOutList.push_back(str);
		}
		if ( ExecDotSearch(ImgPosLB.x, ImgPosLB.y)==true )
		{			
			DotResLB.x += GetDotMatchedOffsetX();
			DotResLB.y += GetDotMatchedOffsetY();
			m_DotMatchedPtList.push_back(m_DotMatchedPt);
			str.Format(_T("Left-Bottom Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosLB.x, ImgPosLB.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
			TextOutList.push_back(str);
		}
		if ( ExecDotSearch(ImgPosRB.x, ImgPosRB.y)==true )
		{			
			DotResRB.x += GetDotMatchedOffsetX();
			DotResRB.y += GetDotMatchedOffsetY();
			m_DotMatchedPtList.push_back(m_DotMatchedPt);
			str.Format(_T("Right-Bottom Pos, %.2f, %.2f, %.2f, %.2f"), ImgPosRB.x, ImgPosRB.y, m_DotMatchedPt.x, m_DotMatchedPt.y);
			TextOutList.push_back(str);
		}

		const double HorX_Top=DotResRT.x-DotResLT.x;
		const double HorX_Bot=DotResRB.x-DotResLB.x;
		const double VerY_Lef=DotResLB.y-DotResLT.y;
		const double VerY_Rig=DotResRB.y-DotResRT.y;
		str.Format(_T("Top-Bot-X Gap %.2fum, Left-Right-Y Gap %.2fum"), HorX_Bot-HorX_Top, VerY_Rig-VerY_Lef);
		//m_strCaliStep += _T(", ");
		//m_strCaliStep += str;
	}
	ExecDotSearch(ImageW/2, ImageH/2);

	TPOINT2D ImgHorGap, ImgVerGap;		
	ImgHorGap.x = ImgResR.x-ImgResL.x;
	ImgHorGap.y = ImgResR.y-ImgResL.y;
	ImgVerGap.x = ImgResB.x-ImgResT.x;
	ImgVerGap.y = ImgResB.y-ImgResT.y;
	double HorRes=0.0;
	double VerRes=0.0;	
	const double DistX=NDotCountX*PitchX*2;
	const double DistY=NDotCountY*PitchY*2;
	const double HorImgRes=JetAPI::CalcDistance(ImgHorGap.x, ImgHorGap.y);
	const double VerImgRes=JetAPI::CalcDistance(ImgVerGap.x, ImgVerGap.y);	
	const double ResX=AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY=AOIDataCollect.GetCameraResolutionY(CameraID);

	if ( fabs(HorImgRes) > 0.0001 )
	{	HorRes=DistX/HorImgRes;	}
	if ( fabs(VerImgRes) > 0.0001 )
	{	VerRes=DistY/VerImgRes;	}
	str.Format(_T("Resolution:(%.4f/%.4f, %.4f/%.4f), FOV(%.0f/%.0f, %.0f/%.0f)"), HorRes, ResX, VerRes, ResY, HorRes*ImageW, ResX*ImageW, VerRes*ImageH, ResY*ImageH);
	//m_strCaliStep += _T(", ");
	m_strCaliStep += str;

	const size_t TextOutCount=TextOutList.size();
	if ( TextOutCount > 0 )
	{	
		FILE *pfile = NULL;
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CameraDot.TXT"));
		pfile=::_tfopen(str, _T("w+"));
		if ( NULL != pfile )
		{			
			for ( size_t i=0; i<TextOutCount; i++ )
			{	::_ftprintf(pfile, _T("%s\n"), TextOutList[i]);	}
			::fclose(pfile); pfile=NULL;
		}
	}

	DrawImageWndMemDC();
	RedrawWnd();

	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		m_DotMatchedPtList.clear();
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	::Sleep(100);
	m_CaliRepeatCount = 0;	
	StartReGrab(FALSE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXAlignHorLineX1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Hor-Line X1");
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double Tolerance = GetAlignTolerance();	
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const double NextPosX = StageOffsetX+OrgPosX;
	const double NextPosY = StageOffsetY+OrgPosY;
	SetDotOrgStagePos(NextPosX, NextPosY);
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		m_DotHorGapY = 0.0;
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	const int MaxCaliRepeatCount=1;
	if ( m_CaliRepeatCount<MaxCaliRepeatCount )
	{
		m_CaliRepeatCount ++;
		StartReGrab(FALSE);
		return true;
	}

	bFinish = false;		
	UpdateDotHorVerPos();
	SetDotOrgOffsetPos(0, 0);	
	const double PosX = m_DotStagePosHor.x;
	const double PosY = m_DotStagePosHor.y;	

	ResetDotSearch();
	m_CaliRepeatCount = 0;
	SetCalibrationMode(CALIBRATION_STAGE_X_POS_ALIGN_2);
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	StartReGrab(FALSE);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXAlignHorLineX2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Verify Hor-Line X2");
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	const double OrgOffsetX = GetDotOrgOffsetPosX();
	const double OrgOffsetY = GetDotOrgOffsetPosY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const int MaxCaliRepeatCount=3;
	m_DotHorGapY = StageOffsetY-OrgOffsetY;	
	if ( m_CaliRepeatCount<MaxCaliRepeatCount )
	{
		m_CaliRepeatCount ++;
		StartReGrab(FALSE);
		return true;
	}

	bFinish = false;
	ResetDotSearch();
	m_CaliRepeatCount = 0;
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	SetCalibrationMode(CALIBRATION_STAGE_X_POS_ALIGN_1);
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	StartReGrab(FALSE);		
	/*
	bFinish = true;	
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);
	*/
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXVerifyX1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXVerifyX2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecXVerifyXEach(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }

	CString str;
	const int CountX = GetDotCountX();
	const int IndexX = (int)(m_DotHorLineList.size());	
	const int NextIndexX = IndexX+1;
	str = _T("Verify Horizontal Line Dot");
	str = LoadMultiLanguageString(str, str);
	SetShowCaliStep(true);
	if ( IndexX < CountX )
	{	m_strCaliStep.Format(_T("%s %d/%d"), str, IndexX+1, CountX); }
	RedrawWnd();
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}	
	
	const double PitchX = GetDotPitchX();	
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double OrgOffsetX = GetDotOrgOffsetPosX();
	const double OrgOffsetY = GetDotOrgOffsetPosY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const double NextPosX = (NextIndexX*PitchX)+OrgPosX;
	const double NextPosY = OrgPosY;
	if ( IndexX < CountX )
	{	
		TDotNode DotNode;
		DotNode.nIndexX = IndexX;
		DotNode.dPosX = ((IndexX)*PitchX)+OrgPosX;
		DotNode.dPosY = OrgPosY;
		DotNode.dAlignedX = DotNode.dPosX;
		DotNode.dAlignedY = DotNode.dPosY;
		DotNode.dOffsetX = StageOffsetX-OrgOffsetX;
		DotNode.dOffsetY = StageOffsetY-OrgOffsetY;		
		m_DotHorLineList.push_back(DotNode);

		double MovePosX=NextPosX;
		double MovePosY=NextPosY;
		if ( NextIndexX < CountX )
		{
			MovePosX=NextPosX;
			MovePosY=NextPosY;
		}
		else
		{
			MovePosX=OrgPosX;
			MovePosY=OrgPosY;
			m_FuncTime.SetEnd();
		}
		if ( MotionCtrlPtr->XYMoveTo(MovePosX, MovePosY) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		ResetDotSearch();
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}		

	bFinish = true;	
	ResetDotSearch();
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);

	SaveDotNodeFile(m_DotHorLineList, false);
	StartReGrab(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecYAlignVerLineY1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Ver-Line Y1");	
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double Tolerance = GetAlignTolerance();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const double NextPosX = StageOffsetX+OrgPosX;
	const double NextPosY = StageOffsetY+OrgPosY;
	SetDotOrgStagePos(NextPosX, NextPosY);
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{			
		m_DotVerGapX = 0.00;
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	const int MaxCaliRepeatCount=1;
	if ( m_CaliRepeatCount<MaxCaliRepeatCount )
	{
		m_CaliRepeatCount ++;
		StartReGrab(FALSE);
		return true;
	}

	bFinish = false;	
	UpdateDotHorVerPos();
	SetDotOrgOffsetPos(0, 0);	
	const double PosX = m_DotStagePosVer.x;
	const double PosY = m_DotStagePosVer.y;	

	ResetDotSearch();
	m_CaliRepeatCount = 0;
	SetCalibrationMode(CALIBRATION_STAGE_Y_POS_ALIGN_2);
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	StartReGrab(FALSE);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecYAlignVerLineY2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Verify Ver-Line Y2");
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetShowCaliStep(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}
	
	const double OrgOffsetX = GetDotOrgOffsetPosX();
	const double OrgOffsetY = GetDotOrgOffsetPosY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const int MaxCaliRepeatCount=3;
	m_DotVerGapX = StageOffsetX-OrgOffsetX;

	if ( m_CaliRepeatCount<MaxCaliRepeatCount )
	{
		m_CaliRepeatCount ++;
		StartReGrab(FALSE);
		return true;
	}

	bFinish = false;		
	ResetDotSearch();
	m_CaliRepeatCount = 0;
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();	
	SetCalibrationMode(CALIBRATION_STAGE_Y_POS_ALIGN_1);
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	StartReGrab(FALSE);		
	/*
	bFinish = true;	
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);
	*/
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecYVerifyY1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecYVerifyY2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecYVerifyYEach(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }

	CString str;
	const int CountY = GetDotCountY();
	const int IndexY = (int)(m_DotVerLineList.size());	
	const int NextIndexY = IndexY+1;
	str = _T("Verify Vertical Line Dot");
	str = LoadMultiLanguageString(str, str);
	SetShowCaliStep(true);
	if ( IndexY < CountY )
	{	m_strCaliStep.Format(_T("%s %d/%d"), str, IndexY+1, CountY); }
	RedrawWnd();
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}	
	
	const double PitchY = GetDotPitchY();	
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double OrgOffsetX = GetDotOrgOffsetPosX();
	const double OrgOffsetY = GetDotOrgOffsetPosY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	const double NextPosX = OrgPosX;
	const double NextPosY = ((NextIndexY)*PitchY)+OrgPosY;
	if ( IndexY < CountY )
	{	
		TDotNode DotNode;
		DotNode.nIndexY = IndexY;
		DotNode.dPosX = OrgPosX;
		DotNode.dPosY = ((IndexY+1)*PitchY)+OrgPosY;
		DotNode.dAlignedX = DotNode.dPosX;
		DotNode.dAlignedY = DotNode.dPosY;
		DotNode.dOffsetX = StageOffsetX-OrgOffsetX;
		DotNode.dOffsetY = StageOffsetY-OrgOffsetY;		
		m_DotVerLineList.push_back(DotNode);		

		double MovePosX=NextPosX;
		double MovePosY=NextPosY;
		if ( NextIndexY < CountY )
		{
			MovePosX=NextPosX;
			MovePosY=NextPosY;
		}
		else
		{
			MovePosX=OrgPosX;
			MovePosY=OrgPosY;
			m_FuncTime.SetEnd();
		}
		if ( MotionCtrlPtr->XYMoveTo(MovePosX, MovePosY) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		ResetDotSearch();
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}		

	bFinish = true;	
	ResetDotSearch();
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);

	SaveDotNodeFile(m_DotVerLineList, false);
	StartReGrab(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::AnalyzeCornerPos()//分析四角落的點座標
{
	int      i=0;
	CString  str;
	TPOINT2D OffsetPox[DOT_CORNER_TOTAL];//偏移值
	TPOINT3D CornerPos[DOT_CORNER_TOTAL];//理論位置
	TPOINT3D MatchedPos[DOT_CORNER_TOTAL];//結果位置

	for ( i=0; i<DOT_CORNER_TOTAL; i++ )
	{
		CornerPos[i] = m_DotCornerPos[i];
		MatchedPos[i] = m_DotMatchedCornerPos[i];

		OffsetPox[i].x = MatchedPos[i].x-CornerPos[i].x;
		OffsetPox[i].y = MatchedPos[i].y-CornerPos[i].y;
	}
	
	//理論
	const double dPosX12 = CornerPos[DOT_CORNER_2].x-CornerPos[DOT_CORNER_1].x;
	const double dPosY12 = CornerPos[DOT_CORNER_2].y-CornerPos[DOT_CORNER_1].y;
	const double dPosX23 = CornerPos[DOT_CORNER_3].x-CornerPos[DOT_CORNER_2].x;
	const double dPosY23 = CornerPos[DOT_CORNER_3].y-CornerPos[DOT_CORNER_2].y;
	const double dPosX34 = CornerPos[DOT_CORNER_4].x-CornerPos[DOT_CORNER_3].x;
	const double dPosY34 = CornerPos[DOT_CORNER_4].y-CornerPos[DOT_CORNER_3].y;
	const double dPosX41 = CornerPos[DOT_CORNER_1].x-CornerPos[DOT_CORNER_4].x;
	const double dPosY41 = CornerPos[DOT_CORNER_1].y-CornerPos[DOT_CORNER_4].y;

	const double dPosX13 = CornerPos[DOT_CORNER_3].x-CornerPos[DOT_CORNER_1].x;
	const double dPosY13 = CornerPos[DOT_CORNER_3].y-CornerPos[DOT_CORNER_1].y;
	const double dPosX24 = CornerPos[DOT_CORNER_4].x-CornerPos[DOT_CORNER_2].x;
	const double dPosY24 = CornerPos[DOT_CORNER_4].y-CornerPos[DOT_CORNER_2].y;

	const double HorLen1 = sqrt((dPosX12*dPosX12)+(dPosY12*dPosY12));//水平線-1
	const double VerLen1 = sqrt((dPosX23*dPosX23)+(dPosY23*dPosY23));//垂直線-1
	const double HorLen2 = sqrt((dPosX34*dPosX34)+(dPosY34*dPosY34));//水平線-2
	const double VerLen2 = sqrt((dPosX41*dPosX41)+(dPosY41*dPosY41));//垂直線-2
	const double DalLen1 = sqrt((dPosX13*dPosX13)+(dPosY13*dPosY13));//對角線-1
	const double DalLen2 = sqrt((dPosX24*dPosX24)+(dPosY24*dPosY24));//對角線-2

	//結果
	const double dPosX12Res = MatchedPos[DOT_CORNER_2].x-MatchedPos[DOT_CORNER_1].x;
	const double dPosY12Res = MatchedPos[DOT_CORNER_2].y-MatchedPos[DOT_CORNER_1].y;
	const double dPosX23Res = MatchedPos[DOT_CORNER_3].x-MatchedPos[DOT_CORNER_2].x;
	const double dPosY23Res = MatchedPos[DOT_CORNER_3].y-MatchedPos[DOT_CORNER_2].y;
	const double dPosX34Res = MatchedPos[DOT_CORNER_4].x-MatchedPos[DOT_CORNER_3].x;
	const double dPosY34Res = MatchedPos[DOT_CORNER_4].y-MatchedPos[DOT_CORNER_3].y;
	const double dPosX41Res = MatchedPos[DOT_CORNER_1].x-MatchedPos[DOT_CORNER_4].x;
	const double dPosY41Res = MatchedPos[DOT_CORNER_1].y-MatchedPos[DOT_CORNER_4].y;

	const double dPosX13Res = MatchedPos[DOT_CORNER_3].x-MatchedPos[DOT_CORNER_1].x;
	const double dPosY13Res = MatchedPos[DOT_CORNER_3].y-MatchedPos[DOT_CORNER_1].y;
	const double dPosX24Res = MatchedPos[DOT_CORNER_4].x-MatchedPos[DOT_CORNER_2].x;
	const double dPosY24Res = MatchedPos[DOT_CORNER_4].y-MatchedPos[DOT_CORNER_2].y;

	const double HorLen1Res = sqrt((dPosX12Res*dPosX12Res)+(dPosY12Res*dPosY12Res));//水平線-1
	const double VerLen1Res = sqrt((dPosX23Res*dPosX23Res)+(dPosY23Res*dPosY23Res));//垂直線-1
	const double HorLen2Res = sqrt((dPosX34Res*dPosX34Res)+(dPosY34Res*dPosY34Res));//水平線-2
	const double VerLen2Res = sqrt((dPosX41Res*dPosX41Res)+(dPosY41Res*dPosY41Res));//垂直線-2
	const double DalLen1Res = sqrt((dPosX13Res*dPosX13Res)+(dPosY13Res*dPosY13Res));//對角線-1
	const double DalLen2Res = sqrt((dPosX24Res*dPosX24Res)+(dPosY24Res*dPosY24Res));//對角線-2

	CString filename;
	FILE *pfile = NULL;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("VerifyDotCorner.TXT"));
	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile )
	{
		str.Format(_T("Error, Open File Fault[%s]"), filename);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	m_FuncTimeFd = m_FuncTime;
	const DWORD time=m_FuncTime.GetFuncTime();
	::_ftprintf(pfile, _T("Time=%d ms\n"), time);

	DOT_CORNER_ID DotCornerID;		
	::_ftprintf(pfile, _T("Corner, PosX, PosY, ResultX, ResultY, OffsetX, OffsetY\n"));

	DotCornerID = DOT_CORNER_1;
	::_ftprintf(pfile, _T("Corner_1, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 
		CornerPos[DotCornerID].x, CornerPos[DotCornerID].y, 
		MatchedPos[DotCornerID].x, MatchedPos[DotCornerID].y, 
		OffsetPox[DotCornerID].x, OffsetPox[DotCornerID].y);

	DotCornerID = DOT_CORNER_2;
	::_ftprintf(pfile, _T("Corner_2, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 
		CornerPos[DotCornerID].x, CornerPos[DotCornerID].y, 
		MatchedPos[DotCornerID].x, MatchedPos[DotCornerID].y, 
		OffsetPox[DotCornerID].x, OffsetPox[DotCornerID].y);

	DotCornerID = DOT_CORNER_3;
	::_ftprintf(pfile, _T("Corner_3, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 
		CornerPos[DotCornerID].x, CornerPos[DotCornerID].y, 
		MatchedPos[DotCornerID].x, MatchedPos[DotCornerID].y, 
		OffsetPox[DotCornerID].x, OffsetPox[DotCornerID].y);

	DotCornerID = DOT_CORNER_4;
	::_ftprintf(pfile, _T("Corner_4, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 
		CornerPos[DotCornerID].x, CornerPos[DotCornerID].y, 
		MatchedPos[DotCornerID].x, MatchedPos[DotCornerID].y, 
		OffsetPox[DotCornerID].x, OffsetPox[DotCornerID].y);

	::_ftprintf(pfile, _T("\n"));
	::_ftprintf(pfile, _T("Line , Distance, Result, Error\n"));
	::_ftprintf(pfile, _T("Hor-1 , %.4f, %.4f, %.4f\n"), HorLen1, HorLen1Res, HorLen1Res-HorLen1);
	::_ftprintf(pfile, _T("Hor-2 , %.4f, %.4f, %.4f\n"), HorLen2, HorLen2Res, HorLen2Res-HorLen2);
	::_ftprintf(pfile, _T("Ver-1 , %.4f, %.4f, %.4f\n"), VerLen1, VerLen1Res, VerLen1Res-VerLen1);
	::_ftprintf(pfile, _T("Ver-2 , %.4f, %.4f, %.4f\n"), VerLen2, VerLen2Res, VerLen2Res-VerLen2);
	::_ftprintf(pfile, _T("Dal-1 , %.4f, %.4f, %.4f\n"), DalLen1, DalLen1Res, DalLen1Res-DalLen1);
	::_ftprintf(pfile, _T("Dal-2 , %.4f, %.4f, %.4f\n"), DalLen2, DalLen2Res, DalLen2Res-DalLen2);
	::_ftprintf(pfile, _T("\n"));

	::fclose(pfile); pfile=NULL;

	if ( GetAlignedMapUsed() == false )
	{	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecAlignCornerPos1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Corner Pos 1");	
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	double PosX=0, PosY=0, PosZ=0;	
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();	
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();	
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();	
	const double Tolerance = GetAlignTolerance();	
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();		
	m_DotMatchedCornerPos[DOT_CORNER_1].x = m_DotMatchedCornerPos[DOT_CORNER_1].x+StageOffsetX;
	m_DotMatchedCornerPos[DOT_CORNER_1].y = m_DotMatchedCornerPos[DOT_CORNER_1].y+StageOffsetY;	
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		const double NextPosX2 = m_DotMatchedCornerPos[DOT_CORNER_1].x;
		const double NextPosY2 = m_DotMatchedCornerPos[DOT_CORNER_1].y;	
		if ( MotionCtrlPtr->XYMoveTo(NextPosX2, NextPosY2) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}

	const double NextPosX = m_DotCornerPos[DOT_CORNER_2].x;
	const double NextPosY = m_DotCornerPos[DOT_CORNER_2].y;	
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}	
	ResetDotSearch();
	m_CaliRepeatCount = 0;	
	SetCalibrationMode(CALIBRATION_STAGE_ALIGN_CORNER_2);	
	StartReGrab(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecAlignCornerPos2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Corner Pos 2");	
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	

	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	double PosX=0, PosY=0, PosZ=0;	
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();	
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double Tolerance = GetAlignTolerance();	
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();
	m_DotMatchedCornerPos[DOT_CORNER_2].x = m_DotMatchedCornerPos[DOT_CORNER_2].x+StageOffsetX;
	m_DotMatchedCornerPos[DOT_CORNER_2].y = m_DotMatchedCornerPos[DOT_CORNER_2].y+StageOffsetY;	
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		const double NextPosX2 = m_DotMatchedCornerPos[DOT_CORNER_2].x;
		const double NextPosY2 = m_DotMatchedCornerPos[DOT_CORNER_2].y;	
		if ( MotionCtrlPtr->XYMoveTo(NextPosX2, NextPosY2) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	const double NextPosX = m_DotCornerPos[DOT_CORNER_3].x;
	const double NextPosY = m_DotCornerPos[DOT_CORNER_3].y;
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}
	ResetDotSearch();
	m_CaliRepeatCount = 0;		
	SetCalibrationMode(CALIBRATION_STAGE_ALIGN_CORNER_3);	
	StartReGrab(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecAlignCornerPos3(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Corner Pos 3");	
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	double PosX=0, PosY=0, PosZ=0;	
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();	
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double Tolerance = GetAlignTolerance();	
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();		
	m_DotMatchedCornerPos[DOT_CORNER_3].x = m_DotMatchedCornerPos[DOT_CORNER_3].x+StageOffsetX;
	m_DotMatchedCornerPos[DOT_CORNER_3].y = m_DotMatchedCornerPos[DOT_CORNER_3].y+StageOffsetY;	
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		const double NextPosX2 = m_DotMatchedCornerPos[DOT_CORNER_3].x;
		const double NextPosY2 = m_DotMatchedCornerPos[DOT_CORNER_3].y;	
		if ( MotionCtrlPtr->XYMoveTo(NextPosX2, NextPosY2) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	const double NextPosX = m_DotCornerPos[DOT_CORNER_4].x;
	const double NextPosY = m_DotCornerPos[DOT_CORNER_4].y;
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}
	ResetDotSearch();
	m_CaliRepeatCount = 0;		
	SetCalibrationMode(CALIBRATION_STAGE_ALIGN_CORNER_4);	
	StartReGrab(FALSE);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecAlignCornerPos4(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	SetShowCaliStep(true);
	m_strCaliStep = _T("Align Corner Pos 4");	
	m_strCaliStep = LoadMultiLanguageString(m_strCaliStep, m_strCaliStep);
	RedrawWnd();	
	
	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}

	double PosX=0, PosY=0, PosZ=0;	
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();	
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();
	const double OrgPosX = GetDotOrgStagePosX();
	const double OrgPosY = GetDotOrgStagePosY();
	const double Tolerance = GetAlignTolerance();	
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();	
	m_DotMatchedCornerPos[DOT_CORNER_4].x = m_DotMatchedCornerPos[DOT_CORNER_4].x+StageOffsetX;
	m_DotMatchedCornerPos[DOT_CORNER_4].y = m_DotMatchedCornerPos[DOT_CORNER_4].y+StageOffsetY;	
	if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
	{	
		const double NextPosX2 = m_DotMatchedCornerPos[DOT_CORNER_4].x;
		const double NextPosY2 = m_DotMatchedCornerPos[DOT_CORNER_4].y;	
		if ( MotionCtrlPtr->XYMoveTo(NextPosX2, NextPosY2) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	m_FuncTime.SetEnd();
	const double NextPosX = m_DotCornerPos[DOT_CORNER_1].x;
	const double NextPosY = m_DotCornerPos[DOT_CORNER_1].y;
	if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
	{	return false;	}	
	AnalyzeCornerPos();		
	bFinish = true;	
	ResetDotSearch();
	m_CaliRepeatCount = 0;		
	SetShowCaliStep(false);	
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);

	MotionCtrlPtr->WaitForMotionStop();
	if ( false == GetAlignedMapUsed() )
	{	StartReGrab(FALSE); }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecCalibrateDotNode(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish)
{
	if ( NULL==m_ImageBuffer || NULL==m_ShowBuffer ) { return false; }
	::memcpy(m_ShowBuffer, m_ImageBuffer, sizeof(IMAGE_DATA)*m_ImageH*m_ImageStep);
	DrawImageWndMemDC();
	if ( ExecDotSearch() == false )
	{	return false; }
	CString str;	
	const int Count = (int)(m_DotNodeList.size());	
	const int Index = m_DotNodeIndex;	
	SetShowCaliStep(true);	
	if ( true == m_DotNodeCanBeXYTable )
	{	str = _T("Search Dot");	}
	else
	{	str = _T("Verify Dot"); }
	str = LoadMultiLanguageString(str, str);
	m_strCaliStep.Format(_T("%s %d/%d"), str, Index+1, Count);
	RedrawWnd();

	bool bRepeat = GetRepeatGrabBtnChecked();
	if ( false == bRepeat )
	{
		bFinish = true;	
		SetCalibrationMode(CALIBRATION_STAGE_STOP);		
		return true;
	}	
	double TotalOffsetX=0, TotalOffsetY=0;
	const double Tolerance = GetAlignTolerance();	
	const double ErrorOffsetX = GetErrorOffsetX();
	const double ErrorOffsetY = GetErrorOffsetY();
	const double StageOffsetX = GetDotMatchedOffsetX();
	const double StageOffsetY = GetDotMatchedOffsetY();			
	const int    MaxCaliRepeatCount=CWnd::GetDlgItemInt(CALISTAGE_MAX_XYCALI_COUNT_EDIT);
	if ( Index < Count )
	{	
		TDotNode &DotNode = m_DotNodeList[Index];
		//在校正位置上取像後的總偏差值
		DotNode.dCaliPosX = DotNode.dCaliPosX+StageOffsetX;
		DotNode.dCaliPosY = DotNode.dCaliPosY+StageOffsetY;
		DotNode.dOffsetX = DotNode.dCaliPosX-DotNode.dAlignedX;
		DotNode.dOffsetY = DotNode.dCaliPosY-DotNode.dAlignedY;		
		if ( fabs(DotNode.dOffsetX)>ErrorOffsetX || fabs(DotNode.dOffsetY)>ErrorOffsetY ) 
		{	DotNode.eResultID = RESULT_ID_NG;	}
		else
		{	DotNode.eResultID = RESULT_ID_OK;	}		
		UpdateDotListWnd();
		if ( true == m_DotNodeCanBeXYTable )
		{
			if ( fabs(StageOffsetX)>Tolerance || fabs(StageOffsetY)>Tolerance )
			{	
				if ( m_CaliRepeatCount < MaxCaliRepeatCount )
				{	
					const double NextPosX2 = DotNode.dCaliPosX;
					const double NextPosY2 = DotNode.dCaliPosY;	
					if ( MotionCtrlPtr->XYMoveTo(NextPosX2, NextPosY2) == false )
					{
						JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
						return false;
					}
					//m_CaliRepeatCount = 0;
					m_CaliRepeatCount ++ ; 
					StartReGrab(FALSE);
					return true;
				}
				else
				{	m_CaliRepeatCount = m_CaliRepeatCount; }
			}
			TotalOffsetX = DotNode.dOffsetX;
			TotalOffsetY = DotNode.dOffsetY;
		}
		else
		{	ExecOtherDotSearch();	}
	}

	const int NextIndex = m_DotNodeIndex+1;
	if ( NextIndex < Count )
	{			
		m_DotNodeIndex = NextIndex;		
		TDotNode &DotNode = m_DotNodeList[NextIndex];
		DotNode.dCaliPosX += TotalOffsetX;
		DotNode.dCaliPosY += TotalOffsetY;
		const double NextPosX = DotNode.dCaliPosX;
		const double NextPosY = DotNode.dCaliPosY;
		if ( ExecXYMoveTo(Mode, NextPosX, NextPosY) == false )	
		{	return false;	}
		m_CaliRepeatCount = 0;		
		StartReGrab(FALSE);
		return true;
	}
	m_FuncTime.SetEnd();
	const double DotOrgPosX = GetDotOrgStagePosX();
	const double DotOrgPosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(DotOrgPosX, DotOrgPosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	if ( true == m_DotNodeCanBeXYTable )
	{
		str.Format(_T("Do you want to Build XY Calibration Table?"));
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	
			BuildStageXYCaliTable();	
			SaveStageDotCompareFile();
		}
		m_DotDateTime = CTime::GetCurrentTime();//校正時間
	}

	bFinish = true;	
	ResetDotSearch();
	SetShowCaliStep(false);	
	SaveDotNodeFile(m_DotNodeList, true);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);		
	SetRepeatGrabBtnCheck(false);
	
	MotionCtrlPtr->WaitForMotionStop();
	StartReGrab(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnSelchangeSliceCombo() 
{
	// TODO: Add your control notification handler code here
	OnGrabBtn();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	m_LBtnUpPos = point;
	m_LBtnDownPos = m_MovingPos = m_LBtnUpPos;	
	SetCapture();	
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, CALISTAGE_IMAGE_WND, &pt) == true)
	{	PickDotNodeList(pt);	}

	m_LBtnUpPos = m_MovingPos = point;
	m_MovingPos.x = m_MovingPos.y = -1;
	m_LBtnUpPos = m_LBtnDownPos = m_MovingPos;
	RedrawWnd();
	CDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	CDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_RBtnUpPos = point;
	m_RBtnDownPos = m_MovingPos = m_RBtnUpPos;
	SetCapture();
	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_RBtnUpPos = m_MovingPos = point;
	m_MovingPos.x = m_MovingPos.y = -1;
	m_RBtnUpPos = m_RBtnDownPos = m_MovingPos;
	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	bool bRedrawWnd=false;
	POINT WndPt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, CALISTAGE_IMAGE_WND, &WndPt) == true)
	{	bRedrawWnd = true;	}
	if ( true == bRedrawWnd )
	{	UpdateImageValue(WndPt);	}
	if ( this != GetCapture() ) 
	{	
		if ( true == bRedrawWnd )
		{	RedrawWnd(); }
		CDialog::OnMouseMove(nFlags, point);
		return; 
	}	
	if ( true == bRedrawWnd )
	{	RedrawWnd(); }
	CDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT    WndPt = point;	
	if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, CALISTAGE_IMAGE_WND, &WndPt) == true)
	{
		TPOINT2D ImagePt=WndPt;	
		TPOINT2D Res, StageCp, StagePt;	
		CAMERA_ID CameraID = GetCameraID();
		const IMAGE_SIZE ImageW = m_ImageW;
		const IMAGE_SIZE ImageH = m_ImageH;	
		Res.x = AOIDataCollect.GetCameraResolutionX(CameraID);
		Res.y = AOIDataCollect.GetCameraResolutionY(CameraID);
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
void CCaliPaneStage::OnDotMatchBtn() 
{
	// TODO: Add your control notification handler code here
	m_DotAlignMode = (DOT_ALIGN_MODE)(JetAPI::GetComboxCurSelData(m_AlignModeCombox));	
	ExecDotSearch();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnShowCenterLineChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(CALISTAGE_SHOW_CENTER_LINE_CHK);
	if ( TRUE == bChk ) { m_ShowCrossLine = true; }
	else { m_ShowCrossLine = false; }
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnShowCursorLineChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(CALISTAGE_SHOW_CURSOR_LINE_CHK);
	if ( TRUE == bChk ) { m_ShowCursorLine = true; }
	else { m_ShowCursorLine = false; }
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotOrgStagePosX() const
{
	return m_DotOrgStagePos.x;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotOrgStagePosY() const
{
	return m_DotOrgStagePos.y;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetDotOrgStagePos(double PosX, double PosY)
{
	m_DotOrgStagePos.x = PosX;
	m_DotOrgStagePos.y = PosY;
	return;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotOrgOffsetPosX() const
{
	return m_DotOrgOffsetPos.x;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotOrgOffsetPosY() const
{
	return m_DotOrgOffsetPos.y;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::SetDotOrgOffsetPos(double OffsetX, double OffsetY)
{
	m_DotOrgOffsetPos.x = OffsetX;
	m_DotOrgOffsetPos.y = OffsetY;
	return;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotMatchedPtX() const
{
	return m_DotMatchedPt.x;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotMatchedPtY() const
{
	return m_DotMatchedPt.y;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotMatchedOffsetX() const
{
	return m_DotMatchedOffset.x;
}
//-------------------------------------------------------------------------------------//
double CCaliPaneStage::GetDotMatchedOffsetY() const
{
	return m_DotMatchedOffset.y;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildDotNodeList()//建立每個點的XY校正表
{
	int   i=0, j=0;
	RECT  DotRect={0,0,0,0};
	RECT  DotRect2={0,0,0,0};
	RECT  WndRect = m_ImageWndRect;	
	double DotPosX=0, DotPosY=0;
	double AlignedX=0, AlignedY=0;
	const int SkipX = m_DotSkipX;
	const int SkipY = m_DotSkipY;
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();		
	const double UnitRatio=GetUnitRatio();
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();	
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();
	const int WndRectW = WndRect.right-WndRect.left;
	const int WndRectH = WndRect.bottom-WndRect.top;
	const int DotRectPitchX=(WndRectW/(CountX+1));
	const int DotRectPitchY=(WndRectH/(CountY+1));
	const int DotRectStartX = DotRectPitchX;
	const int DotRectStartY = DotRectPitchY;
	const int DotRectW=(WndRectW/CountX)/2;
	const int DotRectH=(WndRectH/CountY)/2;	
	const int DorRectSize = MIN(DotRectW, DotRectH);
	const CMapCoordinate &MapCTS=m_AlignedMap;
	BOX_SHAPE_MODE eShapeMode=GetDotShapeMode();

	TDotNode DotNode;
	m_DotNodeList.clear();
	for ( j=0; j<CountY; j++ )
	{
		for ( i=0; i<CountX; i++ )
		{
			DotPosX = PosX+(i*PitchX);
			DotPosY = PosY+(j*PitchY);
			MapCTS.Map2D(DotPosX, DotPosY, AlignedX, AlignedY);

			DotRect.left = DotRectStartX+(i*DotRectPitchX)-(DorRectSize/2);
			DotRect.top = DotRectStartY+(j*DotRectPitchY)-(DorRectSize/2);
			DotRect.right = DotRect.left+DorRectSize;
			DotRect.bottom = DotRect.top+DorRectSize;
			DotRect2 = DotRect;
			DotRect2.top = WndRect.bottom-DotRect.bottom;
			DotRect2.bottom = WndRect.bottom-DotRect.top;

			DotNode = TDotNode();
			DotNode.nIndexX = i*(SkipX+1);
			DotNode.nIndexY = j*(SkipY+1);
			DotNode.dPosX = DotPosX;
			DotNode.dPosY = DotPosY;			
			DotNode.dAlignedX = AlignedX;
			DotNode.dAlignedY = AlignedY;			
			DotNode.dCaliPosX = AlignedX;
			DotNode.dCaliPosY = AlignedY;			
			DotNode.dOffsetX = 0;
			DotNode.dOffsetX = 0;
			DotNode.rcDot = DotRect2;
			DotNode.eResultID = RESULT_ID_NONE;
			DotNode.eShapeMode=eShapeMode;
			m_DotNodeList.push_back(DotNode);
		}
	}

	BuildDotListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::UpdateDotHorVerPos()//計算點的水平與垂直位置	
{	
	CString strPitchX;
	CString strPitchY;
	const double UnitRatio=GetUnitRatio();
	const int SkipX = CWnd::GetDlgItemInt(CALISTAGE_DOT_SKIP_X_EDIT);	
	const int SkipY = CWnd::GetDlgItemInt(CALISTAGE_DOT_SKIP_Y_EDIT);	
	const int CountX = CWnd::GetDlgItemInt(CALISTAGE_DOT_COUNT_X_EDIT);	
	const int CountY = CWnd::GetDlgItemInt(CALISTAGE_DOT_COUNT_Y_EDIT);		

	CWnd::GetDlgItemText(CALISTAGE_DOT_PITCH_X_EDIT, strPitchX);
	CWnd::GetDlgItemText(CALISTAGE_DOT_PITCH_Y_EDIT, strPitchY);	
	const double PitchX = (::_ttof(strPitchX))*UnitRatio;
	const double PitchY = (::_ttof(strPitchY))*UnitRatio;
	const bool bSignX = AOIDataCollect.GetStageSignPositiveX();
	const bool bSignY = AOIDataCollect.GetStageSignPositiveY();	
	if ( true == bSignX )
	{	m_DotPitchX = (PitchX);	}
	else
	{	m_DotPitchX = -(PitchX);	}

	if ( true == bSignY )
	{	m_DotPitchY = (PitchY);	}
	else
	{	m_DotPitchY = -(PitchY);	}

	const BOOL bReverseX = CWnd::IsDlgButtonChecked(CALISTAGE_REVERSE_X_CHK);
	if ( TRUE == bReverseX )
	{	m_DotPitchX = -m_DotPitchX;	}

	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	const double OffsetX=((CountX-1)*m_DotPitchX);
	const double OffsetY=((CountY-1)*m_DotPitchY);	
	const double EndPosX = PosX+OffsetX;
	const double EndPosY = PosY+OffsetY;
	
	m_DotSkipX = SkipX;
	m_DotSkipY = SkipY;
	m_DotCountX = CountX;
	m_DotCountY = CountY;	
	m_DotStagePosHor.x = EndPosX;	
	m_DotStagePosHor.y = PosY;	
	m_DotStagePosVer.x = PosX;	
	m_DotStagePosVer.y = EndPosY;

	const int UsedCountX = GetDotCountX();
	const int UsedCountY = GetDotCountY();
	const double UsedPitchX = GetDotPitchX();
	const double UsedPitchY = GetDotPitchY();
	const double UsedOffsetX=((UsedCountX-1)*UsedPitchX);
	const double UsedOffsetY=((UsedCountY-1)*UsedPitchY);	
	const double UsedEndPosX = PosX+UsedOffsetX;
	const double UsedEndPosY = PosY+UsedOffsetY;

	m_DotCornerPos[DOT_CORNER_1].x = PosX;
	m_DotCornerPos[DOT_CORNER_1].y = PosY;
	m_DotCornerPos[DOT_CORNER_2].x = UsedEndPosX;
	m_DotCornerPos[DOT_CORNER_2].y = PosY;
	m_DotCornerPos[DOT_CORNER_3].x = UsedEndPosX;
	m_DotCornerPos[DOT_CORNER_3].y = UsedEndPosY;
	m_DotCornerPos[DOT_CORNER_4].x = PosX;
	m_DotCornerPos[DOT_CORNER_4].y = UsedEndPosY;

	m_DotMatchedCornerPos[DOT_CORNER_1] = m_DotCornerPos[DOT_CORNER_1];
	m_DotMatchedCornerPos[DOT_CORNER_2] = m_DotCornerPos[DOT_CORNER_2];
	m_DotMatchedCornerPos[DOT_CORNER_3] = m_DotCornerPos[DOT_CORNER_3];
	m_DotMatchedCornerPos[DOT_CORNER_4] = m_DotCornerPos[DOT_CORNER_4];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::UpdateDotParamFromUI()//更新UI至點參數
{
	CString str;
	CString strSizeW;
	CString strSizeH;
	CString strPitchX;
	CString strPitchY;	
	CAMERA_ID  CameraID=GetCameraID();	
	const double UnitRatio = GetUnitRatio();	
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const double ResX2 = (ResX+ResY)*0.5;
	const double ResY2 = (ResX+ResY)*0.5;
	const int    Threshold = (int)(CWnd::GetDlgItemInt(CALISTAGE_THRESHOLD_EDIT));
	DOT_ALIGN_MODE AlignMode = (DOT_ALIGN_MODE)(JetAPI::GetComboxCurSelData(m_AlignModeCombox));
	const bool bWhiteDot = (bool)(CWnd::IsDlgButtonChecked(CALISTAGE_DOT_WHITE_CHK));
	
	CWnd::GetDlgItemText(CALISTAGE_DOT_SIZE_W_EDIT, strSizeW);
	CWnd::GetDlgItemText(CALISTAGE_DOT_SIZE_H_EDIT, strSizeH);
	
	const double SizeW = ::_ttof(strSizeW)*UnitRatio;
	const double SizeH = ::_ttof(strSizeH)*UnitRatio;	
	const double SizeD = (SizeW+SizeH)*0.5;
	const int MarginW = (int)(CWnd::GetDlgItemInt(CALISTAGE_DOT_MARGIN_X_EDIT));
	const int MarginH = (int)(CWnd::GetDlgItemInt(CALISTAGE_DOT_MARGIN_Y_EDIT));
	const int ExtendW = (int)(CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_X_EDIT));
	const int ExtendH = (int)(CWnd::GetDlgItemInt(CALISTAGE_DOT_EXTEND_Y_EDIT));
	const int DotPatW = (int)((SizeW/ResX2)+0.5);
	const int DotPatH = (int)((SizeH/ResY2)+0.5);
	const int PatternW = DotPatW+MarginW+MarginW;
	const int PatternH = DotPatH+MarginH+MarginH;

	if ( MarginW<0 || MarginH<0 ) 
	{
		str = _T("Error, Margin Size is Exception");
		JetAPI::ShowMessageBox(str);
		return false;
	}

	OnSetTolBtn();
	m_Threshold= Threshold;
	m_DotWidth = SizeW;
	m_DotHeight = SizeH;
	m_DotSizeW = DotPatW;
	m_DotSizeH = DotPatH;
	m_DotMarginW = MarginW;
	m_DotMarginH = MarginH;
	m_DotAlignMode = AlignMode;
	m_DotWhiteMode = bWhiteDot;	
	m_DotPatternW = PatternW;
	m_DotPatternH = PatternH;	
	m_DotRoiSize.cx = PatternW+ExtendW+ExtendW;
	m_DotRoiSize.cy = PatternH+ExtendH+ExtendH;

	UpdateDotHorVerPos();
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMotionWndBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_MotionCtrlWnd.GetSafeHwnd() == NULL  ) { return; }
	BOOL bVisible = m_MotionCtrlWnd.IsWindowVisible();
	if ( FALSE == bVisible )
	{	m_MotionCtrlWnd.ShowWindow(SW_SHOW); }
	else
	{	m_MotionCtrlWnd.ShowWindow(SW_HIDE); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDotAlignBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CreateTempFolder() == false ) { return ; }
	if ( UpdateDotParamFromUI() == false ) { return ; }	
	if ( UpdateDotHorVerPos() == false ) { return; }
	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_DOT_ALIGN;	
	LockUIWnd(true);
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return; 
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnAlignX12Btn() 
{
	// TODO: Add your control notification handler code here
	if ( CreateTempFolder() == false ) { return ; }
	if ( UpdateDotHorVerPos() == false ) { return ; }
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}	

	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_X_POS_ALIGN;
	LockUIWnd(true);
	SetShowCaliStep(false);
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToX1Btn() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	LockUIWnd(true);	
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToX2Btn() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( UpdateDotHorVerPos() == false ) { return ; }
	const double PosX = m_DotStagePosHor.x;
	const double PosY = m_DotStagePosHor.y;	
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}	

	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_STOP;
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	//ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);	
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnVerifyXBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateDotHorVerPos();
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_X_POS_VERIFY_EACH;
	LockUIWnd(true);
	m_FuncTime.SetStart();
	SetShowCaliStep(false);	
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnAlignY12Btn() 
{
	// TODO: Add your control notification handler code here
	if ( CreateTempFolder() == false ) { return ; }
	if ( UpdateDotHorVerPos() == false ) { return ; }
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}	

	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_Y_POS_ALIGN;
	LockUIWnd(true);
	SetShowCaliStep(false);
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToY1Btn() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToY2Btn() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	if ( UpdateDotHorVerPos() == false ) { return ; }
	const double PosX = m_DotStagePosVer.x;
	const double PosY = m_DotStagePosVer.y;	
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}	

	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_STOP;
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	//ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);	
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnVerifyYBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateDotHorVerPos();
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_Y_POS_VERIFY_EACH;
	LockUIWnd(true);
	m_FuncTime.SetStart();
	SetShowCaliStep(false);
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveOffsetFile(bool bXaxis, std::vector<TPOINT2D> &List)
{
	CString str;
	CString filename;
	FILE   *pfile=NULL;

	if ( true == bXaxis )
	{	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("VerifyXAxisEachDot.TXT")); }
	else
	{	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("VerifyYAxisEachDot.TXT")); }

	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile )
	{
		str.Format(_T("Error, Open File Fault[%s]"), filename);
		return false;

	}

	size_t       i=0;
	TPOINT2D     Max2D;
	TPOINT2D     Min2D;	
	TPOINT2D     Ave2D;	
	TPOINT2D     Point2D;	
	const size_t Count = List.size();

	::_ftprintf(pfile, _T("%s, %s, %s\n"), _T("idx"), _T("OffsetX"), _T("OffsetY"));
	for ( i=0; i<Count; i++ )
	{
		Point2D = List[i];
		if ( 0==i )
		{	Ave2D = Min2D = Max2D = Point2D;	}
		else
		{
			Ave2D.x += Point2D.x;
			Ave2D.y += Point2D.y;

			if ( Min2D.x > Point2D.x ) { Min2D.x = Point2D.x; }
			if ( Min2D.y > Point2D.y ) { Min2D.y = Point2D.y; }
			if ( Max2D.x < Point2D.x ) { Max2D.x = Point2D.x; }
			if ( Max2D.y < Point2D.y ) { Max2D.y = Point2D.y; }
		}
		::_ftprintf(pfile, _T("%d, %.4f, %.4f\n"), i+1, Point2D.x, Point2D.y);
	}
	if ( Count > 0 )
	{
		Ave2D.x /= Count;
		Ave2D.y /= Count;
		::_ftprintf(pfile, _T("Min, %.4f, %.4f\n"), Min2D.x, Min2D.y);
		::_ftprintf(pfile, _T("Max, %.4f, %.4f\n"), Max2D.x, Max2D.y);
		::_ftprintf(pfile, _T("Ave, %.4f, %.4f\n"), Ave2D.x, Ave2D.y);
	}
	::fclose(pfile); pfile=NULL;

	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveDotNodeFile(std::vector<TDotNode> &List, bool bShowFd)
{	
	CString filename;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("VerifyDotNodeList.TXT"));
	if ( SaveDotNodeFile(filename, List, bShowFd) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveDotNodeFile(LPCTSTR filename, std::vector<TDotNode> &List, bool bShowFd, bool bShowFile)
{
	CString str;	
	FILE   *pfile=NULL;	
	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile )
	{
		str.Format(_T("Error, Open File Fault[%s]"), filename);
		return false;

	}

	size_t       i=0;
	TPOINT2D     Max2D;
	TPOINT2D     Min2D;	
	TPOINT2D     Ave2D;	
	TDotNode     DotNode;	
	const size_t Count = List.size();

	if ( true == bShowFd )
	{
		const size_t CadCnt=m_AlignedFdCadList.size();
		const size_t ResCnt=m_AlignedFdResList.size();
		if ( CadCnt==ResCnt && CadCnt>0 )
		{
			const DWORD FdTime=m_FuncTimeFd.GetFuncTime();
			::_ftprintf(pfile, _T("Fd Time=%d ms\n"),FdTime);

			::_ftprintf(pfile, _T("%s, %s, %s, %s, %s, %s, %s\n"), _T("Fdidx"), _T("PosX"), _T("PosY"), _T("ResX"), _T("ResY"), _T("OffsetX"), _T("OffsetY"));
			for ( i=0; i<CadCnt; i++ )
			{
				const TPOINT2D &CadPos=m_AlignedFdCadList[i];
				const TPOINT2D &ResPos=m_AlignedFdResList[i];
				const TPOINT2D DifPos(ResPos.x-CadPos.x, ResPos.y-CadPos.y);
				::_ftprintf(pfile, _T("%d, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), i+1, CadPos.x, CadPos.y, ResPos.x, ResPos.y, DifPos.x, DifPos.y);
			}
			::_ftprintf(pfile, _T("\n"));
		}
		ClearAlignedFdList();
	}

	const DWORD CalTime=m_FuncTime.GetFuncTime();
	::_ftprintf(pfile, _T("Time=%d ms\n"), CalTime);

	::_ftprintf(pfile, _T("%s, %s, %s, %s, %s, %s, %s\n"), _T("idx"), _T("PosX"), _T("PosY"), _T("AlignedX"), _T("AlignedY"), _T("OffsetX"), _T("OffsetY"));
	for ( i=0; i<Count; i++ )
	{
		DotNode = List[i];
		if ( 0==i )
		{	
			Ave2D.x = Min2D.x = Max2D.x = DotNode.dOffsetX;	
			Ave2D.y = Min2D.y = Max2D.y = DotNode.dOffsetY;	
		}
		else
		{
			Ave2D.x += DotNode.dOffsetX;
			Ave2D.y += DotNode.dOffsetY;

			if ( Min2D.x > DotNode.dOffsetX ) { Min2D.x = DotNode.dOffsetX; }
			if ( Min2D.y > DotNode.dOffsetY ) { Min2D.y = DotNode.dOffsetY; }
			if ( Max2D.x < DotNode.dOffsetX ) { Max2D.x = DotNode.dOffsetX; }
			if ( Max2D.y < DotNode.dOffsetY ) { Max2D.y = DotNode.dOffsetY; }
		}
		::_ftprintf(pfile, _T("%d, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), i+1, DotNode.dPosX, DotNode.dPosY, DotNode.dAlignedX, DotNode.dAlignedY, DotNode.dOffsetX, DotNode.dOffsetY);
	}
	if ( Count > 0 )
	{
		Ave2D.x /= Count;
		Ave2D.y /= Count;
		::_ftprintf(pfile, _T("Min, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 0.00, 0.00, 0.00, 0.00, Min2D.x, Min2D.y);
		::_ftprintf(pfile, _T("Max, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 0.00, 0.00, 0.00, 0.00, Max2D.x, Max2D.y);
		::_ftprintf(pfile, _T("Ave, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"), 0.00, 0.00, 0.00, 0.00, Ave2D.x, Ave2D.y);
	}
	::fclose(pfile); pfile=NULL;

	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnAlignCornerBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( CreateTempFolder() == false ) { return ; }
	if ( UpdateDotHorVerPos() == false ) { return ; }
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_ALIGN_CORNER;
	LockUIWnd(true);
	m_FuncTime.SetStart();
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::CheckCornerLimit()
{
	CString str;	

	//移動至四個端點, 避免超過極限
	double CornerPosX=0;
	double CornerPosY=0;
	for ( int i=0; i<DOT_CORNER_TOTAL; i++ )
	{		
		CornerPosX=m_DotCornerPos[i].x;
		CornerPosY=m_DotCornerPos[i].y;
		if ( MotionCtrlPtr->XYMoveTo(CornerPosX, CornerPosY) == false )
		{
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
			return false;
		}
		MotionCtrlPtr->WaitForMotionStop();
		::Sleep(100);
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::ExecCalibrateDotBtn()
{
	CString str;
	if ( CreateTempFolder() == false ) { return false; }
	UpdateDotHorVerPos();
	const double PosX = GetDotOrgStagePosX();
	const double PosY = GetDotOrgStagePosY();
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	LockUIWnd(true);
	m_FuncTime.SetStart();	
	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_CALIBRATE_DOT_NODE;	
	ConfigGrabParam(CaliMode);	
	SetCalibrationMode(CaliMode);
	const size_t DotNodeCount=m_DotNodeList.size();
	if ( DotNodeCount > 0 )
	{
		MotionCtrlPtr->WaitForMotionStop();	
		const TDotNode &DotNode = m_DotNodeList[0];		
		const double NextPosX = DotNode.dCaliPosX;
		const double NextPosY = DotNode.dCaliPosY;
		if ( ExecXYMoveTo(CaliMode, NextPosX, NextPosY) == false )	
		{	return false;	}
	}
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return false;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnCalibrateDotBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( CreateTempFolder() == false ) { return ; }
	UpdateDotHorVerPos();
	CString   strDotCount;
	CString   strDotPitch;
	CString   strExecVerify;
	CString   strExecCalibrate;	
	const int CountX = GetDotCountX();
	const int CountY = GetDotCountY();
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();			
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	
	strDotCount = _T("Dot Count");
	strDotCount = LoadMultiLanguageString(strDotCount, strDotCount);
	strDotPitch = _T("Dot Pitch");
	strDotPitch = LoadMultiLanguageString(strDotPitch, strDotPitch);
	strExecVerify = _T("Do you want to verify each DOT");
	strExecVerify = LoadMultiLanguageString(strExecVerify, strExecVerify);
	strExecCalibrate = _T("Do you want to calibrate each DOT");
	strExecCalibrate = LoadMultiLanguageString(strExecCalibrate, strExecCalibrate);			
	
	ClearAlignedFdList();
	m_FuncTimeFd.SetStart();
	m_AlignedMap.Identity();	
	SetAlignedMapUsed(false);		
	m_AlignedCount=(int)(JetAPI::GetComboxCurSelData(m_AlignCountCombox));

	SavePatImage();
	if ( FN_DISABLE == MotionParam.m_XYCaliEnable )
	{	
		str.Format(_T("%s (%d x %d)\n%s (%.0f, %.0f)\n%s?"), strDotCount, CountX, CountY, strDotPitch, PitchX, PitchY, strExecCalibrate); 
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return ;	}
	}
	else
	{	
		CString str1, str2;
		str2 = _T("Do you want to aligned the DOT board?");
		str2 = LoadMultiLanguageString(str2, str2);
		str1.Format(_T("%s (%d x %d)\n%s (%.0f, %.0f)\n%s?"), strDotCount, CountX, CountY, strDotPitch, PitchX, PitchY, strExecVerify); 
		str.Format(_T("%s\n%s"), str1, str2);
		DWORD Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		if ( IDCANCEL == Res )
		{	return; }
		if ( IDYES == Res )
		{
			SetAlignedMapUsed(true);
			OnAlignCornerBtn();
			return;
		}
	}		
	
	//移動至四個端點, 避免超過極限
	if ( CheckCornerLimit() == false )
	{	return ; }

	if ( ExecCalibrateDotBtn() == false )
	{	return; }
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::BuildStageXYCaliTable()//建立機台XY校正表
{	
	size_t       i=0;
	CString      filename;		
	const int    CountX=GetDotCountX();
	const int    CountY=GetDotCountY();
	std::vector<TXYCali> XYCaliList;		
	LANE_ID      ActLaneID = GetActiveLaneID();
	LANE_ID      XYCaliLaneID=GetActiveLaneID();

	filename = AOIDataCollect.GetDotNodeCaliFilename(ActLaneID);
	AOIDataCollect.SaveXYDotNodeCaliFile(filename, CountX, CountY, m_DotNodeList);	
	
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();	
	BOOL bUseDualLane = CWnd::IsDlgButtonChecked(CALISTAGE_USE_DUAL_LANE_CHK);

	if ( MULTI_LANE_1==MultiLaneMode || FALSE==bUseDualLane )
	{	AOIDataCollect.ConvertXYDotNodeToXYCali(CountX, CountY, m_DotNodeList, XYCaliList);		}
	else
	{
		CString filename2;
		int     CountX_LA=0, CountY_LA=0;
		int     CountX_LB=0, CountY_LB=0;		
		std::vector<TDotNode> DotNodeList_LA;
		std::vector<TDotNode> DotNodeList_LB;
		switch ( ActLaneID )
		{
		case LANE_ID_B:
			CountX_LB = CountX;
			CountY_LB = CountY;
			DotNodeList_LB = m_DotNodeList;
			filename2 = AOIDataCollect.GetDotNodeCaliFilename(LANE_ID_A);
			AOIDataCollect.LoadXYDotNodeCaliFile(filename2, CountX_LA, CountY_LA, DotNodeList_LA);
			break;
		default:
		case LANE_ID_A:
			CountX_LA = CountX;
			CountY_LA = CountY;
			DotNodeList_LA = m_DotNodeList;
			filename2 = AOIDataCollect.GetDotNodeCaliFilename(LANE_ID_B);
			AOIDataCollect.LoadXYDotNodeCaliFile(filename2, CountX_LB, CountY_LB, DotNodeList_LB);
			break;
		}	

		std::vector<TXYCali> XYCaliList_LA;
		std::vector<TXYCali> XYCaliList_LB;
		std::vector<TDotNode> DotNodeList_LA2;
		std::vector<TDotNode> DotNodeList_LB2;
		
		AOIDataCollect.RefineXYDotNodeListByLane(DotNodeList_LA, DotNodeList_LB, DotNodeList_LA2, DotNodeList_LB2);//提煉AB軌道的DotNodeList

		//因為只有縮短Y的資料, 所以不用管CountX, CountY
		AOIDataCollect.ConvertXYDotNodeToXYCali(CountX_LA, CountY_LA, DotNodeList_LA2, XYCaliList_LA);
		AOIDataCollect.ConvertXYDotNodeToXYCali(CountX_LB, CountY_LB, DotNodeList_LB2, XYCaliList_LB);

		const size_t XYCaliCont_LA = XYCaliList_LA.size();
		const size_t XYCaliCont_LB = XYCaliList_LB.size();

		if ( 0==XYCaliCont_LA || 0==XYCaliCont_LB ) 
		{	XYCaliLaneID = XYCaliLaneID; }
		else
		{ 	XYCaliLaneID = LANE_ID_BOTH; }

		XYCaliList.clear();
		for ( i=0; i<XYCaliCont_LA; i++ )
		{	XYCaliList.push_back(XYCaliList_LA[i]);	}
		for ( i=0; i<XYCaliCont_LB; i++ )
		{	XYCaliList.push_back(XYCaliList_LB[i]);	}
	}
	const bool Rebuild = true;
	MotionCtrlPtr->SetMotionXYCaliLaneID(XYCaliLaneID);
	MotionCtrlPtr->SetMotionXYYCaliList(XYCaliList, Rebuild);
	MotionCtrlPtr->SaveMotionXYCali();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::SaveStageDotCompareFile()//輸出機台Dot比較檔案
{
	CString      str;
	CString      Folder;
	CString      filename;
	CString      strDateTime;
	size_t       Res=0;	
	int          i=0, j=0;
	size_t       k=0;
	FILE        *pfile = NULL;		
	const int    CountX=GetDotCountX();
	const int    CountY=GetDotCountY();
	const double SizeW = m_DotWidth;
	const double SizeH = m_DotHeight;		
	const double PitchX = GetDotPitchX();
	const double PitchY = GetDotPitchY();	
	const double SizeD = (SizeW+SizeH)*0.5;
	CAMERA_ID    CameraID=GetCameraID();
	const size_t DotNodeCount = m_DotNodeList.size();
	const bool   bSignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   bSignY = AOIDataCollect.GetStageSignPositiveY();
	TDotNode     DotNode, *pDotNode=NULL;
	const double absPitchX = ::fabs(PitchX);
	const double absPitchY = ::fabs(PitchY);
	const LANE_ID LaneID = GetActiveLaneID();
	if ( fabs(absPitchX-absPitchY) > 0.001 )//Pitch 不同不輸出
	{	return true;	}

	char Caption[64]="";                 //可輸入任何要記錄的資訊	
	const char JET[8]=AOI3D_VENDOR_A;//"JET";             //辨識字元, 固定"JET"	
	unsigned char StageCoordinateX=0;    //X軸機台座標方向定義, R->L:0, L->R:1
	unsigned char StageCoordinateY=0;    //Y軸機台座標方向定義, T->B:0, B->T:1 
	const double  PixelResolutionX=AOIDataCollect.GetCameraResolutionX(CameraID); //影像解析度-X
	const double  PixelResolutionY=AOIDataCollect.GetCameraResolutionY(CameraID); //影像解析度-Y
	const unsigned short DotXCount=(unsigned short)(CountX);//X方向Dot數量
	const unsigned short DotYCount=(unsigned short)(CountY);//Y方向Dot數量
	const double  DotDia = SizeD/1000.0;          //Dot直徑mm
	const double  DotPitch = PitchX/1000.0;       //Dot間距mm
	TPOINT2F  PtCad;
	TPOINT2F  PtCali;
	std::vector<TPOINT2F> PtCadList;
	std::vector<TPOINT2F> PtCaliList;

	JetAPI::GetTime(strDateTime, m_DotDateTime);	
	str.Format(_T("%s:%s"), _T("Build"), strDateTime);
	JetAPI::TCHAR2char(str, Caption, sizeof(Caption));
	if ( true == bSignX ) { StageCoordinateX = 1; }
	else { StageCoordinateX = 0; }
	if ( true == bSignY ) { StageCoordinateY = 1; }
	else { StageCoordinateY = 0; }

	int CheckIndex_X=0;
	int CheckIndex_Y=0;
	PtCadList.clear();
	PtCaliList.clear();
	for ( i=0; i<CountY; i++ )
	{
		if ( true == bSignY )
		{	CheckIndex_Y = CountY-i-1; }
		else
		{	CheckIndex_Y = i;	}
		for ( j=0; j<CountX; j++ )
		{
			if ( true == bSignX )
			{	CheckIndex_X = CountX-j-1; }
			else
			{	CheckIndex_X = j;	}

			for ( k=0; k<DotNodeCount; k++ )
			{
				pDotNode = &(m_DotNodeList[k]);
				if ( NULL == pDotNode ) { continue; }
				if ( pDotNode->nIndexX != CheckIndex_X ) { continue; }
				if ( pDotNode->nIndexY != CheckIndex_Y ) { continue; }

				PtCad.x = (float)(pDotNode->dPosX);
				PtCad.y = (float)(pDotNode->dPosY);
				PtCali.x = (float)(pDotNode->dCaliPosX);
				PtCali.y = (float)(pDotNode->dCaliPosY);
				PtCadList.push_back(PtCad);
				PtCaliList.push_back(PtCali);
			}
		}
	}
	const size_t PtCadCount = PtCadList.size();
	const size_t PtCaliCount = PtCaliList.size();
	if ( PtCadCount != DotNodeCount ) 
	{
		str.Format(_T("Error, Cad Count Exception [%d/%d]"), PtCadCount, DotNodeCount);
		return false;
	}
	if ( PtCaliCount != DotNodeCount ) 
	{
		str.Format(_T("Error, Cali Count Exception [%d/%d]"), PtCaliCount, DotNodeCount);
		return false;
	}	
	
	Folder = AOIDataCollect.GetAOITempDirectory();
	filename.Format(_T("%s\\%s"), Folder, _T("DotCmpFile.dat"));
	pfile = ::_tfopen(filename, _T("wb"));
	if ( NULL == pfile )
	{
		str.Format(_T("Error, Open File To Save Fault[%s]"), filename);
		JetAPI::ShowMessageBox(str);
		return false;
	}	

	bool bException = false;
	while ( true )
	{
		Res = ::fwrite(JET, sizeof(JET), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [JET] Fault");
			break;
		}
		Res = ::fwrite(Caption, sizeof(Caption), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [Caption] Fault");
			break;
		}
		Res = ::fwrite(&StageCoordinateX, sizeof(StageCoordinateX), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [StageCoordinateX] Fault");
			break;
		}
		Res = ::fwrite(&StageCoordinateY, sizeof(StageCoordinateY), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [StageCoordinateY] Fault");
			break;
		}
		Res = ::fwrite(&PixelResolutionX, sizeof(PixelResolutionX), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [PixelResolutionX] Fault");
			break;
		}
		Res = ::fwrite(&PixelResolutionY, sizeof(PixelResolutionY), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [PixelResolutionY] Fault");
			break;
		}
		Res = ::fwrite(&DotXCount, sizeof(DotXCount), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [DotXCount] Fault");
			break;
		}
		Res = ::fwrite(&DotYCount, sizeof(DotYCount), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [DotYCount] Fault");
			break;
		}
		Res = ::fwrite(&DotDia, sizeof(DotDia), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [DotDia] Fault");
			break;
		}
		Res = ::fwrite(&DotPitch, sizeof(DotPitch), 1, pfile);
		if ( 0 == Res )
		{	
			bException = true;
			str = _T("Error, Write [DotPitch] Fault");
			break;
		}
		
		for ( k=0; k<PtCadCount; k++ )
		{
			PtCad = PtCadList[k];
			Res = ::fwrite(&PtCad, sizeof(PtCad), 1, pfile);
			if ( 0 == Res )
			{	
				bException = true;
				str = _T("Error, Write [PtCad] Fault");
				break;
			}
		}

		for ( k=0; k<PtCaliCount; k++ )
		{
			PtCali = PtCaliList[k];
			Res = ::fwrite(&PtCali, sizeof(PtCali), 1, pfile);
			if ( 0 == Res )
			{	
				bException = true;
				str = _T("Error, Write [PtCali] Fault");
				break;
			}
		}
		break;
	};

	::fclose(pfile); pfile = NULL;
	if ( true == bException )
	{		
		JetAPI::ShowMessageBox(str);
		return false;
	}

	CString filename2;
	CString LaneName2;
	CString ShortName2;
	CString KeyName2= _T("DotCmpFile");
	CString Folder2 = AOIDataCollect.GetAOIDirectory();		
	switch (LaneID)
	{
	case LANE_ID_B:	LaneName2 = _T("LB");	break;
	default:
	case LANE_ID_A:	LaneName2 = _T("LA");	break;
	}
#ifdef TB_SYSTEM_ONLY_BOT
	ShortName2.Format(_T("%s_%s_Bot"), KeyName2, LaneName2);	
#else
	ShortName2.Format(_T("%s_%s"), KeyName2, LaneName2);
#endif//TB_SYSTEM_ONLY_BOT
	filename2.Format(_T("%s\\%s.dat"), Folder2, ShortName2);
	
	::DeleteFile(filename2);
	::Sleep(0);
	::CopyFile(filename, filename2, FALSE);
	::Sleep(0);
	::DeleteFile(filename);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnBuildXYDotTableBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if ( false == m_DotNodeCanBeXYTable )
	{
		str = _T("Error, Can not build XY-Table while enabling XY-Cali Mode");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}	
	str.Format(_T("Do you want to Build XY Calibration Table?"));
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	BuildStageXYCaliTable();
	SaveStageDotCompareFile();
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnEnableXYCaliChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(CALISTAGE_ENABLE_XYCALI_CHK);
	TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	if ( TRUE == bChk )
	{	MotionParam.m_XYCaliEnable = FN_ENABLE; }
	else
	{	MotionParam.m_XYCaliEnable = FN_DISABLE;	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnItemchangedDotListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnClickDotListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDblclkDotListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const size_t DotNodeCount = m_DotNodeList.size();
	const int Index = (int)(m_DotListCtrl.GetItemData(nItem));
	if ( Index<0 || Index>=DotNodeCount ) { return; }

	CString  str;
	TDotNode DotNode=m_DotNodeList[Index];
	const double PosX = DotNode.dAlignedX;
	const double PosY = DotNode.dAlignedY;
	str.Format(_T("Index(%d, %d), Pos(%.0f, %.0f), Offset(%.0f, %.0f)"), DotNode.nIndexX+1, DotNode.nIndexY+1, DotNode.dPosX, DotNode.dPosY, DotNode.dOffsetX, DotNode.dOffsetY);
	SetInfoText(str);
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	ResetDotSearch();
	SetShowCaliStep(false);
	m_DotNodeIndex = Index;	
	SetRepeatGrabBtnCheck(false);	
	LockUIWnd(true);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnSetTolBtn() 
{
	// TODO: Add your control notification handler code here
	CString strTolDot;
	CString strTolHor;
	CString strTolVer;

	CWnd::GetDlgItemText(CALIBRATION_ALIGN_TOL_EDIT, strTolDot);
	CWnd::GetDlgItemText(CALIBRATION_ALIGN_HOR_TOL_EDIT, strTolHor);
	CWnd::GetDlgItemText(CALIBRATION_ALIGN_VER_TOL_EDIT, strTolVer);

	const double TolDot = (::_ttof(strTolDot));
	const double TolHor = (::_ttof(strTolHor));
	const double TolVer = (::_ttof(strTolVer));

	m_AlignTolerance_DOT = TolDot;
	m_AlignTolerance_HOR = TolHor;
	m_AlignTolerance_Ver = TolVer;
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnShowDotMapChk() 
{
	// TODO: Add your control notification handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDotErrSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString strErrX;
	CString strErrY;
	CWnd::GetDlgItemText(CALISTAGE_DOT_ERR_X_EDIT, strErrX);	
	CWnd::GetDlgItemText(CALISTAGE_DOT_ERR_Y_EDIT, strErrY);		

	m_ErrorOffsetX = ::_ttof(strErrX);
	m_ErrorOffsetY = ::_ttof(strErrY);
	
	size_t i=0;
	const double ErrorOffsetX = GetErrorOffsetX();
	const double ErrorOffsetY = GetErrorOffsetY();
	const size_t DotNodeCount = m_DotNodeList.size();	
	for ( i=0; i<DotNodeCount; i++ )
	{
		TDotNode &DotNode = m_DotNodeList[i];
		//在校正位置上取像後的總偏差值		
		if ( fabs(DotNode.dOffsetX)>ErrorOffsetX || fabs(DotNode.dOffsetY)>ErrorOffsetY ) 
		{	DotNode.eResultID = RESULT_ID_NG;	}
		else
		{	DotNode.eResultID = RESULT_ID_OK;	}		
	}
	UpdateDotListWnd();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::PickDotNodeList(POINT pt)
{
	int     i=0;
	CString str;
	double  PosX=0, PosY=0;
	const bool bLockUIWnd = GetLockUIWnd();
	const int DotNodeCount=(int)(m_DotNodeList.size());
	for ( i=0; i<DotNodeCount; i++ )
	{
		const TDotNode &DotNode=m_DotNodeList[i];
		if ( PtInRect(&DotNode.rcDot, pt) == FALSE ) { continue; }
		PosX = DotNode.dPosX;
		PosY = DotNode.dPosY;		
		EnsureVisibleDotListData(i);
		str.Format(_T("Index(%d, %d), Pos(%.0f, %.0f), Offset(%.0f, %.0f)"), DotNode.nIndexX+1, DotNode.nIndexY+1, DotNode.dPosX, DotNode.dPosY, DotNode.dOffsetX, DotNode.dOffsetY);
		SetInfoText(str);
		if ( false == bLockUIWnd )
		{
			ResetDotSearch();
			m_DotNodeIndex = i;
			SetShowCaliStep(false);
			SetRepeatGrabBtnCheck(false);
			MotionCtrlPtr->XYMoveTo(PosX, PosY);				
			LockUIWnd(true);
			SetCalibrationMode(CALIBRATION_STAGE_STOP);
			if ( ExecGrabFirst() == false )
			{
				LockUIWnd(false);
				SetCalibrationMode(CALIBRATION_STAGE_STOP);
				return true;		
			}
		}
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::UpdateImageValue(POINT pt)
{
	BOOL bShowDotMap = CWnd::IsDlgButtonChecked(CALISTAGE_SHOW_DOT_MAP_CHK);
	if ( TRUE == bShowDotMap ) { return; }
	if ( NULL == m_ImageBuffer ) { return; }
	
	TPOINT2D ImagePt;
	TPOINT2D WndPt=pt;
	ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	const int nImageX = (int)(ImagePt.x);
	const int nImageY = (int)(ImagePt.y);
	if ( nImageX<0 || nImageX>=m_ImageW )
	{	return;		}
	if ( nImageY<0 || nImageY>=m_ImageH )
	{	return;		}
	
	size_t Index = 0;
	int    R=0, G=0, B=0, Gray=0;
	if ( 24 == m_BitCount )
	{	
		Index = (nImageY*m_ImageStep)+(nImageX*3); 
		B = m_ImageBuffer[Index];
		G = m_ImageBuffer[Index+1];
		R = m_ImageBuffer[Index+2];
		m_strImageValue.Format(_T("RGB=(%d,%d,%d)"), R, G, B);
	}
	else
	{	
		Index = (nImageY*m_ImageStep)+(nImageX); 
		Gray = m_ImageBuffer[Index];
		m_strImageValue.Format(_T("Gray=(%d)"), Gray);
	}
	SetInfoText(m_strImageValue);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToCornerPosBtn1() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = m_DotMatchedCornerPos[DOT_CORNER_1].x;
	const double PosY = m_DotMatchedCornerPos[DOT_CORNER_1].y;
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);	
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToCornerPosBtn2() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = m_DotMatchedCornerPos[DOT_CORNER_2].x;
	const double PosY = m_DotMatchedCornerPos[DOT_CORNER_2].y;
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToCornerPosBtn3() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = m_DotMatchedCornerPos[DOT_CORNER_3].x;
	const double PosY = m_DotMatchedCornerPos[DOT_CORNER_3].y;
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnMoveToCornerPosBtn4() 
{
	// TODO: Add your control notification handler code here
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const double PosX = m_DotMatchedCornerPos[DOT_CORNER_4].x;
	const double PosY = m_DotMatchedCornerPos[DOT_CORNER_4].y;
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return ;
	}	
	LockUIWnd(true);
	ResetDotSearch();
	SetShowCaliStep(false);
	SetCalibrationMode(CALIBRATION_STAGE_STOP);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnAnalzeCornerBtn() 
{
	// TODO: Add your control notification handler code here
	AnalyzeCornerPos();	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnDotSaveBtn() 
{
	// TODO: Add your control notification handler code here	
	SaveParamFile();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnSaveImageBtn()
{
	// TODO: Add your control notification handler code here	
	SaveImageFile();	
}
//-------------------------------------------------------------------------------------//
int CCaliPaneStage::GetGantryAxis() const
{	
	return MotionCtrlPtr->GetGantryAxis();	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnGantryFetchOffsetBtn() 
{
	// TODO: Add your control notification handler code here	
	CString str;
	double Offset=0;
	const int Axis=GetGantryAxis();
	CWnd::SetDlgItemInt(CALISTAGE_GANTRY_FETCH_OFFSET_EDIT, 0);	
	if ( MotionCtrlPtr->FetchGantryOffset(Axis, Offset) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	CWnd::SetDlgItemInt(CALISTAGE_GANTRY_FETCH_OFFSET_EDIT, (int)(Offset));	
	return;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnGantrySetStdOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str, str1;
	const int Axis=GetGantryAxis();	
	const int Offset=(int)(CWnd::GetDlgItemInt(CALISTAGE_GANTRY_SET_STD_OFFSET_EDIT));
	str = _T("Do you want to set gantry standard offset");
	str = LoadMultiLanguageString(str, str);
	str1.Format(_T("%s [%d]?"), str, Offset);
	if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
	{	return ; }
	MotionCtrlPtr->SetGantryStdOffset(Axis, Offset);

	str=_T("Do you want to calibrate gantry offset?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{
		if ( MotionCtrlPtr->CorrectGantryOffset(Axis) == false )
			//if ( MotionCtrlPtr->CalibrateGantryOffset(Axis) == false )		
		{	str = MotionCtrlPtr->GetErrorString();	}
		else
		{	str = AOIDataDefine.GetFinishText();	}
		JetAPI::ShowMessageBox(str);
	}
	return;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneStage::AnalyzeAlignedMap()//分析座標轉換-玻璃板角度偏差
{	
	int      i=0;		
	TPOINT3D CornerPos[DOT_CORNER_TOTAL];//理論位置
	TPOINT3D MatchedPos[DOT_CORNER_TOTAL];//結果位置
	CMapCoordinate  MapSTC;
	CMapCoordinate &MapCTS=m_AlignedMap;	

	MapCTS.Identity();
	MapSTC.Identity();
	ClearAlignedFdList();
	SetAlignedMapUsed(false);	

	for ( i=0; i<DOT_CORNER_TOTAL; i++ )
	{
		CornerPos[i] = m_DotCornerPos[i];
		MatchedPos[i] = m_DotMatchedCornerPos[i];
	}

	int    Count=0;
	int    CornerIndex=0;
	double CornerPosX[DOT_CORNER_TOTAL];//偏移值-X
	double CornerPosY[DOT_CORNER_TOTAL];//偏移值-Y
	double MatchedPosX[DOT_CORNER_TOTAL];//理論位置-X
	double MatchedPosY[DOT_CORNER_TOTAL];//理論位置-Y
	::memset(CornerPosX, 0x00, sizeof(CornerPosX));
	::memset(CornerPosY, 0x00, sizeof(CornerPosY));
	::memset(MatchedPosX, 0x00, sizeof(MatchedPosX));
	::memset(MatchedPosY, 0x00, sizeof(MatchedPosY));
	const int NFdUse=m_AlignedCount;//2, 3, 4
	
	Count = 0;
	CornerIndex = 0;
	CornerPosX[Count]=CornerPos[CornerIndex].x;
	CornerPosY[Count]=CornerPos[CornerIndex].y;
	MatchedPosX[Count]=MatchedPos[CornerIndex].x;
	MatchedPosY[Count]=MatchedPos[CornerIndex].y;
	Count ++;

	if ( NFdUse > 1 )
	{
		if ( 2 == NFdUse )
		{	CornerIndex = 2;	}
		else
		{	CornerIndex = 1;	}	
		CornerPosX[Count]=CornerPos[CornerIndex].x;
		CornerPosY[Count]=CornerPos[CornerIndex].y;
		MatchedPosX[Count]=MatchedPos[CornerIndex].x;
		MatchedPosY[Count]=MatchedPos[CornerIndex].y;
		Count ++;

		if ( NFdUse > 2 )
		{		
			CornerIndex = 2;
			CornerPosX[Count]=CornerPos[CornerIndex].x;
			CornerPosY[Count]=CornerPos[CornerIndex].y;
			MatchedPosX[Count]=MatchedPos[CornerIndex].x;
			MatchedPosY[Count]=MatchedPos[CornerIndex].y;
			Count ++;

			if ( NFdUse > 3 )
			{		
				CornerIndex = 3;
				CornerPosX[Count]=CornerPos[CornerIndex].x;
				CornerPosY[Count]=CornerPos[CornerIndex].y;
				MatchedPosX[Count]=MatchedPos[CornerIndex].x;
				MatchedPosY[Count]=MatchedPos[CornerIndex].y;
				Count ++;
			}
		}
	}	

	for ( i=0; i<Count; i++ )
	{
		TPOINT2D CadPos(CornerPosX[i], CornerPosY[i]);
		TPOINT2D ResPos(MatchedPosX[i], MatchedPosY[i]);
		m_AlignedFdCadList.push_back(CadPos);		
		m_AlignedFdResList.push_back(ResPos);		
	}

	MapCTS.CalcMatrix2D(CornerPosX, CornerPosY, MatchedPosX, MatchedPosY, Count);
	MapSTC.CalcMatrix2D(MatchedPosX, MatchedPosY, CornerPosX, CornerPosY, Count);		
	
	TPOINT3D CornerPosRes[DOT_CORNER_TOTAL];//理論位置
	TPOINT3D MatchedPosRes[DOT_CORNER_TOTAL];//結果位置
	for ( i=0; i<DOT_CORNER_TOTAL; i++ )
	{
		MapCTS.Map2D(CornerPos[i].x, CornerPos[i].y, MatchedPosRes[i].x, MatchedPosRes[i].y);	
		MapSTC.Map2D(MatchedPos[i].x, MatchedPos[i].y, CornerPosRes[i].x, CornerPosRes[i].y);	
	}

	TPOINT3D CornerPosErr[DOT_CORNER_TOTAL];//理論位置
	TPOINT3D MatchedPosErr[DOT_CORNER_TOTAL];//結果位置
	for ( i=0; i<DOT_CORNER_TOTAL; i++ )
	{
		CornerPosErr[i].x = CornerPosRes[i].x-CornerPos[i].x;
		CornerPosErr[i].y = CornerPosRes[i].y-CornerPos[i].y;
		
		MatchedPosErr[i].x = MatchedPosRes[i].x-MatchedPos[i].x;
		MatchedPosErr[i].y = MatchedPosRes[i].y-MatchedPos[i].y;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneStage::OnAlignCameraBtn()
{
	// TODO: Add your control notification handler code here	
	if ( CreateTempFolder() == false ) { return ; }
	if ( UpdateDotParamFromUI() == false ) { return ; }	
	if ( UpdateDotHorVerPos() == false ) { return; }

	CALIBRATION_STAGE_MODE CaliMode = CALIBRATION_STAGE_DOT_CAMERA;
	LockUIWnd(true);
	SetShowCaliStep(true);
	m_ShowDotMatched = false;
	ConfigGrabParam(CaliMode);
	SetCalibrationMode(CaliMode);
	if ( ExecGrabFirst() == false )
	{
		LockUIWnd(false);
		SetCalibrationMode(CALIBRATION_STAGE_STOP);
		return;		
	}
	return ;	
}
//-------------------------------------------------------------------------------------//
