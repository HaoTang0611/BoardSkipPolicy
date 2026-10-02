// PlcNodeListPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "PlcNodeListPane.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneNodeList dialog
//-------------------------------------------------------------------------------------//
CPLCCtrlPaneNodeList::CPLCCtrlPaneNodeList(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCCtrlPaneNodeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCCtrlPaneNodeList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCCtrlPaneNodeList)
	DDX_Control(pDX, PLCNODE_TEMP_LIST_WND, m_TempListWnd);
	DDX_Control(pDX, PLCNODE_NODE_LIST_WND, m_NodeListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPLCCtrlPaneNodeList, CDialog)
	//{{AFX_MSG_MAP(CPLCCtrlPaneNodeList)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(PLCNODE_ADD_PLC_NODE_BTN, OnAddPlcNodeBtn)
	ON_BN_CLICKED(PLCNODE_REMOVE_PLC_NODE_BTN, OnRemovePlcNodeBtn)
	ON_BN_CLICKED(PLCNODE_SET_NODE_LIST_BTN, OnSetNodeListBtn)
	ON_BN_CLICKED(PLCNODE_SAVE_PLC_NODE_BTN, OnSavePlcNodeBtn)
	ON_BN_CLICKED(PLCNODE_RESTORE_PLC_NODE_BTN, OnRestorePlcNodeBtn)
	ON_BN_CLICKED(PLCNODE_GET_NODE_LIST_BTN, OnGetNodeListBtn)
	ON_BN_CLICKED(PLCNODE_PLC_NODE_READ_BTN, OnPlcNodeReadBtn)
	ON_BN_CLICKED(PLCNODE_PLC_NODE_WRITE_BTN, OnPlcNodeWriteBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CPLCCtrlPaneNodeList message handlers
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneNodeList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(this->m_NodeListWnd);
	JetAPI::InitialListCtrl(this->m_TempListWnd);
	
	CString   str;
	const int width = 32;
	str = _T("Idx");
	str = LoadMultiLanguageString(str, str);
	this->m_NodeListWnd.InsertColumn(0, str, LVCFMT_CENTER, width);
	str = _T("Address");
	str = LoadMultiLanguageString(str, str);
	this->m_NodeListWnd.InsertColumn(1, str, LVCFMT_CENTER, width*2);
	str = _T("Value");
	str = LoadMultiLanguageString(str, str);
	this->m_NodeListWnd.InsertColumn(2, str, LVCFMT_CENTER, width*2);
	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	this->m_NodeListWnd.InsertColumn(3, str, LVCFMT_CENTER, width*4);

	str = _T("Idx");
	str = LoadMultiLanguageString(str, str);
	this->m_TempListWnd.InsertColumn(0, str, LVCFMT_CENTER, width);
	str = _T("Address");
	str = LoadMultiLanguageString(str, str);
	this->m_TempListWnd.InsertColumn(1, str, LVCFMT_CENTER, width*2);	
	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	this->m_TempListWnd.InsertColumn(2, str, LVCFMT_CENTER, width*4);
	str = _T("Removed");
	str = LoadMultiLanguageString(str, str);
	this->m_TempListWnd.InsertColumn(3, str, LVCFMT_CENTER, width*2);

	PlcCtrlPtr->GetPlcNodeList(m_PLCNodeList);	
	m_PLCNodeListTemp = m_PLCNodeList;

	this->SwitchMultiLanguage();
	this->BuildPlcNodeListWnd();
	this->BuildPlcTempListWnd();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		SwitchMultiLanguage();
		PlcCtrlPtr->SetReadAllPlcNode();
		this->SetTimer(PLC_NODE_LIST_TIMER, 1000, 0);	
	}
	else
	{
		PlcCtrlPtr->SetReadFnCodePlcNode();
		this->KillTimer(PLC_NODE_LIST_TIMER);	
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case PLC_NODE_LIST_TIMER:
		this->UpdatePlcNodeListWnd();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::BuildPlcNodeListWnd()
{
	this->m_NodeListWnd.DeleteAllItems();

	CString      str;
	size_t       i=0;
	int          idx=0;
	TPlcNode    *PlcNodePtr = NULL;
	const size_t NodeCount = this->m_PLCNodeList.size();

	idx = 0;
	this->m_NodeListWnd.SetRedraw(FALSE);
	for ( i=0; i<NodeCount; i++ )
	{
		PlcNodePtr = &(m_PLCNodeList[i]);
		if ( PlcNodePtr == NULL ) { continue; }

		str.Format(_T("%d"), i+1);
		this->m_NodeListWnd.InsertItem(idx, str);
		this->m_NodeListWnd.SetItemData(idx, i);

		str = PlcNodePtr->m_Address;
		this->m_NodeListWnd.SetItemText(idx, 1, str);

		str.Format(_T("%d"), PlcNodePtr->m_Value);
		this->m_NodeListWnd.SetItemText(idx, 2, str);

		str = PlcNodePtr->m_Name;
		this->m_NodeListWnd.SetItemText(idx, 3, str);

		idx ++;
	}
	this->m_NodeListWnd.SetRedraw(TRUE);
	//this->m_NodeListWnd.Invalidate();
	//this->m_NodeListWnd.UpdateWindow();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::BuildPlcTempListWnd()
{
	this->m_TempListWnd.DeleteAllItems();

	CString      str;
	size_t       i=0;
	int          idx=0;
	TPlcNode    *PlcNodePtr = NULL;
	const size_t NodeCount = this->m_PLCNodeListTemp.size();

	idx = 0;
	this->m_TempListWnd.SetRedraw(FALSE);
	for ( i=0; i<NodeCount; i++ )
	{
		PlcNodePtr = &(m_PLCNodeListTemp[i]);
		if ( PlcNodePtr == NULL ) { continue; }

		str.Format(_T("%d"), i+1);
		this->m_TempListWnd.InsertItem(idx, str);
		this->m_TempListWnd.SetItemData(idx, i);

		str = PlcNodePtr->m_Address;
		this->m_TempListWnd.SetItemText(idx, 1, str);

		str = PlcNodePtr->m_Name;
		this->m_TempListWnd.SetItemText(idx, 2, str);

		if ( PlcNodePtr->m_Deleted == true )
		{	str = _T("Y"); }
		else
		{	str = _T(""); }
		this->m_TempListWnd.SetItemText(idx, 3, str);

		idx ++;
	}
	this->m_TempListWnd.SetRedraw(TRUE);
	//this->m_TempListWnd.Invalidate();
	//this->m_TempListWnd.UpdateWindow();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnAddPlcNodeBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;
	CString      strAddress;
	TPlcNode     PlcNode;
	CInputBoxWnd InputBox;

	this->GetDlgItemText(PLCNODE_PLC_NODE_ADDRESS_EDIT, strAddress);
	strAddress.MakeUpper();
	PlcNode.SetAddress(strAddress);
	if ( CPLC_Basic::CheckPlcNodeAddress(PlcNode.m_Address) == false )	
	{
		str.Format(_T("Error, Address Exception (%s)"), InputBox.m_DataEdit1);
		JetAPI::ShowMessageBox(str);
		return;
	}

	InputBox.SetParam1(_T("New PLC Node"), _T("Name"), _T(""));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	PlcNode.m_Name = InputBox.m_DataEdit1;	

	PlcNode.m_ToRead = true;

	const int NodeIndex = (int)(this->m_PLCNodeListTemp.size());
	this->m_PLCNodeListTemp.push_back(PlcNode);

	const int ItemIndex = this->m_TempListWnd.GetItemCount();
	str.Format(_T("%d"), ItemIndex+1);
	this->m_TempListWnd.InsertItem(ItemIndex, str);
	this->m_TempListWnd.SetItemData(ItemIndex, NodeIndex);
	str = PlcNode.m_Address;
	this->m_TempListWnd.SetItemText(ItemIndex, 1, str);	
	str = PlcNode.m_Name;
	this->m_TempListWnd.SetItemText(ItemIndex, 2, str);
	str = _T("");
	this->m_TempListWnd.SetItemText(ItemIndex, 3, str);
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnRemovePlcNodeBtn() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int ItemIndex = 0;	
	size_t NodeIndex = 0;
	TPlcNode *PlcNodePtr = NULL;
	const size_t NodeCount = (size_t)(this->m_PLCNodeListTemp.size());

	POSITION pos = this->m_TempListWnd.GetFirstSelectedItemPosition();
	if ( pos == NULL ) { return; }

	while ( pos!=NULL )
	{
		ItemIndex = this->m_TempListWnd.GetNextSelectedItem(pos);
		if ( ItemIndex < 0 ) { continue; }
		NodeIndex = this->m_TempListWnd.GetItemData(ItemIndex);		
		if ( NodeIndex >= NodeCount ) { continue; }
		PlcNodePtr = &m_PLCNodeListTemp[NodeIndex];	
		if ( PlcNodePtr->m_FnCode != 0 ) { continue; }  
		PlcNodePtr->m_Deleted = true;
		this->m_TempListWnd.SetItemText(ItemIndex, 3, _T("Y"));
   };
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnSetNodeListBtn() 
{
	// TODO: Add your control notification handler code here
	size_t       i = 0;
	TPlcNode    *PlcNodePtr=NULL;
	std::vector<TPlcNode>      PLCNodeList=m_PLCNodeListTemp;
	const size_t NodeCount = PLCNodeList.size();
	
	this->m_PLCNodeListTemp.clear();
	for ( i=0; i<NodeCount; i++ )
	{
		PlcNodePtr = &(PLCNodeList[i]);
		if ( PlcNodePtr == NULL ) { continue; }
		if ( PlcNodePtr->m_Deleted == true ) { continue; }

		m_PLCNodeListTemp.push_back(PLCNodeList[i]);
	}

	PlcCtrlPtr->SetPlcNodeList(m_PLCNodeListTemp);	
	PlcCtrlPtr->GetPlcNodeList(m_PLCNodeList);		

	this->BuildPlcNodeListWnd();
	this->BuildPlcTempListWnd();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnSavePlcNodeBtn() 
{
	// TODO: Add your control notification handler code here
	if ( PlcCtrlPtr->SaveExtraPlcNodeList() == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString()); }
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnRestorePlcNodeBtn() 
{
	// TODO: Add your control notification handler code here
	int ItemIndex = 0;	
	size_t NodeIndex = 0;
	TPlcNode *PlcNodePtr = NULL;
	const size_t NodeCount = (size_t)(this->m_PLCNodeListTemp.size());

	POSITION pos = this->m_TempListWnd.GetFirstSelectedItemPosition();
	if ( pos == NULL ) { return; }

	while ( pos!=NULL )
	{
		ItemIndex = this->m_TempListWnd.GetNextSelectedItem(pos);
		if ( ItemIndex < 0 ) { continue; }
		NodeIndex = this->m_TempListWnd.GetItemData(ItemIndex);		
		if ( NodeIndex >= NodeCount ) { continue; }
		PlcNodePtr = &m_PLCNodeListTemp[NodeIndex];	
		if ( PlcNodePtr->m_FnCode != 0 ) { continue; }  
		PlcNodePtr->m_Deleted = false;
		this->m_TempListWnd.SetItemText(ItemIndex, 3, _T(""));
   };
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnGetNodeListBtn() 
{
	// TODO: Add your control notification handler code here
	m_PLCNodeListTemp = m_PLCNodeList;	
	this->BuildPlcTempListWnd();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::UpdatePlcNodeListWnd()
{
	PlcCtrlPtr->GetPlcNodeList(m_PLCNodeList);

	const int NodeCount = (int)(m_PLCNodeList.size());
	const int ItemCount = (int)(m_NodeListWnd.GetItemCount());

	if ( NodeCount != ItemCount ) 
	{	
		this->BuildPlcNodeListWnd();	
		return;
	}	

	int     i = 0;
	int     idx = 0;
	CString str;
	int     ItemIndex = 0;	
	int     NodeIndex = 0;

	TPlcNode *PlcNodePtr = NULL;	
	this->m_NodeListWnd.SetRedraw(false);
	idx = 0;
	for ( i=0; i<ItemCount; i++ )
	{
		NodeIndex = (int)(this->m_NodeListWnd.GetItemData(i));
		if ( (NodeIndex<0) || (NodeIndex>=NodeCount) ) { continue; }

		PlcNodePtr = &(m_PLCNodeList[NodeIndex]);

	//	str.Format(_T("%d"), i+1);
	//	this->m_NodeListWnd.SetItemText(idx, 0, str);

		str = PlcNodePtr->m_Address;
		this->m_NodeListWnd.SetItemText(idx, 1, str);

		str.Format(_T("%d"), PlcNodePtr->m_Value);
		this->m_NodeListWnd.SetItemText(idx, 2, str);

		str = PlcNodePtr->m_Name;
		this->m_NodeListWnd.SetItemText(idx, 3, str);		
		idx ++;
	}
	this->m_NodeListWnd.SetRedraw(true);
	this->m_NodeListWnd.Invalidate();
	this->m_NodeListWnd.UpdateWindow();
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnPlcNodeReadBtn() 
{
	// TODO: Add your control notification handler code here	
	CString  str;	
	int      value = 0;	
	CString  strAddress;	
	TPlcNode PlcNode;
	this->SetDlgItemText(PLCNODE_PLC_NODE_VALUE_EDIT, _T(""));
	this->GetDlgItemText(PLCNODE_PLC_NODE_ADDRESS_EDIT, strAddress);

	PlcNode.SetAddress(strAddress);
	if ( CPLC_Basic        ::CheckPlcNodeAddress(PlcNode.m_Address) == false )
	{
		str.Format(_T("Error, PLC Node Address Exception (%s)"), strAddress);
		JetAPI::ShowMessageBox(str);
		return;
	}

	PlcCtrlPtr->StopPLCPollingThread();
	DWORD TickCount=GetTickCount();
	const bool IsOK = PlcCtrlPtr->PLC_ReadNode(PlcNode.m_Address, value);	
	TickCount = GetTickCount()-TickCount;
	PlcCtrlPtr->StartPLCPollingThread();

	this->SetDlgItemInt(PLCNODE_PLC_NODE_VALUE_EDIT, value);
	str.Format(_T("Time=%u"), TickCount);
	this->SetDlgItemText(PLCNODE_PLC_NODE_INFO_EDIT, str);

	if ( false == IsOK )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::OnPlcNodeWriteBtn() 
{
	// TODO: Add your control notification handler code here
	CString  str;	
	int      value = 0;	
	CString  strAddress;	
	TPlcNode PlcNode;

	this->GetDlgItemText(PLCNODE_PLC_NODE_ADDRESS_EDIT, strAddress);	
	value = (int)(this->GetDlgItemInt(PLCNODE_PLC_NODE_VALUE_EDIT));
	PlcNode.SetAddress(strAddress);
	if ( CPLC_Basic        ::CheckPlcNodeAddress(PlcNode.m_Address) == false )
	{
		str.Format(_T("Error, PLC Node Address Exception (%s)"), strAddress);
		JetAPI::ShowMessageBox(str);
		return;
	}

	PlcCtrlPtr->StopPLCPollingThread();
	DWORD TickCount=GetTickCount();
	const bool IsOK = PlcCtrlPtr->PLC_WriteNode(PlcNode.m_Address, value);	
	TickCount = GetTickCount()-TickCount;
	PlcCtrlPtr->StartPLCPollingThread();

	str.Format(_T("Time=%u"), TickCount);
	this->SetDlgItemText(PLCNODE_PLC_NODE_INFO_EDIT, str);

	if ( false == IsOK )
	{
		str = PlcCtrlPtr->GetPLCErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CPLCCtrlPaneNodeList::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PLC_NODE_LIST_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PLC_NODE_LIST_PANE;
	WndKey = _T("IDD_PLC_NODE_LIST_PANE");
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
	WndID = PLCNODE_SET_NODE_LIST_BTN;
	WndKey = _T("PLCNODE_SET_NODE_LIST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PLCNODE_GET_NODE_LIST_BTN;
	WndKey = _T("PLCNODE_GET_NODE_LIST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_REMOVE_PLC_NODE_BTN;
	WndKey = _T("PLCNODE_REMOVE_PLC_NODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_RESTORE_PLC_NODE_BTN;
	WndKey = _T("PLCNODE_RESTORE_PLC_NODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_SAVE_PLC_NODE_BTN;
	WndKey = _T("PLCNODE_SAVE_PLC_NODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_PLC_NODE_ADDRESS_LABEL;
	WndKey = _T("PLCNODE_PLC_NODE_ADDRESS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_PLC_NODE_READ_BTN;
	WndKey = _T("PLCNODE_PLC_NODE_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_PLC_NODE_WRITE_BTN;
	WndKey = _T("PLCNODE_PLC_NODE_WRITE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PLCNODE_ADD_PLC_NODE_BTN;
	WndKey = _T("PLCNODE_ADD_PLC_NODE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = AAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = AAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CPLCCtrlPaneNodeList::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PLC_NODE_LIST_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
BOOL CPLCCtrlPaneNodeList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:			
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//