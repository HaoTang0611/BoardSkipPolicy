// ProjectListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectListWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
int AllFileListCompareID = 0;
//-------------------------------------------------------------------------------------//
int CALLBACK AllFileListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK AllFileListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CProjectListWnd* pProjectListWnd = (CProjectListWnd*)lParamSort;
	return pProjectListWnd->CompareAllFileList(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectListWnd dialog
//-------------------------------------------------------------------------------------//
CProjectListWnd::CProjectListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StopAllListBeSelected = false;
	m_StopLatestListBeSelected = false;
	m_AllFileListCompareID = -1;
	m_AllFileListMode = PROLIST_ALL_FILE_FOLDER_RADIO;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectListWnd)
	DDX_Control(pDX, PROLIST_PROJECT_MAP_WND, m_ProjectMapWnd);
	DDX_Control(pDX, PROLIST_LATEST_FILE_LIST_WND, m_LatestFileListWnd);
	DDX_Control(pDX, PROLIST_ALL_FILE_LIST_WND, m_AllFileListWnd);
	DDX_Control(pDX, PROLIST_MAP_FRAME_COMBO, m_MapIndexCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, PROLIST_LATEST_FILE_LIST_WND, OnItemchangedLatestFileListWnd)
	ON_NOTIFY(NM_DBLCLK, PROLIST_LATEST_FILE_LIST_WND, OnDblclkLatestFileListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROLIST_ALL_FILE_LIST_WND, OnItemchangedAllFileListWnd)
	ON_NOTIFY(NM_DBLCLK, PROLIST_ALL_FILE_LIST_WND, OnDblclkAllFileListWnd)
	ON_BN_CLICKED(PROLIST_ALL_FILE_FOLDER_BTN, OnAllFileFolderBtn)
	ON_BN_CLICKED(PROLIST_LATEST_FILE_OFFLINE_CHK, OnLatestFileOfflineChk)
	ON_NOTIFY(LVN_COLUMNCLICK, PROLIST_ALL_FILE_LIST_WND, OnColumnclickAllFileListWnd)
	ON_BN_CLICKED(PROLIST_ALL_FILE_FOLDER_RADIO, OnAllFileFolderRadio)
	ON_BN_CLICKED(PROLIST_ALL_FILE_OPEN_CODE_RADIO, OnAllFileOpenCodeRadio)
	ON_NOTIFY(NM_CLICK, PROLIST_LATEST_FILE_LIST_WND, OnClickLatestFileListWnd)
	ON_NOTIFY(NM_CLICK, PROLIST_ALL_FILE_LIST_WND, OnClickAllFileListWnd)
	ON_CBN_SELCHANGE(PROLIST_MAP_FRAME_COMBO, OnSelchangeMapFrameCombo)
	ON_BN_CLICKED(PROLIST_ALL_FILE_SEARCH_RADIO, OnAllFileSearchRadio)
	ON_BN_CLICKED(PROLIST_SMALL_MAP_CHK, OnSmallMapChk)
	ON_BN_CLICKED(PROLIST_UPDATE_LATEST_FILE_BTN, OnUpdateLatestFileBtn)
	ON_BN_CLICKED(PROLIST_DELETE_FILE_BTN, OnDeleteFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	JetAPI::InitialListCtrl(m_AllFileListWnd);	
	JetAPI::InitialListCtrl(m_LatestFileListWnd);	
	SwitchMultiLanguage();
	BuildAllFileListWndHeader();
	BuildLatestFileListWndHeader();
	
	CString Folder;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	Folder = AOIDataCollect.GetOpenProjectDirectory();
	SetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	AOIDataDefine.BuildProjectMapIndexCombox(m_MapIndexCombox);
	
	OPEN_PROJECT_MODE OpenProjectMode = SysParam.m_OpenProjectMode;
	JetAPI::SetComboxCurSel(m_MapIndexCombox, SysParam.m_OpenProjectMapIndex);
	switch ( OpenProjectMode )
	{
	case OPEN_PROJECT_CODE:	m_AllFileListMode = PROLIST_ALL_FILE_OPEN_CODE_RADIO;	break;	
	case OPEN_PROJECT_FILE:
	default:
		m_AllFileListMode = PROLIST_ALL_FILE_FOLDER_RADIO;
		break;
	}
	CheckDlgButton(m_AllFileListMode, TRUE);
	CheckDlgButton(PROLIST_SMALL_MAP_CHK, TRUE);
#ifdef OFFLINE_VERSION
	CheckDlgButton(PROLIST_LATEST_FILE_OFFLINE_CHK, TRUE);
#else
	CheckDlgButton(PROLIST_LATEST_FILE_OFFLINE_CHK, FALSE);
#endif//OFFLINE_VERSION

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::PostMessage(MSG_SELF_WND_EXTRA_MESSAGE, WPARAM_FIRST_UI_CALLBACK, NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_StopAllListBeSelected = true;
	JetAPI::ClearListCtrl(m_AllFileListWnd, FALSE);
	m_StopAllListBeSelected = false;

	m_StopLatestListBeSelected = true;
	JetAPI::ClearListCtrl(m_LatestFileListWnd, FALSE);
	m_StopLatestListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	CWnd     *WndPtr=NULL;
	const int MarginW=4;
	const int MarginH=4;

	WndPtr = GetDlgItem(PROLIST_SELECT_FILENAME_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;		
		WndPtr->MoveWindow(&WndRect);
	}

	if ( m_ProjectMapWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ProjectMapWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_ProjectMapWnd.MoveWindow(&WndRect);
		m_ProjectMapWnd.ShowFittedZoom();
	}

	if ( m_AllFileListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_AllFileListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		//WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		m_AllFileListWnd.MoveWindow(&WndRect);
	}	
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1000;
	lpMMI->ptMinTrackSize.y =  640;
}
//-------------------------------------------------------------------------------------//
CString CProjectListWnd::GetSelectedFilename() const
{
	return m_SelectedFilename;
}
//-------------------------------------------------------------------------------------//
int CProjectListWnd::GetAllFileListCompareID() const
{
	return m_AllFileListCompareID;
}
//-------------------------------------------------------------------------------------//
int CProjectListWnd::CompareAllFileList(size_t index1, size_t index2)
{
	const int ColID = GetAllFileListCompareID();
	const size_t FileCount = m_AllFilenameList.size();
	if ( index1>=FileCount || index2>=FileCount ) { return 0; }
	int     Res=0;
	CTime   time1;
	CTime   time2;	
	WIN32_FIND_DATA *Finder1=&(m_AllFilenameList[index1]);
	WIN32_FIND_DATA *Finder2=&(m_AllFilenameList[index2]);
	switch ( ColID )
	{
	case 0://filename
		Res = ::_tcscmp(Finder1->cFileName, Finder2->cFileName);
		break;
	case 1:
		time1 = Finder1->ftLastWriteTime;
		time2 = Finder2->ftLastWriteTime;		
		if ( time1 > time2 ) { Res = 1; }
		else if ( time1 < time2 ) { Res = -1; }
		else {	Res=0; }
		break;
	default:
		Res = 0;
		break;
	}
	if ( 1 == m_AllFileListSortMode ) 
	{	return Res; }
	Res = Res*-1;	
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_LIST_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_LIST_WND;
	WndKey = _T("IDD_PROJECT_LIST_WND");
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
	WndID = PROLIST_SMALL_MAP_CHK;
	WndKey = _T("PROLIST_SMALL_MAP_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_MAP_FRAME_LABEL;
	WndKey = _T("PROLIST_MAP_FRAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROLIST_LATEST_FILE_LABEL;
	WndKey = _T("PROLIST_LATEST_FILE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_ALL_FILE_FOLDER_RADIO;
	WndKey = _T("PROLIST_ALL_FILE_FOLDER_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_ALL_FILE_OPEN_CODE_RADIO;
	WndKey = _T("PROLIST_ALL_FILE_OPEN_CODE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_ALL_FILE_SEARCH_RADIO;
	WndKey = _T("PROLIST_ALL_FILE_SEARCH_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROLIST_LATEST_FILE_OFFLINE_CHK;
	WndKey = _T("PROLIST_LATEST_FILE_OFFLINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROLIST_UPDATE_LATEST_FILE_BTN;
	WndKey = _T("PROLIST_UPDATE_LATEST_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_DELETE_FILE_BTN;
	WndKey = _T("PROLIST_DELETE_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = PROLIST_ALL_FILE_LABEL;
	WndKey = _T("PROLIST_ALL_FILE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROLIST_ALL_FILE_FOLDER_LABEL;
	WndKey = _T("PROLIST_ALL_FILE_FOLDER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildAllFileListWnd()
{
	bool IsOK = true;
	switch ( m_AllFileListMode )
	{
	case PROLIST_ALL_FILE_FOLDER_RADIO:
		IsOK = BuildAllFileListWnd_Folder();
		break;
	case PROLIST_ALL_FILE_OPEN_CODE_RADIO:
		IsOK = BuildAllFileListWnd_OpenCode();
		break;
	case PROLIST_ALL_FILE_SEARCH_RADIO:
		IsOK = BuildAllFileListWnd_Search();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildAllFileListWnd_Folder()
{
	CString        Folder;
	CThisListCtrl_20 &ListCtrl = m_AllFileListWnd;
	m_AllFileListCompareID = -1;
	m_StopAllListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopAllListBeSelected = false;
	m_AllFilenameList.clear();
	GetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	const int len = Folder.GetLength();
	if ( 0 == len ) { return true; }

	JetAPI::ListFilesInFolder(Folder, _T("PRG"), m_AllFilenameList);

	size_t          i=0;	
	CString         str;
	int             nItem=0;
	int             nSubItem=0;	
	CTime           cTime;
	CString         Filename;
	WIN32_FIND_DATA FindFile;
	const size_t    FilenameCount = m_AllFilenameList.size();

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopAllListBeSelected = true;
	for ( i=0; i<FilenameCount; i++ )
	{
		nSubItem = 0;
		FindFile = m_AllFilenameList[i];
		Filename = FindFile.cFileName;
		
		str.Format(_T("%d"), i+1);
		str = Filename;
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//
		//ftCreationTime;
		//cTime = FindFile.ftCreationTime;
		//ftLastAccessTime;
		//cTime = FindFile.ftLastAccessTime;
		//ftLastWriteTime;

		cTime = FindFile.ftLastWriteTime;
		//JetAPI::GetTime(str, cTime);
		JetAPI::FormatTime(FORMAT_TIME_01, cTime, str);//格式化時間
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		nItem ++;
	}
	m_StopAllListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildAllFileListWnd_OpenCode()
{
	CString        Folder;
	CThisListCtrl_20 &ListCtrl = m_AllFileListWnd;
	m_AllFileListCompareID = -1;
	m_StopAllListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopAllListBeSelected = false;
	m_AllFilenameList.clear();
	Folder = AOIDataCollect.GetAOIProjectDirectory();
	SetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	const int len = Folder.GetLength();
	if ( 0 == len ) { return true; }

	std::wstring  wsValue;
	CString       strValue;
	CString       strLabel;
	CString       strCaption;
	CInputBoxWnd  InputBox;
	std::vector<CString> ProjectNameList;

	strCaption = _T("Input Project Open Code");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Open Code");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true; }
	strValue = InputBox.m_DataEdit1;	
	JetAPI::TCHAR2wstring(strValue, wsValue);
	if ( AOIDataCollect.FindProjectListByCode(wsValue, ProjectNameList) == false )
	{
		strLabel = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(strLabel);
		return true;
	}
	const size_t ProjectNameCount = ProjectNameList.size();
	if ( 0 == ProjectNameCount ) 
	{	return true; }

	if ( JetAPI::ListFilesByFileName(Folder, ProjectNameList, m_AllFilenameList) == false )
	{	return true; }

	size_t          i=0;	
	CString         str;
	int             nItem=0;
	int             nSubItem=0;	
	CTime           cTime;
	CString         Filename;
	WIN32_FIND_DATA FindFile;
	const size_t    FilenameCount = m_AllFilenameList.size();

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopAllListBeSelected = true;
	for ( i=0; i<FilenameCount; i++ )
	{
		nSubItem = 0;
		FindFile = m_AllFilenameList[i];
		Filename = FindFile.cFileName;
		
		str.Format(_T("%d"), i+1);
		str = Filename;
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//
		//ftCreationTime;
		//cTime = FindFile.ftCreationTime;
		//ftLastAccessTime;
		//cTime = FindFile.ftLastAccessTime;
		//ftLastWriteTime;

		cTime = FindFile.ftLastWriteTime;
		//JetAPI::GetTime(str, cTime);
		JetAPI::FormatTime(FORMAT_TIME_01, cTime, str);//格式化時間
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		nItem ++;
	}
	m_StopAllListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	

	if ( nItem > 0 ) 
	{	ListCtrl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildAllFileListWnd_Search()
{
	CString        Folder;
	CThisListCtrl_20 &ListCtrl = m_AllFileListWnd;
	m_AllFileListCompareID = -1;
	m_StopAllListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopAllListBeSelected = false;
	m_AllFilenameList.clear();
	GetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	const int len = Folder.GetLength();
	if ( 0 == len ) { return true; }

	size_t        i=0;
	CString       strValue;
	CString       strLabel;
	CString       strCaption;
	CInputBoxWnd  InputBox;
	std::vector<CString> FileList;
	std::vector<CString> ProjectNameList;

	JetAPI::ListFilesInFolder(Folder, _T("PRG"), FileList);
	const size_t FileCount = FileList.size();
	if ( 0 == FileCount ) { return true; }

	strCaption = _T("Input Search Name");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Search Name");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return true; }
	strValue = InputBox.m_DataEdit1;	
	
	for ( i=0; i<FileCount; i++ )
	{
		if ( JetAPI::FindTextInString(strValue, FileList[i]) == false )
		{	continue; }
		ProjectNameList.push_back(FileList[i]);
	}
	const size_t ProjectNameCount = ProjectNameList.size();
	if ( 0 == ProjectNameCount ) 
	{	return true; }

	if ( JetAPI::ListFilesByFileName(Folder, ProjectNameList, m_AllFilenameList) == false )
	{	return true; }
	
	CString         str;
	int             nItem=0;
	int             nSubItem=0;	
	CTime           cTime;
	CString         Filename;
	WIN32_FIND_DATA FindFile;
	const size_t    FilenameCount = m_AllFilenameList.size();

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopAllListBeSelected = true;
	for ( i=0; i<FilenameCount; i++ )
	{
		nSubItem = 0;
		FindFile = m_AllFilenameList[i];
		Filename = FindFile.cFileName;
		
		str.Format(_T("%d"), i+1);
		str = Filename;
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//
		//ftCreationTime;
		//cTime = FindFile.ftCreationTime;
		//ftLastAccessTime;
		//cTime = FindFile.ftLastAccessTime;
		//ftLastWriteTime;

		cTime = FindFile.ftLastWriteTime;
		//JetAPI::GetTime(str, cTime);
		JetAPI::FormatTime(FORMAT_TIME_01, cTime, str);//格式化時間
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		nItem ++;
	}
	m_StopAllListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	

	if ( nItem > 0 ) 
	{	ListCtrl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildAllFileListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_20 &ListCtrl = m_AllFileListWnd;

		ListCtrl.GetClientRect(&Rect);
		width = 164;
		width2 = (Rect.right-Rect.left-width-32);
		str = _T("Filename");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("DateTime");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildLatestFileListWnd()
{		
	CThisListCtrl_20 &ListCtrl = m_LatestFileListWnd;
	m_StopLatestListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_LatestFilenameList.clear();
	m_StopLatestListBeSelected = false;
	
	BOOL bOffline = IsDlgButtonChecked(PROLIST_LATEST_FILE_OFFLINE_CHK);
	if ( FALSE == bOffline )
	{
		if ( AOIDataCollect.CheckLatestFilenameList(m_LatestFilenameList, false) == false ) { return false; }
	}
	else
	{
		if ( AOIDataCollect.CheckLatestFilenameList(m_LatestFilenameList, true) == false ) { return false; }
	}

	size_t       i=0;	
	CString      str;
	int          nItem=0;
	int          nSubItem=0;
	CString      Filename;
	const size_t FilenameCount = m_LatestFilenameList.size();

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopLatestListBeSelected = true;
	for ( i=0; i<FilenameCount; i++ )
	{
		nSubItem=0;
		Filename = m_LatestFilenameList[i];
		
		str.Format(_T("%d"), i+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)i);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = Filename;
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		nItem ++;
	}
	m_StopLatestListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	if ( nItem > 0 ) 
	{	ListCtrl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::BuildLatestFileListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_20 &ListCtrl = m_LatestFileListWnd;

		ListCtrl.GetClientRect(&Rect);
		width = 64;
		str = _T("Index");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		width2 = (Rect.right-Rect.left-width-32);
		str = _T("Filename");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnItemchangedLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopLatestListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_LatestFileListWnd.GetItemData(nItem);
	const size_t FilenameCount = m_LatestFilenameList.size();
	if ( SelIndex >= FilenameCount ) { return; }
	CString Filename = m_LatestFilenameList[SelIndex];
	ExecLoadProjectMap(Filename);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnDblclkLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	OnOK();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnItemchangedAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopAllListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_AllFileListWnd.GetItemData(nItem);
	const size_t FilenameCount = m_AllFilenameList.size();
	if ( SelIndex >= FilenameCount ) { return; }

	CString Folder;
	CString Filename;	
	CString ShortName;
	WIN32_FIND_DATA FindFile;
	GetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	FindFile = m_AllFilenameList[SelIndex];
	ShortName = FindFile.cFileName;
	Filename.Format(_T("%s\\%s"), Folder, ShortName);
	ExecLoadProjectMap(Filename);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnDblclkAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	OnOK();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::ExecLoadProjectMap(LPCTSTR pfilename)
{
	if ( NULL == pfilename ) { return false; }
	if ( m_SelectedFilename.CompareNoCase(pfilename) == 0 ) { return true; }
	CString ProjectMap;
	CString ProjectFolder;
	bool  bLoad = false;
	const bool bEnhance = true;
	const bool bSmallMap = (bool)(IsDlgButtonChecked(PROLIST_SMALL_MAP_CHK));
	const int MapIndex = (int)(JetAPI::GetComboxCurSelData(m_MapIndexCombox));
	JetAPI::ExtractMainFileName(pfilename, ProjectFolder);	
	m_SelectedFilename = pfilename;
	SetDlgItemText(PROLIST_SELECT_FILENAME_EDIT, m_SelectedFilename);

	bLoad = false;
	ProjectMap = AOIDataDefine.GetProjectMapImageName(ProjectFolder, MapIndex, bSmallMap);	
	bLoad = m_ProjectMapWnd.LoadImageFile(ProjectMap, bEnhance);
	if ( false==bLoad && 0!=MapIndex )
	{	
		ProjectMap = AOIDataDefine.GetProjectMapImageName(ProjectFolder, 0, bSmallMap);	
		bLoad = m_ProjectMapWnd.LoadImageFile(ProjectMap, bEnhance);
	}
	if ( false==bLoad && bSmallMap==true )
	{
		ProjectMap = AOIDataDefine.GetProjectMapImageName(ProjectFolder, 0, false);	
		bLoad = m_ProjectMapWnd.LoadImageFile(ProjectMap, bEnhance);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnAllFileFolderBtn() 
{
	// TODO: Add your control notification handler code here
	CString Folder;
	GetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	if ( JetAPI::OpenFolderDialog(this, Folder) == false ) { return; }
	SetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);	
	BuildAllFileListWnd();
	AOIDataCollect.SetOpenProjectDirectory(Folder);
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnLatestFileOfflineChk() 
{
	// TODO: Add your control notification handler code here
	BuildLatestFileListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnOK() 
{
	// TODO: Add extra validation here
	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnColumnclickAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( m_AllFileListCompareID != ColumnsIdx )
	{	m_AllFileListSortMode = 1; }
	else
	{
		if ( 0 == m_AllFileListSortMode ) { m_AllFileListSortMode = 1; }
		else {	m_AllFileListSortMode = 0;  }
	}
	m_AllFileListCompareID = ColumnsIdx;
	m_AllFileListWnd.SortItems(AllFileListCompareFn, (DWORD_PTR)this);

	const int nItem = m_AllFileListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_AllFileListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnAllFileFolderRadio() 
{
	// TODO: Add your control notification handler code here
	m_AllFileListMode = PROLIST_ALL_FILE_FOLDER_RADIO;
	BuildAllFileListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnAllFileOpenCodeRadio() 
{
	// TODO: Add your control notification handler code here
	m_AllFileListMode = PROLIST_ALL_FILE_OPEN_CODE_RADIO;
	BuildAllFileListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnClickLatestFileListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here	
	const int nItem = m_LatestFileListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem < 0 ) { return; }	
	const size_t SelIndex = m_LatestFileListWnd.GetItemData(nItem);
	const size_t FilenameCount = m_LatestFilenameList.size();
	if ( SelIndex >= FilenameCount ) { return; }
	CString Filename = m_LatestFilenameList[SelIndex];
	ExecLoadProjectMap(Filename);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnClickAllFileListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here	
	CString Folder;
	CString Filename;	
	CString ShortName;
	WIN32_FIND_DATA FindFile;

	const int nItem = m_AllFileListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem < 0 ) { return; }
	const size_t SelIndex = m_AllFileListWnd.GetItemData(nItem);
	const size_t FilenameCount = m_AllFilenameList.size();
	if ( SelIndex >= FilenameCount ) { return; }

	GetDlgItemText(PROLIST_ALL_FILE_FOLDER_EDIT, Folder);
	FindFile = m_AllFilenameList[SelIndex];
	ShortName = FindFile.cFileName;
	Filename.Format(_T("%s\\%s"), Folder, ShortName);
	ExecLoadProjectMap(Filename);

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_SELF_WND_EXTRA_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_FIRST_UI_CALLBACK:			
			BuildLatestFileListWnd();
			BuildAllFileListWnd();
			break;
		}
		break;
	}	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnSelchangeMapFrameCombo() 
{
	// TODO: Add your control notification handler code here
	CString Filename;
	Filename = m_SelectedFilename;
	m_SelectedFilename = _T("");
	ExecLoadProjectMap(Filename);
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnAllFileSearchRadio() 
{
	// TODO: Add your control notification handler code here
	m_AllFileListMode = PROLIST_ALL_FILE_SEARCH_RADIO;
	BuildAllFileListWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnSmallMapChk() 
{
	// TODO: Add your control notification handler code here
	CString Filename;
	Filename = m_SelectedFilename;
	m_SelectedFilename = _T("");
	ExecLoadProjectMap(Filename);
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnUpdateLatestFileBtn() 
{
	// TODO: Add your control notification handler code here
	bool bOffline = (bool)(IsDlgButtonChecked(PROLIST_LATEST_FILE_OFFLINE_CHK));
	AOIDataCollect.UpdateLatestFilenameList(bOffline);	
	BuildLatestFileListWnd();
	return ;
}
//-------------------------------------------------------------------------------------//
bool CProjectListWnd::ExecDeleteFileBtn()
{
	DWORD   Res=0;
	CString str;
	CString str2;
	CString Folder;
	CString filename;
	CWnd::GetDlgItemText(PROLIST_SELECT_FILENAME_EDIT, filename);
	const int len = filename.GetLength();
	if ( 0 == len ) { return false; }	
	
	str = _T("Do you want to delete the project");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%s]?"), str, filename);
	Res = JetAPI::ShowMessageBox(str2, MB_YESNO);
	if ( IDYES != Res )
	{	return false; }

	JetAPI::ExtractMainFileName(filename, Folder);
	::DeleteFile(filename);
	JetAPI::RemoveFolder(Folder);
	::Sleep(0);
	OnUpdateLatestFileBtn();
	BuildLatestFileListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectListWnd::OnDeleteFileBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( AOIDataCollect.OperateLevelEditFuncDelProjectFile() == false )
	{	return; }

	ExecDeleteFileBtn();
	return;
}
//-------------------------------------------------------------------------------------//