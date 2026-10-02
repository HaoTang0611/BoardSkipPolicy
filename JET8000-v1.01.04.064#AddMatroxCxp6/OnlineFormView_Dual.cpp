// OnlineFormView_Dual.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "OnlineFormView_Dual.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define TAB_INDEX_INFO                     0
#define TAB_INDEX_YIELD_BAR                1
#define TAB_INDEX_TOP_10                   2
#define TAB_INDEX_XY_CHART                 3
#define TAB_INDEX_DEFECT_PIE               4
//-------------------------------------------------------------------------------------//
#define DEFECT_MODE_BY_CURRENT            1//現今瑕疵
#define DEFECT_MODE_BY_STATISTIC          2//累計瑕疵
//-------------------------------------------------------------------------------------//
#define ONLINE_VIEW_TIMER_UPDATE_MACHINE_STATE          100//更新機台狀態
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView_Dual
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(COnlineFormView_Dual, CFormView)
//-------------------------------------------------------------------------------------//
COnlineFormView_Dual::COnlineFormView_Dual()
	: CFormView(COnlineFormView_Dual::IDD)
{
	//{{AFX_DATA_INIT(COnlineFormView_Dual)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LaneID = LANE_ID_A;
	m_ProjectPtr_LA = NULL;
	m_ProjectPtr_LB = NULL;
	ResetMachineStates();	
}
//-------------------------------------------------------------------------------------//
COnlineFormView_Dual::~COnlineFormView_Dual()
{
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlineFormView_Dual)
	DDX_Control(pDX, ONLINE_PROJECT_ID_COMBO_LA, m_ProjectIDCombox_LA);
	DDX_Control(pDX, ONLINE_PROJECT_ID_COMBO_LB, m_ProjectIDCombox_LB);	
	DDX_Control(pDX, ONLINE_LANE_ICON_LA, m_LaneIcon_LA);
	DDX_Control(pDX, ONLINE_LANE_ICON_LB, m_LaneIcon_LB);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_ICON_LA, m_ResultIcon_LA);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_ICON_LB, m_ResultIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_ICON_LA, m_NextStationSendIcon_LA);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_ICON_LB, m_NextStationSendIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_OK_ICON_LA, m_NextStationSendOKIcon_LA);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_OK_ICON_LB, m_NextStationSendOKIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_NG_ICON_LA, m_NextStationSendNGIcon_LA);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_NG_ICON_LB, m_NextStationSendNGIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_RECIEVE_ICON_LA, m_NextStationRecieveIcon_LA);
	DDX_Control(pDX, ONLINE_NEXT_STATION_RECIEVE_ICON_LB, m_NextStationRecieveIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_IN_ICON_LA, m_LaneSensorPCBInIcon_LA);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_IN_ICON_LB, m_LaneSensorPCBInIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LA, m_LaneSensorSlowDownIcon_LA);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LB, m_LaneSensorSlowDownIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LA, m_LaneSensorPCBStopIcon_LA);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB, m_LaneSensorPCBStopIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON2_LA, m_LaneSensorPCBStopIcon2_LA);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON2_LB, m_LaneSensorPCBStopIcon2_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LA, m_LaneSensorPCBOutIcon_LA);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LB, m_LaneSensorPCBOutIcon_LB);
	DDX_Control(pDX, ONLINE_LAST_STATION_SEND_ICON_LA, m_LastStationSendIcon_LA);
	DDX_Control(pDX, ONLINE_LAST_STATION_SEND_ICON_LB, m_LastStationSendIcon_LB);
	DDX_Control(pDX, ONLINE_LAST_STATION_RECIEVE_ICON_LA, m_LastStationRecieveIcon_LA);
	DDX_Control(pDX, ONLINE_LAST_STATION_RECIEVE_ICON_LB, m_LastStationRecieveIcon_LB);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_LIST_WND_LA, m_ResultListCtrl_LA);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_LIST_WND_LB, m_ResultListCtrl_LB);	
	DDX_Control(pDX, ONLINE_INFO_LIST_WND_LA, m_InfoListWnd_LA);
	DDX_Control(pDX, ONLINE_INFO_LIST_WND_LB, m_InfoListWnd_LB);	
	DDX_Control(pDX, ONLINE_YIELDING_LIST_WND_LA, m_YieldingListWnd_LA);	
	DDX_Control(pDX, ONLINE_YIELDING_LIST_WND_LB, m_YieldingListWnd_LB);	
	DDX_Control(pDX, ONLINE_YIELDING_SCOPE_COMBO_LA, m_YieldScopeCombox_LA);
	DDX_Control(pDX, ONLINE_YIELDING_SCOPE_COMBO_LB, m_YieldScopeCombox_LB);
	DDX_Control(pDX, ONLINE_TOP10_SCOPE_COMBO_LA, m_Top10ScopeCombox_LA);
	DDX_Control(pDX, ONLINE_TOP10_SCOPE_COMBO_LB, m_Top10ScopeCombox_LB);
	DDX_Control(pDX, ONLINE_DEFECT_FROM_COMBO_LA, m_DefectFromCombox_LA);	
	DDX_Control(pDX, ONLINE_DEFECT_FROM_COMBO_LB, m_DefectFromCombox_LB);	
	DDX_Control(pDX, ONLINE_CHART_WND_DEFECT_LA, m_ChartWnd_Defect_LA);
	DDX_Control(pDX, ONLINE_CHART_WND_DEFECT_LB, m_ChartWnd_Defect_LB);
	DDX_Control(pDX, ONLINE_CHART_WND_XY_LA, m_ChartWnd_XYChart_LA);
	DDX_Control(pDX, ONLINE_CHART_WND_XY_LB, m_ChartWnd_XYChart_LB);
	DDX_Control(pDX, ONLINE_CHART_WND_TOP10_LA, m_ChartWnd_Top10_LA);
	DDX_Control(pDX, ONLINE_CHART_WND_TOP10_LB, m_ChartWnd_Top10_LB);
	DDX_Control(pDX, ONLINE_CHART_WND_YIELDING_LA, m_ChartWnd_Yielding_LA);
	DDX_Control(pDX, ONLINE_CHART_WND_YIELDING_LB, m_ChartWnd_Yielding_LB);
	DDX_Control(pDX, ONLINE_CHART_TAB_LA, m_ChartTab_LA);
	DDX_Control(pDX, ONLINE_CHART_TAB_LB, m_ChartTab_LB);
	DDX_Control(pDX, ONLINE_DEFECT_MODE_COMBO_LA, m_DefectModeCombox_LA);
	DDX_Control(pDX, ONLINE_DEFECT_MODE_COMBO_LB, m_DefectModeCombox_LB);
	DDX_Control(pDX, ONLINE_DEFECT_LIST_WND_LA, m_DefectListCtrl_LA);
	DDX_Control(pDX, ONLINE_DEFECT_LIST_WND_LB, m_DefectListCtrl_LB);
	DDX_Control(pDX, ONLINE_PROJECT_INFO_LIST_WND_LA, m_ProjectInfoListCtrl_LA);
	DDX_Control(pDX, ONLINE_PROJECT_INFO_LIST_WND_LB, m_ProjectInfoListCtrl_LB);
	DDX_Control(pDX, ONLINE_IMAGE_WND_LA, m_ImageWnd_LA);
	DDX_Control(pDX, ONLINE_IMAGE_WND_LB, m_ImageWnd_LB);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(COnlineFormView_Dual, CFormView)
	//{{AFX_MSG_MAP(COnlineFormView_Dual)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(ONLINE_DEFECT_MODE_COMBO_LA, OnSelchangeDefectModeCombo_LA)
	ON_CBN_SELCHANGE(ONLINE_DEFECT_MODE_COMBO_LB, OnSelchangeDefectModeCombo_LB)
	ON_NOTIFY(TCN_SELCHANGE, ONLINE_CHART_TAB_LA, OnSelchangeChartTab_LA)
	ON_NOTIFY(TCN_SELCHANGE, ONLINE_CHART_TAB_LB, OnSelchangeChartTab_LB)
	ON_CBN_SELCHANGE(ONLINE_DEFECT_FROM_COMBO_LA, OnSelchangeDefectFromCombo_LA)
	ON_CBN_SELCHANGE(ONLINE_DEFECT_FROM_COMBO_LB, OnSelchangeDefectFromCombo_LB)
	ON_CBN_SELCHANGE(ONLINE_TOP10_SCOPE_COMBO_LA, OnSelchangeTop10ScopeCombo_LA)
	ON_CBN_SELCHANGE(ONLINE_TOP10_SCOPE_COMBO_LB, OnSelchangeTop10ScopeCombo_LB)
	ON_BN_CLICKED(ONLINE_CLEAR_STATISTIC_BTN_LA, OnClearStatisticBtn_LA)
	ON_BN_CLICKED(ONLINE_CLEAR_STATISTIC_BTN_LB, OnClearStatisticBtn_LB)
	ON_CBN_SELCHANGE(ONLINE_YIELDING_SCOPE_COMBO_LA, OnSelchangeYieldingScopeCombo_LA)	
	ON_CBN_SELCHANGE(ONLINE_YIELDING_SCOPE_COMBO_LB, OnSelchangeYieldingScopeCombo_LB)	
	ON_CBN_SELCHANGE(ONLINE_PROJECT_ID_COMBO_LA, OnSelchangeProjectIDCombo_LA)	
	ON_CBN_SELCHANGE(ONLINE_PROJECT_ID_COMBO_LB, OnSelchangeProjectIDCombo_LB)	
	ON_BN_CLICKED(ONLINE_PROJECT_CLOSE_BTN_LA, OnProjectCloseBtnLA)
	ON_BN_CLICKED(ONLINE_PROJECT_CLOSE_BTN_LB, OnProjectCloseBtnLB)

	ON_BN_CLICKED(ONLINE_LANE_RUN_BTN_LA, OnBnClickedLaneRunBtnLA)
	ON_BN_CLICKED(ONLINE_LANE_RUN_BTN_LB, OnBnClickedLaneRunBtnLB)
	ON_BN_CLICKED(ONLINE_LANE_STOP_BTN_LA, OnBnClickedLaneStopBtnLA)
	ON_BN_CLICKED(ONLINE_LANE_STOP_BTN_LB, OnBnClickedLaneStopBtnLB)
	ON_BN_CLICKED(ONLINE_LANE_BYPASS_BTN_LA, OnBnClickedLaneBypassBtnLA)
	ON_BN_CLICKED(ONLINE_LANE_BYPASS_BTN_LB, OnBnClickedLaneBypassBtnLB)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView_Dual diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void COnlineFormView_Dual::AssertValid() const
{
	CFormView::AssertValid();
}

