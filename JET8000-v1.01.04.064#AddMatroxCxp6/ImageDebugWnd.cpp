// ImageDebugWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ImageDebugWnd.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
#include "ImageMaskWnd.h"
#include "BlobAnalysis.h"
#include "EvsBarcode1D.h"
#include "EvsBarcodeDataMatrix.h"
#include "3DUnWrapping.h"

#include "MimImageBW8.h"
#include "MimImageC24.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDebugWnd dialog
//-------------------------------------------------------------------------------------//
CImageDebugWnd::CImageDebugWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImageDebugWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageDebugWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_clrBK = 0xFF00FF;
	this->m_PhasePtr = NULL;
	this->m_PhaseMaskPtr = NULL;
	this->m_pImage = NULL;
	this->m_pImageShow = NULL;
	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_BitCount = 0;
	this->m_ImageStep = 0;

	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;	

	this->m_ImageWndPt1.x = this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = this->m_ImageWndPt2.y = -1;	
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageDebugWnd)
	DDX_Control(pDX, IDC_DEBUG_COMBO, m_DebugCombox);
	DDX_Control(pDX, IDC_PROFILE_WND, m_ProfileWnd);
	DDX_Control(pDX, IDC_IMAGE_SOURCE_COMBO, m_ImageSourceComboxWnd);
	DDX_Control(pDX, IDC_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImageDebugWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImageDebugWnd)
	ON_BN_CLICKED(IDC_LOAD_BMP_BTN, OnLoadBmpBtn)
	ON_BN_CLICKED(IDC_SAVE_BMP_BTN, OnSaveBmpBtn)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_IMAGE_SOURCE_COMBO, OnSelchangeImageSourceCombo)
	ON_BN_CLICKED(IDC_SAVE_ROI_BMP_BTN, OnSaveRoiBmpBtn)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_FLOOD_FILL_BTN, OnFloodFillBtn)
	ON_BN_CLICKED(IDC_BLOB_BTN, OnBlobBtn)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_DEBUG_COMBO, OnSelchangeDebugCombo)
	ON_BN_CLICKED(IDC_LEARN_PATTERN_BTN, OnLearnPatternBtn)
	ON_BN_CLICKED(IDC_MATCH_PATTERN_BTN, OnMatchPatternBtn)
	ON_BN_CLICKED(IDC_1D_BARCODE_BTN, On1DBarcodeBtn)
	ON_BN_CLICKED(IDC_DATA_MATRIX_BTN, OnDataMatrixBtn)
	ON_BN_CLICKED(IDC_THIN_BTN, OnThinBtn)
	ON_BN_CLICKED(IDC_SMOOTH_BTN, OnSmoothBtn)
	ON_BN_CLICKED(IDC_LOAD_RAW_BTN, OnLoadRawBtn)
	ON_BN_CLICKED(IDC_SHARP_BTN, OnSharpBtn)
	ON_BN_CLICKED(IDC_AVERAGE_BTN, OnAverageBtn)
	ON_BN_CLICKED(IDC_SOBEL_BTN, OnSobelBtn)
	ON_BN_CLICKED(IDC_THRESHOLD_BTN, OnThresholdBtn)
	ON_BN_CLICKED(IDC_UN_WRAPPING_BTN, OnUnWrappingBtn)
	ON_BN_CLICKED(IDC_MASK_WND_BTN, OnMaskWndBtn)
	ON_BN_CLICKED(IDC_IMAGE_32BIT_BTN, OnImage32bitBtn)
	ON_BN_CLICKED(IDC_IMAGE_HSV_BTN, OnImageHSVBtn)
	ON_BN_CLICKED(IDC_MODIFY_SATURATION_BTN, OnModifySaturationBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageDebugWnd message handlers
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnLoadBmpBtn() 
{
	// TODO: Add your control notification handler code here	
//	const size_t size_DBL = sizeof(double);//8 BYTE
//	const size_t size_INT = sizeof(int);//4 BYTE	
//	const size_t size_SHORT = sizeof(short);//2 BYTE
//	SHRT_MAX = 	32767;//32768
//	USHRT_MAX = 65535;//65536
//	INT_MAX = 2147483647//2147483648=>65536倍的SHRT_MAX
//	UINT_MAX = 4294967295//4294967296=>65536倍的USHRT_MAX	

	//TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	//CFileDialog dialog (TRUE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	const int Align = 4;
	CString extname = dialog.GetFileExt();
	CString filename = dialog.GetPathName();
	const int idx = filename.ReverseFind(_T('\\'));
	CString foldername =  filename.Left(idx);

	this->SetWindowText(filename);	
	extname.MakeUpper();

	if ( extname == _T("JPG") )
	{
		if ( this->IsDlgButtonChecked(IDC_GRAY_IMAGE_RADIO) == TRUE )
		{
			if ( ImageAPI.LoadJPGGrayImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, false) == false )
			{
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 8;
			ImageAPI.CloneGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);			
	//		this->BuildSinPatternImage(m_ImageW, m_ImageH, m_ImageStep, 4, foldername);
		}
		else
		{
			if ( ImageAPI.LoadJPGColorImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, false) == false )
			{	
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 24;		
			ImageAPI.CloneColorImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);		
		}
	}
	else if ( extname == _T("PNG") )
	{
		if ( this->IsDlgButtonChecked(IDC_GRAY_IMAGE_RADIO) == TRUE )
		{
			if ( ImageAPI.LoadPNGGrayImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, false) == false )
			{
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 8;
			ImageAPI.CloneGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);			
	//		this->BuildSinPatternImage(m_ImageW, m_ImageH, m_ImageStep, 4, foldername);
		}
		else
		{
			if ( ImageAPI.LoadPNGColorImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, false) == false )
			{	
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 24;		
			ImageAPI.CloneColorImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);		
		}
	}
	else
	{
		if ( this->IsDlgButtonChecked(IDC_GRAY_IMAGE_RADIO) == TRUE )
		{
			if ( ImageAPI.LoadBMPGrayImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, true) == false )
			{
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 8;
			ImageAPI.CloneGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);			
	//		this->BuildSinPatternImage(m_ImageW, m_ImageH, m_ImageStep, 4, foldername);
		}
		else
		{
			if ( ImageAPI.LoadBMPColorImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_pImage, Align, true) == false )
			{	
				JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
				return;
			}
			this->m_BitCount = 24;		
			ImageAPI.CloneColorImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);		
		}
	}

	this->m_strPixel = _T("");	
	this->m_ImageZoom = 1;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;
	this->SetDlgItemInt(IDC_ROI_CPX_EDIT, m_ImageW/2);
	this->SetDlgItemInt(IDC_ROI_CPY_EDIT, m_ImageH/2);
	this->m_Blob.Clear();	

	this->m_Dib.SetImage(m_pImage, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, true);	
	this->RedrawWnd();
	
	//TestExtractSubRoiSubPix();
	//TestMorphImage();
	//TestRotateImage();
	//TestMiMLibImage();
	//TestJetImage();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSaveBmpBtn() 
{
	// TODO: Add your control notification handler code here
	/*
	CString aa = _T("C:\\JetAOI3D\\Temp\\aa.txt");
	FILE *pfile = ::_tfopen(aa, _T("wt+,ccs=UTF-8"));//UNICODE, UTF-8, UTF-16LE
	if ( NULL != pfile )
	{
		CString str = _T("我的");
		::_ftprintf(pfile, _T("My Unicode Test 01\n"));
		::_ftprintf(pfile, _T("My Unicode Test 02 = %s\n"), str);
		::fclose(pfile); pfile=NULL;
	}
	return;
	*/
	
	//TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	//CFileDialog dialog (FALSE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	

	double ElapsedTime = 0;
	CString str;
	CString extname = dialog.GetFileExt();
	CString filename = dialog.GetPathName();	

	extname.MakeUpper();
	QueryPerformanceCounter(&m_nStartTime);	
	if ( ImageAPI.SaveImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, true) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());	}
	QueryPerformanceCounter(&m_nEndTime);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f ms"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	JetMemory.free_func(m_PhasePtr);
	JetMemory.free_func(m_PhaseMaskPtr);
	JetMemory.free_func(m_pImage);
	JetMemory.free_func(m_pImageShow);	

	ImageAPI.SetCallBackWnd(NULL);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::RedrawWnd()
{
	CWnd *pWnd = this->GetDlgItem(IDC_IMAGE_WND);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return ; }

	CClientDC dc(pWnd);	
	HDC hDC = dc.GetSafeHdc();
	HDC hBKDC = m_ImageWndMemDC.GetSafeHdc();	
	RECT Rect={0};
	HPEN hPen = NULL;
	HPEN hOldPen = NULL;
//	this->m_Dib.Draw(hDC);
	POINT OffsetPts;
	OffsetPts.x = JetAPI::Round(this->m_ImageOffset.x*m_ImageZoom);
	OffsetPts.y = JetAPI::Round(this->m_ImageOffset.y*m_ImageZoom);
	::IntersectClipRect(hDC, m_ImageWndRect.left, m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);

	COLORREF clrBK = m_clrBK;	
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hBKDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}

	if ( ImageAPI.DrawImageToDC(hBKDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImageShow, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_clrBK) == false )
	{	return ; }
	
	POINT CP={(m_ImageWndRect.left+m_ImageWndRect.right)/2, (m_ImageWndRect.top+m_ImageWndRect.bottom)/2};
//	::MoveToEx(hBKDC, m_ImageWndRect.left, CP.y, NULL);
//	::LineTo(hBKDC, m_ImageWndRect.right, CP.y);
//	::MoveToEx(hBKDC, CP.x, m_ImageWndRect.top, NULL);
//	::LineTo(hBKDC, CP.x, m_ImageWndRect.bottom);
	
	m_Blob.DrawBlobResult(hBKDC, CP.x, CP.y, m_ImageZoom, OffsetPts.x, OffsetPts.y);

	POINT pt1={JetAPI::Round(m_ImageWndPt1.x), JetAPI::Round(m_ImageWndPt1.y)};
	POINT pt2={JetAPI::Round(m_ImageWndPt2.x), JetAPI::Round(m_ImageWndPt2.y)};
	if ( IMAGE_DEBUG_PROFILE == m_DebugMode )
	{	
		if ( pt1.x >= 0 )
		{
			hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
			hOldPen = (HPEN)::SelectObject(hBKDC, hPen);
			::MoveToEx(hBKDC, pt1.x, pt1.y, NULL);
			::LineTo(hBKDC, pt2.x, pt2.y);
			::SelectObject(hBKDC, hOldPen);
			::DeleteObject(hPen); hPen=NULL;
		}
	}
	else if ( IMAGE_DEBUG_MATCH==m_DebugMode || IMAGE_DEBUG_BARCODE==m_DebugMode )
	{	
		if ( pt1.x >= 0 )
		{	
			JetAPI::PointsToRect(pt1, pt2, Rect);
			hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
			hOldPen = (HPEN)::SelectObject(hBKDC, hPen);
			::MoveToEx(hBKDC, Rect.left, Rect.top, NULL);
			::LineTo(hBKDC, Rect.right, Rect.top);
			::LineTo(hBKDC, Rect.right, Rect.bottom);
			::LineTo(hBKDC, Rect.left, Rect.bottom);
			::LineTo(hBKDC, Rect.left, Rect.top);
			::SelectObject(hBKDC, hOldPen);
			::DeleteObject(hPen); hPen=NULL;
		}
	}

	if ( this->m_strPixel.GetLength() > 0 ) 
	{
		::SetTextColor(hBKDC, 0xFFFFFF);
		::TextOut(hBKDC, 0, 0, m_strPixel, m_strPixel.GetLength());
	}
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hBKDC, 0, 0, SRCCOPY );

	this->DrawProfileWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SIZE szProfile={0, 160};
	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		this->m_ImageWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);
		//Rect.left = 4;
		Rect.right = cx-4;
		Rect.bottom = cy-8-szProfile.cy;
		this->m_ImageWnd.MoveWindow(&Rect);
		this->m_ImageWnd.GetClientRect(&m_ImageWndRect);		
		this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_clrBK);
	}

	if ( this->m_ProfileWnd.GetSafeHwnd() != NULL )
	{
		RECT Rect={0};
		this->m_ProfileWnd.GetWindowRect(&Rect);
		this->ScreenToClient(&Rect);
		//Rect.left = 4;
		Rect.right = cx-4;		
		Rect.bottom = cy-4;
		Rect.top = Rect.bottom-szProfile.cy;
		this->m_ProfileWnd.MoveWindow(&Rect);
		this->m_ProfileWnd.GetClientRect(&m_ImageWndRect);
	}
}
//-------------------------------------------------------------------------------------//
BOOL CImageDebugWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->ShowWindow(SW_SHOWMAXIMIZED);	
	this->CheckDlgButton(IDC_COLOR_IMAGE_RADIO, TRUE);
	this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	this->m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_clrBK);

	this->m_ImageZoom = 1;	

	CString text;
	int      index = 0;	
	AOIDataDefine.BuildImageSourceModeCombox(m_ImageSourceComboxWnd, FRAME_COLOR);
	m_ImageSourceComboxWnd.SetCurSel(0);

	index = 0;
	JetAPI::ClearCombox(m_DebugCombox);
	text = _T("Disable");	
	m_DebugCombox.InsertString(-1, text);//-1表示加在最後面
	m_DebugCombox.SetItemData(index, IMAGE_DEBUG_DISABLE); index ++;
	
	text = _T("Profile");	
	m_DebugCombox.InsertString(-1, text);//-1表示加在最後面
	m_DebugCombox.SetItemData(index, IMAGE_DEBUG_PROFILE); index ++;

