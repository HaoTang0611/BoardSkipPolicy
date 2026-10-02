// NewProjectLoadCadxyPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectLoadCadxyPane.h"
//-------------------------------------------------------------------------------------//
#pragma warning (disable:4786)
#include <set>
#include <string>
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "ComponentConfigWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneLoadCadxy dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneLoadCadxy::CNewProjectPaneLoadCadxy(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneLoadCadxy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneLoadCadxy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StartLine = 1;
	m_ProjectPtr = NULL;
	m_NewProjectMode = NEW_PROJECT_ONLINE;
	m_EnableMultiDistrictMode = false;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneLoadCadxy)
	DDX_Control(pDX, CADXY_TEXT_FILTER_COMBO, m_TextFilterCombox);
	DDX_Control(pDX, CADXY_DATA_TYPE_COMBOX, m_DataTypeCombox);
	DDX_Control(pDX, CADXY_RESULT_LIST_WND, m_ResultListWnd);
	DDX_Control(pDX, CADXY_PREVIEW_LIST_WND, m_PreviewListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneLoadCadxy, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneLoadCadxy)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(CADXY_OPEN_FILE_BTN, OnOpenFileBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, CADXY_PREVIEW_LIST_WND, OnColumnclickPreviewListWnd)
	ON_CBN_CLOSEUP(CADXY_DATA_TYPE_COMBOX, OnCloseupDataTypeCombox)
	ON_CBN_SELCHANGE(CADXY_DATA_TYPE_COMBOX, OnSelchangeDataTypeCombox)
	ON_BN_CLICKED(CADXY_PREVIEW_BTN, OnPreviewBtn)	
	ON_BN_CLICKED(CADXY_UNIT_MM_RADIO, OnUnitMMRadio)
	ON_BN_CLICKED(CADXY_UNIT_INCH_RADIO, OnUnitInchRadio)
	ON_BN_CLICKED(CADXY_UNIT_DEFINE_RADIO, OnUnitDefineRadio)
	ON_BN_CLICKED(CADXY_LOAD_BTN, OnLoadBtn)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(CADXY_SAVE_DEFAULT_BTN, OnSaveDefaultBtn)
	ON_BN_CLICKED(CADXY_LOAD_DEFAULT_BTN, OnLoadDefaultBtn)
	ON_BN_CLICKED(CADXY_DELIMITER_OTHER, OnDelimiterOther)
	ON_BN_CLICKED(CADXY_DELIMITER_TAB, OnDelimiterTab)
	ON_BN_CLICKED(CADXY_DELIMITER_COMMA, OnDelimiterComma)
	ON_BN_CLICKED(CADXY_DELIMITER_SPACE, OnDelimiterSpace)
	ON_BN_CLICKED(CADXY_DELIMITER_SEMICOLON, OnDelimiterSemicolon)
	ON_BN_CLICKED(CADXY_NEW_COMPONENT_BTN, OnNewComponentBtn)
	ON_BN_CLICKED(CADXY_MATCH_ALIAS_BTN, OnMatchAliasBtn)
	ON_BN_CLICKED(CADXY_LOAD_PID_BTN, OnLoadPidBtn)	
	ON_BN_CLICKED(CADXY_OPEN_CSV_BTN, OnOpenCSVBtn)
	ON_BN_CLICKED(CADXY_OPEN_ASC_BTN, OnOpenASCBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneLoadCadxy message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneLoadCadxy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
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

	CAD_FILE_CONTENT_MODE CadFileContentMode = AOIDataCollect.GetLoadCADFileContentMode();
	switch ( CadFileContentMode ) 
	{
	case CAD_FILE_CONTENT_NORMAL:	CWnd::CheckDlgButton(CADXY_FILE_WITH_FD_CHK, FALSE);	break;
	case CAD_FILE_CONTENT_FIDUCIAL: CWnd::CheckDlgButton(CADXY_FILE_WITH_FD_CHK, TRUE);		break;		
	}
	
	this->CheckDlgButton(CADXY_DELIMITER_COMMA, TRUE);
	this->CheckDlgButton(CADXY_UNIT_MM_RADIO, TRUE);
	this->OnUnitMMRadio();		
	
	this->SwitchMultiLanguage();

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
void CNewProjectPaneLoadCadxy::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
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

void CNewProjectPaneLoadCadxy::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_LOAD_CADXY;
	WndKey = _T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");
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

	WndID = CADXY_FILE_WITH_FD_CHK;
	WndKey = _T("CADXY_FILE_WITH_FD_CHK");
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

	WndID = CADXY_NEW_COMPONENT_BTN;
	WndKey = _T("CADXY_NEW_COMPONENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CADXY_MATCH_ALIAS_BTN;
	WndKey = _T("CADXY_MATCH_ALIAS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CADXY_LOAD_PID_BTN;
	WndKey = _T("CADXY_LOAD_PID_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneLoadCadxy::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_LOAD_CADXY");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneLoadCadxy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::SetProjectPtr(CAOIProject *ProjectPtr)
{
	m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CNewProjectPaneLoadCadxy::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneLoadCadxy::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnOpenFileBtn() 
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
void CNewProjectPaneLoadCadxy::OnColumnclickPreviewListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CNewProjectPaneLoadCadxy::OnCloseupDataTypeCombox() 
{
	// TODO: Add your control notification handler code here
	this->m_DataTypeCombox.ShowWindow(SW_HIDE);
	this->m_PreviewListWnd.EnableWindow();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnSelchangeDataTypeCombox() 
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
double CNewProjectPaneLoadCadxy::GetUnitFactor()
{
	CString str;
	this->GetDlgItemText(CADXY_UNIT_DEFINE_EDIT, str);
	return ::_tcstod(str, NULL)*1000.0;	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::BuildDataTypeCombox()
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
char CNewProjectPaneLoadCadxy::GetTextFilter()
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
bool CNewProjectPaneLoadCadxy::BuildDelimiterList()
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
bool CNewProjectPaneLoadCadxy::BuildPreviewListWndHeader()
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
bool CNewProjectPaneLoadCadxy::BuildPreviewListWnd()
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
bool CNewProjectPaneLoadCadxy::BuildResultListWndHeader()
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
bool CNewProjectPaneLoadCadxy::BuildResultListWnd()
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
void CNewProjectPaneLoadCadxy::OnPreviewBtn() 
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
void CNewProjectPaneLoadCadxy::OnUnitMMRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(FALSE);
	pWnd->SetWindowText(_T("1.00"));
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnUnitInchRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(FALSE);
	pWnd->SetWindowText(_T("25.40"));
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnUnitDefineRadio() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetDlgItem(CADXY_UNIT_DEFINE_EDIT);
	pWnd->EnableWindow(TRUE);
	pWnd->SetWindowText(_T("1.00"));
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	size_t   NCols = 0;
	size_t   NRows = 0;
	size_t   NGCount = 0;
	char     BoardName[256]="";
	char TextFilter = GetTextFilter();		
	this->BuildDelimiterList();	
	std::vector<CString>  NGList;
	const double Factor = GetUnitFactor();
	const double ComW = GetDlgItemInt(CADXY_COMPONENT_WIDTH_EDIT);
	const double ComH = GetDlgItemInt(CADXY_COMPONENT_HEIGHT_EDIT);

	CWnd::GetDlgItemText(CADXY_BOARD_NAME_EDIT, str);
	if ( str.GetLength() > 0 ) 
	{	JetAPI::TCHAR2char(str, BoardName, 256);	}	
	m_StartLine = (int)(this->GetDlgItemInt(CADXY_LOAD_FORMAT_START_RAW_EDIT));

	//m_wDelimiterList
	if ( LoadCADXYFile(m_Filename, m_StartLine, m_ColTypeList, m_DelimiterList, m_wDelimiterList, TextFilter, Factor, ComW, ComH, BoardName) == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}

	if ( CheckCompoentData(NGList) == false ) 
	{	JetAPI::ShowMessageBox(this->m_ErrorString);	}

	NGCount = NGList.size();
	if ( NGCount > 0 ) 
	{
		if ( ReAssignComponentBoard() == false )
		{	JetAPI::ShowMessageBox(this->m_ErrorString); }
		
		if ( CheckCompoentData(NGList) == false ) 
		{	JetAPI::ShowMessageBox(this->m_ErrorString);	}		
	}
	NGCount = NGList.size();
	if ( NGCount > 0 ) 
	{	RemoveNGComponent(); }			
	SaveNGComponentList(NGList);
	SaveLoadCADXYResult();

	if ( BuildResultListWnd() == false )
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
bool CNewProjectPaneLoadCadxy::LoadCADXYFile(LPCTSTR pfilename, size_t StartLine, const std::vector<int> &ColumnDef, const std::vector<char> &Delimiters, const std::vector<wchar_t> &wDelimiters, char TextFilter, const double Factor, double ComW, double ComH, char BoardName[])//將CADXY檔案讀入資料陣列(CADX_CS)
{	
	CAOIProject *Project = GetActiveProjectPtr();
	if ( NULL == Project ) 
	{	return false; }
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

	/*
	if ( NULL == pfilename )
	{
		this->m_ErrorString.Format(_T("Please Open File First"));
		return false;
	}	
	//--------------------------------------------------------------------//
	const size_t NColumns    = ColumnDef.size();
	const size_t NDelimiters = Delimiters.size();	
	const size_t wNDelimiters= wDelimiters.size();
	if ( wNDelimiters != NDelimiters )
	{
		m_ErrorString = _T("Error, wNDelimiters != NDelimiters ");
		return false;
	}	
	//--------------------------------------------------------------------//	
	FILE *pfile = JetAPI::OpenReadFile(pfilename, bUnicode);	
	if ( pfile == NULL )
	{		
		this->m_ErrorString.Format(_T("Open File Fault (%s)"), pfilename);
		return false;
	}
	//--------------------------------------------------------------------//	

	const size_t textlinesize = 1024;	
	const size_t MaxValidData = 16;
	bool IsGetData[MaxValidData]={0};

	char TextFilterS[4]="";		
	char Textline[textlinesize] = "";
	char SubText[textlinesize] = "";
	char SubText2[textlinesize] = "";
	char TopS[textlinesize] = "";
	TextFilterS[0] = TextFilter;
	TextFilterS[1] = '\0';

	wchar_t wTextFilterS[4]=L"";
	wchar_t wTextline[textlinesize] = L"";
	wchar_t wSubText[textlinesize] = L"";
	wchar_t wSubText2[textlinesize] = L"";
	wchar_t wTopS[textlinesize] = L"";
	wTextFilterS[0] = TextFilter;
	wTextFilterS[1] = L'\0';

	CString Name;
	size_t textlen = 0;
	size_t i=0, j=0, k=0, s=0, t=0;
	size_t DataLine = 0;
	const char SpecDelimiter = ' ';//空白字元是區間內的特殊字元	
	const wchar_t wSpecDelimiter = L' ';//空白字元是區間內的特殊字元	
	size_t chidx=0;
	size_t CurrentNColumns = 0;
	size_t NRows = 0;//第幾列
	double tempF=0.0f;
	bool IsTop = true;
	bool CheckBoardSymbol = false;	
	BOX_TOWARD BoxToward;
	CAOIModel *ModelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const double Precision =  DBL_PRECISION;

	std::wstring std_string;
	std::set<std::wstring> BoardSet;
	std::set<std::wstring>::iterator BoardSetItr;

	//清除舊有的零件
	Project->SelectProjectAllComponents(false);
	PanelPtr->SelectPanelAllComponents(true);
	Project->DeleteProjectComponentSelected();

	//清除舊有的單板
	Project->SelectProjectAllBoards(false);
	PanelPtr->SelectPanelAllBoards(true);
	Project->DeleteProjectBoardSelected();
	
	//加入新的
	BoardPtr = AOIObjManager.CreateBoardObj();	
	Project->AddProjectBoardPtr(BoardPtr, false);
	PanelPtr->AddPanelBoardPtr(BoardPtr);
	PanelPtr->SetPanelSelected(true);

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
		if ( DataLine < StartLine ) { continue; }

		if ( false == bUnicode )
		{	textlen=::strlen(Textline); }
		else
		{	textlen=::wcslen(wTextline); }
		if ( textlen == 0 ) //enter鍵
		{	
			DataLine ++;
			NRows ++;
			continue;	
		}
		if ( false == bUnicode )
		{	::memset(SubText, 0x00, textlinesize*sizeof(char)); }
		else
		{	::memset(wSubText, 0x00, textlinesize*sizeof(wchar_t)); }
		CurrentNColumns = 0;
		k = 0;
		chidx = 0;
		//和分隔字元比較
		CurrentNColumns = 0;		
		chidx = 0;
		::memset(IsGetData, 0x00, sizeof(IsGetData));
		ComponentPtr = AOIObjManager.CreateComponentObj();
		if ( NULL == ComponentPtr )
		{
			::fclose(pfile); pfile = NULL;
			this->m_ErrorString.Format(_T("Error, Create New Component Fault"));
			return false;
		}
		
		Project->AddProjectComponentPtr(ComponentPtr, false);
		PanelPtr->AddPanelComponentPtr(ComponentPtr);
		BoardPtr->AddBoardComponentPtr(ComponentPtr);
		do
		{
			//------先確認一開始的字元是否為分隔字元-----------------//
			if ( CurrentNColumns == 0 )
			{
				for ( i=0; i<NDelimiters; i++ )
				{
					if ( false == bUnicode )
					{
						if ( Textline[chidx] == Delimiters[i] )
						{	break;	}
					}
					else
					{
						if ( wTextline[chidx] == wDelimiters[i] )
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
					if ( Textline[chidx] == Delimiters[i] )
					{	break;	}
				}
				else
				{
					if ( wTextline[chidx] == wDelimiters[i] )
					{	break;	}
				}
			}
			if ( i != NDelimiters )
			{				
				//如果有找到分隔字元
				//將資料放進去CADXY資料
				if ( false == bUnicode )
				{	SubText[k] = '\0'; }
				else
				{	wSubText[k] = L'\0'; }
				if ( CurrentNColumns >= NColumns )
				{
					::fclose(pfile); pfile = NULL;
					this->m_ErrorString.Format(_T("Columns too much (%d)"), NColumns);
					return false;
				}				
				switch ( ColumnDef[CurrentNColumns] )
				{
				case CADXY_LIST_HEADER_MODEL_NAME:
					ModelPtr = ComponentPtr->GetComponentModelPtr();
					if ( false == bUnicode )
					{
						JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);				
						JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
						ComponentPtr->SetComponentModelName(SubText);
						if ( NULL != ModelPtr )
						{	ModelPtr->SetModelName(SubText); }
					}
					else
					{
						JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);				
						JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
						ComponentPtr->SetComponentModelName(wSubText);
						if ( NULL != ModelPtr )
						{	ModelPtr->SetModelName(wSubText); }
					}
					IsGetData[CADXY_LIST_HEADER_MODEL_NAME] = true;
					break;
				case CADXY_LIST_HEADER_COMPONENT:
					if ( false == bUnicode )
					{
						JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
						JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
						ComponentPtr->SetComponentName(SubText);
					}
					else
					{
						JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
						JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
						ComponentPtr->SetComponentName(wSubText);
					}
					IsGetData[CADXY_LIST_HEADER_COMPONENT] = true;
					break;
				case CADXY_LIST_HEADER_POSITIONX:
					if ( false == bUnicode )
					{	tempF = ::atof(SubText)*Factor; }
					else
					{	tempF = ::_wtof(wSubText)*Factor; }					
					ComponentPtr->SetComponentCadPosX(tempF);
				//	ComponentPtr->SetCADPositionXOrg_C(tempF);

					ComponentPtr->SetComponentStagePosX(tempF);
					IsGetData[CADXY_LIST_HEADER_POSITIONX] = true;
					break;
				case CADXY_LIST_HEADER_POSITIONY:
					if ( false == bUnicode )
					{	tempF = ::atof(SubText)*Factor; }
					else
					{	tempF = ::_wtof(wSubText)*Factor; }
					ComponentPtr->SetComponentCadPosY(tempF);
				//	ComponentPtr->SetCADPositionYOrg_C(tempF);

					ComponentPtr->SetComponentStagePosY(tempF);
					IsGetData[CADXY_LIST_HEADER_POSITIONY] = true;
					break;
				case CADXY_LIST_HEADER_ANGLE:
					if ( false == bUnicode )
					{	tempF = (float)::atof(SubText); }
					else
					{	tempF = (float)::_wtof(wSubText); }
					ComponentPtr->SetComponentAngle(tempF);
					IsGetData[CADXY_LIST_HEADER_ANGLE] = true;
					break;
				case CADXY_LIST_HEADER_PART_NUMBER:
					if ( false == bUnicode )
					{
						JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
						JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
						ComponentPtr->SetComponentPartNumber(SubText);
					}
					else
					{
						JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
						JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
						ComponentPtr->SetComponentPartNumber(wSubText);
					}
					IsGetData[CADXY_LIST_HEADER_PART_NUMBER] = true;					
					break;
				case CADXY_LIST_HEADER_BOARD_ID:
					CheckBoardSymbol = true;
					if ( false == bUnicode )
					{
						JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
						JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
						::_strupr(SubText);
						ComponentPtr->SetComponentTempText(SubText);
					}
					else
					{
						JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
						JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
						::_wcsupr(wSubText);
						ComponentPtr->SetComponentTempText(wSubText);
					}
					IsGetData[CADXY_LIST_HEADER_BOARD_ID] = true;
					BoardSet.insert(ComponentPtr->GetComponentTempText());
					break;
				case CADXY_LIST_HEADER_NOZZLE:
					if ( false == bUnicode )
					{
						JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
						JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
						ComponentPtr->SetComponentNozzleName(SubText);
					}
					else
					{
						JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
						JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
						ComponentPtr->SetComponentNozzleName(wSubText);
					}
					IsGetData[CADXY_LIST_HEADER_NOZZLE] = true;
					break;
				default://NULL
					break;
				}
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
							if ( Textline[i] == Delimiters[j] )
							{	break;		}
						}
						else
						{
							if ( wTextline[i] == wDelimiters[j] )
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
				{	SubText[k] = Textline[chidx]; }
				else
				{	wSubText[k] = wTextline[chidx]; }
				chidx ++;
				k ++;
				if ( chidx >= textlen )
				{
					if ( false == bUnicode )
					{	SubText[k] = '\0'; }
					else
					{	wSubText[k] = L'\0'; }
					if ( CurrentNColumns >= NColumns )
					{
						::fclose(pfile); pfile = NULL;						
						this->m_ErrorString.Format(_T("Columns too much (%d)"), NColumns);
						return false;
					}				
					switch ( ColumnDef[CurrentNColumns] )
					{
					case CADXY_LIST_HEADER_MODEL_NAME:
						ModelPtr = ComponentPtr->GetComponentModelPtr();
						if ( false == bUnicode )
						{
							JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);					
							JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
							ComponentPtr->SetComponentModelName(SubText);
							if ( NULL != ModelPtr )
							{	ModelPtr->SetModelName(SubText); }
						}
						else
						{
							JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);					
							JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
							ComponentPtr->SetComponentModelName(wSubText);
							if ( NULL != ModelPtr )
							{	ModelPtr->SetModelName(wSubText); }
						}
						IsGetData[CADXY_LIST_HEADER_MODEL_NAME] = true;
						break;
					case CADXY_LIST_HEADER_COMPONENT:
						if ( false == bUnicode )
						{
							JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
							JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
							ComponentPtr->SetComponentName(SubText);
						}
						else
						{
							JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
							JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
							ComponentPtr->SetComponentName(wSubText);
						}
						IsGetData[CADXY_LIST_HEADER_COMPONENT] = true;
						break;
					case CADXY_LIST_HEADER_POSITIONX:
						if ( false == bUnicode )
						{	tempF = ::atof(SubText)*Factor; }
						else
						{	tempF = ::_wtof(wSubText)*Factor; }
						ComponentPtr->SetComponentCadPosX(tempF);
					//	ComponentPtr->SetCADPositionXOrg_C(tempF);

						ComponentPtr->SetComponentStagePosX(-tempF);
						IsGetData[CADXY_LIST_HEADER_POSITIONX] = true;
						break;
					case CADXY_LIST_HEADER_POSITIONY:
						if ( false == bUnicode )
						{	tempF = ::atof(SubText)*Factor; }
						else
						{	tempF = ::_wtof(wSubText)*Factor; }
						ComponentPtr->SetComponentCadPosY(tempF);
					//	ComponentPtr->SetCADPositionYOrg_C(tempF);

						ComponentPtr->SetComponentStagePosY(tempF);
						IsGetData[CADXY_LIST_HEADER_POSITIONY] = true;
						break;
					case CADXY_LIST_HEADER_ANGLE:
						if ( false == bUnicode )
						{	tempF = ::atof(SubText); }
						else
						{	tempF = ::_wtof(wSubText); }
						ComponentPtr->SetComponentAngle(tempF);
						IsGetData[CADXY_LIST_HEADER_ANGLE] = true;
						break;
					case CADXY_LIST_HEADER_PART_NUMBER:
						if ( false == bUnicode )
						{
							JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
							JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
							ComponentPtr->SetComponentPartNumber(SubText);
						}
						else
						{
							JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
							JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
							ComponentPtr->SetComponentPartNumber(wSubText);
						}
						IsGetData[CADXY_LIST_HEADER_PART_NUMBER] = true;
						break;
					case CADXY_LIST_HEADER_BOARD_ID:
						CheckBoardSymbol = true;
						if ( false == bUnicode )
						{
							JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
							JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
							::_strupr(SubText);
							ComponentPtr->SetComponentTempText(SubText);
						}
						else
						{
							JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
							JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
							::_strupr(SubText);
							ComponentPtr->SetComponentTempText(wSubText);
						}
						IsGetData[CADXY_LIST_HEADER_BOARD_ID] = true;
						BoardSet.insert(ComponentPtr->GetComponentTempText());
						break;
					case CADXY_LIST_HEADER_NOZZLE:
						if ( false == bUnicode )
						{
							JetAPI::FilerTextA(SubText, TextFilterS, SubText2, textlinesize);
							JetAPI::AdjustTextA(SubText2, '_', SubText, textlinesize);
							ComponentPtr->SetComponentNozzleName(SubText);
						}
						else
						{
							JetAPI::FilerTextW(wSubText, wTextFilterS, wSubText2, textlinesize);
							JetAPI::AdjustTextW(wSubText2, L'_', wSubText, textlinesize);
							ComponentPtr->SetComponentNozzleName(wSubText);
						}
						IsGetData[CADXY_LIST_HEADER_NOZZLE] = true;
						break;
					default://NULL
						break;
					}
					break;
				}
			}
		} while ( chidx < textlen );
		
		tempF = ComponentPtr->GetComponentAngle();
		if ( fabs(tempF)<Precision || fabs(tempF-360.0)<Precision )
		{	BoxToward = BOX_TOWARD_RIGHT;	}
		else if ( fabs(tempF-90) < Precision )
		{	BoxToward = BOX_TOWARD_UP;	}
		else if ( fabs(tempF-180) < Precision )
		{	BoxToward = BOX_TOWARD_LEFT;	}
		else if ( fabs(tempF-270) < Precision )
		{	BoxToward = BOX_TOWARD_DOWN;	}
		else 
		{	BoxToward = BOX_TOWARD_RIGHT; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();		
		ModelPtr->GetModelBodyBox().SetBoxToward(BoxToward);
		ComponentPtr->SetComponentCadBiasPosX(0);
		ComponentPtr->SetComponentCadBiasPosY(0);
		ComponentPtr->SetComponentRoiSizeW(ComW+1000);
		ComponentPtr->SetComponentRoiSizeH(ComH+1000);
		ComponentPtr->SetComponentBodySizeW(ComW);
		ComponentPtr->SetComponentBodySizeH(ComH);
		ComponentPtr->CalcComponentCadCornerPos();
		ComponentPtr->LayoutComponentStageCornerPos();
		//確認資料都有讀到
		if ( IsGetData[CADXY_LIST_HEADER_MODEL_NAME] == false )
		{	
			ComponentPtr->SetComponentModelName(ComponentPtr->GetComponentPartNumber());	
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{	ModelPtr->SetModelName(ComponentPtr->GetComponentPartNumber()); }
		}

		if ( IsGetData[CADXY_LIST_HEADER_COMPONENT] == false )
		{
			::fclose(pfile); pfile = NULL;			
			this->m_ErrorString.Format(_T("No Component Name Data (Line:%d)"), DataLine);
			return false;
		}

		if ( IsGetData[CADXY_LIST_HEADER_POSITIONX] == false )
		{
			::fclose(pfile); pfile = NULL;			
			this->m_ErrorString.Format(_T("No Position X Data (Line:%d)"), DataLine);
			return false;
		}

		if ( IsGetData[CADXY_LIST_HEADER_POSITIONY] == false )
		{
			::fclose(pfile); pfile = NULL;			
			this->m_ErrorString.Format(_T("No Position Y Data (Line:%d)"), DataLine);
			return false;
		}

		if ( IsGetData[CADXY_LIST_HEADER_ANGLE] == false )
		{
			::fclose(pfile); pfile = NULL;			
			this->m_ErrorString.Format(_T("No Angle Data (Line:%d)"), DataLine);
			return false;
		}

		if ( IsGetData[CADXY_LIST_HEADER_PART_NUMBER] == false )
		{
			::fclose(pfile); pfile = NULL;			
			this->m_ErrorString.Format(_T("No Part Number Data (Line:%d)"), DataLine);
			return false;
		}

		//ComponentPtr->SetComponentIndex_C(0);		
		//ComponentPtr->SetBoardNO_C(0);
		//this->m_NewBoard.AddNewComponent(tempComponent);
		//ComponentPtr->Initialize_C();
		//將新的資料放進去陣列中
		NRows ++;
	}
	//--------------------------------------------------------------------//
	::fclose(pfile);
	pfile = NULL;
	//--------------------------------------------------------------------//
	const size_t BoardSetSize = BoardSet.size();
	const size_t ComponentCount = PanelPtr->GetPanelComponentCount();
	if ( BoardSetSize > 1 )
	{
		//多個單板載入
		//1. 移除舊的單板下的零件
		BoardPtr->RemoveBoardAllComponents();

		//2. 補足剩餘單板數量
		for ( i=1; i<BoardSetSize; i++ )
		{
			BoardPtr = AOIObjManager.CreateBoardObj();	
			if ( NULL == BoardPtr ) { return false; }
			Project->AddProjectBoardPtr(BoardPtr, false);
			PanelPtr->AddPanelBoardPtr(BoardPtr);
		}

		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			std_string = ComponentPtr->GetComponentTempText();

			j = 0;
			for ( BoardSetItr=BoardSet.begin(); BoardSetItr!=BoardSet.end(); BoardSetItr++ )
			{
				if ( std_string == *BoardSetItr )
				{	break; }
				j ++;
			}
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, true);
			if ( NULL == BoardPtr )
			{	return false; }
			BoardPtr->AddBoardComponentPtr(ComponentPtr);
		}
	}		
	
	//將Cad座標移動至專案底圖內
	TPOINT2D  MapResolution;
	TREGION4D MapCadRgn, MapStageRgn;
	Project->GetProjectMapInfo(MapResolution, MapCadRgn, MapStageRgn);	
	const double PanelCpx = MapCadRgn.GetCpX();
	const double PanelCpy = MapCadRgn.GetCpY();
	//PanelPtr->SetPanelCadPos(PanelCpx, PanelCpy);

	//建立預設的座標轉換公式
	PanelPtr->BuildPanelDefaultMap(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);

	double PosX = 0;
	double PosY = 0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, OfflineMode);
	if ( PanelPtr->SetPanelStagePos(PosX, PosY, DistrictID) == false )
	{	return false;	}
//	this->m_NewBoard.SetBoardNO_B(1);
//	this->m_NewBoard.SetBoardIndex_B(0);	
*/
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
	// TODO: Add your message handler code here	
	if ( TRUE == bShow )
	{	PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, FALSE); }
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecNextPane()
{
	CAD_FILE_CONTENT_MODE CadFileContentMode;
	BOOL bChk = CWnd::IsDlgButtonChecked(CADXY_FILE_WITH_FD_CHK);	
	if ( TRUE == bChk ) { CadFileContentMode = CAD_FILE_CONTENT_FIDUCIAL; }
	else { CadFileContentMode = CAD_FILE_CONTENT_NORMAL; }
	AOIDataCollect.SetLoadCADFileContentMode(CadFileContentMode);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecPrevPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecFinishPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ReInitialPane()
{	
	m_Filename = _T("");
	if ( CWnd::GetSafeHwnd() == NULL )
	{	return true; }

	//建立報表標頭資料
	{
		CListCtrl &ListWnd = m_PreviewListWnd;
		JetAPI::ClearListCtrl(ListWnd, TRUE);	
		this->BuildPreviewListWndHeader();
	}

	//建立結果標頭資料
	{
		CListCtrl  &ListWnd = m_ResultListWnd;
		JetAPI::ClearListCtrl(ListWnd, TRUE);		
		this->BuildResultListWndHeader();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneLoadCadxy::GetSaveDefaultFilename()
{
	CString IniFile;
	IniFile.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("LoadCadXY.INI"));	
	return IniFile;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecSaveDefaultBtn()
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
	KeyName = _T("With Fiducial");
	if ( CWnd::IsDlgButtonChecked(CADXY_FILE_WITH_FD_CHK) == TRUE )	
	{	String.Format(_T("%d"), FN_ENABLE); }
	else
	{	String.Format(_T("%d"), FN_DISABLE); }
	if ( SaveINIData(Section, KeyName, String, IniFile, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	//----------------------------------------------------------------------//
	KeyName = _T("Board Name");	
	CWnd::GetDlgItemText(CADXY_BOARD_NAME_EDIT, String);
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
bool CNewProjectPaneLoadCadxy::ExecLoadDefaultBtn()
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
	KeyName = _T("With Fiducial");
	Default.Format(_T("%d"), FN_DISABLE);
	if ( LoadINIData(Section, KeyName, Default, String, MaxText, IniFile, false, ErrString) == false )
	{	JetAPI::ShowMessageBox(ErrString);	return false;	}
	nValue = ::_ttoi(String);
	CtrlID = CADXY_FILE_WITH_FD_CHK;
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
void CNewProjectPaneLoadCadxy::OnSaveDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	ExecSaveDefaultBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnLoadDefaultBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecLoadDefaultBtn();
	BuildPreviewListWndHeader();
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnDelimiterOther() 
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
bool CNewProjectPaneLoadCadxy::RemoveNGComponent()//移除不好的零件
{
	if ( NULL == m_ProjectPtr ) 
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	size_t         NGCount=0;
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

	NGCount=0;
	ProjectPtr->SelectProjectAllComponents(false);
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
				NGCount ++;				
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

				NGCount ++;				
				ComponentPtr1->SetComponentSelected(true);
				break;
			}
		}
	}
	if ( 0 == NGCount ) 
	{
		PanelPtr->SetPanelSelected(true);
		return true; 
	}
	ProjectPtr->DeleteProjectComponentSelected();
	PanelPtr->SetPanelSelected(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ReAssignComponentBoard()//重設零件單板
{
	if ( NULL == m_ProjectPtr ) 
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}

	size_t              i=0, j=0;
	CAOIBoard          *BoardPtr=NULL;
	CAOIComponent      *ComponentPtr=NULL;
	CComponentConfigWnd ComponentConfigWnd;
	const DISTRICT_ID   DistrictID = ProjectPtr->GetProjectActDistrictID();

	ComponentConfigWnd.SetLockPanelListWnd(true);
	ComponentConfigWnd.SetProjectPtr(ProjectPtr);
	ComponentConfigWnd.SetComponentConfigMode(COMPONENT_CONFIG_BOARD_ASSIGN);
	ComponentConfigWnd.SetComponentConfigImageMode(COMPONENT_CONFIG_IMAGE_TMP);
	if ( ComponentConfigWnd.DoModal() == IDCANCEL )
	{	return true; }

	bool bModified = ComponentConfigWnd.GetModified();
	if ( false == bModified ) 
	{	return true; }

	unsigned int TempBoardIndex=0;
	unsigned int ComponentBoardIndex=0;
	TBoardRect       *BoardRectPtr=NULL;
	TComponentConfig *ComponentConfigPtr=NULL;
	std::vector<TBoardRect> BoardRectList;
	std::vector<TComponentConfig> ComponentConfigList;
	ComponentConfigWnd.CloneBoardRectList(BoardRectList);
	ComponentConfigWnd.CloneComponentConfigList(ComponentConfigList);

	const size_t BoardRectCount = BoardRectList.size();
	const size_t ComponentConfigCount = ComponentConfigList.size();
	const size_t ComponentCount = PanelPtr->GetPanelComponentCount();

	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = &(ComponentConfigList[i]);
		if ( NULL == ComponentConfigPtr ) { continue; }
		ComponentPtr = ComponentConfigPtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempIndex(ComponentConfigPtr->uBoardIndexNew);
	}

	ProjectPtr->SelectProjectAllBoards(false);
	PanelPtr->SelectPanelAllBoards(true);
	PanelPtr->RemovePanelAllBoards();
	ProjectPtr->DestroyProjectBoardSelected();
		
	for ( i=0; i<BoardRectCount; i++ )
	{
		BoardRectPtr = &(BoardRectList[i]);
		if ( NULL == BoardRectPtr )	{ continue; }
		if ( BoardRectPtr->PanelPtr != PanelPtr ) { continue; }

		BoardPtr = AOIObjManager.CreateBoardObj();	
		if ( NULL == BoardPtr ) { return false; }
		ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
		PanelPtr->AddPanelBoardPtr(BoardPtr);
		BoardPtr->SetBoardTempInt(i);
	}

	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentBoardIndex = ComponentPtr->GetComponentTempIndex();				
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			TempBoardIndex = BoardPtr->GetBoardTempInt();
			if ( TempBoardIndex != ComponentBoardIndex ) { continue; }
			break;
		}
		if ( j == PanelBoardCount )
		{	return false; }				
		BoardPtr->AddBoardComponentPtr(ComponentPtr);				
	}
	PanelPtr->SetPanelSelected(true);	
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	PanelPtr->MovePanelStagePos(0, 0, DistrictID);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::CheckCompoentData(std::vector<CString> &NGList)//確定零件資料是否正確
{
	if ( NULL == m_ProjectPtr ) 
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
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
				break;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::SaveNGComponentList(const std::vector<CString> &NGList)
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
bool CNewProjectPaneLoadCadxy::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::SaveLoadCADXYResult()
{	
	if ( NULL == m_ProjectPtr ) 
	{	return false; }

	size_t         i=0, j=0;
	size_t         ComponentCount=0;
	CAOIBoard     *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	CAOIProject   *ProjectPtr = GetActiveProjectPtr();
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
void CNewProjectPaneLoadCadxy::OnDelimiterTab() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnDelimiterComma() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnDelimiterSpace() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnDelimiterSemicolon() 
{
	// TODO: Add your control notification handler code here
	OnPreviewBtn();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnNewComponentBtn() 
{
	// TODO: Add your control notification handler code here	
	CString      str;
	CString      strLabel;
	CString      strValue;
	CString      strCaption;
	CString      strPartNumber;
	CString      strComponentName;	
	CInputBoxWnd InputBox;
	str = _T("Set New Componetn Name");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Component Name:");
	strLabel = LoadMultiLanguageString(str, str);
	strValue = _T("New");	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	while ( true ) 
	{
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return;  }
		strComponentName = InputBox.m_DataEdit1;
		strComponentName.MakeUpper();
		strComponentName.TrimLeft();
		strComponentName.TrimRight();
		if ( strComponentName.GetLength() == 0 ) 
		{	continue; }
		break;
	};

	str = _T("Set Part Number");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Part Number:");
	strLabel = LoadMultiLanguageString(str, str);
	strValue = strComponentName;	
	InputBox.SetParam1(strCaption, strLabel, strValue);
	while ( true ) 
	{
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return;  }
		strPartNumber = InputBox.m_DataEdit1;
		strPartNumber.MakeUpper();
		strPartNumber.TrimLeft();
		strPartNumber.TrimRight();
		if ( strPartNumber.GetLength() == 0 ) 
		{	continue; }
		break;
	};

	strPartNumber.MakeUpper();
	strComponentName.MakeUpper();	
	if ( ExecNewComponent(strComponentName, strPartNumber) == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}
	if ( this->BuildResultListWnd() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return ;
	}
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecNewComponent(LPCTSTR Name, LPCTSTR PartNumber)//創建新的零件
{
	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return false;
	}
	
	const size_t MaxValidData = 16;
	bool IsGetData[MaxValidData]={0};
	const size_t BufferSize = 256;
	char  CompnentName[BufferSize]="";
	char  PartNumberName[BufferSize]="";	
	const double CadPosX=0;
	const double CadPosY=0;
	const double Angle  = 0;
	const DISTRICT_ID DistrictID = Project->GetProjectActDistrictID();
	const double ComW = (int)(this->GetDlgItemInt(CADXY_COMPONENT_WIDTH_EDIT));
	const double ComH = (int)(this->GetDlgItemInt(CADXY_COMPONENT_HEIGHT_EDIT));
	CAOIBoard *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;

	std::string std_string;
	std::set<std::string> BoardSet;
	std::set<std::string>::iterator BoardSetItr;	

	//清除舊有的零件
	Project->SelectProjectAllComponents(false);
	PanelPtr->SelectPanelAllComponents(true);
	Project->DeleteProjectComponentSelected();

	//清除舊有的單板
	Project->SelectProjectAllBoards(false);
	PanelPtr->SelectPanelAllBoards(true);
	Project->DeleteProjectBoardSelected();
	
	//加入新的
	BoardPtr = AOIObjManager.CreateBoardObj();	
	Project->AddProjectBoardPtr(BoardPtr, false);
	PanelPtr->AddPanelBoardPtr(BoardPtr);
	PanelPtr->SetPanelSelected(true);
	
	ComponentPtr = AOIObjManager.CreateComponentObj();
	if ( NULL == ComponentPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Create New Component Fault"));
		return false;
	}
	
	JetAPI::TCHAR2char(Name, CompnentName, BufferSize);	
	JetAPI::TCHAR2char(PartNumber, PartNumberName, BufferSize);

	Project->AddProjectComponentPtr(ComponentPtr, false);
	PanelPtr->AddPanelComponentPtr(ComponentPtr);
	BoardPtr->AddBoardComponentPtr(ComponentPtr);
	
	ComponentPtr->SetComponentModelName(PartNumberName);
	ComponentPtr->SetComponentName(CompnentName);
	ComponentPtr->SetComponentCadPosX(CadPosX);
	ComponentPtr->SetComponentStagePosX(CadPosX);
	ComponentPtr->SetComponentCadPosY(CadPosY);
	ComponentPtr->SetComponentStagePosY(CadPosY);
	ComponentPtr->SetComponentAngle(Angle);
	ComponentPtr->SetComponentPartNumber(PartNumberName);					
	ComponentPtr->SetComponentNozzleName(L"");
	ComponentPtr->SetComponentCadBiasPosX(0);
	ComponentPtr->SetComponentCadBiasPosY(0);
	ComponentPtr->SetComponentRoiSizeW(ComW+1000);
	ComponentPtr->SetComponentRoiSizeH(ComH+1000);
	ComponentPtr->SetComponentBodySizeW(ComW);
	ComponentPtr->SetComponentBodySizeH(ComH);
	ComponentPtr->CalcComponentCadCornerPos();
	ComponentPtr->LayoutComponentStageCornerPos();
	//--------------------------------------------------------------------//
	const size_t BoardSetSize = BoardSet.size();
	const size_t ComponentCount = PanelPtr->GetPanelComponentCount();
	//--------------------------------------------------------------------//	
	//將Cad座標移動至專案底圖內
	TPOINT2D  MapResolution;
	TREGION4D MapCadRgn, MapStageRgn;	
	Project->GetProjectMapInfo(MapResolution, MapCadRgn, MapStageRgn);	
	const double PanelCpx = MapCadRgn.GetCpX();
	const double PanelCpy = MapCadRgn.GetCpY();
	//PanelPtr->SetPanelCadPos(PanelCpx, PanelCpy, DistrictID);

	//建立預設的座標轉換公式	
	PanelPtr->BuildPanelDefaultMap(DistrictID);
	PanelPtr->LayoutPanelBoardListRegion(DistrictID);

	double PosX = 0;
	double PosY = 0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, OfflineMode);	
	if ( PanelPtr->SetPanelStagePos(PosX, PosY, DistrictID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnMatchAliasBtn() 
{
	// TODO: Add your control notification handler code here
	ExecMatchAliasName();
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecMatchAliasName()//合併料號別名
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
void CNewProjectPaneLoadCadxy::OnLoadPidBtn() 
{
	// TODO: Add your control notification handler code here
	ExecLoadSpiPidFile();
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::ExecLoadSpiPidFile()//載入SPI的Pid檔案
{
	CAOIProject *Project = m_ProjectPtr;
	CAOIPanel   *PanelPtr = Project->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr )
	{
		m_ErrorString.Format(_T("Error, No Active Panel"));
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return false;
	}

	TCHAR szFilters[]=_T("PID Files (*.pid)|*.pid|All Files (*.*)|*.*||");
	//CFileDialog dialog (TRUE, _T("pid;asc;prn;csv"), _T("*.asc"), OFN_FILEMUSTEXIST, szFilters);
	CFileDialog dialog (TRUE, _T("pid"), _T("*.pid"), OFN_FILEMUSTEXIST, szFilters);	
	if ( dialog.DoModal() == IDCANCEL )
	{	return  true; }

	const double AddAngle=0.0;
	const bool bReverseAngle=false;
	const CString Filename = dialog.GetPathName();		
	if ( Project->LoadProjectSpiPidFile(Filename, PanelPtr, AddAngle, bReverseAngle) == false ) 
	{
		m_ErrorString = Project->GetErrorString();
		JetAPI::ShowMessageBox(this->m_ErrorString);
		return false;
	}	

	double PosX = 0;
	double PosY = 0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	DISTRICT_ID DistrictID = Project->GetProjectActDistrictID();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, OfflineMode);
	if ( PanelPtr->SetPanelStagePos(PosX, PosY, DistrictID) == false )
	{
		m_ErrorString = Project->GetErrorString();
		JetAPI::ShowMessageBox(this->m_ErrorString);
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
bool CNewProjectPaneLoadCadxy::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneLoadCadxy::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneLoadCadxy::OnOpenCSVBtn() 
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
void CNewProjectPaneLoadCadxy::OnOpenASCBtn() 
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