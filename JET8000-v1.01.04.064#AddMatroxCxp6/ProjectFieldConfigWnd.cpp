// ProjectFieldConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ProjectFieldConfigWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectFieldConfigWnd dialog
//-------------------------------------------------------------------------------------//
CProjectFieldConfigWnd::CProjectFieldConfigWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CProjectFieldConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectFieldConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_ImageBit = 8;
	m_ImageWndBkColor = 0x000000;	
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	m_Resolution.x = 10;
	m_Resolution.y = 10;
	m_AreaID = 0;
	m_BuildID = 0;
	m_DivisionID = 0;	
	m_DistrictID = DISTRICT_ID_A;
	m_FieldGrabTime = 0;

	m_FieldBuildMode = FIELD_BUILD_NONE;
	m_InspectionFieldBuildMode = FIELD_BUILD_NONE;
	m_FieldBuildAreaMode = FIELD_BUILD_AREA_NONE;
	m_InspectionFieldBuildAreaMode = FIELD_BUILD_AREA_NONE;

	m_StopFieldListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectFieldConfigWnd)
	DDX_Control(pDX, PROFIELD_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, PROFIELD_FIELD_LIST_WND, m_FieldListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectFieldConfigWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CProjectFieldConfigWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(PROFIELD_AREA_BY_BOARD_CHK, OnAreaByBoardChk)
	ON_BN_CLICKED(PROFIELD_AREA_BY_COMPONENT_CHK, OnAreaByComponentChk)
	ON_BN_CLICKED(PROFIELD_CHECK_BOARD_ONE_FOV_BTN, OnCheckBoardOneFovBtn)	
	ON_BN_CLICKED(PROFIELD_FIELD_BY_PROJECT_CHK, OnFieldByProjectChk)
	ON_BN_CLICKED(PROFIELD_FIELD_BY_PANEL_CHK, OnFieldByPanelChk)
	ON_BN_CLICKED(PROFIELD_FIELD_BY_BOARD_CHK, OnFieldByBoardChk)
	ON_BN_CLICKED(PROFIELD_DIVISION_MASS_AREA_CHK, OnDivisionMassAreaChk)
	ON_BN_CLICKED(PROFIELD_DIVISION_DIA_LINE_CHK, OnDivisionDiaLineChk)
	ON_BN_CLICKED(PROFIELD_DIVISION_HOR_LINE_CHK, OnDivisionHorLineChk)
	ON_BN_CLICKED(PROFIELD_DIVISION_VER_LINE_CHK, OnDivisionVerLineChk)
	ON_BN_CLICKED(PROFIELD_RUN_PATH_BTN, OnRunPathBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, PROFIELD_FIELD_LIST_WND, OnItemchangedFieldListWnd)
	ON_BN_CLICKED(PROFIELD_SHOW_FIELD_CHK, OnShowFieldChk)
	ON_BN_CLICKED(PROFIELD_SHOW_COMPONENT_CHK, OnShowComponentChk)
	ON_BN_CLICKED(PROFIELD_FIELD_SORT_UP_BTN, OnFieldSortUpBtn)
	ON_BN_CLICKED(PROFIELD_FIELD_SORT_DOWN_BTN, OnFieldSortDownBtn)
	ON_BN_CLICKED(PROFIELD_FIELD_SORT_TOP_BTN, OnFieldSortTopBtn)
	ON_BN_CLICKED(PROFIELD_FIELD_SORT_BOT_BTN, OnFieldSortBotBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectFieldConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectFieldConfigWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);	
	JetAPI::InitialListCtrl(m_FieldListWnd);
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_ImageWndBkColor);	
	m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_ImageWndBkColor);	
	
	BuildFieldListWndHeader();
	SwitchMultiLanguage();
	CString str;	
	if ( 0 != m_AreaID )
	{	CWnd::CheckDlgButton(m_AreaID, TRUE);	}
	if ( 0 != m_BuildID ) 
	{	CWnd::CheckDlgButton(m_BuildID, TRUE); }
	if ( PROFIELD_FIELD_BY_PROJECT_CHK == m_BuildID ) 
	{
		EnableFieldAreaWnd(false);
		EnableFieldDivisionWnd(false); 
	}
	if ( 0 != m_DivisionID ) 
	{	CWnd::CheckDlgButton(m_DivisionID, TRUE); }	

	BuildFieldListWnd();
	str.Format(_T("%.2f"), m_ProjectParam.m_FieldSectionFactor);
	CWnd::SetDlgItemText(PROFIELD_SECTION_FACTOR_EDIT, str);
	CWnd::SetDlgItemInt(PROFIELD_FIELD_GRAB_TIME_EDIT, m_FieldGrabTime);
	str.Format(_T("Total Fields=%d"), m_FieldList.size());

	if ( FIELD_PATH_SPATH_HOR == m_ProjectParam.m_FieldPathMode )
	{	CWnd::CheckDlgButton(PROFIELD_HOR_PATH_RAD, TRUE); }
	if ( FIELD_PATH_SPATH_VER == m_ProjectParam.m_FieldPathMode )
	{	CWnd::CheckDlgButton(PROFIELD_VER_PATH_RAD, TRUE); }
	if ( FIELD_PATH_SPATH_USER == m_ProjectParam.m_FieldPathMode )
	{	CWnd::CheckDlgButton(PROFIELD_USER_PATH_RAD, TRUE); }

	CWnd::CheckDlgButton(PROFIELD_SHOW_FIELD_CHK, TRUE);
	SetFieldInfo(str);
	CreateBKImage(true);		
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
	ClearFieldList(m_FieldList);
	ClearFieldList(m_ProjectFieldList);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;

	WndPtr = CWnd::GetDlgItem(PROFIELD_INFO_EDIT);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;		
		WndPtr->MoveWindow(&WndRect, TRUE);
	}

	if ( m_FieldListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_FieldListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);		
		WndRect.bottom = cy-4;
		m_FieldListWnd.MoveWindow(&WndRect, TRUE);
	}

	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_ImageWnd.MoveWindow(&WndRect, TRUE);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_ImageWndBkColor);	
		m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, m_ImageWndBkColor);	
		CreateBKImage(true);
	}
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  768;
}
//-------------------------------------------------------------------------------------//
BOOL CProjectFieldConfigWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectFieldConfigWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
	if ( NULL != m_ProjectPtr )
	{
		std::vector<CAOIField*> FieldList;
		m_DistrictID = m_ProjectPtr->GetProjectActDistrictID();
		m_ProjectParam = m_ProjectPtr->GetProjectParameter();		
		m_FieldGrabTime= m_ProjectPtr->CalcProjectFrameGrabTime();
		m_FieldBuildMode = m_ProjectPtr->GetProjectFieldBuildMode();
		m_InspectionFieldBuildMode = m_ProjectPtr->GetProjectInspectionFieldBuildMode();
		m_FieldBuildAreaMode = m_ProjectPtr->GetProjectFieldBuildAreaMode();
		m_InspectionFieldBuildAreaMode = m_ProjectPtr->GetProjectInspectionFieldBuildAreaMode();

		m_ProjectPtr->GetProjectInspectionFileList(FieldList);		
		CloneFieldList(FieldList, m_FieldList);
		CloneFieldList(FieldList, m_ProjectFieldList);	

		switch ( m_ProjectParam.m_InspectionFieldBuildMode ) 
		{
		case FIELD_BUILD_MATRIX: m_BuildID=PROFIELD_FIELD_BY_PROJECT_CHK; break;
		case FIELD_BUILD_RANDOM_PANEL: m_BuildID=PROFIELD_FIELD_BY_PANEL_CHK; break;
		case FIELD_BUILD_RANDOM_BOARD: m_BuildID=PROFIELD_FIELD_BY_BOARD_CHK; break;
		//case FIELD_BUILD_RANDOM_PROJECT:
		}
		switch ( m_ProjectParam.m_InspectionFieldBuildAreaMode ) 
		{
		case FIELD_BUILD_AREA_BOARD: m_AreaID=PROFIELD_AREA_BY_BOARD_CHK; break;
		case FIELD_BUILD_AREA_COMPONENT: m_AreaID=PROFIELD_AREA_BY_COMPONENT_CHK; break;		
		}
		switch ( m_ProjectParam.m_FieldDivisionMode ) 
		{
		case FIELD_DIVISION_MASS_AREA: m_DivisionID=PROFIELD_DIVISION_MASS_AREA_CHK; break;
		case FIELD_DIVISION_DIAGONAL_LINE: m_DivisionID=PROFIELD_DIVISION_DIA_LINE_CHK; break;
		case FIELD_DIVISION_HORIZONTAL_LINE: m_DivisionID=PROFIELD_DIVISION_HOR_LINE_CHK; break;
		case FIELD_DIVISION_VERTICAL_LINE: m_DivisionID=PROFIELD_DIVISION_VER_LINE_CHK; break;
		}		
	}
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CProjectFieldConfigWnd::GetProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_FIELD_CONFIG_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_FIELD_CONFIG_WND;
	WndKey = _T("IDD_PROJECT_FIELD_CONFIG_WND");
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
	WndID = PROFIELD_AREA_BY_GROUP;
	WndKey = _T("PROFIELD_AREA_BY_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_AREA_BY_BOARD_CHK;
	WndKey = _T("PROFIELD_AREA_BY_BOARD_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_AREA_BY_COMPONENT_CHK;
	WndKey = _T("PROFIELD_AREA_BY_COMPONENT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_CHECK_BOARD_ONE_FOV_BTN;
	WndKey = _T("PROFIELD_CHECK_BOARD_ONE_FOV_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = PROFIELD_FIELD_BY_GROUP;
	WndKey = _T("PROFIELD_FIELD_BY_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_FIELD_BY_PROJECT_CHK;
	WndKey = _T("PROFIELD_FIELD_BY_PROJECT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROFIELD_FIELD_BY_PANEL_CHK;
	WndKey = _T("PROFIELD_FIELD_BY_PANEL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_FIELD_BY_BOARD_CHK;
	WndKey = _T("PROFIELD_FIELD_BY_BOARD_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = PROFIELD_DIVISION_MODE_GROUP;
	WndKey = _T("PROFIELD_DIVISION_MODE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_DIVISION_MASS_AREA_CHK;
	WndKey = _T("PROFIELD_DIVISION_MASS_AREA_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_DIVISION_DIA_LINE_CHK;
	WndKey = _T("PROFIELD_DIVISION_DIA_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_DIVISION_HOR_LINE_CHK;
	WndKey = _T("PROFIELD_DIVISION_HOR_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_DIVISION_VER_LINE_CHK;
	WndKey = _T("PROFIELD_DIVISION_VER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = PROFIELD_SECTION_FACTOR_LABEL;
	WndKey = _T("PROFIELD_SECTION_FACTOR_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_FIELD_GRAB_TIME_LABEL;
	WndKey = _T("PROFIELD_FIELD_GRAB_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PROFIELD_HOR_PATH_RAD;
	WndKey = _T("PROFIELD_HOR_PATH_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROFIELD_VER_PATH_RAD;
	WndKey = _T("PROFIELD_VER_PATH_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROFIELD_USER_PATH_RAD;
	WndKey = _T("PROFIELD_USER_PATH_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROFIELD_RUN_PATH_BTN;
	WndKey = _T("PROFIELD_RUN_PATH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PROFIELD_SHOW_FIELD_CHK;
	WndKey = _T("PROFIELD_SHOW_FIELD_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PROFIELD_SHOW_COMPONENT_CHK;
	WndKey = _T("PROFIELD_SHOW_COMPONENT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PROFIELD_FIELD_SORT_UP_BTN;
	WndKey = _T("PROFIELD_FIELD_SORT_UP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROFIELD_FIELD_SORT_DOWN_BTN;
	WndKey = _T("PROFIELD_FIELD_SORT_DOWN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROFIELD_FIELD_SORT_TOP_BTN;
	WndKey = _T("PROFIELD_FIELD_SORT_TOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PROFIELD_FIELD_SORT_BOT_BTN;
	WndKey = _T("PROFIELD_FIELD_SORT_BOT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectFieldConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_FIELD_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::ClearFieldList(std::vector<CAOIField*> &FieldList)
{
	size_t       i=0;
	CAOIField   *FieldPtr = NULL;
	const size_t FieldCount = FieldList.size();

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldList[i];
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->ClearFieldAllFrames();
		AOIObjManager.DestroyFieldObj(FieldList[i]);
		FieldList[i] = NULL;
	}
	FieldList.clear();
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::CloneFieldList(std::vector<CAOIField*> &SrcFieldList, std::vector<CAOIField*> &DstFieldList)
{
	ClearFieldList(DstFieldList);

	size_t       i=0, j=0;
	CAOIField   *SrcFieldPtr = NULL;
	CAOIField   *DstFieldPtr = NULL;
	const size_t FieldCount = SrcFieldList.size();

	for ( i=0; i<FieldCount; i++ )
	{
		SrcFieldPtr = SrcFieldList[i];
		if ( NULL == SrcFieldPtr ) { continue; }		
		SrcFieldPtr->ClearFieldAllFrames();//記得清楚影像列表, 否則會共用
		DstFieldPtr = SrcFieldPtr->CloneFieldObj();
		if ( NULL == DstFieldPtr ) { continue; }
		DstFieldList.push_back(DstFieldPtr);		
	}	
	return ;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::RedrawWnd()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }

	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC || NULL==hMemDC2 ) { return; }	

	RECT WndRect = m_ImageWndRect;
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	
	DrawFieldList(hMemDC2);	
	DrawCameraPos(hMemDC2);	
	DrawComponentList(hMemDC2);	
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::CreateBKImage(bool bResetView)
{
	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	DrawProjectMap(hDC, bResetView);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::DrawProjectMap(HDC hDC, bool bResetView)
{
	const char fnName[] = "CProjectFieldConfigWnd::DrawProjectMap";
	if ( NULL == hDC ) { return; }
	RECT         WndRect = m_ImageWndRect;
	COLORREF     BkColor = m_ImageWndBkColor;
	CAOIProject *ProjectPtr = GetProjectPtr();
	HBRUSH       hBrush = ::CreateSolidBrush(BkColor);
	::FillRect(hDC, &WndRect, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;
	if ( NULL == ProjectPtr ) 
	{	return;	}
	
	TPOINT2D   OffsetPt;
	double     ZoomScale=1.0;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_PTR  MapPtr=NULL;
	TPOINT2D   MapRes;	
	TREGION4D  MapCadRgn;
	TREGION4D  MpaStageRgnDA;	
	TREGION4D  MpaStageRgnDB;	
	unsigned int ImageIndex = ProjectPtr->GetProjectMapIndex();	
	ProjectPtr->GetProjectMapInfo_DA(MapRes, MapCadRgn, MpaStageRgnDA);
	ProjectPtr->GetProjectMapInfo_DB(MapRes, MapCadRgn, MpaStageRgnDB);
	if ( ProjectPtr->GetProjectMapPtr(ImageIndex, MapW, MapH, MapStep, BitCount, MapPtr) == false ) 
	{	return; }

	const size_t BufferSize= ImageAPI.CalcBufferSize(MapStep, MapH);
	if ( JetMemory.alloc_func(BufferSize, DstPtr, fnName, "DstPtr") == false ) 
	{	return; }

	if ( false == bResetView ) 
	{
		OffsetPt = m_ImageOffset;
		ZoomScale = m_ImageZoom;
	}
	else
	{	ImageAPI.CalcImageWndFitZoom(MapW, MapH, WndRect, 1.05, ZoomScale); }
	AOIDataCollect.ExecEnhanceDisplayImage(MapW, MapH, MapStep, BitCount, MapPtr, DstPtr);	
	ImageAPI.DrawImageToDC(hDC, MapW, MapH, MapStep, BitCount, DstPtr, WndRect, OffsetPt, ZoomScale, BkColor);	

	m_ImageW = MapW;
	m_ImageH = MapH;
	m_ImageStep = MapStep;
	m_ImageBit = BitCount;
	m_ImageZoom = ZoomScale;
	m_ImageOffset = OffsetPt;	
	m_Resolution = MapRes;
	m_ImageStageRgnDA = MpaStageRgnDA;
	m_ImageStageRgnDB = MpaStageRgnDB;

	JetMemory.free_func(DstPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::DrawFieldList(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	BOOL bShow = CWnd::IsDlgButtonChecked(PROFIELD_SHOW_FIELD_CHK);
	if ( FALSE == bShow ) { return; }

	RECT         WndRect = m_ImageWndRect;		

	size_t       i=0;
	CString      str;
	POINT        WndCp;
	bool         bShowText=true;
	SIZE         WndSize;
	SIZE         WndSize2;
	TSIZE2D      FieldSize;
	TPOINT2D     WndPos;
	TPOINT2D     ImagePos;
	TPOINT2D     FieldPos;
	TPOINT2D     ImageStageCpDA;
	TPOINT2D     ImageStageCpDB;
	DISTRICT_ID  DistrictID;
	CAOIField   *FieldPtr = NULL;
	const int    TextOffsetX=4;
	const int    TextOffsetY=4;
	double       ZoomScale=m_ImageZoom;
	TPOINT2D     OffsetPt = m_ImageOffset;;	
	const size_t FieldCount = m_FieldList.size();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF clr1 = SystemParam.m_DistrictColor1;
	const COLORREF clr2 = SystemParam.m_DistrictColor2;
	const COLORREF clrSel = SystemParam.m_ComponentSelectedColor;
	const COLORREF clrText = SystemParam.m_ComponentTextColor;

	HPEN hPenDA = ::CreatePen(PS_DOT, 1, clr1);
	HPEN hPenDB = ::CreatePen(PS_DOT, 1, clr2);
	HPEN hPenSel = ::CreatePen(PS_SOLID, 2, 0x241CED);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenDA));
	
	COLORREF TextColorOld = ::SetTextColor(hDC, 0xFFFFFF);
	ImageStageCpDA.x = m_ImageStageRgnDA.GetCpX();
	ImageStageCpDA.y = m_ImageStageRgnDA.GetCpY();
	ImageStageCpDB.x = m_ImageStageRgnDB.GetCpX();
	ImageStageCpDB.y = m_ImageStageRgnDB.GetCpY();
	for ( i=0; i<FieldCount; i++ )
	{	
		FieldPtr = m_FieldList[i];
		if ( NULL == FieldPtr ) { continue; }		
		FieldPos.x = FieldPtr->GetFieldStagePosX();
		FieldPos.y = FieldPtr->GetFieldStagePosY();
		DistrictID = FieldPtr->GetFieldDistrictID();
		FieldSize.cx = FieldPtr->GetFieldSizeW_Inner();
		FieldSize.cy = FieldPtr->GetFieldSizeH_Inner();

		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			AOIDataCollect.MapStagePtToCamera(m_ImageW, m_ImageH, m_Resolution, FieldPos, ImageStageCpDA, ImagePos);	
			break;
		case DISTRICT_ID_B:
			AOIDataCollect.MapStagePtToCamera(m_ImageW, m_ImageH, m_Resolution, FieldPos, ImageStageCpDB, ImagePos);	
			break;
		}		
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, WndRect, OffsetPt, ZoomScale, ImagePos, WndPos);

		WndCp.x = (int)(WndPos.x);
		WndCp.y = (int)(WndPos.y);
		WndSize.cx = (int)(FieldSize.cx/m_Resolution.x/ZoomScale);
		WndSize.cy = (int)(FieldSize.cy/m_Resolution.y/ZoomScale);
		WndSize2.cx = WndSize.cx/2;
		WndSize2.cy = WndSize.cy/2;

		if ( FieldPtr->GetFieldSelected() == false ) 
		{	
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	::SelectObject(hDC, hPenDA);	break;
			case DISTRICT_ID_B:	::SelectObject(hDC, hPenDB);	break;
			}
		}
		else
		{	::SelectObject(hDC, hPenSel);	}
		ImageAPI.DrawRectLine(hDC, WndCp, WndSize2.cx, WndSize2.cy);

		if ( true == bShowText )
		{
			str.Format(_T("%d"), i+1);
			::TextOut(hDC, WndCp.x-WndSize2.cx+TextOffsetX, WndCp.y-WndSize2.cy+TextOffsetY, str, str.GetLength());
		}
	}

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenDA); hPenDA=NULL;	
	::DeleteObject(hPenDB); hPenDB=NULL;	
	::DeleteObject(hPenSel); hPenSel=NULL;		
	::SetTextColor(hDC, TextColorOld);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::DrawCameraPos(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	//CAOIProject *ProjectPtr = GetProjectPtr();
	//if ( NULL == ProjectPtr ) { return; }	

	RECT         WndRect = m_ImageWndRect;		

	size_t       i=0;
	CString      str;
	POINT        WndCp;
	bool         bShowText=true;
	SIZE         WndSize;
	SIZE         WndSize2;
	TSIZE2D      FovSize;
	TPOINT2D     WndPos;
	TPOINT2D     ImagePos;
	TPOINT2D     CameraPos;
	TPOINT2D     ImageStageCp;		
	TREGION4D    ImageStageRgn;
	double       ZoomScale=m_ImageZoom;
	TPOINT2D     OffsetPt = m_ImageOffset;;		
	DISTRICT_ID  DistrictID = GetDistrictID();

	HPEN hPen = ::CreatePen(PS_SOLID, 2, 0x20FFFF);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	ImageStageRgn = m_ImageStageRgnDA;	break;
	case DISTRICT_ID_B:	ImageStageRgn = m_ImageStageRgnDB;	break;
	}
	ImageStageCp.x = ImageStageRgn.GetCpX();
	ImageStageCp.y = ImageStageRgn.GetCpY();

	MotionCtrlPtr->GetCurrentPos(CameraPos.x, CameraPos.y);
	AOIDataCollect.GetFovSizeOuter(FovSize.cx, FovSize.cy);
	AOIDataCollect.MapStagePtToCamera(m_ImageW, m_ImageH, m_Resolution, CameraPos, ImageStageCp, ImagePos);		
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, WndRect, OffsetPt, ZoomScale, ImagePos, WndPos);

	WndCp.x = (int)(WndPos.x);
	WndCp.y = (int)(WndPos.y);
	WndSize.cx = (int)(FovSize.cx/m_Resolution.x/ZoomScale);
	WndSize.cy = (int)(FovSize.cy/m_Resolution.y/ZoomScale);
	WndSize2.cx = WndSize.cx/2;
	WndSize2.cy = WndSize.cy/2;
	ImageAPI.DrawRectLine(hDC, WndCp, WndSize2.cx, WndSize2.cy);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;		
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::DrawComponentList(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	BOOL bShow = CWnd::IsDlgButtonChecked(PROFIELD_SHOW_COMPONENT_CHK);
	if ( FALSE == bShow ) { return; }

	RECT         WndRect = m_ImageWndRect;		

	size_t       i=0;
	CString      str;
	RECT         ComponentRect;	
	bool         bShowText=false;	
	POINT        nWndCornerPos[4];	
	TPOINT2D     ImageStageCpDA;
	TPOINT2D     ImageStageCpDB;
	TPOINT2D     WndCornerPos[4];
	TPOINT2D     ImageCornerPos[4];
	TPOINT2D     ComponentCornerPos[4];
	DISTRICT_ID  DistrictID;
	CAOIComponent *ComponentPtr = NULL;
	const int    TextOffsetX=4;
	const int    TextOffsetY=4;
	double       ZoomScale=m_ImageZoom;
	TPOINT2D     OffsetPt = m_ImageOffset;	
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF clr1 = SystemParam.m_ComponentColor1;
	const COLORREF clr2 = SystemParam.m_ComponentColor2;
	const COLORREF clrSel = SystemParam.m_ComponentSelectedColor;
	const COLORREF clrText = SystemParam.m_ComponentTextColor;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	
	COLORREF TextColorOld = ::SetTextColor(hDC, clrText);
	ImageStageCpDA.x = m_ImageStageRgnDA.GetCpX();
	ImageStageCpDA.y = m_ImageStageRgnDA.GetCpY();
	ImageStageCpDB.x = m_ImageStageRgnDB.GetCpX();
	ImageStageCpDB.y = m_ImageStageRgnDB.GetCpY();
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		DistrictID = ComponentPtr->GetComponentDistrictID();
		ComponentPtr->GetComponentRoiStageCornerPos(ComponentCornerPos);
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			AOIDataCollect.MapStageCornerToCamera(m_ImageW, m_ImageH, m_Resolution, ComponentCornerPos, ImageStageCpDA, ImageCornerPos);	
			break;
		case DISTRICT_ID_B:
			AOIDataCollect.MapStageCornerToCamera(m_ImageW, m_ImageH, m_Resolution, ComponentCornerPos, ImageStageCpDB, ImageCornerPos);
			break;
		}
		ImageAPI.MapImageCornerPtToWndCornerPt_DBL(m_ImageW, m_ImageH, WndRect, OffsetPt, ZoomScale, ImageCornerPos, WndCornerPos);		
		ImageAPI.DrawPolyLine(hDC, WndCornerPos, 4);		
		if ( true == bShowText )
		{
			str = ComponentPtr->GetComponentName();
			JetAPI::CornerPt2DToCornerPt(WndCornerPos, nWndCornerPos);
			JetAPI::CornerPtToRect(nWndCornerPos, ComponentRect);			
			::TextOut(hDC, ComponentRect.left+TextOffsetX, ComponentRect.top+TextOffsetY, str, str.GetLength());
		}
	}

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;
	::SetTextColor(hDC, TextColorOld);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( this != CWnd::GetCapture() )
	{	return; }

	POINT OffsetPt;	
	OffsetPt.x = point.x-m_LastMovePt.x;
	OffsetPt.y = point.y-m_LastMovePt.y;
	if (nFlags&MK_RBUTTON)
	{
		m_ImageOffset.x += OffsetPt.x;
		m_ImageOffset.y += OffsetPt.y;
		CreateBKImage(false);
		RedrawWnd();
	}
	m_LastMovePt= point;
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CProjectFieldConfigWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;		
	CreateBKImage(false);
	RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( ::PtInRect(&m_ImageWndRect, point) == FALSE ) 
	{	return; }	
	SetCapture();
	m_LastMovePt = m_LBtnUpPt = m_LBtnDownPt = point;		
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_LBtnUpPt = point;
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL != ProjectPtr ) 
	{
		UpdateUIToParam();
		str = _T("Do you want to change the field setting?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{
			ProjectPtr->SetProjectParameter(m_ProjectParam);
			ProjectPtr->SetProjectInspectionFileList(m_ProjectFieldList);	
		}
		else
		{	ProjectPtr->ModifyProjectInspectionFileList(m_FieldList);		}
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL != ProjectPtr ) 
	{	
		ProjectPtr->SetProjectParameter(m_ProjectParam);
		ProjectPtr->SetProjectFieldBuildMode(m_FieldBuildMode);	
		ProjectPtr->SetProjectInspectionFieldBuildMode(m_InspectionFieldBuildMode);
		ProjectPtr->SetProjectFieldBuildAreaMode(m_FieldBuildAreaMode);
		ProjectPtr->SetProjectInspectionFieldBuildAreaMode(m_InspectionFieldBuildAreaMode);
		ProjectPtr->SetProjectInspectionFileList(m_ProjectFieldList);	
	}
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::SetFieldInfo(LPCTSTR str)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	CWnd::SetDlgItemText(PROFIELD_INFO_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::EnableFieldAreaWnd(bool bEnable)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	JetAPI::EnableCtrlWnd(this, PROFIELD_AREA_BY_BOARD_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, PROFIELD_AREA_BY_COMPONENT_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, PROFIELD_CHECK_BOARD_ONE_FOV_BTN, bEnable);	
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::EnableFieldDivisionWnd(bool bEnable)
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	JetAPI::EnableCtrlWnd(this, PROFIELD_DIVISION_MASS_AREA_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, PROFIELD_DIVISION_DIA_LINE_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, PROFIELD_DIVISION_HOR_LINE_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, PROFIELD_DIVISION_VER_LINE_CHK, bEnable);
	return;
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::ChangeProjectFieldType(UINT AreaID, UINT BuildID, UINT DivisionID)
{
	FIELD_BUILD_MODE FieldBuildMode=FIELD_BUILD_RETURN;//區域建立模式	
	FIELD_BUILD_AREA_MODE FieldAreaMode=FIELD_BUILD_AREA_NONE;
	FIELD_DIVISION_MODE FieldDivisionMode=FIELD_DIVISION_RETURN;
	switch ( AreaID )
	{
	case PROFIELD_AREA_BY_BOARD_CHK:	FieldAreaMode=FIELD_BUILD_AREA_BOARD;	break;
	case PROFIELD_AREA_BY_COMPONENT_CHK:	FieldAreaMode=FIELD_BUILD_AREA_COMPONENT;	break;
	}
	switch ( BuildID )
	{
	case PROFIELD_FIELD_BY_PROJECT_CHK:	FieldBuildMode=FIELD_BUILD_MATRIX;	break;
	case PROFIELD_FIELD_BY_PANEL_CHK:	FieldBuildMode=FIELD_BUILD_RANDOM_PANEL;	break;		
	case PROFIELD_FIELD_BY_BOARD_CHK:	FieldBuildMode=FIELD_BUILD_RANDOM_BOARD;	break;		
	//case FIELD_BUILD_RANDOM_PROJECT:
	}
	switch ( DivisionID )
	{
	case PROFIELD_DIVISION_MASS_AREA_CHK:	FieldDivisionMode=FIELD_DIVISION_MASS_AREA;	break;
	case PROFIELD_DIVISION_DIA_LINE_CHK:	FieldDivisionMode=FIELD_DIVISION_DIAGONAL_LINE;	break;		
	case PROFIELD_DIVISION_HOR_LINE_CHK:	FieldDivisionMode=FIELD_DIVISION_HORIZONTAL_LINE;	break;		
	case PROFIELD_DIVISION_VER_LINE_CHK:	FieldDivisionMode=FIELD_DIVISION_VERTICAL_LINE;	break;		
	}

	CWnd::CheckDlgButton(AreaID, TRUE);
	CWnd::CheckDlgButton(BuildID, TRUE);
	CWnd::CheckDlgButton(DivisionID, TRUE);	

	if ( PROFIELD_AREA_BY_BOARD_CHK != AreaID ) 
	{	CWnd::CheckDlgButton(PROFIELD_AREA_BY_BOARD_CHK, FALSE); }
	if ( PROFIELD_AREA_BY_COMPONENT_CHK != AreaID ) 
	{	CWnd::CheckDlgButton(PROFIELD_AREA_BY_COMPONENT_CHK, FALSE); }	

	if ( PROFIELD_FIELD_BY_PROJECT_CHK != BuildID ) 
	{	CWnd::CheckDlgButton(PROFIELD_FIELD_BY_PROJECT_CHK, FALSE); }
	if ( PROFIELD_FIELD_BY_PANEL_CHK != BuildID ) 
	{	CWnd::CheckDlgButton(PROFIELD_FIELD_BY_PANEL_CHK, FALSE); }
	if ( PROFIELD_FIELD_BY_BOARD_CHK != BuildID ) 
	{	CWnd::CheckDlgButton(PROFIELD_FIELD_BY_BOARD_CHK, FALSE); }

	if ( PROFIELD_DIVISION_MASS_AREA_CHK != DivisionID ) 
	{	CWnd::CheckDlgButton(PROFIELD_DIVISION_MASS_AREA_CHK, FALSE); }
	if ( PROFIELD_DIVISION_DIA_LINE_CHK != DivisionID ) 
	{	CWnd::CheckDlgButton(PROFIELD_DIVISION_DIA_LINE_CHK, FALSE); }
	if ( PROFIELD_DIVISION_HOR_LINE_CHK != DivisionID ) 
	{	CWnd::CheckDlgButton(PROFIELD_DIVISION_HOR_LINE_CHK, FALSE); }
	if ( PROFIELD_DIVISION_VER_LINE_CHK != DivisionID ) 
	{	CWnd::CheckDlgButton(PROFIELD_DIVISION_VER_LINE_CHK, FALSE); }

	if ( FIELD_BUILD_RETURN == FieldBuildMode ) { return false; }
	if ( FIELD_DIVISION_RETURN == FieldDivisionMode ) { return false; }

	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }		
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();
	TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	const bool  bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();

	ProjectParam.m_InspectionFieldBuildMode = FieldBuildMode;
	ProjectParam.m_InspectionFieldBuildAreaMode = FieldAreaMode;	
	ProjectParam.m_FieldDivisionMode = FieldDivisionMode;

	ProjectPtr->LayoutProjectRegion();
	ProjectPtr->ClearProjectAllInspectionField();
	ProjectPtr->SetProjectAllObjToNeedToCalculateRgn(true);
	if ( false == bMultiDistrictMode ) 
	{
		if ( ProjectPtr->CreateProjectInspectionObject(FieldBuildMode, FieldAreaMode) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	}
	else
	{		
		ProjectPtr->SetProjectActDistrictID(DISTRICT_ID_A, true);
		if ( ProjectPtr->CreateProjectInspectionObject(FieldBuildMode, FieldAreaMode) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
		ProjectPtr->SetProjectActDistrictID(DISTRICT_ID_B, true);
		if ( ProjectPtr->CreateProjectInspectionObject(FieldBuildMode, FieldAreaMode) == false )
		{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
		ProjectPtr->SetProjectActDistrictID(DistrictID, true);
	}	

	if ( FIELD_BUILD_AREA_BOARD == ProjectPtr->GetProjectFieldBuildAreaMode() )
	{	ExecCheckBoardOneFovBtn();	}

	CString str;
	std::vector<CAOIField*> FieldList;
	ProjectPtr->GetProjectInspectionFileList(FieldList);
	CloneFieldList(FieldList, m_FieldList);	
	BuildFieldListWnd();
	str.Format(_T("Total Fields=%d"), m_FieldList.size());	
	SetFieldInfo(str);
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnAreaByBoardChk() 
{
	// TODO: Add your control notification handler code here
	m_AreaID = PROFIELD_AREA_BY_BOARD_CHK;	
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }		
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnAreaByComponentChk() 
{
	// TODO: Add your control notification handler code here
	m_AreaID = PROFIELD_AREA_BY_COMPONENT_CHK;	
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }		
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::ExecCheckBoardOneFovBtn()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }		

	bool bBoardOneField=true;
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const bool  bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	if ( false == bMultiDistrictMode ) 
	{
		if ( ProjectPtr->CheckProjectBoardAllObjectInOneField() == false )
		{	bBoardOneField = false;	}
	}
	else
	{		
		ProjectPtr->SetProjectActDistrictID(DISTRICT_ID_A, true);
		if ( ProjectPtr->CheckProjectBoardAllObjectInOneField() == false )
		{	bBoardOneField = false;	}
		ProjectPtr->SetProjectActDistrictID(DISTRICT_ID_B, true);
		if ( ProjectPtr->CheckProjectBoardAllObjectInOneField() == false )
		{	bBoardOneField = false;	}
		ProjectPtr->SetProjectActDistrictID(DistrictID, true);
	}
	if ( false == bBoardOneField )
	{	JetAPI::ShowMessageBox(_T("Warrning, Not Every Board In One Field"));	}			
	return bBoardOneField;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnCheckBoardOneFovBtn()
{
	// TODO: Add your control notification handler code here
	if ( ExecCheckBoardOneFovBtn() == true )
	{	JetAPI::ShowMessageBox(_T("All Board In One Field"));	}
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldByProjectChk() 
{
	// TODO: Add your control notification handler code here	
	m_BuildID = PROFIELD_FIELD_BY_PROJECT_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }
	EnableFieldAreaWnd(false);
	EnableFieldDivisionWnd(false);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldByPanelChk() 
{
	// TODO: Add your control notification handler code here
	m_BuildID = PROFIELD_FIELD_BY_PANEL_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }
	EnableFieldAreaWnd(true);
	EnableFieldDivisionWnd(true);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldByBoardChk() 
{
	// TODO: Add your control notification handler code here
	m_BuildID = PROFIELD_FIELD_BY_BOARD_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }	
	EnableFieldAreaWnd(false);
	EnableFieldDivisionWnd(true);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnDivisionMassAreaChk() 
{
	// TODO: Add your control notification handler code here
	m_DivisionID = PROFIELD_DIVISION_MASS_AREA_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }	
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnDivisionDiaLineChk() 
{
	// TODO: Add your control notification handler code here
	m_DivisionID = PROFIELD_DIVISION_DIA_LINE_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnDivisionHorLineChk() 
{
	// TODO: Add your control notification handler code here
	m_DivisionID = PROFIELD_DIVISION_HOR_LINE_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }	
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnDivisionVerLineChk() 
{
	// TODO: Add your control notification handler code here
	m_DivisionID = PROFIELD_DIVISION_VER_LINE_CHK;
	if ( ChangeProjectFieldType(m_AreaID, m_BuildID, m_DivisionID) == false ) 
	{	return ; }	
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::BuildRunPath(FIELD_PATH_MODE FieldPathMode, TPOINT2D &StartPos, double Factor, std::vector<CAOIField*> &FieldList, std::vector<TPOINT2D> &RunPathList)
{	
	size_t  i=0;
	CString         str;
	TSIZE2D FovSizeInner;
	double SectionFactor = Factor;
	TPOINT3D        FovPos;
	TJetField       JetField;
	size_t          RgnCount=0;
	CAOIField      *FieldPtr=NULL;	
	int            DefNotSet  = 0;
	const int      DefByBoard = 1;
	const int      DefByPanel = 2;
	const size_t FieldCount = FieldList.size();	
	AOIDataCollect.GetFovSizeInner(FovSizeInner.cx, FovSizeInner.cy);

	CJetFieldDivider FieldDivider;
	const double FOVW2 = FovSizeInner.cx/2;
	const double FOVH2 = FovSizeInner.cy/2;
	FieldDivider.SetPathMode(FieldPathMode);
	FieldDivider.SetSectionFactor(SectionFactor);
	FieldDivider.SetFieldMaxW(FovSizeInner.cx);
	FieldDivider.SetFieldMaxH(FovSizeInner.cy);	
	FieldDivider.SetPathStartPosX(StartPos.x);//確定檢測的起點相同
	FieldDivider.SetPathStartPosY(StartPos.y);

	TJetFieldList JetPathList;
	TJetFieldList JetFieldList;		
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldList[i];
		if ( NULL == FieldPtr ) { continue; }			
		RgnCount = FieldPtr->GetFieldRgnPtrCount();
		//if ( 0 == RgnCount ) 
		//{	continue; }
		FovPos = FieldPtr->GetFieldStagePos();
		JetField.FieldID = i;
		JetField.UserIndex = FieldPtr->GetFieldGrabIndexByUser();
		JetField.Ptr  = NULL;
		JetField.Ptr2 = FieldPtr;
		JetField.minX = FovPos.x-FOVW2;
		JetField.maxX = FovPos.x+FOVW2;
		JetField.minY = FovPos.y-FOVH2;
		JetField.maxY = FovPos.y+FOVH2;
		JetFieldList.push_back(JetField);
	}

	if ( FieldDivider.ExecFieldPath(JetFieldList) == false )
	{
		str.Format(_T("Error, FieldDivider.ExecFieldPath Fault"));
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	TPOINT2D      FieldPos;
	TJetField    *JetPathPtr=NULL;	
	FieldDivider.CloneJetPathList(JetPathList);
	const size_t PathCount = JetPathList.size();
	for ( i=0; i<PathCount; i++ )
	{	
		JetPathPtr = &(JetPathList[i]);			
		FieldPtr = (CAOIField*)(JetPathPtr->Ptr2);
		if ( NULL == FieldPtr ) { continue; }
		FovPos = FieldPtr->GetFieldStagePos();
		FieldPos.x = FovPos.x;
		FieldPos.y = FovPos.y;
		RunPathList.push_back(FieldPos);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnRunPathBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	
	CAOIFd                *FdPtrDA=NULL;	
	CAOIFd                *FdPtrDB=NULL;	
	TPOINT2D               StartPosDA;	
	TPOINT2D               StartPosDB;
	double                 SectionFactor=1.0;
	CString                strSectionFactor;
	DISTRICT_ID DistrictID = GetDistrictID();
	std::vector<CAOIField*>  FieldListDA;
	std::vector<CAOIField*>  FieldListDB;	
	std::vector<TPOINT2D>  RunPathListDA;	
	std::vector<TPOINT2D>  RunPathListDB;		
	LANE_ID                LaneID = AOIDataCollect.GetActiveLaneID();
	TProjectParameter     &ProjectParam = ProjectPtr->GetProjectParameter();
	FIELD_PATH_MODE FieldPathMode = ProjectParam.m_FieldPathMode;
	const size_t FdCount = ProjectPtr->GetProjectFdSortedCount();		
	const bool bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	FdPtrDA = ProjectPtr->GetProjectFdLastSortedPtr(DISTRICT_ID_A);	
	FdPtrDB = ProjectPtr->GetProjectFdLastSortedPtr(DISTRICT_ID_B);	
	if ( NULL != FdPtrDA ) 
	{
		StartPosDA.x = FdPtrDA->GetFdTeachStagePosX();		
		StartPosDA.y = FdPtrDA->GetFdTeachStagePosY();		
		AOIDataCollect.MapStagePosLaneByLaneID(StartPosDA.x , StartPosDA.y, LaneID);	
	}
	if ( NULL != FdPtrDB ) 
	{	
		StartPosDB.x = FdPtrDB->GetFdTeachStagePosX();		
		StartPosDB.y = FdPtrDB->GetFdTeachStagePosY();		
		AOIDataCollect.MapStagePosLaneByLaneID(StartPosDB.x , StartPosDB.y, LaneID);	
	}

	m_FieldGrabTime = CWnd::GetDlgItemInt(PROFIELD_FIELD_GRAB_TIME_EDIT);
	CWnd::GetDlgItemText(PROFIELD_SECTION_FACTOR_EDIT, strSectionFactor);
	SectionFactor = ::_ttof(strSectionFactor);
	ProjectParam.m_FieldSectionFactor = SectionFactor;

	if ( CWnd::IsDlgButtonChecked(PROFIELD_HOR_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_HOR; }
	if ( CWnd::IsDlgButtonChecked(PROFIELD_VER_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_VER; }
	if ( CWnd::IsDlgButtonChecked(PROFIELD_USER_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_USER; }	
	ProjectParam.m_FieldPathMode = FieldPathMode;

	if ( SeparateFieldList(m_FieldList, FieldListDA, FieldListDB) == false ) 
	{	return; }
	if ( BuildRunPath(FieldPathMode, StartPosDA, SectionFactor, FieldListDA, RunPathListDA) == false ) 
	{	return; }	
	if ( BuildRunPath(FieldPathMode, StartPosDB, SectionFactor, FieldListDB, RunPathListDB) == false ) 
	{	return; }		

	size_t       i=0;
	CString      str;
	TPOINT2D     Pos;		
	double       dRunTime=0;
	HWND         hWnd = GetSafeHwnd();
	DWORD        dwTimeUs = m_FieldGrabTime;
	DWORD        dwTimeMs = dwTimeUs/1000;
	bool         OfflineMode=false;
	LARGE_INTEGER fnEnd, fnStart;
	const BOOL   SkipMsg = TRUE;
	const size_t RunCountDA = RunPathListDA.size();
	const size_t RunCountDB = RunPathListDB.size();
	MOTION_MOVING_MODE MotionMovingMode=MOTION_MOVING_GO_STOP;
#ifdef OFFLINE_VERSION
	OfflineMode = true;
#endif//OFFLINE_VERSION
	
	SetDistrictID(DISTRICT_ID_A);
	MotionCtrlPtr->XYMoveTo(StartPosDA.x, StartPosDA.y, OfflineMode, MotionMovingMode);
	MotionCtrlPtr->WaitForMotionStop();
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<RunCountDA; i++ )
	{
		Pos = RunPathListDA[i];
		MotionCtrlPtr->XYMoveTo(Pos.x, Pos.y, OfflineMode, MotionMovingMode);
		if ( MotionCtrlPtr->WaitForMotionStop() == false ) 
		{
			str = MotionCtrlPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			break;
		}
		if ( dwTimeMs > 0 ) 
		{	
			//JetAPI::TimeDelay_TickCount(dwTimeMs);	
			JetAPI::SleepMessage(dwTimeMs, SkipMsg, hWnd);
		}
		RedrawWnd();		
	}

	if ( true == bMultiDistrictMode )
	{
		SetDistrictID(DISTRICT_ID_B);
		MotionCtrlPtr->XYMoveTo(StartPosDB.x, StartPosDB.y, OfflineMode, MotionMovingMode);
		MotionCtrlPtr->WaitForMotionStop();
		JetAPI::SetFuncTimeStart(fnStart);
		for ( i=0; i<RunCountDB; i++ )
		{
			Pos = RunPathListDB[i];
			MotionCtrlPtr->XYMoveTo(Pos.x, Pos.y, OfflineMode, MotionMovingMode);
			if ( MotionCtrlPtr->WaitForMotionStop() == false ) 
			{
				str = MotionCtrlPtr->GetErrorString();
				JetAPI::ShowMessageBox(str);
				break;
			}
			if ( dwTimeMs > 0 ) 
			{	
				//JetAPI::TimeDelay_TickCount(dwTimeMs);	
				JetAPI::SleepMessage(dwTimeMs, SkipMsg, hWnd);
			}
			RedrawWnd();		
		}
	}
	SetDistrictID(DistrictID);
	JetAPI::SetFuncTimeEnd(fnEnd);
	dRunTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間

	str.Format(_T("Elapse Time:%.2f ms"), dRunTime);
	JetAPI::ShowMessageBox(str);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::BuildFieldListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_19 &ListCtrl = m_FieldListWnd;

		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right-Rect.left-width-32);
		str = AOIDataDefine.GetIndexText();		
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = AOIDataDefine.GetDistrictText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::BuildFieldListWnd()
{	
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return; }	
	m_StopFieldListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopFieldListBeSelected = false;

	CString str;	
	size_t                  i=0;
	size_t                  idx=0;
	int                     nItem=0;
	DISTRICT_ID             DistrictID;
	CAOIField              *FieldPtr=NULL;
	std::vector<CAOIField*> FieldList = m_FieldList;
	const size_t FieldCount = FieldList.size();

	CSortObj                SortObj;
	std::vector<CSortObj>   SortList;
	SortObj.SetSortMode(SORT_BY_CNT);
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldList[i];
		if ( NULL == FieldPtr ) { continue; }
		idx=FieldPtr->GetFieldGrabIndexByUser();
		if ( -1 == idx )
		{	idx = i; }
		FieldPtr->SetFieldGrabIndexByUser(idx);

		SortObj.SetID(i);
		SortObj.SetValueCnt(idx);
		SortList.push_back(SortObj);
	}
	const size_t SortCount = SortList.size();
	std::sort(SortList.begin(), SortList.end());	

	nItem = 0;
	m_StopFieldListBeSelected = true;
	ListCtrl.SetRedraw(FALSE);
	//for ( i=0; i<FieldCount; i++ )
	for ( i=0; i<SortCount; i++ )
	{	
		idx = i;
		SortObj = SortList[i];
		idx = SortObj.GetID();
		FieldPtr = FieldList[idx];	
		if ( NULL == FieldPtr ) { continue; }
		DistrictID = FieldPtr->GetFieldDistrictID();

		str.Format(_T("%d"), idx+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, idx);
		ListCtrl.SetItemText(nItem, 0, str);
		
		//str.Format(_T("%d"), FieldPtr->GetFieldIndex()+1);
		str = AOIDataDefine.GetDistrictIDText(DistrictID);
		ListCtrl.SetItemText(nItem, 1, str);
		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	m_StopFieldListBeSelected = false;
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnItemchangedFieldListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( true == m_StopFieldListBeSelected ) { return ; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_FieldListWnd.GetItemData(nItem);
	SelectField(SelIndex);
	RedrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CProjectFieldConfigWnd::GetDistrictID()
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::SetDistrictID(DISTRICT_ID val)
{
	m_DistrictID = val;
}
//-------------------------------------------------------------------------------------//
bool  CProjectFieldConfigWnd::SelectField(size_t idx)
{
	size_t i=0;
	CAOIField   *FieldPtr=NULL;
	std::vector<CAOIField*> FieldList = m_FieldList;
	const size_t FieldCount = FieldList.size();
	if ( idx >= FieldCount ) { return false; }

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldList[i];
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldSelected(false);

		if ( i == idx ) 
		{	FieldPtr->SetFieldSelected(true); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::SeparateFieldList(const std::vector<CAOIField*> &List, std::vector<CAOIField*> &ListA, std::vector<CAOIField*> &ListB)
{
	size_t       i=0;
	DISTRICT_ID  DistrictID;
	CAOIField   *FieldPtr = NULL;
	const size_t FieldCount = List.size();

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = List[i];
		if ( NULL == FieldPtr ) { continue; }
		DistrictID = FieldPtr->GetFieldDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			ListA.push_back(FieldPtr);
			break;
		case DISTRICT_ID_B:	
			ListB.push_back(FieldPtr);
			break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::CombinePositionList(const std::vector<TPOINT2D> &ListA, const std::vector<TPOINT2D> &ListB, std::vector<TPOINT2D> &List)
{	
	size_t       i=0;
	const size_t PosCountB = ListB.size();

	List = ListA;
	for ( i=0; i<PosCountB; i++ )
	{	List.push_back(ListB[i]);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::UpdateUIToParam()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }

	CString str;
	TProjectParameter     &ProjectParam = ProjectPtr->GetProjectParameter();
	FIELD_PATH_MODE FieldPathMode = ProjectParam.m_FieldPathMode;
	CWnd::GetDlgItemText(PROFIELD_SECTION_FACTOR_EDIT, str);	
	ProjectParam.m_FieldSectionFactor = ::_ttof(str);

	if ( CWnd::IsDlgButtonChecked(PROFIELD_HOR_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_HOR; }
	if ( CWnd::IsDlgButtonChecked(PROFIELD_VER_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_VER; }
	if ( CWnd::IsDlgButtonChecked(PROFIELD_USER_PATH_RAD) == TRUE )
	{	FieldPathMode = FIELD_PATH_SPATH_USER; }	
	ProjectParam.m_FieldPathMode = FieldPathMode;
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnShowFieldChk() 
{
	// TODO: Add your control notification handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnShowComponentChk() 
{
	// TODO: Add your control notification handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CProjectFieldConfigWnd::MoveListItem(int nItem, UINT Mode)
{
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }	
	if ( -1 == nItem ) { return false; }

	size_t i=0, idx=0;	
	int    nItemNext = nItem;
	std::vector<CAOIField*> FieldList = m_FieldList;
	const int    ItemCount = ListCtrl.GetItemCount();
	const size_t FieldCount = FieldList.size();
	const size_t SelIndex = m_FieldListWnd.GetItemData(nItem);		
	if ( SelIndex >= FieldCount ) { return false; }
	size_t       NextIndex=0;
	CAOIField   *FieldPtr=NULL;
	CAOIField   *FieldPtr2=FieldList[SelIndex];
	const size_t UserIndex=FieldPtr2->GetFieldGrabIndexByUser();

	//idx, DistrictID
	if ( PROFIELD_FIELD_SORT_UP_BTN == Mode )
	{
		if ( 0 == nItem ) { return true; }
		if ( 0 == UserIndex ) { return true; }
		if ( -1 == UserIndex )
		{	NextIndex = 0; }
		else
		{	NextIndex = UserIndex-1; }
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = FieldList[i];
			if ( NULL == FieldPtr ) { continue; }
			idx = FieldPtr->GetFieldGrabIndexByUser();
			if ( idx != NextIndex ) { continue; }
			FieldPtr->SetFieldGrabIndexByUser(UserIndex);
			break;
		}
		nItemNext = nItem-1;
		FieldPtr2->SetFieldGrabIndexByUser(NextIndex);
	}
	if ( PROFIELD_FIELD_SORT_DOWN_BTN == Mode )
	{
		if ( (ItemCount-1) == nItem ) { return true; }
		if ( (FieldCount-1) == UserIndex ) { return true; }
		NextIndex = UserIndex+1;		
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = FieldList[i];
			if ( NULL == FieldPtr ) { continue; }
			idx = FieldPtr->GetFieldGrabIndexByUser();
			if ( idx != NextIndex ) { continue; }
			FieldPtr->SetFieldGrabIndexByUser(UserIndex);
			break;
		}
		nItemNext = nItem+1;
		FieldPtr2->SetFieldGrabIndexByUser(NextIndex);
	}
	if ( PROFIELD_FIELD_SORT_TOP_BTN == Mode )
	{
		if ( 0 == nItem ) { return true; }
		if ( 0 == UserIndex ) { return true; }
		NextIndex = 0;		
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = FieldList[i];
			if ( NULL == FieldPtr ) { continue; }
			idx = FieldPtr->GetFieldGrabIndexByUser();
			if ( idx > UserIndex ) { continue; }
			idx += 1;
			FieldPtr->SetFieldGrabIndexByUser(idx);			
		}
		nItemNext = 0;
		FieldPtr2->SetFieldGrabIndexByUser(0);
	}
	if ( PROFIELD_FIELD_SORT_BOT_BTN == Mode )
	{
		if ( (ItemCount-1) == nItem ) { return true; }
		if ( (FieldCount-1) == UserIndex ) { return true; }
		NextIndex = FieldCount-1;
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = FieldList[i];
			if ( NULL == FieldPtr ) { continue; }
			idx = FieldPtr->GetFieldGrabIndexByUser();
			if ( idx < UserIndex ) { continue; }
			idx -= 1;
			FieldPtr->SetFieldGrabIndexByUser(idx);			
		}
		nItemNext = ItemCount-1;
		FieldPtr2->SetFieldGrabIndexByUser(FieldCount-1);
	}
	BuildFieldListWnd();
	ListCtrl.SetItemState(nItemNext, LVIS_SELECTED, LVIS_SELECTED);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldSortUpBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	MoveListItem(nItem, PROFIELD_FIELD_SORT_UP_BTN);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldSortDownBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	MoveListItem(nItem, PROFIELD_FIELD_SORT_DOWN_BTN);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldSortTopBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	MoveListItem(nItem, PROFIELD_FIELD_SORT_TOP_BTN);
}
//-------------------------------------------------------------------------------------//
void CProjectFieldConfigWnd::OnFieldSortBotBtn() 
{
	// TODO: Add your control notification handler code here
	CThisListCtrl_19 &ListCtrl = m_FieldListWnd;
	const int nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	MoveListItem(nItem, PROFIELD_FIELD_SORT_BOT_BTN);
}
//-------------------------------------------------------------------------------------//