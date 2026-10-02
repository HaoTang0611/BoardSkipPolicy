// UserRegisterWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "UserRegisterWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "UserLevelWnd.h"
#include "FingerprintWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserRegisterWnd dialog
//-------------------------------------------------------------------------------------//
CUserRegisterWnd::CUserRegisterWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CUserRegisterWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserRegisterWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StopUserListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserRegisterWnd)
	DDX_Control(pDX, USEREG_USER_LIST_WND, m_UserListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CUserRegisterWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CUserRegisterWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(USEREG_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(USEREG_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(USEREG_NEW_BTN, OnNewBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserRegisterWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CUserRegisterWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_UserListCtrl);	
	BuildUserListWndHeader();
	
	m_UserModified = false;
	CString filename = AOIDataCollect.GetUserFilename();
	m_LoginUser = AOIDataCollect.GetLoginUserNode();
#ifdef _DEBUG
	m_LoginUser.eUserLevel = USER_LEVEL_JET_RD;
#endif _DEBUG

	AOIDataCollect.ReadUserFile(filename, m_UserList);	
	BuildUserListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	if ( true == m_UserModified )
	{
		str = _T("Do you want to save to file?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	OnSaveBtn();		}
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnCancel()
{
	DelUserFingerprint(m_UserListAdd);
	m_UserListAdd.clear();
	m_UserListDel.clear();
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_USER_REGISTER_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_USER_REGISTER_WND;
	WndKey = _T("IDD_USER_REGISTER_WND");
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
	WndID = USEREG_USER_LIST_LABEL;
	WndKey = _T("USEREG_USER_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = USEREG_NEW_BTN;
	WndKey = _T("USEREG_NEW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = USEREG_DEL_BTN;
	WndKey = _T("USEREG_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = USEREG_SAVE_BTN;
	WndKey = _T("USEREG_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CUserRegisterWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_USER_REGISTER_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CUserRegisterWnd::BuildUserListWnd()
{
	CThisListCtrl_35 &ListCtrl = m_UserListCtrl;	
	m_StopUserListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopUserListBeSelected = false;

	int           i=0;
	CString       str;
	CString       strName;	
	CString       strLevel;	
	TUserNode    *UserNodePtr=NULL;
	int           nItem=0;	
	int           nSubItem = 1;
	const size_t  UserCount = m_UserList.size();

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopUserListBeSelected = true;
	for ( i=0; i<UserCount; i++ )
	{
		UserNodePtr = &(m_UserList[i]);
		if ( NULL == UserNodePtr ) { continue; }
		if ( UserNodePtr->eUserLevel >= m_LoginUser.eUserLevel ) { continue; }		

		strName = UserNodePtr->wUserName;
		strLevel = AOIDataDefine.GetUserLevelModeText(UserNodePtr->eUserLevel);
		nSubItem = 0;

		ListCtrl.InsertItem(nItem, strName);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strName);
		nSubItem ++;

		ListCtrl.SetItemText(nItem, nSubItem, strLevel);
		nSubItem ++;
		nItem ++;
	}	
	m_StopUserListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserRegisterWnd::BuildUserListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_35 &ListCtrl = m_UserListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/4;

	width2 = width*2;
	str = _T("User");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Level");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserRegisterWnd::DelUserFingerprint(std::vector<TUserNode> &DelList)
{
	CFingerprintWnd FingerprintWnd;
	if (USER_LOGIN_OPTIONS_PASSWORD == AOIDataCollect.GetSystemParameter().m_UserLoginOptions) { return true; }
	if (false == FingerprintWnd.DeleteFingerprintInDB(DelList)) { return false; }
	DelList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserRegisterWnd::LogModifyUserList()
{
	size_t i;
	for (i = 0; i < m_UserListAdd.size(); i++) {
		LogOperCtrl.SaveLogUserLogDelAdd(L"Add", m_UserListAdd[i].wUserName);
	}
	for (i = 0; i < m_UserListDel.size(); i++) {
		LogOperCtrl.SaveLogUserLogDelAdd(L"Del", m_UserListDel[i].wUserName);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here
	CString filename = AOIDataCollect.GetUserFilename();
	if ( AOIDataCollect.WriteUserFile(filename, m_UserList) == false )
	{
		CString str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	else
	{	
		DelUserFingerprint(m_UserListDel);
		LogModifyUserList();
		m_UserListAdd.clear();
		m_UserListDel.clear();
		m_UserModified = false; 
	}
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnDelBtn() 
{
	// TODO: Add your control notification handler code here
	CString   str;
	CThisListCtrl_35 &ListCtrl = m_UserListCtrl;

	size_t    i=0;
	int       nItem=0;		
	size_t    UserIdx=0;	
	std::vector<int>       SelIndexList;
	std::vector<TUserNode> TmpUserList;
	const size_t UserCount = m_UserList.size();
	POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
	if ( NULL == pos ) { return; }

	str = _T("Do you want to delete the users?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }

	for ( i=0; i<UserCount; i++ )
	{	SelIndexList.push_back(FN_ENABLE);	}

	while (pos)
	{
		nItem = ListCtrl.GetNextSelectedItem(pos);
		UserIdx = ListCtrl.GetItemData(nItem);
		if ( UserIdx >= UserCount ) { continue; }
		SelIndexList[UserIdx] = FN_DISABLE;
	};

	TmpUserList = m_UserList;;
	m_UserList.clear();
	for ( i=0; i<UserCount; i++ )
	{
		if ( FN_DISABLE == SelIndexList[i] ) { 
			m_UserListDel.push_back(TmpUserList[i]);
			continue; 
		}
		m_UserList.push_back(TmpUserList[i]);
	}
	m_UserModified = true;
	BuildUserListWnd();
}
//-------------------------------------------------------------------------------------//
void CUserRegisterWnd::OnNewBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;
	CString      UserName;
	CString      PassWord;
	CString      PassWordConfirm;
	CString      strName;
	CString      strValue;
	CString      strCaption;
	TUserNode    UserNode;
	CInputBoxWnd InputBox;
	CUserLevelWnd UserLevelWnd;
	CFingerprintWnd FingerprintWnd;


	str = _T("Input User Name");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Name");
	strName = LoadMultiLanguageString(str, str);
	while ( true )
	{
		InputBox.SetParam1(strCaption, strName, strValue);
		if ( InputBox.DoModal() == IDCANCEL ) { return; }
		strValue = UserName = InputBox.m_DataEdit1;
		JetAPI::TCHAR2wchar(UserName, UserNode.wUserName, sizeof(UserNode.wUserName));
		if ( AOIDataCollect.FindUserNode(m_UserList, UserName) == true )
		{	continue;	}
		break;
	};

	strValue = _T("");
	str = _T("Input Password");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Password");
	strName = LoadMultiLanguageString(str, str);
	InputBox.SetPasswordMode(true);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return; }
	PassWord = InputBox.m_DataEdit1;

	str = _T("Input confirm Password ");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Confirm Password");
	strName = LoadMultiLanguageString(str, str);
	InputBox.SetPasswordMode(true);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return; }
	PassWordConfirm = InputBox.m_DataEdit1;
	if ( PassWordConfirm != PassWord )
	{
		str = _T("Error, confirm password fault");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return;
	}	
	JetAPI::TCHAR2wchar(PassWord, UserNode.wPassword, sizeof(UserNode.wPassword));

	if (USER_LOGIN_OPTIONS_PASSWORD<AOIDataCollect.GetSystemParameter().m_UserLoginOptions) {
		FingerprintWnd.SetFingerPrintMode(USER_FINGERPRINT_MODE_ENROLL);
		if (FingerprintWnd.DoModal() == IDCANCEL) { return; }
		FingerprintWnd.GetEnrollFingerPrint(UserNode.wFinger);
		UserNode.wFingerEnable = true;
	}
	m_UserListAdd.push_back(UserNode);
	UserLevelWnd.SetChargeLevel(m_LoginUser.eUserLevel);
	if ( UserLevelWnd.DoModal() == IDCANCEL ) { 
		DelUserFingerprint(m_UserListAdd);
		return; 
	}
	UserNode.eUserLevel = UserLevelWnd.GetResultLevel();
	m_UserList.push_back(UserNode);
	m_UserModified = true;
	BuildUserListWnd();
}
//-------------------------------------------------------------------------------------//