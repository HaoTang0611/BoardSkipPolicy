// SystemColorPane.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "SystemColorPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int SETTING_COL   = 2;//設定的欄位
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemColorPane dialog
//-------------------------------------------------------------------------------------//
CSystemColorPane::CSystemColorPane(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemColorPane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemColorPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ParamActPtr = NULL;
	m_SysParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemColorPane)
	DDX_Control(pDX, SYSCOLOR_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, SYSCOLOR_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, SYSCOLOR_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, SYSCOLOR_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemColorPane, CDialog)
	//{{AFX_MSG_MAP(CSystemColorPane)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, SYSCOLOR_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, SYSCOLOR_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(SYSCOLOR_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(SYSCOLOR_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(SYSCOLOR_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(SYSCOLOR_PARAM_BTN, OnParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemColorPane message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemColorPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	//CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_ParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	const int MarginX = 4;
	const int MarginY = 4;

	WndPtr = CWnd::GetDlgItem(SYSCOLOR_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.left = MarginX;
		WndRect.right = cx;		
		WndRect.bottom = cy-MarginY;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect, FALSE);
		InfoRect = WndRect;
	}
	else
	{
		InfoRect.left = 0;	InfoRect.right = cx;
		InfoRect.top = cy; InfoRect.bottom = cy;
	}

	if ( m_ParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_ParamListCtrl.MoveWindow(&WndRect, FALSE);
		if ( TRUE == bVisible )
		{	m_ParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CSystemColorPane::PreTranslateMessage(MSG* pMsg) 
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
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CSystemColorPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemColorPane::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CSystemColorPane::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText;
	COLORREF clr = (COLORREF)(ParamPtr->GetValue_CLR());
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());		
	CColorDialog Wnd(clr, CC_FULLOPEN|CC_RGBINIT);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }	
	clr = Wnd.GetColor();
	ItemText.Format(_T("%d"), clr);
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return ; }

	ParamPtr->SetNewValue(ItemText);	
	SetActParamUni(NULL);

	ItemText = ParamPtr->GetParamText();
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_31 *pListCtrl = (CThisListCtrl_31*)(ParamPtr->GetListCtrl());
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}		
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_COLOR_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_COLOR_PANE;
	WndKey = _T("IDD_SYSTEM_COLOR_PANE");
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
CString CSystemColorPane::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_COLOR_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::SetSystemParameterPtr(TSystemParameter *Ptr)
{
	m_SysParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CSystemColorPane::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::BuildParamList()
{
	CThisListCtrl_31 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	SYSTEM_PARAM_ID SysParam;
	TSystemParameter *Ptr = m_SysParameterPtr;
	const int nSubItem = 1;	
	const bool bReadOnly = true;
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//相位遮罩-低對比-顏色
	ParamUnit = CParamUni();
	str = _T("Phase Noise Low Contrast Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PHASE_NOISE_LOW_CONTRAST_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PhaseNoiseLowContrastColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);	

	//相位遮罩-潛力-顏色
	ParamUnit = CParamUni();
	str = _T("Phase Noise Low Potential Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PHASE_NOISE_LOW_POTENTIAL_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PhaseNoiseLowPotentialColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);	

	//相位遮罩-過亮度-顏色
	ParamUnit = CParamUni();
	str = _T("Phase Noise Over Saturated Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PHASE_NOISE_OVER_SATURATED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PhaseNoiseOverSaturatedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);	

	//相位遮罩-無效點外擴-顏色
	ParamUnit = CParamUni();
	str = _T("Phase Noise Extend Void Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PHASE_NOISE_EXTEND_VOID_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PhaseNoiseExtendVoidColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);	
	
	//相位遮罩-高度異常-顏色
	ParamUnit = CParamUni();
	str = _T("Phase Noise Height Unexpected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_UNEXPECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_SpaceNoiseHeightUnexpectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);	

	//相位遮罩-過低異常-顏色
	ParamUnit = CParamUni();
	str = _T("Space Noise Height Over Low Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_SPACE_NOISE_HEIGHT_OVER_LOW_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_SpaceNoiseHeightOverLowColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//相位遮罩-可靠高度-顏色		
	ParamUnit = CParamUni();
	str = _T("Space Best Valid Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_SPACE_VALID_BEST_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_SpaceBestValidColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//模組未設定顏色	
	ParamUnit = CParamUni();
	str = _T("Model Unset Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_MODEL_UNSET_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_ModelUnsetColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//2值化遮罩-顏色-3色燈
	ParamUnit = CParamUni();
	str = _T("Binary Mask Color RGB");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BINARY_MASK_COLOR_RGB;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BinaryMaskColorRGB);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//2值化遮罩-顏色-預設
	ParamUnit = CParamUni();
	str = _T("Binary Mask Color Default");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BINARY_MASK_COLOR_DEFAULT;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BinaryMaskColorDefault);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(bReadOnly);
	ParamList.push_back(ParamUnit);

	//2值化遮罩-顏色-透明度				
	ParamUnit = CParamUni();
	str = _T("Binary Mask Color Alpha");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BINARY_MASK_COLOR_ALPHA;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_INT(Ptr->m_BinaryMaskColorAlpha, 0, 255);
	ParamUnit.SetDesction(strCaption);	
	//ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//整板的顏色-1	
	ParamUnit = CParamUni();
	str = _T("Panel Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PANEL_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PanelColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//整板的顏色-2
	ParamUnit = CParamUni();
	str = _T("Panel Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PANEL_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PanelColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);
		
	//整板文字顏色		
	ParamUnit = CParamUni();
	str = _T("Panel Text Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PANEL_TEXT_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PanelTextColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//整板選取到的顏色		
	ParamUnit = CParamUni();
	str = _T("Panel Selected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_PANEL_SELECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_PanelSelectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//單板的顏色-1		
	ParamUnit = CParamUni();
	str = _T("Board Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BOARD_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BoardColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);
	
	//單板的顏色-2		
	ParamUnit = CParamUni();
	str = _T("Board Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BOARD_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BoardColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//單板文字顏色	
	ParamUnit = CParamUni();
	str = _T("Board Text Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BOARD_TEXT_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BoardTextColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);	

	//單板選取到的顏色
	ParamUnit = CParamUni();
	str = _T("Board Selected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BOARD_SELECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BoardSelectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);	
	
	//定位點的顏色-1
	ParamUnit = CParamUni();
	str = _T("Fd Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_FD_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_FdColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);
			
	//定位點的顏色-2		
	ParamUnit = CParamUni();
	str = _T("Fd Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_FD_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_FdColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//定位點文字顏色
	ParamUnit = CParamUni();
	str = _T("Fd Text Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_FD_TEXT_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_FdTextColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//定位點選取到的顏色
	ParamUnit = CParamUni();
	str = _T("Fd Selected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_FD_SELECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_FdSelectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//條碼的顏色-1
	ParamUnit = CParamUni();
	str = _T("Barcode Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BARCODE_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BarcodeColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);


	//條碼的顏色-2		
	ParamUnit = CParamUni();
	str = _T("Barcode Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BARCODE_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BarcodeColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//條碼文字顏色
	ParamUnit = CParamUni();
	str = _T("Barcode Text Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BARCODE_TEXT_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BarcodeTextColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//條碼選取到的顏色
	ParamUnit = CParamUni();
	str = _T("Barcode Selected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_BARCODE_SELECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_BarcodeSelectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);


	//零件的顏色-1
	ParamUnit = CParamUni();
	str = _T("Component Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_COMPONENT_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_ComponentColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//零件的顏色-2
	ParamUnit = CParamUni();
	str = _T("Component Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_COMPONENT_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_ComponentColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//零件文字顏色	
	ParamUnit = CParamUni();
	str = _T("Component Text Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_COMPONENT_TEXT_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_ComponentTextColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//零件選取到的顏色	
	ParamUnit = CParamUni();
	str = _T("Component Selected Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_COMPONENT_SELECTED_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_ComponentSelectedColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//分段的顏色-1
	ParamUnit = CParamUni();
	str = _T("District Color 1");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_DISTRICT_COLOR_1;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_DistrictColor1);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//分段的顏色-2
	ParamUnit = CParamUni();
	str = _T("District Color 2");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_DISTRICT_COLOR_2;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_DistrictColor2);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測OK顏色		
	ParamUnit = CParamUni();
	str = _T("Inspected Result OK Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_OK_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultOKColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測NG顏色		
	ParamUnit = CParamUni();
	str = _T("Inspected Result NG Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_NG_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultNGColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測Skip顏色		
	ParamUnit = CParamUni();
	str = _T("Inspected Result Skip Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_SKIP_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultSkipColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測Bypass顏色		
	ParamUnit = CParamUni();
	str = _T("Inspected Result Bypass Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_BYPASS_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultBypassColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測UnTest顏色	
	ParamUnit = CParamUni();
	str = _T("Inspected Result Un-Test Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_UNTEST_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultUnTestColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測Warning顏色	
	ParamUnit = CParamUni();
	str = _T("Inspected Result Warning Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_WARNING_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultWarningColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);

	//檢測Exception顏色		
	ParamUnit = CParamUni();
	str = _T("Inspected Result Exception Color");
	strCaption = LoadMultiLanguageString(str, str);
	SysParam = SYSTEM_INSPECTED_RESULT_EXCEPTION_COLOR;
	ParamUnit.SetParamID((UINT)(SysParam));	
	ParamUnit.SetCaption(strCaption);
	ParamUnit.SetValue_CLR(Ptr->m_InspectedResultExceptionColor);
	ParamUnit.SetDesction(strCaption);	
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	ParamUnit.SetReadOnly(false);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_31 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_SysParameterPtr ) { return true; }

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	COLORREF      Color=0;
	CParamUni    *ParamPtr=NULL;	
	const int     nSubItem1 = 1;		
	const int     nSubItem2 = 2;
	const bool    bChagneColor=false;
	PARAM_DATA_TYPE DataType;
	
	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		
		DataType = ParamPtr->GetDataType();

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		if ( bChagneColor )
		{
			if ( PARAM_DATA_CLR == DataType )
			{	
				//SetItemTextColor//SetItemTextBkColor
				Color = ParamPtr->GetValue_CLR();
				ListCtrl.SetItemTextBkColor(nItem, 0, Color);
				ListCtrl.SetItemTextBkColor(nItem, nSubItem1, 0xFFFFFF);
				ListCtrl.SetItemTextBkColor(nItem, nSubItem2, 0xFFFFFF);
			}
			else
			{	ListCtrl.SetItemTextBkColor(nItem, 0xFFFFFF); }
		}
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_31 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*5;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemColorPane::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = SYSCOLOR_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::ExecItemchangedParamListWnd(CThisListCtrl_31 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
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
			ListCtrl.UpdateData();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	m_BtnCtrl.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::ExecDblclkParamListWnd(CThisListCtrl_31 &ListCtrl, int nItem, int nSubItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < SETTING_COL ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
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
	m_BtnCtrl.ShowWindow(SW_HIDE);
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
bool CSystemColorPane::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());	
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_31 *pListCtrl = (CThisListCtrl_31*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemColorPane::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	SYSTEM_PARAM_ID SysParam = (SYSTEM_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CAOIDataCollect::SetSystemParameterStringByID(SysParam, *m_SysParameterPtr, ItemText) == false )
	{	return false; }

	ParamPtr->SetNewValue_SEL(Param);	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_31 *pListCtrl = (CThisListCtrl_31*)(ParamPtr->GetListCtrl());
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