// LoadBomWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "LoadBomWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadBomWnd dialog
//-------------------------------------------------------------------------------------//
CLoadBomWnd::CLoadBomWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CLoadBomWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadBomWnd)	
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadBomWnd)
	DDX_Control(pDX, BOM_DATA_TYPE_COMBOX, m_DataTypeCombox);
	DDX_Control(pDX, BOM_RESULT_LIST_WND, m_ResultListWnd);
	DDX_Control(pDX, BOM_PREVIEW_LIST_WND, m_PreviewListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CLoadBomWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CLoadBomWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(BOM_OPEN_CSV_BTN, OnOpenCSVBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, BOM_PREVIEW_LIST_WND, OnColumnclickPreviewListWnd)
	ON_CBN_CLOSEUP(BOM_DATA_TYPE_COMBOX, OnCloseupDataTypeCombox)
	ON_CBN_SELCHANGE(BOM_DATA_TYPE_COMBOX, OnSelchangeDataTypeCombox)
	ON_BN_CLICKED(BOM_PREVIEW_BTN, OnPreviewBtn)
	ON_BN_CLICKED(BOM_LOAD_BTN, OnLoadBtn)
	ON_BN_CLICKED(BOM_SAVE_DEFAULT_BTN, OnSaveDefaultBtn)
	ON_BN_CLICKED(BOM_LOAD_DEFAULT_BTN, OnLoadDefaultBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadBomWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CLoadBomWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	CWnd::SetDlgItemInt(BOM_LOAD_FORMAT_START_RAW_EDIT, 0);

	m_ColTypeList.push_back(BOM_LIST_HEADER_NULL);
	m_ColTypeList.push_back(BOM_LIST_HEADER_ITEM);
	m_ColTypeList.push_back(BOM_LIST_HEADER_PART_NUMBER);	
	m_ColTypeList.push_back(BOM_LIST_HEADER_DESCRIPTION);	
	m_ColTypeList.push_back(BOM_LIST_HEADER_USAGE);	
	m_ColTypeList.push_back(BOM_LIST_HEADER_LOCATION);

	JetAPI::InitialListCtrl(m_PreviewListWnd);
	BuildPreviewListWndHeader();	

	JetAPI::InitialListCtrl(m_ResultListWnd);
	BuildResultListWndHeader();

	m_DataTypeCombox.ShowWindow(SW_HIDE);	
	BuildDataTypeCombox();

	SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());

	CString IniFile = GetSaveDefaultFilename();
	if ( JetAPI::IsFileExist(IniFile) == true )
	{
		ExecLoadDefaultBtn();
		BuildPreviewListWndHeader();
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_PreviewListWnd.GetSafeHwnd() == NULL ) { return; }

	if ( this->m_PreviewListWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		this->m_PreviewListWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);		
		Rect.right = cx-4;
		//Rect.bottom = cy-8-szProfile.cy;
		this->m_PreviewListWnd.MoveWindow(&Rect);		
	}

	if ( this->m_ResultListWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		this->m_ResultListWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);		
		Rect.right = cx-4;
		Rect.bottom = cy-4;
		this->m_ResultListWnd.MoveWindow(&Rect);		
	}
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::GetBomNodeList(std::vector<TComponentNode> &List)
{
	List = m_BomNodeList;
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_LOAD_BOM_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//		
	//SetMultiLanauage(LoadIDAndName(CADXY_OPEN_FILE_BTN));
	SetMultiLanauage(LoadIDAndName(BOM_OPEN_CSV_BTN));	
	SetMultiLanauage(LoadIDAndName(BOM_FILE_UNICODE_CHK));	
	//SetMultiLanauage(LoadIDAndName(BOM_SAVE_DEFAULT_BTN));
	//SetMultiLanauage(LoadIDAndName(BOM_LOAD_DEFAULT_BTN));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(BOM_PREVIEW_BTN));
	SetMultiLanauage(LoadIDAndName(BOM_LOAD_FORMAT_START_RAW_LABEL));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(BOM_LOAD_BTN));	
	//---------------------------------------------------------------------------------//		
	SetMultiLanauage(LoadIDAndName(BOM_LOAD_DEFAULT_BTN));	
	SetMultiLanauage(LoadIDAndName(BOM_SAVE_DEFAULT_BTN));	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_LOAD_BOM_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLoadBomWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_LOAD_BOM_WND");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnOpenCSVBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("TXT Files (*.txt)|*.txt|ASC Files (*.asc)|*.asc|PRN Files (*.prn)|*.asc|CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("txt;asc;prn;csv"), _T("*.csv"), OFN_FILEMUSTEXIST, szFilters);
	//dialog.m_ofn.lpstrInitialDir = pWizard->GetProjectDirectory();	
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	this->m_Filename = dialog.GetPathName();
	this->OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::BuildPreviewListWndHeader()
{
	CListCtrl &ListWnd = m_PreviewListWnd;
	JetAPI::ClearListCtrlHeaderList(ListWnd);

	int    i=0;
	int    ColumnsWidth = 0;	
	RECT ListRect={0};
	CString SubText;
	const int NColumns = (int)(this->m_ColTypeList.size());
	
	ListWnd.GetClientRect(&ListRect);
	ColumnsWidth = (ListRect.right-ListRect.left-32)/(NColumns);
	if ( ColumnsWidth < 32 ) { ColumnsWidth = 32; }

	SubText = _T("Index");	
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(0, SubText,LVCFMT_CENTER,ColumnsWidth);
	for (i=1; i<NColumns; i++)	
	{	
		SubText = GetDataTypeText(m_ColTypeList[i]);
		ListWnd.InsertColumn(i, SubText,LVCFMT_CENTER,ColumnsWidth);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::BuildDataTypeCombox()
{
	int idx = 0;
	CString str;

	idx=0;
	JetAPI::ClearCombox(m_DataTypeCombox);

	str = _T("NULL Col");	
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_NULL);
	idx ++;

	str = _T("Item Col");	
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_ITEM);
	idx ++;

	str = _T("Part Number Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_PART_NUMBER);
	idx ++;

	str = _T("Description Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_DESCRIPTION);
	idx ++;

	str = _T("Usage Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_USAGE);
	idx ++;

	str = _T("Location Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, BOM_LIST_HEADER_LOCATION);
	idx ++;	
	return;
}
//-------------------------------------------------------------------------------------//
CString  CLoadBomWnd::GetDataTypeText(int Type)
{
	CString SubText;
	switch ( Type )
	{		
	case BOM_LIST_HEADER_ITEM:			SubText = _T("Item");			break;
	case BOM_LIST_HEADER_PART_NUMBER:	SubText = _T("Part Number");	break;
	case BOM_LIST_HEADER_DESCRIPTION:	SubText = _T("Description");	break;
	case BOM_LIST_HEADER_USAGE:			SubText = _T("Usage");			break;
	case BOM_LIST_HEADER_LOCATION:		SubText = _T("Location");		break;
	default:
	case BOM_LIST_HEADER_NULL:			SubText = _T("NULL");			break;
	}
	SubText = LoadMultiLanguageString(SubText, SubText);
	return SubText;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::BuildBomLoadParam(TLoadParam_BOM &LoadParam)
{
	LoadParam.bUnicode = (bool)(CWnd::IsDlgButtonChecked(BOM_FILE_UNICODE_CHK));	
	LoadParam.nStartLine = (int)(this->GetDlgItemInt(BOM_LOAD_FORMAT_START_RAW_EDIT));

	int   index=0;
	int   ColType=0;	
	const size_t Cols = m_ColTypeList.size();
	for ( size_t i=0; i<Cols; i++ )
	{
		index = i-1;//扣除引數欄
		ColType = m_ColTypeList[i];
		switch ( ColType )
		{
		case BOM_LIST_HEADER_ITEM:			LoadParam.nItemIdx=index;	break;
		case BOM_LIST_HEADER_PART_NUMBER:	LoadParam.nPNIdx=index;		break;
		case BOM_LIST_HEADER_DESCRIPTION:	LoadParam.nDescIdx=index;	break;
		case BOM_LIST_HEADER_USAGE:			LoadParam.nUsageIdx=index;	break;
		case BOM_LIST_HEADER_LOCATION:		LoadParam.nLocationIdx=index;	break;
		default:
		case BOM_LIST_HEADER_NULL:
			break;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::BuildPreviewListWnd()
{
	CListCtrl &ListWnd = m_PreviewListWnd;
	JetAPI::ClearListCtrl(ListWnd, TRUE);
	
	bool bSucc=true;
	CString SubText;
	size_t i=0, j=0, k=0;	
	TLoadParam_BOM LoadParam;
	BuildBomLoadParam(LoadParam);

	size_t Textlen = 0;
	size_t SubStringCount=0;	
	const size_t Textlinesize = 1024;

	char TextFilter = '\"';	
	std::vector<char> DelimList={','};	
	char Textline[Textlinesize]= "";	
	std::string StringLine, SubString, Str;	
	std::vector<std::string> SubStringList;	

	wchar_t wTextFilter = L'\"';
	std::vector<wchar_t> wDelimList={L','};
	wchar_t wTextline[Textlinesize]= L"";	
	std::wstring wStringLine, wSubString, wStr;		
	std::vector<std::wstring> wSubStringList;
	
	const size_t NColumns = m_ColTypeList.size();	
	const bool bUnicode = LoadParam.bUnicode;	
	const int  nStartLine = LoadParam.nStartLine;
	//-----------建立報表標頭資料----------------------------------------------------------------//			
	this->BuildPreviewListWndHeader();
	//--------------------------------------------------------------------//
	//將資料填入列表中
	FILE *pfile = NULL;	
	if ( this->m_Filename.GetLength() == 0 )
	{
		m_ErrorString = _T("Please Open File First");
		return false;
	}
	pfile = JetAPI::OpenReadFile(m_Filename, bUnicode);	
	if ( pfile == NULL ) 
	{	
		m_ErrorString.Format(_T("Error, Open File Fault [%s]"), m_Filename);
		return false;
	}
	//--------------------------------------------------------------------//
	int    NRows = 0;//第幾列
	int    NCols = 0;
	size_t DataLine = 0;		
	//--------------------------------------------------------------------//
	COLORREF Color1 = 0xFFCFCF;
	COLORREF Color2 = 0xCFCFFF;
	//--------------------------------------------------------------------//	
	while(!feof(pfile) )
	{ 
		DataLine++;
		if ( false == bUnicode )
		{
			if( fgets(Textline,Textlinesize,pfile) ==NULL ) 
			{	continue;	}
		}
		else
		{
			if( fgetws(wTextline,Textlinesize,pfile) ==NULL ) 
			{	continue;	}
		}
		//未達到使用者要求的開始列數
		if ( DataLine < nStartLine ) { continue; }

		if ( false == bUnicode )
		{	Textlen=(int)strlen(Textline); }
		else
		{	Textlen=(int)wcslen(wTextline); }
		SubText.Format(_T("%d"), DataLine);
		this->m_PreviewListWnd.InsertItem(NRows, SubText);

		//if ( NRows%2 == 0 ) { this->m_PreviewListWnd.SetItemTextBKColor(NRows, Color1); }
		//else { this->m_PreviewListWnd.SetItemTextBKColor(NRows, Color2); }

		if ( Textlen == 0 ) //enter鍵
		{	
			DataLine ++;
			NRows ++;
			continue;	
		}
		if ( false == bUnicode )
		{	
			StringLine = Textline;			
			bSucc = JetAPI::ListSubString(StringLine, DelimList, TextFilter, SubStringList);	
			SubStringCount = SubStringList.size();
		}
		else
		{	
			wStringLine = wTextline;			
			bSucc = JetAPI::ListSubString(wStringLine, wDelimList, wTextFilter, wSubStringList);	
			SubStringCount = wSubStringList.size();
		}
		if ( false == bSucc )
		{	return false; }

		NCols = MIN(NColumns, SubStringCount);
		for ( i=0; i<NCols; i++ )
		{
			if ( false == bUnicode )
			{	SubText = CString(SubStringList[i].c_str());	}
			else
			{	SubText = CString(wSubStringList[i].c_str());	}
			m_PreviewListWnd.SetItemText(NRows, i+1, SubText);
		}		
		NRows ++;
	}
	//--------------------------------------------------------------------//
	::fclose(pfile);
	pfile = NULL;	
	//--------------------------------------------------------------------//	
	if ( NRows > 0 ) 
	{	this->m_PreviewListWnd.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);		}
	this->m_PreviewListWnd.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnColumnclickPreviewListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	const int ColumnsIdx = pNMListView->iSubItem;
	if ( ColumnsIdx == 0 ) { return ; }		
	this->m_CurColIndex = ColumnsIdx;
	CHeaderCtrl *pHeaderCtrl = this->m_PreviewListWnd.GetHeaderCtrl();
	if ( pHeaderCtrl == NULL )
	{
		JetAPI::ShowMessageBox(_T("Error for m_PreviewListWnd.GetHeaderCtrl()"));
		return;
	}
	this->m_PreviewListWnd.EnableWindow(FALSE);	

	RECT ItemRect;
	if ( pHeaderCtrl->GetItemRect(ColumnsIdx, &ItemRect) == FALSE )
	{		
		JetAPI::ShowMessageBox(_T("Error for pHeaderCtrl->GetItemRect()"));			
		this->m_PreviewListWnd.EnableWindow();
		return;
	}
	int Offset = 2;
	int Pos = this->m_PreviewListWnd.GetScrollPos(SB_HORZ);	
	ItemRect.left   -= Pos;
	ItemRect.right  -= Pos;
	ItemRect.top    -= Offset;
	ItemRect.bottom += Offset;
	
	this->m_DataTypeCombox.SetCurSel(this->m_ColTypeList[ColumnsIdx]);
	this->m_PreviewListWnd.ClientToScreen(&ItemRect);
	this->ScreenToClient(&ItemRect);
	this->m_DataTypeCombox.MoveWindow(&ItemRect);	
	this->m_DataTypeCombox.ShowWindow(SW_SHOW);

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnCloseupDataTypeCombox() 
{
	// TODO: Add your control notification handler code here
	this->m_DataTypeCombox.ShowWindow(SW_HIDE);
	this->m_PreviewListWnd.EnableWindow();
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnSelchangeDataTypeCombox() 
{
	// TODO: Add your control notification handler code here
	const size_t Cols = m_ColTypeList.size();
	if ( m_CurColIndex < 1 ) { return; }
	if ( m_CurColIndex >= Cols ) { return ; }	
	
	TCHAR Text[64]=_T("");	
	const int DataType=m_DataTypeCombox.GetCurSel();;
	this->m_ColTypeList[m_CurColIndex] = DataType;
	::_tcscpy(Text, GetDataTypeText(DataType));	

	LVCOLUMN Column;
	Column.mask = LVCF_WIDTH|LVCF_TEXT;
	Column.pszText = NULL;//一定要先設定成NULL否則取不出來
	if ( this->m_PreviewListWnd.GetColumn(m_CurColIndex, &Column) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Error m_PreviewListWnd.GetColumn(m_CurColIndex, &Column)"));
		this->m_DataTypeCombox.ShowWindow(SW_HIDE);
		this->m_PreviewListWnd.EnableWindow();
		return ;
	}	
	Column.pszText = Text;	
	if ( this->m_PreviewListWnd.SetColumn(m_CurColIndex, &Column) == FALSE )
	{
		JetAPI::ShowMessageBox(_T("Error m_PreviewListWnd.GetColumn(m_CurColIndex, &Column)"));
		this->m_DataTypeCombox.ShowWindow(SW_HIDE);
		this->m_PreviewListWnd.EnableWindow();
		return ;
	}
	this->m_DataTypeCombox.ShowWindow(SW_HIDE);
	this->m_PreviewListWnd.EnableWindow();
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnPreviewBtn() 
{
	// TODO: Add your control notification handler code here	
	size_t   NCols = 0;
	size_t   NRows = 0;		
	TLoadParam_BOM LoadParam;
	BuildBomLoadParam(LoadParam);	
	CWnd::SetDlgItemText(CADXY_LOAD_FILE_NAME_EDIT, m_Filename);	
	if ( false == JetAPI::PreLoadBomFile(m_Filename, LoadParam, NCols, NRows, this->m_ErrorString) )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return;
	}	

	size_t i = 0;	
	std::vector<int> ColTypeListTemp = m_ColTypeList;
	const size_t TempCount = ColTypeListTemp.size();
	const size_t ColCount = MIN(NCols, TempCount);
	m_ColTypeList.clear();
	for ( i=0; i<ColCount; i++ )
	{	m_ColTypeList.push_back(ColTypeListTemp[i]); }
	for ( i=ColCount; i<NCols; i++ )
	{	this->m_ColTypeList.push_back(BOM_LIST_HEADER_NULL); }	
	
	if ( this->BuildPreviewListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return;
	}	
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::BuildResultListWndHeader()
{
	int         index=0;
	int         Width = 0;
	int         Width2= 64;
	CString     SubText;
	RECT        ListClientRect={0};
	CListCtrl  &ListWnd = m_ResultListWnd;

	ListWnd.GetClientRect(&ListClientRect);
	Width = (ListClientRect.right-ListClientRect.left-32-Width2)/4;

	index = 0;
	SubText = _T("Index");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width2);	index++;

	SubText = _T("Component");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(1, SubText, LVCFMT_CENTER, Width);	index++;
	
	SubText = _T("Part Number");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::BuildResultListWnd()
{
	CListCtrl  &ListWnd = m_ResultListWnd;
	JetAPI::ClearListCtrl(ListWnd, TRUE);		
	this->BuildResultListWndHeader();
	

	size_t i=0;	
	CString ItemText;
	int    idx=0, subidx=0;
	const std::vector<TComponentNode> &BomNodeList=m_BomNodeList;	
	const size_t NodeCount = BomNodeList.size();

	idx=0;
	ListWnd.SetRedraw(FALSE);
	for ( i=0; i<NodeCount; i++ )
	{
		const TComponentNode &BomNodeRef=BomNodeList[i];

		subidx=0;

		ItemText.Format(_T("%d"), i+1);
		ListWnd.InsertItem(idx, ItemText);

		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = BomNodeRef.sComponentName;
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = BomNodeRef.sPartNumber;
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;
	
		idx ++;
	}
	ListWnd.SetRedraw(TRUE);
	ListWnd.Invalidate();
	ListWnd.UpdateWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here	
	TLoadParam_BOM LoadParam;
	BuildBomLoadParam(LoadParam);
	if ( JetAPI::LoadBomFile(m_Filename, LoadParam, m_BomNodeList, m_ErrorString) == false )
	{	
		JetAPI::ShowMessageBox(m_ErrorString);
		return ;
	}
	if ( this->BuildResultListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLoadBomWnd::GetSaveDefaultFilename()
{
	CString IniFile;
	IniFile.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("LoadBOM.INI"));	
	return IniFile;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::ExecSaveDefaultBtn()
{
	size_t  i=0;	
	CString IniFile;
	CString KeyName;
	CString String;
	CString Section;	
	CString ErrString;
	const size_t DataTypeCount = m_ColTypeList.size();

	IniFile = GetSaveDefaultFilename();
	//----------------------------------------------------------------------//
	Section = _T("General Setting");
	KeyName = _T("Unicode File");
	if ( CWnd::IsDlgButtonChecked(BOM_FILE_UNICODE_CHK) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//
	Section = _T("Column Setting");
	KeyName = _T("N Columns");
	String.Format(_T("%d"), DataTypeCount);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}

	for ( i=0; i<DataTypeCount; i++ )
	{		
		KeyName.Format(_T("Columns Data Type %d"), i+1);
		String.Format(_T("%d"), m_ColTypeList[i]);
		if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
		{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	}	
	//----------------------------------------------------------------------//
	Section = _T("Load BOM Setting");
	//Title的裡面的資料型態
	KeyName = _T("Start Line");	
	CWnd::GetDlgItemText(BOM_LOAD_FORMAT_START_RAW_EDIT, String);	
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}	
	//----------------------------------------------------------------------//		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadBomWnd::ExecLoadDefaultBtn()
{
	size_t  i=0;		
	UINT    CtrlID=0;
	int     nValue=0;
	size_t  DataTypeCount=0;
	CString IniFile;
	CString KeyName;
	CString Default;
	CString Section;	
	CString ErrString;
	const size_t MaxText = MAX_JET_PATH;
	TCHAR   String[MaxText]=_T("");	

	IniFile = GetSaveDefaultFilename();
	//----------------------------------------------------------------------//
	Section = _T("General Setting");
	KeyName = _T("Unicode File");
	Default.Format(_T("%d"), FN_DISABLE);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = BOM_FILE_UNICODE_CHK;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }	
	//----------------------------------------------------------------------//	
	Section = _T("Column Setting");
	KeyName = _T("N Columns");
	Default.Format(_T("%d"), 0);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	DataTypeCount = ::_ttoi(String);
	m_ColTypeList.clear();

	for ( i=0; i<DataTypeCount; i++ )
	{		
		KeyName.Format(_T("Columns Data Type %d"), i+1);
		Default.Format(_T("%d"), BOM_LIST_HEADER_NULL);
		if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
		{	JetAPI::ShowMessageBox(ErrString);	return false;	}
		nValue = ::_ttoi(String);
		m_ColTypeList.push_back(nValue);
	}	
	//----------------------------------------------------------------------//
	Section = _T("Load BOM Setting");
	//Title的裡面的資料型態
	KeyName = _T("Start Line");	
	Default.Format(_T("%d"), 0);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(BOM_LOAD_FORMAT_START_RAW_EDIT, String);	
	//----------------------------------------------------------------------//		
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnSaveDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	ExecSaveDefaultBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadBomWnd::OnLoadDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	ExecLoadDefaultBtn();
	BuildPreviewListWndHeader();
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//