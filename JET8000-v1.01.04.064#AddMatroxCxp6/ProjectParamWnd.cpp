// ProjectParamWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamWnd dialog
//-------------------------------------------------------------------------------------//
CProjectParamWnd::CProjectParamWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectParamWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamWnd)
	DDX_Control(pDX, PROJECTPARAM_PANE_TAB_WND, m_PaneTabWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectParamWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TCN_SELCHANGE, PROJECTPARAM_PANE_TAB_WND, OnSelchangePaneTabWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here		
	//---------------------------------------------------------------------------------//	
	InitPanelWnd();
	AdjustPaneWndPosition();
	//---------------------------------------------------------------------------------//	
	SwitchMultiLanguage();
	ExecSelchangePaneTabWnd();
	//---------------------------------------------------------------------------------//	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
TProjectParameter& CProjectParamWnd::GetProjectParameter()
{
	return m_ProjectParameter;
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::SetProjectParameter(CAOIProject *ProjectPtr, const TProjectParameter &Param)
{
	m_ProjectPtr = ProjectPtr;
	m_ProjectParameter = Param;
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_WND;
	WndKey = _T("IDD_PROJECT_PARAM_WND");
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
}
//-------------------------------------------------------------------------------------//
CString CProjectParamWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamWnd::InitPanelWnd()
{
	CString str;
	m_ProjectPaneBasic.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);	
	m_ProjectPaneSave.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);	
	m_ProjectPaneAlarm.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);
	m_ProjectPaneBarcode.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);	
	m_ProjectPanelRepair.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);		
	m_ProjectPaneSpecTest.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);
	m_ProjectVersionCode.SetProjectParameterPtr(m_ProjectPtr, &m_ProjectParameter);

	if ( m_ProjectPaneBasic.Create(IDD_PROJECT_PARAM_BASIC_PANE, this) == FALSE )
	{	return false; }
	if ( m_ProjectPaneSave.Create(IDD_PROJECT_PARAM_SAVE_PANE, this) == FALSE )
	{	return false; }			
	if ( m_ProjectPaneAlarm.Create(IDD_PROJECT_PARAM_ALARM_PANE, this) == FALSE )
	{	return false; }	
	if ( m_ProjectPaneBarcode.Create(IDD_PROJECT_PARAM_BARCODE_PANE, this) == FALSE )
	{	return false; }	
	if ( m_ProjectPanelRepair.Create(IDD_PROJECT_PARAM_REPAIR_PANE, this) == FALSE )
	{	return false; }		
	if (m_ProjectPaneSpecTest.Create(IDD_PROJECT_PARAM_SPEC_TEST_PANE, this) == FALSE)
	{	return false;	}
	if (m_ProjectVersionCode.Create(IDD_PROJECT_PARAM_VERSION_CODE_PANE, this) == FALSE)
	{	return false;	}

	int     tcIndex=0;	
	AddPaneTabWnd(_T("Basic"), m_ProjectPaneBasic, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Save"), m_ProjectPaneSave, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Alarm"), m_ProjectPaneAlarm, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Barcode"), m_ProjectPaneBarcode, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Repair"), m_ProjectPanelRepair, m_PaneTabWnd, tcIndex);
	AddPaneTabWnd(_T("Spec. Test"), m_ProjectPaneSpecTest, m_PaneTabWnd, tcIndex);	
	AddPaneTabWnd(_T("Version Code"), m_ProjectVersionCode, m_PaneTabWnd, tcIndex);	

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamWnd::AddPaneTabWnd(LPCTSTR Title, CWnd &Wnd, CTabCtrl &TabWnd, int &Index)
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
void CProjectParamWnd::AdjustPaneWndPosition()
{
	if ( m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	CWnd  *pWnd = NULL;
	SIZE WndSize={0};
	RECT WndRect={0};
	RECT MainWndRect={0};
	RECT PaneWndRect={0};
	const int MarginR=8;
	CWnd::GetClientRect(&MainWndRect);
	this->m_PaneTabWnd.GetClientRect(&PaneWndRect);
	this->m_PaneTabWnd.ClientToScreen(&PaneWndRect);
	this->ScreenToClient(&PaneWndRect);

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
	PaneWndRect.bottom = MainWndRect.bottom-MarginR;
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
void CProjectParamWnd::ExecSelchangePaneTabWnd()
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
void CProjectParamWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 720;
	lpMMI->ptMinTrackSize.y = 600;
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::OnOK() 
{
	// TODO: Add extra validation here
	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CProjectParamWnd::OnSelchangePaneTabWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ExecSelchangePaneTabWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//