// LogOperViewerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "LogOperViewerWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define LOG_OPER_CHECK_COUNT            100000//惠璶絋粄计秖
//-------------------------------------------------------------------------------------//
#define CONTENT_INDEX_DATATIME           0//ず甧-ら戳ま计
#define CONTENT_INDEX_USER               1//ず甧-ㄏノま计
#define CONTENT_INDEX_SCORE              2//ず甧-絛氓ま计
#define CONTENT_INDEX_TARGET             3//ず甧-ヘ夹ま计
#define CONTENT_INDEX_CONTENT            4//ず甧-ず甧ま计
//-------------------------------------------------------------------------------------//
#define LOG_OPER_LIST_COLUMN_INDEX       0//ま计-絪腹
#define LOG_OPER_LIST_COLUMN_DATE_TIME   1//ま计-ら戳
#define LOG_OPER_LIST_COLUMN_USER        2//ま计-ㄏノ
#define LOG_OPER_LIST_COLUMN_SCORE       3//ま计-絛氓
#define LOG_OPER_LIST_COLUMN_TARGET      4 //ま计-ヘ夹
#define LOG_OPER_LIST_COLUMN_CONTENT     5//ま计-ず甧
#define LOG_OPER_LIST_COLUMN_COUNT       6//ま计-絪腹
//-------------------------------------------------------------------------------------//
int CALLBACK LogOperListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK LogOperListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CLogOperViewerWnd* pWnd = (CLogOperViewerWnd*)lParamSort;
	return pWnd->CompareLopOperList(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLogOperViewerWnd dialog
//-------------------------------------------------------------------------------------//
CLogOperViewerWnd::CLogOperViewerWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CLogOperViewerWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogOperViewerWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ShowFolder = true;
	m_StopItemListBeSelected = false;
	CTime cTime = ::CTime::GetCurrentTime();
	m_BeginDate = CTime(cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), 0, 0, 0);
	m_BeginTime = m_BeginDate;
	m_EndDate = CTime(cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), 23, 59, 59);
	m_EndTime = m_EndDate;	
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogOperViewerWnd)
	DDX_Control(pDX, LOGOPER_LIST_WND, m_ListWnd);
	DDX_Control(pDX, LOGOPER_USER_COMBO, m_UserCombox);
	DDX_Control(pDX, LOGOPER_SCORE_COMBO, m_ScoreCombox);
	DDX_DateTimeCtrl(pDX, LOGOPER_BEGIN_DATE_CTRL, m_BeginDate);
	DDX_DateTimeCtrl(pDX, LOGOPER_BEGIN_TIME_CTRL, m_BeginTime);
	DDX_DateTimeCtrl(pDX, LOGOPER_END_DATE_CTRL, m_EndDate);
	DDX_DateTimeCtrl(pDX, LOGOPER_END_TIME_CTRL, m_EndTime);		
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CLogOperViewerWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CLogOperViewerWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(LOGOPER_BUILD_BTN, OnBuildBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, LOGOPER_LIST_WND, OnColumnclickListWnd)
	ON_BN_CLICKED(LOGOPER_FOLDER_BTN, OnFolderBtn)
	ON_BN_CLICKED(LOGOPER_USER_CHK, OnUserChk)
	ON_CBN_SELCHANGE(LOGOPER_USER_COMBO, OnSelchangeUserCombo)
	ON_BN_CLICKED(LOGOPER_SCORE_CHK, OnScoreChk)
	ON_CBN_SELCHANGE(LOGOPER_SCORE_COMBO, OnSelchangeScoreCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLogOperViewerWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CLogOperViewerWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	CString Folder = AOIDataCollect.GetAOILogOperDirectory();
	SwitchMultiLanguage();	
	JetAPI::InitialListCtrl(m_ListWnd);	
	CWnd::SetDlgItemText(LOGOPER_FOLDER_EDIT, Folder);
	BuildListWndHeader();
	BuildListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	BOOL bShow=m_ShowFolder;
	JetAPI::ShowCtrlWnd(this, LOGOPER_FOLDER_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, LOGOPER_FOLDER_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, LOGOPER_FOLDER_BTN, bShow);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	if ( m_ListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.left = 4;
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_ListWnd.MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_LOG_OPER_VIEWER_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_LOG_OPER_VIEWER_WND;
	WndKey = _T("IDD_LOG_OPER_VIEWER_WND");
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
	WndID = LOGOPER_BEGIN_LABEL;
	WndKey = _T("LOGOPER_BEGIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LOGOPER_END_LABEL;
	WndKey = _T("LOGOPER_END_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	


	WndID = LOGOPER_USER_CHK;
	WndKey = _T("LOGOPER_USER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LOGOPER_SCORE_CHK;
	WndKey = _T("LOGOPER_SCORE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LOGOPER_BUILD_BTN;
	WndKey = _T("LOGOPER_BUILD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = LOGOPER_FOLDER_LABEL;
	WndKey = _T("LOGOPER_FOLDER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CLogOperViewerWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_LOG_OPER_VIEWER_WND");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
inline int CLogOperViewerWnd::GetColActIndex() const
{
	return m_ColActIndex;
}
//-------------------------------------------------------------------------------------//
inline void CLogOperViewerWnd::SetColActIndex(int val)
{
	m_ColActIndex = val;
}
//-------------------------------------------------------------------------------------//
inline int CLogOperViewerWnd::GetColSortMode() const
{
	return m_ColSortMode;
}
//-------------------------------------------------------------------------------------//
inline void CLogOperViewerWnd::SetColSortMode(int val)
{
	m_ColSortMode = val;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::CheckReloadFile()
{	
	if ( m_FileDateEnd.GetLength() != 14 ){ return true; }
	if ( m_FileDateBegin.GetLength() != 14 ){ return true; }	

	CString strFolder=GetLogOperFolder();
	CString strEnd=GetLogOperFileDateEnd();
	CString strBegin=GetLogOperFileDateBegin();		
	if ( strEnd.CompareNoCase(m_FileDateEnd) !=0 )
	{	return true; }
	if ( strFolder.CompareNoCase(m_FileFolder) !=0 )
	{	return true; }
	if ( strBegin.CompareNoCase(m_FileDateBegin) !=0 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::ExecBuildFunc()
{
	if ( CheckReloadFile() == false )
	{	return true; }

	CString Folder = GetLogOperFolder();
	std::vector<CString>  FileList;
	std::vector<CString>  &ItemList=m_ItemList;
	std::vector<CString>  &UserList=m_UserList;
	std::vector<CString>  &ScoreList=m_ScoreList;

	ItemList.clear();
	UserList.clear();
	ScoreList.clear();
	JetAPI::ClearCombox(m_UserCombox);
	JetAPI::ClearCombox(m_ScoreCombox);
	if ( JetAPI::ListFilesInFolder(Folder, _T("TXT"), FileList) == false )
	{	return false; }

	size_t       i=0;	
	DWORD        Res;
	CString      str;
	CString      str2;
	CString      Content;
	CString      Filename;		
	FILE        *pfile = NULL;		
	int          BadRowCount=0;
	bool         bCheckTime=true;	
	std::vector<CString> SubList;	
	CString      strDateEnd=GetLogOperFileDateEnd();
	CString      strDateBegin=GetLogOperFileDateBegin();
	const size_t FileCount=FileList.size();	
	const int    UserIndex=CONTENT_INDEX_USER;
	const int    ScoreIndex=CONTENT_INDEX_SCORE;
	const int    SubCount=LOG_OPER_LIST_COLUMN_COUNT-1;
	size_t       ItemCheckCount=LOG_OPER_CHECK_COUNT;

	Res = IDYES;
	for ( i=0; i<FileCount; i++ )
	{
		Filename.Format(_T("%s\\%s"), Folder, FileList[i]);
		pfile = LogOperCtrl.OpenLogOperFile(Filename);
		if ( NULL == pfile ) { continue; }

		BadRowCount=0;
		bCheckTime = true;		
		while ( true )
		{
			if ( BadRowCount > 10 )//び岿粇戈, 铬筁
			{	break; }

			if ( LogOperCtrl.CheckLogOperFileEnd(pfile) == true )
			{	break; }

			if ( LogOperCtrl.ReadLogOperText(pfile, Content) == false )
			{	continue; }

			//if ( LogOperCtrl.CheckLogOperContentCount(Content) < SubCount )
			//{	continue; }

			if ( true == bCheckTime )
			{
				SubList.clear();
				if ( LogOperCtrl.DecodeLogOperContent(Content, SubList) == false )
				{	continue; }
				if ( SubList.size() < SubCount ) 
				{
					BadRowCount ++;
					continue; 
				}
				if ( SubList[0].GetLength() < 14 ) //獶ら戳, Unicode郎繷ぃǎ
				{
					BadRowCount ++;
					continue; 
				}
				if ( SubList[0]<strDateBegin || SubList[0]>strDateEnd )
				{	break;	}	

				//bCheckTime = false;
				AddTagList(SubList[UserIndex], UserList);
				AddTagList(SubList[ScoreIndex], ScoreList);
			}
			ItemList.push_back(Content);

			if ( ItemList.size() > ItemCheckCount )
			{
				str = _T("Do you want to Continue Load Data");
				str2.Format(_T("%s[%d] ?"), str, ItemList.size());
				Res = JetAPI::ShowMessageBox(str2, MB_YESNO);
				if ( IDNO == Res )
				{	break; }
				if ( IDYES == Res )
				{	ItemCheckCount += LOG_OPER_CHECK_COUNT;	}
			}
		};		
		LogOperCtrl.CloseLogOperFile(pfile);
		if ( IDNO == Res )
		{	break; }
	}
	m_FileFolder = Folder;
	m_FileDateEnd = strDateEnd;
	m_FileDateBegin = strDateBegin;	
	BuildTagCombox(m_UserCombox, UserList);
	BuildTagCombox(m_ScoreCombox, ScoreList);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLogOperViewerWnd::GetLogOperFolder()
{
	CString Folder;
	CWnd::GetDlgItemText(LOGOPER_FOLDER_EDIT, Folder);
	return Folder;
}
//-------------------------------------------------------------------------------------//
CString CLogOperViewerWnd::GetLogOperFileDateEnd()
{
	CString str;
	CString Time=_T("235959");		
	CString Date=m_EndDate.Format(_T("%Y%m%d"));	
	str.Format(_T("%s%s"), Date, Time);
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CLogOperViewerWnd::GetLogOperFileDateBegin()
{
	CString str;
	CString Time=_T("000000");		
	CString Date=m_BeginDate.Format(_T("%Y%m%d"));	
	str.Format(_T("%s%s"), Date,Time);
	return str;	
}
//-------------------------------------------------------------------------------------//
CString  CLogOperViewerWnd::GetLogOperRangeTimeEnd()
{
	CString str;
	CString Time=m_EndTime.Format(_T("%H%M%S"));
	CString Date=m_EndDate.Format(_T("%Y%m%d"));
	str.Format(_T("%s%s"), Date, Time);
	return str;	
}
//-------------------------------------------------------------------------------------//
CString CLogOperViewerWnd::GetLogOperRangeTimeBegin()
{
	CString str;
	CString Time=m_BeginTime.Format(_T("%H%M%S"));
	CString Date=m_BeginDate.Format(_T("%Y%m%d"));
	str.Format(_T("%s%s"), Date, Time);
	return str;	
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::FormatTime(LPCTSTR DateTime, CString &Str)
{
	if ( NULL == DateTime ) { return false; }
	int   idx=0;
	TCHAR Year[8], Mon[8], Day[8], Hou[8], Min[8], Sec[8];
	Year[0]=DateTime[idx++];	Year[1]=DateTime[idx++];
	Year[2]=DateTime[idx++];	Year[3]=DateTime[idx++];
	Year[4]=_T('\0');

	Mon[0]=DateTime[idx++];	Mon[1]=DateTime[idx++];		Mon[2]=_T('\0');
	Day[0]=DateTime[idx++];	Day[1]=DateTime[idx++];		Day[2]=_T('\0');
	Hou[0]=DateTime[idx++];	Hou[1]=DateTime[idx++];		Hou[2]=_T('\0');
	Min[0]=DateTime[idx++];	Min[1]=DateTime[idx++];		Min[2]=_T('\0');
	Sec[0]=DateTime[idx++];	Sec[1]=DateTime[idx++];		Sec[2]=_T('\0');

	//2019/11/15,18:07:57.
	Str.Format(_T("%s/%s/%s %s:%s:%s"), Year, Mon, Day, Hou, Min, Sec);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::AddTagList(LPCTSTR Tag, std::vector<CString> &List)
{
	size_t i=0;
	const size_t Cnt=List.size();
	for ( i=0; i<Cnt; i++ )
	{
		if ( List[i].CompareNoCase(Tag) == 0 ) 
		{	return true; }
	}
	List.push_back(Tag);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::CheckTagFilter(UINT ChkID, CComboBox &Combox, CString &strFilter)
{
	bool bFilter=true;
	const int nSel=Combox.GetCurSel();
	if ( CWnd::IsDlgButtonChecked(ChkID) == TRUE )
	{	bFilter = true; }
	else 
	{	bFilter = false; }
	if ( nSel < 0 )
	{	bFilter = false;	}
	else
	{	Combox.GetLBText(nSel, strFilter);	}
	return bFilter;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::BuildTagCombox(CComboBox &Combox, const std::vector<CString> &List)
{
	size_t       i=0;
	int          idx=0;	
	DWORD        Param=0;		
	const size_t Cnt=List.size();

	idx = 0;
	JetAPI::ClearCombox(Combox);	
	for ( i=0; i<Cnt; i++ )
	{
		Combox.InsertString(-1, List[i]);
		Combox.SetItemData(idx, (DWORD_PTR)(i));
		idx ++;
	}	
	if ( Combox.GetCount() > 0 )
	{	Combox.SetCurSel(0); }
	return true;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_55& CLogOperViewerWnd::GetBuildListWnd()
{
	return m_ListWnd;
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::SetShowFolder(bool bShow)
{
	m_ShowFolder = bShow;
}
//-------------------------------------------------------------------------------------//
int CLogOperViewerWnd::CompareLopOperList(size_t index1, size_t index2)
{
	const int ColID = GetColActIndex();
	const size_t FileCount = m_ItemList.size();
	if ( index1>=FileCount || index2>=FileCount ) { return 0; }
	int     Res=0;
	int     Idx=0;
	std::vector<CString> List1;
	std::vector<CString> List2;
	CString &Str1=(m_ItemList[index1]);
	CString &Str2=(m_ItemList[index2]);		
	LogOperCtrl.DecodeLogOperContent(Str1, List1);
	LogOperCtrl.DecodeLogOperContent(Str2, List2);
	switch ( ColID )
	{	
	case LOG_OPER_LIST_COLUMN_DATE_TIME:
		Idx = CONTENT_INDEX_DATATIME;
		Res = List1[Idx].CompareNoCase(List2[Idx]);
		break;
	case LOG_OPER_LIST_COLUMN_USER:
		Idx = CONTENT_INDEX_USER;
		Res = List1[Idx].CompareNoCase(List2[Idx]);
		break;
	case LOG_OPER_LIST_COLUMN_SCORE:
		Idx = CONTENT_INDEX_SCORE;
		Res = List1[Idx].CompareNoCase(List2[Idx]);
		break;
	case LOG_OPER_LIST_COLUMN_TARGET:
		Idx = CONTENT_INDEX_TARGET;
		Res = List1[Idx].CompareNoCase(List2[Idx]);
		break;
	case LOG_OPER_LIST_COLUMN_CONTENT:
		Idx = CONTENT_INDEX_CONTENT;
		Res = List1[Idx].CompareNoCase(List2[Idx]);
		break;
	default:
	case LOG_OPER_LIST_COLUMN_INDEX:
		if ( index1 > index2 ) { Res = 1; }
		else { Res = -1; }
		break;
	}
	if ( 1 == GetColSortMode() ) 
	{	return Res; }
	Res = Res*-1;	
	return Res;		
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::BuildListWnd()
{
	UpdateData();
	CThisListCtrl_55 &ListCtrl = GetBuildListWnd();	
	m_StopItemListBeSelected = true;	
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	SetColActIndex(LOG_OPER_LIST_COLUMN_INDEX);
	m_StopItemListBeSelected = false;
	if ( ExecBuildFunc() == false ) { return false; }	

	size_t   i=0, j=0;
	CString  str;
	size_t   UseCount=0;
	size_t   SubCount=0;
	int      nItem = 0;
	int      nSubItem = 0;
	std::vector<CString>  ContentList;
	const std::vector<CString>  &ItemList=m_ItemList;
	const size_t ItemCount = ItemList.size();
	const int    SubColCount=LOG_OPER_LIST_COLUMN_COUNT-1;
	bool         bFilterUser = false;
	bool         bFilterScore = false;	
	CString      strUser = _T("");
	CString      strScore = _T("");		
	const int    UserIndex=CONTENT_INDEX_USER;
	const int    ScoreIndex=CONTENT_INDEX_SCORE;
	const int    DateTimeIndex=CONTENT_INDEX_DATATIME;
	CString      strTimeEnd=GetLogOperRangeTimeEnd();
	CString      strTimeBegin=GetLogOperRangeTimeBegin();

	bFilterUser = CheckTagFilter(LOGOPER_USER_CHK, m_UserCombox, strUser);
	bFilterScore = CheckTagFilter(LOGOPER_SCORE_CHK, m_ScoreCombox, strScore);

	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ItemCount; i++ )
	{
		nSubItem = 0;
		ContentList.clear();
		LogOperCtrl.DecodeLogOperContent(ItemList[i], ContentList);		
		SubCount = ContentList.size();		
		if ( ContentList[DateTimeIndex]<strTimeBegin || ContentList[DateTimeIndex]>strTimeEnd )
		{	continue; }
		if ( true == bFilterUser )
		{
			if ( strUser.CompareNoCase(ContentList[UserIndex]) != 0 ) 
			{	continue; }
		}
		if ( true == bFilterScore )
		{
			if ( strScore.CompareNoCase(ContentList[ScoreIndex]) != 0 ) 
			{	continue; }
		}

		str.Format(_T("%d"), nItem+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)(i));
		ListCtrl.SetItemText(nItem, nSubItem, str); nSubItem ++;

		UseCount = MIN(SubColCount, SubCount);
		for ( j=0; j<UseCount; j++ )
		{
			switch ( j )
			{
			case DateTimeIndex:				
				FormatTime(ContentList[j], str);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				break;
			default:
				ListCtrl.SetItemText(nItem, nSubItem, ContentList[j]);
				break;
			}
			nSubItem ++;
		}

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperViewerWnd::BuildListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_55 &ListCtrl = GetBuildListWnd();

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("DateTime");
	str = LoadMultiLanguageString(str, str);	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width;
	str = _T("User");
	str = LoadMultiLanguageString(str, str);		
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Score");
	str = LoadMultiLanguageString(str, str);		
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	str = _T("Target");
	str = LoadMultiLanguageString(str, str);		
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = _T("Content");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnBuildBtn() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnColumnclickListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int SortMode = GetColSortMode();	
	const int ColumnsIdx = pNMListView->iSubItem;
	CThisListCtrl_55 &ListCtrl = GetBuildListWnd();
	if ( GetColActIndex() != ColumnsIdx )
	{	SortMode = 1; }
	else
	{
		if ( 0 == SortMode ) { SortMode = 1; }
		else {	SortMode = 0;  }
	}
	SetColSortMode(SortMode);
	SetColActIndex(ColumnsIdx);	
	ListCtrl.SortItems(LogOperListCompareFn, (DWORD_PTR)this);	
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	ListCtrl.EnsureVisible(nItem, FALSE); }	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnFolderBtn() 
{
	// TODO: Add your control notification handler code here
	//GetLogOperFolder
	CString Folder = GetLogOperFolder();
	if ( JetAPI::OpenFolderDialog(this, Folder) == false )
	{	return; }
	CWnd::SetDlgItemText(LOGOPER_FOLDER_EDIT, Folder);
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnUserChk() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnSelchangeUserCombo() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnScoreChk() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//
void CLogOperViewerWnd::OnSelchangeScoreCombo() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//