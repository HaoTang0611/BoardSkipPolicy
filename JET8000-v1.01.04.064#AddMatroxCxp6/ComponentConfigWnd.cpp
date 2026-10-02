// ComponentConfigWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ComponentConfigWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentConfigWnd dialog
//-------------------------------------------------------------------------------------//
CComponentConfigWnd::CComponentConfigWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CComponentConfigWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComponentConfigWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	POINT  Pt={0,0};	
	m_Modified = false;
	m_ShowSelectLine = false;
	m_ProjectPtr = NULL;	

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
	m_LockPanelListWnd = false;
	m_ComponentConfigMode = COMPONENT_CONFIG_NONE;
	m_ComponentConfigImageMode = COMPONENT_CONFIG_IMAGE_MAP;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComponentConfigWnd)
	DDX_Control(pDX, COMCFG_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, COMCFG_PANEL_LIST_WND, m_PanelListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CComponentConfigWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CComponentConfigWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(COMCFG_SET_BOARD_BTN, OnSetBoardBtn)
	ON_BN_CLICKED(COMCFG_APPLY_BOARD_BTN, OnApplyBoardBtn)
	ON_BN_CLICKED(COMCFG_CLEAR_BOARD_BTN, OnClearBoardBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentConfigWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CComponentConfigWnd::OnInitDialog() 
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
	BuildBoardRectList();
	BuildComponentConfigList();
	BuildPanelListWnd();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnSize(UINT nType, int cx, int cy) 
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
void CComponentConfigWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
BOOL CComponentConfigWnd::PreTranslateMessage(MSG* pMsg) 
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
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CComponentConfigWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	bool bIsOK = true;	
	COMPONENT_CONFIG_MODE ComponentConfigMode = GetComponentConfigMode();
	
	switch ( ComponentConfigMode )
	{
	case COMPONENT_CONFIG_BOARD_ASSIGN:
		bIsOK = CheckComponentConfigFinish_BoardAssign(str);
		break;
	}
	
	if ( false == bIsOK )
	{
		JetAPI::ShowMessageBox(str);
		return;
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_COMPONENT_CONFIG_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_COMPONENT_CONFIG_WND;
	WndKey = _T("IDD_COMPONENT_CONFIG_WND");
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
	WndID = COMCFG_PANEL_LIST_LABEL;
	WndKey = _T("COMCFG_PANEL_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = COMCFG_SET_BOARD_BTN;
	WndKey = _T("COMCFG_SET_BOARD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = COMCFG_APPLY_BOARD_BTN;
	WndKey = _T("COMCFG_APPLY_BOARD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = COMCFG_CLEAR_BOARD_BTN;
	WndKey = _T("COMCFG_CLEAR_BOARD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = COMCFG_USER_SELECT_MODE_CHK;
	WndKey = _T("COMCFG_USER_SELECT_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CComponentConfigWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_COMPONENT_CONFIG_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CComponentConfigWnd::GetActivePanelPtr()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return NULL; }
	return ProjectPtr->GetProjectActivePanel();
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CComponentConfigWnd::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SetLockPanelListWnd(bool bLock)
{
	m_LockPanelListWnd = bLock;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::GetLockPanelListWnd() const
{
	return m_LockPanelListWnd;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SetComponentConfigMode(COMPONENT_CONFIG_MODE Mode)
{
	m_ComponentConfigMode = Mode;
}
//-------------------------------------------------------------------------------------//
COMPONENT_CONFIG_MODE CComponentConfigWnd::GetComponentConfigMode() const
{
	return m_ComponentConfigMode;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SetComponentConfigImageMode(COMPONENT_CONFIG_IMAGE_MODE Mode)
{
	m_ComponentConfigImageMode = Mode;
}
//-------------------------------------------------------------------------------------//
COMPONENT_CONFIG_IMAGE_MODE CComponentConfigWnd::GetComponentConfigImageMode() const
{
	return m_ComponentConfigImageMode;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::CloneBoardRectList(std::vector<TBoardRect> &List)
{
	List = m_BoardRectList;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::CloneComponentConfigList(std::vector<TComponentConfig> &List)
{
	List = m_ComponentConfigList;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::SetModified(bool val)//
{
	m_Modified = val;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::GetModified() const
{
	return m_Modified;
}
//-------------------------------------------------------------------------------------//	
void CComponentConfigWnd::SetShowSelectLine(bool val)
{
	m_ShowSelectLine = val;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::GetShowSelectLine() const
{
	return m_ShowSelectLine;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::BuildBoardRectList()
{
	m_BoardRectList.clear();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	size_t       i=0;		
	TRECT4D      ImageRect;
	TRECT4D      StageRect;
	TPOINT2D     MapStageCp;
	TBoardRect   BoardRect;
	CAOIBoard   *BoardPtr=NULL;	
	TREGION4D    BoardCadRgn;
	TREGION4D    BoardStageRgn;
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
		//BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		BoardPtr->GetBoardRgnCad(BoardCadRgn);
		BoardPtr->GetBoardRgnStage(DistrictID, BoardStageRgn);		
		
		StageRect.left   = MIN(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.top    = MIN(BoardStageRgn.minY, BoardStageRgn.maxY);
		StageRect.right  = MAX(BoardStageRgn.minX, BoardStageRgn.maxX);
		StageRect.bottom = MAX(BoardStageRgn.minY, BoardStageRgn.maxY);
		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);

		BoardRect.PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardRect.BoardIndex = BoardPtr->GetBoardIndex_Panel();
		BoardRect.PanelPtr  = BoardPtr->GetBoardPanelPtr();
		BoardRect.BoardPtr  = BoardPtr;
		BoardRect.BoardRgn = BoardCadRgn;
		BoardRect.BoardMapRect = ImageRect;		
		m_BoardRectList.push_back(BoardRect);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CComponentConfigWnd::GetBoardRectCount()
{	
	return m_BoardRectList.size();
}
//-------------------------------------------------------------------------------------//
TBoardRect* CComponentConfigWnd::GetBoardRectPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck ) 
	{
		const size_t Count = m_BoardRectList.size();
		if ( idx >= Count )
		{	return NULL; }
	}
	return &(m_BoardRectList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::UpdateBoardRectList(CAOIPanel *PanelPtr)
{
	size_t      i=0;
	TBoardRect *BoardRectPtr = NULL;
	size_t      BoardRectCount = GetBoardRectCount();	
	for ( i=0; i<BoardRectCount; i++ )
	{
		BoardRectPtr = GetBoardRectPtr(i, false);
		if ( NULL == PanelPtr )
		{	BoardRectPtr->Visibled = true;	}
		else
		{
			if ( BoardRectPtr->PanelPtr == PanelPtr ) 
			{	BoardRectPtr->Visibled = true; }
			else
			{	BoardRectPtr->Visibled = false; }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::RemoveVisibleBoardRect()
{
	size_t                     i=0, j=0;
	size_t                     BarcodeComponentCount=0;
	TBoardRect                *BoardRectPtr=NULL;
	TComponentConfig          *ComponentConfigPtr=NULL;
	std::vector<unsigned int>  BoardIndexMap;
	size_t     BoardRectCount = GetBoardRectCount();
	const size_t ComponentConfigCount = GetComponentConfigCount();
	for ( i=0; i<BoardRectCount; i++ )
	{
		BoardRectPtr = GetBoardRectPtr(i, false);
		if ( NULL == BoardRectPtr ) 
		{
			BoardIndexMap.push_back(-1); 
			continue;
		}
		if ( false == BoardRectPtr->Visibled )
		{	BoardIndexMap.push_back(i); }
		else
		{	BoardIndexMap.push_back(-1); }
	}
	j = 0;
	for ( i=0; i<BoardRectCount; i++ )
	{
		if ( -1 == BoardIndexMap[i] ) { continue; }
		BoardIndexMap[i] = j;
		j++;
	}
	std::vector<TBoardRect> TmpBoardRectList;
	TmpBoardRectList=m_BoardRectList;
	m_BoardRectList.clear();
	for ( i=0; i<BoardRectCount; i++ )
	{
		if ( -1 == BoardIndexMap[i] ) { continue; }
		TmpBoardRectList[i].BoardIndex = BoardIndexMap[i];
		m_BoardRectList.push_back(TmpBoardRectList[i]);
	}
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( -1 == ComponentConfigPtr->uBoardIndexNew ) { continue; }
		if ( ComponentConfigPtr->uBoardIndexNew >= BoardRectCount ) 
		{	continue; }
		ComponentConfigPtr->uBoardIndexNew = BoardIndexMap[ComponentConfigPtr->uBoardIndexNew];		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::RemoveNoComponentBoardRect()//移除單板區域
{	
	size_t                     i=0, j=0;
	size_t                     BarcodeComponentCount=0;
	TComponentConfig          *ComponentConfigPtr=NULL;
	std::vector<unsigned int>  BoardIndexMap;
	size_t     BoardRectCount = GetBoardRectCount();
	const size_t ComponentConfigCount = GetComponentConfigCount();
	for ( i=0; i<BoardRectCount; i++ )
	{	BoardIndexMap.push_back(i); }

	for ( i=0; i<BoardRectCount; i++ )
	{
		BarcodeComponentCount = 0;
		for ( j=0; j<ComponentConfigCount; j++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(j, false);
			if ( NULL == ComponentConfigPtr ) { continue; }			
			if ( ComponentConfigPtr->uBoardIndexNew != i ) { continue; }
			BarcodeComponentCount ++;
			break;
		}
		if ( BarcodeComponentCount > 0 ) { continue; }
		BoardIndexMap[i] = -1;
	}

	j = 0;
	for ( i=0; i<BoardRectCount; i++ )
	{
		if ( -1 == BoardIndexMap[i] ) { continue; }
		BoardIndexMap[i] = j;
		j++;
	}
	std::vector<TBoardRect> TmpBoardRectList;
	TmpBoardRectList=m_BoardRectList;
	m_BoardRectList.clear();
	for ( i=0; i<BoardRectCount; i++ )
	{
		if ( -1 == BoardIndexMap[i] ) { continue; }
		TmpBoardRectList[i].BoardIndex = BoardIndexMap[i];
		m_BoardRectList.push_back(TmpBoardRectList[i]);
	}
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( -1 == ComponentConfigPtr->uBoardIndexNew ) { continue; }
		if ( ComponentConfigPtr->uBoardIndexNew >= BoardRectCount ) 
		{	continue; }
		ComponentConfigPtr->uBoardIndexNew = BoardIndexMap[ComponentConfigPtr->uBoardIndexNew];		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::BuildComponentConfigList()
{
	m_ComponentConfigList.clear();
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	size_t       i=0;		
	TRECT4D      ImageRect;
	TRECT4D      StageRect;
	TPOINT2D     MapStageCp;	
	CAOIBoard       *BoardPtr=NULL;
	CAOIComponent   *ComponentPtr=NULL;	
	TREGION4D    ComponentStageRgn;
	TComponentConfig ComponentConfig;
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	RECT         WndRect = m_ImageWndRect;
	TPOINT2D     OffsetPt = m_ViewOffset;	
	TPOINT2D     MapResolution=m_ImageRes;
	TREGION4D    MapStageRgn=m_ImageStageRgn;	
	double       ZoomScale = m_ZoomScale;	
	//const size_t ComponentCount = PanelPtr->GetPanelComponentCount();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	unsigned int ActivePanelIndex = ProjectPtr->GetProjectActivePanelIndex();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();
	for ( i=0; i<ComponentCount; i++ )
	{
		//ComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);		
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		//if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }		
		ComponentPtr->GetComponentBodyStageRegion(ComponentStageRgn);
		
		StageRect.left   = MIN(ComponentStageRgn.minX, ComponentStageRgn.maxX);
		StageRect.top    = MIN(ComponentStageRgn.minY, ComponentStageRgn.maxY);
		StageRect.right  = MAX(ComponentStageRgn.minX, ComponentStageRgn.maxX);
		StageRect.bottom = MAX(ComponentStageRgn.minY, ComponentStageRgn.maxY);
		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, MapResolution, StageRect, MapStageCp, ImageRect);

		ComponentConfig.PanelPtr = ComponentPtr->GetComponentPanelPtr();
		ComponentConfig.BoardPtr = ComponentPtr->GetComponentBoardPtr();
		ComponentConfig.ComponentPtr = ComponentPtr;
		ComponentConfig.uPanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		ComponentConfig.uBoardIndex = ComponentPtr->GetComponentBoardIndex_Panel();
		ComponentConfig.uComponentIndex = ComponentPtr->GetComponentIndex_Board();
		ComponentConfig.bVisibled = true;
		ComponentConfig.bSelected = false;
		ComponentConfig.bDelected = false;
		ComponentPtr->GetComponentBodyCadRegion(ComponentConfig.rgnComponentCadRgn);
		ComponentConfig.rcComponentMapRect = ImageRect;		
		ComponentConfig.uBoardIndexNew = ComponentPtr->GetComponentBoardIndex_Project();		
		ComponentConfig.strComponentName = ComponentPtr->GetComponentName();
		m_ComponentConfigList.push_back(ComponentConfig);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CComponentConfigWnd::GetComponentConfigCount()
{
	return m_ComponentConfigList.size();
}
//-------------------------------------------------------------------------------------//
TComponentConfig* CComponentConfigWnd::GetComponentConfigPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck ) 
	{
		const size_t Count = m_ComponentConfigList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return &(m_ComponentConfigList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ResetComponentConfigListSelected()
{
	size_t         i=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		ComponentConfigPtr->bSelected = false;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::IncludeVisibledComponentConfigList()
{
	size_t         i=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		ComponentConfigPtr->bIncluded = ComponentConfigPtr->bVisibled;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::IncludeSelectedComponentConfigList()
{
	size_t         i=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		ComponentConfigPtr->bIncluded = ComponentConfigPtr->bSelected;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ResortComponentConfigListBoardIndex()
{
	CAOIPanel         *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	size_t             i=0, j=0;
	unsigned int       BoardIndex=0;
	unsigned int       BoardIndexNew=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	TComponentConfig  *ComponentConfigPtr2 = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();	
	RemoveVisibleBoardRect();
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }		
		ComponentConfigPtr->uBoardIndexNew = -1;
	}

	TREGION4D       BoardCadRgn;
	TRECT4D         BoardMapRect;
	TBoardRect      BoardRect;
	BoardIndexNew = GetBoardRectCount();
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		if ( ComponentConfigPtr->uBoardIndexNew >= 0 ) { continue; }
		
		BoardIndex = ComponentConfigPtr->uBoardIndex;
		ComponentConfigPtr->uBoardIndexNew = BoardIndexNew;
		BoardRect.PanelPtr = ComponentConfigPtr->PanelPtr;		
		BoardCadRgn = ComponentConfigPtr->rgnComponentCadRgn;
		BoardMapRect = ComponentConfigPtr->rcComponentMapRect;
		for ( j=i+1; j<ComponentConfigCount; j++ )
		{
			ComponentConfigPtr2 = GetComponentConfigPtr(j, false);
			if ( NULL == ComponentConfigPtr2 ) { continue; }
			if ( false == ComponentConfigPtr2->bVisibled ) { continue; }			
			if ( ComponentConfigPtr2->uBoardIndexNew >= 0 ) { continue; }
			if ( BoardIndex != ComponentConfigPtr2->uBoardIndex ) { continue; }

			ComponentConfigPtr2->uBoardIndexNew = BoardIndexNew;
			BoardMapRect.Expand(ComponentConfigPtr->rcComponentMapRect);
			BoardCadRgn.Expand(ComponentConfigPtr->rgnComponentCadRgn);			
		}

		BoardRect.BoardIndex = BoardIndexNew;
		BoardRect.PanelIndex = 0;
		BoardRect.BoardPtr = NULL;
		BoardRect.BoardRgn = BoardCadRgn;
		BoardRect.BoardMapRect = BoardMapRect;
		if ( NULL != PanelPtr )
		{
			BoardRect.PanelPtr = PanelPtr;
			BoardRect.PanelIndex = PanelPtr->GetPanelIndex_Project(); 
		}
		m_BoardRectList.push_back(BoardRect);
		BoardIndexNew ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::UpdateComponentConfigList(CAOIPanel *PanelPtr)
{
	size_t         i=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();	
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( NULL==PanelPtr ) 
		{	ComponentConfigPtr->bVisibled = true;	}
		else
		{
			if ( ComponentConfigPtr->PanelPtr != PanelPtr ) 
			{	ComponentConfigPtr->bVisibled = false; }
			else
			{	ComponentConfigPtr->bVisibled = true; }
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ResetComponentConfigListBoardIndex(bool bSelected)
{
	size_t         i=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		if ( true == bSelected )
		{
			if ( false == ComponentConfigPtr->bSelected ) 
			{	continue; }
		}
		ComponentConfigPtr->uBoardIndexNew = -1;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::BuildPanelListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl_06 &ListCtrl = m_PanelListWnd;

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
bool CComponentConfigWnd::BuildPanelListWnd()
{
	CThisListCtrl_06 &ListCtrl = m_PanelListWnd;
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
	unsigned int ActPanelIndex = ProjectPtr->GetProjectActivePanelIndex();

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
		int nActItem = (int)(ActPanelIndex);
		if ( -1 == nActItem  )
		{	nActItem = 0; }
		ListCtrl.SetItemState(nActItem, LVIS_SELECTED, LVIS_SELECTED);
		ExecChangePanelListItem(nActItem);
	}

	const bool bLock = GetLockPanelListWnd();
	if ( true == bLock )
	{	ListCtrl.EnableWindow(FALSE); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ExecChangePanelListItem(int nItem)
{
	CThisListCtrl_06 &ListCtrl = m_PanelListWnd;
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
			CString ErrorString;
			str = _T("Do you want to set the component board?");
			str = LoadMultiLanguageString(str, str);
			if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
			{	
				if ( ExecUpdateComponentBoard(ErrorString) == false )
				{
					JetAPI::ShowMessageBox(ErrorString);
					ResortComponentConfigListBoardIndex();
				}
			}
			else
			{	ResortComponentConfigListBoardIndex(); }
			SetModified(false);
		}		
	}
	ProjectPtr->SetProjectActivePanel(PanelPtr);
	UpdateBoardRectList(PanelPtr);
	UpdateComponentConfigList(PanelPtr);
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::RedrawWnd()
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
	DrawBoardRectList(hDrawDC);
	DrawComponentConfigList(hDrawDC);
	DrawSelectRect(hDrawDC);	
	//DrawProjectComponent(hDrawDC);
	::SetBkMode(hDrawDC, OldBkMode);
	if ( hDrawDC != hDC )
	{	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hDrawDC, 0, 0, SRCCOPY );	}	
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::CreateBKImage()
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
void CComponentConfigWnd::DrawSelectRect(HDC hDC)
{
	const bool bShowRect = GetShowSelectLine();
	if ( false == bShowRect ) { return ; }
	HPEN          hPen = ::CreatePen(PS_DASH, 1, 0xFFFFFF);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	ImageAPI.DrawRectLine(hDC, m_LBtnDownPos, m_LastPos);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen=NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::DrawBoardRectList(HDC hDC)
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
	TBoardRect    *BoardRectPtr = NULL;
	const size_t   BoardRectCount = GetBoardRectCount();	
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
	for ( i=0; i<BoardRectCount; i++ )
	{
		BoardRectPtr = GetBoardRectPtr(i, false);
		if ( NULL == BoardRectPtr ) { continue; }
		if ( false == BoardRectPtr->Visibled ) { continue; }
		ImageRect = BoardRectPtr->BoardMapRect;
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);		
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		::SelectObject(hDC, hPen);
		ImageAPI.DrawRectLine(hDC, DrawRect);

		str.Format(_T("%d"), BoardRectPtr->BoardIndex+1);		
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
void CComponentConfigWnd::DrawComponentConfigList(HDC hDC)
{
	if ( NULL == hDC ) { return; }	
	CString        str;
	size_t         i=0;
	int            Len=0;
	int            LenSizeX=0;
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
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	COMPONENT_CONFIG_MODE ComponentConfigMode = GetComponentConfigMode();

	HPEN          hPen = ::CreatePen(PS_SOLID, 1, 0xF0F040);	
	HPEN          hPenSel = ::CreatePen(PS_SOLID, 2, 0xFFFFFF);
	HPEN          hPenUnset = ::CreatePen(PS_SOLID, 1, 0x2040F0);
	HPEN          hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF      clrText = ::SetTextColor(hDC, 0xFFFFFF);

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		
		ImageRect = ComponentConfigPtr->rcComponentMapRect;
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImageRect, DrawRect4D);

		JetAPI::AdjustRect(DrawRect4D, DrawRect4D);		
		JetAPI::Rect4DToRect(DrawRect4D, DrawRect);
		if ( DrawRect.right < WndRect.left ) { continue; }
		if ( DrawRect.bottom < WndRect.top ) { continue; }
		if ( DrawRect.left > WndRect.right ) { continue; }
		if ( DrawRect.top > WndRect.bottom ) { continue; }

		if ( true == ComponentConfigPtr->bSelected ) 
		{	::SelectObject(hDC, hPenSel);	}
		else
		{	
			switch ( ComponentConfigMode )
			{
			case COMPONENT_CONFIG_BOARD_ASSIGN:
				if ( -1 != ComponentConfigPtr->uBoardIndexNew )
				{	::SelectObject(hDC, hPen);	}
				else
				{	::SelectObject(hDC, hPenUnset);	}
				break;
			default:
				::SelectObject(hDC, hPen);
				break;
			}			
		}
		ImageAPI.DrawRectLine(hDC, DrawRect);
		str = ComponentConfigPtr->strComponentName;		
		Len = str.GetLength();
		LenSizeX = Len*6;
		if ( LenSizeX > (DrawRect.right-DrawRect.left) ) { continue; }
		::TextOut(hDC, DrawRect.left, DrawRect.top, str, Len);
	}	
	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPen); hPen=NULL;
	::DeleteObject(hPenSel); hPenSel=NULL;
	::DeleteObject(	hPenUnset); hPenUnset=NULL;
	::SetTextColor(hDC, clrText);	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ClearImageBuffer()
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
bool CComponentConfigWnd::CreateImageBuffer()
{
	bool bIsOK = true;
	COMPONENT_CONFIG_IMAGE_MODE ImageMode = GetComponentConfigImageMode();
	switch ( ImageMode )
	{
	case COMPONENT_CONFIG_IMAGE_MAP:
		bIsOK = CreateImageBuffer_Map();
		break;
	case COMPONENT_CONFIG_IMAGE_TMP:
		bIsOK = CreateImageBuffer_Tmp();
		break;
	}
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::CreateImageBuffer_Map()
{
	const char fnName[] = "CComponentConfigWnd::CreateImageBuffer_Map";
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
bool CComponentConfigWnd::CreateImageBuffer_Tmp()
{
	const char fnName[] = "CComponentConfigWnd::CreateImageBuffer_Tmp";
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::SwitchShowImage()
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
bool CComponentConfigWnd::BuildShowImage(size_t index)
{
	bool bIsOK = true;
	COMPONENT_CONFIG_IMAGE_MODE ImageMode = GetComponentConfigImageMode();
	switch ( ImageMode )
	{
	case COMPONENT_CONFIG_IMAGE_MAP:
		bIsOK = BuildShowImage_Map(index);
		break;
	case COMPONENT_CONFIG_IMAGE_TMP:
		bIsOK = BuildShowImage_Tmp(index);
		break;
	}
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::BuildShowImage_Map(size_t index)
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
bool CComponentConfigWnd::BuildShowImage_Tmp(size_t index)
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_ImagePtr ) { return false; }
	if ( 0 == m_ImageSize ) { return false; }
	
	::memset(m_ImagePtr, 0x00, sizeof(IMAGE_DATA)*m_ImageSize);
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(true);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, COMCFG_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	SetShowSelectLine(false);
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_LBtnUpPos = pt;
	
	POINT DifP;
	RECT  SelRect={0,0,0,0};	
	DifP.x = m_LBtnUpPos.x-m_LBtnDownPos.x;
	DifP.y = m_LBtnUpPos.y-m_LBtnDownPos.y;
	JetAPI::PointsToRect(m_LBtnDownPos, m_LBtnUpPos, SelRect);	

	ExecRectComponentConfig(SelRect);
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnMouseMove(UINT nFlags, CPoint point) 
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
BOOL CComponentConfigWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
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
void CComponentConfigWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	SetShowSelectLine(false);
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, COMCFG_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_RBtnUpPos = m_RBtnDownPos = m_LastPos = pt;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnRButtonUp(UINT nFlags, CPoint point) 
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
bool CComponentConfigWnd::ExecUpdateComponentBoard(CString &ErrorString)
{	
	size_t             i=0, j=0, k=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   BoardRectCount = GetBoardRectCount();
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		if ( -1 == ComponentConfigPtr->uBoardIndexNew )
		{
			ErrorString.Format(_T("Component [%s] does not set the board"), ComponentConfigPtr->strComponentName);
			return false; 
		}		
		if ( ComponentConfigPtr->uBoardIndexNew >= BoardRectCount ) 
		{
			ErrorString.Format(_T("Component [%s] board value is exception"), ComponentConfigPtr->strComponentName);
			return false; 
		}
	}	

	//確認是否重複
	size_t           NameCount=0;
	std::vector<CString> NameList;
	for ( i=0; i<BoardRectCount; i++ )
	{
		for ( j=0; j<ComponentConfigCount; j++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(j, false);
			if ( NULL == ComponentConfigPtr ) { continue; }		
			if ( ComponentConfigPtr->uBoardIndexNew != i ) { continue; }
			NameCount = NameList.size();
			for ( k=0; k<NameCount; k++ )
			{
				if ( NameList[k].CompareNoCase(ComponentConfigPtr->strComponentName) != 0 ) 
				{	continue; }

				ErrorString.Format(_T("Component [%s] name is repeated"), ComponentConfigPtr->strComponentName);
				return false;
			}
			NameList.push_back(ComponentConfigPtr->strComponentName);
		}
		NameList.clear();
	}
	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ExecSetComponentSelectedBoard()
{	
	CAOIPanel         *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	CString            str;
	size_t             i=0;			
	size_t             Count=0;
	TBoardRect         BoardRect;
	TREGION4D          BoardCadRgn;
	TRECT4D            BoardMapRect;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   BoardRectCount = GetBoardRectCount();
	const size_t   ComponentConfigCount = GetComponentConfigCount();

	BoardCadRgn.Limit(true);
	BoardMapRect.Limit(true);	
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		if ( false == ComponentConfigPtr->bSelected ) { continue; }
		if ( ComponentConfigPtr->uBoardIndexNew != -1 ) { continue; }//已有編號就跳過

		ComponentConfigPtr->uBoardIndexNew = (unsigned int)(BoardRectCount);
		BoardMapRect.Expand(ComponentConfigPtr->rcComponentMapRect);
		BoardCadRgn.Expand(ComponentConfigPtr->rgnComponentCadRgn);		

		Count ++;
	}	
	if ( 0 == Count ) { return true; }

	BoardRect.BoardIndex = BoardRectCount;
	BoardRect.PanelIndex = PanelPtr->GetPanelIndex_Project(); 
	BoardRect.PanelPtr = PanelPtr;
	BoardRect.BoardPtr = NULL;
	BoardRect.BoardRgn = BoardCadRgn;
	BoardRect.BoardMapRect = BoardMapRect;	
	m_BoardRectList.push_back(BoardRect);
	RedrawWnd();
	SetModified(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ExecApplyToOtherComponentBoard()//套用至其他零件的單板
{
	CAOIPanel         *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return false; }

	CString            str;
	size_t             i=0;			
	size_t             Count=0;	
	unsigned int       BoardIndexNew=0; 	
	TPOINT2D           BoardMapPos;
	TPOINT2D           ComponentMapPos;	
	TPOINT2D           ComponentMapOffset;	
	TPOINT2D           RefComponentMapPos;
	TPOINT2D           ActComponentMapPos;
	TREGION4D          BoardCadRgn;
	TRECT4D            BoardMapRect;
	TRECT4D            RefBoardMapRect;
	TRECT4D            ChkBoardMapRect;
	TBoardRect         BoardRect;	
	TBoardRect        *RefBoardRectPtr=NULL;	
	TComponentConfig  *ComponentConfigPtr = NULL;
	TComponentConfig  *RefComponentConfigPtr = NULL;
	TComponentConfig  *ActComponentConfigPtr = NULL;
	
	const size_t   BoardRectCount = GetBoardRectCount();
	const size_t   ComponentConfigCount = GetComponentConfigCount();
	BOOL bUserSelectMode = CWnd::IsDlgButtonChecked(COMCFG_USER_SELECT_MODE_CHK);

	if ( TRUE == bUserSelectMode )
	{	IncludeSelectedComponentConfigList();	}
	else
	{	IncludeVisibledComponentConfigList();	}
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		if ( ComponentConfigPtr->uBoardIndexNew != -1 )
		{	ComponentConfigPtr->bCalculated = true;	}
		else
		{	ComponentConfigPtr->bCalculated = false;	}
	}

	while ( true ) 
	{
		RefComponentConfigPtr = NULL;
		ActComponentConfigPtr = NULL;
		for ( i=0; i<ComponentConfigCount; i++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(i, false);
			if ( NULL == ComponentConfigPtr ) { continue; }
			if ( false == ComponentConfigPtr->bIncluded ) { continue; }
			if ( true == ComponentConfigPtr->bCalculated ) { continue; }
			ActComponentConfigPtr = ComponentConfigPtr;
			break; 
			
		}
		if ( NULL == ActComponentConfigPtr ) 
		{	break; }

		ActComponentConfigPtr->bCalculated = true;
		for ( i=0; i<ComponentConfigCount; i++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(i, false);
			if ( NULL == ComponentConfigPtr ) { continue; }
			if ( false == ComponentConfigPtr->bIncluded ) { continue; }
			if ( false == ComponentConfigPtr->bCalculated ) { continue; }
			if( ComponentConfigPtr == ActComponentConfigPtr ) { continue; }
			if ( ComponentConfigPtr->strComponentName.CompareNoCase(ActComponentConfigPtr->strComponentName) != 0 ) 
			{	continue; }
			RefComponentConfigPtr = ComponentConfigPtr;
			break; 
			
		}
		if ( NULL == RefComponentConfigPtr ) 
		{	continue; }		
		RefBoardRectPtr = GetBoardRectPtr(RefComponentConfigPtr->uBoardIndexNew, true);
		if ( NULL == RefBoardRectPtr ) { continue; }		
		RefBoardMapRect = RefBoardRectPtr->BoardMapRect;
		BoardMapPos.x = RefBoardMapRect.GetCpX();
		BoardMapPos.y = RefBoardMapRect.GetCpY();		
		RefComponentMapPos.x = RefComponentConfigPtr->rcComponentMapRect.GetCpX();
		RefComponentMapPos.y = RefComponentConfigPtr->rcComponentMapRect.GetCpY();
		ActComponentMapPos.x = ActComponentConfigPtr->rcComponentMapRect.GetCpX();
		ActComponentMapPos.y = ActComponentConfigPtr->rcComponentMapRect.GetCpY();
		ComponentMapOffset.x = ActComponentMapPos.x-RefComponentMapPos.x;
		ComponentMapOffset.y = ActComponentMapPos.y-RefComponentMapPos.y;
		ChkBoardMapRect = RefBoardMapRect;
		ChkBoardMapRect.Move(ComponentMapOffset);

		BoardIndexNew = (unsigned int)(GetBoardRectCount());
		ActComponentConfigPtr->uBoardIndexNew = BoardIndexNew;
		BoardCadRgn = ActComponentConfigPtr->rgnComponentCadRgn;		
		BoardMapRect = ActComponentConfigPtr->rcComponentMapRect;		
		for ( i=0; i<ComponentConfigCount; i++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(i, false);
			if ( NULL == ComponentConfigPtr ) { continue; }
			if ( false == ComponentConfigPtr->bVisibled ) { continue; }
			if ( true == ComponentConfigPtr->bCalculated ) { continue; }
			ComponentMapPos.x = ComponentConfigPtr->rcComponentMapRect.GetCpX();
			ComponentMapPos.y = ComponentConfigPtr->rcComponentMapRect.GetCpY();
			if ( ChkBoardMapRect.CheckPtInside(ComponentMapPos) == false ) { continue; }

			ComponentConfigPtr->bCalculated = true;
			ComponentConfigPtr->uBoardIndexNew = BoardIndexNew;
			BoardMapRect.Expand(ComponentConfigPtr->rcComponentMapRect);
			BoardCadRgn.Expand(ComponentConfigPtr->rgnComponentCadRgn);		
		}
		BoardRect.BoardIndex = BoardIndexNew;
		BoardRect.PanelIndex = 0;
		BoardRect.BoardPtr = NULL;
		BoardRect.BoardRgn = BoardCadRgn;
		BoardRect.BoardMapRect = BoardMapRect;
		if ( NULL != PanelPtr )
		{	
			BoardRect.PanelPtr = PanelPtr; 
			BoardRect.PanelIndex = PanelPtr->GetPanelIndex_Project(); 
		}
		m_BoardRectList.push_back(BoardRect);
	};	
	RedrawWnd();
	SetModified(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::ExecRectComponentConfig(RECT SelRect)
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
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   ComponentConfigCount = GetComponentConfigCount();

	MapStageCp.x = MapStageRgn.GetCpX();
	MapStageCp.y = MapStageRgn.GetCpY();	
	ImageAPI.MapWndRectToImageRect_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndSelRect, ImgSelRect);	

	if ( AOIDataCollect.CheckMultiSelectMode() == false ) 
	{	ResetComponentConfigListSelected();	}
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }
		if ( false == ComponentConfigPtr->bVisibled ) { continue; }
		ImageRect = ComponentConfigPtr->rcComponentMapRect;
		if ( ImageRect.minX > ImgSelRect.right ) { continue; }
		if ( ImageRect.minY > ImgSelRect.bottom ) { continue; }
		if ( ImageRect.maxX < ImgSelRect.left ) { continue; }
		if ( ImageRect.maxY < ImgSelRect.top  ) { continue; }

		if ( ImageRect.minX < ImgSelRect.left ) { continue; }		
		if ( ImageRect.minY < ImgSelRect.top ) { continue; }
		if ( ImageRect.maxX > ImgSelRect.right ) { continue; }
		if ( ImageRect.maxY > ImgSelRect.bottom  ) { continue; }
		
		ComponentConfigPtr->bSelected = true;
	}	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnSetBoardBtn() 
{
	// TODO: Add your control notification handler code here
	ExecSetComponentSelectedBoard();
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnApplyBoardBtn() 
{
	// TODO: Add your control notification handler code here
	ExecApplyToOtherComponentBoard();
}
//-------------------------------------------------------------------------------------//
void CComponentConfigWnd::OnClearBoardBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bUserSelectMode = CWnd::IsDlgButtonChecked(COMCFG_USER_SELECT_MODE_CHK);
	if ( FALSE == bUserSelectMode )
	{		
		ResetComponentConfigListBoardIndex(false);
		RemoveVisibleBoardRect();
	}
	else
	{	
		ResetComponentConfigListBoardIndex(true);
		RemoveNoComponentBoardRect();
	}	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CComponentConfigWnd::CheckComponentConfigFinish_BoardAssign(CString &ErrorString)
{
	size_t             i=0, j=0, k=0;
	TComponentConfig  *ComponentConfigPtr = NULL;
	const size_t   BoardRectCount = GetBoardRectCount();
	const size_t   ComponentConfigCount = GetComponentConfigCount();		
	for ( i=0; i<ComponentConfigCount; i++ )
	{
		ComponentConfigPtr = GetComponentConfigPtr(i, false);
		if ( NULL == ComponentConfigPtr ) { continue; }		
		if ( -1 == ComponentConfigPtr->uBoardIndexNew )
		{
			ErrorString.Format(_T("Component [%s] does not set the board"), ComponentConfigPtr->strComponentName);
			return false; 
		}		
		if ( ComponentConfigPtr->uBoardIndexNew >= BoardRectCount ) 
		{
			ErrorString.Format(_T("Component [%s] board value is exception"), ComponentConfigPtr->strComponentName);
			return false; 
		}
	}	

	//確認是否重複
	size_t           NameCount=0;
	std::vector<CString> NameList;
	for ( i=0; i<BoardRectCount; i++ )
	{
		for ( j=0; j<ComponentConfigCount; j++ )
		{
			ComponentConfigPtr = GetComponentConfigPtr(j, false);
			if ( NULL == ComponentConfigPtr ) { continue; }		
			if ( ComponentConfigPtr->uBoardIndexNew != i ) { continue; }
			NameCount = NameList.size();
			for ( k=0; k<NameCount; k++ )
			{
				if ( NameList[k].CompareNoCase(ComponentConfigPtr->strComponentName) != 0 ) 
				{	continue; }

				ErrorString.Format(_T("Component [%s] name is repeated"), ComponentConfigPtr->strComponentName);
				return false;
			}
			NameList.push_back(ComponentConfigPtr->strComponentName);
		}
		NameList.clear();
	}
	return true;
}
//-------------------------------------------------------------------------------------//