// Draw3DWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "Draw3DWnd.h"
//-------------------------------------------------------------------------------------//
#include "dib.h"
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDraw3DWnd dialog
//-------------------------------------------------------------------------------------//
CDraw3DWnd::CDraw3DWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDraw3DWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDraw3DWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AnimationPlay = false;
	m_ShowProfileWnd = false;
	m_ColorRangeMax = -1;
	m_ColorRangeMin = -1;
	m_ColorRangeDefined = false;
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDraw3DWnd)
	DDX_Control(pDX, DRAW3D_PROFILE_WND, m_ProfileWnd);
	DDX_Control(pDX, DRAW3D_SLIDER_WND_Y2, m_SliderWndY2);
	DDX_Control(pDX, DRAW3D_SLIDER_WND_Y1, m_SliderWndY1);
	DDX_Control(pDX, DRAW3D_SLIDER_WND_X2, m_SliderWndX2);
	DDX_Control(pDX, DRAW3D_SLIDER_WND_X1, m_SliderWndX1);
	DDX_Control(pDX, DRAW3D_3DIMAGE_WND, m_OpenGLWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDraw3DWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CDraw3DWnd)
	ON_WM_SIZE()
	ON_COMMAND(DRAW3D_LOAD_IMAGE_MENU, OnLoadImageMenu)
	ON_COMMAND(DRAW3D_LOAD_SPACE_MENU, OnLoadSpaceMenu)	
	ON_COMMAND(DRAW3D_SAVE_FLOAT_MENU, OnSaveFloatMenu)
	ON_COMMAND(DRAW3D_OBJECT_COLOR_MENU, OnObjectColorMenu)	
	ON_COMMAND(DRAW3D_OBJECT_LINE_MENU, OnObjectLineMenu)	
	ON_COMMAND(DRAW3D_OBJECT_TEXTURE_MENU, OnObjectTextureMenu)	
	ON_COMMAND(DRAW3D_OBJECT_COLOR_LINE_MENU, OnObjectColorLineMenu)	
	ON_COMMAND(DRAW3D_UPPER_PLANE_MENU, OnUpperPlaneMenu)	
	ON_COMMAND(DRAW3D_LOWER_PLANE_MENU, OnLowerPlaneMenu)		
	ON_COMMAND(DRAW3D_BOUND_BOX_MENU, OnBoundBoxMenu)
	ON_COMMAND(DRAW3D_SHOW_PROFILE_WND_MENU, OnShowProfileWndMenu)	
	ON_COMMAND(DRAW3D_SHOW_AXIS_MENU, OnShowAxisMenu)		
	ON_COMMAND(DRAW3D_DEFAULT_MENU, OnDefaultMenu)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(DRAW3D_BASE_PLANE_MENU, OnBasePlaneMenu)
	ON_COMMAND(DRAW3D_PLANE_PITCH_MENU, OnPlanePitchMenu)
	ON_COMMAND(DRAW3D_PLANE_HEIGHT_MENU, OnPlaneHeightMenu)	
	ON_COMMAND(DRAW3D_COLOR_RANGE_MENU, OnColorRangeMenu)
	ON_COMMAND(DRAW3D_ANIMATION_PLAY_MENU, OnAnimationPlayMenu)
	ON_COMMAND(DRAW3D_ANIMATION_STOP_MENU, OnAnimationStopMenu)		
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_HIGH_MOST, OnDetailLevelMenuHighMost)	
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_HIGH_MORE, OnDetailLevelMenuHighMore)	
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_HIGH, OnDetailLevelMenuHigh)	
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_MIDDLE, OnDetailLevelMenuMiddle)	
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_LOW, OnDetailLevelMenuLow)
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_LOW_MORE, OnDetailLevelMenuLowMore)
	ON_COMMAND(DRAW3D_DETAIL_LEVEL_MENU_LOW_MOST, OnDetailLevelMenuLowMost)
	ON_COMMAND(DRAW3D_CLIP_PLANE_MENU, OnClipPlaneMenu)	
	ON_COMMAND(DRAW3D_CLIP_PLANE_HOR_MENU, OnClipPlaneHorMenu)
	ON_COMMAND(DRAW3D_CLIP_PLANE_VER_MENU, OnClipPlaneVerMenu)
	ON_COMMAND(DRAW3D_CLIP_PLANE_ANY_MENU, OnClipPlaneAnyMenu)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDraw3DWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CDraw3DWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_OpenGLWnd.InitialDisplay();
	m_OpenGLWnd.CreateMaxDataBuffer();
	m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_TEXTURE);

	InitProfileWnd(m_ProfileWnd);
	this->AdjustControlWnd();
	this->SwitchMultiLanguage();
	this->UpdateCommandUI();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustControlWnd();	
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	//return;
	this->ShowWindow(SW_HIDE);
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_DRAW3D_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_DRAW3D_WND;
	WndKey = _T("IDD_DRAW3D_WND");
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
	CMenu *pMenu = this->GetMenu();	
	if ( pMenu != NULL )
	{	AOIDataCollect.SwitchMultiLanguageMenu(*pMenu, IDR_DRAW3D_MENU); }
}
//-------------------------------------------------------------------------------------//
CString CDraw3DWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_DRAW3D_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::AdjustControlWnd()//修正控制項位置
{
	if ( this->m_OpenGLWnd.GetSafeHwnd() == NULL )
	{	return; }
	
	//1. 扣除Profile式窗與四邊的Slider才是3D圖像
	SIZE size={0};
	RECT WndRect={0};
	RECT MainWndRect={0};
	RECT ProfileRect={0};
	RECT SldRectX1={0};
	RECT SldRectY1={0};
	RECT SldRectX2={0};
	RECT SldRectY2={0};
	int  WndGap = 4;
	int  SldMargin = 36;	
	this->GetClientRect(&MainWndRect);	
	bool  ShowProfileWnd = m_ShowProfileWnd;
	const int cx = MainWndRect.right-MainWndRect.left;
	const int cy = MainWndRect.bottom-MainWndRect.top;	
	int OpenGLWndW = cx;
	int OpenGLWndH = cy-WndGap-WndGap;
	if ( true == ShowProfileWnd )
	{
		OpenGLWndH = OpenGLWndH/2;
		if ( OpenGLWndH < 120 ) 
		{	ShowProfileWnd = false; }
	}		
	const int OpenGLCy=MIN(OpenGLWndW, OpenGLWndH);	

	this->m_ProfileWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	size.cx = WndRect.right-WndRect.left;
	size.cy = WndRect.bottom-WndRect.top;
	WndRect.bottom = MainWndRect.bottom-WndGap;
	WndRect.left = MainWndRect.left+WndGap;
	WndRect.right = MainWndRect.right-WndGap;
	WndRect.top = WndRect.bottom-size.cy;
	this->m_ProfileWnd.MoveWindow(&WndRect);
	ProfileRect = WndRect;

	this->m_SliderWndX1.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	size.cx = WndRect.right-WndRect.left;
	size.cy = WndRect.bottom-WndRect.top;
	WndRect.bottom = ProfileRect.top-WndGap;
	WndRect.top = WndRect.bottom-size.cy;
	WndRect.left = MainWndRect.left+SldMargin;
	WndRect.right = MainWndRect.right-SldMargin;
	this->m_SliderWndX1.MoveWindow(&WndRect);
	SldRectX1 = WndRect;

	this->m_SliderWndX2.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	size.cx = WndRect.right-WndRect.left;
	size.cy = WndRect.bottom-WndRect.top;
	WndRect.top = MainWndRect.top+WndGap;
	WndRect.bottom = WndRect.top+size.cy;
	WndRect.left = MainWndRect.left+SldMargin;
	WndRect.right = MainWndRect.right-SldMargin;
	this->m_SliderWndX2.MoveWindow(&WndRect);
	SldRectX2 = WndRect;

	this->m_SliderWndY1.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	size.cx = WndRect.right-WndRect.left;
	size.cy = WndRect.bottom-WndRect.top;
	WndRect.bottom = ProfileRect.top-SldMargin;
	WndRect.top = MainWndRect.top+SldMargin;
	WndRect.left = MainWndRect.left+WndGap;
	WndRect.right = WndRect.left+size.cx;
	this->m_SliderWndY1.MoveWindow(&WndRect);
	SldRectY1 = WndRect;

	this->m_SliderWndY2.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	size.cx = WndRect.right-WndRect.left;
	size.cy = WndRect.bottom-WndRect.top;
	WndRect.bottom = ProfileRect.top-SldMargin;
	WndRect.top = MainWndRect.top+SldMargin;
	WndRect.right = MainWndRect.right-WndGap;
	WndRect.left = WndRect.right-size.cx;	
	this->m_SliderWndY2.MoveWindow(&WndRect);
	SldRectY2 = WndRect;
	
	this->m_OpenGLWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);
	WndRect.left = SldRectY1.right+WndGap;
	WndRect.right = SldRectY2.left-WndGap;
	WndRect.top = SldRectX2.bottom+WndGap;
	WndRect.bottom = SldRectX1.top-WndGap;

	WndRect.top = MainWndRect.top+WndGap;
	WndRect.left = MainWndRect.left+WndGap;
	WndRect.right = MainWndRect.right-WndGap;	
	WndRect.bottom = MainWndRect.top+OpenGLCy;	
	m_OpenGLWnd.MoveWindow(&WndRect);	

	if ( true == ShowProfileWnd )
	{
		const int OpenGLWndHEnd = WndRect.bottom;		
		WndRect.top = OpenGLWndHEnd+4;
		WndRect.bottom = MainWndRect.bottom-WndGap;
		m_ProfileWnd.MoveWindow(&WndRect);	
		if ( m_ProfileWnd.IsWindowVisible() == FALSE )
		{	m_ProfileWnd.ShowWindow(SW_SHOW); }
	}
	else
	{	m_ProfileWnd.ShowWindow(SW_HIDE);	}
	
	return;
}
//-------------------------------------------------------------------------------------//
BOOL CDraw3DWnd::PreTranslateMessage(MSG* pMsg)
{		
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CDraw3DWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{	
	case MSG_OPEN_GL_WND:
		switch(wParam)
		{
		case WPARAM_UPDATE_CLIP_PLANE:		
			BuildProfileWnd(m_ProfileWnd);			
			break;
		}
	}	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::ExecLoadImageFile()
{
	const char fnName[] = "CDraw3DWnd::ExecLoadImageFile";
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	CDib dib;
	CString filename = dialog.GetPathName();
	if ( dib.Load(filename) == false )
	{	return false; }	

	bool IsColor = false;
	int  nChannel = 1;
	BITMAPINFO *pInfo = dib.GetDIBInfo();
	unsigned char *pDiBits = dib.GetDIBBits();
	const int ImageW = pInfo->bmiHeader.biWidth;
	const int ImageH = pInfo->bmiHeader.biHeight;
	const int BitCount = pInfo->bmiHeader.biBitCount;	
	float *pBuffer3D = NULL;
	unsigned char *pBuffer2D = NULL;

	if ( BitCount == 24 )
	{	
		IsColor = true;
		nChannel = 3;
	}
	else if ( BitCount == 8 )
	{	
		IsColor = false;	
		nChannel = 1;
	}
	else
	{	return false; }

	const size_t BufferSize3D=ImageAPI.CalcBufferSize(ImageW, ImageH);
	const size_t BufferSize2D=ImageAPI.CalcBufferSize(ImageW*nChannel, ImageH);
	JetMemory.alloc_func(BufferSize3D, pBuffer3D, fnName, "pBuffer3D");
	JetMemory.alloc_func(BufferSize2D, pBuffer2D, fnName, "pBuffer2D");
	if ( NULL==pBuffer2D || NULL==pBuffer3D ) 
	{ 
		JetMemory.free_func(pBuffer3D);
		JetMemory.free_func(pBuffer2D);		
		return false; 
	}

	size_t i=0, j=0, srcidx=0, destidx=0;
	const size_t srcBytePerLine = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t ImageWHC = ImageAPI.CalcBufferSize(ImageW*nChannel, ImageH);

	for ( i=0; i<ImageH; i++ )
	{
		srcidx = i*srcBytePerLine;
		destidx = i*ImageW*nChannel;

		::memcpy(&pBuffer2D[destidx], &pDiBits[srcidx], sizeof(unsigned char)*ImageW*nChannel);
	}

	j = 0;
	for ( i=0; i<ImageW*ImageH; i++ )
	{
		switch ( nChannel )
		{
		case 1:
			pBuffer3D[i] = pBuffer2D[j];
			j ++;
			break;
		case 3:
			pBuffer3D[i] = pBuffer2D[j+2];
			j += 3;
			break;
		}
	}	
	RECT PadRect={0};
	RECT RoiRect={0};		
	this->m_OpenGLWnd.Set3DData(pBuffer3D, pBuffer2D, ImageW, ImageH, IsColor, -1, -1, -1, -1, PadRect, RoiRect, 0);	
	JetMemory.free_func(pBuffer3D);
	JetMemory.free_func(pBuffer2D);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::ExecLoadSpaceFile()
{
	const char fnName[] = "CDraw3DWnd::ExecLoadSpaceFile";
	TCHAR szFilters[]=_T("Z3D Files (*.Z3D)|*.Z3D|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("Z3D"), _T("*.Z3D"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	IMAGE_SIZE SpaceW=0;
	IMAGE_SIZE SpaceH=0;
	IMAGE_SIZE SpaceStep=0;
	SPACE_PTR  SpacePtr = NULL;

	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr = NULL;	
	const int  nAlign = 4;
	const bool Reverse = true;

	const bool bSaveSpace=false;
	CString str;
	CString filename = dialog.GetPathName();
	CString filename2 = dialog.GetPathName();
	CString filename3 = dialog.GetPathName();
	const int nLen = filename2.GetLength();
	if ( nLen < 5 ) { return false; }
	//filename2.SetAt(nLen-5, _T('2'));	
	filename2.SetAt(nLen-3, _T('P'));	
	filename2.SetAt(nLen-2, _T('N'));	
	filename2.SetAt(nLen-1, _T('G'));	
	
	filename3.SetAt(nLen-3, _T('T'));	
	filename3.SetAt(nLen-2, _T('X'));	
	filename3.SetAt(nLen-1, _T('T'));	
	//讀取Space灰階圖檔
	if ( ImageAPI.LoadSpaceGrayImage(filename, SpaceW, SpaceH, SpaceStep, SpacePtr, Reverse) == false )
	{
		JetMemory.free_func(SpacePtr);	
		JetMemory.free_func(ImagePtr);	
		return false; 
	}
	if ( ImageAPI.LoadImage(filename2, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, Reverse) == false )
	{	
		JetMemory.free_func(SpacePtr);	
		JetMemory.free_func(ImagePtr);	
		return false;
	}
	if ( ImageW!=SpaceW || ImageH!=SpaceH )
	{
		str = _T("Error, Image Size Exception");
		JetAPI::ShowMessageBox(str);
		JetMemory.free_func(SpacePtr);	
		JetMemory.free_func(ImagePtr);	
		return false;
	}

	if ( true == bSaveSpace )
	{	ImageAPI.SaveSpaceValueFile(filename3, SpaceW, SpaceH, SpaceStep, SpacePtr);	}

	RECT PadRect={0};
	RECT RoiRect={0};
	bool IsColor = false;
	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;
	if ( 8 == BitCount ) {	IsColor = false;	}
	if ( 24 == BitCount )	{ 	IsColor = true; }

	IMAGE_PTR  Buffer2DPtr = NULL;
	SPACE_PTR  Buffer3DPtr = NULL;	
	IMAGE_SIZE SpaceStep2=SpaceW;
	IMAGE_SIZE ImageStep2=JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 1);
	if ( ImageAPI.AlignSpaceImageBuffer(SpaceW, SpaceH, SpaceStep, SpacePtr, SpaceStep2, Buffer3DPtr, false) == false )
	{
		JetMemory.free_func(SpacePtr);	
		JetMemory.free_func(ImagePtr);	
		JetMemory.free_func(Buffer2DPtr);	
		JetMemory.free_func(Buffer3DPtr);	
		return false;
	}
	if ( ImageAPI.AlignImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageStep2, Buffer2DPtr, false) == false )
	{
		JetMemory.free_func(SpacePtr);	
		JetMemory.free_func(ImagePtr);	
		JetMemory.free_func(Buffer2DPtr);	
		JetMemory.free_func(Buffer3DPtr);	
		return false;
	}	
	m_OpenGLWnd.Set3DData(Buffer3DPtr, Buffer2DPtr, ImageW, ImageH, IsColor, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, PadRect, RoiRect, 0);	
	m_OpenGLWnd.SetModalCenter(true);

	JetMemory.free_func(SpacePtr);	
	JetMemory.free_func(ImagePtr);	
	JetMemory.free_func(Buffer2DPtr);	
	JetMemory.free_func(Buffer3DPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::ExecSaveFloatFile()
{
	TCHAR szFilters[]=_T("FImg Files (*.FImg)|*.FImg|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("FImg"), _T("*.FImg"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	IMAGE_SIZE SpaceW=0;
	IMAGE_SIZE SpaceH=0;
	IMAGE_SIZE SpaceStep=0;
	SPACE_PTR  SpacePtr = NULL;
	CString filename = dialog.GetPathName();

	//SpacePtr = m_OpenGLWnd.Get3DData(SpaceW, SpaceH, SpaceStep);
	if ( m_OpenGLWnd.Clone3DData(SpaceW, SpaceH, SpaceStep, SpacePtr) == false )
	{	return  false; }	
	if ( ImageAPI.SaveGraySpaceFloatFile(filename, SpaceW, SpaceH, SpaceStep, SpacePtr, false) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());	}
	JetMemory.free_func(SpacePtr);
	return true;
}
//-------------------------------------------------------------------------------------//

void CDraw3DWnd::OnLoadImageMenu() 
{
	// TODO: Add your command handler code here
	ExecLoadImageFile();	
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnLoadSpaceMenu()
{
	ExecLoadSpaceFile();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnSaveFloatMenu()
{
	ExecSaveFloatFile();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::ReleaseData()//釋放資料
{
	m_OpenGLWnd.ReleaseAllBuffer();
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::CheckCalcObject(IMAGE_SIZE DataW, IMAGE_SIZE DataH) const//確認是否要計算目標物體
{
	bool bCalcObj=false;
	const IMAGE_SIZE MaxSizeW=CALC_OBJEC_MAX_IMAGE_W;
	const IMAGE_SIZE MaxSizeH=CALC_OBJEC_MAX_IMAGE_H;
	if ( DataW<MaxSizeW || DataH<MaxSizeH )
	{	bCalcObj = true;	}
	return bCalcObj;
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::Set3DData(const int* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight)
{
	if ( true == m_ColorRangeDefined )
	{
		RulerMinH = m_ColorRangeMin;
		RulerMaxH = m_ColorRangeMax;
	}
	this->m_OpenGLWnd.Set3DData(p3D, p2D, DataW, DataH, IsColor, RulerMinH, RulerMaxH, ShowMinH, ShowMaxH, PadRect, ROIRect, PadSpecHeight);	
	BuildProfileWnd(m_ProfileWnd);
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::Set3DData(const float* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight)
{
	if ( true == m_ColorRangeDefined )
	{
		RulerMinH = m_ColorRangeMin;
		RulerMaxH = m_ColorRangeMax;
	}
	this->m_OpenGLWnd.Set3DData(p3D, p2D, DataW, DataH, IsColor, RulerMinH, RulerMaxH, ShowMinH, ShowMaxH, PadRect, ROIRect, PadSpecHeight);	
	BuildProfileWnd(m_ProfileWnd);
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::SetBoundPadOpen(bool Open)//顯示物件外框
{
	this->m_OpenGLWnd.SetBoundPadOpen(Open);
	this->m_OpenGLWnd.SetShowPadHeight(Open);
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnObjectColorMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnObjectLineMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_LINE);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnObjectTextureMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_TEXTURE);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnObjectColorLineMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR_LINE);	
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnUpperPlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetUpperPlaneOpen() == true )
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(true); }
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnLowerPlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetLowerPlaneOpen() == true )
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(true); }
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnClipPlaneMenu()
{
	if ( this->m_OpenGLWnd.GetClipPlaneOpen() == true )
	{
		this->m_ShowProfileWnd = false;
		this->m_OpenGLWnd.SetClipPlaneOpen(false); 
	}
	else
	{
		this->m_ShowProfileWnd = true;
		this->m_OpenGLWnd.SetClipPlaneOpen(true); 
	}
	this->AdjustControlWnd();
	this->UpdateCommandUI();	
	this->Invalidate();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnClipPlaneHorMenu()
{
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_HOR);
	this->BuildProfileWnd(m_ProfileWnd);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnClipPlaneVerMenu()
{
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_VER);
	this->BuildProfileWnd(m_ProfileWnd);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnClipPlaneAnyMenu()
{
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_ANY);
	this->BuildProfileWnd(m_ProfileWnd);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnBoundBoxMenu() 
{
	// TODO: Add your command handler code here
	bool bOpen=true;
	if ( this->m_OpenGLWnd.GetBoundPadOpen() == true )
	{	bOpen = false; }
	else
	{	bOpen = true; }
	this->m_OpenGLWnd.SetBoundPadOpen(bOpen);
	this->m_OpenGLWnd.SetShowPadHeight(bOpen);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnShowProfileWndMenu()
{
	if ( true == m_ShowProfileWnd ) 
	{	
		this->m_ShowProfileWnd = false;	
		this->m_OpenGLWnd.SetClipPlaneOpen(false); 
	}
	else
	{	
		this->m_ShowProfileWnd = true;	
		this->m_OpenGLWnd.SetClipPlaneOpen(true); 
	}
	this->AdjustControlWnd();
	this->UpdateCommandUI();
	this->Invalidate();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnShowAxisMenu()
{
	if ( this->m_OpenGLWnd.GetIsShowAxis() == true )
	{	this->m_OpenGLWnd.SetIsShowAxis(false); }
	else
	{	this->m_OpenGLWnd.SetIsShowAxis(true); }
	this->UpdateCommandUI();
	this->Invalidate();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDefaultMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetModalCenter(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::UpdateCommandUI()
{
	//Menu
	UINT    ItemID = 0;
	CMenu *pMenu = this->GetMenu();
	if ( pMenu != NULL )
	{	
		OPENGL_OBJECT_MODE ObjectMode = this->m_OpenGLWnd.GetMainObjectMode();
		switch ( ObjectMode )
		{
		case OPENGL_OBJECT_COLOR:
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_MENU, MF_CHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_LINE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_TEXTURE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_LINE_MENU, MF_UNCHECKED); 
			
			break;
		case OPENGL_OBJECT_LINE:
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_LINE_MENU, MF_CHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_TEXTURE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_LINE_MENU, MF_UNCHECKED); 
			break;
		case OPENGL_OBJECT_TEXTURE:
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_LINE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_TEXTURE_MENU, MF_CHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_LINE_MENU, MF_UNCHECKED); 
			break;
		case OPENGL_OBJECT_COLOR_LINE:
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_LINE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_TEXTURE_MENU, MF_UNCHECKED); 
			pMenu->CheckMenuItem(DRAW3D_OBJECT_COLOR_LINE_MENU, MF_CHECKED); 
			break;
		}  
		
		ItemID = DRAW3D_BASE_PLANE_MENU;
		if ( this->m_OpenGLWnd.GetBasePlaneOpen() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = DRAW3D_UPPER_PLANE_MENU;
		if ( this->m_OpenGLWnd.GetUpperPlaneOpen() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = DRAW3D_LOWER_PLANE_MENU;
		if ( this->m_OpenGLWnd.GetLowerPlaneOpen() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }		
		
		ItemID = DRAW3D_BOUND_BOX_MENU;
		if ( this->m_OpenGLWnd.GetBoundPadOpen() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }		

		ItemID = DRAW3D_SHOW_PROFILE_WND_MENU;
		if ( true == m_ShowProfileWnd )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }				

		ItemID = DRAW3D_SHOW_AXIS_MENU;
		if ( true == m_OpenGLWnd.GetIsShowAxis() )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }		

		//動畫
		ItemID = DRAW3D_ANIMATION_PLAY_MENU;
		if ( this->m_OpenGLWnd.GetAnimationPlay() == true ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		ItemID = DRAW3D_ANIMATION_STOP_MENU;
		if ( this->m_OpenGLWnd.GetAnimationPlay() == false ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		//精細度
		int nLv = m_OpenGLWnd.GetDetailLevel();		
		ItemID = DRAW3D_DETAIL_LEVEL_MENU_HIGH_MOST;
		if ( OPEN_GL_FINENESS_HIGHT_MOST == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	
		ItemID = DRAW3D_DETAIL_LEVEL_MENU_HIGH_MORE;
		if ( OPEN_GL_FINENESS_HIGHT_MORE == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	
		ItemID = DRAW3D_DETAIL_LEVEL_MENU_HIGH;
		if ( OPEN_GL_FINENESS_HIGHT == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		ItemID = DRAW3D_DETAIL_LEVEL_MENU_MIDDLE;
		if ( OPEN_GL_FINENESS_MIDDLE == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		ItemID = DRAW3D_DETAIL_LEVEL_MENU_LOW;
		if ( OPEN_GL_FINENESS_LOW == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		ItemID = DRAW3D_DETAIL_LEVEL_MENU_LOW_MORE;
		if ( OPEN_GL_FINENESS_LOW_MORE == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		ItemID = DRAW3D_DETAIL_LEVEL_MENU_LOW_MOST;
		if ( OPEN_GL_FINENESS_LOW_MOST == nLv ) 
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }	

		//橫切面
		ItemID = DRAW3D_CLIP_PLANE_MENU;
		if ( this->m_OpenGLWnd.GetClipPlaneOpen() == true )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }
		
		int ClipDir = m_OpenGLWnd.GetClipPlaneDir();
		ItemID = DRAW3D_CLIP_PLANE_HOR_MENU;		
		if ( OPENGL_CLIP_PLANE_HOR == ClipDir )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = DRAW3D_CLIP_PLANE_VER_MENU;
		if ( OPENGL_CLIP_PLANE_VER == ClipDir )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = DRAW3D_CLIP_PLANE_ANY_MENU;
		if ( OPENGL_CLIP_PLANE_ANY == ClipDir )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }

		ItemID = DRAW3D_COLOR_RANGE_MENU;
		if ( true == m_ColorRangeDefined )
		{	pMenu->CheckMenuItem(ItemID, MF_CHECKED); }
		else
		{	pMenu->CheckMenuItem(ItemID, MF_UNCHECKED); }
	}
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnBasePlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetBasePlaneOpen() == true )
	{	this->m_OpenGLWnd.SetBasePlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetBasePlaneOpen(true); }
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnPlanePitchMenu() 
{
	// TODO: Add your command handler code here
	CString strValue;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;	

	strCaption = _T("Move Plane Pitch");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Pitch (um):");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.2f"), m_OpenGLWnd.GetBoundPitch());
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	float Pitch = (float)(::_tcstod(InputBox.m_DataEdit1, NULL));
	m_OpenGLWnd.SetBountPitch(Pitch);
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnPlaneHeightMenu()
{
	CString strValue;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;	

	strCaption = _T("Plane Height");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Height (um):");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.2f"), m_OpenGLWnd.GetBoundHeight());
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	float Height = (float)(::_tcstod(InputBox.m_DataEdit1, NULL));
	m_OpenGLWnd.SetBountHeight(Height);
	this->UpdateCommandUI();
	//this->Invalidate();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnColorRangeMenu()
{	
	DWORD   Res=0;
	CString strValue1;
	CString strValue2;
	CString strLabel1;
	CString strLabel2;
	CString strCaption;
	CString strMax=AOIDataDefine.GetMaxText();
	CString strMin=AOIDataDefine.GetMinText();
	CInputBoxWnd InputBox;	

	strCaption = _T("Define Color Range");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	Res = JetAPI::ShowMessageBox(strCaption, MB_YESNOCANCEL);
	if ( IDCANCEL == Res ) { return; }
	if ( IDYES != Res )
	{	m_ColorRangeDefined = false;	}
	else
	{
		strCaption = _T("Set Color Range");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	

		strLabel1.Format(_T("%s (um)"), strMin);
		strLabel2.Format(_T("%s (um)"), strMax);		
		strValue1.Format(_T("%.2f"), m_ColorRangeMin);
		strValue2.Format(_T("%.2f"), m_ColorRangeMax);
		InputBox.SetParam2(strCaption, strLabel1, strValue1, strLabel2, strValue2);
		if ( InputBox.DoModal() == IDCANCEL ) 
		{	return;  }

		m_ColorRangeDefined = true;
		m_ColorRangeMin = ::_ttof(InputBox.m_DataEdit1);
		m_ColorRangeMax = ::_ttof(InputBox.m_DataEdit2);
	}
	
	float RuleMin=m_OpenGLWnd.GetDataMinH();
	float RuleMax=m_OpenGLWnd.GetDataMaxH();
	if ( true == m_ColorRangeDefined )
	{
		RuleMin=m_ColorRangeMin;
		RuleMax=m_ColorRangeMax;
	}	
	m_OpenGLWnd.SetRuleMinH(RuleMin);
	m_OpenGLWnd.SetRuleMaxH(RuleMax);	
	this->UpdateCommandUI();
	//this->Invalidate();
	
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnAnimationPlayMenu()
{
	m_OpenGLWnd.SetAnimationPlay(true);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnAnimationStopMenu()
{
	m_OpenGLWnd.SetAnimationPlay(false);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuHighMost()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_HIGHT_MOST);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuHighMore()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_HIGHT_MORE);
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuHigh()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_HIGHT);//1
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuMiddle()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_MIDDLE);//2
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuLow()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_LOW);//4
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuLowMore()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_LOW_MORE);//4
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CDraw3DWnd::OnDetailLevelMenuLowMost()
{
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_LOW_MOST);//4
	UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::InitProfileWnd(CThisChartCtrl &ChartWnd)//初始化圖表視窗-剖線圖
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 14;//12
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/
	ChartWnd.SetChartType(CHART_TYPE_LINE);
	ChartWnd.SetXAxisUnit("pxl");
	ChartWnd.SetYAxisUnit("um");
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);
	ChartWnd.SetIsShowLegend(false);
//	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_BOTTOM);	
	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_RIGHT);	

	ChartWnd.Set3DThicness(10);
	ChartWnd.SetChartTopSpace(30);
	ChartWnd.SetChartBottomSpace(40);
	ChartWnd.SetChartLeftSpace(40);
	ChartWnd.SetChartRightSpace(20);

	ChartWnd.SetSeriesLeftSpace(5);
	ChartWnd.SetSeriesRightSpace(5);
	ChartWnd.SetSeriesTopSpace(5);
	ChartWnd.SetSeriesBottomSpace(5);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::BuildProfileWnd(CThisChartCtrl &ChartWnd)//建立圖表視窗-剖線圖
{
	ChartWnd.Clear();
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }
	ChartWnd.Set3DThicness(20);		

	CString str;
	CString XLabel = _T("");
	CJetSeries *pSeries1 = new CJetSeries();
	CJetSeries *pSeries2 = new CJetSeries();
	if ( NULL==pSeries1 || NULL==pSeries2 )
	{
		delete pSeries1; pSeries1=NULL;
		delete pSeries2; pSeries2=NULL;
		return false; 
	}

	pSeries1->SetSeriesType(SERIES_TYPE_LINE);
	pSeries1->SetIsVisible(true);
	pSeries1->SetDotWidth(0);
	pSeries1->SetLineColor(0x0000FF);	
	//pSeries1->SetDotColor(0x0000FF);
	pSeries1->SetIsShowMarkValue(false);

	pSeries2->SetSeriesType(SERIES_TYPE_BAR);
	pSeries2->SetIsVisible(true);
	pSeries2->SetDotWidth(0);
	pSeries2->SetLineColor(0xD0FFFF);	
	//pSeries2->SetDotColor(0x0000FF);
	pSeries2->SetIsShowMarkValue(true);
	
	
	int   i=0;	
	int   SeriesIndex = 0;
	int   nPosX=0, nPosY=0;
	int   nSizeW=0;
	double PosX=0, PosY=0;	
	const int SmoothSize=5;
	CString strValue;
	std::vector<float>  DataList;
	std::vector<TPiece> PieceList;	
	const float MinH = m_OpenGLWnd.GetShowMinH();
	const float MaxH = m_OpenGLWnd.GetShowMaxH();
	m_OpenGLWnd.BuildProfileValue(DataList);
	SmoothProfile(SmoothSize, DataList);
	const int DataCount = (int)(DataList.size());
	SeriesIndex = 0;
	for ( i=0; i<DataCount; i++ )
	{	pSeries1->AddXYValue(true, i, i, DataList[i]);	}

	int PieceCount = 0;
	double dRatio = 1.25;
	const int  nMinW = 5;
	dRatio = 1.25;
	BuildPieceList(DataList, dRatio, nMinW, PieceList);
	PieceCount = (int)(PieceList.size());
	if ( 0 == PieceCount )
	{
		dRatio = 1.00;
		BuildPieceList(DataList, dRatio, nMinW, PieceList);
		PieceCount = (int)(PieceList.size());
	}
	for ( i=0; i<PieceCount; i++ )
	{	
		nPosX = (PieceList[i].nX1+PieceList[i].nX2)/2;
		nSizeW = abs(PieceList[i].nX2-PieceList[i].nX1);
		strValue.Format(_T("%.0f"), PieceList[i].fValue);		
		pSeries2->AddXYValue(true, nPosX, nPosX, PieceList[i].fValue, NULL, strValue, 0x000000, nSizeW);	
	}	

	ChartWnd.SetYAxisMin(MinH);
	ChartWnd.SetYAxisMax(MaxH);
	ChartWnd.SetYAxisPercentageTarget(MaxH);
	ChartWnd.SetYAxisPercentageVisible(true);
	//ChartWnd.SetYAxisFix(true);
	
	ChartWnd.AddSeries(pSeries2);
	ChartWnd.AddSeries(pSeries1);	
	delete pSeries1; pSeries1=NULL;	
	delete pSeries2; pSeries2=NULL;	

	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::SmoothProfile(int KenSize, std::vector<float> &DataList)//平滑線段
{
	size_t       i=0, j=0;	
	int          Count=0;
	float        DropVal=0.0f;
	double       Sum=0;
	double       Ave=0;
	const size_t DataCount = DataList.size();	
	if ( KenSize > DataCount ) { return false; }
	const int KenSize2 = KenSize/2;
	std::vector<float> TmpDataList=DataList;
	for ( i=KenSize2; i<DataCount-KenSize2; i++ )
	{
		Sum = 0.0f;
		Count = 0;
		for ( j=i-KenSize2; j<=i+KenSize2; j++ )
		{
			Sum += TmpDataList[j];
			Count ++;
		}
		if ( Count == 0 ) { continue; }
		Ave = Sum/Count;		
		DataList[i] = Ave;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDraw3DWnd::BuildPieceList(const std::vector<float> &Profile, double dRatio, int nMinW, std::vector<TPiece> &PieceList)//取得子線段
{
	size_t       i=0, j=0;	
	int          Count=0;
	float        DropVal=0.0f;
	double       Sum=0;
	double       Ave=0;
	const size_t DataCount = Profile.size();		

	PieceList.clear();
	if ( 0 == DataCount ) { return true; }	
	for ( i=0; i<DataCount; i++ )
	{
		Sum += Profile[i];
		Count ++;		
	}
	if ( Count > 0 ) 
	{	Ave = Sum/Count; }
	const float Threshold = Ave*dRatio;

	Sum=0;
	Count=0;
	TPiece Piece;
	float fValue=0;	
	for ( i=0; i<DataCount; i++ )
	{
		fValue = Profile[i];
		if ( fValue < Threshold ) 
		{
			if ( Piece.nX1 >= 0 )
			{
				if ( Count > nMinW ) 
				{
					Ave = Sum/Count;
					Piece.nX2 = i;
					Piece.fValue = Ave;					
					PieceList.push_back(Piece);				
				}
				Piece = TPiece();
			}
			Sum = 0;
			Count = 0;			
			continue; 
		}		
		Sum += fValue;
		Count ++;
		if ( Piece.nX1 < 0 ) 
		{	Piece.nX1 = i; }
	}
	if ( Piece.nX1 >= 0  )
	{		
		if ( Count > nMinW ) 
		{
			Ave = Sum/Count;
			Piece.nX2 = i;
			Piece.fValue = Ave;
			PieceList.push_back(Piece);
		}
		Piece = TPiece();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
