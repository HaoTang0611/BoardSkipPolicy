// InputDateTimeWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "InputDateTimeWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define DEFAULT_YEAR          2016
#define DEFAULT_MONTH           06 
#define DEFAULT_DAY             01
#define DEFAULT_HOUR            12//24
#define DEFAULT_MINUTE          00
#define DEFAULT_SECOND          00
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputDateTimeWnd dialog
//-------------------------------------------------------------------------------------//
CInputDateTimeWnd::CInputDateTimeWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CInputDateTimeWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDateTimeWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Date=CTime::GetCurrentTime();
	m_Time=CTime::GetCurrentTime();	
	m_DateTimeMode=INPUT_DATE_TIME_BOTH;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDateTimeWnd)
	DDX_Control(pDX, INPUTBOX_TIME_CTRL, m_TimeCtrl);
	DDX_Control(pDX, INPUTBOX_DATE_CTRL, m_DateCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CInputDateTimeWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CInputDateTimeWnd)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputDateTimeWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CInputDateTimeWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchDateTimeMode();
	SwitchMultiLanguage();	
	UpdateWndText();	
	
	const bool bNone=m_EnabledNone;	
	ModifyDateTimeShowNone(m_DateCtrl, bNone, INPUTBOX_DATE_CTRL);
	ModifyDateTimeShowNone(m_TimeCtrl, bNone, INPUTBOX_TIME_CTRL);
	
	if ( 0 == m_Date.GetTime() )
	{	m_DateCtrl.SetTime();	}
	else
	{	m_DateCtrl.SetTime(&m_Date);	}
	if ( 0 == m_Time.GetTime() )
	{	m_TimeCtrl.SetTime();	}
	else
	{	m_TimeCtrl.SetTime(&m_Time);	}			

	//change to 24 hours
	m_TimeCtrl.SetFormat(_T("HH:mm:ss"));
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
	return;
}
//-------------------------------------------------------------------------------------//
const CTime& CInputDateTimeWnd::GetDate() const
{
	return m_Date;
}
//-------------------------------------------------------------------------------------//
const CTime& CInputDateTimeWnd::GetTime() const
{
	return m_Time;
}
//-------------------------------------------------------------------------------------//
const CTime&  CInputDateTimeWnd::GetResult() const
{
	INPUT_DATE_TIME_MODE DateTimeMode=m_DateTimeMode;
	if ( INPUT_DATE_TIME_DATE == DateTimeMode )
	{	return GetDate(); }
	if ( INPUT_DATE_TIME_TIME == DateTimeMode )
	{	return GetTime(); }
	return GetDateTime();
}
//-------------------------------------------------------------------------------------//
const CTime& CInputDateTimeWnd::GetDateTime() const
{
	return m_DateTime;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetDate(const CTime &Date, LPCTSTR WndText, bool bNone)
{	
	m_Date=Date;
	m_Time=CTime(0);		
	SetWndText(WndText);
	SetEnableNone(bNone);
	SetDateTimeMode(INPUT_DATE_TIME_DATE);
	CombineDateTime(m_Date, m_Time, m_DateTime);
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetTime(const CTime &Time, LPCTSTR WndText, bool bNone)
{	
	m_Time=Time;
	m_Date=CTime(0);	
	SetWndText(WndText);
	SetEnableNone(bNone);
	SetDateTimeMode(INPUT_DATE_TIME_TIME);		
	CombineDateTime(m_Date, m_Time, m_DateTime);
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetDateTime(const CTime &DateTime, LPCTSTR WndText, bool bNone)
{
	SetDateTime(DateTime, DateTime, WndText, bNone);
	return;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetDateTime(const CTime &Date, const CTime &Time, LPCTSTR WndText, bool bNone)
{	
	m_Date=Date;
	m_Time=Time;	
	SetWndText(WndText);	
	SetEnableNone(bNone);
	SetDateTimeMode(INPUT_DATE_TIME_BOTH);	
	CombineDateTime(m_Date, m_Time, m_DateTime);
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::UpdateWndText()
{
	if ( 0 == m_WndText.GetLength() ) { return; }
	CString str;
	CString WndText;
	CWnd::GetWindowText(WndText);
	str.Format(_T("%s - %s"), WndText, m_WndText);	
	CWnd::SetWindowText(str);
	return;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SwitchDateTimeMode()//ち传ら戳丁家Α
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_DateCtrl.GetSafeHwnd() == NULL ) { return; }
	if ( m_TimeCtrl.GetSafeHwnd() == NULL ) { return; }
	INPUT_DATE_TIME_MODE  DateTimeMode=m_DateTimeMode;
	if ( INPUT_DATE_TIME_DATE == DateTimeMode )
	{
		JetAPI::ShowCtrlWnd(this, INPUTBOX_TIME_CTRL, FALSE);
		JetAPI::ShowCtrlWnd(this, INPUTBOX_TIME_LABEL, FALSE);		
	}
	if ( INPUT_DATE_TIME_TIME == DateTimeMode )
	{
		RECT rcDate, rcTime;
		POINT Offset={0, 0};
		m_DateCtrl.GetWindowRect(&rcDate);
		m_TimeCtrl.GetWindowRect(&rcTime);
		Offset.x = rcDate.left-rcTime.left;
		Offset.y = rcDate.top-rcTime.top;
		JetAPI::ShowCtrlWnd(this, INPUTBOX_DATE_CTRL, FALSE);
		JetAPI::ShowCtrlWnd(this, INPUTBOX_DATE_LABEL, FALSE);
		JetAPI::MoveCtrlWnd(this, INPUTBOX_TIME_CTRL, Offset);
		JetAPI::MoveCtrlWnd(this, INPUTBOX_TIME_LABEL, Offset);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CInputDateTimeWnd::ModifyDateTimeShowNone(CDateTimeCtrl &Ctrl, bool bNone, UINT ID)
{
	if ( Ctrl.GetSafeHwnd() == NULL ) { return false; }
	//[DTS_UPDOWN],[DTS_SHOWNONE]ミ碞ぃ跑
	
	DWORD Flag=DTS_SHOWNONE;
	DWORD Style=Ctrl.GetStyle();
	DWORD Res=Style&Flag;
	if ( true==bNone && 0!= Res)
	{	return true; }
	if ( false==bNone && 0== Res)
	{	return true; }	
	if ( bNone )
	{	Style |= Flag; }
	else
	{	Style &= ~(Flag); }
	
	RECT Rect;
	Ctrl.GetWindowRect(&Rect);
	CWnd::ScreenToClient(&Rect);
	Ctrl.DestroyWindow();
	if ( Ctrl.Create(Style, Rect, this, ID) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_INPUT_DATE_TIME_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(INPUTBOX_DATE_LABEL));
	SetMultiLanauage(LoadIDAndName(INPUTBOX_TIME_LABEL));
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CInputDateTimeWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_INPUT_DATE_TIME_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetEnableNone(bool bNone)
{
	m_EnabledNone = bNone;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetWndText(LPCTSTR WndText)
{
	m_WndText = WndText;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetupDate(const CTime &Date)
{
	//CTime(0)1970/01/01::08:00:00, ┮眏计
	//if ( Date.GetTime() != 0 ) { return ; }	
	const int nYear   = Date.GetYear();
	const int nMonth  = Date.GetMonth();
	const int nDay    = Date.GetDay();
	const int nHour   = DEFAULT_HOUR;//Date.GetHour();
	const int nMinute = DEFAULT_MINUTE;//Date.GetMinute();
	const int nSecond = DEFAULT_SECOND;//Date.GetSecond();
	m_Date=CTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	return ;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetupTime(const CTime &Time)
{
	//CTime(0)1970/01/01::08:00:00, ┮眏计
	//if ( Time.GetTime() != 0 ) { return ; }	
	const int nYear   = DEFAULT_YEAR;//Time.GetYear();
	const int nMonth  = DEFAULT_MONTH;//Time.GetMonth();
	const int nDay    = DEFAULT_DAY;//Time.GetDay();
	const int nHour   = Time.GetHour();
	const int nMinute = Time.GetMinute();
	const int nSecond = Time.GetSecond();
	m_Time=CTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	return ;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::SetDateTimeMode(INPUT_DATE_TIME_MODE Mode)
{
	m_DateTimeMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CInputDateTimeWnd::CombineDateTime(const CTime &Date, const CTime &Time, CTime &DateTime)
{
	const int nYear=Date.GetYear();
	const int nMonth=Date.GetMonth();
	const int nDay=Date.GetDay();
	const int nHour=Time.GetHour();
	const int nMinute=Time.GetMinute();
	const int nSecond=Time.GetSecond();
	DateTime = CTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	return true;
}
//-------------------------------------------------------------------------------------//
void CInputDateTimeWnd::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_DateCtrl.GetTime(m_Date) == GDT_NONE )
	{	m_Date = CTime(0);		}
	else
	{	SetupDate(m_Date);		}
	if ( m_TimeCtrl.GetTime(m_Time) == GDT_NONE )
	{	m_Time = CTime(0);		}
	else
	{	SetupTime(m_Time);		}
	CombineDateTime(m_Date, m_Time, m_DateTime);
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
