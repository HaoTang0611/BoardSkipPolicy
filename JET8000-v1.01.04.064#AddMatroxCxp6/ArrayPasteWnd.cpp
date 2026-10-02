// ArrayPasteWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ArrayPasteWnd.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CArrayPasteWnd dialog
//-------------------------------------------------------------------------------------//
CArrayPasteWnd::CArrayPasteWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CArrayPasteWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrayPasteWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	m_ImageIndex = 0;
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = m_ShowImageW*3;
	m_ShowBitCount = 24;	
	m_ShowImagePtr = NULL;

	m_SetRgn = false;
	m_ColPitch = 0.0;
	m_RowPitch = 0.0;
	m_ChangeSelName = true;
	m_ArrayPasteMode = ARRAY_PASTE_BOARD;
	m_NameMode = ARRAY_PASTE_NAME_DEFAULT;
	m_FrameResolution.x = 10;
	m_FrameResolution.y = 10;	
	PreInitUniFrameBuffer();
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrayPasteWnd)
	DDX_Control(pDX, APW_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CArrayPasteWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CArrayPasteWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(APW_SHOW_MAP_BTN, OnShowMapBtn)
	ON_WM_PAINT()
	ON_BN_CLICKED(APW_REGION_POS_TO_BTN1, OnRegionPosToBtn1)
	ON_BN_CLICKED(APW_REGION_POS_TO_BTN2, OnRegionPosToBtn2)
	ON_BN_CLICKED(APW_REGION_POS_GET_BTN1, OnRegionPosGetBtn1)
	ON_BN_CLICKED(APW_REGION_POS_GET_BTN2, OnRegionPosGetBtn2)
	ON_BN_CLICKED(APW_REGION_CALC_PITCH, OnRegionCalcPitch)
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(APW_MOTION_JOG_CHK, OnMotionJogChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CArrayPasteWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CArrayPasteWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	CWnd::ShowWindow(SW_MAXIMIZE);
	
	m_ImageWnd.SetShowCursorLine(false);
	m_ImageWnd.SetShowWndCenterLine(true);
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);

	m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
	m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, false);

	CWnd::SetDlgItemInt(APW_PARAM_COL_NUM_EDIT, 1);
	CWnd::SetDlgItemInt(APW_PARAM_ROW_NUM_EDIT, 1);
	CWnd::SetDlgItemInt(APW_PARAM_COL_PITCH_EDIT, 0);
	CWnd::SetDlgItemInt(APW_PARAM_ROW_PITCH_EDIT, 0);

	CWnd::SetDlgItemInt(APW_REGION_POS_X_EDIT1, 0);
	CWnd::SetDlgItemInt(APW_REGION_POS_Y_EDIT1, 0);
	CWnd::SetDlgItemInt(APW_REGION_POS_X_EDIT2, 0);
	CWnd::SetDlgItemInt(APW_REGION_POS_Y_EDIT2, 0);
	
	CWnd::CheckDlgButton(APW_MOTION_JOG_CHK, FALSE);

	UINT NameCtrlID=0;
	switch ( m_NameMode )
	{
	case ARRAY_PASTE_NAME_ROW_FIRST:	NameCtrlID = APW_NAME_ROW_COL_RAD;	break;
	case ARRAY_PASTE_NAME_COL_FIRST:	NameCtrlID = APW_NAME_COL_ROW_RAD;	break;
	default:
	case ARRAY_PASTE_NAME_DEFAULT:		NameCtrlID = APW_NAME_DEFAULT_RAD;	break;
	}
	CWnd::CheckDlgButton(NameCtrlID, TRUE);	
	CWnd::CheckDlgButton(APW_NAME_CHANGE_SEL_CHK, m_ChangeSelName);
	if ( ARRAY_PASTE_COMPONENT != m_ArrayPasteMode )
	{
		JetAPI::EnableCtrlWnd(this, APW_NAME_ROW_COL_RAD, FALSE);
		JetAPI::EnableCtrlWnd(this, APW_NAME_COL_ROW_RAD, FALSE);
		JetAPI::EnableCtrlWnd(this, APW_NAME_DEFAULT_RAD, FALSE);
		JetAPI::EnableCtrlWnd(this, APW_NAME_CHANGE_SEL_CHK, FALSE);
	}

	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);	
	MotionCtrlPtr->SetIsJogMode(false);		
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	ExecMoveToStage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustCtrlWndPosition();
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = 1120;
	lpMMI->ptMinTrackSize.y = 800;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnShowMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }

	m_ProjectMapWnd.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
