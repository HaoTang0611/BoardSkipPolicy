// MachineStatusWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "MachineStatusWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define MACHINE_STATUS_TIMER_POLLING    101
//-------------------------------------------------------------------------------------//
CMachineStatusWnd MachineStatusWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMachineStatusWnd dialog
//-------------------------------------------------------------------------------------//
CMachineStatusWnd::CMachineStatusWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CMachineStatusWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMachineStatusWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_MachineAlarm = false;	
	m_AutoHideReady = true;
	m_AlarmWndFont = NULL;
	m_AlarmWndFontSizeH = 48;
#ifdef AOI_EXCEPTION_CODE_USE
	m_AlarmWndFontSizeH = 32;
#endif//AOI_EXCEPTION_CODE_USE
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMachineStatusWnd)
	DDX_Control(pDX, MCNSTS_ALARM_WND, m_AlarmWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMachineStatusWnd, CDialog)
	//{{AFX_MSG_MAP(CMachineStatusWnd)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMachineStatusWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CMachineStatusWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	CreateAlamWndFont();	
	SwitchMultiLanguage();
	m_AlarmWnd.GetClientRect(&m_AlarmWndRect);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::CenterWindow(NULL);	
	CWnd::SetTimer(MACHINE_STATUS_TIMER_POLLING, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearAlamWndFont();
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case MACHINE_STATUS_TIMER_POLLING:
		CWnd::KillTimer(nIDEvent);
		ExecPollingMachineStatus();
		CWnd::SetTimer(nIDEvent, 100, NULL);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnOK() 
{
	// TODO: Add extra validation here
	ResetMachineAlarm();
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	ResetMachineAlarm();
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::GetIsPolling() const
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::ResetMachineAlarm()
{	
	ResetInfoText();
	SetMachineAlarm(false);		
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::GetMachineAlarm() const
{
	return m_MachineAlarm;
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::GetAutoHideReady() const
{	
	return m_AutoHideReady;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::SetMachineAlarm(bool Alarm)
{
	m_MachineAlarm = Alarm;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MACHEIN_STATUS_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_MACHEIN_STATUS_WND;
	WndKey = _T("IDD_MACHEIN_STATUS_WND");
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
	/*
	WndID = AAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("BBBBBBBBBBBBBBBBB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
	return;
}
//-------------------------------------------------------------------------------------//
CString CMachineStatusWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MACHEIN_STATUS_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::RedrawWnd()
{
	if ( m_AlarmWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_AlarmWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	DrawAlarmWnd(hDC);
	return;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::ClearAlamWndFont()
{
	if ( NULL == m_AlarmWndFont ) { return; }		
	::DeleteObject(m_AlarmWndFont); 
	m_AlarmWndFont = NULL; 	
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::CreateAlamWndFont()
{
	ClearAlamWndFont();

	LOGFONT LogFont;
	HFONT   hFont = NULL;	
	CString strRatio;
	CString strOnlinestate;
	const int FontSize = m_AlarmWndFontSizeH;		
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));//Calibri
	hFont = ::CreateFontIndirect(&LogFont);
	m_AlarmWndFont = hFont;	
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::DrawAlarmWnd(HDC hDC)
{	
	COLORREF   TextColor = 0xFFFFFF;
	HFONT      hFont = m_AlarmWndFont;
	const bool bAlarm=GetMachineAlarm();		
	if ( true == bAlarm )
	{	TextColor = 0x0000FF;	}
	else
	{	TextColor = 0x00FF00;	}

	POINT       Pt;
	COLORREF    BkClr=0x000000;
	CString     Text=m_InfoText; 
	const RECT &Rect=m_AlarmWndRect;
	HBRUSH      hBrush = ::CreateSolidBrush(BkClr);
	HFONT       hOldFont = (HFONT)::SelectObject(hDC, hFont);
	COLORREF    OldBkClr = ::SetBkColor(hDC, BkClr);
	COLORREF    OldTextClr = ::SetTextColor(hDC, TextColor);	
	const int   TextLen = Text.GetLength();
	const int   TextWidth=TextLen*m_AlarmWndFontSizeH;
	//const int BkMode = ::SetBkMode(hDC, TRANSPARENT);

	Pt.x = (Rect.left+Rect.right-TextWidth)/2;
	Pt.y = (Rect.top+Rect.bottom-m_AlarmWndFontSizeH)/2;
	Pt.x = MAX(0, Pt.x);
	Pt.y = MAX(0, Pt.y);

	::FillRect(hDC, &Rect, hBrush);
	::TextOut(hDC, Pt.x, Pt.y, Text, TextLen);	
	
	::SetBkColor(hDC, OldBkClr);
	::SetTextColor(hDC, OldTextClr);	
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hBrush); hBrush=NULL;
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::CheckMachineReady()
{
	m_ErrorString = _T("");
	m_AOIExceptionCode = AOI_EXCEPTION_NONE;
	const bool bSucc = CheckMachineReadyFn();
	if ( false == bSucc )
	{
		m_AOIExceptionCode = AOIExceptionCodeCtrl.GetAOILastExceptionCode();
		return false;
	}
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::CheckMachineReadyFn()
{
	const bool bAutoReset = false;
	const bool bChkStartLight = false;		
	const bool bChkMotionEnb = false;
#ifndef CAMERA_OBJ_DISABLE
	if ( CameraCtrl.GetCameraCtrlReady() == false )
	{
		m_ErrorString = CameraCtrl.GetErrorString();		
		return false;	
	}
#endif//CAMERA_OBJ_DISABLE

#ifndef MOTION_OBJ_DISABLE
	if ( MotionCtrlPtr->CheckMotionReady(bChkMotionEnb) == false )
	{
		m_ErrorString = MotionCtrlPtr->GetErrorString();
		return false;	
	}
#endif//MOTION_OBJ_DISABLE

#ifndef PLC_OBJ_DISABLE
	if ( PlcCtrlPtr->CheckPLCReady(bChkStartLight, bAutoReset) == false )
	{
		m_ErrorString = PlcCtrlPtr->GetPLCErrorString();
		return false;	
	}
#endif//PLC_OBJ_DISABLE

#ifndef LIGHT_CTRL_DISABLE
	if ( LightCtrlBoard.CheckIsConnected() == false )
	{
		m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
#endif//LIGHT_CTRL_DISABLE

#ifndef PHASE_CTRL_DISABLE
	if ( Light3DCtrl.CheckLight3DCastIsConnected() == false )
	{
		m_ErrorString = Light3DCtrl.GetErrorString();
		return false;
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::ResetInfoText()
{		
	SetInfoText(_T("OK"));	
}
//-------------------------------------------------------------------------------------//
void CMachineStatusWnd::SetInfoText(LPCTSTR Text)
{
	if ( m_InfoText.CompareNoCase(Text) == 0 )
	{	return; }
	m_InfoText = Text;
	CWnd::SetDlgItemText(MCNSTS_INFO_EDIT, Text);
}
//-------------------------------------------------------------------------------------//
bool CMachineStatusWnd::ExecPollingMachineStatus()
{
	const bool bAlarm=GetMachineAlarm();
	const bool bIsPolling=GetIsPolling();
	if ( false == bIsPolling ) { return true; }	
	const bool bMachineReady = CheckMachineReady();	
	if ( true == bMachineReady ) 
	{
		if ( true == bAlarm )
		{
			ResetMachineAlarm();
			CWnd::Invalidate();			
			AOIExceptionCodeCtrl.ResetAOIExceptionCode();
			AOIDataCollect.ExecMESComm_ProcessID(MES_STATAUS_AOI_ALARM_CLEAR);//執行MES溝通-程序運作

			if ( GetAutoHideReady() )
			{
				RedrawWnd();
				::Sleep(500);
				CWnd::ShowWindow(SW_HIDE);	
			}
		}
		return true; 
	}	
	
	SetMachineAlarm(true);
	CString ShowText = m_ErrorString;
#ifdef AOI_EXCEPTION_CODE_USE
	ShowText.Format(_T("%s[Code:%05d]"), m_ErrorString, m_AOIExceptionCode);
#endif//AOI_EXCEPTION_CODE_USE
	if ( CWnd::IsWindowVisible() == FALSE )
	{		
		CWnd::ShowWindow(SW_SHOW);
		CWnd::BringWindowToTop();		
	}
	if ( false==bAlarm || m_InfoText.CompareNoCase(ShowText)!=0 )
	{		
		SetInfoText(ShowText);
		CWnd::Invalidate();		
		AOIDataCollect.ExecMESComm_SetAOIExceptionCode(ShowText);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//

