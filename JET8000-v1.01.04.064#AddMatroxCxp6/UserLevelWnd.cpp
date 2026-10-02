// UserLevelWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "UserLevelWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserLevelWnd dialog
//-------------------------------------------------------------------------------------//
CUserLevelWnd::CUserLevelWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CUserLevelWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLevelWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ChargeLevel = USER_LEVEL_SIGN_OUT;
	m_ResultLevel = USER_LEVEL_SIGN_OUT;
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLevelWnd)
	DDX_Control(pDX, USERLV_LEVEL_LIST_WND, m_LevelListCtrl);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CUserLevelWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CUserLevelWnd)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, USERLV_LEVEL_LIST_WND, OnDblclkLevelListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CUserLevelWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CUserLevelWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_LevelListCtrl);	
	BuildLevelListWndHeader();
	BuildLevelListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::OnOK() 
{
	// TODO: Add extra validation here
	CThisListCtrl_34 &ListCtrl = m_LevelListCtrl;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem >= 0 ) 
	{
		DWORD_PTR Data = ListCtrl.GetItemData(nItem);
		m_ResultLevel = (USER_LEVEL_MODE)(Data);
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_USER_LEVEL_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_USER_LEVEL_WND;
	WndKey = _T("IDD_USER_LEVEL_WND");
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
}
//-------------------------------------------------------------------------------------//
CString CUserLevelWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_USER_LEVEL_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::SetChargeLevel(USER_LEVEL_MODE Lv)
{
	m_ChargeLevel = Lv;	
}
//-------------------------------------------------------------------------------------//
USER_LEVEL_MODE CUserLevelWnd::GetResultLevel() const
{
	return m_ResultLevel;
}
//-------------------------------------------------------------------------------------//
bool CUserLevelWnd::BuildLevelListWnd()
{
	CThisListCtrl_34 &ListCtrl = m_LevelListCtrl;

	int     nItem=0;
	CString strLevel;
	USER_LEVEL_MODE LevelMode;

	nItem = 0;
	LevelMode = USER_LEVEL_OPERATOR;
	if ( LevelMode < m_ChargeLevel )
	{
		strLevel = AOIDataDefine.GetUserLevelModeText(LevelMode);
		ListCtrl.InsertItem(nItem, strLevel);
		ListCtrl.SetItemData(nItem, LevelMode);
		nItem ++;
	}

	LevelMode = USER_LEVEL_ENGINEER;
	if ( LevelMode < m_ChargeLevel )
	{
		strLevel = AOIDataDefine.GetUserLevelModeText(LevelMode);
		ListCtrl.InsertItem(nItem, strLevel);
		ListCtrl.SetItemData(nItem, LevelMode);
		nItem ++;
	}

	LevelMode = USER_LEVEL_SUPERVISOR;
	if ( LevelMode < m_ChargeLevel )
	{
		strLevel = AOIDataDefine.GetUserLevelModeText(LevelMode);
		ListCtrl.InsertItem(nItem, strLevel);
		ListCtrl.SetItemData(nItem, LevelMode);
		nItem ++;
	}

	LevelMode = USER_LEVEL_JET_FAE;
	if ( LevelMode < m_ChargeLevel )
	{
		strLevel = AOIDataDefine.GetUserLevelModeText(LevelMode);
		ListCtrl.InsertItem(nItem, strLevel);
		ListCtrl.SetItemData(nItem, LevelMode);
		nItem ++;
	}

	LevelMode = USER_LEVEL_JET_SENIOR;
	if ( LevelMode < m_ChargeLevel )
	{
		strLevel = AOIDataDefine.GetUserLevelModeText(LevelMode);
		ListCtrl.InsertItem(nItem, strLevel);
		ListCtrl.SetItemData(nItem, LevelMode);
		nItem ++;
	}
	//USER_LEVEL_JET_RD     = 201,
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserLevelWnd::BuildLevelListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_34 &ListCtrl = m_LevelListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-16)/1;

	width2 = width;
	str = _T("Level");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CUserLevelWnd::OnDblclkLevelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//