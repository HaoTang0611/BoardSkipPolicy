// ProjectParamPaneSave.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneSave.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
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
// CProjectParamPaneSave dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneSave::CProjectParamPaneSave(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;
	m_ProParameterPtr = NULL;	
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneSave)		
	DDX_Control(pDX, PROSAVE_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, PROSAVE_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROSAVE_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROSAVE_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneSave, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneSave)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, PROSAVE_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, PROSAVE_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(PROSAVE_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PROSAVE_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROSAVE_PARAM_COMBO, OnKillfocusParamCombo)	
	ON_BN_CLICKED(PROSAVE_PARAM_BTN, OnParamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneSave message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	BuildParamListWndHeader();
	if ( CWnd::IsWindowVisible() )
	{	BuildParamListWnd(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnSize(UINT nType, int cx, int cy) 
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

	WndPtr = CWnd::GetDlgItem(PROSAVE_INFO_EDIT);
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
void CProjectParamPaneSave::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildParamListWnd();	
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneSave::PreTranslateMessage(MSG* pMsg) 
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
LRESULT CProjectParamPaneSave::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
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
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnKillfocusParamEdit()
{
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnSelchangeParamCombo()
{
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnKillfocusParamCombo()
{
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	HideCtrlBtn();
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return ; }	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	bool bSucc = false;
	CString ItemText = ParamPtr->GetParamText();
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());	

	switch ( ParamID )
	{
	case PROJECT_PARAM_SAVE_MODEL_IMAGE_ON_OFF_AI:
		bSucc = ExecBtn_SaveModelImageOnOff_AI(ParamPtr, ItemText);
		break;
	}	
	if ( false == bSucc )
	{	return; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_26 *pListCtrl = (CThisListCtrl_26*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_SAVE_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_SAVE_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_SAVE_PANE");
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
CString CProjectParamPaneSave::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_SAVE_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneSave::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::BuildParamList()
{
	CThisListCtrl_26 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return true; }

	int       intValue=0;
	size_t    i=0, j=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;
	CParamUni    ParamUnit;	
	PROJECT_PARAM_ID ParamID;	
	TProjectParameter *Ptr = m_ProParameterPtr;
	const size_t FrameCount = ProjectPtr->GetProjectFrameUniqueIDCount();
	const int nSubItem = 1;	
	CParamList &ParamList = m_ParamList;

	ParamList.clear();

	//儲存專案檢測底圖縮圖比例
	ParamUnit = CParamUni();
	str = _T("Project Test Map Scale Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_TEST_PROJECT_SCALE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ProjectTestMapScaleMode, 1, 32);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//啟用儲存專案底圖
	ParamUnit = CParamUni();
	str = _T("Save Project Test Map");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP;
	ParamUnit.SetParamID((UINT)(ParamID));		
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_DISABLE);	ParamUnit.AddSelItem(SAVE_TEST_MAP_DISABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PANEL);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PANEL, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_BOARD);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_BOARD, strValue);	
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG_PANEL);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG_PANEL, strValue);
	strValue = AOIDataDefine.GetSaveTestMapModeText(SAVE_TEST_MAP_ENB_PROG_BOARD);	ParamUnit.AddSelItem(SAVE_TEST_MAP_ENB_PROG_BOARD, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	int nShow=FN_ENABLE;
	if ( FN_ENABLE == nShow )
	{
		if ( FrameCount > 0 )
		{
			//啟用儲存專案底圖-01
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-01");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_01;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_01);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}
		if ( FrameCount > 1 )
		{
			//啟用儲存專案底圖-02
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-02");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_02;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_02);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( FrameCount > 2 )
		{
			//啟用儲存專案底圖-03
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-03");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_03;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_03);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( FrameCount > 3 )
		{
			//啟用儲存專案底圖-04
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-04");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_04;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_04);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}
	
		if ( FrameCount > 4 )
		{
			//啟用儲存專案底圖-05
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-05");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_05;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_05);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( FrameCount > 5 )
		{
			//啟用儲存專案底圖-06
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-06");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_06;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_06);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( FrameCount > 6 )
		{
			//啟用儲存專案底圖-07
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-07");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_07;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_07);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}

		if ( FrameCount > 7 )
		{
			//啟用儲存專案底圖-08
			ParamUnit = CParamUni();
			str = _T("Save Project Test Map-08");
			strCaption = LoadMultiLanguageString(str, str);
			ParamUnit.SetCaption(strCaption);
			ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_08;
			ParamUnit.SetParamID((UINT)(ParamID));		
			strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
			strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
			ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMap_08);
			strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
			ParamUnit.SetDesction(strDescription);
			ParamList.push_back(ParamUnit);
		}
	}
	
	//啟用儲存專案檢測底圖至維修站	
	ParamUnit = CParamUni();
	str = _T("Save Project Test Map To Repair");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_TEST_PROJECT_MAP_TO_REPAIR;
	ParamUnit.SetParamID((UINT)(ParamID));		
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveProjectTestMapToRepair);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//在線調機模式
	ParamUnit = CParamUni();
	str = _T("Online Tuning Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_ONLINE_TUNING_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DEFECT);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DEFECT, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_EVERYONE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_EVERYONE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_OnlineTuningMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存模組圖片模式
	ParamUnit = CParamUni();
	str = _T("Save Model Image Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_MODEL_IMAGE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DEFECT);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DEFECT, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_EVERYONE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_EVERYONE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveModelImageMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存區域圖片模式
	ParamUnit = CParamUni();
	str = _T("Save Field Image Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_FIELD_IMAGE_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DISABLE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DISABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DEFECT);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DEFECT, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_EVERYONE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_EVERYONE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveFieldImageMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//儲存模組圖片模式-AI
	ParamUnit = CParamUni();
	str = _T("Save Model Image Mode For AI");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_MODEL_IMAGE_MODE_AI;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DISABLE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DISABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_DEFECT);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_DEFECT, strValue);
	strValue = AOIDataDefine.GetSaveTestImageModeText(SAVE_TEST_IMAGE_EVERYONE);	ParamUnit.AddSelItem(SAVE_TEST_IMAGE_EVERYONE, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveModelImageMode_AI);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存模組圖片開關-AI	
	ParamUnit = CParamUni();
	str = _T("Save Model Image On/Off For AI");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_MODEL_IMAGE_ON_OFF_AI;
	ParamUnit.SetParamID((UINT)(ParamID));
	ProjectPtr->BuildProjectSaveModelImageOnOff_AI_Text(Ptr->m_SaveModelImageOnOff_AI, str);
	ParamUnit.SetValue_STR(str);
	ParamUnit.SetReadOnly(true);
	ParamUnit.SetBtnWndPtr(&m_BtnCtrl);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存模組圖片3D檔案-AI		
	ParamUnit = CParamUni();
	str = _T("Save Model Image 3D File For AI");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_MODEL_IMAGE_3D_FILE_AI;
	ParamUnit.SetParamID((UINT)(ParamID));	
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveModelImage3DFile_AI);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存調機影像範疇
	ParamUnit = CParamUni();
	str = _T("Save Offline Image Scope");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_OFFLINE_IMAGE_SCOPE;
	ParamUnit.SetParamID((UINT)(ParamID));		
	strValue = AOIDataDefine.GetOfflineImageScopeText(OFFLINE_IMAGE_FOV);	ParamUnit.AddSelItem(OFFLINE_IMAGE_FOV, strValue);
	strValue = AOIDataDefine.GetOfflineImageScopeText(OFFLINE_IMAGE_PART);	ParamUnit.AddSelItem(OFFLINE_IMAGE_PART, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveOfflineImageScope);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//儲存統計資料
	ParamUnit = CParamUni();
	str = _T("Save Static Data");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_STATIC_DATA;
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	ParamUnit.SetValue_SEL(Ptr->m_SaveStaticData);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//儲存零件所有檢測框模式	
	ParamUnit = CParamUni();
	str = _T("Save Component Wnd List");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_COMPONENT_WND_LIST_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));
	//AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);	
	strValue = AOIDataDefine.GetSaveTestDataText(SAVE_TEST_DATA_DISABLE);	ParamUnit.AddSelItem(SAVE_TEST_DATA_DISABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestDataText(SAVE_TEST_DATA_ENABLE);	ParamUnit.AddSelItem(SAVE_TEST_DATA_ENABLE, strValue);
	strValue = AOIDataDefine.GetSaveTestDataText(SAVE_TEST_DATA_DEFECT);	ParamUnit.AddSelItem(SAVE_TEST_DATA_DEFECT, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_SaveComponentWndListMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//專案連線伺服器模式	
	ParamUnit = CParamUni();
	str = _T("Project Link Server Mode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_PROJECT_LINK_SERVER_MODE;
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_DISABLE);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_DISABLE, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_ENABLE_ALL);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_ENABLE_ALL, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_PROJECT_ONLY);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_PROJECT_ONLY, strValue);	
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_ENABLE_ALL_ASK);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_ENABLE_ALL_ASK, strValue);
	strValue = AOIDataDefine.GetProjectLinkServerModeText(PROJECT_LINK_SERVER_PROJECT_ONLY_ASK);	ParamUnit.AddSelItem(PROJECT_LINK_SERVER_PROJECT_ONLY_ASK, strValue);	
	ParamUnit.SetValue_SEL(Ptr->m_ProjectLinkServerMode);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//專案伺服器資料庫群組		
	ParamUnit = CParamUni();
	str = _T("Server Library Group");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SERVER_LIBRARY_GROUP;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	ParamUnit.SetValue_STR(Ptr->m_ProjectServerLibraryGroup.c_str());
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檢測前是否儲存頭檔
	ParamUnit = CParamUni();
	str = _T("Save The SPC Header JSON File Before Inspection");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SAVE_SPC_HEADER_JSON_BEFORE_INSPECTION;//INSPECTION
	ParamUnit.SetParamID((UINT)(ParamID));
	AOIDataDefine.BuildEnableDisableParamUni(ParamUnit);
	ParamUnit.SetValue_SEL(Ptr->m_SaveSPCHeaderJSONBeforeInspection);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::BuildParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_26 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopParamListBeSelected = false;
	if ( NULL == m_ProParameterPtr ) { return true; }

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

		strIndex.Format(_T("%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_26 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/4;
	width2 = 48;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*2;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSave::SetDescriptionText(const CParamUni *Ptr)
{
	UINT CtrlID = PROSAVE_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::ExecItemchangedParamListWnd(CThisListCtrl_26 &ListCtrl, int nItem)
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
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	HideCtrlBtn();	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::ExecDblclkParamListWnd(CThisListCtrl_26 &ListCtrl, int nItem, int nSubItem)
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
	HideCtrlBtn();
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
bool CProjectParamPaneSave::HideCtrlBtn()
{	
	m_BtnCtrl.ShowWindow(SW_HIDE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::ExecReleaseParamCtrl()
{	
	HideCtrlBtn();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSave::ExecUpdateParamByEdit()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());		
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_26 *pListCtrl = (CThisListCtrl_26*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneSave::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_26 *pListCtrl = (CThisListCtrl_26*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneSave::ExecBtn_SaveModelImageOnOff_AI(CParamUni *ParamUni, CString &ResultText)
{	
	if ( NULL == ParamUni ) { return false; }
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_ProParameterPtr ) { return false; }

	int &OnOffRef = m_ProParameterPtr->m_SaveModelImageOnOff_AI;

	DWORD Bit = 1;
	DWORD Flag = (DWORD)(OnOffRef);			
	std::vector<TFrameParam> FrameParamList;	
	ProjectPtr->CloneProjectFrameParamList(FrameParamList);
	const bool ShowUniID = false;
	const int FrameParamCount = FrameParamList.size();

	TListNode Node;
	int EnableDisableID = 0;	
	std::vector<TListNode> DropList;
	std::vector<TListNode> NodelList;	

	Node.Data = FN_ENABLE;
	Node.Text = AOIDataDefine.GetEnableDisableText(FN_ENABLE);
	DropList.push_back(Node);

	Node.Data = FN_DISABLE;
	Node.Text = AOIDataDefine.GetEnableDisableText(FN_DISABLE);
	DropList.push_back(Node);

	for ( int i=0; i<FrameParamCount; i++ )
	{	
		TFrameParam &FrameParam = FrameParamList[i];		
		if ( 0 != (Flag&Bit) )
		{	EnableDisableID = FN_ENABLE;	}		
		else
		{	EnableDisableID = FN_DISABLE;	}		
		Node.Text = FrameParam.GetFrameGridName(ShowUniID);
		Node.Text2 = AOIDataDefine.GetEnableDisableText(EnableDisableID);
		NodelList.push_back(Node);
		Bit = Bit << 1;
	}

	CInputListWnd   EnumWnd;
	CString strLabel = _T("Save");
	CString strCaption = _T("Set AI Save Model Image");		
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = LoadMultiLanguageString(strCaption, strCaption);

	EnumWnd.SetDropList(DropList);
	EnumWnd.SetParam1(strCaption, strLabel, (DWORD_PTR)(0), NodelList);
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return false; }

	DWORD Flag2=0;	
	std::vector<TListNode> NodelList2;
	EnumWnd.GetDataList(NodelList2);
	const size_t NodeCount2=NodelList2.size();

	Bit=1;	
	for ( int i=0; i<NodeCount2; i++ )
	{
		const TListNode &NodeRef=NodelList2[i];
		int Enabled=AOIDataDefine.FindEnableDisableIDByText(NodeRef.Text2);
		if ( FN_ENABLE == Enabled )
		{	Flag2 |= Bit;	}
		Bit = Bit << 1;
	}

	OnOffRef = Flag2;
	ProjectPtr->BuildProjectSaveModelImageOnOff_AI_Text(Flag2, ResultText);
	ParamUni->SetNewValue_STR(ResultText);
	return true;
}
//-------------------------------------------------------------------------------------//