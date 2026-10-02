// PlcTowerLightPane.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "PlcTowerLightPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneTowerLight dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlPaneTowerLight::CPLCCtrlPaneTowerLight(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlPaneTowerLight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlPaneTowerLight)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlPaneTowerLight)
	DDX_Control(pDX, PLCTOWER_STATE_PCB_BACK_COMBO, m_StatePCBBackCombox);
	DDX_Control(pDX, PLCTOWER_STATE_PCB_OUT_COMBO, m_StatePCBOutCombox);
	DDX_Control(pDX, PLCTOWER_STATE_PCB_IN_COMBO, m_StatePCBInCombox);
	DDX_Control(pDX, PLCTOWER_STATE_WAIT_NEXT_COMBO, m_StateWaitNextCombox);
	DDX_Control(pDX, PLCTOWER_STATE_WAIT_LAST_COMBO, m_StateWaitLastCombox);
	DDX_Control(pDX, PLCTOWER_STATE_BYPASS_COMBO, m_StateBypassCombox);
	DDX_Control(pDX, PLCTOWER_STATE_INSPECTION_COMBO, m_StateInspectionCombox);
	DDX_Control(pDX, PLCTOWER_STATE_STOP_COMBO, m_StateStopCombox);
	DDX_Control(pDX, PLCTOWER_TOWER_LIGHT_MODE_COMBO, m_TowerLightModeCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlPaneTowerLight, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlPaneTowerLight)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(PLCTOWER_TOWER_LIGHT_MODE_COMBO, OnSelchangeTowerLightModeCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_STOP_COMBO, OnSelchangeStateStopCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_INSPECTION_COMBO, OnSelchangeStateInspectionCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_BYPASS_COMBO, OnSelchangeStateBypassCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_WAIT_LAST_COMBO, OnSelchangeStateWaitLastCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_WAIT_NEXT_COMBO, OnSelchangeStateWaitNextCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_PCB_IN_COMBO, OnSelchangeStatePCBInCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_PCB_OUT_COMBO, OnSelchangeStatePCBOutCombo)
	ON_CBN_SELCHANGE(PLCTOWER_STATE_PCB_BACK_COMBO, OnSelchangeStatePCBBackCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneTowerLight message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneTowerLight::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CPLC_Basic::BuildTowerLightModeCombox(m_TowerLightModeCombox);	
	CPLC_Basic::BuildTowerLightStateCombox(m_StateStopCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StateInspectionCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StateBypassCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StateWaitLastCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StateWaitNextCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StatePCBInCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StatePCBOutCombox);
	CPLC_Basic::BuildTowerLightStateCombox(m_StatePCBBackCombox);
	
	SwitchMultiLanguage();
	UpdateLightTowerStateToUI();		
	ExecEnableStateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		SwitchMultiLanguage();
		UpdateLightTowerStateToUI();	
		ExecEnableStateUI();
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_TOWER_LIGHT_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_TOWER_LIGHT_PANE;
	WndKey = _T("IDD_PLC_TOWER_LIGHT_PANE");
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
	WndID = PLCTOWER_TOWER_LIGHT_MODE_LABEL;
	WndKey = _T("PLCTOWER_TOWER_LIGHT_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_GROUP;
	WndKey = _T("PLCTOWER_STATE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCTOWER_STATE_STOP_LABEL;
	WndKey = _T("PLCTOWER_STATE_STOP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_INSPECTION_LABEL;
	WndKey = _T("PLCTOWER_STATE_INSPECTION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_BYPASS_LABEL;
	WndKey = _T("PLCTOWER_STATE_BYPASS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_WAIT_LAST_LABEL;
	WndKey = _T("PLCTOWER_STATE_WAIT_LAST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_WAIT_NEXT_LABEL;
	WndKey = _T("PLCTOWER_STATE_WAIT_NEXT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_PCB_IN_LABEL;
	WndKey = _T("PLCTOWER_STATE_PCB_IN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCTOWER_STATE_PCB_OUT_LABEL;
	WndKey = _T("PLCTOWER_STATE_PCB_OUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCTOWER_STATE_PCB_BACK_LABEL;
	WndKey = _T("PLCTOWER_STATE_PCB_BACK_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneTowerLight::UpdateLightTowerStateToUI()
{
	int Value = 0;	
	Value = PlcCtrlPtr->GetTowerLightMode();
	JetAPI::SetComboxCurSel(m_TowerLightModeCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_Stop();
	JetAPI::SetComboxCurSel(m_StateStopCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_Inspection();
	JetAPI::SetComboxCurSel(m_StateInspectionCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_Bypass();
	JetAPI::SetComboxCurSel(m_StateBypassCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_WaitLast();
	JetAPI::SetComboxCurSel(m_StateWaitLastCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_WaitNext();
	JetAPI::SetComboxCurSel(m_StateWaitNextCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_PCBIn();
	JetAPI::SetComboxCurSel(m_StatePCBInCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_PCBOut();
	JetAPI::SetComboxCurSel(m_StatePCBOutCombox, Value);

	Value = PlcCtrlPtr->GetTowerLightState_PCBBack();
	JetAPI::SetComboxCurSel(m_StatePCBBackCombox, Value);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneTowerLight::ExecEnableStateUI()//±Ò¥Îª¬ºA¤¶­±
{
	int Value = 0;	
	bool Enable = false;
	Value = PlcCtrlPtr->GetTowerLightMode();
	if ( PLC_TOWER_LIGHT_USER_DEFINE == Value )
	{	Enable = true;	}
	else 
	{	Enable = false;	}
	return ExecEnableStateUI(Enable);
}
//-------------------------------------------------------------------------------------//
bool CPLCCtrlPaneTowerLight::ExecEnableStateUI(bool Enable)//±Ò¥Îª¬ºA¤¶­±
{
	UINT CtrlID=0;
	BOOL bEnable = Enable;

	CtrlID = PLCTOWER_STATE_STOP_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_INSPECTION_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_BYPASS_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_WAIT_LAST_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_WAIT_NEXT_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_PCB_IN_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_PCB_OUT_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = PLCTOWER_STATE_PCB_BACK_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeTowerLightModeCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_TowerLightModeCombox));
	if ( PlcCtrlPtr->WriteTowerLightMode(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightMode();
		JetAPI::SetComboxCurSel(m_TowerLightModeCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
	ExecEnableStateUI();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStateStopCombo() 
{
	// TODO: Add your control notification handler code here
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StateStopCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_Stop(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_Stop();
		JetAPI::SetComboxCurSel(m_StateStopCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStateInspectionCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StateInspectionCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_Inspection(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_Inspection();
		JetAPI::SetComboxCurSel(m_StateInspectionCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStateBypassCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StateBypassCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_Bypass(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_Bypass();
		JetAPI::SetComboxCurSel(m_StateBypassCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStateWaitLastCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StateWaitLastCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_WaitLast(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_WaitLast();
		JetAPI::SetComboxCurSel(m_StateWaitLastCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStateWaitNextCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StateWaitNextCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_WaitNext(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_WaitNext();
		JetAPI::SetComboxCurSel(m_StateWaitNextCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStatePCBInCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StatePCBInCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_PCBIn(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_PCBIn();
		JetAPI::SetComboxCurSel(m_StatePCBInCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStatePCBOutCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StatePCBOutCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_PCBOut(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_PCBOut();
		JetAPI::SetComboxCurSel(m_StatePCBOutCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneTowerLight::OnSelchangeStatePCBBackCombo() 
{
	// TODO: Add your control notification handler code here	
	int Value = (int)(JetAPI::GetComboxCurSelData(m_StatePCBBackCombox));	
	if ( PlcCtrlPtr->WriteTowerLightState_PCBBack(Value) == false )
	{
		Value = PlcCtrlPtr->GetTowerLightState_PCBBack();
		JetAPI::SetComboxCurSel(m_StatePCBBackCombox, Value);
		JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());
		return;
	}
}
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneTowerLight::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:			
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//