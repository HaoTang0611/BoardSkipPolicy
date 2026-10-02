// RemoteParamWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "RemoteParamWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL = 1;
const int MAX_LIST_CTRL_COUNT = 12;
//-------------------------------------------------------------------------------------//
enum REMOTE_PARAM
{
	REMOTE_PARAM_REMOTE_PC_NAME,
	REMOTE_PARAM_PROJECT_FOLDER,
	REMOTE_PARAM_TUNING_FOLDER,	
	REMOTE_PARAM_RETURN
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CRemoteParamWnd dialog
//-------------------------------------------------------------------------------------//
CRemoteParamWnd::CRemoteParamWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CRemoteParamWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteParamWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteParamWnd)	
	DDX_Control(pDX, REMOTE_FOLDER_BTN, m_FoderBtn);
	DDX_Control(pDX, REMOTE_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, REMOTE_PARAM_COMBO, m_ComboxCtrl);
	DDX_Control(pDX, REMOTE_LIST_WND_01, m_ParamListCtrl_01);
	DDX_Control(pDX, REMOTE_LIST_WND_02, m_ParamListCtrl_02);
	DDX_Control(pDX, REMOTE_LIST_WND_03, m_ParamListCtrl_03);
	DDX_Control(pDX, REMOTE_LIST_WND_04, m_ParamListCtrl_04);
	DDX_Control(pDX, REMOTE_LIST_WND_05, m_ParamListCtrl_05);
	DDX_Control(pDX, REMOTE_LIST_WND_06, m_ParamListCtrl_06);
	DDX_Control(pDX, REMOTE_LIST_WND_07, m_ParamListCtrl_07);
	DDX_Control(pDX, REMOTE_LIST_WND_08, m_ParamListCtrl_08);
	DDX_Control(pDX, REMOTE_LIST_WND_09, m_ParamListCtrl_09);
	DDX_Control(pDX, REMOTE_LIST_WND_10, m_ParamListCtrl_10);
	DDX_Control(pDX, REMOTE_LIST_WND_11, m_ParamListCtrl_11);
	DDX_Control(pDX, REMOTE_LIST_WND_12, m_ParamListCtrl_12);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CRemoteParamWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CRemoteParamWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(REMOTE_NEW_BTN, OnNewBtn)
	ON_BN_CLICKED(REMOTE_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(REMOTE_LOAD_BTN, OnLoadBtn)
	ON_BN_CLICKED(REMOTE_FOLDER_BTN, OnFolderBtn)
	ON_BN_CLICKED(REMOTE_CLEAR_BTN, OnClearBtn)
	ON_EN_KILLFOCUS(REMOTE_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(REMOTE_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(REMOTE_PARAM_COMBO, OnKillfocusParamCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_01, OnItemchangedListWnd01)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_01, OnDblclkListWnd01)
	ON_BN_CLICKED(REMOTE_USE_CHK_01, OnUseChk01)
	ON_BN_CLICKED(REMOTE_DEL_BTN_01, OnDelBtn01)	
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_02, OnItemchangedListWnd02)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_02, OnDblclkListWnd02)
	ON_BN_CLICKED(REMOTE_USE_CHK_02, OnUseChk02)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_02, OnDelBtn02)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_03, OnItemchangedListWnd03)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_03, OnDblclkListWnd03)
	ON_BN_CLICKED(REMOTE_USE_CHK_03, OnUseChk03)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_03, OnDelBtn03)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_04, OnItemchangedListWnd04)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_04, OnDblclkListWnd04)
	ON_BN_CLICKED(REMOTE_USE_CHK_04, OnUseChk04)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_04, OnDelBtn04)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_05, OnItemchangedListWnd05)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_05, OnDblclkListWnd05)
	ON_BN_CLICKED(REMOTE_USE_CHK_05, OnUseChk05)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_05, OnDelBtn05)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_06, OnItemchangedListWnd06)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_06, OnDblclkListWnd06)
	ON_BN_CLICKED(REMOTE_USE_CHK_06, OnUseChk06)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_06, OnDelBtn06)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_07, OnItemchangedListWnd07)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_07, OnDblclkListWnd07)
	ON_BN_CLICKED(REMOTE_USE_CHK_07, OnUseChk07)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_07, OnDelBtn07)
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_08, OnItemchangedListWnd08)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_08, OnDblclkListWnd08)
	ON_BN_CLICKED(REMOTE_USE_CHK_08, OnUseChk08)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_08, OnDelBtn08)	
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_09, OnItemchangedListWnd09)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_09, OnDblclkListWnd09)
	ON_BN_CLICKED(REMOTE_USE_CHK_09, OnUseChk09)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_09, OnDelBtn09)	
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_10, OnItemchangedListWnd10)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_10, OnDblclkListWnd10)
	ON_BN_CLICKED(REMOTE_USE_CHK_10, OnUseChk10)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_10, OnDelBtn10)	
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_11, OnItemchangedListWnd11)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_11, OnDblclkListWnd11)
	ON_BN_CLICKED(REMOTE_USE_CHK_11, OnUseChk11)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_11, OnDelBtn11)	
	ON_NOTIFY(LVN_ITEMCHANGED, REMOTE_LIST_WND_12, OnItemchangedListWnd12)
	ON_NOTIFY(NM_DBLCLK, REMOTE_LIST_WND_12, OnDblclkListWnd12)
	ON_BN_CLICKED(REMOTE_USE_CHK_12, OnUseChk12)	
	ON_BN_CLICKED(REMOTE_DEL_BTN_12, OnDelBtn12)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CRemoteParamWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CRemoteParamWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	SwitchMultiLanguage();
	BuildParamListWndHeader();
	BuildParamListWnd();
	UpdateParamUI();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearParamListWnd();
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::GetRemoteParamList(std::vector<TRemoteParam> &RemoteParamList)
{
	AOIDataCollect.ArrangeRemoteParamList(m_RemoteParamList, RemoteParamList);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::SetRemoteParamList(const std::vector<TRemoteParam> &RemoteParamList)
{
	m_RemoteParamList = RemoteParamList;
}
//-------------------------------------------------------------------------------------//
int CRemoteParamWnd::MapListCtrlIDToParamListIndex(UINT ListID)//ListID轉成List引數	
{
	int ParamListIndex=-1;
	switch ( ListID )
	{
	case REMOTE_LIST_WND_01:	ParamListIndex= 0; break;
	case REMOTE_LIST_WND_02:	ParamListIndex= 1; break;
	case REMOTE_LIST_WND_03:	ParamListIndex= 2; break;
	case REMOTE_LIST_WND_04:	ParamListIndex= 3; break;	
	case REMOTE_LIST_WND_05:	ParamListIndex= 4; break;	
	case REMOTE_LIST_WND_06:	ParamListIndex= 5; break;	
	case REMOTE_LIST_WND_07:	ParamListIndex= 6; break;	
	case REMOTE_LIST_WND_08:	ParamListIndex= 7; break;	
	case REMOTE_LIST_WND_09:	ParamListIndex= 8; break;	
	case REMOTE_LIST_WND_10:	ParamListIndex= 9; break;	
	case REMOTE_LIST_WND_11:	ParamListIndex=10; break;	
	case REMOTE_LIST_WND_12:	ParamListIndex=11; break;	
	}
	return ParamListIndex;
}
//-------------------------------------------------------------------------------------//
CParamUni* CRemoteParamWnd::GetListCtrlParamUniPtr(UINT ListID, size_t Index)//取得列表的參數指標
{
	CParamUni   *ParamPtr=NULL;
	const size_t ParamListCount = m_ParamListSet.size();
	const int ParamListIndex = MapListCtrlIDToParamListIndex(ListID);
	if ( ParamListIndex < 0 ) { return ParamPtr; }
	if ( ParamListIndex >= ParamListCount ) { return ParamPtr; }
	const size_t ParamCount = m_ParamListSet[ParamListIndex].size();
	if ( Index >= ParamCount ) { return ParamPtr; }
	ParamPtr = &(m_ParamListSet[ParamListIndex][Index]);
	return ParamPtr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CRemoteParamWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamList()
{
	size_t       i=0;
	CParamList   ParamList;
	const size_t RemoteParamCount=m_RemoteParamList.size();

	m_ParamListSet.clear();
	for ( i=0; i<RemoteParamCount; i++ )
	{
		if ( BuildParamListKernel(m_RemoteParamList[i], ParamList) == false )
		{	return false; }
		m_ParamListSet.push_back(ParamList);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamListKernel(const TRemoteParam &Param, CParamList &ParamList)
{
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	int       nItem=0;
	REMOTE_PARAM RemoteEnum;
	CParamUni    ParamUnit;	
	const int nSubItem = 1;
	const size_t ParamIndex=Param.nIndex;
	const bool   bDeleted = Param.bDeleted;

	ParamList.clear();	
	
	//遠端電腦名稱
	ParamUnit = CParamUni();
	str = _T("PC Name");
	strCaption = LoadMultiLanguageString(str, str);
	RemoteEnum = REMOTE_PARAM_REMOTE_PC_NAME;
	ParamUnit.SetParamIndex(ParamIndex);
	ParamUnit.SetParamID((UINT)(RemoteEnum));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Param.sRemotePCName);
	ParamUnit.SetDesction(strCaption);	
	//ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);	

	//專案資料夾	
	ParamUnit = CParamUni();
	str = _T("Project Folder");
	strCaption = LoadMultiLanguageString(str, str);
	RemoteEnum = REMOTE_PARAM_PROJECT_FOLDER;
	ParamUnit.SetParamIndex(ParamIndex);
	ParamUnit.SetParamID((UINT)(RemoteEnum));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Param.sProjectFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);
	
	//調機資料夾	
	ParamUnit = CParamUni();
	str = _T("Tuning Folder");
	strCaption = LoadMultiLanguageString(str, str);
	RemoteEnum = REMOTE_PARAM_TUNING_FOLDER;
	ParamUnit.SetParamIndex(ParamIndex);
	ParamUnit.SetParamID((UINT)(RemoteEnum));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_STR(Param.sTuningFolder);
	ParamUnit.SetDesction(strCaption);
	ParamUnit.SetBtnWndPtr(&m_FoderBtn);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ClearParamListWnd()
{
	ClearParamListWndKernel(m_ParamListCtrl_01);
	ClearParamListWndKernel(m_ParamListCtrl_02);
	ClearParamListWndKernel(m_ParamListCtrl_03);
	ClearParamListWndKernel(m_ParamListCtrl_04);
	ClearParamListWndKernel(m_ParamListCtrl_05);
	ClearParamListWndKernel(m_ParamListCtrl_06);
	ClearParamListWndKernel(m_ParamListCtrl_07);
	ClearParamListWndKernel(m_ParamListCtrl_08);
	ClearParamListWndKernel(m_ParamListCtrl_09);
	ClearParamListWndKernel(m_ParamListCtrl_10);
	ClearParamListWndKernel(m_ParamListCtrl_11);
	ClearParamListWndKernel(m_ParamListCtrl_12);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ClearParamListWndKernel(CThisListCtrl_42 &ListCtrl)
{
	SetActParamUni(NULL);
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamListWnd()
{
	ClearParamListWnd();
	if ( BuildParamList() == false ) 
	{	return false; }

	size_t       i=0;
	TRemoteParam RemoteParam;
	CThisListCtrl_42 *ListCtrlPtr=NULL;
	const size_t ParamListCount=m_ParamListSet.size();
	const size_t RemoteParamCount=m_RemoteParamList.size();
	if ( ParamListCount != RemoteParamCount )
	{	return false; }

	for ( i=0; i<ParamListCount; i++ )
	{
		switch ( i )
		{
		case  0:	ListCtrlPtr = &m_ParamListCtrl_01; break;
		case  1:	ListCtrlPtr = &m_ParamListCtrl_02; break;
		case  2:	ListCtrlPtr = &m_ParamListCtrl_03; break;
		case  3:	ListCtrlPtr = &m_ParamListCtrl_04; break;
		case  4:	ListCtrlPtr = &m_ParamListCtrl_05; break;
		case  5:	ListCtrlPtr = &m_ParamListCtrl_06; break;
		case  6:	ListCtrlPtr = &m_ParamListCtrl_07; break;
		case  7:	ListCtrlPtr = &m_ParamListCtrl_08; break;
		case  8:	ListCtrlPtr = &m_ParamListCtrl_09; break;
		case  9:	ListCtrlPtr = &m_ParamListCtrl_10; break;
		case 10:	ListCtrlPtr = &m_ParamListCtrl_11; break;
		case 11:	ListCtrlPtr = &m_ParamListCtrl_12; break;
		default:
			ListCtrlPtr = NULL;
			break;
		}
		if ( NULL == ListCtrlPtr ) 
		{	break; }
		RemoteParam = m_RemoteParamList[i];
		if ( true == RemoteParam.bDeleted ) { continue; }
		if ( BuildParamListWndKernel(m_ParamListSet[i], *ListCtrlPtr) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamListWndKernel(CParamList &ParamList, CThisListCtrl_42 &ListCtrl)
{
	ClearParamListWndKernel(ListCtrl);

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	const int ParamCount = (int)(ParamList.size());
	
	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamListWndHeader()
{	
	BuildParamListWndHeaderKernel(m_ParamListCtrl_01);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_02);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_03);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_04);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_05);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_06);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_07);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_08);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_09);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_10);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_11);
	BuildParamListWndHeaderKernel(m_ParamListCtrl_12);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::BuildParamListWndHeaderKernel(CThisListCtrl_42 &ListCtrl)
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT	

	JetAPI::InitialListCtrl(ListCtrl);
	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*4;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_REMOTE_PARAM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_REMOTE_PARAM_WND;
	WndKey = _T("IDD_REMOTE_PARAM_WND");
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
	WndID = REMOTE_NEW_BTN;
	WndKey = _T("REMOTE_NEW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_SAVE_BTN;
	WndKey = _T("REMOTE_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_LOAD_BTN;
	WndKey = _T("REMOTE_LOAD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = REMOTE_CLEAR_BTN;
	WndKey = _T("REMOTE_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_01;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 1);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_01;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_01;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_02;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 2);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_02;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_02;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_03;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 3);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_03;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_03;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_04;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 4);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_04;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_04;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_05;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 5);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_05;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_05;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_06;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 6);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_06;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_06;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_07;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 7);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_07;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_07;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_08;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 8);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_08;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_08;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_09;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 9);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_09;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_09;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_10;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 10);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_10;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_10;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_11;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 11);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_11;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_11;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = REMOTE_PARAM_GROUP_12;
	WndKey = _T("REMOTE_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	LabelText.Format(_T("%s - %d"), NewLabelText, 12);
	this->SetDlgItemText(WndID, LabelText);	
	
	WndID = REMOTE_USE_CHK_12;
	WndKey = _T("REMOTE_USE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = REMOTE_DEL_BTN_12;
	WndKey = _T("REMOTE_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CRemoteParamWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_REMOTE_PARAM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
BOOL CRemoteParamWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CRemoteParamWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnNewBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;	
	CString      strName;
	CString      strValue;
	CString      strCaption;
	CInputBoxWnd InputBox;
	TRemoteParam RemoteParam;
	const size_t RemoteParamCount = m_RemoteParamList.size();
	if ( RemoteParamCount >= MAX_LIST_CTRL_COUNT )
	{	return;	}

	str = _T("Input PC Name");
	strCaption = LoadMultiLanguageString(str, str);	
	strName = AOIDataDefine.GetNameText();
	strValue.Format(_T("PC - %d"), RemoteParamCount+1);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return ; }
	size_t ActIndex = AOIDataCollect.GetRemoteParamActivedIndex(m_RemoteParamList);

	strValue = InputBox.m_DataEdit1;	
	RemoteParam.nIndex = RemoteParamCount;
	RemoteParam.sRemotePCName = strValue;
	if ( -1 == ActIndex )
	{	RemoteParam.bActived = true;	}
	m_RemoteParamList.push_back(RemoteParam);
	BuildParamListWnd();
	UpdateParamUI();
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here		
	std::vector<TRemoteParam>  RemoteParamList;
	AOIDataCollect.ArrangeRemoteParamList(m_RemoteParamList, RemoteParamList);
	AOIDataCollect.SetSystemRemoteParamList(RemoteParamList);
	AOIDataCollect.SaveSystemRemoteParameter();
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	m_RemoteParamList.clear();
	AOIDataCollect.LoadSystemRemoteParameter();
	AOIDataCollect.GetSystemRemoteParamList(m_RemoteParamList);
	BuildParamListWnd();
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to clear all remote param ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	m_RemoteParamList.clear();
	BuildParamListWnd();
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::SetRemoteParamByParamUni(CParamUni *ParamUniPtr, std::vector<TRemoteParam> &ParamList, LPCTSTR String)
{
	if ( NULL == ParamUniPtr ) { return false; }
	const size_t ParamListCount = ParamList.size();
	const size_t ParamListIndex = ParamUniPtr->GetParamIndex();		
	if ( ParamListIndex >= ParamListCount ) { return false; }

	REMOTE_PARAM RemoteEnum = (REMOTE_PARAM)(ParamUniPtr->GetParamID());	
	switch ( RemoteEnum )
	{
	case REMOTE_PARAM_REMOTE_PC_NAME:
		ParamList[ParamListIndex].sRemotePCName = String;
		break;
	case REMOTE_PARAM_PROJECT_FOLDER:
		ParamList[ParamListIndex].sProjectFolder = String;
		break;
	case REMOTE_PARAM_TUNING_FOLDER:
		ParamList[ParamListIndex].sTuningFolder = String;
		break;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecReleaseCtrlWnd()
{
	m_FoderBtn.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecReleaseParamCtrl()
{
	ExecReleaseCtrlWnd();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	//m_FolderListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecItemchangedParamListWnd(CThisListCtrl_42 &ListCtrl, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	UINT       ListCtrlID = ListCtrl.GetDlgCtrlID();
	size_t     ParamIndex = ListCtrl.GetItemData(nItem);	
	CParamUni *ParamPtr = GetListCtrlParamUniPtr(ListCtrlID, ParamIndex);
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	const int  nSubItem = ParamPtr->GetSubItemIndex();	
	
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
	{	ExecReleaseCtrlWnd();	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecDblclkParamListWnd(CThisListCtrl_42 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SETTING_COL ) { return true; }

	UINT ListCtrlID = ListCtrl.GetDlgCtrlID();
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);

	size_t       i=0;
	int          nSelIdx=0;
	int          nValue=0;
	CRect        ItemRect;
	RECT         CtrlRect={0};	
	CString      ItemText;	
	const int    Offset = 2;	
	CParamUni   *ParamPtr = GetListCtrlParamUniPtr(ListCtrlID, ParamIndex);
	const bool   ReadOnly = ParamPtr->GetReadOnly();
	if ( true == ReadOnly ) { return true; }

	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	const size_t     SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	ExecReleaseCtrlWnd();
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
bool CRemoteParamWnd::ExecUpdateParamByBtn()
{
	ExecReleaseCtrlWnd();	
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return true; }	
	CString ItemText = ParamPtr->GetParamText();
	if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return false; }
	if ( SetRemoteParamByParamUni(ParamPtr, m_RemoteParamList, ItemText) == false )	
	{	return false; }

	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_42 *pListCtrl = (CThisListCtrl_42*)(ParamPtr->GetListCtrl());
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;		
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( SetRemoteParamByParamUni(ParamPtr, m_RemoteParamList, ItemText) == false )	
	{	return false; }
	
	CThisListCtrl_42 *pListCtrl = (CThisListCtrl_42*)(ParamPtr->GetListCtrl());
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;			
	const int nCurSel = m_ComboxCtrl.GetCurSel();	
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( SetRemoteParamByParamUni(ParamPtr, m_RemoteParamList, ItemText) == false )	
	{	return false; }	

	ParamPtr->SetNewValue_SEL(Param);	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_42 *pListCtrl = (CThisListCtrl_42*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ChangeActIndex(int index)
{
	const size_t ParamCount = m_RemoteParamList.size();
	if ( index<0 || index>=ParamCount ) { return false; }
	AOIDataCollect.SetRemoteParamActived(index, m_RemoteParamList);	
	UpdateParamUI();
	OnOK();
	return true;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::UpdateParamUI()
{
	size_t     i=0;
	UINT       UsedID=0;
	const size_t RemoteParamCount=m_RemoteParamList.size();
	for ( i=0; i<RemoteParamCount; i++ )
	{
		switch ( i )
		{
		case  0:	UsedID = REMOTE_USE_CHK_01;	break;
		case  1:	UsedID = REMOTE_USE_CHK_02;	break;
		case  2:	UsedID = REMOTE_USE_CHK_03;	break;
		case  3:	UsedID = REMOTE_USE_CHK_04;	break;
		case  4:	UsedID = REMOTE_USE_CHK_05;	break;
		case  5:	UsedID = REMOTE_USE_CHK_06;	break;
		case  6:	UsedID = REMOTE_USE_CHK_07;	break;
		case  7:	UsedID = REMOTE_USE_CHK_08;	break;
		case  8:	UsedID = REMOTE_USE_CHK_09;	break;
		case  9:	UsedID = REMOTE_USE_CHK_10;	break;
		case 10:	UsedID = REMOTE_USE_CHK_11;	break;
		case 11:	UsedID = REMOTE_USE_CHK_12;	break;
		default:
			UsedID = 0;
			break;
		}
		if ( 0 == UsedID ) { break; }
		CWnd::CheckDlgButton(UsedID, m_RemoteParamList[i].bActived);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CRemoteParamWnd::ExecDeleteRemoteParam(size_t index)
{
	CString      str, str2;
	const size_t RemoteParamCount=m_RemoteParamList.size();
	if ( index >= RemoteParamCount ) { return false;}

	str = _T("Do you want to delete remote param");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s [%d] ?"), str, index+1);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) != IDYES )
	{	return false; }
	m_RemoteParamList[index].bDeleted = true;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnFolderBtn()
{
	ExecUpdateParamByBtn();
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnKillfocusParamEdit()
{
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnSelchangeParamCombo()
{
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnKillfocusParamCombo()
{
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd01(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_01, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd01(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_01, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk01() 
{
	// TODO: Add your control notification handler code here
	if ( ChangeActIndex(0) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_01, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn01()
{
	// TODO: Add your control notification handler code here	
	if ( ExecDeleteRemoteParam(0) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_01);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd02(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_02, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd02(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_02, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk02() 
{
	// TODO: Add your control notification handler code here
	if ( ChangeActIndex(1) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_02, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn02()
{
	// TODO: Add your control notification handler code here	
	if ( ExecDeleteRemoteParam(1) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_02);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd03(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_03, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd03(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_03, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk03()
{
	// TODO: Add your control notification handler code here
	if ( ChangeActIndex(2) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_03, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn03()
{
	// TODO: Add your control notification handler code here	
	if ( ExecDeleteRemoteParam(2) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_03);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd04(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_04, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd04(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_04, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk04()
{
	// TODO: Add your control notification handler code here	
	if ( ChangeActIndex(3) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_04, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn04()
{
	// TODO: Add your control notification handler code here		
	if ( ExecDeleteRemoteParam(3) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_04);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd05(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_05, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd05(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_05, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk05()
{
	// TODO: Add your control notification handler code here	
	if ( ChangeActIndex(4) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_05, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn05()
{
	if ( ExecDeleteRemoteParam(4) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_05);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd06(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_06, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd06(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_06, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk06()
{
	// TODO: Add your control notification handler code here	
	if ( ChangeActIndex(5) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_06, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn06()
{
	if ( ExecDeleteRemoteParam(5) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_06);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd07(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_07, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd07(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_07, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk07()
{
	// TODO: Add your control notification handler code here	
	if ( ChangeActIndex(6) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_07, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn07()
{
	if ( ExecDeleteRemoteParam(6) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_07);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd08(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_08, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd08(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_08, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk08()
{
	// TODO: Add your control notification handler code here	
	if ( ChangeActIndex(7) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_08, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn08()
{
	if ( ExecDeleteRemoteParam(7) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_08);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd09(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_09, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd09(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_09, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk09()
{
	if ( ChangeActIndex(8) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_09, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn09()
{
	if ( ExecDeleteRemoteParam(8) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_09);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd10(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_10, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd10(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_10, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk10()
{
	if ( ChangeActIndex(9) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_10, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn10()
{
	if ( ExecDeleteRemoteParam(9) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_10);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd11(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_11, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd11(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_11, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk11()
{
	if ( ChangeActIndex(10) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_11, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn11()
{
	if ( ExecDeleteRemoteParam(10) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_11);	
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnItemchangedListWnd12(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;		}
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
	ExecItemchangedParamListWnd(m_ParamListCtrl_12, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDblclkListWnd12(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl_12, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnUseChk12()
{
	if ( ChangeActIndex(11) == false )
	{	CWnd::CheckDlgButton(REMOTE_USE_CHK_12, FALSE); }
}
//-------------------------------------------------------------------------------------//
void CRemoteParamWnd::OnDelBtn12()
{
	if ( ExecDeleteRemoteParam(11) == false ) { return ; }
	ClearParamListWndKernel(m_ParamListCtrl_12);	
}
//-------------------------------------------------------------------------------------//
