// PlcCtrlWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PlcCtrlWnd.h"
//-------------------------------------------------------------------------------------//
#include "Plc_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
CPLCCtrlWnd  PlcCtrlWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlWnd dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlWnd::CPLCCtrlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlWnd)
	DDX_Control(pDX, PLCCTRL_PANE_TAB_WND, m_PaneTabWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlWnd, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, PLCCTRL_PANE_TAB_WND, OnSelchangePaneTabWnd)
	ON_BN_CLICKED(PLCCTRL_CONNECT_BTN, OnConnectBtn)
	ON_BN_CLICKED(PLCCTRL_DISCONNECT_BTN, OnDisconnectBtn)
	ON_BN_CLICKED(PLCCTRL_SAFTY_PASS_CHK, OnSaftyPassChk)
	ON_BN_CLICKED(PLCCTRL_SAVE_PARAM_BTN, OnSaveParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LandStatusPane.Create(IDD_PLC_LANE_STATUS_PANE, this);
	m_TowerLightPane.Create(IDD_PLC_TOWER_LIGHT_PANE, this);
	m_LaneAdjustPane.Create(IDD_PLC_LANE_ADJUST_PANE, this);
	m_ParamListPane.Create(IDD_PLC_PARAM_LIST_PANE, this);
	m_NodeListPane.Create(IDD_PLC_NODE_LIST_PANE, this);
	AdjustPaneWndPosition();
	
	int    tcIndex=0;
	TCITEM tcItem;	
	CString str;

	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	
	if ( m_LandStatusPane.GetSafeHwnd() != NULL )
	{
		tcItem.pszText = _T("Lane Status");	
		tcItem.lParam = (LPARAM)(&m_LandStatusPane);
		m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}

	if ( m_TowerLightPane.GetSafeHwnd() != NULL )
	{
		tcItem.pszText = _T("Tower Light");	
		tcItem.lParam = (LPARAM)(&m_TowerLightPane);
		m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}

	if ( m_LaneAdjustPane.GetSafeHwnd() != NULL )
	{
		tcItem.pszText = _T("Lane Adjust");	
		tcItem.lParam = (LPARAM)(&m_LaneAdjustPane);
		m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}

	if ( m_ParamListPane.GetSafeHwnd() != NULL )
	{
		tcItem.pszText = _T("Param List");	
		tcItem.lParam = (LPARAM)(&m_ParamListPane);
		m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}

	if ( m_NodeListPane.GetSafeHwnd() != NULL )
	{
		tcItem.pszText = _T("Node Status");	
		tcItem.lParam = (LPARAM)(&m_NodeListPane);
		m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;	
	}
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	str.Format(_T("%s"), PlcCtrlPtr->GetPLCConnectParam());
	this->SetDlgItemText(PLCCTRL_CONNECT_PARAM_EDIT, str);

	if ( PlcCtrlPtr->GetPLCIsConnected() == true )
	{
		if ( PlcCtrlPtr->GetSaftyBypass() == true )
		{	CWnd::CheckDlgButton(PLCCTRL_SAFTY_PASS_CHK, TRUE); }
		else
		{	CWnd::CheckDlgButton(PLCCTRL_SAFTY_PASS_CHK, FALSE); }
	}

	SwitchMultiLanguage();		
	ExecSelchangePaneTabWnd();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( this->m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	this->AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	UINT ShowMode = 0;
	if ( TRUE == bShow )
	{	ShowMode = SW_SHOW;	}
	else
	{	ShowMode = SW_HIDE;	}
	
	CString str;
	TCITEM tcItem;
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;
	const int TabIndex = this->m_PaneTabWnd.GetCurSel();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if ( TabIndex >= 0 ) 
	{
		this->m_PaneTabWnd.GetItem(TabIndex, &tcItem);
		CWnd *pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == TRUE )
		{	pWnd->ShowWindow(ShowMode);	}		
	}

	if ( FN_DISABLE == SysParam.m_ShowPlcSaftySettingUI )
	{	JetAPI::ShowCtrlWnd(this, PLCCTRL_SAFTY_PASS_CHK, FALSE);	}
	else
	{	JetAPI::ShowCtrlWnd(this, PLCCTRL_SAFTY_PASS_CHK, TRUE);	}

	if ( PlcCtrlPtr->GetPLCIsConnected() == true )
	{
		str = PlcCtrlPtr->GetPLCVersion();	
		this->SetDlgItemText(PLCCTRL_VERSION_EDIT, str);

		if ( PlcCtrlPtr->GetSaftyBypass() == true )
		{	CWnd::CheckDlgButton(PLCCTRL_SAFTY_PASS_CHK, TRUE);	}
		else
		{	CWnd::CheckDlgButton(PLCCTRL_SAFTY_PASS_CHK, FALSE);	}		
	}

	const int LastStationLineMode = PlcCtrlPtr->GetLastStationLineMode();
	switch ( LastStationLineMode )
	{
	case LAST_STATION_LINE_MODE_2:	str=_T("Line 2");	break;
	case LAST_STATION_LINE_MODE_2_4:str=_T("Line 2+4");	break;		
	case LAST_STATION_LINE_MODE_4:	str=_T("Line 4");	break;
	default:
		str = _T("");
		break;
	}
	CWnd::SetDlgItemText(PLCCTRL_LAST_STATION_LINE_MODE_EDIT, str);

	if ( FALSE == bShow )
	{	AOIDataCollect.UserLogout_Check(); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	this->ShowWindow(SW_HIDE);
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_CTRL_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_CTRL_WND;
	WndKey = _T("IDD_PLC_CTRL_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PLCCTRL_CONNECT_PARAM_LABEL;
	WndKey = _T("PLCCTRL_CONNECT_PARAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCCTRL_VERSION_LABEL;
	WndKey = _T("PLCCTRL_VERSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCCTRL_LAST_STATION_LINE_MODE_LABEL;
	WndKey = _T("PLCCTRL_LAST_STATION_LINE_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCCTRL_CONNECT_BTN;
	WndKey = _T("PLCCTRL_CONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCCTRL_DISCONNECT_BTN;
	WndKey = _T("PLCCTRL_DISCONNECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCCTRL_SAFTY_PASS_CHK;
	WndKey = _T("PLCCTRL_SAFTY_PASS_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCCTRL_SAVE_PARAM_BTN;
	WndKey = _T("PLCCTRL_SAVE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	TCITEM tcItem;
	const size_t  BufSize=32;
	TCHAR TxtBuff[BufSize]=_T("");

	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = TxtBuff;
	tcItem.cchTextMax = BufSize;
	const int TabItemCount = this->m_PaneTabWnd.GetItemCount();
	for ( i=0; i<TabItemCount; i++ )
	{
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd == &(m_LandStatusPane) )
		{
			WndID = IDD_PLC_LANE_STATUS_PANE;
			WndKey = _T("IDD_PLC_LANE_STATUS_PANE");
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
		else if ( pWnd == &(m_TowerLightPane) )
		{
			WndID = IDD_PLC_TOWER_LIGHT_PANE;
			WndKey = _T("IDD_PLC_TOWER_LIGHT_PANE");
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
		else if ( pWnd == &(m_LaneAdjustPane) )
		{
			WndID = IDD_PLC_LANE_ADJUST_PANE;
			WndKey = _T("IDD_PLC_LANE_ADJUST_PANE");			
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
		else if ( pWnd == &(m_ParamListPane) )
		{
			WndID = IDD_PLC_PARAM_LIST_PANE;
			WndKey = _T("IDD_PLC_PARAM_LIST_PANE");			
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
		else if ( pWnd == &(m_NodeListPane) )
		{
			WndID = IDD_PLC_NODE_LIST_PANE;
			WndKey = _T("IDD_PLC_NODE_LIST_PANE");
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}	
	}
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void  CPLCCtrlWnd::AdjustPaneWndPosition()
{
	if ( m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	RECT PaneWndRect={0};
	this->m_PaneTabWnd.GetClientRect(&PaneWndRect);
	this->m_PaneTabWnd.ClientToScreen(&PaneWndRect);
	this->ScreenToClient(&PaneWndRect);
	PaneWndRect.top += 24; 
	PaneWndRect.left += 4;
	PaneWndRect.right -= 4;
	PaneWndRect.bottom -= 4;
	if ( m_LandStatusPane.GetSafeHwnd() != NULL )
	{	m_LandStatusPane.MoveWindow(&PaneWndRect);	}

	if ( m_TowerLightPane.GetSafeHwnd() != NULL )
	{	m_TowerLightPane.MoveWindow(&PaneWndRect);	}
	
	if ( m_ParamListPane.GetSafeHwnd() != NULL )
	{	m_ParamListPane.MoveWindow(&PaneWndRect);	}	

	if ( m_LaneAdjustPane.GetSafeHwnd() != NULL )
	{	m_LaneAdjustPane.MoveWindow(&PaneWndRect);	}

	if ( m_NodeListPane.GetSafeHwnd() != NULL )
	{	m_NodeListPane.MoveWindow(&PaneWndRect);	}	

}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ExecSelchangePaneTabWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::ExecSelchangePaneTabWnd()
{
	int    i = 0;
	CWnd  *pWnd = NULL;
	TCITEM tcItem;	
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;
	const int TabCount = m_PaneTabWnd.GetItemCount();
	const int TabIndex = m_PaneTabWnd.GetCurSel();

	//Hide Pane
	for ( i=0; i<TabCount; i++ )
	{
		if ( i == TabIndex ) { continue; }
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { continue; }	
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		if ( pWnd->IsWindowVisible() == FALSE ) { continue; }
		pWnd->ShowWindow(SW_HIDE);
	}

	//Show Pane
	if ( TabIndex >= 0 )
	{
		this->m_PaneTabWnd.GetItem(TabIndex, &tcItem);
		CWnd *pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == TRUE )
		{
			if ( (NULL!=pWnd) && (NULL!=pWnd->GetSafeHwnd()) )
			{	pWnd->ShowWindow(SW_SHOW);	}
		}
	}	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnConnectBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	this->SetDlgItemText(PLCCTRL_VERSION_EDIT, _T(""));

	this->GetDlgItemText(PLCCTRL_CONNECT_PARAM_EDIT, str);
	if ( PlcCtrlPtr->GetPLCIsConnected() == true )
	{	PlcCtrlPtr->PLC_Disconnect(); }	
	if ( PlcCtrlPtr->PLC_ConnectTo(str) == false )
	{
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
	str = PlcCtrlPtr->GetPLCVersion();	
	this->SetDlgItemText(PLCCTRL_VERSION_EDIT, str);	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnDisconnectBtn() 
{
	// TODO: Add your control notification handler code here
	if ( AOIDataCollect.OperateLevel_Supervisor() == false )
	{	return; }

	PlcCtrlPtr->PLC_Disconnect();
	this->SetDlgItemText(PLCCTRL_VERSION_EDIT, _T(""));
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnSaftyPassChk() 
{
	// TODO: Add your control notification handler code here
	bool On = false;
	if ( this->IsDlgButtonChecked(PLCCTRL_SAFTY_PASS_CHK) == TRUE )
	{	On = true; }
	else
	{	On = false; }

	if ( true == On )
	{
		if ( AOIDataCollect.OperateLevelEditFuncSetPlcSaftyPass() == false )
		{	
			CheckDlgButton(PLCCTRL_SAFTY_PASS_CHK, FALSE);
			return; 
		}
	}
	if ( PlcCtrlPtr->WriteBypassSafty(On) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	

	if ( true == On )
	{	
		DWORD DelayTime=100;
		PlcCtrlPtr->PushDownStopBtn();
		if ( DelayTime > 0 ) { ::Sleep(DelayTime); }
		PlcCtrlPtr->PushDownResetBtn();
		if ( DelayTime > 0 ) { ::Sleep(DelayTime); }
		PlcCtrlPtr->PushDownStartBtn();			
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlWnd::OnSaveParamBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	if ( PlcCtrlPtr->SavePLCINIParameter() == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }	
}
//-------------------------------------------------------------------------------------//