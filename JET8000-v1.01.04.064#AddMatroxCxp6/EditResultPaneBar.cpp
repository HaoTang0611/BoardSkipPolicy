// EditResultPaneBar.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditResultPaneBar.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditResultPaneBar dialog
//-------------------------------------------------------------------------------------//
CEditResultPaneBar::CEditResultPaneBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CEditResultPaneBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CEditResultPaneBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditResultPaneBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditResultPaneBar, CDialogBar)
	//{{AFX_MSG_MAP(CEditResultPaneBar)
	ON_MESSAGE(WM_INITDIALOG , OnInitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditResultPaneBar message handlers
void CEditResultPaneBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	CWnd *pWnd = CWnd::GetParent();
	CDialogBar::OnUpdateCmdUI((CFrameWnd*)pWnd, bDisableIfNoHandler);
}
//-------------------------------------------------------------------------------------//
void CEditResultPaneBar::SwitchMultiLanguage()
{	
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_RESULT_PANE_BAR");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_RESULT_PANE_BAR;
	WndKey = _T("IDD_EDIT_RESULT_PANE_BAR");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = ERPB_SWITCH_PREVIOUS_BTN;
	WndKey = _T("ERPB_SWITCH_PREVIOUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ERPB_SWITCH_NEXT_BTN;
	WndKey = _T("ERPB_SWITCH_NEXT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ERPB_COMPONENT_RETEST_BTN;
	WndKey = _T("ERPB_COMPONENT_RETEST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ERPB_SHOW_PASS_CHK;
	WndKey = _T("ERPB_SHOW_PASS_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
LRESULT CEditResultPaneBar::OnInitDialog(WPARAM wParam, LPARAM lParam) 
{
	//CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam, lParam);
	if ( !UpdateData(FALSE))//在此之後會自動建立控制項的物件
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
		return FALSE;
	}
	SwitchMultiLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
bool CEditResultPaneBar::GetShowPassChk()
{
	if ( NULL == this ) { return false; }
	if ( CWnd::GetSafeHwnd() == NULL ) { return false; }
	BOOL bCheck = CWnd::IsDlgButtonChecked(ERPB_SHOW_PASS_CHK);
	if ( FALSE == bCheck ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultPaneBar::SetResultDateTime(LPCTSTR filename)
{
	if ( NULL == this ) { return false; }
	if ( CWnd::GetSafeHwnd() == NULL ) { return false; }
	CWnd::SetDlgItemText(ERPB_RESULT_DATE_TIME_EDIT, filename);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditResultPaneBar::SetResultListText(LPCTSTR filename)
{
	if ( NULL == this ) { return false; }
	if ( CWnd::GetSafeHwnd() == NULL ) { return false; }
	CWnd::SetDlgItemText(ERPB_RESULT_LIST_EDIT, filename);	
	return true;
}
//-------------------------------------------------------------------------------------//