// EditPartNumberListPaneBar.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditPartNumberListPaneBar.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditPartNumberListPaneBar dialog
//-------------------------------------------------------------------------------------//
CEditPartNumberListPaneBar::CEditPartNumberListPaneBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CEditPartNumberListPaneBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListPaneBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditPartNumberListPaneBar)
	DDX_Control(pDX, EPNB_SWITCH_MODE_COMBO, m_SwitchModeCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditPartNumberListPaneBar, CDialogBar)
	//{{AFX_MSG_MAP(CEditPartNumberListPaneBar)
	ON_MESSAGE(WM_INITDIALOG , OnInitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditPartNumberListPaneBar message handlers
void CEditPartNumberListPaneBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	CWnd *pWnd = CWnd::GetParent();
	CDialogBar::OnUpdateCmdUI((CFrameWnd*)pWnd, bDisableIfNoHandler);
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListPaneBar::SwitchMultiLanguage()
{	
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_PART_NUMBER_LIST_PANE_BAR");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_PART_NUMBER_LIST_PANE_BAR;
	WndKey = _T("IDD_EDIT_PART_NUMBER_LIST_PANE_BAR");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = EPNB_SWITCH_PREVIOUS_BTN;
	WndKey = _T("EPNB_SWITCH_PREVIOUS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = EPNB_SWITCH_NEXT_BTN;
	WndKey = _T("EPNB_SWITCH_NEXT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
bool CEditPartNumberListPaneBar::BuildMoveTypeCombox()
{	
	CComboBox &Combox = m_SwitchModeCombox;
	AOIDataDefine.BuidlSwitchModelItemCombox(Combox);	
	JetAPI::SetComboxCurSel(Combox, SWITCH_MODEL_ITEM_ANY);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditPartNumberListPaneBar::SetPartNumberNameEdit(LPCTSTR str)
{
	CWnd::SetDlgItemText(EPNB_PART_NUMBER_NAME_EDIT, str);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditPartNumberListPaneBar::OnInitDialog(WPARAM wParam, LPARAM lParam) 
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