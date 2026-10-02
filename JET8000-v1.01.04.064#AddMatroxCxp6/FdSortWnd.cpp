// FdSortWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "FdSortWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int       nFdSortDisable  = 0;
const int       nFdSortStart = nFdSortDisable+1;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdSortWnd dialog
//-------------------------------------------------------------------------------------//
CFdSortWnd::CFdSortWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFdSortWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFdSortWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	POINT  Pt={0,0};	
	m_Modified = false;
	m_ShowSelectLine = false;
	m_ProjectPtr = NULL;
	m_FdSortIndex = nFdSortStart;

	m_ZoomScale = 1.0;	
	m_ViewOffset.x = m_ViewOffset.y = 0;

	m_LastPos = Pt;	
	m_LBtnUpPos = Pt;
	m_LBtnDownPos = Pt;	
	m_RBtnUpPos = Pt;
	m_RBtnDownPos = Pt;	

	m_ImageIndex = 0;
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 8;
	m_ImagePtr = NULL;
	m_ImageSize = 0;
	m_ImageInfoPtr = NULL;	

	m_FdSortPickMode = FD_SORT_PICK_NONE;
	m_FdSortScopeMode = FD_SORT_SCOPE_PANEL;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFdSortWnd)
	DDX_Control(pDX, FDSRT_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFdSortWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CFdSortWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(FDSRT_FD_SORT_RESET_BTN, OnFdSortResetBtn)
	ON_BN_CLICKED(FDSRT_FD_SORT_SET_BTN, OnFdSortSetBtn)
	ON_BN_CLICKED(FDSRT_FD_SORT_STOP_BTN, OnFdSortStopBtn)
	ON_BN_CLICKED(FDSRT_FD_SORT_RESTORE_RDO, OnFdSortRestoreRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_RUN_BTN, OnFdSortRunBtn)
	ON_BN_CLICKED(FDSRT_FD_SORT_LRTB_RDO, OnFdSortLRTBRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_RLTB_RDO, OnFdSortRLTBRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_LRBT_RDO, OnFdSortLRBTRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_RLBT_RDO, OnFdSortRLBTRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_TBLR_RDO, OnFdSortTBLRRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_BTLR_RDO, OnFdSortBTLRRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_TBRL_RDO, OnFdSortTBRLRdo)
	ON_BN_CLICKED(FDSRT_FD_SORT_BTRL_RDO, OnFdSortBTRLRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdSortWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CFdSortWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	//m_ImageWnd.GetClientRect(&m_ImageWndRect);
	//m_ImageWndMemDC.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);
	//m_ImageWndMemDC2.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);
	CWnd::CheckDlgButton(FDSRT_S_PATH_CHK, TRUE);
	CWnd::SetDlgItemInt(FDSRT_GRAY_TIME_EDIT, 200);
	SwitchMultiLanguage();	
	CreateImageBuffer();
	BuildShowImage(0);
	CreateBKImage();
	BuildFdSortList();	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_ImageWnd.MoveWindow(&WndRect);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);
		m_ImageWndMemDC2.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);
	}
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
BOOL CFdSortWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:			
			OnFdSortStopBtn();
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CFdSortWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SetProjectPtr(CAOIProject *Ptr, FD_SORT_SCOPE_MODE Mode)
{
	m_ProjectPtr = Ptr;	
	SetFdSortScopeMode(Mode);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SetModified(bool val)
{
	m_Modified = val;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::GetModified() const
{
	return m_Modified;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SetShowSelectLine(bool val)
{
	m_ShowSelectLine = val;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::GetShowSelectLine() const
{
	return m_ShowSelectLine;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CFdSortWnd::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
FD_SORT_PICK_MODE CFdSortWnd::GetFdSortPickMode() const
{
	return m_FdSortPickMode;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SetFdSortPickMode(FD_SORT_PICK_MODE Mode)
{
	m_FdSortPickMode = Mode;
}
//-------------------------------------------------------------------------------------//
FD_SORT_SCOPE_MODE CFdSortWnd::GetFdSortScopeMode() const
{
	return m_FdSortScopeMode;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SetFdSortScopeMode(FD_SORT_SCOPE_MODE Mode)
{
	m_FdSortScopeMode = Mode;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_FD_SORT_WND");	
	FD_SORT_SCOPE_MODE FdSortScopeMode=GetFdSortScopeMode();
	//---------------------------------------------------------------------------------//
	WndID = IDD_FD_SORT_WND;
	WndKey = _T("IDD_FD_SORT_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	switch ( FdSortScopeMode )
	{
	case FD_SORT_SCOPE_BOARD: WndKey = AOIDataDefine.GetBoardText();	break;	
	case FD_SORT_SCOPE_PANEL: WndKey = AOIDataDefine.GetPanelText();	break;
	default: WndKey=_T(""); break;
	}
	if ( WndKey.GetLength() > 0 )
	{
		LabelText=NewLabelText;
		NewLabelText.Format(_T("%s - %s"), LabelText, WndKey);
	}
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = FDSRT_USER_SELECT_MODE_CHK;
	WndKey = _T("FDSRT_USER_SELECT_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_RESET_BTN;
	WndKey = _T("FDSRT_FD_SORT_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_GROUP;
	WndKey = _T("FDSRT_FD_SORT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_SET_BTN;
	WndKey = _T("FDSRT_FD_SORT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_STOP_BTN;
	WndKey = _T("FDSRT_FD_SORT_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_S_PATH_CHK;
	WndKey = _T("FDSRT_S_PATH_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_RUN_BTN;
	WndKey = _T("FDSRT_FD_SORT_RUN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_SET_LABEL;
	WndKey = _T("FDSRT_FD_SORT_SET_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_LRTB_RDO;
	WndKey = _T("FDSRT_FD_SORT_LRTB_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_RLTB_RDO;
	WndKey = _T("FDSRT_FD_SORT_RLTB_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_LRBT_RDO;
	WndKey = _T("FDSRT_FD_SORT_LRBT_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = FDSRT_FD_SORT_RLBT_RDO;
	WndKey = _T("FDSRT_FD_SORT_RLBT_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_TBLR_RDO;
	WndKey = _T("FDSRT_FD_SORT_TBLR_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_BTLR_RDO;
	WndKey = _T("FDSRT_FD_SORT_BTLR_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_TBRL_RDO;
	WndKey = _T("FDSRT_FD_SORT_TBRL_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_BTRL_RDO;
	WndKey = _T("FDSRT_FD_SORT_BTRL_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDSRT_FD_SORT_RESTORE_RDO;
	WndKey = _T("FDSRT_FD_SORT_RESTORE_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = FDSRT_GRAY_TIME_LABEL;
	WndKey = _T("FDSRT_GRAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CFdSortWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_FD_SORT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//	
bool CFdSortWnd::BuildFdSortList()
{
	m_FdSortList.clear();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t       i=0;		
	TRECT4D      ImageRect;
	TRECT4D      StageRect;
	TPOINT2D     MapStageCp;
	CAOIFd      *FdPtr=NULL;
	TREGION4D    FdStageRgn;
	TFdSort      FdSort;
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	RECT         WndRect = m_ImageWndRect;
	TPOINT2D     OffsetPt = m_ViewOffset;	
	TPOINT2D     MapResolution=m_ImageRes;
	TREGION4D    MapStageRgn=m_ImageStageRgn;	
	double       ZoomScale = m_ZoomScale;
	FD_SORT_SCOPE_MODE FdSortScopeMode=GetFdSortScopeMode();
	const size_t FdCount = ProjectPtr->GetProjectFdCount();	
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	unsigned int ActivePanelIndex = ProjectPtr->GetProjectActivePanelIndex();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FD_SORT_SCOPE_PANEL == FdSortScopeMode )
		{
			if ( FdPtr->CheckFdIsPanelFd() == false ) { continue;	}			
		}
		if ( FD_SORT_SCOPE_BOARD == FdSortScopeMode )
		{
			if ( FdPtr->CheckFdIsBoardFd() == false ) { continue;	}						
		}
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		FdPtr->GetFdRoiStageRegion(FdStageRgn);		
		//FdPtr->GetFdBodyStageRegion(FdStageRgn);		
		
		StageRect.left   = MIN(FdStageRgn.minX, FdStageRgn.maxX);
		StageRect.top    = MIN(FdStageRgn.minY, FdStageRgn.maxY);
		StageRect.right  = MAX(FdStageRgn.minX, FdStageRgn.maxX);
		StageRect.bottom = MAX(FdStageRgn.minY, FdStageRgn.maxY);
		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);

		FdSort.FdPtr = FdPtr;		
		FdSort.uFdIndex = (unsigned int)(i);
		FdSort.bVisibled = true;
		FdSort.bSelected = false;
		FdSort.bDelected = false;		
		FdSort.rcFdMapRect = ImageRect;	
		FdSort.nFdSortIndex = (int)(FdPtr->GetFdSortID());				
		FdSort.nFdSortIndexBefore = FdSort.nFdSortIndex;		
		m_FdSortList.push_back(FdSort);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CFdSortWnd::GetFdSortCount()
{
	return m_FdSortList.size();
}
//-------------------------------------------------------------------------------------//
TFdSort* CFdSortWnd::GetFdSortPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck ) 
	{
		const size_t Count=m_FdSortList.size();
		if ( idx >= Count ) { return NULL; }
	}
	return &(m_FdSortList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ResetFdSortList(bool bResetAll)
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();		
	m_FdSortIndex = nFdSortStart;
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }				
		FdSortPtr->nFdSortIndex = nFdSortDisable;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ResetFdSortListSelected()
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }				
		FdSortPtr->bSelected = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::FillInFdSortList()//填入尚未設定的單板排序 
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }			
		if ( FdSortPtr->nFdSortIndex >= nFdSortStart ) { continue; }
		FdSortPtr->nFdSortIndex = m_FdSortIndex;
		m_FdSortIndex ++;
	}		
	UpdateFdSortCount(m_FdSortIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::RestoreFdSortList()
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }
		FdSortPtr->nFdSortIndex = FdSortPtr->nFdSortIndexBefore;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::IncludeVisibledFdSortList()
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }		
		FdSortPtr->bIncluded = FdSortPtr->bVisibled;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::IncludeSelectedFdSortList()
{
	size_t         i=0;
	TFdSort	      *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }		
		FdSortPtr->bIncluded = FdSortPtr->bSelected;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::UpdateFdSortCount(int val)
{
	CString str;
	str.Format(_T("%d"), val);
	CWnd::SetDlgItemText(FDSRT_FD_SORT_SET_EDIT, str);
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ExecAutoFdSortFn(FD_SORT_SEARCH_MODE Mode, FD_SORT_SEARCH_MODE SubMode)
{	
	TFdSort *FdSortPtrFirst = NULL;
	TFdSort *FdSortPtrNext  = NULL;	
	FD_SORT_SEARCH_MODE TempMode = Mode;
	FD_SORT_SEARCH_MODE NextMode = Mode;
	const BOOL bSPath = CWnd::IsDlgButtonChecked(FDSRT_S_PATH_CHK);
	const BOOL bUserSelectMode = CWnd::IsDlgButtonChecked(FDSRT_USER_SELECT_MODE_CHK);	

	if ( FALSE == bUserSelectMode )
	{	IncludeVisibledFdSortList(); }
	else
	{	IncludeSelectedFdSortList(); }

	SetModified(true);
	ResetFdSortList(false);	

	switch ( Mode )
	{
	case FD_SORT_SEARCH_LEFT://最左邊
		NextMode = FD_SORT_SEARCH_RIGHT;
		break;
	case FD_SORT_SEARCH_TOP://最上面
		NextMode = FD_SORT_SEARCH_BOTTOM;
		break;
	case FD_SORT_SEARCH_RIGHT://最左邊
		NextMode = FD_SORT_SEARCH_LEFT;
		break;
	case FD_SORT_SEARCH_BOTTOM://最上面
		NextMode = FD_SORT_SEARCH_TOP;
		break;
	}
	TempMode = Mode;
	m_FdSortIndex = nFdSortStart;
	do 
	{
		//FdSortPtrFirst = SearchFirstFdSortPtr(TempMode, SubMode);//For Board Sort
		FdSortPtrFirst = SearchFirstFdSortPtr(SubMode, TempMode);//For Fd Sort
		if ( NULL == FdSortPtrFirst ) { break; }		
		FdSortPtrFirst->nFdSortIndex = m_FdSortIndex;
		m_FdSortIndex++;

		do
		{
			FdSortPtrNext = SearchNextFdSortPtr(FdSortPtrFirst, NextMode);
			if ( NULL == FdSortPtrNext ) { break; }			
			FdSortPtrNext->nFdSortIndex = m_FdSortIndex;			
			FdSortPtrFirst = FdSortPtrNext;
			m_FdSortIndex++;
		} while (true);

		//S-Path
		if ( TRUE == bSPath )
		{
			switch ( TempMode )
			{		
			case FD_SORT_SEARCH_LEFT://最左邊
				TempMode = FD_SORT_SEARCH_RIGHT;
				break;
			case FD_SORT_SEARCH_TOP://最上面
				TempMode = FD_SORT_SEARCH_BOTTOM;
				break;
			case FD_SORT_SEARCH_RIGHT://最左邊
				TempMode = FD_SORT_SEARCH_LEFT;
				break;
			case FD_SORT_SEARCH_BOTTOM://最上面
				TempMode = FD_SORT_SEARCH_TOP;
				break;		
			}
			switch ( TempMode )
			{
			case FD_SORT_SEARCH_LEFT://最左邊
				NextMode = FD_SORT_SEARCH_RIGHT;
				break;
			case FD_SORT_SEARCH_TOP://最上面
				NextMode = FD_SORT_SEARCH_BOTTOM;
				break;
			case FD_SORT_SEARCH_RIGHT://最左邊
				NextMode = FD_SORT_SEARCH_LEFT;
				break;
			case FD_SORT_SEARCH_BOTTOM://最上面
				NextMode = FD_SORT_SEARCH_TOP;
				break;
			}
		}
	} while ( true );
	UpdateFdSortCount(m_FdSortIndex);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
TFdSort* CFdSortWnd::SearchNextFdSortPtr(TFdSort *RefPtr, FD_SORT_SEARCH_MODE Mode)
{
	if ( NULL == RefPtr ) { return NULL; }

	size_t        i = 0;
	size_t        MinIndex = -1;
	TRECT4D       RefRect=RefPtr->rcFdMapRect;
	TFdSort      *FdSortPtr = NULL;
	const size_t  FdSortCount = GetFdSortCount();		
	
	double Min = 0, Now = 0;
	double CPX = 0, CPY = 0;
	double RCPX = RefRect.GetCpX();
	double RCPY = RefRect.GetCpY();	
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		//if ( false == FdSortPtr->bVisibled ) { continue; }		
		if ( false == FdSortPtr->bIncluded ) { continue; }
		if ( FdSortPtr->nFdSortIndex >= nFdSortStart ) { continue; }
		if ( FdSortPtr == RefPtr ) { continue; }
		CPX = FdSortPtr->rcFdMapRect.GetCpX();
		CPY = FdSortPtr->rcFdMapRect.GetCpY();
		if ( FD_SORT_SEARCH_LEFT==Mode || FD_SORT_SEARCH_RIGHT==Mode )
		{
			//剔除超過此單板上下範圍
			if ( CPY < RefRect.top  ) { continue; }
			if ( CPY > RefRect.bottom ) { continue; }
			if ( FD_SORT_SEARCH_LEFT==Mode )//找比此單板更左邊的
			{	if ( CPX > RefRect.right ) { continue; }	}
			else if ( FD_SORT_SEARCH_RIGHT==Mode )//找比此單板更右邊的
			{	if ( CPX < RefRect.left ) { continue; }			}
		}
		else if ( FD_SORT_SEARCH_TOP==Mode || FD_SORT_SEARCH_BOTTOM==Mode )
		{
			//剔除超過此單板左右範圍
			if ( CPX < RefRect.left ) { continue; }	
			if ( CPX > RefRect.right ) { continue; }
			if ( FD_SORT_SEARCH_TOP==Mode )//找比此單板更上面的
			{	if ( CPY > RefRect.bottom ) { continue; }	}
			if ( FD_SORT_SEARCH_BOTTOM==Mode )//找比此單板更下面的
			{	if ( CPY < RefRect.top ) { continue; }	}
		}
		
		//Now = ::abs(CPX-RCPX)+::abs(CPY-RCPY);
		if ( FD_SORT_SEARCH_LEFT==Mode || FD_SORT_SEARCH_RIGHT==Mode )
		{	Now = ::abs(CPX-RCPX);	}
		else if ( FD_SORT_SEARCH_TOP==Mode || FD_SORT_SEARCH_BOTTOM==Mode )
		{	Now = ::abs(CPY-RCPY);	}
		if ( -1==MinIndex || Now<Min ) 
		{ 
			Min = Now;
			MinIndex = i;
		}		
	}
	if ( -1 == MinIndex ) { return NULL; }
	FdSortPtr = GetFdSortPtr(MinIndex, true);
	return FdSortPtr;
}
//-------------------------------------------------------------------------------------//
TFdSort* CFdSortWnd::SearchLimitFdSortPtr(TFdSort *RefPtr, FD_SORT_SEARCH_MODE Mode)
{
	if ( NULL == RefPtr ) { return NULL; }

	size_t        i = 0;
	double        CPX=0, CPY=0;	
	size_t        ResultIndex = -1;
	TRECT4D       ResultRect;
	TRECT4D       RefRect=RefPtr->rcFdMapRect;
	TFdSort      *FdSortPtr = NULL;
	const size_t  FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		//if ( false == FdSortPtr->bVisibled ) { continue; }		
		if ( true == FdSortPtr->bCounted ) { continue; }
		if ( false == FdSortPtr->bIncluded ) { continue; }
		if ( FdSortPtr->nFdSortIndex >= nFdSortStart ) { continue; }
		if ( FdSortPtr == RefPtr ) { continue; }
		CPX = FdSortPtr->rcFdMapRect.GetCpX();
		CPY = FdSortPtr->rcFdMapRect.GetCpY();
		if ( FD_SORT_SEARCH_LEFT==Mode || FD_SORT_SEARCH_RIGHT==Mode )
		{
			//剔除超過此單板上下範圍
			if ( CPY < RefRect.top  ) { continue; }
			if ( CPY > RefRect.bottom ) { continue; }
			if ( FD_SORT_SEARCH_LEFT==Mode )//找比此單板更左邊的
			{	if ( CPX > RefRect.right ) { continue; }	}
			else if ( FD_SORT_SEARCH_RIGHT==Mode )//找比此單板更右邊的
			{	if ( CPX < RefRect.left ) { continue; }	}
		}
		else if ( FD_SORT_SEARCH_TOP==Mode || FD_SORT_SEARCH_BOTTOM==Mode )
		{
			//剔除超過此單板左右範圍
			if ( CPX < RefRect.left ) { continue; }	
			if ( CPX > RefRect.right ) { continue; }
			if ( FD_SORT_SEARCH_TOP==Mode )//找比此單板更上面的
			{	if ( CPY > RefRect.bottom ) { continue; }	}
			if ( FD_SORT_SEARCH_BOTTOM==Mode )//找比此單板更下面的
			{	if ( CPY < RefRect.top ) { continue; }	}
		}	
		if ( -1 == ResultIndex ) 
		{	
			ResultIndex = i;	
			ResultRect = FdSortPtr->rcFdMapRect;
		}
		else
		{
			switch ( Mode ) 
			{
			case FD_SORT_SEARCH_LEFT://最左邊
				if ( FdSortPtr->rcFdMapRect.left < ResultRect.left ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}				
				break;
			case FD_SORT_SEARCH_TOP://最上面
				if ( FdSortPtr->rcFdMapRect.top < ResultRect.top ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			case FD_SORT_SEARCH_RIGHT://最左邊
				if ( FdSortPtr->rcFdMapRect.right > ResultRect.right ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			case FD_SORT_SEARCH_BOTTOM://最下面
				if ( FdSortPtr->rcFdMapRect.bottom > ResultRect.bottom ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			}	
		}
	}
	if ( -1 == ResultIndex ) { return NULL; }
	FdSortPtr = GetFdSortPtr(ResultIndex, true);
	return FdSortPtr;
}
//-------------------------------------------------------------------------------------//
TFdSort* CFdSortWnd::SearchFirstFdSortPtr(FD_SORT_SEARCH_MODE Mode, FD_SORT_SEARCH_MODE SubMode)
{
	size_t        i=0;
	TRECT4D       ResultRect;
	size_t        ResultIndex=-1;
	TFdSort      *FdSortPtr = NULL;
	const size_t  FdSortCount = GetFdSortCount();	
	
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		//if ( false == FdSortPtr->bVisibled ) { continue; }		
		if ( false == FdSortPtr->bIncluded ) { continue; }

		if ( FdSortPtr->nFdSortIndex >= nFdSortStart ) { continue; }		
		if ( -1 == ResultIndex ) 
		{ 
			ResultIndex = i; 
			ResultRect = FdSortPtr->rcFdMapRect;
		}
		else
		{
			switch ( Mode ) 
			{
			case FD_SORT_SEARCH_LEFT://最左邊
				if ( FdSortPtr->rcFdMapRect.left < ResultRect.left ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}				
				break;
			case FD_SORT_SEARCH_TOP://最上面
				if ( FdSortPtr->rcFdMapRect.top < ResultRect.top ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			case FD_SORT_SEARCH_RIGHT://最左邊
				if ( FdSortPtr->rcFdMapRect.right > ResultRect.right ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			case FD_SORT_SEARCH_BOTTOM://最下面
				if ( FdSortPtr->rcFdMapRect.bottom > ResultRect.bottom ) 
				{
					ResultIndex = i; 
					ResultRect = FdSortPtr->rcFdMapRect;
				}
				break;
			}			
		}
	}
	if ( -1 == ResultIndex ) { return NULL; }
	FdSortPtr = GetFdSortPtr(ResultIndex, true);
	if ( NULL == FdSortPtr ) { return NULL; }	
	TFdSort *FdSortPtrNext = NULL;

	std::vector<TFdSort*> FdCountedList;
	FdSortPtr->bCounted = true;
	FdCountedList.push_back(FdSortPtr);
	do
	{	
		FdSortPtrNext = SearchLimitFdSortPtr(FdSortPtr, SubMode);		
		if ( NULL == FdSortPtrNext ) { break; }		
		FdSortPtr = FdSortPtrNext;	

		FdSortPtr->bCounted = true;
		FdCountedList.push_back(FdSortPtr);
	} while (true);

	for ( size_t i=0; i<FdCountedList.size(); i++ )
	{	FdCountedList[i]->bCounted = false;	}
	return FdSortPtr;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::RedrawWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDrawDC=NULL;
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }
	if ( NULL == hMemDC2 ) { return; }

	RECT  Rect={0,0,0,0};
	POINT OffsetPt={0,0};	
	IMAGE_SIZE ImageW = m_ImageW;
	IMAGE_SIZE ImageH = m_ImageH;
	RECT  WndRect=m_ImageWndRect;	
	double ZoomScale = m_ZoomScale;
	BOOL bShowRectLine = TRUE;
	BOOL bShowDivideLine = TRUE;

	::IntersectClipRect(hDC, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	hDrawDC = hMemDC2;
	::BitBlt(hDrawDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );		
	
	OffsetPt.x = (int)(m_ViewOffset.x);
	OffsetPt.y = (int)(m_ViewOffset.y);	
	
	const int OldBkMode = ::SetBkMode(hDrawDC, TRANSPARENT);	
	DrawFdSortList(hDrawDC);
	DrawSelectRect(hDrawDC);
	//DrawProjectFd(hDrawDC);
	//DrawProjectPanel(hDrawDC);	
	//DrawProjectComponent(hDrawDC);
	DrawCameraRgn(hDrawDC);
	::SetBkMode(hDrawDC, OldBkMode);
	if ( hDrawDC != hDC )
	{	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hDrawDC, 0, 0, SRCCOPY );	}	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::CreateBKImage()
{
	if ( NULL == m_ImagePtr ) { return ; }

	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }	
	RECT         Rect = m_ImageWndRect;
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	IMAGE_SIZE   ImageStep = m_ImageStep;
	IMAGE_SIZE   ImageBitCount = m_BitCount;	
	IMAGE_PTR    ImagePtr = m_ImagePtr;
	if ( NULL == ImagePtr ) { return ; }
	BITMAPINFO *pInfo = m_ImageInfoPtr;
	if ( NULL == pInfo ) { return; }
	
	double    dZoom=1.0;
	TPOINT2D  OffsetPt2D;
	const int nDstX = 0;
	const int nDstY = 0;
	const int nDstW = Rect.right-Rect.left;
	const int nDstH = Rect.bottom-Rect.top;

	const int nSrcX = 0;
	const int nSrcY = 0;
	const int nSrcW = (int)(ImageW);
	const int nSrcH = (int)(ImageH);	
	COLORREF BkColor = 0x000000;
	OffsetPt2D = m_ViewOffset;
	if ( ImageAPI.SetBMPInfo(pInfo, ImageW, ImageH, ImageBitCount) == false ) { return; }
	ImageAPI.DrawImageToDC(hDC, pInfo, ImagePtr, Rect, OffsetPt2D, m_ZoomScale, BkColor);
	return;	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawSelectRect(HDC hDC)
{
	const bool bShowRect = GetShowSelectLine();
	if ( false == bShowRect ) { return ; }
	HPEN          hPen = ::CreatePen(PS_DASH, 1, 0x8040F0);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	ImageAPI.DrawRectLine(hDC, m_LBtnDownPos, m_LastPos);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawCameraRgn(HDC hDC)
{	
	if ( NULL == hDC ) { return; }
	
	CString        str;
	CString        strFd;
	size_t         i=0;
	POINT          DrawCp;
	RECT           DrawRect;
	bool           bInSide=true;
	TSIZE2D        FovSize;
	TPOINT2D       StagePos;	
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;	
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;			
	HPEN           hPen = ::CreatePen(PS_SOLID, 2, 0xFF8080);
	HPEN           hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	MotionCtrlPtr->GetCurrentPos(StagePos.x, StagePos.y);
	AOIDataCollect.GetFovSizeReal(FovSize.cx, FovSize.cy);
	StageRect.left   = StagePos.x-(FovSize.cx/2);
	StageRect.top    = StagePos.y-(FovSize.cy/2);
	StageRect.right  = StagePos.x+(FovSize.cx/2);
	StageRect.bottom = StagePos.y+(FovSize.cy/2);

	AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
	ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

	bInSide = true;
	JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
	JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
	if ( DrawRect.right < WndRect.left ) { bInSide = false; }
	if ( DrawRect.bottom < WndRect.top ) { bInSide = false; }
	if ( DrawRect.left > WndRect.right ) { bInSide = false; }
	if ( DrawRect.top > WndRect.bottom ) { bInSide = false; }
	if ( true == bInSide )
	{
		DrawCp.x = (DrawRect.left+DrawRect.right)/2;
		DrawCp.y = (DrawRect.top+DrawRect.bottom)/2;
		//::SelectObject(hDC, hPen);
		ImageAPI.DrawRectLine(hDC, DrawRect);		
	}		
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPen); hPen=NULL;		
	return; 
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawProjectFd(HDC hDC)
{	
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	
	if ( NULL == hDC ) { return; }
	
	CString        str;
	CString        strFd;
	size_t         i=0;
	POINT          DrawCp;
	RECT           DrawRect;
	TSIZE2D        BodySize;	
	TPOINT2D       StagePos;
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;	
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;		
	CAOIFd        *FdPtr = NULL;
	const size_t   FdCount = ProjectPtr->GetProjectFdCount();
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();

	HPEN          hPenA = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
	HPEN          hPenB = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
	HPEN          hPenN = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPenA));
	COLORREF      clrText = ::SetTextColor(hDC, 0x2200A0);
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	strFd = AOIDataDefine.GetFdText();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }		
		StagePos.x = FdPtr->GetFdStagePosX();
		StagePos.y = FdPtr->GetFdStagePosY();
		BodySize.cx = FdPtr->GetFdBodySizeW();
		BodySize.cy = FdPtr->GetFdBodySizeH();

		StageRect.left   = StagePos.x-(BodySize.cx/2);
		StageRect.top    = StagePos.y-(BodySize.cy/2);
		StageRect.right  = StagePos.x+(BodySize.cx/2);
		StageRect.bottom = StagePos.y+(BodySize.cy/2);

		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		DrawCp.x = (DrawRect.left+DrawRect.right)/2;
		DrawCp.y = (DrawRect.top+DrawRect.bottom)/2;

		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			::SelectObject(hDC, hPenA);	
			break;
		case DISTRICT_ID_B:
			::SelectObject(hDC, hPenB);	
			break;
		default:
			::SelectObject(hDC, hPenN);
			break;
		}			
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str.Format(_T("%s-%d"), strFd, i+1);
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenA); hPenA=NULL;
	::DeleteObject(hPenB); hPenB=NULL;
	::DeleteObject(hPenN); hPenN=NULL;	
	::SetTextColor(hDC, clrText);
	return; 
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawProjectPanel(HDC hDC)
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	
	if ( NULL == hDC ) { return; }
	
	CString        str;
	size_t         i=0;
	RECT           DrawRect;		
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;		
	TREGION4D      BoardCadRgn;
	TREGION4D      BoardStageRgn;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;
	CAOIPanel     *PanelPtr = NULL;		
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const size_t   PanelCount = ProjectPtr->GetProjectPanelCount();
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();

	COLORREF      BoardColor=SystemParam.m_BoardColor1;	
	COLORREF      TextColor=SystemParam.m_BoardTextColor;	
	HPEN          hPen = ::CreatePen(PS_SOLID, 2, BoardColor);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF      clrText = ::SetTextColor(hDC, TextColor);

	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	const int FontSize = 32;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
	hFont = CreateFontIndirect(&LogFont);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		BoardCadRgn = PanelPtr->GetPanelRgnCad();
		PanelPtr->GetPanelRgnStage(DistrictID, BoardStageRgn);		
		
		StageRect.left   = MIN(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.top    = MIN(BoardStageRgn.minY, BoardStageRgn.maxY);
		StageRect.right  = MAX(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.bottom = MAX(BoardStageRgn.minY, BoardStageRgn.maxY);

		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);		
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str.Format(_T("%d"), i+1);
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;
	::SetTextColor(hDC, clrText);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawFdSortList(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	
	CString        str;
	size_t         i=0;	
	RECT           DrawRect;
	RECT           DrawRectPre;
	POINT          DrawLine[2];	
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;		
	TREGION4D      BoardCadRgn;
	TREGION4D      BoardStageRgn;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;	
	TFdSort       *FdSortPtr = NULL;
	TFdSort       *FdSortPtrPre = NULL;
	const size_t   FdSortCount = GetFdSortCount();			
	HPEN          hPen = ::CreatePen(PS_SOLID, 1, 0x208080);
	HPEN          hPenSel = ::CreatePen(PS_SOLID, 2, 0xFFFFFF);
	HPEN          hPenLine = ::CreatePen(PS_DOT, 1, 0xF0F0F0);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF      clrText = ::SetTextColor(hDC, 0xFFFFFF);

	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	const int FontSize = 32;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
	hFont = CreateFontIndirect(&LogFont);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	

	CSortObj              SortObj;
	std::vector<CSortObj> SortList;		
	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		SortObj.SetID(i);
		SortObj.SetPtr(FdSortPtr);
		SortObj.SetValueInt(FdSortPtr->nFdSortIndex);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();

	FdSortPtrPre = NULL;
	for ( i=0; i<SortCount; i++ )
	{
		FdSortPtr = (TFdSort*)(SortList[i].GetPtr());

		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }
		
		ImageRect = FdSortPtr->rcFdMapRect;
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);		
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		if ( true == FdSortPtr->bSelected ) 
		{	::SelectObject(hDC, hPenSel);	}
		else
		{	::SelectObject(hDC, hPen);	}
		ImageAPI.DrawRectLine(hDC, DrawRect);

		if ( NULL != FdSortPtrPre )
		{
			DrawLine[0].x = (DrawRect.left+DrawRect.right)/2;
			DrawLine[0].y = (DrawRect.top+DrawRect.bottom)/2;
			DrawLine[1].x = (DrawRectPre.left+DrawRectPre.right)/2;
			DrawLine[1].y = (DrawRectPre.top+DrawRectPre.bottom)/2;

			::SelectObject(hDC, hPenLine);
			ImageAPI.DrawPolyLine(hDC, DrawLine, 2);
		}

		str.Format(_T("%d"), FdSortPtr->nFdSortIndex);
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
		DrawRectPre = DrawRect;
		FdSortPtrPre = FdSortPtr;
	}


	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPen); hPen=NULL;
	::DeleteObject(hPenSel); hPenSel=NULL;
	::DeleteObject(hPenLine); hPenLine=NULL;	
	::SetTextColor(hDC, clrText);
	return ;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::DrawProjectComponent(HDC hDC)
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	
	if ( NULL == hDC ) { return; }
	
	CString        str;
	size_t         i=0;
	POINT          DrawCp;
	RECT           DrawRect;
	TSIZE2D        BodySize;	
	TPOINT2D       StagePos;
	TRECT4D        DrawRect4D;
	TRECT4D        ImageRect;
	TRECT4D        StageRect;
	TPOINT2D       MapStageCp;	
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;		
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();

	HPEN          hPenA = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
	HPEN          hPenB = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
	HPEN          hPenN = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPenA));
	COLORREF      clrText = ::SetTextColor(hDC, 0x2200A0);
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }

		StagePos.x = ComponentPtr->GetComponentStagePosX();	
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		BodySize.cx = ComponentPtr->GetComponentBodySizeW();
		BodySize.cy = ComponentPtr->GetComponentBodySizeH();		

		StageRect.left   = StagePos.x-(BodySize.cx/2);
		StageRect.top    = StagePos.y-(BodySize.cy/2);
		StageRect.right  = StagePos.x+(BodySize.cx/2);
		StageRect.bottom = StagePos.y+(BodySize.cy/2);

		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		DrawCp.x = (DrawRect.left+DrawRect.right)/2;
		DrawCp.y = (DrawRect.top+DrawRect.bottom)/2;

		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	
			::SelectObject(hDC, hPenA);	
			break;
		case DISTRICT_ID_B:
			::SelectObject(hDC, hPenB);	
			break;
		default:
			::SelectObject(hDC, hPenN);
			break;
		}			
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str = ComponentPtr->GetComponentName();
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPenA); hPenA=NULL;
	::DeleteObject(hPenB); hPenB=NULL;
	::DeleteObject(hPenN); hPenN=NULL;	
	::SetTextColor(hDC, clrText);
	return; 
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ClearImageBuffer()
{
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 8;
	JetMemory.free_func(m_ImagePtr);
	m_ImagePtr = NULL;
	m_ImageSize = 0;
	if ( NULL != m_ImageInfoPtr ) 
	{	delete[] m_ImageInfoPtr; }
	m_ImageStageRgn=TREGION4D();
	m_ImageRes.x = m_ImageRes.y = 1.0;
	m_ImageInfoPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::CreateImageBuffer()
{
	const char fnName[] = "CFdSortWnd::CreateImageBuffer";
	ClearImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	const int  nAlign = 4;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE MapBitCount=0;
	IMAGE_PTR  MapPtr=0;
	IMAGE_SIZE ColorBitCnt=24;	
	unsigned int index = 0;
	if ( ProjectPtr->GetProjectMapPtr(index, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
	{	return false;	}
	if ( NULL == MapPtr ) 
	{	return false; }

	size_t InfoSize=0;
	if ( ImageAPI.CreateBMPInfoBuffer(m_ImageInfoPtr, InfoSize) == false ) 
	{	return false; }

	IMAGE_PTR    BufferPtr=NULL;
	IMAGE_SIZE   MaxStep = JetAPI::GetBMPImagePixelsPerLine(MapW, ColorBitCnt, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(MaxStep, MapH);
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == false ) 
	{	return false;	}
	::memset(BufferPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	m_ImagePtr = BufferPtr;
	m_ImageSize = BufferSize;	
	ProjectPtr->GetProjectMapTeachRgn(m_ImageStageRgn);	
	ProjectPtr->GetProjectMapResolution(m_ImageRes.x, m_ImageRes.y);
	m_ZoomScale = ImageAPI.CalcImageWndFitScale(MapW, MapH, m_ImageWndRect);
	return BuildShowImage(index);	
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::SwitchShowImage()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_ImagePtr ) { return false; }

	const int  nAlign = 4;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE MapBitCount=0;
	IMAGE_PTR  MapPtr=0;
	IMAGE_SIZE ColorBitCnt=24;		
	unsigned int Index = m_ImageIndex+1;
	if ( ProjectPtr->GetProjectMapPtr(Index, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
	{	Index = 0;	}
	if ( BuildShowImage(Index) == false )
	{	return false; }
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::BuildShowImage(size_t index)
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_ImagePtr ) { return false; }

	const int  nAlign = 4;
	IMAGE_SIZE MapW=0;
	IMAGE_SIZE MapH=0;
	IMAGE_SIZE MapStep=0;
	IMAGE_SIZE MapBitCount=0;
	IMAGE_PTR  MapPtr=0;
	IMAGE_SIZE ColorBitCnt=24;		
	if ( ProjectPtr->GetProjectMapPtr(index, MapW, MapH, MapStep, MapBitCount, MapPtr) == false ) 
	{	return false;	}
	if ( NULL == MapPtr ) 
	{	return false; }
	const size_t BufferSize = MapStep*MapH;
	if ( BufferSize > m_ImageSize )
	{	return false; }

	if ( AOIDataCollect.ExecEnhanceDisplayImage(MapW, MapH, MapStep, MapBitCount, MapPtr, m_ImagePtr) == false ) 
	{	return false; }

	m_ImageIndex = index;
	m_ImageW    = MapW;
	m_ImageH    = MapH;
	m_ImageStep = MapStep;
	m_BitCount  = MapBitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ChangeFdSortRadio(UINT ActID)
{
	UINT ID = 0;
	ID = FDSRT_FD_SORT_LRTB_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);	

	ID = FDSRT_FD_SORT_RLTB_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_LRBT_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_RLBT_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_TBLR_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_BTLR_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_TBRL_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_BTRL_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = FDSRT_FD_SORT_RESTORE_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ExecUpdateFdSort()//更新定位點排序
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString       str;
	size_t        i=0;		
	int           FdSortIndex=0;
	CAOIFd       *FdPtr = NULL;		
	TFdSort      *FdSortPtr=NULL;
	const size_t  FdCount = ProjectPtr->GetProjectFdCount();	
	const size_t  FdSortCount = GetFdSortCount();	
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }	
		if ( false == FdSortPtr->bVisibled ) { continue; }
		FdPtr = FdSortPtr->FdPtr;
		FdSortIndex = FdSortPtr->nFdSortIndex;
		if ( NULL == FdPtr ) { continue; }		
		FdPtr->SetFdSortID(FdSortIndex);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ExecPickFdSort(POINT Pt)
{	
	FD_SORT_PICK_MODE PickMode = GetFdSortPickMode();	
	const size_t FdSortIndex = CheckPickFdSort(Pt);
	if ( -1 == FdSortIndex ) { return false; }
	TFdSort  *FdSortPtr = NULL;
	SelectFdSort(FdSortIndex);		
	FdSortPtr = GetFdSortPtr(FdSortIndex, true);
	if ( NULL != FdSortPtr )
	{
		if ( FD_SORT_PICK_ENABLE == PickMode ) 
		{
			FdSortPtr->nFdSortIndex = m_FdSortIndex; 
			m_FdSortIndex ++;
			UpdateFdSortCount(m_FdSortIndex);
		}
	}
	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::ExecRectFdSort(RECT SelRect)
{
	CString        str;
	size_t         i=0;	
	TREGION4D      ImageRect;	
	TPOINT2D       MapStageCp;
	TRECT4D        ImgSelRect=SelRect;
	TRECT4D        WndSelRect=SelRect;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;	
	TFdSort       *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ImageAPI.MapWndRectToImageRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndSelRect, ImgSelRect);	

	if ( AOIDataCollect.CheckMultiSelectMode() == false ) 
	{	ResetFdSortListSelected();	}
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }
		ImageRect = FdSortPtr->rcFdMapRect;
		if ( ImageRect.minX > ImgSelRect.right ) { continue; }
		if ( ImageRect.minY > ImgSelRect.bottom ) { continue; }
		if ( ImageRect.maxX < ImgSelRect.left ) { continue; }
		if ( ImageRect.maxY < ImgSelRect.top  ) { continue; }

		if ( ImageRect.minX < ImgSelRect.left ) { continue; }		
		if ( ImageRect.minY < ImgSelRect.top ) { continue; }
		if ( ImageRect.maxX > ImgSelRect.right ) { continue; }
		if ( ImageRect.maxY > ImgSelRect.bottom  ) { continue; }
		
		FdSortPtr->bSelected = true;
	}	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CFdSortWnd::CheckPickFdSort(POINT Pt)
{
	CString        str;
	size_t         i=0;
	TPOINT2D       ImgPt=Pt;
	TPOINT2D       WndPt=Pt;
	TREGION4D      ImageRect;	
	TPOINT2D       MapStageCp;
	TREGION4D      MapStageRgn=m_ImageStageRgn;
	TPOINT2D       MapResolution=m_ImageRes;
	IMAGE_SIZE     ImageW = m_ImageW;
	IMAGE_SIZE     ImageH = m_ImageH;
	RECT           WndRect = m_ImageWndRect;
	TPOINT2D       OffsetPt = m_ViewOffset;
	double         ZoomScale = m_ZoomScale;	
	TFdSort       *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImgPt);	
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( false == FdSortPtr->bVisibled ) { continue; }		
		
		if ( FdSortPtr->nFdSortIndex >= nFdSortStart ) 
		{	continue; }
		
		ImageRect = FdSortPtr->rcFdMapRect;
		if ( ImgPt.x < ImageRect.minX ) { continue; }
		if ( ImgPt.y < ImageRect.minY ) { continue; }
		if ( ImgPt.x > ImageRect.maxX ) { continue; }
		if ( ImgPt.y > ImageRect.maxY ) { continue; }
		return i;
	}	
	return -1;
}
//-------------------------------------------------------------------------------------//
bool CFdSortWnd::SelectFdSort(size_t index)
{
	CString        str;
	size_t         i=0;
	TFdSort       *FdSortPtr = NULL;
	const size_t   FdSortCount = GetFdSortCount();
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		if ( i != index ) 
		{	FdSortPtr->bSelected = false;	}
		else
		{	FdSortPtr->bSelected = true;	}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(true);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, FDSRT_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_LBtnUpPos = pt;
	
	POINT DifP;
	RECT  SelRect={0,0,0,0};
	FD_SORT_PICK_MODE PickMode = GetFdSortPickMode();
	DifP.x = m_LBtnUpPos.x-m_LBtnDownPos.x;
	DifP.y = m_LBtnUpPos.y-m_LBtnDownPos.y;
	JetAPI::PointsToRect(m_LBtnDownPos, m_LBtnUpPos, SelRect);
	switch ( PickMode )
	{		
	case FD_SORT_PICK_ENABLE:
		if ( abs(DifP.x)<2 && abs(DifP.y)<2 )
		{	ExecPickFdSort(pt);	}		
		break;
	default:
		ExecRectFdSort(SelRect);
		break;
	}
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( CWnd::GetCapture() != this ) 
	{
		return CBaseDialog::OnMouseMove(nFlags, point);
	}
	POINT Dp={0,0};
	POINT pt = point;
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);	
	Dp.x = pt.x-m_LastPos.x;
	Dp.y = pt.y-m_LastPos.y;
	if ( nFlags&MK_LBUTTON )
	{	
		//ExecMoveMap(Dp.x, Dp.y);	
		RedrawWnd();
	}
	if ( nFlags&MK_RBUTTON )
	{		
		m_ViewOffset.x += Dp.x;
		m_ViewOffset.y += Dp.y;
		CreateBKImage();
		RedrawWnd();		
	}
	m_LastPos = pt;
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CFdSortWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ZoomScale;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);
	ImageAPI.CalcImageWndZoom(m_ZoomScale, NextImageZoom, m_ViewOffset);
	m_ZoomScale = NextImageZoom;		
	CreateBKImage();
	RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(false);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, FDSRT_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_RBtnUpPos = m_RBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_RBtnUpPos = pt;

	POINT DifP;	
	DifP.x = m_RBtnUpPos.x-m_RBtnDownPos.x;
	DifP.y = m_RBtnUpPos.y-m_RBtnDownPos.y;

	if ( abs(DifP.x)<2 && abs(DifP.y)<2 )
	{	
		bool SwitchFrameMode = false;		
		SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();		
		if ( true == SwitchFrameMode )
		{	SwitchShowImage(); }		
	}
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortResetBtn() 
{
	// TODO: Add your control notification handler code here
	ResetFdSortList(true);		
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortSetBtn() 
{
	// TODO: Add your control notification handler code here
	SetModified(true);
	ResetFdSortList(true);
	SetFdSortPickMode(FD_SORT_PICK_ENABLE);
	UpdateFdSortCount(m_FdSortIndex);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortStopBtn() 
{
	// TODO: Add your control notification handler code here
	FillInFdSortList();
	SetFdSortPickMode(FD_SORT_PICK_NONE); 
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortRestoreRdo() 
{
	// TODO: Add your control notification handler code here
	SetModified(false);
	RestoreFdSortList();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortRunBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return ; }

	size_t                i=0;
	CSortObj              SortObj;
	std::vector<CSortObj> SortList;
	TFdSort     *FdSortPtr=NULL;
	const size_t FdSortCount=GetFdSortCount();	
	if ( 0 == FdSortCount ) { return; }
	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<FdSortCount; i++ )
	{
		FdSortPtr = GetFdSortPtr(i, false);
		if ( NULL == FdSortPtr ) { continue; }
		SortObj.SetID(i);
		SortObj.SetPtr(FdSortPtr->FdPtr);
		SortObj.SetValueInt(FdSortPtr->nFdSortIndex);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());

	CString       str;
	TPOINT3D      FdPos;
	CAOIFd       *FdPtr=NULL;
	double        dRunTime=0;		
	HWND          hWnd = GetSafeHwnd();
	bool          OfflineMode=false;
	LARGE_INTEGER fnEnd, fnStart;
	const BOOL    SkipMsg = TRUE;
	const size_t  SortCount=SortList.size();
	DWORD         dwTimeMs = CWnd::GetDlgItemInt(FDSRT_GRAY_TIME_EDIT);
	if ( 0 == SortCount ) { return; }
#ifdef OFFLINE_VERSION
	OfflineMode = true;
#endif//OFFLINE_VERSION
	FdPtr = (CAOIFd*)(SortList[0].GetPtr());
	if ( NULL == FdPtr ) { return; }
	FdPos = FdPtr->GetFdStagePos();

	MotionCtrlPtr->XYMoveTo(FdPos.x, FdPos.y, OfflineMode, MOTION_MOVING_GO_STOP);
	MotionCtrlPtr->WaitForMotionStop();
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<SortCount; i++ )
	{
		FdPtr = (CAOIFd*)(SortList[i].GetPtr());
		if ( NULL == FdPtr ) { continue; }
		FdPos = FdPtr->GetFdStagePos();
		MotionCtrlPtr->XYMoveTo(FdPos.x, FdPos.y, OfflineMode, MOTION_MOVING_GO_STOP);
		if ( MotionCtrlPtr->WaitForMotionStop() == false ) 
		{
			str = MotionCtrlPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			break;
		}
		if ( dwTimeMs > 0 ) 
		{	JetAPI::SleepMessage(dwTimeMs, SkipMsg, hWnd);	}
		RedrawWnd();
	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	dRunTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間

	str.Format(_T("Elapse Time:%.2f ms"), dRunTime);
	JetAPI::ShowMessageBox(str);
	return ;
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortLRTBRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_LEFT, FD_SORT_SEARCH_TOP);		
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortRLTBRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_RIGHT, FD_SORT_SEARCH_TOP);	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortLRBTRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_LEFT, FD_SORT_SEARCH_BOTTOM);	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortRLBTRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_RIGHT, FD_SORT_SEARCH_BOTTOM);	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortTBLRRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_TOP, FD_SORT_SEARCH_LEFT);	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortBTLRRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_BOTTOM, FD_SORT_SEARCH_LEFT);	
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortTBRLRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_TOP, FD_SORT_SEARCH_RIGHT);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnFdSortBTRLRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoFdSortFn(FD_SORT_SEARCH_BOTTOM, FD_SORT_SEARCH_RIGHT);
}
//-------------------------------------------------------------------------------------//
void CFdSortWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	const bool bModified = GetModified();
	if ( true == bModified ) 
	{
		str = _T("Do you want to set the Fd sort?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	ExecUpdateFdSort();	 }
	}	
	SetModified(false);
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//