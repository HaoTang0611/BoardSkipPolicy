// SystemOnlinePane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemOnlinePane.h"
//-------------------------------------------------------------------------------------//
#include "InputDateTimeWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL   = 2;//設定的欄位
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemOnlinePane dialog
//-------------------------------------------------------------------------------------//
CSystemOnlinePane::CSystemOnlinePane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemOnlinePane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemOnlinePane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOnlinePane)
	DDX_Control(pDX, SYSONLINE_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSONLINE_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSONLINE_TIME_BTN, m_TimeBtnCtrl);	
	DDX_Control(pDX, SYSONLINE_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSONLINE_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemOnlinePane, CDialog)
	//{{AFX_MSG_MAP(CSystemOnlinePane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSONLINE_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSONLINE_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSONLINE_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_KILLFOCUS(SYSONLINE_PARAM_COMBO, OnKillfocusParamCombo)
	ON_CBN_SELCHANGE(SYSONLINE_PARAM_COMBO, OnSelchangeParamCombo)
	ON_BN_CLICKED(SYSONLINE_PARAM_BTN, OnParamBtn)
	ON_BN_CLICKED(SYSONLINE_TIME_BTN, OnTimeBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemOnlinePane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemOnlinePane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_ParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	WndPtr = CWnd::GetDlgItem(SYSONLINE_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = cx;		
		WndRect.bottom = cy-MarginY;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect, FALSE);
		InfoRect = WndRect;
	}
	else
	{
		InfoRect.left = 0;	InfoRect.right = cx;
		InfoRect.top = cy; InfoRect.bottom = cy;
	}

	if ( m_ParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_ParamListCtrl.MoveWindow(&WndRect, FALSE);
		if ( TRUE == bVisible )
		{	m_ParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
BOOL CSystemOnlinePane::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			if ( m_EditCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByEdit();
				m_EditCtrl.ShowWindow(SW_HIDE);	
				m_EditCtrl.SetWindowText(_T(""));
				return TRUE;				
			}
			if ( m_ComboxCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByCombox();
				m_ComboxCtrl.ShowWindow(SW_HIDE);
				JetAPI::ClearCombox(m_ComboxCtrl);
				return TRUE;				
			}
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();	
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CSystemOnlinePane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_SYSTEM_ONLINE_PANE), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{	
	LPCTSTR Section=_T("IDD_SYSTEM_ONLINE_PANE");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CSystemOnlinePane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_ONLINE_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemOnlinePane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CString CSystemOnlinePane::GetTimeString(__int64 Time) const
{
	return AOIDataDefine.GetOnlineAutoStopBySpecTimeText(Time);	
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::BuildParamList()
{
	CThisListCtrl_60 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	SYSTEM_PARAM_ID ParamID;
	TSystemParameter *Ptr = m_SysParameterPtr;
	const int nSubItem = 1;	
	USER_LEVEL_MODE UserLevelMode = AOIDataCollect.GetCurrentUserLevel();	
	const int ComputerCPUCoreNumber = AOIDataCollect.GetComputerCPUCoreNumber();
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//在線輸入-專案工單號碼	
	ParamUnit = CParamUni();
	str = _T("Online Input Project Work Number");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_INPUT_PROJECT_WORK_NUMBER;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.AddSelItem(ONLINE_INPUT_DISABLE, AOIDataDefine.GetOnlineInputTimingText(ONLINE_INPUT_DISABLE));
	ParamUnit.AddSelItem(ONLINE_INPUT_BEFORE_SIGN_IN, AOIDataDefine.GetOnlineInputTimingText(ONLINE_INPUT_BEFORE_SIGN_IN));
	ParamUnit.SetValue_SEL(Ptr->m_OnlineInputProjectWorkNumber);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

#ifdef ONLINE_AUTO_CALIBRATION_USE	
	//在線自動校正上蓋延遲時間-ms
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Target Cap Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_CAP_DELAY_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationCapDelayTime, 0, 10000);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//在線自動校正模式-XYZ歸零	
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Mode XYZ Home");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_XYZ_HOME;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_OFF);		ParamUnit.AddSelItem(FUNC_EXEC_OFF, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_AUTO);		ParamUnit.AddSelItem(FUNC_EXEC_AUTO, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_ASK);		ParamUnit.AddSelItem(FUNC_EXEC_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineAutoCalibrationMode_XYZHome);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正週期-XYZ歸零-小時	
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Period XYZ Home");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_XYZ_HOME;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationPeriod_XYZHome, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正模式-2D電流
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Mode 2D Current");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_2D_CURRENT;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_OFF);		ParamUnit.AddSelItem(FUNC_EXEC_OFF, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_AUTO);		ParamUnit.AddSelItem(FUNC_EXEC_AUTO, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_ASK);		ParamUnit.AddSelItem(FUNC_EXEC_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineAutoCalibrationMode_2DCurrent);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正週期-2D電流-小時		
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Period 2D Current");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_2D_CURRENT;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationPeriod_2DCurrent, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

#ifndef DISABLE_3D
	//在線自動校正DLP-LED-顏色
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration DLP LED Color");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_DLP_LED_COLOR;
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.AddSelItem(DLP_LED_COLOR_RED, AOIDataDefine.GetColorText_Red());
	ParamUnit.AddSelItem(DLP_LED_COLOR_GREEN, AOIDataDefine.GetColorText_Green());
	ParamUnit.AddSelItem(DLP_LED_COLOR_BLUE, AOIDataDefine.GetColorText_Blue());
	ParamUnit.AddSelItem(DLP_LED_COLOR_WHITE, AOIDataDefine.GetColorText_White());
	ParamUnit.SetValue_SEL(Ptr->m_OnlineAutoCalibrationDlpLedColor);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正週期-3D電流-小時		
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Period 3D Current");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_CURRENT;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationPeriod_3DCurrent, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	//ParamList.push_back(ParamUnit);
	
	//在線自動校正模式-3D相平面
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Mode 3D Zero Plane");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_MODE_3D_ZERO_PLANE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_OFF);		ParamUnit.AddSelItem(FUNC_EXEC_OFF, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_AUTO);		ParamUnit.AddSelItem(FUNC_EXEC_AUTO, strValue);
	strValue = AOIDataDefine.GetFuncExecModeText(FUNC_EXEC_ASK);		ParamUnit.AddSelItem(FUNC_EXEC_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_OnlineAutoCalibrationMode_3DZeroPlane);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正週期-3D相平面-小時		
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Period 3D Zero Plane");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_ZERO_PLANE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationPeriod_3DZeroPlane, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線自動校正週期-3D高度比例-小時
	ParamUnit = CParamUni();
	str = _T("Online Auto Calibration Period 3D Height Factor");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_CALIBRATION_PERIOD_3D_HEIGHT_FACTOR;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoCalibrationPeriod_3DHeightFactor, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	//ParamList.push_back(ParamUnit);
#endif//DISABLE_3D

#endif//ONLINE_AUTO_CALIBRATION_USE

	const bool bReadOnly = true;
	//在線自動停機-閒置時間-分鐘
	ParamUnit = CParamUni();
	str = _T("Online Auto Stop By Idle Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_STOP_BY_IDLE_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_OnlineAutoStopByIdleTime, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
#ifdef ONLINE_AUTO_STOP_USE
	//在線自動停機-特定時間-時時分分秒秒
	ParamUnit = CParamUni();
	str = _T("Online Auto Stop By Spec Time 1");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(GetTimeString(Ptr->m_OnlineAutoStopBySpecTime1));
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_TimeBtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//在線自動停機-特定時間-時時分分秒秒
	ParamUnit = CParamUni();
	str = _T("Online Auto Stop By Spec Time 2");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(GetTimeString(Ptr->m_OnlineAutoStopBySpecTime2));
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_TimeBtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//在線自動停機-特定時間-時時分分秒秒
	ParamUnit = CParamUni();
	str = _T("Online Auto Stop By Spec Time 3");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(GetTimeString(Ptr->m_OnlineAutoStopBySpecTime3));
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);	
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_TimeBtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);
#endif//ONLINE_AUTO_STOP_USE

	//自動切換線上畫面-秒		
	ParamUnit = CParamUni();
	str = _T("Auto Switch To OnlineView Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SWITCH_TO_ONILINEVIEW_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_AutoSwitchToOnlineViewTime, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線顯示專案檢測底圖	
	ParamUnit = CParamUni();
	str = _T("Online Show Project Test Map");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_ONLINE_SHOW_PROJECT_TEST_MAP;
	ParamUnit.SetParamID((UINT)(ParamID));		
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Ptr->m_OnlineShowProjectTestMap);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動切換線上遠端控制時間-毫秒	
	ParamUnit = CParamUni();
	str = _T("Auto Switch To Online Remote Ctrl Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_TIME;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(Ptr->m_AutoSwitchToOnlineRemoteCtrlTime, 0);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//自動切換線上遠端控制模式
	ParamUnit = CParamUni();
	str = _T("Auto Switch To Online Remote Ctrl Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_AUTO_SWITCH_TO_ONILINE_REMOTE_CTRL_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.AddSelItem(MES_EQP_CTRL_STATE_NONE, AOIDataDefine.GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_NONE));
	ParamUnit.AddSelItem(MES_EQP_CTRL_STATE_OFFLINE, AOIDataDefine.GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_OFFLINE));
	ParamUnit.AddSelItem(MES_EQP_CTRL_STATE_LOCAL, AOIDataDefine.GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_LOCAL));
	ParamUnit.AddSelItem(MES_EQP_CTRL_STATE_REMOTE, AOIDataDefine.GetMesEqpCtrlStateText(MES_EQP_CTRL_STATE_REMOTE));
	ParamUnit.SetValue_SEL(Ptr->m_AutoSwitchToOnlineRemoteCtrlMode);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_60 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	
	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_60 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*5;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = 0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;	
	if ( 0!=ResOld && 0==ResNew )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSONLINE_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecItemchangedParamListWnd(CThisListCtrl_60 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();	
	const int nSubItem = ParamPtr->GetSubItemIndex();
	
	SetDescriptionText(ParamPtr);
	if ( NULL!=BtnWndPtr && BtnWndPtr->GetSafeHwnd()!=NULL) 	
	{
		CRect ItemRect;
		if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == TRUE )
		{	
			SIZE BtnSize={0};
			RECT BtnRect={0};
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;			
			BtnRect.left = BtnRect.right-BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);
			BtnWndPtr->ShowWindow(SW_SHOW);			
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	
		m_BtnCtrl.ShowWindow(SW_HIDE);	
		m_TimeBtnCtrl.ShowWindow(SW_HIDE);	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecDblclkParamListWnd(CThisListCtrl_60 &ListCtrl, int nItem, int nSubItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SETTING_COL ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_TimeBtnCtrl.ShowWindow(SW_HIDE);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				m_ComboxCtrl.InsertString(nSelIdx, ItemText);
				m_ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(m_ComboxCtrl, ParamPtr->GetSelParam());
			m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			m_ComboxCtrl.SetFocus();
			m_ComboxCtrl.ShowDropDown();
			m_ComboxCtrl.ShowWindow(SW_SHOW);
			m_ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			m_ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		if ( m_EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			m_EditCtrl.SetWindowText(ItemText);
			m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
			m_EditCtrl.SetFocus();
			m_EditCtrl.SetSel(0,-1);			
			m_EditCtrl.ShowWindow(SW_SHOW);	
			m_EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			m_EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	

	if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_60 *pListCtrl = (CThisListCtrl_60*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
void CSystemOnlinePane::OnTimeBtn() 
{
	// TODO: Add your control notification handler code here
	m_TimeBtnCtrl.ShowWindow(SW_HIDE);
#ifdef ONLINE_AUTO_STOP_USE
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CTime DateTime;	
	CString TextShow;
	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	
	
	switch ( SysParam )
	{
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_1:
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_2:
	case SYSTEM_ONLINE_AUTO_STOP_BY_SPEC_TIME_3:
		if ( ExecInputDateTime(ParamPtr, DateTime) == false ) { return ; }				
		ItemText.Format(_T("%I64d"), DateTime.GetTime());		
		TextShow=GetTimeString(DateTime.GetTime());
		break;
	default:
		if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
		TextShow = ItemText;
		break;
	}	
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(TextShow);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_60 *pListCtrl = (CThisListCtrl_60*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, TextShow);
		pListCtrl->SetFocus();
	}
#endif//ONLINE_AUTO_STOP_USE
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_60 *pListCtrl = (CThisListCtrl_60*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	m_TimeBtnCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_60 *pListCtrl = (CThisListCtrl_60*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemOnlinePane::ExecInputDateTime(CParamUni *ParamPtr, CTime &DateTime)
{
	if ( NULL == ParamPtr ) { return false; }	
	CString ItemText;
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	
	if ( CAOIDataCollect::GetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }

	const bool bNone=true;
	CInputDateTimeWnd InputWnd;
	const __int64 Int64=::_ttoi64(ItemText);	
	//InputWnd.SetDate(CTime(Int64));
	InputWnd.SetTime(CTime(Int64), NULL, bNone);
	//InputWnd.SetDateTime(CTime(Int64), CTime(Int64));
	if ( InputWnd.DoModal() != IDOK )
	{	return false; }

	DateTime = InputWnd.GetResult();
	const int nYear=DateTime.GetYear();
	const int nMonth=DateTime.GetMonth();
	const int nDay=DateTime.GetDay();
	const int nHour=DateTime.GetHour();
	const int nMinute=DateTime.GetMinute();
	const int nSecond=0;//DateTime.GetSecond();
	DateTime=CTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	return true;
}
//-------------------------------------------------------------------------------------//