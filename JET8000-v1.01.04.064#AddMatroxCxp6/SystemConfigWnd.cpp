// SystemConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemConfigWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemConfigWnd dialog
//-------------------------------------------------------------------------------------//
CSystemConfigWnd::CSystemConfigWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSystemConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemConfigWnd)
	DDX_Control(pDX, SYSTEMCONFIG_PANE_TAB_WND, m_PaneTabWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemConfigWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CSystemConfigWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, SYSTEMCONFIG_PANE_TAB_WND, OnSelchangePaneTabWnd)
	ON_BN_CLICKED(SYSTEMCONFIG_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(SYSTEMCONFIG_LOAD_BTN, OnLoadBtn)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemConfigWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//---------------------------------------------------------------------------------//		
	if ( InitPanelWnd() == false ) 
	{	return FALSE; }	
	AdjustPaneWndPosition();
	//---------------------------------------------------------------------------------//	
	SwitchMultiLanguage();
	ExecSelchangePaneTabWnd();
	//---------------------------------------------------------------------------------//	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_CONFIG_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_CONFIG_WND;
	WndKey = _T("IDD_SYSTEM_CONFIG_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	m_WndText = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = SYSTEMCONFIG_SAVE_BTN;
	WndKey = _T("SYSTEMCONFIG_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = SYSTEMCONFIG_LOAD_BTN;
	WndKey = _T("SYSTEMCONFIG_LOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CSystemConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CSystemConfigWnd::InitPanelWnd()
{
	CString str;	
	bool    bShowLogParam=false;
	bool    bShowThreadParam=false;
	bool    bShowDebugParam=false;	
	const bool bShowPaneParam = false;
	TSystemParameter *Ptr = &m_SysParam;
	USER_LEVEL_MODE  UserLevelMode = AOIDataCollect.GetCurrentUserLevel();//使用者權限
	
	if ( UserLevelMode>USER_LEVEL_ENGINEER )
	{	
		bShowLogParam = true;
		bShowThreadParam = true; 
		bShowDebugParam = true;
	}
	else
	{	
		bShowLogParam = false;
		bShowThreadParam = false; 
		bShowDebugParam = false;
	}
#ifdef _DEBUG
	bShowThreadParam = true;
	bShowDebugParam = true;
#endif//_DEBUG

	SetPaneSystemParameter();
	if ( m_PaneBasic.Create(IDD_SYSTEM_BASIC_PANE, this) == FALSE ) { return false; }
	if ( m_PaneFolder.Create(IDD_SYSTEM_FOLDER_PANE, this) == FALSE ) { return false; }
	if ( m_PaneDefault.Create(IDD_SYSTEM_DEFAULT_PANE, this) == FALSE ) { return false; }		
	if ( m_PaneColor.Create(IDD_SYSTEM_COLOR_PANE, this) == FALSE ) { return false; }			
if ( true == bShowLogParam )
{
	if ( m_PaneLog.Create(IDD_SYSTEM_LOG_PANE, this) == FALSE ) { return false; }
}

#ifdef AI_MODEL_USE
	if ( m_PaneAI.Create(IDD_SYSTEM_AI_PANE, this) == FALSE ) { return false; }	
#endif//AI_MODEL_USE

	if ( m_PaneOnline.Create(IDD_SYSTEM_ONLINE_PANE, this) == FALSE ) { return false; }	

#ifdef OFFLINE_VERSION
	if ( m_PaneOffline.Create(IDD_SYSTEM_OFFLINE_PANE, this) == FALSE ) { return false; }			
#endif//OFFLINE_VERSION	

	if ( m_PaneAdvance.Create(IDD_SYSTEM_ADVANCE_PANE, this) == FALSE ) { return false; }

#ifndef OFFLINE_VERSION
	if ( m_PaneAuthorization.Create(IDD_SYSTEM_AUTHORIZATION_PANE, this) == FALSE ) { return false; }	
#endif//OFFLINE_VERSION

#ifndef MES_DISABLE
	if ( m_PaneMES.Create(IDD_SYSTEM_MES_PANE, this) == FALSE ) { return false; }	
#endif//MES_DISABLE

#ifndef M2M_DISABLE
	if ( m_PaneM2M.Create(IDD_SYSTEM_M2M_PANE, this) == FALSE ) { return false; }
#endif//M2M_DISABLE

#ifndef HASI_DISABLE
	if (m_PaneHASI.Create(IDD_SYSTEM_HASI_PANE, this) == FALSE) { return false; }
#endif //HASI_DISABLE

if ( true == bShowThreadParam )
{
	if ( m_PaneThread.Create(IDD_SYSTEM_THREAD_PANE, this) == FALSE ) { return false; }
}

if ( true == bShowDebugParam )
{
	if ( m_PaneDebug.Create(IDD_SYSTEM_DEBUG_PANE, this) == FALSE ) { return false; }
}
	
if ( true == bShowPaneParam )
{
	if ( m_PaneParam.Create(IDD_SYSTEM_PARAM_PANE, this) == FALSE ) { return false; }
}

	int     tcIndex=0;	
	AddPaneTabWnd(_T("Basic"), m_PaneBasic, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Folder"), m_PaneFolder, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Default"), m_PaneDefault, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Color"), m_PaneColor, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Log"), m_PaneLog, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("AI"), m_PaneAI, m_PaneTabWnd, tcIndex);	
	AddPaneTabWnd(_T("Online"), m_PaneOnline, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Offline"), m_PaneOffline, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Advance"), m_PaneAdvance, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Authorization"), m_PaneAuthorization, m_PaneTabWnd, tcIndex);		
	AddPaneTabWnd(_T("MES"), m_PaneMES, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("M2M"), m_PaneM2M, m_PaneTabWnd, tcIndex);	
	AddPaneTabWnd(_T("HAS I"), m_PaneHASI, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Thread"), m_PaneThread, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Debug"), m_PaneDebug, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Param"), m_PaneParam, m_PaneTabWnd, tcIndex);		
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemConfigWnd::AddPaneTabWnd(LPCTSTR Title, CWnd &Wnd, CTabCtrl &TabWnd, int &Index)
{
	if ( Wnd.GetSafeHwnd() == NULL ) { return true; }

	CString str=Title;		
	TCHAR   tcBuffer[MAX_JET_PATH]=_T("");

	str = LoadMultiLanguageString(str, str);
	::_tcscpy(tcBuffer, str);	

	TCITEM  tcItem;
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = tcBuffer;
	tcItem.lParam = (LPARAM)(&Wnd);
	TabWnd.InsertItem(Index, &tcItem);	
	Index ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemConfigWnd::SetPaneSystemParameter()
{	
	m_PaneBasic.SetSystemParameterPtr(&m_SysParam);
	m_PaneFolder.SetSystemParameterPtr(&m_SysParam);
	m_PaneDefault.SetSystemParameterPtr(&m_SysParam);	
	m_PaneColor.SetSystemParameterPtr(&m_SysParam);		
	m_PaneLog.SetSystemParameterPtr(&m_SysParam);		
	m_PaneAI.SetSystemParameterPtr(&m_SysParam);
	m_PaneOnline.SetSystemParameterPtr(&m_SysParam);
	m_PaneOffline.SetSystemParameterPtr(&m_SysParam);
	m_PaneAdvance.SetSystemParameterPtr(&m_SysParam);
	m_PaneAuthorization.SetSystemParameterPtr(&m_SysParam);	
	m_PaneMES.SetSystemParameterPtr(&m_SysParam);
	m_PaneM2M.SetSystemParameterPtr(&m_SysParam);
	m_PaneThread.SetSystemParameterPtr(&m_SysParam);
	m_PaneDebug.SetSystemParameterPtr(&m_SysParam);
	m_PaneParam.SetSystemParameterPtr(&m_SysParam);
	m_PaneHASI.SetSystemParameterPtr(&m_SysParam);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::AdjustPaneWndPosition()
{
	if ( m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	CWnd  *pWnd = NULL;
	SIZE WndSize={0};
	RECT WndRect={0};
	RECT MainWndRect={0};
	RECT PaneWndRect={0};
	const int MarginR=8;
	CWnd::GetClientRect(&MainWndRect);
	m_PaneTabWnd.GetWindowRect(&PaneWndRect);	
	ScreenToClient(&PaneWndRect);

	pWnd = CWnd::GetDlgItem(IDOK);
	if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = MainWndRect.right-MarginR;
		WndRect.left = WndRect.right-WndSize.cx;
		pWnd->MoveWindow(&WndRect);
		PaneWndRect.right  = WndRect.left-MarginR;
	}
	pWnd = CWnd::GetDlgItem(IDCANCEL);
	if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = MainWndRect.right-MarginR;
		WndRect.left = WndRect.right-WndSize.cx;
		pWnd->MoveWindow(&WndRect);
		PaneWndRect.right  = WndRect.left-MarginR;
	}
	pWnd = CWnd::GetDlgItem(SYSTEMCONFIG_SAVE_BTN);
	if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = MainWndRect.right-MarginR;
		WndRect.left = WndRect.right-WndSize.cx;
		pWnd->MoveWindow(&WndRect);
		PaneWndRect.right  = WndRect.left-MarginR;
	}

	pWnd = CWnd::GetDlgItem(SYSTEMCONFIG_LOAD_BTN);
	if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = MainWndRect.right-MarginR;
		WndRect.left = WndRect.right-WndSize.cx;
		pWnd->MoveWindow(&WndRect);
		PaneWndRect.right  = WndRect.left-MarginR;
	}
	//PaneWndRect.top    = MainWndRect.top + 36; 
	//PaneWndRect.left   = MainWndRect.left + 4;	
	PaneWndRect.bottom = MainWndRect.bottom-4;
	m_PaneTabWnd.MoveWindow(&PaneWndRect);	

	PaneWndRect.top    += 24;
	PaneWndRect.left   += 4;
	PaneWndRect.right  -= 4;
	PaneWndRect.bottom -= 4;

	int    i = 0;	
	TCITEM tcItem;
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;
	const int TabCount = this->m_PaneTabWnd.GetItemCount();
	//Adjust Pane Rect
	for ( i=0; i<TabCount; i++ )
	{	
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { continue; }
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->MoveWindow(&PaneWndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::ExecSelchangePaneTabWnd()
{
	int    i = 0;
	CWnd  *pWnd = NULL;
	CString WndText;
	TCITEM tcItem;
	TCHAR  tcBuffer[MAX_JET_PATH]=_T("");	
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = tcBuffer;
	tcItem.cchTextMax = MAX_JET_PATH;
	const int TabCount = this->m_PaneTabWnd.GetItemCount();
	const int TabIndex = this->m_PaneTabWnd.GetCurSel();

	//Hide Pane 
	for ( i=0; i<TabCount; i++ )
	{		
		if ( i == TabIndex ) { continue; }
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { continue; }
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->ShowWindow(SW_HIDE);
	}
	
	if ( TabIndex >= 0 )
	{
		this->m_PaneTabWnd.GetItem(TabIndex, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { return; }
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
		if ( m_WndText.GetLength() > 0 )
		{	WndText.Format(_T("%s-%s"), m_WndText, tcItem.pszText); }
		else
		{	WndText = tcItem.pszText; }
		CWnd::SetWindowText(WndText);
		pWnd->ShowWindow(SW_SHOW);		
	}
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	this->ExecSelchangePaneTabWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here	
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

#ifndef HASI_DISABLE
	bool bM2M_HASI_Enable = AOIDataCollect.GetHASI_Enable();
#endif //HASI_DISABLE

	AOIDataCollect.SetSystemParameter(m_SysParam);	
	if ( AOIDataCollect.UpdateSystemParameter() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}

#ifndef HASI_DISABLE
	if (bM2M_HASI_Enable != AOIDataCollect.GetHASI_Enable()) {
		AOIDataCollect.ExecHASI_SaveStateFile();
		if (true == AOIDataCollect.GetHASI_Enable()) {
			if (AOIDataCollect.CreateHASI_MoniterThread() == false) {
				JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
				return;
			}
		}
		else {
			if (AOIDataCollect.DeleteHASI_MoniterThread() == false) {
				JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
				return;
			}
		}
	}
#endif //HASI_DISABLE

	if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	if ( MotionCtrlPtr->SaveMotionParameter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	//Light3DCtrl.SaveAllLight3DCastParameter();
	//AOIDataCollect.SaveSystemSliceParamINI();
	AOIDataCollect.SaveSystemParameter();
	AOIDataCollect.SaveSystemFilenameSyntax();
	//AOIDataCollect.SaveCalibrationParameter();		
	//AOIDataCollect.ApplyCalibrationParameter();
	AOIDataCollect.CopySystemParamFileToProjectFolder();
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	const bool bCreateTempFolder = false;
	str = _T("Do you want to load system parameters?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) { return; }
	if ( AOIDataCollect.LoadSystemFilenameSyntax() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	if ( AOIDataCollect.LoadSystemParameter(bCreateTempFolder) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		AOIDataCollect.SetSystemParameter(m_SysParam);
		return;
	}		
	AOIDataDefine.LoadDefineTextFile();
	m_SysParam = AOIDataCollect.GetSystemParameter();
	SetPaneSystemParameter();	
	ExecSelchangePaneTabWnd();	
	if ( AOIDataCollect.ApplySystemParameterToAllProject() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 400;
}
//-------------------------------------------------------------------------------------//
TSystemParameter CSystemConfigWnd::GetSystemParameter() const
{
	return this->m_SysParam;
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::SetSystemParameter(const TSystemParameter &SysParam)
{
	this->m_SysParam = SysParam;
}
//-------------------------------------------------------------------------------------//
void CSystemConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	str = _T("Do you wanto to save system parameter?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	OnSaveBtn();	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//