// AlgImageCompareWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "AlgImageCompareWnd.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
//-------------------------------------------------------------------------------------//
enum IMGCMP_SHOW_MODE
{
	IMGCMP_SHOW_RAW,	
	IMGCMP_SHOW_EDGE,
	IMGCMP_SHOW_REMOVE_EDGE,
	IMGCMP_SHOW_GAUSSIAN,
	IMGCMP_SHOW_BINARY,
	IMGCMP_SHOW_DILATE,	
	IMGCMP_SHOW_OPEN,
	IMGCMP_SHOW_CLOSE,
	IMGCMP_SHOW_BLOB,
	IMGCMP_SHOW_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMGCMP_SIZE_LOG_MODE
{
	IMGCMP_SIZE_LOG_OFF,
	IMGCMP_SIZE_LOG_OR,
	IMGCMP_SIZE_LOG_AND,
	IMGCMP_SIZE_LOG_RETURN
};
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageCompareWnd dialog
//-------------------------------------------------------------------------------------//
CAlgImageCompareWnd::CAlgImageCompareWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgImageCompareWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgImageCompareWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ActWndID = NULL;
	m_Matched = false;
	m_ColorRatio = 2.0;
	m_TempFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ImgCmp"));

	m_WndBKColor=0x000000;
	m_ImageW = 1024;
	m_ImageH = 1024;	
	m_ImageStep = m_ImageW;
	m_ImageBitCount = 8;
	m_ImagePtr = NULL;
	m_ImageOffset = TPOINT2D();
	m_ImageZoom = 1.0;;
	
	m_PatternW = 1024;
	m_PatternH = 1024;	
	m_PatternStep = m_PatternW;
	m_PatternBitCount = 8;
	m_PatternPtr = NULL;
	m_PatternOffset = TPOINT2D();
	m_PatternZoom = 1.0;;

	m_ResultW = 1024;
	m_ResultH = 1024;	
	m_ResultStep = m_ResultW;
	m_ResultBitCount = 8;
	m_ResultPtr = NULL;
	m_ResultOffset = TPOINT2D();
	m_ResultZoom = 1.0;;

