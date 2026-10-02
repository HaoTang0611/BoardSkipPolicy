// BoardConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BoardConfigWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int       nBoardSortDisable = -1;
const int       nBoardSortStart   = nBoardSortDisable+1;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardConfigWnd dialog
//-------------------------------------------------------------------------------------//
CBoardConfigWnd::CBoardConfigWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBoardConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoardConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	POINT  Pt={0,0};	
	m_Modified = false;
	m_ShowSelectLine = false;
	m_ProjectPtr = NULL;
	m_BoardOrderIndex = nBoardSortStart;

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

	m_BoardConfigMode=BOARD_CONFIG_NONE;
	m_BoardConfigPickMode = BOARD_CONFIG_PICK_NONE;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoardConfigWnd)
	DDX_Control(pDX, BCW_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, BCW_PANEL_LIST_WND, m_PanelListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBoardConfigWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBoardConfigWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(BCW_BOARD_SORT_SET_BTN, OnBoardSortSetBtn)
	ON_BN_CLICKED(BCW_BOARD_SORT_LRTB_RDO, OnBoardSortLRTBRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_RLTB_RDO, OnBoardSortRLTBRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_LRBT_RDO, OnBoardSortLRBTRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_RLBT_RDO, OnBoardSortRLBTRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_TBLR_RDO, OnBoardSortTBLRRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_BTLR_RDO, OnBoardSortBTLRRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_TBRL_RDO, OnBoardSortTBRLRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_BTRL_RDO, OnBoardSortBTRLRdo)
	ON_NOTIFY(NM_DBLCLK, BCW_PANEL_LIST_WND, OnDblclkPanelListWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(BCW_BOARD_SORT_RESTORE_RDO, OnBoardSortRestoreRdo)
	ON_BN_CLICKED(BCW_BOARD_SORT_STOP_BTN, OnBoardSortStopBtn)
	ON_BN_CLICKED(BCW_ALL_PANEL_MODE_CHK, OnAllPanelModeChk)
	ON_BN_CLICKED(BCW_USER_SELECT_MODE_CHK, OnUserSelectModeChk)
	ON_BN_CLICKED(BCW_BOARD_SORT_RESET_BTN, OnBoardSortResetBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBoardConfigWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(m_PanelListWnd);
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	//m_ImageWnd.GetClientRect(&m_ImageWndRect);
	//m_ImageWndMemDC.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);
	//m_ImageWndMemDC2.CreateMemDC(m_ImageWnd.GetSafeHwnd(), NULL);

	BuildPanelListWndHeader();
	SwitchMultiLanguage();	
	CreateImageBuffer();
	BuildShowImage(0);
	CreateBKImage();
	BuildBoardConfigList();
	BuildPanelListWnd();
	BuildBoardConfigListPosIndex();
	CWnd::CheckDlgButton(BCW_USE_MATRIX_MODE_CHK, m_MatrixMode);

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnSize(UINT nType, int cx, int cy) 
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
void CBoardConfigWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
	CAOIPanel  *PanelPtr = Ptr->GetProjectActivePanel();
	if ( NULL == PanelPtr ) 
	{
		PanelPtr = Ptr->GetProjectPanelPtr(0, true);
		if ( NULL != PanelPtr ) 
		{	Ptr->SetProjectActivePanel(PanelPtr); }
	}
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SetBoardConfigMode(BOARD_CONFIG_MODE Mode)
{
	m_BoardConfigMode = Mode;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SetModified(bool val)//
{
	m_Modified = val;
}
//-------------------------------------------------------------------------------------//d
bool CBoardConfigWnd::GetModified() const
{
	return m_Modified;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SetShowSelectLine(bool val)
{
	m_ShowSelectLine = val;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::GetShowSelectLine() const
{
	return m_ShowSelectLine;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CBoardConfigWnd::GetActivePanelPtr()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return NULL; }
	return ProjectPtr->GetProjectActivePanel();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CBoardConfigWnd::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
BOARD_CONFIG_MODE CBoardConfigWnd::GetBoardConfigMode() const
{
	return m_BoardConfigMode;
}
//-------------------------------------------------------------------------------------//
BOARD_CONFIG_PICK_MODE CBoardConfigWnd::GetBoardConfigPickMode() const
{
	return m_BoardConfigPickMode;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SetBoardConfigPickMode(BOARD_CONFIG_PICK_MODE Mode)
{
	m_BoardConfigPickMode = Mode;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_BOARD_CONFIG_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_BOARD_CONFIG_WND;
	WndKey = _T("IDD_BOARD_CONFIG_WND");
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
	WndID = BCW_PANEL_LIST_LABEL;
	WndKey = _T("BCW_PANEL_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_ALL_PANEL_MODE_CHK;
	WndKey = _T("BCW_ALL_PANEL_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = BCW_USER_SELECT_MODE_CHK;
	WndKey = _T("BCW_USER_SELECT_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_RESET_BTN;
	WndKey = _T("BCW_BOARD_SORT_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_GROUP;
	WndKey = _T("BCW_BOARD_SORT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_SET_BTN;
	WndKey = _T("BCW_BOARD_SORT_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_STOP_BTN;
	WndKey = _T("BCW_BOARD_SORT_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_SET_LABEL;
	WndKey = _T("BCW_BOARD_SORT_SET_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_LRTB_RDO;
	WndKey = _T("BCW_BOARD_SORT_LRTB_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_RLTB_RDO;
	WndKey = _T("BCW_BOARD_SORT_RLTB_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_LRBT_RDO;
	WndKey = _T("BCW_BOARD_SORT_LRBT_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = BCW_BOARD_SORT_RLBT_RDO;
	WndKey = _T("BCW_BOARD_SORT_RLBT_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_TBLR_RDO;
	WndKey = _T("BCW_BOARD_SORT_TBLR_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_BTLR_RDO;
	WndKey = _T("BCW_BOARD_SORT_BTLR_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_TBRL_RDO;
	WndKey = _T("BCW_BOARD_SORT_TBRL_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_BTRL_RDO;
	WndKey = _T("BCW_BOARD_SORT_BTRL_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_BOARD_SORT_RESTORE_RDO;
	WndKey = _T("BCW_BOARD_SORT_RESTORE_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = BCW_USE_S_ORDER_CHK;
	WndKey = _T("BCW_USE_S_ORDER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BCW_USE_MATRIX_MODE_CHK;
	WndKey = _T("BCW_USE_MATRIX_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CBoardConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BOARD_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::GetSPath() const//是否使用S路徑
{
	BOOL bChk=CWnd::IsDlgButtonChecked(BCW_USE_S_ORDER_CHK);
	if ( FALSE == bChk ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::GetUseMatrixMode() const//使用陣列模式(行列等數)
{
	if ( false == m_MatrixMode ) { return false; }
	BOOL bChk=CWnd::IsDlgButtonChecked(BCW_USE_MATRIX_MODE_CHK);
	if ( FALSE == bChk ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::BuildBoardConfigList()
{
	m_BoardConfigList.clear();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t       i=0;		
	TRECT4D      ImageRect;
	TRECT4D      StageRect;
	TPOINT2D     MapStageCp;
	CAOIBoard   *BoardPtr=NULL;
	TREGION4D    BoardStageRgn;
	TBoardConfig BoardConfig;
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	RECT         WndRect = m_ImageWndRect;
	TPOINT2D     OffsetPt = m_ViewOffset;	
	TPOINT2D     MapResolution=m_ImageRes;
	TREGION4D    MapStageRgn=m_ImageStageRgn;	
	double       ZoomScale = m_ZoomScale;
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	unsigned int ActivePanelIndex = ProjectPtr->GetProjectActivePanelIndex();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		BoardPtr->GetBoardRgnStage(DistrictID, BoardStageRgn);		
		
		StageRect.left   = MIN(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.top    = MIN(BoardStageRgn.minY, BoardStageRgn.maxY);
		StageRect.right  = MAX(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.bottom = MAX(BoardStageRgn.minY, BoardStageRgn.maxY);
		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);

		BoardConfig.PanelPtr = BoardPtr->GetBoardPanelPtr();
		BoardConfig.BoardPtr = BoardPtr;
		BoardConfig.uPanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardConfig.uBoardIndex = BoardPtr->GetBoardIndex_Panel();
		BoardConfig.bVisibled = true;
		BoardConfig.bSelected = false;
		BoardConfig.bDelected = false;		
		BoardConfig.rcBoardMapRect = ImageRect;	
		BoardConfig.nBoardOrderIndex = (int)(BoardConfig.uBoardIndex);				
		m_BoardConfigList.push_back(BoardConfig);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::BuildBoardConfigListPosIndex()
{	
	int PosXIndex = 0;
	int PosYIndex = 0;		
	const int nBoardSortDone = nBoardSortDisable+1;
	BOARD_ORDER_SEARCH_MODE Mode = BOARD_ORDER_SEARCH_BOTTOM;
	BOARD_ORDER_SEARCH_MODE SubMode = BOARD_ORDER_SEARCH_LEFT;	
	BOARD_ORDER_SEARCH_MODE NextMode=GetOppositeDirectionMode(SubMode);	

	m_MatrixMode = false;
	for ( size_t i=0; i<GetBoardConfigCount(); i++ )
	{
		TBoardConfig *BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		BoardConfigPtr->nBoardOrderIndex_Backup = BoardConfigPtr->nBoardOrderIndex;
		BoardConfigPtr->nBoardOrderIndex = nBoardSortDisable;
	}

	IncludeVisibledBoardConfigList();
	do 
	{
		std::vector<TBoardConfig*> BoardConfigList;
		TBoardConfig *BoardConfigPtrFirst = SearchFirstBoardConfigPtr(Mode, SubMode);
		if ( NULL == BoardConfigPtrFirst ) { break; }	
		BoardConfigPtrFirst->nBoardPosIndex_X = PosXIndex;	
		BoardConfigPtrFirst->nBoardPosIndex_Y = PosYIndex;	
		if ( BOARD_ORDER_SEARCH_BOTTOM==SubMode || BOARD_ORDER_SEARCH_TOP==SubMode )
		{	PosYIndex ++;	}
		if ( BOARD_ORDER_SEARCH_LEFT==SubMode || BOARD_ORDER_SEARCH_RIGHT==SubMode )
		{	PosXIndex ++;	}	
		BoardConfigPtrFirst->nBoardOrderIndex = nBoardSortDone;
		BoardConfigList.push_back(BoardConfigPtrFirst);
		do
		{
			TBoardConfig *BoardConfigPtrNext = SearchNextBoardConfigPtr(BoardConfigPtrFirst, NextMode);
			if ( NULL == BoardConfigPtrNext ) { break; }	
			BoardConfigList.push_back(BoardConfigPtrNext);			
			BoardConfigPtrNext->nBoardPosIndex_X = PosXIndex;	
			BoardConfigPtrNext->nBoardPosIndex_Y = PosYIndex;	
			if ( BOARD_ORDER_SEARCH_BOTTOM==NextMode || BOARD_ORDER_SEARCH_TOP==NextMode )
			{	PosYIndex ++;	}
			if ( BOARD_ORDER_SEARCH_LEFT==NextMode || BOARD_ORDER_SEARCH_RIGHT==NextMode )
			{	PosXIndex ++;	}	
			BoardConfigPtrNext->nBoardOrderIndex = nBoardSortDone;
			BoardConfigPtrFirst = BoardConfigPtrNext;			
		} while (true);
		
		if ( BOARD_ORDER_SEARCH_BOTTOM==Mode || BOARD_ORDER_SEARCH_TOP==Mode )
		{	
			PosYIndex ++;
			PosXIndex = 0;
		}
		if ( BOARD_ORDER_SEARCH_LEFT==Mode || BOARD_ORDER_SEARCH_RIGHT==Mode )
		{	
			PosXIndex ++;
			PosYIndex = 0;
		}
	} while ( true );

	for ( size_t i=0; i<GetBoardConfigCount(); i++ )
	{
		TBoardConfig *BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		BoardConfigPtr->nBoardOrderIndex = BoardConfigPtr->nBoardOrderIndex_Backup;
		BoardConfigPtr->nBoardOrderIndex_Backup = nBoardSortDisable;
	}
	

	int PosIndexMax_X = -1;
	int PosIndexMax_Y = -1;
	for ( size_t i=0; i<GetBoardConfigCount(); i++ )
	{
		TBoardConfig *BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		if ( PosIndexMax_X < BoardConfigPtr->nBoardPosIndex_X )
		{	PosIndexMax_X = BoardConfigPtr->nBoardPosIndex_X;	}
		if ( PosIndexMax_Y < BoardConfigPtr->nBoardPosIndex_Y )
		{	PosIndexMax_Y = BoardConfigPtr->nBoardPosIndex_Y;	}
	}

	if ( PosIndexMax_X>=0 && PosIndexMax_Y>=0 )
	{
		m_MatrixMode = true;
		for ( int j=0; j<PosIndexMax_Y; j++ )
		{
			bool bFindX = false;
			for ( size_t i=0; i<GetBoardConfigCount(); i++ )
			{
				TBoardConfig *BoardConfigPtr = GetBoardConfigPtr(i, false);
				if ( NULL == BoardConfigPtr ) { continue; }		
				if ( j != BoardConfigPtr->nBoardPosIndex_Y ) { continue; }
				if ( PosIndexMax_X != BoardConfigPtr->nBoardPosIndex_X ) { continue; }
				bFindX = true;
				break;
			}
			if ( false == bFindX )
			{
				m_MatrixMode = false;
				break;
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CBoardConfigWnd::GetBoardConfigCount()
{
	return m_BoardConfigList.size();
}
//-------------------------------------------------------------------------------------//
TBoardConfig* CBoardConfigWnd::GetBoardConfigPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck ) 
	{
		const size_t Count=m_BoardConfigList.size();
		if ( idx >= Count ) { return NULL; }
	}
	return &(m_BoardConfigList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::UpdateBoardConfigList(CAOIPanel *PanelPtr)
{	
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();
	const BOOL     AllPanelMode = CWnd::IsDlgButtonChecked(BCW_ALL_PANEL_MODE_CHK);	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( NULL==PanelPtr || TRUE==AllPanelMode ) 
		{	BoardConfigPtr->bVisibled = true;	}
		else
		{
			if ( BoardConfigPtr->PanelPtr != PanelPtr ) 
			{	BoardConfigPtr->bVisibled = false; }
			else
			{	BoardConfigPtr->bVisibled = true; }
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ResetBoardConfigList_Order(bool bResetAll)
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();
		
	m_BoardOrderIndex = nBoardSortStart;
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( true == bResetAll )
		{
			if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		}
		else
		{
			if ( false == BoardConfigPtr->bIncluded ) { continue; }		
		}
		BoardConfigPtr->nBoardOrderIndex = nBoardSortDisable;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ResetBoardConfigListSelected()
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();
		
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		BoardConfigPtr->bSelected = false;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::FillInBoardConfigList_Order()//填入尚未設定的單板排序 
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();
		
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) { continue; }
		BoardConfigPtr->nBoardOrderIndex = m_BoardOrderIndex;
		m_BoardOrderIndex ++;
	}
	UpdateBoardOrderCount(m_BoardOrderIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::RestoreBoardConfigList_Order()
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();
		
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		BoardConfigPtr->nBoardOrderIndex = (int)(BoardConfigPtr->uBoardIndex);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::IncludeVisibledBoardConfigList()
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		BoardConfigPtr->bIncluded = BoardConfigPtr->bVisibled;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::IncludeSelectedBoardConfigList()
{
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }		
		BoardConfigPtr->bIncluded = BoardConfigPtr->bSelected;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::UpdateBoardOrderCount(int val)
{
	CString str;
	str.Format(_T("%d"), val);
	CWnd::SetDlgItemText(BCW_BOARD_SORT_SET_EDIT, str);
}
//-------------------------------------------------------------------------------------//
TRECT4D CBoardConfigWnd::GetBoardFilterRect(TBoardConfig *RefPtr)
{
	TRECT4D       FilterRect;
	if ( NULL == RefPtr )
	{	return FilterRect; }

	double        FilterRectW=0;
	double        FilterRectH=0;
	const TRECT4D &RefRect=RefPtr->rcBoardMapRect;		
	const double  RefCPX = RefRect.GetCpX();
	const double  RefCPY = RefRect.GetCpY();	
	CalcBoardPitch(RefPtr, FilterRectW, FilterRectH);	
	FilterRect.left  = RefCPX-(FilterRectW*0.5);
	FilterRect.top   = RefCPY-(FilterRectH*0.5);
	FilterRect.right = RefCPX+(FilterRectW*0.5);
	FilterRect.bottom= RefCPY+(FilterRectH*0.5);
	return FilterRect;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::CalcBoardPitch(TBoardConfig *RefPtr, double &PitchX, double &PitchY)
{		
	size_t        FindCountX=0;	
	size_t        FindCountY=0;	
	TBoardConfig *BoardConfigPtr = NULL;
	const TRECT4D &RefRect=RefPtr->rcBoardMapRect;			
	const double  RefCPX = RefRect.GetCpX();
	const double  RefCPY = RefRect.GetCpY();
	const double  RefRectW = RefRect.GetWidth();
	const double  RefRectH = RefRect.GetHeight();
	const double  MinDistX = RefRectW*0.1;
	const double  MinDistY = RefRectH*0.1;
	const size_t  BoardConfigCount = GetBoardConfigCount();				
	PitchX = PitchY = 0;
	for ( size_t i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		//if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		if ( true == BoardConfigPtr->bCounted ) { continue; }
		if ( false == BoardConfigPtr->bIncluded ) { continue; }
		if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) { continue; }
		if ( BoardConfigPtr == RefPtr ) { continue; }
		const double CPX = BoardConfigPtr->rcBoardMapRect.GetCpX();
		const double CPY = BoardConfigPtr->rcBoardMapRect.GetCpY();
		const double DistX = fabs(CPX-RefCPX);
		const double DistY = fabs(CPY-RefCPY);
		if ( DistX > MinDistX )
		{
			if ( 0==FindCountX || PitchX>DistX )
			{	
				PitchX = DistX;	
				FindCountX ++;
			}
		}
		if ( DistY > MinDistY )
		{
			if ( 0==FindCountY || PitchY>DistY )
			{	
				PitchY = DistY;	
				FindCountY ++;
			}
		}
	}
	if ( 0 == FindCountX )
	{	PitchX = RefRectW;	}
	else
	{	PitchX = MIN(RefRectW, PitchX);	}

	if ( 0 == FindCountY )
	{	PitchY = RefRectH;	}
	else
	{	PitchY = MIN(RefRectH, PitchY);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
BOARD_ORDER_SEARCH_MODE CBoardConfigWnd::GetOppositeDirectionMode(BOARD_ORDER_SEARCH_MODE Mode) const
{
	BOARD_ORDER_SEARCH_MODE Opposite;
	switch ( Mode )
	{
	case BOARD_ORDER_SEARCH_LEFT:	Opposite=BOARD_ORDER_SEARCH_RIGHT; break;
	case BOARD_ORDER_SEARCH_TOP:	Opposite=BOARD_ORDER_SEARCH_BOTTOM; break;
	case BOARD_ORDER_SEARCH_RIGHT:  Opposite=BOARD_ORDER_SEARCH_LEFT; break;
	case BOARD_ORDER_SEARCH_BOTTOM: Opposite=BOARD_ORDER_SEARCH_TOP; break;
	default:						Opposite=BOARD_ORDER_SEARCH_RETURN;	break;
	}
	return Opposite;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_MODE Mode, BOARD_ORDER_SEARCH_MODE SubMode)
{	
	const bool bSPath=GetSPath();
	int BoardSortStart = nBoardSortStart;
	TBoardConfig *BoardConfigPtrFirst = NULL;
	TBoardConfig *BoardConfigPtrNext  = NULL;	
	BOARD_ORDER_SEARCH_MODE NextMode = Mode;
	const BOOL bUserSelectMode = CWnd::IsDlgButtonChecked(BCW_USER_SELECT_MODE_CHK);	

	if ( FALSE == bUserSelectMode )
	{	IncludeVisibledBoardConfigList(); }
	else
	{
		CInputBoxWnd InputBox;
		CString strCaption, strLabel, strValue;
		strCaption = _T("Set Start Index");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strLabel = _T("Start Index");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strValue.Format(_T("%d"), m_BoardOrderIndex+1);
		InputBox.SetParam1(strCaption, strLabel, strValue);		
		if ( InputBox.DoModal() != IDOK )
		{	return false; }
		BoardSortStart = ::_ttoi(InputBox.m_DataEdit1)-1;		
		if ( BoardSortStart<0 || BoardSortStart>=GetBoardConfigCount() )
		{	return false; }	
		IncludeSelectedBoardConfigList(); 
	}

	SetModified(true);
	ResetBoardConfigList_Order(false);	
	NextMode=GetOppositeDirectionMode(Mode);

	//m_BoardOrderIndex =nBoardSortStart;	
	m_BoardOrderIndex =BoardSortStart;
	do 
	{
		BoardConfigPtrFirst = SearchFirstBoardConfigPtr(Mode, SubMode);
		if ( NULL == BoardConfigPtrFirst ) { break; }
		BoardConfigPtrFirst->nBoardOrderIndex = m_BoardOrderIndex;
		m_BoardOrderIndex++;
		
		do
		{
			BoardConfigPtrNext = SearchNextBoardConfigPtr(BoardConfigPtrFirst, NextMode);
			if ( NULL == BoardConfigPtrNext ) { break; }			
			BoardConfigPtrNext->nBoardOrderIndex = m_BoardOrderIndex;			
			BoardConfigPtrFirst = BoardConfigPtrNext;
			m_BoardOrderIndex++;
		} while (true);

		//S-Path
		if ( true == bSPath )
		{
			Mode=GetOppositeDirectionMode(Mode);			
			NextMode=GetOppositeDirectionMode(NextMode);			
		}
	} while ( true );
	UpdateBoardOrderCount(m_BoardOrderIndex);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
TBoardConfig* CBoardConfigWnd::SearchNextBoardConfigPtr(TBoardConfig *RefPtr, BOARD_ORDER_SEARCH_MODE Mode)
{
	if ( NULL == RefPtr ) { return NULL; }

	size_t        i = 0;
	size_t        MinIndex = -1;
	TRECT4D       RefRect=RefPtr->rcBoardMapRect;
	TRECT4D       FilterRect=GetBoardFilterRect(RefPtr);
	TBoardConfig *BoardConfigPtr = NULL;
	const size_t  BoardConfigCount = GetBoardConfigCount();		
	
	double Min = 0, Now = 0;
	double CPX = 0, CPY = 0;
	double RCPX = RefRect.GetCpX();
	double RCPY = RefRect.GetCpY();	
	const bool bMatrixMode = GetUseMatrixMode();
	const int  RefPosIndex_X = RefPtr->nBoardPosIndex_X;
	const int  RefPosIndex_Y = RefPtr->nBoardPosIndex_Y;
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		//if ( false == BoardConfigPtr->bVisibled ) { continue; }
		if ( false == BoardConfigPtr->bIncluded ) { continue; }
		if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) { continue; }
		if ( BoardConfigPtr == RefPtr ) { continue; }
		CPX = BoardConfigPtr->rcBoardMapRect.GetCpX();
		CPY = BoardConfigPtr->rcBoardMapRect.GetCpY();
		int PosIndex_X = BoardConfigPtr->nBoardPosIndex_X;
		int PosIndex_Y = BoardConfigPtr->nBoardPosIndex_Y;
		if ( BOARD_ORDER_SEARCH_LEFT==Mode || BOARD_ORDER_SEARCH_RIGHT==Mode )
		{
			if ( true == bMatrixMode )
			{
				if ( PosIndex_Y != RefPosIndex_Y ) { continue; }
			}
			else
			{
				//剔除超過此單板上下範圍
				if ( CPY < FilterRect.top  ) { continue; }
				if ( CPY > FilterRect.bottom ) { continue; }
			}
			if ( BOARD_ORDER_SEARCH_LEFT==Mode )//找比此單板更左邊的
			{	if ( CPX > FilterRect.right ) { continue; }	}
			else if ( BOARD_ORDER_SEARCH_RIGHT==Mode )//找比此單板更右邊的
			{	if ( CPX < FilterRect.left ) { continue; }			}
		}
		else if ( BOARD_ORDER_SEARCH_TOP==Mode || BOARD_ORDER_SEARCH_BOTTOM==Mode )
		{
			if ( true == bMatrixMode )
			{
				if ( PosIndex_X != RefPosIndex_X ) { continue; }
			}
			else
			{
				//剔除超過此單板左右範圍
				if ( CPX < FilterRect.left ) { continue; }	
				if ( CPX > FilterRect.right ) { continue; }
			}
			if ( BOARD_ORDER_SEARCH_TOP==Mode )//找比此單板更上面的
			{	if ( CPY > FilterRect.bottom ) { continue; }	}
			if ( BOARD_ORDER_SEARCH_BOTTOM==Mode )//找比此單板更下面的
			{	if ( CPY < FilterRect.top ) { continue; }	}
		}
		
		
		//Now = ::abs(CPX-RCPX)+::abs(CPY-RCPY);
		if ( BOARD_ORDER_SEARCH_LEFT==Mode || BOARD_ORDER_SEARCH_RIGHT==Mode )
		{	Now = ::abs(CPX-RCPX);	}
		else if ( BOARD_ORDER_SEARCH_TOP==Mode || BOARD_ORDER_SEARCH_BOTTOM==Mode )
		{	Now = ::abs(CPY-RCPY);	}
		if ( -1==MinIndex || Now < Min ) 
		{ 
			Min = Now;
			MinIndex = i;			
		}		
	}
	if ( -1 == MinIndex ) { return NULL; }
	BoardConfigPtr = GetBoardConfigPtr(MinIndex, true);
	return BoardConfigPtr;
}
//-------------------------------------------------------------------------------------//
TBoardConfig* CBoardConfigWnd::SearchLimitBoardConfigPtr(TBoardConfig *RefPtr, BOARD_ORDER_SEARCH_MODE Mode)
{
	if ( NULL == RefPtr ) { return NULL; }

	size_t        i = 0;	
	double        CPX=0, CPY=0;		
	size_t        ResultIndex = -1;	
	TRECT4D       ResultRect;	
	TRECT4D       RefRect=RefPtr->rcBoardMapRect;	
	TRECT4D       FilterRect=GetBoardFilterRect(RefPtr);
	TBoardConfig *BoardConfigPtr = NULL;
	const size_t  BoardConfigCount = GetBoardConfigCount();		
	const double  RefCPX = RefRect.GetCpX();
	const double  RefCPY = RefRect.GetCpY();	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		//if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		if ( true == BoardConfigPtr->bCounted ) { continue; }
		if ( false == BoardConfigPtr->bIncluded ) { continue; }
		if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) { continue; }
		if ( BoardConfigPtr == RefPtr ) { continue; }		
		CPX = BoardConfigPtr->rcBoardMapRect.GetCpX();
		CPY = BoardConfigPtr->rcBoardMapRect.GetCpY();
		if ( BOARD_ORDER_SEARCH_LEFT==Mode || BOARD_ORDER_SEARCH_RIGHT==Mode )
		{
			//剔除超過此單板上下範圍
			if ( CPY < FilterRect.top  ) { continue; }
			if ( CPY > FilterRect.bottom ) { continue; }
			if ( BOARD_ORDER_SEARCH_LEFT==Mode )//找比此單板更左邊的
			{	if ( CPX > FilterRect.right ) { continue; }	}
			else if ( BOARD_ORDER_SEARCH_RIGHT==Mode )//找比此單板更右邊的
			{	if ( CPX < FilterRect.left ) { continue; }	}
		}
		else if ( BOARD_ORDER_SEARCH_TOP==Mode || BOARD_ORDER_SEARCH_BOTTOM==Mode )
		{
			//剔除超過此單板左右範圍
			if ( CPX < FilterRect.left ) { continue; }	
			if ( CPX > FilterRect.right ) { continue; }
			if ( BOARD_ORDER_SEARCH_TOP==Mode )//找比此單板更上面的
			{	if ( CPY > FilterRect.bottom ) { continue; }	}
			if ( BOARD_ORDER_SEARCH_BOTTOM==Mode )//找比此單板更下面的
			{	if ( CPY < FilterRect.top ) { continue; }	}
		}	
		if ( -1 == ResultIndex ) 
		{	
			ResultIndex = i;	
			ResultRect = BoardConfigPtr->rcBoardMapRect;
		}
		else
		{
			switch ( Mode ) 
			{
			case BOARD_ORDER_SEARCH_LEFT://最左邊
				if ( BoardConfigPtr->rcBoardMapRect.left < ResultRect.left ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}				
				break;
			case BOARD_ORDER_SEARCH_TOP://最上面
				if ( BoardConfigPtr->rcBoardMapRect.top < ResultRect.top ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			case BOARD_ORDER_SEARCH_RIGHT://最左邊
				if ( BoardConfigPtr->rcBoardMapRect.right > ResultRect.right ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			case BOARD_ORDER_SEARCH_BOTTOM://最下面
				if ( BoardConfigPtr->rcBoardMapRect.bottom > ResultRect.bottom ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			}	
		}		
	}
	if ( -1 == ResultIndex ) { return NULL; }
	BoardConfigPtr = GetBoardConfigPtr(ResultIndex, true);
	return BoardConfigPtr;
}
//-------------------------------------------------------------------------------------//
TBoardConfig* CBoardConfigWnd::SearchFirstBoardConfigPtr(BOARD_ORDER_SEARCH_MODE Mode, BOARD_ORDER_SEARCH_MODE SubMode)
{
	size_t        i=0;
	TRECT4D       ResultRect;
	size_t        ResultIndex=-1;
	TBoardConfig *BoardConfigPtr = NULL;
	const size_t  BoardConfigCount = GetBoardConfigCount();	

	const int     FirstPosX = 1;
	const int     FirstPosY = 2;
	const int     FirstPosNone=0;
	const int     FirstPosMode = FirstPosY;		
	bool bSwitchMode = false;
	if ( FirstPosMode == FirstPosX )
	{
		if ( BOARD_ORDER_SEARCH_LEFT==SubMode || BOARD_ORDER_SEARCH_RIGHT==SubMode )
		{	bSwitchMode = true; }
	}
	if ( FirstPosMode == FirstPosY )
	{
		if ( BOARD_ORDER_SEARCH_TOP==SubMode || BOARD_ORDER_SEARCH_BOTTOM==SubMode )
		{	bSwitchMode = true; }
	}
	if ( true == bSwitchMode )
	{
		BOARD_ORDER_SEARCH_MODE T = Mode;
		Mode = SubMode;
		SubMode = T;
	}
	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		//if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		if ( false == BoardConfigPtr->bIncluded ) { continue; }

		if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) { continue; }		
		if ( -1 == ResultIndex ) 
		{ 
			ResultIndex = i; 
			ResultRect = BoardConfigPtr->rcBoardMapRect;
		}
		else
		{
			switch ( Mode ) 
			{
			case BOARD_ORDER_SEARCH_LEFT://最左邊
				if ( BoardConfigPtr->rcBoardMapRect.left < ResultRect.left ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}				
				break;
			case BOARD_ORDER_SEARCH_TOP://最上面
				if ( BoardConfigPtr->rcBoardMapRect.top < ResultRect.top ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			case BOARD_ORDER_SEARCH_RIGHT://最左邊
				if ( BoardConfigPtr->rcBoardMapRect.right > ResultRect.right ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			case BOARD_ORDER_SEARCH_BOTTOM://最下面
				if ( BoardConfigPtr->rcBoardMapRect.bottom > ResultRect.bottom ) 
				{
					ResultIndex = i; 
					ResultRect = BoardConfigPtr->rcBoardMapRect;
				}
				break;
			}			
		}
	}
	if ( -1 == ResultIndex ) { return NULL; }
	BoardConfigPtr = GetBoardConfigPtr(ResultIndex, true);
	if ( NULL == BoardConfigPtr ) { return NULL; }	
	TBoardConfig *BoardConfigPtrNext = NULL;

	std::vector<TBoardConfig*> BoardCountedList;
	BoardConfigPtr->bCounted = true;
	BoardCountedList.push_back(BoardConfigPtr);	
	do
	{	
		BoardConfigPtrNext = SearchLimitBoardConfigPtr(BoardConfigPtr, SubMode);		
		if ( NULL == BoardConfigPtrNext ) { break; }		
		BoardConfigPtr = BoardConfigPtrNext;	

		BoardConfigPtr->bCounted = true;
		BoardCountedList.push_back(BoardConfigPtr);
	} while (true);

	for ( size_t i=0; i<BoardCountedList.size(); i++ )
	{	BoardCountedList[i]->bCounted = false;	}
	return BoardConfigPtr;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::BuildPanelListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_05 &ListCtrl = m_PanelListWnd;

		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right-Rect.left-width-32);
		str = AOIDataDefine.GetPanelText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = AOIDataDefine.GetCountText();
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::BuildPanelListWnd()
{
	CThisListCtrl_05 &ListCtrl = m_PanelListWnd;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString      str;	
	size_t       i=0;
	int          nItem=0;
	int          nSubItem=0;
	size_t       PanelBoardCount=0;	
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();

	nItem = 0;
	nSubItem = 0;
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() ) { continue; }

		nSubItem = 0;
		PanelBoardCount = PanelPtr->GetPanelBoardCount();

		str.Format(_T("%d"), i+1);

		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, (DWORD_PTR)(PanelPtr));
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%d"), PanelBoardCount);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}

	if ( nItem > 0 ) 
	{
		const int nActItem = 0;
		ListCtrl.SetItemState(nActItem, LVIS_SELECTED, LVIS_SELECTED);
		ExecChangePanelListItem(nActItem);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecChangePanelListItem(int nItem)
{
	CThisListCtrl_05 &ListCtrl = m_PanelListWnd;
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString    str;
	const bool bModified = GetModified();	
	CAOIPanel *ActPanelPtr = ProjectPtr->GetProjectActivePanel();
	DWORD_PTR  ItemData = ListCtrl.GetItemData(nItem);
	CAOIPanel *PanelPtr = (CAOIPanel*)(ItemData);
	if ( PanelPtr != ActPanelPtr )
	{
		if ( true == bModified ) 
		{
			str = _T("Do you want to set the boards order?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	ExecUpdateBoardOrder(); }
			else
			{	RestoreBoardConfigList_Order(); }
			SetModified(false);
		}		
	}
	ProjectPtr->SetProjectActivePanel(PanelPtr);
	UpdateBoardConfigList(PanelPtr);
	ChangeBoardOrderRadio(BCW_BOARD_SORT_RESTORE_RDO);
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(true);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, BCW_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_LBtnUpPos = pt;
	
	POINT DifP;
	RECT  SelRect={0,0,0,0};
	BOARD_CONFIG_PICK_MODE PickMode = GetBoardConfigPickMode();
	DifP.x = m_LBtnUpPos.x-m_LBtnDownPos.x;
	DifP.y = m_LBtnUpPos.y-m_LBtnDownPos.y;
	JetAPI::PointsToRect(m_LBtnDownPos, m_LBtnUpPos, SelRect);
	switch ( PickMode )
	{		
	case BOARD_CONFIG_PICK_ORDER:
		if ( abs(DifP.x)<2 && abs(DifP.y)<2 )
		{	ExecPickBoardConfig(pt);	}		
		break;
	default:
		ExecRectBoardConfig(SelRect);
		break;
	}
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnMouseMove(UINT nFlags, CPoint point) 
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
BOOL CBoardConfigWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
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
void CBoardConfigWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(false);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, BCW_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_RBtnUpPos = m_RBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnRButtonUp(UINT nFlags, CPoint point) 
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
BOOL CBoardConfigWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			if ( BOARD_CONFIG_PICK_ORDER == m_BoardConfigPickMode ) 
			{	OnBoardSortStopBtn();	}
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CBoardConfigWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortSetBtn() 
{
	// TODO: Add your control notification handler code here	
	SetModified(true);
	ResetBoardConfigList_Order(true);
	SetBoardConfigPickMode(BOARD_CONFIG_PICK_ORDER);
	UpdateBoardOrderCount(m_BoardOrderIndex);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ChangeBoardOrderRadio(UINT ActID)
{
	UINT ID = 0;
	ID = BCW_BOARD_SORT_LRTB_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);	

	ID = BCW_BOARD_SORT_RLTB_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_LRBT_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_RLBT_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_TBLR_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_BTLR_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_TBRL_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_BTRL_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);

	ID = BCW_BOARD_SORT_RESTORE_RDO;
	JetAPI::CheckRadioWnd(this, ID, ActID);
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortLRTBRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_LEFT, BOARD_ORDER_SEARCH_TOP);	
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortRLTBRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_RIGHT, BOARD_ORDER_SEARCH_TOP);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortLRBTRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_LEFT, BOARD_ORDER_SEARCH_BOTTOM);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortRLBTRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_RIGHT, BOARD_ORDER_SEARCH_BOTTOM);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortTBLRRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_TOP, BOARD_ORDER_SEARCH_LEFT);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortBTLRRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_BOTTOM, BOARD_ORDER_SEARCH_LEFT);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortTBRLRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_TOP, BOARD_ORDER_SEARCH_RIGHT);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortBTRLRdo() 
{
	// TODO: Add your control notification handler code here
	ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_BOTTOM, BOARD_ORDER_SEARCH_RIGHT);
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnDblclkPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecChangePanelListItem(nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::RedrawWnd()
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
	DrawBoardConfigList(hDrawDC);
	DrawSelectRect(hDrawDC);
	//DrawProjectFd(hDrawDC);
	//DrawProjectBoard(hDrawDC);
	//DrawProjectBarcode(hDrawDC);
	//DrawProjectComponent(hDrawDC);
	::SetBkMode(hDrawDC, OldBkMode);
	if ( hDrawDC != hDC )
	{	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hDrawDC, 0, 0, SRCCOPY );	}	
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::CreateBKImage()
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
void CBoardConfigWnd::DrawSelectRect(HDC hDC)
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
void CBoardConfigWnd::DrawProjectFd(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
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
	const size_t   FdCount = PanelPtr->GetPanelFdCount();
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
		FdPtr = PanelPtr->GetPanelFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( NULL == FdPtr->GetFdBoardPtr() ) { continue; }		
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
void CBoardConfigWnd::DrawProjectBoard(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
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
	CAOIBoard     *BoardPtr = NULL;		
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const size_t   BoardCount = PanelPtr->GetPanelBoardCount();	
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
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardCadRgn = BoardPtr->GetBoardRgnCad();
		BoardPtr->GetBoardRgnStage(DistrictID, BoardStageRgn);		
		
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
void CBoardConfigWnd::DrawProjectBarcode(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	if ( NULL == hDC ) { return; }
	
	CString        str;
	CString        strBarcode;
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
	CAOIBarcode   *BarcodePtr = NULL;
	const size_t   BarcodeCount = PanelPtr->GetPanelBarcodeCount();
	DISTRICT_ID    DistrictID = ProjectPtr->GetProjectActDistrictID();

	HPEN          hPenA = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
	HPEN          hPenB = ::CreatePen(PS_SOLID, 1, 0x00FFFF);
	HPEN          hPenN = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPenA));
	COLORREF      clrText = ::SetTextColor(hDC, 0x2200A0);
	
	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	strBarcode = AOIDataDefine.GetBarcodeText();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = PanelPtr->GetPanelBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }

		StagePos.x = BarcodePtr->GetBarcodeStagePosX();
		StagePos.y = BarcodePtr->GetBarcodeStagePosY();
		BodySize.cx = BarcodePtr->GetBarcodeBodySizeW();
		BodySize.cy = BarcodePtr->GetBarcodeBodySizeH();

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
		str.Format(_T("%s-%d"), strBarcode, i+1);
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
void CBoardConfigWnd::DrawBoardConfigList(HDC hDC)
{	
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
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();	
	BOARD_CONFIG_MODE BoardConfigMode = GetBoardConfigMode();
	HPEN          hPen = ::CreatePen(PS_SOLID, 1, 0x408020);
	HPEN          hPenSel = ::CreatePen(PS_SOLID, 2, 0xFFFFFF);
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
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( false == BoardConfigPtr->bVisibled ) { continue; }
		
		ImageRect = BoardConfigPtr->rcBoardMapRect;
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);		
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		if ( true == BoardConfigPtr->bSelected ) 
		{	::SelectObject(hDC, hPenSel);	}
		else
		{	::SelectObject(hDC, hPen);	}
		ImageAPI.DrawRectLine(hDC, DrawRect);

		switch ( BoardConfigMode ) 
		{
		case BOARD_CONFIG_ORDER:
			str.Format(_T("%d"), BoardConfigPtr->nBoardOrderIndex+1);
			break;
		default:
			str.Format(_T("%d"), i+1);
			break;
		}
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, str.GetLength());
	}
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPen); hPen=NULL;
	::DeleteObject(hPenSel); hPenSel=NULL;
	::SetTextColor(hDC, clrText);
	return ;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::DrawProjectComponent(HDC hDC)
{
	CAOIProject    *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel      *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
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
	const size_t   ComponentCount = PanelPtr->GetPanelComponentCount();
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
		ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
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
bool CBoardConfigWnd::ClearImageBuffer()
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
bool CBoardConfigWnd::CreateImageBuffer()
{
	const char fnName[] = "CBoardConfigWnd::CreateImageBuffer";
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
bool CBoardConfigWnd::SwitchShowImage()
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
bool CBoardConfigWnd::BuildShowImage(size_t index)
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
bool CBoardConfigWnd::ExecUpdateBoardOrder()//更新單板排序
{
	bool bIsOK = true;
	BOOL bAllPanelMode = CWnd::IsDlgButtonChecked(BCW_ALL_PANEL_MODE_CHK);
	if ( TRUE == bAllPanelMode ) 
	{	bIsOK = ExecUpdateBoardOrder_All();	}
	else
	{	bIsOK = ExecUpdateBoardOrder_Panel();	}
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecUpdateBoardOrder_All()//更新單板排序
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	CString                 str;
	size_t                  i=0;	
	int                     BoardOrderIndex=0;
	int                     MaxBoardOrderIndex=0;
	CAOIPanel			   *PanelPtr = NULL;
	CAOIBoard              *BoardPtr=NULL;
	std::vector<CAOIBoard*> BoardList;
	TBoardConfig *BoardConfigPtr=NULL;
	const size_t  PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t  BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t  BoardConfigCount = GetBoardConfigCount();
	
	MaxBoardOrderIndex = -1;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		PanelPtr = BoardPtr->GetBoardPanelPtr();
		if ( NULL == PanelPtr ) 
		{
			str.Format(_T("Error, Board[%d} not Panel Ptr"), i+1);
			JetAPI::ShowMessageBox(str);			
			return false;
		}
		BoardPtr->SetBoardTempInt(-1);
	}
	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }	
		if ( false == BoardConfigPtr->bVisibled ) { continue; }
		BoardPtr = BoardConfigPtr->BoardPtr;
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardConfigPtr->nBoardOrderIndex;
		BoardPtr->SetBoardTempInt(BoardOrderIndex);		
		if ( MaxBoardOrderIndex < BoardOrderIndex ) 
		{	MaxBoardOrderIndex = BoardOrderIndex; }
	}
	MaxBoardOrderIndex ++;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardPtr->GetBoardTempInt();
		if ( BoardOrderIndex >= 0 ) { continue; }
		BoardPtr->SetBoardTempInt(MaxBoardOrderIndex);
		MaxBoardOrderIndex ++;
	}


	CSortObj                SortObj;
	std::vector<CSortObj>   SortListTmp;
	SortObj.SetSortMode(SORT_BY_ID);
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardPtr->GetBoardTempInt();
		SortObj.SetID(BoardOrderIndex);
		SortObj.SetPtr(BoardPtr);
		SortListTmp.push_back(SortObj);
	}
	std::sort(SortListTmp.begin(), SortListTmp.end());

	const size_t SortCount = SortListTmp.size();
	if ( SortCount != BoardCount )
	{	return false; }
	
	ProjectPtr->SelectProjectAllBoards(true);
	ProjectPtr->RemoveProjectBoardSelected();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->RemovePanelAllBoards();
	}
	for ( i=0; i<SortCount; i++ )
	{
		SortObj = SortListTmp[i];
		BoardOrderIndex = SortObj.GetID();
		BoardPtr = (CAOIBoard*)(SortObj.GetPtr());
		PanelPtr = BoardPtr->GetBoardPanelPtr();
		if ( NULL == PanelPtr ) {	continue;	}
		ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
		PanelPtr->AddPanelBoardPtr(BoardPtr);		
	}

	ProjectPtr->LayoutProjectBoardList();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->UpdateBoardIndexToObjList();
	}	
	ProjectPtr->UpdateProjectComponentModelIsolatedFolder();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecUpdateBoardOrder_Panel()//更新單板排序
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	size_t                  i=0;	
	int                     BoardOrderIndex=0;
	int                     MaxBoardOrderIndex=0;
	CAOIBoard              *BoardPtr=NULL;	
	std::vector<CAOIBoard*> BoardList;
	TBoardConfig *BoardConfigPtr=NULL;
	const size_t  BoardCount = PanelPtr->GetPanelBoardCount();
	const size_t  BoardConfigCount = GetBoardConfigCount();
	
	MaxBoardOrderIndex = -1;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(-1);
	}
	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }	
		if ( false == BoardConfigPtr->bVisibled ) { continue; }
		BoardPtr = BoardConfigPtr->BoardPtr;
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardConfigPtr->nBoardOrderIndex;
		BoardPtr->SetBoardTempInt(BoardOrderIndex);		
		if ( MaxBoardOrderIndex < BoardOrderIndex ) 
		{	MaxBoardOrderIndex = BoardOrderIndex; }
	}
	MaxBoardOrderIndex ++;
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardPtr->GetBoardTempInt();
		if ( BoardOrderIndex >= 0 ) { continue; }
		BoardPtr->SetBoardTempInt(MaxBoardOrderIndex);
		MaxBoardOrderIndex ++;
	}


	CSortObj                SortObj;
	std::vector<CSortObj>   SortListTmp;
	SortObj.SetSortMode(SORT_BY_ID);
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardOrderIndex = BoardPtr->GetBoardTempInt();
		SortObj.SetID(BoardOrderIndex);
		SortObj.SetPtr(BoardPtr);
		SortListTmp.push_back(SortObj);
	}
	std::sort(SortListTmp.begin(), SortListTmp.end());

	const size_t SortCount = SortListTmp.size();
	if ( SortCount != BoardCount )
	{	return false; }

	ProjectPtr->SelectProjectAllBoards(false);
	PanelPtr->SelectPanelAllBoards(true);	
	ProjectPtr->RemoveProjectBoardSelected();

	PanelPtr->RemovePanelAllBoards();
	for ( i=0; i<SortCount; i++ )
	{
		SortObj = SortListTmp[i];
		BoardOrderIndex = SortObj.GetID();
		BoardPtr = (CAOIBoard*)(SortObj.GetPtr());
		ProjectPtr->AddProjectBoardPtr(BoardPtr, false);
		PanelPtr->AddPanelBoardPtr(BoardPtr);
		//BoardPtr->UpdateBoardIndexToObjList();
	}

	//避免其他整板下的單板在專案列表順序錯誤, 所以全部重新整理
	ProjectPtr->LayoutProjectBoardList();
	const size_t  ProjectBoardCount = ProjectPtr->GetProjectBoardCount();
	for ( i=0; i<ProjectBoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->UpdateBoardIndexToObjList();
	}	
	ProjectPtr->UpdateProjectComponentModelIsolatedFolder();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecPickBoardConfig(POINT Pt)
{
	BOARD_CONFIG_PICK_MODE PickMode = GetBoardConfigPickMode();	
	const size_t BoardConfigIndex = CheckPickBoardConfig(Pt, PickMode);
	if ( -1 == BoardConfigIndex ) { return false; }
	TBoardConfig  *BoardConfigPtr = NULL;
	SelectBoardConfig(BoardConfigIndex);		
	BoardConfigPtr = GetBoardConfigPtr(BoardConfigIndex, true);
	if ( NULL != BoardConfigPtr )
	{
		if ( BOARD_CONFIG_PICK_ORDER == PickMode ) 
		{	
			BoardConfigPtr->nBoardOrderIndex = m_BoardOrderIndex; 
			m_BoardOrderIndex ++;
			UpdateBoardOrderCount(m_BoardOrderIndex);
		}
	}
	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::ExecRectBoardConfig(RECT SelRect)
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
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ImageAPI.MapWndRectToImageRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndSelRect, ImgSelRect);	

	if ( AOIDataCollect.CheckMultiSelectMode() == false ) 
	{	ResetBoardConfigListSelected();	}
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( false == BoardConfigPtr->bVisibled ) { continue; }
		ImageRect = BoardConfigPtr->rcBoardMapRect;
		if ( ImageRect.minX > ImgSelRect.right ) { continue; }
		if ( ImageRect.minY > ImgSelRect.bottom ) { continue; }
		if ( ImageRect.maxX < ImgSelRect.left ) { continue; }
		if ( ImageRect.maxY < ImgSelRect.top  ) { continue; }

		if ( ImageRect.minX < ImgSelRect.left ) { continue; }		
		if ( ImageRect.minY < ImgSelRect.top ) { continue; }
		if ( ImageRect.maxX > ImgSelRect.right ) { continue; }
		if ( ImageRect.maxY > ImgSelRect.bottom  ) { continue; }
		
		BoardConfigPtr->bSelected = true;
	}	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CBoardConfigWnd::CheckPickBoardConfig(POINT Pt, BOARD_CONFIG_PICK_MODE PickMode)
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
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImgPt);	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( false == BoardConfigPtr->bVisibled ) { continue; }		
		if ( BOARD_CONFIG_PICK_ORDER == PickMode ) 
		{
			if ( BoardConfigPtr->nBoardOrderIndex >= nBoardSortStart ) 
			{	continue; }
		}
		ImageRect = BoardConfigPtr->rcBoardMapRect;
		if ( ImgPt.x < ImageRect.minX ) { continue; }
		if ( ImgPt.y < ImageRect.minY ) { continue; }
		if ( ImgPt.x > ImageRect.maxX ) { continue; }
		if ( ImgPt.y > ImageRect.maxY ) { continue; }
		return i;
	}	
	return -1;
}
//-------------------------------------------------------------------------------------//
bool CBoardConfigWnd::SelectBoardConfig(size_t index)
{
	CString        str;
	size_t         i=0;
	TBoardConfig  *BoardConfigPtr = NULL;
	const size_t   BoardConfigCount = GetBoardConfigCount();	
	for ( i=0; i<BoardConfigCount; i++ )
	{
		BoardConfigPtr = GetBoardConfigPtr(i, false);
		if ( NULL == BoardConfigPtr ) { continue; }
		if ( i != index ) 
		{	BoardConfigPtr->bSelected = false;	}
		else
		{	BoardConfigPtr->bSelected = true;	}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortRestoreRdo() 
{
	// TODO: Add your control notification handler code here
	SetModified(false);
	RestoreBoardConfigList_Order();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	const bool bModified = GetModified();
	if ( true == bModified ) 
	{
		str = _T("Do you want to set the boards order?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
		{	ExecUpdateBoardOrder();	 }
	}	
	SetModified(false);
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortStopBtn() 
{
	// TODO: Add your control notification handler code here
	FillInBoardConfigList_Order();
	SetBoardConfigPickMode(BOARD_CONFIG_PICK_NONE); 
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnAllPanelModeChk() 
{
	// TODO: Add your control notification handler code here
	CAOIPanel *PanelPtr = GetActivePanelPtr();	
	UpdateBoardConfigList(PanelPtr);	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnUserSelectModeChk() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBoardConfigWnd::OnBoardSortResetBtn() 
{
	// TODO: Add your control notification handler code here
	BOARD_CONFIG_MODE BoardConfigMode = GetBoardConfigMode();
	switch ( BoardConfigMode )
	{
	case BOARD_CONFIG_ORDER:
		ResetBoardConfigList_Order(true);	
		break;
	}
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//