#ifdef EVISION_MATCH_USE
	text = _T("Match");	
	m_DebugCombox.InsertString(-1, text);//-1表示加在最後面
	m_DebugCombox.SetItemData(index, IMAGE_DEBUG_MATCH); index ++;
#endif//EVISION_MATCH_USE

	text = _T("Barcode");	
	m_DebugCombox.InsertString(-1, text);//-1表示加在最後面
	m_DebugCombox.SetItemData(index, IMAGE_DEBUG_BARCODE); index ++;
	
	m_DebugMode = IMAGE_DEBUG_DISABLE;
	JetAPI::SetComboxCurSel(m_DebugCombox, IMAGE_DEBUG_DISABLE);

	this->SetDlgItemText(IDC_WEIGHT_EDIT_R, _T("100"));
	this->SetDlgItemText(IDC_WEIGHT_EDIT_G, _T("100"));
	this->SetDlgItemText(IDC_WEIGHT_EDIT_B, _T("100"));

	this->SetDlgItemText(IDC_SHARP_THRESHOLD_EDIT, _T("10"));
	this->SetDlgItemText(IDC_SHARP_AMOUNT_EDIT, _T("400"));

	this->SetDlgItemInt(IDC_LOW_CONTRAST_EDIT, 5);
	this->SetDlgItemInt(IDC_OVER_SATURATION_EDIT, 250);
	
	SetDlgItemInt(IDC_IMAGE_HSV_HUE_EDIT, 0);
	SetDlgItemInt(IDC_IMAGE_HSV_SAT_EDIT, 0);
	SetDlgItemInt(IDC_IMAGE_HSV_VAL_EDIT, 0);

	SetDlgItemInt(IDC_ALPHA_RED_EDIT, 0);
	SetDlgItemInt(IDC_ALPHA_GREEN_EDIT, 0);
	SetDlgItemInt(IDC_ALPHA_BLUE_EDIT, 0);

	SetDlgItemInt(IDC_SATURATED_RED_EDIT, 1);
	SetDlgItemInt(IDC_SATURATED_BLUE_EDIT, 1);	
	

	ImageAPI.SetCallBackWnd(this->GetSafeHwnd());
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	QueryPerformanceFrequency(&m_nFreq);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSelchangeImageSourceCombo() 
{
	// TODO: Add your control notification handler code here
	if ( this->m_BitCount != 24 ) { return; }

	RECT RoiRect={0, 0, 0, 0};
	IMAGE_SRC_MODE ImageSrcMode = (IMAGE_SRC_MODE)JetAPI::GetComboxCurSelData(m_ImageSourceComboxWnd);
	int WR = this->GetDlgItemInt(IDC_WEIGHT_EDIT_R);
	int WG = this->GetDlgItemInt(IDC_WEIGHT_EDIT_G);
	int WB = this->GetDlgItemInt(IDC_WEIGHT_EDIT_B);

	unsigned char *pGray = NULL;
	const IMAGE_SIZE BytePerLineGry = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, 8);
	RoiRect.right = (int)(m_ImageW);
	RoiRect.bottom = (int)(m_ImageH);
	if ( ImageAPI.ColorImageToGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, RoiRect, BytePerLineGry, pGray, ImageSrcMode, WR, WG, WB, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}

	ImageAPI.RGBImageToColorImage3(m_ImageW, m_ImageH, BytePerLineGry, pGray, pGray, pGray, m_ImageStep, m_pImageShow, false);//RGB轉彩色	
	JetMemory.free_func(pGray);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSaveRoiBmpBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString filename = dialog.GetPathName();
	RECT RoiRect={0};
	unsigned char *pRoi = NULL;

	RoiRect.left = m_ImageW/4;
	RoiRect.right = RoiRect.left + (m_ImageW/2);
	RoiRect.top = m_ImageH/4;
	RoiRect.bottom = RoiRect.top + (m_ImageH/2);

	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const IMAGE_SIZE RoiBytePerLine = JetAPI::GetBMPImagePixelsPerLine(RoiW, m_BitCount, 8);

	if ( 8 == m_BitCount )
	{
		if ( ImageAPI.ExtractGrayRoiImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, RoiRect, RoiBytePerLine, pRoi, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return;
		}
		if ( ImageAPI.SaveBMPGrayImage(filename, RoiW, RoiH, RoiBytePerLine, pRoi, true) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
	}
	else
	{
		if ( ImageAPI.ExtractColorRoiImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, RoiRect, RoiBytePerLine, pRoi, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return;
		}
		if ( ImageAPI.SaveBMPColorImage(filename, RoiW, RoiH, RoiBytePerLine, pRoi, true) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
	}
	JetMemory.free_func(pRoi);
}
//-------------------------------------------------------------------------------------//
BOOL CImageDebugWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	TPOINT2D ImageOffset = this->m_ImageOffset;
	double ImageZoom = this->m_ImageZoom;
	if ( zDelta > 0 ) 
	{	ImageZoom *= 1.1;	}
	else
	{	ImageZoom /= 1.1;	}
	ImageZoom = AOIDataCollect.AdjustImageZoom(ImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, ImageZoom, m_ImageOffset);
	m_ImageZoom = ImageZoom;	
	
	this->RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnFloodFillBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->m_BitCount != 8 ) { return; }

	RECT    Rect={0};
	Rect.right  = m_ImageW;
	Rect.bottom = m_ImageH;
	int SeedX = (this->m_ImageW+1)/2;
	int SeedY = (this->m_ImageH+1)/2;
	unsigned char nNull = 0;
	unsigned char nFilled = 128;
	ImageAPI.GrayImageFloodFill(m_ImageW, m_ImageH, m_ImageStep, m_pImageShow, Rect, SeedX, SeedY, nNull, nFilled);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnBlobBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->m_BitCount != 8 ) { return; }
	CString str;
	RECT Roi={0};
	//CJetBlob Blob;
	CBlobAnalysis Blob2;
	TBlobResult     *pBlob=NULL;		
	size_t i=0, j=0, k=0, idx;	
	const int MaxLoop = 1;
	const int ThH = 255;
	const int ThL = 100;

	Roi.right = m_ImageW;
	Roi.bottom = m_ImageH;
	double ElapsedTime1=0;
	double ElapsedTime2=0;
	m_Blob.SetBlobConnectivity(BLOB_CONNECTIVITY_8);
	QueryPerformanceCounter(&m_nStartTime);		

	for ( i=0; i<MaxLoop; i++ )
	{
		if ( m_Blob.GrayImageRoiBlobDetect(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, ThL, ThH) == false )
		{	return; }
	}
	QueryPerformanceCounter(&m_nEndTime);	
	ElapsedTime1 = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	ElapsedTime1 = ElapsedTime1/MaxLoop;


	Blob2.SetThresholdL(ThL);
	Blob2.SetThresholdH(ThH);
	Blob2.SetClass(true);
	Blob2.SetImageFormat(true);
	//Blob2.SetConnectType(true);
	Blob2.SetConnectType(false);
	QueryPerformanceCounter(&m_nStartTime);
	for ( i=0; i<MaxLoop; i++ )
	{
		Blob2.CalPadNumber(m_ImageW, m_ImageH, m_pImage);
	}
	QueryPerformanceCounter(&m_nEndTime);	
	ElapsedTime2 = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;	
	ElapsedTime2 = ElapsedTime2/MaxLoop;
	
	const int BlobCount = (int)(m_Blob.GetBlobCount());
	const int BlobCount2 = Blob2.PadNumber;

	str.Format(_T("Time1=%.0f us(%d), Time2=%.0f us(%d)"), ElapsedTime1, BlobCount, ElapsedTime2, BlobCount2);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
