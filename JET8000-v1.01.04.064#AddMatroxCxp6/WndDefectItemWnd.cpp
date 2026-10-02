// WndDefectItemWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "WndDefectItemWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CWndDefectItemWnd dialog
//-------------------------------------------------------------------------------------//
CWndDefectItemWnd::CWndDefectItemWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CWndDefectItemWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWndDefectItemWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWndDefectItemWnd)
	DDX_Control(pDX, WDI_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, WDI_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, WDI_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CWndDefectItemWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CWndDefectItemWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_DBLCLK, WDI_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_NOTIFY(NM_KILLFOCUS, WDI_PARAM_LIST_WND, OnKillfocusParamListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, WDI_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_EN_KILLFOCUS(WDI_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(WDI_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(WDI_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(WDI_ENABLE_ALL_BTN, OnEnableAllBtn)
	ON_BN_CLICKED(WDI_DISABLE_ALL_BTN, OnDisableAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CWndDefectItemWnd message handlers
BOOL CWndDefectItemWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();	
	BuildParamListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return ; }
	if ( m_ParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	int nRight = cx;
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;
	std::vector<UINT> CtrlIDList_R;

	CtrlIDList_R.push_back(IDOK);
	CtrlIDList_R.push_back(IDCANCEL);
	CtrlIDList_R.push_back(WDI_ENABLE_ALL_BTN);
	CtrlIDList_R.push_back(WDI_DISABLE_ALL_BTN);
	const size_t CtrlIDCount_R = CtrlIDList_R.size();
	
	for ( size_t i=0; i<CtrlIDCount_R; i++ )
	{
		UINT CtrlID = CtrlIDList_R[i];
		WndPtr = CWnd::GetDlgItem(CtrlID);
		if ( NULL == WndPtr ) { continue; }
		if ( NULL == WndPtr->GetSafeHwnd() ) { continue; }
		
		RECT Rect;
		WndPtr->GetWindowRect(&Rect);
		CWnd::ScreenToClient(&Rect);		
		int hW=Rect.right-Rect.left;
		Rect.right = cx-4;
		Rect.left=Rect.right-hW;
		WndPtr->MoveWindow(&Rect, TRUE);
		if ( nRight > Rect.left )
		{	nRight = Rect.left;	}
	}
	nRight -= MarginX;	

	WndPtr = CWnd::GetDlgItem(WDI_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = nRight;//cx;		
		WndRect.bottom = cy-MarginY;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect, TRUE);
		InfoRect = WndRect;
	}
	else
	{
		InfoRect.left = 0;	InfoRect.right = nRight;
		InfoRect.top = cy; InfoRect.bottom = cy;
	}

	if ( m_ParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		WndRect.left = MarginX;
		WndRect.right = nRight-MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_ParamListCtrl.MoveWindow(&WndRect, TRUE);
		//if ( TRUE == bVisible )
		//{	m_ParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	//if ( TRUE == bShow )
	//{	BuildParamListWnd();	}
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 420;
	lpMMI->ptMinTrackSize.y = 360;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnOK() 
{
	// TODO: Add extra validation here	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnCancel() 
{
	// TODO: Add extra cleanup here	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_WND_DEFECT_ITEM_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(WDI_ENABLE_ALL_BTN));
	SetMultiLanauage(LoadIDAndName(WDI_DISABLE_ALL_BTN));
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{	
	LPCTSTR Section=_T("IDD_WND_DEFECT_ITEM_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CWndDefectItemWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_WND_DEFECT_ITEM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
const CWndDefectItem& CWndDefectItemWnd::GetWndDefectItem() const
{
	return m_WndDefectItem;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::SetWndDefectItem(const CWndDefectItem &DefectItem)
{
	m_WndDefectItem = DefectItem;
}
//-------------------------------------------------------------------------------------//
CParamUni* CWndDefectItemWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::BuildParamList()
{
	CParamList &ParamList = m_ParamList;	
	CThisListCtrl_71 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;	
	
	CString strValue;
	CString strCaption;
	CString strDescription;
	WND_DEFECT_ID WndDefectID;
	const CWndDefectItem &DefectItem = m_WndDefectItem;
	const std::vector<WND_DEFECT_ID> &DefectIDList=AOIDataCollect.GetWndDefectIDList();
	const size_t DefectIDCount=DefectIDList.size();

	ParamList.clear();	
	for ( size_t i=0; i<DefectIDCount; i++ )
	{	
		CParamUni ParamUnit;
		WndDefectID = DefectIDList[i];		
		const int WndDefectEnable = DefectItem.GetItemCount(WndDefectID);
		strCaption = AOIDataDefine.GetWndDefectIDText(WndDefectID);
		ParamUnit.SetCaption(strCaption);	
		ParamUnit.SetParamID(WndDefectID);	
		strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_ENABLE);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_ENABLE, strValue);
		strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_DISABLE);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_DISABLE, strValue);	
		//strValue = AOIDataDefine.GetWndDefectItemModeText(WND_DEFECT_ITEM_NO_SHOW);	ParamUnit.AddSelItem(WND_DEFECT_ITEM_NO_SHOW, strValue);	
		strDescription.Format(_T("%s [ID:%04d]"), strCaption, WndDefectID);
		ParamUnit.SetValue_SEL(WndDefectEnable);	
		ParamUnit.SetDesction(strDescription);		
		ParamList.push_back(ParamUnit);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::BuildParamListWnd()
{	
	SetActParamUni(NULL);	
	CThisListCtrl_71 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;	

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;	
	
	BuildParamList();

	CParamList &ParamList = m_ParamList;
	const int ParamCount = (int)(ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		CParamUni &ParamUni = ParamList[i];		
		
		ParamUni.SetListCtrl(&ListCtrl);
		ParamUni.SetWndCtrlID(WDI_PARAM_LIST_WND);
		ParamUni.SetItemIndex(nItem);
		ParamUni.SetSubItemIndex(nSubItem1);		

		strCaption = ParamUni.GetCaption();
		strValue = ParamUni.GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		//ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem1, strValue);
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_71 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-48)/2;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CWndDefectItemWnd::OnKillfocusParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CWndDefectItemWnd::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnEnableAllBtn() 
{
	// TODO: Add your control notification handler code here
	m_WndDefectItem.SetAllItemCount(WND_DEFECT_ITEM_ENABLE);
	BuildParamListWnd();
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::OnDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	m_WndDefectItem.SetAllItemCount(WND_DEFECT_ITEM_DISABLE);
	BuildParamListWnd();
}
//-------------------------------------------------------------------------------------//
void CWndDefectItemWnd::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = WDI_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::ExecItemchangedParamListWnd(CThisListCtrl_71 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	CParamList &ParamList = m_ParamList;
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(ParamList[ParamIndex]);	
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
	//else
	//{	m_BtnCtrl.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::ExecDblclkParamListWnd(CThisListCtrl_71 &ListCtrl, int nItem, int nSubItem)
{	
	const int    SetCol = 1;
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SetCol ) { return true; }

	CParamList &ParamList = m_ParamList;
	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(ParamList[ParamIndex]);		
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
	//m_BtnCtrl.ShowWindow(SW_HIDE);
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
bool CWndDefectItemWnd::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItemWnd::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	UINT WndCtrlID = ParamPtr->GetWndCtrlID();
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

	WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(ParamPtr->GetParamID());
	m_WndDefectItem.SetItemCount(DefectID, Param);
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_71 *pListCtrl = (CThisListCtrl_71*)(ParamPtr->GetListCtrl());
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
bool CWndDefectItemWnd::ExecReleaseParamCtrl()
{	
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CWndDefectItemWnd::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CWndDefectItemWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//