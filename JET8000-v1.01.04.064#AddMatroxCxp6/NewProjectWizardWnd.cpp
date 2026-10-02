// NewProjectWizardWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectWizardWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectWizardWnd dialog
//-------------------------------------------------------------------------------------//
CNewProjectWizardWnd::CNewProjectWizardWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CNewProjectWizardWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectWizardWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_ImageBuffer = NULL;	
	m_ImageBufferSize = 0;
	m_ShowBuffer = NULL;	
	m_ShowBufferSize = 0;	

	m_PanelPtr = NULL;
	m_ProjectPtr = NULL;
	m_ProjectExist = NULL;
	m_EnableMultiDistrictMode = false;
	m_NewProjectMode=NEW_PROJECT_ONLINE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectWizardWnd)
	DDX_Control(pDX, NEWPROJECT_EXIT_BTN, m_ExitBtn);
	DDX_Control(pDX, NEWPROJECT_FINISH_BTN, m_FinishBtn);
	DDX_Control(pDX, NEWPROJECT_PRE_PANE_BTN, m_PrePaneBtn);
	DDX_Control(pDX, NEWPROJECT_NEXT_PANE_BTN, m_NextPaneBtn);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectWizardWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CNewProjectWizardWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(NEWPROJECT_NEXT_PANE_BTN, OnNextPaneBtn)
	ON_BN_CLICKED(NEWPROJECT_PRE_PANE_BTN, OnPrePaneBtn)
	ON_BN_CLICKED(NEWPROJECT_FINISH_BTN, OnFinishBtn)
	ON_BN_CLICKED(NEWPROJECT_EXIT_BTN, OnExitBtn)
	ON_BN_CLICKED(NEWPROJECT_MOTION_CTRL_BTN, OnMotionCtrlBtn)
	ON_BN_CLICKED(NEWPROJECT_PROJECT_MAP_BTN, OnProjectMapBtn)
	ON_BN_CLICKED(NEWPROJECT_LIGHT_CTRL_WND_BTN, OnLightCtrlWndBtn)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectWizardWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectWizardWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//---------------------------------------------------------------------------------//
	CreateImageBuffer();
	CreateShowBuffer();
	//---------------------------------------------------------------------------------//	
	const bool bNewProject = CheckNewProject();
	NEW_PROJECT_MODE NewProjectMode = GetNewProjectMode();
	//---------------------------------------------------------------------------------//
	if ( false == bNewProject )
	{	m_ProjectPtr = m_ProjectExist;	}
	else
	{	m_ProjectPtr = AOIObjManager.CreateProjectObj();	}
	if ( NULL == m_ProjectPtr )
	{
		JetAPI::ShowMessageBox(_T("Error, Create Project Fault"));
		CWnd::PostMessage(WM_CLOSE, NULL, NULL);		
		return FALSE;
	}
	m_PanelPtr = AOIObjManager.CreatePanelObj();
	if ( m_PanelPtr == NULL )
	{
		if ( true == bNewProject )
		{	AOIObjManager.DestroyProjectObj(m_ProjectPtr);	}
		JetAPI::ShowMessageBox(_T("Error, Create Panel Fault"));
		CWnd::PostMessage(WM_CLOSE, NULL, NULL);		
		return FALSE;
	}
	DISTRICT_ID DistrictID=DISTRICT_ID_A;
	const bool bMultiDistrictMode = GetEnableMultiDistrictMode();	
	AOIDataCollect.SetActiveDistrictID(DistrictID);	
	m_ProjectPtr->SetProjectActDistrictID(DistrictID, false);
	m_ProjectPtr->SetProjectMultiDistrictMode(bMultiDistrictMode);
	m_ProjectPtr->AddProjectPanelPtr(m_PanelPtr, false);
	m_ProjectPtr->SelectProjectAllPanels(false);
	m_PanelPtr->SetPanelSelected(true);	
	m_PanelPtr->SetPanelActDistrictID(DistrictID);
	m_ProjectPtr->SetProjectActivePanel(m_PanelPtr);	
	//---------------------------------------------------------------------------------//	
	m_PaneIntroduction.SetProjectPtr(m_ProjectPtr);
	m_PaneIntroduction.SetIsNewProject(bNewProject);
	m_PaneIntroduction.SetNewProjectMode(NewProjectMode);	
	m_PaneIntroduction.SetEnableMultiDistrictMode(bMultiDistrictMode);
	m_PaneIntroduction.Create(IDD_NEW_PROJECT_PANE_INTROD, this);	
	//---------------------------------------------------------------------------------//	
	m_PaneRgnImage.SetDistrictID(DistrictID);
	m_PaneRgnImage.SetProjectPtr(m_ProjectPtr);	
	m_PaneRgnImage.SetNewProjectMode(NewProjectMode);
	m_PaneRgnImage.SetEnableMultiDistrictMode(bMultiDistrictMode);
	m_PaneRgnImage.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
	m_PaneRgnImage.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);	
	m_PaneRgnImage.Create(IDD_NEW_PROJECT_PANE_REGION_IMAGE, this);	
	//---------------------------------------------------------------------------------//
	if ( NEW_PROJECT_OFFLINE != NewProjectMode )
	{	
		//-----------------------------------------------------------------------------//
		m_PaneLoadLib.SetProjectPtr(m_ProjectPtr);
		m_PaneLoadLib.SetNewProjectMode(NewProjectMode);
		m_PaneLoadLib.SetEnableMultiDistrictMode(bMultiDistrictMode);
		m_PaneLoadLib.Create(IDD_NEW_PROJECT_PANE_LOAD_LIBRARY, this);		
		//-----------------------------------------------------------------------------//
		m_PaneLoadCadxy.SetProjectPtr(m_ProjectPtr);
		m_PaneLoadCadxy.SetNewProjectMode(NewProjectMode);
		m_PaneLoadCadxy.SetEnableMultiDistrictMode(bMultiDistrictMode);
		m_PaneLoadCadxy.Create(IDD_NEW_PROJECT_PANE_LOAD_CADXY, this);		
		//-----------------------------------------------------------------------------//
		if ( true == bMultiDistrictMode ) 
		{
			m_PaneDivideDistrict.SetProjectPtr(m_ProjectPtr);
			m_PaneDivideDistrict.SetNewProjectMode(NewProjectMode);
			m_PaneDivideDistrict.SetEnableMultiDistrictMode(bMultiDistrictMode);
			m_PaneDivideDistrict.Create(IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT, this);			
		}
		m_PaneAlignPanel.SetDistrictID(DistrictID);
		m_PaneAlignPanel.SetProjectPtr(m_ProjectPtr);
		m_PaneAlignPanel.SetNewProjectMode(NewProjectMode);
		m_PaneAlignPanel.SetEnableMultiDistrictMode(bMultiDistrictMode);
		m_PaneAlignPanel.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
		m_PaneAlignPanel.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
		m_PaneAlignPanel.Create(IDD_NEW_PROJECT_PANE_ALIGN_PANEL, this);		
		//-----------------------------------------------------------------------------//
		m_PaneFiducial.SetDistrictID(DistrictID);
		m_PaneFiducial.SetProjectPtr(m_ProjectPtr);
		m_PaneFiducial.SetNewProjectMode(NewProjectMode);
		m_PaneFiducial.SetEnableMultiDistrictMode(bMultiDistrictMode);
		m_PaneFiducial.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
		m_PaneFiducial.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
		m_PaneFiducial.Create(IDD_NEW_PROJECT_PANE_FIDUCIAL, this);		
		//-----------------------------------------------------------------------------//
		if ( true == bMultiDistrictMode ) 
		{
			DISTRICT_ID DistrictID2 = DISTRICT_ID_B;
			m_PaneAlignPanel_DB.SetDistrictID(DistrictID2);
			m_PaneAlignPanel_DB.SetProjectPtr(m_ProjectPtr);
			m_PaneAlignPanel_DB.SetNewProjectMode(NewProjectMode);
			m_PaneAlignPanel_DB.SetEnableMultiDistrictMode(bMultiDistrictMode);
			m_PaneAlignPanel_DB.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
			m_PaneAlignPanel_DB.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
			m_PaneAlignPanel_DB.Create(IDD_NEW_PROJECT_PANE_ALIGN_PANEL, this);			
			//-----------------------------------------------------------------------------//
			m_PaneFiducial_DB.SetDistrictID(DistrictID2);
			m_PaneFiducial_DB.SetProjectPtr(m_ProjectPtr);
			m_PaneFiducial_DB.SetNewProjectMode(NewProjectMode);
			m_PaneFiducial_DB.SetEnableMultiDistrictMode(bMultiDistrictMode);
			m_PaneFiducial_DB.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer);
			m_PaneFiducial_DB.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer);
			m_PaneFiducial_DB.Create(IDD_NEW_PROJECT_PANE_FIDUCIAL, this);			
		}		
	}	
	//---------------------------------------------------------------------------------//	
	if ( m_PaneIntroduction.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneIntroduction); }
	if ( m_PaneRgnImage.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneRgnImage); }
	if ( m_PaneLoadLib.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneLoadLib); }
	if ( m_PaneLoadCadxy.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneLoadCadxy); }
	if ( m_PaneDivideDistrict.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneDivideDistrict); }	
	if ( m_PaneAlignPanel.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneAlignPanel); }
	if ( m_PaneFiducial.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneFiducial); }	
	if ( m_PaneAlignPanel_DB.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneAlignPanel_DB); }
	if ( m_PaneFiducial_DB.GetSafeHwnd() != NULL )
	{	m_PanePtrList.push_back(&m_PaneFiducial_DB); }	
	//---------------------------------------------------------------------------------//		
	m_CurPaneIndex = 0;	
	m_CurPaneWnd = &m_PaneIntroduction;
	//---------------------------------------------------------------------------------//		
	m_LightCtrlWnd.Create(IDD_LIGHT_CTRL_BOARD_WND, this);
	m_MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this);
	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);	
	ShowWindow(SW_SHOWMAXIMIZED);
	//---------------------------------------------------------------------------------//	
	//this->ModifyControlWndPos();
	SwitchMultiLanguage();
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
	//---------------------------------------------------------------------------------//	
	MotionCtrlPtr->SetIsJogMode(true);
	AOIDataCollect.SetOfflineMode(false);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataCollect.SetActiveProjectPtr(m_ProjectPtr);	
	//---------------------------------------------------------------------------------//	
