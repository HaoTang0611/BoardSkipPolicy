// PlcParamListPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PlcParamListPane.h"
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
// CPLCCtrlPaneParamList dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlPaneParamList::CPLCCtrlPaneParamList(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlPaneParamList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlPaneParamList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlPaneParamList)	
	DDX_Control(pDX, PLCPARAM_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PLCPARAM_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PLCPARAM_PARAM_LIST_WND, m_ParamListCtrl);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlPaneParamList, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlPaneParamList)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, PLCPARAM_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_CLICK, PLCPARAM_PARAM_LIST_WND, OnClickParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PLCPARAM_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(PLCPARAM_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PLCPARAM_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PLCPARAM_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(PLCPARAM_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(PLCPARAM_UPDATE_BTN, OnUpdateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneParamList message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneParamList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	BuildParamListWndHeaer();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	int   ListCtrlRight=cx;
	int   ListCtrlBottom=cy;
	const int GapX=4;
	const int GapY=4;
	CWnd *WndPtr = NULL;
	WndPtr = CWnd::GetDlgItem(PLCPARAM_SAVE_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		SIZE WndSize={0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = cx-GapX;
		WndRect.left = WndRect.right-WndSize.cx;
		WndPtr->MoveWindow(&WndRect);
		ListCtrlRight = WndRect.left-GapX;
	}
	WndPtr = CWnd::GetDlgItem(PLCPARAM_UPDATE_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		SIZE WndSize={0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = cx-GapX;
		WndRect.left = WndRect.right-WndSize.cx;
		WndPtr->MoveWindow(&WndRect);
		ListCtrlRight = WndRect.left-GapX;
	}
	WndPtr = CWnd::GetDlgItem(PLCPARAM_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};		
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = ListCtrlRight-GapX;
		WndRect.bottom = cy-GapY;
		WndPtr->MoveWindow(&WndRect);
		ListCtrlBottom = WndRect.top-GapY;
	}

	if ( m_ParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ParamListCtrl.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = ListCtrlRight-GapX;
		WndRect.bottom = ListCtrlBottom-GapY;
		m_ParamListCtrl.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow ) 
	{	BuildParamListWnd(); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneParamList::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CPLCCtrlPaneParamList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_PARAM_LIST_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_PARAM_LIST_PANE;
	WndKey = _T("IDD_PLC_PARAM_LIST_PANE");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PLCPARAM_SAVE_BTN;
	WndKey = _T("PLCPARAM_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCPARAM_UPDATE_BTN;
	WndKey = _T("PLCPARAM_UPDATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CPLCCtrlPaneParamList::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PLC_PARAM_LIST_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CParamUni*  CPLCCtrlPaneParamList::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::BuildParamList()
{
	SetActParamUni(NULL);
	CThisListCtrl_16 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == PlcCtrlPtr ) { return false; }
	

	int       intValue=0;
	size_t    i=0, j=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	PLC_PARAM_ID ParamID;	
	const TPLCParameter &Param = PlcCtrlPtr->GetPLCParameter();	
	const int nSubItem = 1;	
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//PLC通訊延遲時間	
	ParamUnit = CParamUni();
	str = _T("PLC Communicate Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_COMM_DELAY_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PLCCommDelayTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//進板前確認機台有無板子	
	ParamUnit = CParamUni();
	str = _T("Check PCB Inside Before PCB-In");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_CHECK_PCB_INSIDE_BEFORE_PCB_IN;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_CheckPCBInsideBeforePCBIn);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB進出板逾時-10s-A軌 
	ParamUnit = CParamUni();
	str = _T("PCB In Out Timeout Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_IN_OUT_TIMEOUT_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBInOutTimeout_LA, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB進出板逾時-10s-B軌 
	ParamUnit = CParamUni();
	str = _T("PCB In Out Timeout Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_IN_OUT_TIMEOUT_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBInOutTimeout_LB, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//氣壓不足監控時間-0.5s
	ParamUnit = CParamUni();
	str = _T("Check Air Lost Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_AIR_LOST_CHECK_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_AirLostCheckTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//定位Sensor感應後持續運轉延遲時間-0.1sec
	ParamUnit = CParamUni();
	str = _T("PCB Stop Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_STOP_DELAY_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBStopDelayTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//維修模式時-汽缸夾鬆板愈時-1s
	ParamUnit = CParamUni();
	str = _T("Clamp On Off Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_CLAMP_ON_OFF_DELAY_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_ClampOnOffDelayTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB出板帶進板延遲時間-0.1s-A軌
	ParamUnit = CParamUni();
	str = _T("PCB Out With In Delay Time Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBOutWithInDelayTime_LA, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB出板帶進板延遲時間-0.1s-B軌
	ParamUnit = CParamUni();
	str = _T("PCB Out With In Delay Time Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_OUT_WITH_IN_DELAY_TIME_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBOutWithInDelayTime_LB, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//PCB出板後延遲時間-0.1s-A軌
	ParamUnit = CParamUni();
	str = _T("PCB Out Delay Time Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_OUT_DELAY_TIME_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBOutDelayTime_LA, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB出板後延遲時間-0.1s-B軌
	ParamUnit = CParamUni();
	str = _T("PCB Out Delay Time Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_OUT_DELAY_TIME_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBOutDelayTime_LB, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB板清除時間-時間單位:1=100ms
	ParamUnit = CParamUni();
	str = _T("PCB Clear Timeout Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_CLEAR_TIMEOUT_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBClearTimeout_LA, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB板清除時間-時間單位:1=100ms
	ParamUnit = CParamUni();
	str = _T("PCB Clear Timeout Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_CLEAR_TIMEOUT_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_PCBClearTimeout_LB, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//機台向上一站要板持續時間:1=100ms
	ParamUnit = CParamUni();
	str = _T("Turn On Last Signal Delay Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_TURN_ON_LAST_SIGNAL_DELAY_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_TurnOnLastSignalDelayTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//入料檢知頻寬設定(2個板間隔時間):1=100ms
	ParamUnit = CParamUni();
	str = _T("Check PCB Double In Pitch Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_CHECK_PCB_DOUBLE_IN_PITCH_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_CheckPCBDoubleInPitchTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//入料檢知時間設定(單板最長時間):1=100ms
	ParamUnit = CParamUni();
	str = _T("Check PCB Double In Board Time");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_CHECK_PCB_DOUBLE_IN_BOARD_TIME;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_INT(Param.m_CheckPCBDoubleInBoardTime, 0);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//PCB進出自動縮停止塊-A軌
	ParamUnit = CParamUni();
	str = _T("PCB-In Auto Off Stop Bar Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_PCBInAutoOffStopBar_LA);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		
	
	//PCB進出自動縮停止塊-B軌
	ParamUnit = CParamUni();
	str = _T("PCB-In Auto Off Stop Bar Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_IN_AUTO_OFF_STOP_BAR_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_PCBInAutoOffStopBar_LB);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//PCB自動進出板帶停板邊-A軌
	ParamUnit = CParamUni();
	str = _T("PCB Auto Run With Side Stop Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_PCBAutoRunWithSideStop_LA);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//PCB自動進出板帶停板邊-B軌
	ParamUnit = CParamUni();
	str = _T("PCB Auto Run With Side Stop Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_AUTO_RUN_WITH_SIDE_STOP_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_PCBAutoRunWithSideStop_LB);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//關閉軌道感測器電源-A軌
	ParamUnit = CParamUni();
	str = _T("Turn Off Conveyer Sensor Power Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_TurnOffConveyerSensorPower_LA);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//關閉軌道感測器電源-B軌
	ParamUnit = CParamUni();
	str = _T("Turn Off Conveyer Sensor Power Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_TURN_OFF_LANE_SENSOR_POWER_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_TurnOffConveyerSensorPower_LB);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//PCB出板時清除OK, NG訊號	
	ParamUnit = CParamUni();
	str = _T("PCB Out With Clear OK NG Signal");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_PCB_OUT_WITH_CLEAR_OK_NG_SIGNAL;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_PCBOutWithClearOKNGSignal);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//啟用確認PCB重複進板
	ParamUnit = CParamUni();
	str = _T("Enable Check PCB Double In ");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_CHECK_PCB_DOUBLE_IN;	
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE); ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE); ParamUnit.AddSelItem(FN_ENABLE, strValue);	
	ParamUnit.SetValue_SEL(Param.m_EnableCheckPCBDoubleIn);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用確認檢測時PCB板闖入
	ParamUnit = CParamUni();
	str = _T("Enable Check PCB Barge In ");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_CHECK_PCB_BARGE_IN;	
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Param.m_EnableCheckPCBBargeIn);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用風扇警報	
	ParamUnit = CParamUni();
	str = _T("Enable Fan Alarm");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_FAN_ALARM;	
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Param.m_EnabledFanAlarm);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用開門斷電
	ParamUnit = CParamUni();
	str = _T("Enable Open-Door Stop Power");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_OPEN_DOOR_STOP_POWER;	
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Param.m_EnabledOpenDoorStopPower);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用擋板塊感應器-A軌				
	ParamUnit = CParamUni();
	str = _T("Enable Stopper Sensor Lane A");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_STOPPER_SENSOR_LA;	
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Param.m_EnabledStopperSensor_LA);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//啟用擋板塊感應器-B軌
	ParamUnit = CParamUni();
	str = _T("Enable Stopper Sensor Lane B");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PLC_PARAM_ENABLE_STOPPER_SENSOR_LB;	
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Param.m_EnabledStopperSensor_LB);
	strDescription = CPLC_Basic::ObtainPLCParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_16 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;	

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::BuildParamListWndHeaer()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_16 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/4;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::SetDescriptionText(const CParamUni *Ptr)
{
	if ( NULL == Ptr ) { return; }
	CWnd::SetDlgItemText(PLCPARAM_INFO_EDIT, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::HideCtrlBtn()
{
	//m_SetFocusBtn.ShowWindow(SW_HIDE);
	//m_SetFolderBtn.ShowWindow(SW_HIDE);
	//m_SetLaneWidthBtn.ShowWindow(SW_HIDE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::ExecReleaseParamCtrl()
{
	HideCtrlBtn();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::ExecItemchangedParamListWnd(CThisListCtrl_16 &ListCtrl, int nItem)
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
	{	HideCtrlBtn();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneParamList::ExecDblclkParamListWnd(CThisListCtrl_16 &ListCtrl, int nItem, int nSubItem)
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
	HideCtrlBtn();
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
bool CPLCCtrlPaneParamList::ExecUpdateParamByEdit()
{
	if ( NULL == PlcCtrlPtr ) { return false; }
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }	
	SetActParamUni(NULL);	

	CString ItemText;
	PLC_PARAM_ID ParamID = (PLC_PARAM_ID)(ParamPtr->GetParamID());		
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}

	TPLCParameter &PLCParam = PlcCtrlPtr->GetPLCParameter();
	if ( CPLC_Basic::SetPLCParameterStringByID(ParamID, PLCParam, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_16 *pListCtrl = (CThisListCtrl_16*)(ParamPtr->GetListCtrl());
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
bool CPLCCtrlPaneParamList::ExecUpdateParamByCombox()
{
	if ( NULL == PlcCtrlPtr ) { return false; }
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	PLC_PARAM_ID ParamID = (PLC_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }

	TPLCParameter &PLCParam = PlcCtrlPtr->GetPLCParameter();
	if ( CPLC_Basic::SetPLCParameterStringByID(ParamID, PLCParam, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_16 *pListCtrl = (CThisListCtrl_16*)(ParamPtr->GetListCtrl());
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
void CPLCCtrlPaneParamList::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	DWORD Res=0;
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
void CPLCCtrlPaneParamList::OnClickParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CPLCCtrlPaneParamList::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == PlcCtrlPtr ) { return ; }
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	CString str;
	if ( PlcCtrlPtr->SavePLCINIParameter() == false )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
	}
	else
	{	AOIDataCollect.BackupAllSystemIniFiles();	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneParamList::OnUpdateBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == PlcCtrlPtr ) { return ; }

	CString str;
	if ( PlcCtrlPtr->PLC_WriteINIParameter() == false )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//