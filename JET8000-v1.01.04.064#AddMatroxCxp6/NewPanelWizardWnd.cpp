// NewPanelWizardWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewPanelWizardWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewPanelWizardWnd dialog
//-------------------------------------------------------------------------------------//
CNewPanelWizardWnd::CNewPanelWizardWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CNewPanelWizardWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPanelWizardWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_NewCadMode=NEW_CAD_PANEL;

	m_PanelPtr = NULL;
	m_ProjectPtr = NULL;
	m_CurPaneIndex = INVALID_INDEX;

	m_ImageBuffer = NULL;//影像記憶體空間
	m_ImageBufferSize = 0;//影像記憶體尺寸		

	m_ShowBuffer = NULL;//顯示記憶體空間
	m_ShowBufferSize = 0;//顯記憶體尺寸
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPanelWizardWnd)
	DDX_Control(pDX, NEWPANEL_EXIT_BTN, m_ExitBtn);
	DDX_Control(pDX, NEWPANEL_FINISH_BTN, m_FinishBtn);
	DDX_Control(pDX, NEWPANEL_PRE_PANE_BTN, m_PrePaneBtn);
	DDX_Control(pDX, NEWPANEL_NEXT_PANE_BTN, m_NextPaneBtn);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewPanelWizardWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CNewPanelWizardWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(NEWPANEL_PRE_PANE_BTN, OnPrePaneBtn)
	ON_BN_CLICKED(NEWPANEL_NEXT_PANE_BTN, OnNextPaneBtn)
	ON_BN_CLICKED(NEWPANEL_EXIT_BTN, OnExitBtn)
	ON_BN_CLICKED(NEWPANEL_PROJECT_MAP_BTN, OnProjectMapBtn)
	ON_BN_CLICKED(NEWPANEL_MOTION_CTRL_BTN, OnMotionCtrlBtn)
	ON_BN_CLICKED(NEWPANEL_FINISH_BTN, OnFinishBtn)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewPanelWizardWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewPanelWizardWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();	
	// TODO: Add extra initialization here	
	CString str;
	//---------------------------------------------------------------------------------//	
	if ( NULL == m_ProjectPtr ) 
	{ 
		str = _T("Error, No Project Ptr");
		JetAPI::ShowMessageBox(str);
		CWnd::PostMessage(WM_CLOSE, NULL, NULL);		
		return FALSE;
	}
	//---------------------------------------------------------------------------------//	
	m_PanelPtr = AOIObjManager.CreatePanelObj();
	if ( NULL == m_PanelPtr )
	{
		str = _T("Error, No Panel Ptr");
		JetAPI::ShowMessageBox(str);
		CWnd::PostMessage(WM_CLOSE, NULL, NULL);		
		return FALSE; 
	}
	//---------------------------------------------------------------------------------//		
	CreateImageBuffer();
	CreateShowBuffer();
	//---------------------------------------------------------------------------------//	
	m_ProjectPtr->AddProjectPanelPtr(m_PanelPtr, false);
	m_ProjectPtr->SelectProjectAllPanels(false);
	//m_PanelPtr->SetPanelProjectPtr(m_ProjectPtr);
	m_PanelPtr->SetPanelSelected(true);	
	//---------------------------------------------------------------------------------//
	m_PaneLoadCadxy.SetProjectPtr(m_ProjectPtr);
	m_PaneLoadCadxy.Create(IDD_NEW_PROJECT_PANE_LOAD_CADXY, this);
	m_PanePtrList.push_back(&m_PaneLoadCadxy);
	//---------------------------------------------------------------------------------//
	m_PaneAlignPanel.SetProjectPtr(m_ProjectPtr);
	m_PaneAlignPanel.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
	m_PaneAlignPanel.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
	m_PaneAlignPanel.Create(IDD_NEW_PROJECT_PANE_ALIGN_PANEL, this);
	m_PanePtrList.push_back(&m_PaneAlignPanel);
	//---------------------------------------------------------------------------------//
	if ( NEW_CAD_PANEL == m_NewCadMode )
	{
		m_PaneFiducial.SetProjectPtr(m_ProjectPtr);
		m_PaneFiducial.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
		m_PaneFiducial.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
		m_PaneFiducial.Create(IDD_NEW_PROJECT_PANE_FIDUCIAL, this);
		m_PanePtrList.push_back(&m_PaneFiducial);
	}
	//---------------------------------------------------------------------------------//	
	m_CurPaneIndex = 0;	
	m_CurPaneWnd = &m_PaneLoadCadxy;
	//---------------------------------------------------------------------------------//			
	m_MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this);
	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
	m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, true);		
	ShowWindow(SW_SHOWMAXIMIZED);
	//---------------------------------------------------------------------------------//	
	//this->ModifyControlWndPos();
	SwitchMultiLanguage();
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
	//---------------------------------------------------------------------------------//	
	MotionCtrlPtr->SetIsJogMode(true);
	//AOIDataCollect.SetOfflineMode(false);	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	//---------------------------------------------------------------------------------//	