//	JetAPI::ShowMessageBox(str);

	if ( 0 == BlobCount ) { return; }
	::memset(this->m_pImageShow, 0x00, sizeof(unsigned char)*ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH));

	unsigned char Gray = 0;	
	TScanLine  *pScanLine=NULL;
	
	int BlobID = 0;
	const size_t ScanLineCount = m_Blob.GetScanLineCount();
	int dGray = 255/BlobCount;
	if ( dGray == 0 ) { dGray = 1; }

	Gray = 0;
	for ( i=0; i<BlobCount; i++ )
	{
		pBlob = m_Blob.GetBlobPtr(i, false);
		if ( NULL == pBlob ) { continue; }
		BlobID = pBlob->m_BlobID;
		if ( 255 == Gray )
		{	Gray = 1;	}
		else
		{	Gray += dGray; }
		for ( j=0; j<ScanLineCount; j++ )
		{
			pScanLine = m_Blob.GetScanLinePtr(j, false);
			if ( NULL == pScanLine ) { continue; }
			if ( pScanLine->m_BlobID != BlobID ) { continue; }

			idx = pScanLine->m_Yidx*m_ImageStep;
			for ( k=pScanLine->m_XidxS; k<pScanLine->m_XidxE; k++ )
			{	m_pImageShow[idx+k] = Gray;	}
		}
	}
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;
	this->SetCapture();
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	this->m_RBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_RBtnUpPos = this->m_RBtnDownPos = this->m_MovingPos;	
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT WndPt = point;
	if ( PtInControlWnd(WndPt, IDC_IMAGE_WND, WndPt) == true )
	{	
		CString  str;
		size_t   index=0;
		int      R=0, G=0, B=0;
		POINT    iImagePt;
		TPOINT2D ImagePt=WndPt;
		TPOINT2D WndPt2 =WndPt;
		const IMAGE_SIZE ImageW = m_Dib.GetImageW();
		const IMAGE_SIZE ImageH = m_Dib.GetImageH();
		const int nImageW = (int)(ImageW);
		const int nImageH = (int)(ImageH);
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt2);
		
		JetAPI::Point2DToPoint(ImagePt, iImagePt);
		if ( iImagePt.x<0 || iImagePt.x>=nImageW || iImagePt.y<0 || iImagePt.y>=nImageH || NULL==m_pImage )
		{	str.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y);	}		
		else if ( 8==m_BitCount )
		{
			index = iImagePt.y*m_ImageStep+iImagePt.x;
			R = G = B = m_pImage[index];
			str.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), Gray=%d"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R);
		}
		else if ( 24==m_BitCount )
		{
			index = iImagePt.y*m_ImageStep+(iImagePt.x*3);
			B = m_pImage[index];
			G = m_pImage[index+1];
			R = m_pImage[index+2];
			str.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f), Color(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y, R, G, B);
		}
		else
		{	str.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Pos2(%.2f, %.2f)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, WndPt2.x, WndPt2.y);		} 
		this->SetDlgItemText(IDC_PIXEL_INFO_EDIT, str);
	}

	if ( this != GetCapture() ) 
	{		
		CBaseDialog::OnMouseMove(nFlags, point);
		return; 
	}	

	POINT pt  = point;
	if ( nFlags&MK_LBUTTON )
	{
		if ( PtInControlWnd(pt, IDC_IMAGE_WND, pt) == true )
		{	
			this->m_ImageWndPt2 = (pt);				
		}
	}
	else if ( nFlags&MK_RBUTTON )
	{
		m_ImageOffset.x += point.x-m_MovingPos.x;
		m_ImageOffset.y += point.y-m_MovingPos.y;
		m_MovingPos = point;		
	}
	this->RedrawWnd();
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//m_ImageWndPt1
	POINT pt = point;
	this->m_LBtnUpPos = point;
	this->m_LBtnDownPos = this->m_MovingPos = this->m_LBtnUpPos;
	this->SetCapture();

	
	if ( PtInControlWnd(pt, IDC_IMAGE_WND, pt) == true )
	{	this->m_ImageWndPt1 = pt;	}

	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//m_ImageWndPt1
	::ReleaseCapture();
	POINT pt = point;
	if ( PtInControlWnd(pt, IDC_IMAGE_WND, pt) == true )
	{	this->m_ImageWndPt2 = pt;	}

	this->m_LBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	this->RedrawWnd();
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CImageDebugWnd::PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2)
{
	return JetAPI::CheckPtInCtrlWnd(this, pt, ControlID, &pt2);
	/*
	CWnd *pWnd = this->GetDlgItem(ControlID);
	if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) )
	{	return false; }

	pt2 = pt;
	this->MapWindowPoints(pWnd, &pt2, 1);
	
	RECT Rect={0};
	pWnd->GetClientRect(&Rect);
	if ( ::PtInRect(&Rect, pt2) == FALSE )
	{	return false; }
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::DrawProfileWnd()
{
	if ( IMAGE_DEBUG_PROFILE != m_DebugMode )
	{	return; }

	if ( m_ProfileWnd.GetSafeHwnd() == NULL )
	{	return; }

	BITMAPINFO *pInfo = this->m_Dib.GetDIBInfo();
	if ( NULL == pInfo ) { return; }

	CClientDC dc(&m_ProfileWnd);
	HDC hDC = dc.GetSafeHdc();

	RECT   WndRect={0};
	RECT   WndRect2={0};
	this->m_ProfileWnd.GetClientRect(&WndRect);
	HBRUSH hBrush = ::CreateSolidBrush(0x000000);
	::FillRect(hDC, &WndRect, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;

	POINT pt1={0};
	POINT pt2={0};
	TPOINT2D ImagePt1, ImagePt2;
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();
	std::vector<TPIXEL_GRY> ProfileGry;
	std::vector<TPIXEL_RGB> ProfileClr;

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);

	ImagePt1.y = ImageH-ImagePt1.y;
	ImagePt2.y = ImageH-ImagePt2.y;
	WndRect2 = WndRect;
	if ( ImagePt1.x < ImagePt2.x )
	{	
		pt1.x = JetAPI::Round(ImagePt1.x);
		pt1.y = JetAPI::Round(ImagePt1.y);
		pt2.x = JetAPI::Round(ImagePt2.x); 
		pt2.y = JetAPI::Round(ImagePt2.y); 
	}
	else
	{
		pt1.x = JetAPI::Round(ImagePt2.x);
		pt1.y = JetAPI::Round(ImagePt2.y);
		pt2.x = JetAPI::Round(ImagePt1.x); 
		pt2.y = JetAPI::Round(ImagePt1.y);
	}	
	switch ( BitCount )
	{
	case 8:
		ImageAPI.CalcGrayImageProfile(ImageW, ImageH, BytePerLine, pSrc, pt1, pt2, ProfileGry);
		break;
	case 24:
		ImageAPI.CalcColorImageProfile(ImageW, ImageH, BytePerLine, pSrc, pt1, pt2, ProfileClr);
		break;
	}
	
	::InflateRect(&WndRect2, -24, -24);

	//draw profile
	size_t i=0;
	POINT pt={0};
	double stepx=0, stepy=0;
	TPIXEL_GRY *PTBW8Ptr = NULL;
	TPIXEL_RGB *PTC24Ptr = NULL;
	
	const size_t size_gray = ProfileGry.size();
	const size_t size_color = ProfileClr.size();
	HPEN hPen = NULL;
	HPEN hOldPen = NULL;

	if ( size_gray > 0 ) 
	{
		stepx = WndRect2.right-WndRect2.left;
		stepy = WndRect2.bottom-WndRect2.top;

		stepx = stepx/size_gray;
		stepy = stepy/255;

		hPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);

		::MoveToEx(hDC, WndRect2.left, WndRect2.bottom, NULL);
		for ( i=0; i<size_gray; i++ )
		{
			PTBW8Ptr = &(ProfileGry[i]);

			pt.x = JetAPI::Round(i*stepx)+WndRect2.left;
			pt.y = JetAPI::Round(PTBW8Ptr->gray*stepy);	
			pt.y = WndRect2.bottom-pt.y;			
			::LineTo(hDC, pt.x, pt.y);
		}

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}

	if ( size_color > 0 ) 
	{
		stepx = WndRect2.right-WndRect2.left;
		stepy = WndRect2.bottom-WndRect2.top;

		stepx = stepx/size_color;
		stepy = stepy/255;

		//Red
		hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::MoveToEx(hDC, WndRect2.left, WndRect2.bottom, NULL);
		for ( i=0; i<size_color; i++ )
		{
			PTC24Ptr = &(ProfileClr[i]);

			pt.x = JetAPI::Round(i*stepx)+WndRect2.left;
			pt.y = JetAPI::Round(PTC24Ptr->r*stepy);	
			pt.y = WndRect2.bottom-pt.y;
			::LineTo(hDC, pt.x, pt.y);
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen=NULL;

		//Green
		hPen = ::CreatePen(PS_SOLID, 1, 0x00FF00);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::MoveToEx(hDC, WndRect2.left, WndRect2.bottom, NULL);
		for ( i=0; i<size_color; i++ )
		{
			PTC24Ptr = &(ProfileClr[i]);

			pt.x = JetAPI::Round(i*stepx)+WndRect2.left;
			pt.y = JetAPI::Round(PTC24Ptr->g*stepy);	
			pt.y = WndRect2.bottom-pt.y;
			::LineTo(hDC, pt.x, pt.y);
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen=NULL;

		//Blue
		hPen = ::CreatePen(PS_SOLID, 1, 0xFF0000);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::MoveToEx(hDC, WndRect2.left, WndRect2.bottom, NULL);
		for ( i=0; i<size_color; i++ )
		{
			PTC24Ptr = &(ProfileClr[i]);

			pt.x = JetAPI::Round(i*stepx)+WndRect2.left;
			pt.y = JetAPI::Round(PTC24Ptr->b*stepy);	
			pt.y = WndRect2.bottom-pt.y;
			::LineTo(hDC, pt.x, pt.y);
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}		
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSelchangeDebugCombo() 
{
	// TODO: Add your control notification handler code here
	m_ImageWndPt1.x = m_ImageWndPt1.y = -1;
	m_ImageWndPt2.x = m_ImageWndPt2.y = -1;
	this->m_DebugMode = (IMAGE_DEBUG_MODE)(JetAPI::GetComboxCurSelData(this->m_DebugCombox));
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnLearnPatternBtn() 
{
	// TODO: Add your control notification handler code here
	if ( IMAGE_DEBUG_MATCH != this->m_DebugMode ) { return; }
#ifdef EVISION_MATCH_USE
	CString str;

	RECT  RoiRect={0};
	POINT pt1={0};
	POINT pt2={0};
	TPOINT2D ImagePt1, ImagePt2;	
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();	
	
	unsigned char *pROI = NULL;	
	bool IsColor = false;

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);

	pt1.x = JetAPI::Round(ImagePt1.x);
	pt1.y = ImageH-JetAPI::Round(ImagePt1.y);
	pt2.x = JetAPI::Round(ImagePt2.x);
	pt2.y = ImageH-JetAPI::Round(ImagePt2.y);
	JetAPI::PointsToRect(pt1, pt2, RoiRect);

	const unsigned int RoiW = RoiRect.right-RoiRect.left;
	const unsigned int RoiH = RoiRect.bottom-RoiRect.top;	
	const unsigned int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, BytePerLine, BitCount, pSrc, RoiRect, RoiStep, pROI, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiPattern.BMP"));
	if ( ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, pROI, true) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
		
	if ( m_LibMatch.LearnPattern(RoiW, RoiH, RoiStep, BitCount, pROI, true) == false )
	{
		JetMemory.free_func(pROI);	
		JetAPI::ShowMessageBox(m_LibMatch.GetErrorString());		
		return;
	}
	JetMemory.free_func(pROI);	
#endif//EVISION_MATCH_USE
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnMatchPatternBtn() 
{
	// TODO: Add your control notification handler code here
	if ( IMAGE_DEBUG_MATCH != this->m_DebugMode ) { return; }
#ifdef EVISION_MATCH_USE
	if ( m_LibMatch.GetPatternLearnt() == false )
	{
		JetAPI::ShowMessageBox(_T("Not Learnt"));		
		return;
	}

	CString str;
	int   i = 0;
	RECT  RoiRect={0};
	bool  IsColor = false;	
	
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int ImageW2 = ImageW/2;
	const unsigned int ImageH2 = ImageH/2;
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();	

	unsigned char *pROI = NULL;	
	unsigned int RoiW = ImageW/2;
	unsigned int RoiH = ImageH/2;
	int RoiCpX = this->GetDlgItemInt(IDC_ROI_CPX_EDIT);
	int RoiCpY = this->GetDlgItemInt(IDC_ROI_CPY_EDIT);
	RoiRect.left = RoiCpX-(RoiW/2);
	RoiRect.right = RoiRect.left+RoiW;
	RoiRect.top = RoiCpY-(RoiH/2);
	RoiRect.bottom = RoiRect.top+RoiH;
	unsigned int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	int Result = 0;
	double ResX=0, ResY=0;

	POINT    ImagePt[4];
	TPOINT2D ImagePtd[4];
	TPOINT2D CornerPt[4];
	TPOINT2D CornerPt2[4];
	TPOINT2D CornerCP;	
	TPOINT2D StageCP;	
	TPOINT2D StageSize;
	TPOINT2D ErrorPt;
	TPOINT2D ErrorMax;
	TPOINT2D ResultPos;
	TPOINT2D ResultPosFirst;
	TPOINT2D ResultPos1, ResultPos2;
	TPOINT2D ImageResultPt;
	TPOINT2D ErrorPt1, ErrorPt2;
	double ResolutionX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	double ResolutionY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);

	StageSize.x = RoiW*ResolutionX*0.5;
	StageSize.y = RoiH*ResolutionY*0.5;

	for ( i=-30; i<30; i++ )
	{
		CornerCP.x = StageCP.x + i;
		CornerCP.y = StageCP.y + i;

		CornerPt[0].x = CornerCP.x-StageSize.x;
		CornerPt[0].y = CornerCP.y-StageSize.y;

		CornerPt[1].x = CornerCP.x+StageSize.x;
		CornerPt[1].y = CornerCP.y-StageSize.y;

		CornerPt[2].x = CornerCP.x+StageSize.x;
		CornerPt[2].y = CornerCP.y+StageSize.y;

		CornerPt[3].x = CornerCP.x-StageSize.x;
		CornerPt[3].y = CornerCP.y+StageSize.y;

		AOIDataCollect.MapStageCornerToCameraPt(PRIMARY_CAMERA_ID, CornerPt, StageCP, ImagePt);

		ImagePtd[0] = ImagePt[0];
		ImagePtd[1] = ImagePt[1];
		ImagePtd[2] = ImagePt[2];
		ImagePtd[3] = ImagePt[3];

		AOIDataCollect.MapCameraCornerToStage(PRIMARY_CAMERA_ID, ImagePtd, StageCP, CornerPt2);

		ErrorPt.x = CornerPt2[0].x - CornerPt[0].x;
		ErrorPt.y = CornerPt2[0].y - CornerPt[0].y;

		ErrorMax.x = __max(ErrorMax.x, ErrorPt.x);
		ErrorMax.y = __max(ErrorMax.y, ErrorPt.y);


		JetAPI::CornerPtToRect(ImagePt, RoiRect);		
		RoiW = RoiRect.right-RoiRect.left;
		RoiH = RoiRect.bottom-RoiRect.top;
		RoiCpX = (RoiRect.left+RoiRect.right)/2;
		RoiCpY = (RoiRect.top+RoiRect.bottom)/2;;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	
		m_LibMatch.SetInterpolate(true);		
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, BytePerLine, BitCount, pSrc, RoiRect, RoiStep, pROI, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return;
		}
		str.Format(_T("%s\\%d%s"), AOIDataCollect.GetAOITempDirectory(), i+1, _T("RoiImage.BMP"));
		if ( ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, pROI, true) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
		
		if ( m_LibMatch.Match(RoiW, RoiH, RoiStep, BitCount, pROI, true) == false )
		{		
			JetMemory.free_func(pROI);	
			JetAPI::ShowMessageBox(m_LibMatch.GetErrorString());
			return;
		}	
		JetMemory.free_func(pROI);	

		Result = m_LibMatch.GetNumPositions();
		if ( Result > 0 ) 
		{
			ResX = m_LibMatch.GetResultPosX(0);
			ResY = m_LibMatch.GetResultPosY(0);			
		}
		else
		{
			ResX = 0;
			ResY = 0;			
		}

		ImageResultPt.x = ResX+RoiCpX-ImageW2;//Roi結果轉成FOV座標
		ImageResultPt.y = ResY+RoiCpY-ImageH2;//Roi結果轉成FOV座標
		AOIDataCollect.MapCameraPtToStage(PRIMARY_CAMERA_ID, ImageResultPt, StageCP, ResultPos);

		if ( i%13 == 0 ) 
		{	ResultPosFirst = ResultPos; }

		ResultPos1.x = ResultPos.x + ErrorPt.x;
		ResultPos1.y = ResultPos.y + ErrorPt.y;

		ResultPos2.x = ResultPos.x - ErrorPt.x;
		ResultPos2.y = ResultPos.y - ErrorPt.y;

		ErrorPt1.x = ResultPos1.x-ResultPosFirst.x;
		ErrorPt1.y = ResultPos1.y-ResultPosFirst.y;

		ErrorPt2.x = ResultPos2.x-ResultPosFirst.x;
		ErrorPt2.y = ResultPos2.y-ResultPosFirst.y;		
		
		i = i;
	}

	
	/*
	if ( 24 == BitCount )
	{
		IsColor = true;
		if ( ImageAPI.ExtractColorRoiImage(ImageW, ImageH, BytePerLine, pSrc, RoiRect, RoiStep, pROI, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return;
		}
		str.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("RoiImageC24.BMP"));
		if ( ImageAPI.SaveBMPColorImage(str, RoiW, RoiH, RoiStep, pROI, true) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
	}
	else
	{
		IsColor = false;
		if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, BytePerLine, pSrc, RoiRect, RoiStep, pROI, false) == false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return;
		}
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiImageBW8.BMP"));
		if ( ImageAPI.SaveBMPGrayImage(str, RoiW, RoiH, RoiStep, pROI, true) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }
	}
	
	if ( m_LibMatch.DoMatch_Memory(pROI, RoiW, RoiH, RoiStep, IsColor, true) == false )
	{		
		JetMemory.free_func(pROI);	
		JetAPI::ShowMessageBox(m_LibMatch.GetErrorString());
		return;
	}
	JetMemory.free_func(pROI);	
	double ResX=0, ResY=0;
	const int Result = m_LibMatch.GetNumPositions();
	if ( Result > 0 ) 
	{
		ResX = m_LibMatch.GetResultPosX(0);
		ResY = m_LibMatch.GetResultPosY(0);
	}
	str.Format(_T("Pattern Pos(%.2f, %.2f)"), ResX, ResY);
	this->SetDlgItemText(IDC_INFO_EDIT, str);

	*/	
#endif//EVISION_MATCH_USE
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::On1DBarcodeBtn() 
{
	// TODO: Add your control notification handler code here
	if ( IMAGE_DEBUG_BARCODE != this->m_DebugMode ) { return; }
#ifdef EVISION_1D_BARCODE_USE
	CString str;
	int   i = 0;

	RECT  RoiRect={0};
	POINT pt1={0};
	POINT pt2={0};
	TPOINT2D ImagePt1, ImagePt2;	
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int ImageW2 = ImageW/2;
	const unsigned int ImageH2 = ImageH/2;
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();	
	if ( BitCount != 8 ) { return ; }

	unsigned char *pROI = NULL;		

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);

	pt1.x = JetAPI::Round(ImagePt1.x);
	pt1.y = ImageH-JetAPI::Round(ImagePt1.y);
	pt2.x = JetAPI::Round(ImagePt2.x);
	pt2.y = ImageH-JetAPI::Round(ImagePt2.y);
	JetAPI::PointsToRect(pt1, pt2, RoiRect);

	CEvsRoiBW8    RoiBW8;	
	CEvsImageBW8  ImageBW8;
	CEvsBarcode1D Barcode1D;

	const unsigned int RoiW = RoiRect.right-RoiRect.left;
	const unsigned int RoiH = RoiRect.bottom-RoiRect.top;	
	const unsigned int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);	
	if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, BytePerLine, pSrc, RoiRect, RoiStep, pROI, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiBarcodeBW8.BMP"));
	if ( ImageAPI.SaveBMPGrayImage(str, RoiW, RoiH, RoiStep, pROI, true) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }			
	
	if ( ImageBW8.SetImagePtr(pROI, RoiW, RoiH, RoiStep, true) == false )
	{
		JetMemory.free_func(pROI);
		JetAPI::ShowMessageBox(ImageBW8.GetErrorString());
		return ;
	}	

	const size_t BarcodeTextLen = 128;
	char   BarcodeText[BarcodeTextLen]="";
	RoiBW8.Attach(&ImageBW8);
	RoiBW8.SetPlacement(0, 0, RoiW, RoiH);


	//CEvsRoiBW8    RoiBW8;	
