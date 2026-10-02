// EditModelListPaneBar.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditModelListPaneBar.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelListPaneBar dialog
//-------------------------------------------------------------------------------------//
CEditModelListPaneBar::CEditModelListPaneBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CEditModelListPaneBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CEditModelListPaneBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditModelListPaneBar)
	DDX_Control(pDX, EMPB_SWITCH_MODE_COMBO, m_SwitchModeCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditModelListPaneBar, CDialogBar)
	//{{AFX_MSG_MAP(CEditModelListPaneBar)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG , OnInitDialog)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelListPaneBar message handlers
//-------------------------------------------------------------------------------------//
void CEditModelListPaneBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	CWnd *pWnd = CWnd::GetParent();
	CDialogBar::OnUpdateCmdUI((CFrameWnd*)pWnd, bDisableIfNoHandler);
}
//-------------------------------------------------------------------------------------//
void CEditModelListPaneBar::SwitchMultiLanguage()
{	
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_MODEL_LIST_PANE_BAR");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_MODEL_LIST_PANE_BAR;
	WndKey = _T("IDD_EDIT_MODEL_LIST_PANE_BAR");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = EMPB_SWITCH_PREVIOUS_BTN;
	WndKey = _T("EMPB_SWITCH_PREVIOUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = EMPB_SWITCH_NEXT_BTN;
	WndKey = _T("EMPB_SWITCH_NEXT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
int CEditModelListPaneBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CEditModelListPaneBar::BuildMoveTypeCombox()
{	
	CComboBox &Combox = m_SwitchModeCombox;
	AOIDataDefine.BuidlSwitchModelItemCombox(Combox);	
	JetAPI::SetComboxCurSel(Combox, SWITCH_MODEL_ITEM_ANY);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelListPaneBar::SetModelNameEdit(LPCTSTR str)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	CWnd::SetDlgItemText(EMPB_MODEL_NAME_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CEditModelListPaneBar::SetGroupNameEdit(LPCTSTR str)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	CWnd::SetDlgItemText(EMPB_GROUP_NAME_EDIT, str);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditModelListPaneBar::OnInitDialog(WPARAM wParam, LPARAM lParam) 
{
	//CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam, lParam);
	if ( !UpdateData(FALSE))//在此之後會自動建立控制項的物件
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
		return FALSE;
	}
	BuildMoveTypeCombox();
	SwitchMultiLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//