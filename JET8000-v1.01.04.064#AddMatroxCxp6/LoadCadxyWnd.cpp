// LoadCadxyWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "LoadCadxyWnd.h"
//-------------------------------------------------------------------------------------//
#pragma warning (disable:4786)
#include <set>
#include <string>
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadCadxyWnd dialog
//-------------------------------------------------------------------------------------//
CLoadCadxyWnd::CLoadCadxyWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CLoadCadxyWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadCadxyWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_StartLine = 1;
	this->m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadCadxyWnd)
	DDX_Control(pDX, CADXY_TEXT_FILTER_COMBO, m_TextFilterCombox);
	DDX_Control(pDX, CADXY_DATA_TYPE_COMBOX, m_DataTypeCombox);
	DDX_Control(pDX, CADXY_RESULT_LIST_WND, m_ResultListWnd);
	DDX_Control(pDX, CADXY_PREVIEW_LIST_WND, m_PreviewListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CLoadCadxyWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CLoadCadxyWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(CADXY_OPEN_FILE_BTN, OnOpenFileBtn)
	ON_BN_CLICKED(CADXY_OPEN_CSV_BTN, OnOpenCSVBtn)
	ON_BN_CLICKED(CADXY_OPEN_ASC_BTN, OnOpenASCBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, CADXY_PREVIEW_LIST_WND, OnColumnclickPreviewListWnd)
	ON_CBN_CLOSEUP(CADXY_DATA_TYPE_COMBOX, OnCloseupDataTypeCombox)
	ON_CBN_SELCHANGE(CADXY_DATA_TYPE_COMBOX, OnSelchangeDataTypeCombox)
	ON_BN_CLICKED(CADXY_PREVIEW_BTN, OnPreviewBtn)
	ON_BN_CLICKED(CADXY_LOAD_BTN, OnLoadBtn)
	ON_BN_CLICKED(CADXY_UNIT_MM_RADIO, OnUnitMMRadio)
	ON_BN_CLICKED(CADXY_UNIT_INCH_RADIO, OnUnitInchRadio)
	ON_BN_CLICKED(CADXY_UNIT_DEFINE_RADIO, OnUnitDefineRadio)
	ON_BN_CLICKED(CADXY_SAVE_DEFAULT_BTN, OnSaveDefaultBtn)
	ON_BN_CLICKED(CADXY_LOAD_DEFAULT_BTN, OnLoadDefaultBtn)
	ON_BN_CLICKED(CADXY_DELIMITER_TAB, OnDelimiterTab)
	ON_BN_CLICKED(CADXY_DELIMITER_COMMA, OnDelimiterComma)
	ON_BN_CLICKED(CADXY_DELIMITER_SPACE, OnDelimiterSpace)
	ON_BN_CLICKED(CADXY_DELIMITER_SEMICOLON, OnDelimiterSemicolon)
	ON_BN_CLICKED(CADXY_DELIMITER_OTHER, OnDelimiterOther)
	ON_BN_CLICKED(CADXY_MATCH_ALIAS_BTN, OnMatchAliasBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CLoadCadxyWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CLoadCadxyWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	this->SetDlgItemInt(CADXY_COMPONENT_WIDTH_EDIT, 1000);
	this->SetDlgItemInt(CADXY_COMPONENT_HEIGHT_EDIT, 1000);
	this->SetDlgItemInt(CADXY_LOAD_FORMAT_START_RAW_EDIT, this->m_StartLine);
	
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_NULL);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_COMPONENT);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_POSITIONX);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_POSITIONY);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_ANGLE);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_MODEL_NAME);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_PART_NUMBER);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_BOARD_ID);
	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_NOZZLE);	

	JetAPI::InitialListCtrl(m_PreviewListWnd);
	this->BuildPreviewListWndHeader();	

	JetAPI::InitialListCtrl(m_ResultListWnd);
	this->BuildResultListWndHeader();

	this->m_DataTypeCombox.ShowWindow(SW_HIDE);	
	this->BuildDataTypeCombox();
	AOIDataDefine.BuildLoadCadxyTextFilterCombox(m_TextFilterCombox);
	this->CheckDlgButton(CADXY_DELIMITER_COMMA, TRUE);
	this->CheckDlgButton(CADXY_UNIT_MM_RADIO, TRUE);
	this->OnUnitMMRadio();		
	
	this->SwitchMultiLanguage();
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
void CLoadCadxyWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnSize(UINT nType, int cx, int cy) 
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
void CLoadCadxyWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_LOAD_CADXY_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_LOAD_CADXY_WND;
	WndKey = _T("IDD_LOAD_CADXY_WND");
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
	WndID = CADXY_COMPONENT_WIDTH_LABEL;
	WndKey = _T("CADXY_COMPONENT_WIDTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_COMPONENT_HEIGHT_LABEL;
	WndKey = _T("CADXY_COMPONENT_HEIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CADXY_OPEN_FILE_BTN;
	WndKey = _T("CADXY_OPEN_FILE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_OPEN_CSV_BTN;
	WndKey = _T("CADXY_OPEN_CSV_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_OPEN_ASC_BTN;
	WndKey = _T("CADXY_OPEN_ASC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_FILE_UNICODE_CHK;
	WndKey = _T("CADXY_FILE_UNICODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_BOARD_NAME_LABEL;
	WndKey = _T("CADXY_BOARD_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_SAVE_DEFAULT_BTN;
	WndKey = _T("CADXY_SAVE_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_LOAD_DEFAULT_BTN;
	WndKey = _T("CADXY_LOAD_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CADXY_DELIMITER_GROUP;
	WndKey = _T("CADXY_DELIMITER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_DELIMITER_TAB;
	WndKey = _T("CADXY_DELIMITER_TAB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_DELIMITER_COMMA;
	WndKey = _T("CADXY_DELIMITER_COMMA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_DELIMITER_SPACE;
	WndKey = _T("CADXY_DELIMITER_SPACE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_DELIMITER_SEMICOLON;
	WndKey = _T("CADXY_DELIMITER_SEMICOLON");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_DELIMITER_OTHER;
	WndKey = _T("CADXY_DELIMITER_OTHER");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CADXY_UNIT_GROUP;
	WndKey = _T("CADXY_UNIT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_UNIT_MM_RADIO;
	WndKey = _T("CADXY_UNIT_MM_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_UNIT_INCH_RADIO;
	WndKey = _T("CADXY_UNIT_INCH_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_UNIT_DEFINE_RADIO;
	WndKey = _T("CADXY_UNIT_DEFINE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CADXY_PREVIEW_BTN;
	WndKey = _T("CADXY_PREVIEW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_LOAD_FORMAT_START_RAW_LABEL;
	WndKey = _T("CADXY_LOAD_FORMAT_START_RAW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = CADXY_TEXT_FILTER_LABEL;
	WndKey = _T("CADXY_TEXT_FILTER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CADXY_LOAD_BTN;
	WndKey = _T("CADXY_LOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_MATCH_ALIAS_BTN;
	WndKey = _T("CADXY_MATCH_ALIAS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CLoadCadxyWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_LOAD_CADXY_WND");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::SetProjectPtr(CAOIProject *Ptr)
{
	if ( NULL == Ptr ) { return false; }
	CAOIPanel *PanelPtr = Ptr->GetProjectActivePanel();
	if ( NULL == PanelPtr ) { return false; }
	m_ProjectPtr = Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnOpenFileBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("TXT Files (*.txt)|*.txt|ASC Files (*.asc)|*.asc|PRN Files (*.prn)|*.asc|CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("txt;asc;prn;csv"), _T("*.txt"), OFN_FILEMUSTEXIST, szFilters);
	//dialog.m_ofn.lpstrInitialDir = pWizard->GetProjectDirectory();	
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	this->m_Filename = dialog.GetPathName();
	this->OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnOpenCSVBtn() 
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
void CLoadCadxyWnd::OnOpenASCBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("TXT Files (*.txt)|*.txt|ASC Files (*.asc)|*.asc|PRN Files (*.prn)|*.asc|CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("txt;asc;prn;csv"), _T("*.asc"), OFN_FILEMUSTEXIST, szFilters);
	//dialog.m_ofn.lpstrInitialDir = pWizard->GetProjectDirectory();	
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	this->m_Filename = dialog.GetPathName();
	this->OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
char CLoadCadxyWnd::GetTextFilter()
{
	char ch = 0x00;
	const int CurSel = this->m_TextFilterCombox.GetCurSel();
	switch ( CurSel )
	{
	case 1:
		ch = '"';
		break;
	case 2:
		ch = '\'';
		break;
	default:
		ch = 0x00;
		break;
	}
	return ch;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::BuildDelimiterList()
{
	BOOL bCheck = TRUE;
	m_DelimiterList.clear();
	m_wDelimiterList.clear();
	
	bCheck = this->IsDlgButtonChecked(CADXY_DELIMITER_TAB);
	if ( TRUE == bCheck )
	{	
		//m_DelimiterList.push_back(0x09); 
		m_DelimiterList.push_back('\t'); 
		m_wDelimiterList.push_back(L'\t'); 
	}

	bCheck = this->IsDlgButtonChecked(CADXY_DELIMITER_COMMA);
	if ( TRUE == bCheck )
	{	
		m_DelimiterList.push_back(','); 
		m_wDelimiterList.push_back(L','); 
	}

	bCheck = this->IsDlgButtonChecked(CADXY_DELIMITER_SPACE);
	if ( TRUE == bCheck )
	{	
		m_DelimiterList.push_back(' '); 
		m_wDelimiterList.push_back(' '); 
	}

	bCheck = this->IsDlgButtonChecked(CADXY_DELIMITER_SEMICOLON);
	if ( TRUE == bCheck )
	{	
		m_DelimiterList.push_back(';'); 
		m_wDelimiterList.push_back(';'); 
	}
	
	bCheck = this->IsDlgButtonChecked(CADXY_DELIMITER_OTHER);
	if ( TRUE == bCheck )	
	{	
		//unicode 需要轉成Ansi
		//m_DelimiterList
		//m_wDelimiterList
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::BuildPreviewListWndHeader()
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
		switch ( this->m_ColTypeList[i] )
		{		
		case CADXY_LIST_HEADER_MODEL_NAME:
			SubText = _T("Model Name");			
			break;
		case CADXY_LIST_HEADER_COMPONENT:
			SubText = _T("Component");
			break;
		case CADXY_LIST_HEADER_POSITIONX:
			SubText = _T("X");			
			break;
		case CADXY_LIST_HEADER_POSITIONY:
			SubText = _T("Y");
			break;
		case CADXY_LIST_HEADER_ANGLE:
			SubText = _T("Angle");
			break;
		case CADXY_LIST_HEADER_PART_NUMBER:
			SubText = _T("Part Number");
			break;
		case CADXY_LIST_HEADER_BOARD_ID:
			SubText = _T("Board ID");
			break;
		case CADXY_LIST_HEADER_NOZZLE:
			SubText = _T("Nozzle Name");
			break;
		default:
			SubText = _T("NULL");
			break;
		}
		SubText = LoadMultiLanguageString(SubText, SubText);
		ListWnd.InsertColumn(i, SubText,LVCFMT_CENTER,ColumnsWidth);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::BuildPreviewListWnd()
{
	CListCtrl &ListWnd = m_PreviewListWnd;
	JetAPI::ClearListCtrl(ListWnd, TRUE);
	
	size_t textlen = 0;
	const size_t textlinesize = 1024;
	char Textline[textlinesize]= "";
	char ansiString[textlinesize]= "";	
	wchar_t wTextline[textlinesize]= L"";
	wchar_t wideString[textlinesize]= L"";	
	CString SubText;
	size_t i=0, j=0, k=0;	
	const size_t NColumns = m_ColTypeList.size();	
	const bool bUnicode = (bool)(CWnd::IsDlgButtonChecked(CADXY_FILE_UNICODE_CHK));
	const char SpecDelimiter = ' ';//空白字元是區間內的特殊字元	
	const wchar_t wSpecDelimiter = L' ';//空白字元是區間內的特殊字元	
	const size_t NDelimiters = m_DelimiterList.size();	
	const size_t wNDelimiters = m_wDelimiterList.size();
	if ( wNDelimiters != NDelimiters )
	{
		m_ErrorString = _T("Error, wNDelimiters != NDelimiters");
		return false;
	}
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
	size_t DataLine = 0;
	size_t chidx=0;
	int    NRows = 0;//第幾列
	int    CurrentNColumns = 0;	
	//--------------------------------------------------------------------//
	COLORREF Color1 = 0xFFCFCF;
	COLORREF Color2 = 0xCFCFFF;
	//--------------------------------------------------------------------//	
	while(!feof(pfile) )
	{ 
		DataLine++;
		if ( false == bUnicode )
		{
			if( fgets(Textline,textlinesize,pfile) ==NULL ) 
			{	continue;	}
		}
		else
		{
			if( fgetws(wTextline,textlinesize,pfile) ==NULL ) 
			{	continue;	}
		}
		//未達到使用者要求的開始列數
		if ( DataLine < this->m_StartLine ) { continue; }

		if ( false == bUnicode )
		{	textlen=(int)strlen(Textline); }
		else
		{	textlen=(int)wcslen(wTextline); }
		SubText.Format(_T("%d"), DataLine);
		this->m_PreviewListWnd.InsertItem(NRows, SubText);

		//if ( NRows%2 == 0 ) { this->m_PreviewListWnd.SetItemTextBKColor(NRows, Color1); }
		//else { this->m_PreviewListWnd.SetItemTextBKColor(NRows, Color2); }

		if ( textlen == 0 ) //enter鍵
		{	
			DataLine ++;
			NRows ++;
			continue;	
		}
		if ( false == bUnicode )
		{	::memset(ansiString, 0x00, textlinesize*sizeof(char)); }
		else
		{	::memset(wideString, 0x00, textlinesize*sizeof(wchar_t)); }

		CurrentNColumns = 0;
		k = 0;
		chidx = 0;
		//和分隔字元比較
		CurrentNColumns = 0;		
		chidx = 0;		
		do
		{
			//------先確認一開始的字元是否為分隔字元-----------------//
			if ( CurrentNColumns == 0 )
			{
				for ( i=0; i<NDelimiters; i++ )
				{
					if ( false == bUnicode )
					{
						if ( Textline[chidx] == m_DelimiterList[i] )
						{	break;	}
					}
					else
					{
						if ( wTextline[chidx] == m_wDelimiterList[i] )
						{	break;	}
					}
				}
				if ( i == NDelimiters )
				{
					//如果一開始不是分隔字元					
					CurrentNColumns ++;
					//濾除分隔字元與資料間的空白字元
					for ( i=chidx; i<textlen; i++ )
					{
						if ( false == bUnicode )
						{	
							if ( Textline[i] != SpecDelimiter ) 
							{ break; }
						}
						else
						{
							if ( wTextline[i] != wSpecDelimiter ) 
							{ break; }
						}
					}
					chidx = i;
					continue;
				}
				else
				{
					chidx ++;
					continue;
				}
			}
			
			//尋找分隔字元
			for ( i=0; i<NDelimiters; i++ )
			{				
				if ( false == bUnicode )
				{
					if ( Textline[chidx] == m_DelimiterList[i] )
					{	break;	}
				}
				else
				{
					if ( wTextline[chidx] == m_wDelimiterList[i] )
					{	break;	}
				}
			}
			if ( i != NDelimiters )
			{				
				//如果有找到分隔字元
				//將資料放進去預覽表單
				if ( false == bUnicode )
				{
					ansiString[k] = '\0';
					SubText = ansiString;
				}
				else
				{
					wideString[k] = L'\0';
					SubText = wideString;
				}
				this->m_PreviewListWnd.SetItemText(NRows, CurrentNColumns, SubText);
				k = 0;

				for ( i=chidx+1; i<textlen; i++ )
				{
					//濾除分隔字元與資料間的空白字元
					if ( false == bUnicode )
					{
						if ( Textline[i] == SpecDelimiter ) 
						{ continue; }
					}
					else
					{
						if ( wTextline[i] == wSpecDelimiter ) 
						{ continue; }
					}

					for ( j=0; j<NDelimiters; j++ )
					{
						if ( false == bUnicode )
						{
							if ( Textline[i] == m_DelimiterList[j] )
							{	break;		}
						}
						else
						{
							if ( wTextline[i] == m_wDelimiterList[j] )
							{	break;		}
						}
					}

					if ( j == NDelimiters )
					{	
						//如果有找到下一筆資料的話						
						CurrentNColumns ++;
						chidx = i;
						break;
					}
					
				}
				if ( i == textlen )
				{
					chidx = i;
					break;
				}
			}
			else
			{
				//如果沒有找到分隔字元
				if ( false == bUnicode )
				{	ansiString[k] = Textline[chidx]; }
				else
				{	wideString[k] = wTextline[chidx]; }
				chidx ++;
				k ++;
				if ( chidx >= textlen )
				{
					if ( false == bUnicode )
					{
						ansiString[k] = '\0';
						SubText = ansiString;
					}
					else
					{
						wideString[k] = L'\0';
						SubText = wideString;
					}
					this->m_PreviewListWnd.SetItemText(NRows, CurrentNColumns, SubText);
					break;
				}
			}
		} while ( chidx < textlen );		
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
bool CLoadCadxyWnd::BuildResultListWndHeader()
{
	int         index=0;
	int         Width = 0;
	CString     SubText;
	RECT        ListClientRect={0};
	CListCtrl  &ListWnd = m_ResultListWnd;

	ListWnd.GetClientRect(&ListClientRect);
	Width = (ListClientRect.right-ListClientRect.left-32)/9;

	index = 0;
	SubText = _T("Index");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;

	SubText = _T("Component");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(1, SubText, LVCFMT_CENTER, Width);	index++;

	SubText = _T("X (um)");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;

	SubText = _T("Y (um)");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	

	SubText = _T("Angle");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	

	SubText = _T("Model Name");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	

	SubText = _T("Part Number");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	

	SubText = _T("Board ID");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	

	SubText = _T("Nozzle Name");
	SubText = LoadMultiLanguageString(SubText, SubText);
	ListWnd.InsertColumn(index, SubText, LVCFMT_CENTER, Width);	index++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::BuildResultListWnd()
{
	CListCtrl  &ListWnd = m_ResultListWnd;
	JetAPI::ClearListCtrl(ListWnd, TRUE);		
	this->BuildResultListWndHeader();
	if ( NULL == m_ProjectPtr ) 
	{	return false; }

	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	size_t i=0;
	int    idx=0, subidx=0;
	CString ItemText;
	CAOIBoard     *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t NComponents = PanelPtr->GetPanelComponentCount();

	idx=0;
	ListWnd.SetRedraw(FALSE);
	for ( i=0; i<NComponents; i++ )
	{
		ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }

		subidx=0;

		ItemText.Format(_T("%d"), i+1);
		ListWnd.InsertItem(idx, ItemText);

		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = ComponentPtr->GetComponentName();
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText.Format(_T("%.0f"), ComponentPtr->GetComponentCadPosX());
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText.Format(_T("%.0f"), ComponentPtr->GetComponentCadPosY());
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText.Format(_T("%.2f"), ComponentPtr->GetComponentAngle());
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = ComponentPtr->GetComponentModelName();
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = ComponentPtr->GetComponentPartNumber();
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL != BoardPtr )
		{	ItemText.Format(_T("%d"), BoardPtr->GetBoardIndex_Panel()+1);	}
		else
		{	ItemText.Format(_T("%d"), 0);	}
		//ItemText.Format(_T("%d"), ComponentPtr->GetComponentBoardIndex_Panel()+1);
		ListWnd.SetItemText(idx, subidx, ItemText);
		subidx ++;

		ItemText = ComponentPtr->GetComponentNozzleName();
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
void CLoadCadxyWnd::OnColumnclickPreviewListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CLoadCadxyWnd::OnCloseupDataTypeCombox() 
{
	// TODO: Add your control notification handler code here
	this->m_DataTypeCombox.ShowWindow(SW_HIDE);
	this->m_PreviewListWnd.EnableWindow();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnSelchangeDataTypeCombox() 
{
	// TODO: Add your control notification handler code here
	const size_t Cols = m_ColTypeList.size();
	if ( m_CurColIndex < 1 ) { return; }
	if ( m_CurColIndex >= Cols ) { return ; }
	
	TCHAR Text[64]=_T("");
	this->m_ColTypeList[m_CurColIndex] = this->m_DataTypeCombox.GetCurSel();
	this->m_DataTypeCombox.GetLBText(m_DataTypeCombox.GetCurSel(), Text);
	
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
void CLoadCadxyWnd::OnPreviewBtn() 
{
	// TODO: Add your control notification handler code here
	bool     bIsOK=true;
	size_t   NCols = 0;
	size_t   NRows = 0;
	this->BuildDelimiterList();
	const double Factor = this->GetUnitFactor();
	const bool bUnicode = (bool)(CWnd::IsDlgButtonChecked(CADXY_FILE_UNICODE_CHK));
	this->m_StartLine = (int)(this->GetDlgItemInt(CADXY_LOAD_FORMAT_START_RAW_EDIT));
	
	CWnd::SetDlgItemText(CADXY_LOAD_FILE_NAME_EDIT, m_Filename);
	if ( false == bUnicode )
	{	bIsOK = JetAPI::PreLoadCADXYFile(m_Filename, m_StartLine, m_DelimiterList, NCols, NRows, this->m_ErrorString); }
	else
	{	bIsOK = JetAPI::PreLoadCADXYFile(m_Filename, m_StartLine, m_wDelimiterList, NCols, NRows, this->m_ErrorString);	}
	if ( false == bIsOK )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return;
	}	

	size_t i = 0;
	const size_t ColCount = this->m_ColTypeList.size();
	if ( ColCount <= NCols )
	{
		for ( i=ColCount; i<NCols; i++ )
		{	this->m_ColTypeList.push_back(CADXY_LIST_HEADER_NULL); }
	}
	else
	{
		std::vector<int> ColTypeListTemp = m_ColTypeList;
		m_ColTypeList.clear();
		for ( i=0; i<NCols; i++ )
		{	m_ColTypeList.push_back(ColTypeListTemp[i]);	}
	}	
	
	if ( this->BuildPreviewListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return;
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	size_t   NCols = 0;
	size_t   NRows = 0;
	char     BoardName[256]="";
	std::vector<CString> NGList;
	char TextFilter = GetTextFilter();		
	this->BuildDelimiterList();	
	const double Factor = GetUnitFactor();
	const double ComW = GetDlgItemInt(CADXY_COMPONENT_WIDTH_EDIT);
	const double ComH = GetDlgItemInt(CADXY_COMPONENT_HEIGHT_EDIT);

	CWnd::GetDlgItemText(CADXY_BOARD_NAME_EDIT, str);
	if ( str.GetLength() > 0 ) 
	{	JetAPI::TCHAR2char(str, BoardName, 256);	}	
	m_StartLine = (int)(GetDlgItemInt(CADXY_LOAD_FORMAT_START_RAW_EDIT));

	//m_wDelimiterList
	if ( LoadCADXYFile(m_Filename, m_StartLine, m_ColTypeList, m_DelimiterList, m_wDelimiterList, TextFilter, Factor, ComW, ComH, BoardName) == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}

	if ( this->CheckCompoentData(NGList) == false ) 
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}

	if ( this->BuildResultListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}

	/*
	str.Format("%s\\CADXY.csv", TEMP_DEFULAT_DIRECTORY);
	if ( this->ExportCADXYFile(str) == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}
	
	if ( this->FillCADXYListData() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}  
	*/
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::LoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<int> &ColumnDef, const std::vector<char> &Delimiters, const std::vector<wchar_t> &wDelimiters, char TextFilter, const double Factor, double ComW, double ComH, char BoardName[])//將CADXY檔案讀入資料陣列(CADX_CS)
{
	if ( NULL == m_ProjectPtr ) 
	{	return false; }
	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();	
	const bool  bUnicode = (bool)(CWnd::IsDlgButtonChecked(CADXY_FILE_UNICODE_CHK));
	DISTRICT_ID DistrictID = Project->GetProjectActDistrictID();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	//將CADXY檔案讀入資料陣列(CADX_CS)
	if ( Project->LoadProjectCadxyFile(pfilename, PanelPtr, bUnicode, StartLine, ColumnDef, Delimiters, wDelimiters, TextFilter, Factor, ComW, ComH, BoardName) == false )
	{
		m_ErrorString = Project->GetErrorString();
		return false;
	}

	double PosX = 0;
	double PosY = 0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, OfflineMode);	
	if ( PanelPtr->SetPanelStagePos(PosX, PosY, DistrictID) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnUnitMMRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(FALSE);
	pWnd->SetWindowText(_T("1.00"));
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnUnitInchRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(FALSE);
	pWnd->SetWindowText(_T("25.40"));
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnUnitDefineRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(TRUE);
	pWnd->SetWindowText(_T("1.00"));
}
//-------------------------------------------------------------------------------------//
double CLoadCadxyWnd::GetUnitFactor()
{
	CString str;
	this->GetDlgItemText(CADXY_UNIT_DEFINE_EDIT, str);
	return ::_tcstod(str, NULL)*1000.0;	
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::BuildDataTypeCombox()
{
	int idx = 0;
	CString str;

	idx=0;
	JetAPI::ClearCombox(m_DataTypeCombox);

	str = _T("NULL Col");	
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_NULL);
	idx ++;

	str = _T("Model Name Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_MODEL_NAME);
	idx ++;

	str = _T("Component Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_COMPONENT);
	idx ++;

	str = _T("X Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_POSITIONX);
	idx ++;

	str = _T("Y Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_POSITIONY);
	idx ++;

	str = _T("Angle Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_ANGLE);
	idx ++;

	str = _T("Part Number Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_PART_NUMBER);
	idx ++;

	str = _T("Board ID Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_BOARD_ID);
	idx ++;

	str = _T("Nozzle Name Col");
	str = LoadMultiLanguageString(str, str);
	this->m_DataTypeCombox.AddString(str);
	this->m_DataTypeCombox.SetItemData(idx, CADXY_LIST_HEADER_NOZZLE);
	idx ++;
}
//-------------------------------------------------------------------------------------//
CString CLoadCadxyWnd::GetSaveDefaultFilename()
{
	CString IniFile;
	IniFile.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("LoadCadXY.INI"));	
	return IniFile;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::ExecSaveDefaultBtn()
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
	if ( CWnd::IsDlgButtonChecked(CADXY_FILE_UNICODE_CHK) == TRUE )	
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
	Section = _T("Load CADXY Setting");
	//Title的裡面的資料型態
	KeyName = _T("Start Line");	
	CWnd::GetDlgItemText(CADXY_LOAD_FORMAT_START_RAW_EDIT, String);	
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}	
	//----------------------------------------------------------------------//	
	//文字的贅字	
	DWORD_PTR CurSelData = JetAPI::GetComboxCurSelData(m_TextFilterCombox);
	KeyName = _T("Text Delimiter");	
	String.Format(_T("%d"), CurSelData);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//
	KeyName = _T("Board Name");	
	CWnd::GetDlgItemText(CADXY_BOARD_NAME_EDIT, String);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//
	//欄位分格符號
	//Tab Delimiter
	KeyName = _T("Tab Delimiter");
	if ( CWnd::IsDlgButtonChecked(CADXY_DELIMITER_TAB) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	//Comma Delimiter
	KeyName = _T("Comma Delimiter");
	if ( CWnd::IsDlgButtonChecked(CADXY_DELIMITER_COMMA) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	//Space Delimiter
	KeyName = _T("Space Delimiter");
	if ( CWnd::IsDlgButtonChecked(CADXY_DELIMITER_SPACE) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	//Semicolon Delimiter
	KeyName = _T("Semicolon Delimiter");
	if ( CWnd::IsDlgButtonChecked(CADXY_DELIMITER_SEMICOLON) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	//Other Delimiter
	KeyName = _T("Other Delimiter");
	if ( CWnd::IsDlgButtonChecked(CADXY_DELIMITER_OTHER) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}

	KeyName = _T("Others Delimiter");
	CWnd::GetDlgItemText(CADXY_DELIMITER_OTHER_EDIT, String);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	//單位換算
	KeyName = _T("CADXY Unit Mode");
	if ( CWnd::IsDlgButtonChecked(CADXY_UNIT_MM_RADIO) == TRUE )	
	{	String.Format(_T("%d"), 1); }
	else if ( CWnd::IsDlgButtonChecked(CADXY_UNIT_INCH_RADIO) == TRUE )
	{	String.Format(_T("%d"), 2); }
	else 
	{	String.Format(_T("%d"), 3); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}

	KeyName = _T("CADXY Unit Factor");	
	CWnd::GetDlgItemText(CADXY_UNIT_DEFINE_EDIT, String);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	KeyName = _T("CADXY Component Size W");	
	CWnd::GetDlgItemText(CADXY_COMPONENT_WIDTH_EDIT, String);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}

	KeyName = _T("CADXY Component Size H");	
	CWnd::GetDlgItemText(CADXY_COMPONENT_HEIGHT_EDIT, String);
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::ExecLoadDefaultBtn()
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
	CtrlID = CADXY_FILE_UNICODE_CHK;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }	
	//----------------------------------------------------------------------//
	KeyName = _T("Board Name");	
	Default = _T("");
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_BOARD_NAME_EDIT, String);			
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
		Default.Format(_T("%d"), CADXY_LIST_HEADER_NULL);
		if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
		{	JetAPI::ShowMessageBox(ErrString);	return false;	}
		nValue = ::_ttoi(String);
		m_ColTypeList.push_back(nValue);
	}	
	//----------------------------------------------------------------------//
	Section = _T("Load CADXY Setting");
	//Title的裡面的資料型態
	KeyName = _T("Start Line");	
	Default.Format(_T("%d"), 0);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_LOAD_FORMAT_START_RAW_EDIT, String);	
	//----------------------------------------------------------------------//	
	//文字的贅字		
	KeyName = _T("Text Delimiter");	
	Default.Format(_T("%d"), 0);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	JetAPI::SetComboxCurSel(m_TextFilterCombox, nValue);	
	//----------------------------------------------------------------------//
	//欄位分格符號
	//Tab Delimiter
	KeyName = _T("Tab Delimiter");
	Default.Format(_T("%d"), FN_DISABLE);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_DELIMITER_TAB;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }	
	//----------------------------------------------------------------------//	
	//Comma Delimiter
	KeyName = _T("Comma Delimiter");
	Default.Format(_T("%d"), FN_DISABLE);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_DELIMITER_COMMA;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }	
	//----------------------------------------------------------------------//	
	//Space Delimiter
	KeyName = _T("Space Delimiter");
	Default.Format(_T("%d"), FN_DISABLE);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_DELIMITER_SPACE;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }
	//----------------------------------------------------------------------//	
	//Semicolon Delimiter
	KeyName = _T("Semicolon Delimiter");
	Default.Format(_T("%d"), FN_DISABLE);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_DELIMITER_SEMICOLON;
	if ( FN_ENABLE == nValue )
	{	CWnd::CheckDlgButton(CtrlID, TRUE); }
	else
	{	CWnd::CheckDlgButton(CtrlID, FALSE); }
	//----------------------------------------------------------------------//	
	//Other Delimiter
	KeyName = _T("Other Delimiter");
	Default.Format(_T("%d"), FN_DISABLE);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_DELIMITER_OTHER;
	if ( FN_ENABLE == nValue )
	{	
		CWnd::CheckDlgButton(CtrlID, TRUE); 
		JetAPI::EnableCtrlWnd(this, CADXY_DELIMITER_OTHER_EDIT, TRUE);
	}
	else
	{	
		CWnd::CheckDlgButton(CtrlID, FALSE); 
		JetAPI::EnableCtrlWnd(this, CADXY_DELIMITER_OTHER_EDIT, FALSE);
	}

	KeyName = _T("Others Delimiter");
	Default = _T("");
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_DELIMITER_OTHER_EDIT, String);	
	//----------------------------------------------------------------------//	
	//單位換算
	KeyName = _T("CADXY Unit Mode");
	Default.Format(_T("%d"), 1);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	switch ( nValue )
	{
	case 2:
		CWnd::CheckDlgButton(CADXY_UNIT_MM_RADIO, FALSE);
		CWnd::CheckDlgButton(CADXY_UNIT_DEFINE_RADIO, FALSE);
		CWnd::CheckDlgButton(CADXY_UNIT_INCH_RADIO, TRUE);
		JetAPI::EnableCtrlWnd(this, CADXY_UNIT_DEFINE_EDIT, FALSE);
		break;
	case 3:
		CWnd::CheckDlgButton(CADXY_UNIT_MM_RADIO, FALSE);		
		CWnd::CheckDlgButton(CADXY_UNIT_INCH_RADIO, FALSE);
		CWnd::CheckDlgButton(CADXY_UNIT_DEFINE_RADIO, TRUE);
		JetAPI::EnableCtrlWnd(this, CADXY_UNIT_DEFINE_EDIT, TRUE);
		break;
	default:		
		CWnd::CheckDlgButton(CADXY_UNIT_INCH_RADIO, FALSE);
		CWnd::CheckDlgButton(CADXY_UNIT_DEFINE_RADIO, FALSE);
		CWnd::CheckDlgButton(CADXY_UNIT_MM_RADIO, TRUE);
		JetAPI::EnableCtrlWnd(this, CADXY_UNIT_DEFINE_EDIT, FALSE);
		break;
	}

	KeyName = _T("CADXY Unit Factor");	
	Default.Format(_T("%d"), 1);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_UNIT_DEFINE_EDIT, String);	
	//----------------------------------------------------------------------//	
	KeyName = _T("CADXY Component Size W");	
	Default.Format(_T("%d"), 1000);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_COMPONENT_WIDTH_EDIT, String);		

	KeyName = _T("CADXY Component Size H");	
	Default.Format(_T("%d"), 1000);	
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	CWnd::SetDlgItemText(CADXY_COMPONENT_HEIGHT_EDIT, String);		
	//----------------------------------------------------------------------//	
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnSaveDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	ExecSaveDefaultBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnLoadDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	ExecLoadDefaultBtn();
	BuildPreviewListWndHeader();
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::CheckCompoentData(std::vector<CString> &NGList)//確定零件資料是否正確
{
	if ( NULL == m_ProjectPtr ) 
	{	return false; }
	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	size_t         s=0, t=0;
	size_t         i=0, j=0, k=0;	
	size_t         ComponentCount = 0;
	CAOIBoard     *BoardPtr = NULL;
	CString        str;
	CString        ComName1;
	CString        ComName2;
	CString        PartNumber;
	CAOIComponent *ComponentPtr1 = NULL;
	CAOIComponent *ComponentPtr2 = NULL;
	const size_t BoardCount = PanelPtr->GetPanelBoardCount();	

	NGList.clear();
	Project->SelectProjectAllComponents(false);
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }

		ComponentCount = BoardPtr->GetBoardComponentCount();
		for ( j=0; j<ComponentCount; j++ )
		{
			s = ComponentCount-j-1;
			ComponentPtr1 = BoardPtr->GetBoardComponentPtr(s, false);
			if ( NULL == ComponentPtr1 ) { continue; }			
			ComName1 = ComponentPtr1->GetComponentName();
			PartNumber = ComponentPtr1->GetComponentPartNumber();
			PartNumber.TrimLeft(); 
			PartNumber.TrimRight();
			if ( PartNumber.GetLength() == 0 ) 
			{
				str.Format(_T("Board %d  %s no part number."), i+1, ComName1);
				NGList.push_back(str);
				ComponentPtr1->SetComponentSelected(true);
				continue;
			}

			for ( k=j+1; k<ComponentCount; k++ )
			{
				t = ComponentCount-k-1;
				ComponentPtr2 = BoardPtr->GetBoardComponentPtr(t, false);
				if ( NULL == ComponentPtr2 ) { continue; }
				ComName2 = ComponentPtr2->GetComponentName();
				if ( ComName1.CompareNoCase(ComName2) != 0 ) { continue; }

				str.Format(_T("Board %d  %s is repeated."), i+1, ComName1);
				NGList.push_back(str);
				ComponentPtr1->SetComponentSelected(true);
				break;
			}
		}
	}
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(Project);
	Project->DeleteProjectComponentSelected();
	PanelPtr->SetPanelSelected(true);

	const size_t NGCount = NGList.size();
	SaveNGComponentList(NGList);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::SaveNGComponentList(const std::vector<CString> &NGList)
{
	const size_t NGCount = NGList.size();
	if ( 0 == NGCount ) { return true; }
	
	size_t i=0;
	FILE *pfile = NULL;
	CString filename;
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("LoadCadXYNG.TXT"));
	pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile ) { return false; }
	
	::_ftprintf(pfile, _T("%s\n"), _T("NG Component List"));
	for ( i=0; i<NGCount; i++ )
	{	::_ftprintf(pfile, _T("%s\n"), (LPCTSTR)(NGList[i]));	}
	::fclose(pfile); pfile=NULL;
	::Sleep(0);
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::SaveLoadCADXYResult()
{	
	if ( NULL == m_ProjectPtr ) 
	{	return false; }

	size_t         i=0, j=0;
	size_t         ComponentCount=0;
	CAOIBoard     *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	CAOIProject   *ProjectPtr = m_ProjectPtr;
	CAOIPanel     *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	const size_t BoardCount = PanelPtr->GetPanelBoardCount();
#ifdef _DEBUG
	bool Debug = false;
	if ( true == Debug )
	{	//輸出確認
		FILE *pfile = NULL;
		CString filename;
		TCHAR   TMode[32] = _T("");
		_tcscpy(TMode, _T("w+"));
		JetAPI::ModifyOpenFileMode_Write(TMode);
		filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("LoadCadXYOutput.TXT"));
		pfile = ::_tfopen(filename, TMode);
		if ( NULL != pfile )
		{			
			::_ftprintf(pfile, L"Board, Index, Component, Cad-X, Cad-Y, Angle\n");
			for ( i=0; i<BoardCount; i++ )
			{
				BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
				if ( NULL == BoardPtr ) { continue; }

				ComponentCount = BoardPtr->GetBoardComponentCount();
				for ( j=0; j<ComponentCount; j++ )
				{					
					ComponentPtr = BoardPtr->GetBoardComponentPtr(j, false);
					if ( NULL == ComponentPtr ) { continue; }
					::_ftprintf(pfile, L"%d, %d, %s, %.0f, %.0f, %.2f\n", i+1, j+1, ComponentPtr->GetComponentName(), 
						ComponentPtr->GetComponentCadPosX(), ComponentPtr->GetComponentCadPosY(), ComponentPtr->GetComponentAngle());
				}
			}			
			::fclose(pfile); pfile=NULL;
			::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
		}		
	}
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnDelimiterTab() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnDelimiterComma() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnDelimiterSpace() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnDelimiterSemicolon() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnDelimiterOther() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(CADXY_DELIMITER_OTHER);
	JetAPI::EnableCtrlWnd(this, CADXY_DELIMITER_OTHER_EDIT, bCheck);
	if ( TRUE == bCheck )
	{
		CWnd *pWnd = CWnd::GetDlgItem(CADXY_DELIMITER_OTHER_EDIT);
		if ( NULL != pWnd )
		{	pWnd->SetFocus(); }
	}
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CLoadCadxyWnd::OnMatchAliasBtn() 
{
	// TODO: Add your control notification handler code here
	ExecMatchAliasName();
}
//-------------------------------------------------------------------------------------//
bool CLoadCadxyWnd::ExecMatchAliasName()//合併料號別名
{
	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	CString      str;
	CString      LibraryAliasFile;
	std::vector<TAliasNode> AliasList;
	LibraryAliasFile = AOIDataCollect.GetAOILibraryAliasFilename();		
	if ( AOIDataCollect.LoadAliasFile(LibraryAliasFile, AliasList) == false ) 
	{
		m_ErrorString = AOIDataCollect.GetErrorString();
		return false;
	}
	if ( PanelPtr->ReplacePanelComponentModelName(AliasList) == false )
	{
		m_ErrorString = _T("Error, Replace Component Model Name Fault");
		return false;
	}
	if ( this->BuildResultListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//