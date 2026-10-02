// MotionCtrlWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "MotionCtrlWnd.h"
//-------------------------------------------------------------------------------------//
#include "Motion_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
CMotionCtrlWnd MotionCtrlWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlWnd dialog
//-------------------------------------------------------------------------------------//
CMotionCtrlWnd::CMotionCtrlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionCtrlWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotionCtrlWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotionCtrlWnd)
	DDX_Control(pDX, MOTION_PANE_TAB_WND, m_PaneTabWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMotionCtrlWnd, CDialog)
	//{{AFX_MSG_MAP(CMotionCtrlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, MOTION_PANE_TAB_WND, OnSelchangePaneTabWnd)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(MOTION_SAVE_PARAM_BTN, OnSaveParamBtn)
	ON_BN_CLICKED(MOTION_LOAD_PARAM_BTN, OnLoadParamBtn)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(MOTION_APPLY_PARAM_BTN, OnApplyParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CMotionCtrlWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitPanelWnd();
	this->AdjustPaneWndPosition();	

	this->SwitchMultiLanguage();
	//this->ExecSelchangePaneTabWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	this->ShowWindow(SW_HIDE);
	
	//m_MotionPaneParam;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlWnd::InitPanelWnd()
{
	if ( this->m_MotionPaneStatus.Create(IDD_MOTION_STATUS_PANE, this) == FALSE ) { return false; }
	if ( this->m_MotionPaneParam.Create(IDD_MOTION_PARAM_PANE, this) == FALSE ) { return false; }

	CString str;
	int     tcIndex=0;
	TCHAR   tcBuffer[MAX_JET_PATH]=_T("");
	TCITEM tcItem;	
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = tcBuffer;
	
	if ( m_MotionPaneStatus.GetSafeHwnd() != NULL )
	{
		str = _T("Status");	
		str = this->LoadMultiLanguageString(str, str);
		::_tcscpy(tcBuffer, str);	
		tcItem.lParam = (LPARAM)(&m_MotionPaneStatus);
		this->m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}
	
	if ( m_MotionPaneParam.GetSafeHwnd() != NULL )
	{
		str = _T("Parameter");	
		str = this->LoadMultiLanguageString(str, str);
		::_tcscpy(tcBuffer, str);	
		tcItem.lParam = (LPARAM)(&m_MotionPaneParam);
		this->m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
		tcIndex ++;
	}

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return true;
}
//-------------------------------------------------------------------------------------//
void  CMotionCtrlWnd::AdjustPaneWndPosition()
{
	if ( m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE BtnWndSize={0};
	RECT BtnWndRect={0};
	RECT MainWndRect={0};
	RECT PaneWndRect={0};
	CWnd::GetClientRect(&MainWndRect);
	m_PaneTabWnd.GetWindowRect(&PaneWndRect);	
	ScreenToClient(&PaneWndRect);

	WndPtr = CWnd::GetDlgItem(MOTION_APPLY_PARAM_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnWndSize);
		BtnWndRect.bottom = MainWndRect.bottom-4;
		BtnWndRect.top = BtnWndRect.bottom-BtnWndSize.cy;
		WndPtr->MoveWindow(&BtnWndRect);
	}

	WndPtr = CWnd::GetDlgItem(MOTION_SAVE_PARAM_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnWndSize);
		BtnWndRect.bottom = MainWndRect.bottom-4;
		BtnWndRect.top = BtnWndRect.bottom-BtnWndSize.cy;
		WndPtr->MoveWindow(&BtnWndRect);
	}
	WndPtr = CWnd::GetDlgItem(MOTION_LOAD_PARAM_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&BtnWndRect);
		this->ScreenToClient(&BtnWndRect);
		JetAPI::GetRectSize(BtnWndRect, BtnWndSize);
		BtnWndRect.bottom = MainWndRect.bottom-4;
		BtnWndRect.top = BtnWndRect.bottom-BtnWndSize.cy;
		WndPtr->MoveWindow(&BtnWndRect);
	}

	PaneWndRect.left = MainWndRect.left+4;
	PaneWndRect.right = MainWndRect.right-4;
	PaneWndRect.bottom = BtnWndRect.top-4;
	m_PaneTabWnd.MoveWindow(&PaneWndRect);

	PaneWndRect.top += 24; 
	PaneWndRect.left += 4;
	PaneWndRect.right -= 4;
	PaneWndRect.bottom -= 4;
	if ( this->m_MotionPaneStatus.GetSafeHwnd() != NULL )
	{	this->m_MotionPaneStatus.MoveWindow(&PaneWndRect);	}

	if ( this->m_MotionPaneParam.GetSafeHwnd() != NULL )
	{	this->m_MotionPaneParam.MoveWindow(&PaneWndRect);	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	this->ExecSelchangePaneTabWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::ExecSelchangePaneTabWnd()
{
	int    i = 0;
	CWnd  *pWnd = NULL;
	TCITEM tcItem;
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;
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
void CMotionCtrlWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	UINT ShowMode = 0;
	UINT ShowModePre = 0;
	if ( TRUE == bShow )
	{	
		ShowMode = SW_SHOW;	
		ShowModePre = SW_HIDE;
	}
	else
	{	ShowMode = SW_HIDE;	}


	if ( TRUE == bShow )
	{	m_MotionPaneStatus.EnableUIWnd(TRUE, 0); }

	TCITEM tcItem;
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;
	const int TabIndex = this->m_PaneTabWnd.GetCurSel();
	if ( TabIndex >= 0 ) 
	{
		this->m_PaneTabWnd.GetItem(TabIndex, &tcItem);
		CWnd *pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == TRUE )
		{
			if ( 0 != ShowModePre )
			{	pWnd->ShowWindow(ShowModePre);		}
			pWnd->ShowWindow(ShowMode);	
		}
	}
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::SwitchMultiLanguage()
{		
	int     i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MOTION_CTRL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MOTION_CTRL_WND;
	WndKey = _T("IDD_MOTION_CTRL_WND");
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
	WndID = MOTION_APPLY_PARAM_BTN;
	WndKey = _T("MOTION_APPLY_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MOTION_SAVE_PARAM_BTN;
	WndKey = _T("MOTION_SAVE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MOTION_LOAD_PARAM_BTN;
	WndKey = _T("MOTION_LOAD_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	/*
	TCITEM tcItem;
	const size_t  BufSize=32;
	TCHAR TxtBuff[BufSize]=_T("");

	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = TxtBuff;
	tcItem.cchTextMax = BufSize;
	const int TabItemCount = (int)(this->m_PaneTabWnd.GetItemCount());
	for ( i=0; i<TabItemCount; i++ )
	{
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd == &(m_MotionPaneStatus) )
		{
			WndID = IDD_MOTION_STATUS_PANE;
			WndKey = _T("IDD_MOTION_STATUS_PANE");
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
		else if ( pWnd == &(m_MotionPaneParam) )
		{
			WndID = IDD_MOTION_PARAM_PANE;
			WndKey = _T("IDD_MOTION_PARAM_PANE");
			LabelText = tcItem.pszText;
			AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
			::_tcscpy(tcItem.pszText, NewLabelText);
			this->m_PaneTabWnd.SetItem(i, &tcItem);
		}
	}*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CMotionCtrlWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MOTION_CTRL_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnSaveParamBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	if ( MotionCtrlPtr->SaveMotionParameter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	return ;
	if ( m_MotionPaneParam.IsWindowVisible() == TRUE )
	{
		m_MotionPaneParam.SaveAllParamToINI();
		if ( MotionCtrlPtr->LoadMotionParameter() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	}
	else
	{
		if ( MotionCtrlPtr->SaveMotionParameter() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	}
	AOIDataCollect.BackupAllSystemIniFiles();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnLoadParamBtn() 
{
	// TODO: Add your control notification handler code here
	if ( MotionCtrlPtr->LoadMotionParameter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	m_MotionPaneParam.BuildAllParamList();	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 880;
	lpMMI->ptMinTrackSize.y = 560;
	lpMMI->ptMinTrackSize.y = 620;
}
//-------------------------------------------------------------------------------------//
BOOL CMotionCtrlWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlWnd::OnApplyParamBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	if ( MotionCtrlPtr->UpdateMotionParamter() == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }
	
	TMotionParameter &MotionParam=MotionCtrlPtr->GetMotionParameter();
	if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
	{
		if ( MotionCtrlPtr->LoadMotionXYCali() == false )
		{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); }	
	}
	return ;	
}
//-------------------------------------------------------------------------------------//