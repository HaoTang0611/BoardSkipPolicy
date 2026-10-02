// FilenameSyntaxWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "FilenameSyntaxWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL_MODE     = 1;//設定的欄位
const int SETTING_COL_FOLDER   = 3;//設定的欄位
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFilenameSyntaxWnd dialog
//-------------------------------------------------------------------------------------//
CFilenameSyntaxWnd::CFilenameSyntaxWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFilenameSyntaxWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilenameSyntaxWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ColIdxAct = 0;
	m_ParamActPtr = NULL;		
	m_StopParamListBeSelected = false;

	CString Filename, Section;
	CString Folder=AOIDataCollect.GetAOITempDirectory();
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;	
	Section = _T("Filename Syntax Test");
	Filename.Format(_T("%s\\%s"), Folder, _T("FilenameSyntaxTest.INI"));	
	FilenameSyntax.SetFilenameSyntaxTag(_T("Test"));
	FilenameSyntax.SetFilenameSyntaxIni(Filename, Section);
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilenameSyntaxWnd)	
	DDX_Control(pDX, FNS_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, FNS_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, FNS_PARAM_LIST_WND, m_ParamListCtrl);	
	DDX_Control(pDX, FNS_PANEL_INDEX_WIDTH_COMBO, m_PanelIndexWidthCombox);	
	DDX_Control(pDX, FNS_BOARD_INDEX_WIDTH_COMBO, m_BoardIndexWidthCombox);	
	DDX_Control(pDX, FNS_OBJECT_INDEX_WIDTH_COMBO, m_ObjectIndexWidthCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFilenameSyntaxWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CFilenameSyntaxWnd)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()	
	ON_NOTIFY(LVN_ITEMCHANGED, FNS_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, FNS_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(FNS_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(FNS_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(FNS_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(FNS_ADD_NODE_BTN, OnAddNodeBtn)
	ON_BN_CLICKED(FNS_INSERT_NODE_BTN, OnInsertNodeBtn)
	ON_BN_CLICKED(FNS_DEL_NODE_BTN, OnDelNodeBtn)
	ON_BN_CLICKED(FNS_CLEAR_BTN, OnClearBtn)	
	ON_BN_CLICKED(FNS_RESIZE_BTN, OnResizeBtn)
	ON_BN_CLICKED(FNS_DEL_UNUSED_BTN, OnDelUnusedBtn)
	ON_BN_CLICKED(FNS_SAVE_FILE_BTN, OnSaveFileBtn)
	ON_BN_CLICKED(FNS_LOAD_FILE_BTN, OnLoadFileBtn)		
	ON_BN_CLICKED(FNS_UPDATE_PARAM_BTN, OnUpdateParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFilenameSyntaxWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CFilenameSyntaxWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	BuildIndexWidthCombox(m_PanelIndexWidthCombox);
	BuildIndexWidthCombox(m_BoardIndexWidthCombox);
	BuildIndexWidthCombox(m_ObjectIndexWidthCombox);

	UpdateKernalToUI();
	BuildParamListWndHeader();	
	BuildParamListWnd();
	UpdateFilenameText();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnOK() 
{
	// TODO: Add extra validation here
	const bool bVerify=true;
	if ( UpdateFilenameText(bVerify) == false )
	{	return; }
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
BOOL CFilenameSyntaxWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			if ( m_EditCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByEdit();
				m_EditCtrl.ShowWindow(SW_HIDE);	
				m_EditCtrl.SetWindowText(_T(""));
				return TRUE;				
			}
			if ( m_ComboxCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByCombox();
				m_ComboxCtrl.ShowWindow(SW_HIDE);
				JetAPI::ClearCombox(m_ComboxCtrl);
				return TRUE;				
			}
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();		
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CFilenameSyntaxWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
const CFilenameSyntax& CFilenameSyntaxWnd::GetFilenameSyntax() const
{
	return m_FilenameSyntax;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::SetFilenameSyntax(const CFilenameSyntax &Syntax)
{
	m_FilenameSyntax = Syntax;
}
//-------------------------------------------------------------------------------------//
int CFilenameSyntaxWnd::GetColIdxAct()
{
	return m_ColIdxAct;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::SetColIdxAct(int Idx)
{
	m_ColIdxAct = Idx;
}
//-------------------------------------------------------------------------------------//
CParamUni* CFilenameSyntaxWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
	if ( NULL == Ptr ) 
	{ SetColIdxAct(0); }
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::BuildParamList()
{
	CThisListCtrl_62 &ListCtrl = GetParamListWndRef();
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	
	int       intValue=0;
	bool      bFolderMode;
	size_t    i=0, j=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;	
	CParamUni    ParamUnit;	
	UINT      ParamID;
	const int nSubItem = 1;		
	CFilenameSyntaxNode*  NodePtr=NULL;
	CParamList &ParamList = m_ParamList;	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  	
	const size_t NodeCount=FilenameSyntax.GetSyntaxNodeCount();

	FILE_NAME_SYNTAX_MODE SyntaxMode;	
	std::vector<FILE_NAME_SYNTAX_MODE> SyntaxEnumList;
	FilenameSyntax.GetSyntaxEnumList(SyntaxEnumList);
	const size_t EnumCount=(size_t)(SyntaxEnumList.size());
	ParamList.clear();
	for ( i=0; i<NodeCount; i++ )
	{
		NodePtr = FilenameSyntax.GetSyntaxNodePtr(i, false);	
		if ( NULL == NodePtr ) { continue; }

		SyntaxMode = NodePtr->GetSyntaxMode();
		bFolderMode = NodePtr->GetFolderMode();

		ParamUnit = CParamUni();
		str.Format(_T("%02d"), i+1);		
		str = GetSyntaxText(SyntaxMode);
		FilenameSyntax.GetSyntaxContent(SyntaxMode, strDescription);		

		ParamID = i;	
		ParamUnit.SetCaption(str);
		ParamUnit.SetParamID((UINT)(ParamID));	
		if ( true == bFolderMode )
		{	ParamUnit.SetTempText(_T("Yes"));	}
	
		for ( j=0; j<EnumCount; j++ )
		{
			SyntaxMode=SyntaxEnumList[j];
			ParamUnit.AddSelItem(SyntaxMode, GetSyntaxText(SyntaxMode));			
		}
		ParamUnit.SetValue_SEL(NodePtr->GetSyntaxMode());		
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CThisListCtrl_62& CFilenameSyntaxWnd::GetParamListWndRef()
{
	return m_ParamListCtrl;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_62 &ListCtrl = GetParamListWndRef();	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;	

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	CString       strFolderMode;
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	const int     nSubItem3 = 3;
	
	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem1);		

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetDesction();
		strFolderMode = ParamPtr->GetTempText();

		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		ListCtrl.SetItemText(nItem, nSubItem3, strFolderMode);		
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_62 &ListCtrl = GetParamListWndRef();

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/5;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Mode");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Text");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;	

	width2 = 48;
	str = _T("Folder");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::BuildIndexWidthCombox(CComboBox &Combox)
{
	size_t       i=0,j=0;
	int          idx=0;
	CString      str;
	CString      str2;
	DWORD        Param=0;		
	const size_t Cnt=9;

	idx = 0;
	JetAPI::ClearCombox(Combox);		
	for ( i=0; i<Cnt; i++ )
	{
		Param = i;
		str2=_T("");
		for ( j=0; j<Param; j++ )
		{	str2 += _T("#");	}
		if ( 0 == Param )
		{	str.Format(_T("%d"), Param); }
		else
		{	str.Format(_T("%d-%s"), Param, str2); }		
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::UpdateKernalToUI()
{
	CString str;
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  

	CWnd::CheckDlgButton(FNS_ENABLE_CHK, FilenameSyntax.GetFilenameSyntaxEnabled());

	str=CString(FilenameSyntax.GetExtensionName());
	CWnd::SetDlgItemText(FNS_EXT_NAME_EDIT, str);

	str=CString(FilenameSyntax.GetUserdefineText1());
	CWnd::SetDlgItemText(FNS_USER_DEF_EDIT1, str);
	str=CString(FilenameSyntax.GetUserdefineText2());
	CWnd::SetDlgItemText(FNS_USER_DEF_EDIT2, str);
	str=CString(FilenameSyntax.GetUserdefineText3());
	CWnd::SetDlgItemText(FNS_USER_DEF_EDIT3, str);

	JetAPI::SetComboxCurSel(m_PanelIndexWidthCombox, FilenameSyntax.GetPanelIndexWidth());
	JetAPI::SetComboxCurSel(m_BoardIndexWidthCombox, FilenameSyntax.GetBoardIndexWidth());
	JetAPI::SetComboxCurSel(m_ObjectIndexWidthCombox, FilenameSyntax.GetObjectIndexWidth());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::UpdateUIToKernal()
{
	CString str;
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  
	
	if ( CWnd::IsDlgButtonChecked(FNS_ENABLE_CHK) == TRUE )
	{	FilenameSyntax.SetFilenameSyntaxEnabled(true);	}
	else
	{	FilenameSyntax.SetFilenameSyntaxEnabled(false);	}	

	CWnd::GetDlgItemText(FNS_EXT_NAME_EDIT, str);
	FilenameSyntax.SetExtensionName(str);
	
	CWnd::GetDlgItemText(FNS_USER_DEF_EDIT1, str);
	FilenameSyntax.SetUserdefineText1(str);	
	CWnd::GetDlgItemText(FNS_USER_DEF_EDIT2, str);
	FilenameSyntax.SetUserdefineText2(str);	
	CWnd::GetDlgItemText(FNS_USER_DEF_EDIT3, str);
	FilenameSyntax.SetUserdefineText3(str);
	
	FilenameSyntax.SetPanelIndexWidth(JetAPI::GetComboxCurSelData(m_PanelIndexWidthCombox));
	FilenameSyntax.SetBoardIndexWidth(JetAPI::GetComboxCurSelData(m_BoardIndexWidthCombox));
	FilenameSyntax.SetObjectIndexWidth(JetAPI::GetComboxCurSelData(m_ObjectIndexWidthCombox));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::UpdateFilenameText(bool bVerify)
{
	bool IsOK=true;
	CString Filename;	
	const bool bCreateFile=bVerify;
	CString Folder=AOIDataCollect.GetAOITempDirectory();
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  
	if ( false == bVerify )
	{	IsOK=FilenameSyntax.BuildFilename(NULL, bCreateFile, Filename); }
	else
	{	IsOK=FilenameSyntax.BuildFilename(Folder, bCreateFile, Filename);	}
	if ( false == bVerify )
	{	CWnd::SetDlgItemText(FNS_FILENAME_EDIT, Filename); }
	else
	{
		if ( false==IsOK )
		{	JetAPI::ShowMessageBox(FilenameSyntax.GetErrorString());	}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CString CFilenameSyntaxWnd::GetSyntaxText(FILE_NAME_SYNTAX_MODE Mode)
{
	CString str;
	bool bException=false;
	switch ( Mode )
	{
	case FILE_NAME_SYNTAX_NULL: str=_T("NULL");	break;

	case FILE_NAME_SYNTAX_MACHINE_NAME: str=_T("Machine Name");	break;
	case FILE_NAME_SYNTAX_MACHINE_LINE: str=_T("Machine Line");	break;
	case FILE_NAME_SYNTAX_MACHINE_STATION: str=_T("Machine Station");	break;
	//case FILE_NAME_SYNTAX_MACHINE_HOST_IP: str=_T("Machine Host IP");	break;

	case FILE_NAME_SYNTAX_PROJECT_NAME: str=_T("Project Name");	break;
	case FILE_NAME_SYNTAX_PROJECT_MODULE: str=_T("Project Module");	break;
	case FILE_NAME_SYNTAX_PROJECT_LOT: str=_T("Project Lot");	break;
	case FILE_NAME_SYNTAX_PROJECT_LANE: str=_T("Project Lane");	break;

	case FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDD: str=_T("Test Time(YYYYMMDD)");	break;
	case FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDDHHMMSS: str=_T("Test Time(YYYYMMDDhhmmss)");	break;		

	//case FILE_NAME_SYNTAX_BARCODE_SCOPE: str=_T("Scope Barcode");	break;
	case FILE_NAME_SYNTAX_BARCODE_PROJECT: str=_T("Project Barcode");	break;
	case FILE_NAME_SYNTAX_BARCODE_PANEL: str=_T("Panel Barcode");	break;
	case FILE_NAME_SYNTAX_BARCODE_BOARD: str=_T("Board Barcode");	break;

	case FILE_NAME_SYNTAX_INDEX_OBJECT: str=_T("Object Index");	break;
	case FILE_NAME_SYNTAX_INDEX_PANEL: str=_T("Panel Index");	break;
	case FILE_NAME_SYNTAX_INDEX_BOARD: str=_T("Board Index");	break;

	case FILE_NAME_SYNTAX_PUNC_HYPHEN: str=_T("-");	break;
	case FILE_NAME_SYNTAX_PUNC_UNDER_LINE: str=_T("_");	break;
	case FILE_NAME_SYNTAX_PUNC_AT_SIGN: str=_T("@");	break;
	case FILE_NAME_SYNTAX_PUNC_NUMBER_SIGN: str=_T("#");	break;

	case FILE_NAME_SYNTAX_USER_DEFINE_1: str=_T("User Def.1");	break;
	case FILE_NAME_SYNTAX_USER_DEFINE_2: str=_T("User Def.2");	break;
	case FILE_NAME_SYNTAX_USER_DEFINE_3: str=_T("User Def.3");	break;

	default:
		bException = true;
		str.Format(_T("Undefine [%02d]"), Mode);
		break;
	}

	if ( false == bException )
	{	str = LoadMultiLanguageString(str, str);	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::CreateSyntaxode(CFilenameSyntaxNode &SyntaxNode, bool bAckFolder)
{	
	int             i=0;
	TListNode       Node;	
	CInputListWnd   EnumWnd;		
	DWORD_PTR       OldIndex=0;	
	std::vector<TListNode> NodelList;
	CString         strCaption, strLabel;
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  

	FILE_NAME_SYNTAX_MODE SyntaxMode;
	std::vector<FILE_NAME_SYNTAX_MODE> SyntaxEnumList;
	FilenameSyntax.GetSyntaxEnumList(SyntaxEnumList);
	const int EnumCount=(int)(SyntaxEnumList.size());
	for ( i=0; i<EnumCount; i++ )
	{
		SyntaxMode=SyntaxEnumList[i];
		Node.Data=SyntaxMode;
		Node.Text=GetSyntaxText(SyntaxMode);
		NodelList.push_back(Node);
	}
	
	strCaption = _T("New Syntax Node");
	strLabel = _T("Syntax Node");
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return false; }
	
	DWORD Res=IDNO;
	if ( true == bAckFolder )
	{
		strLabel = _T("Folder Node");
		Res=JetAPI::ShowMessageBox(strLabel, MSG_MB_YESNOCANCEL);
		if ( IDCANCEL == Res )  
		{	return false; }	
	}
	
	UpdateUIToKernal();
	FILE_NAME_SYNTAX_MODE NewISyntaxMode = (FILE_NAME_SYNTAX_MODE)(EnumWnd.GetSelData());	
	SyntaxNode.SetSyntaxMode(NewISyntaxMode);
	if ( IDYES == Res )
	{	SyntaxNode.SetFolderMode(true); }	
	return true;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_FILENAME_SYNTAX_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(FNS_ENABLE_CHK));
	SetMultiLanauage(LoadIDAndName(FNS_EXT_NAME_LABEL));
	SetMultiLanauage(LoadIDAndName(FNS_USER_DEF_LABEL1));
	SetMultiLanauage(LoadIDAndName(FNS_USER_DEF_LABEL2));
	SetMultiLanauage(LoadIDAndName(FNS_USER_DEF_LABEL3));

	SetMultiLanauage(LoadIDAndName(FNS_PANEL_INDEX_WIDTH_LABEL));
	SetMultiLanauage(LoadIDAndName(FNS_BOARD_INDEX_WIDTH_LABEL));
	SetMultiLanauage(LoadIDAndName(FNS_OBJECT_INDEX_WIDTH_LABEL));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(FNS_ADD_NODE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_INSERT_NODE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_DEL_NODE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_CLEAR_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_RESIZE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_DEL_UNUSED_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_SAVE_FILE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_LOAD_FILE_BTN));
	SetMultiLanauage(LoadIDAndName(FNS_UPDATE_PARAM_BTN));	
	//---------------------------------------------------------------------------------//
	CString WndText, Tag, NewWndText;	
	Tag=m_FilenameSyntax.GetFilenameSyntaxTag();
	if ( Tag.GetLength() > 0 )
	{
		CWnd::GetWindowText(WndText);	
		NewWndText.Format(_T("%s[%s]"), WndText, Tag);
		CWnd::SetWindowText(NewWndText);
	}
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_FILENAME_SYNTAX_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CFilenameSyntaxWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_FILENAME_SYNTAX_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::HideCtrlBtn()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecReleaseParamCtrl()
{
	HideCtrlBtn();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::SetDescriptionText(const CParamUni *Ptr)
{
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecItemchangedParamListWnd(CThisListCtrl_62 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();	
	const int nSubItem = ParamPtr->GetSubItemIndex();
	
	SetDescriptionText(ParamPtr);
	if ( NULL!=BtnWndPtr && BtnWndPtr->GetSafeHwnd()!=NULL) 	
	{
		CRect ItemRect;
		if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == TRUE )
		{	
			SIZE BtnSize={0};
			RECT BtnRect={0};
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;			
			BtnRect.left = BtnRect.right-BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);
			BtnWndPtr->ShowWindow(SW_SHOW);			
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	HideCtrlBtn();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecDblclkParamListWnd(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( SETTING_COL_MODE == nSubItem )
	{	return ExecDblclkParamListWnd_Mode(ListCtrl, nItem, nSubItem);	}
	if ( SETTING_COL_FOLDER == nSubItem )
	{	return ExecDblclkParamListWnd_Folder(ListCtrl, nItem, nSubItem);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecDblclkParamListWnd_Mode(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem != SETTING_COL_MODE ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	HideCtrlBtn();
	SetColIdxAct(nSubItem);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				m_ComboxCtrl.InsertString(nSelIdx, ItemText);
				m_ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(m_ComboxCtrl, ParamPtr->GetSelParam());
			m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			m_ComboxCtrl.SetFocus();
			m_ComboxCtrl.ShowDropDown();
			m_ComboxCtrl.ShowWindow(SW_SHOW);
			m_ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			m_ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		if ( m_EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			m_EditCtrl.SetWindowText(ItemText);
			m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
			m_EditCtrl.SetFocus();
			m_EditCtrl.SetSel(0,-1);
			m_EditCtrl.ShowWindow(SW_SHOW);	
			m_EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			m_EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecDblclkParamListWnd_Folder(CThisListCtrl_62 &ListCtrl, int nItem, int nSubItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem != SETTING_COL_FOLDER ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  		
	const size_t ParamID = (size_t)(ParamPtr->GetParamID());		
	CFilenameSyntaxNode *NodePtr=FilenameSyntax.GetSyntaxNodePtr(ParamID, true);
	if ( NULL == NodePtr ) { return true; }
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = PARAM_DATA_SEL;

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	HideCtrlBtn();
	SetColIdxAct(nSubItem);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);			
			m_ComboxCtrl.InsertString(nSelIdx, _T("No"));
			m_ComboxCtrl.SetItemData(nSelIdx, FN_DISABLE);
			nSelIdx ++;

			m_ComboxCtrl.InsertString(nSelIdx, _T("Yes"));
			m_ComboxCtrl.SetItemData(nSelIdx, FN_ENABLE);
			nSelIdx ++;
						
			JetAPI::SetComboxCurSel(m_ComboxCtrl, NodePtr->GetFolderMode());
			m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			m_ComboxCtrl.SetFocus();
			m_ComboxCtrl.ShowDropDown();
			m_ComboxCtrl.ShowWindow(SW_SHOW);
			m_ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			m_ComboxCtrl.Invalidate();
		}	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecUpdateParamByEdit()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);	

	CString ItemText, DesctionText;	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  		
	const size_t ParamID = (size_t)(ParamPtr->GetParamID());		
	CFilenameSyntaxNode *NodePtr=FilenameSyntax.GetSyntaxNodePtr(ParamID, true);
	if ( NULL == NodePtr ) { return true; }
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}

	//FILE_NAME_SYNTAX_MODE SyntaxMode=(FILE_NAME_SYNTAX_MODE)(Param);
	//FilenameSyntax.GetSyntaxContent(SyntaxMode, DesctionText);	
	//NodePtr->SetMode(SyntaxMode);
	//ParamPtr->SetDesction(DesctionText);

	CThisListCtrl_62 *pListCtrl = (CThisListCtrl_62*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
	}
	UpdateFilenameText();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecUpdateParamByCombox()
{
	const int ColIdxAct=GetColIdxAct();
	if ( SETTING_COL_MODE == ColIdxAct )
	{	return ExecUpdateParamByCombox_Mode();	}
	if ( SETTING_COL_FOLDER == ColIdxAct )
	{	return ExecUpdateParamByCombox_Folder();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecUpdateParamByCombox_Mode()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText, DesctionText;	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  		
	const size_t ParamID = (size_t)(ParamPtr->GetParamID());		
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	CFilenameSyntaxNode *NodePtr=FilenameSyntax.GetSyntaxNodePtr(ParamID, true);
	if ( NULL == NodePtr ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }	

	FILE_NAME_SYNTAX_MODE SyntaxMode=(FILE_NAME_SYNTAX_MODE)(Param);
	FilenameSyntax.GetSyntaxContent(SyntaxMode, DesctionText);	
	NodePtr->SetSyntaxMode(SyntaxMode);
	ParamPtr->SetDesction(DesctionText);

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_62 *pListCtrl = (CThisListCtrl_62*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);

		ItemText = ParamPtr->GetDesction();
		pListCtrl->SetItemText(nItem, nSubItem+1, ItemText);
		pListCtrl->SetFocus();
	}
	UpdateFilenameText();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntaxWnd::ExecUpdateParamByCombox_Folder()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  		
	const size_t ParamID = (size_t)(ParamPtr->GetParamID());		
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	CFilenameSyntaxNode *NodePtr=FilenameSyntax.GetSyntaxNodePtr(ParamID, true);
	if ( NULL == NodePtr ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( FN_DISABLE == Param )
	{	NodePtr->SetFolderMode(false);	}
	if ( FN_ENABLE == Param )
	{	NodePtr->SetFolderMode(true);	}
	
	CThisListCtrl_62 *pListCtrl = (CThisListCtrl_62*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = SETTING_COL_FOLDER;	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		if ( FN_DISABLE == Param ) { ItemText=_T(""); }
		if ( FN_ENABLE == Param ) { ItemText=_T("Yes"); }
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);		
		pListCtrl->SetFocus();
	}
	UpdateFilenameText();
	return true;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = 0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;	
	if ( 0!=ResOld && 0==ResNew )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnAddNodeBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bAckFolder = false;
	CFilenameSyntaxNode SyntaxNode;		
	if ( CreateSyntaxode(SyntaxNode, bAckFolder) == false )
	{	return ; }

	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  	
	FilenameSyntax.AddSyntaxNode(SyntaxNode);	
	BuildParamListWnd();
	UpdateFilenameText();
	return ;
	
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnInsertNodeBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bAckFolder = false;
	CFilenameSyntaxNode SyntaxNode;	
	if ( CreateSyntaxode(SyntaxNode, bAckFolder) == false )
	{	return ; }

	CThisListCtrl_62 &ListCtrl = GetParamListWndRef();	
	const int nItem = ListCtrl.GetNextItem(-1,  LVNI_FOCUSED|LVNI_SELECTED);	

	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  	
	FilenameSyntax.InsertSyntaxNode(size_t(nItem), SyntaxNode);	
	BuildParamListWnd();
	UpdateFilenameText();
	return ;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnDelNodeBtn() 
{
	// TODO: Add your control notification handler code here	
	CThisListCtrl_62 &ListCtrl = GetParamListWndRef();	
	const int nItem = ListCtrl.GetNextItem(-1,  LVNI_FOCUSED|LVNI_SELECTED);
	if ( nItem < 0 ) { return ; }

	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;  	
	if ( FilenameSyntax.DelSyntaxNode(nItem) == false )
	{	return; }

	BuildParamListWnd();
	UpdateFilenameText();
	return ;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;
	FilenameSyntax.ClearSyntaxList();
	BuildParamListWnd();
	UpdateFilenameText();
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnResizeBtn() 
{
	// TODO: Add your control notification handler code here
	CInputBoxWnd InputBox;
	CString strCaption, strLabel, strValue;
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;
	size_t SyntaxCount=FilenameSyntax.GetSyntaxNodeCount();

	strCaption = _T("Set Syntax Count");
	strLabel = AOIDataDefine.GetCountText();
	strValue.Format(_T("%d"), SyntaxCount);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return;	}
	SyntaxCount = ::_ttoi(InputBox.m_DataEdit1);
	FilenameSyntax.ResizeSyntaxList(SyntaxCount);
	BuildParamListWnd();
	UpdateFilenameText();
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnDelUnusedBtn() 
{
	// TODO: Add your control notification handler code here	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;	
	FilenameSyntax.DelSyntaxNodeUnused();
	BuildParamListWnd();
	UpdateFilenameText();
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnSaveFileBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateUIToKernal();
	const bool bVerify=true;	
	if ( UpdateFilenameText(bVerify) == false )
	{	return; }	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;		
	if ( FilenameSyntax.SaveFilenameSyntaxIni() == false )
	{	JetAPI::ShowMessageBox(FilenameSyntax.GetErrorString());	}
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnLoadFileBtn() 
{
	// TODO: Add your control notification handler code here	
	CFilenameSyntax &FilenameSyntax=m_FilenameSyntax;	
	if ( FilenameSyntax.LoadFilenameSyntaxIni() == false )
	{	JetAPI::ShowMessageBox(FilenameSyntax.GetErrorString());	}
	BuildParamListWnd();
	UpdateFilenameText();
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntaxWnd::OnUpdateParamBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateUIToKernal();
	BuildParamListWnd();
	UpdateFilenameText();
	return;
}
//-------------------------------------------------------------------------------------//
