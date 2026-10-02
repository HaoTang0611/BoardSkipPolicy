// LightCtrlBoardWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "LightCtrlBoardWnd.h"
//-------------------------------------------------------------------------------------//
#include "Light3DCtrl.h"
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define LIGHT_CTRL_BOARD_TEXT_SIZE 1024
//-------------------------------------------------------------------------------------//
CLightCtrlBoardWnd LightCtrlBoardWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLightCtrlBoardWnd dialog
//-------------------------------------------------------------------------------------//
CLightCtrlBoardWnd::CLightCtrlBoardWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLightCtrlBoardWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightCtrlBoardWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LoopBackIndex = 0;	
	m_TimerDelayTime = 0;
	m_TableFirstIndex = 0;
	m_WriteReadTableCount = 0;
	m_TriggerTestExecCount = 0;
	m_UpdateTableInfoEdit = true;
	ClearTotalCount();	
	m_StopListItemChanged = false;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightCtrlBoardWnd)
	DDX_Control(pDX, LCB_TABLE_LIST_WND, m_TableListWnd);	
	DDX_Control(pDX, LCB_TR_3D_CAST_ID_COMBO, m_3DCastIDComboxR);
	DDX_Control(pDX, LCB_TW_3D_CAST_ID_COMBO, m_3DCastIDComboxW);
	DDX_Control(pDX, LCB_TR_TABLE_TYPE_COMBO, m_TableTypeComboxR);
	DDX_Control(pDX, LCB_TW_TABLE_TYPE_COMBO, m_TableTypeComboxW);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CLightCtrlBoardWnd, CDialog)
	//{{AFX_MSG_MAP(CLightCtrlBoardWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(LCB_BASIC_WRITE_BTN, OnBasicWriteBtn)
	ON_BN_CLICKED(LCB_BASIC_READ_BTN, OnBasicReadBtn)
	ON_BN_CLICKED(LCB_CONNECT_CHK, OnConnectChk)
	ON_BN_CLICKED(LCB_FPGA_MODE_CHK, OnFPGAModeChk)
	ON_BN_CLICKED(LCB_ASSIGN_MODE_CHK, OnAssignModeChk)
	ON_BN_CLICKED(LCB_WRITE_MODE_CHK, OnWriteModeChk)
	ON_BN_CLICKED(LCB_READ_MODE_CHK, OnReadModeChk)	
	ON_BN_CLICKED(LCB_LOOP_BACK_CHK, OnLoopBackChk)
	ON_WM_TIMER()
	ON_BN_CLICKED(LCB_LOOP_BACK_RESET_BTN, OnLoopBackResetBtn)
	ON_BN_CLICKED(LCB_CLEAR_COUNT_BTN, OnClearCountBtn)
	ON_BN_CLICKED(LCB_CLEAR_ALL_BTN, OnClearAllBtn)
	ON_BN_CLICKED(LCB_READ_COUNT_BTN, OnReadCountBtn)
	ON_BN_CLICKED(LCB_TABLE_WRITE_BTN, OnTableWriteBtn)
	ON_BN_CLICKED(LCB_TABLE_READ_BTN, OnTableReadBtn)	
	ON_BN_CLICKED(LCB_FIRE_TRIGGER_BTN, OnFireTriggerBtn)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(LCB_READ_ALL_TABLE_BTN, OnReadAllTableBtn)
	ON_BN_CLICKED(LCB_CLEAR_ALL_TABLE_BTN, OnClearAllTableBtn)
	ON_BN_CLICKED(LCB_READ_STATUS_BTN, OnReadStatusBtn)
	ON_NOTIFY(NM_DBLCLK, LCB_TABLE_LIST_WND, OnDblclkTableListWnd)
	ON_NOTIFY(NM_CLICK, LCB_TABLE_LIST_WND, OnClickTableListWnd)
	ON_BN_CLICKED(LCB_TW_DEFAULT_BTN, OnTWDefaultBtn)
	ON_BN_CLICKED(LCB_ADVANCED_CHK, OnAdvancedChk)
	ON_BN_CLICKED(LCB_BUILD_INSPECTION_TABLE_BTN, OnBuildInspectionTableBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, LCB_TABLE_LIST_WND, OnItemchangedTableListWnd)
	ON_BN_CLICKED(LCB_WRITE_READ_TABLE_TEST_BTN, OnWriteReadTableTestBtn)
	ON_BN_CLICKED(LCB_TABLE_FIRST_ID_SET_BTN, OnTableFirstIdSetBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLightCtrlBoardWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CLightCtrlBoardWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();

	int idx=0;
	int Align = LVCFMT_CENTER;//LVCFMT_CENTER;LVCFMT_RIGHT
	int Width = 64;
	int Width2 = 48;
	BOOL bEnable = FALSE;
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();

	JetAPI::InitialListCtrl(m_TableListWnd);
	if ( LEDChannelCount <= 8 )
	{	Width = 64;	}
	else
	{	Width = 48;	}		

	m_TableListWnd.InsertColumn(idx, _T("Idx"), Align, 32); idx++;
	m_TableListWnd.InsertColumn(idx, _T("Type"), Align, Width2);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("Bt"), Align, 32);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("Exp."), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("Delay"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("3D"), Align, Width2);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("3D Trig"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("Pusle"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("Timeout"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 1"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 2"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 3"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 4"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 5"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 6"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 7"), Align, Width);	idx++;
	m_TableListWnd.InsertColumn(idx, _T("LED 8"), Align, Width);	idx++;
	if ( LEDChannelCount >= 9 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED 9"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 10 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED10"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 11 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED11"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 12 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED12"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 13 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED13"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 14 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED14"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 15 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED15"), Align, Width);	idx++;	}
	if ( LEDChannelCount >= 16 )
	{	m_TableListWnd.InsertColumn(idx, _T("LED16"), Align, Width);	idx++;	}		

	if ( LIGHT_CTRL_BOARD_3DA6 == LightCtrlBoardType )
	{
		bEnable = FALSE;	
		JetAPI::EnableEditWnd(this, LCB_TW_LED_POWER_EDIT9, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_LED_TURN_ON_CHK9, bEnable);
		JetAPI::EnableEditWnd(this, LCB_TW_LED_POWER_EDIT10, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_LED_TURN_ON_CHK10, bEnable);
		JetAPI::EnableEditWnd(this, LCB_TW_LED_POWER_EDIT11, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_LED_TURN_ON_CHK11, bEnable);
		JetAPI::EnableEditWnd(this, LCB_TW_LED_POWER_EDIT12, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_LED_TURN_ON_CHK12, bEnable);

		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK1, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK2, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK3, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK4, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK5, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK6, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK7, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TW_CAMERA_ENABLE_CHK8, bEnable);

		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK1, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK2, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK3, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK4, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK5, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK6, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK7, bEnable);
		JetAPI::EnableCtrlWnd(this, LCB_TR_CAMERA_ENABLE_CHK8, bEnable);
	}

	CWnd::SetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_EXEC_EDIT, 0);
	CWnd::SetDlgItemInt(LCB_FIRE_TRIGGER_DELAY_TIME_EDIT, 250);

	CWnd::CheckDlgButton(LCB_WRITE_READ_WRITE_TEST_CHK, TRUE);
	CWnd::SetDlgItemInt(LCB_WRITE_READ_COUNT_EDIT, MAX_TABLE_COUNT);
	CWnd::SetDlgItemInt(LCB_WRITE_READ_TABLE_TEST_EDIT, m_WriteReadTableCount);
	CWnd::SetDlgItemInt(LCB_WRITE_READ_DELAY_TIME_EDIT, 500);
	InitialParam();

#ifndef _DEBUG
	SwitchToBasicUI();
#else
	CWnd::CheckDlgButton(LCB_ADVANCED_CHK, TRUE);
	SwitchToAdvancedUI();
#endif//_DEBUG
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_TableListWnd.GetSafeHwnd() == NULL ) { return; }

	if ( m_TableListWnd.GetSafeHwnd() != NULL )
	{
		SIZE WndSize={0};
		RECT WndRect={0};
		m_TableListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.bottom = cy-16;
		JetAPI::GetRectSize(WndRect, WndSize);
		if ( WndSize.cy > 64 )
		{	m_TableListWnd.MoveWindow(&WndRect, FALSE);	}
	}
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( bShow == TRUE )
	{		
		const bool bConnected = LightCtrlBoard.GetIsConnected();
		CString BoardType = LightCtrlBoard.GetLightCtrlBoardTypeText();
		CWnd::SetDlgItemText(LCB_BOARD_TYPE_EDIT, BoardType);

		if ( true == bConnected )	
		{ 
			CString Version = LightCtrlBoard.GetFullVersion();			
			CWnd::SetDlgItemText(LCB_VERSION_EDIT, Version);
			
			ExecReadCountBtn();
			ExecReadStatusBtn();

			TLCB_TRIG_TABLE tmpTable;
			CWnd::CheckDlgButton(LCB_CONNECT_CHK, TRUE);
			CLightCtrlBoardWnd::OnReadAllTableBtn();
			const int MaxTableCount = (int)(m_TableList.size());
			if ( MaxTableCount > 0 ) 
			{	tmpTable = m_TableList[0];	}
			else
			{	LightCtrlBoard.DefaultTable(tmpTable);	}
			UpdateTableToReadUI(tmpTable);
			UpdateTableToWriteUI(tmpTable);
		}
		else											
		{ 
			CWnd::CheckDlgButton(LCB_CONNECT_CHK, FALSE);
			CWnd::SetDlgItemText(LCB_VERSION_EDIT, _T(""));
			AOIDataCollect.UserLogout_Check();
		}
		
	}
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::InitialParam()
{
	UINT CtrlID = 0;
	bool bConnected;
	BOOL bCheck = TRUE; 
	m_LoopBackIndex = 0;
	CtrlID = LCB_CONNECT_CHK;
	bConnected = LightCtrlBoard.GetIsConnected();
	if ( true == bConnected ) { bCheck = TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = LCB_BASIC_WRITE_ADDRESS_EDIT;
	CWnd::SetDlgItemText(CtrlID, _T("00"));

	CtrlID = LCB_BASIC_WRITE_DATA_EDIT;
	CWnd::SetDlgItemText(CtrlID, _T("0000"));

	CtrlID = LCB_BASIC_READ_ADDRESS_EDIT;
	CWnd::SetDlgItemText(CtrlID, _T("00"));

	CtrlID = LCB_BASIC_READ_DATA_EDIT;
	CWnd::SetDlgItemText(CtrlID, _T("0000"));

	CtrlID = LCB_COUNT_PC_TO_FPGA_EDIT;
	CWnd::SetDlgItemText(CtrlID, _T("0"));	
	CWnd::SetDlgItemInt(LCB_COUNT_PC_TO_FPGA_EDIT, 0);		
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT2, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT3, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT4, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT5, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT6, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT7, _T("0"));
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT8, _T("0"));
	CWnd::SetDlgItemInt(LCB_COUNT_CAMERA_RECEIVE_EDIT, 0);		
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT1, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT2, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT3, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT4, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT5, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT6, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT7, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_FPGA_TO_3D_CAST_EDIT8, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT1, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT2, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT3, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT4, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT5, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT6, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT7, 0);
	CWnd::SetDlgItemInt(LCB_COUNT_3D_CAST_TO_FPGA_EDIT8, 0);	
	
	//Trigger
	CWnd::SetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_EDIT, 1);

	//Table
	CWnd::SetDlgItemInt(LCB_TW_INDEX_EDIT, 1);	
	CWnd::SetDlgItemInt(LCB_TR_INDEX_EDIT, 1);

	CWnd::SetDlgItemInt(LCB_TW_TABLE_INDEX_EDIT, 1);
	CWnd::SetDlgItemInt(LCB_TR_TABLE_INDEX_EDIT, 1);	
	CLightCtrlBoard::BuildTableTypeCombox(m_TableTypeComboxW);
	CLightCtrlBoard::BuildTableTypeCombox(m_TableTypeComboxR);
	CLightCtrlBoard::BuildTable3DCastCombox(m_3DCastIDComboxW);
	CLightCtrlBoard::BuildTable3DCastCombox(m_3DCastIDComboxR);

	JetAPI::SetComboxCurSel(m_3DCastIDComboxW, NULL);
	JetAPI::SetComboxCurSel(m_3DCastIDComboxR, NULL);
	JetAPI::SetComboxCurSel(m_TableTypeComboxW, NULL);
	JetAPI::SetComboxCurSel(m_TableTypeComboxR, NULL);
	
	int i=0;
	TLCB_TRIG_TABLE tmpTable;	
	const int MaxTableCount = MAX_TABLE_COUNT;	
	m_TableList.clear();
	LightCtrlBoard.DefaultTable(tmpTable);
	for ( i=0; i<MaxTableCount; i++ )
	{
		tmpTable.sTableID = i;
		m_TableList.push_back(tmpTable);
	}	
	tmpTable.sTableID = 0;
	UpdateTableListWnd(m_TableList);
	UpdateTableToReadUI(tmpTable);
	UpdateTableToWriteUI(tmpTable);		
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_LIGHT_CTRL_BOARD_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_LIGHT_CTRL_BOARD_WND;
	WndKey = _T("IDD_LIGHT_CTRL_BOARD_WND");
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
	//LCB = Light Ctrl Board
	WndID = LCB_ADVANCED_CHK;
	WndKey = _T("LCB_ADVANCED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LCB_CONNECT_CHK;
	WndKey = _T("LCB_CONNECT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	


	WndID = LCB_VERSION_LABEL;
	WndKey = _T("LCB_VERSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_LOOP_BACK_CHK;
	WndKey = _T("LCB_LOOP_BACK_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LCB_LOOP_BACK_RESET_BTN;
	WndKey = _T("LCB_LOOP_BACK_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = LCB_BASIC_GROUP;
	WndKey = _T("LCB_BASIC_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_WRITE_ADDRESS_LABEL;
	WndKey = _T("LCB_BASIC_WRITE_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_WRITE_DATA_LABEL;
	WndKey = _T("LCB_BASIC_WRITE_DATA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_WRITE_BTN;
	WndKey = _T("LCB_BASIC_WRITE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_READ_ADDRESS_LABEL;
	WndKey = _T("LCB_BASIC_READ_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_READ_DATA_LABEL;
	WndKey = _T("LCB_BASIC_READ_DATA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BASIC_READ_BTN;
	WndKey = _T("LCB_BASIC_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = LCB_CTRL_MODE_GROUP;
	WndKey = _T("LCB_CTRL_MODE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_FPGA_MODE_CHK;
	WndKey = _T("LCB_FPGA_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_ASSIGN_MODE_CHK;
	WndKey = _T("LCB_ASSIGN_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_WRITE_MODE_CHK;
	WndKey = _T("LCB_WRITE_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_READ_MODE_CHK;
	WndKey = _T("LCB_READ_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = LCB_READ_STATUS_BTN;
	WndKey = _T("LCB_READ_STATUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_CLEAR_ALL_BTN;
	WndKey = _T("LCB_CLEAR_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_CLEAR_COUNT_BTN;
	WndKey = _T("LCB_CLEAR_COUNT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_READ_COUNT_BTN;
	WndKey = _T("LCB_READ_COUNT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_COUNT_PC_TO_FPGA_LABEL;
	WndKey = _T("LCB_COUNT_PC_TO_FPGA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_COUNT_FPGA_TO_CAMERA_LABEL;
	WndKey = _T("LCB_COUNT_FPGA_TO_CAMERA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_COUNT_FPGA_TO_3D_CAST_LABEL;
	WndKey = _T("LCB_COUNT_FPGA_TO_3D_CAST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_COUNT_CAMERA_RECEIVE_LABEL;
	WndKey = _T("LCB_COUNT_CAMERA_RECEIVE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Table Write
	WndID = LCB_TABLE_WRITE_GROUP;
	WndKey = _T("LCB_TABLE_WRITE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = LCB_TW_TABLE_INDEX_LABEL;
	WndKey = _T("LCB_TW_TABLE_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LCB_TW_DEFAULT_BTN;
	WndKey = _T("LCB_TW_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TABLE_WRITE_BTN;
	WndKey = _T("LCB_TABLE_WRITE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_TABLE_TYPE_LABEL;
	WndKey = _T("LCB_TW_TABLE_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_NEXT_TABLE_TIME_LABEL;
	WndKey = _T("LCB_TW_NEXT_TABLE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_CAMERA_EXP_TIME_LABEL;
	WndKey = _T("LCB_TW_CAMERA_EXP_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_CAMERA_DELAY_TIME_LABEL;
	WndKey = _T("LCB_TW_CAMERA_DELAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_3D_CAST_TRIG_CONT_LABEL;
	WndKey = _T("LCB_TW_3D_CAST_TRIG_CONT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_3D_CAST_ID_LABEL;
	WndKey = _T("LCB_TW_3D_CAST_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_3D_CAST_PULSE_TIME_LABEL;
	WndKey = _T("LCB_TW_3D_CAST_PULSE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_3D_CAST_CALLBACK_TIMEOUT_LABEL;
	WndKey = _T("LCB_TW_3D_CAST_CALLBACK_TIMEOUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL1;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK1;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL2;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK2;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL3;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK3;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL4;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK4;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL5;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK5;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL6;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK6;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL7;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK7;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL8;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK8;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL9;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL9");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK9;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK9");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL10;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK10;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL11;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK11;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_CHANNEL_LABEL12;
	WndKey = _T("LCB_TW_LED_CHANNEL_LABEL12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_LED_TURN_ON_CHK12;
	WndKey = _T("LCB_TW_LED_TURN_ON_CHK12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TW_CAMERA_ENABLE_GROUP;
	WndKey = _T("LCB_TW_CAMERA_ENABLE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Table Read
	WndID = LCB_TABLE_READ_GROUP;
	WndKey = _T("LCB_TABLE_READ_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TABLE_READ_BTN;
	WndKey = _T("LCB_TABLE_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_TABLE_INDEX_LABEL;
	WndKey = _T("LCB_TR_TABLE_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_TABLE_TYPE_LABEL;
	WndKey = _T("LCB_TR_TABLE_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_NEXT_TABLE_TIME_LABEL;
	WndKey = _T("LCB_TR_NEXT_TABLE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_CAMERA_EXP_TIME_LABEL;
	WndKey = _T("LCB_TR_CAMERA_EXP_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_CAMERA_DELAY_TIME_LABEL;
	WndKey = _T("LCB_TR_CAMERA_DELAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_3D_CAST_TRIG_CONT_LABEL;
	WndKey = _T("LCB_TR_3D_CAST_TRIG_CONT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_3D_CAST_ID_LABEL;
	WndKey = _T("LCB_TR_3D_CAST_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_3D_CAST_PULSE_TIME_LABEL;
	WndKey = _T("LCB_TR_3D_CAST_PULSE_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_3D_CAST_CALLBACK_TIMEOUT_LABEL;
	WndKey = _T("LCB_TR_3D_CAST_CALLBACK_TIMEOUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL1;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK1;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL2;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK2;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL3;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK3;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL4;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK4;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL5;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK5;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL6;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK6;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL7;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK7;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL8;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK8;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL9;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL9");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK9;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK9");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL10;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK10;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL11;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK11;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_CHANNEL_LABEL12;
	WndKey = _T("LCB_TR_LED_CHANNEL_LABEL12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_LED_TURN_ON_CHK12;
	WndKey = _T("LCB_TR_LED_TURN_ON_CHK12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TR_CAMERA_ENABLE_GROUP;
	WndKey = _T("LCB_TR_CAMERA_ENABLE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = LCB_FIRE_TRIGGER_BTN;
	WndKey = _T("LCB_FIRE_TRIGGER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TABLE_FIRST_ID_SET_BTN;
	WndKey = _T("LCB_TABLE_FIRST_ID_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TRIGGER_REPEAT_CHK;
	WndKey = _T("LCB_TRIGGER_REPEAT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_TRIGGER_TABLE_COUNT_LABEL;
	WndKey = _T("LCB_TRIGGER_TABLE_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_FIRE_TRIGGER_COUNT_REST_CHK;
	WndKey = _T("LCB_FIRE_TRIGGER_COUNT_REST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_FIRE_TRIGGER_DELAY_TIME_LABEL;
	WndKey = _T("LCB_FIRE_TRIGGER_DELAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = LCB_READ_ALL_TABLE_BTN;
	WndKey = _T("LCB_READ_ALL_TABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_CLEAR_ALL_TABLE_BTN;
	WndKey = _T("LCB_CLEAR_ALL_TABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_BUILD_INSPECTION_TABLE_BTN;
	WndKey = _T("LCB_BUILD_INSPECTION_TABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = LCB_WRITE_READ_TABLE_TEST_BTN;
	WndKey = _T("LCB_WRITE_READ_TABLE_TEST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_WRITE_READ_TABLE_TEST_CHK;
	WndKey = _T("LCB_WRITE_READ_TABLE_TEST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_WRITE_READ_WRITE_TEST_CHK;
	WndKey = _T("LCB_WRITE_READ_WRITE_TEST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LCB_WRITE_READ_COUNT_LABEL;
	WndKey = _T("LCB_WRITE_READ_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = LCB_WRITE_READ_DELAY_TIME_LABEL;
	WndKey = _T("LCB_WRITE_READ_DELAY_TIME_LABEL");
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
	*/
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::LockUIWnd(bool bLock)
{
	BOOL bEnable = TRUE;
	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }
}
//-------------------------------------------------------------------------------------//
BOOL CLightCtrlBoardWnd::CheckContrlBoard()
{
	if ( LightCtrlBoard.GetIsConnected() == false )
	{
		JetAPI::ShowMessageBox(_T("Error, Not Connect to Light Ctrl Board"));
		return FALSE; 
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
LIGHT_CTRL_BOARD_IMP_CLS CLightCtrlBoardWnd::GetLightCtrlBoardClass()
{	
	return LightCtrlBoard.GetLightCtrlBoardClass();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnConnectChk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = LCB_CONNECT_CHK;
	BOOL bCheck = CWnd::IsDlgButtonChecked(CtrlID);
	const int StrSize = LIGHT_CTRL_BOARD_TEXT_SIZE;	
	
	if ( FALSE == bCheck )
	{
		LightCtrlBoard.DisConnect();
		CWnd::SetDlgItemText(LCB_VERSION_EDIT, _T(""));
		return ;
	}
	if ( LightCtrlBoard.Connect() == false )	
	{
		CWnd::CheckDlgButton(CtrlID, FALSE);
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());		
		return;
	}	

	CString str = _T("");
	CString Version = LightCtrlBoard.GetFullVersion();	
	CWnd::SetDlgItemText(LCB_VERSION_EDIT, Version);

	if ( LightCtrlBoard.ClearAllCount() == false )
	{ 
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
	}

	if ( LightCtrlBoard.ClearAll() == false )
	{ 
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
	}

	TLCB_TRIG_TABLE tmpTable;	
	CLightCtrlBoardWnd::OnReadAllTableBtn();
	const int MaxTableCount = (int)(m_TableList.size());
	if ( MaxTableCount > 0 ) 
	{	tmpTable = m_TableList[0];	}
	else
	{	LightCtrlBoard.DefaultTable(tmpTable);	}
	UpdateTableToReadUI(tmpTable);
	UpdateTableToWriteUI(tmpTable);	

	CLightCtrlBoardWnd::OnFPGAModeChk();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnBasicWriteBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	CString strData;
	CString strAddress;

	CWnd::GetDlgItemText(LCB_BASIC_WRITE_DATA_EDIT, strData);
	CWnd::GetDlgItemText(LCB_BASIC_WRITE_ADDRESS_EDIT, strAddress);

	std::string Data = "";
	std::string Address = "";
	JetAPI::TCHAR2string(strData, Data);
	JetAPI::TCHAR2string(strAddress, Address);
	if ( LightCtrlBoard.WriteLightCtrlBorad(Address, Data) == false )
	{	JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	}	
	UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnBasicReadBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	CString strData;
	CString strAddress;

	CWnd::GetDlgItemText(LCB_BASIC_READ_DATA_EDIT, strData);
	CWnd::GetDlgItemText(LCB_BASIC_READ_ADDRESS_EDIT, strAddress);

	std::string Data = "";
	std::string Address = "";	
	JetAPI::TCHAR2string(strAddress, Address);
	if ( LightCtrlBoard.ReadLightCtrlBorad(Address, Data) == false )
	{	JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	}
	strData = CString(Data.c_str());
	CWnd::SetDlgItemText(LCB_BASIC_READ_DATA_EDIT, strData);
	UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnFPGAModeChk() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.SetMode_FPGA() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	CLightCtrlBoardWnd::UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnAssignModeChk() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.SetMode_PCAssign() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	CLightCtrlBoardWnd::UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnWriteModeChk() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.SetMode_PCWrite() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	CLightCtrlBoardWnd::UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnReadModeChk() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.SetMode_PCRead() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	CLightCtrlBoardWnd::UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnLoopBackChk() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if ( CWnd::IsDlgButtonChecked(LCB_LOOP_BACK_CHK) == FALSE )	{	return ; }	
	CWnd::SetDlgItemText(LCB_LOOP_BACK_EDIT, _T(""));
	LIGHT_CTRL_BOARD_IMP_CLS ImpCls=GetLightCtrlBoardClass();	
	if ( LIGHT_CTRL_BOARD_IMP_FPGA == ImpCls )
	{	ExecLoopBackTimer(m_LoopBackIndex); }
	if ( LIGHT_CTRL_BOARD_IMP_ARDUINO == ImpCls )
	{	CWnd::CheckDlgButton(LCB_LOOP_BACK_CHK, FALSE); }
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecLoopBackTimer(int LoopIndex)
{
	const int StrSize = LIGHT_CTRL_BOARD_TEXT_SIZE;	
	CString str;
	char DataWS[StrSize] = "";
	char DataRSA[StrSize] = "";	
	char DataRSB[StrSize] = "";	
	if ( LightCtrlBoard.LoopBackTest(LoopIndex, DataWS,  DataRSA, DataRSB) == false )
	{	return false; }

	CString strDataW = DataWS;
	CString strDataRA = DataRSA;
	CString strDataRB = DataRSB;
	if ( strDataRA!=strDataW || strDataRB!=strDataW )
	{
		str.Format(_T("Err, %d, W:%s, RA:%s, RB:%s"), LoopIndex, strDataW, strDataRA, strDataRB);
		CWnd::SetDlgItemText(LCB_LOOP_BACK_EDIT, str);
		return FALSE;
	}
	str.Format(_T("%d, W:%s, RA:%s, RB:%s"), LoopIndex, strDataW, strDataRA, strDataRB);	
	CWnd::SetDlgItemText(LCB_LOOP_BACK_EDIT, str);
	CWnd::SetTimer(LCB_TIMER_LOOP_BACK, 0, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecFireTriggerBtn()
{
	bool IsError=false;
	CString ErrStr;
	LightCtrlBoard.CheckLightCtrlBoardError(IsError, ErrStr);
	if ( true == IsError )
	{
		JetAPI::ShowMessageBox(ErrStr);
		return false;
	}

	BOOL bResetCnt = CWnd::IsDlgButtonChecked(LCB_FIRE_TRIGGER_COUNT_REST_CHK);
	if ( TRUE == bResetCnt )
	{
		if( LightCtrlBoard.ClearAllCount() == false )	
		{
			JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
			return false;
		}
		if ( ClearCameraCount(false) == false )
		{	return false; }
	}

	if ( LightCtrlBoard.TriggerStart() == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}
	m_TriggerTestExecCount ++;
	CWnd::SetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_EXEC_EDIT, m_TriggerTestExecCount);
	CWnd::SetTimer(LCB_TIMER_REPEAT_TRIGGER, m_TimerDelayTime, NULL);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecFireTriggerNext()
{
	ExecReadCountBtn();
	if ( ExecReadStatusBtn() == false )
	{	return false;	}
	if ( CWnd::IsDlgButtonChecked(LCB_TRIGGER_REPEAT_CHK) == FALSE )
	{	return true;	}

	if ( ExecFireTriggerBtn() == false )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::EnableDLPMask()//確認DLP使用遮罩
{
	size_t           i=0;
	int     DLPMask=0;
	int     DLPChannel = 0;
	int     DLPChannelMask = 0;
	TLCB_TRIG_TABLE *pTable = NULL;
	const size_t     TableCount = m_TableList.size();
	for ( i=0; i<TableCount; i++ )
	{
		pTable = &(m_TableList[i]);

		DLPChannel = pTable->sDLPActiveChannel;
		DLPChannelMask = CLightCtrlBoard::GetTableDLPChannelMask(DLPChannel);
		if ( 0 == (DLPMask&DLPChannelMask) )
		{	DLPMask |= DLPChannelMask; }		
	}
	/*
	if ( DLPMask > 0 ) 
	{
		LIGHT_3D_CAST_ID Light3DID;
		//確認DLP是否PLAY中
		DLPChannel = 0;
		for ( i=0; i<8; i++ )
		{
			DLPChannel = (int)(i+1);
			Light3DID = (LIGHT_3D_CAST_ID)(LIGHT_3D_CAST_01+i);
			DLPChannelMask = CLightCtrlBoard::GetTableDLPChannelMask(DLPChannel);
			if ( 0 == (DLPMask&DLPChannelMask) )
			{	
				if ( Light3DCtrl.ExecLight3DSequencePlay(Light3DID) == false )
				{
					JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
					return FALSE;
				}
			}
		}
	}
	*/

	if ( LightCtrlBoard.EnableDLPChannel(DLPMask) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ClearTotalCount()
{
	m_TotalCnt_PCtoFPGA = 0;
	m_TotalCnt_FPGAtoCamera1 = 0;
	m_TotalCnt_FPGAtoCamera2 = 0;
	m_TotalCnt_FPGAtoCamera3 = 0;
	m_TotalCnt_FPGAtoCamera4 = 0;
	m_TotalCnt_FPGAtoCamera5 = 0;
	m_TotalCnt_FPGAtoCamera6 = 0;
	m_TotalCnt_FPGAtoCamera7 = 0;
	m_TotalCnt_FPGAtoCamera8 = 0;
	m_TotalCnt_FPGAtoDLP1 = 0;
	m_TotalCnt_FPGAtoDLP2 = 0;
	m_TotalCnt_FPGAtoDLP3 = 0;
	m_TotalCnt_FPGAtoDLP4 = 0;
	m_TotalCnt_FPGAtoDLP5 = 0;
	m_TotalCnt_FPGAtoDLP6 = 0;
	m_TotalCnt_FPGAtoDLP7 = 0;
	m_TotalCnt_FPGAtoDLP8 = 0;
	m_TotalCnt_DLP1toFPGA = 0;
	m_TotalCnt_DLP2toFPGA = 0;
	m_TotalCnt_DLP3toFPGA = 0;
	m_TotalCnt_DLP4toFPGA = 0;
	m_TotalCnt_DLP5toFPGA = 0;
	m_TotalCnt_DLP6toFPGA = 0;
	m_TotalCnt_DLP7toFPGA = 0;
	m_TotalCnt_DLP8toFPGA = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::KillTimer(nIDEvent);
	switch ( nIDEvent )
	{
	case LCB_TIMER_LOOP_BACK:
		if ( CWnd::IsDlgButtonChecked(LCB_LOOP_BACK_CHK) == FALSE )
		{
			::Sleep(100);
			ExecReadStatusBtn();
			return ; 
		}
		m_LoopBackIndex ++;
		if ( m_LoopBackIndex > 0xFFFF )
		{
			CWnd::CheckDlgButton(LCB_LOOP_BACK_CHK, FALSE);
			CWnd::SetDlgItemText(LCB_LOOP_BACK_EDIT, _T("Success"));
			return;
		}
		if ( ExecLoopBackTimer(m_LoopBackIndex) == false )
		{	CWnd::CheckDlgButton(LCB_LOOP_BACK_CHK, FALSE);	}
		break;
	case LCB_TIMER_REPEAT_TRIGGER:
		if ( ExecFireTriggerNext() == false )
		{	CWnd::CheckDlgButton(LCB_TRIGGER_REPEAT_CHK, FALSE);	}
		break;
	case LCB_TIMER_REPEAT_WRITE_READ:
		ExecWriteReadTableBtn();		
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateFPGACtrlModeChk()
{
	const int CtrlMode = LightCtrlBoard.GetCurFPGAMode();
	switch ( CtrlMode )
	{
	case FPGA_MODE_FPGA:
		CWnd::CheckDlgButton(LCB_FPGA_MODE_CHK, TRUE);
		CWnd::CheckDlgButton(LCB_WRITE_MODE_CHK, FALSE);
		CWnd::CheckDlgButton(LCB_READ_MODE_CHK, FALSE);
		CWnd::CheckDlgButton(LCB_ASSIGN_MODE_CHK, FALSE);
		break;
	case FPGA_MODE_WRITE:
		CWnd::CheckDlgButton(LCB_WRITE_MODE_CHK, TRUE);
		CWnd::CheckDlgButton(LCB_FPGA_MODE_CHK, FALSE);		
		CWnd::CheckDlgButton(LCB_READ_MODE_CHK, FALSE);
		CWnd::CheckDlgButton(LCB_ASSIGN_MODE_CHK, FALSE);
		break;
	case FPGA_MODE_READ:
		CWnd::CheckDlgButton(LCB_READ_MODE_CHK, TRUE);		
		CWnd::CheckDlgButton(LCB_FPGA_MODE_CHK, FALSE);		
		CWnd::CheckDlgButton(LCB_WRITE_MODE_CHK, FALSE);
		CWnd::CheckDlgButton(LCB_ASSIGN_MODE_CHK, FALSE);
		break;
	case FPGA_MODE_ASSIGN:
		CWnd::CheckDlgButton(LCB_ASSIGN_MODE_CHK, TRUE);
		CWnd::CheckDlgButton(LCB_READ_MODE_CHK, FALSE);		
		CWnd::CheckDlgButton(LCB_FPGA_MODE_CHK, FALSE);		
		CWnd::CheckDlgButton(LCB_WRITE_MODE_CHK, FALSE);		
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnLoopBackResetBtn() 
{
	// TODO: Add your control notification handler code here	
	CWnd::CheckDlgButton(LCB_LOOP_BACK_CHK, FALSE);
	m_LoopBackIndex = 0;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnClearCountBtn() 
{
	// TODO: Add your control notification handler code here
	ClearTotalCount();
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.ClearAllCount() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	ClearCameraCount(true);
	ExecReadCountBtn();
	CWnd::SetDlgItemInt(LCB_TW_INDEX_EDIT, 1);
	CWnd::SetDlgItemInt(LCB_TR_INDEX_EDIT, 1);
	UpdateFPGACtrlModeChk();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnClearAllBtn() 
{
	// TODO: Add your control notification handler code here
	ClearTotalCount();
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }	
	if( LightCtrlBoard.ClearAll() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	::Sleep(100);
	ExecReadCountBtn();
	ExecReadStatusBtn();
	CWnd::SetDlgItemInt(LCB_TW_INDEX_EDIT, 1);
	CWnd::SetDlgItemInt(LCB_TR_INDEX_EDIT, 1);
	UpdateFPGACtrlModeChk();	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecReadCountBtn()
{
	CString str;
	UINT count = 0;
	BOOL bResetCnt = CWnd::IsDlgButtonChecked(LCB_FIRE_TRIGGER_COUNT_REST_CHK);
	if ( CheckContrlBoard() == FALSE ) { return false; }
	if( LightCtrlBoard.ReadBoardAllCount() == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }

	if( LightCtrlBoard.GetPCtoFPGATrigCount(count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }	
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_PCtoFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_PCtoFPGA);
	}
	else
	{	
		m_TotalCnt_PCtoFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_PC_TO_FPGA_EDIT, str);
	
	//FPGA to Camera
	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera1 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera1);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera1 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT, str);	

	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_2, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera2 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera2);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera2 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT2, str);	

	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_3, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera3 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera3);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera3 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT3, str);	

	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_4, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera4 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera4);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera4 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT4, str);	
	
	
	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_5, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera5 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera5);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera5 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT5, str);	
	
	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_6, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera6 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera6);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera6 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT6, str);	

	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_7, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera7 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera7);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera7 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT7, str);	
	
	if( LightCtrlBoard.GetFPGAtoCCDTrigCount(CAMERA_ID_8, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoCamera8 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoCamera8);
	}
	else
	{	
		m_TotalCnt_FPGAtoCamera8 = count; 
		str.Format(_T("%d"), count);
	}
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_CAMERA_EDIT8, str);		
	

	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_1, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP1 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP1);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP1 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT1, str);	
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_2, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP2 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP2);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP2 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT2, str);
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_3, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP3 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP3);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP3 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT3, str);
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_4, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP4 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP4);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP4 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT4, str);	
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_5, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP5 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP5);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP5 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT5, str);
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_6, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP6 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP6);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP6 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT6, str);

	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_7, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP7 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP7);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP7 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT7, str);
	
	if( LightCtrlBoard.GetFPGAtoDLPTrigCount(DLP_CHANNEL_8, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_FPGAtoDLP8 += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_FPGAtoDLP8);
	}
	else
	{	
		m_TotalCnt_FPGAtoDLP8 = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_FPGA_TO_3D_CAST_EDIT8, str);

	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_1, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP1toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP1toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP1toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT1, str);
	
	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_2, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP2toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP2toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP2toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT2, str);
	
	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_3, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP3toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP3toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP3toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT3, str);
	
	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_4, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP4toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP4toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP4toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT4, str);
	
	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_5, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP5toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP5toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP5toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT5, str);

	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_6, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP6toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP6toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP6toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT6, str);

	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_7, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP7toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP7toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP7toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT7, str);
	
	if( LightCtrlBoard.GetDLPtoFPGATrigCount(DLP_CHANNEL_8, count) == false )
	{ JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString()); return false; }		
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_DLP8toFPGA += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_DLP8toFPGA);
	}
	else
	{	
		m_TotalCnt_DLP8toFPGA = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_3D_CAST_TO_FPGA_EDIT8, str);		
	UpdateCameraCount();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnReadCountBtn() 
{
	// TODO: Add your control notification handler code here
	ExecReadCountBtn();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnTableWriteBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }

	TLCB_TRIG_TABLE tmpTable;
	LightCtrlBoard.DefaultTable(tmpTable);

	TLCB_TRIG_TABLE *pTable = &tmpTable;
	TLCB_LED_ITEM tmpPWM;
	CString str = "";
	int TempI = 0;
	int MultiDLPID = 0;
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	
	this->GetDlgItemText(LCB_TW_CAMERA_EXP_TIME_EDIT, str);
	const int CameraExpTime = ::_ttoi(str);

	this->GetDlgItemText(LCB_TW_CAMERA_DELAY_TIME_EDIT, str);
	const int CameraDelayTime = ::_ttoi(str);

	const int TableID = this->GetDlgItemInt(LCB_TW_INDEX_EDIT)-1;
		
	pTable->sTableID = TableID;
	this->SetDlgItemInt(LCB_TW_TABLE_INDEX_EDIT, pTable->sTableID+1);
//(1)
	pTable->sTableType = (int)(JetAPI::GetComboxCurSelData(m_TableTypeComboxW));
	
	this->GetDlgItemText(LCB_TW_3D_CAST_TRIG_CONT_EDIT, str);		TempI = ::_ttoi(str);
	pTable->sDLPTrigOutNumber = TempI;		//DLP觸發數量

	//8個DLP每個Table最多只能啟動一個DLP
	pTable->sDLPActiveChannel=(BYTE)(JetAPI::GetComboxCurSelData(m_3DCastIDComboxW));	

//(2)(3)
	this->GetDlgItemText(LCB_TW_NEXT_TABLE_TIME_EDIT, str);	TempI = ::_ttoi(str);
	pTable->sNextTableTime = TempI;				//Table間距時間, 第一個Table要設0

//(4)(5)
	pTable->sLEDTableTotalTime=CameraDelayTime+CameraExpTime;		//LED Table總時間 (相機延遲時間+相機曝光時間)

//(6)
	pTable->sCCDDelayTime=CameraDelayTime;				//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)

//(7)
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT1, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK1) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM1=tmpPWM;		
	
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT2, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK2) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM2=tmpPWM;			
//(8)
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT3, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK3) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM3=tmpPWM;
	
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT4, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK4) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM4=tmpPWM;			
//(9)
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT5, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK5) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM5=tmpPWM;
	
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT6, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK6) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM6=tmpPWM;			
//(10)
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT7, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK7) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }
	pTable->sPWM7=tmpPWM;
	
	this->GetDlgItemText(LCB_TW_LED_POWER_EDIT8, str); tmpPWM.sPower = ::_ttoi(str);
	if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK8) == 0 )	{ tmpPWM.sIsON = false; }
	else															{ tmpPWM.sIsON = true;  }			
	pTable->sPWM8=tmpPWM;			

	if ( LEDChannelCount >= 9 )
	{
		//(11-v2)
		this->GetDlgItemText(LCB_TW_LED_POWER_EDIT9, str); tmpPWM.sPower = ::_ttoi(str);
		if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK9) == 0 )	{ tmpPWM.sIsON = false; }
		else															{ tmpPWM.sIsON = true;  }			
		pTable->sPWM9=tmpPWM;
	}
	if ( LEDChannelCount >= 10 )
	{
		//(11-v2)
		this->GetDlgItemText(LCB_TW_LED_POWER_EDIT10, str); tmpPWM.sPower = ::_ttoi(str);
		if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK10) == 0 )	{ tmpPWM.sIsON = false; }
		else															{ tmpPWM.sIsON = true;  }			
		pTable->sPWM10=tmpPWM;
	}
	if ( LEDChannelCount >= 11 )
	{
		//(12-v2)
		this->GetDlgItemText(LCB_TW_LED_POWER_EDIT11, str); tmpPWM.sPower = ::_ttoi(str);
		if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK11) == 0 )	{ tmpPWM.sIsON = false; }
		else															{ tmpPWM.sIsON = true;  }			
		pTable->sPWM11=tmpPWM;
	}
	if ( LEDChannelCount >= 12 )
	{
		//(12-v2)
		this->GetDlgItemText(LCB_TW_LED_POWER_EDIT12, str); tmpPWM.sPower = ::_ttoi(str);
		if( this->IsDlgButtonChecked(LCB_TW_LED_TURN_ON_CHK12) == 0 )	{ tmpPWM.sIsON = false; }
		else															{ tmpPWM.sIsON = true;  }			
		pTable->sPWM12=tmpPWM;
	}

	//(13)
	//pTable->sPWM13
	//pTable->sPWM14
	//(14)
	//pTable->sPWM15
	//pTable->sPWM16
	if ( LIGHT_CTRL_BOARD_3DA6 == LightCtrlBoardType )
	{	pTable->sCameraEnable = 0x01; }
	else
	{	UpdateTableFromWriteUI_Camera(pTable->sCameraEnable);	}

//(11-v1)
	this->GetDlgItemText(LCB_TW_3D_CAST_PULSE_TIME_EDIT, str);	TempI = ::_ttoi(str);
	pTable->sDLPPulseTime=TempI;			//DLP Pulse Width的時間(us)
//(12-v1)(13-v1)
	this->GetDlgItemText(LCB_TW_3D_CAST_CALLBACK_TIMEOUT_EDIT, str);	TempI = ::_ttoi(str);
	pTable->sDLPCallbackTime=TempI;			//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常
//(14-v1)
	if ( TABLE_TYPE_DLP == pTable->sTableType ) //DLP的相機曝光時間(us)
	{	pTable->sDLPCCDExpTime=CameraExpTime;	}
	else
	{	pTable->sDLPCCDExpTime=0; }	

	if (TABLE_TYPE_DLP == pTable->sTableType)
	{			
		LIGHT_3D_CAST_ID CastID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(pTable->sDLPActiveChannel);
		LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL==CastPtr || false==CastPtr->GetDLPReadySignalEnable() )
		{	pTable->sDLPReadySignalEnable = FN_DISABLE;		}
		else
		{	pTable->sDLPReadySignalEnable = FN_ENABLE;	}
	}
	else
	{	pTable->sDLPReadySignalEnable = FN_DISABLE;		}

//寫入Table	
	if( LightCtrlBoard.TableSingleWrite(*pTable) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}

	LightCtrlBoard.SetDLPEnable(0);

	this->SetDlgItemInt(LCB_TW_INDEX_EDIT, TableID+2);

//再讀出Table
	OnTableReadBtn();
	UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnTableReadBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }

	const int TableID = this->GetDlgItemInt(LCB_TR_INDEX_EDIT)-1;
	
	TLCB_TRIG_TABLE tmpTable;
	LightCtrlBoard.DefaultTable(tmpTable);
	tmpTable.sTableID = TableID;
	if( LightCtrlBoard.TableSingleRead(tmpTable) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	if ( TableID < m_TableList.size() )
	{	m_TableList[TableID] = tmpTable;	}
	UpdateTableListWnd(m_TableList);

	this->SetDlgItemInt(LCB_TR_INDEX_EDIT, TableID+2);
	UpdateTableToReadUI(tmpTable);
	UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateTableToReadUI_Camera(UINT val)
{
	BOOL bCheck=FALSE;
	if ( val&0x01 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK1, bCheck);

	if ( val&0x02 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK2, bCheck);

	if ( val&0x04 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK3, bCheck);
	
	if ( val&0x08 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK4, bCheck);

	if ( val&0x10 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK5, bCheck);

	if ( val&0x20 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK6, bCheck);

	if ( val&0x40 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK7, bCheck);

	if ( val&0x80 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TR_CAMERA_ENABLE_CHK8, bCheck);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateTableToWriteUI_Camera(UINT val)
{
	BOOL bCheck=FALSE;
	if ( val&0x01 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK1, bCheck);

	if ( val&0x02 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK2, bCheck);

	if ( val&0x04 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK3, bCheck);
	
	if ( val&0x08 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK4, bCheck);

	if ( val&0x10 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK5, bCheck);

	if ( val&0x20 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK6, bCheck);

	if ( val&0x40 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK7, bCheck);

	if ( val&0x80 ) { bCheck=TRUE; }
	else { bCheck = FALSE; }
	CWnd::CheckDlgButton(LCB_TW_CAMERA_ENABLE_CHK8, bCheck);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CLightCtrlBoardWnd::UpdateTableFromWriteUI_Camera(UINT &val)
{
	val = 0x00;
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK1) == TRUE ) { val |= 0x01; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK2) == TRUE ) { val |= 0x02; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK3) == TRUE ) { val |= 0x04; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK4) == TRUE ) { val |= 0x08; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK5) == TRUE ) { val |= 0x10; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK6) == TRUE ) { val |= 0x20; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK7) == TRUE ) { val |= 0x40; }
	if ( CWnd::IsDlgButtonChecked(LCB_TW_CAMERA_ENABLE_CHK8) == TRUE ) { val |= 0x80; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateTableToReadUI(TLCB_TRIG_TABLE &table)
{
	TLCB_TRIG_TABLE *pTable = &table;
	CString tmpStr = _T("");
	const int TableMode = pTable->sTableType;
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();

	this->SetDlgItemInt(LCB_TR_TABLE_INDEX_EDIT, pTable->sTableID+1);	
	JetAPI::SetComboxCurSel(m_TableTypeComboxR, pTable->sTableType);

	//pTable->sCameraEnable;
	tmpStr.Format(_T("%d"), pTable->sNextTableTime);
	this->SetDlgItemText(LCB_TR_NEXT_TABLE_TIME_EDIT, tmpStr);

	if( TableMode == TABLE_TYPE_LED )
	{ tmpStr.Format(_T("%d"), pTable->sLEDTableTotalTime-pTable->sCCDDelayTime); }
	else if( TableMode == TABLE_TYPE_DLP )
	{ tmpStr.Format(_T("%d"), pTable->sDLPCCDExpTime); }
	else
	{ tmpStr.Format(_T("%d"), 0); }
	this->SetDlgItemText(LCB_TR_CAMERA_EXP_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sCCDDelayTime);
	this->SetDlgItemText(LCB_TR_CAMERA_DELAY_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sDLPTrigOutNumber);
	this->SetDlgItemText(LCB_TR_3D_CAST_TRIG_CONT_EDIT, tmpStr);

	//tmpStr.Format(_T("CH. %d"), pTable->sDLPActiveChannel);	
	JetAPI::SetComboxCurSel(m_3DCastIDComboxR, pTable->sDLPActiveChannel);		

	tmpStr.Format(_T("%d"), pTable->sDLPPulseTime);
	this->SetDlgItemText(LCB_TR_3D_CAST_PULSE_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sDLPCallbackTime);
	this->SetDlgItemText(LCB_TR_3D_CAST_CALLBACK_TIMEOUT_EDIT, tmpStr);
	
	TLCB_LED_ITEM *pPWM = NULL;
	pPWM = &pTable->sPWM1;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK1, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK1, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT1, tmpStr);

	pPWM = &pTable->sPWM2;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK2, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK2, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT2, tmpStr);
	
	pPWM = &pTable->sPWM3;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK3, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK3, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT3, tmpStr);
	
	pPWM = &pTable->sPWM4;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK4, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK4, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT4, tmpStr);
	
	pPWM = &pTable->sPWM5;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK5, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK5, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT5, tmpStr);
	
	pPWM = &pTable->sPWM6;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK6, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK6, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT6, tmpStr);

	pPWM = &pTable->sPWM7;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK7, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK7, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT7, tmpStr);
	
	pPWM = &pTable->sPWM8;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK8, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK8, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TR_LED_POWER_EDIT8, tmpStr);
	
	if ( LEDChannelCount >= 9 )
	{
		pPWM = &pTable->sPWM9;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK9, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK9, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TR_LED_POWER_EDIT9, tmpStr);
	}

	if ( LEDChannelCount >= 10 )
	{
		pPWM = &pTable->sPWM10;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK10, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK10, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TR_LED_POWER_EDIT10, tmpStr);
	}

	if ( LEDChannelCount >= 11 )
	{
		pPWM = &pTable->sPWM11;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK11, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK11, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TR_LED_POWER_EDIT11, tmpStr);
	}

	if ( LEDChannelCount >= 12 )
	{
		pPWM = &pTable->sPWM12;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK12, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TR_LED_TURN_ON_CHK12, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TR_LED_POWER_EDIT12, tmpStr);
	}

	//pTable->sPWM13;
	//pTable->sPWM14;
	//pTable->sPWM15;
	//pTable->sPWM16;

	if ( LIGHT_CTRL_BOARD_3DA6 == LightCtrlBoardType )
	{	UpdateTableToReadUI_Camera(1);		}
	else
	{	UpdateTableToReadUI_Camera(pTable->sCameraEnable);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateTableToWriteUI(TLCB_TRIG_TABLE &table)
{
	TLCB_TRIG_TABLE *pTable = &table;
	CString tmpStr = _T("");
	const int TableMode = pTable->sTableType;
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();

	this->SetDlgItemInt(LCB_TW_TABLE_INDEX_EDIT, pTable->sTableID+1);	
	JetAPI::SetComboxCurSel(m_TableTypeComboxW, pTable->sTableType);

	tmpStr.Format(_T("%d"), pTable->sNextTableTime);
	this->SetDlgItemText(LCB_TW_NEXT_TABLE_TIME_EDIT, tmpStr);	

	if( TableMode == TABLE_TYPE_LED )
	{ tmpStr.Format(_T("%d"), pTable->sLEDTableTotalTime-pTable->sCCDDelayTime); }
	else if( TableMode == TABLE_TYPE_DLP )
	{ tmpStr.Format(_T("%d"), pTable->sDLPCCDExpTime); }
	else
	{ tmpStr.Format(_T("%d"), 0); }
	this->SetDlgItemText(LCB_TW_CAMERA_EXP_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sCCDDelayTime);
	this->SetDlgItemText(LCB_TW_CAMERA_DELAY_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sDLPTrigOutNumber);
	this->SetDlgItemText(LCB_TW_3D_CAST_TRIG_CONT_EDIT, tmpStr);

	//tmpStr.Format(_T("CH. %d"), pTable->sDLPActiveChannel);	
	JetAPI::SetComboxCurSel(m_3DCastIDComboxW, pTable->sDLPActiveChannel);	

	tmpStr.Format(_T("%d"), pTable->sDLPPulseTime);
	this->SetDlgItemText(LCB_TW_3D_CAST_PULSE_TIME_EDIT, tmpStr);

	tmpStr.Format(_T("%d"), pTable->sDLPCallbackTime);
	this->SetDlgItemText(LCB_TW_3D_CAST_CALLBACK_TIMEOUT_EDIT, tmpStr);
	
	TLCB_LED_ITEM *pPWM = NULL;
	pPWM = &pTable->sPWM1;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK1, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK1, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT1, tmpStr);

	pPWM = &pTable->sPWM2;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK2, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK2, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT2, tmpStr);
	
	pPWM = &pTable->sPWM3;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK3, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK3, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT3, tmpStr);
	
	pPWM = &pTable->sPWM4;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK4, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK4, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT4, tmpStr);
	
	pPWM = &pTable->sPWM5;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK5, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK5, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT5, tmpStr);
	
	pPWM = &pTable->sPWM6;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK6, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK6, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT6, tmpStr);

	pPWM = &pTable->sPWM7;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK7, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK7, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT7, tmpStr);
	
	pPWM = &pTable->sPWM8;
	if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK8, TRUE); }
	else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK8, FALSE); } 
	tmpStr.Format(_T("%2d"), pPWM->sPower);	
	this->SetDlgItemText(LCB_TW_LED_POWER_EDIT8, tmpStr);

	if ( LEDChannelCount >= 9 )
	{
		pPWM = &pTable->sPWM9;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK9, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK9, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TW_LED_POWER_EDIT9, tmpStr);
	}

	if ( LEDChannelCount >= 10 )
	{
		pPWM = &pTable->sPWM10;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK10, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK10, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TW_LED_POWER_EDIT10, tmpStr);
	}

	if ( LEDChannelCount >= 11 )
	{
		pPWM = &pTable->sPWM11;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK11, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK11, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TW_LED_POWER_EDIT11, tmpStr);
	}

	if ( LEDChannelCount >= 12 )
	{
		pPWM = &pTable->sPWM12;
		if ( true == pPWM->sIsON ) { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK12, TRUE); }
		else                       { CWnd::CheckDlgButton(LCB_TW_LED_TURN_ON_CHK12, FALSE); } 
		tmpStr.Format(_T("%2d"), pPWM->sPower);	
		this->SetDlgItemText(LCB_TW_LED_POWER_EDIT12, tmpStr);
	}
	//pTable->sPWM13
	//pTable->sPWM14
	//pTable->sPWM15
	//pTable->sPWM16

	if ( LIGHT_CTRL_BOARD_3DA6 == LightCtrlBoardType )
	{	UpdateTableToWriteUI_Camera(1);	}
	else
	{	UpdateTableToWriteUI_Camera(pTable->sCameraEnable);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnFireTriggerBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }

	if ( CLightCtrlBoardWnd::EnableDLPMask() == false ) 
	{	return ; }

	m_TimerDelayTime = 0;
	m_TriggerTestExecCount = 0;
	const int TriggerTableCount = CWnd::GetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_EDIT);	
	if ( LightCtrlBoard.WriteTableRunCount(TriggerTableCount) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	if( LightCtrlBoard.SetMode_FPGA() == false )	
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	CLightCtrlBoardWnd::UpdateFPGACtrlModeChk();
	CalcTriggerTableEllapsedTime();
	AOIDataCollect.SetCallbackWnd(NULL);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	if ( ExecCameraGrab() == false ) { return; }
	ExecFireTriggerBtn();	
}
//-------------------------------------------------------------------------------------//
HBRUSH CLightCtrlBoardWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnReadAllTableBtn() 
{
	// TODO: Add your control notification handler code here
	CString       str;
	LARGE_INTEGER fnEnd;
	LARGE_INTEGER fnStart;	
	double        fnTime[10]={0.0};
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	
	JetAPI::SetFuncTimeStart(fnStart);
	if ( LightCtrlBoard.TableListRead(m_TableList, MAX_TABLE_COUNT) == false )
	{	JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[0] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s=%.3f ms"), _T("TableListRead"), fnTime[0]);	
	SetTableInfoEdit(str);

	UpdateTableListWnd(m_TableList);
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnClearAllTableBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	if( LightCtrlBoard.TableReset(MAX_TABLE_COUNT) == false )
	{	JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	}

	TLCB_TRIG_TABLE tmpTable;
	LightCtrlBoard.DefaultTable(tmpTable);
	tmpTable.sTableID = 0;	
	CWnd::SetDlgItemInt(LCB_TW_INDEX_EDIT, 1);
	CWnd::SetDlgItemInt(LCB_TR_INDEX_EDIT, 1);
	//UpdateTableToReadUI(tmpTable);
	//UpdateTableToWriteUI(tmpTable);
	OnReadAllTableBtn();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecReadStatusBtn()
{
	LIGHT_CTRL_BOARD_IMP_CLS ImpCls=GetLightCtrlBoardClass();	
	if ( LIGHT_CTRL_BOARD_IMP_FPGA == ImpCls )
	{	return ExecReadStatusBtn_Fpga();	}
	if ( LIGHT_CTRL_BOARD_IMP_ARDUINO == ImpCls )
	{	return ExecReadStatusBtn_Arduino();	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecReadStatusBtn_Fpga()
{
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return false; }

	int Value=0;
	CString strValue;
	if( LightCtrlBoard.ReadErrorString(strValue) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());		
		return false;
	}	

	const size_t StrSize=LIGHT_CTRL_BOARD_TEXT_SIZE;
	char DataRS[StrSize]="";
	const int Len = strValue.GetLength();
	JetAPI::TCHAR2char(strValue, DataRS, StrSize);	
	JetAPI::HexToInt(DataRS, Len, Value);

	this->UpdateFPGACtrlModeChk();	
	
	char    BufferC[64]="";
	wchar_t BufferW[64]=L"";
	JetAPI::IntToBin(Value, 16, BufferC);
	JetAPI::IntToBin(Value, 16, BufferW);
	
	CString ReadData = BufferC;	
	ReadData.Insert(16-4, _T(" "));
	ReadData.Insert(16-8, _T(" "));
	ReadData.Insert(16-12, _T(" "));
	this->SetDlgItemText(LCB_READ_STATUS_EDIT, ReadData);	

	CString ErrorStr = _T("");
	if( (Value&0xff) > 0 )	
	{	LightCtrlBoard.DecodeErrorCodeText(Value, ErrorStr);	}
	CWnd::SetDlgItemText(LCB_ERROR_STRING_EDIT, ErrorStr);	

	bool FPGAOK = false;
	bool Working = false;
	if( LightCtrlBoard.GetIsFPGAOK(FPGAOK) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}	
	if( LightCtrlBoard.GetIsWorkingNow(Working) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}	
	CString tmpStr = _T("");
	if( FPGAOK == true )
	{ tmpStr = _T("FPGA_OK"); }
	else
	{ tmpStr = _T("FPGA_ERROR"); }

	if( Working == true )
	{ tmpStr += _T("(WORKING NOW)"); }
	this->SetDlgItemText(LCB_FPGA_STATUS_EDIT, tmpStr);
	if ( false==FPGAOK || true==Working )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecReadStatusBtn_Arduino()
{
	if ( CheckContrlBoard() == FALSE ) { return false; }

	int Value=0;
	CString strErr;	
	if( LightCtrlBoard.ReadErrorString(strErr) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());		
		return false;
	}	
	CWnd::SetDlgItemText(LCB_READ_STATUS_EDIT, strErr);	
	CWnd::SetDlgItemText(LCB_ERROR_STRING_EDIT, strErr);

	bool FPGAOK = false;
	bool Working = false;
	if( LightCtrlBoard.GetIsFPGAOK(FPGAOK) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}	
	if( LightCtrlBoard.GetIsWorkingNow(Working) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return false;
	}	
	CString tmpStr = _T("");
	if( FPGAOK == true )
	{ tmpStr = _T("Board_OK"); }
	else
	{ tmpStr = _T("Board_ERROR"); }
	CWnd::SetDlgItemText(LCB_READ_STATUS_EDIT, tmpStr);	
	if( Working == true )
	{ tmpStr = _T("WORKING NOW"); }
	else
	{ tmpStr = _T("WORKING FAULT"); }
	this->SetDlgItemText(LCB_FPGA_STATUS_EDIT, tmpStr);
	if ( false==FPGAOK || true==Working )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnReadStatusBtn() 
{
	// TODO: Add your control notification handler code here
	ExecReadStatusBtn();

	bool IsError=false;
	CString ErrorStr;
	LightCtrlBoard.CheckLightCtrlBoardError(IsError, ErrorStr);
	JetAPI::ShowMessageBox(ErrorStr);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateTableListWnd(std::vector<TLCB_TRIG_TABLE> &TableList)
{
	if ( m_TableListWnd.GetSafeHwnd() == NULL ) { return false; }

	bool    Rebuild = false;
	int     i=0, iSubItem=0;
	CString str;
	COLORREF clrBK = 0xD0FFFF;
	TLCB_LED_ITEM   *pLEDCh = NULL;
	TLCB_TRIG_TABLE *pTable = NULL;
	const int TableCount = (int)(TableList.size());
	CListCtrl &ListCtrl = m_TableListWnd;
	const int NItems = ListCtrl.GetItemCount();
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();	

	CalcTableUsedCount(TableList);
	ListCtrl.SetRedraw(FALSE);
	m_StopListItemChanged = true;
	if ( NItems != TableCount )
	{
		Rebuild = true;
		ListCtrl.DeleteAllItems();
	}
	else
	{	Rebuild = false; }
	//ListCtrl.SetBkColor(clrBK);
	ListCtrl.SetTextBkColor(clrBK);
	for ( i=0; i<TableCount; i++ )
	{
		iSubItem=0;
		pTable = &(TableList[i]);

		str.Format(_T("%d"), i+1);
		if ( true == Rebuild )
		{	ListCtrl.InsertItem(i, str); }

		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//Type
		switch ( pTable->sTableType )
		{
		case TABLE_TYPE_LED:	str = _T("LED");	break;
		case TABLE_TYPE_DLP:	str = _T("3D");	break;
		default:	str = _T("None");	break;
		}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//Next Table Between Time
		str.Format(_T("%d"), pTable->sNextTableTime);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//Camera Exposure Time
		if( pTable->sTableType == TABLE_TYPE_LED )
		{ str.Format(_T("%d"), pTable->sLEDTableTotalTime-pTable->sCCDDelayTime); }
		else if( pTable->sTableType == TABLE_TYPE_DLP )
		{ str.Format(_T("%d"), pTable->sDLPCCDExpTime); }
		else
		{ str.Format(_T("%d"), 0); }
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//Camera Delay Time
		str.Format(_T("%d"), pTable->sCCDDelayTime);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//3D Cast ID
		str.Format(_T("%d"), pTable->sDLPActiveChannel);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//3D Cast Trigger Count
		str.Format(_T("%d"), pTable->sDLPTrigOutNumber);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//3D Cast Pulse Time
		str.Format(_T("%d"), pTable->sDLPPulseTime);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//3D Cast Callback Timeout
		str.Format(_T("%d"), pTable->sDLPCallbackTime);
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 1
		pLEDCh = &pTable->sPWM1;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 2
		pLEDCh = &pTable->sPWM2;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 3
		pLEDCh = &pTable->sPWM3;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 4
		pLEDCh = &pTable->sPWM4;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 5
		pLEDCh = &pTable->sPWM5;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 6
		pLEDCh = &pTable->sPWM6;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 7
		pLEDCh = &pTable->sPWM7;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		//LED CH. 8
		pLEDCh = &pTable->sPWM8;
		if( true == pLEDCh->sIsON )
		{	str.Format(_T("%d"), pLEDCh->sPower);	}
		else
		{	str = _T(" ");	}
		ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;

		if ( LEDChannelCount >= 9 )
		{
			//LED CH. 9
			pLEDCh = &pTable->sPWM9;
			if( true == pLEDCh->sIsON )
			{	str.Format(_T("%d"), pLEDCh->sPower);	}
			else
			{	str = _T(" ");	}
			ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;
		}

		if ( LEDChannelCount >= 10 )
		{
			//LED CH.10
			pLEDCh = &pTable->sPWM10;
			if( true == pLEDCh->sIsON )
			{	str.Format(_T("%d"), pLEDCh->sPower);	}
			else
			{	str = _T(" ");	}
			ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;
		}

		if ( LEDChannelCount >= 11 )
		{
			//LED CH.11
			pLEDCh = &pTable->sPWM11;
			if( true == pLEDCh->sIsON )
			{	str.Format(_T("%d"), pLEDCh->sPower);	}
			else
			{	str = _T(" ");	}
			ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;
		}

		if ( LEDChannelCount >= 12 )
		{
			//LED CH.12
			pLEDCh = &pTable->sPWM12;
			if( true == pLEDCh->sIsON )
			{	str.Format(_T("%d"), pLEDCh->sPower);	}
			else
			{	str = _T(" ");	}
			ListCtrl.SetItemText(i, iSubItem, str);	iSubItem++;
		}

		//sPWM13
		//sPWM14
		//sPWM15
		//sPWM16
	}	
	m_StopListItemChanged = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnDblclkTableListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here		
	NMITEMACTIVATE *pItem = (NMITEMACTIVATE*)pNMHDR;
	const int nItem = pItem->iItem;
	const int TableCount = (int)(m_TableList.size());	
	if ( nItem<0 || nItem>=TableCount ) { return; }

	TLCB_TRIG_TABLE tmpTable = m_TableList[nItem];
	const int TableID = this->GetDlgItemInt(LCB_TW_INDEX_EDIT)-1;
	tmpTable.sTableID = TableID;
	CLightCtrlBoardWnd::UpdateTableToWriteUI(tmpTable);
	CLightCtrlBoardWnd::OnTableWriteBtn();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnClickTableListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pItem = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pItem->iItem;
	const int TableCount = (int)(m_TableList.size());	
	if ( nItem<0 || nItem>=TableCount ) { return; }
	TLCB_TRIG_TABLE tmpTable = m_TableList[nItem];
	const int TableID = this->GetDlgItemInt(LCB_TW_INDEX_EDIT)-1;
	tmpTable.sTableID = TableID;
//	CLightCtrlBoardWnd::UpdateTableToWriteUI(tmpTable);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnTWDefaultBtn() 
{
	// TODO: Add your control notification handler code here	
	const int TableID = this->GetDlgItemInt(LCB_TW_INDEX_EDIT)-1;
	TLCB_TRIG_TABLE tmpTable;	
	LightCtrlBoard.DefaultTable(tmpTable);
	tmpTable.sTableID = TableID;
	UpdateTableToWriteUI(tmpTable);
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnAdvancedChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(LCB_ADVANCED_CHK);
	if ( TRUE == bChk )
	{	SwitchToAdvancedUI();	}
	else
	{	SwitchToBasicUI();	}
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::SwitchToBasicUI()//基本UI
{
	RECT rcWnd={0};
	CWnd *Parent = CWnd::GetParent();
	CWnd::GetWindowRect(&rcWnd);
	//if ( NULL!=Parent && Parent->GetSafeHwnd()!=NULL)
	//{	Parent->ScreenToClient(&rcWnd);	}
	const int ccx = rcWnd.right-rcWnd.left;
	const int ccy = rcWnd.bottom-rcWnd.top;
	const int cx = 432;
	const int cy = 680;
	rcWnd.right  = rcWnd.left+cx;
	rcWnd.bottom = rcWnd.top+cy;
	CWnd::MoveWindow(&rcWnd);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::SwitchToAdvancedUI()//進階UI
{
	RECT rcWnd={0};
	CWnd *Parent = CWnd::GetParent();
	CWnd::GetWindowRect(&rcWnd);
	//if ( NULL!=Parent && Parent->GetSafeHwnd()!=NULL)
	//{	Parent->ScreenToClient(&rcWnd);	}
	const int ccx = rcWnd.right-rcWnd.left;
	const int ccy = rcWnd.bottom-rcWnd.top;
	const int cx = 1460;
	const int cy =  680;
	rcWnd.right  = rcWnd.left+cx;
	rcWnd.bottom = rcWnd.top+cy;
	CWnd::MoveWindow(&rcWnd);
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnBuildInspectionTableBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CheckContrlBoard() == FALSE ) { return ; }	
	bool          IsOK=true;
	CString       str;
	LARGE_INTEGER fnEnd;
	LARGE_INTEGER fnStart;	
	double        fnTime[10]={0.0};	
	std::vector<TSliceParam> SliceParamList;
	const bool bDisable3D = AOIDataCollect.GetDisable3D();
	str.Format(_T("CLightCtrlBoardWnd::OnBuildInspectionTableBtn Start"));
	AOIDataCollect.SaveMovingTimeMsg(str);
	AOIDataCollect.CloneSystemSliceParamList(SliceParamList);
	if ( CameraCtrl.SetBatchGrabExpourseTime(SliceParamList) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return;
	}
	m_TableList.clear();
	::memset(fnTime, 0x00, sizeof(fnTime));

	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = LightCtrlBoard.ConvertSliceParamListToLCBTableList(SliceParamList, m_TableList);
	if ( false == IsOK )
	{	
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return;
	}
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime[0] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	/*
	const size_t MaxLocCount=60;
	const size_t TableCount=m_TableList.size();
	if ( TableCount > MaxLocCount )
	{
		size_t  i=0;
		std::vector<TLCB_TRIG_TABLE> TempList;
		for ( i=0; i<MaxLocCount; i++ )
		{	TempList.push_back(m_TableList[i]); }
		m_TableList = TempList;
	}
	*/
	JetAPI::SetFuncTimeStart(fnStart);
	if ( LightCtrlBoard.TableListWrite(m_TableList) == false )
	{	JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	}
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime[1] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

	JetAPI::SetFuncTimeStart(fnStart);
	if ( AOIDataCollect.SetupAllLightSetting() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime[2] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

	if ( false == bDisable3D )
	{
		if ( Light3DCtrl.ExecAllLight3DSequencePlay() == false )
		{	JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString()); }
	}

	bool bTestReadTime = true;
	if ( true == bTestReadTime )
	{
		const int MaxReadCount = 20;
		std::vector<TLCB_TRIG_TABLE> TableList;
		JetAPI::SetFuncTimeStart(fnStart);
		for ( int i=0; i<MaxReadCount; i++ )
		{	LightCtrlBoard.TableListRead(TableList, MAX_TABLE_COUNT); }
		JetAPI::SetFuncTimeStart(fnEnd);
		fnTime[3] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
		fnTime[3] /= MaxReadCount;
	}

	if ( false == bTestReadTime )
	{	str.Format(_T("%s=%.3f ms, %s=%.3f ms, %s=%.3f ms"), _T("BuildLightCtrlBoardTable"), fnTime[0], _T("TableListWrite"), fnTime[1], _T("SetupAllLightSetting"), fnTime[2]);	}
	else
	{	str.Format(_T("%s=%.3f ms, %s=%.3f ms, %s=%.3f ms, %s=%.3f ms"), _T("BuildLightCtrlBoardTable"), fnTime[0], _T("TableListWrite"), fnTime[1], _T("SetupAllLightSetting"), fnTime[2], _T("ReadTable"), fnTime[3]);	}
	SetTableInfoEdit(str);
	str.Format(_T("CLightCtrlBoardWnd::OnBuildInspectionTableBtn End"));
	AOIDataCollect.SaveMovingTimeMsg(str);

	m_UpdateTableInfoEdit = false;
	CLightCtrlBoardWnd::OnReadAllTableBtn();
	m_UpdateTableInfoEdit = true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnItemchangedTableListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopListItemChanged )
	{
		*pResult = 0;
		return;
	}		
	DWORD  Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( NULL == Res )
	{	return; }
	const int nItem =  pNMListView->iItem;
	const int TableCount = (int)(m_TableList.size());
	if ( nItem<0 || nItem>=TableCount ) { return; }
	TLCB_TRIG_TABLE tmpTable = m_TableList[nItem];
	const int TableID = this->GetDlgItemInt(LCB_TW_INDEX_EDIT)-1;
	tmpTable.sTableID = TableID;
	CLightCtrlBoardWnd::UpdateTableToWriteUI(tmpTable);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecCameraGrab()//執行相機取像
{
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	CAMERA_GRAB_MODE GrabMode = CAMERA_GRAB_EXTERNAL_TRIGGER;
	CAMERA_CALLBACK_TIMMING CallbackTimming = CAMERA_CALLBACK_EACH_FRAME;//CAMERA_CALLBACK_BATCH_GRAB_DONE;
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	const int ExposureTime = CaliParam.m_ExposureTime_us;// 6000;
	const int NFrames = -1;
	const int GrabLoop = 1;

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
	if ( CameraCtrl.SetCameraNFramesToGrab(CameraID, NFrames) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	if ( CameraCtrl.SetCameraBatchGrabCount(CameraID, GrabLoop) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}	
	if ( CameraCtrl.StartCameraGrab(CameraID) == false)
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ClearCameraCount(bool ClearTotalCnt)//清除相機取相數量
{
	if ( true == ClearTotalCnt )
	{	m_TotalCnt_CameraReceieve = 0; }
	CameraCtrl.ClearAllCameraCount();//復歸所有相機次數		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::UpdateCameraCount()//更新相機取相數量
{
	CString   str;
	long      count=0;	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	long cntCameraBak=0, cntExpBak=0, cntImageBak=0, cntImageCpy=0;
	BOOL bResetCnt = CWnd::IsDlgButtonChecked(LCB_FIRE_TRIGGER_COUNT_REST_CHK);
	if ( CameraCtrl.GetCameraCount(CameraID, cntCameraBak, cntExpBak, cntImageBak, cntImageCpy) == true )
	{	count = cntCameraBak;	}
	else { count = 0; }	
	if ( TRUE == bResetCnt )
	{	
		m_TotalCnt_CameraReceieve += count; 
		str.Format(_T("%d / %d"), count, m_TotalCnt_CameraReceieve);
	}
	else
	{	
		m_TotalCnt_CameraReceieve = count; 
		str.Format(_T("%d"), count);
	}	
	CWnd::SetDlgItemText(LCB_COUNT_CAMERA_RECEIVE_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::CalcTriggerTableEllapsedTime()//計算觸發表格的經過時間
{	
	size_t  i=0;	
	DWORD   TalbeTime_us=0;
	DWORD   TalbeTotalTime_us=0;
	TLCB_TRIG_TABLE  *pTable = NULL;		
	const DWORD TotalTableCount = (DWORD)(m_TableList.size());
	const DWORD TriggerTableCount = CWnd::GetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_EDIT);
	const DWORD CalcTableCount = MIN(TriggerTableCount, TotalTableCount);

	m_TimerDelayTime = 0;
	TalbeTotalTime_us = 0;
	for ( i=0; i<CalcTableCount; i++ )
	{
		pTable = &(m_TableList[i]);

		switch ( pTable->sTableType )
		{
		case TABLE_TYPE_LED:
			TalbeTime_us = pTable->sLEDTableTotalTime;
			break;
		case TABLE_TYPE_DLP:
			TalbeTime_us = pTable->sCCDDelayTime + pTable->sDLPCCDExpTime + pTable->sDLPPulseTime;
			break;
		default:
			TalbeTime_us = 0;
			break;
		}
		TalbeTime_us += pTable->sNextTableTime;
		TalbeTotalTime_us += TalbeTime_us;
	}
	m_TimerDelayTime = TalbeTotalTime_us/1000;
	int ExtraDelayTime = (int)(CWnd::GetDlgItemInt(LCB_FIRE_TRIGGER_DELAY_TIME_EDIT));
	if ( ExtraDelayTime < 10 ) { ExtraDelayTime = 10; }
	m_TimerDelayTime = m_TimerDelayTime + ExtraDelayTime;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::CalcTableUsedCount(std::vector<TLCB_TRIG_TABLE> &TableList)
{
	size_t i=0;
	int    Count = 0;
	TLCB_TRIG_TABLE Table;
	const size_t TableCount = TableList.size();
	const int LEDChannelCount = LightCtrlBoard.GetLEDChannelCount();
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();

	for ( i=0; i<TableCount; i++ )
	{
		Table=TableList[i];
		if ( LEDChannelCount < 9 )	{	Table.sPWM9.sIsON = false;	}
		if ( LEDChannelCount < 10 )	{	Table.sPWM10.sIsON = false;	}
		if ( LEDChannelCount < 11 )	{	Table.sPWM11.sIsON = false;	}
		if ( LEDChannelCount < 12 )	{	Table.sPWM12.sIsON = false;	}
		if ( LEDChannelCount < 13 )	{	Table.sPWM13.sIsON = false;	}
		if ( LEDChannelCount < 14 )	{	Table.sPWM14.sIsON = false;	}
		if ( LEDChannelCount < 15 )	{	Table.sPWM15.sIsON = false;	}
		if ( LEDChannelCount < 16 )	{	Table.sPWM16.sIsON = false;	}
		switch ( Table.sTableType )
		{
		case TABLE_TYPE_LED:
			if ( true==Table.sPWM1.sIsON || true==Table.sPWM2.sIsON || true==Table.sPWM3.sIsON || true==Table.sPWM4.sIsON ||
				 true==Table.sPWM5.sIsON || true==Table.sPWM6.sIsON || true==Table.sPWM7.sIsON || true==Table.sPWM8.sIsON ||
				 true==Table.sPWM9.sIsON || true==Table.sPWM10.sIsON || true==Table.sPWM11.sIsON || true==Table.sPWM12.sIsON ||
				 true==Table.sPWM13.sIsON || true==Table.sPWM14.sIsON || true==Table.sPWM15.sIsON || true==Table.sPWM16.sIsON )
			{	Count ++;	}
			break;
		case TABLE_TYPE_DLP:
			if ( 0 != Table.sDLPActiveChannel )
			{	Count ++;	}
			break;
		}		
	}
	CWnd::SetDlgItemInt(LCB_TRIGGER_TABLE_COUNT_USED_EDIT, Count);
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnWriteReadTableTestBtn() 
{
	// TODO: Add your control notification handler code here
	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return ; }
	
	CString str;
	if ( LightCtrlBoard.TableListRead(m_TableList, MAX_TABLE_COUNT) == false )
	{	
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	
		return ;
	}	
	std::vector<int>             NGTableList;
	str.Format(_T("%s\\LightCtrlTableTestWrite.TXT"), AOIDataCollect.GetAOITempDirectory());
	LightCtrlBoard.WriteTableListToFile(str, m_TableList, NGTableList);
	::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);

	m_WriteReadTableCount = 1;
	CLightCtrlBoardWnd::UpdateTableListWnd(m_TableList);
	CWnd::CheckDlgButton(LCB_WRITE_READ_TABLE_TEST_CHK, TRUE);
	ExecWriteReadTableBtn();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecWriteReadTableBtn()
{
	CString str;
	if ( CheckContrlBoard() == FALSE ) { return false; }
	CWnd::SetDlgItemInt(LCB_WRITE_READ_TABLE_TEST_EDIT, m_WriteReadTableCount);
	BOOL bWrtie = CWnd::IsDlgButtonChecked(LCB_WRITE_READ_WRITE_TEST_CHK);

	std::vector<int>             NGTableList;
	std::vector<TLCB_TRIG_TABLE> TableListOut;
	std::vector<TLCB_TRIG_TABLE> TableListIn;

	//要轉換DLP Channel
	LightCtrlBoard.ReMapTableList_ReadToWrite(m_TableList, TableListIn);
	if ( TRUE == bWrtie )
	{
		UINT DelayTime = CWnd::GetDlgItemInt(LCB_WRITE_READ_DELAY_TIME_EDIT);		
		if ( LightCtrlBoard.TableListWrite(TableListIn) == false )
		{	
			JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());	
			CLightCtrlBoardWnd::UpdateTableListWnd(m_TableList);
			return false;
		}	
		if ( DelayTime>0 && DelayTime<1000000 ) 
		{	::Sleep(DelayTime); }
	}
	
	
	const int TableMaxCount = CWnd::GetDlgItemInt(LCB_WRITE_READ_COUNT_EDIT);	
	int   TableCountIn = (int)(TableListIn.size());
	if ( TableCountIn > TableMaxCount ) 
	{	TableCountIn = TableMaxCount; }
	if ( LightCtrlBoard.TableListRead(TableListOut, TableCountIn) == false ) 
	{
		str.Format(_T("%s\\LightCtrlTableTestRead.TXT"), AOIDataCollect.GetAOITempDirectory());
		LightCtrlBoard.WriteTableListToFile(str, TableListOut, NGTableList);
		::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);

		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		CLightCtrlBoardWnd::UpdateTableListWnd(m_TableList);
		return false; 
	}	
	
	if ( LightCtrlBoard.CheckTableListEqually(TableListIn, TableListOut, NGTableList) == false )
	{
		str.Format(_T("%s\\LightCtrlTableTestRead.TXT"), AOIDataCollect.GetAOITempDirectory());
		LightCtrlBoard.WriteTableListToFile(str, TableListOut, NGTableList);
		::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);

		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());			
		CLightCtrlBoardWnd::UpdateTableListWnd(m_TableList);
		return false; 
	}	
	CLightCtrlBoardWnd::UpdateTableListWnd(TableListOut);
	if ( CWnd::IsWindowVisible() == FALSE ) 
	{	return true; }
	if ( CWnd::IsDlgButtonChecked(LCB_WRITE_READ_TABLE_TEST_CHK) == FALSE )
	{	return true;	}
	m_WriteReadTableCount ++;
	CWnd::SetTimer(LCB_TIMER_REPEAT_WRITE_READ, 50, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::SetTableInfoEdit(LPCTSTR Info)
{
	if ( NULL == Info ) { return false; }
	if ( false == m_UpdateTableInfoEdit ) { return true; }
	CWnd::SetDlgItemText(LCB_TABLE_INFO_EDIT, Info);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardWnd::ExecTableFirstIdSetBtn()	
{
	CString WndTxt, Title, Default;
	CInputBoxWnd InputWnd;

	WndTxt = _T("Set Ctrl Board Table First Index");
	//WndTxt = LoadMultiLanguageString(WndTxt, WndTxt);

	Title  = _T("Table First Index");
	//Title = LoadMultiLanguageString(Title, Title);
	Default.Format(_T("%d"), m_TableFirstIndex);
	InputWnd.SetParam1(WndTxt, Title, Default);
	if ( InputWnd.DoModal() == IDCANCEL ) { return true; }
	const int TableFirstIndex = ::_ttoi(InputWnd.m_DataEdit1);
	if ( TableFirstIndex < 0 ) { return false; }

	if ( CLightCtrlBoardWnd::CheckContrlBoard() == FALSE ) { return false; }	
	if ( LightCtrlBoard.SetTriggerFirstIndex(TableFirstIndex) == false )
	{	return false;	}	
	m_TableFirstIndex = TableFirstIndex;	
	CWnd::SetDlgItemInt(LCB_TABLE_FIRST_ID_EDIT, m_TableFirstIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardWnd::OnTableFirstIdSetBtn() 
{
	// TODO: Add your control notification handler code here
	ExecTableFirstIdSetBtn();
	::Sleep(100);
	LightCtrlBoard.SwitchToRead();
	UpdateFPGACtrlModeChk();
}
//-------------------------------------------------------------------------------------//