	POINT TempPt={0,0};
	m_LBtnPtUp = m_LBtnPtDown = TempPt;
	m_RBtnPtUp = m_RBtnPtDown = TempPt;
	m_MousePtLast = m_MousePtCurrent = TempPt;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgImageCompareWnd)
	DDX_Control(pDX, ALGIMGCMP_SHOW_MODE_COMBOX, m_ShowModeCombox);
	DDX_Control(pDX, ALGIMGCMP_MORPH_SHAPE_MODE_COMBO, m_MorphShapeCombox);	
	DDX_Control(pDX, ALGIMGCMP_PARAM_MIN_MODE_COMBO, m_MinLogModeCombox);	
	DDX_Control(pDX, ALGIMGCMP_PARAM_MAX_MODE_COMBO, m_MaxLogModeCombox);		
	DDX_Control(pDX, ALGIMGCMP_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, ALGIMGCMP_PATTERN_WND, m_PatternWnd);
	DDX_Control(pDX, ALGIMGCMP_RESULT_WND, m_ResultWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgImageCompareWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgImageCompareWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_BN_CLICKED(ALGIMGCMP_LOAD_IMAGE_BTN, OnLoadImageBtn)
	ON_BN_CLICKED(ALGIMGCMP_LOAD_PATTERN_BTN, OnLoadPatternBtn)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(ALGIMGCMP_MATCH_BTN, OnMatchBtn)
	ON_BN_CLICKED(ALGIMGCMP_RESET_VIEW_BTN, OnResetViewBtn)
	ON_BN_CLICKED(ALGIMGCMP_PATTERN_POS_SET_BTN, OnPatternPosSetBtn)
	ON_BN_CLICKED(ALGIMGCMP_PATTERN_CALC_BTN, OnPatternCalcBtn)
	ON_CBN_SELCHANGE(ALGIMGCMP_SHOW_MODE_COMBOX, OnSelchangeShowModeCombox)
	ON_BN_CLICKED(ALGIMGCMP_PATTERN_RES_BTN, OnPatternResBtn)
	ON_BN_CLICKED(ALGIMGCMP_PATTERN_USE_SCALE_CHK, OnPatternUseScaleChk)
	ON_BN_CLICKED(ALGIMGCMP_ENHANCE_IMAGE_CHK, OnEnhanceImageChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageCompareWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgImageCompareWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_WndBKColor=0xB0E4EF;
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	/*
	COLORREF BKColor = m_WndBKColor;
	this->GetClientRect(&m_WndRect);
	this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	this->m_PatternWnd.GetClientRect(&m_PatternWndRect);
	this->m_ResultWnd.GetClientRect(&m_ResultWndRect);
	this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, BKColor);	
	this->m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, BKColor);	
	this->m_PatternWndMemDC.CreateMemDC(&m_PatternWnd, BKColor);
	this->m_PatternWndMemDC2.CreateMemDC(&m_PatternWnd, BKColor);
	this->m_ResultWndMemDC.CreateMemDC(&m_ResultWnd, BKColor);
	this->m_ResultWndMemDC2.CreateMemDC(&m_ResultWnd, BKColor);
	*/	
	BuildShowModeCombox();
	BuildMorphShapeModeCombox();
	BuildSizeLogicModeCombox(m_MinLogModeCombox);
	BuildSizeLogicModeCombox(m_MaxLogModeCombox);
	SwitchMultiLanguage();

	CString str;	
	int nDarkLevel=50;
	int nBrightLevel=200;
	int nTolerance=50;
	int nSmooth=0;
	int nGaussian=0;
	int nDilate=0;
	int nOpen=3;	
	int nClose=5;	
	int nMinW=240;
	int nMinH=240;
	int nMaxW=100000;
	int nMaxH=100000;
	BOOL  bMatchScale = FALSE;
	BOOL  bEdgeRemove = FALSE;
	double fMaxRatio=100.0;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();	
	if ( NULL != ProjectPtr )
	{		
		TProjectParameter &Param = ProjectPtr->GetProjectParameter();
		bMatchScale=Param.m_DropOutPartMatchUseScale;
		nDarkLevel = Param.m_DropOutPartDarkLevel;
		nBrightLevel = Param.m_DropOutPartLightLevel;
		nTolerance = Param.m_DropOutPartTolerance;
		nSmooth = Param.m_DropOutPartSmoothSize;		
		bEdgeRemove = Param.m_DropOutPartEdgeRemove;
		nDilate = Param.m_DropOutPartDilateSize;		
		nGaussian = Param.m_DropOutPartGaussian;
		nOpen = Param.m_DropOutPartFilterOpenSize;
		nClose = Param.m_DropOutPartFilterCloseSize;
		nMinW = Param.m_DropOutPartMinSizeW;
		nMinH = Param.m_DropOutPartMinSizeH;
		fMaxRatio=Param.m_DropOutPartMaxSizeR;		
	}
	if ( nSmooth > 0 ) 
	{	CWnd::CheckDlgButton(ALGIMGCMP_SMOOTH_SIZE_CHK, TRUE); }
	CWnd::CheckDlgButton(ALGIMGCMP_ENHANCE_IMAGE_CHK, TRUE);	
	CWnd::SetDlgItemInt(ALGIMGCMP_REMOVE_EDGE_GAUS_EDIT, 0);	
	CWnd::CheckDlgButton(ALGIMGCMP_REMOVE_EDGE_CHK, bEdgeRemove);
	JetAPI::SetComboxCurSel(m_MinLogModeCombox, IMGCMP_SIZE_LOG_OR);//IMGCMP_SIZE_LOG_AND
	JetAPI::SetComboxCurSel(m_MaxLogModeCombox, IMGCMP_SIZE_LOG_OFF);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_DARK_LEVEL_EDIT, nDarkLevel);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_BRIGHT_LEVEL_EDIT, nBrightLevel);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_TOLERANCE_EDIT, nTolerance);
	CWnd::SetDlgItemInt(ALGIMGCMP_SMOOTH_SIZE_EDIT, nSmooth);		
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_GAUSSIAN_EDIT, nGaussian);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_DILATE_EDIT, nDilate);	
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_OPEN_EDIT, nOpen);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_CLOSE_EDIT, nClose);		
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_MIN_W_EDIT, nMinW);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_MIN_H_EDIT, nMinH);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_MAX_W_EDIT, nMaxW);
	CWnd::SetDlgItemInt(ALGIMGCMP_PARAM_MAX_H_EDIT, nMaxH);
	CWnd::SetDlgItemInt(ALGIMGCMP_RESOLUTION_SCALE_EDIT, 4);	
	str.Format(_T("%.2f"), fMaxRatio);
	CWnd::SetDlgItemText(ALGIMGCMP_PARAM_MAX_RATIO_EDIT, str);	

	CWnd::CheckDlgButton(ALGIMGCMP_PATTERN_USE_SCALE_CHK, bMatchScale);	

	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_X_EDIT, _T("0"));	
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_Y_EDIT, _T("0"));

	str.Format(_T("%.2f"), m_PatToImgOffset.x);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_X_EDIT, str);
	str.Format(_T("%.2f"), m_PatToImgOffset.y);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_Y_EDIT, str);

	const double ResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	str.Format(_T("%.4f"), ResX);
	CWnd::SetDlgItemText(ALGIMGCMP_RESOLUTION_X_EDIT, str);
	str.Format(_T("%.4f"), ResY);
	CWnd::SetDlgItemText(ALGIMGCMP_RESOLUTION_Y_EDIT, str);	

	SetFocusEditWnd();

	::CreateDirectory(m_TempFolder, NULL);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseImageBuffer();
	ReleasePatternBuffer();
	ReleaseResultBuffer();

	JetAPI::ClearFolder(m_TempFolder);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	int   EditRectB = 0;	
	COLORREF BKColor = m_WndBKColor;

	WndPtr = GetDlgItem(ALGIMGCMP_INFO_EDIT);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		RECT  WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		EditRectB = WndRect.bottom;
	}
	WndPtr = GetDlgItem(ALGIMGCMP_INFO_EDIT2);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		RECT  WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		EditRectB = WndRect.bottom;
	}
	const int Margin = 4;
	const int ImageWndH = (cy-EditRectB-Margin-Margin)/2;
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0,0,0,0};
		m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.top = EditRectB+Margin;
		WndRect.bottom = WndRect.top+ImageWndH;
		m_ImageWnd.MoveWindow(&WndRect);

		this->m_ImageWnd.GetClientRect(&m_ImageWndRect);	
		this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, BKColor);		
		this->m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, BKColor);		
		EditRectB = WndRect.bottom;
	}
	if ( m_PatternWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0,0,0,0};
		m_PatternWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.top = EditRectB+Margin;
		WndRect.bottom = WndRect.top+ImageWndH;
		m_PatternWnd.MoveWindow(&WndRect);

		this->m_PatternWnd.GetClientRect(&m_PatternWndRect);
		this->m_PatternWndMemDC.CreateMemDC(&m_PatternWnd, BKColor);
		this->m_PatternWndMemDC2.CreateMemDC(&m_PatternWnd, BKColor);

		//EditRectB = WndRect.bottom;
	}
	if ( m_ResultWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0,0,0,0};
		m_ResultWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.top = EditRectB+Margin;
		WndRect.bottom = WndRect.top+ImageWndH;
		m_ResultWnd.MoveWindow(&WndRect);

		this->m_ResultWnd.GetClientRect(&m_ResultWndRect);
		this->m_ResultWndMemDC.CreateMemDC(&m_ResultWnd, BKColor);
		this->m_ResultWndMemDC2.CreateMemDC(&m_ResultWnd, BKColor);
		//EditRectB = WndRect.bottom;
	}	
	this->GetClientRect(&m_WndRect);
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	if ( lpMMI->ptMinTrackSize.x < 1600 ) { lpMMI->ptMinTrackSize.x = 1600; }
	if ( lpMMI->ptMinTrackSize.y < 800 ) { lpMMI->ptMinTrackSize.y = 800; }
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT WndID = CheckBtnClickWndID(point);
	if ( NULL == WndID ) 
	{	return; }
	SetCapture();	
	m_ActWndID = WndID;
	m_LBtnPtUp = m_LBtnPtDown = point;
	m_MousePtLast = m_MousePtCurrent = point;	
	UpdateInfoText(ALGIMGCMP_INFO_EDIT2);
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();

	m_ActWndID = NULL;
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateCursorPosToWnds(point);	
	if ( this != GetCapture() ) 
	{	return; }

	POINT Offset;
	m_MousePtCurrent = point;
	Offset.x = m_MousePtCurrent.x-m_MousePtLast.x;
	Offset.y = m_MousePtCurrent.y-m_MousePtLast.y;
	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}
	else if ( (nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		m_ImageOffset.x += Offset.x;
		m_ImageOffset.y += Offset.y;
		m_PatternOffset.x += Offset.x;
		m_PatternOffset.y += Offset.y;
		m_ResultOffset.x += Offset.x;
		m_ResultOffset.y += Offset.y;
		DrawImageWndBkDC();
		DrawPatternWndBkDC();
		DrawResultWndBkDC();
		RedrawWnd();
	}
	m_MousePtLast = point;	
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CAlgImageCompareWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	POINT point = pt;
	CWnd::ScreenToClient(&point);
	UINT WndID = CheckBtnClickWndID(point);
	if ( NULL == WndID ) 
	{	
		return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
	}

	if ( zDelta > 0 ) 
	{	
		m_ImageZoom *= 1.1; 
		m_PatternZoom *= 1.1;
		m_ResultZoom *= 1.1;
	}
	else
	{	
		m_ImageZoom /= 1.1; 
		m_PatternZoom /= 1.1;
		m_ResultZoom /= 1.1;
	}
	DrawImageWndBkDC();
	DrawPatternWndBkDC();
	DrawResultWndBkDC();
	RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT WndID = CheckBtnClickWndID(point);
	if ( NULL == WndID ) 
	{	return; }
	SetCapture();
	m_ActWndID = WndID;
	m_LBtnPtUp = m_LBtnPtDown = point;
	m_MousePtLast = m_MousePtCurrent = point;	
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();

	m_ActWndID = NULL;
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_IMAGE_COMPARE_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_IMAGE_COMPARE_WND;
	WndKey = _T("IDD_ALG_IMAGE_COMPARE_WND");
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
	//WndID = ALGCHAR_ROI_GROUP;
	//WndKey = _T("ALGCHAR_ROI_GROUP");
	//this->GetDlgItemText(WndID, LabelText);
	//AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	//this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CAlgImageCompareWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_IMAGE_COMPARE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::BuildShowModeCombox()
{
	CComboBox &Combox=m_ShowModeCombox;
	if ( Combox.GetSafeHwnd() == NULL ) { return true; }
	
	
	int       idx=0;	
	CString   String;
	DWORD     Data=0;
	
	idx = 0;
	JetAPI::ClearCombox(Combox);		
	
	String = _T("Raw");
	Data = IMGCMP_SHOW_RAW;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Edge");
	Data = IMGCMP_SHOW_EDGE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Remove Edge");
	Data = IMGCMP_SHOW_REMOVE_EDGE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	
	String = _T("Gaussian");
	Data = IMGCMP_SHOW_GAUSSIAN;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Binary");
	Data = IMGCMP_SHOW_BINARY;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Dilate");
	Data = IMGCMP_SHOW_DILATE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;	

	String = _T("Open");
	Data = IMGCMP_SHOW_OPEN;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Close");
	Data = IMGCMP_SHOW_CLOSE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	/*
	String = _T("Blob");
	Data = IMGCMP_SHOW_BLOB;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;
	*/

	if ( idx > 0 )
	{	Combox.SetCurSel(idx-1); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::BuildMorphShapeModeCombox()
{
	CComboBox &Combox=m_MorphShapeCombox;
	if ( Combox.GetSafeHwnd() == NULL ) { return true; }
	
	
	int       idx=0;	
	CString   String;
	DWORD     Data=0;
	
	idx = 0;
	JetAPI::ClearCombox(Combox);

	String = _T("Rect");
	Data = MORPH_SHAPE_RECT;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Cross");
	Data = MORPH_SHAPE_CROSS;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("Ellipse");
	Data = MORPH_SHAPE_ELLIPSE;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	if ( idx > 0 )
	{	
		//Combox.SetCurSel(0); 
		JetAPI::SetComboxCurSel(Combox, MORPH_SHAPE_ELLIPSE);//MORPH_SHAPE_RECT
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::BuildSizeLogicModeCombox(CComboBox &Combox)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return true; }
	
	
	int       idx=0;	
	CString   String;
	DWORD     Data=0;
	
	idx = 0;
	JetAPI::ClearCombox(Combox);
	
	String = _T("OFF");
	Data = IMGCMP_SIZE_LOG_OFF;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("OR");
	Data = IMGCMP_SIZE_LOG_OR;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	String = _T("AND");
	Data = IMGCMP_SIZE_LOG_AND;	
	Combox.InsertString(-1, String);
	Combox.SetItemData(idx, Data);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::SetFocusEditWnd()
{
	CWnd *WndPtr = NULL;

	WndPtr = CWnd::GetDlgItem(ALGIMGCMP_PARAM_DARK_LEVEL_EDIT);
	if ( NULL != WndPtr ) 
	{	
		WndPtr->SetFocus(); 
		return true;
	}
	WndPtr = CWnd::GetDlgItem(ALGIMGCMP_PARAM_BRIGHT_LEVEL_EDIT);
	if ( NULL != WndPtr ) 
	{	
		WndPtr->SetFocus(); 
		return true;
	}
	WndPtr = CWnd::GetDlgItem(ALGIMGCMP_PARAM_TOLERANCE_EDIT);
	if ( NULL != WndPtr ) 
	{	
		WndPtr->SetFocus(); 
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdateInfoText(UINT EditID)
{
//	CWnd::SetDlgItemText(EditID, _T(""));
//	if ( NULL==m_ImagePtr || NULL==m_PatternPtr ) { return false; }

	CString str;
	CString strTmp;
	CString strImage;
	CString strPattern;
	CString strResult;
	bool    bCalc=false;
	int     ImgR=0, ImgG=0, ImgB=0;
	int     PatR=0, PatG=0, PatB=0;

	bCalc=true;
	if ( NULL != m_ImagePtr )
	{		
		POINT ImagePt;
		size_t index=0;
		int   R=0, G=0, B=0;		
		JetAPI::Point2DToPoint(m_ImageImgPt, ImagePt);
		ImagePt.x = (int)(m_ImageImgPt.x);
		ImagePt.y = (int)(m_ImageImgPt.y);
		if ( ImagePt.x<0 || ImagePt.y<0 || ImagePt.x>=m_ImageW || ImagePt.y>=m_ImageH ) 
		{
			bCalc = false;
			strImage = _T(""); 
		}
		else
		{
			if ( 24 == m_ImageBitCount )
			{
				index = (ImagePt.y*m_ImageStep)+(ImagePt.x*3);
				B = m_ImagePtr[index];
				G = m_ImagePtr[index+1];
				R = m_ImagePtr[index+2];
			}
			else
			{
				index = (ImagePt.y*m_ImageStep)+(ImagePt.x);
				R = G = B = m_ImagePtr[index];
			}			
			ImgR = R;
			ImgG = G;
			ImgB = B;
			strImage.Format(_T("Img(%d, %d)=(%d, %d, %d)"), ImagePt.x, ImagePt.y, R, G, B);
		}
	}
	else 
	{	bCalc = false; }

	if ( NULL != m_PatternPtr )
	{		
		POINT ImagePt;
		size_t index=0;
		int   R=0, G=0, B=0;		
		JetAPI::Point2DToPoint(m_PatternImgPt, ImagePt);
		ImagePt.x = (int)(m_PatternImgPt.x);
		ImagePt.y = (int)(m_PatternImgPt.y);
		if ( ImagePt.x<0 || ImagePt.y<0 || ImagePt.x>=m_PatternW || ImagePt.y>=m_PatternH ) 
		{
			bCalc = false;
			strPattern = _T(""); 
		}
		else
		{
			if ( 24 == m_PatternBitCount )
			{
				index = (ImagePt.y*m_PatternStep)+(ImagePt.x*3);
				B = m_PatternPtr[index];
				G = m_PatternPtr[index+1];
				R = m_PatternPtr[index+2];
			}
			else
			{
				index = (ImagePt.y*m_PatternStep)+(ImagePt.x);
				R = G = B = m_PatternPtr[index];
			}
			PatR = R;
			PatG = G;
			PatB = B;
			strPattern.Format(_T("Pat(%d, %d)=(%d, %d, %d)"), ImagePt.x, ImagePt.y, R, G, B);
		}
	}
	else 
	{	bCalc = false; }
	
	if ( NULL != m_ResultPtr )
	{		
		POINT ImagePt;
		size_t index=0;
		int   R=0, G=0, B=0;		
		JetAPI::Point2DToPoint(m_ResultImgPt, ImagePt);
		ImagePt.x = (int)(m_ResultImgPt.x);
		ImagePt.y = (int)(m_ResultImgPt.y);
		if ( 524==ImagePt.x && 376==ImagePt.y ) 
		{
			ImagePt.x = ImagePt.x;
		}
		if ( ImagePt.x<0 || ImagePt.y<0 || ImagePt.x>=m_ResultW || ImagePt.y>=m_ResultH ) 
		{
			bCalc = false;
			strResult = _T(""); 
		}
		else
		{
			if ( 24 == m_ResultBitCount )
			{
				index = (ImagePt.y*m_ResultStep)+(ImagePt.x*3);
				B = m_ResultPtr[index];
				G = m_ResultPtr[index+1];
				R = m_ResultPtr[index+2];
			}
			else
			{
				index = (ImagePt.y*m_ResultStep)+(ImagePt.x);
				R = G = B = m_ResultPtr[index];
			}			
			strResult.Format(_T("Res(%d, %d)=(%d, %d, %d)"), ImagePt.x, ImagePt.y, R, G, B);
		}
		JetAPI::Point2DToPoint(m_ImageImgPt, ImagePt);
		ImagePt.x = (int)(m_ImageImgPt.x);
		ImagePt.y = (int)(m_ImageImgPt.y);
	}
	else 
	{	bCalc = false; }	
	
	if ( strImage.GetLength() != 0 ) 
	{
		if ( str.GetLength() == 0 ) 
		{	str = strImage; }
		else
		{	
			strTmp = str;
			str.Format(_T("%s, %s"), strTmp, strImage);
		}
	}

	if ( strPattern.GetLength() != 0 ) 
	{
		if ( str.GetLength() == 0 ) 
		{	str = strPattern; }
		else
		{	
			strTmp = str;
			str.Format(_T("%s, %s"), strTmp, strPattern);
		}
	}
	if ( strResult.GetLength() != 0 ) 
	{
		if ( str.GetLength() == 0 ) 
		{	str = strResult; }
		else
		{	
			strTmp = str;
			str.Format(_T("%s, %s"), strTmp, strResult);
		}
	}

	if ( true == bCalc )
	{
		int Sum1 = PatR+PatG+PatB;
		int Sum2 = ImgR+ImgG+ImgB;		
		int nR1=0, nG1=0, nB1=0, nV1=0;
		int nR2=0, nG2=0, nB2=0, nV2=0;
		int dR=0, dG=0, dB=0, dV=0, dMax=0;
		const double ColorRatio = m_ColorRatio;

		if ( Sum1 > 0 ) 
		{
			nV1 = MAX(PatR, PatG);
			nV1 = MAX(nV1, PatB);
			nR1 = (int)(PatR*255/Sum1);
			nG1 = (int)(PatG*255/Sum1);
			nB1 = (int)(PatB*255/Sum1);
		}

		if ( Sum2 > 0 ) 
		{
			nV2 = MAX(ImgR, ImgG);
			nV2 = MAX(nV2, ImgB);
			nR2 = (int)(ImgR*255/Sum2);
			nG2 = (int)(ImgG*255/Sum2);
			nB2 = (int)(ImgB*255/Sum2);
		}		
		
		dR = (int)(abs(nR2-nR1)*ColorRatio);
		dG = (int)(abs(nG2-nG1)*ColorRatio);
		dB = (int)(abs(nB2-nB1)*ColorRatio);
		dV = (int)abs(nV2-nV1);
		dMax = MAX(dR, dG);
		dMax = MAX(dB, dMax);
		dMax = MAX(dV, dMax);		

		strResult.Format(_T("===>>Img(%d, %d, %d, %d), Pat(%d, %d, %d, %d), Dif(%d, %d, %d, %d)=[%d]"), 
			nR2, nG2, nB2, nV2, nR1, nG1, nB1, nV1, dR, dG, dB, dV, dMax);

		if ( str.GetLength() == 0 ) 
		{	str = strResult; }
		else
		{	
			strTmp = str;
			str.Format(_T("%s, %s"), strTmp, strResult);
		}
	}
	CWnd::SetDlgItemText(EditID, str);
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CAlgImageCompareWnd::CheckBtnClickWndID(POINT pt)
{
	UINT WndID = 0;
	if ( ::PtInRect(&m_WndRect, pt) == FALSE ) 
	{	return WndID; }
	
	POINT LocalPt = pt;
	POINT GlobalPt = pt;
	RECT  WndRect={0,0,0,0};
	CWnd *WndPtr = NULL;
	CWnd::ClientToScreen(&GlobalPt);

	WndID = ALGIMGCMP_IMAGE_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{	return WndID; }
	}

	WndID = ALGIMGCMP_PATTERN_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{	return WndID; }
	}

	WndID = ALGIMGCMP_RESULT_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{	return WndID; }
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdateCursorPosToWnds(POINT pt)
{
	UINT WndID = CheckBtnClickWndID(pt);
	if ( 0 == WndID )
	{	return true; }

	POINT LocalPt = pt;
	POINT GlobalPt = pt;
	RECT  WndRect={0,0,0,0};
	CWnd *WndPtr = NULL;
	CWnd::ClientToScreen(&GlobalPt);

	WndID = ALGIMGCMP_IMAGE_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{
			UpdateImageWndPtToOtherWnds(LocalPt);
			UpdateInfoText(ALGIMGCMP_INFO_EDIT);
			RedrawWnd();
			return true;
		}
	}

	WndID = ALGIMGCMP_PATTERN_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{
			UpdatePatternWndPtToOtherWnds(LocalPt);		
			UpdateInfoText(ALGIMGCMP_INFO_EDIT);
			RedrawWnd();
			return true;
		}
	}

	WndID = ALGIMGCMP_RESULT_WND;
	WndPtr = CWnd::GetDlgItem(WndID);
	if ( NULL!=WndPtr && NULL!=WndPtr->GetSafeHwnd() )
	{
		LocalPt = GlobalPt;
		WndPtr->ScreenToClient(&LocalPt);
		WndPtr->GetClientRect(&WndRect);
		if ( ::PtInRect(&WndRect, LocalPt) == TRUE ) 
		{
			UpdateResultWndPtToOtherWnds(LocalPt);
			UpdateInfoText(ALGIMGCMP_INFO_EDIT);
			RedrawWnd();
			return true;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ResetAllPt()
{	
	m_ImageWndPt.x = m_ImageWndPt.y = -1;
	m_ImageImgPt.x = m_ImageImgPt.y = -1;
	m_PatternImgPt.x = m_PatternImgPt.y = -1;
	m_PatternWndPt.x = m_PatternWndPt.y = -1;
	m_ResultImgPt.x = m_ResultImgPt.y = -1;
	m_ResultWndPt.x = m_ResultWndPt.y = -1;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdateImageWndPtToOtherWnds(POINT pt)
{
	ResetAllPt();
	m_ImageWndPt = pt;		
	if ( NULL == m_ImagePtr ) { return false; }
	POINT    nImgPt;
	POINT    nPatToImgOffset;
	TPOINT2D ImgPt=pt;
	TPOINT2D WndPt=pt;
	ImageAPI.MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImgPt);
	m_ImageImgPt.x = (ImgPt.x);
	m_ImageImgPt.y = (ImgPt.y);	
	JetAPI::Point2DToPoint(ImgPt, nImgPt);	
	JetAPI::Point2DToPoint(m_PatToImgOffset, nPatToImgOffset);
	if ( NULL != m_PatternPtr )
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x-m_PatToImgOffset.x;
		TmpImgPt.y = ImgPt.y-m_PatToImgOffset.y;
		TmpImgPt.x = nImgPt.x-nPatToImgOffset.x;
		TmpImgPt.y = nImgPt.y-nPatToImgOffset.y;
		m_PatternImgPt.x = (TmpImgPt.x);
		m_PatternImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_PatternW, m_PatternH, m_PatternWndRect, m_PatternOffset, m_PatternZoom, TmpImgPt, TmpWndPt);
		m_PatternWndPt.x = TmpWndPt.x;
		m_PatternWndPt.y = TmpWndPt.y;
	}	
	if ( NULL != m_ResultPtr )
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x-m_PatToImgOffset.x;
		TmpImgPt.y = ImgPt.y-m_PatToImgOffset.y;
		TmpImgPt.x = nImgPt.x-nPatToImgOffset.x;
		TmpImgPt.y = nImgPt.y-nPatToImgOffset.y;
		m_ResultImgPt.x = (TmpImgPt.x);
		m_ResultImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_ResultW, m_ResultH, m_ResultWndRect, m_ResultOffset, m_ResultZoom, TmpImgPt, TmpWndPt);
		m_ResultWndPt.x = TmpWndPt.x;
		m_ResultWndPt.y = TmpWndPt.y;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdatePatternWndPtToOtherWnds(POINT pt)
{
	ResetAllPt();
	m_PatternWndPt = pt;	
	if ( NULL == m_PatternPtr ) { return false; }
	POINT    nImgPt;
	POINT    nPatToImgOffset;
	TPOINT2D ImgPt=pt;
	TPOINT2D WndPt=pt;
	ImageAPI.MapWndPtToImagePt_DBL(m_PatternW, m_PatternH, m_PatternWndRect, m_PatternOffset, m_PatternZoom, WndPt, ImgPt);
	m_PatternImgPt.x = (ImgPt.x);
	m_PatternImgPt.y = (ImgPt.y);
	JetAPI::Point2DToPoint(ImgPt, nImgPt);
	JetAPI::Point2DToPoint(m_PatToImgOffset, nPatToImgOffset);
	if ( NULL != m_ImagePtr )	
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x+m_PatToImgOffset.x;
		TmpImgPt.y = ImgPt.y+m_PatToImgOffset.y;
		TmpImgPt.x = nImgPt.x+nPatToImgOffset.x;
		TmpImgPt.y = nImgPt.y+nPatToImgOffset.y;
		m_ImageImgPt.x = (TmpImgPt.x);
		m_ImageImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, TmpImgPt, TmpWndPt);
		m_ImageWndPt.x = TmpWndPt.x;
		m_ImageWndPt.y = TmpWndPt.y;
	}	
	if ( NULL != m_ResultPtr )	
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x;
		TmpImgPt.y = ImgPt.y;
		m_ResultImgPt.x = (TmpImgPt.x);
		m_ResultImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_ResultW, m_ResultH, m_ResultWndRect, m_ResultOffset, m_ResultZoom, TmpImgPt, TmpWndPt);
		m_ResultWndPt.x = TmpWndPt.x;
		m_ResultWndPt.y = TmpWndPt.y;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdateResultWndPtToOtherWnds(POINT pt)
{
	ResetAllPt();
	m_ResultWndPt = pt;	
	if ( NULL == m_ResultPtr ) { return false; }
	POINT    nImgPt;
	POINT    nPatToImgOffset;
	TPOINT2D ImgPt=pt;
	TPOINT2D WndPt=pt;
	ImageAPI.MapWndPtToImagePt_DBL(m_ResultW, m_ResultH, m_ResultWndRect, m_ResultOffset, m_ResultZoom, WndPt, ImgPt);
	m_ResultImgPt.x = (ImgPt.x);
	m_ResultImgPt.y = (ImgPt.y);	
	JetAPI::Point2DToPoint(ImgPt, nImgPt);
	JetAPI::Point2DToPoint(m_PatToImgOffset, nPatToImgOffset);
	if ( NULL != m_ImagePtr )	
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x+m_PatToImgOffset.x;
		TmpImgPt.y = ImgPt.y+m_PatToImgOffset.y;
		TmpImgPt.x = nImgPt.x+nPatToImgOffset.x;
		TmpImgPt.y = nImgPt.y+nPatToImgOffset.y;
		m_ImageImgPt.x = (TmpImgPt.x);
		m_ImageImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, TmpImgPt, TmpWndPt);
		m_ImageWndPt.x = TmpWndPt.x;
		m_ImageWndPt.y = TmpWndPt.y;
	}	
	if ( NULL != m_PatternPtr )	
	{
		TPOINT2D TmpImgPt=pt;
		TPOINT2D TmpWndPt=pt;
		TmpImgPt.x = ImgPt.x;
		TmpImgPt.y = ImgPt.y;
		m_PatternImgPt.x = (TmpImgPt.x);
		m_PatternImgPt.y = (TmpImgPt.y);
		ImageAPI.MapImagePtToWndPt_DBL(m_PatternW, m_PatternH, m_PatternWndRect, m_PatternOffset, m_PatternZoom, TmpImgPt, TmpWndPt);
		m_PatternWndPt.x = TmpWndPt.x;
		m_PatternWndPt.y = TmpWndPt.y;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::LoadImage(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }
	CString str;
	bool bIsOK = true;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	const int nAlign = 4;
	const bool bReverse = true;
	bIsOK = ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, bReverse);
	if ( false == bIsOK )
	{
		str = ImageAPI.GetImageApiErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	m_ImageFile = filename;
	DrawImageWndBkDC();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ReleaseImageBuffer()
{
	ClearMatch();
	if ( NULL == m_ImagePtr ) { return true; }
	m_Matched = false;
	m_ImageFile = _T("");
	JetMemory.free_func(m_ImagePtr); 
	m_ImagePtr = NULL;
	m_ImageW = 1024;
	m_ImageH = 1024;	
	m_ImageStep = m_ImageW;
	m_ImageBitCount = 8;
	m_ImageOffset=TPOINT2D();
	m_ImageZoom=1.0;
	m_PatToImgOffset=TPOINT2D();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::SetImageBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	if ( ReleaseImageBuffer() == false ) { return false; }
	m_ImagePtr = Ptr;
	m_ImageW = W;
	m_ImageH = H;	
	m_ImageStep = Step;
	m_ImageBitCount = BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::LoadPattern(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }	
	CString str;
	bool bIsOK = true;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	const int nAlign = 4;
	const bool bReverse = true;
	bIsOK = ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, bReverse);
	if ( false == bIsOK )
	{
		str = ImageAPI.GetImageApiErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	SetPatternBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr);

	IMAGE_SIZE ResultW=ImageW;
	IMAGE_SIZE ResultH=ImageH;	
	IMAGE_SIZE ResultBitCount=8;
	IMAGE_SIZE ResultStep=JetAPI::GetBMPImagePixelsPerLine(ResultW, ResultBitCount, nAlign);
	IMAGE_PTR  ResultPtr=NULL;
	const size_t ResultBufferSize=ImageAPI.CalcBufferSize(ResultStep, ResultH);
	if ( JetMemory.alloc_func(ResultBufferSize, ResultPtr, "CAlgImageCompareWnd::LoadPattern", "ResultPtr") == false )
	{
		ReleaseResultBuffer();
		return false;
	}
	else
	{
		::memset(ResultPtr, 0x00, sizeof(IMAGE_DATA)*(ResultBufferSize));
		SetResultBuffer(ResultW, ResultH, ResultStep, ResultBitCount, ResultPtr);	
	}
	m_PatternFile = filename;
	DrawPatternWndBkDC();
	DrawResultWndBkDC();
	RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ReleasePatternBuffer()
{
	ClearMatch();
	if ( NULL == m_PatternPtr ) { return true; }
	m_Matched = false;
	m_PatternFile = _T("");
	JetMemory.free_func(m_PatternPtr); 
	m_PatternPtr = NULL;
	m_PatternW = 1024;
	m_PatternH = 1024;	
	m_PatternStep = m_PatternW;
	m_PatternBitCount = 8;
	m_PatternOffset=TPOINT2D();
	m_PatternZoom=1.0;
	m_PatToImgOffset=TPOINT2D();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::SetPatternBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	if ( ReleasePatternBuffer() == false ) { return false; }
	m_PatternPtr = Ptr;
	m_PatternW = W;
	m_PatternH = H;	
	m_PatternStep = Step;
	m_PatternBitCount = BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ReleaseResultBuffer()
{
	if ( NULL == m_ResultPtr ) { return true; }	
	JetMemory.free_func(m_ResultPtr); 
	m_ResultPtr = NULL;
	m_ResultW = 1024;
	m_ResultH = 1024;	
	m_ResultStep = m_ResultW;
	m_ResultBitCount = 8;
	m_ResultOffset=TPOINT2D();
	m_ResultZoom=1.0;	
	m_ResultBlobList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::SetResultBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	if ( ReleaseResultBuffer() == false ) { return false; }
	m_ResultPtr = Ptr;
	m_ResultW = W;
	m_ResultH = H;	
	m_ResultStep = Step;
	m_ResultBitCount = BitCount;	
	return true;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::RedrawWnd()
{
	DrawImageWnd();
	DrawPatternWnd();
	DrawResultWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawImageWnd()
{
	CClientDC dc(&m_ImageWnd);	
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC || NULL==hMemDC2 ) { return ; }

	RECT WndRect = m_ImageWndRect;
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	POINT Pos;
	SIZE CorsorSz={3,3};
	RECT CursorPosRect={0,0,0,0};	
	JetAPI::Point2DToPoint(m_ImageWndPt, Pos);
	if ( ::PtInRect(&WndRect, Pos) == TRUE )
	{
		const int len=1;
		CursorPosRect.left = Pos.x-len;
		CursorPosRect.top = Pos.y-len;
		CursorPosRect.right = Pos.x+len;
		CursorPosRect.bottom = Pos.y+len;

		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, CursorPosRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}

	int i=0;	
	RECT  ResImgRect={0,0,0,0};
	RECT  ResWndRect={0,0,0,0};
	TPOINT2D ImgPt1, WndPt1, ImgPt2, WndPt2;
	const int NBlobs = (int)(m_ResultBlobList.size());
	const int nPatToImgOffsetX = JetAPI::Floor(m_PatToImgOffset.x);
	const int nPatToImgOffsetY = JetAPI::Floor(m_PatToImgOffset.y);	
	for ( i=0; i<NBlobs; i++ )
	{
		ResImgRect = m_ResultBlobList[i];
		ImgPt1.x = ResImgRect.left+nPatToImgOffsetX;
		ImgPt1.y = ResImgRect.top+nPatToImgOffsetY;
		ImgPt2.x = ResImgRect.right+nPatToImgOffsetX;
		ImgPt2.y = ResImgRect.bottom+nPatToImgOffsetY;
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImgPt1, WndPt1);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImgPt2, WndPt2);
		ResWndRect.left = (int)(MIN(WndPt1.x, WndPt2.x));
		ResWndRect.top  = (int)(MIN(WndPt1.y, WndPt2.y));
		ResWndRect.right = (int)(MAX(WndPt1.x, WndPt2.x));
		ResWndRect.bottom  = (int)(MAX(WndPt1.y, WndPt2.y));

		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, ResWndRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawImageWndBkDC()
{
	HDC hDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL==hDC ) { return ; }
	COLORREF BKColor = m_WndBKColor;
	HBRUSH hBrush = ::CreateSolidBrush(BKColor);
	if ( NULL != hBrush ) 
	{
		::FillRect(hDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush);
		hBrush= NULL;
	}
	DrawImage(hDC);
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawImage(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	if ( NULL == m_ImagePtr ) { return; }
	RECT WndRect = m_ImageWndRect;
	COLORREF BKColor = m_WndBKColor;
	IMAGE_SIZE ImageW=m_ImageW;
	IMAGE_SIZE ImageH=m_ImageH;	
	IMAGE_SIZE ImageStep=m_ImageStep;
	IMAGE_SIZE BitCount=m_ImageBitCount;
	IMAGE_PTR  ImagePtr=m_ImagePtr;	
	BOOL bEnhance = CWnd::IsDlgButtonChecked(ALGIMGCMP_ENHANCE_IMAGE_CHK);
	if ( FALSE == bEnhance )
	{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_ImageOffset, m_ImageZoom, BKColor); }
	else
	{
		IMAGE_PTR Buffer=NULL;
		const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, Buffer, "CAlgImageCompareWnd::DrawImage", "Buffer") == false ) 
		{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_ImageOffset, m_ImageZoom, BKColor); }
		else
		{	
			AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Buffer);
			ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, Buffer, WndRect, m_ImageOffset, m_ImageZoom, BKColor);	
			JetMemory.free_func(Buffer);
		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawPatternWnd()
{
	CClientDC dc(&m_PatternWnd);	
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_PatternWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_PatternWndMemDC2.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC ) { return ; }

	RECT WndRect = m_PatternWndRect;
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	POINT Pos;
	SIZE CorsorSz={3,3};
	RECT CursorPosRect={0,0,0,0};	
	JetAPI::Point2DToPoint(m_PatternWndPt, Pos);
	if ( ::PtInRect(&WndRect, Pos) == TRUE )
	{
		const int len=1;
		CursorPosRect.left = Pos.x-len;
		CursorPosRect.top = Pos.y-len;
		CursorPosRect.right = Pos.x+len;
		CursorPosRect.bottom = Pos.y+len;
		
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, CursorPosRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}

	int i=0;	
	RECT  ResImgRect={0,0,0,0};
	RECT  ResWndRect={0,0,0,0};
	TPOINT2D ImgPt1, WndPt1, ImgPt2, WndPt2;
	const int NBlobs = (int)(m_ResultBlobList.size());
	for ( i=0; i<NBlobs; i++ )
	{
		ResImgRect = m_ResultBlobList[i];
		ImgPt1.x = ResImgRect.left;
		ImgPt1.y = ResImgRect.top;
		ImgPt2.x = ResImgRect.right;
		ImgPt2.y = ResImgRect.bottom;
		ImageAPI.MapImagePtToWndPt_DBL(m_PatternW, m_PatternH, m_PatternWndRect, m_PatternOffset, m_PatternZoom, ImgPt1, WndPt1);
		ImageAPI.MapImagePtToWndPt_DBL(m_PatternW, m_PatternH, m_PatternWndRect, m_PatternOffset, m_PatternZoom, ImgPt2, WndPt2);
		ResWndRect.left = (int)(MIN(WndPt1.x, WndPt2.x));
		ResWndRect.top  = (int)(MIN(WndPt1.y, WndPt2.y));
		ResWndRect.right = (int)(MAX(WndPt1.x, WndPt2.x));
		ResWndRect.bottom  = (int)(MAX(WndPt1.y, WndPt2.y));

		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, ResWndRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawPatternWndBkDC()
{
	HDC hDC = m_PatternWndMemDC.GetSafeHdc();
	if ( NULL==hDC ) { return ; }
	COLORREF BKColor = m_WndBKColor;
	HBRUSH hBrush = ::CreateSolidBrush(BKColor);
	if ( NULL != hBrush ) 
	{
		::FillRect(hDC, &m_PatternWndRect, hBrush);
		::DeleteObject(hBrush);
		hBrush= NULL;
	}
	DrawPattern(hDC);
	return;	
}
//-------------------------------------------------------------------------------------//	
void CAlgImageCompareWnd::DrawPattern(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	if ( NULL == m_PatternPtr ) { return; }
	RECT WndRect = m_PatternWndRect;
	COLORREF BKColor = m_WndBKColor;
	IMAGE_SIZE ImageW=m_PatternW;
	IMAGE_SIZE ImageH=m_PatternH;	
	IMAGE_SIZE ImageStep=m_PatternStep;
	IMAGE_SIZE BitCount=m_PatternBitCount;
	IMAGE_PTR  ImagePtr=m_PatternPtr;	
	BOOL bEnhance = CWnd::IsDlgButtonChecked(ALGIMGCMP_ENHANCE_IMAGE_CHK);
	if ( FALSE == bEnhance )
	{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_PatternOffset, m_PatternZoom, BKColor); }
	else
	{
		IMAGE_PTR Buffer=NULL;
		const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, Buffer, "CAlgImageCompareWnd::DrawImage", "Buffer") == false ) 
		{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_PatternOffset, m_PatternZoom, BKColor); }
		else
		{	
			AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Buffer);
			ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, Buffer, WndRect, m_PatternOffset, m_PatternZoom, BKColor);	
			JetMemory.free_func(Buffer);
		}
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawResultWnd()
{
	CClientDC dc(&m_ResultWnd);	
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ResultWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ResultWndMemDC2.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC || NULL==hMemDC2) { return ; }

	RECT WndRect = m_ResultWndRect;
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

	POINT Pos;
	SIZE CorsorSz={3,3};
	RECT CursorPosRect={0,0,0,0};	
	JetAPI::Point2DToPoint(m_ResultWndPt, Pos);
	if ( ::PtInRect(&WndRect, Pos) == TRUE )
	{
		const int len=1;
		CursorPosRect.left = Pos.x-len;
		CursorPosRect.top = Pos.y-len;
		CursorPosRect.right = Pos.x+len;
		CursorPosRect.bottom = Pos.y+len;
		
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, CursorPosRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}

	int i=0;	
	RECT  ResImgRect={0,0,0,0};
	RECT  ResWndRect={0,0,0,0};
	TPOINT2D ImgPt1, WndPt1, ImgPt2, WndPt2;
	const int NBlobs = (int)(m_ResultBlobList.size());
	for ( i=0; i<NBlobs; i++ )
	{
		ResImgRect = m_ResultBlobList[i];
		ImgPt1.x = ResImgRect.left;
		ImgPt1.y = ResImgRect.top;
		ImgPt2.x = ResImgRect.right;
		ImgPt2.y = ResImgRect.bottom;
		ImageAPI.MapImagePtToWndPt_DBL(m_ResultW, m_ResultH, m_ResultWndRect, m_ResultOffset, m_ResultZoom, ImgPt1, WndPt1);
		ImageAPI.MapImagePtToWndPt_DBL(m_ResultW, m_ResultH, m_ResultWndRect, m_ResultOffset, m_ResultZoom, ImgPt2, WndPt2);
		ResWndRect.left = (int)(MIN(WndPt1.x, WndPt2.x));
		ResWndRect.top  = (int)(MIN(WndPt1.y, WndPt2.y));
		ResWndRect.right = (int)(MAX(WndPt1.x, WndPt2.x));
		ResWndRect.bottom  = (int)(MAX(WndPt1.y, WndPt2.y));

		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0xFFFF00);
		HPEN hOldPen = (HPEN)::SelectObject(hMemDC2, hPen);
		ImageAPI.DrawRectLine(hMemDC2, ResWndRect);
		::SelectObject(hMemDC2, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );
	return;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawResultWndBkDC()
{
	HDC hDC = m_ResultWndMemDC.GetSafeHdc();
	if ( NULL==hDC ) { return ; }
	COLORREF BKColor = m_WndBKColor;
	HBRUSH hBrush = ::CreateSolidBrush(BKColor);
	if ( NULL != hBrush ) 
	{
		::FillRect(hDC, &m_ResultWndRect, hBrush);
		::DeleteObject(hBrush);
		hBrush= NULL;
	}
	DrawResult(hDC);
	return;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::DrawResult(HDC hDC)
{
	if ( NULL == hDC ) { return; }
	if ( NULL == m_ResultPtr ) { return; }
	RECT WndRect = m_ResultWndRect;
	COLORREF BKColor = m_WndBKColor;
	IMAGE_SIZE ImageW=m_ResultW;
	IMAGE_SIZE ImageH=m_ResultH;	
	IMAGE_SIZE ImageStep=m_ResultStep;
	IMAGE_SIZE BitCount=m_ResultBitCount;
	IMAGE_PTR  ImagePtr=m_ResultPtr;	
	BOOL bEnhance = CWnd::IsDlgButtonChecked(ALGIMGCMP_ENHANCE_IMAGE_CHK);
	if ( FALSE == bEnhance )
	{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_ResultOffset, m_ResultZoom, BKColor);	 }
	else
	{
		IMAGE_PTR Buffer=NULL;
		const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, Buffer, "CAlgImageCompareWnd::DrawImage", "Buffer") == false ) 
		{	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, WndRect, m_ResultOffset, m_ResultZoom, BKColor);	 }
		else
		{	
			AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Buffer);
			ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, Buffer, WndRect, m_ResultOffset, m_ResultZoom, BKColor);	
			JetMemory.free_func(Buffer);
		}
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnLoadImageBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; } 

	CString filename = dialog.GetPathName();
	CWnd::SetWindowText(filename);
	LoadImage(filename);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnLoadPatternBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; } 

	CString filename = dialog.GetPathName();
	LoadPattern(filename);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnMatchBtn() 
{
	// TODO: Add your control notification handler code here
	ExecMatch();
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ClearMatch()
{
	m_Matched = false;
	m_PatToImgOffset.x = m_PatToImgOffset.y = 0;
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_X_EDIT, _T("0"));	
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_Y_EDIT, _T("0"));
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_X_EDIT, _T("0"));	
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_Y_EDIT, _T("0"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ExecMatch()
{
	CString str;
	ClearMatch();
	const char fnName[] = "CAlgImageCompareWnd::ExecMatch";
	if ( NULL==m_ImagePtr || NULL==m_PatternPtr )
	{
		str = _T("Error, No Image Ptr");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	if ( m_ImageW<m_PatternW || m_ImageH<m_PatternH )
	{
		str = _T("Error, pattern size is smaller than image size");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	if ( m_ImageBitCount != m_PatternBitCount )
	{
		str = _T("Error, Image BitCount is not equal to Pattern's");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	CJetMatch Match;	
	CString TmpImageFile;
	CString TmpPatternFile;
	CString Folder = AOIDataCollect.GetAOITempDirectory();
	char  ImgFilename[MAX_JET_PATH]="";
	char  PatFilename[MAX_JET_PATH]="";	
	char  MatchFilename[MAX_JET_PATH]="";
	const bool bRobustness = true;
	const int nMinReduceArea = 4096;
	const int nFinalReduction = 1;//加速用
	const bool bUseInterpolate=true;
	const size_t ImageBufferSize=ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();

	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		str = Match.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);

	TmpImageFile.Format(_T("%s\\%s"), Folder, _T("ImgCmpTempImg.PNG"));
	TmpPatternFile.Format(_T("%s\\%s"), Folder, _T("ImgCmpTempPat.PNG"));
	::DeleteFile(TmpImageFile);
	::DeleteFile(TmpPatternFile);
	::Sleep(0);
	::CopyFile(m_ImageFile, TmpImageFile, FALSE);
	::CopyFile(m_PatternFile, TmpPatternFile, FALSE);

	JetAPI::TCHAR2char(TmpImageFile, ImgFilename, MAX_JET_PATH);	
	JetAPI::TCHAR2char(TmpPatternFile, PatFilename, MAX_JET_PATH);		

	const int  SmoothSize = CWnd::GetDlgItemInt(ALGIMGCMP_SMOOTH_SIZE_EDIT);
	const bool bUseSmooth = false;//(bool)(CWnd::IsDlgButtonChecked(ALGIMGCMP_SMOOTH_SIZE_CHK));
	if ( true == bUseSmooth )
	{		
		IMAGE_PTR SmoothPtr=NULL;
		const int nSmoothSize = ((SmoothSize/2)*2)+1;		
		if ( JetMemory.alloc_func(ImageBufferSize, SmoothPtr, fnName, "SmoothPtr") == true )		
		{			
			if ( ImageAPI.SmoothImage3(m_PatternW, m_PatternH, m_PatternStep, m_PatternBitCount, m_PatternPtr, nSmoothSize, SmoothPtr) == true )
			{	ImageAPI.SaveImage(TmpPatternFile, m_PatternW, m_PatternH, m_PatternStep, m_PatternBitCount, SmoothPtr, true);	}
		
			if ( ImageAPI.SmoothImage3(m_ImageW, m_ImageH, m_ImageStep, m_ImageBitCount, m_ImagePtr, nSmoothSize, SmoothPtr) == true )
			{	ImageAPI.SaveImage(TmpImageFile, m_ImageW, m_ImageH, m_ImageStep, m_ImageBitCount, SmoothPtr, true);	}
			JetMemory.free_func(SmoothPtr);
		}
	}

	if ( Match.LearnPattern(PatFilename, true) == false )
	//if ( Match.LearnPattern(m_PatternW, m_PatternH, m_PatternStep, m_PatternBitCount, m_PatternPtr, true) == false )
	{
		str = Match.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	JET_MATCH_LIB_TYPE MatchType = Match.GetMatchLibType();
	if ( JET_MATCH_LIB_MIM == MatchType )
	{	str.Format(_T("%s\\%s"), m_TempFolder, _T("ImgCmp.imm"));	}
	if ( JET_MATCH_LIB_EVS == MatchType )
	{	str.Format(_T("%s\\%s"), m_TempFolder, _T("ImgCmp.mch"));	}
	if ( JET_MATCH_LIB_NONE != MatchType )
	{
		JetAPI::TCHAR2char(str, MatchFilename, MAX_JET_PATH); 	
		Match.SaveModel(MatchFilename);
	}
	const bool bUseScale = CWnd::IsDlgButtonChecked(ALGIMGCMP_PATTERN_USE_SCALE_CHK);
	Match.SetInterpolate(bUseInterpolate);	
	Match.SetMinScore(-1);//fMinScore
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if ( true == bUseScale )
	{
		const float ScaleRange=0.1;
		const float ScaleMin = 1.0f-ScaleRange;
		const float ScaleMax = 1.0f+ScaleRange;
		Match.SetUseScale(true);
		Match.SetMinScale(ScaleMin);
		Match.SetMinScaleX(ScaleMin);
		Match.SetMinScaleY(ScaleMin);
		Match.SetMaxScale(ScaleMax);
		Match.SetMaxScaleX(ScaleMax);
		Match.SetMaxScaleY(ScaleMax);
	}
	Match.SetUseAngle(false);
	Match.SetMaxAngle(0);
	Match.SetMinAngle(0);
	//if ( Match.Match(m_ImageW, m_ImageH, m_ImageStep, m_ImageBitCount, m_ImagePtr, true) == false )
	if ( Match.Match(ImgFilename, true) == false )
	{
		str = Match.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	const int idx = 0;
	const int NResults = Match.GetNumPositions();	
	const double ResultS = Match.GetResultScore(idx)*100.0;
	const double ResultX = Match.GetResultPosX(idx);
	const double ResultY = Match.GetResultPosY(idx);
	const double ResultA = Match.GetResultAngle(idx);		
	const double ResultSX = Match.GetResultScaleX(idx);
	const double ResultSY = Match.GetResultScaleY(idx);
	const double PatternW2 = m_PatternW*0.5;
	const double PatternH2 = m_PatternH*0.5;
	
	m_Matched = true;
	str.Format(_T("%.2f"), ResultX);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_X_EDIT, str);
	str.Format(_T("%.2f"), ResultY);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_RES_Y_EDIT, str);

	str.Format(_T("Score(%.2f), Result(%.2f, %.2f), Skew(%.2f), Scale(%.2f, %.2f), Offset(%.2f, %.2f)"), ResultS, ResultX, ResultY, ResultA, ResultSX, ResultSY, m_PatToImgOffset.x, m_PatToImgOffset.y);
	CWnd::SetDlgItemText(ALGIMGCMP_MATCH_INFO_EDIT, str);

	UpdatePatToImgOffset(ResultX, ResultY);	
	/*
	const double ViewOffsetX = m_PatToImgOffset.x/m_ResultZoom;
	const double ViewOffsetY = m_PatToImgOffset.y/m_ResultZoom;
	m_PatternOffset.x = m_ImageOffset.x-ViewOffsetX;
	m_PatternOffset.y = m_ImageOffset.y;//-m_PatToImgOffset.y;
	m_ResultOffset.x = m_ImageOffset.x-ViewOffsetX;
	m_ResultOffset.y = m_ImageOffset.y;//-m_PatToImgOffset.y;
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::UpdatePatToImgOffset(double PatResX, double PatResY)
{
	CString str;
	const double ResultX = PatResX;
	const double ResultY = PatResY;
	const double PatternW2 = m_PatternW*0.5;
	const double PatternH2 = m_PatternH*0.5;

	//const int nResultX = JetAPI::Floor(ResultX);
	//const int nResultY = JetAPI::Floor(ResultY);
	const int nResultX = (int)(ResultX+0.5);
	const int nResultY = (int)(ResultY+0.5);
	const int nPatternW2 = m_PatternW/2;
	const int nPatternH2 = m_PatternH/2;

	m_PatToImgOffset.x = ResultX-PatternW2;
	m_PatToImgOffset.y = ResultY-PatternH2;

	m_PatToImgOffset.x = nResultX-nPatternW2;
	m_PatToImgOffset.y = nResultY-nPatternH2;

	str.Format(_T("%.2f"), m_PatToImgOffset.x);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_X_EDIT, str);
	str.Format(_T("%.2f"), m_PatToImgOffset.y);
	CWnd::SetDlgItemText(ALGIMGCMP_PATTERN_POS_Y_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnResetViewBtn() 
{
	// TODO: Add your control notification handler code here	
	m_ImageOffset=TPOINT2D();
	m_ImageZoom=1.0;
	
	m_PatternOffset=TPOINT2D();;
	m_PatternZoom=1.0;

	m_ResultOffset=TPOINT2D();;
	m_ResultZoom=1.0;

	DrawImageWndBkDC();
	DrawPatternWndBkDC();
	DrawResultWndBkDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnPatternPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString strX;
	CString strY;
	CWnd::GetDlgItemText(ALGIMGCMP_PATTERN_POS_X_EDIT, strX);	
	CWnd::GetDlgItemText(ALGIMGCMP_PATTERN_POS_Y_EDIT, strY);
	m_PatToImgOffset.x = ::_ttof(strX);
	m_PatToImgOffset.y = ::_ttof(strY);
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnPatternCalcBtn() 
{
	// TODO: Add your control notification handler code here
	if ( false == m_Matched )
	{
		if ( ExecMatch() == false )
		{	return; }
	}
	ExecCalculate();
	DrawResultWndBkDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
CString CAlgImageCompareWnd::GetShowModeImageFile(int ShowMode)
{
	CString filename;
	CString LocalFolder;
	LocalFolder = m_TempFolder;
	::CreateDirectory(LocalFolder, NULL);	
	if ( IMGCMP_SHOW_RAW == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Raw"));	}	
	if ( IMGCMP_SHOW_EDGE == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Edge"));	}	
	if ( IMGCMP_SHOW_REMOVE_EDGE == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("RemoveEdge"));	}		
	if ( IMGCMP_SHOW_GAUSSIAN == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Gaussian"));	}
	if ( IMGCMP_SHOW_BINARY == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Binary"));	}
	if ( IMGCMP_SHOW_DILATE == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Dilate"));	}
	if ( IMGCMP_SHOW_OPEN == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Open"));	}
	if ( IMGCMP_SHOW_CLOSE == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Close"));	}
	if ( IMGCMP_SHOW_BLOB == ShowMode )
	{	filename.Format(_T("%s\\%d_%s.PNG"), LocalFolder, ShowMode, _T("Blob"));	}	
	return filename;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageCompareWnd::ExecCalculate()
{
	CString str;
	m_ResultBlobList.clear();
	::CreateDirectory(m_TempFolder, NULL);
	RECT   CalcRect={0,0,0,0};
	const int CalcMargin = 8;
	const char fnName[] = "CAlgImageCompareWnd::ExecCalculate()";
	CalcRect.left = CalcMargin;
	CalcRect.top  = CalcMargin;
	CalcRect.right= m_PatternW-CalcMargin;
	CalcRect.bottom= m_PatternH-CalcMargin;
	if ( NULL==m_ImagePtr || NULL==m_PatternPtr || NULL==m_ResultPtr )
	{
		str = _T("Error, No Image Ptr");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	if ( m_ImageW<m_PatternW || m_ImageH<m_PatternH )
	{
		str = _T("Error, pattern size is smaller than image size");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	if ( m_ImageBitCount != m_PatternBitCount )
	{
		str = _T("Error, Image BitCount is not equal to Pattern's");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	const int PatOffsetX = JetAPI::Floor(m_PatToImgOffset.x);
	const int PatOffsetY = JetAPI::Floor(m_PatToImgOffset.y);
	const int RectMinX = CalcRect.left+PatOffsetX;
	const int RectMinY = CalcRect.top+PatOffsetY;
	const int RectMaxX = CalcRect.right+PatOffsetX;
	const int RectMaxY = CalcRect.bottom+PatOffsetY;
	if ( RectMinX<0 || RectMinY<0 || m_ImageW<RectMaxX || m_ImageH<RectMaxY )
	{
		str = _T("Error, pattern match is out of image size");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	TPOINT2D ImageRes;
	int    DR=0, DG=0, DB=0, DV=0, Dif=0, DColor=0;
	int    nR1=0, nG1=0, nB1=0, nV1=0, nSum1=0;
	int    nR2=0, nG2=0, nB2=0, nV2=0, nSum2=0;		
	int    Threshold  =  0;	
	double BaseRatio=1.0;
	double DarkRatio=1.0;
	double ColorRatio = m_ColorRatio;//2.0/BaseRatio;
	const int  nDarkLevel = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_DARK_LEVEL_EDIT)*3;
	const int  nLightLevel = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_BRIGHT_LEVEL_EDIT)*3;
	const int  nTolerance = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_TOLERANCE_EDIT);
	const int  nResScale = CWnd::GetDlgItemInt(ALGIMGCMP_RESOLUTION_SCALE_EDIT);
	
	CWnd::GetDlgItemText(ALGIMGCMP_RESOLUTION_Y_EDIT, str);
	ImageRes.x = ::_ttof(str);
	CWnd::GetDlgItemText(ALGIMGCMP_RESOLUTION_X_EDIT, str);	
	ImageRes.y = ::_ttof(str);
	ImageRes.x = ImageRes.x*nResScale;
	ImageRes.y = ImageRes.y*nResScale;
	
	Threshold = nTolerance;	
	

	//RGBV Compare		
	int nDMax = 0;
	size_t i=0, j=0, k=0;
	size_t PatIdx=0, ImgIdx=0, ResIdx=0;
	IMAGE_PTR PatBuffer=NULL;
	IMAGE_PTR ImgBuffer=NULL;
	IMAGE_PTR RawBuffer1=NULL;
	IMAGE_PTR RawBuffer2=NULL;
	IMAGE_PTR EdgeBuffer=NULL;
	const size_t ImageBufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	const size_t PatternBufferSize = ImageAPI.CalcBufferSize(m_PatternStep, m_PatternH);
	const size_t RawBufferSize = ImageAPI.CalcBufferSize(m_ResultStep, m_ResultH);
	if ( JetMemory.alloc_func(PatternBufferSize, PatBuffer, fnName, "PatBuffer") == false ||
		 JetMemory.alloc_func(ImageBufferSize, ImgBuffer, fnName, "ImgBuffer") == false ||
		 JetMemory.alloc_func(RawBufferSize, RawBuffer1, fnName, "RawBuffer1") == false ||
		 JetMemory.alloc_func(RawBufferSize, RawBuffer2, fnName, "RawBuffer2") == false ||
		 JetMemory.alloc_func(RawBufferSize, EdgeBuffer, fnName, "EdgeBuffer") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);
		JetMemory.free_func(PatBuffer);
		JetMemory.free_func(ImgBuffer);
		JetMemory.free_func(RawBuffer1);
		JetMemory.free_func(RawBuffer2);
		JetMemory.free_func(EdgeBuffer);		
		return false;
	}

	IMAGE_PTR ImgPtr = m_ImagePtr;
	const IMAGE_SIZE ImageW = (int)(m_ImageW);
	const IMAGE_SIZE ImageH = (int)(m_ImageH);
	const IMAGE_SIZE ImageStep = (int)(m_ImageStep);	

	IMAGE_PTR PatPtr = m_PatternPtr;
	const IMAGE_SIZE PatternW = (int)(m_PatternW);
	const IMAGE_SIZE PatternH = (int)(m_PatternH);
	const IMAGE_SIZE PatternStep = (int)(m_PatternStep);
	const IMAGE_SIZE BitCount = (int)(m_PatternBitCount);		

	IMAGE_PTR ResultPtr = m_ResultPtr;
	const IMAGE_SIZE ResultW = (int)(m_ResultW);
	const IMAGE_SIZE ResultH = (int)(m_ResultH);
	const IMAGE_SIZE ResultStep = (int)(m_ResultStep);
	const IMAGE_SIZE ResultBitCount = (int)(m_ResultBitCount);

	IMGCMP_SHOW_MODE       CurrentMode;
	const IMGCMP_SHOW_MODE ShowMode = (IMGCMP_SHOW_MODE)(JetAPI::GetComboxCurSelData(m_ShowModeCombox));
	
	::memset(ResultPtr, 0x00, sizeof(IMAGE_DATA)*RawBufferSize);
	::memset(RawBuffer1, 0x00, sizeof(IMAGE_DATA)*RawBufferSize);
	::memset(RawBuffer2, 0x00, sizeof(IMAGE_DATA)*RawBufferSize);
	::memset(EdgeBuffer, 0x00, sizeof(IMAGE_DATA)*RawBufferSize);	
	
	const int  SmoothSize = CWnd::GetDlgItemInt(ALGIMGCMP_SMOOTH_SIZE_EDIT);
	const bool bUseSmooth = (bool)(CWnd::IsDlgButtonChecked(ALGIMGCMP_SMOOTH_SIZE_CHK));
	if ( true == bUseSmooth )
	{		
		const int nSmoothSize = ((SmoothSize/2)*2)+1;		
		if ( ImageAPI.SmoothImage3(PatternW, PatternH, PatternStep, BitCount, PatPtr, nSmoothSize, PatBuffer) == false ||
			 ImageAPI.SmoothImage3(ImageW, ImageH, ImageStep, BitCount, ImgPtr, nSmoothSize, ImgBuffer) == false )
		{	
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(PatBuffer);
			JetMemory.free_func(ImgBuffer);
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;
		}		
	}
	else
	{
		::memcpy(PatBuffer, PatPtr, sizeof(IMAGE_DATA)*PatternBufferSize);
		::memcpy(ImgBuffer, ImgPtr, sizeof(IMAGE_DATA)*ImageBufferSize);		
	}
	
	int TempX=0, TempY=0;
	for ( i=CalcRect.top; i<CalcRect.bottom; i++ )
	{
		TempY = (int)(i);
		for ( j=CalcRect.left; j<CalcRect.right; j++ )
		{
			TempX = (int)(j);
			ResIdx = i*m_ResultStep+j;			

		#ifdef _DEBUG
			if ( 524==TempX && 376==TempY )
			{
				TempX = TempX;
			}
		#endif//_DEBUG
			if ( 24 == BitCount )
			{
				PatIdx = (i*PatternStep)+(j*3);
				ImgIdx = ((i+PatOffsetY)*ImageStep)+(j+PatOffsetX)*3;

				nB1 = PatBuffer[PatIdx];
				nG1 = PatBuffer[PatIdx+1];
				nR1 = PatBuffer[PatIdx+2];
				nV1 = 0;

				nB2 = ImgBuffer[ImgIdx];
				nG2 = ImgBuffer[ImgIdx+1];
				nR2 = ImgBuffer[ImgIdx+2];	
				nV2 = 0;					

				nSum1 = nR1+nG1+nB1;
				nSum2 = nR2+nG2+nB2;
				if ( nSum1 > 0 ) 
				{	
					nV1 = nSum1/3;//取平均
					//nV1 = MAX(nR1, nG1);//取最亮
					//nV1 = MAX(nV1, nB1);
					nR1=(int)(nR1*255/nSum1);
					nG1=(int)(nG1*255/nSum1);
					nB1=(int)(nB1*255/nSum1);						
				}
				if ( nSum2 > 0 ) 
				{	
					nV2 = nSum2/3;//取平均
					//nV2 = MAX(nR2, nG2);//取最亮
					//nV2 = MAX(nV2, nB2);
					nR2=(int)(nR2*255/nSum2);
					nG2=(int)(nG2*255/nSum2);
					nB2=(int)(nB2*255/nSum2);						
				}

			#ifdef _DEBUG
				if ( nSum2<20 && nSum1>150 )				
				{
					nV2 = nV2;
				}
			#endif//_DEBUG

				if ( nSum1<nDarkLevel && nSum2<nDarkLevel )						
				{	
					DR  = 0;
					DG  = 0;
					DB  = 0;
					DV  = (int)(::abs(nV1-nV2)*DarkRatio);		
					nDMax = DV;
					Threshold = nDarkLevel/2;	
				}
				else if ( nSum1>nLightLevel && nSum2>nLightLevel )
				{	
					DR  = 0;
					DG  = 0;
					DB  = 0;
					DV  = (int)(::abs(nV1-nV2)*DarkRatio);		
					nDMax = DV;
					Threshold = nDarkLevel/2;	
				}
				else
				{	
					DR  = (int)(::abs(nR1-nR2)*ColorRatio);
					DG  = (int)(::abs(nG1-nG2)*ColorRatio);
					DB  = (int)(::abs(nB1-nB2)*ColorRatio);
					DV  = (int)(::abs(nV1-nV2));
					DColor=(DR+DG+DB)/3;
					nDMax = MAX(DR, DG);
					nDMax = MAX(DB, nDMax);
					nDMax = MAX(DV, nDMax);		

					//nDMax = MAX(DV, DColor);
					Threshold = nTolerance; 
				}
				if ( nDMax > 255 )
				{	nDMax = 255; }

				RawBuffer1[ResIdx] = (unsigned char)(nDMax);
				//if ( DR>Threshold || DG>Threshold || DB>Threshold || DV>Threshold )
				//{	RawMaskPtrL[ResIdx] = 255; }

				//Dif = MAX(DR, DG);
				//Dif = MAX(Dif, DB);
				//TestMaskPtr[ResIdx] = Dif;
			}
			else
			{
				PatIdx = (i*PatternStep)+(j);
				ImgIdx = ((i+PatOffsetY)*ImageStep)+(j+PatOffsetX);

				nV1 = PatBuffer[PatIdx];
				nV2 = ImgBuffer[ImgIdx];

				Dif = ::abs(nV1-nV2);
				if ( Dif > 255 ) { Dif = 255; }
				RawBuffer1[ResIdx] = Dif;
				//if ( Dif > nTolerance )
				//{	RawMaskPtrL[ResIdx] = 255; }
				
				//TestMaskPtr[ResIdx] = Dif;
			}
		}
	}
	JetMemory.free_func(PatBuffer);
	JetMemory.free_func(ImgBuffer);

	CurrentMode = IMGCMP_SHOW_RAW;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}	

	//Remove Edge Line
	const int nEdgeLevel=8;
	const bool bRemoveEdge=(bool)(CWnd::IsDlgButtonChecked(ALGIMGCMP_REMOVE_EDGE_CHK));
	if ( true == bRemoveEdge )
	{		
		//if ( ImageAPI.SobelGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, EdgeBuffer) == false )
		//const int MorphGradient=3;//2階為分
		//if ( ImageAPI.MorphGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, MORPH_GRADIENT, MORPH_SHAPE_RECT, MorphGradient, 1, RawBuffer2) == false )
		
		const int EdgeErode=3;//侵蝕-Erode
		if ( ImageAPI.ErodeGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, EdgeErode, 1, RawBuffer2) == false )
		{
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;	
		}		
		for ( i=0; i<RawBufferSize; i++ )
		{
			if ( RawBuffer1[i] < RawBuffer2[i] ) 
			{
				EdgeBuffer[i] = 0;
				continue;
			}
			EdgeBuffer[i] = RawBuffer1[i]-RawBuffer2[i];
		}
		/*
		const int EdgeDilate=3;//膨脹-Dilate
		if ( ImageAPI.DilateGrayImage3(ResultW, ResultH, ResultStep, EdgeBuffer, EdgeDilate, 1, RawBuffer2) == false )
		{	
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;
		}
		::memcpy(EdgeBuffer, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);
		/*
		for ( i=0; i<RawBufferSize; i++ )
		{
			if ( RawBuffer2[i] > nEdgeLevel ) 
			{
				EdgeBuffer[i] = 255;
				continue;
			}
			EdgeBuffer[i] = 0;
		}	
		*/		
		//高斯平滑		
		int EdgeGaussian=CWnd::GetDlgItemInt(ALGIMGCMP_REMOVE_EDGE_GAUS_EDIT);
		if ( EdgeGaussian > 0 ) 
		{
			EdgeGaussian = ImageAPI.GetKernelSize(EdgeGaussian);
			if ( ImageAPI.GaussianGrayImage3(ResultW, ResultH, ResultStep, EdgeBuffer, EdgeGaussian, RawBuffer2) == false )
			{
				JetMemory.free_func(RawBuffer1);
				JetMemory.free_func(RawBuffer2);
				JetMemory.free_func(EdgeBuffer);
				return false;	
			}
			::memcpy(EdgeBuffer, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);
		}
	}
	else
	{	::memcpy(EdgeBuffer, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize); }
	CurrentMode = IMGCMP_SHOW_EDGE;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, EdgeBuffer, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, EdgeBuffer, sizeof(IMAGE_DATA)*RawBufferSize);	}	

	if ( true == bRemoveEdge )
	{
		for ( i=0; i<RawBufferSize; i++ )
		{
			/*
			if ( EdgeBuffer[i] > nEdgeLevel ) 
			{
				RawBuffer1[i] = 0;
				continue;
			}*/
			if ( RawBuffer1[i] < EdgeBuffer[i] ) 
			{	
				RawBuffer1[i] = 0; 
				continue;
			}
			RawBuffer1[i] = RawBuffer1[i]-EdgeBuffer[i];
		}
	}
	CurrentMode = IMGCMP_SHOW_REMOVE_EDGE;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}	

	//Gaussian
	const int GaussianSize = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_GAUSSIAN_EDIT);
	if ( GaussianSize > 0 ) 
	{			
		const int nGaussianSize=((GaussianSize/2)*2)+1;
		if ( ImageAPI.GaussianGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, nGaussianSize, RawBuffer2) == false )
		{
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;	
		}
		::memcpy(RawBuffer1, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);
	}	
	CurrentMode = IMGCMP_SHOW_GAUSSIAN;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}	

	//Binary		
	for ( i=0; i<RawBufferSize; i++ )
	{		
		if ( RawBuffer1[i] < nTolerance ) { RawBuffer1[i] = 0; }
		else { RawBuffer1[i] = 255; }
	}	
	CurrentMode = IMGCMP_SHOW_BINARY;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}
	
	//影像處理
	//膨脹-Dilate	
	const int DilateSize = (int)(CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_DILATE_EDIT));
	if ( DilateSize > 0 )
	{
		const size_t nDilateSize=ImageAPI.GetKernelSize(DilateSize);
		if ( ImageAPI.DilateGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, nDilateSize, 1, RawBuffer2) == false )
		{	
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;
		}
		::memcpy(RawBuffer1, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);		
	}
	CurrentMode = IMGCMP_SHOW_DILATE;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}
	
	int   MorphMode = 0;
	int   ShapeMode = JetAPI::GetComboxCurSelData(m_MorphShapeCombox);//MORPH_SHAPE_RECT;
	const int NoiseFilterOpen = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_OPEN_EDIT);
	if ( NoiseFilterOpen > 0 ) 	
	{
		const int OpenSizeL=((NoiseFilterOpen/2)*2)+1;
		MorphMode = MORPH_OPEN;		
		if ( ImageAPI.MorphGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, MorphMode, ShapeMode, OpenSizeL, 1, RawBuffer2) == false )
		{	
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;
		}
		::memcpy(RawBuffer1, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);		
	}
	CurrentMode = IMGCMP_SHOW_OPEN;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}

	const int NoiseFilterClose = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_CLOSE_EDIT);
	if ( NoiseFilterClose > 0 ) 
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ((NoiseFilterClose/2)*2)+1;
		if ( ImageAPI.MorphGrayImage3(ResultW, ResultH, ResultStep, RawBuffer1, MorphMode, ShapeMode, CloseSize, 1, RawBuffer2) == false )	
		{	
			JetMemory.free_func(RawBuffer1);
			JetMemory.free_func(RawBuffer2);
			JetMemory.free_func(EdgeBuffer);
			return false;
		}
		::memcpy(RawBuffer1, RawBuffer2, sizeof(MASK_DATA)*RawBufferSize);		
	}
	CurrentMode = IMGCMP_SHOW_CLOSE;
	str = GetShowModeImageFile(CurrentMode);
	ImageAPI.SaveImage(str, ResultW, ResultH, ResultStep, ResultBitCount, RawBuffer1, true);
	if ( ShowMode == CurrentMode )
	{	::memcpy(ResultPtr, RawBuffer1, sizeof(IMAGE_DATA)*RawBufferSize);	}
	
	//m_ResultBlobList
	//區塊分析
	RECT     RoiRect;
	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
	RoiRect.left = 0; RoiRect.top = 0;
	RoiRect.right = ResultW; RoiRect.bottom = ResultH;
	if ( BlobDetector.GrayImageRoiBlobDetect(ResultW, ResultH, ResultStep, RawBuffer1, RoiRect, 128, 255) == false )
	{	
		JetMemory.free_func(RawBuffer1);
		JetMemory.free_func(RawBuffer2);
		JetMemory.free_func(EdgeBuffer);
		return false; 
	}
	
	TPOINT2D     BoxPos;
	TSIZE2D      BoxSize;	
	TPOINT2D     BlobPos;
	RECT         BlobRect={0,0,0,0};
	CAOIBox      ResBox, *BoxPtr=NULL;
	int          BlobW=0, BlobH=0, BlobArea=0, ComponentIndex=0;
	TBlobResult *BlobPtr=NULL;	
	const size_t BlobResCount = BlobDetector.GetBlobCount();
	std::vector<TBlobResult> BlobList(BlobResCount);		

	double       RoiSizeW=0;
	double       RoiSizeH=0;	
	double       BodySizeW=0;
	double       BodySizeH=0;		
	double       BodySizeR=0;		
	double       MaxSizeR = 0.0;
	const double MaxSizeW = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_MAX_W_EDIT);
	const double MaxSizeH = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_MAX_H_EDIT);
	const double MinSizeW = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_MIN_W_EDIT);
	const double MinSizeH = CWnd::GetDlgItemInt(ALGIMGCMP_PARAM_MIN_H_EDIT);
	const int BlobMaxW = (int)(MaxSizeW/ImageRes.x);	
	const int BlobMinW = (int)(MinSizeW/ImageRes.x);
	const int BlobMaxH = (int)(MaxSizeH/ImageRes.y);
	const int BlobMinH = (int)(MinSizeH/ImageRes.y);
	IMGCMP_SIZE_LOG_MODE MinSizeLogMode = (IMGCMP_SIZE_LOG_MODE)(JetAPI::GetComboxCurSelData(m_MinLogModeCombox));
	IMGCMP_SIZE_LOG_MODE MaxSizeLogMode = (IMGCMP_SIZE_LOG_MODE)(JetAPI::GetComboxCurSelData(m_MaxLogModeCombox));
	
	CWnd::GetDlgItemText(ALGIMGCMP_PARAM_MAX_RATIO_EDIT, str);	
	MaxSizeR = ::_ttof(str);

	for ( i=0; i<BlobResCount; i++ )
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if ( NULL == BlobPtr ) { continue; }		
		BlobRect = BlobPtr->m_BlobRect;		
		BlobW = BlobRect.right-BlobRect.left;
		BlobH = BlobRect.bottom-BlobRect.top;		
		BlobArea = BlobPtr->m_BlobPixels;
		BlobPos.x = (BlobRect.right+BlobRect.left)*0.5;
		BlobPos.y = (BlobRect.top+BlobRect.bottom)*0.5;
		if ( IMGCMP_SIZE_LOG_OR == MaxSizeLogMode )
		{
			if ( BlobW > BlobMaxW ) { continue; }
			if ( BlobH > BlobMaxH ) { continue; }
		}
		else if ( IMGCMP_SIZE_LOG_AND == MaxSizeLogMode )
		{
			if ( BlobW > BlobMaxW && BlobH > BlobMaxH ) { continue; }
		}
		if ( IMGCMP_SIZE_LOG_OR == MinSizeLogMode )
		{
			if ( BlobW < BlobMinW ) { continue; }
			if ( BlobH < BlobMinH ) { continue; }		
		}
		else if ( IMGCMP_SIZE_LOG_AND == MinSizeLogMode )
		{
			if ( BlobW < BlobMinW && BlobH < BlobMinH ) { continue; }			
		}

		BodySizeR = 0;
		if ( MaxSizeR > 1.0 )
		{
			if ( BlobW > BlobH )
			{	
				if ( BlobH < (2*BlobMinH) )
				{	BodySizeR = (BlobW*1.0)/BlobH;	}
			}
			else
			{	
				if ( BlobW < (2*BlobMinW) )
				{	BodySizeR = (BlobH*1.0)/BlobW;	}
			}
			if ( BodySizeR > MaxSizeR ) 
			{	continue; }
		}
		m_ResultBlobList.push_back(BlobRect);
	}		
	
	JetMemory.free_func(RawBuffer1);
	JetMemory.free_func(RawBuffer2);
	JetMemory.free_func(EdgeBuffer);


	const int nResultBlobx = (int)(m_ResultBlobList.size());
	CWnd::SetDlgItemInt(ALGIMGCMP_CALC_INFO_EDIT, nResultBlobx);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnSelchangeShowModeCombox() 
{
	// TODO: Add your control notification handler code here
	OnPatternCalcBtn();
	SetFocusEditWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnPatternResBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CString ResX;
	CString ResY;
	CWnd::GetDlgItemText(ALGIMGCMP_PATTERN_RES_X_EDIT, ResX);	
	CWnd::GetDlgItemText(ALGIMGCMP_PATTERN_RES_Y_EDIT, ResY);

	const double ResultX = ::_ttof(ResX);
	const double ResultY = ::_ttof(ResY);	
	UpdatePatToImgOffset(ResultX, ResultY);;	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnPatternUseScaleChk() 
{
	// TODO: Add your control notification handler code here
	ExecMatch();
}
//-------------------------------------------------------------------------------------//
void CAlgImageCompareWnd::OnEnhanceImageChk() 
{
	// TODO: Add your control notification handler code here
	DrawImageWndBkDC();
	DrawPatternWndBkDC();
	DrawResultWndBkDC();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//