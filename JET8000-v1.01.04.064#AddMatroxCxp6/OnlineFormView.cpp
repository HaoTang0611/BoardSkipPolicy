// OnlineFormView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "OnlineFormView.h"
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
// COnlineFormView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(COnlineFormView, CFormView)
//-------------------------------------------------------------------------------------//
COnlineFormView::COnlineFormView()
	: CFormView(COnlineFormView::IDD)
{
	//{{AFX_DATA_INIT(COnlineFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LaneID = LANE_ID_A;
	m_ProjectPtr = NULL;

	ResetMachineStates();	
}
//-------------------------------------------------------------------------------------//
COnlineFormView::~COnlineFormView()
{
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlineFormView)	
	DDX_Control(pDX, ONLINE_PROJECT_ID_COMBO, m_ProjectIDCombox);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_ICON, m_ResultIcon);
	DDX_Control(pDX, ONLINE_PROJECT_RESULT_LIST_WND, m_ResultListCtrl);
	DDX_Control(pDX, ONLINE_MACHINE_FRONT_CAP_ICON, m_MachineFrontCapIcon);	
	DDX_Control(pDX, ONLINE_MACHINE_REAR_CAP_ICON, m_MachineRearCapIcon);	
	DDX_Control(pDX, ONLINE_MACHINE_AIR_LOST_ICON, m_MachineAirLostIcon);	
	DDX_Control(pDX, ONLINE_MACHINE_EMS_ON_ICON, m_MachineEMSOnIcon);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_ICON, m_NextStationSendIcon);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_ICON_LB, m_NextStationSendIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_OK_ICON, m_NextStationSendOKIcon);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_OK_ICON_LB, m_NextStationSendOKIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_NG_ICON, m_NextStationSendNGIcon);
	DDX_Control(pDX, ONLINE_NEXT_STATION_SEND_NG_ICON_LB, m_NextStationSendNGIcon_LB);
	DDX_Control(pDX, ONLINE_NEXT_STATION_RECIEVE_ICON, m_NextStationRecieveIcon);	
	DDX_Control(pDX, ONLINE_NEXT_STATION_RECIEVE_ICON_LB, m_NextStationRecieveIcon_LB);	
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_IN_ICON, m_LaneSensorPCBInIcon);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_IN_ICON_LB, m_LaneSensorPCBInIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON, m_LaneSensorSlowDownIcon);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LB, m_LaneSensorSlowDownIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON, m_LaneSensorPCBStopIcon);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON2, m_LaneSensorPCBStopIcon2);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB, m_LaneSensorPCBStopIcon_LB);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB2, m_LaneSensorPCBStopIcon_LB2);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_OUT_ICON, m_LaneSensorPCBOutIcon);
	DDX_Control(pDX, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LB, m_LaneSensorPCBOutIcon_LB);
	DDX_Control(pDX, ONLINE_LAST_STATION_SEND_ICON, m_LastStationSendIcon);
	DDX_Control(pDX, ONLINE_LAST_STATION_SEND_ICON_LB, m_LastStationSendIcon_LB);
	DDX_Control(pDX, ONLINE_LAST_STATION_RECIEVE_ICON, m_LastStationRecieveIcon);
	DDX_Control(pDX, ONLINE_LAST_STATION_RECIEVE_ICON_LB, m_LastStationRecieveIcon_LB);
	DDX_Control(pDX, ONLINE_INFO_LIST_WND, m_InfoListWnd);	
	DDX_Control(pDX, ONLINE_YIELDING_LIST_WND, m_YieldingListWnd);	
	DDX_Control(pDX, ONLINE_YIELDING_SCOPE_COMBO, m_YieldScopeCombox);
	DDX_Control(pDX, ONLINE_TOP10_SCOPE_COMBO, m_Top10ScopeCombox);
	DDX_Control(pDX, ONLINE_DEFECT_FROM_COMBO, m_DefectFromCombox);	
	DDX_Control(pDX, ONLINE_CPK_TYPE_COMBO, m_CpkTypeCombox);
	DDX_Control(pDX, ONLINE_CHART_WND_DEFECT, m_ChartWnd_Defect);
	DDX_Control(pDX, ONLINE_CHART_WND_XY, m_ChartWnd_XYChart);
	DDX_Control(pDX, ONLINE_CHART_WND_TOP10, m_ChartWnd_Top10);
	DDX_Control(pDX, ONLINE_CHART_WND_YIELDING, m_ChartWnd_Yielding);
	DDX_Control(pDX, ONLINE_CHART_WND_CPK01, m_ChartWnd_CPK01);
	DDX_Control(pDX, ONLINE_CHART_WND_CPK02, m_ChartWnd_CPK02);
	DDX_Control(pDX, ONLINE_CHART_WND_CPK03, m_ChartWnd_CPK03);
	DDX_Control(pDX, ONLINE_CHART_WND_CPK04, m_ChartWnd_CPK04);
	DDX_Control(pDX, ONLINE_CHART_WND_CPK05, m_ChartWnd_CPK05);
	DDX_Control(pDX, ONLINE_CHART_TAB, m_ChartTab);
	DDX_Control(pDX, ONLINE_DEFECT_MODE_COMBO, m_DefectModeCombox);
	DDX_Control(pDX, ONLINE_DEFECT_LIST_WND, m_DefectListCtrl);
	DDX_Control(pDX, ONLINE_PROJECT_INFO_LIST_WND, m_ProjectInfoListCtrl);
	DDX_Control(pDX, ONLINE_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(COnlineFormView, CFormView)
	//{{AFX_MSG_MAP(COnlineFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()	
	ON_WM_TIMER()	
	ON_CBN_SELCHANGE(ONLINE_DEFECT_MODE_COMBO, OnSelchangeDefectModeCombo)
	ON_NOTIFY(TCN_SELCHANGE, ONLINE_CHART_TAB, OnSelchangeChartTab)
	ON_CBN_SELCHANGE(ONLINE_DEFECT_FROM_COMBO, OnSelchangeDefectFromCombo)
	ON_CBN_SELCHANGE(ONLINE_TOP10_SCOPE_COMBO, OnSelchangeTop10ScopeCombo)
	ON_BN_CLICKED(ONLINE_CLEAR_STATISTIC_BTN, OnClearStatisticBtn)
	ON_CBN_SELCHANGE(ONLINE_YIELDING_SCOPE_COMBO, OnSelchangeYieldingScopeCombo)	
	ON_CBN_SELCHANGE(ONLINE_CPK_TYPE_COMBO, OnSelchangeCpkTypeCombo)	
	ON_CBN_SELCHANGE(ONLINE_PROJECT_ID_COMBO, OnSelchangeProjectIDCombo)	
	ON_BN_CLICKED(ONLINE_MACHINE_BUTTON_START_BTN, OnMachineButtonStartBtn)
	ON_BN_CLICKED(ONLINE_MACHINE_BUTTON_RESET_BTN, OnMachineButtonResetBtn)
	ON_BN_CLICKED(ONLINE_MACHINE_BUTTON_STOP_BTN, OnMachineButtonStopBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void COnlineFormView::AssertValid() const
{
	CFormView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
//-------------------------------------------------------------------------------------//
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView message handlers
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	AdjustCtrlWnd();
	LANE_ID LaneID = GetActiveLaneID();
	InitImageWnd(m_ImageWnd);
	m_DefectListCtrl.SetOwner(this);
	m_ProjectInfoListCtrl.SetOwner(this);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	CString str;	
	JetAPI::InitialListCtrl(m_DefectListCtrl);	
	JetAPI::InitialListCtrl(m_ResultListCtrl);
	JetAPI::InitialListCtrl(m_ProjectInfoListCtrl);		
	JetAPI::InitialListCtrl(m_InfoListWnd);
	JetAPI::InitialListCtrl(m_YieldingListWnd);

	//AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);
	str = AOIDataDefine.GetTestResultImageName(TEST_RESULT_NONE);
	m_DibResult.Load(str);
	m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);
	m_LEDYellow.LoadBitmap(IDB_LED_MEDIAN_YELLOW);	
	SwitchMultiLanguage();

	InitChartTab(m_ChartTab);
	InitChartWnd();	
	BuildResultListWndHeader(m_ResultListCtrl);
	BuildDefectListWndHeader(m_DefectListCtrl);	
	BuildProjectInfoListWndHeader(m_ProjectInfoListCtrl);
	
	BuildProjectIDCombox(LaneID, m_ProjectIDCombox);
	BuildDefectModeCombox(m_DefectModeCombox);
	UpdateMultiLaneUI();
#ifdef PLC_OBJ_DISABLE
	BOOL bEnabled = FALSE;
	JetAPI::EnableCtrlWnd(this, ONLINE_MACHINE_BUTTON_START_BTN, bEnabled);	
	JetAPI::EnableCtrlWnd(this, ONLINE_MACHINE_BUTTON_RESET_BTN, bEnabled);
	JetAPI::EnableCtrlWnd(this, ONLINE_MACHINE_BUTTON_STOP_BTN, bEnabled);	
#endif//PLC_OBJ_DISABLE		

	CWnd::SetTimer(ONLINE_VIEW_TIMER_UPDATE_MACHINE_STATE, 100, NULL);
	CWnd::PostMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_INITIAL_UPDATE, NULL);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustCtrlWnd();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{		
		AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
		ExecSelchangeChartTab();
	}	
	else
	{
		int a = 3;
		a = 7;
	}
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnTimer(UINT_PTR nIDEvent) 
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
bool COnlineFormView::AdjustCtrlWnd()//調整控制項檢測框位置
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return true; }
	if ( CWnd::IsWindowVisible() == FALSE )  { return true; }

	POINT WndOffset={0};
	SIZE WndSize={0};
	RECT TabRect={0};
	RECT LaneRect={0};
	RECT FullRect={0};
	RECT ChartRect={0};	
	RECT ChartTabRect={0};
	RECT TopComboRect={0};	
	RECT FromComboRect={0};	
	CWnd *WndPtr = NULL;
	const int MarginX=4;
	const int MarginY=4;
	CWnd::GetClientRect(&FullRect);
	WndSize.cx = FullRect.right-FullRect.left;
	WndSize.cy = FullRect.bottom-FullRect.top;
	if ( 0==WndSize.cx || 0==WndSize.cy ) 
	{	return true; }

	//先找個控制項來確定是否可以進入調整
	WndPtr = CWnd::GetDlgItem(ONLINE_DEFECT_FROM_COMBO);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&FromComboRect);
		CWnd::ScreenToClient(&FromComboRect);
	}

	if ( m_YieldScopeCombox.GetSafeHwnd() == NULL )
	{	return true; }

	//Lane A	
	if ( m_YieldScopeCombox.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldScopeCombox.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		TopComboRect = WndRect;
		ChartRect.top = TopComboRect.bottom + MarginY;
	}
	if ( m_Top10ScopeCombox.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=TopComboRect;
		m_Top10ScopeCombox.MoveWindow(&WndRect);		
	}
	ChartRect.bottom = FullRect.bottom - MarginY;

	CThisChartCtrl &ChartWnd1 = m_ChartWnd_Yielding;
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
	if ( m_ChartTab.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_ChartTab.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = ChartRect.right;
		ChartTabRect.left = WndRect.left;
		TabRect = WndRect;
		//m_ChartTab.MoveWindow(&WndRect);		
	}

	CThisChartCtrl &ChartWnd2 = m_ChartWnd_Top10;
	if ( ChartWnd2.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartRect;
		WndRect.top = TopComboRect.bottom+MarginY;
		WndRect.left = ChartTabRect.left;
		ChartWnd2.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd3 = m_ChartWnd_XYChart;
	if ( ChartWnd3.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd3.MoveWindow(&WndRect);		
	}
	CThisChartCtrl &ChartWnd4 = m_ChartWnd_Defect;
	if ( ChartWnd4.GetSafeHwnd() != NULL )
	{
		RECT  WndRect=ChartTabRect;		
		ChartWnd4.MoveWindow(&WndRect);		
	}	

	if ( m_InfoListWnd.GetSafeHwnd() != NULL )
	{	
		RECT  WndRect=ChartTabRect;		
		WndRect.top = TabRect.bottom+MarginY;
		WndRect.right = FromComboRect.left-MarginX;
		m_InfoListWnd.MoveWindow(&WndRect);
	}

	if ( m_YieldingListWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		m_YieldingListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.bottom = ChartRect.bottom;		
		m_YieldingListWnd.MoveWindow(&WndRect);
	}
	
	//Lane Sensor
	WndPtr = CWnd::GetDlgItem(ONLINE_LAST_STATION_GROUP);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT  WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);		
		LaneRect = WndRect;
		LaneRect.right = FullRect.right-MarginX;
		LaneRect.left = LaneRect.right-WndSize.cx;
		WndOffset.x = ((LaneRect.right+LaneRect.left)-(WndRect.right+WndRect.left))/2;
		WndOffset.y = 0;
	}	
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_A_LABEL, WndOffset);	
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_B_LABEL, WndOffset);

	JetAPI::MoveCtrlWnd(this, ONLINE_PLC_LANE_STATUS_EDIT, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_PLC_LANE_STATUS_EDIT_LB, WndOffset);
	
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_GROUP, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_SEND_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_SEND_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_SEND_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_ICON_LB, WndOffset);

	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_GROUP, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON2, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_LABEL2, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB2, WndOffset);

	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_GROUP, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_ICON_LB, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_LABLE, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_ICON_LB, WndOffset);

	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_STATUS_GROUP, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_FRONT_CAP_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_FRONT_CAP_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_REAR_CAP_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_REAR_CAP_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_AIR_LOST_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_AIR_LOST_LABEL, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_EMS_ON_ICON, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_EMS_ON_LABEL, WndOffset);

	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_BUTTON_GROUP, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_BUTTON_START_BTN, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_BUTTON_STOP_BTN, WndOffset);
	JetAPI::MoveCtrlWnd(this, ONLINE_MACHINE_BUTTON_RESET_BTN, WndOffset);

	BOOL bShowLaneCtrl = TRUE;
