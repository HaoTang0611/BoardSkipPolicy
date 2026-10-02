// DebugWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "DebugWnd.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugWnd dialog
//-------------------------------------------------------------------------------------//
CDebugWnd::CDebugWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDebugWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CameraID = PRIMARY_CAMERA_ID;
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugWnd)
	DDX_Control(pDX, IDC_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, IDC_TREE_COMPONENT_WND, m_TreeWndComponent);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDebugWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CDebugWnd)
	ON_BN_CLICKED(IDC_ADD_PANEL_BTN, OnAddPanelBtn)
	ON_BN_CLICKED(IDC_ADD_BOARD_BTN, OnAddBoardBtn)
	ON_BN_CLICKED(IDC_ADD_COMPONENT_BTN, OnAddComponentBtn)
	ON_BN_CLICKED(IDC_DUMP_BTN, OnDumpBtn)
	ON_BN_CLICKED(IDC_CLONE_PANEL_BTN, OnClonePanelBtn)
	ON_BN_CLICKED(IDC_CLONE_BOARD_BTN, OnCloneBoardBtn)
	ON_BN_CLICKED(IDC_CLONE_COMPONENT_BTN, OnCloneComponentBtn)
	ON_BN_CLICKED(IDC_DELETE_PANEL_BTN, OnDeletePanelBtn)
	ON_BN_CLICKED(IDC_DELETE_BOARD_BTN, OnDeleteBoardBtn)
	ON_BN_CLICKED(IDC_DELETE_COMPONENT_BTN, OnDeleteComponentBtn)
	ON_BN_CLICKED(IDC_CLEAR_ALL_BTN, OnClearAllBtn)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADD_PROJECT_BTN, OnAddProjectBtn)
	ON_BN_CLICKED(IDC_CLONE_PROJECT_BTN, OnCloneProjectBtn)
	ON_BN_CLICKED(IDC_DELETE_PROJECT_BTN, OnDeleteProjectBtn)
	ON_BN_CLICKED(IDC_ADD_FD_BTN, OnAddFdBtn)
	ON_BN_CLICKED(IDC_CLONE_FD_BTN, OnCloneFdBtn)
	ON_BN_CLICKED(IDC_DELETE_FD_BTN, OnDeleteFdBtn)
	ON_BN_CLICKED(IDC_ADD_SB_BTN, OnAddSBBtn)
	ON_BN_CLICKED(IDC_CLONE_SB_BTN, OnCloneSBBtn)
	ON_BN_CLICKED(IDC_DELETE_SB_BTN, OnDeleteSBBtn)
	ON_BN_CLICKED(IDC_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(IDC_LOAD_BTN, OnLoadBtn)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_COMPONENT_WND, OnClickTreeComponentWnd)
	ON_BN_CLICKED(IDC_LOAD_DIB_BTN, OnLoadDibBtn)
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_PROJECT_IMAGE_BTN, OnProjectImageBtn)
	ON_BN_CLICKED(IDC_GRAB_PANEL_FD_BTN, OnGrabPanelFdBtn)
	ON_BN_CLICKED(IDC_RESET_THREAD_BTN, OnResetThreadBtn)
	ON_BN_CLICKED(IDC_GRAB_BOARD_FD_BTN, OnGrabBoardFdBtn)
	ON_BN_CLICKED(IDC_GRAB_COMPONENT_BTN, OnGrabComponentBtn)
	ON_BN_CLICKED(IDC_GO_ORG_BTN, OnGoOrgBtn)
	ON_BN_CLICKED(IDC_SHOW_MEMORY_BTN, OnShowMemoryBtn)
	ON_BN_CLICKED(IDC_SHOW_THREAD_BTN, OnShowThreadBtn)
	ON_BN_CLICKED(IDC_REBUILD_FIELD_BTN, OnRebuildFieldBtn)
	ON_BN_CLICKED(IDC_SHOW_TIME_BTN, OnShowTimeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugWnd message handlers
//-------------------------------------------------------------------------------------//
CAOIProject* CDebugWnd::GetProjectPtr()
{
	return GetLastProjectPtr();
//	return GetFirstProjectPtr();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CDebugWnd::GetLastProjectPtr()
{
	return AOIDataCollect.GetActiveProject();	
}
//-------------------------------------------------------------------------------------//
CAOIProject* CDebugWnd::GetFirstProjectPtr()
{
	return AOIDataCollect.GetProjectPtr(0, true);	
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddPanelBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	

	CAOIPanel *PanelPtr = AOIObjManager.CreatePanelObj();
	if ( NULL == PanelPtr ) { return; }
	ProjectPtr->AddProjectPanelPtr(PanelPtr, false);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddBoardBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }
	CAOIBoard *BoardPtr = AOIObjManager.CreateBoardObj();
	if ( NULL == BoardPtr ) { return; }
	PanelPtr->AddPanelBoardPtr(BoardPtr);
	ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddComponentBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }

	const size_t BoardCount = PanelPtr->GetPanelBoardCount();
	if ( 0 == BoardCount ) { return ; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(BoardCount-1, true);
	if ( NULL == BoardPtr ) { return; }

	CAOIComponent *ComponentPtr = AOIObjManager.CreateComponentObj();
	if ( NULL == ComponentPtr ) { return; }
	BoardPtr->AddBoardComponentPtr(ComponentPtr);
	PanelPtr->AddPanelComponentPtr(ComponentPtr);
	ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDumpBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CString str;	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("DumpProject.TXT"));
	FILE *pfile = ::_tfopen(str, TMode);
	ProjectPtr->DumpProjectFile(pfile);
	::fclose(pfile); pfile = NULL;

	::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnClonePanelBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }	

	CInputBoxWnd InputBox;
	InputBox.SetParam1(_T("Clone Number"), _T("Number:"), _T("1"));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	size_t i = 0;	
	const size_t CloneCount = ::_ttoi(InputBox.m_DataEdit1);
	for ( i=0; i<CloneCount; i++ )
	{
		ProjectPtr->AddProjectPanelPtr(PanelPtr, true);
		/*
		PanelPtr = PanelPtr->ClonePanelObj();
		if ( NULL == PanelPtr )
		{	break; }
		ProjectPtr->AddProjectPanelPtr(PanelPtr, false);
		*/
	}	
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnCloneBoardBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }

	const size_t BoardCount = PanelPtr->GetPanelBoardCount();
	if ( 0 == BoardCount ) { return ; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(BoardCount-1, true);
	if ( NULL == BoardPtr ) { return; }
	
	CInputBoxWnd InputBox;
	InputBox.SetParam1(_T("Clone Number"), _T("Number:"), _T("1"));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	size_t i = 0;	
	const size_t CloneCount = ::_ttoi(InputBox.m_DataEdit1);
	for ( i=0; i<CloneCount; i++ )
	{
		ProjectPtr->AddProjectBoardPtr(BoardPtr, true);
		/*
		BoardPtr = BoardPtr->CloneBoardObj();
		if ( NULL == BoardPtr )
		{	break; }
		ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
		PanelPtr->AddPanelBoardPtr(BoardPtr);
		*/
	}
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnCloneComponentBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }

	const size_t BoardCount = PanelPtr->GetPanelBoardCount();
	if ( 0 == BoardCount ) { return ; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(BoardCount-1, true);
	if ( NULL == BoardPtr ) { return; }

	const size_t ComponentCount = BoardPtr->GetBoardComponentCount();
	if ( 0 == ComponentCount ) { return ; }
	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(ComponentCount-1, true);
	if ( NULL == ComponentPtr ) { return; }

	ProjectPtr->AddProjectComponentPtr(ComponentPtr, true);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeletePanelBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	
	if ( NULL == PanelPtr ) { return; }	

	PanelPtr->SetPanelSelected(TRUE);
	LogOperCtrl.SaveLogProjectPanelSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectPanelSelected();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeleteBoardBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	
	if ( NULL == PanelPtr ) { return; }
	
	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	if ( 0 == PanelBoardCount ) { return; }

	CInputBoxWnd InputBox;
	InputBox.SetParam1(_T("Delete Number"), _T("Number:"), _T("1"));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	size_t i = 0;
	size_t DeleteCount = ::_ttoi(InputBox.m_DataEdit1);
	if ( DeleteCount > PanelBoardCount )
	{	DeleteCount = PanelBoardCount; }
	
	CAOIBoard *BoardPtr = NULL;
	
	for ( i=0; i<DeleteCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, true);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(TRUE);
	}
	LogOperCtrl.SaveLogProjectBoardSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectBoardSelected();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeleteComponentBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	
	if ( NULL == PanelPtr ) { return; }	
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return; }	
	CAOIComponent *ComponentPtr = BoardPtr->GetBoardComponentPtr(0, true);
	if ( NULL == ComponentPtr ) { return; }

	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->DeleteProjectComponentSelected();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnClearAllBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	
	ProjectPtr->ClearProjectAllObjects();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::ShowProjectInfo()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CString   str;
	const size_t NProjects = AOIDataCollect.GetProjectPtrCount();
	const size_t NFds = ProjectPtr->GetProjectFdCount();
	const size_t NMarks = ProjectPtr->GetProjectMarkCount();
	const size_t NBarcodes = ProjectPtr->GetProjectBarcodeCount();
	const size_t NPanels = ProjectPtr->GetProjectPanelCount();
	const size_t NBoards = ProjectPtr->GetProjectBoardCount();
	const size_t NFields = ProjectPtr->GetProjectFieldCount();
	const size_t NComponents = ProjectPtr->GetProjectComponentCount();
	const double TimeMs = AOIDataCollect.GetThreadSequenceThreadElapseTimems();

	str.Format(_T("Project:%u, FD:%u, Mark:%u, Barcode:%u, Panel:%u, Board:%u, Component:%u, Field:%u, Time=%.2fms"), NProjects, NFds, NMarks, NBarcodes, NPanels, NBoards, NComponents, NFields, TimeMs);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->BuildTreeWndCompolnent();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddProjectBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ProjectPtr ) { return; }	
	const size_t ProjectIndex = AOIDataCollect.GetProjectPtrCount();
	AOIDataCollect.AddProjectPtr(ProjectPtr, false);
	AOIDataCollect.SetActiveProjectIndex(ProjectIndex);
	m_ImageWnd.SetProjectPtr(AOIDataCollect.GetActiveProject());
	m_ProjectMapWnd.SetProjectPtr(AOIDataCollect.GetActiveProject(), true);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnCloneProjectBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	ProjectPtr = ProjectPtr->CloneProjectObj();	

	const size_t ProjectIndex = AOIDataCollect.GetProjectPtrCount();
	AOIDataCollect.AddProjectPtr(ProjectPtr, false);
	AOIDataCollect.SetActiveProjectIndex(ProjectIndex);
	m_ImageWnd.SetProjectPtr(AOIDataCollect.GetActiveProject());
	m_ProjectMapWnd.SetProjectPtr(AOIDataCollect.GetActiveProject(), true);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeleteProjectBtn() 
{
	// TODO: Add your control notification handler code here
//	CAOIProject *ProjectPtr = this->GetFirstProjectPtr();
//	if ( NULL == ProjectPtr ) { return; }	

//	AOIObjManager.DestroyProjectObj(m_ProjectPtrList[i]);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddFdBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = AOIObjManager.CreateFdObj();
	if ( NULL == FdPtr ) { return; }
	PanelPtr->AddPanelFdPtr(FdPtr);
	ProjectPtr->AddProjectFdPtr(FdPtr, false);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnCloneFdBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	if ( 0 == PanelCount ) { return ; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelCount-1, true);	
	if ( NULL == PanelPtr ) { return; }

	const size_t FdCount = PanelPtr->GetPanelFdCount();
	if ( 0 == FdCount ) { return ; }
	CAOIFd *FdPtr = PanelPtr->GetPanelFdPtr(FdCount-1, true);
	if ( NULL == FdPtr ) { return; }
	FdPtr->SetFdUniqueID(-1);
	ProjectPtr->AddProjectFdPtr(FdPtr, true);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeleteFdBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);	
	if ( NULL == PanelPtr ) { return; }
	
	CAOIFd *FdPtr = PanelPtr->GetPanelFdPtr(0, true);
	if ( NULL == FdPtr ) { return; }

	FdPtr->SetFdSelected(TRUE);
	ProjectPtr->DeleteProjectFdSelected();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnAddSBBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	

	CAOIBarcode *BarcodePtr = AOIObjManager.CreateBarcodeObj();
	if ( NULL == BarcodePtr ) { return; }
	ProjectPtr->AddProjectBarcodePtr(BarcodePtr, false);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnCloneSBBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	const size_t BarcodeCount = ProjectPtr->GetProjectBarcodeCount();
	if ( 0 == BarcodeCount ) { return ; }
	CAOIBarcode   *BarcodePtr = ProjectPtr->GetProjectBarcodePtr(BarcodeCount-1, true);	
	if ( NULL == BarcodePtr ) { return; }	

	ProjectPtr->AddProjectBarcodePtr(BarcodePtr, true);
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnDeleteSBBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CAOIBarcode   *BarcodePtr = ProjectPtr->GetProjectBarcodePtr(0, true);	
	if ( NULL == BarcodePtr ) { return; }	

	BarcodePtr->SetBarcodeSelected(true);
	LogOperCtrl.SaveLogProjectBarcodeSelectedDelete(ProjectPtr);	
	ProjectPtr->DeleteProjectBarcodeSelected();
	this->ShowProjectInfo();
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	TCHAR szFilters[]=_T("PRG Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString filename = dialog.GetPathName();
	DWORD dwCopyFlag = COPY_PROJECT_FOLDER_NO_OFFLINE;
	const bool bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
	if ( ProjectPtr->SaveProject(filename, filename, dwCopyFlag, bPartialCopy) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) 
	{ 
		ProjectPtr = AOIObjManager.CreateProjectObj();
		if ( NULL == ProjectPtr ) { return; }	
		const size_t ProjectIndex = AOIDataCollect.GetProjectPtrCount();
		AOIDataCollect.AddProjectPtr(ProjectPtr, false);
		AOIDataCollect.SetActiveProjectIndex(ProjectIndex);
		m_ImageWnd.SetProjectPtr(AOIDataCollect.GetActiveProject());		
	}

	TCHAR szFilters[]=_T("PRG Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString filename = dialog.GetPathName();

	const bool bLibraryMode = false;
	m_ImageWnd.SetProjectPtr(NULL);		
	m_ProjectMapWnd.SetProjectPtr(NULL, true);			
	if ( ProjectPtr->LoadProject(filename, filename, bLibraryMode) == false )
	{	
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	m_ImageWnd.SetProjectPtr(AOIDataCollect.GetActiveProject());		
	m_ProjectMapWnd.SetProjectPtr(ProjectPtr, true);	
	this->ShowProjectInfo();
	this->m_ProjectMapWnd.ShowWindow(SW_SHOW);

	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_OPEN, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CDebugWnd::BuildTreeWndCompolnent()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	this->m_TreeWndComponent.SetProjectPtr(ProjectPtr);
	this->m_TreeWndComponent.BuildTreeImageList();
	this->m_TreeWndComponent.BuildTreeWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CWnd *pWnd = NULL;
	pWnd = this->GetDlgItem(IDC_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		RECT rcWnd={0};
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);
		rcWnd.left = 4;
		rcWnd.right = cx-4;
		pWnd->MoveWindow(&rcWnd);
	}

	if ( this->m_TreeWndComponent.GetSafeHwnd() != NULL )
	{
		RECT rcWnd={0};
		m_TreeWndComponent.GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);
		rcWnd.bottom = cy-4;
		m_TreeWndComponent.MoveWindow(&rcWnd);
	}

	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT rcWnd={0};
		m_ImageWnd.GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);
		rcWnd.right = cx-4;
		rcWnd.bottom = cy-4;
		m_ImageWnd.MoveWindow(&rcWnd);
	}
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnClickTreeComponentWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CDebugWnd::OnLoadDibBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	
	
	this->m_ImageWnd.ReleaseImageBuffer();
	if ( this->m_Dib.Load(dialog.GetPathName()) == FALSE ) { return ; }

	IMAGE_SIZE ImageW = m_Dib.GetImageW();
	IMAGE_SIZE ImageH = m_Dib.GetImageH();
	IMAGE_SIZE ImageStep = m_Dib.GetImageBytePerLine();
	IMAGE_SIZE BitCount = m_Dib.GetImageBitCount();
	unsigned char *pDibBits = m_Dib.GetDIBBits();
	this->m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, pDibBits, false, true);
}