void COnlineFormView_Dual::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView_Dual message handlers
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	AdjustCtrlWnd();
	CString str;	

	InitImageWnd(m_ImageWnd_LA);
	InitImageWnd(m_ImageWnd_LB);

	m_ImageWnd_LA.SetShowLaneID(LANE_ID_A);
	m_ImageWnd_LB.SetShowLaneID(LANE_ID_B);
	m_DefectListCtrl_LA.SetOwner(this);
	m_DefectListCtrl_LB.SetOwner(this);
	m_ProjectInfoListCtrl_LA.SetOwner(this);
	m_ProjectInfoListCtrl_LB.SetOwner(this);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	JetAPI::InitialListCtrl(m_DefectListCtrl_LA);	
	JetAPI::InitialListCtrl(m_DefectListCtrl_LB);	
	JetAPI::InitialListCtrl(m_ResultListCtrl_LA);
	JetAPI::InitialListCtrl(m_ResultListCtrl_LB);
	JetAPI::InitialListCtrl(m_ProjectInfoListCtrl_LA);	
	JetAPI::InitialListCtrl(m_ProjectInfoListCtrl_LB);	
	JetAPI::InitialListCtrl(m_InfoListWnd_LA);
	JetAPI::InitialListCtrl(m_InfoListWnd_LB);	
	JetAPI::InitialListCtrl(m_YieldingListWnd_LA);
	JetAPI::InitialListCtrl(m_YieldingListWnd_LB);

	str = AOIDataDefine.GetOnlineLaneImageName(LANE_ID_A);
	m_DibLane_LA.Load(str);	
	str = AOIDataDefine.GetOnlineLaneImageName(LANE_ID_B);
	m_DibLane_LB.Load(str);
	str = AOIDataDefine.GetTestResultImageName(TEST_RESULT_NONE);
	m_DibResult_LA.Load(str);	
	m_DibResult_LB.Load(str);	
	m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);
	m_LEDYellow.LoadBitmap(IDB_LED_MEDIAN_YELLOW);	
	SwitchMultiLanguage();

	InitChartTab(m_ChartTab_LA);
	InitChartTab(m_ChartTab_LB);
	InitChartWnd(LANE_ID_A);	
	InitChartWnd(LANE_ID_B);	
	BuildResultListWndHeader(m_ResultListCtrl_LA);
	BuildResultListWndHeader(m_ResultListCtrl_LB);
	BuildDefectListWndHeader(m_DefectListCtrl_LA);	
	BuildDefectListWndHeader(m_DefectListCtrl_LB);	
	BuildProjectInfoListWndHeader(m_ProjectInfoListCtrl_LA);
	BuildProjectInfoListWndHeader(m_ProjectInfoListCtrl_LB);
	
	BuildProjectIDCombox(LANE_ID_A, m_ProjectIDCombox_LA);
	BuildProjectIDCombox(LANE_ID_B, m_ProjectIDCombox_LB);
	BuildDefectModeCombox(m_DefectModeCombox_LA);	
	BuildDefectModeCombox(m_DefectModeCombox_LB);	

	CWnd::SetTimer(ONLINE_VIEW_TIMER_UPDATE_MACHINE_STATE, 100, NULL);
	CWnd::PostMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_INITIAL_UPDATE, NULL);
}
//-------------------------------------------------------------------------------------//
BOOL COnlineFormView_Dual::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT COnlineFormView_Dual::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	HWND  hWnd = NULL;
	const bool InspectionDrawing = AOIDataCollect.GetInspectionDrawing();
	switch ( message )
	{	
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:		
		case WPARAM_PROJECT_UPDATE:
			SwitchProject();
			break;
		case WPARAM_PROJECT_CLOSE:		
			CloseProject();
			if ( CWnd::IsWindowVisible() == TRUE )
			{	RedrawWnd(true); }
			break;
		case WPARAM_PROJECT_PART_SELECTED:			
			break;
		case WPARAM_PROJECT_PART_DELETED:			
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:			
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			SwitchProject();
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:			
			break;		
		case WPARAM_PROJECT_SWITCH_MAP:
			SwitchLane();
			SwitchProjectMap();
			break;
		case WPARAM_PROJECT_SWITCH_LANE:
			SwitchLane();
			break;
		case WPARAM_UPDATE_MACHINE_STATES:			
			UpdateMachineStates(true);
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_INITIAL_UPDATE:
			ExecSelchangeChartTab(LANE_ID_A, m_ChartTab_LA);	
			ExecSelchangeChartTab(LANE_ID_B, m_ChartTab_LB);
			SwitchProject();	
			ExecInspection_Finish();		
			break;
		case WPARAM_SWITCH_FRAME_IMAGE:						
			RedrawWnd(FALSE);			
			break;
		case WPARAM_UPDATE_VIEW_PART_SELECTED:			
			break;
		case WPARAM_REDRAW_VIEW_WND:
			RedrawWnd(FALSE);
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_UPDATE_ALG_IMAGE:
			break;
		case WPARAM_EXEC_WND_INSPECT:			
			break;
		case WPARAM_SHOW_WND_POSITION:			
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			m_ImageWnd_LB.UpdateProjectMap();
			m_ImageWnd_LA.UpdateProjectMap();
			RedrawWnd(FALSE);
			break;
		case WPARAM_REDRAW_PROJECT_MAP:
			if ( NULL == lParam )
			{	RedrawWnd(FALSE); }
			else if ( true == InspectionDrawing )
			{	RedrawWnd(FALSE); }
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_UPDATE_PROJECT_TEST_MAP:			
			m_ImageWnd_LB.UpdateProjectTestMap();
			m_ImageWnd_LA.UpdateProjectTestMap();
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			switch ( lParam )
			{
			case LPARAM_DRAW_PROJECT_MODE_NORMAL:
				m_ImageWnd_LA.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
				m_ImageWnd_LB.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);				
				RedrawWnd(FALSE);
				break;
			case LPARAM_DRAW_PROJECT_MODE_INSPECTING:
				m_ImageWnd_LA.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
				m_ImageWnd_LB.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);				
				RedrawWnd(FALSE);
				break;
			}
		}
		break;	
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecMoveToStage();		
		break;
	case MSG_INSPECTION_CALLBACK:
		switch ( wParam )
		{		
		case WPARAM_INSPECTION_PROJECT_RESET:
			ExecUpdateProject();
			break;
		case WPARAM_INSPECTION_FINISH://檢測狀態-檢測結束
			ExecInspection_Finish();
			break;
		case WPARAM_INSPECTION_ONLINE_FINISH:
			ExecOnlineInspection_Finish();
			break;
		}		
		break;	
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		LockUIWnd(false);
		m_ImageWnd_LA.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
		m_ImageWnd_LB.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);		
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		break;	
	}
	return CFormView::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustCtrlWnd();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{		
		AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
		ExecSelchangeChartTab(LANE_ID_A, m_ChartTab_LA);
		ExecSelchangeChartTab(LANE_ID_B, m_ChartTab_LB);
	}	
	else
	{
		int a = 3;
		a = 7;
	}
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case ONLINE_VIEW_TIMER_UPDATE_MACHINE_STATE:
		UpdateMachineStates(false);
		break;
	}
	CFormView::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::AdjustCtrlWnd()//調整控制項檢測框位置
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return true; }
	if ( CWnd::IsWindowVisible() == FALSE )  { return true; }

	POINT WndOffset={0};
	SIZE WndSize={0};
	RECT LaneRect={0};
	RECT FullRect={0};
	RECT ChartRect={0};	
	RECT ChartTabRect={0};
	RECT TopComboRect={0};	
	CWnd *WndPtr = NULL;
	const int MarginX=4;
	const int MarginY=4;
	CWnd::GetClientRect(&FullRect);	
	WndSize.cx = FullRect.right-FullRect.left;
	WndSize.cy = FullRect.bottom-FullRect.top;
	if ( 0==WndSize.cx || 0==WndSize.cy ) 
	{	return true; }

	RECT FullRectL=FullRect;
	RECT FullRectR=FullRect;
	const int FullW = FullRect.right-FullRect.left;
	const int FullH = FullRect.bottom-FullRect.top;
	const int FullW2 = FullW/2;
	const int FullH2 = FullH/2;
	FullRectL.right = FullRectL.left+FullW2;
	FullRectR.left = FullRectL.right;
	if ( AdjustCtrlWnd_LA(FullRectL) == false )
	{	return false; }
	if ( AdjustCtrlWnd_LB(FullRectR) == false )
	{	return false; }
	
	BOOL bShowLaneCtrl = FALSE;
#ifdef OFFLINE_VERSION
	bShowLaneCtrl = FALSE;
