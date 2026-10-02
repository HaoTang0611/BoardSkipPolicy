// AlgPatternListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgPatternListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternListWnd dialog
//-------------------------------------------------------------------------------------//
CAlgPatternListWnd::CAlgPatternListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgPatternListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgPatternListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_BkColor = 0x000000;
	m_ImagePtr = NULL;
	m_ImageW = 256;
	m_ImageH = 256;
	m_ImageStep = 256;
	m_ImageBitCount = 8;

	m_ShowPtr = NULL;
	m_ShowW = 256;
	m_ShowH = 256;
	m_ShowStep = 256;
	m_ShowBitCount = 8;
	m_BitmapInfoPtr = NULL;

	m_WndPtr = NULL;
	m_Modified = false;	
	m_StopPatternListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgPatternListWnd)
	DDX_Control(pDX, PATLST_PATTERN_ICON_SIZE_COMBO, m_PatternIconSizeCombox);	
	DDX_Control(pDX, PATLST_INFO_LIST_WND, m_InfoListWnd);
	DDX_Control(pDX, PATLST_PATTERN_LIST_WND, m_PatternListWnd);
	DDX_Control(pDX, PATLST_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgPatternListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgPatternListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_BN_CLICKED(PATLST_GRAY_CHK, OnGrayChk)
	ON_BN_CLICKED(PATLST_BINARY_CHK, OnBinaryChk)
	ON_CBN_SELCHANGE(PATLST_PATTERN_ICON_SIZE_COMBO, OnSelchangePatternIconSizeCombo)
	ON_BN_CLICKED(PATLST_DELETE_BTN, OnDeleteBtn)
	ON_BN_CLICKED(PATLST_CLEAR_BTN, OnClearBtn)
	ON_BN_CLICKED(PATLST_SHOW_ROI_CHK, OnShowRoiChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgPatternListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	m_BkColor = 0x000000;
	m_ImageMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);

	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_InfoListWnd);	
	CWnd::CheckDlgButton(PATLST_SHOW_ROI_CHK, TRUE);

	CreateBKImageWnd();	
	BuildInfoListCtrlHeader(m_InfoListWnd);
	BuildInfoListCtrl(m_WndPtr, m_InfoListWnd);
	UpdateWndInfoText(m_WndPtr);
	BuildPatternIconSizeCombox();
	BuildPatternListCtrl(m_WndPtr, m_PatternListWnd);
	CWnd::SetDlgItemText(PATLST_FOLDER_INFO_EDIT, m_NativeFolder);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseImageBuffer();	
	ReleaseShowBuffer();
	ClearPatternListCtrl(m_PatternListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }
	if ( m_PatternListWnd.GetSafeHwnd() == NULL ) { return; }

	int BtnX = cx;
	int EditY=cy;
	CWnd *WndPtr = NULL;	
	POINT Offset={0,0};
	WndPtr = CWnd::GetDlgItem(PATLST_DELETE_BTN);
	if ( NULL != WndPtr ) 
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		Offset.x = cx-WndRect.right-8;
		Offset.y = 0;
		BtnX = cx-8-(WndRect.right-WndRect.left);
	}
	JetAPI::MoveCtrlWnd(this, IDOK, Offset);
	JetAPI::MoveCtrlWnd(this, IDCANCEL, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_DELETE_BTN, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_CLEAR_BTN, Offset);	
	JetAPI::MoveCtrlWnd(this, PATLST_PATTERN_ICON_SIZE_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_PATTERN_ICON_SIZE_COMBO, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_GRAY_CHK, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_BINARY_CHK, Offset);
	JetAPI::MoveCtrlWnd(this, PATLST_SHOW_ROI_CHK, Offset);

	WndPtr = CWnd::GetDlgItem(PATLST_WND_INFO_EDIT);
	if ( NULL != WndPtr ) 
	{
		SIZE WndSize={0,0};
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.bottom = cy-4;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect);
		EditY = WndRect.top;
	}
	
	if ( m_InfoListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_InfoListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);		
		WndRect.bottom = EditY-4;
		m_InfoListWnd.MoveWindow(&WndRect);
	}

	WndPtr = CWnd::GetDlgItem(PATLST_FOLDER_INFO_EDIT);
	if ( NULL != WndPtr ) 
	{
		SIZE WndSize={0,0};
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;
		WndSize.cy = WndRect.bottom-WndRect.top;
		WndRect.right = BtnX-8;
		WndRect.bottom = cy-4;
		WndRect.top = WndRect.bottom-WndSize.cy;
		WndPtr->MoveWindow(&WndRect);
		EditY = WndRect.top;
	}

	if ( m_PatternListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_PatternListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = BtnX-8;
		WndRect.bottom = EditY-4;
		m_PatternListWnd.MoveWindow(&WndRect);
	}
	//return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();	
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
BOOL CAlgPatternListWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CWnd *WndPtr = NULL;
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_DELETE:
			WndPtr = CWnd::GetFocus();
			if ( WndPtr == &m_PatternListWnd )			
			{	
				OnDeleteBtn();
				return TRUE;
			}
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CAlgPatternListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_PATTERN_LIST_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_PATTERN_LIST_WND;
	WndKey = _T("IDD_ALG_PATTERN_LIST_WND");
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
	WndID = PATLST_WND_INFO_LABEL;
	WndKey = _T("PATLST_WND_INFO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PATLST_PATTERN_ICON_SIZE_LABEL;
	WndKey = _T("PATLST_PATTERN_ICON_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATLST_GRAY_CHK;
	WndKey = _T("PATLST_GRAY_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATLST_BINARY_CHK;
	WndKey = _T("PATLST_BINARY_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATLST_SHOW_ROI_CHK;
	WndKey = _T("PATLST_SHOW_ROI_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATLST_DELETE_BTN;
	WndKey = _T("PATLST_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATLST_CLEAR_BTN;
	WndKey = _T("PATLST_CLEAR_BTN");
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
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CAlgPatternListWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_PATTERN_LIST_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::SetWndPtr(CAOIWnd *WndPtr)
{
	m_WndPtr = WndPtr;
	SetModified(false);
	return true;
}
//-------------------------------------------------------------------------------------//
inline CAOIWnd* CAlgPatternListWnd::GetWndPtr()
{
	return m_WndPtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::GetModified() const
{
	return m_Modified;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::SetModified(bool val)
{
	m_Modified = val;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::UpdateWndInfoText(CAOIWnd *WndPtr)
{
	CString str;
	const UINT CtrlID = PATLST_WND_INFO_EDIT;
	if ( NULL == WndPtr ) 
	{
		CWnd::SetDlgItemText(CtrlID, str);
		return true;
	}
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) 
	{
		CWnd::SetDlgItemText(CtrlID, str);
		return true;
	}

	CString ModelName = ModelPtr->GetModelName();
	ALG_TYPE      AlgType = WndPtr->GetWndAlgType();
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	CString strAlgType = AOIDataDefine.GetAlgTypeText(AlgType);
	CString strDefectID = AOIDataDefine.GetWndDefectIDText(WndDefectID);	
	
	str.Format(_T("%s, %s, %s"), ModelName, strDefectID, strAlgType);
	CWnd::SetDlgItemText(CtrlID, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::BuildInfoListCtrlHeader(CThisListCtrl_02 &ListCtrl)
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		ListCtrl.GetClientRect(&Rect);
		width = 64;
		width2 = (Rect.right-Rect.left-width-32);
		str = _T("Item");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("Name");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;
		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::BuildInfoListCtrl(CAOIWnd *WndPtr, CThisListCtrl_02 &ListCtrl)
{		
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	if ( NULL == WndPtr ) 
	{	return false;	}
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) 
	{	return false;	}
	
	CString   str;	
	int       nItem=0;
	const int nSubItem=1;	
	CString  ModelName = ModelPtr->GetModelName();
	ALG_TYPE      AlgType = WndPtr->GetWndAlgType();
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	CString strAlgType = AOIDataDefine.GetAlgTypeText(AlgType);
	CString strDefectID = AOIDataDefine.GetWndDefectIDText(WndDefectID);	

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);	
	
	//ModelName
	str = AOIDataDefine.GetModelText();
	ListCtrl.InsertItem(nItem, str);	
	ListCtrl.SetItemText(nItem, nSubItem, ModelName);
	nItem ++;

	//Defect ID
	str = AOIDataDefine.GetDefectText();
	ListCtrl.InsertItem(nItem, str);	
	ListCtrl.SetItemText(nItem, nSubItem, strDefectID);
	nItem ++;

	//Algorithm ID
	str = AOIDataDefine.GetAlgorithmText();
	ListCtrl.InsertItem(nItem, str);	
	ListCtrl.SetItemText(nItem, nSubItem, strAlgType);
	nItem ++;

	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::SetPatternFolder(LPCTSTR str)
{
	m_NativeFolder = str;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::SetImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr)
{	
	ReleaseImageBuffer();
	if ( BuildShowBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }	

	m_ImagePtr = ImagePtr;
	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_ImageBitCount = BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::ReleaseImageBuffer()
{
	IMAGE_SIZE ImageSize = 256;	
	m_ImagePtr = NULL;
	m_ImageW = ImageSize;
	m_ImageH = ImageSize;
	m_ImageStep = ImageSize;
	m_ImageBitCount = 8;		
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR  CAlgPatternListWnd::GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{	
	ImageW = m_ImageW;
	ImageH = m_ImageH;
	ImageStep = m_ImageStep;
	BitCount = m_ImageBitCount;
	return m_ImagePtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::ReleaseShowBuffer()
{
	if ( NULL != m_BitmapInfoPtr )
	{	
		delete[] m_BitmapInfoPtr;
		m_BitmapInfoPtr=NULL; 
	}

	if ( NULL != m_ShowPtr )
	{	JetMemory.free_func(m_ShowPtr); }
	
	m_ShowW = 256;
	m_ShowH = 256;
	m_ShowStep = 256;
	m_ShowBitCount = 8;
	m_ShowPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR CAlgPatternListWnd::GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_ShowW;
	ImageH = m_ShowH;
	ImageStep = m_ShowStep;
	BitCount = m_ShowBitCount;
	return m_ShowPtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::BuildShowBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	const char fnName[] = "CAlgPatternListWnd::BuildShowBuffer";
	ReleaseShowBuffer();
	if ( NULL == Ptr ) { return true; }
	
	int nAlign = 4;
	IMAGE_PTR   ShowPtr = NULL;
	IMAGE_SIZE  ShowW = ImageW;
	IMAGE_SIZE  ShowH = ImageH;	
	IMAGE_SIZE  ShowBitCount = 24;;
	IMAGE_SIZE  ShowStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ShowBitCount, nAlign);
	const size_t ShowSize = ImageAPI.CalcBufferSize(ShowStep, ShowH);
	if ( JetMemory.alloc_func(ShowSize, ShowPtr, fnName, "m_ShowPtr") == false ) 
	{	return false; }

	if ( ShowStep == ImageStep )
	{	::memcpy(ShowPtr, Ptr, sizeof(IMAGE_DATA)*ShowSize);	}
	else
	{
		if ( 8 == BitCount )
		{
			if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, Ptr, Ptr, Ptr, ShowStep, ShowPtr, false) == false )
			{
				JetMemory.free_func(ShowPtr);
				return false;
			}
		}
		if ( 24 == BitCount )
		{
			if ( ImageAPI.AlignColorImageBuffer3(ImageW, ImageH, ImageStep, Ptr, ShowStep, ShowPtr, false) == false )
			{
				JetMemory.free_func(ShowPtr);
				return false;
			}
		}
	}
	if ( AOIDataCollect.ExecEnhanceDisplayImage(ShowW, ShowH, ShowStep, ShowBitCount, ShowPtr, ShowPtr) == false ) 
	{
		JetMemory.free_func(ShowPtr);
		return false;
	}
	if ( ImageAPI.CreateBMPInfo(m_BitmapInfoPtr, ShowW, ShowH, ShowBitCount) == false ) 
	{
		JetMemory.free_func(ShowPtr);
		return false;
	}
	m_ShowPtr = ShowPtr;
	m_ShowW   = ShowW;
	m_ShowH   = ShowH;
	m_ShowStep= ShowStep;
	m_ShowBitCount= ShowBitCount;		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::RedrawWnd()
{
	DrawImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::DrawImageWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }

	RECT WndRect={0,0,0,0};
	m_ImageWnd.GetClientRect(&WndRect);		
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::CreateBKImageWnd()
{
	HDC hDC = m_ImageMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }

	RECT Rect={0,0,0,0};
	m_ImageWnd.GetClientRect(&Rect);		
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);		
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush=NULL;	
	}
	DrawImage(hDC, Rect);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::DrawImage(HDC hDC, const RECT &Rect)
{
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	IMAGE_SIZE   ImageBitCount = 0;

	ImagePtr = GetShowBuffer(ImageW, ImageH, ImageStep, ImageBitCount);
	if ( NULL == ImagePtr ) { return ; }
	BITMAPINFO *pInfo = m_BitmapInfoPtr;
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
	const double dRatio = 1.02;
	COLORREF BkColor = m_BkColor;
	
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, Rect, dRatio, dZoom);//計算影像視窗縮放參數	
	AOIDataCollect.LimitImageZoomScale(dZoom, true);
	ImageAPI.DrawImageToDC(hDC, pInfo, ImagePtr, Rect, OffsetPt2D, dZoom, BkColor);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::BuildPatternIconSizeCombox()
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		
	CComboBox &Combox = m_PatternIconSizeCombox;
	idx = 0;
	JetAPI::ClearCombox(Combox);		

	for ( i=0; i<8; i++ )
	{
		Param = 64*(i+1);
		str.Format(_T("%d"), Param);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;	
	}

	Combox.SetCurSel(2);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::ClearPatternListCtrl(CThisListCtrl_02 &ListCtrl)
{	
	m_StopPatternListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopPatternListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternListWnd::BuildPatternListCtrl(CAOIWnd *WndPtr, CThisListCtrl_02 &ListCtrl)
{
	ClearPatternListCtrl(ListCtrl);	
	if ( NULL == WndPtr )
	{	return true;	}
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	BOX_TOWARD PatternToward=WndToward;

	int          FrameW=4;
	int          FrameH=4;
	const int    IconW = JetAPI::GetComboxCurSelData(m_PatternIconSizeCombox);
	const int    IconH = JetAPI::GetComboxCurSelData(m_PatternIconSizeCombox);
	CDib         dib;	
	CBitmap      bmp;	
	HDC			 hMemDC = NULL;	
	HGDIOBJ		 hOldObj = NULL;	
	CPalette    *pPalette = NULL;
	HPALETTE	 hPalette = NULL;	
	BITMAPINFO   BitMapInfo;
	BITMAPINFO  *pBitMapInfo = NULL; 
	HBITMAP      hBitMap = NULL;	

	FrameW = (IconW/64)*4;
	FrameH = (IconH/64)*4;
	FrameW = 4;
	FrameH = 4;
	const int    IconWInner = IconW-FrameW-FrameW;
	const int    IconHInner = IconH-FrameH-FrameH;

	CSize        SpaceSize;
	COLORREF     clrMaskBk=RGB(0,0,0);
	COLORREF     clrMask=RGB(0,0,0);
	COLORREF     BKClr = 0x00;		
	CImageList  &ImageList = m_PatternImageList;		
	RECT         IconRect={0, 0, IconW, IconH};
	RECT         IconRectInner={FrameW, FrameH, IconW-FrameW, IconH-FrameH};
	const bool   bShowText = true;
	const bool   bShowGray = (bool)(CWnd::IsDlgButtonChecked(PATLST_GRAY_CHK));
	const bool   bShowBinary = (bool)(CWnd::IsDlgButtonChecked(PATLST_BINARY_CHK));
	const bool   bShowRoi = (bool)(CWnd::IsDlgButtonChecked(PATLST_SHOW_ROI_CHK));

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+24;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	

	size_t       i=0, j=0;
	size_t       PatRoiCount=0;
	bool         IsOK = true;
	bool         bGroupAll = false;
	int          nItem=0;	
	int          BmpAddResultID=0;
	int          nWidth=0, nHeight=0;
	int          nTmpW=0, nTmpH=0;
	int          nItem_W=0, nItem_H=0;
	int          nDW=0, nDH=0;	
	int          nRoiX=2, nRoiY=2;
	RECT         PatternRect={0,0,0,0};
	double       dPatZoom=1.0;
	double       dTmpRatio=0.0;	
	double       PatOffsetX=0.0;
	double       PatOffsetY=0.0;
	double       PatSkew=0.0;	
	double       PatScore=0.0;
	int          PolarityIdx=0;
	RESULT_ID    PatResultID;
	IMAGE_SIZE   PatternW=0;
	IMAGE_SIZE   PatternH=0;
	IMAGE_SIZE   PatternStep=0;
	IMAGE_SIZE   PatternBit=0;
	IMAGE_PTR    PatternPtr=NULL;
	MASK_DATA    mask = 0xFF;	
	IMAGE_DATA   mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;
	CPatternParam *PatParamPtr=NULL;
	
	int          nTextX = 8;
	int          nTextY = 8;
	bool         bIsOK = false;
	bool         bEnhanceImage=false;
	const int    nAlign = 4;
	const int    nTextYPitch = 16;	
	CString      strRoiInfo;
	CString      strInfoSize;
	CString      strInfoLand;		
	CString      strItemText;
	CString      PatternImageName;			
	const size_t PatternCount = AlgParam.GetAlgPatternCount();
	const size_t PatternParamCount = AlgParam.GetAlgPatternParamCount();;
	if ( PatternCount != PatternParamCount ) 
	{	return false; }
	
	HPEN hPenOK = NULL;
	HPEN hPenNG = NULL;
	HPEN hPenOld = NULL;
	COLORREF ColorOK=0x00FF00;
	COLORREF ColorNG=0x0000FF;
	COLORREF ColorOld=0;
	HBRUSH    hBrush = NULL;
	HBRUSH    hBrushOK = NULL;
	HBRUSH    hBrushNG = NULL;
	BKClr = ::GetSysColor(COLOR_BTNFACE);
	BKClr = 0x000000;//0xFFFFFF;
	hBrush = ::CreateSolidBrush(BKClr);
	hBrushOK = ::CreateSolidBrush(0x008F00);
	hBrushNG = ::CreateSolidBrush(0x00008F);
	hMemDC = ::CreateCompatibleDC(NULL);
	hPenOK = ::CreatePen(PS_SOLID, 1, ColorOK);
	hPenNG = ::CreatePen(PS_SOLID, 1, ColorNG);
	hPenOld = (HPEN)::SelectObject(hMemDC, hPenOK);
	
	::memset(&BitMapInfo, 0x00, sizeof(BitMapInfo));
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);		
	BitMapInfo.bmiHeader.biWidth = IconW;
	BitMapInfo.bmiHeader.biHeight = IconH;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 24;
	BitMapInfo.bmiHeader.biSizeImage = IconW*IconH*3;

	AOIDataCollect.GetMaskImageColor(FRAME_UNIQUE_ID_NULL, mskR, mskG, mskB, mskV, Alpha);
	//hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
	//hOldObj = ::SelectObject(hMemDC, hBitMap);				
	// set stretch mode
	::SetStretchBltMode(hMemDC, COLORONCOLOR);//HALFTONE
	::SetTextColor(hMemDC, 0x0080FF);
	::SetBkMode(hMemDC, TRANSPARENT);

	ListCtrl.SetRedraw(FALSE);
	m_StopPatternListBeSelected = true;
	for ( i=0; i<PatternCount; i++ )
	{	
		PatParamPtr = AlgParam.GetAlgPatternParamPtr(i, false);
		if ( NULL == PatParamPtr ) 
		{	continue;	}
		if ( AlgParam.LoadAlgPatternImage(i, PatternToward, PatternW, PatternH, PatternStep, PatternBit, PatternPtr) == false ) { continue; }
		if ( NULL == PatternPtr ) { continue; }

		bEnhanceImage = true;
		PolarityIdx = PatParamPtr->GetResultPolarityIdx();
		PatOffsetX = PatParamPtr->GetResultX(PolarityIdx);
		PatOffsetY = PatParamPtr->GetResultY(PolarityIdx);
		PatSkew = PatParamPtr->GetResultSkew(PolarityIdx);
		PatScore = PatParamPtr->GetResultReading(PolarityIdx);
		PatResultID = PatParamPtr->GetReultID(PolarityIdx);				
		const std::vector<TPATTERN_ROI> &PatternRoiList=PatParamPtr->GetPatRoiList(WndToward, PolarityIdx);

		strItemText.Format(_T("Pat#%d(S:%.0f, P:%d)"), i+1, PatScore, PolarityIdx+1);
		ListCtrl.InsertItem(nItem, strItemText, nItem);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, 0, strItemText);				
		nItem ++;

		nTextX = 8;
		nTextY = 8;
		strInfoSize = _T("");

		hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
		hOldObj = ::SelectObject(hMemDC, hBitMap);
		switch ( PatResultID ) 
		{
		case RESULT_ID_OK:
			::FillRect(hMemDC, &IconRect, hBrushOK);
			::FillRect(hMemDC, &IconRectInner, hBrush);
			break;
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			::FillRect(hMemDC, &IconRect, hBrushNG);
			::FillRect(hMemDC, &IconRectInner, hBrush);
			break;
		default:
			::FillRect(hMemDC, &IconRect, hBrush);
			break;
		}		
		
		CAlgBinaryParam BinaryParam = PatParamPtr->GetBinaryParam();
		if ( false!=bShowGray || false!=bShowBinary )
		{	
			RECT  Rect={0,0,0,0};
			MASK_PTR   MaskPtr=NULL;		
			IMAGE_PTR  GrayPtr=NULL;								
			const IMAGE_SIZE GrayBitCount = 8;
			const IMAGE_SIZE GrayStep = JetAPI::GetBMPImagePixelsPerLine(PatternW, GrayBitCount, nAlign);
			JetAPI::SizeToRect(PatternW, PatternH, Rect);
			if ( AlgParam.ExecAlgImageBinary(BinaryParam, PatternW, PatternH, PatternStep, PatternBit, PatternPtr, NULL, NULL, nAlign, GrayPtr, MaskPtr) == false )
			{
				PatternW = PatternH = 0;
				JetMemory.free_func(PatternPtr);
				continue;
			}
				
			if ( true == bShowGray )
			{
				bIsOK = ImageAPI.RGBImageToColorImage3(PatternW, PatternH, GrayStep, GrayPtr, GrayPtr, GrayPtr, PatternStep, PatternPtr, false);		
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				if ( false == bIsOK )
				{	
					PatternW = PatternH = 0;
					JetMemory.free_func(PatternPtr);
					continue;
				}
				bEnhanceImage = false;
				if ( AOIDataCollect.ExecEnhanceDisplayImage(PatternW, PatternH, PatternStep, PatternBit, PatternPtr, PatternPtr) == false ) 
				{ 
					PatternW = PatternH = 0;
					JetMemory.free_func(PatternPtr);				
					continue; 
				}
			}
			else if ( true==bShowBinary && BINARY_DISABLE!=BinaryParam.GetBinaryMode() )
			{
				bEnhanceImage = false;
				if ( AOIDataCollect.ExecEnhanceDisplayImage(PatternW, PatternH, PatternStep, PatternBit, PatternPtr, PatternPtr) == false ) 
				{
					PatternW = PatternH = 0;
					JetMemory.free_func(PatternPtr);				
					continue; 
				}
				if ( 24 == PatternBit )
				{	bIsOK = ImageAPI.ColorImageApplyMask(PatternW, PatternH, PatternStep, PatternPtr, Rect, GrayStep, MaskPtr, mask, mskR, mskG, mskB, Alpha);	}
				else if ( 8 == PatternBit )
				{	bIsOK = ImageAPI.GrayImageApplyMask(PatternW, PatternH, PatternStep, PatternPtr, Rect, GrayStep, MaskPtr, mask, mskV, Alpha);	}
				else
				{	bIsOK = false;	}
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
			}
			else
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
			}
		}		
		else
		{
			const double    OffsetValue=0.0;
			const double    GainValue = BinaryParam.GetGrayGainValue();
			const bool      GainEnabled=BinaryParam.CheckGrayGainEnabed();
		#ifdef PATTERN_BINARY_USE
			if ( true == GainEnabled )
			{	ImageAPI.ImageOffsetGain3(PatternW, PatternH, PatternStep, PatternBit, PatternPtr, PatternPtr, OffsetValue, GainValue); }
		#endif//PATTERN_BINARY_USE
		}
		if ( true == bEnhanceImage ) 
		{
			if ( AOIDataCollect.ExecEnhanceDisplayImage(PatternW, PatternH, PatternStep, PatternBit, PatternPtr, PatternPtr) == false ) 
			{ 
				PatternW = PatternH = 0;
				JetMemory.free_func(PatternPtr);				
				continue; 
			}
		}
		if ( dib.SetImage(PatternPtr, PatternW, PatternH, PatternStep, PatternBit, true) == false )
		{
			PatternW = PatternH = 0;
			JetMemory.free_func(PatternPtr);
			continue;
		}
		PatternW = PatternH = 0;
		JetMemory.free_func(PatternPtr);

		pBitMapInfo = dib.GetDIBInfo();
		nWidth = pBitMapInfo->bmiHeader.biWidth;
		nHeight = pBitMapInfo->bmiHeader.biHeight;

		nTmpW = nWidth;
		nTmpH = nHeight;
		dTmpRatio = 1.0;
		if( nTmpW > nTmpH )
		{
			dTmpRatio = nTmpH;
			dTmpRatio = dTmpRatio/nTmpW;
			nTmpW = IconWInner;
			nTmpH = (int)(nTmpW*dTmpRatio);
		}
		else
		{
			dTmpRatio = nTmpW;
			dTmpRatio = dTmpRatio/nTmpH;
			nTmpH = IconHInner;
			nTmpW = (int)(nTmpH*dTmpRatio);
		}
		nItem_W = nTmpW;
		nItem_H = nTmpH;				

		pPalette = dib.GetPalette();		
		if(pPalette != NULL)
		{
			hPalette = ::SelectPalette(hMemDC, (HPALETTE)pPalette->GetSafeHandle(), FALSE);
			::RealizePalette(hMemDC);	//maps entries from the current logical palette to the system palette.
		}		
		nDW = FrameW+(IconWInner-nItem_W)/2;
		nDH = FrameH+(IconHInner-nItem_H)/2;
		// populate the thumbnail bitmap bits
		::StretchDIBits(hMemDC, nDW, nDH, 
					nItem_W, nItem_H, 
					0, 0, 
					nWidth,
					nHeight, 
					dib.GetDIBBits(), 
					dib.GetDIBInfo(), 
					BI_RGB, 
					SRCCOPY);
		
		if ( true == bShowRoi ) 
		{
			dPatZoom = nItem_W*1.0/nWidth;
			PatRoiCount = PatternRoiList.size();
			for ( j=0; j<PatRoiCount; j++ )
			{
				const TPATTERN_ROI &PatternRoi = PatternRoiList[j];			
				PatternRect.left = (int)((PatternRoi.RoiRect.left)*dPatZoom);
				PatternRect.top  = (int)((PatternRoi.RoiRect.top)*dPatZoom);
				PatternRect.right= (int)((PatternRoi.RoiRect.right)*dPatZoom);
				PatternRect.bottom= (int)((PatternRoi.RoiRect.bottom)*dPatZoom);
				::OffsetRect(&PatternRect, nDW, nDH);

				if ( RESULT_ID_NG == PatternRoi.ResultID )
				{	
					::SelectObject(hMemDC, hPenNG); 
					ColorOld = ::SetTextColor(hMemDC, ColorNG);
				}
				else
				{	
					::SelectObject(hMemDC, hPenOK); 
					ColorOld = ::SetTextColor(hMemDC, ColorOK);
				}
				strRoiInfo.Format(_T("%.0f"), PatternRoi.ResultScore);
				::TextOut(hMemDC, PatternRect.left+nRoiX, PatternRect.top+nRoiY, strRoiInfo, strRoiInfo.GetLength());
				ImageAPI.DrawRectLine(hMemDC, PatternRect);

				::SetTextColor(hMemDC, ColorOld);
			}
		}

		
		if ( true == bShowText )
		{
			strRoiInfo=CString(PatParamPtr->GetPatText());
			::TextOut(hMemDC, nRoiX*2, nRoiY*2, strRoiInfo, strRoiInfo.GetLength());
		}

		// restore DC object
		::SelectObject(hMemDC, hOldObj);

		// restore DC palette
		if(pPalette != NULL)
		{	::SelectPalette(hMemDC, (HPALETTE)hPalette, FALSE); }		

		// clean up
		//::DeleteObject(hMemDC);	hMemDC = NULL;

		bmp.Attach(hBitMap);
		BmpAddResultID = ImageList.Add(&bmp, clrMask);			
		bmp.DeleteObject();		
	}
	m_StopPatternListBeSelected = false;
	
	// clean up
	::SelectObject(hMemDC, hPenOld);
	::DeleteObject(hPenOK);
	::DeleteObject(hPenNG);
	

	::DeleteObject(hMemDC);	hMemDC = NULL;
	::DeleteObject(hBrush); hBrush=NULL;
	::DeleteObject(hBrushOK); hBrushOK=NULL;
	::DeleteObject(hBrushNG); hBrushNG=NULL;

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnGrayChk() 
{
	// TODO: Add your control notification handler code here
	const bool   bShowGray = (bool)(CWnd::IsDlgButtonChecked(PATLST_GRAY_CHK));
	if ( true == bShowGray ) 
	{	CWnd::CheckDlgButton(PATLST_BINARY_CHK, FALSE);	}
	
	BuildPatternListCtrl(m_WndPtr, m_PatternListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnBinaryChk() 
{
	// TODO: Add your control notification handler code here
	const bool   bShowBinary = (bool)(CWnd::IsDlgButtonChecked(PATLST_BINARY_CHK));
	if ( true == bShowBinary ) 
	{	CWnd::CheckDlgButton(PATLST_GRAY_CHK, FALSE);	}

	BuildPatternListCtrl(m_WndPtr, m_PatternListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnSelchangePatternIconSizeCombo() 
{
	// TODO: Add your control notification handler code here
	BuildPatternListCtrl(m_WndPtr, m_PatternListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = AOIDataCollect.GetIsLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIWnd *WndPtr = GetWndPtr();
	if ( NULL == WndPtr ) { return; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return; }

	CThisListCtrl_02 &ListCtrl = m_PatternListWnd;
	POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
	if ( NULL == pos ) { return; }

	CString  str;
	str = _T("Do you want to delete the paterns ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return ; }

	size_t i=0;
	size_t Index=0;
	std::vector<size_t> IndexList;
	while ( pos )
	{
		int nItem = ListCtrl.GetNextSelectedItem(pos);
		if ( nItem < 0 ) { continue; }
		Index = ListCtrl.GetItemData(nItem);
		IndexList.push_back(Index);
	};
	std::sort(IndexList.begin(), IndexList.end());

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const size_t IndexCount=IndexList.size();
	for ( i=0; i<IndexCount; i++ )
	{
		Index = IndexList[IndexCount-i-1];
		AlgParam.DeleteAlgPattern(Index);
	}
	SetModified(true);
	ModelPtr->SetModelNeedSaveFiles(true);
	LogOperCtrl.SaveLogModelWndAlgPatternContentDelete(WndPtr);	
	//ModelPtr->ApplyModelWnd(WndPtr);
	//AOIDataCollect.CloseActiveComponent(ComponentPtr);
	BuildPatternListCtrl(WndPtr, ListCtrl);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = AOIDataCollect.GetIsLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }
	CAOIWnd *WndPtr = GetWndPtr();
	if ( NULL == WndPtr ) { return; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return ; }

	CString   str;
	CThisListCtrl_02 &ListCtrl = m_PatternListWnd;

	str = _T("Do you want to clear all paterns?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return ; }
	SetModified(true);
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.ClearAlgPatternFiles();
	ModelPtr->SetModelNeedSaveFiles(true);
	LogOperCtrl.SaveLogModelWndAlgPatternContentClearAll(WndPtr);
	//ModelPtr->ApplyModelWnd(WndPtr);
	//AOIDataCollect.CloseActiveComponent(ComponentPtr);		
	BuildPatternListCtrl(WndPtr, ListCtrl);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternListWnd::OnShowRoiChk() 
{
	// TODO: Add your control notification handler code here
	BuildPatternListCtrl(m_WndPtr, m_PatternListWnd);
}
//-------------------------------------------------------------------------------------//