BOOL CDebugWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default	
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDebugWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	CDebugWnd::RedrawWnd();
}

void CDebugWnd::RedrawWnd()
{	
//	CClientDC dc(&m_ImageWnd);
//	HDC hDC = dc.GetSafeHdc();


//	::MoveToEx(hDC, 0, 300, NULL);
//	::LineTo(hDC, 600, 300);

}

void CDebugWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
	const UINT CtrlID = pWnd->GetDlgCtrlID();
	switch ( CtrlID )
	{
	case IDC_TREE_COMPONENT_WND:
		ExecTreeCtrlComponentMenu(point);		
		break;
	default:
		::AfxMessageBox(_T("CDebugWnd::OnContextMenu"));
		break;
	}
}

LRESULT CDebugWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CWnd *pWnd = NULL;
	UINT CtrlID=0;
	const bool InspectionDrawing = AOIDataCollect.GetInspectionDrawing();	
	switch ( message )
	{
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_REDRAW_PROJECT_MAP:
			if ( m_ProjectMapWnd.GetSafeHwnd()!=NULL && m_ProjectMapWnd.IsWindowVisible() == TRUE )
			{
				if ( NULL == lParam )
				{	this->m_ProjectMapWnd.RedrawWnd(TRUE);	}
				else if ( true == InspectionDrawing )
				{	this->m_ProjectMapWnd.RedrawWnd(TRUE);	}
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
				this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
				break;
			case LPARAM_DRAW_PROJECT_MODE_INSPECTING:
				this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
				break;
			}
			break;	
		}
		break;
	case MSG_CAMERA_CALLBACK:
		this->RetrieveCameraImage(wParam, lParam, true);
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		CWnd::CheckDlgButton(IDC_REPEAT_CHK, FALSE);
		AOIDataCollect.ExecSystemException(wParam, lParam);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:		
		DrawCtrlWnd(wParam, lParam);
		break;
	case MSG_TREE_WND_MOVE_TO_ACTIVE_OBJ:
		MoveToActiveObj(wParam, lParam);
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecGrabImage();
		break;	
	case MSG_MOTION_CALLBACK:		
		break;	
	case MSG_INSPECTION_CALLBACK:		
		if ( wParam == WPARAM_INSPECTION_FINISH )
		{			
			CDebugWnd::ShowProjectInfo();
			CDebugWnd::UpdatProjectMapWnd();
			this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_NORMAL);
			if ( CWnd::IsDlgButtonChecked(IDC_REPEAT_CHK) == TRUE )
			{
				CtrlID = IDC_GRAB_COMPONENT_BTN;
				pWnd = this->GetDlgItem(CtrlID);
				if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
				{	this->PostMessage(WM_COMMAND, MAKEWPARAM(CtrlID, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd()); }				
			}
			else
			{	
				AOIDataCollect.SetIsLockUIWnd(false);
				ExecGrabImage();					
			}
		}
		else if ( wParam == WPARAM_INSPECTION_PROJECT_TEST )
		{	
			CDebugWnd::ShowProjectInfo();	
			CDebugWnd::UpdatProjectMapWnd();
		}
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}

 BOOL CDebugWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{	
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( this->m_CameraID != CameraID ) { return FALSE; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return TRUE;	
	}
	/*
	IMAGE_PTR ImagePtr = NULL;
	size_t ImageW=0, ImageH=0, ImageStep, BitCount;	
	CameraCtrl.SetCameraToSendCallback(CameraID, FALSE);
	if ( CameraCtrl.GetCameraImage3(CameraID, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		return FALSE; 
	}*/	
	CameraCtrl.KeepCameraTempRingBuffer(CameraID);
	CameraCtrl.IncrementCameraCopyToHostCount(CameraID);	
	CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
	CameraCtrl.FreeCameraTempRingBuffer(CameraID);	
	
	IMAGE_PTR  ShowImagePtr = NULL;
	IMAGE_SIZE ShowImageW=0, ShowImageH=0, ShowImageStep, ShowBitCount;		
	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.GetSystemParameter().m_ImageDisplayMode;	
	if ( CameraCtrl.FillCameraImage(CameraID, ImageDisplayMode, ShowImageW, ShowImageH, ShowImageStep, ShowBitCount, ShowImagePtr) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return FALSE;
	}
	if ( AOIDataCollect.ExecEnhanceDisplayImage(ShowImageW, ShowImageH, ShowImageStep, ShowBitCount, ShowImagePtr, ShowImagePtr) == false )
	{
		JetMemory.free_func(ShowImagePtr);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return FALSE;
	}

	TPOINT2D  ImageRes;
	TPOINT3D  StagePos;
	TREGION4D StageRgn;
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(CameraID);	
	MotionCtrlPtr->GetCurrentPos(StagePos.x, StagePos.y, StagePos.z);
	StageRgn.minX = StagePos.x-(FovSizeW*0.5);
	StageRgn.maxX = StageRgn.minX+(FovSizeW);
	StageRgn.minY = StagePos.y-(FovSizeH*0.5);
	StageRgn.maxY = StageRgn.minY+(FovSizeH);
	this->m_ImageWnd.SetShowWndCenterLine(true);
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(ShowImageW, ShowImageH, ShowImageStep, ShowBitCount, ShowImagePtr, true, true);
	JetMemory.free_func(ShowImagePtr);

	m_ProjectMapWnd.RedrawWnd(FALSE);
	return TRUE;
}
//-------------------------------------------------------------------------------------//

