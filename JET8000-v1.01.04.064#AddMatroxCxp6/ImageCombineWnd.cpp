// ImageCombineWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ImageCombineWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageCombineWnd dialog
//-------------------------------------------------------------------------------------//
CImageCombineWnd::CImageCombineWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImageCombineWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageCombineWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	size_t     i=0;
	POINT      Pt={0,0};
	RECT       Rect={0,0,0,0};
	const size_t MaxCount = GetMaxCount();

	m_MapIndex = 0;
	m_ShowW = 0;
	m_ShowH = 0;
	m_ShowStep = 0;
	m_ShowBitCnt = 8;
	m_ShowBuffer = NULL;	
	m_ShowSize = 0;
	m_ShowInfoPtr = NULL;

	m_MapLoad_R = false;
	m_MapLoad_L = false;
	m_CombineMapMode = COMBINE_MAP_BY_RIGHT;

	m_LastPos = Pt;
	m_MovingPos = Pt;
	m_LBtnUpPos = Pt;
	m_LBtnDownPos = Pt;	
	m_RBtnUpPos = Pt;
	m_RBtnDownPos = Pt;	

	m_MapResultRect_R = Rect;
	m_MapResultRect_L = Rect;
	for ( i=0; i<MaxCount; i++ )
	{
		m_MapW[i] = 0;
		m_MapH[i] = 0;
		m_BitCount[i] = 8;	
		m_MapStep[i] = 0;
		m_MapBuffer[i] = NULL;		
		m_MapSize[i] = 0;	

		m_MapW_R[i] = 0;
		m_MapH_R[i] = 0;	
		m_MapStep_R[i] = 0;
		m_BitCount_R[i] = 8;	
		m_MapBuffer_R[i] = NULL;		
		m_MapSize_R[i] = 0;	

		m_MapW_L[i] = 0;
		m_MapH_L[i] = 0;	
		m_MapStep_L[i] = 0;
		m_BitCount_L[i] = 8;	
		m_MapBuffer_L[i] = NULL;		
		m_MapSize_L[i] = 0;		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageCombineWnd)
	DDX_Control(pDX, ICW_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImageCombineWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImageCombineWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()	
	ON_BN_CLICKED(ICW_COMBINE_IMAGE_BTN, OnCombineImageBtn)
	ON_BN_CLICKED(ICW_LOAD_IMAGE_BTN1, OnLoadImageBtn1)
	ON_BN_CLICKED(ICW_LOAD_IMAGE_BTN2, OnLoadImageBtn2)
	ON_BN_CLICKED(ICW_AUTO_MATCH_IMAGE_BTN, OnAutoMatchImageBtn)
	ON_BN_CLICKED(ICW_SHOW_RECT_LINE_CHK, OnShowRectLineChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageCombineWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CImageCombineWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);	
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC.CreateMemDC(m_ImageWnd, 0x000000);
	SwitchMultiLanguage();

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::CheckDlgButton(ICW_SHOW_RECT_LINE_CHK, TRUE);
	OnAutoMatchImageBtn();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearShowBuffer();
	ClearMapBuffer_R();
	ClearMapBuffer_L();
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	SIZE   WndSize={0,0};
	POINT  Offset={0,0};
	POINT  WndPos={0,0};
	CWnd  *WndPtr=NULL;
	RECT   WndRect={0,0,0,0};
	int       WndR=cx;
	const int GapX=4;
	const int GapY=4;

	WndPtr = CWnd::GetDlgItem(IDOK);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndPos.x = (WndRect.right+WndRect.left)/2;
		WndPos.y = (WndRect.top+WndRect.bottom)/2;

		int PosX = cx-GapX-(WndSize.cx/2);
		Offset.x = PosX-WndPos.x;
		Offset.y = 0;

		WndR = PosX-(WndSize.cx/2);
	}

	JetAPI::MoveCtrlWnd(this, IDOK, Offset);
	JetAPI::MoveCtrlWnd(this, IDCANCEL, Offset);
	JetAPI::MoveCtrlWnd(this, ICW_SHOW_RECT_LINE_CHK, Offset);
	JetAPI::MoveCtrlWnd(this, ICW_LOAD_IMAGE_BTN1, Offset);
	JetAPI::MoveCtrlWnd(this, ICW_LOAD_IMAGE_BTN2, Offset);
	JetAPI::MoveCtrlWnd(this, ICW_COMBINE_IMAGE_BTN, Offset);
	JetAPI::MoveCtrlWnd(this, ICW_AUTO_MATCH_IMAGE_BTN, Offset);	

	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.left = GapX;
		WndRect.top  = GapY;
		WndRect.right = WndR-GapX;
		WndRect.bottom = cy-GapY;
		m_ImageWnd.MoveWindow(&WndRect);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(m_ImageWnd, 0x000000);
		CreateBKImage();
		RedrawWnd();
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  768;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ICW_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = m_MovingPos = pt;
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_LBtnUpPos = m_MovingPos = pt;

	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( CWnd::GetCapture() != this ) 
	{
		return CBaseDialog::OnMouseMove(nFlags, point);
	}
	POINT Dp={0,0};
	POINT pt = point;
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_MovingPos = pt;
	Dp.x = m_MovingPos.x-m_LastPos.x;
	Dp.y = m_MovingPos.y-m_LastPos.y;
	if ( nFlags&MK_LBUTTON )
	{	ExecMoveMap(Dp.x, Dp.y);	}
	if ( nFlags&MK_RBUTTON )
	{
		m_ViewOffset.x += Dp.x;
		m_ViewOffset.y += Dp.y;
		CreateBKImage();
		RedrawWnd();
	}
	m_LastPos = m_MovingPos;
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CImageCombineWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
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
void CImageCombineWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ICW_IMAGE_WND, &pt) == false ) 
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}

	CWnd::SetCapture();
	m_RBtnUpPos = m_RBtnDownPos = m_LastPos = m_MovingPos = pt;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	::ReleaseCapture();
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	m_RBtnUpPos = m_MovingPos = pt;

	
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_IMAGE_COMBINE_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_IMAGE_COMBINE_WND;
	WndKey = _T("IDD_IMAGE_COMBINE_WND");
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
	WndID = ICW_SHOW_RECT_LINE_CHK;
	WndKey = _T("ICW_SHOW_RECT_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ICW_LOAD_IMAGE_BTN1;
	WndKey = _T("ICW_LOAD_IMAGE_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ICW_LOAD_IMAGE_BTN2;
	WndKey = _T("ICW_LOAD_IMAGE_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ICW_COMBINE_IMAGE_BTN;
	WndKey = _T("ICW_COMBINE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ICW_AUTO_MATCH_IMAGE_BTN;
	WndKey = _T("ICW_AUTO_MATCH_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CImageCombineWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_IMAGE_COMBINE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::BuildMapBuffer()
{
	const char fnName[] = "CImageCombineWnd::BuildMapBuffer";
	size_t  i=0;
	size_t  MapIndex=0;
	const size_t MaxCount = GetMaxCount();
	ClearShowBuffer();
	ClearMapBuffer();
	if ( NULL == m_MapBuffer_R[MapIndex] || NULL == m_MapBuffer_L[MapIndex] ) { return false; }

	CString     str;
	IMAGE_SIZE  MapW=0;
	IMAGE_SIZE  MapH=0;
	size_t      MaxSize=0;
	size_t      MapSize[FRAME_MAX_COUNT]={0};
	IMAGE_SIZE  MapStep[FRAME_MAX_COUNT]={0};
	IMAGE_SIZE  BitCount[FRAME_MAX_COUNT]={0};	
	IMAGE_PTR   MapPtr[FRAME_MAX_COUNT]={NULL};
	double     StageRgnW = 0;
	double     StageRgnH = 0;	
	TPOINT2D   ImageRes;
	TREGION4D  StageRgn;

	ImageRes = m_MapResolution_R;
	//機台整體範圍
	StageRgn.minX = MIN(m_MapStageRgn_R.minX, m_MapStageRgn_L.minX);
	StageRgn.minY = MIN(m_MapStageRgn_R.minY, m_MapStageRgn_L.minY);
	StageRgn.maxX = MIN(m_MapStageRgn_R.maxX, m_MapStageRgn_L.maxX);
	StageRgn.maxY = MIN(m_MapStageRgn_R.maxY, m_MapStageRgn_L.maxY);	
	StageRgnW = StageRgn.GetWidth();//機台整體寬度
	StageRgnH = StageRgn.GetHeight();//機台整體長度

	MapH = JetAPI::Ceil(StageRgnH/ImageRes.y);
	MapW = m_MapW_R[MapIndex]+m_MapW_L[MapIndex];	
	m_MapPos_R.x = MapW-((m_MapW_R[MapIndex]+1)/2);
	m_MapPos_R.y = m_MapH_R[MapIndex]/2;
	m_MapPos_L.x = (m_MapW_L[MapIndex]/2);
	m_MapPos_L.y = m_MapH_L[MapIndex]/2;
	for ( i=0; i<MaxCount; i++ )
	{
		MapPtr[i] = NULL;
		if ( NULL == m_MapBuffer_R[i] || NULL == m_MapBuffer_L[i] ) { continue; }
		if ( m_BitCount_R[i] != m_BitCount_L[i] ) { continue; }

		BitCount[i] = m_BitCount_R[i];
		MapStep[i] = JetAPI::GetBMPImagePixelsPerLine(MapW, BitCount[i], 4);
		MapSize[i] = ImageAPI.CalcBufferSize(MapStep[i], MapH);
		if ( MaxSize < MapSize[i] ) 
		{	MaxSize = MapSize[i];	}
		if ( JetMemory.alloc_func(MapSize[i],  MapPtr[i], fnName, "MapPtr") == false ) 
		{
			ClearMapBuffer();			
			return false;
		}
		::memset(MapPtr[i], 0x00, sizeof(IMAGE_DATA)*MapSize[i]);		

		m_MapW[i] = MapW;
		m_MapH[i] = MapH;	
		m_BitCount[i] = BitCount[i];	
		m_MapStep[i] = MapStep[i];
		m_MapBuffer[i] = MapPtr[i];		
		m_MapSize[i] = MapSize[i];	
	}

	str.Format(_T("%s\\CombineMap.PNG"), AOIDataCollect.GetAOITempDirectory());
	ImageAPI.SaveImage(str, m_MapW[0], m_MapH[0], m_MapStep[0], m_BitCount[0], m_MapBuffer[0], true);

	size_t      InfoSize=0;
	BITMAPINFO *InfoPtr= NULL;
	IMAGE_PTR  ShowPtr = NULL;
	if ( JetMemory.alloc_func(MaxSize,  ShowPtr, fnName, "ShowPtr") == false ) 
	{
		ClearMapBuffer();			
		return false;
	}		
	if ( ImageAPI.CreateBMPInfoBuffer(InfoPtr, InfoSize) == false ) 
	{
		JetMemory.free_func(ShowPtr);
		return false;
	}

	m_ShowSize = MaxSize;	
	m_ShowBuffer = ShowPtr;	
	m_ShowInfoPtr = InfoPtr;

	m_ZoomScale = 1.0;
	const double dRatio = 1.02;	
	m_ViewOffset.x = m_ViewOffset.y = 0;
	ImageAPI.CalcImageWndFitZoom(MapW, MapH, m_ImageWndRect, dRatio, m_ZoomScale);//計算影像視窗縮放參數	
	m_ZoomScale = 1.0;
	return true;
}
//-------------------------------------------------------------------------------------//
COMBINE_MAP_MODE CImageCombineWnd::GetCombineMapMode() const
{
	return m_CombineMapMode;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::SetCombineMapMode(COMBINE_MAP_MODE Mode)
{
	m_CombineMapMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::GetMapBuffer(IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt)
{
	size_t       i=0;
	const size_t MaxCount = MIN(GetMaxCount(), Cnt);

	for ( i=0; i<MaxCount; i++ )
	{
		if ( NULL==m_MapBuffer[i] ) 
		{	continue; }
		W[i] = m_MapW[i];
		H[i] = m_MapH[i];		
		Step[i] = m_MapStep[i];
		Bit[i] = m_BitCount[i];
		Ptr[i] = m_MapBuffer[i];
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::SetMapInfo_R(TREGION4D &Rgn, TPOINT2D &Res, IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt)
{	
	const char fnName[] = "CImageCombineWnd::SetMapInfo_R";
	size_t     i=0;	
	size_t     BufferSize=0;	

	ClearMapBuffer_R();
	for ( i=0; i<Cnt; i++ )
	{
		if ( NULL == Ptr[i] ) { continue; }
		BufferSize = Step[i]*H[i];
		if ( 0 == BufferSize ) { continue; }

		m_MapW_R[i] = W[i];
		m_MapH_R[i] = H[i];	
		m_MapStep_R[i] = Step[i];
		m_BitCount_R[i] = Bit[i];			
		m_MapBuffer_R[i] = Ptr[i];		
	}
	m_MapStageRgn_R = Rgn;
	m_MapResolution_R = Res;

	ExecSaveMapR();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::SetMapInfo_L(TREGION4D &Rgn, TPOINT2D &Res, IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt)
{
	const char fnName[] = "CImageCombineWnd::SetMapInfo_L";
	size_t     i=0;	
	size_t     BufferSize=0;

	ClearMapBuffer_L();
	for ( i=0; i<Cnt; i++ )
	{
		if ( NULL == Ptr[i] ) { continue; }
		BufferSize = Step[i]*H[i];
		if ( 0 == BufferSize ) { continue; }

		m_MapW_L[i] = W[i];
		m_MapH_L[i] = H[i];	
		m_MapStep_L[i] = Step[i];
		m_BitCount_L[i] = Bit[i];			
		m_MapBuffer_L[i] = Ptr[i];		
	}
	m_MapStageRgn_L = Rgn;
	m_MapResolution_L = Res;

	ExecSaveMapL();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CImageCombineWnd::GetMaxCount() const
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::ClearMapBuffer()
{
	size_t i=0;
	const size_t MaxCount = GetMaxCount();
	for ( i=0; i<MaxCount; i++ )
	{
		m_MapW[i] = 0;
		m_MapH[i] = 0;	
		m_MapStep[i] = 0;
		m_BitCount[i] = 8;					
		JetMemory.free_func(m_MapBuffer[i]);
		m_MapBuffer[i] = NULL;
		m_MapSize[i] = 0;			
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::GetMapResultRect(RECT &Rect)
{
	Rect = m_MapResultRect;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::GetMapResultRect_R(RECT &Rect)
{
	Rect = m_MapResultRect_R;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::GetMapResultRect_L(RECT &Rect)
{
	Rect = m_MapResultRect_L;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::ClearShowBuffer()
{
	if ( NULL != m_ShowInfoPtr )
	{	
		delete[] m_ShowInfoPtr;
		m_ShowInfoPtr=NULL; 
	}

	m_ShowW = 0;
	m_ShowH = 0;
	m_ShowStep = 0;
	m_ShowBitCnt = 8;
	JetMemory.free_func(m_ShowBuffer);
	m_ShowBuffer = NULL;	
	m_ShowSize = 0;
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::ClearMapBuffer_R()
{
	size_t i=0;
	const size_t MaxCount = GetMaxCount();
	for ( i=0; i<MaxCount; i++ )
	{
		m_MapW_R[i] = 0;
		m_MapH_R[i] = 0;	
		m_MapStep_R[i] = 0;
		m_BitCount_R[i] = 8;			
		if ( true == m_MapLoad_R )
		{	JetMemory.free_func(m_MapBuffer_R[i]); }
		m_MapBuffer_R[i] = NULL;		
		m_MapSize_R[i] = 0;			
	}	
	m_MapLoad_R = false;
	m_MapPos_R.x = m_MapPos_R.y = 0;
	m_MapStageRgn_R = TREGION4D();
	m_MapResolution_R = TPOINT2D();
	::memset(&m_MapRect_R, 0x00, sizeof(m_MapRect_R));
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::ClearMapBuffer_L()
{
	size_t i=0;
	const size_t MaxCount = GetMaxCount();
	for ( i=0; i<MaxCount; i++ )
	{
		m_MapW_L[i] = 0;
		m_MapH_L[i] = 0;	
		m_MapStep_L[i] = 0;
		m_BitCount_L[i] = 8;	
		if ( true == m_MapLoad_L )
		{	JetMemory.free_func(m_MapBuffer_L[i]); }
		m_MapBuffer_L[i] = NULL;		
		m_MapSize_L[i] = 0;		
	}
	m_MapLoad_L = false;
	m_MapPos_L.x = m_MapPos_L.y = 0;
	m_MapStageRgn_L = TREGION4D();
	m_MapResolution_L = TPOINT2D();
	::memset(&m_MapRect_L, 0x00, sizeof(m_MapRect_L));
	return;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecMergeImage()
{
	size_t i=0;
	size_t BufferSize=0;
	const size_t MaxCount = GetMaxCount();
	COMBINE_MAP_MODE CombineMapMode = GetCombineMapMode();
	for ( i=0; i<MaxCount; i++ )
	{
		if ( NULL==m_MapBuffer[i] || NULL == m_MapBuffer_R[i] || NULL == m_MapBuffer_L[i] ) { continue; }
		if ( m_BitCount_R[i]!=m_BitCount[i] || m_BitCount_L[i]!=m_BitCount[i] ) { continue; }
		
	#ifdef _DEBUG
		::memset(m_MapBuffer[i], 0xFF, sizeof(IMAGE_DATA)*m_MapSize[i]);
	#else
		::memset(m_MapBuffer[i], 0x00, sizeof(IMAGE_DATA)*m_MapSize[i]);	
	#endif//_DEBUG
				
		if ( COMBINE_MAP_BY_RIGHT == CombineMapMode )
		{
			//for map Left
			if ( PasteToMap(i, m_MapW_L[i], m_MapH_L[i], m_MapStep_L[i], m_BitCount[i], m_MapBuffer_L[i], m_MapPos_L, m_MapRect_L) == false ) 
			{	continue; }		

			//for map Right
			if ( PasteToMap(i, m_MapW_R[i], m_MapH_R[i], m_MapStep_R[i], m_BitCount[i], m_MapBuffer_R[i], m_MapPos_R, m_MapRect_R) == false ) 
			{	continue; }		
		}
		if ( COMBINE_MAP_BY_LEFT == CombineMapMode )
		{
			//for map Right
			if ( PasteToMap(i, m_MapW_R[i], m_MapH_R[i], m_MapStep_R[i], m_BitCount[i], m_MapBuffer_R[i], m_MapPos_R, m_MapRect_R) == false ) 
			{	continue; }		

			//for map Left
			if ( PasteToMap(i, m_MapW_L[i], m_MapH_L[i], m_MapStep_L[i], m_BitCount[i], m_MapBuffer_L[i], m_MapPos_L, m_MapRect_L) == false ) 
			{	continue; }		
		}
	}

	RECT    ResultRect={0,0,0,0};	
	ResultRect.left  = MIN(m_MapRect_R.left, m_MapRect_L.left);
	ResultRect.top   = MIN(m_MapRect_R.top, m_MapRect_L.top);
	ResultRect.right = MAX(m_MapRect_R.right, m_MapRect_L.right);
	ResultRect.bottom = MAX(m_MapRect_R.bottom, m_MapRect_L.bottom);
	m_MapResultRect = ResultRect;
	m_MapResultRect_R = m_MapRect_R;
	m_MapResultRect_L = m_MapRect_L;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::PasteToMap(size_t index, IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, const POINT &Pos, RECT &Rect)
{
	RECT    MapRect={0,0,0,0};
	RECT    MapRectFull={0,0,0,0};
	const IMAGE_SIZE MapWFull = m_MapW[index];
	const IMAGE_SIZE MapHFull = m_MapH[index];
	const IMAGE_SIZE MapStepFull = m_MapStep[index];
	JetAPI::SizeToRect(W, H, MapRect);
	MapRectFull.left = Pos.x-(W/2);
	MapRectFull.top  = Pos.y-(H/2);
	MapRectFull.right  = MapRectFull.left+W;
	MapRectFull.bottom = MapRectFull.top+H;
	Rect = MapRectFull;	
	
	if ( MapRectFull.left < 0 )
	{ 
		MapRect.left -= MapRectFull.left;
		MapRectFull.left = 0; 
	}
	if ( MapRectFull.top < 0 )
	{ 
		MapRect.top -= MapRectFull.top;
		MapRectFull.top = 0; 
	}
	if ( MapRectFull.right > MapWFull )
	{ 
		MapRect.right -= (MapRectFull.right-MapWFull);
		MapRectFull.right = MapWFull; 
	}
	if ( MapRectFull.bottom > MapHFull )
	{ 
		MapRect.bottom -= (MapRectFull.bottom-MapHFull);
		MapRectFull.bottom = MapHFull; 
	}			
	Rect = MapRectFull;
	const int RectW=MapRect.right-MapRect.left;
	const int RectH=MapRect.bottom-MapRect.top;
	const int RectWFull=MapRectFull.right-MapRectFull.left;
	const int RectHFull=MapRectFull.bottom-MapRectFull.top;
	if ( RectW!=RectWFull || RectH!=RectHFull )
	{	return false; }
	
	int  i=0;
	int  SrcIdx=0;
	int  DstIdx=0;
	int  CopyLen = RectW;	
	if ( 8 == BitCount ) 
	{	
		CopyLen = CopyLen; 
		for ( i=0; i<RectH; i++ )
		{
			SrcIdx = (i+MapRect.top)*Step+(MapRect.left);
			DstIdx = (i+MapRectFull.top)*MapStepFull+(MapRectFull.left);
			::memcpy(&(m_MapBuffer[index][DstIdx]), &(Ptr[SrcIdx]), sizeof(IMAGE_DATA)*CopyLen);
		}
	}
	if ( 24 == BitCount ) 
	{	
		CopyLen = CopyLen*3; 
		for ( i=0; i<RectH; i++ )
		{
			SrcIdx = (i+MapRect.top)*Step+(MapRect.left*3);
			DstIdx = (i+MapRectFull.top)*MapStepFull+(MapRectFull.left*3);
			::memcpy(&(m_MapBuffer[index][DstIdx]), &(Ptr[SrcIdx]), sizeof(IMAGE_DATA)*CopyLen);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::RedrawWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }

	RECT  Rect={0,0,0,0};
	POINT OffsetPt={0,0};	
	IMAGE_SIZE ImageW = m_ShowW;
	IMAGE_SIZE ImageH = m_ShowH;
	RECT  WndRect=m_ImageWndRect;	
	double ZoomScale = m_ZoomScale;
	BOOL bShowRectLine = CWnd::IsDlgButtonChecked(ICW_SHOW_RECT_LINE_CHK);

	::IntersectClipRect(hDC, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	

	
	COMBINE_MAP_MODE CombineMapMode = m_CombineMapMode;
	
	HPEN  hPen1 = ::CreatePen(PS_DASH, 1, 0xFFFF00);
	HPEN  hPen2 = ::CreatePen(PS_DASH, 1, 0x0000FF);
	HPEN  hPenF = ::CreatePen(PS_SOLID, 2, 0x8F8F8F);
	HPEN  hOldPen = (HPEN)(::SelectObject(hDC, hPen1));	

	OffsetPt.x = (int)(m_ViewOffset.x);
	OffsetPt.y = (int)(m_ViewOffset.y);
	if ( TRUE == bShowRectLine )
	{		
		if ( COMBINE_MAP_BY_RIGHT == CombineMapMode )
		{
			::SelectObject(hDC, hPen2);	
			ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_L, Rect);			
			ImageAPI.DrawRectLine(hDC, Rect);

			::SelectObject(hDC, hPen1);	
			ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_R, Rect);				
			ImageAPI.DrawRectLine(hDC, Rect);
		}
		if ( COMBINE_MAP_BY_LEFT == CombineMapMode )
		{
			::SelectObject(hDC, hPen1);	
			ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_R, Rect);				
			ImageAPI.DrawRectLine(hDC, Rect);

			::SelectObject(hDC, hPen2);	
			ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapRect_L, Rect);			
			ImageAPI.DrawRectLine(hDC, Rect);
		}

		::SelectObject(hDC, hPenF);	
		ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, m_MapResultRect, Rect);			
		ImageAPI.DrawRectLine(hDC, Rect);
		
	}

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen1);
	::DeleteObject(hPen2);
	::DeleteObject(hPenF);	
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::CreateBKImage()
{
	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }	
	RECT         Rect = m_ImageWndRect;
	IMAGE_SIZE   ImageW = m_ShowW;
	IMAGE_SIZE   ImageH = m_ShowH;
	IMAGE_SIZE   ImageStep = m_ShowStep;
	IMAGE_SIZE   ImageBitCount = m_ShowBitCnt;	
	IMAGE_PTR    ImagePtr = m_ShowBuffer;
	if ( NULL == ImagePtr ) { return ; }
	BITMAPINFO *pInfo = m_ShowInfoPtr;
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
	if ( ImageAPI.SetBMPInfo(pInfo, ImageW, ImageH, ImageBitCount) == false ) { return ; }
	ImageAPI.DrawImageToDC(hDC, pInfo, ImagePtr, Rect, OffsetPt2D, m_ZoomScale, BkColor);
	
	//m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr, true, false);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::BuildShowImage(size_t index)
{
	const size_t MaxCount = GetMaxCount();
	if ( index >= MaxCount ) { return ; }
	if ( NULL == m_ShowBuffer ) { return; }

	IMAGE_PTR  MapPtr = m_MapBuffer[index];
	IMAGE_SIZE MapW = m_MapW[index];
	IMAGE_SIZE MapH = m_MapH[index];
	IMAGE_SIZE MapStep = m_MapStep[index];
	IMAGE_SIZE BitCount= m_BitCount[index];	
	if ( NULL == MapPtr ) { return; }

	m_ShowW = MapW;
	m_ShowH = MapH;
	m_ShowStep = MapStep;
	m_ShowBitCnt = BitCount;
	AOIDataCollect.ExecEnhanceDisplayImage(MapW, MapH, MapStep, BitCount, MapPtr, m_ShowBuffer);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecSaveMapR()
{
	CString filename;
	CString filename2;
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CombineMapR.PNG"));
	filename2.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CombineMapR.TXT"));

	IMAGE_SIZE ImageW=m_MapW_R[0];
	IMAGE_SIZE ImageH=m_MapH_R[0];
	IMAGE_SIZE ImageStep=m_MapStep_R[0];
	IMAGE_SIZE BitCount=m_BitCount_R[0];
	IMAGE_PTR  ImagePtr=m_MapBuffer_R[0];

	if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false ) 
	{	return false; }

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename2, _T("w+"));
	if ( NULL != pfile ) 
	{
		::fprintf(pfile, ("%.6f, %.6f\n"), m_MapResolution_R.x, m_MapResolution_R.y);
		::fprintf(pfile, ("%.6f, %.6f, %.6f, %.6f\n"), m_MapStageRgn_R.minX, m_MapStageRgn_R.minY, m_MapStageRgn_R.maxX, m_MapStageRgn_R.maxY);
		::fclose(pfile); pfile=NULL;
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecSaveMapL()
{
	CString filename;
	CString filename2;
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CombineMapL.PNG"));
	filename2.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CombineMapL.TXT"));

	IMAGE_SIZE ImageW=m_MapW_L[0];
	IMAGE_SIZE ImageH=m_MapH_L[0];
	IMAGE_SIZE ImageStep=m_MapStep_L[0];
	IMAGE_SIZE BitCount=m_BitCount_L[0];
	IMAGE_PTR  ImagePtr=m_MapBuffer_L[0];

	if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false ) 
	{	return false; }

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename2, _T("w+"));
	if ( NULL != pfile ) 
	{
		::fprintf(pfile, ("%.6f, %.6f\n"), m_MapResolution_L.x, m_MapResolution_L.y);
		::fprintf(pfile, ("%.6f, %.6f, %.6f, %.6f\n"), m_MapStageRgn_L.minX, m_MapStageRgn_L.minY, m_MapStageRgn_L.maxX, m_MapStageRgn_L.maxY);
		::fclose(pfile); pfile=NULL;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecLoadMapR()
{
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return false; }

	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	
	CString    filename2;
	CString    filenameM;
	CString    filename=dialog.GetPathName();

	JetAPI::ExtractMainFileName(filename, filenameM);
	filename2.Format(_T("%s.TXT"), filenameM);

	ClearMapBuffer_R();
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false; }

	m_MapLoad_R = true;
	m_MapW_R[0] = ImageW;
	m_MapH_R[0] = ImageH;
	m_BitCount_R[0] = BitCount;	
	m_MapStep_R[0] = ImageStep;
	m_MapBuffer_R[0] = ImagePtr;	
	m_MapSize_R[0] = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename2, _T("r+"));
	if ( NULL != pfile ) 
	{
		::fscanf(pfile, ("%lf, %lf\n"), &m_MapResolution_R.x, &m_MapResolution_R.y);
		::fscanf(pfile, ("%lf, %lf, %lf, %lf\n"), &m_MapStageRgn_R.minX, &m_MapStageRgn_R.minY, &m_MapStageRgn_R.maxX, &m_MapStageRgn_R.maxY);
		::fclose(pfile); pfile=NULL;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecLoadMapL()
{
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	
	CString    filename2;
	CString    filenameM;
	CString    filename=dialog.GetPathName();

	JetAPI::ExtractMainFileName(filename, filenameM);
	filename2.Format(_T("%s.TXT"), filenameM);

	ClearMapBuffer_L();
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false; }

	m_MapLoad_L = true;
	m_MapW_L[0] = ImageW;
	m_MapH_L[0] = ImageH;
	m_BitCount_L[0] = BitCount;	
	m_MapStep_L[0] = ImageStep;
	m_MapBuffer_L[0] = ImagePtr;	
	m_MapSize_L[0] = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename2, _T("r+"));
	if ( NULL != pfile ) 
	{
		::fscanf(pfile, ("%lf, %lf\n"), &m_MapResolution_L.x, &m_MapResolution_L.y);
		::fscanf(pfile, ("%lf, %lf, %lf, %lf\n"), &m_MapStageRgn_L.minX, &m_MapStageRgn_L.minY, &m_MapStageRgn_L.maxX, &m_MapStageRgn_L.maxY);
		::fclose(pfile); pfile=NULL;		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecMoveMap(int dx, int dy)
{
	COMBINE_MAP_MODE CombineMapMode = GetCombineMapMode();
	if ( COMBINE_MAP_BY_RIGHT == CombineMapMode )
	{	return ExecMoveMapL(dx, dy); }
	if ( COMBINE_MAP_BY_LEFT  == CombineMapMode )
	{	return ExecMoveMapR(dx, dy); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecMoveMapR(int dx, int dy)
{
	m_MapPos_R.x += dx;
	m_MapPos_R.y += dy;
	ExecMergeImage();	
	BuildShowImage(m_MapIndex);
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecMoveMapL(int dx, int dy)
{
	m_MapPos_L.x += dx;
	m_MapPos_L.y += dy;
	ExecMergeImage();	
	BuildShowImage(m_MapIndex);
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnCombineImageBtn() 
{
	// TODO: Add your control notification handler code here	
	BuildMapBuffer();	
	ExecMergeImage();	
	BuildShowImage(m_MapIndex);
	CreateBKImage();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnLoadImageBtn1() 
{
	// TODO: Add your control notification handler code here
	ExecLoadMapR();
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnLoadImageBtn2() 
{
	// TODO: Add your control notification handler code here
	ExecLoadMapL();
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnAutoMatchImageBtn() 
{
	// TODO: Add your control notification handler code here
	ExecAutoMatch();
}
//-------------------------------------------------------------------------------------//
bool CImageCombineWnd::ExecAutoMatch()
{
	const unsigned int MapIndex = m_MapIndex;
	const size_t MaxCount = GetMaxCount();
	if ( MapIndex >= MaxCount ) { return false; }

	IMAGE_SIZE MapWFull=m_MapW[MapIndex];
	IMAGE_SIZE MapHFull=m_MapH[MapIndex];

	int        CalcCnt=0;
	IMAGE_SIZE MapWR=m_MapW_R[MapIndex];
	IMAGE_SIZE MapHR=m_MapH_R[MapIndex];	
	IMAGE_SIZE BitCntR=m_BitCount_R[MapIndex];
	IMAGE_SIZE MapStepR=m_MapStep_R[MapIndex];
	IMAGE_PTR  MapPtrR=m_MapBuffer_R[MapIndex];
	RECT       MapRectR=m_MapRect_R;

	IMAGE_SIZE MapWL=m_MapW_L[MapIndex];
	IMAGE_SIZE MapHL=m_MapH_L[MapIndex];
	IMAGE_SIZE BitCntL=m_BitCount_L[MapIndex];
	IMAGE_SIZE MapStepL=m_MapStep_L[MapIndex];
	IMAGE_PTR  MapPtrL=m_MapBuffer_L[MapIndex];
	RECT       MapRectL=m_MapRect_L;	
	
	if ( BitCntR!=BitCntL ) { return false; }
	if ( NULL==MapPtrR || NULL==MapPtrL ) { return false; }

	size_t    i=0;
	COMBINE_MAP_MODE CombineMapMode = GetCombineMapMode();

	IMAGE_SIZE PatW=0;
	IMAGE_SIZE PatH=0;
	IMAGE_SIZE PatStep=0;
	IMAGE_SIZE BitCnt=BitCntR;
	IMAGE_PTR  PatPtr=NULL;
	RECT       PatRect={0,0,0,0};

	IMAGE_SIZE RoiX=0;
	IMAGE_SIZE RoiY=0;
	IMAGE_SIZE RoiW=0;
	IMAGE_SIZE RoiH=0;
	IMAGE_SIZE RoiStep=0;	
	IMAGE_PTR  RoiPtr=NULL;
	RECT       RoiRect={0,0,0,0};

	CJetMatch  Match;
	int        NResults=0;
	int        RoiPitch=0;
	const bool bRobustness = true;
	const int  nMinReduceArea = 512;
	const int  nFinalReduction = 0;
	const bool UseInterpolate = true;
	const double ScoreLSL = 90;
	double ResultX=0, ResultY=0, ResultA=0, ResultS=0, ResultSX=0, ResultSY=0;	
	double BestResX=0, BestResY=0, BestResA=0, BestResS=0, BestResSX=0, BestResSY=0;		
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	return false;		}	

	m_MapPos_R.x = MapWFull-((MapWR+1)/2);
	m_MapPos_R.y = MapHR/2;
	m_MapPos_L.x = MapWL/2;
	m_MapPos_L.y = MapHL/2;

	if ( COMBINE_MAP_BY_RIGHT == CombineMapMode ) //以右邊為主
	{	
		PatW = MapWR/10;
		PatH = MapHR/2;
		PatStep = JetAPI::GetBMPImagePixelsPerLine(PatW, BitCnt, 4);
		PatRect.left = 0; 
		PatRect.right = PatRect.left+PatW;
		PatRect.top = (MapHR/2)-(PatH/2);
		PatRect.bottom = PatRect.top+PatH;

		if ( ImageAPI.ExtractRoiImage(MapWR, MapHR, MapStepR, BitCnt, MapPtrR, PatRect, PatStep, PatPtr, false) == false ) 
		{	return false;	}

		//initial eMatch
		Match.SetMatchDefaultParam();
		Match.SetMaxInitialPositions(4);
		Match.SetRobustness(bRobustness);		
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);		
		if ( Match.LearnPattern(PatW, PatH, PatStep, BitCnt, PatPtr, true) == false )
		{
			JetMemory.free_func(PatPtr);
			return false;
		}

		CalcCnt=0;
		RoiPitch = PatW/4;
		//RoiPitch = MapW2/4;
		RoiRect.right = MapWL+RoiPitch;
		while ( true ) 
		{
			RoiW=PatW*2;
			RoiH=MapHL;
			RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCnt, 4);	
			RoiRect.right = RoiRect.right-RoiPitch;
			RoiRect.left = RoiRect.right-RoiW; 			
			RoiRect.top = 0;
			RoiRect.bottom = RoiRect.top+RoiH;
			if ( RoiRect.left < (MapWL/3) ) { break; }
			if ( ImageAPI.ExtractRoiImage(MapWL, MapHL, MapStepL, BitCnt, MapPtrL, RoiRect, RoiStep, RoiPtr, false) == false ) 
			{
				JetMemory.free_func(PatPtr);
				JetMemory.free_func(RoiPtr);
				return false;
			}

			Match.SetInterpolate(UseInterpolate);			
			Match.SetMinScore(-1);//fMinScore
			if ( Match.Match(RoiW, RoiH, RoiStep, BitCnt, RoiPtr, true) == false )
			{
				JetMemory.free_func(PatPtr);
				JetMemory.free_func(RoiPtr);
				return false;
			}

			ResultS = 0;
			NResults = Match.GetNumPositions();
			if ( NResults > 0 )
			{				
				int idx =0;
				ResultS = Match.GetResultScore(idx)*100.0;
				ResultX = Match.GetResultPosX(idx);
				ResultY = Match.GetResultPosY(idx);
				ResultA = Match.GetResultAngle(idx);		
				ResultSX = Match.GetResultScaleX(idx);
				ResultSY = Match.GetResultScaleY(idx);		
				if ( ResultS < 0.0 ) { ResultS = 0.0; }
			}			
			if ( BestResS < ResultS )
			{
				BestResS = ResultS;
				BestResX = ResultX+RoiRect.left;
				BestResY = ResultY+RoiRect.top;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;				
			}			
			CalcCnt ++;
			JetMemory.free_func(RoiPtr);
			//if ( ResultS > ScoreLSL )
			//{	break;	}
		};
		JetMemory.free_func(PatPtr);

		//此點等於右側圖的左邊線位置
		RoiX=BestResX-(PatW/2);
		RoiY=BestResY-(PatH/2);
		
		m_MapPos_L.x = (MapWL/2)+(MapWFull-MapWR-RoiX);
		m_MapPos_L.y = (MapHL/2)+(PatRect.top-RoiY);
	}

	if ( COMBINE_MAP_BY_LEFT == CombineMapMode ) //以左邊為主
	{
		PatW = MapWL/10;
		PatH = MapHL/2;
		PatStep = JetAPI::GetBMPImagePixelsPerLine(PatW, BitCnt, 4);
		PatRect.right = MapWL;
		PatRect.left = PatRect.right-PatW; 		
		PatRect.top = (MapHL/2)-(PatH/2);
		PatRect.bottom = PatRect.top+PatH;

		if ( ImageAPI.ExtractRoiImage(MapWL, MapHL, MapStepL, BitCnt, MapPtrL, PatRect, PatStep, PatPtr, false) == false ) 
		{	return false;	}

		//initial eMatch
		Match.SetMatchDefaultParam();
		Match.SetRobustness(bRobustness);
		Match.SetMinReducedArea(nMinReduceArea);
		Match.SetFinalReduction(nFinalReduction);
		if ( Match.LearnPattern(PatW, PatH, PatStep, BitCnt, PatPtr, true) == false )
		{
			JetMemory.free_func(PatPtr);
			return false;
		}

		CalcCnt = 0;
		RoiPitch = PatW/4;
		//RoiPitch = MapW2/4;
		RoiRect.left = -RoiPitch;
		while ( true ) 
		{
			RoiW=PatW*2;
			RoiH=MapHR;
			RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCnt, 4);	
			RoiRect.left = RoiRect.left+RoiPitch; 			
			RoiRect.right = RoiRect.left+RoiW;			
			RoiRect.top = 0;
			RoiRect.bottom = RoiRect.top+RoiH;
			if ( RoiRect.right > (MapWR*2/3) ) { break; }
			if ( ImageAPI.ExtractRoiImage(MapWR, MapHR, MapStepR, BitCnt, MapPtrR, RoiRect, RoiStep, RoiPtr, false) == false ) 
			{
				JetMemory.free_func(PatPtr);
				JetMemory.free_func(RoiPtr);
				return false;
			}

			Match.SetInterpolate(UseInterpolate);			
			Match.SetMinScore(-1);//fMinScore
			if ( Match.Match(RoiW, RoiH, RoiStep, BitCnt, RoiPtr, true) == false )
			{
				JetMemory.free_func(PatPtr);
				JetMemory.free_func(RoiPtr);
				return false;
			}

			ResultS = 0;
			NResults = Match.GetNumPositions();
			if ( NResults > 0 )
			{				
				int idx =0;
				ResultS = Match.GetResultScore(idx)*100.0;
				ResultX = Match.GetResultPosX(idx);
				ResultY = Match.GetResultPosY(idx);
				ResultA = Match.GetResultAngle(idx);		
				ResultSX = Match.GetResultScaleX(idx);
				ResultSY = Match.GetResultScaleY(idx);		
				if ( ResultS < 0.0 ) { ResultS = 0.0; }
			}			
			if ( BestResS < ResultS )
			{
				BestResS = ResultS;
				BestResX = ResultX+RoiRect.left;
				BestResY = ResultY+RoiRect.top;
				BestResA = ResultA;
				BestResSX = ResultSX;
				BestResSY = ResultSY;				
			}
			CalcCnt ++;
			JetMemory.free_func(RoiPtr);
			//if ( ResultS > ScoreLSL )
			//{	break;	}
		};
		JetMemory.free_func(PatPtr);

		//此點等於右側圖的左邊線位置
		RoiX=BestResX+(PatW/2);
		RoiY=BestResY-(PatH/2);

		m_MapPos_R.x = MapWFull-((MapWR+1)/2)-RoiX;		
		m_MapPos_R.y = (MapHR/2)+(PatRect.top-RoiY);
	}
	
	ExecMergeImage();	
	BuildShowImage(m_MapIndex);
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CImageCombineWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_LEFT:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveMap(-10, 0); }
			else
			{	ExecMoveMap(-1, 0); }
			break;
		case VK_UP:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveMap(0, -10); }
			else
			{	ExecMoveMap(0, -1); }
			break;
		case VK_RIGHT:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveMap(10, 0); }
			else
			{	ExecMoveMap(1, 0); }
			break;
		case VK_DOWN:
			if ( JetAPI::CheckIsPressVRKey(VK_SHIFT) == true ) 
			{	ExecMoveMap(0, 10); }
			else
			{	ExecMoveMap(0, 1); }
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CImageCombineWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_IMAGE_WND_DRAW_NEXT:
		//RedrawWnd();
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		//OnImageWndNotify(wParam, lParam);
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnImageWndMouseMove()
{
	POINT DiffPos={0,0};

	::GetCursorPos(&m_MovingPos);
	CWnd::ScreenToClient(&m_MovingPos);

	DiffPos.x = m_MovingPos.x-m_LastPos.x;
	DiffPos.y = m_MovingPos.y-m_LastPos.y;		

	m_LastPos = m_MovingPos;		
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnImageWndLButtonUp()
{
	::GetCursorPos(&m_MovingPos);
	CWnd::ScreenToClient(&m_MovingPos);
	m_LBtnUpPos = m_LastPos = m_MovingPos;		
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnImageWndLButtonDown()
{
	::GetCursorPos(&m_MovingPos);
	CWnd::ScreenToClient(&m_MovingPos);
	m_LBtnUpPos = m_LBtnDownPos = m_LastPos = m_MovingPos;		
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam )
	{
	case WPARAM_LBUTTON_DOWN:
		OnImageWndLButtonDown();		
		break;
	case WPARAM_LBUTTON_UP:
		OnImageWndLButtonUp();		
		break;
	case WPARAM_LBUTTON_DBCLICK:
		break;
	case WPARAM_RBUTTON_DOWN:
		break;
	case WPARAM_RBUTTON_UP:
		break;
	case WPARAM_RBUTTON_DBCLICK:
		break;
	case WPARAM_MOUSE_MOVE:
		OnImageWndMouseMove();
		break;
	case WPARAM_MOUSE_WHEEL:
		break;
	case WPARAM_CONTEXT_MENU:
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnShowRectLineChk() 
{
	// TODO: Add your control notification handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageCombineWnd::OnOK() 
{
	// TODO: Add extra validation here	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//