#ifdef OFFLINE_VERSION	
	JetAPI::EnableCtrlWnd(this, NEWPANEL_MOTION_CTRL_BTN, FALSE);	
#endif//OFFLINE_VERSION
	//---------------------------------------------------------------------------------//	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	size_t i = 0;
	CWnd *pWnd = NULL;
	const size_t PaneCount = m_PanePtrList.size();	
	//Destroy Pane Wnd
	for ( i=0; i<PaneCount; i++ )
	{
		pWnd = m_PanePtrList[i];
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->DestroyWindow();
	}
	m_PanePtrList.clear();

	DestroyImageBuffer();
	DestroyShowBuffer();
	m_ProjectMapWnd.SetProjectPtr(NULL, true);
	MotionCtrlPtr->SetIsJogMode(false);
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ModifyControlWndPos();
}
//-------------------------------------------------------------------------------------//
LRESULT CNewPanelWizardWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	const bool InspectionDrawing = AOIDataCollect.GetInspectionDrawing();	
	switch ( message )
	{
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{		
		case WPARAM_UPDATE_PROJECT_MAP:
			break;
		case WPARAM_REDRAW_PROJECT_MAP:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL && m_ProjectMapWnd.IsWindowVisible() == TRUE )
			{
				if ( NULL == lParam )
				{	m_ProjectMapWnd.RedrawWnd(TRUE);	}
				else if ( true == InspectionDrawing )
				{	m_ProjectMapWnd.RedrawWnd(TRUE);	}
			}
			break;
		case WPARAM_SHOW_PROJECT_MAP_WND:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL )
			{
				if ( FALSE == lParam )
				{	m_ProjectMapWnd.ShowWindow(SW_HIDE);	}
				else
				{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
			}
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			switch ( lParam )
			{
			case LPARAM_DRAW_PROJECT_MODE_NORMAL:
				m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
				break;
			case LPARAM_DRAW_PROJECT_MODE_INSPECTING:
				m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
				break;
			}
		break;	
		}
		break;	
	}		
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
BOOL CNewPanelWizardWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
bool CNewPanelWizardWnd::CreateImageBuffer()//建立影像資料
{
	const char fnName[] = "CNewPanelWizardWnd::CreateImageBuffer";
	DestroyImageBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_ImageBuffer, fnName, "m_ImageBuffer") == false )	
	{	return false;	}

	::memset(m_ImageBuffer, 0x00, sizeof(unsigned char)*BufferSize);
	this->m_ImageBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewPanelWizardWnd::DestroyImageBuffer()//摧毀影像資料
{
	if ( NULL != this->m_ImageBuffer )
	{	JetMemory.free_func(m_ImageBuffer);	}
	m_ImageBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewPanelWizardWnd::CreateShowBuffer()//建立顯示資料
{
	const char fnName[] = "CNewPanelWizardWnd::CreateShowBuffer";
	DestroyShowBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_ShowBuffer, fnName, "m_ShowBuffer") == false )	
	{	return false;	}

	::memset(m_ShowBuffer, 0x00, sizeof(unsigned char)*BufferSize);
	this->m_ShowBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewPanelWizardWnd::DestroyShowBuffer()//摧毀顯示資料
{
	if ( NULL != this->m_ShowBuffer )
	{	JetMemory.free_func(m_ShowBuffer);	}
	m_ShowBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::ModifyPreNextFinishBtn()
{
	const size_t PaneCount = this->m_PanePtrList.size();
	if ( 0 == m_CurPaneIndex )
	{	this->m_PrePaneBtn.ShowWindow(SW_HIDE);	}
	else
	{	this->m_PrePaneBtn.ShowWindow(SW_SHOW);	}

	if ( (PaneCount-1) == m_CurPaneIndex )
	{	
		this->m_NextPaneBtn.ShowWindow(SW_HIDE);	
		this->m_FinishBtn.ShowWindow(SW_SHOW);
	}
	else
	{	
		this->m_NextPaneBtn.ShowWindow(SW_SHOW);	
		this->m_FinishBtn.ShowWindow(SW_HIDE);
	}
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::ModifyControlWndPos()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_NextPaneBtn.GetSafeHwnd() == NULL ) { return; }
	CWnd *pWnd = NULL;
	RECT Rect={0};
	RECT NewRect={0};
	RECT WndRect={0};	
	SIZE WndSize={0};
	POINT WndOffset={0};
	this->GetClientRect(&WndRect);
	
	//Exit Btn
	m_ExitBtn.GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	WndOffset.x = WndRect.right-24-Rect.right;
	WndOffset.y = WndRect.bottom-24-Rect.bottom;
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	m_ExitBtn.MoveWindow(&Rect);	

	//Next, finish Btn	
	m_NextPaneBtn.GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	m_NextPaneBtn.MoveWindow(&Rect);
	m_FinishBtn.MoveWindow(&Rect);

	//Pre Btn	
	m_PrePaneBtn.GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	m_PrePaneBtn.MoveWindow(&Rect);
	NewRect = Rect;

	//MotionCtrlBtn
	pWnd = this->GetDlgItem(NEWPANEL_MOTION_CTRL_BTN);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	pWnd->MoveWindow(&Rect);	
	NewRect = Rect;	

	//ProjectImageBtn
	pWnd = this->GetDlgItem(NEWPANEL_PROJECT_MAP_BTN);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	pWnd->MoveWindow(&Rect);	
	NewRect = Rect;	

	//Group
	pWnd = this->GetDlgItem(NEWPANEL_PANE_GROUP_WND);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	Rect.top = WndRect.top + 4;
	Rect.bottom = NewRect.top - 4;
	Rect.left = WndRect.left + 4;
	Rect.right = WndRect.right - 4;	
	pWnd->MoveWindow(&Rect);

	int    Margin = 16;
	size_t i = 0;
	const size_t PaneCount = m_PanePtrList.size();	
	pWnd->GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	Rect.left += Margin;
	Rect.right -= Margin;
	Rect.top += Margin;
	Rect.bottom -= Margin;
	for ( i=0; i<PaneCount; i++ )
	{
		pWnd = m_PanePtrList[i];
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->MoveWindow(&Rect);
	}
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::DoSelchangePaneWnd()
{
	size_t i = 0;
	const size_t PaneCount = m_PanePtrList.size();
	if ( (m_CurPaneIndex<0) || (m_CurPaneIndex>=PaneCount) ) { return; }
	CWnd *pWnd = m_PanePtrList[m_CurPaneIndex];
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }

	CString WndText;
	UINT ControlID = 0;
	UINT NewControlID = pWnd->GetDlgCtrlID();
	this->m_CurPaneWnd = pWnd;
	//Hide Pane 
	for ( i=0; i<PaneCount; i++ )
	{
		if ( i == m_CurPaneIndex ) { continue; }
		pWnd = this->m_PanePtrList[i];
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->ShowWindow(SW_HIDE);
	}

	//Show Pane 	
	pWnd = this->m_PanePtrList[m_CurPaneIndex];
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->ShowWindow(SW_SHOW);
	
	size_t  WndID = 0;
	CString WndKey;	
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PANEL_WIZARD_WND");	
	//---------------------------------------------------------------------------------//	
	if ( pWnd == &m_PaneLoadCadxy )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");	
		WndID = IDD_NEW_PROJECT_PANE_LOAD_CADXY;
		WndKey = _T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");
	}
	else if ( pWnd == &m_PaneAlignPanel )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");	
		WndID = IDD_NEW_PROJECT_PANE_ALIGN_PANEL;
		WndKey = _T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");
	}
	else if ( pWnd == &m_PaneFiducial )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_FIDUCIAL");	
		WndID = IDD_NEW_PROJECT_PANE_FIDUCIAL;
		WndKey = _T("IDD_NEW_PROJECT_PANE_FIDUCIAL");
	}		
		
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);

	if ( m_WndText.GetLength() > 0 )
	{	WndText.Format(_T("%s-%s"), m_WndText, NewLabelText); }
	else
	{	WndText = NewLabelText; }
	this->SetWindowText(WndText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CNewPanelWizardWnd::GetActivePanelPtr()
{
	return m_PanelPtr;
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::SetNewCadMode(NEW_CAD_MODE Mode)
{
	m_NewCadMode = Mode;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CNewPanelWizardWnd::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PANEL_WIZARD_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PANEL_WIZARD_WND;
	WndKey = _T("IDD_NEW_PANEL_WIZARD_WND");
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
	WndID = NEWPANEL_FINISH_BTN;
	WndKey = _T("NEWPANEL_FINISH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = NEWPANEL_PROJECT_MAP_BTN;
	WndKey = _T("NEWPANEL_PROJECT_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPANEL_MOTION_CTRL_BTN;
	WndKey = _T("NEWPANEL_MOTION_CTRL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPANEL_PRE_PANE_BTN;
	WndKey = _T("NEWPANEL_PRE_PANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPANEL_NEXT_PANE_BTN;
	WndKey = _T("NEWPANEL_NEXT_PANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPANEL_EXIT_BTN;
	WndKey = _T("NEWPANEL_EXIT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPANEL_FINISH_BTN;
	WndKey = _T("NEWPANEL_FINISH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnPrePaneBtn() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	const size_t PaneCount = m_PanePtrList.size();
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];
		if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecPrevPane() == false ) { return; }	}
	}
	if ( 0 == m_CurPaneIndex ) { return; }
	m_CurPaneIndex --;
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnNextPaneBtn() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	const size_t PaneCount = m_PanePtrList.size();
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];				
		if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecNextPane() == false ) { return; }	}
	}

	if ( (PaneCount-1) == m_CurPaneIndex ) { return; }
	m_CurPaneIndex ++;
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnExitBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr )
	{
		CBaseDialog::OnCancel();
		return;
	}

	ProjectPtr->SelectProjectAllPanels(false);
	m_PanelPtr->SetPanelSelected(true);
	ProjectPtr->DeleteProjectPanelSelected();	
	m_PanelPtr = NULL;
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnProjectMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }
	m_ProjectMapWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnMotionCtrlBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_MotionCtrlWnd.GetSafeHwnd() == NULL ) { return; }
	m_MotionCtrlWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnFinishBtn() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	const size_t PaneCount = m_PanePtrList.size();
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];		
		if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecFinishPane() == false ) { return; }	}
	}
	if ( NULL == ProjectPtr ) { return; }
	//ProjectPtr->SetProjectFdModified(false);
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewPanelWizardWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1200;
	lpMMI->ptMinTrackSize.y = 600;
}
//-------------------------------------------------------------------------------------//