//	CEvsImageBW8  ImageBW82=ImageBW8;
//	CEvsRoiBW8    RoiBW82;
//	RoiBW82.Attach(&ImageBW82);
//	RoiBW82.SetPlacement(0, 0, RoiW, RoiH);

	if ( Barcode1D.Read(&RoiBW8, BarcodeText, BarcodeTextLen) == false )
	{
		JetMemory.free_func(pROI);
		JetAPI::ShowMessageBox(Barcode1D.GetErrorString());
		return;
	}
	JetMemory.free_func(pROI);

	str = BarcodeText;
	this->SetDlgItemText(IDC_INFO_EDIT, str);	
#endif//EVISION_1D_BARCODE_USE
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnDataMatrixBtn() 
{
	// TODO: Add your control notification handler code here
	if ( IMAGE_DEBUG_BARCODE != this->m_DebugMode ) { return; }
#ifdef EVISION_DATA_MATRIX_USE
	CString str;
	int   i = 0;

	RECT  RoiRect={0};
	POINT pt1={0};
	POINT pt2={0};
	TPOINT2D ImagePt1, ImagePt2;	
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int ImageW2 = ImageW/2;
	const unsigned int ImageH2 = ImageH/2;
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();	
	if ( BitCount != 8 ) { return ; }

	unsigned char *pROI = NULL;		

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);

	pt1.x = JetAPI::Round(ImagePt1.x);
	pt1.y = ImageH-JetAPI::Round(ImagePt1.y);
	pt2.x = JetAPI::Round(ImagePt2.x);
	pt2.y = ImageH-JetAPI::Round(ImagePt2.y);
	JetAPI::PointsToRect(pt1, pt2, RoiRect);

	CEvsRoiBW8    RoiBW8;	
	CEvsImageBW8  ImageBW8;
	CEvsBarcodeDataMatrix BarcodeDataMatrix;

	const unsigned int RoiW = RoiRect.right-RoiRect.left;
	const unsigned int RoiH = RoiRect.bottom-RoiRect.top;	
	const unsigned int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);	
	if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, BytePerLine, pSrc, RoiRect, RoiStep, pROI, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiBarcodeBW8.BMP"));
	if ( ImageAPI.SaveBMPGrayImage(str, RoiW, RoiH, RoiStep, pROI, true) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); }			
	
	if ( ImageBW8.SetImagePtr(pROI, RoiW, RoiH, RoiStep, true) == false )
	{
		JetMemory.free_func(pROI);
		JetAPI::ShowMessageBox(ImageBW8.GetErrorString());
		return ;
	}	

	const size_t BarcodeTextLen = 128;
	char   BarcodeText[BarcodeTextLen]="";
	RoiBW8.Attach(&ImageBW8);
	RoiBW8.SetPlacement(0, 0, RoiW, RoiH);

	if ( BarcodeDataMatrix.Reader1(RoiBW8, BarcodeText, BarcodeTextLen) == false )
	{
		JetMemory.free_func(pROI);
		JetAPI::ShowMessageBox(BarcodeDataMatrix.GetErrorString());
		return;
	}
	JetMemory.free_func(pROI);

	str = BarcodeText;
	this->SetDlgItemText(IDC_INFO_EDIT, str);	