void CDebugWnd::DrawCtrlWnd(WPARAM wParam, LPARAM lParam)
{
	UINT CtrlID = (UINT)(wParam);
	HDC  hDC = (HDC)(lParam);

	switch ( CtrlID )
	{
	case IDC_IMAGE_WND:
	//	::MoveToEx(hDC, 0, 300, NULL);
	//	::LineTo(hDC, 600, 300);
		break;
	}	
}

void CDebugWnd::MoveToActiveObj(WPARAM wParam, LPARAM lParam)
{
	TPOINT3D       StagePos;
	TREGION4D      StageRgn;
	CAOIFd        *FdPtr = AOIDataCollect.GetActiveFd();
	CAOIComponent *pComponent = AOIDataCollect.GetActiveComponent();
	CAOIWindow    *pWindow = AOIDataCollect.GetActiveWindow();
	if ( NULL != pWindow )
	{
		
	}	
	else if ( NULL != pComponent )
	{
		StagePos = pComponent->GetComponentStagePos();		
		m_CameraID = pComponent->GetComponentCameraID();
	}
	else if ( NULL != FdPtr )
	{
		if ( this->IsDlgButtonChecked(IDC_TO_TEACH_CHK) == TRUE )
		{	StagePos = FdPtr->GetFdTeachStagePos();	}
		else
		{	StagePos = FdPtr->GetFdStagePos();	}		
		m_CameraID = FdPtr->GetFdCameraID();
	}
	else
	{	return; }
	AOIDataCollect.MoveStageTo(StagePos.x, StagePos.y, StageRgn);	
}