#ifdef OFFLINE_VERSION
	bShowLaneCtrl = FALSE;
#endif//OFFLINE_VERSION
	if ( FALSE == bShowLaneCtrl )
	{			
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_A_LABEL, bShowLaneCtrl);	
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_B_LABEL, bShowLaneCtrl);

		JetAPI::ShowCtrlWnd(this, ONLINE_PLC_LANE_STATUS_EDIT, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_PLC_LANE_STATUS_EDIT_LB, bShowLaneCtrl);
	
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_GROUP, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_SEND_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_SEND_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_SEND_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_ICON_LB, bShowLaneCtrl);

		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_GROUP, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON2, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_LABEL2, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB2, bShowLaneCtrl);

		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_GROUP, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_ICON_LB, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_LABLE, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_ICON_LB, bShowLaneCtrl);

		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_STATUS_GROUP, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_FRONT_CAP_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_FRONT_CAP_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_REAR_CAP_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_REAR_CAP_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_AIR_LOST_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_AIR_LOST_LABEL, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_EMS_ON_ICON, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_EMS_ON_LABEL, bShowLaneCtrl);

		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_BUTTON_GROUP, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_BUTTON_START_BTN, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_BUTTON_STOP_BTN, bShowLaneCtrl);
		JetAPI::ShowCtrlWnd(this, ONLINE_MACHINE_BUTTON_RESET_BTN, bShowLaneCtrl);		
	}

	CImageWnd &ImageWnd=m_ImageWnd;	
	const bool ShowCpkChart = GetShowCpkChartWnd();
	if ( ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		//WndRect.left = FullRect.left;
		WndRect.top = FullRect.top+MarginY;
		if ( TRUE == bShowLaneCtrl )
		{	WndRect.right = LaneRect.left-MarginX; }
		else
		{	WndRect.right = FullRect.right-MarginX; }
		
		if ( false == ShowCpkChart )
		{	WndRect.bottom = FullRect.bottom - MarginY;	}		
		else
		{	WndRect.bottom = TopComboRect.top - MarginY;	}
		ImageWnd.MoveWindow(&WndRect);
		ImageWnd.ShowFittedZoom();
	}

	if ( false == ShowCpkChart )
	{
		JetAPI::ShowCtrlWnd(this, ONLINE_CPK_TYPE_COMBO, FALSE);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_CPK01, FALSE);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_CPK02, FALSE);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_CPK03, FALSE);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_CPK04, FALSE);
		JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_CPK05, FALSE);		
	}
	else
	{		
		RECT CpkChartRect;
		RECT ImageWndRect;
		ImageWnd.GetWindowRect(&ImageWndRect);		
		CWnd::ScreenToClient(&ImageWndRect);
		if ( m_CpkTypeCombox.GetSafeHwnd() != NULL )
		{
			RECT  WndRect={0};
			m_CpkTypeCombox.GetWindowRect(&WndRect);
			const int WndSizeW=WndRect.right-WndRect.left;
			const int WndSizeH=WndRect.bottom-WndRect.top;
			WndRect.top = ImageWndRect.bottom+MarginY;
			WndRect.bottom = WndRect.top+WndSizeH;
			WndRect.left = ImageWndRect.left;
			WndRect.right = WndRect.left+WndSizeW;
			m_CpkTypeCombox.MoveWindow(&WndRect);
			ImageWndRect.bottom = WndRect.bottom;
		}

		int nCpkShow = 0;
		const int CpkChartCount = 5;
		const int ImageWndW=ImageWndRect.right-ImageWndRect.left;
		const int CpkChartWndLeft = ImageWndRect.left;
		const int CpkChartPitch= ImageWndW/CpkChartCount;
		const int CpkChartWndW = CpkChartPitch-MarginX;
		CpkChartRect = ImageWndRect;
		CpkChartRect.top = ImageWndRect.bottom+MarginY+MarginY;
		CpkChartRect.bottom = FullRect.bottom-MarginY;		
		if ( m_ChartWnd_CPK01.GetSafeHwnd() != NULL )
		{
			RECT  WndRect=CpkChartRect;
			WndRect.left = CpkChartWndLeft+(nCpkShow*CpkChartPitch);
			WndRect.right = WndRect.left+CpkChartWndW;
			m_ChartWnd_CPK01.MoveWindow(&WndRect);
			nCpkShow ++;
		}
		if ( m_ChartWnd_CPK02.GetSafeHwnd() != NULL )
		{
			RECT  WndRect=CpkChartRect;
			WndRect.left = CpkChartWndLeft+(nCpkShow*CpkChartPitch);
			WndRect.right = WndRect.left+CpkChartWndW;
			m_ChartWnd_CPK02.MoveWindow(&WndRect);
			nCpkShow ++;
		}
		if ( m_ChartWnd_CPK03.GetSafeHwnd() != NULL )
		{
			RECT  WndRect=CpkChartRect;
			WndRect.left = CpkChartWndLeft+(nCpkShow*CpkChartPitch);
			WndRect.right = WndRect.left+CpkChartWndW;
			m_ChartWnd_CPK03.MoveWindow(&WndRect);
			nCpkShow ++;
		}
		if ( m_ChartWnd_CPK04.GetSafeHwnd() != NULL )
		{
			RECT  WndRect=CpkChartRect;
			WndRect.left = CpkChartWndLeft+(nCpkShow*CpkChartPitch);
			WndRect.right = WndRect.left+CpkChartWndW;
			m_ChartWnd_CPK04.MoveWindow(&WndRect);
			nCpkShow ++;
		}
		if ( m_ChartWnd_CPK05.GetSafeHwnd() != NULL )
		{
			RECT  WndRect=CpkChartRect;
			WndRect.left = CpkChartWndLeft+(nCpkShow*CpkChartPitch);
			WndRect.right = WndRect.left+CpkChartWndW;
			m_ChartWnd_CPK05.MoveWindow(&WndRect);
			nCpkShow ++;
		}
	}

	if ( FALSE == bShowLaneCtrl ) 
	{	CWnd::Invalidate(); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::GetShowCpkChartWnd() const
{	//return false;
	return AOIDataCollect.GetCpkChartEnabled();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ONLINE_FORMVIEW");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ONLINE_FORMVIEW;
	WndKey = _T("IDD_ONLINE_FORMVIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = ONLINE_PROJECT_ID_LABEL;
	WndKey = _T("ONLINE_PROJECT_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_PROJECT_INFO_GROUP;
	WndKey = _T("ONLINE_PROJECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ONLINE_DEFECT_INFO_GROUP;
	WndKey = _T("ONLINE_DEFECT_INFO_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_CLEAR_STATISTIC_BTN;
	WndKey = _T("ONLINE_CLEAR_STATISTIC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = ONLINE_PROJECT_RESULT_GROUP;
	WndKey = _T("ONLINE_PROJECT_RESULT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	
	//---------------------------------------------------------------------------------//
	//Last Station
	WndID = ONLINE_LANE_A_LABEL;
	WndKey = _T("ONLINE_LANE_A_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_B_LABEL;
	WndKey = _T("ONLINE_LANE_B_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_GROUP;
	WndKey = _T("ONLINE_LAST_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_SEND_LABLE;
	WndKey = _T("ONLINE_LAST_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LAST_STATION_RECIEVE_LABLE;
	WndKey = _T("ONLINE_LAST_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	//Lane Sensor
	WndID = ONLINE_LANE_GROUP;
	WndKey = _T("ONLINE_LANE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_IN_LABEL;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL;
	WndKey = _T("ONLINE_LANE_SENSOR_SLOW_DOWN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP_LABEL;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_OUT_LABEL;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_OUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_LANE_SENSOR_PCB_STOP_LABEL2;
	WndKey = _T("ONLINE_LANE_SENSOR_PCB_STOP_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	//Last Station
	WndID = ONLINE_NEXT_STATION_GROUP;
	WndKey = _T("ONLINE_NEXT_STATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_LABLE;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_OK_LABLE;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_OK_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_SEND_NG_LABLE;
	WndKey = _T("ONLINE_NEXT_STATION_SEND_NG_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_NEXT_STATION_RECIEVE_LABLE;
	WndKey = _T("ONLINE_NEXT_STATION_RECIEVE_LABLE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ONLINE_MACHINE_STATUS_GROUP;
	WndKey = _T("ONLINE_MACHINE_STATUS_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_FRONT_CAP_LABEL;
	WndKey = _T("ONLINE_MACHINE_FRONT_CAP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_REAR_CAP_LABEL;
	WndKey = _T("ONLINE_MACHINE_REAR_CAP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_AIR_LOST_LABEL;
	WndKey = _T("ONLINE_MACHINE_AIR_LOST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_EMS_ON_LABEL;
	WndKey = _T("ONLINE_MACHINE_EMS_ON_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ONLINE_MACHINE_BUTTON_GROUP;
	WndKey = _T("ONLINE_MACHINE_BUTTON_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_BUTTON_START_BTN;
	WndKey = _T("ONLINE_MACHINE_BUTTON_START_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_BUTTON_RESET_BTN;
	WndKey = _T("ONLINE_MACHINE_BUTTON_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ONLINE_MACHINE_BUTTON_STOP_BTN;
	WndKey = _T("ONLINE_MACHINE_BUTTON_STOP_BTN");
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
CString COnlineFormView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ONLINE_FORMVIEW");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* COnlineFormView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
LANE_ID COnlineFormView::GetActiveLaneID()
{
	LANE_ID  LaneID;
	LaneID = m_LaneID;
	return m_LaneID;	
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildProjectIDCombox(LANE_ID LaneID, CComboBox &Combox)
{
	size_t       i=0;
	int          idx=0;	
	CString      str;	
	CString      str2;	
	CString      strVersionName;
	CAOIProject *ProjectPtr = NULL;
	TVersionCode *PVersionCode = NULL;
	CString      strVersion=AOIDataDefine.GetVersionCodeText();		
	CAOIProject *ProjectPtrAct = AOIDataCollect.GetActiveProject();	
	const size_t ProjectCount = AOIDataCollect.GetProjectPtrCount();
	
	JetAPI::ClearCombox(Combox);

	idx = 0;
	for ( i=0; i<ProjectCount; i++ )
	{
		ProjectPtr = AOIDataCollect.GetProjectPtr(i, false);
		if ( NULL == ProjectPtr ) { continue; }
		//if ( ProjectPtr->GetProjectLaneEnable(LaneID) == false ) { continue; }

		str = ProjectPtr->GetProjectFileMainName();
		PVersionCode = ProjectPtr->GetProjectVersionCodeActivePtr();//取得目前專案版本號
		if ( NULL != PVersionCode )
		{
			str2 = str;
			strVersionName=PVersionCode->wsCodeName.c_str();
			str.Format(_T("%s(%s:%s)"), str2, strVersion, strVersionName);			
		}

		if ( ProjectPtr == ProjectPtrAct ) 
		{	str = str + _T(" *");	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, i);

		idx ++;
	}

	if ( NULL != ProjectPtrAct )
	{
		idx = ProjectPtrAct->GetProjectIndex();
		JetAPI::SetComboxCurSel(Combox, idx);
	}
	else if ( idx > 0 )
	{	Combox.SetCurSel(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildDefectModeCombox(CComboBox &Combox)
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
void COnlineFormView::CloseProject()
{
	CString str;
	m_ProjectPtr = NULL;
	m_ImageWnd.SetProjectPtr(NULL);
	m_ImageWnd.ReleaseImageBuffer();	
	m_ImageWnd.BuildSystemRegion();
	m_ImageWnd.RedrawWnd(true);
	
	str = AOIDataDefine.GetTestResultImageName(TEST_RESULT_NONE);
	m_DibResult.Load(str);

	SetDefectCountEdit(0, 0);
	m_TestResultBarcode = _T("");	
	JetAPI::ClearListCtrl(m_ResultListCtrl, FALSE);
	JetAPI::ClearListCtrl(m_DefectListCtrl, FALSE);
	JetAPI::ClearListCtrl(m_ProjectInfoListCtrl, FALSE);
	JetAPI::ClearCombox(m_ProjectIDCombox);	
	BuildChartWnd(NULL);
	BuildChartWnd_CPK(NULL);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::SwitchLane()
{	
	SwitchProjectMap();
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::SwitchProject()
{
	CloseProject();
	LANE_ID LaneID = GetActiveLaneID();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}		
	BuildProjectIDCombox(LaneID, m_ProjectIDCombox);
	ExecSwitchProject(LaneID, ProjectPtr);		
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::SwitchProjectMap()
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return; }
	TPOINT2D   ImageRes;		
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;

	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.RedrawWnd(false);
	return;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::SwitchProjectMark()
{
	CAOIProject *ProjectPtrCur = GetActiveProject();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( ProjectPtr == ProjectPtrCur ) { return; }
	SwitchProject();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::ExecSwitchProject(LANE_ID LaneID, CAOIProject *ProjectPtr)
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

	m_ProjectPtr = ProjectPtr;
	m_ImageWnd.SetProjectPtr(ProjectPtr);
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true);
	m_ImageWnd.ShowFittedZoom();
	
	if ( TASK_STATE_NONE==TaskStateMode || TASK_STATE_IDLE==TaskStateMode)
	{	m_ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);	}
	else
	{	m_ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	}
	m_ImageWnd.RedrawWnd(FALSE);

	BuildDefectListWnd(ProjectPtr);
	BuildProjectInfoListWnd(ProjectPtr, m_ProjectInfoListCtrl);
	ExecInspection_Finish();
	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_UPDATE_DOCUMENT_TITLE, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
BOOL COnlineFormView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CFormView::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT COnlineFormView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			break;
		case WPARAM_PROJECT_SWITCH_LANE:
			SwitchProjectMap();
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
			ExecSelchangeChartTab();	
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
			m_ImageWnd.UpdateProjectMap();
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
			m_ImageWnd.UpdateProjectTestMap();
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			switch ( lParam )
			{
			case LPARAM_DRAW_PROJECT_MODE_NORMAL:
				m_ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
				RedrawWnd(FALSE);
				break;
			case LPARAM_DRAW_PROJECT_MODE_INSPECTING:
				m_ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
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
		m_ImageWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		break;	
	}
	return CFormView::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	COnlineFormView::RedrawWnd(FALSE);	
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::RedrawWnd(BOOL bRedrawBK)
{
	m_ImageWnd.RedrawWnd(bRedrawBK);
	DrawResultWnd();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::DrawResultWnd()
{
	if ( m_ResultIcon.GetSafeHwnd() == NULL ) { return; }
	RECT Rect={0};
	CClientDC dc(&m_ResultIcon);	
	HDC hDC = dc.GetSafeHdc();

	m_ResultIcon.GetClientRect(&Rect);	
	BITMAPINFO *pInfo = m_DibResult.GetDIBInfo();
	if ( NULL == pInfo ) 
	{ 
		COLORREF Color = ::GetSysColor(COLOR_3DFACE);
		HBRUSH hBrush = ::CreateSolidBrush(Color);
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
		return ; 
	}	
	int OldMode = ::SetStretchBltMode(hDC, HALFTONE);
	m_DibResult.DrawPartion(hDC, 0, 0, pInfo->bmiHeader.biWidth, pInfo->bmiHeader.biHeight, 0, 0, Rect.right, Rect.bottom);
	::SetStretchBltMode(hDC, OldMode);
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::ExecMoveToStage()
{
	if ( this->GetLockUIWnd() == true ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	COnlineFormView::RedrawWnd(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::ExecUpdateProject()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	m_TestResultBarcode = ProjectPtr->GetProjectBarcode();
	BuildResultListWnd(ProjectPtr, m_ResultListCtrl);
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::ExecInspection_Finish()
{
	m_ImageWnd.ResetUpdateTestMapTickCount();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CString str;	
	TTestResult ResultLatest    = ProjectPtr->GetProjectResultLatest();	
	m_TestResultBarcode = ProjectPtr->GetProjectBarcode();
	str = AOIDataDefine.GetTestResultImageName(ResultLatest.sResultID);
	m_DibResult.Load(str);
	BuildResultListWnd(ProjectPtr, m_ResultListCtrl);
	BuildDefectListWnd(ProjectPtr);
	BuildChartWnd(ProjectPtr);
	BuildChartWnd_CPK(ProjectPtr);
	DrawResultWnd();

	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode();	
	TASK_STATE_MODE TaskStateMode = AOIDataCollect.GetOnlineTaskState();
	if ( TASK_TUNING_PROJECT == TaskMode || TASK_TUNING_OFFLINE == TaskMode )
	{	LockUIWnd(false);	}	
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::ExecOnlineInspection_Finish()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	AOIDataCollect.ExecOnlineInspectionFinish();
	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeDefectModeCombo() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{	BuildDefectListWnd(ProjectPtr); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeChartTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ExecSelchangeChartTab();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::ExecSelchangeChartTab()
{
	if ( m_ChartTab.GetSafeHwnd() == NULL ) 
	{	return true; }

	CWnd *pWnd=NULL;	
	BOOL  ShowWnd=TRUE;
	BOOL  Show_InfoList=FALSE;
	BOOL  Show_BarChart=FALSE;
	BOOL  Show_PieChart=FALSE;
	BOOL  Show_XYChart=FALSE;
	BOOL  Show_DefectPieChart=FALSE;	
	BOOL  ShowDefectFrom=FALSE;
	const int TabIndex = this->m_ChartTab.GetCurSel();
	switch ( TabIndex )
	{
	case TAB_INDEX_INFO://Info List
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
	JetAPI::ShowCtrlWnd(this, ONLINE_DEFECT_FROM_COMBO, ShowWnd);

	//Info List
	ShowWnd = Show_InfoList;
	JetAPI::ShowCtrlWnd(this, ONLINE_INFO_LIST_WND, ShowWnd);

	//Yield Bar Chart	
	ShowWnd = Show_BarChart;
	JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_SCOPE_COMBO, ShowWnd);
	JetAPI::ShowCtrlWnd(this, ONLINE_YIELDING_LIST_WND, ShowWnd);
	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_YIELDING, ShowWnd);
	
	//Top-10 Pie Chart
	ShowWnd = Show_PieChart;
	JetAPI::ShowCtrlWnd(this, ONLINE_TOP10_SCOPE_COMBO, ShowWnd);
	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_TOP10, ShowWnd);

	//XY Chart
	ShowWnd = Show_XYChart;
	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_XY, ShowWnd);	

	//Defect Pie Chart
	ShowWnd = Show_DefectPieChart;
	JetAPI::ShowCtrlWnd(this, ONLINE_CHART_WND_DEFECT, ShowWnd);	
	
	CAOIProject *ProjectPtr = GetActiveProject();	
	BuildChartWnd(ProjectPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildDefectListWnd(CAOIProject *ProjectPtr)
{
	bool IsOK = true;
	if ( NULL == ProjectPtr ) { return true; }
	const int Param = JetAPI::GetComboxCurSelData(m_DefectModeCombox);
	ProjectPtr->LockProject();
	if ( DEFECT_MODE_BY_STATISTIC == Param )
	{	IsOK = BuildDefectListWndKernel_Statistic(ProjectPtr);	}
	else
	{	IsOK = BuildDefectListWndKernel_Current(ProjectPtr);	}	
	ProjectPtr->UnlockProject();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildDefectListWndKernel_Current(CAOIProject *ProjectPtr)
{	
	size_t         i=0;
	CString        str;
	CString        strComponent;
	CString        strPartNumber;
	int            nItem=0;
	int            nSubItem=0;
	unsigned int   PanelIndex = 0;
	unsigned int   BoardIndex = 0;	
	RESULT_ID      ResultID=RESULT_ID_NONE;
	CThisListCtrl_15 &ListCtrl = m_DefectListCtrl;

	SetDefectCountEdit(0, 0);
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount();	
	const size_t ComponentNotAgentCount = ProjectPtr->GetProjectComponentNotAgentCount();	

	nItem = 0;	
	//ListCtrl.SetTextBkColor(0xFFD0FF);
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<DefectComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectDefectComponentPtr(i, false);
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
	SetDefectCountEdit(nItem, ComponentNotAgentCount);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildDefectListWndKernel_Statistic(CAOIProject *ProjectPtr)
{
	size_t         i=0;
	CString        str;
	CString        strComponent;
	CString        strPartNumber;
	int            nItem=0;
	int            nSubItem=0;
	unsigned int   PanelIndex = 0;
	unsigned int   BoardIndex = 0;	
	RESULT_ID      ResultID=RESULT_ID_NONE;
	DEFECT_FROM_MODE    DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));
	CThisListCtrl_15 &ListCtrl = m_DefectListCtrl;

	SetDefectCountEdit(0, 0);
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
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS();	}
		else
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI();	}
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
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountARS();	}
		else
		{	DefectCount = ComponentPtr->GetComponentTotalNGCountAOI();	}

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
	SetDefectCountEdit(nItem, ComponentNotAgentCount);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildDefectListWndHeader(CThisListCtrl_15 &ListCtrl)
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
bool COnlineFormView::SetDefectCountEdit(int nDefects, size_t nTotal)
{
	CString str;
	CString Text;
	str = _T("Defects");
	str = LoadMultiLanguageString(str, str);
	Text.Format(_T("%s: %d/%d"), str, nDefects, nTotal);
	CWnd::SetDlgItemText(ONLINE_DEFECT_COUNT_EDIT_LA, Text);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildProjectInfoListWnd(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL  )
	{	return false; }

	int     nItem=0;
	int     nSubItem=0;
	CString str;
	CString strInfo;
	CString strtTitle;

	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	if ( NULL == ProjectPtr ) { return true; }
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
bool COnlineFormView::BuildProjectInfoListWndHeader(CThisListCtrl_15 &ListCtrl)
{
	if ( ListCtrl.GetSafeHwnd() == NULL  )
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
bool COnlineFormView::BuildResultListWnd(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl)
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

	TTestResult ResultLatest    = ProjectPtr->GetProjectResultLatest();
	TTestResult ResultStatistic = ProjectPtr->GetProjectResultStatistic();

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
	str = m_TestResultBarcode;
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem ++;	
		
	ListCtrl.SetRedraw(TRUE);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildResultListWndHeader(CThisListCtrl_15 &ListCtrl)
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
bool COnlineFormView::InitChartTab(CTabCtrl &TabCtrl)
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

	//CTabCtrl	&TabCtrl = m_ChartTab;
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
bool COnlineFormView::InitImageWnd(CImageWnd &ImageWnd)
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
bool COnlineFormView::ClearChartWnd(CThisChartCtrl &ChartWnd)
{
	if ( ChartWnd.GetSafeHwnd() == NULL ) { return true; }
	ChartWnd.Clear();	
	ChartWnd.SetInfoStr();
	ChartWnd.SetTitle(_T(""));
	ChartWnd.BuildChart();
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::InitChartWnd()
{
	DEFECT_FROM_MODE DefectFromMode = AOIDataCollect.GetOnlineViewDefectFromeMode();
	AOIDataDefine.BuildDefectFromCombox(m_DefectFromCombox);
	AOIDataDefine.BuildYieldingScopeCombox(m_YieldScopeCombox);
	AOIDataDefine.BuildTop10ScopeCombox(m_Top10ScopeCombox);
	AOIDataDefine.BuildCpkFromCombox(m_CpkTypeCombox);
	JetAPI::SetComboxCurSel(m_DefectFromCombox, DefectFromMode);
	JetAPI::SetComboxCurSel(m_YieldScopeCombox, YIELDING_SCOPE_TEST);
	JetAPI::SetComboxCurSel(m_Top10ScopeCombox, TOP10_SCOPE_COMPONENT);
	JetAPI::SetComboxCurSel(m_CpkTypeCombox, CPK_FROM_OFFSET_X);

	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT

	nCol = 0;
	width = 96;
	width2 = 256;
	width = 96;
	width2 = 192;
	CThisListCtrl_15 &ListCtrl_Info = m_InfoListWnd;

	ListCtrl_Info.GetClientRect(&Rect);
	//width = (Rect.right-Rect.left-2)/2;	
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
	CThisListCtrl_15 &ListCtrl_Yield = m_YieldingListWnd;

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

	InitChartWnd_Yielding(m_ChartWnd_Yielding);//初始化圖表視窗-良率
	InitChartWnd_Top10(m_ChartWnd_Top10);//初始化圖表視窗-Top-10
	InitChartWnd_XYChart(m_ChartWnd_XYChart);//初始化圖表視窗-座標圖	
	InitChartWnd_DefectStatistic(m_ChartWnd_Defect);//初始化圖表視窗-瑕疵統計
	InitChartWnd_CPK(m_ChartWnd_CPK01);
	InitChartWnd_CPK(m_ChartWnd_CPK02);
	InitChartWnd_CPK(m_ChartWnd_CPK03);
	InitChartWnd_CPK(m_ChartWnd_CPK04);
	InitChartWnd_CPK(m_ChartWnd_CPK05);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::InitChartWnd_Yielding(CThisChartCtrl &ChartWnd)//初始化圖表視窗-良率
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
bool COnlineFormView::InitChartWnd_XYChart(CThisChartCtrl &ChartWnd)//初始化圖表視窗-座標圖
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
bool COnlineFormView::InitChartWnd_Top10(CThisChartCtrl &ChartWnd)//初始化圖表視窗-Top-10
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
bool COnlineFormView::InitChartWnd_DefectStatistic(CThisChartCtrl &ChartWnd)//初始化圖表視窗-瑕疵統計
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
bool COnlineFormView::InitChartWnd_CPK(CThisChartCtrl &ChartWnd)//初始化圖表視窗-CPK
{
	if ( ChartWnd.GetSafeHwnd() == NULL ) { return true; }
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

	font.sFontColor = 0xE8A200;//0x0000ff;
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
	ChartWnd.SetChartRightSpace(4);

	ChartWnd.SetSeriesLeftSpace(10);//50
	ChartWnd.SetSeriesRightSpace(10);//50
	ChartWnd.SetSeriesTopSpace(0);
	ChartWnd.SetSeriesBottomSpace(0);
	ChartWnd.SetIsTranspose(false);	

	ChartWnd.SetIsIntYValue(true);
	ChartWnd.SetYAxisMin(0);
	ChartWnd.SetYAxisMax(100);
	ChartWnd.SetYAxisFix(true);

	ChartWnd.SetInfoLocationMode(JET_CHART_INFO_LOCATION_MODE_LEFT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildChartWnd(CAOIProject *ProjectPtr)//建立圖表視窗
{	
	if ( NULL == ProjectPtr )
	{
		ClearChartWnd(m_ChartWnd_Defect);	
		ClearChartWnd(m_ChartWnd_XYChart);	
		ClearChartWnd(m_ChartWnd_Top10);	
		ClearChartWnd(m_ChartWnd_Yielding);			
		return true;
	}
	BuildChartWnd_Info(ProjectPtr, m_InfoListWnd);
	BuildChartWnd_Yielding(ProjectPtr, m_ChartWnd_Yielding, m_YieldingListWnd);
	BuildChartWnd_Top10(ProjectPtr, m_ChartWnd_Top10);//建立圖表視窗-Top-10
	BuildChartWnd_XYChart(ProjectPtr, m_ChartWnd_XYChart);//建立圖表視窗-座標圖	
	BuildChartWnd_DefectStatistic(ProjectPtr, m_ChartWnd_Defect);//建立圖表視窗-瑕疵統計
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildChartWnd_Info(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl)//建立圖表視窗-訊息
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
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t ComponentBypassCount = ProjectPtr->GetProjectComponentBypassCount();
	TTestResult AOITestResult = ProjectPtr->GetProjectResultStatistic();
	TTestResult ARSTestResult = ProjectPtr->GetProjectResultStatistic_ARS();
	DEFECT_FROM_MODE  DefectFrom=(DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));

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
	{	Yielding = TestResult.sBoard.OK*100.0/TestResult.sBoard.Total;		}
	else
	{	Yielding = 0.0;	}
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
	//str.Format(_T("%.6f (%u/%u)"), Yielding, TestResult.sBoard.OK, TestResult.sBoard.Total);	
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nItem ++;
	}

	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildChartWnd_Yielding(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CThisListCtrl_15 &ListCtrl)//建立圖表視窗-良率
{
	ChartWnd.Clear();
	ListCtrl.DeleteAllItems();
	ListCtrl.SetTextBkColor(0xD0FFFF);
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }	
	if ( NULL == ProjectPtr ) {	return true;	}

	CString str;	
	double nTotal = 0;
	double nTotalBad = 0;	
	TTestResult ResultStatistic_AOI = ProjectPtr->GetProjectResultStatistic();
	TTestResult ResultStatistic_ARS = ProjectPtr->GetProjectResultStatistic_ARS();
	DEFECT_FROM_MODE  DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));
	YIELDING_SCOPE YieldingScope = (YIELDING_SCOPE)(JetAPI::GetComboxCurSelData(m_YieldScopeCombox));	
	TTestResult  AOITestResult = ResultStatistic_AOI;
	TTestResult  ARSTestResult = ResultStatistic_ARS;

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
bool COnlineFormView::BuildChartWnd_XYChart(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd)//建立圖表視窗-座標圖
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
		
		OffsetX = ComponentPtr->GetComponentResultOffsetX();
		OffsetY = ComponentPtr->GetComponentResultOffsetY();				
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
bool COnlineFormView::BuildChartWnd_Top10(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd)//建立圖表視窗-Top-10
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
	DEFECT_FROM_MODE  DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));
	TOP10_SCOPE  Top10Scope = (TOP10_SCOPE)(JetAPI::GetComboxCurSelData(m_Top10ScopeCombox));	

	switch ( Top10Scope )
	{
	case TOP10_SCOPE_COMPONENT:
		if ( ProjectPtr->CalcProjectTop10ListComponent(DefectFrom, Top10List) == false )
		{	return false; }		
		break;
	case TOP10_SCOPE_MODEL:		
		if ( ProjectPtr->CalcProjectTop10ListModel(DefectFrom, Top10List) == false )
		{	return false; }		
		break;
	case TOP10_SCOPE_PART_NUMBER:
		if ( ProjectPtr->CalcProjectTop10ListPartNumber(DefectFrom, Top10List) == false )
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
bool COnlineFormView::BuildChartWnd_DefectStatistic(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd)//建立圖表視窗-瑕疵統計
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
	DEFECT_FROM_MODE  DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));

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
		{	ComponentPtr->GetComponentTotalEachDefectCountARS(EachDefectCount);	}
		else if ( DEFECT_FROM_AOI == DefectFrom )
		{	ComponentPtr->GetComponentTotalEachDefectCountAOI(EachDefectCount);	}
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
bool COnlineFormView::BuildChartWnd_CPK(CAOIProject *ProjectPtr)//建立圖表視窗-常態分布	
{
	if ( NULL == ProjectPtr )
	{
		ClearChartWnd(m_ChartWnd_CPK01);	
		ClearChartWnd(m_ChartWnd_CPK02);	
		ClearChartWnd(m_ChartWnd_CPK03);	
		ClearChartWnd(m_ChartWnd_CPK04);	
		ClearChartWnd(m_ChartWnd_CPK05);
		return true; 
	}
	BuildChartWnd_CPK(ProjectPtr, 0, m_ChartWnd_CPK01);
	BuildChartWnd_CPK(ProjectPtr, 1, m_ChartWnd_CPK02);
	BuildChartWnd_CPK(ProjectPtr, 2, m_ChartWnd_CPK03);
	BuildChartWnd_CPK(ProjectPtr, 3, m_ChartWnd_CPK04);
	BuildChartWnd_CPK(ProjectPtr, 4, m_ChartWnd_CPK05);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::BuildChartWnd_CPK(CAOIProject *ProjectPtr, int nChart, CThisChartCtrl &ChartWnd)//建立圖表視窗-常態分布	
{	//return true;
	if ( ChartWnd.GetSafeHwnd() == NULL ) { return true; }
	ChartWnd.Clear();	
	ChartWnd.SetInfoStr(NULL);
	if ( ChartWnd.IsWindowVisible() == FALSE )	{ return true; }
	ChartWnd.Set3DThicness(20);		
	ChartWnd.BuildChart();
	if ( NULL == ProjectPtr ) {	return true;	}

	size_t i=0;	
	size_t index=0;	
	double Value=0;	
	const LANE_ID LaneID = GetActiveLaneID();		
	const size_t CpkDataCont = ProjectPtr->GetProjectResultCpkList_Lane(LaneID).size();
	if ( nChart >= CpkDataCont ) { return false; }
	const TCpkItem &CpkItemRef = ProjectPtr->GetProjectResultCpkList_Lane(LaneID)[nChart];
	
	CString  strUnit;
	TCpkTemp CpkTemp;
	const int UseValueMode  = JetAPI::GetComboxCurSelData(m_CpkTypeCombox);	
	switch ( UseValueMode )
	{
	default:
	case CPK_FROM_OFFSET_X:	
		strUnit = _T("um");
		CpkTemp = CpkItemRef.OffsetX;	
		break;
	case CPK_FROM_OFFSET_Y:	
		strUnit = _T("um");
		CpkTemp = CpkItemRef.OffsetY;
		break;				
	case CPK_FROM_SKEW_ANGLE:
		strUnit = _T("");
		CpkTemp = CpkItemRef.SkewAngle;
		break;
	}
	const std::vector<float> &BarList = CpkTemp.BinList;
	const int nCount = (int)(BarList.size());		
	if ( 0 == nCount )	{ return true; }
	
	const double Ave = CpkTemp.Ave;
	const double Min = CpkTemp.Min;
	const double Max = CpkTemp.Max;
	const double Cpk = CpkTemp.Cpk;	
	const double Sigma = CpkTemp.Sigma;
	const double Bin = CpkTemp.Bin;
	const double LSL = CpkTemp.LSL;
	const double USL = CpkTemp.USL;		

	const int nSigma = 3;	
	const double Sigma_3 = Sigma*3;
	const double TotalSigma=(nSigma*Sigma); 	
	const double LCL = Ave-(TotalSigma);
	const double UCL = Ave+(TotalSigma);		
	const double LimitL= MIN(Ave-(1.5*TotalSigma), LSL);
	const double LimitU= MAX(Ave+(1.5*TotalSigma), USL);	
	
	CString str;
	CString XLabel = _T("");
	COLORREF clrCL = 0x0000AF;
	COLORREF clrSL = 0x00AFAF;
	const bool bShowLine_SL = false;
	const int styleCl = SERIES_TYPE_LINE;
	CJetSeries *pSeries1 = new CJetSeries();
	CJetSeries *pSeries2 = new CJetSeries();	
	CJetSeries *pSeriesCL = new CJetSeries();	
	CJetSeries *pSeriesLCL = new CJetSeries();	
	CJetSeries *pSeriesUCL = new CJetSeries();	
	CJetSeries *pSeriesLSL = new CJetSeries();	
	CJetSeries *pSeriesUSL = new CJetSeries();		
	if ( pSeries1 == NULL || pSeries2==NULL || pSeriesLCL==NULL || pSeriesUCL==NULL || pSeriesCL==NULL || pSeriesLSL==NULL || pSeriesUSL==NULL )
	{ 
		delete pSeries1; pSeries1=NULL;
		delete pSeries2; pSeries2=NULL;		
		delete pSeriesCL; pSeriesCL=NULL;		
		delete pSeriesLCL; pSeriesLCL=NULL;
		delete pSeriesUCL; pSeriesUCL=NULL;
		delete pSeriesLSL; pSeriesLSL=NULL;
		delete pSeriesUSL; pSeriesUSL=NULL;
		return false;	
	}

	pSeries1->SetSeriesType(SERIES_TYPE_BAR);
	pSeries1->SetIsVisible(true);
	pSeries1->SetLineWidth(1);
	pSeries1->SetLineColor(0x00AF00);	
	pSeries1->SetIsShowMarkValue(false);

	pSeries2->SetSeriesType(SERIES_TYPE_LINE);
	pSeries2->SetIsVisible(true);
	pSeries2->SetLineWidth(1);
	pSeries2->SetLineColor(0xAF0000);	
	pSeries2->SetIsShowMarkValue(false);

	pSeriesCL->SetSeriesType(styleCl);
	pSeriesCL->SetIsVisible(true);
	pSeriesCL->SetLineWidth(1);
	pSeriesCL->SetLineColor(clrCL);	
	pSeriesCL->SetIsShowMarkValue(false);

	pSeriesLCL->SetSeriesType(styleCl);
	pSeriesLCL->SetIsVisible(true);
	pSeriesLCL->SetLineWidth(1);
	pSeriesLCL->SetLineColor(clrCL);	
	pSeriesLCL->SetIsShowMarkValue(false);

	pSeriesUCL->SetSeriesType(styleCl);
	pSeriesUCL->SetIsVisible(true);
	pSeriesUCL->SetLineWidth(1);
	pSeriesUCL->SetLineColor(clrCL);	
	pSeriesUCL->SetIsShowMarkValue(false);

	if ( true == bShowLine_SL )
	{
		pSeriesLSL->SetSeriesType(styleCl);
		pSeriesLSL->SetIsVisible(true);
		pSeriesLSL->SetLineWidth(2);
		pSeriesLSL->SetLineColor(clrSL);	
		pSeriesLSL->SetIsShowMarkValue(false);

		pSeriesUSL->SetSeriesType(styleCl);
		pSeriesUSL->SetIsVisible(true);
		pSeriesUSL->SetLineWidth(2);
		pSeriesUSL->SetLineColor(clrSL);	
		pSeriesUSL->SetIsShowMarkValue(false);
	}

	index = 0;
	for ( i=0; i<BarList.size(); i++ )
	{
		int nX = (int)(i);
		nX *= Bin;
		nX += LimitL;
		str.Format(_T("%d"), nX);
		if ( 0 != (i%2) )
		{	str = _T(""); }
		pSeries1->AddXYValue(true, i, nX, BarList[i], str, XLabel);
		index ++;
	}

	str = _T("");	
	const double A=1.0/(Sigma*sqrt(2*PI));
	const double C=2*Sigma*Sigma;
	const double MaxV=*(std::max_element(BarList.begin(), BarList.end()));	
		
	double MaxTmp=0;
	std::vector<double> NormListX, NormListY;
	NormListX.resize(BarList.size(), 0);
	NormListY.resize(BarList.size(), 0);
	for ( i=0; i<BarList.size(); i++ )
	{	
		double fX = (double)(i);
		fX *= Bin;
		fX += LimitL;
		double B=-1*(fX-Ave)*(fX-Ave);
		double fY=A*std::exp(B/C);
		NormListX[i] = fX; 
		NormListY[i] = fY;
		if ( MaxTmp < fY )
		{	MaxTmp = fY; }
	}
		
	double NormalScale = 0.0;
	if ( MaxTmp > 0.0 )
	{	NormalScale = MaxV/MaxTmp;	}

	for ( i=0; i<NormListY.size(); i++ )
	{	
		double fX = NormListX[i];
		double fY = NormListY[i]*NormalScale;
		pSeries2->AddXYValue(true, i, (int)(fX), fY, str, XLabel);			
	}		
	
	Value = Ave;
	str = _T("CL");	
	index = JetAPI::Floor((Value-LimitL)/Bin);
	pSeriesCL->AddXYValue(true, index, Value, 0, str, XLabel);
	pSeriesCL->AddXYValue(true, index, Value, 100.0f, str, XLabel);	

	Value = LCL;
	str = _T("LCL");	
	index = JetAPI::Floor((Value-LimitL)/Bin);
	pSeriesLCL->AddXYValue(true, index, Value, 0, str, XLabel);
	pSeriesLCL->AddXYValue(true, index, Value, 100.0f, str, XLabel);

	Value = UCL;
	str = _T("UCL");	
	index = JetAPI::Floor((Value-LimitL)/Bin);
	pSeriesUCL->AddXYValue(true, index, Value, 0, str, XLabel);
	pSeriesUCL->AddXYValue(true, index, Value, 100.0f, str, XLabel);

	if ( true == bShowLine_SL )
	{
		Value = LSL;	
		str = _T("LSL");	
		index = JetAPI::Floor((Value-LimitL)/Bin);
		pSeriesLSL->AddXYValue(true, index, Value, 0, str, XLabel);
		pSeriesLSL->AddXYValue(true, index, Value, 100.0f, str, XLabel);	

		Value = USL;	
		str = _T("USL");	
		index = JetAPI::Floor((Value-LimitL)/Bin);
		pSeriesUSL->AddXYValue(true, index, Value, 0, str, XLabel);
		pSeriesUSL->AddXYValue(true, index, Value, 100.0f, str, XLabel);	
	}

	CString strResult;
	COLORREF clrTitle=0x00;
	CString strTitle, strInfo1, strInfo2, strInfo3, strInfo4;		
	m_CpkTypeCombox.GetWindowText(strTitle);
	switch ( CpkItemRef.ResultID )
	{
	case TEST_RESULT_OK:	strResult = _T("OK");	clrTitle=0x008000;	break;
	case TEST_RESULT_NG:
	case TEST_RESULT_FD:	strResult = _T("NG");	clrTitle=0x000080; 	break;	      
	default:
	case TEST_RESULT_NONE:	strResult = _T("UnTest");clrTitle=0x808080; break;
	}	
	strInfo1.Format(_T("Cpk:%.2f"), Cpk);	
	strInfo2.Format(_T("Min:%.2f,  Max=%.2f"), Min, Max);
	strInfo3.Format(_T("Ave:%.2f%s,  Sigma=%.2f"), Ave, strUnit, Sigma);	
	strInfo4.Format(_T("LCL=%.2f,  UCL=%.2f"), LCL, UCL);
	//strTitle = CpkItemRef.DateTime.Format(_T("%Y%m%d%H%M%S"));
	strTitle.Format(_T("%s  %s"), CpkItemRef.DateTime.Format(_T("%H:%M:%S")), strResult);	
	
	ChartWnd.SetTitle(strTitle);
	ChartWnd.GetTitleFont().sFontColor = clrTitle;

	ChartWnd.SetChartTopSpace(116);
	ChartWnd.SetInfoStr(strInfo1, strInfo2, strInfo3, strInfo4);
	ChartWnd.AddSeries(pSeries1);
	ChartWnd.AddSeries(pSeries2);
	ChartWnd.AddSeries(pSeriesCL);
	ChartWnd.AddSeries(pSeriesLCL);
	ChartWnd.AddSeries(pSeriesUCL);
	if ( true == bShowLine_SL )
	{
		ChartWnd.AddSeries(pSeriesLSL);
		ChartWnd.AddSeries(pSeriesUSL);		
	}
	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();
	delete pSeries1; pSeries1=NULL;	
	delete pSeries2; pSeries2=NULL;	
	delete pSeriesCL; pSeriesCL=NULL;
	delete pSeriesLCL; pSeriesLCL=NULL;		
	delete pSeriesUCL; pSeriesUCL=NULL;
	delete pSeriesLSL; pSeriesLSL=NULL;
	delete pSeriesUSL; pSeriesUSL=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeDefectFromCombo() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	DEFECT_FROM_MODE    DefectFrom = (DEFECT_FROM_MODE)(JetAPI::GetComboxCurSelData(m_DefectFromCombox));
	AOIDataCollect.SetOnlineViewDefectFromeMode(DefectFrom);	
	BuildChartWnd(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeTop10ScopeCombo() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	BuildChartWnd(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnClearStatisticBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	str = _T("Do you want to clear the statistic records?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	ProjectPtr->ResetProjectStatisticRecords();	
	ExecInspection_Finish();
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeYieldingScopeCombo() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	BuildChartWnd(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeCpkTypeCombo()
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();	
	BuildChartWnd_CPK(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::ResetMachineStates()//復歸機台狀態
{
	int val = -1;
	m_LastStationSend = val;
	m_LastStationRecieve = val;
	m_NextStationSend = val;
	m_NextStationSendOK = val;
	m_NextStationSendNG = val;
	m_NextStationRecieve = val;
	m_LaneSensorPCBIn = val;
	m_LaneSensorPCBSlow = val;
	m_LaneSensorPCBStop = val;
	m_LaneSensorPCBStop2 = val;
	m_LaneSensorPCBOut = val;
	m_LastStationSend_LB = val;
	m_LastStationRecieve_LB = val;
	m_NextStationSend_LB = val;
	m_NextStationSendOK_LB = val;
	m_NextStationSendNG_LB = val;
	m_NextStationRecieve_LB = val;
	m_LaneSensorPCBIn_LB = val;
	m_LaneSensorPCBSlow_LB = val;
	m_LaneSensorPCBStop_LB = val;
	m_LaneSensorPCBStop_LB2 = val;
	m_LaneSensorPCBOut_LB = val;
	m_MachineFrontCap = val;
	m_MachineRearCap = val;
	m_MachineAirLost = val;
	m_MachineEMSOn = val;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::UpdateMachineStates(bool bReadPLC)//更新機台狀態
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
	CWnd::SetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT, strVal2);	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBIn(LaneID);
	if ( nValue != m_LaneSensorPCBIn )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBInIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBInIcon.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBIn = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBSlow(LaneID);
	if ( nValue != m_LaneSensorPCBSlow )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorSlowDownIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorSlowDownIcon.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBSlow = nValue;			

	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
	if ( nValue != m_LaneSensorPCBStop )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop = nValue;	

	nValue = PlcCtrlPtr->GetConveryerSensorPCBOut(LaneID);
	if ( nValue != m_LaneSensorPCBOut )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBOutIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBOutIcon.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBOut = nValue;
	
	nValue = PlcCtrlPtr->GetConveryerSensorPCBStop2(LaneID);
	if ( nValue != m_LaneSensorPCBStop2 )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon2.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon2.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop2 = nValue;	

	nValue = PlcCtrlPtr->GetLastStationSignal(LaneID);
	if ( nValue != m_LastStationRecieve )
	{
		if ( TurnOn == nValue )
		{	m_LastStationRecieveIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_LastStationRecieveIcon.SetBitmap(m_LEDGray); }
	}
	m_LastStationRecieve = nValue;

	nValue = PlcCtrlPtr->GetSendToLastStation(LaneID);
	if ( nValue != m_LastStationSend )
	{
		if ( TurnOn == nValue )
		{	m_LastStationSendIcon.SetBitmap(m_LEDYellow); }
		else
		{	m_LastStationSendIcon.SetBitmap(m_LEDGray); }
	}
	m_LastStationSend = nValue;

	nValue = PlcCtrlPtr->GetNextStationSignal(LaneID);
	if ( nValue != m_NextStationRecieve )
	{
		if ( TurnOn == nValue )
		{	m_NextStationRecieveIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationRecieveIcon.SetBitmap(m_LEDGray); }
	}
	m_NextStationRecieve = nValue;	

	nValue = PlcCtrlPtr->GetSendToNextStation(LaneID);
	if ( nValue != m_NextStationSend )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendIcon.SetBitmap(m_LEDYellow); }
		else
		{	m_NextStationSendIcon.SetBitmap(m_LEDGray); }
	}
	m_NextStationSend = nValue;	
	
	nValue = PlcCtrlPtr->GetSendToNextStationOK(LaneID);
	if ( nValue != m_NextStationSendOK )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendOKIcon.SetBitmap(m_LEDGreen); }
		else
		{	m_NextStationSendOKIcon.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendOK = nValue;

	nValue = PlcCtrlPtr->GetSendToNextStationNG(LaneID);
	if ( nValue != m_NextStationSendNG )
	{
		if ( TurnOn == nValue )
		{	m_NextStationSendNGIcon.SetBitmap(m_LEDRed); }
		else
		{	m_NextStationSendNGIcon.SetBitmap(m_LEDGray); }
	}
	m_NextStationSendNG = nValue;	
	
	//Conveyer - Lane B
	LaneID = LANE_ID_B;
	nValue = PlcCtrlPtr->GetConveryerStatus(LaneID);		
	PlcCtrlPtr->GetConveryerStatusText(nValue, strVal);
	strVal2.Format(_T("B: %s"), strVal);
	CWnd::SetDlgItemText(ONLINE_PLC_LANE_STATUS_EDIT_LB, strVal2);	

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
	if ( nValue != m_LaneSensorPCBStop_LB2 )
	{
		if ( TurnOn == nValue )
		{	m_LaneSensorPCBStopIcon_LB2.SetBitmap(m_LEDGreen); }
		else
		{	m_LaneSensorPCBStopIcon_LB2.SetBitmap(m_LEDGray); }
	}
	m_LaneSensorPCBStop_LB2 = nValue;		
	
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool COnlineFormView::UpdateMultiLaneUI()//更新多軌道介面
{
	bool bShowLaneLB=false;
	MULTI_LANE_MODE MultiLaneMode = AOIDataCollect.CheckMultiLaneMode();
	if ( MULTI_LANE_2 == MultiLaneMode ) 
	{	bShowLaneLB = true; }
	else
	{	bShowLaneLB = false; }
#ifdef OFFLINE_VERSION
	bShowLaneLB = false;
#endif//OFFLINE_VERSION
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_B_LABEL, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_PLC_LANE_STATUS_EDIT_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_OK_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_SEND_NG_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_NEXT_STATION_RECIEVE_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_IN_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_SLOW_DOWN_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_OUT_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LANE_SENSOR_PCB_STOP_ICON_LB2, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_SEND_ICON_LB, bShowLaneLB);
	JetAPI::ShowCtrlWnd(this, ONLINE_LAST_STATION_RECIEVE_ICON_LB, bShowLaneLB);	
	return true;
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnSelchangeProjectIDCombo() 
{
	// TODO: Add your control notification handler code here	
	size_t ProjectIndex = 0;	
	CAOIProject *ProjectPtr = NULL;
	CComboBox  &Combox = m_ProjectIDCombox;
	LANE_ID LaneID = GetActiveLaneID();
	ProjectIndex = JetAPI::GetComboxCurSelData(Combox);
	ProjectPtr = AOIDataCollect.GetProjectPtr(ProjectIndex, true);
	if ( NULL == ProjectPtr ) { return; }

	if ( ProjectPtr != m_ProjectPtr )
	{	ExecSwitchProject(LaneID, ProjectPtr);	}
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnMachineButtonStartBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownStartBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnMachineButtonResetBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownResetBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void COnlineFormView::OnMachineButtonStopBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsOK = true;	
	IsOK = PlcCtrlPtr->PushDownStopBtn();
	if ( false == IsOK )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//