#endif//EVISION_DATA_MATRIX_USE
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnThinBtn() 
{
	// TODO: Add your control notification handler code here
	if ( this->m_BitCount != 8 ) { return; }

	CString str;
	RECT Rect={0};
	Rect.right  = m_ImageW;
	Rect.bottom = m_ImageH;
	int SeedX = (this->m_ImageW+1)/2;
	int SeedY = (this->m_ImageH+1)/2;
	const int ThH = 255;
	const int ThL = 100;
	double ElapsedTime=0;

	QueryPerformanceCounter(&m_nStartTime);
	ImageAPI.ThinningGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImageShow, Rect, m_ImageStep, m_pImageShow, ThL, ThH);
	QueryPerformanceCounter(&m_nEndTime);
	
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f ms"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSmoothBtn() 
{
	// TODO: Add your control notification handler code here		
	const char fnName[] = "CImageDebugWnd::OnSmoothBtn";

	CString str;	
	double ElapsedTime=0;

	//const char fnName[] = __FUNCTION__;	
	//JetAPI::ShowMessageBox(fnName);	
	const size_t KerSize = 3;
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	QueryPerformanceCounter(&m_nStartTime);
	ImageAPI.SmoothImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, KerSize, m_pImageShow);		
	//::memcpy(m_pImageShow, m_pImage, BufferSize);
	QueryPerformanceCounter(&m_nEndTime);

	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f us"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnLoadRawBtn() 
{
	// TODO: Add your control notification handler code here
	JetAPI::EnableEditWnd(this, IDC_WEIGHT_EDIT_R, FALSE);	
	JetAPI::EnableEditWnd(this, IDC_COLOR_IMAGE_RADIO, FALSE);	

	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString str;
	TCHAR TMode[32] = _T("");
	CString filename = dialog.GetPathName();
	const int idx = filename.ReverseFind(_T('\\'));
	CString foldername =  filename.Left(idx);
	_tcscpy(TMode, _T("r"));
	JetAPI::ModifyOpenFileMode_Read(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Open File Fault (%s)"), filename);
		::AfxMessageBox(str);
		return;
	}

	size_t len = 0;
	const size_t txtSize = 256;
	TCHAR txtBuffer[txtSize]=_T("");
	std::vector<CString> fileList;
	while ( ::_fgetts(txtBuffer, txtSize, pfile) != NULL )
	{
		len = ::_tcslen(txtBuffer);
		if ( len < 3 ) 
		{	continue; }

		if (_T('\n') == txtBuffer[len-1])
		{	txtBuffer[len-1] = _T('\0');	}		
		fileList.push_back(txtBuffer);
	};
	::fclose(pfile); pfile = NULL;
	
	bool bIsOk=true;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	const size_t MaxPtrCount = 16;
	double P1=3, P2=4, P3=5, PeriodP=0;	
	unsigned char *Ptr=NULL;
	unsigned char *PtrList[MaxPtrCount]={0x00};		
	PHASE_PTR BasePhasePtr=NULL;	
	double ElapsedTime=0;	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	P1 = SysParam.m_PhasePeriod1;
	P2 = SysParam.m_PhasePeriod2;
	P3 = SysParam.m_PhasePeriod3;

	int PatternID = 0;
	TPhaseNoiseParam NoiseParam;	
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	NoiseParam.PhaseNoiseDef = SysParam.m_PhaseNoiseDefine;
	NoiseParam.PhaseLowContrastA = CWnd::GetDlgItemInt(IDC_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedA = CWnd::GetDlgItemInt(IDC_OVER_SATURATION_EDIT);
	NoiseParam.PhaseLowPotentialA = SysParam.m_PhaseNoiseLowPotentialA;

	NoiseParam.PhaseLowContrastB = CWnd::GetDlgItemInt(IDC_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedB = CWnd::GetDlgItemInt(IDC_OVER_SATURATION_EDIT);
	NoiseParam.PhaseLowPotentialB = SysParam.m_PhaseNoiseLowPotentialA;

	NoiseParam.PhaseLowContrastC = CWnd::GetDlgItemInt(IDC_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedC = CWnd::GetDlgItemInt(IDC_OVER_SATURATION_EDIT);
	NoiseParam.PhaseLowPotentialC = SysParam.m_PhaseNoiseLowPotentialA;

	size_t i=0, j=0;
	const int Align = 4;
	const size_t FileCount = fileList.size();
	for ( i=0; i<FileCount; i++ )
	{
		bIsOk = true;
		str = fileList[i];
		filename.Format(_T("%s\\%s"), foldername, str);		
		if ( i < MaxPtrCount )
		{	
			bIsOk = ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, PtrList[i], Align, true);
			if ( bIsOk == false )
			{
				str.Format(_T("Error, Open File Fault(%s)"), filename);
				::AfxMessageBox(str);

				for ( j=0; j<MaxPtrCount; j++ )
				{	JetMemory.free_func(PtrList[j]);	}				
				return ;
			}
			if ( 8 != BitCount ) 
			{
				str.Format(_T("Error, Open File Fault [BitCount!=8](%s)"), filename);
				JetAPI::ShowMessageBox(str);

				for ( j=0; j<MaxPtrCount; j++ )
				{	JetMemory.free_func(PtrList[j]);	}				
				return ;
			}
		}
	}	
	
//	int    CastPhaseMode = LIGHT3D_PHASE_4_4_M;
//	IMAGE_SIZE ZeroW=0, ZeroH=0, ZeroStep=0;
//	Light3DCtrl.GetLight3DPhaseZero(LIGHT_3D_CAST_01, CastPhaseMode, ZeroW, ZeroH, ZeroStep, BasePhasePtr);	
//	if ( ZeroW!=ImageW || ZeroH!=ImageH || ZeroStep!=ImageStep )
//	{	BasePhasePtr = NULL; }

	double Gamma=1.0;	
	QueryPerformanceCounter(&m_nStartTime);
	switch ( FileCount )
	{
	case 3://1周期, 3張圖
		PatternID = PHASE_PATTERN_A;
		bIsOk = ImageAPI.GrayImage3FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PatternID, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr);
		break;
	case 4://1周期, 4張圖
		PatternID = PHASE_PATTERN_A;
		bIsOk = ImageAPI.GrayImage4FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PatternID, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr);
		break;
	case 5://1周期, 5張圖
		PatternID = PHASE_PATTERN_A;
		bIsOk = ImageAPI.GrayImage5FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], PatternID, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr);
		break;		
	case 6://2周期, 3 and 3
		bIsOk = ImageAPI.GrayImage3FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], P1, PtrList[3], PtrList[4], PtrList[5], P2, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);		
		break;
	case 8://2周期, 4 and 4
		bIsOk = ImageAPI.GrayImage4FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, PtrList[4], PtrList[5], PtrList[6], PtrList[7], P2, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);
		break;	
	case 10://2周期, 5 and 5
		bIsOk = ImageAPI.GrayImage5FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], P1, PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], P2, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);		
		break;
	case 9://3周期, 3 and 3 and 3
		bIsOk = ImageAPI.GrayImage3FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], P1, PtrList[3], PtrList[4], PtrList[5], P2, PtrList[6], PtrList[7], PtrList[8], P3, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);		
		break;
	case 12://3周期, 4 and 4 and 4
		bIsOk = ImageAPI.GrayImage4FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, PtrList[4], PtrList[5], PtrList[6], PtrList[7], P2, PtrList[8], PtrList[9], PtrList[10], PtrList[11], P3, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);		
		break;
	case 15://3周期, 5 and 5 and 5
		bIsOk = ImageAPI.GrayImage5FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], P1, PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], P2, PtrList[10], PtrList[11], PtrList[12], PtrList[13], PtrList[14], P3, Gamma, BasePhasePtr, NoiseParam, m_PhaseMaskPtr, m_PhasePtr, PeriodP);		
		break;
	}
	QueryPerformanceCounter(&m_nEndTime);
	BasePhasePtr = NULL;
	if ( false == bIsOk )		
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
		
		JetMemory.free_func(m_PhaseMaskPtr);		
		JetMemory.free_func(BasePhasePtr);			
		JetMemory.free_func(m_PhasePtr);		
		for ( j=0; j<MaxPtrCount; j++ )
		{	JetMemory.free_func(PtrList[j]);	}
		return;
	}
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f ms"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	
	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("MyMask"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, 8, m_PhaseMaskPtr, true);

	//if ( ImageAPI.CloneGrayImage(ImageW, ImageH, ImageStep, DstPtr, this->m_pImage, false) == false )//2D Image		
	bIsOk = ImageAPI.PhaseGrayImageConvertToGray(ImageW, ImageH, ImageStep, m_PhasePtr, ImageStep, this->m_pImage, false);
	if ( bIsOk == false )//Phase Image
	{	
		JetMemory.free_func(m_PhaseMaskPtr);		
		JetMemory.free_func(BasePhasePtr);
		JetMemory.free_func(m_PhasePtr);	
		for ( j=0; j<MaxPtrCount; j++ )
		{	JetMemory.free_func(PtrList[j]);	}

		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
		return;
	}
	JetMemory.free_func(BasePhasePtr);		

	for ( j=0; j<MaxPtrCount; j++ )
	{	JetMemory.free_func(PtrList[j]);	}

	this->m_ImageW = ImageW;
	this->m_ImageH = ImageH;
	this->m_BitCount = 8;
	this->m_ImageStep = ImageStep;	
	ImageAPI.CloneGrayImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, false);	
	this->m_strPixel = _T("");	
	this->m_ImageZoom = 1;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;
	this->SetDlgItemInt(IDC_ROI_CPX_EDIT, m_ImageW/2);
	this->SetDlgItemInt(IDC_ROI_CPY_EDIT, m_ImageH/2);
	this->m_Blob.Clear();	

	this->m_Dib.SetImage(m_pImage, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, true);	
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CImageDebugWnd::BuildSinPatternImage(int ImageW, int ImageH, int ImageStep, int PhaseStep, LPCTSTR Folder)
{	
	CString        filename;
	CString        folder = Folder;
	int            PhaseShift = 0;
	const int      P1 = 60;
	const int      P2 = 80;
	const int      P3 = 100;
	const bool     bVer = true;
	unsigned char *pRaw = NULL;		
	switch ( PhaseStep )
	{
	case 3:
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_1_000.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 120, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_1_120.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 240, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_1_240.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_2_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 120, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_2_120.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 240, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_2_240.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_3_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 120, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_3_120.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 240, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_3Step_3_240.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);		
		break;
	case 4:
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_1_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);
		
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 90, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_1_090.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_1_180.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_1_270.BMP"));
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_2_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 90, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_2_090.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_2_180.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_2_270.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);
		
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_3_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 90, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_3_090.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_3_180.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_4Step_3_270.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);
		break;
	case 5:
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 0, bVer);		
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_1_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 90, bVer);		
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_1_090.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_1_180.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_1_270.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P1, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_1_360.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_2_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 90, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_2_090.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_2_180.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_2_270.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P2, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_2_360.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);
		
		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_3_000.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 90, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_3_090.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 180, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_3_180.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 270, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_3_270.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);

		ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pRaw, P3, 0, bVer);
		filename.Format(_T("%s\\%s"), folder, _T("MyRawImage_5Step_3_360.BMP"));		
		ImageAPI.SaveBMPGrayImage(filename, ImageW, ImageH, ImageStep, pRaw, false);		
		break;
	}
	JetMemory.free_func(pRaw);
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSharpBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[] = "CImageDebugWnd::OnSharpBtn";

	CString str;	
	double ElapsedTime=0;
	const int Radius = 3;
	const int Threshold = this->GetDlgItemInt(IDC_SHARP_THRESHOLD_EDIT);
	const int Amount = this->GetDlgItemInt(IDC_SHARP_AMOUNT_EDIT);

	//const char fnName[] = __FUNCTION__;	
	//JetAPI::ShowMessageBox(fnName);	

	RECT Roi={0, 0, m_ImageW, m_ImageH};
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	QueryPerformanceCounter(&m_nStartTime);
	if ( this->m_BitCount == 8 )
	{
		//ImageAPI.Sharpness4GrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);
		//ImageAPI.Sharpness8GrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);
		//ImageAPI.SharpnessGausGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Radius, Threshold, Amount, m_pImageShow);
		ImageAPI.SharpnessLapsGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);
		//ImageAPI.Smooth1GrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow);
	}
	else if ( this->m_BitCount == 24 )
	{
		//ImageAPI.Sharpness4ColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);
		//ImageAPI.Sharpness8ColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);
		//ImageAPI.SharpnessGausColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Radius, Threshold, Amount, m_pImageShow);		
		ImageAPI.SharpnessLapsColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Threshold, Amount, m_pImageShow);		
	//	ImageAPI.Smooth1ColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow);						
	}
	//m_pImage[0] = 0xFF;
	//::memcpy(m_pImageShow, m_pImage, BufferSize);
	QueryPerformanceCounter(&m_nEndTime);

	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f ms"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnAverageBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[] = "CImageDebugWnd::OnAverageBtn";
	CString str;	
	double ElapsedTime=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	QueryPerformanceCounter(&m_nStartTime);
	//ImageAPI.AverageImage2Frame3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImage, m_pImageShow);	
	//ImageAPI.AverageImage3Frame3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImage, m_pImage, m_pImageShow);	
	ImageAPI.AverageImage4Frame3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImage, m_pImage, m_pImage, m_pImageShow);	
	QueryPerformanceCounter(&m_nEndTime);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f us"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnSobelBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[] = "CImageDebugWnd::OnSobelBtn";
	CString str;	
	double ElapsedTime=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	QueryPerformanceCounter(&m_nStartTime);	
	ImageAPI.SobelImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImageShow);	
	QueryPerformanceCounter(&m_nEndTime);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f us"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
	return;	
}
//-------------------------------------------------------------------------------------//
bool CImageDebugWnd::CalcPhasePeriod()
{
	if ( IMAGE_DEBUG_PROFILE != m_DebugMode )
	{	return true; }

	BITMAPINFO *pInfo = this->m_Dib.GetDIBInfo();
	if ( NULL == pInfo ) { return true; }


	RECT   WndRect={0};
	RECT   WndRect2={0};
	this->m_ProfileWnd.GetClientRect(&WndRect);

	CString str;
	POINT pt1={0};
	POINT pt2={0};
	TPOINT2D ImagePt1, ImagePt2;
	unsigned char *pSrc = this->m_Dib.GetDIBBits();
	const unsigned int ImageW = this->m_Dib.GetImageW();
	const unsigned int ImageH = this->m_Dib.GetImageH();
	const unsigned int BitCount = this->m_Dib.GetImageBitCount();
	const unsigned int BytePerLine = this->m_Dib.GetImageBytePerLine();
	std::vector<TPIXEL_GRY> ProfileGry;
	std::vector<TPIXEL_RGB> ProfileClr;
	unsigned char *pSobelImaeg = NULL;

	if ( ImageAPI.SobelImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, pSrc, pSobelImaeg) == false )
	{	return false; }

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseProfile.BMP"));
	ImageAPI.SaveBMPImage(str, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, pSobelImaeg, true);

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, ImagePt1);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, ImagePt2);

	ImagePt1.y = ImageH-ImagePt1.y;
	ImagePt2.y = ImageH-ImagePt2.y;
	WndRect2 = WndRect;
	if ( ImagePt1.x < ImagePt2.x )
	{	
		pt1.x = JetAPI::Round(ImagePt1.x);
		pt1.y = JetAPI::Round(ImagePt1.y);
		pt2.x = JetAPI::Round(ImagePt2.x); 
		pt2.y = JetAPI::Round(ImagePt2.y); 
	}
	else
	{
		pt1.x = JetAPI::Round(ImagePt2.x);
		pt1.y = JetAPI::Round(ImagePt2.y);
		pt2.x = JetAPI::Round(ImagePt1.x); 
		pt2.y = JetAPI::Round(ImagePt1.y);
	}	
	switch ( BitCount )
	{
	case 8:
		ImageAPI.CalcGrayImageProfile(ImageW, ImageH, BytePerLine, pSobelImaeg, pt1, pt2, ProfileGry);
		break;
	case 24:
		ImageAPI.CalcColorImageProfile(ImageW, ImageH, BytePerLine, pSobelImaeg, pt1, pt2, ProfileClr);
		break;
	}

	JetMemory.free_func(pSobelImaeg);
	::InflateRect(&WndRect2, -24, -24);

	//draw profile
	
	FILE *pfile =NULL;
	size_t i=0;
	POINT pt={0};
	double stepx=0, stepy=0;
	TPIXEL_GRY *PTBW8Ptr = NULL;
	TPIXEL_RGB *PTC24Ptr = NULL;
	
	const size_t size_gray = ProfileGry.size();
	const size_t size_color = ProfileClr.size();	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseProfile.txt"));
	pfile = ::_tfopen(str, TMode);
	if ( NULL != pfile )
	{
		::_ftprintf(pfile, _T("idx, x, y, gray\n"));
		for ( i=0; i<size_gray; i++ )
		{
			PTBW8Ptr = &(ProfileGry[i]);
			if ( NULL == PTBW8Ptr ) { continue; }
			::_ftprintf(pfile, _T("%d, %d, %d, %d\n"), i+1, PTBW8Ptr->x, PTBW8Ptr->y, PTBW8Ptr->gray);

		}
		::fclose(pfile);
		pfile = NULL;
	}

	bool First = true;
	int   lastIndex = -1;
	double   sx=0, sy=0;
	double   dx=0, dy=0, dl=0;
	const int Level = 200;
	const int Range = 10; 
	First = true;
	std::vector<TPIXEL_GRY> ProfileGry2;
	for ( i=0; i<size_gray; i++ )
	{
		PTBW8Ptr = &(ProfileGry[i]);
		if ( NULL == PTBW8Ptr ) { continue; }		
		if ( PTBW8Ptr->gray < Level )
		{	continue;	}
		ProfileGry2.push_back(*PTBW8Ptr);
	}

	int count = 0; 
	double sum = 0;
	int local_count=0;
	double local_dl=0;
	const size_t size_gray2 = ProfileGry2.size();
	
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseProfile2.txt"));
	pfile = ::_tfopen(str, TMode);
	if ( NULL != pfile )
	{
		::_ftprintf(pfile, _T("idx, x, y, gray\n"));
		for ( i=0; i<size_gray2; i++ )
		{
			PTBW8Ptr = &(ProfileGry2[i]);
			if ( NULL == PTBW8Ptr ) { continue; }
			::_ftprintf(pfile, _T("%d, %d, %d, %d\n"), i+1, PTBW8Ptr->x, PTBW8Ptr->y, PTBW8Ptr->gray);

		}
		::fclose(pfile);
		pfile = NULL;
	}

	TPIXEL_GRY pixelGry;
	std::vector<TPIXEL_GRY> ProfileGry3;
	for ( i=0; i<size_gray2-1; i++ )
	{
		dx = abs(ProfileGry2[i+1].x-ProfileGry2[i].x);
		dy = abs(ProfileGry2[i+1].y-ProfileGry2[i].y);
		dx = dx*dx;
		dy = dy*dy;
		dl = sqrt(dx+dy);
		if ( dl < Range ) 
		{	
			pixelGry.x += ProfileGry2[i].x;
			pixelGry.y += ProfileGry2[i].y;
			pixelGry.gray += ProfileGry2[i].gray;
			local_count ++;			
			continue;
		}
		if ( local_count  == 0 ) { continue; }

		pixelGry.x /= local_count;
		pixelGry.y /= local_count;
		pixelGry.gray /= local_count;
		ProfileGry3.push_back(pixelGry);
		local_count = 0;
		pixelGry = TPIXEL_GRY();		
	}

	const size_t size_gray3 = ProfileGry3.size();
	if ( size_gray3 > 1 )
	{	
		sum = 0;
		count = 0;
		for ( i=0; i<size_gray3-1; i++ )
		{
			dx = abs(ProfileGry3[i+1].x-ProfileGry3[i].x);
			dy = abs(ProfileGry3[i+1].y-ProfileGry3[i].y);
			dx = dx*dx;
			dy = dy*dy;
			dl = sqrt(dx+dy);
			sum += dl;
			count ++;
		}
		sum /= count;
	}	
	else 
	{	sum = 0;	}
	str.Format(_T("Count:%d\nPixel:%.2f"), size_gray3, sum);
	::AfxMessageBox(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnThresholdBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( 8 != this->m_BitCount ) { return; }

	CString str;	
	RECT Roi={0};
	int  Threshold = 0;
	double ElapsedTime=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);

	Roi.left = 0;
	Roi.top = 0;
	Roi.right = m_ImageW;
	Roi.bottom = m_ImageH;
	QueryPerformanceCounter(&m_nStartTime);	
	//ImageAPI.SobelImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImageShow);	
	//ImageAPI.CalcGrayImageOTSUThreshold(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, Threshold);
	ImageAPI.CalcGrayImageIsoDataThreshold(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, Threshold);
	
	QueryPerformanceCounter(&m_nEndTime);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f us, Threshold=%d"), ElapsedTime, Threshold);
	this->SetDlgItemText(IDC_INFO_EDIT, str);

	ImageAPI.BinaryGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, m_ImageStep, m_pImageShow, Threshold, 255);

	const int Gap = 4;
	const int CalcSize=64;	
	//ImageAPI.AdaptiveBinaryGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, m_ImageStep, m_pImageShow, CalcSize, Gap);
	this->RedrawWnd();
	return;
	
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnUnWrappingBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL==m_PhaseMaskPtr || NULL==m_PhasePtr ) { return; }

	CString str;
	double ElapsedTime=0;
	MASK_PTR  PhaseMaskPtr = NULL;//m_PhaseMaskPtr
	SPACE_PTR SpacePtr = NULL;
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;
	const IMAGE_SIZE ImageStep = m_ImageStep;
	const double SpaceRatio = -1;
	if ( Calc3DUnWarpping.CreateBuffer(ImageW, ImageH, ImageStep) == false )
	{
		JetAPI::ShowMessageBox(Calc3DUnWarpping.GetErrorString());
		return;
	}

	QueryPerformanceCounter(&m_nStartTime);	
	if ( Calc3DUnWarpping.ExecPhaseUnWrapping(ImageW, ImageH, ImageStep, m_PhasePtr, PhaseMaskPtr, SpacePtr) == false )
	{
		JetAPI::ShowMessageBox(Calc3DUnWarpping.GetErrorString());
		return;
	}
	QueryPerformanceCounter(&m_nEndTime);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f us"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);

	RECT RoiRect={0};
	RoiRect.left = 0;
	RoiRect.top = 0;
	RoiRect.right = (int)(ImageW);
	RoiRect.bottom = (int)(ImageH);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, SpacePtr, PhaseMaskPtr, RoiRect, m_ImageStep, this->m_pImage, SpaceRatio, false) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}

	::memcpy(m_pImageShow, m_pImage, sizeof(unsigned char)*BufferSize);
	this->m_Dib.SetImage(m_pImage, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, true);	
	JetMemory.free_func(SpacePtr);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnMaskWndBtn() 
{
	// TODO: Add your control notification handler code here
	CImageMaskWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::TestExtractSubRoiSubPix()
{
	//Extract Sub Image using Sub-Pixel
	CString   strDst;
	RECT      RoiRect;
	TRECT4D   RoiRect4D;
	IMAGE_PTR DstPtr=NULL;	
	IMAGE_PTR DstPtrR=NULL;	
	IMAGE_PTR DstPtrG=NULL;	
	IMAGE_PTR DstPtrB=NULL;	
	IMAGE_PTR SrcPtrR=NULL;	
	IMAGE_PTR SrcPtrG=NULL;	
	IMAGE_PTR SrcPtrB=NULL;		
	const int ImageCpX = m_ImageW/2;
	const int ImageCpY = m_ImageH/2;
	const IMAGE_SIZE RoiW = m_ImageW/2;
	const IMAGE_SIZE RoiH = m_ImageH/2;
	const IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, m_BitCount, 4);
	const IMAGE_SIZE MonoBitCount = 8;
	const IMAGE_SIZE MonoStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, MonoBitCount, 4);
	const IMAGE_SIZE MonoRoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, MonoBitCount, 4);
	const double dShiftX=0.75;
	const double dShiftY=0.25;

	RoiRect.left = ImageCpX-(RoiW/2);
	RoiRect.top = ImageCpY-(RoiH/2);
	RoiRect.right = RoiRect.left+RoiW;
	RoiRect.bottom = RoiRect.left+RoiH;

	RoiRect4D.left = ImageCpX-(RoiW/2);
	RoiRect4D.top = ImageCpY-(RoiH/2);
	RoiRect4D.right = RoiRect4D.left+RoiW;
	RoiRect4D.bottom = RoiRect4D.left+RoiH;

	RoiRect4D.left += dShiftX;
	RoiRect4D.top  += dShiftY;
	RoiRect4D.right += dShiftX;
	RoiRect4D.bottom  += dShiftY;	

	ImageAPI.ExtractRoiImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, RoiRect, RoiStep, DstPtr, false);	
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Roi.PNG"));
	ImageAPI.SavePNGImage(strDst, RoiW, RoiH, RoiStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.ExtractRoiImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, RoiRect4D, RoiStep, DstPtr, false);	
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Roi4D.PNG"));
	ImageAPI.SavePNGImage(strDst, RoiW, RoiH, RoiStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.ColorImageToRGBImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, SrcPtrR, SrcPtrG, SrcPtrB, MonoStep, false);
	ImageAPI.ExtractRGBRoiImage(m_ImageW, m_ImageH, MonoStep, SrcPtrR, SrcPtrG, SrcPtrB, RoiRect4D, RoiStep, DstPtr, false);	
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RGB-Roi4D.PNG"));
	ImageAPI.SavePNGImage(strDst, RoiW, RoiH, RoiStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.ExtractRGBRoiImage(m_ImageW, m_ImageH, MonoStep, SrcPtrR, SrcPtrG, SrcPtrB, RoiRect4D, MonoRoiStep, DstPtrR, DstPtrG, DstPtrB, false);	
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RGB-Roi4D-2.PNG"));
	ImageAPI.SavePNGImage(strDst, RoiW, RoiH, RoiStep, m_BitCount, DstPtr, false);

	JetMemory.free_func(SrcPtrR);
	JetMemory.free_func(SrcPtrG);
	JetMemory.free_func(SrcPtrB);
	JetMemory.free_func(DstPtrR);
	JetMemory.free_func(DstPtrG);
	JetMemory.free_func(DstPtrB);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::TestMorphImage()
{
	//形態運算	
	CString   strDst;
	IMAGE_PTR DstPtr=NULL;
	const int KenSize = 11;
	const int InterCount = 1;
	const int ShapeMode = MORPH_SHAPE_RECT;
	ImageAPI.MorphImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, MORPH_OPEN, ShapeMode, KenSize, InterCount, DstPtr);//先侵蝕再膨脹
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Open.PNG"));
	ImageAPI.SavePNGImage(strDst, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.MorphImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, MORPH_CLOSE, ShapeMode, KenSize, InterCount, DstPtr);//先膨脹再侵蝕
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Close.PNG"));
	ImageAPI.SavePNGImage(strDst, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.MorphImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, MORPH_GRADIENT, ShapeMode, KenSize, InterCount, DstPtr);//膨脹扣除侵蝕
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Gradient.PNG"));
	ImageAPI.SavePNGImage(strDst, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.MorphImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, MORPH_TOPHAT, ShapeMode, KenSize, InterCount, DstPtr);//膨脹扣除本身
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("TopHat.PNG"));
	ImageAPI.SavePNGImage(strDst, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);

	ImageAPI.MorphImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, MORPH_BLACKHAT, ShapeMode, KenSize, InterCount, DstPtr);//本身扣除侵蝕
	strDst.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BlackHat.PNG"));
	ImageAPI.SavePNGImage(strDst, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, DstPtr, false);
	JetMemory.free_func(DstPtr);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::TestRotateImage()
{
	//圖像旋轉測試
	CString    DstStr;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_PTR  DstPtrR=NULL;
	IMAGE_PTR  DstPtrG=NULL;
	IMAGE_PTR  DstPtrB=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;	
	if ( ImageAPI.RotateImage(0, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate000.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}
	if ( ImageAPI.RotateImage(90, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate090.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}
	if ( ImageAPI.RotateImage(180, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate180.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}
	if ( ImageAPI.RotateImage(270, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate270.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}

	if ( ImageAPI.RotateImage(30, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate030.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}	

	if ( ImageAPI.RotateImage(150, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate150.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}

	if ( ImageAPI.RotateImage(210, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate210.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}

	if ( ImageAPI.RotateImage(330, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, DstW, DstH, DstStep, DstPtr) == true )
	{
		DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Rotate330.PNG"));
		ImageAPI.SavePNGImage(DstStr, DstW, DstH, DstStep, m_BitCount, DstPtr, true);
		JetMemory.free_func(DstPtr);
	}

	if ( 8 == m_BitCount )
	{
		if ( ImageAPI.RotateRGBImage(0, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_000.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(90, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_090.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(180, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_180.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(270, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_270.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(30, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_030.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(150, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_150.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(210, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_210.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}

		if ( ImageAPI.RotateRGBImage(330, m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImage, m_pImage, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB) == true )
		{
			DstStr.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RotateRGB_330.BMP"));			
			ImageAPI.SaveBMPRGBImage(DstStr, DstW, DstH, DstStep, DstPtrR, DstPtrG, DstPtrB, true);
			JetMemory.free_func(DstPtrR);
			JetMemory.free_func(DstPtrG);
			JetMemory.free_func(DstPtrB);
		}
	}
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::TestMiMLibImage()
{
#ifdef MIM_LIB_USE
	CString str;
	char tempfilename[256]="";
	CMimImageBW8 ImageBW8;
	CMimImageBW8 ImageBW82;
	CMimImageC24 ImageC24;
	CMimImageC24 ImageC242;
	if ( 8 == m_BitCount )
	{		
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageSRCBW8.BMP");		
		str = tempfilename;
		this->m_Dib.Save(str);

		if ( ImageBW8.SetImagePtr(m_pImage, m_ImageW, m_ImageH, m_ImageStep, true) == false )
		{	JetAPI::ShowMessageBox(ImageBW8.GetErrorString()); }
		//if ( ImageBW8.LoadImage(tempfilename) == false )
		//{	JetAPI::ShowMessageBox(ImageBW8.GetErrorString());		}

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageBW8.BMP");		
		ImageBW8.SaveImage(tempfilename);

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageBW8.JPG");		
		ImageBW8.SaveImage(tempfilename);

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageBW8.PNG");		
		ImageBW8.SaveImage(tempfilename);		
	}
	else if ( 24 == m_BitCount )
	{		
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageSRCC24.BMP");		
		str = tempfilename;
		this->m_Dib.Save(str);

		if ( ImageC24.SetImagePtr(m_pImage, m_ImageW, m_ImageH, m_ImageStep, true) == false )
		{	JetAPI::ShowMessageBox(ImageBW8.GetErrorString()); }
		//if ( ImageBW8.LoadImage(tempfilename) == false )
		//{	JetAPI::ShowMessageBox(ImageBW8.GetErrorString());		}

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageC24.BMP");		
		ImageC24.SaveImage(tempfilename);

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageC24.JPG");		
		ImageC24.SaveImage(tempfilename);

		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageC24.PNG");		
		ImageC24.SaveImage(tempfilename);

		ImageC242 = ImageC24;
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "MiMImageC242.BMP");		
		ImageC242.SaveImage(tempfilename);
	}
#endif//MIM_LIB_USE	
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::TestJetImage()
{	
	int i = 0;
	CString str;
	char tempfilename[256]="";
	CJetImage Image1(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, true);
	CJetImage Image2 = Image1;
	CJetImage Image3;
	Image1.CloneImage(Image3);

	CvMat     *MatPtr=NULL;
	IplImage  *ImagePtr=NULL;
	if ( Image1.CloneImage(MatPtr) == true ) 
	{
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "CvMat.PNG");	
		::cvSaveImage(tempfilename, MatPtr);

		CJetImage Image4(MatPtr);
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CvMat2.PNG"));	
		Image4.SaveImage(str);

		::cvReleaseMat(&MatPtr);
	}

	if ( Image1.CloneImage(ImagePtr) == true ) 
	{
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "CvImage.PNG");	
		::cvSaveImage(tempfilename, ImagePtr);
		::cvReleaseImage(&ImagePtr);
	}

	unsigned char *Ptr = NULL;	
	CvMat *MatPtr16Bit = NULL;
	MatPtr16Bit = ::cvCreateMat(m_ImageH, m_ImageW, CV_MAKETYPE(CV_16U,3));
	Image1.CloneImage(MatPtr);
	if ( NULL != MatPtr16Bit && NULL!=MatPtr )
	{
		::cvConvertScale(MatPtr, MatPtr16Bit, 2);
		//::cvConvertImage(src, dst, floa
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "CvMat-16Bits.PNG");	
		::cvSaveImage(tempfilename, MatPtr16Bit);
		::cvReleaseMat(&MatPtr16Bit);

		MatPtr16Bit = cvLoadImageM( tempfilename, CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);
		if ( NULL != MatPtr16Bit )
		{
			::cvConvertScale(MatPtr16Bit, MatPtr, 0.5);
			::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "CvMat-8Bits.PNG");	
			::cvSaveImage(tempfilename, MatPtr);

			::cvReleaseMat(&MatPtr16Bit);
		}
		//cvCreateMatHeader
		//cvInitMatHeader
		//::cvReleaseMatHeader
	}

	//Test Mat Header Only
	CvMat *MatHeader = NULL;
	MatHeader = cvCreateMatHeader(m_ImageH, m_ImageW, CV_MAKETYPE(CV_8U,3));
	if ( NULL != MatHeader )
	{
		MatHeader->data.ptr = Image1.GetImagePtr();
		::sprintf(tempfilename, "%s\\%s", AOIDataCollect.GetAOITempDirectoryA(), "CvMatHeader.PNG");	
		::cvSaveImage(tempfilename, MatHeader);
		::cvReleaseMat(&MatHeader);//cvReleaseMatHeader
	}
	::cvReleaseMat(&MatPtr);

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CvMatHeader-2.PNG"));	
	Image1.SaveImage(str);	

	i = 1;
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnImage32bitBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	const int Align = 4;
	
	CString extname = dialog.GetFileExt();
	CString filename = dialog.GetPathName();
	const int idx = filename.ReverseFind(_T('\\'));
	CString foldername =  filename.Left(idx);

	this->SetWindowText(filename);	
	extname.MakeUpper();
	/*
	if ( ImageAPI.LoadImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, Align, true) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	ImageAPI.CloneImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImageShow, false);

	this->m_strPixel = _T("");	
	this->m_ImageZoom = 1;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;
	this->SetDlgItemInt(IDC_ROI_CPX_EDIT, m_ImageW/2);
	this->SetDlgItemInt(IDC_ROI_CPY_EDIT, m_ImageH/2);
	this->m_Blob.Clear();	

	this->m_Dib.SetImage(m_pImage, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, true);	
	this->RedrawWnd();
	*/
	
	char  afilename[MAX_JET_PATH]="";
	char  afilename2[MAX_JET_PATH]="";
	CvMat *MatPtrInput = NULL;
	CvMat *MatPtrOutput = NULL;
	const int AlphaRed = GetDlgItemInt(IDC_ALPHA_RED_EDIT);
	const int AlphaGrn = GetDlgItemInt(IDC_ALPHA_GREEN_EDIT);
	const int AlphaBlu = GetDlgItemInt(IDC_ALPHA_BLUE_EDIT);

	JetAPI::TCHAR2char(filename, afilename, MAX_JET_PATH);
	//IplImage * pImg = ::cvLoadImage(afilename, CV_LOAD_IMAGE_ANYCOLOR);//cvSaveImage
	//if ( NULL == pImg ) { return; }
	MatPtrInput = ::cvLoadImageM(afilename, CV_LOAD_IMAGE_ANYCOLOR);//cvSaveImage
	if ( NULL == MatPtrInput ) { return ; }	
	const int ImageW = MatPtrInput->cols;
	const int ImageH = MatPtrInput->rows;
	const int iStep = MatPtrInput->step;
	const int channels = CV_MAT_CN(MatPtrInput->type);
	unsigned char *iPtr = MatPtrInput->data.ptr;
	MatPtrOutput = ::cvCreateMat(ImageH, ImageW, CV_MAKETYPE(CV_8U,4));
	if ( NULL == MatPtrOutput ) 
	{ 
		::cvReleaseMat(&MatPtrInput);
		return ; 
	}
	int i=0, j=0;
	int iIdx=0, oIdx=0;
	const int oStep = MatPtrOutput->step;
	unsigned char *oPtr = MatPtrOutput->data.ptr;
	if ( 3 == channels )
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{
				iIdx = (i*iStep)+(j*3);
				oIdx = (i*oStep)+(j*4);

				oPtr[oIdx] = iPtr[iIdx];
				oPtr[oIdx+1] = iPtr[iIdx+1];
				oPtr[oIdx+2] = iPtr[iIdx+2];

				if ( AlphaRed != oPtr[oIdx+2] || 
					 AlphaGrn != oPtr[oIdx+1] ||
					 AlphaBlu != oPtr[oIdx] )
				{	oPtr[oIdx+3] = 255;	}
				else
				{	oPtr[oIdx+3] = 0;	}
			}
		}
	}
	if ( 1 == channels )
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{
				iIdx = (i*iStep)+(j);
				oIdx = (i*oStep)+(j*4);

				oPtr[oIdx] = iPtr[iIdx];
				oPtr[oIdx+1] = iPtr[iIdx];
				oPtr[oIdx+2] = iPtr[iIdx];

				if ( AlphaRed != oPtr[oIdx] )
				{	oPtr[oIdx+3] = 255;	}
				else
				{	oPtr[oIdx+3] = 0;	}
			}
		}
	}

	CString filename2;
	CString filename3;
	JetAPI::ExtractMainFileName(filename, filename2);
	filename3 = filename2 + CString(_T("-32Bit"));
	filename2.Format(_T("%s.%s"), filename3, extname);
	JetAPI::TCHAR2char(filename2, afilename2, MAX_JET_PATH);	
	::cvSaveImage(afilename2, MatPtrOutput);
	
	::cvReleaseMat(&MatPtrInput);
	::cvReleaseMat(&MatPtrOutput);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnImageHSVBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	const int Align = 4;
	
	CString extname = dialog.GetFileExt();
	CString filename = dialog.GetPathName();
	const int idx = filename.ReverseFind(_T('\\'));
	CString foldername =  filename.Left(idx);

	this->SetWindowText(filename);	
	extname.MakeUpper();
	/*
	if ( ImageAPI.LoadImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, Align, true) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	ImageAPI.CloneImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, m_pImageShow, false);

	this->m_strPixel = _T("");	
	this->m_ImageZoom = 1;
	this->m_ImageOffset.x = m_ImageOffset.y = 0;
	this->m_ImageWndPt1.x = -1;
	this->m_ImageWndPt1.y = -1;
	this->m_ImageWndPt2.x = -1;
	this->m_ImageWndPt2.y = -1;
	this->SetDlgItemInt(IDC_ROI_CPX_EDIT, m_ImageW/2);
	this->SetDlgItemInt(IDC_ROI_CPY_EDIT, m_ImageH/2);
	this->m_Blob.Clear();	

	this->m_Dib.SetImage(m_pImage, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, true);	
	this->RedrawWnd();
	*/
	
	//ImageAPI.RGBImageColorFilter3
	char  afilename[MAX_JET_PATH]="";
	char  afilename2[MAX_JET_PATH]="";
	//CvMat *PtrTemp = NULL;
	//CvMat *PtrInput = NULL;
	//CvMat *PtrOutput = NULL;
	IplImage *PtrTemp = NULL;
	IplImage *PtrInput = NULL;
	IplImage *PtrOutput = NULL;
	const int HSV_Hue = (int)(GetDlgItemInt(IDC_IMAGE_HSV_HUE_EDIT));
	const int HSV_Sat = (int)(GetDlgItemInt(IDC_IMAGE_HSV_SAT_EDIT));
	const int HSV_Val = (int)(GetDlgItemInt(IDC_IMAGE_HSV_VAL_EDIT));

	JetAPI::TCHAR2char(filename, afilename, MAX_JET_PATH);
	PtrInput = ::cvLoadImage(afilename, CV_LOAD_IMAGE_ANYCOLOR);//cvSaveImage
	if ( NULL == PtrInput ) { return; }
	//MatPtrInput = ::cvLoadImageM(afilename, CV_LOAD_IMAGE_ANYCOLOR);//cvSaveImage
	//if ( NULL == MatPtrInput ) { return ; }	

	const int ImageW = PtrInput->width;
	const int ImageH = PtrInput->height;
	const int ImageStep = PtrInput->widthStep;	
	const int ImageDepth = PtrInput->depth;
	const int ImageChannels = PtrInput->nChannels;
	if ( 1 == ImageChannels ) 
	{ 
		::cvReleaseImage(&PtrInput);
		return ; 
	}
	PtrTemp = ::cvCreateImage(cvSize(ImageW, ImageH), ImageDepth, ImageChannels);
	PtrOutput = ::cvCreateImage(cvSize(ImageW, ImageH), ImageDepth, ImageChannels);
	if ( NULL==PtrOutput || NULL==PtrTemp )
	{
		::cvReleaseImage(&PtrTemp);
		::cvReleaseImage(&PtrInput);
		::cvReleaseImage(&PtrOutput);
		return;
	}
	//COLOR_BGR2HSV
	//COLOR_RGB2HSV	
	cvCvtColor(PtrInput, PtrTemp, cv::COLOR_BGR2HSV);
	//unsigned char *iPtr = (unsigned char*)PtrTemp->imageData;
	//MatPtrOutput = ::cvCreateMat(ImageH, ImageW, CV_MAKETYPE(CV_8U,4));	
	int i=0, j=0;
	int H=0, S=0, V=0;
	int Idx=0, iIdx=0, oIdx=0;		
	unsigned char *tPtr = (unsigned char*)PtrTemp->imageData;
	for ( i=0; i<ImageH; i++ )
	{
		for ( j=0; j<ImageW; j++ )
		{
			if ( 16==i && 16==j ) 
			{
				i = i;
			}
			Idx = (i*ImageStep)+(j*3);
			H = tPtr[Idx];
			S = tPtr[Idx+1];
			V = tPtr[Idx+2];
			H += HSV_Hue;
			if ( H > 240 ) { H -= 240; }
			if ( H < 0 ) { H += 240; }

			S += HSV_Sat;
			if ( S > 255 ) { S = 255; }
			if ( S < 0 ) { S = 0; }

			V += HSV_Val;
			if ( V > 255 ) { V = 255; }
			if ( V < 0 ) { V = 0; }

			tPtr[Idx] = (unsigned char)(H);
			tPtr[Idx+1] = (unsigned char)(S);
			tPtr[Idx+2] = (unsigned char)(V);
		}
	}
	
	cvCvtColor(PtrTemp, PtrOutput, cv::COLOR_HSV2BGR);

	CString filename2;
	CString filename3;
	JetAPI::ExtractMainFileName(filename, filename2);
	filename3 = filename2 + CString(_T("-Modify"));
	filename2.Format(_T("%s.%s"), filename3, extname);
	JetAPI::TCHAR2char(filename2, afilename2, MAX_JET_PATH);	
	::cvSaveImage(afilename2, PtrOutput);
	
	::cvReleaseImage(&PtrTemp);
	::cvReleaseImage(&PtrInput);
	::cvReleaseImage(&PtrOutput);
}
//-------------------------------------------------------------------------------------//
void CImageDebugWnd::OnModifySaturationBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[] = "CImageDebugWnd::OnModifySaturationBtn";

	CString str;	
	double ElapsedTime=0;
	double wRG=1.0, wBG=1.0;	
	CWnd::GetDlgItemText(IDC_SATURATED_RED_EDIT, str);
	wRG = ::_ttof(str);
	CWnd::GetDlgItemText(IDC_SATURATED_BLUE_EDIT, str);
	wBG = ::_ttof(str);

	RECT Roi={0, 0, m_ImageW, m_ImageH};
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	QueryPerformanceCounter(&m_nStartTime);	
	if ( this->m_BitCount == 8 )
	{	ImageAPI.SaturateGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, wRG, wBG);	}
	else if ( this->m_BitCount == 24 )
	{	ImageAPI.SaturateColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, m_pImageShow, wRG, wBG);	}		
	//unsigned char* LutPtr = ImageAPI.BuildLocalGammaTable(2);//3ms
	//JetMemory.free_func(LutPtr);
	//float *IntegrapPtr=NULL;
	//ImageAPI.BuildIntegralColorImage(m_ImageW, m_ImageH, m_ImageStep, m_pImage, IntegrapPtr);
	//JetMemory.free_func(IntegrapPtr);
	//ImageAPI.SmoothImageByIntegral3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, 51, m_pImageShow);
	//ImageAPI.SmoothImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, 51, m_pImageShow);
		
	int CalcSize = AOIDataCollect.GetSystemParameter().m_ImageDisplayLocalGammaCalcSize;
	double ScaleVal = AOIDataCollect.GetSystemParameter().m_ImageDisplayLocalGammaScaleVal;
	ScaleVal = wRG;
	//ImageAPI.LocalGammaImage3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImage, CalcSize, ScaleVal, m_pImageShow);	

	if ( this->m_BitCount == 8 )
	{			
		//Roi.left = 1000;
		//Roi.right = m_ImageW-500;
		//Roi.top = 500;
		//Roi.bottom = m_ImageH-750;
		//ImageAPI.AdaptiveBinaryGrayImage3(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, m_ImageStep, m_pImageShow, 31, 5);	
		//ImageAPI.AdaptiveBinaryGrayImage3_Integral(m_ImageW, m_ImageH, m_ImageStep, m_pImage, Roi, m_ImageStep, m_pImageShow, 31, 5);			
	}	
	QueryPerformanceCounter(&m_nEndTime);	
	

//	AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_pImageShow, m_pImageShow);
	ElapsedTime = (m_nEndTime.QuadPart - m_nStartTime.QuadPart)*1000.0/m_nFreq.QuadPart;
	str.Format(_T("Time=%.0f ms"), ElapsedTime);
	this->SetDlgItemText(IDC_INFO_EDIT, str);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//