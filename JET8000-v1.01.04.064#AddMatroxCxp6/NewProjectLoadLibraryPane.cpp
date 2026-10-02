// NewProjectLoadLibraryPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectLoadLibraryPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneLoadLibrary dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneLoadLibrary::CNewProjectPaneLoadLibrary(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneLoadLibrary::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneLoadLibrary)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_LibraryLoaded = false;
	m_ModelImageWndBkClr = 0x000000;
	m_StopModelListBeSelected = FALSE;	
	m_NewProjectMode = NEW_PROJECT_ONLINE;
	m_EnableMultiDistrictMode = false;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneLoadLibrary)
	DDX_Control(pDX, LIBRARY_MODEL_IMAGE_WND, m_ModelImageWnd);
	DDX_Control(pDX, LIBRARY_MODEL_LIST_WND, m_ModelListWnd);
	DDX_Control(pDX, LIBRARY_MODEL_FRAME_INDEX_COMBO, m_ModelFrameIndexCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneLoadLibrary, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneLoadLibrary)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, LIBRARY_MODEL_LIST_WND, OnItemchangedModelListWnd)
	ON_BN_CLICKED(LIBRARY_LOAD_LIBRARY_BTN, OnLoadLibraryBtn)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(LIBRARY_MODEL_FRAME_INDEX_COMBO, OnSelchangeModelBKImageIndexCombox)
	ON_BN_CLICKED(LIBRARY_LOAD_PROJECT_BTN, OnLoadProjectBtn)
	ON_BN_CLICKED(LIBRARY_LOAD_SERVER_LIBRARY_BTN, OnLoadServerLibraryBtn)
	ON_BN_CLICKED(LIBRARY_SELECT_SERVER_LIBRARY_BTN, OnSelectServerLibraryBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneLoadLibrary message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneLoadLibrary::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ModelImageWnd.GetClientRect(&m_ModelImageWndRect);
	m_ModelImageWndMemDC.CreateMemDC(&m_ModelImageWnd, m_ModelImageWndBkClr);

	JetAPI::InitialListCtrl(m_ModelListWnd);
	AOIDataDefine.BuildProjectMapIndexCombox(m_ModelFrameIndexCombox);	
	JetAPI::SetComboxCurSel(m_ModelFrameIndexCombox, 0);

	SwitchMultiLanguage();
	BuildModelListWndHeader();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearModelListWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ModelListWnd.GetSafeHwnd() == NULL ) { return; }
	if ( m_ModelImageWnd.GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;
	RECT  ImageWndRect={0};
	const int MarginX = 4;
	const int MarginY = 4;
	ImageWndRect.right = cx;
	ImageWndRect.bottom = cy;

	WndPtr = CWnd::GetDlgItem(LIBRARY_MODEL_NAME_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{	
		SIZE WndSize={0};
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.right = cx-MarginX;	
		WndRect.left = WndRect.right-WndSize.cx;
		WndPtr->MoveWindow(&WndRect, FALSE);		
	}

	if ( m_ModelImageWnd.GetSafeHwnd() != NULL )
	{
		SIZE WndSize={0};
		RECT WndRect={0};
		m_ModelImageWnd.GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.right = cx-MarginX;
		WndRect.left = WndRect.right-WndSize.cx;
		m_ModelImageWnd.MoveWindow(&WndRect, FALSE);	
		m_ModelImageWnd.GetClientRect(&m_ModelImageWndRect);
		m_ModelImageWndMemDC.CreateMemDC(&m_ModelImageWnd, m_ModelImageWndBkClr);
		ImageWndRect = WndRect;
	}

	if ( m_ModelListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ModelListWnd.GetWindowRect(&WndRect);
		ScreenToClient(&WndRect);		
		WndRect.left  = 0 + MarginX;
		//WndRect.top   = 0 + MarginY;
		WndRect.right = ImageWndRect.left-MarginX;		
		WndRect.bottom = cy-MarginY;
		m_ModelListWnd.MoveWindow(&WndRect, FALSE);	
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_LOAD_LIBRARY");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_LOAD_LIBRARY;
	WndKey = _T("IDD_NEW_PROJECT_PANE_LOAD_LIBRARY");
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
	WndID = LIBRARY_MODEL_LIST_LABEL;
	WndKey = _T("LIBRARY_MODEL_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LIBRARY_LOAD_LIBRARY_BTN;
	WndKey = _T("LIBRARY_LOAD_LIBRARY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LIBRARY_LOAD_PROJECT_BTN;
	WndKey = _T("LIBRARY_LOAD_PROJECT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LIBRARY_LOAD_SERVER_LIBRARY_BTN;
	WndKey = _T("LIBRARY_LOAD_SERVER_LIBRARY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LIBRARY_SELECT_SERVER_LIBRARY_BTN;
	WndKey = _T("LIBRARY_SELECT_SERVER_LIBRARY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LIBRARY_MODEL_FRAME_INDEX_LABEL;
	WndKey = _T("LIBRARY_MODEL_FRAME_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneLoadLibrary::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_LOAD_LIBRARY");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneLoadLibrary::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::SetProjectPtr(CAOIProject *ProjectPtr)
{
	this->m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::CheckProjectPtr(CAOIProject *Ptr)
{
	if ( AOIDataCollect.CheckProjectPtr(Ptr) == false )
	{	
		m_ErrorString = AOIDataCollect.GetErrorString();
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneLoadLibrary::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		//if ( false == m_LibraryLoaded )
		//{	ExecLoadLibrary();	}
	}
	PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, FALSE);
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecNextPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecPrevPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecFinishPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ReInitialPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ClearModelListWnd()
{
	m_StopModelListBeSelected = TRUE;
	JetAPI::ClearListCtrl(m_ModelListWnd, FALSE);
	m_StopModelListBeSelected = FALSE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::BuildModelListWnd()
{	
	ClearModelListWnd();
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( CheckProjectPtr(ProjectPtr) == false )	
	{	return false;	}

	size_t        i=0;
	size_t        LandCount=0;
	int           nItem = 0;
	int           nSubItem=0;
	CString       ItemText;
	MODEL_TYPE    ModelType;
	CAOIModel    *ModelPtr = NULL;
	CListCtrl    &ListWnd = m_ModelListWnd;
	const size_t ModelCount = ProjectPtr->GetProjectModelCount();

	nItem = 0;
	ListWnd.SetRedraw(FALSE);
	m_StopModelListBeSelected = TRUE;
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();		

		nSubItem = 0;

		ItemText.Format(_T("%d"), nItem+1);
		ListWnd.InsertItem(nItem, ItemText);
		ListWnd.SetItemData(nItem, (DWORD_PTR)(ModelPtr));

		ListWnd.SetItemText(nItem, nSubItem, ItemText);
		nSubItem ++;

		ItemText = ModelPtr->GetModelName();
		ListWnd.SetItemText(nItem, nSubItem, ItemText);
		nSubItem ++;

		ItemText = AOIDataDefine.GetModelTypeText(ModelType);		
		ListWnd.SetItemText(nItem, nSubItem, ItemText);
		nSubItem ++;

		LandCount = ModelPtr->GetModelLandCount();
		ItemText.Format(_T("%d"), LandCount);
		ListWnd.SetItemText(nItem, nSubItem, ItemText);
		nSubItem ++;

		nItem ++;
	}
	m_StopModelListBeSelected = FALSE;
	ListWnd.SetRedraw(TRUE);

	if ( nItem > 0 ) 
	{
		int nSelItem = 0;
		HDC hDC = m_ModelImageWndMemDC.GetSafeHdc();
		ModelPtr = (CAOIModel*)(ListWnd.GetItemData(nSelItem));
		ListWnd.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
		DrawModelImage(hDC, ModelPtr);
		DrawModelImageWnd();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::BuildModelListWndHeader()
{
	CListCtrl &ListWnd = m_ModelListWnd;
	JetAPI::ClearListCtrlHeaderList(ListWnd);

	int       i=0;
	int       nCol = 0;
	int       Width=0;
	const int NColumns = 5;
	RECT      ListRect={0};
	CString   SubText;
	
	ListWnd.GetClientRect(&ListRect);
	Width = (ListRect.right-ListRect.left-32)/(NColumns);
	if ( Width < 32 ) { Width = 32; }

	nCol = 0;

	SubText = _T("Index");	
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(nCol, SubText,LVCFMT_CENTER,Width);
	nCol ++;
	
	SubText = _T("Model");	
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(nCol, SubText,LVCFMT_CENTER,Width*2);
	nCol ++;

	SubText = _T("Type");	
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(nCol, SubText,LVCFMT_CENTER,Width);
	nCol ++;

	SubText = _T("Land Count");	
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(nCol, SubText,LVCFMT_CENTER,Width);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnItemchangedModelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if ( TRUE == m_StopModelListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	HDC hDC = m_ModelImageWndMemDC.GetSafeHdc();
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelListWnd.GetItemData(nItem));
	DrawModelImage(hDC, ModelPtr);
	RedrawWnd();

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecLoadLibrary()
{	
	CString      str;
	CString      LibraryName;
	CString      filenameTmp;
	CAOIProject *LibraryPtr = NULL;

	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( CheckProjectPtr(ProjectPtr) == false )	
	{	return false;	}

	LibraryName = AOIDataCollect.GetAOILibraryName();
	LibraryPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == LibraryPtr )
	{
		m_ErrorString = _T("Error, Create Project Object Fault");		
		return false;
	}	
	
	if ( AOIDataCollect.CreateTempProjectFile(LibraryName, filenameTmp) == false )
	{
		m_ErrorString = AOIDataCollect.GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}

	const bool bLibraryMode = true;
	if ( LibraryPtr->LoadProject(filenameTmp, LibraryName, bLibraryMode) == false )
	{
		m_ErrorString = LibraryPtr->GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}

	const size_t ModelCount = ProjectPtr->GetProjectModelCount();
	if ( ModelCount > 0 ) 
	{
		str = _T("Do you want to clear the project library?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	ProjectPtr->ClearProjectAllModels();	}		
	}

	if ( ProjectPtr->MergeProjectLibrary(LibraryPtr) == false )
	{
		m_ErrorString = ProjectPtr->GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}
	AOIObjManager.DestroyProjectObj(LibraryPtr);
	m_LibraryLoaded = true;
	BuildModelListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecLoadProject()
{
	CString      str;
	CString      LibraryName;
	CString      filenameTmp;
	CAOIProject *LibraryPtr = NULL;

	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( CheckProjectPtr(ProjectPtr) == false )	
	{	return false;	}
	
	CString ShowName = ProjectPtr->GetProjectShowName();
	TCHAR szFilename[MAX_JET_PATH]=_T("");
	TCHAR szFilters[]=_T("PRG Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters);
	//::_tcscpy(szFilename, ShowName);
	//dialog.m_ofn.lpstrFile  = szFilename;
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	LibraryName = dialog.GetPathName();
	if ( LibraryName.CompareNoCase(ShowName) == 0 ) 
	{
		m_ErrorString.Format(_T("Error, It can not the same project\n[%s]"), LibraryName);
		return false;
	}
	LibraryPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == LibraryPtr )
	{
		m_ErrorString = _T("Error, Create Project Object Fault");		
		return false;
	}	

	if ( AOIDataCollect.CreateTempProjectFile(LibraryName, filenameTmp) == false )
	{
		m_ErrorString = AOIDataCollect.GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}

	const bool bLibraryMode = true;
	if ( LibraryPtr->LoadProject(filenameTmp, LibraryName, bLibraryMode) == false )
	{
		m_ErrorString = LibraryPtr->GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}

	const size_t ModelCount = ProjectPtr->GetProjectModelCount();
	if ( ModelCount > 0 ) 
	{
		str = _T("Do you want to clear the project library?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	ProjectPtr->ClearProjectAllModels();	}		
	}

	if ( ProjectPtr->MergeProjectLibrary(LibraryPtr) == false )
	{
		m_ErrorString = ProjectPtr->GetErrorString();
		AOIObjManager.DestroyProjectObj(LibraryPtr);		
		return false;
	}
	AOIObjManager.DestroyProjectObj(LibraryPtr);
	m_LibraryLoaded = true;
	BuildModelListWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::ExecLoadServerLibrary()
{
	CString      str;	
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( CheckProjectPtr(ProjectPtr) == false ) 
	{	return false; }		

	const size_t ModelCount = ProjectPtr->GetProjectModelCount();	
	if ( ModelCount > 0 ) 
	{
		str = _T("Do you want to clear the project library?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	ProjectPtr->ClearProjectAllModels();	}		
	}

	CString ServerFolder = AOIDataCollect.GetAOIServerLibraryFolder();
	if ( ProjectPtr->LoadProjectServerLibrary(ServerFolder) == false )
	{
		m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

	m_LibraryLoaded = true;
	BuildModelListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadLibrary::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnLoadLibraryBtn() 
{
	// TODO: Add your control notification handler code here
	if ( ExecLoadLibrary() == false )
	{	JetAPI::ShowMessageBox(m_ErrorString); }
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::RedrawWnd()
{
	DrawModelImageWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::DrawModelImageWnd()
{
	CClientDC dc(&m_ModelImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ModelImageWndMemDC.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC ) { return; }
	RECT WndRect = m_ModelImageWndRect;

	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::DrawModelImage(HDC hDC, CAOIModel *ModelPtr)
{
	if ( NULL==hDC || NULL==ModelPtr ) 
	{
		CWnd::SetDlgItemText(LIBRARY_MODEL_NAME_EDIT, _T(""));
		return; 
	}
	RECT WndRect = m_ModelImageWndRect;	
	DRAW_MODEL_MODE DrawModelMode = DRAW_MODEL_EDIT;
	TMODEL_DRAW_PARAM DrawParam;

	CDib  dib;
	bool   IsOK=true;
	double Zoom = 1.0;
	int    nAlign = 4;
	double ImageResX = 10.0;
	double ImageResY = 10.0;
	int    ImageIndex = 0;	
	TREGION4D ModelRgn;
	double    ModelRgnW = 0;
	double    ModelRgnH = 0;		

	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;		
	CString    ModelName = ModelPtr->GetModelName();
	CString    ModelBkImageName;
	const bool  bUseGeneralBKImageIndex=true;
	const unsigned int DefaultBKImageIndex = (unsigned int)(JetAPI::GetComboxCurSelData(m_ModelFrameIndexCombox));
	//ImageResX = ModelPtr->GetModelBKImageResX();
	//ImageResY = ModelPtr->GetModelBKImageResY();
	
	ModelPtr->GetModelTotalRegion(ModelRgn);
	ModelRgnW = ModelRgn.GetWidth();
	ModelRgnH = ModelRgn.GetHeight();

	CWnd::SetDlgItemText(LIBRARY_MODEL_NAME_EDIT, ModelName);

	HBRUSH hBrush = ::CreateSolidBrush(m_ModelImageWndBkClr);
	::FillRect(hDC, &WndRect, hBrush);
	::DeleteObject(hBrush);	hBrush = NULL;

	ImageIndex = ModelPtr->GetModelBKImageIndex();
	if ( true == bUseGeneralBKImageIndex )
	{	ImageIndex = DefaultBKImageIndex; }
	ModelBkImageName = ModelPtr->GetModelBKImageFilename(ImageIndex);	
	IsOK = ImageAPI.LoadImage(ModelBkImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true);
	if ( false == IsOK )
	{
		if ( true==bUseGeneralBKImageIndex && 0!=DefaultBKImageIndex )
		{
			ImageIndex = 0;//重新取編號0的底圖
			ModelBkImageName = ModelPtr->GetModelBKImageFilename(ImageIndex);
			IsOK = ImageAPI.LoadImage(ModelBkImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true);
		}
	}
	if ( true == IsOK )
	{		
		if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr) == true ) 
		{ 
			if ( dib.SetImage(ImagePtr, ImageW, ImageH, ImageStep, BitCount, true) == true )
			{	
				int nWidth  = (int)(ImageW);
				int nHeight = (int)(ImageH);
				int nDestX = 0;
				int nDestY = 0;
				int nDestW = nWidth;
				int nDestH = nHeight;
				ImageResX = ModelRgnW/nWidth;
				ImageResY = ModelRgnH/nHeight;
				ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, WndRect, 1.2, Zoom);
				nDestW = (int)(nWidth/Zoom);
				nDestH = (int)(nHeight/Zoom);
				nDestX = (WndRect.right-WndRect.left-nDestW)/2;
				nDestY = (WndRect.bottom-WndRect.top-nDestH)/2;
				// populate the thumbnail bitmap bits
				int OldMode = ::SetStretchBltMode(hDC, HALFTONE);
				::StretchDIBits(hDC, nDestX, nDestY, 
							nDestW, nDestH, 
							0, 0, 
							nWidth,
							nHeight, 
							dib.GetDIBBits(), 
							dib.GetDIBInfo(), 
							BI_RGB, 
							SRCCOPY);
				::SetStretchBltMode(hDC, OldMode);
		
			}
		}
		ImageW = ImageH = 0;
		JetMemory.free_func(ImagePtr);
	}	
	
	//ImageAPI.CalcImageWndFitZoom(W, H, 
	//JetAPI::
	DrawParam.WndRect = WndRect;
	DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
	DrawParam.Scale = Zoom;//*1.2;		
	DrawParam.ViewOffsetX =  0;
	DrawParam.ViewOffsetY =  0;	
	DrawParam.ResolutionX = ImageResX;
	DrawParam.ResolutionY = ImageResY;
	DrawParam.ShowEditLine = false;
	ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	RedrawWnd();
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnSelchangeModelBKImageIndexCombox()
{
	const int nItem = (int)(m_ModelListWnd.GetNextItem(-1, LVNI_SELECTED));
	if ( nItem < 0 ) { return; }

	HDC hDC = m_ModelImageWndMemDC.GetSafeHdc();
	CAOIModel *ModelPtr = (CAOIModel*)(m_ModelListWnd.GetItemData(nItem));
	DrawModelImage(hDC, ModelPtr);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnLoadProjectBtn() 
{
	// TODO: Add your control notification handler code here
	if ( ExecLoadProject() == false )
	{	JetAPI::ShowMessageBox(m_ErrorString); }
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnLoadServerLibraryBtn() 
{
	// TODO: Add your control notification handler code here
	if ( ExecLoadServerLibrary() == false )
	{	JetAPI::ShowMessageBox(m_ErrorString); }	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadLibrary::OnSelectServerLibraryBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( CheckProjectPtr(ProjectPtr) == false ) 
	{	return ; }
	TProjectParameter &ProParam = ProjectPtr->GetProjectParameter();
	//TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	CString str;
	CString str2;
	CString Folder;
	CString ServerFolder = AOIDataCollect.GetAOIServerFolder();
	CString ServerLibraryFolder = AOIDataCollect.GetAOIServerLibraryFolder();
	Folder = ServerLibraryFolder;
	if ( JetAPI::OpenFolderDialog(this, Folder) == false ) { return; }
	if ( Folder.CompareNoCase(ServerLibraryFolder) == 0 )
	{	return; }
	
	CString TempFolder;
	CString LastFolder;
	CString LibraryFolder = Folder;	
	JetAPI::ExtractTopFolder(Folder, LastFolder);
	TempFolder.Format(_T("%s\\%s"), ServerFolder, LastFolder);
	if ( TempFolder.CompareNoCase(LibraryFolder) != 0 )
	{
		str = _T("Error, Can not set the folder be Serve Library Folder");		
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s [%s]"), str, Folder);
		JetAPI::ShowMessageBox(str2);
		return;
	}
	
	std::wstring wFolder;
	JetAPI::TCHAR2wstring(LastFolder, wFolder);
	ProParam.m_ProjectServerLibraryGroup = wFolder;
	ProjectPtr->CreateProjectServerLibraryFolder();
	return;
}
//-------------------------------------------------------------------------------------//