// SystemHASIPane.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "SystemHASIPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL = 2;//設定的欄位
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemHASIPane dialog
//-------------------------------------------------------------------------------------//
CSystemHASIPane::CSystemHASIPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemHASIPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemHASIPane)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemHASIPane)
	DDX_Control(pDX, SYSM2M_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSM2M_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSM2M_PARAM_COMBO, m_ComboxCtrl);
	DDX_Control(pDX, SYSM2M_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemHASIPane, CDialog)
	//{{AFX_MSG_MAP(CSystemHASIPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSM2M_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSM2M_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSM2M_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(SYSM2M_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(SYSM2M_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(SYSM2M_PARAM_BTN, OnParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemM2MPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemHASIPane::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if (CWnd::IsWindowVisible())
	{
		BuildParamListWnd();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_ParamListCtrl.GetSafeHwnd() == NULL) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize = { 0 };
	RECT  InfoRect = { 0 };
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	WndPtr = CWnd::GetDlgItem(SYSM2M_INFO_EDIT);
	if (NULL != WndPtr && WndPtr->GetSafeHwnd() != NULL)
	{
		RECT WndRect = { 0 };
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = cx;
		WndRect.bottom = cy - MarginY;
		WndRect.top = WndRect.bottom - WndSize.cy;
		WndPtr->MoveWindow(&WndRect, FALSE);
		InfoRect = WndRect;
	}
	else
	{
		InfoRect.left = 0;	InfoRect.right = cx;
		InfoRect.top = cy; InfoRect.bottom = cy;
	}

	if (m_ParamListCtrl.GetSafeHwnd() != NULL)
	{
		RECT WndRect = { 0 };
		WndRect.left = MarginX;
		WndRect.right = cx - MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top - MarginY;
		m_ParamListCtrl.MoveWindow(&WndRect, FALSE);
		if (TRUE == bVisible)
		{
			m_ParamListCtrl.Invalidate();
		}
	}
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (TRUE == bShow)
	{
		BuildParamListWnd();
	}
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnOK()
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnCancel()
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CSystemHASIPane::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (m_EditCtrl.IsWindowVisible() == TRUE)
			{
				if (ExecUpdateParamByEdit() == true)
				{
					m_EditCtrl.ShowWindow(SW_HIDE);
					m_EditCtrl.SetWindowText(_T(""));
					return TRUE;
				}
			}
			if (m_ComboxCtrl.IsWindowVisible() == TRUE)
			{
				if (ExecUpdateParamByCombox() == true)
				{
					m_ComboxCtrl.ShowWindow(SW_HIDE);
					JetAPI::ClearCombox(m_ComboxCtrl);
					return TRUE;
				}
			}
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CSystemHASIPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_SYSTEM_M2M_PANE), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section = _T("IDD_SYSTEM_HASI_PANE");
	if (AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false)
	{
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CSystemHASIPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section = _T("IDD_SYSTEM_HASI_PANE");
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemHASIPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::BuildParamList()
{
	CThisListCtrl_64 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if (NULL == m_SysParameterPtr) { return true; }

	int       i = 0;
	int       intValue = 0;
	CString   str;
	CString   strValue;
	CString   strCaption;
	CString   strDescription;
	int       nItem = 0;
	CParamUni    ParamUnit;
	SYSTEM_PARAM_ID ParamID;
	TSystemParameter *Ptr = m_SysParameterPtr;
	const int nSubItem = 1;
	CParamList &ParamList = m_ParamList;

	ParamList.clear();


	//啟用HASI
	ParamUnit = CParamUni();
	str = _T("Use M2M HAS I");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));

	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_Enable);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);


	//HAS I 共享資料夾
	ParamUnit = CParamUni();
	str = _T("Share Folder");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	//ParamID = SYSTEM_M2M_HASI_SHARE_FOLDER;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(Ptr->m_M2M_HASI_ShareFolder);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamList.push_back(ParamUnit);

	/*
	//啟用Serial File
	ParamUnit = CParamUni();
	str = _T("Use Serial File");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_SERIALFILE_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_SerialFile_Enable);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//SerialFile- Size of Serial Queue 
	ParamUnit = CParamUni();
	str = _T("Serial File Queue Size");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_SERIALFILE_QUEUESIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_M2M_HASI_SerialFile_QueueSize);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//等待資料的延遲時間-ms
	ParamUnit = CParamUni();
	str = _T("Serial Timeout (ms)");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_SERIALFILE_DWELLTIME;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_M2M_HASI_SerialFile_DwellTime);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//啟用SPIOffsetFile
	ParamUnit = CParamUni();
	str = _T("Use sco File");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_SPIOFFSETFILE_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_SPIOffsetFile_Enable);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	*/
	//啟用PNP-Hanwha
	ParamUnit = CParamUni();
	str = _T("PnP Enable");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_PNP_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);		ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);		ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_PNP_Enable);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//產線上 AOI 的檢查階段
	ParamUnit = CParamUni();
	str = _T("AOI Stage");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_AOI_STAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	strValue = AOIDataDefine.GetHASIStageText(HASI_AOI_STAGE_NONE); ParamUnit.AddSelItem(HASI_AOI_STAGE_NONE, strValue);
	//strValue = AOIDataDefine.GetHASIStageText(HASI_AOI_STAGE_PRE); ParamUnit.AddSelItem(HASI_AOI_STAGE_PRE, strValue);
	strValue = AOIDataDefine.GetHASIStageText(HASI_AOI_STAGE_POST); ParamUnit.AddSelItem(HASI_AOI_STAGE_POST, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_AOI_Stage);
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//產線上 AOI 的檢查階段
	ParamUnit = CParamUni();
	str = _T("State Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = SYSTEM_M2M_HASI_STATE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	str = AOIDataDefine.GetHASIStateModeText(Ptr->m_M2M_HASI_StateMode);
	ParamUnit.SetValue_STR(str);
	ParamUnit.SetReadOnly(true);
	//if (Ptr->m_M2M_HASI_StateMode!= HASI_STATE_MODE_STOP && Ptr->m_M2M_HASI_StateMode != HASI_STATE_MODE_TEST) {
	//	str = AOIDataDefine.GetHASIStateModeText(Ptr->m_M2M_HASI_StateMode);
	//	ParamUnit.SetValue_STR(str);
	//	ParamUnit.SetReadOnly(true);
	//}
	//else {
	//	strValue = AOIDataDefine.GetHASIStateModeText(HASI_STATE_MODE_STOP); ParamUnit.AddSelItem(HASI_STATE_MODE_STOP, strValue);
	//	strValue = AOIDataDefine.GetHASIStateModeText(HASI_STATE_MODE_TEST); ParamUnit.AddSelItem(HASI_STATE_MODE_TEST, strValue);
	//	ParamUnit.SetValue_SEL(Ptr->m_M2M_HASI_StateMode);
	//	ParamUnit.SetReadOnly(false);
	//}
	strDescription = CAOIDataCollect::ObtainSystemParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_64 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if (NULL == m_SysParameterPtr) { return true; }

	int           i = 0;
	CString       str;
	CString       strIndex;
	CString       strValue;
	CString       strCaption;
	int           nItem = 0;
	CParamUni    *ParamPtr = NULL;
	const int     nSubItem1 = 1;
	const int     nSubItem2 = 2;

	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for (i = 0; i<ParamCount; i++)
	{
		ParamPtr = &(m_ParamList[i]);
		if (NULL == ParamPtr) { continue; }

		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);

		strIndex.Format(_T("%d"), nItem + 1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem++;
	}
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width = 64;
	int width2 = 64;
	RECT      Rect;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_64 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right - Rect.left - 8) / 8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol++;

	width2 = width * 3;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol++;

	width2 = width * 5;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (true == m_StopParamListBeSelected) { return; }
	const int nItem = pNMListView->iItem;
	const int nSubItem = pNMListView->iSubItem;
	if (nItem < 0)
	{
		SetDescriptionText(NULL);
		return;
	}
	DWORD Res = 0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;
	if (0 != ResOld && 0 == ResNew)
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if (Res == 0) { return; }
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if (Res == 0) { return; }
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;
	const int nItem = pNMListView->iItem;
	const int nSubItem = pNMListView->iSubItem;
	if (nItem < 0) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnKillfocusParamEdit()
{
	// TODO: Add your control notification handler code here
	if (m_EditCtrl.IsWindowVisible() == FALSE) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnSelchangeParamCombo()
{
	// TODO: Add your control notification handler code here
	if (ExecUpdateParamByCombox() == false)
	{
		return;
	}
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	JetAPI::ClearCombox(m_ComboxCtrl);
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnKillfocusParamCombo()
{
	// TODO: Add your control notification handler code here
	if (m_ComboxCtrl.IsWindowVisible() == FALSE) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	JetAPI::ClearCombox(m_ComboxCtrl);
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::OnParamBtn()
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if (NULL == ParamPtr) { return; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if (NULL == BtnWndPtr) { return; }

	CString ItemText = ParamPtr->GetParamText();
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());

	if (JetAPI::OpenFolderDialog(this, ItemText) == false) { return; }
	if (CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false)
	{
		return;
	}
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);

	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();
	CThisListCtrl_64 *pListCtrl = (CThisListCtrl_64*)(ParamPtr->GetListCtrl());

	if (NULL != pListCtrl)
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CSystemHASIPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSM2M_INFO_EDIT;
	if (NULL == Ptr)
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::ExecItemchangedParamListWnd(CThisListCtrl_64 &ListCtrl, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if (nItem<0 || nItem >= ItemCount) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if (ParamIndex >= ParamCount) { return false; }
	CParamUni *ParamPtr = &(m_ParamList[ParamIndex]);
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	const int nSubItem = ParamPtr->GetSubItemIndex();

	SetDescriptionText(ParamPtr);
	if (NULL != BtnWndPtr && BtnWndPtr->GetSafeHwnd() != NULL)
	{
		CRect ItemRect;
		if (ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == TRUE)
		{
			SIZE BtnSize = { 0 };
			RECT BtnRect = { 0 };
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;
			BtnRect.left = BtnRect.right - BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);
			BtnWndPtr->ShowWindow(SW_SHOW);
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);
		}
	}
	else
	{
		m_BtnCtrl.ShowWindow(SW_HIDE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::ExecDblclkParamListWnd(CThisListCtrl_64 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if (nItem<0 || nItem >= ItemCount) { return false; }
	if (nSubItem < SETTING_COL) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if (ParamIndex >= ParamCount) { return false; }

	size_t          i = 0;
	int             nSelIdx = 0;
	int             nValue = 0;
	CRect           ItemRect;
	RECT            CtrlRect = { 0 };
	CString         ItemText;
	const int       Offset = 2;
	CParamUni      *ParamPtr = &(m_ParamList[ParamIndex]);
	const bool      ReadOnly = ParamPtr->GetReadOnly();
	if (true == ReadOnly) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if (ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE)
	{
		return false;
	}
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	m_BtnCtrl.ShowWindow(SW_HIDE);
	SetActParamUni(ParamPtr);
	if (PARAM_DATA_SEL == DataType)
	{
		if (m_ComboxCtrl.GetSafeHwnd() != NULL)
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);
			for (i = 0; i<SelItemCount; i++)
			{
				if (ParamPtr->GetSelItem(i, true, nValue, ItemText) == false) { continue; }
				m_ComboxCtrl.InsertString(nSelIdx, ItemText);
				m_ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx++;
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
		if (m_EditCtrl.GetSafeHwnd() != NULL)
		{
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);
			m_EditCtrl.SetWindowText(ItemText);
			m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
			m_EditCtrl.SetFocus();
			m_EditCtrl.SetSel(0, -1);
			m_EditCtrl.ShowWindow(SW_SHOW);
			m_EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			m_EditCtrl.Invalidate();
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if (NULL == ParamPtr) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if (PARAM_DATA_SEL == DataType) { return true; }
	SetActParamUni(NULL);

	CString ItemText;
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());
	m_EditCtrl.GetWindowText(ItemText);
	if (ParamPtr->SetNewValue(ItemText) == false)
	{
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if (CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false)
	{
		return false;
	}

	CThisListCtrl_64 *pListCtrl = (CThisListCtrl_64*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();
	if (NULL != pListCtrl)
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if (nItem >= 0 && nItem<ItemCount)
		{
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);
			pListCtrl->SetFocus();
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if (NULL == ParamPtr) { return true; }
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if (PARAM_DATA_SEL != DataType) { return true; }
	SetActParamUni(NULL);

	CString ItemText;
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if (nCurSel < 0) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if (Param == ParamPtr->GetSelParam()) { return false; }
	ItemText.Format(_T("%d"), Param);
	if (ParamPtr->SetNewValue_SEL(Param) == false)
	{
		return false;
	}
	if (CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false)
	{
		return false;
	}

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_64 *pListCtrl = (CThisListCtrl_64*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();
	if (NULL != pListCtrl)
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if (nItem<0 || nItem >= ItemCount) { return true; }
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemHASIPane::ExecReleaseParamCtrl()
{
	m_EditCtrl.ShowWindow(SW_HIDE);
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//