BOOL CArrayPasteWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }

	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_LEFT:
			m_ImageWnd.MoveViewOffset(1, 0);
			m_ImageWnd.RedrawWnd(TRUE);
			break;
		case VK_UP:
			m_ImageWnd.MoveViewOffset(0, 1);
			m_ImageWnd.RedrawWnd(TRUE);
			break;
		case VK_RIGHT:
			m_ImageWnd.MoveViewOffset(-1, 0);
			m_ImageWnd.RedrawWnd(TRUE);
			break;
		case VK_DOWN:
			m_ImageWnd.MoveViewOffset(0, -1);
			m_ImageWnd.RedrawWnd(TRUE);
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CArrayPasteWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:		
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;	
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			ExecMoveToStage();
			break;
		}
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecMoveToStage();
		break;	
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		OnImageWndNotify(wParam, lParam);		
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
double CArrayPasteWnd::GetColPitch() const
{
	return m_ColPitch;
}
//-------------------------------------------------------------------------------------//
double CArrayPasteWnd::GetRowPitch() const
{	 
	return m_RowPitch;
}
//-------------------------------------------------------------------------------------//
int CArrayPasteWnd::GetNameMode() const
{
	return m_NameMode;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SetNameMode(int Mode)
{
	m_NameMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::GetChangeSelName() const
{
	return m_ChangeSelName;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SetArrayPasteMode(int Mode)
{
	m_ArrayPasteMode = Mode;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::GetIdxList(std::vector<POINT> &IdxList)
{
	IdxList = m_IdxList;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::GetPosList(std::vector<TPOINT2D> &PosList)
{
	PosList = m_PosList;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SetSelectedRgn(const TREGION4D &Rgn)
{
	m_SetRgn = true;
	m_SelectedRgn = Rgn;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SetMapCoordinate(CMapCoordinate *MapPtrCTS, CMapCoordinate *MapPtrSTC)
{
	m_CadToStageMap = *MapPtrCTS;
	m_StageToCadMap = *MapPtrSTC;	
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CArrayPasteWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
int CArrayPasteWnd::GetMaxFrameCount() const
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::PreInitUniFrameBuffer()//預先影像記憶體
{
	int i=0;	
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{	JetAPI::InitialUniFrame(m_UniFrameList[i]);	}	
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::ReleaseUniFrameBuffer()//釋放影像記憶體	
{
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::BuildShowImageBuffer(bool ResetView)//建立顯示的影像記憶體
{
	const char fnName[] = "CArrayPasteWnd::BuildShowImageBuffer";
	ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	size_t       i=0;
	TUNI_FRAME   UniFrame;
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	const size_t MaxUniFrameCount = GetMaxFrameCount();
	if ( MapIndex >= MaxUniFrameCount ) { MapIndex = 0; }
	if ( NULL == m_UniFrameList[MapIndex].ImagePtr )
	{	MapIndex = 0;	}	
	UniFrame = m_UniFrameList[MapIndex];
	ImageW    = UniFrame.ImageW;
	ImageH    = UniFrame.ImageH;
	ImageStep = UniFrame.ImageStep;
	BitCount  = UniFrame.BitCount;
	ImagePtr  = UniFrame.ImagePtr;

	if ( NULL == ImagePtr )
	{
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*ShowBufferSize);
	}
	else
	{
		m_ShowBitCount = 24;
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, m_ShowBitCount, 4);
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);		
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		if ( ShowBufferSize == BufferSize )
		{	::memcpy(m_ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
		else
		{
			if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ShowImageStep, m_ShowImagePtr, false) == false )
			{
				ReleaseShowImageBuffer();
				return;
			}
		}
	}

	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	if ( DRAW_IMAGE_BY_RAW != DrawImageMode )
	{
		AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_NORMAL);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ShowImagePtr);
	}
	else
	{	AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_BY_RAW);	}
	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.SetImageInfo(CameraID, m_FrameStageRgn, m_FrameResolution, IMAGE_DATA_FOV);
	m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, true, ResetView);
	//m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(FALSE);
	return;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::ReleaseShowImageBuffer()//釋放顯示影像記憶體	
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::FillCurrentFrames(double Ratio)
{
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	const size_t MaxFrames = GetMaxFrameCount();	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();		

	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);
	AOIDataCollect.GetStagePos(Pos.x, Pos.y, Pos.z);

	m_FrameResolution.x = Res.cx;
	m_FrameResolution.y = Res.cy;
	m_FrameStageRgn.minX = Pos.x-(FOVWum*0.5);
	m_FrameStageRgn.maxX = Pos.x+(FOVWum*0.5);
	m_FrameStageRgn.minY = Pos.y-(FOVHum*0.5);
	m_FrameStageRgn.maxY = Pos.y+(FOVHum*0.5);

	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	double FovRatio = Ratio;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return true; }

	HCURSOR hCursor=NULL;
	HCURSOR hOldCursor=NULL;
	CWinApp *AppPtr = ::AfxGetApp();
	if ( NULL != AppPtr )
	{	
		hCursor = AppPtr->LoadStandardCursor(IDC_WAIT); 
		hOldCursor = ::SetCursor(hCursor);
	}		
		
	ImageW = (IMAGE_SIZE)(ImageW*Ratio);
	ImageH = (IMAGE_SIZE)(ImageH*Ratio);
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, m_UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	
	unsigned int ImageIndex = ProjectPtr->GetProjectMapIndex();
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( NULL == m_UniFrameList[i].ImagePtr ) { continue; }		
		m_UniFrameList[i].ImageW = ImageW;
		m_UniFrameList[i].ImageH = ImageH;
	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);	
	if ( NULL != hOldCursor )
	{	::SetCursor(hOldCursor);	}

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowBitCount = 24;	
	m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, m_ShowBitCount, 4);
	return true;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::AdjustCtrlWndPosition()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;	
	RECT  WndRect={0};
	RECT  MainRect={0};
	RECT  ImageWndRect={0};
	const int MarginX = 4;
	const int MarginY = 4;

	CWnd::GetClientRect(&MainRect);
	ImageWndRect = MainRect;
	::InflateRect(&ImageWndRect, -MarginX, -MarginY);
	m_ImageWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	//WndRect.left = ImageWndRect.left;
	WndRect.top = ImageWndRect.top;
	WndRect.right = ImageWndRect.right;
	WndRect.bottom = ImageWndRect.bottom;
	m_ImageWnd.MoveWindow(&WndRect);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SwitchMultiLanguage()
{
	CString Section=_T("IDD_ARRAY_PASTE_WND");
	CString WndKey;
	int WndID = 0;
	CString LabelText, NewLabelText;
	CWnd *pWnd = NULL;
	//---------------------------------------------------------------------------------//
	WndID = IDD_ARRAY_PASTE_WND;
	WndKey = _T("IDD_ARRAY_PASTE_WND");
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
	WndID = APW_PARAM_GROUP;
	WndKey = _T("APW_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_PARAM_COL_NUM_LABEL;
	WndKey = _T("APW_PARAM_COL_NUM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_PARAM_ROW_NUM_LABEL;
	WndKey = _T("APW_PARAM_ROW_NUM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_PARAM_COL_PITCH_LABEL;
	WndKey = _T("APW_PARAM_COL_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_PARAM_ROW_PITCH_LABEL;
	WndKey = _T("APW_PARAM_ROW_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = APW_PARAM_COL_UNIT_LABEL;
	WndKey = _T("APW_PARAM_COL_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_PARAM_ROW_UNIT_LABEL;
	WndKey = _T("APW_PARAM_ROW_UNIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = APW_REGION_GROUP;
	WndKey = _T("APW_REGION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_X_LABEL;
	WndKey = _T("APW_REGION_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = APW_REGION_Y_LABEL;
	WndKey = _T("APW_REGION_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_LABEL1;
	WndKey = _T("APW_REGION_POS_LABEL1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_LABEL2;
	WndKey = _T("APW_REGION_POS_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_TO_BTN1;
	WndKey = _T("APW_REGION_POS_TO_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_GET_BTN1;
	WndKey = _T("APW_REGION_POS_GET_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_TO_BTN2;
	WndKey = _T("APW_REGION_POS_TO_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_POS_GET_BTN2;
	WndKey = _T("APW_REGION_POS_GET_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_REGION_CALC_PITCH;
	WndKey = _T("APW_REGION_CALC_PITCH");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = APW_REGION_CALC_PITCH;
	WndKey = _T("APW_REGION_CALC_PITCH");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = APW_SHOW_MAP_BTN;
	WndKey = _T("APW_SHOW_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = APW_NAME_GROUP;
	WndKey = _T("APW_NAME_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_NAME_DEFAULT_RAD;
	WndKey = _T("APW_NAME_DEFAULT_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_NAME_ROW_COL_RAD;
	WndKey = _T("APW_NAME_ROW_COL_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_NAME_COL_ROW_RAD;
	WndKey = _T("APW_NAME_COL_ROW_RAD");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = APW_NAME_CHANGE_SEL_CHK;
	WndKey = _T("APW_NAME_CHANGE_SEL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	

	/*
	WndID = APW_LAYOUT_GROUP;
	WndKey = _T("APW_LAYOUT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
	/*
APW_PARAM_COL_NUM_EDIT
APW_PARAM_ROW_NUM_EDIT
APW_PARAM_COL_PITCH_EDIT
APW_PARAM_ROW_PITCH_EDIT

APW_REGION_X_LABEL
APW_REGION_Y_LABEL

APW_REGION_POS_X_LABEL1
APW_REGION_POS_X_EDIT1
APW_REGION_POS_Y_EDIT1
APW_REGION_POS_X_EDIT
APW_REGION_POS_Y_EDIT2
	*/
}
//-------------------------------------------------------------------------------------//
CString CArrayPasteWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ARRAY_PASTE_WND");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::CreateBKImage()
{
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::LockUIWnd(bool bLock)
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::GetLockUIWnd() const
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::ExecMoveToStage()
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	if ( true == OfflineMode )
	{	return ExecUpdateFov(PosX, PosY, PosZ);	}
	return ExecGrabFov(PosX, PosY, PosZ);	
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::ExecShowWndPosition()
{	
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();	
	BuildShowImageBuffer(true);
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::ExecGrabFov(double PosX, double PosY, double PosZ)
{
	CString str;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);	
	//AOIDataCollect.MoveCameraToProjectFocusPos(ProjectPtr);
#ifndef LIGHT_CTRL_DISABLE
	this->LockUIWnd(true);	
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
	return true;
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::ExecUpdateFov(double PosX, double PosY, double PosZ)
{
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double FovSizeWd2 = FovSizeW/2;
	const double FovSizeHd2 = FovSizeH/2;
	const double FovSizeWd4 = FovSizeW/4;
	const double FovSizeHd4 = FovSizeH/4;
	const double ZoomMin = AOIDataCollect.GetImageZoomMin();
	const double ZoomMax = AOIDataCollect.GetImageZoomMax();
	const double FovZoomX = FovMinW/FovSizeW;
	const double FovZoomY = FovMinH/FovSizeH;
	const double FovZoomNeed = MAX(FovZoomX, FovZoomY);	
	const double FovZoomNeedUsed = JetAPI::AdjustValue(FovZoomNeed, 0.5);

	TPOINT2D ImageRes;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;	

	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);	
	
	AOIDataCollect.ResetFovTargetParam();

	double Ratio = MAX(1.0, FovZoomNeedUsed);
	FillCurrentFrames(Ratio);

	BuildShowImageBuffer(true);	
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	CString str;	
	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}		
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }
	
	LockUIWnd(false);
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }

	const double Ratio = 1.0;
	double PosX=0, PosY=0, PosZ=0;
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);	
	double     ImageResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	double     ImageResY = AOIDataCollect.GetCameraResolutionY(CameraID);	
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);		
	m_FrameResolution.x = ImageResX;
	m_FrameResolution.y = ImageResY;
	m_FrameStageRgn.minX = PosX-(FOVWum*0.5);
	m_FrameStageRgn.maxX = PosX+(FOVWum*0.5);
	m_FrameStageRgn.minY = PosY-(FOVHum*0.5);
	m_FrameStageRgn.maxY = PosY+(FOVHum*0.5);

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	const size_t GrabFrameParamCount = GrabFrameParamList.size();
	if ( 0 == GrabFrameParamCount ) { return false; }

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}	
	if ( NULL == ProjectPtr )
	{	m_ImageIndex = 0; }
	else
	{	m_ImageIndex = ProjectPtr->GetProjectMapIndex(); }	
	const size_t UniFrameCount = UniFrameList.size();
	const size_t MinUniFrameCount = MIN(FRAME_MAX_COUNT, UniFrameCount);
	for ( i=0; i<MinUniFrameCount; i++ )
	{	m_UniFrameList[i] = UniFrameList[i];	}
	for ( i=MinUniFrameCount; i<UniFrameCount; i++ )
	{	JetAPI::ClearUniFrame(UniFrameList[i]);	}
	BuildShowImageBuffer(bCameraCallBack);	
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnRegionPosToBtn1() 
{
	// TODO: Add your control notification handler code here
	CString strX;
	CString strY;
	double PosX = 0;
	double PosY = 0;	
	CWnd::GetDlgItemText(APW_REGION_POS_X_EDIT1, strX);
	CWnd::GetDlgItemText(APW_REGION_POS_Y_EDIT1, strY);
	PosX = JetAPI::StrToDbl(strX);
	PosY = JetAPI::StrToDbl(strY);
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnRegionPosToBtn2() 
{
	// TODO: Add your control notification handler code here
	CString strX;
	CString strY;
	double PosX = 0;
	double PosY = 0;
	CWnd::GetDlgItemText(APW_REGION_POS_X_EDIT2, strX);
	CWnd::GetDlgItemText(APW_REGION_POS_Y_EDIT2, strY);
	PosX = JetAPI::StrToDbl(strX);
	PosY = JetAPI::StrToDbl(strY);
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnRegionPosGetBtn1() 
{
	// TODO: Add your control notification handler code here
	CString  strX;
	CString  strY;
	TPOINT2D StagePos;
	StagePos = m_ImageWnd.GetStagePosAtWndCenterPos();
	strX.Format(_T("%.0f"), StagePos.x);
	strY.Format(_T("%.0f"), StagePos.y);
	CWnd::SetDlgItemText(APW_REGION_POS_X_EDIT1, strX);
	CWnd::SetDlgItemText(APW_REGION_POS_Y_EDIT1, strY);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnRegionPosGetBtn2() 
{
	// TODO: Add your control notification handler code here
	CString  strX;
	CString  strY;
	TPOINT2D StagePos;
	StagePos = m_ImageWnd.GetStagePosAtWndCenterPos();
	strX.Format(_T("%.0f"), StagePos.x);
	strY.Format(_T("%.0f"), StagePos.y);
	CWnd::SetDlgItemText(APW_REGION_POS_X_EDIT2, strX);
	CWnd::SetDlgItemText(APW_REGION_POS_Y_EDIT2, strY);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnRegionCalcPitch() 
{
	// TODO: Add your control notification handler code here
	CString   str, str1, str2;
	CString   strX1, strY1;
	CString   strX2, strY2;
	double    PitchX=0;
	double    PitchY=0;
	double    CadPosX1=0, CadPosY1=0;
	double    CadPosX2=0, CadPosY2=0;
	double    StagePosX1=0, StagePosY1=0;
	double    StagePosX2=0, StagePosY2=0;
	const int nCols = CWnd::GetDlgItemInt(APW_PARAM_COL_NUM_EDIT);
	const int nRows = CWnd::GetDlgItemInt(APW_PARAM_ROW_NUM_EDIT);
	const int nMaxCount = MAX(nCols, nRows);
	if ( nCols<1 || nRows<1 )
	{
		str = _T("Error, Col or Row value is exception!");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s (Col:%d, Row:%d)"), str, nCols, nRows);
		JetAPI::ShowMessageBox(str2);
		return ;
	}	
	CWnd::GetDlgItemText(APW_REGION_POS_X_EDIT1, strX1);
	CWnd::GetDlgItemText(APW_REGION_POS_Y_EDIT1, strY1);
	CWnd::GetDlgItemText(APW_REGION_POS_X_EDIT2, strX2);
	CWnd::GetDlgItemText(APW_REGION_POS_Y_EDIT2, strY2);
	StagePosX1 = JetAPI::StrToDbl(strX1);
	StagePosY1 = JetAPI::StrToDbl(strY1);
	StagePosX2 = JetAPI::StrToDbl(strX2);
	StagePosY2 = JetAPI::StrToDbl(strY2);

	m_StageToCadMap.Map2D(StagePosX1, StagePosY1, CadPosX1, CadPosY1);
	m_StageToCadMap.Map2D(StagePosX2, StagePosY2, CadPosX2, CadPosY2);

	if ( 1==nCols || 1==nRows )
	{	
		if ( 1 == nMaxCount )
		{
			PitchX = (CadPosX2-CadPosX1)/(nMaxCount);
			PitchY = (CadPosY2-CadPosY1)/(nMaxCount);
		}
		else
		{
			PitchX = (CadPosX2-CadPosX1)/(nMaxCount-1);
			PitchY = (CadPosY2-CadPosY1)/(nMaxCount-1);
		}
	}
	else
	{
		PitchX = (CadPosX2-CadPosX1)/(nCols-1);
		PitchY = (CadPosY2-CadPosY1)/(nRows-1);
	}	
	str1.Format(_T("%.4f"), PitchX);
	str2.Format(_T("%.4f"), PitchY);
	CWnd::SetDlgItemText(APW_PARAM_COL_PITCH_EDIT, str1);
	CWnd::SetDlgItemText(APW_PARAM_ROW_PITCH_EDIT, str2);
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnClose();
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnOK() 
{
	// TODO: Add extra validation here
	bool bAlignMapPos = false;
	if ( true == m_SetRgn )
	{
		CString str;
		str = _T("Do you want to Auto-Adjust Position?");
		str = LoadMultiLanguageString(str, str);
		const int Ret = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
		if ( IDCANCEL == Ret ) { return; }
		if ( IDYES == Ret )
		{	bAlignMapPos = true;	}
	}
	if ( BuildPosList(bAlignMapPos) == false )
	{	return; }

	if ( CWnd::IsDlgButtonChecked(APW_NAME_ROW_COL_RAD) == TRUE )
	{	m_NameMode = ARRAY_PASTE_NAME_ROW_FIRST;	}
	else if ( CWnd::IsDlgButtonChecked(APW_NAME_COL_ROW_RAD) == TRUE )
	{	m_NameMode = ARRAY_PASTE_NAME_COL_FIRST;	}
	else
	{	m_NameMode = ARRAY_PASTE_NAME_DEFAULT;	}		

	if ( CWnd::IsDlgButtonChecked(APW_NAME_CHANGE_SEL_CHK) == TRUE )
	{	m_ChangeSelName = true;	}
	else
	{	m_ChangeSelName = false; }	

	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here		
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::SwitchFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();	
	if ( NULL == ProjectPtr ) { return; }
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();
	if ( false == SwitchFrameMode ) { return; }
	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);	
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	//ExecMoveToStage();
	BuildShowImageBuffer(false);
	CreateBKImage();
	RedrawWnd();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{	
	if ( WPARAM_CONTEXT_MENU == wParam )
	{	SwitchFrameImage();	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CArrayPasteWnd::OnMotionJogChk() 
{
	// TODO: Add your control notification handler code here
	bool bJog = false;
	BOOL bCheck = CWnd::IsDlgButtonChecked(APW_MOTION_JOG_CHK);	
	if ( TRUE == bCheck )
	{	bJog = true; }
	else
	{	bJog = false; }
	MotionCtrlPtr->SetIsJogMode(bJog); 
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::BuildPosList(bool bAlignMapPos)//建立位置列表
{
	if ( PitchPosList() == false ) { return false; }	
	if ( true == bAlignMapPos )
	{
		if ( AlignPosList() == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::PitchPosList()//等間距位置列表
{
	size_t   i=0;
	size_t   j=0;
	size_t   k=0;
	POINT    Idx;
	TPOINT2D Pos;
	CString str;
	CString strX;
	CString strY;	
	const int nCols = CWnd::GetDlgItemInt(APW_PARAM_COL_NUM_EDIT);
	const int nRows = CWnd::GetDlgItemInt(APW_PARAM_ROW_NUM_EDIT);
	const int nMaxCount = MAX(nCols, nRows);
	CWnd::GetDlgItemText(APW_PARAM_COL_PITCH_EDIT, strX);
	CWnd::GetDlgItemText(APW_PARAM_ROW_PITCH_EDIT, strY);

	m_PosList.clear();
	m_IdxList.clear();
	m_ColPitch = JetAPI::StrToDbl(strX);
	m_RowPitch = JetAPI::StrToDbl(strY);	

	if ( nCols>1 && fabs(m_ColPitch)<0.0001 )
	{
		str = _T("Error, X-Pitch is exception!");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;		
	}
	if ( nRows>1 && fabs(m_RowPitch)<0.0001 )
	{
		str = _T("Error, Y-Pitch is exception!");
		str = LoadMultiLanguageString(str, str);
		JetAPI::ShowMessageBox(str);
		return false;		
	}

	if ( 1==nRows || 1==nCols )
	{
		Idx.x = 0;
		Idx.y = 0;
		for ( i=1; i<nMaxCount; i++ )
		{
			Pos.x = (i*m_ColPitch);
			Pos.y = (i*m_RowPitch);
			m_PosList.push_back(Pos);

			if ( 1 == nRows )
			{	Idx.x = i;	}
			else
			{	Idx.y = i;	}
			m_IdxList.push_back(Idx);
		}
	}
	else
	{
		for ( i=0; i<nRows; i++ )
		{
			for ( j=0; j<nCols; j++ )		
			{
				if ( 0==i && 0==j ) { continue; }

				Pos.x = (j*m_ColPitch);
				Pos.y = (i*m_RowPitch);
				m_PosList.push_back(Pos);

				Idx.x = j;
				Idx.y = i;
				m_IdxList.push_back(Idx);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CArrayPasteWnd::AlignPosList()//對齊位置列表
{
	if ( false == m_SetRgn ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }				
	
	IMAGE_PTR  MapPtr=NULL;
	IMAGE_SIZE MapW=0, MapH=0, MapStep=0, BitCount=0;		
	const unsigned int MapIndex=ProjectPtr->GetProjectMapIndex();
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, MapW, MapH, MapStep, BitCount, MapPtr) == false )
	{	return false; }
	if ( ProjectPtr->CreateProjectMapShowPtr(MapIndex, MapPtr, false) == false )
	{	return false; }
	//ProjectPtr->GetProjectMapShowInfo(ImageRes, RgnCad, RgnStage);
	//ProjectPtr->GetProjectMapShowPtr(MapIndex, MapW, MapH, MapStep, BitCount, MapPtr);	

	TPOINT2D MapRes;
	TREGION4D MapRgn;			
	TPOINT2D StagePosMin, MapPosMin, CadPosMin;	
	TPOINT2D StagePosMax, MapPosMax, CadPosMax;
	TREGION4D SelCadRgn;
	TREGION4D SelStageRgn=m_SelectedRgn;
	CMapCoordinate CadToStageMap=m_CadToStageMap;	
	CMapCoordinate StageToCadMap=m_StageToCadMap;	
	std::vector<TPOINT2D> &PosList=m_PosList;
	const size_t PosCount=PosList.size();
	LANE_ID LaneID=ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	

	StageToCadMap.Map2D(SelStageRgn.minX, SelStageRgn.minY, CadPosMin.x, CadPosMin.y);
	StageToCadMap.Map2D(SelStageRgn.maxX, SelStageRgn.maxY, CadPosMax.x, CadPosMax.y);
	SelCadRgn.minX = MIN(CadPosMin.x, CadPosMax.x);
	SelCadRgn.minY = MIN(CadPosMin.y, CadPosMax.y);
	SelCadRgn.maxX = MAX(CadPosMin.x, CadPosMax.x);
	SelCadRgn.maxY = MAX(CadPosMin.y, CadPosMax.y);

	StagePosMin.x=SelStageRgn.minX;
	StagePosMin.y=SelStageRgn.minY;
	StagePosMax.x=SelStageRgn.maxX;
	StagePosMax.y=SelStageRgn.maxY;
	ProjectPtr->GetProjectMapResolution(MapRes.x, MapRes.y);
	ProjectPtr->MapProjectStageToMapPos(StagePosMin.x, StagePosMin.y, MapPosMin.x, MapPosMin.y, LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(StagePosMax.x, StagePosMax.y, MapPosMax.x, MapPosMax.y, LaneID, DistrictID);
	MapRgn.minX = MIN(MapPosMin.x, MapPosMax.x);
	MapRgn.minY = MIN(MapPosMin.y, MapPosMax.y);
	MapRgn.maxX = MAX(MapPosMin.x, MapPosMax.x);
	MapRgn.maxY = MAX(MapPosMin.y, MapPosMax.y);
	const double MapRgnW = MapRgn.GetWidth();
	const double MapRgnH = MapRgn.GetHeight();
	
	CString str;
	CString strFolder;
	const bool bSaveImage=false;
	strFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ArrayPaste"));
	::CreateDirectory(strFolder, NULL);
	if ( true == bSaveImage )
	{	JetAPI::ClearFolder(strFolder);	}

	RECT MapRect;
	JetAPI::Region4DToRect(MapRgn, MapRect, true);	
	const int MapRectW=MapRect.right-MapRect.left;
	const int MapRectH=MapRect.bottom-MapRect.top;
	const int MapRectX=(int)((MapRect.left+MapRect.right)*0.5);
	const int MapRectY=(int)((MapRect.top+MapRect.bottom)*0.5);

	CJetMatch Match;	//影像匹配
	const bool bRobustness = true;
	const int nMaxPositions = 1;
	const int nMinReduceArea = 64;
	const int nFinalReduction = 0;	
	const bool bUseInterpolate=true;
	const float fMinScore = 0.40f;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType) == false )
	{	return false;	}

	//initial eMatch
	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);

	IMAGE_PTR  PatPtr=NULL;	
	const IMAGE_SIZE PatW=MapRectW;
	const IMAGE_SIZE PatH=MapRectH;
	const IMAGE_SIZE PatStep=JetAPI::GetBMPImagePixelsPerLine(PatW, BitCount, 4);	
	if ( ImageAPI.ExtractRoiImage(MapW, MapH, MapStep, BitCount, MapPtr, MapRect, PatStep, PatPtr, false) == false )
	{	return false; }

	if ( true == bSaveImage )
	{
		str.Format(_T("%s\\%s"), strFolder, _T("SelPattern.PNG"));
		ImageAPI.SaveImage(str, PatW, PatH, PatStep, BitCount, PatPtr, true);
	}
	if ( Match.LearnPattern(PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
	{
		Match.SetMinReducedArea(nMinReduceArea*4);
		if ( Match.LearnPattern(PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
		{
			JetMemory.free_func(PatPtr);		
			return false;
		}
	}	

	TREGION4D SearchMapRgn;	
	TREGION4D SearchMapRgnOrg;
	const double SearchMapExtW=MapRgnW*0.5;
	const double SearchMapExtH=MapRgnH*0.5;
	for ( size_t i=0; i<PosCount; i++ )
	{
		TPOINT2D CadOffsetPos = PosList[i];
		CadPosMin.x = SelCadRgn.minX+CadOffsetPos.x;
		CadPosMin.y = SelCadRgn.minY+CadOffsetPos.y;
		CadPosMax.x = SelCadRgn.maxX+CadOffsetPos.x;
		CadPosMax.y = SelCadRgn.maxY+CadOffsetPos.y;
		CadToStageMap.Map2D(CadPosMin.x, CadPosMin.y, StagePosMin.x, StagePosMin.y);
		CadToStageMap.Map2D(CadPosMax.x, CadPosMax.y, StagePosMax.x, StagePosMax.y);		
		ProjectPtr->MapProjectStageToMapPos(StagePosMin.x, StagePosMin.y, MapPosMin.x, MapPosMin.y, LaneID, DistrictID);
		ProjectPtr->MapProjectStageToMapPos(StagePosMax.x, StagePosMax.y, MapPosMax.x, MapPosMax.y, LaneID, DistrictID);
		SearchMapRgn.minX = MIN(MapPosMin.x, MapPosMax.x);
		SearchMapRgn.minY = MIN(MapPosMin.y, MapPosMax.y);
		SearchMapRgn.maxX = MAX(MapPosMin.x, MapPosMax.x);
		SearchMapRgn.maxY = MAX(MapPosMin.y, MapPosMax.y);

		SearchMapRgnOrg = SearchMapRgn;
		SearchMapRgn.minX -= SearchMapExtW;
		SearchMapRgn.minY -= SearchMapExtH;
		SearchMapRgn.maxX += SearchMapExtW;
		SearchMapRgn.maxY += SearchMapExtH;

		SearchMapRgn.minX = MAX(0, SearchMapRgn.minX);
		SearchMapRgn.minY = MAX(0, SearchMapRgn.minY);
		SearchMapRgn.maxX = MIN(MapW, SearchMapRgn.maxX);
		SearchMapRgn.maxY = MIN(MapH, SearchMapRgn.maxY);		
		
		RECT SearchMapRect;//搜尋區域		
		IMAGE_PTR RoiPtr=NULL;
		IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;
		JetAPI::Region4DToRect(SearchMapRgn, SearchMapRect, true);			
		RoiW = SearchMapRect.right-SearchMapRect.left;
		RoiH = SearchMapRect.bottom-SearchMapRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
		if ( ImageAPI.ExtractRoiImage(MapW, MapH, MapStep, BitCount, MapPtr, SearchMapRect, RoiStep, RoiPtr, false) == false )
		{	continue;	}
		if ( true == bSaveImage )
		{
			str.Format(_T("%s\\Roi[%06d].PNG"), strFolder, i+1);
			ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
		}	
	
		Match.SetMaxPositions(nMaxPositions);
		Match.SetInterpolate(bUseInterpolate);	
		Match.SetMinScore(fMinScore);//fMinScore
		if ( Match.Match(RoiW, RoiH, RoiStep, BitCount, RoiPtr, true) == false )
		{	
			JetMemory.free_func(RoiPtr);
			continue;
		}
		const int ResultCount=Match.GetNumPositions();
		JetMemory.free_func(RoiPtr);
		if ( 0 == ResultCount ) { continue; }
		TPOINT2D RoiCadOffset, RoiImgOffset;
		const double RoiPosX = Match.GetResultPosX(0);
		const double RoiPosY = Match.GetResultPosY(0);
		const double SearchMapCpAfterX = SearchMapRgn.GetCpX();
		const double SearchMapCpAfterY = SearchMapRgn.GetCpY();
		const double SearchMapCpBeforeX = SearchMapRgnOrg.GetCpX();
		const double SearchMapCpBeforeY = SearchMapRgnOrg.GetCpY();
		const double SearchMapOffsetX = SearchMapCpAfterX-SearchMapCpBeforeX;
		const double SearchMapOffsetY = SearchMapCpAfterY-SearchMapCpBeforeY;
		const double SearchMapCpX=(SearchMapRect.right-SearchMapRect.left)*0.5;
		const double SearchMapCpY=(SearchMapRect.bottom-SearchMapRect.top)*0.5;
		const double SearchMapCpX2=SearchMapCpX-SearchMapOffsetX;
		const double SearchMapCpY2=SearchMapCpY-SearchMapOffsetY;
		RoiImgOffset.x = RoiPosX-SearchMapCpX2;
		RoiImgOffset.y = RoiPosY-SearchMapCpY2;		
		AOIDataCollect.MapImageOffsetToCad(RoiImgOffset, MapRes, RoiCadOffset);
		PosList[i].x += RoiCadOffset.x;
		PosList[i].y += RoiCadOffset.y;
		RoiPtr = NULL;
	}
	JetMemory.free_func(PatPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//