BOOL CDebugWnd::ExecGrabImage()
{	
	MotionCtrlPtr->WaitForMotionStop();
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	FRAME_TYPE   FrameType;
	unsigned int FrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;//取像畫面的唯一碼	
	if ( AOIDataCollect.ExecGrabFrameImage(FrameUniqueID, FrameType) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return FALSE;
	}	
	return TRUE;
}

void CDebugWnd::ExecTreeCtrlComponentMenu(CPoint point)
{
	//::AfxMessageBox(_T("CTreeCtrlComponent::OnContextMenu"));
	CPoint CtrlPt = point;
	this->m_TreeWndComponent.ScreenToClient(&CtrlPt);
	UINT menuID = this->m_TreeWndComponent.GetPopupMenuID(CtrlPt);
	if ( NULL == menuID ) { return; }

	CMenu menu;		
	if ( menuID == 0 ) { return ; }
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return ;

}


BOOL CDebugWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
	this->BuildTreeWndCompolnent();

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	m_ImageWnd.SetShowCursorLine(false);
	m_ImageWnd.SetShowCursorInfo(true);
	m_ImageWnd.SetProjectPtr(ProjectPtr);
	m_ProjectMapWnd.SetProjectPtr(ProjectPtr, true);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());
	this->ShowProjectInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDebugWnd::OnProjectImageBtn() 
{
	// TODO: Add your control notification handler code here
	this->m_ProjectMapWnd.ShowWindow(SW_SHOW);
}