#ifdef OFFLINE_VERSION
	JetAPI::EnableCtrlWnd(this, NEWPROJECT_LIGHT_CTRL_WND_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, NEWPROJECT_MOTION_CTRL_BTN, FALSE);	
#endif//OFFLINE_VERSION
	//---------------------------------------------------------------------------------//	
	if ( false == bNewProject )
	{	this->m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, true);	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	size_t i = 0;
	CWnd *pWnd = NULL;
	const size_t PaneCount = this->m_PanePtrList.size();	
	//Destroy Pane Wnd
	for ( i=0; i<PaneCount; i++ )
	{
		pWnd = this->m_PanePtrList[i];
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->DestroyWindow();
	}
	m_PanePtrList.clear();

	DestroyImageBuffer();
	DestroyShowBuffer();
	m_ProjectMapWnd.SetProjectPtr(NULL, true);
	MotionCtrlPtr->SetIsJogMode(false);
	AOIDataCollect.SetActiveProjectPtr(NULL);
	AOIDataCollect.ReleaseModelUniFrameList();
	AOIDataCollect.ReleaseFieldUniFrameList();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	this->ModifyControlWndPos();
}
//-------------------------------------------------------------------------------------//
bool CNewProjectWizardWnd::CreateImageBuffer()//建立影像資料
{
	const char fnName[] = "CNewProjectWizardWnd::CreateImageBuffer";
	DestroyImageBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_ImageBuffer, fnName, "m_ImageBuffer") == false )	
	{	return false;	}

	::memset(m_ImageBuffer, 0x00, sizeof(unsigned char)*BufferSize);
	this->m_ImageBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectWizardWnd::DestroyImageBuffer()//摧毀影像資料
{
	if ( NULL != this->m_ImageBuffer )
	{	JetMemory.free_func(m_ImageBuffer);	}
	m_ImageBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectWizardWnd::CreateShowBuffer()//建立顯示資料
{
	const char fnName[] = "CNewProjectWizardWnd::CreateShowBuffer";
	DestroyShowBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_ShowBuffer, fnName, "m_ShowBuffer") == false )	
	{	return false;	}

	::memset(m_ShowBuffer, 0x00, sizeof(unsigned char)*BufferSize);
	this->m_ShowBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectWizardWnd::DestroyShowBuffer()//摧毀顯示資料
{
	if ( NULL != this->m_ShowBuffer )
	{	JetMemory.free_func(m_ShowBuffer);	}
	m_ShowBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnNextPaneBtn() 
{
	// TODO: Add your control notification handler code here	
	CWnd *pWnd = NULL;
	if ( NULL == m_ProjectPtr )
	{	return; }		
	const size_t PaneCount = m_PanePtrList.size();
	const NEW_PROJECT_MODE NewProjectMode = GetNewProjectMode();
	const size_t PanelCount = m_ProjectPtr->GetProjectPanelCount();	
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];		
		if ( pWnd == &m_PaneIntroduction )
		{	if ( m_PaneIntroduction.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneRgnImage )
		{	if ( m_PaneRgnImage.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadLib )
		{	if ( m_PaneLoadLib.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneDivideDistrict )
		{	if ( m_PaneDivideDistrict.ExecNextPane() == false ) { return; }	}		
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneAlignPanel_DB )
		{	if ( m_PaneAlignPanel_DB.ExecNextPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial_DB )
		{	if ( m_PaneFiducial_DB.ExecNextPane() == false ) { return; }	}
	}

	if ( (PaneCount-1) == m_CurPaneIndex ) { return; }
	m_CurPaneIndex ++;
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnPrePaneBtn() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	if ( NULL == m_ProjectPtr )
	{	return; }
	size_t MinIndex = 0;
	const size_t PaneCount = m_PanePtrList.size();
	const NEW_PROJECT_MODE NewProjectMode = GetNewProjectMode();
	const size_t PanelCount = m_ProjectPtr->GetProjectPanelCount();
	if ( PanelCount > 1 ) 
	{	MinIndex = 3; }
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];
		if ( pWnd == &m_PaneIntroduction )
		{	if ( m_PaneIntroduction.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneRgnImage )
		{	if ( m_PaneRgnImage.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadLib )
		{	if ( m_PaneLoadLib.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneDivideDistrict )
		{	if ( m_PaneDivideDistrict.ExecPrevPane() == false ) { return; }	}		
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneAlignPanel_DB )
		{	if ( m_PaneAlignPanel_DB.ExecPrevPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial_DB )
		{	if ( m_PaneFiducial_DB.ExecPrevPane() == false ) { return; }	}
	}
	if ( MinIndex == m_CurPaneIndex ) { return; }
	m_CurPaneIndex --;
	DoSelchangePaneWnd();
	ModifyPreNextFinishBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnFinishBtn() 
{
	// TODO: Add your control notification handler code here
	size_t       i=0;
	CWnd *pWnd = NULL;
	CAOIProject *ProjectPtr = GetProjectPtr();	
	const size_t PaneCount = m_PanePtrList.size();
	const NEW_PROJECT_MODE NewProjectMode = GetNewProjectMode();
	if ( m_CurPaneIndex < PaneCount )
	{
		pWnd = m_PanePtrList[m_CurPaneIndex];
		if ( pWnd == &m_PaneIntroduction )
		{	if ( m_PaneIntroduction.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneRgnImage )
		{	if ( m_PaneRgnImage.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadLib )
		{	if ( m_PaneLoadLib.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneLoadCadxy )
		{	if ( m_PaneLoadCadxy.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneDivideDistrict )
		{	if ( m_PaneDivideDistrict.ExecFinishPane() == false ) { return; }	}		
		else if ( pWnd == &m_PaneAlignPanel )
		{	if ( m_PaneAlignPanel.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial )
		{	if ( m_PaneFiducial.ExecFinishPane() == false ) { return; }	}		
		else if ( pWnd == &m_PaneAlignPanel_DB )
		{	if ( m_PaneAlignPanel_DB.ExecFinishPane() == false ) { return; }	}
		else if ( pWnd == &m_PaneFiducial_DB )
		{	if ( m_PaneFiducial_DB.ExecFinishPane() == false ) { return; }	}		
	}
	if ( NULL == ProjectPtr ) { return; }

	CString      str;
	DWORD        Res=0;
	const bool  bMultiDistrictMode = GetEnableMultiDistrictMode();	
	DISTRICT_ID ActDistrictID = ProjectPtr->GetProjectActDistrictID();	
	if ( NEW_PROJECT_OFFLINE!=NewProjectMode && false==bMultiDistrictMode )
	{		
		str = _T("Do you want to add new panel?");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if ( IDYES == Res )
		{
			DISTRICT_ID DistrictID = DISTRICT_ID_A;
			AOIDataCollect.SetActiveDistrictID(DistrictID);
			ProjectPtr->SetProjectActDistrictID(DistrictID, true);
			m_PanelPtr = AOIObjManager.CreatePanelObj();
			m_ProjectPtr->AddProjectPanelPtr(m_PanelPtr, false);
			m_ProjectPtr->SelectProjectAllPanels(false);
			m_PanelPtr->SetPanelSelected(true);	
			m_ProjectPtr->SetProjectActivePanel(m_PanelPtr);
			m_PanelPtr->SetPanelActDistrictID(DistrictID);

			for ( i=0; i<PaneCount; i++ )
			{
				pWnd = m_PanePtrList[i];
				if ( NULL == pWnd ) { continue; }
				if ( pWnd == &m_PaneLoadCadxy )
				{
					m_CurPaneIndex = i;
					m_PaneLoadCadxy.ReInitialPane();
					break;
				}
			}		
			DoSelchangePaneWnd();
			ModifyPreNextFinishBtn();
			return;
		}
	}		
	if ( NEW_PROJECT_OFFLINE == NewProjectMode )
	{
		ProjectPtr->SelectProjectAllPanels(true);
		ProjectPtr->DeleteProjectPanelSelected();
	}
	ProjectPtr->ClearProjectAllInspectionField();	
	ProjectPtr->SetProjectFdModified(false);

	CString ProjectFolder;
	CString ProjectFilename = ProjectPtr->GetProjectFileName();
	JetAPI::ExtractMainFileName(ProjectFilename, ProjectFolder);
	CString ProjectMapName = AOIDataDefine.GetProjectMapFileName(ProjectFolder);
	ProjectPtr->SaveProjectMapFile(ProjectMapName);	

	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::ModifyControlWndPos()
{
	if ( this->m_NextPaneBtn.GetSafeHwnd() == NULL ) { return; }
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
	this->m_ExitBtn.MoveWindow(&Rect);	

	//Next, finish Btn	
	m_NextPaneBtn.GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	this->m_NextPaneBtn.MoveWindow(&Rect);
	this->m_FinishBtn.MoveWindow(&Rect);

	//Pre Btn	
	m_PrePaneBtn.GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	this->m_PrePaneBtn.MoveWindow(&Rect);
	NewRect = Rect;

	//MotionCtrlBtn
	pWnd = this->GetDlgItem(NEWPROJECT_MOTION_CTRL_BTN);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	pWnd->MoveWindow(&Rect);	
	NewRect = Rect;	

	//ProjectMapBtn
	pWnd = this->GetDlgItem(NEWPROJECT_PROJECT_MAP_BTN);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	pWnd->MoveWindow(&Rect);	
	NewRect = Rect;
	
	//LightCtrlWnd
	pWnd = this->GetDlgItem(NEWPROJECT_LIGHT_CTRL_WND_BTN);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->GetWindowRect(&Rect);	
	this->ScreenToClient(&Rect);
	::OffsetRect(&Rect, WndOffset.x, WndOffset.y);
	pWnd->MoveWindow(&Rect);	
	NewRect = Rect;

	//Group
	pWnd = this->GetDlgItem(NEWPROJECT_PANE_GROUP_WND);
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
bool CNewProjectWizardWnd::CheckNewProject() const
{
	if ( NULL == m_ProjectExist ) { return true; }
	//if ( m_ProjectExist != m_ProjectPtr ) { return true; }
	return false; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::ModifyPreNextFinishBtn()
{	
	const size_t PaneCount = m_PanePtrList.size();
	if ( 0 == m_CurPaneIndex )
	{	m_PrePaneBtn.ShowWindow(SW_HIDE);	}
	else
	{	m_PrePaneBtn.ShowWindow(SW_SHOW);	}

	if ( (PaneCount-1) == m_CurPaneIndex )
	{	
		m_NextPaneBtn.ShowWindow(SW_HIDE);	
		m_FinishBtn.ShowWindow(SW_SHOW);
	}
	else
	{	
		m_NextPaneBtn.ShowWindow(SW_SHOW);	
		m_FinishBtn.ShowWindow(SW_HIDE);
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::DoSelchangePaneWnd()
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
		pWnd = m_PanePtrList[i];
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		pWnd->ShowWindow(SW_HIDE);
	}

	//Show Pane 	
	pWnd = m_PanePtrList[m_CurPaneIndex];
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
	pWnd->ShowWindow(SW_SHOW);
	
	size_t  WndID = 0;
	CString WndKey;	
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_WIZARD_WND");	
	//---------------------------------------------------------------------------------//
	if ( pWnd == &m_PaneIntroduction )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_INTROD");	
		WndID = IDD_NEW_PROJECT_PANE_INTROD;
		WndKey = _T("IDD_NEW_PROJECT_PANE_INTROD");
	}
	else if ( pWnd == &m_PaneRgnImage )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_REGION_IMAGE");	
		WndID = IDD_NEW_PROJECT_PANE_REGION_IMAGE;
		WndKey = _T("IDD_NEW_PROJECT_PANE_REGION_IMAGE");
	}
	else if ( pWnd == &m_PaneLoadLib )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_LOAD_LIBRARY");	
		WndID = IDD_NEW_PROJECT_PANE_LOAD_LIBRARY;
		WndKey = _T("IDD_NEW_PROJECT_PANE_LOAD_LIBRARY");
	}
	else if ( pWnd == &m_PaneLoadCadxy )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");	
		WndID = IDD_NEW_PROJECT_PANE_LOAD_CADXY;
		WndKey = _T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");
	}
	else if ( pWnd == &m_PaneDivideDistrict )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT");	
		WndID = IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT;
		WndKey = _T("IDD_NEW_PROJECT_PANE_DIVIDE_DISTRICT");
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
	else if ( pWnd == &m_PaneAlignPanel_DB )
	{
		Section=_T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");	
		WndID = IDD_NEW_PROJECT_PANE_ALIGN_PANEL;
		WndKey = _T("IDD_NEW_PROJECT_PANE_ALIGN_PANEL");
	}
	else if ( pWnd == &m_PaneFiducial_DB )
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
void CNewProjectWizardWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_WIZARD_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_WIZARD_WND;
	WndKey = _T("IDD_NEW_PROJECT_WIZARD_WND");
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
	WndID = NEWPROJECT_PROJECT_MAP_BTN;
	WndKey = _T("NEWPROJECT_PROJECT_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPROJECT_MOTION_CTRL_BTN;
	WndKey = _T("NEWPROJECT_MOTION_CTRL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPROJECT_LIGHT_CTRL_WND_BTN;
	WndKey = _T("NEWPROJECT_LIGHT_CTRL_WND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = NEWPROJECT_EXIT_BTN;
	WndKey = _T("NEWPROJECT_EXIT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPROJECT_NEXT_PANE_BTN;
	WndKey = _T("NEWPROJECT_NEXT_PANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPROJECT_PRE_PANE_BTN;
	WndKey = _T("NEWPROJECT_PRE_PANE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = NEWPROJECT_FINISH_BTN;
	WndKey = _T("NEWPROJECT_FINISH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CNewProjectWizardWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_WIZARD_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	CString filename = m_ProjectPtr->GetProjectFileName();	
	if ( CheckNewProject() == true )	
	{
		AOIObjManager.DestroyProjectObj(m_ProjectPtr);	
		CAOIProject::DeleteProjectTempFile(filename);
	}
	else
	{
		if ( NULL != m_PanelPtr )
		{
			m_ProjectPtr->ResetProjectActiveIndex();
			m_ProjectPtr->SelectProjectAllPanels(false);
			m_PanelPtr->SetPanelSelected(true);
			m_ProjectPtr->DeleteProjectPanelSelected();				
		}
	}
	SetProjectExist(NULL);
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnExitBtn() 
{
	// TODO: Add your control notification handler code here
	CNewProjectWizardWnd::OnCancel();
//	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnMotionCtrlBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_MotionCtrlWnd.GetSafeHwnd() == NULL ) { return; }
	m_MotionCtrlWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectWizardWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	const bool InspectionDrawing = AOIDataCollect.GetInspectionDrawing();	
	switch ( message )
	{
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{		
		case WPARAM_UPDATE_PROJECT_MAP:			
			this->m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, true);		
			if ( m_ProjectMapWnd.GetSafeHwnd() != NULL )
			{
				if ( m_ProjectMapWnd.IsWindowVisible() == FALSE )
				{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
			}
			break;
		case WPARAM_REDRAW_PROJECT_MAP:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL && m_ProjectMapWnd.IsWindowVisible() == TRUE )
			{
				if ( true == InspectionDrawing )
				{
					CAOIProject *ProjectPtr = GetProjectPtr();
					if ( NULL != ProjectPtr )
					{	ProjectPtr->UpdateProjectMapShowPtr(); }
				}	
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
void CNewProjectWizardWnd::OnProjectMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }
	m_ProjectMapWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnLightCtrlWndBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_LightCtrlWnd.GetSafeHwnd() == NULL ) { return; }
	m_LightCtrlWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CNewProjectWizardWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1200;
	lpMMI->ptMinTrackSize.y = 600;
}
//-------------------------------------------------------------------------------------//