#endif//OFFLINE_VERSION
	if ( FALSE == bShowLaneCtrl ) 
	{	CWnd::Invalidate(); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::AdjustCtrlWnd_LA(RECT &FullRect)//調整控制項檢測框位置
{
	POINT WndOffset={0};
	SIZE WndSize={0};
	RECT TabRect={0};	
	RECT LaneRect={0};	
	RECT ChartRect={0};	
	RECT ChartTabRect={0};
	RECT TopComboRect={0};
	RECT FromComboRect={0};
	CWnd *WndPtr = NULL;
	const int MarginX=4;
	const int MarginY=4;
	//先找個控制項來確定是否可以進入調整
	WndPtr = CWnd::GetDlgItem(ONLINE_DEFECT_FROM_COMBO_LA);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&FromComboRect);
		CWnd::ScreenToClient(&FromComboRect);
	}

	if ( m_YieldScopeCombox_LA.GetSafeHwnd() == NULL )
	{	return true; }

	//Lane A	
	if ( m_YieldScopeCombox_LA.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldScopeCombox_LA.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		TopComboRect = WndRect;
		ChartRect.top = TopComboRect.bottom + MarginY;
	}
	if ( m_Top10ScopeCombox_LA.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=TopComboRect;
		m_Top10ScopeCombox_LA.MoveWindow(&WndRect);		
	}
	ChartRect.bottom = FullRect.bottom - MarginY;

	CThisChartCtrl &ChartWnd1 = m_ChartWnd_Yielding_LA;
	if ( ChartWnd1.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		ChartWnd1.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		ChartRect.left = WndRect.left;
		ChartRect.right = WndRect.right;
		WndRect = ChartRect;		
		ChartWnd1.MoveWindow(&WndRect);		
	}
	ChartTabRect = ChartRect;
	if ( m_ChartTab_LA.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_ChartTab_LA.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = ChartRect.right;
		ChartTabRect.left = WndRect.left;
		TabRect = WndRect;
		//m_ChartTab_LA.MoveWindow(&WndRect);		
	}

	CThisChartCtrl &ChartWnd2 = m_ChartWnd_Top10_LA;
	if ( ChartWnd2.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartRect;
		WndRect.top = TopComboRect.bottom+MarginY;
		WndRect.left = ChartTabRect.left;
		ChartWnd2.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd3 = m_ChartWnd_XYChart_LA;
	if ( ChartWnd3.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd3.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd4 = m_ChartWnd_Defect_LA;
	if ( ChartWnd4.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd4.MoveWindow(&WndRect);		
	}
	if ( m_InfoListWnd_LA.GetSafeHwnd() != NULL )
	{	
		RECT  WndRect=ChartTabRect;		
		WndRect.top = TabRect.bottom+MarginY;
		WndRect.right = FromComboRect.left-MarginX;
		m_InfoListWnd_LA.MoveWindow(&WndRect);
	}	
	if ( m_YieldingListWnd_LA.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldingListWnd_LA.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.bottom = ChartRect.bottom;
		m_YieldingListWnd_LA.MoveWindow(&WndRect);
	}
	if ( m_LaneIcon_LA.GetSafeHwnd() != NULL )
	{
		m_LaneIcon_LA.ShowWindow(false);
		/*RECT  WndRect={0};
		m_LaneIcon_LA.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = FullRect.right-MarginX;
		WndRect.bottom = FullRect.bottom-MarginY;
		m_LaneIcon_LA.MoveWindow(&WndRect);*/
	}
	
	CImageWnd &ImageWnd=m_ImageWnd_LA;
	if ( ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		//WndRect.left = FullRect.left;
		WndRect.top = FullRect.top+MarginY;
		WndRect.right = FullRect.right-MarginX;
		//WndRect.bottom = FullRect.bottom - MarginY;
		//WndRect.bottom = WndRect.top + 532;
		ImageWnd.MoveWindow(&WndRect);
		ImageWnd.ShowFittedZoom();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::AdjustCtrlWnd_LB(RECT &FullRect)//調整控制項檢測框位置
{
	POINT WndOffset={0};
	SIZE WndSize={0};	
	RECT TabRect={0};	
	RECT LaneRect={0};	
	RECT ChartRect={0};	
	RECT ChartTabRect={0};
	RECT TopComboRect={0};	
	RECT FromComboRect={0};		
	CWnd *WndPtr = NULL;
	const int MarginX=4;
	const int MarginY=4;
	//先找個控制項來確定是否可以進入調整
	WndPtr = CWnd::GetDlgItem(ONLINE_DEFECT_FROM_COMBO_LB);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&FromComboRect);
		CWnd::ScreenToClient(&FromComboRect);
	}

	if ( m_YieldScopeCombox_LB.GetSafeHwnd() == NULL )
	{	return true; }

	//Lane B
	if ( m_YieldScopeCombox_LB.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldScopeCombox_LB.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		TopComboRect = WndRect;
		ChartRect.top = TopComboRect.bottom + MarginY;
	}
	if ( m_Top10ScopeCombox_LB.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=TopComboRect;
		m_Top10ScopeCombox_LB.MoveWindow(&WndRect);		
	}
	ChartRect.bottom = FullRect.bottom - MarginY;

	CThisChartCtrl &ChartWnd1 = m_ChartWnd_Yielding_LB;
	if ( ChartWnd1.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		ChartWnd1.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		ChartRect.left = WndRect.left;
		ChartRect.right = WndRect.right;
		WndRect = ChartRect;		
		ChartWnd1.MoveWindow(&WndRect);		
	}
	ChartTabRect = ChartRect;
	if ( m_ChartTab_LB.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_ChartTab_LB.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = ChartRect.right;
		ChartTabRect.left = WndRect.left;
		TabRect = WndRect;
		//m_ChartTab_LB.MoveWindow(&WndRect);		
	}

	CThisChartCtrl &ChartWnd2 = m_ChartWnd_Top10_LB;
	if ( ChartWnd2.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartRect;
		WndRect.top = TopComboRect.bottom+MarginY;
		WndRect.left = ChartTabRect.left;
		ChartWnd2.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd3 = m_ChartWnd_XYChart_LB;
	if ( ChartWnd3.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd3.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd4 = m_ChartWnd_Defect_LB;
	if ( ChartWnd4.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd4.MoveWindow(&WndRect);		
	}	
	if ( m_InfoListWnd_LB.GetSafeHwnd() != NULL )
	{	
		RECT  WndRect=ChartTabRect;		
		WndRect.top = TabRect.bottom+MarginY;
		WndRect.right = FromComboRect.left-MarginX;
		m_InfoListWnd_LB.MoveWindow(&WndRect);
	}
	if ( m_YieldingListWnd_LB.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldingListWnd_LB.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.bottom = ChartRect.bottom;
		m_YieldingListWnd_LB.MoveWindow(&WndRect);
	}
	if ( m_LaneIcon_LB.GetSafeHwnd() != NULL )
	{
		m_LaneIcon_LB.ShowWindow(false);
		/*RECT  WndRect={0};
		m_LaneIcon_LB.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = FullRect.right-MarginX;
		WndRect.bottom = FullRect.bottom-MarginY;
		m_LaneIcon_LB.MoveWindow(&WndRect);*/
	}
	
	CImageWnd &ImageWnd=m_ImageWnd_LB;
	if ( ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		//WndRect.left = FullRect.left;
		WndRect.top = FullRect.top+MarginY;
		WndRect.right = FullRect.right-MarginX;
		//WndRect.bottom = FullRect.bottom - MarginY;
		//WndRect.bottom = WndRect.top + 532;
		ImageWnd.MoveWindow(&WndRect);
		ImageWnd.ShowFittedZoom();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ONLINE_FORMVIEW_DUAL");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ONLINE_FORMVIEW_DUAL;
	WndKey = _T("IDD_ONLINE_FORMVIEW_DUAL");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = ONLINE_PROJECT_ID_LABEL_LA;
	WndKey = _T("ONLINE_PROJECT_ID_LABEL_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_PROJECT_INFO_GROUP_LA;
	WndKey = _T("ONLINE_PROJECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ONLINE_DEFECT_INFO_GROUP_LA;
	WndKey = _T("ONLINE_DEFECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_CLEAR_STATISTIC_BTN_LA;
	WndKey = _T("ONLINE_CLEAR_STATISTIC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = ONLINE_PROJECT_RESULT_GROUP_LA;
	WndKey = _T("ONLINE_PROJECT_RESULT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = ONLINE_PROJECT_ID_LABEL_LB;
	WndKey = _T("ONLINE_PROJECT_ID_LABEL_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_PROJECT_INFO_GROUP_LB;
	WndKey = _T("ONLINE_PROJECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ONLINE_DEFECT_INFO_GROUP_LB;
	WndKey = _T("ONLINE_DEFECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_CLEAR_STATISTIC_BTN_LB;
	WndKey = _T("ONLINE_CLEAR_STATISTIC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = ONLINE_PROJECT_RESULT_GROUP_LB;
	WndKey = _T("ONLINE_PROJECT_RESULT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	//Lane A-Last Station	
	WndID = ONLINE_LAST_STATION_GROUP_LA;
	WndKey = _T("ONLINE_LAST_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_SEND_LABLE_LA;
	WndKey = _T("ONLINE_LAST_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_RECIEVE_LABLE_LA;
	WndKey = _T("ONLINE_LAST_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	//Lane A-Lane Sensor
	WndID = ONLINE_LANE_GROUP_LA;
	WndKey = _T("ONLINE_LANE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_IN_LABEL_LA;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL_LA;
	WndKey = _T("ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP_LABEL_LA;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_OUT_LABEL_LA;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_OUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP2_LABEL_LA;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP2_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//Lane A-Last Station
	WndID = ONLINE_NEXT_STATION_GROUP_LA;
	WndKey = _T("ONLINE_NEXT_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_LABLE_LA;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_OK_LABLE_LA;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_OK_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_NG_LABLE_LA;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_NG_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_RECIEVE_LABLE_LA;
	WndKey = _T("ONLINE_NEXT_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//Lane A-Online button
	WndID = ONLINE_LANE_BUTTON_GROUP_LA;
	WndKey = _T("ONLINE_LANE_BUTTON_GROUP_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ONLINE_LANE_RUN_BTN_LA;
	WndKey = _T("ONLINE_LANE_RUN_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ONLINE_LANE_STOP_BTN_LA;
	WndKey = _T("ONLINE_LANE_STOP_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ONLINE_LANE_BYPASS_BTN_LA;
	WndKey = _T("ONLINE_LANE_BYPASS_BTN_LA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Lane B-Last Station	
	WndID = ONLINE_LAST_STATION_GROUP_LB;
	WndKey = _T("ONLINE_LAST_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_SEND_LABLE_LB;
	WndKey = _T("ONLINE_LAST_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_RECIEVE_LABLE_LB;
	WndKey = _T("ONLINE_LAST_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	//Lane B-Lane Sensor
	WndID = ONLINE_LANE_GROUP_LB;
	WndKey = _T("ONLINE_LANE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_IN_LABEL_LB;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL_LB;
	WndKey = _T("ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP_LABEL_LB;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_OUT_LABEL_LB;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_OUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP2_LABEL_LB;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP2_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//Lane B-Last Station
	WndID = ONLINE_NEXT_STATION_GROUP_LB;
	WndKey = _T("ONLINE_NEXT_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_LABLE_LB;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_OK_LABLE_LB;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_OK_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_NG_LABLE_LB;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_NG_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_RECIEVE_LABLE_LB;
	WndKey = _T("ONLINE_NEXT_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//Lane B-Online button

	WndID = ONLINE_LANE_BUTTON_GROUP_LB;
	WndKey = _T("ONLINE_LANE_BUTTON_GROUP_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ONLINE_LANE_RUN_BTN_LB;
	WndKey = _T("ONLINE_LANE_RUN_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	WndID = ONLINE_LANE_STOP_BTN_LB;
	WndKey = _T("ONLINE_LANE_STOP_BTN_LB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	WndID = ONLINE_LANE_BYPASS_BTN_LB;
	WndKey = _T("ONLINE_LANE_BYPASS_BTN_LB");
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
CString COnlineFormView_Dual::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ONLINE_FORMVIEW_DUAL");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* COnlineFormView_Dual::GetActiveProject()
{
	CAOIProject *Ptr = AOIDataCollect.GetActiveProject();
	return Ptr;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* COnlineFormView_Dual::GetActiveProject(LANE_ID LaneID)
{
	CAOIProject *Ptr = NULL;
	switch ( LaneID )
	{
	case LANE_ID_B:	Ptr = m_ProjectPtr_LB;	break;
	default:
	case LANE_ID_A:	
		Ptr = m_ProjectPtr_LA;	
		break;
	}
	return Ptr;
}
//-------------------------------------------------------------------------------------//
LANE_ID COnlineFormView_Dual::GetActiveLaneID_Dual()
{
	LANE_ID  LaneID;
	m_LaneID = AOIDataCollect.GetActiveLaneID();
	LaneID = m_LaneID;
	return LaneID;	
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildProjectIDCombox(LANE_ID LaneID, CComboBox &Combox)
{
	size_t       i=0;
	int          idx=0;	
	int          ActIdx=-1;
	CString      str;	
	CString      str2;	
	CString      strVersionName;
	CAOIProject *ProjectPtr = NULL;
	TVersionCode *PVersionCode = NULL;
	CString      strVersion=AOIDataDefine.GetVersionCodeText();		
	CAOIProject *ProjectPtrAct = GetActiveProject();	
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();	
	
	JetAPI::ClearCombox(Combox);	

	idx = 0;
	for ( i=0; i<ProjectCount; i++ )
	{
		ProjectPtr = AOIDataCollect.GetProjectPtr(i, false);
		if ( NULL == ProjectPtr ) { continue; }
		if ( ProjectCount > 1 ) 
		{
			if ( ProjectPtr->GetProjectLaneEnable(LaneID) == false )
			{	continue; }
		}
		str = ProjectPtr->GetProjectFileMainName();
		PVersionCode = ProjectPtr->GetProjectVersionCodeActivePtr();//取得目前專案版本號
		if ( NULL != PVersionCode )
		{
			str2 = str;
			strVersionName=PVersionCode->wsCodeName.c_str();
			str.Format(_T("%s(%s:%s)"), str2, strVersion, strVersionName);			
		}

		if ( ProjectPtr == ProjectPtrAct ) 
		{
			ActIdx = i;
			str = str + _T(" *");	
		}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, i);

		idx ++;
	}

	if ( -1 != ActIdx )
	{
		idx = ActIdx;
		JetAPI::SetComboxCurSel(Combox, idx);
	}
	else if ( idx > 0 )
	{	Combox.SetCurSel(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildDefectModeCombox(CComboBox &Combox)
{	
	if ( Combox.GetSafeHwnd() == NULL  )
	{	return false; }

	int          idx=0;	
	CString      str;
	
	JetAPI::ClearCombox(Combox);

	str = _T("Current");
	str = LoadMultiLanguageString(str, str);	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DEFECT_MODE_BY_CURRENT);
	idx ++;

	str = _T("Statistics");
	str = LoadMultiLanguageString(str, str);	
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, DEFECT_MODE_BY_STATISTIC);
	idx ++;
	
	JetAPI::SetComboxCurSel(Combox, DEFECT_MODE_BY_CURRENT);
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchLane()
{
	CImageWnd *WndPtr_Act = NULL;//啟用視窗
	CImageWnd *WndPtr_Off = NULL;//關閉視窗
	LANE_ID LaneID = GetActiveLaneID_Dual();
	switch (LaneID)
	{
	case LANE_ID_A:
		WndPtr_Act = &m_ImageWnd_LA;
		WndPtr_Off = &m_ImageWnd_LB;
		break;
	case LANE_ID_B:
		WndPtr_Act = &m_ImageWnd_LB;
		WndPtr_Off = &m_ImageWnd_LA;
		break;
	}
	if (NULL != WndPtr_Act)
	{
		WndPtr_Act->SetShowCameraPos(true);
		WndPtr_Act->SetShowCameraRgn(true);
	}

	if (NULL != WndPtr_Off)
	{
		WndPtr_Off->SetShowCameraPos(false);
		WndPtr_Off->SetShowCameraRgn(false);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::CloseProject()
{
	CloseProject_LA();
	CloseProject_LB();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::CloseProject_LA()
{
	CString str;
	m_ProjectPtr_LA = NULL;
	m_TestResultBarcode_LA = _T("");	
	m_ImageWnd_LA.SetProjectPtr(NULL);
	m_ImageWnd_LA.ReleaseImageBuffer();	
	m_ImageWnd_LA.BuildSystemRegion();
	m_ImageWnd_LA.RedrawWnd(true);
	
	str = AOIDataDefine.GetTestResultImageName(TEST_RESULT_NONE);
	m_DibResult_LA.Load(str);

	SetDefectCountEdit(LANE_ID_A, 0, 0);	
	JetAPI::ClearListCtrl(m_ResultListCtrl_LA, FALSE);
	JetAPI::ClearListCtrl(m_DefectListCtrl_LA, FALSE);
	JetAPI::ClearListCtrl(m_ProjectInfoListCtrl_LA, FALSE);
	JetAPI::ClearCombox(m_ProjectIDCombox_LA);	
	BuildChartWnd(LANE_ID_A, NULL);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::CloseProject_LB()
{
	CString str;
	m_ProjectPtr_LB = NULL;
	m_TestResultBarcode_LB = _T("");	
	m_ImageWnd_LB.SetProjectPtr(NULL);
	m_ImageWnd_LB.ReleaseImageBuffer();	
	m_ImageWnd_LB.BuildSystemRegion();
	m_ImageWnd_LB.RedrawWnd(true);
	
	str = AOIDataDefine.GetTestResultImageName(TEST_RESULT_NONE);
	m_DibResult_LB.Load(str);

	SetDefectCountEdit(LANE_ID_B, 0, 0);	
	JetAPI::ClearListCtrl(m_ResultListCtrl_LB, FALSE);
	JetAPI::ClearListCtrl(m_DefectListCtrl_LB, FALSE);
	JetAPI::ClearListCtrl(m_ProjectInfoListCtrl_LB, FALSE);
	JetAPI::ClearCombox(m_ProjectIDCombox_LB);	
	BuildChartWnd(LANE_ID_B, NULL);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProject()
{	
	SwitchProject_LA();
	SwitchProject_LB();
	SwitchLane();	
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProject_LA()
{
	CloseProject_LA();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	const size_t ProjectCount=AOIDataCollect.GetLaneProjectCount_LA();
	if ( ProjectCount > 0 )
	{	ProjectPtr = AOIDataCollect.GetLaneProjectPtr_LA(0, true);		}	
	if ( NULL == ProjectPtr )	{	return;	}		
	SwitchProject_LA(ProjectPtr);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProject_LB()
{
	CloseProject_LB();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	const size_t ProjectCount=AOIDataCollect.GetLaneProjectCount_LB();
	if ( ProjectCount > 0 )
	{	ProjectPtr = AOIDataCollect.GetLaneProjectPtr_LB(0, true);		}	
	if ( NULL == ProjectPtr )	{	return;	}	
	SwitchProject_LB(ProjectPtr);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProject_LA(CAOIProject *ProjectPtr)
{
	if ( NULL == ProjectPtr ) { return ; }
	BuildProjectIDCombox(LANE_ID_A, m_ProjectIDCombox_LA);	
	ExecSwitchProject_LA(ProjectPtr);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProject_LB(CAOIProject *ProjectPtr)
{
	if ( NULL == ProjectPtr ) { return ; }
	BuildProjectIDCombox(LANE_ID_B, m_ProjectIDCombox_LB);	
	ExecSwitchProject_LB(ProjectPtr);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProjectMap()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return; }

	TPOINT2D   ImageRes;		
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;
	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);	

	if ( ProjectPtr == m_ProjectPtr_LA )
	{
		m_ImageWnd_LA.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
		m_ImageWnd_LA.RedrawWnd(false);		
	}
	if ( ProjectPtr == m_ProjectPtr_LB )
	{
		m_ImageWnd_LB.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
		m_ImageWnd_LB.RedrawWnd(false);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::SwitchProjectMark()
{	
	CAOIProject *ProjectPtr_LA = AOIDataCollect.GetLaneProjectPtr_LA(0, true);
	CAOIProject *ProjectPtr_LB = AOIDataCollect.GetLaneProjectPtr_LB(0, true);	
	if ( ProjectPtr_LA != m_ProjectPtr_LA )
	{	SwitchProject_LA(); }
	if ( ProjectPtr_LB != m_ProjectPtr_LB )
	{	SwitchProject_LB(); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::ExecSwitchProject_LA(CAOIProject *ProjectPtr)
{
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;	
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	TASK_STATE_MODE  TaskStateMode = AOIDataCollect.GetOnlineTaskState();

	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false);		
	
	m_ProjectPtr_LA = ProjectPtr;
	m_ImageWnd_LA.SetProjectPtr(ProjectPtr);
	m_ImageWnd_LA.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd_LA.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true);
	m_ImageWnd_LA.ShowFittedZoom();
	
	if ( TASK_STATE_NONE==TaskStateMode || TASK_STATE_IDLE==TaskStateMode)
	{	m_ImageWnd_LA.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);	}
	else
	{	m_ImageWnd_LA.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	}
	m_ImageWnd_LA.RedrawWnd(FALSE);
	BuildDefectListWnd(LANE_ID_A, ProjectPtr, m_DefectModeCombox_LA);		
	BuildProjectInfoListWnd(ProjectPtr, m_ProjectInfoListCtrl_LA);
	ExecInspection_Finish_LA(ProjectPtr);	
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::ExecSwitchProject_LB(CAOIProject *ProjectPtr)
{
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;	
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	TASK_STATE_MODE  TaskStateMode = AOIDataCollect.GetOnlineTaskState();

	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false);	
	
	m_ProjectPtr_LB = ProjectPtr;
	m_ImageWnd_LB.SetProjectPtr(ProjectPtr);
	m_ImageWnd_LB.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd_LB.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true);
	m_ImageWnd_LB.ShowFittedZoom();
	
	if ( TASK_STATE_NONE==TaskStateMode || TASK_STATE_IDLE==TaskStateMode)
	{	m_ImageWnd_LB.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);	}
	else
	{	m_ImageWnd_LB.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	}
	m_ImageWnd_LB.RedrawWnd(FALSE);		
	BuildDefectListWnd(LANE_ID_B, ProjectPtr, m_DefectModeCombox_LB);
	BuildProjectInfoListWnd(ProjectPtr, m_ProjectInfoListCtrl_LB);
	ExecInspection_Finish_LB(ProjectPtr);	
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class	
	RedrawWnd(FALSE);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::RedrawWnd(BOOL bRedrawBK)
{
	m_ImageWnd_LA.RedrawWnd(bRedrawBK);
	m_ImageWnd_LB.RedrawWnd(bRedrawBK);
	DrawIconWnd(m_DibLane_LA, m_LaneIcon_LA);
	DrawIconWnd(m_DibLane_LB, m_LaneIcon_LB);	
	DrawIconWnd(m_DibResult_LA, m_ResultIcon_LA);
	DrawIconWnd(m_DibResult_LB, m_ResultIcon_LB);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::DrawIconWnd(CDib &dib, CStatic &IconWnd)
{
	if ( IconWnd.GetSafeHwnd() == NULL ) { return; }
	if ( IconWnd.IsWindowVisible() == FALSE ) { return; }

	RECT Rect={0};
	CClientDC dc(&IconWnd);	
	HDC hDC = dc.GetSafeHdc();

	IconWnd.GetClientRect(&Rect);	
	BITMAPINFO *pInfo = dib.GetDIBInfo();
	if ( NULL == pInfo ) 
	{ 
		COLORREF Color = ::GetSysColor(COLOR_3DFACE);
		HBRUSH hBrush = ::CreateSolidBrush(Color);
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
		return ; 
	}	
	int OldMode = ::SetStretchBltMode(hDC, HALFTONE);
	dib.DrawPartion(hDC, 0, 0, pInfo->bmiHeader.biWidth, pInfo->bmiHeader.biHeight, 0, 0, Rect.right, Rect.bottom);
	::SetStretchBltMode(hDC, OldMode);
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecMoveToStage()
{
	if ( this->GetLockUIWnd() == true ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	RedrawWnd(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecUpdateProject()
{	
	LANE_ID LaneID = GetActiveLaneID_Dual();
	CAOIProject *ProjectPtr = GetActiveProject(LaneID);
	if ( NULL == ProjectPtr ) { return false; }
	if ( LANE_ID_B == LaneID )
	{
		m_TestResultBarcode_LB = ProjectPtr->GetProjectBarcode();	
		BuildDefectListWnd(LaneID, ProjectPtr, m_DefectModeCombox_LB);
	}
	else
	{
		m_TestResultBarcode_LA = ProjectPtr->GetProjectBarcode();	
		BuildDefectListWnd(LaneID, ProjectPtr, m_DefectModeCombox_LA);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::ExecInspection_Finish()
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( ProjectPtr == m_ProjectPtr_LA )
	{	ExecInspection_Finish_LA(ProjectPtr); }
	if ( ProjectPtr == m_ProjectPtr_LB )
	{	ExecInspection_Finish_LB(ProjectPtr); }
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode();	
	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if ( TASK_TUNING_PROJECT == TaskMode || TASK_TUNING_OFFLINE == TaskMode )
	{	LockUIWnd(false);	}	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::ExecInspection_Finish_LA(CAOIProject *ProjectPtr)
{		
	m_ImageWnd_LA.ResetUpdateTestMapTickCount();
	if ( NULL == ProjectPtr ) { return ; }

	CString str;	
	LANE_ID LaneID = LANE_ID_A;
	TTestResult ResultLatest    = ProjectPtr->GetProjectResultLatest_Lane(LaneID);	
	str = AOIDataDefine.GetTestResultImageName(ResultLatest.sResultID);	
	m_DibResult_LA.Load(str);
	m_TestResultBarcode_LA = ProjectPtr->GetProjectBarcode(); 
	BuildResultListWnd(LaneID, ProjectPtr, m_ResultListCtrl_LA);
	BuildDefectListWnd(LaneID, ProjectPtr, m_DefectModeCombox_LA);
	BuildChartWnd(LaneID, ProjectPtr);
	DrawIconWnd(m_DibResult_LA, m_ResultIcon_LA);	
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::ExecInspection_Finish_LB(CAOIProject *ProjectPtr)
{	
	m_ImageWnd_LB.ResetUpdateTestMapTickCount();
	if ( NULL == ProjectPtr ) { return ; }

	CString str;	
	LANE_ID LaneID = LANE_ID_B;
	TTestResult ResultLatest    = ProjectPtr->GetProjectResultLatest_Lane(LaneID);	
	str = AOIDataDefine.GetTestResultImageName(ResultLatest.sResultID);	
	m_DibResult_LB.Load(str);
	m_TestResultBarcode_LB = ProjectPtr->GetProjectBarcode(); 
	BuildResultListWnd(LaneID, ProjectPtr, m_ResultListCtrl_LB);
	BuildDefectListWnd(LaneID, ProjectPtr, m_DefectModeCombox_LB);
	BuildChartWnd(LaneID, ProjectPtr);
	DrawIconWnd(m_DibResult_LB, m_ResultIcon_LB);
	return;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecOnlineInspection_Finish()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }	
	AOIDataCollect.ExecOnlineInspectionFinish();
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeDefectModeCombo_LA() 
{
	// TODO: Add your control notification handler code here	
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_A);
	if ( NULL != ProjectPtr )
	{	BuildDefectListWnd(LANE_ID_A, ProjectPtr, m_DefectModeCombox_LA); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeDefectModeCombo_LB() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_B);
	if ( NULL != ProjectPtr )
	{	BuildDefectListWnd(LANE_ID_B, ProjectPtr, m_DefectModeCombox_LB); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeChartTab_LA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ExecSelchangeChartTab(LANE_ID_A, m_ChartTab_LA);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeChartTab_LB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ExecSelchangeChartTab(LANE_ID_B, m_ChartTab_LB);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecSelchangeChartTab(LANE_ID LaneID, CTabCtrl &TabCtrl)
{
	if ( TabCtrl.GetSafeHwnd() == NULL ) 
	{	return true; }

	CWnd *pWnd=NULL;	
	BOOL  ShowWnd=TRUE;
	BOOL  Show_InfoList=FALSE;
	BOOL  Show_BarChart=FALSE;
	BOOL  Show_PieChart=FALSE;
	BOOL  Show_XYChart=FALSE;
	BOOL  Show_DefectPieChart=FALSE;	
	BOOL  ShowDefectFrom=FALSE;
	const int TabIndex = TabCtrl.GetCurSel();
	switch ( TabIndex )
	{
	case TAB_INDEX_INFO:
		Show_InfoList = TRUE;
		ShowDefectFrom = TRUE;
		break;
	case TAB_INDEX_YIELD_BAR://Bar Chart
		Show_BarChart = TRUE;
		ShowDefectFrom = TRUE;
		break;
	case TAB_INDEX_TOP_10://Pie Chart		
		Show_PieChart = TRUE;		
		ShowDefectFrom = TRUE;
		break;
	case TAB_INDEX_XY_CHART://Radar Chart		
		Show_XYChart = TRUE;	
		ShowDefectFrom = FALSE;
		break;
	case TAB_INDEX_DEFECT_PIE://Defect Pie Chart		
		Show_DefectPieChart = TRUE;
		ShowDefectFrom = TRUE;
		break;
	}
	
	//Defect From Combox
	ShowWnd = ShowDefectFrom;
	if ( LANE_ID_B == LaneID )
	{	JetAPI::ShowCtrlWnd(this, ONLINE_DEFECT_FROM_COMBO_LB, ShowWnd);	}
	else
	{	JetAPI::ShowCtrlWnd(this, ONLINE_DEFECT_FROM_COMBO_LA, ShowWnd);	}

	//Info List
	ShowWnd = Show_InfoList;
	if ( LANE_ID_B == LaneID )
	{	JetAPI::ShowCtrlWnd(this, ONLINE_INFO_LIST_WND_LB, ShowWnd);	}
	else
	{	JetAPI::ShowCtrlWnd(this, ONLINE_INFO_LIST_WND_LA, ShowWnd);	}

	//Yield Bar Chart	
	ShowWnd = Show_BarChart;
	if ( LANE_ID_B == LaneID )
	{
		JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_SCOPE_COMBO_LB, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_LIST_WND_LB, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_YIELDING_LB, ShowWnd);
	}
	else
	{
		JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_SCOPE_COMBO_LA, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_LIST_WND_LA, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_YIELDING_LA, ShowWnd);
	}
	
	//Top-10 Pie Chart
	ShowWnd = Show_PieChart;
	if ( LANE_ID_B == LaneID )
	{
		JetAPI::ShowCtrlWnd(this, ONLINE_TOP10_SCOPE_COMBO_LB, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_TOP10_LB, ShowWnd);
	}
	else
	{
		JetAPI::ShowCtrlWnd(this, ONLINE_TOP10_SCOPE_COMBO_LA, ShowWnd);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_TOP10_LA, ShowWnd);
	}

	//XY Chart
	ShowWnd = Show_XYChart;
	if ( LANE_ID_B == LaneID )
	{	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_XY_LB, ShowWnd);		}
	else
	{	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_XY_LA, ShowWnd);		}

	//Defect Pie Chart
	ShowWnd = Show_DefectPieChart;
	if ( LANE_ID_B == LaneID )
	{	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_DEFECT_LB, ShowWnd);		}
	else
	{	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_DEFECT_LA, ShowWnd);		}

	CAOIProject *ProjectPtr = GetActiveProject(LaneID);	
	BuildChartWnd(LaneID, ProjectPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildDefectListWnd(LANE_ID LaneID, CAOIProject *ProjectPtr, CComboBox &DefectModeCombox)
{
	bool IsOK = true;
	if ( NULL == ProjectPtr ) { return true; }
	const int Param = JetAPI::GetComboxCurSelData(DefectModeCombox);
	ProjectPtr->LockProject();
	if ( LANE_ID_B == LaneID )
	{
		CThisListCtrl_38 &DefectListCtrl=m_DefectListCtrl_LB;
		if ( DEFECT_MODE_BY_STATISTIC == Param )
		{	IsOK = BuildDefectListWndKernel_Statistic(LaneID, ProjectPtr, m_DefectFromCombox_LB, DefectListCtrl);	}
		else
		{	IsOK = BuildDefectListWndKernel_Current(LaneID, ProjectPtr, DefectListCtrl);	}	
	}
	else
	{
		CThisListCtrl_38 &DefectListCtrl=m_DefectListCtrl_LA;
		if ( DEFECT_MODE_BY_STATISTIC == Param )
		{	IsOK = BuildDefectListWndKernel_Statistic(LaneID, ProjectPtr, m_DefectFromCombox_LA, DefectListCtrl);	}
		else
		{	IsOK = BuildDefectListWndKernel_Current(LaneID, ProjectPtr, DefectListCtrl);	}	
	}
	ProjectPtr->UnlockProject();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildDefectListWndKernel_Current(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl)
{	
	if ( ListCtrl.GetSafeHwnd() == NULL ) 
	{	return false; }
	size_t         i=0;
	CString        str;
	CString        strComponent;
	CString        strPartNumber;
	int            nItem=0;
	int            nSubItem=0;
	unsigned int   PanelIndex = 0;
	unsigned int   BoardIndex = 0;	
	RESULT_ID      ResultID=RESULT_ID_NONE;	

	SetDefectCountEdit(LaneID, 0, 0);
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t ComponentNotAgentCount = ProjectPtr->GetProjectComponentNotAgentCount();	
	const size_t DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount_Lane(LaneID);	

	nItem = 0;	
	//ListCtrl.SetTextBkColor(0xFFD0FF);
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<DefectComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectDefectComponentPtr_Lane(LaneID, i, false);
		if ( NULL == ComponentPtr ) { continue; }
		
		nSubItem = 0;
		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
		strComponent = ComponentPtr->GetComponentName();
		strPartNumber = ComponentPtr->GetComponentPartNumber();
		str = AOIDataDefine.GetComponentFullNameReverse(PanelIndex, BoardIndex, strComponent);

		//零件名稱
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//零件料號
		str = strPartNumber;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = _T("1");
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	SetDefectCountEdit(LaneID, nItem, ComponentNotAgentCount);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildDefectListWndKernel_Statistic(LANE_ID LaneID, CAOIProject *ProjectPtr, CComboBox &FromCombox, CThisListCtrl_38 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL ) 
	{	return false; }
	if ( FromCombox.GetSafeHwnd() == NULL )
	{	return false; }

	size_t         i=0;
	CString        str;
	CString        strComponent;
	CString        strPartNumber;
	int            nItem=0;
	int            nSubItem=0;
	unsigned int   PanelIndex = 0;
	unsigned int   BoardIndex = 0;	
	RESULT_ID      ResultID=RESULT_ID_NONE;
	DEFECT_FROM_MODE    DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(FromCombox));	

	SetDefectCountEdit(LaneID, 0, 0);
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	if ( NULL == ProjectPtr ) { return true; }

	CSortObj       SortObj;
	std::vector<CSortObj> SortList;

	size_t         index=0;
	size_t         DefectCount=0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();	
	const size_t ComponentNotAgentCount = ProjectPtr->GetProjectComponentNotAgentCount();	

	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		if ( DEFECT_FROM_ARS == DefectFrom )
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS_Lane(LaneID);	}
		else
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI_Lane(LaneID);	}
		if ( 0 == DefectCount ) { continue; }	

		SortObj.SetID(i);
		SortObj.SetValueInt((int)(DefectCount));		
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();

	nItem = 0;	
	//ListCtrl.SetTextBkColor(0xFFD0FF);
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<SortCount; i++ )
	{
		index = SortList[SortCount-i-1].GetID();
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(index, true);
		if ( NULL == ComponentPtr ) { continue; }		
		
		nSubItem = 0;
		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		BoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
		strComponent = ComponentPtr->GetComponentName();
		strPartNumber = ComponentPtr->GetComponentPartNumber();
		str = AOIDataDefine.GetComponentFullNameReverse(PanelIndex, BoardIndex, strComponent);
		if ( DEFECT_FROM_ARS == DefectFrom )
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS_Lane(LaneID);	}
		else
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI_Lane(LaneID);	}

		//零件名稱
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//零件料號
		str = strPartNumber;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%d"), DefectCount);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	SetDefectCountEdit(LaneID, nItem, ComponentNotAgentCount);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildDefectListWndHeader(CThisListCtrl_38 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT		

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-16)/5;
	width2 = width*2;

	str = _T("Component");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	
	str = _T("Part Number");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	
	str = _T("Defects");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::SetDefectCountEdit(LANE_ID LaneID, int nDefects, size_t nTotal)
{
	CString str;
	CString Text;
	str = _T("Defects");
	str = LoadMultiLanguageString(str, str);
	Text.Format(_T("%s: %d/%d"), str, nDefects, nTotal);
	if ( LANE_ID_B == LaneID )
	{	CWnd::SetDlgItemText(ONLINE_DEFECT_COUNT_EDIT_LB, Text);	}
	else
	{	CWnd::SetDlgItemText(ONLINE_DEFECT_COUNT_EDIT_LA, Text);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitImageWnd(CImageWnd &ImageWnd)
{	
	if ( ImageWnd.GetSafeHwnd() == NULL  )
	{	return false; }
	ImageWnd.SetShowLBtnPos(false);
	ImageWnd.SetShowCameraRgn(true);
	ImageWnd.SetShowBoard(true);//顯示單板
	ImageWnd.SetShowSystem(true);//顯示系統	
	ImageWnd.SetShowFiducial(true);//顯示專案定位點
	ImageWnd.SetShowBarcode(true);//顯示專案條碼	
	ImageWnd.SetShowComponent(true);//顯示專案零件
	ImageWnd.SetShowCursorLine(false);
	ImageWnd.SetShowDistrictRect(true);
	ImageWnd.SetShowComponentName(false);
	if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_ShowComponentDefectOnly )
	{	ImageWnd.SetShowComponentDefectOnly(true);	}
	else
	{	ImageWnd.SetShowComponentDefectOnly(false);	}
	ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);	
	ImageWnd.SetShowFieldRgn(false);
	ImageWnd.SetShowMoveToMsg(true);	
	//ImageWnd.SetShowFieldRgn(true);
	//ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildProjectInfoListWnd(CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl)
{	
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	int     nItem=0;
	int     nSubItem=0;
	CString str;
	CString strInfo;
	CString strtTitle;	

	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	if ( NULL == ProjectPtr )
	{	return false; }
	nItem = 0;

	//ListCtrl.SetTextBkColor(0xFFFFD0);
	ListCtrl.SetRedraw(FALSE);

	//檔案名稱
	//nSubItem = 0;
	//str = _T("Filename");
	//strtTitle = LoadMultiLanguageString(str, str);
	//ListCtrl.InsertItem(nItem, strtTitle);
	//ListCtrl.SetItemText(nItem, nSubItem, strtTitle);	nSubItem ++;
	//strInfo = ProjectPtr->GetProjectShowName();
	//ListCtrl.SetItemText(nItem, nSubItem, strInfo);	nSubItem ++;
	//nItem ++;	

	//機種名稱
	nSubItem = 0;
	str = _T("Module");
	strtTitle = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, strtTitle);
	ListCtrl.SetItemText(nItem, nSubItem, strtTitle);	nSubItem ++;
	strInfo = ProjectPtr->GetProjectModuleName();
	ListCtrl.SetItemText(nItem, nSubItem, strInfo);	nSubItem ++;
	nItem ++;

	//上下面邊
	nSubItem = 0;
	str = _T("Side");
	strtTitle = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, strtTitle);
	ListCtrl.SetItemText(nItem, nSubItem, strtTitle);	nSubItem ++;
	strInfo = AOIDataDefine.GetPanelSideModeText(ProjectPtr->GetProjectPanelSideMode());
	ListCtrl.SetItemText(nItem, nSubItem, strInfo);	nSubItem ++;
	nItem ++;	

	//工單號碼
	nSubItem = 0;
	str = _T("Work Number");
	strtTitle = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, strtTitle);
	ListCtrl.SetItemText(nItem, nSubItem, strtTitle);	nSubItem ++;
	strInfo = ProjectPtr->GetProjectWorkNumber();
	ListCtrl.SetItemText(nItem, nSubItem, strInfo);	nSubItem ++;
	nItem ++;
	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildProjectInfoListWndHeader(CThisListCtrl_38 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT		

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-4)/5;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width*4;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildResultListWnd(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	size_t         i=0;
	CString        str;
	CString        strComponent;
	CString        strPartNumber;
	int            nItem=0;
	int            nSubItem=0;
	unsigned int   PanelIndex = 0;
	unsigned int   BoardIndex = 0;	
	RESULT_ID      ResultID=RESULT_ID_NONE;	
	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);		
	if ( NULL == ProjectPtr ) { return true; }

	TTestResult ResultLatest    = ProjectPtr->GetProjectResultLatest_Lane(LaneID);
	TTestResult ResultStatistic = ProjectPtr->GetProjectResultStatistic_Lane(LaneID);

	nItem = 0;	
	//ListCtrl.SetTextBkColor(0xFFD0FF);
	ListCtrl.SetRedraw(FALSE);

	//檢測日期
	nSubItem = 0;
	str = _T("Test Date");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	JetAPI::FormatTime(FORMAT_TIME_01, ResultLatest.sDateTimeS, str);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	//花費時間
	nItem ++;
	nSubItem = 0;
	str = _T("Spent Time");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	str.Format(_T("%.2fs,   Ave:%.2fs"), ResultLatest.sTestTime.tTest, ResultStatistic.sTestTime.tTest);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;

	//循環時間
	/*
	nItem ++;
	nSubItem = 0;
	str = _T("Cycle Time");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	str.Format(_T("%.2fs,   Ave:%.2fs"), ResultLatest.sTestTime.tCycle, ResultStatistic.sTestTime.tCycle);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	*/
	//條碼
	nItem ++;
	nSubItem = 0;
	str = _T("Barcode");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;
	if ( LANE_ID_B == LaneID )
	{	str = m_TestResultBarcode_LB;	}
	else
	{	str = m_TestResultBarcode_LA;	}	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;	
		
	ListCtrl.SetRedraw(TRUE);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildResultListWndHeader(CThisListCtrl_38 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT		

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-4)/4;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width*3;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartTab(CTabCtrl &TabCtrl)
{
	if ( TabCtrl.GetSafeHwnd() == NULL  )
	{	return false; }

	int     idx=0;
	CString str;
	TCITEM  item;
	TCHAR   lpBuffer[256] = _T("");

	idx=0;
	item.mask = TCIF_TEXT;
	item.pszText = lpBuffer;
	item.cchTextMax = 256;

	TabCtrl.DeleteAllItems();
	
	str = _T("Info");//Info
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(item.pszText, str);
	TabCtrl.InsertItem(idx,&item);
	idx ++;

	str = _T("Yield");//Yield
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(item.pszText, str);
	TabCtrl.InsertItem(idx,&item);
	idx ++;

	str = _T("Top-10");//Top-10
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(item.pszText, str);
	TabCtrl.InsertItem(idx,&item);
	idx ++;

	str = _T("XY");//XY
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(item.pszText, str);
	TabCtrl.InsertItem(idx,&item);
	idx ++;

	str = _T("Defect");//Defect
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(item.pszText, str);
	TabCtrl.InsertItem(idx,&item);
	idx ++;

	if ( idx > 0 ) 
	{	TabCtrl.SetCurSel(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ClearChartWnd(CThisChartCtrl &ChartWnd)
{
	if ( ChartWnd.GetSafeHwnd() == NULL ) { return true; }
	ChartWnd.Clear();	
	ChartWnd.SetInfoStr();
	ChartWnd.SetTitle(_T(""));
	ChartWnd.BuildChart();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartWnd(LANE_ID LaneID)
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT	
	DEFECT_FROM_MODE DefectFromMode = AOIDataCollect.GetOnlineViewDefectFromeMode();
	if ( LANE_ID_B == LaneID )
	{
		AOIDataDefine.BuildDefectFromCombox(m_DefectFromCombox_LB);
		AOIDataDefine.BuildYieldingScopeCombox(m_YieldScopeCombox_LB);
		AOIDataDefine.BuildTop10ScopeCombox(m_Top10ScopeCombox_LB);
		JetAPI::SetComboxCurSel(m_DefectFromCombox_LB, DefectFromMode);
		JetAPI::SetComboxCurSel(m_YieldScopeCombox_LB, YIELDING_SCOPE_TEST);
		JetAPI::SetComboxCurSel(m_Top10ScopeCombox_LB, TOP10_SCOPE_COMPONENT);

	
		nCol = 0;
		width  = 96;
		width2 = 256;
		width = 96;
		width2 = 192;
		CThisListCtrl_38 &ListCtrl_Info = m_InfoListWnd_LB;

		ListCtrl_Info.GetClientRect(&Rect);		
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Info.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Info");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Info.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	
		nCol = 0;
		width  = 64;
		width2 = 64;
		CThisListCtrl_38 &ListCtrl_Yield = m_YieldingListWnd_LB;

		ListCtrl_Yield.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-2)/2;
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Yield.InsertColumn(nCol, str, Align, width);
		nCol ++;

		width2 = width;
		str = _T("Value");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Yield.InsertColumn(nCol, str, Align, width2);
		nCol ++;	

		InitChartWnd_Yielding(m_ChartWnd_Yielding_LB);//初始化圖表視窗-良率
		InitChartWnd_Top10(m_ChartWnd_Top10_LB);//初始化圖表視窗-Top-10
		InitChartWnd_XYChart(m_ChartWnd_XYChart_LB);//初始化圖表視窗-座標圖	
		InitChartWnd_DefectStatistic(m_ChartWnd_Defect_LB);//初始化圖表視窗-瑕疵統計
	}
	else
	{
		AOIDataDefine.BuildDefectFromCombox(m_DefectFromCombox_LA);
		AOIDataDefine.BuildYieldingScopeCombox(m_YieldScopeCombox_LA);
		AOIDataDefine.BuildTop10ScopeCombox(m_Top10ScopeCombox_LA);
		JetAPI::SetComboxCurSel(m_DefectFromCombox_LA, DefectFromMode);
		JetAPI::SetComboxCurSel(m_YieldScopeCombox_LA, YIELDING_SCOPE_TEST);
		JetAPI::SetComboxCurSel(m_Top10ScopeCombox_LA, TOP10_SCOPE_COMPONENT);
	
		nCol = 0;
		width  = 96;
		width2 = 256;
		width = 96;
		width2 = 192;
		CThisListCtrl_38 &ListCtrl_Info = m_InfoListWnd_LA;

		ListCtrl_Info.GetClientRect(&Rect);		
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Info.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Info");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Info.InsertColumn(nCol, str, Align, width2);
		nCol ++;	

	
		nCol = 0;
		width  = 64;
		width2 = 64;
		CThisListCtrl_38 &ListCtrl_Yield = m_YieldingListWnd_LA;

		ListCtrl_Yield.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-2)/2;
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Yield.InsertColumn(nCol, str, Align, width);
		nCol ++;

		width2 = width;
		str = _T("Value");
		str = LoadMultiLanguageString(str, str);
		ListCtrl_Yield.InsertColumn(nCol, str, Align, width2);
		nCol ++;	

		InitChartWnd_Yielding(m_ChartWnd_Yielding_LA);//初始化圖表視窗-良率
		InitChartWnd_Top10(m_ChartWnd_Top10_LA);//初始化圖表視窗-Top-10
		InitChartWnd_XYChart(m_ChartWnd_XYChart_LA);//初始化圖表視窗-座標圖	
		InitChartWnd_DefectStatistic(m_ChartWnd_Defect_LA);//初始化圖表視窗-瑕疵統計
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartWnd_Yielding(CThisChartCtrl &ChartWnd)//初始化圖表視窗-良率
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 12;
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/	

	ChartWnd.SetChartType(CHART_TYPE_LINE);
	ChartWnd.SetXAxisUnit(_T(""));
	ChartWnd.SetYAxisUnit(_T("%"));
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);

	ChartWnd.SetChartTopSpace(35);
	ChartWnd.SetChartBottomSpace(30);
	ChartWnd.SetChartLeftSpace(30);
	ChartWnd.SetChartRightSpace(30);

	ChartWnd.SetSeriesLeftSpace(50);
	ChartWnd.SetSeriesRightSpace(50);
	ChartWnd.SetSeriesTopSpace(0);
	ChartWnd.SetSeriesBottomSpace(0);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	ChartWnd.SetYAxisMin(0);
	ChartWnd.SetYAxisMax(100);
	ChartWnd.SetYAxisFix(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartWnd_XYChart(CThisChartCtrl &ChartWnd)//初始化圖表視窗-座標圖
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 12;
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/
	ChartWnd.SetChartType(CHART_TYPE_RADAR);
	ChartWnd.SetXAxisUnit("um");
	ChartWnd.SetYAxisUnit("um");
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);
	ChartWnd.SetIsShowLegend(true);
//	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_BOTTOM);	
	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_RIGHT);	

	ChartWnd.Set3DThicness(10);
	ChartWnd.SetChartTopSpace(30);
	ChartWnd.SetChartBottomSpace(40);
	ChartWnd.SetChartLeftSpace(40);
	ChartWnd.SetChartRightSpace(20);

	ChartWnd.SetSeriesLeftSpace(5);
	ChartWnd.SetSeriesRightSpace(5);
	ChartWnd.SetSeriesTopSpace(5);
	ChartWnd.SetSeriesBottomSpace(5);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	//ChartWnd.SetXAxisMin(0);
	//ChartWnd.SetXAxisMax(1000);
	//ChartWnd.SetXAxisFix(true);
	//ChartWnd.SetYAxisMin(0);
	//ChartWnd.SetYAxisMax(1000);
	//ChartWnd.SetYAxisFix(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartWnd_Top10(CThisChartCtrl &ChartWnd)//初始化圖表視窗-Top-10
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 12;
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/

	ChartWnd.SetChartType(CHART_TYPE_PIE);
	ChartWnd.SetXAxisUnit("");
	ChartWnd.SetYAxisUnit("%");
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);
	ChartWnd.SetIsShowLegend(true);
//	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_BOTTOM);	
	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_RIGHT);	

	ChartWnd.Set3DThicness(10);
	ChartWnd.SetChartTopSpace(60);
	ChartWnd.SetChartBottomSpace(30);
	ChartWnd.SetChartLeftSpace(30);
	ChartWnd.SetChartRightSpace(30);

	ChartWnd.SetSeriesLeftSpace(50);
	ChartWnd.SetSeriesRightSpace(50);
	ChartWnd.SetSeriesTopSpace(0);
	ChartWnd.SetSeriesBottomSpace(0);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	ChartWnd.SetYAxisMin(0);
	ChartWnd.SetYAxisMax(100);
	ChartWnd.SetYAxisFix(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::InitChartWnd_DefectStatistic(CThisChartCtrl &ChartWnd)//初始化圖表視窗-瑕疵統計
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 12;
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/	

	ChartWnd.SetChartType(CHART_TYPE_PIE);
	ChartWnd.SetXAxisUnit("");
	ChartWnd.SetYAxisUnit("%");
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);
	ChartWnd.SetIsShowLegend(true);
//	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_BOTTOM);	
	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_RIGHT);	

	ChartWnd.Set3DThicness(10);
	ChartWnd.SetChartTopSpace(60);
	ChartWnd.SetChartBottomSpace(30);
	ChartWnd.SetChartLeftSpace(30);
	ChartWnd.SetChartRightSpace(30);

	ChartWnd.SetSeriesLeftSpace(50);
	ChartWnd.SetSeriesRightSpace(50);
	ChartWnd.SetSeriesTopSpace(0);
	ChartWnd.SetSeriesBottomSpace(0);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	ChartWnd.SetYAxisMin(0);
	ChartWnd.SetYAxisMax(100);
	ChartWnd.SetYAxisFix(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd(LANE_ID LaneID, CAOIProject *ProjectPtr)//建立圖表視窗
{	
	if ( LANE_ID_B == LaneID )
	{
		if ( NULL == ProjectPtr )
		{
			ClearChartWnd(m_ChartWnd_Yielding_LB);
			ClearChartWnd(m_ChartWnd_Top10_LB);
			ClearChartWnd(m_ChartWnd_XYChart_LB);
			ClearChartWnd(m_ChartWnd_Defect_LB);
		}
		else
		{
			BuildChartWnd_Info(LaneID, ProjectPtr, m_InfoListWnd_LB);
			BuildChartWnd_Yielding(LaneID, ProjectPtr, m_ChartWnd_Yielding_LB, m_YieldingListWnd_LB);
			BuildChartWnd_Top10(LaneID, ProjectPtr, m_ChartWnd_Top10_LB);//建立圖表視窗-Top-10
			BuildChartWnd_XYChart(LaneID, ProjectPtr, m_ChartWnd_XYChart_LB);//建立圖表視窗-座標圖	
			BuildChartWnd_DefectStatistic(LaneID, ProjectPtr, m_ChartWnd_Defect_LB, m_DefectFromCombox_LB);//建立圖表視窗-瑕疵統計
		}
	}
	else
	{	
		if ( NULL == ProjectPtr )
		{
			ClearChartWnd(m_ChartWnd_Yielding_LA);
			ClearChartWnd(m_ChartWnd_Top10_LA);
			ClearChartWnd(m_ChartWnd_XYChart_LA);
			ClearChartWnd(m_ChartWnd_Defect_LA);
		}
		else
		{
			BuildChartWnd_Info(LaneID, ProjectPtr, m_InfoListWnd_LA);
			BuildChartWnd_Yielding(LaneID, ProjectPtr, m_ChartWnd_Yielding_LA, m_YieldingListWnd_LA);
			BuildChartWnd_Top10(LaneID, ProjectPtr, m_ChartWnd_Top10_LA);//建立圖表視窗-Top-10
			BuildChartWnd_XYChart(LaneID, ProjectPtr, m_ChartWnd_XYChart_LA);//建立圖表視窗-座標圖	
			BuildChartWnd_DefectStatistic(LaneID, ProjectPtr, m_ChartWnd_Defect_LA, m_DefectFromCombox_LA);//建立圖表視窗-瑕疵統計
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd_Info(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl)//建立圖表視窗-訊息
{
	if ( ListCtrl.GetSafeHwnd() == NULL )
	{	return false; }

	ListCtrl.DeleteAllItems();
	if ( NULL == ProjectPtr ) { return false; }
	if ( ListCtrl.IsWindowVisible() == FALSE ) { return true; }

	CString      str;	
	int          nItem=0;	
	double       Yielding=0.0;
	TTestResult  TestResult;
	const int    nSubItem = 1;
	const bool   ShowInfo=false;
	const bool   ShowYield=true;
	DEFECT_FROM_MODE  DefectFrom = DEFECT_FROM_NONE;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t ComponentBypassCount = ProjectPtr->GetProjectComponentBypassCount();
	TTestResult AOITestResult = ProjectPtr->GetProjectResultStatistic_Lane(LaneID);
	TTestResult ARSTestResult = ProjectPtr->GetProjectResultStatistic_ARS_Lane(LaneID);

	if ( LANE_ID_B == LaneID )
	{	DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LB));	}
	if ( LANE_ID_A == LaneID )
	{	DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LA));	}

	switch ( DefectFrom )
	{
	case DEFECT_FROM_AOI:	TestResult = AOITestResult;	break;
	case DEFECT_FROM_ARS:	TestResult = ARSTestResult;	break;	
	}

	ListCtrl.SetRedraw(FALSE);
	if ( true == ShowInfo )
	{
	//機種名稱	
	str = _T("Module");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);	
	str = ProjectPtr->GetProjectModuleName();
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//上下面邊	
	str = _T("Side");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);	
	str = AOIDataDefine.GetPanelSideModeText(ProjectPtr->GetProjectPanelSideMode());
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;	

	//工單號碼
	str = _T("Work Number");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);	
	str = ProjectPtr->GetProjectWorkNumber();
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;	
	}
	
	//整板數
	str = _T("Panel Count");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%d"), PanelCount);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;
	
	//單板數
	str = _T("Board Count");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%d"), BoardCount);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//零件數
	str = _T("Component Count");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%d"), ComponentCount);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//零件不檢測數
	str = _T("Component Bypass Count");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%d"), ComponentBypassCount);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	if ( ShowYield )
	{
	//總檢測數
	str = _T("Test Count");
	str = LoadMultiLanguageString(str, str);
	//ListCtrl.InsertItem(nItem, str);
	//str.Format(_T("%d"), TestResult.sTest.Total);
	//ListCtrl.SetItemText(nItem, nSubItem, str);
	//nItem ++;

	//檢測良率
	str = _T("Test Yielding");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);	
	if ( TestResult.sTest.Total > 0 )
	{	Yielding = TestResult.sTest.OK*100.0/TestResult.sTest.Total;	}
	else
	{	Yielding = 0.0; }
	str.Format(_T("%.2f %% (%u/%u)"), Yielding, TestResult.sTest.OK, TestResult.sTest.Total);	
	Yielding = TestResult.sTest.CalcYielding();
	str.Format(_T("%.2f %%"), Yielding);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//整板良率
	str = _T("Panel Yielding");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	if ( TestResult.sPanel.Total > 0 )
	{	Yielding = TestResult.sPanel.OK*100.0/TestResult.sPanel.Total;	}
	else
	{	Yielding = 0.0; }	
	str.Format(_T("%.2f %% (%u/%u)"), Yielding, TestResult.sPanel.OK, TestResult.sPanel.Total);	
	Yielding = TestResult.sPanel.CalcYielding();
	str.Format(_T("%.2f %%"), Yielding);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//單板良率
	str = _T("Board Yielding");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	if ( TestResult.sBoard.Total > 0 )
	{	Yielding = TestResult.sBoard.OK*100.0/TestResult.sBoard.Total;	}
	else
	{	Yielding = 0.0; }	
	str.Format(_T("%.2f %% (%u/%u)"), Yielding, TestResult.sBoard.OK, TestResult.sBoard.Total);	
	Yielding = TestResult.sBoard.CalcYielding();
	str.Format(_T("%.2f %%"), Yielding);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;

	//零件良率
	str = _T("Component Yielding");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	if ( TestResult.sComponent.Total > 0 )
	{	Yielding = TestResult.sComponent.OK*100.0/TestResult.sComponent.Total;	}
	else
	{	Yielding = 0.0; }
	Yielding = TestResult.sComponent.CalcYielding();	
	str.Format(_T("%.6f %%"), Yielding);	
	//str.Format(_T("%.6f (%u/%u)"), Yielding, TestResult.sComponent.OK, TestResult.sComponent.Total);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;
	}
	
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd_Yielding(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CThisListCtrl_38 &ListCtrl)//建立圖表視窗-良率
{
	ChartWnd.Clear();
	ListCtrl.DeleteAllItems();
	ListCtrl.SetTextBkColor(0xD0FFFF);
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }	
	if ( NULL == ProjectPtr ) {	return true;	}

	CString str;	
	double nTotal = 0;
	double nTotalBad = 0;	
	TTestResult ResultStatistic_AOI = ProjectPtr->GetProjectResultStatistic_Lane(LaneID);
	TTestResult ResultStatistic_ARS = ProjectPtr->GetProjectResultStatistic_ARS_Lane(LaneID);
	DEFECT_FROM_MODE  DefectFrom = DEFECT_FROM_NONE;
	YIELDING_SCOPE YieldingScope = YIELDING_SCOPE_NONE;	
	TTestResult  AOITestResult = ResultStatistic_AOI;
	TTestResult  ARSTestResult = ResultStatistic_ARS;

	if ( LANE_ID_B == LaneID )
	{
		DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LB));
		YieldingScope = (YIELDING_SCOPE)(JetAPI::GetComboxCurSelData(m_YieldScopeCombox_LB));	
	}
	if ( LANE_ID_A == LaneID )
	{
		DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LA));
		YieldingScope = (YIELDING_SCOPE)(JetAPI::GetComboxCurSelData(m_YieldScopeCombox_LA));	
	}

	if ( DEFECT_FROM_AOI == DefectFrom )
	{		
		switch( YieldingScope )
		{
		case YIELDING_SCOPE_TEST:
			nTotal = (double)(AOITestResult.sTest.Total);
			nTotalBad = (double)(AOITestResult.sTest.NG);
			break;
		case YIELDING_SCOPE_PANEL:
			nTotal = (double)(AOITestResult.sPanel.Total);
			nTotalBad = (double)(AOITestResult.sPanel.NG);		
			break;
		case YIELDING_SCOPE_BOARD:
			nTotal = (double)(AOITestResult.sBoard.Total);
			nTotalBad = (double)(AOITestResult.sBoard.NG);		
			break;
		case YIELDING_SCOPE_COMPONENT:
			nTotal = (double)(AOITestResult.sComponent.Total);
			nTotalBad = (double)(AOITestResult.sComponent.NG);	
			break;
		default:
			return true;
		}	
	}
	else if ( DEFECT_FROM_ARS == DefectFrom )
	{
		switch( YieldingScope )
		{
		case YIELDING_SCOPE_TEST:
			nTotal = (double)(ARSTestResult.sTest.Total);
			nTotalBad = (double)(ARSTestResult.sTest.NG);			
			break;
		case YIELDING_SCOPE_PANEL:
			nTotal = (double)(ARSTestResult.sPanel.Total);
			nTotalBad = (double)(ARSTestResult.sPanel.NG);		
			break;
		case YIELDING_SCOPE_BOARD:
			nTotal = (double)(ARSTestResult.sBoard.Total);
			nTotalBad = (double)(ARSTestResult.sBoard.NG);		
			break;
		case YIELDING_SCOPE_COMPONENT:
			nTotal = (double)(ARSTestResult.sComponent.Total);
			nTotalBad = (double)(ARSTestResult.sComponent.NG);	
			break;
		default:
			return true;
		}	
	}
	else
	{
		nTotalBad = 0;
		nTotal = 0;
	}
	
	int    nItem = 0;
	const int nSubItem = 1;
	nItem = 0;
	str = _T("Total");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%.0f"), nTotal);
	ListCtrl.SetItemText(nItem, nSubItem, str);	
	nItem ++;

	str = _T("OK");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%.0f"), nTotal - nTotalBad);
	ListCtrl.SetItemText(nItem, nSubItem, str);	
	nItem ++;

	str = _T("OK(%)");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	if ( nTotal == 0 )
	{	str.Format(_T("0.00%%")); }
	else	
	{	str.Format(_T("%.2f%%"), 100.0*(nTotal - nTotalBad)/nTotal); }
	ListCtrl.SetItemText(nItem, nSubItem, str);	
	nItem ++;
	
	str = _T("NG");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	str.Format(_T("%.0f"), nTotalBad);
	ListCtrl.SetItemText(nItem, nSubItem, str);	
	nItem ++;
	
	str = _T("NG(%)");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertItem(nItem, str);
	if ( nTotal == 0 )
	{	str.Format(_T("0.00%%")); }
	else	
	{	
		if ( YIELDING_SCOPE_COMPONENT == YieldingScope )
		{	str.Format(_T("%.6f%%"), 100.0*(nTotalBad)/nTotal);	}
		else
		{	str.Format(_T("%.2f%%"), 100.0*(nTotalBad)/nTotal);  }
	}
	ListCtrl.SetItemText(nItem, nSubItem, str);	
	nItem ++;

/*
	if ( nTotal == 0 )
	{
		m_ChartWnd_YieldBar.BuildChart();
		return;
	}
*/
	CString strOK = _T("OK");
	CString strNG = _T("NG");
	CString XLabel = _T("");
	CJetSeries *pSeries1 = new CJetSeries();
	CJetSeries *pSeries2 = new CJetSeries();
	if ( NULL==pSeries1 || NULL==pSeries2 )
	{
		delete pSeries1; pSeries1=NULL;
		delete pSeries2; pSeries2=NULL;
		return true;
	}

	strOK = LoadMultiLanguageString(strOK, strOK);
	strNG = LoadMultiLanguageString(strNG, strNG);

	pSeries1->SetSeriesType(SERIES_TYPE_BAR);
	pSeries1->SetIsVisible(true);
	pSeries1->SetLineWidth(50);
	pSeries1->SetLineColor(0x00FF00);	
	pSeries1->SetIsShowMarkValue(true);

	pSeries2->SetSeriesType(SERIES_TYPE_BAR);
	pSeries2->SetIsVisible(true);
	pSeries2->SetLineWidth(50);
	pSeries2->SetLineColor(0x0000FF);	
	pSeries2->SetIsShowMarkValue(true);
	if ( nTotal == 0 )
	{
		XLabel.Format(_T("0.00%%"));
		pSeries1->AddXYValue(true, 0, 0, 0.00, strOK, XLabel);
		pSeries1->AddXYValue(false, 1, 1, 0, strNG, XLabel);
		
		XLabel.Format(_T("0.00%%"));
		pSeries2->AddXYValue(false, 0, 0, 0, _T(""), XLabel);
		pSeries2->AddXYValue(true, 1, 1, 0.00, _T(""), XLabel);
	}
	else
	{
		if ( YIELDING_SCOPE_COMPONENT == YieldingScope )
		{
			XLabel.Format(_T("%.6f%%"), 100*(nTotal-nTotalBad)/nTotal);
			pSeries1->AddXYValue(true, 0, 0, (float)(100*(nTotal-nTotalBad)/nTotal), strOK, XLabel);
			pSeries1->AddXYValue(false, 1, 1, 0, strNG, XLabel);
			
			XLabel.Format(_T("%.6f%%"), 100*(nTotalBad)/nTotal);
			pSeries2->AddXYValue(false, 0, 0, 0, _T(""), XLabel);
			pSeries2->AddXYValue(true, 1, 1, (float)(100*(nTotalBad)/nTotal), _T(""), XLabel);
		}
		else
		{
			XLabel.Format(_T("%.2f%%"), 100*(nTotal-nTotalBad)/nTotal);
			pSeries1->AddXYValue(true, 0, 0, (float)(100*(nTotal-nTotalBad)/nTotal), strOK, XLabel);
			pSeries1->AddXYValue(false, 1, 1, 0, strNG, XLabel);
			
			XLabel.Format(_T("%.2f%%"), 100*(nTotalBad)/nTotal);
			pSeries2->AddXYValue(false, 0, 0, 0, _T(""), XLabel);
			pSeries2->AddXYValue(true, 1, 1, (float)(100*(nTotalBad)/nTotal), _T(""), XLabel);
		}
	}
	ChartWnd.AddSeries(pSeries1);	
	ChartWnd.AddSeries(pSeries2);	
	delete pSeries1; pSeries1=NULL;
	delete pSeries2; pSeries2=NULL;	
	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd_XYChart(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd)//建立圖表視窗-座標圖
{	
	ChartWnd.Clear();
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }
	ChartWnd.Set3DThicness(20);	
	if ( NULL == ProjectPtr ) {	return true;	}

	CString str;
	CString XLabel = _T("");
	CJetSeries *pSeries1 = new CJetSeries();
	if ( NULL == pSeries1 ) { return false; }

	pSeries1->SetSeriesType(SERIES_TYPE_DOT);
	pSeries1->SetIsVisible(true);
	pSeries1->SetDotWidth(2);
	//pSeries1->SetLineColor(0x00FF00);	
	pSeries1->SetDotColor(0x0000FF);
	pSeries1->SetIsShowMarkValue(false);
	
	int   i=0;	
	int   SeriesIndex = 0;
	double OffsetX=0, OffsetY=0;	
	CAOIComponent *ComponentPtr = NULL;
	const int ComponentCount = (int)(ProjectPtr->GetProjectComponentCount());

	SeriesIndex = 0;
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }
		if ( ComponentPtr->CheckComponentModelEnabled() == false ) { continue; }
		
		OffsetX = ComponentPtr->GetComponentResultOffsetX_Lane(LaneID);
		OffsetY = ComponentPtr->GetComponentResultOffsetY_Lane(LaneID);
		pSeries1->AddXYValue(true, SeriesIndex, (float)(OffsetX), (float)(OffsetY));
		SeriesIndex ++;
	}
	
	ChartWnd.SetRadarRadius(-1);
	ChartWnd.AddSeries(pSeries1);
	delete pSeries1; pSeries1=NULL;	
	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd_Top10(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd)//建立圖表視窗-Top-10
{
	ChartWnd.Clear();
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }
	ChartWnd.Set3DThicness(20);		
	if ( NULL == ProjectPtr ) {	return true;	}

	size_t i=0;
	size_t value=0;
	size_t index=0;	
	TTop10Node  *Top10NodePtr = NULL;
	std::vector<TTop10Node> Top10List;
	TOP10_SCOPE  Top10Scope = TOP10_SCOPE_NONE;	
	DEFECT_FROM_MODE  DefectFrom = DEFECT_FROM_NONE;
	if ( LANE_ID_A == LaneID )
	{
		Top10Scope = (TOP10_SCOPE)(JetAPI::GetComboxCurSelData(m_Top10ScopeCombox_LA));	
		DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LA));
		
	}
	if ( LANE_ID_B == LaneID )
	{
		Top10Scope = (TOP10_SCOPE)(JetAPI::GetComboxCurSelData(m_Top10ScopeCombox_LB));	
		DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LB));		
	}
	switch ( Top10Scope )
	{
	case TOP10_SCOPE_COMPONENT:
		if ( ProjectPtr->CalcProjectTop10ListComponent_Lane(LaneID, DefectFrom, Top10List) == false )
		{	return false; }		
		break;
	case TOP10_SCOPE_MODEL:		
		if ( ProjectPtr->CalcProjectTop10ListModel_Lane(LaneID, DefectFrom, Top10List) == false )
		{	return false; }		
		break;
	case TOP10_SCOPE_PART_NUMBER:
		if ( ProjectPtr->CalcProjectTop10ListPartNumber_Lane(LaneID, DefectFrom, Top10List) == false )
		{	return false; }		
		break;
	default:
		break;
	}	

	CString str;
	CString XLabel = _T("");
	CJetSeries *pSeries1 = new CJetSeries();
	if ( pSeries1 == NULL ) { return false;	}

	pSeries1->SetSeriesType(SERIES_TYPE_PIE);
	pSeries1->SetIsVisible(true);
	pSeries1->SetLineWidth(50);
	//pSeries1->SetLineColor(0x00FF00);	
	pSeries1->SetIsShowMarkValue(true);

	const size_t Top10Count = Top10List.size();
	for ( i=0 ; i<Top10Count; i++ )
	{
		Top10NodePtr = &(Top10List[i]);
		switch ( DefectFrom )
		{
		case DEFECT_FROM_ARS:
			value = Top10NodePtr->DefectCountARS;
			break;
		case DEFECT_FROM_AOI:
			value = Top10NodePtr->DefectCountAOI;
			break;
		default:
			value = 0;
			break;
		}		
		if ( value <= 0 )
		{	break; }

		str = Top10NodePtr->NodeName;		
		pSeries1->AddXYValue(true, i, i, (float)(value), str, NULL, NULL);
	}
	ChartWnd.AddSeries(pSeries1);
	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();
	delete pSeries1; pSeries1=NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::BuildChartWnd_DefectStatistic(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CComboBox &FromCombox)//建立圖表視窗-瑕疵統計
{
	ChartWnd.Clear();
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }
	ChartWnd.Set3DThicness(20);	
	if ( NULL == ProjectPtr ) {	return true;	}

	size_t  i=0, j=0, k=0, s=0;		
	CWndDefectItem EachDefectCount;
	CWndDefectItem TotalDefectCount;
	CAOIComponent *ComponentPtr = NULL;	
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=AOIDataCollect.GetWndDefectIDList();
	const size_t WndDefectIDCount = WndDefectIDList.size();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	DEFECT_FROM_MODE  DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(FromCombox));

	EachDefectCount.SetAll(0);
	TotalDefectCount.SetAll(0);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }
		if ( ComponentPtr->CheckComponentModelEnabled() == false ) { continue; }
				
		if ( DEFECT_FROM_ARS == DefectFrom )
		{	ComponentPtr->GetComponentTotalEachDefectCountARS_Lane(LaneID, EachDefectCount);	}
		else if ( DEFECT_FROM_AOI == DefectFrom )
		{	ComponentPtr->GetComponentTotalEachDefectCountAOI_Lane(LaneID, EachDefectCount);	}
		TotalDefectCount.AddWndDefectItemCount(EachDefectCount);		
	}

	int TotalCount = TotalDefectCount.CalcSum();
	if ( TotalCount == 0 ) 	
	{	
		ChartWnd.BuildChart();
		ChartWnd.RedrawWindow();	
		return true; 
	}

	int       Index=0;	
	double    value = 0;
	CString   str;
	CString   XLabel = _T("");	
	CSortObj  SortNode;
	CSortObj *pSortNode=NULL;	
	std::vector<CSortObj> SortList;		
	std::vector<double>TotalDefectRatio(WndDefectIDCount);	
	WND_DEFECT_ID WndDefectID=WND_DEFECT_NONE;
	
	CJetSeries *pSeries1 = new CJetSeries();	
	if ( pSeries1 == NULL )
	{	return false;	}

	SortNode.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<WndDefectIDCount; i++ )
	{
		WndDefectID = WndDefectIDList[i];
		TotalDefectRatio[i] = TotalDefectCount.GetItemCount(WndDefectID);
		TotalDefectRatio[i] = 100.0*TotalDefectRatio[i]/TotalCount;

		SortNode.SetID(i);			
		SortNode.SetValueDbl(TotalDefectRatio[i]);
		SortList.push_back(SortNode);
	}	
	
	std::sort(SortList.begin(), SortList.end());
	const size_t NSortDatas = SortList.size();
	j = 0;
	for ( i=NSortDatas-1; i!=-1; i-- )
	{
		pSortNode = &(SortList[i]);
		Index = (int)(pSortNode->GetID());
		if ( Index<0 || Index >=WndDefectIDCount ) { continue; }
		value = pSortNode->GetValueDbl();
		if ( value <= 0.0 ) { continue; }

		WndDefectID = WndDefectIDList[Index];
		str = AOIDataDefine.GetWndDefectIDText(WndDefectID);

		pSeries1->AddXYValue(true, j, j, (float)(value), str, NULL, NULL);
		j ++;
	}
	ChartWnd.AddSeries(pSeries1);	
	delete pSeries1; pSeries1=NULL;

	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeDefectFromCombo_LA() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_A);	
	DEFECT_FROM_MODE    DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LA));
	AOIDataCollect.SetOnlineViewDefectFromeMode(DefectFrom);	
	BuildChartWnd(LANE_ID_A, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeDefectFromCombo_LB() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_B);	
	DEFECT_FROM_MODE    DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox_LB));
	AOIDataCollect.SetOnlineViewDefectFromeMode(DefectFrom);	
	BuildChartWnd(LANE_ID_B, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeTop10ScopeCombo_LA() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_A);	
	BuildChartWnd(LANE_ID_A, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeTop10ScopeCombo_LB() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_B);	
	BuildChartWnd(LANE_ID_B, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnClearStatisticBtn_LA() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_A);
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	str = _T("Do you want to clear the statistic records?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	ProjectPtr->ResetProjectStatisticRecords();	
	ExecInspection_Finish_LA(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnClearStatisticBtn_LB() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_B);
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	str = _T("Do you want to clear the statistic records?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	ProjectPtr->ResetProjectStatisticRecords();	
	ExecInspection_Finish_LB(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeYieldingScopeCombo_LA() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_A);
	BuildChartWnd(LANE_ID_A, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeYieldingScopeCombo_LB() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject(LANE_ID_B);
	BuildChartWnd(LANE_ID_B, ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeProjectIDCombo_LA() 
{
	// TODO: Add your control notification handler code here	
	size_t ProjectIndex = 0;	
	CAOIProject *ProjectPtr = NULL;
	CComboBox  &Combox = m_ProjectIDCombox_LA;
	ProjectIndex = JetAPI::GetComboxCurSelData(Combox);
	ProjectPtr = AOIDataCollect.GetProjectPtr(ProjectIndex, true);
	if ( NULL == ProjectPtr ) { return; }

	if ( ProjectPtr != m_ProjectPtr_LA )
	{	ExecSwitchProject_LA(ProjectPtr);	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_UPDATE_DOCUMENT_TITLE, NULL);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnSelchangeProjectIDCombo_LB() 
{
	// TODO: Add your control notification handler code here	
	size_t ProjectIndex = 0;	
	CAOIProject *ProjectPtr = NULL;
	CComboBox  &Combox = m_ProjectIDCombox_LB;
	ProjectIndex = JetAPI::GetComboxCurSelData(Combox);
	ProjectPtr = AOIDataCollect.GetProjectPtr(ProjectIndex, true);
	if ( NULL == ProjectPtr ) { return; }

	if ( ProjectPtr != m_ProjectPtr_LB )
	{	ExecSwitchProject_LB(ProjectPtr);	}
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_UPDATE_DOCUMENT_TITLE, NULL);
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ResetMachineStates()//復歸機台狀態
{
	int val = -1;
	m_LastStationSend_LA = val;
	m_LastStationRecieve_LA = val;
	m_NextStationSend_LA = val;
	m_NextStationSendOK_LA = val;
	m_NextStationSendNG_LA = val;
	m_NextStationRecieve_LA = val;
	m_LaneSensorPCBIn_LA = val;
	m_LaneSensorPCBSlow_LA = val;
	m_LaneSensorPCBStop_LA = val;
	m_LaneSensorPCBStop2_LA = val;
	m_LaneSensorPCBOut_LA = val;

	m_LastStationSend_LB = val;
	m_LastStationRecieve_LB = val;
	m_NextStationSend_LB = val;
	m_NextStationSendOK_LB = val;
	m_NextStationSendNG_LB = val;
	m_NextStationRecieve_LB = val;
	m_LaneSensorPCBIn_LB = val;
	m_LaneSensorPCBSlow_LB = val;
	m_LaneSensorPCBStop_LB = val;
	m_LaneSensorPCBStop2_LB = val;
	m_LaneSensorPCBOut_LB = val;

	m_MachineFrontCap = val;
	m_MachineRearCap = val;
	m_MachineAirLost = val;
	m_MachineEMSOn = val;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::UpdateMachineStates(bool bReadPLC)//更新機台狀態
{
	int nValue = 0;
	CString   strVal;
	CString   strVal2;
	const int TurnOn = 1;		
	LANE_ID   LaneID = LANE_ID_A;
	nValue = PlcCtrlPtr->GetIsPCBRightInDirection();
	if ( true == bReadPLC )
	{	PlcCtrlPtr->PLC_ReadAllStats(false);	}
	/*
	if ( nValue != m_PCBInDirection )
	{
		if ( TurnOn == nValue )
		{
			this->m_PCBRightInWnd.SetBitmap(m_LEDGreen);
			this->m_PCBLeftInWnd.SetBitmap(m_LEDGray);	
		}
		else
		{
			this->m_PCBRightInWnd.SetBitmap(m_LEDGray);
			this->m_PCBLeftInWnd.SetBitmap(m_LEDGreen);		
		}
	}*/		
	
	//Conveyer - Lane A
	LaneID = LANE_ID_A;
	nValue = PlcCtrlPtr->GetConveryerStatus(LaneID);		
	PlcCtrlPtr->GetConveryerStatusText(nValue, strVal);	
	strVal2.Format(_T("A: %s"), strVal);
	CWnd::GetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT_LA, strVal);
	if ( strVal.CompareNoCase(strVal2) != 0 )
	{	CWnd::SetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT_LA, strVal2);	}

	nValue = PlcCtrlPtr->GetConveryerSensorPCBIn(LaneID);
	if ( nValue != m_LaneSensorPCBIn_LA )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBInIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBInIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBIn_LA = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow(LaneID);
	if ( nValue != m_LaneSensorPCBSlow_LA )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorSlowDownIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorSlowDownIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBSlow_LA = nValue;			

	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
	if ( nValue != m_LaneSensorPCBStop_LA )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop_LA = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID);
	if ( nValue != m_LaneSensorPCBOut_LA )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBOutIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBOutIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBOut_LA = nValue;
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2(LaneID);
	if ( nValue != m_LaneSensorPCBStop2_LA )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon2_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon2_LA.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop2_LA = nValue;	

	nValue = PlcCtrlPtr->GetLastStationSignal(LaneID);
	if ( nValue != m_LastStationRecieve_LA )
	{
		if ( TurnOn == nValue )
		{	m_LastStationRecieveIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_LastStationRecieveIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LastStationRecieve_LA = nValue;

	nValue = PlcCtrlPtr->GetSendToLastStation(LaneID);
	if ( nValue != m_LastStationSend_LA )
	{
		if ( TurnOn == nValue )
		{	m_LastStationSendIcon_LA.SetBitmap(m_LEDYellow); }
		else
		{	m_LastStationSendIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_LastStationSend_LA = nValue;

	nValue = PlcCtrlPtr->GetNextStationSignal(LaneID);
	if ( nValue != m_NextStationRecieve_LA )
	{
		if ( TurnOn == nValue )
		{	m_NextStationRecieveIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationRecieveIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_NextStationRecieve_LA = nValue;	

	nValue = PlcCtrlPtr->GetSendToNextStation(LaneID);
	if ( nValue != m_NextStationSend_LA )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendIcon_LA.SetBitmap(m_LEDYellow); }
		else
		{	m_NextStationSendIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_NextStationSend_LA = nValue;	
	
	nValue = PlcCtrlPtr->GetSendToNextStationOK(LaneID);
	if ( nValue != m_NextStationSendOK_LA )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendOKIcon_LA.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationSendOKIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendOK_LA = nValue;

	nValue = PlcCtrlPtr->GetSendToNextStationNG(LaneID);
	if ( nValue != m_NextStationSendNG_LA )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendNGIcon_LA.SetBitmap(m_LEDRed); }
		else
		{	m_NextStationSendNGIcon_LA.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendNG_LA = nValue;	
	
	//Conveyer - Lane B
	LaneID = LANE_ID_B;
	nValue = PlcCtrlPtr->GetConveryerStatus(LaneID);
	PlcCtrlPtr->GetConveryerStatusText(nValue, strVal);
	strVal2.Format(_T("B: %s"), strVal);
	CWnd::GetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT_LB, strVal);
	if ( strVal.CompareNoCase(strVal2) != 0 )
	{	CWnd::SetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT_LB, strVal2);	}

	nValue = PlcCtrlPtr->GetConveryerSensorPCBIn(LaneID);
	if ( nValue != m_LaneSensorPCBIn_LB )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBInIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBInIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBIn_LB = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow(LaneID);
	if ( nValue != m_LaneSensorPCBSlow_LB )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorSlowDownIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorSlowDownIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBSlow_LB = nValue;			

	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
	if ( nValue != m_LaneSensorPCBStop_LB )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop_LB = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID);
	if ( nValue != m_LaneSensorPCBOut_LB )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBOutIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBOutIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBOut_LB = nValue;
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2(LaneID);
	if ( nValue != m_LaneSensorPCBStop2_LB )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon2_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon2_LB.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop2_LB = nValue;		
	
	nValue = PlcCtrlPtr->GetLastStationSignal(LaneID);
	if ( nValue != m_LastStationRecieve_LB )
	{
		if ( TurnOn == nValue )
		{	m_LastStationRecieveIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_LastStationRecieveIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LastStationRecieve_LB = nValue;

	nValue = PlcCtrlPtr->GetSendToLastStation(LaneID);
	if ( nValue != m_LastStationSend_LB )
	{
		if ( TurnOn == nValue )
		{	m_LastStationSendIcon_LB.SetBitmap(m_LEDYellow); }
		else
		{	m_LastStationSendIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_LastStationSend_LB = nValue;

	nValue = PlcCtrlPtr->GetNextStationSignal(LaneID);
	if ( nValue != m_NextStationRecieve_LB )
	{
		if ( TurnOn == nValue )
		{	m_NextStationRecieveIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationRecieveIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_NextStationRecieve_LB = nValue;	

	nValue = PlcCtrlPtr->GetSendToNextStation(LaneID);
	if ( nValue != m_NextStationSend_LB )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendIcon_LB.SetBitmap(m_LEDYellow); }
		else
		{	m_NextStationSendIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_NextStationSend_LB = nValue;	
	
	nValue = PlcCtrlPtr->GetSendToNextStationOK(LaneID);
	if ( nValue != m_NextStationSendOK_LB )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendOKIcon_LB.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationSendOKIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendOK_LB = nValue;

	nValue = PlcCtrlPtr->GetSendToNextStationNG(LaneID);
	if ( nValue != m_NextStationSendNG_LB )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendNGIcon_LB.SetBitmap(m_LEDRed); }
		else
		{	m_NextStationSendNGIcon_LB.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendNG_LB = nValue;	
	
	/*
	nValue = PlcCtrlPtr->GetCurrentFrontCapStats();
	if ( nValue != m_MachineFrontCap )
	{
		if ( TurnOn == nValue )
		{	m_MachineFrontCapIcon.SetBitmap(m_LEDRed); }
		else
		{	m_MachineFrontCapIcon.SetBitmap(m_LEDGray); }
	}
	m_MachineFrontCap = nValue;		

	nValue = PlcCtrlPtr->GetCurrentRearCapStats();
	if ( nValue != m_MachineRearCap )
	{
		if ( TurnOn == nValue )
		{	m_MachineRearCapIcon.SetBitmap(m_LEDRed); }
		else
		{	m_MachineRearCapIcon.SetBitmap(m_LEDGray); }
	}
	m_MachineRearCap = nValue;	

	nValue = PlcCtrlPtr->GetCurrentAirStats();
	if ( nValue != m_MachineAirLost )
	{
		if ( TurnOn == nValue )
		{	m_MachineAirLostIcon.SetBitmap(m_LEDRed); }
		else
		{	m_MachineAirLostIcon.SetBitmap(m_LEDGray); }
	}
	m_MachineAirLost = nValue;	

	nValue = PlcCtrlPtr->GetCurrentEMSStats();
	if ( nValue != m_MachineEMSOn )
	{
		if ( TurnOn == nValue )
		{	m_MachineEMSOnIcon.SetBitmap(m_LEDRed); }
		else
		{	m_MachineEMSOnIcon.SetBitmap(m_LEDGray); }
	}
	m_MachineEMSOn = nValue;		
	*/

	CButton* pBtn = NULL;
#ifndef OFFLINE_VERSION
	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	LANE_STATE_MODE TaskLaneStateMode_LA, TaskLaneStateMode_LB;

	TaskLaneStateMode_LA = AOIDataCollect.GetOnlineLaneTaskState(LANE_ID_A);
	TaskLaneStateMode_LB = AOIDataCollect.GetOnlineLaneTaskState(LANE_ID_B);
	bool bLockUI = AOIDataCollect.GetIsLockUIWnd();
	bool bToStop = AOIDataCollect.CheckStopOnlineTask();
	bool bRemoteState = AOIDataCollect.CheckMES_CtrlState(MES_EQP_CTRL_STATE_REMOTE);
	const bool bMultiLaneModeOff = AOIDataCollect.CheckMultiLaneMode_Off();
	const bool bOfflineState = AOIDataCollect.CheckMES_CtrlState(MES_EQP_CTRL_STATE_OFFLINE);
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	BOOL bBtnEnable;
	if (true == bRemoteState)
	{
		if (FN_ENABLE == AOIDataCollect.GetSystemParameter().m_ITSSetSecsGemRemoteLocal)
		{
			bRemoteState = false;
		}
	}

	
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_RUN_BTN_LA);
	bBtnEnable = TRUE;
	if (NULL == ProjectPtr || true == bRemoteState || true == bOfflineState){
		bBtnEnable = FALSE;
	}
	else if (true == bLockUI) {
		if (true == bToStop || LANE_STATE_RUNNING == TaskLaneStateMode_LA || LANE_STATE_BYPASS == TaskLaneStateMode_LA) {
			bBtnEnable = FALSE;
		}
	}
	pBtn->EnableWindow(bBtnEnable);
	
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_RUN_BTN_LB);
	bBtnEnable = TRUE;
	if (NULL == ProjectPtr || true == bRemoteState || true == bOfflineState) {
		bBtnEnable = FALSE;
	}
	else if (true == bLockUI) {
		if (true == bToStop || LANE_STATE_RUNNING == TaskLaneStateMode_LB || LANE_STATE_BYPASS == TaskLaneStateMode_LB) {
			bBtnEnable = FALSE;
		}
	}
	pBtn->EnableWindow(bBtnEnable);

	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_BYPASS_BTN_LA);
	bBtnEnable = TRUE;
	if (NULL == ProjectPtr || true == bRemoteState || true == bOfflineState) {
		bBtnEnable = FALSE;
	}
	else if (true == bLockUI) {
		if (true == bToStop || LANE_STATE_RUNNING == TaskLaneStateMode_LA || LANE_STATE_BYPASS == TaskLaneStateMode_LA) {
			bBtnEnable = FALSE;
		}
	}
	pBtn->EnableWindow(bBtnEnable);

	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_BYPASS_BTN_LB);
	bBtnEnable = TRUE;
	if (NULL == ProjectPtr || true == bRemoteState || true == bOfflineState) {
		bBtnEnable = FALSE;
	}
	else if (true == bLockUI) {
		if (true == bToStop || LANE_STATE_RUNNING == TaskLaneStateMode_LB || LANE_STATE_BYPASS == TaskLaneStateMode_LB) {
			bBtnEnable = FALSE;
		}
	}
	pBtn->EnableWindow(bBtnEnable);

#else
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_RUN_BTN_LA);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_RUN_BTN_LB);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_BYPASS_BTN_LA);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_BYPASS_BTN_LB);
	pBtn->EnableWindow(FALSE);
#endif//OFFLINE_VERSION

#ifndef OFFLINE_VERSION
	//	TASK_STATE_MODE  TaskStateMode = AOIDataCollect.GetOnlineTaskState();

	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_STOP_BTN_LA);
	if (TASK_STATE_IDLE == TaskStateMode || TASK_STATE_NONE == TaskStateMode || LANE_STATE_PAUSE == TaskLaneStateMode_LA)
	{
		pBtn->EnableWindow(FALSE);
	}
	else { pBtn->EnableWindow(TRUE); }

	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_STOP_BTN_LB);
	if (TASK_STATE_IDLE == TaskStateMode || TASK_STATE_NONE == TaskStateMode || LANE_STATE_PAUSE == TaskLaneStateMode_LB)
	{
		pBtn->EnableWindow(FALSE);
	}
	else { pBtn->EnableWindow(TRUE); }
#else
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_STOP_BTN_LA);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(ONLINE_LANE_STOP_BTN_LB);
	pBtn->EnableWindow(FALSE);
#endif//OFFLINE_VERSION

	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecProjectClose(LANE_ID LaneID)//關閉專案
{
	if ( GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject(LaneID);
	if ( NULL == ProjectPtr ) { return true; }
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();		
	//if ( ProjectPtr->GetProjectLaneEnable(LaneID) == false )
	//{	return true; }
	
	if ( MULTI_LANE_1 != MultiLaneMode )
	{	AOIDataCollect.SetActiveLaneID(LaneID);	}
	AOIDataCollect.SetActiveProjectPtr(ProjectPtr);
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_PROJECT_CLOSE, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecOnlineRun()
{
	THREAD_STATE_MODE ThreadStateMode = AOIDataCollect.GetOnlineInspectionThreadState();
	if (THREAD_STATE_RUNNING == ThreadStateMode) { return true; }
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_RUN, NULL);

	const size_t MaxCount = 10;
	size_t Count = 0;
	while (false == AOIDataCollect.WaitForOnlineInspectionThreadStart()) {
		Count++;
		if (Count < MaxCount) {
			JetAPI::ShowMessageBox(_T("Wait For Online Inspection Thread too long"));
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView_Dual::ExecOnlineBypass()
{
	THREAD_STATE_MODE ThreadStateMode = AOIDataCollect.GetOnlineInspectionThreadState();
	if (THREAD_STATE_RUNNING == ThreadStateMode) { return true; }
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_BYPASS, NULL);

	const size_t MaxCount = 10;
	size_t Count = 0;
	while (false == AOIDataCollect.WaitForOnlineInspectionThreadStart()) {
		Count++;
		if (Count < MaxCount) {
			JetAPI::ShowMessageBox(_T("Wait For Online Inspection Thread too long"));
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnProjectCloseBtnLA() 
{
	// TODO: Add your control notification handler code here
	ExecProjectClose(LANE_ID_A);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnProjectCloseBtnLB() 
{
	// TODO: Add your control notification handler code here
	ExecProjectClose(LANE_ID_B);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneStopBtnLA()
{
	if (AOIDataCollect.OperateLevelOnlineStop() == false) { return; }
	LANE_STATE_MODE  LaneStateMode = AOIDataCollect.GetOnlineLaneTaskState(LANE_ID_B);
	if (LANE_STATE_PAUSE != LaneStateMode) {
		AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_PAUSE);
		AOIDataCollect.UserLogout_Check();
		return;
	}
	//如果兩條線都關閉，OnlineTaskState 改為stop
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_STOP, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneStopBtnLB()
{
	if (AOIDataCollect.OperateLevelOnlineStop() == false) { return; }
	LANE_STATE_MODE  LaneStateMode = AOIDataCollect.GetOnlineLaneTaskState(LANE_ID_A);
	if (LANE_STATE_PAUSE != LaneStateMode) {
		AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_PAUSE);
		AOIDataCollect.UserLogout_Check();
		return;
	}
	//如果兩條線都關閉，OnlineTaskState 改為stop
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_ONLINE_STOP, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneRunBtnLA()
{
	if (AOIDataCollect.OperateLevelOnlineRun() == false) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_RUNNING);

	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if (TASK_STATE_RUNNING == TaskStateMode) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_PAUSE);
	if (false == ExecOnlineRun()) { return; }
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneRunBtnLB()
{
	if (AOIDataCollect.OperateLevelOnlineRun() == false) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_RUNNING);

	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if (TASK_STATE_RUNNING == TaskStateMode) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_PAUSE);
	if (false == ExecOnlineRun()) { return; }
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneBypassBtnLA()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (AOIDataCollect.OperateLevelOnlineBypass() == false) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_BYPASS);

	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if (TASK_STATE_RUNNING == TaskStateMode) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_PAUSE);
	//if (false == ExecOnlineBypass()) { return; }
	if (false == ExecOnlineRun()) { return; }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView_Dual::OnBnClickedLaneBypassBtnLB()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (AOIDataCollect.OperateLevelOnlineBypass() == false) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_B, LANE_STATE_BYPASS);

	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if (TASK_STATE_RUNNING == TaskStateMode) { return; }
	AOIDataCollect.SetOnlineLaneTaskState(LANE_ID_A, LANE_STATE_PAUSE);
	//if (false == ExecOnlineBypass()) { return; }
	if (false == ExecOnlineRun()) { return; }
}
//-------------------------------------------------------------------------------------//