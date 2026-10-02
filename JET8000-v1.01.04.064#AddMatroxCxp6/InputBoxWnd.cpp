// InputBoxWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputBoxWnd dialog
//-------------------------------------------------------------------------------------//
CInputBoxWnd::CInputBoxWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CInputBoxWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBoxWnd)
	m_WndText = _T("Input Box");
	m_TitleLabel1 = _T("");
	m_TitleLabel2 = _T("");
	m_DataEdit1 = _T("");
	m_DataEdit2 = _T("");
	//}}AFX_DATA_INIT
	m_DataCount = 0;	
	m_WndMovePos = false;
	m_ReadOnly = false;
	m_ReadOnly2 = false;
	m_PasswordMode = false;
	m_PasswordMode2 = false;
	m_WndPos.x = m_WndPos.y = 0;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBoxWnd)
	DDX_Text(pDX, INPUTBOX_TITLE_LABEL1, m_TitleLabel1);
	DDX_Text(pDX, INPUTBOX_TITLE_LABEL2, m_TitleLabel2);
	DDX_Text(pDX, INPUTBOX_DATA_EDIT1, m_DataEdit1);
	DDX_Text(pDX, INPUTBOX_DATA_EDIT2, m_DataEdit2);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CInputBoxWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CInputBoxWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputBoxWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CInputBoxWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	CWnd::SetWindowText(m_WndText);
	if ( true == m_WndMovePos )
	{
		RECT  WndRect = {0};
		SIZE  WndSize;
		CWnd::GetWindowRect(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.left = m_WndPos.x - (WndSize.cx/2);
		WndRect.right = WndRect.left + WndSize.cx;
		WndRect.top = m_WndPos.y - (WndSize.cy/2);
		WndRect.bottom = WndRect.top + WndSize.cy;		
		CWnd::MoveWindow(&WndRect);
		//m_WndMovePos = false;
	}
	
	if ( 1 == m_DataCount )
	{
		SIZE  WndSize={0};
		SIZE  ClientSize={0};
		RECT  WndRect = {0};
		RECT  ClientRect = {0};		

		CWnd::GetWindowRect(&WndRect);
		CWnd::GetClientRect(&ClientRect);

		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		ClientSize.cx = ClientRect.right-ClientRect.left;
		ClientSize.cy = ClientRect.bottom-ClientRect.top;
		
		const int TitleH = WndSize.cy-ClientSize.cy;
		WndRect.bottom = WndRect.top+TitleH+(ClientSize.cy/2);
		CWnd::MoveWindow(&WndRect);
	}	

	if ( true == m_ReadOnly )
	{	SetEditReadOnly(INPUTBOX_DATA_EDIT1);	}
	if ( true == m_ReadOnly2 )
	{	SetEditReadOnly(INPUTBOX_DATA_EDIT2);	}

	if ( true == m_PasswordMode )
	{	SetEditPasswordMode(INPUTBOX_DATA_EDIT1);	}
	if ( true == m_PasswordMode2 )
	{	SetEditPasswordMode(INPUTBOX_DATA_EDIT2);	}

	this->UpdateData();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Label, LPCTSTR Default)
{
	m_DataCount = 1;
	this->m_WndText = WndTxt;
	this->m_TitleLabel1 = Label;
	this->m_DataEdit1 = Default;
	return;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SetParam2(LPCTSTR WndTxt, LPCTSTR Label1, LPCTSTR Default1, LPCTSTR Label2, LPCTSTR Default2)
{
	m_DataCount = 2;
	this->m_WndText = WndTxt;
	this->m_TitleLabel1 = Label1;
	this->m_DataEdit1 = Default1;
	this->m_TitleLabel2 = Label2;
	this->m_DataEdit2 = Default2;
	return;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SetWndPos(const POINT &Pos)
{
	m_WndPos = Pos;
	m_WndMovePos = true;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SetReadOnly(bool bReadOnly, bool bReadOnly2)
{
	m_ReadOnly = bReadOnly;
	m_ReadOnly2 = bReadOnly2;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SetPasswordMode(bool bPassword, bool bPassword2)
{
	m_PasswordMode = true;
}
//-------------------------------------------------------------------------------------//
void CInputBoxWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_INPUT_BOX_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_INPUT_BOX_WND;
	WndKey = _T("IDD_INPUT_BOX_WND");
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
	//---------------------------------------------------------------------------------/
}
//-------------------------------------------------------------------------------------//
bool CInputBoxWnd::SetEditReadOnly(UINT CtrlID)
{
	CEdit *pEdit = (CEdit*)GetDlgItem(CtrlID);
	if ( NULL==pEdit || NULL==pEdit->GetSafeHwnd() ) { return false; }
	pEdit->SetReadOnly(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CInputBoxWnd::SetEditPasswordMode(UINT CtrlID)
{
	CEdit *pEdit = (CEdit*)GetDlgItem(CtrlID);
	if ( NULL==pEdit || NULL==pEdit->GetSafeHwnd() ) { return false; }
	pEdit->SetPasswordChar(_T('*'));
	return true;
}
//-------------------------------------------------------------------------------------//