void CDebugWnd::OnGrabPanelFdBtn() 
{
	// TODO: Add your control notification handler code here
	this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());
	AOIDataCollect.SetTaskMode(TASK_ALIGN_PROJECT);
	AOIDataCollect.StartThreadSequenceThread(true);	
}

void CDebugWnd::OnResetThreadBtn() 
{
	// TODO: Add your control notification handler code here
	AOIDataCollect.IdleAllThread(false);
	AOIDataCollect.SetTaskMode(TASK_NONE);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
}

void CDebugWnd::OnGrabBoardFdBtn() 
{
	// TODO: Add your control notification handler code here
	this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	
	AOIDataCollect.SetTaskMode(TASK_ALIGN_PROJECT);
	AOIDataCollect.StartThreadSequenceThread(true);	
}

void CDebugWnd::OnGrabComponentBtn() 
{
	// TODO: Add your control notification handler code here
	//IDC_FIELD_MATRIX_CHK
	BOOL bMatrixChk = this->IsDlgButtonChecked(IDC_FIELD_MATRIX_CHK);
	BOOL bSaveField = CWnd::IsDlgButtonChecked(DEBUG_SAVE_FIELD_CHK);
	this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());

	if ( TRUE == bSaveField )
	{	AOIDataCollect.SetSaveOfflineFiles(true); }
	else
	{	AOIDataCollect.SetSaveOfflineFiles(false); }

	if ( FALSE == bMatrixChk )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT); }
	else
	{	AOIDataCollect.SetTaskMode(TASK_EXPORT_OFFLINE); }
	AOIDataCollect.StartThreadSequenceThread(true);
}

