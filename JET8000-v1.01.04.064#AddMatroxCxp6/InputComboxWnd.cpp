// InputComboxWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "InputComboxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputComboxWnd dialog
//-------------------------------------------------------------------------------------//
CInputComboxWnd::CInputComboxWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CInputComboxWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputComboxWnd)
	m_TitleLabel1 = _T("");
	//}}AFX_DATA_INIT
	m_WndMovePos = false;
	m_WndPos.x = m_WndPos.y = 0;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputComboxWnd)
	DDX_Control(pDX, INPUTBOX_DATA_COMBO1, m_DataCombox1);
	DDX_Text(pDX, INPUTBOX_TITLE_LABEL1, m_TitleLabel1);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CInputComboxWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CInputComboxWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputComboxWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CInputComboxWnd::OnInitDialog() 
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
	UpdateData();
	BuildComboxCtrl();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::OnOK() 
{
	// TODO: Add extra validation here	
	m_SelData = TComboxNode();
	m_SelData.Data = -1;
	m_SelData.Ptr  = NULL;
	m_SelData.Text = _T("");

	CComboBox &Combox = m_DataCombox1;
	m_CurSel = Combox.GetCurSel();
	if ( m_CurSel >= 0 ) 
	{
		TComboxNode *NodePtr = NULL;	
		NodePtr = (TComboxNode*)(Combox.GetItemDataPtr(m_CurSel));
		if ( NULL != NodePtr )
		{	m_SelData = *NodePtr;	}
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SetWndPos(const POINT &Pos)
{
	m_WndPos = Pos;
	m_WndMovePos = true;
}
//-------------------------------------------------------------------------------------//
int CInputComboxWnd::GetSelIndex1() const
{
	return m_CurSel;
}
//-------------------------------------------------------------------------------------//
TComboxNode&  CInputComboxWnd::GetSelNode()
{
	return m_SelData;
}
//-------------------------------------------------------------------------------------//
void* CInputComboxWnd::GetSelPtr() const
{
	return m_SelData.Ptr;
}
//-------------------------------------------------------------------------------------//
DWORD_PTR CInputComboxWnd::GetSelData() const
{
	return m_SelData.Data;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CInputComboxWnd::GetSelText() const
{
	return m_SelData.Text;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SetSelIndex1(int nSel)
{
	m_CurSel = nSel;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, void* Default, const std::vector<TComboxNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, LPCTSTR Default, const std::vector<TComboxNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SetParam1(LPCTSTR WndTxt, LPCTSTR Title, DWORD_PTR Default, const std::vector<TComboxNode> &DataList)
{
	m_WndText = WndTxt;
	m_TitleLabel1 = Title;	
	m_CurSel = SearchIndex(Default, DataList);
	m_DataList = DataList;
}
//-------------------------------------------------------------------------------------//
bool CInputComboxWnd::BuildComboxCtrl()
{
	CComboBox &Combox = m_DataCombox1;
	JetAPI::ClearCombox(Combox);

	size_t       i=0;	
	int          Index=0;
	TComboxNode *NodePtr=NULL;
	const size_t Count = m_DataList.size();

	Index=0;
	for ( i=0; i<Count; i++ )
	{
		NodePtr = &(m_DataList[i]);
		Combox.InsertString(-1, NodePtr->Text);		
		Combox.SetItemDataPtr(Index, NodePtr);
		Index ++;
	}

	if ( m_CurSel>=0 && m_CurSel<Index ) 
	{	Combox.SetCurSel(m_CurSel);	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CInputComboxWnd::SearchIndex(void *Ptr, const std::vector<TComboxNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;		
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TComboxNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Ptr != Ptr ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
int CInputComboxWnd::SearchIndex(LPCTSTR Text, const std::vector<TComboxNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;		
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TComboxNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Text.CompareNoCase(Text) != 0 ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
int CInputComboxWnd::SearchIndex(DWORD_PTR Data, const std::vector<TComboxNode> &DataList)
{
	int          nSel = -1;
	size_t       i=0;	
	const size_t Count = DataList.size();

	for ( i=0; i<Count; i++ )
	{
		const TComboxNode *NodePtr = &(DataList[i]);
		if ( NodePtr->Data != Data ) { continue; }
		nSel = i;
		break;
	}
	return nSel;
}
//-------------------------------------------------------------------------------------//
void CInputComboxWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_INPUT_COMBO_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_INPUT_COMBO_WND;
	WndKey = _T("IDD_INPUT_COMBO_WND");
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