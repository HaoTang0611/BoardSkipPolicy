// ProjectParamPaneSpecTest.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectParamPaneSpecTest.h"
//-------------------------------------------------------------------------------------//
#include "SpaceBaseParamWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
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
// CProjectParamPaneSpecTest dialog
//-------------------------------------------------------------------------------------//
CProjectParamPaneSpecTest::CProjectParamPaneSpecTest(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectParamPaneSpecTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectParamPaneSpecTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ParamActPtr = NULL;	
	m_ProParameterPtr = NULL;
	m_StopDefectEnableListBeSelected = false;
	m_StopDropOutParamListBeSelected = false;
	m_StopScratchParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectParamPaneSpecTest)
	DDX_Control(pDX, PROSPCTEST_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, PROSPCTEST_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, PROSPCTEST_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, PROSPCTEST_DROP_BASE_PLANE_BTN, m_DropBasePlaneBtn);	
	DDX_Control(pDX, PROSPCTEST_DROP_SPACE_FILTER_BTN, m_DropSpaceFilterBtn);		
	DDX_Control(pDX, PROSPCTEST_DROP_OUT_LIST_WND, m_DropOutParamListCtrl);	
	DDX_Control(pDX, PROSPCTEST_SCRATCH_LIST_WND, m_ScratchParamListCtrl);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectParamPaneSpecTest, CDialog)
	//{{AFX_MSG_MAP(CProjectParamPaneSpecTest)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(PROSPCTEST_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(PROSPCTEST_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(PROSPCTEST_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(PROSPCTEST_PARAM_BTN, OnParamBtn)		
	ON_NOTIFY(LVN_ITEMCHANGED, PROSPCTEST_DROP_OUT_LIST_WND, OnItemchangedDropOutListWnd)
	ON_NOTIFY(NM_DBLCLK, PROSPCTEST_DROP_OUT_LIST_WND, OnDblclkDropOutListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, PROSPCTEST_SCRATCH_LIST_WND, OnItemchangedScratchListWnd)
	ON_NOTIFY(NM_DBLCLK, PROSPCTEST_SCRATCH_LIST_WND, OnDblclkScratchListWnd)	
	ON_BN_CLICKED(PROSPCTEST_DROP_BASE_PLANE_BTN, OnDropBasePlaneBtn)
	ON_BN_CLICKED(PROSPCTEST_DROP_SPACE_FILTER_BTN, OnDropSpaceFilterBtn)
	//ON_BN_CLICKED(PROSPCTEST_DROP_REGION_BTN, OnDropRegionSettingBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectParamPaneSpecTest message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneSpecTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();	
	JetAPI::InitialListCtrl(m_DropOutParamListCtrl);
	JetAPI::InitialListCtrl(m_ScratchParamListCtrl);
	
	BuildDropOutParamListWndHeader();
	BuildScratchParamListWndHeader();
	
	if ( CWnd::IsWindowVisible() )
	{
		BuildDropOutParamListWnd();
		BuildScratchParamListWnd();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( m_DropOutParamListCtrl.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	SIZE  WndSize={0};
	RECT  InfoRect={0};
	BOOL  bVisible = CWnd::IsWindowVisible();
	int       TopPosY = 0;	
	POINT     OffsetL;
	POINT     OffsetR;
	const int MarginX = 4;
	const int MarginY = 4;	
	const int ListSizeW = (cx-MarginX-MarginX-MarginX)/2;
	OffsetL.x = OffsetL.y = 0;
	OffsetR.x = OffsetR.y = 0;		

	WndPtr = CWnd::GetDlgItem(PROSPCTEST_DROP_OUT_LABEL);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		OffsetL.x = MarginX-WndRect.left;		
		TopPosY = WndRect.bottom;
	}

	WndPtr = CWnd::GetDlgItem(PROSPCTEST_INFO_EDIT);
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
	
	if ( m_DropOutParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		m_DropOutParamListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);		
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_DropOutParamListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_DropOutParamListCtrl.Invalidate();	}
	}
	if ( m_ScratchParamListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};		
		m_ScratchParamListCtrl.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);		
		WndRect.top = TopPosY+MarginY;
		WndRect.bottom = InfoRect.top-MarginY;
		m_ScratchParamListCtrl.MoveWindow(&WndRect, FALSE);		
		if ( TRUE == bVisible )
		{	m_ScratchParamListCtrl.Invalidate();	}
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		BuildDropOutParamListWnd();	
		BuildScratchParamListWnd();
	}
}
//-------------------------------------------------------------------------------------//
BOOL CProjectParamPaneSpecTest::PreTranslateMessage(MSG* pMsg) 
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
void CProjectParamPaneSpecTest::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_BtnCtrl.ShowWindow(SW_HIDE);	
	m_DropBasePlaneBtn.ShowWindow(SW_HIDE);	
	m_DropSpaceFilterBtn.ShowWindow(SW_HIDE);	
	m_DropRegionBtn.ShowWindow(SW_HIDE);
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return; }
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();
	if ( NULL == BtnWndPtr ) { return; }

	CString ItemText = ParamPtr->GetParamText();
	PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());	

	if ( JetAPI::OpenFolderDialog(this, ItemText) == false ) { return; }
	if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
	{	return ; }
	ParamPtr->SetNewValue(ItemText);
	SetActParamUni(NULL);
	
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	CThisListCtrl_27 *pListCtrl = (CThisListCtrl_27*)(ParamPtr->GetListCtrl());

	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);
		pListCtrl->SetFocus();
	}
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_PARAM_SPEC_TEST_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_PARAM_SPEC_TEST_PANE;
	WndKey = _T("IDD_PROJECT_PARAM_SPEC_TEST_PANE");
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
	WndID = PROSPCTEST_DROP_OUT_LABEL;
	WndKey = _T("PROSPCTEST_DROP_OUT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROSPCTEST_SCRATCH_LABEL;
	WndKey = _T("PROSPCTEST_SCRATCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectParamPaneSpecTest::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_PARAM_SPEC_TEST_PANE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::SetProjectParameterPtr(CAOIProject *ProjectPtr, TProjectParameter *Ptr)
{
	this->m_ProjectPtr = ProjectPtr;
	this->m_ProParameterPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CParamUni* CProjectParamPaneSpecTest::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildDropOutParamList()
{
	CThisListCtrl_27 &ListCtrl = m_DropOutParamListCtrl;
	m_StopDropOutParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopDropOutParamListBeSelected = false;
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }
	CAOIProject *ProjectPtr = m_ProjectPtr;	

	size_t    i=0;
	int       intValue=0;	
	CString   str;
	CString   strKey;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;	
	PROJECT_PARAM_ID ParamID;
	CParamUni    ParamUnit;			
	const int nSubItem = 1;		
	TProjectParameter *Ptr = m_ProParameterPtr;
	unsigned int FrameUniqueID=0;
	const size_t FrameUniqueIDCount = ProjectPtr->GetProjectFrameUniqueIDCount();
	m_DropOutList.clear();	

	//檢測拋件
	ParamUnit = CParamUni();
	str = _T("Enable");
	strKey = _T("Drop Out Part Enable");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartEnable);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-存圖
	ParamUnit = CParamUni();
	str = _T("Save Image");
	strKey = _T("Drop Out Part Save Image");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_SAVE_IMAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartSaveImage);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);	

	//檢測拋件-影像唯一碼		
	ParamUnit = CParamUni();
	str = _T("Frame");
	strKey = _T("Drop Out Part Frame");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_FRAME_UNIQUE_ID;
	ParamUnit.SetParamID((UINT)(ParamID));	
	
	for ( i=0; i<FrameUniqueIDCount; i++ )
	{
		FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(i, false);
		TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
		if ( NULL==FrameParamPtr ) { continue; }
		strValue = FrameParamPtr->FrameName;
		ParamUnit.AddSelItem(FrameUniqueID, strValue);		
	}
	if ( ParamUnit.SetValue_SEL(Ptr->m_DropOutPartFrameUniqueID) == false )
	{	Ptr->m_DropOutPartFrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(0, true);	}
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartFrameUniqueID);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);	
	

	//檢測拋件-縮放匹配啟用
	ParamUnit = CParamUni();
	str = _T("Match Use Scale");
	strKey = _T("Drop Out Part Match Use Scale");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_MATCH_USE_SCALE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartMatchUseScale);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);
	
	//過低定義
	ParamUnit = CParamUni();
	str = _T("Over Low");
	strKey = _T("Drop Out Part Over Low");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_OVER_LOW;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartOverLow, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//過高定義
	ParamUnit = CParamUni();
	str = _T("Over High");
	strKey = _T("Drop Out Part Over High");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_OVER_HIGH;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartOverHigh, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-暗部定義
	ParamUnit = CParamUni();
	str = _T("Dark Level");
	strKey = _T("Drop Out Part Dark Level");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_DARK_LEVEL;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartDarkLevel, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-過亮定義
	ParamUnit = CParamUni();
	str = _T("Light Level");
	strKey = _T("Drop Out Part Light Level");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_LIGHT_LEVEL;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartLightLevel, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);	

	//檢測拋件-公差
	ParamUnit = CParamUni();
	str = _T("Tolerance");
	strKey = _T("Drop Out Part Tolerance");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_TOLERANCE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartTolerance, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);	

	//檢測拋件-平滑過濾		
	ParamUnit = CParamUni();
	str = _T("Smooth Filter Size");
	strKey = _T("Drop Out Part Smooth Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_SMOOTH_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartSmoothSize, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-邊線移除	
	ParamUnit = CParamUni();
	str = _T("Remove Edge");
	strKey = _T("Drop Out Part Remove Edge");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_EDGE_REMOVE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartEdgeRemove);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-膨脹尺寸
	ParamUnit = CParamUni();
	str = _T("Dilate Size");
	strKey = _T("Drop Out Part Dilate Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_DILATE_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartDilateSize, 0, 100);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-高斯過濾
	ParamUnit = CParamUni();
	str = _T("Gaussian Filter Size");
	strKey = _T("Drop Out Part Gaussian Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_GAUSSIAN_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartGaussian, 0, 100);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-開運算
	ParamUnit = CParamUni();
	str = _T("Open Filter Size");
	strKey = _T("Drop Out Part Filter Open Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_FILTER_OPEN_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartFilterOpenSize, 0, 100);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-閉運算
	ParamUnit = CParamUni();
	str = _T("Close Filter Size");
	strKey = _T("Drop Out Part Filter Close Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_FILTER_CLOSE_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartFilterCloseSize, 0, 100);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-最小尺寸寬度
	ParamUnit = CParamUni();
	str = _T("Min Size W");
	strKey = _T("Drop Out Part Min Size W");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_MIN_SIZE_W;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_DropOutPartMinSizeW, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-最小尺寸長度
	ParamUnit = CParamUni();
	str = _T("Min Size H");
	strKey = _T("Drop Out Part Min Size H");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_MIN_SIZE_H;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_DropOutPartMinSizeH, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-最大尺寸比例
	ParamUnit = CParamUni();
	str = _T("Max Size R");
	strKey = _T("Drop Out Part Max Size R");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_MAX_SIZE_R;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_DropOutPartMaxSizeR, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-基準面設定
	ParamUnit = CParamUni();	
	strValue = _T("Set");
	str = _T("Base Plane Param");
	strKey = _T("Drop Out Part Base Plane Param");		
	strCaption = LoadMultiLanguageString(strKey, str);	
	strValue = LoadMultiLanguageString(strValue, strValue);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_BASE_PLANE_PARAM;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(strValue);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetReadOnly(true);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_DropBasePlaneBtn);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-空間雜訊過濾處理
	ParamUnit = CParamUni();
	strValue = _T("Set");
	str = _T("Space Noise Filter");
	strKey = _T("Drop Out Part Space Noise Filter");
	strCaption = LoadMultiLanguageString(strKey, str);	
	strValue = LoadMultiLanguageString(strValue, strValue);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_SPACE_NOISE_FILTER;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_STR(strValue);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetReadOnly(true);
	ParamUnit.SetDesction(strDescription);
	ParamUnit.SetBtnWndPtr(&m_DropSpaceFilterBtn);
	m_DropOutList.push_back(ParamUnit);
	

	ParamUnit = CParamUni();
	str = _T("Xboard Excluded");
	strKey = _T("Drop Out Part Xboard Excluded");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_XBOARD_EXCLUDED;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_DropOutPartXBoardExcluded);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);
		

	//case ://空間雜訊過濾處理
	/*
	//檢測拋件-樣板尺寸寬度	
	ParamUnit = CParamUni();
	str = _T("Calc Size Width");
	strKey = _T("Drop Out Part Calc Size Width");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_CALC_SIZE_W;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartCalcSizeW, 32);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);

	//檢測拋件-樣板尺寸長度	
	ParamUnit = CParamUni();
	str = _T("Calc Size Height");
	strKey = _T("Drop Out Part Calc Size Height");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_DROP_OUT_PART_CALC_SIZE_H;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_DropOutPartCalcSizeH, 32);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	m_DropOutList.push_back(ParamUnit);	
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildDropOutParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_27 &ListCtrl = m_DropOutParamListCtrl;	
	m_StopDropOutParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopDropOutParamListBeSelected = false;
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
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildDropOutParamList();

	const int ParamCount = (int)(m_DropOutList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopDropOutParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_DropOutList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetWndCtrlID(WndCtrlID);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		

		strIndex.Format(_T("A.%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopDropOutParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildDropOutParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_27 &ListCtrl = m_DropOutParamListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/4;
		width2 = 48;
		str = AOIDataDefine.GetIndexText();	
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		width2 = width;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildScratchParamList()
{
	CThisListCtrl_27 &ListCtrl = m_ScratchParamListCtrl;
	m_StopScratchParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopScratchParamListBeSelected = false;
	if ( NULL == m_ProjectPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }
	CAOIProject *ProjectPtr = m_ProjectPtr;	

	size_t    i=0;
	int       intValue=0;	
	CString   str;
	CString   strKey;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	int       nItem=0;	
	PROJECT_PARAM_ID ParamID;
	CParamUni    ParamUnit;			
	const int nSubItem = 1;		
	TProjectParameter *Ptr = m_ProParameterPtr;
	unsigned int FrameUniqueID=0;
	const size_t FrameUniqueIDCount = ProjectPtr->GetProjectFrameUniqueIDCount();
	CParamList &ParamList = m_ScratchList;

	ParamList.clear();	

	//檢測刮傷
	ParamUnit = CParamUni();
	str = _T("Enable");
	strKey = _T("Scratch Part Enable");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_ENABLE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_ScratchPartEnable);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-存圖
	ParamUnit = CParamUni();
	str = _T("Save Image");
	strKey = _T("Scratch Part Save Image");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_SAVE_IMAGE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_ScratchPartSaveImage);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檢測刮傷-影像唯一碼		
	ParamUnit = CParamUni();
	str = _T("Frame");
	strKey = _T("Scratch Part Frame");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_FRAME_UNIQUE_ID;
	ParamUnit.SetParamID((UINT)(ParamID));	
	
	for ( i=0; i<FrameUniqueIDCount; i++ )
	{
		FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(i, false);
		TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
		if ( NULL==FrameParamPtr ) { continue; }
		strValue = FrameParamPtr->FrameName;
		ParamUnit.AddSelItem(FrameUniqueID, strValue);		
	}
	if ( ParamUnit.SetValue_SEL(Ptr->m_ScratchPartFrameUniqueID) == false )
	{	Ptr->m_ScratchPartFrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(0, true);	}
	ParamUnit.SetValue_SEL(Ptr->m_ScratchPartFrameUniqueID);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//檢測刮傷-縮放匹配啟用
	ParamUnit = CParamUni();
	str = _T("Match Use Scale");
	strKey = _T("Scratch Part Match Use Scale");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MATCH_USE_SCALE;
	ParamUnit.SetParamID((UINT)(ParamID));
	intValue = FN_DISABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	intValue = FN_ENABLE;	strValue = AOIDataDefine.GetEnableDisableText(intValue);	ParamUnit.AddSelItem(intValue, strValue);
	ParamUnit.SetValue_SEL(Ptr->m_ScratchPartMatchUseScale);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//檢測刮傷-樣板尺寸寬度	
	ParamUnit = CParamUni();
	str = _T("Calc Size Width");
	strKey = _T("Scratch Part Calc Size Width");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_W;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_ScratchPartCalcSizeW, 0, 32);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-樣板尺寸長度	
	ParamUnit = CParamUni();
	str = _T("Calc Size Height");
	strKey = _T("Scratch Part Calc Size Height");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_H;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_ScratchPartCalcSizeH, 0, 32);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);		

	//檢測刮傷-顏色外擴		
	ParamUnit = CParamUni();
	str = _T("Color Expand");
	strKey = _T("Scratch Part Color Expand");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_COLOR_EXPAND;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartColorExpand, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-開運算		
	ParamUnit = CParamUni();
	str = _T("Filter Open Size");
	strKey = _T("Scratch Part Filter Open Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_FILTER_OPEN_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartFilterOpenSize, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-閉運算		
	ParamUnit = CParamUni();
	str = _T("Filter Close Size");
	strKey = _T("Scratch Part Filter Close Size");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_FILTER_CLOSE_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartFilterCloseSize, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-最小尺寸寬度	
	ParamUnit = CParamUni();
	str = _T("Min Size Width");
	strKey = _T("Scratch Part Min Size Width");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_W;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_ScratchPartMinSizeW, 0, 20);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檢測刮傷-最小尺寸長度	
	ParamUnit = CParamUni();
	str = _T("Min Size Height");
	strKey = _T("Scratch Part Min Size Height");
	strCaption = LoadMultiLanguageString(strKey, str);	
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_H;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_DBL(Ptr->m_ScratchPartMinSizeH, 0, 20);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//檢測刮傷-高斯平滑過濾
	ParamUnit = CParamUni();
	str = _T("Gaussian Filter");
	strKey = _T("Scratch Part Gaussian Filter");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_SMOOTH_SIZE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartSmoothSize, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);


	//檢測刮傷-邊緣閾值
	ParamUnit = CParamUni();
	str = _T("Edge Threshold");
	strKey = _T("Scratch Part Edge Threshold");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_EDGE_THRESHOLD;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartEdgeThreshold, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-刮傷最小像素
	ParamUnit = CParamUni();
	str = _T("Min Area");
	strKey = _T("Scratch Part Min Area");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MIN_PIXELS;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartMinPixels, 0);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	//檢測刮傷-灰階下界
	ParamUnit = CParamUni();
	str = _T("Scratch Part Min Grayscale");
	strKey = _T("Scratch Part Min Grayscale");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MIN_GRAYSCALE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartMinGrayscale, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//檢測刮傷-灰階上界
	ParamUnit = CParamUni();
	str = _T("Scratch Part Max Grayscale");
	strKey = _T("Scratch Part Max Grayscale");
	strCaption = LoadMultiLanguageString(strKey, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = PROJECT_PARAM_SCRATCH_PART_MAX_GRAYSCALE;
	ParamUnit.SetParamID((UINT)(ParamID));
	ParamUnit.SetValue_INT(Ptr->m_ScratchPartMaxGrayscale, 0, 255);
	strDescription = CAOIProject::ObtainProjectParameterDescText(ParamID);
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildScratchParamListWnd()
{
	SetActParamUni(NULL);
	CThisListCtrl_27 &ListCtrl = m_ScratchParamListCtrl;	
	m_StopScratchParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopScratchParamListBeSelected = false;
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
	const UINT    WndCtrlID = ListCtrl.GetDlgCtrlID();

	BuildScratchParamList();

	const int ParamCount = (int)(m_ScratchList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopScratchParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ScratchList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetWndCtrlID(WndCtrlID);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem2);		

		strIndex.Format(_T("B.%d"), nItem+1);
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem1, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem2, strValue);
		nItem ++;
	}	
	m_StopScratchParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::BuildScratchParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	{
		CThisListCtrl_27 &ListCtrl = m_ScratchParamListCtrl;

		ListCtrl.GetClientRect(&Rect);
		width = (Rect.right-Rect.left-8)/4;
		width2 = 48;
		str = AOIDataDefine.GetIndexText();	
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		width2 = width;
		str = _T("Information");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::SetDescriptionText(const CParamUni *Ptr)
{	
	UINT CtrlID = PROSPCTEST_INFO_EDIT;
	if ( NULL == Ptr )
	{
		CWnd::SetDlgItemText(CtrlID, _T(""));
		return ;
	}
	CWnd::SetDlgItemText(CtrlID, Ptr->GetDesction());	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);
	m_DropBasePlaneBtn.ShowWindow(SW_HIDE);	
	m_DropSpaceFilterBtn.ShowWindow(SW_HIDE);		
	m_DropRegionBtn.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	//m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::ExecItemchangedParamListWnd(CThisListCtrl_27 &ListCtrl, CParamList &ParamList, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

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
	else
	{	
		m_BtnCtrl.ShowWindow(SW_HIDE);	
		m_DropBasePlaneBtn.ShowWindow(SW_HIDE);	
		m_DropSpaceFilterBtn.ShowWindow(SW_HIDE);	
		m_DropRegionBtn.ShowWindow(SW_HIDE);
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CProjectParamPaneSpecTest::ExecDblclkParamListWnd(CThisListCtrl_27 &ListCtrl, CParamList &ParamList, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	//if ( nSubItem < SETTING_COL ) { return true; }

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
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_DropBasePlaneBtn.ShowWindow(SW_HIDE);	
	m_DropSpaceFilterBtn.ShowWindow(SW_HIDE);	
	m_DropRegionBtn.ShowWindow(SW_HIDE);
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
bool CProjectParamPaneSpecTest::ExecUpdateParamByEdit()
{
	CParamUni *ParamPtr = GetActParamUni();
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
	
	CThisListCtrl_27 *pListCtrl = (CThisListCtrl_27*)(ParamPtr->GetListCtrl());
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
bool CProjectParamPaneSpecTest::ExecUpdateParamByCombox()
{
	CParamUni *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }
	if ( NULL == m_ProParameterPtr ) { return true; }	
	UINT WndCtrlID = ParamPtr->GetWndCtrlID();
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;		
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }	

	if ( PROSPCTEST_DROP_OUT_LIST_WND == WndCtrlID )
	{
		ItemText.Format(_T("%d"),Param);
		PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());
		if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
		{	return false; }
	}

	if ( PROSPCTEST_SCRATCH_LIST_WND == WndCtrlID )
	{
		ItemText.Format(_T("%d"),Param);
		PROJECT_PARAM_ID ParamID = (PROJECT_PARAM_ID)(ParamPtr->GetParamID());
		if ( CAOIProject::SetProjectParameterStringByID(ParamID, *m_ProParameterPtr, ItemText) == false )
		{	return false; }
	}

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_27 *pListCtrl = (CThisListCtrl_27*)(ParamPtr->GetListCtrl());
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
void CProjectParamPaneSpecTest::OnItemchangedDropOutListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopDropOutParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		

	ExecItemchangedParamListWnd(m_DropOutParamListCtrl, m_DropOutList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnDblclkDropOutListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	if ( nSubItem < SETTING_COL ){ return; }
	ExecDblclkParamListWnd(m_DropOutParamListCtrl, m_DropOutList, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnItemchangedScratchListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopScratchParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		

	ExecItemchangedParamListWnd(m_ScratchParamListCtrl, m_ScratchList, nItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnDblclkScratchListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	if ( nSubItem < SETTING_COL ){ return; }
	ExecDblclkParamListWnd(m_ScratchParamListCtrl, m_ScratchList, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnDropBasePlaneBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProjectPtr ) { return ; }
	if ( NULL == m_ProParameterPtr ) { return ; }

	int  nBaseColoeIndex=0;	
	bool bBaseColorEnabled=false;		
	TBasePlaneParam BasePlaneParam;
	CSpaceBaseParamWnd ParamWnd;
	CAOIProject *ProjectPtr=m_ProjectPtr;
	TProjectParameter *Ptr = m_ProParameterPtr;
	TNoiseFilterParam  &SpaceNoiseFilter=Ptr->m_DropOutPartSpaceNoiseFilter;//空間雜訊過濾處理
	
	BasePlaneParam = SpaceNoiseFilter.BasePlaneParam;
	bBaseColorEnabled = BasePlaneParam.BasePlane2DMaskEnabled;
	nBaseColoeIndex   = BasePlaneParam.BasePlane2DMaskGroupLinkIndex;
	if ( nBaseColoeIndex >= 0 ) { nBaseColoeIndex -= PROJECT_COLOR_ID_BOARD_BEGIN; }

	//ParamWnd.SetProjectPtr(ProjectPtr);
	ParamWnd.SetBasePlaneParam(BasePlaneParam);
	ParamWnd.SetBaseColorIndex(nBaseColoeIndex);
	ParamWnd.SetBaseColorEnabled(bBaseColorEnabled);	
	if ( ParamWnd.DoModal() == IDCANCEL )
	{	return ; }	

	ParamWnd.GetBasePlaneParam(BasePlaneParam);
	nBaseColoeIndex = ParamWnd.GetBaseColorIndex();
	bBaseColorEnabled = ParamWnd.GetBaseColorEnabled();
	const bool bParamSetting = ParamWnd.GetBasePlaneParamSetting();
	const bool bColorSetting = ParamWnd.GetBasePlaneColorSetting();
	TBasePlaneParam &BasePlaneParamRef=SpaceNoiseFilter.BasePlaneParam;

	if ( true == bParamSetting )
	{	BasePlaneParamRef = BasePlaneParam; }

	if ( true == bColorSetting ) 
	{
		if ( false==bBaseColorEnabled || -1==nBaseColoeIndex )
		{	BasePlaneParamRef.BasePlane2DMaskEnabled = false;	}
		else
		{
			nBaseColoeIndex += PROJECT_COLOR_ID_BOARD_BEGIN;
			CColorGroup *ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(nBaseColoeIndex, true);
			if ( NULL == ColorGroupPtr )
			{	BasePlaneParamRef.BasePlane2DMaskEnabled = false;	}
			else
			{
				BasePlaneParamRef.BasePlane2DMaskEnabled = true;			
				BasePlaneParamRef.BasePlane2DMaskGroupLinkIndex = nBaseColoeIndex;
				BasePlaneParamRef.BasePlane2DMaskFrameIndex = ColorGroupPtr->GetColorGroupFrameIndex();
				BasePlaneParamRef.BasePlane2DMaskFrameUniqueID = ColorGroupPtr->GetColorGroupFrameUniqueID();
			}
		}
	}
	AOIDataCollect.UpdateSystemBasePlaneParamToProject();
}
//-------------------------------------------------------------------------------------//
void CProjectParamPaneSpecTest::OnDropSpaceFilterBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ProjectPtr ) { return ; }
	if ( NULL == m_ProParameterPtr ) { return ; }

	CSpaceNoiseFilterParamWnd Wnd;	
	TNoiseFilterParam NoiseFilterParam;
	TProjectParameter *Ptr = m_ProParameterPtr;
	TNoiseFilterParam  &SpaceNoiseFilter=Ptr->m_DropOutPartSpaceNoiseFilter;//空間雜訊過濾處理

	NoiseFilterParam = SpaceNoiseFilter;
	Wnd.SetNoiseFilterParam(NoiseFilterParam);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }
	Wnd.GetNoiseFilterParam(NoiseFilterParam);

	TBasePlaneParam BasePlaneParam = SpaceNoiseFilter.BasePlaneParam;
	SpaceNoiseFilter = NoiseFilterParam;
	SpaceNoiseFilter.BasePlaneParam = BasePlaneParam;
	AOIDataCollect.UpdateSystemNoiseFilterParamToProject();
}
//-------------------------------------------------------------------------------------//