void CDebugWnd::OnGoOrgBtn() 
{
	// TODO: Add your control notification handler code here
	if ( MotionCtrlPtr->XYMoveTo(0, 0) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	MotionCtrlPtr->WaitForMotionStop();
}

void CDebugWnd::OnShowMemoryBtn() 
{
	// TODO: Add your control notification handler code here
	CString filename;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOILogDirectory(), _T("JetMemoryTmp.txt"));
	JetMemory.save_memory_node_list(filename);
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
}

void CDebugWnd::OnShowThreadBtn() 
{
	// TODO: Add your control notification handler code here
	CString filename;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ThreadInfoTmp.txt"));	
	if ( AOIDataCollect.SaveAllThreadState(filename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
}

void CDebugWnd::OnRebuildFieldBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	OFFLINE_FILE_MODE OfflineFileMode;
	BOOL bMatrix = CWnd::IsDlgButtonChecked(IDC_FIELD_MATRIX_CHK);
	if ( TRUE == bMatrix )
	{
		OfflineFileMode = OFFLINE_FILE_PROGRAM;
		if ( ProjectPtr->ClearProjectAllProgramField() == false )
		{
			JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
			return ;
		}		

		if ( ProjectPtr->CreateProjectProgramObject() == false )
		{
			JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
			return ;
		}
	}
	else
	{
		OfflineFileMode = OFFLINE_FILE_INSPECTION;
		if ( ProjectPtr->ClearProjectAllInspectionField() == false )
		{
			JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
			return ;
		}		

		if ( ProjectPtr->CreateProjectInspectionObject(FIELD_BUILD_RANDOM_PANEL, FIELD_BUILD_AREA_COMPONENT) == false )//case FIELD_BUILD_RANDOM_BOARD, case FIELD_BUILD_RANDOM_PROJECT:
		{
			JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
			return ;
		}
	}	
	
	CDebugWnd::ShowProjectInfo();	
}

void CDebugWnd::OnShowTimeBtn() 
{
	// TODO: Add your control notification handler code here
	CString filename;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ExecTimeTmp.txt"));	
	if ( AOIDataCollect.SaveObjExecTime(filename) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}	
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
}

void CDebugWnd::UpdatProjectMapWnd()
{
	m_ProjectMapWnd.UpdateProjectMapWnd();
}
