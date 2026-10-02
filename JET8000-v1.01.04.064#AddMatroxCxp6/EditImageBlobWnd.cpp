// EditImageBlobWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditImageBlobWnd.h"
//-------------------------------------------------------------------------------------//
const int BLOB_COLUMN_ID_INDEX    = 0;
const int BLOB_COLUMN_ID_WIDTH    = 1;
const int BLOB_COLUMN_ID_HEIGHT   = 2;
const int BLOB_COLUMN_ID_LENGTH   = 3;
const int BLOB_COLUMN_ID_AREA     = 4;
const int BLOB_COLUMN_ID_ASPECT_RATIO  = 5;
const int BLOB_COLUMN_ID_FILL_RATIO = 6;
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
int CALLBACK BlobListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK BlobListCompareFn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if ( NULL == lParamSort ) { return 0; }
	CEditImageBlobWnd* pBlobListWnd = (CEditImageBlobWnd*)lParamSort;
	return pBlobListWnd->CompareBlobListWnd(lParam1, lParam2);
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageBlobWnd dialog
//-------------------------------------------------------------------------------------//
CEditImageBlobWnd::CEditImageBlobWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImageBlobWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageBlobWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;	
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;    
	m_BlobToward = BOX_TOWARD_NULL;

	m_ImagePtr = NULL;
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_ImageBitCount = 0;	
	
	m_MaskPtr = NULL;;
	m_MaskW = 1024;;
	m_MaskH = 1024;;
	m_MaskStep = 1024;;
	m_MaskBitCount = 8;;	

	m_ShowPtr = NULL;
	m_ShowW = 1024;
	m_ShowH = 1024;
	m_ShowStep = 1024;	
	m_ShowBitCount = 0;	

	m_BKColor = 0x00FFFF;
	m_ImageResX = 10.0;
	m_ImageResY = 10.0;
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	m_LastCursorPos.y = m_LastCursorPos.x = 0;
	m_CurrentCursorPos.y = m_CurrentCursorPos.x = 0;	
	
	m_BlobListSortMode = 1;
	m_BlobListWndColID = BLOB_COLUMN_ID_INDEX;
	m_StopBlobListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageBlobWnd)
	DDX_Control(pDX, BLOB_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, BLOB_LIST_WND, m_ListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageBlobWnd, CDialog)
	//{{AFX_MSG_MAP(CEditImageBlobWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, BLOB_LIST_WND, OnItemchangedListWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(BLOB_SHOW_RAW_IMAGE_CHK, OnShowRawImageChk)
	ON_BN_CLICKED(BLOB_UPDATE_WND_SELECTED_BTN, OnUpdateWndSelectedBtn)
	ON_NOTIFY(LVN_COLUMNCLICK, BLOB_LIST_WND, OnColumnclickListWnd)
	ON_WM_RBUTTONDBLCLK()	
	ON_BN_CLICKED(BLOB_SHOW_ALL_CHK, OnShowAllChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageBlobWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageBlobWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BKColor = 0xE0E0E0;
	m_MemDC1.CreateMemDC(&m_ImageWnd, m_BKColor);
	m_MemDC2.CreateMemDC(&m_ImageWnd, m_BKColor);
	m_ImageWnd.GetClientRect(&m_WndRect);
	JetAPI::InitialListCtrl(m_ListWnd);	

	SwitchMultiLanguage();
	BuildListWndHeader();
	BuildListWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyShowBuffer();
	DestroyMaskBuffer();
	DestroyImageBuffer();	
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	if ( m_ListWnd.GetSafeHwnd() != NULL )
	{
		int nGap = 0;
		RECT WndRect={0,0,0,0};
		m_ListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);		
		WndRect.left = nGap;
		WndRect.right = cx-nGap;
		WndRect.bottom = cy-nGap;
		m_ListWnd.MoveWindow(&WndRect);		
	}
	
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.top = 4;
		WndRect.left = 4;
		WndRect.right = cx-4;
		m_ImageWnd.MoveWindow(&WndRect);		
		m_MemDC1.CreateMemDC(&m_ImageWnd, m_BKColor);
		m_MemDC2.CreateMemDC(&m_ImageWnd, m_BKColor);
		m_ImageWnd.GetClientRect(&m_WndRect);
		CreateBKImage();
		RedrawWnd();
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		UpdateWndSelected();
		AOIDataCollect.SetEditImagePageWndID(WPARAM_SHOW_IMAGE_BLOB_PAGE);
	}
	else
	{
	}
}
//-------------------------------------------------------------------------------------//
BOOL CEditImageBlobWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageBlobWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_CLOSE:
			CloseProject();
			break;
		case WPARAM_PROJECT_SWITCH:			
			UpdateWndSelected();
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateWndSelected(); }
			}
			break;		
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
		default:			
			break;
		}		
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_IMAGE_MODEL_SELECTED:
		case WPARAM_UPDATE_IMAGE_WND_SELECTED:
		case WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS:
		case WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateWndSelected(); }
			}
			break;
		}
		break;
	case MSG_EDIT_VIEW_BLOB_WND:	
		switch ( wParam )
		{
		case WPARAM_UPDATE_BLOB_WND_SELECTED:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateWndSelected(); }
			}
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_BLOB_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_IMAGE_BLOB_WND;
	WndKey = _T("IDD_EDIT_IMAGE_BLOB_WND");
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
	WndID = BLOB_UPDATE_WND_SELECTED_BTN;
	WndKey = _T("BLOB_UPDATE_WND_SELECTED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BLOB_SHOW_ALL_CHK;
	WndKey = _T("BLOB_SHOW_ALL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BLOB_SHOW_RAW_IMAGE_CHK;
	WndKey = _T("BLOB_SHOW_RAW_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//			
	WndID = BLOB_XSIZE_LABEL;
	WndKey = _T("BLOB_XSIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BLOB_YSIZE_LABEL;
	WndKey = _T("BLOB_YSIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BLOB_AREA_LABEL;
	WndKey = _T("BLOB_AREA_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BLOB_FILL_RATIO_LABEL;
	WndKey = _T("BLOB_FILL_RATIO_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CEditImageBlobWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_BLOB_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::BuildListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT	
	{
		CThisListCtrl_08 &ListCtrl = m_ListWnd;

		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right-Rect.left-width-32);
		str = _T("idx");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = _T("X-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);		
		nCol ++;

		str = _T("Y-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;
	
		str = _T("L-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;	

		str = _T("Area");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*2);
		nCol ++;

		str = _T("Aspect");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*1.0);
		nCol ++;

		str = _T("Fill");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*1.0);
		nCol ++;

		str = _T("L/S");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*1.0);
		nCol ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::ClearListWnd()
{
	CThisListCtrl_08 &ListCtrl = m_ListWnd;
	m_StopBlobListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopBlobListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::BuildListWnd()
{
	ClearListWnd();
	
	size_t          i=0;
	CString         str;
	int             nItem=0;
	int             nSubItem=0;
	int             BlobW=0;
	int             BlobH=0;
	int             BlobA=0;
	double          BlobSizeW=0;
	double          BlobSizeH=0;
	double          BlobSizeD=0;
	double          BlobArea=0;
	double          BlobAspectRatio=0;
	double          BlobFillRatio=0.0;
	double          BlobLongShortRatio=0.0;
	double          ScaleX=1.0;
	double          ScaleY=1.0;
	COLORREF        clrNG=0x0000FF;
	COLORREF        clrOK=0x000000;
	RECT            BlobRect;
	TBlobObj        BlobObject;
	CThisListCtrl_08 &ListCtrl = m_ListWnd;
	const size_t BlobCount = m_BlobList.size();
	const BOOL bShowAll = CWnd::IsDlgButtonChecked(BLOB_SHOW_ALL_CHK);

	nItem=0;
	m_BlobListSortMode = 1;
	m_BlobListWndColID = BLOB_COLUMN_ID_INDEX;	
	m_StopBlobListBeSelected = true;
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<BlobCount; i++ )
	{
		BlobObject = m_BlobList[i];

		if ( FALSE == bShowAll )
		{
			if ( false == BlobObject.BlobMatchResult )
			{	continue; }
		}

		str.Format(_T("%d"), nItem+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);
		//ListCtrl.SetItemTextColor(nItem, 0, 0xFFFF00);

		BlobRect = BlobObject.BlobRect;
		BlobW = BlobRect.right-BlobRect.left+1;
		BlobH = BlobRect.bottom-BlobRect.top+1;
		BlobA = BlobObject.BlobPixels;
		BlobSizeW = BlobObject.BlobSizeW;
		BlobSizeH = BlobObject.BlobSizeH;
		BlobSizeD = BlobObject.BlobSizeD;
		BlobArea  = BlobObject.BlobSizeA;
		BlobAspectRatio = BlobObject.BlobAspectRatio;		
		BlobFillRatio = BlobObject.BlobFillRatio;
		BlobLongShortRatio = BlobObject.BlobLongShortRatio;

		nSubItem = 1;

		//Width
		str.Format(_T("%.0f"), BlobSizeW);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchSizeW ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;

		//Height
		str.Format(_T("%.0f"), BlobSizeH);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchSizeH ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;
	
		//Diag
		str.Format(_T("%.0f"), BlobSizeD);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchSizeH ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;	

		//Area
		str.Format(_T("%.0f"), BlobArea);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchSizeA ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;

		//Aspect Ratio 
		str.Format(_T("%.0f"), BlobAspectRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchAspectRatio ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;

		//Fill Ratio
		str.Format(_T("%.0f"), BlobFillRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchFillRatio ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;
		
		//Big Small Ratio
		str.Format(_T("%.0f"), BlobLongShortRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		if ( false == BlobObject.BlobMatchLongShortRatio ) 
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrNG); }
		else
		{	ListCtrl.SetItemTextColor(nItem, nSubItem, clrOK); }
		nSubItem ++;

		nItem ++;
	}	
	ListCtrl.SetRedraw(TRUE);
	m_StopBlobListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::RedrawWnd()
{
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC1 = m_MemDC1.GetSafeHdc();
	HDC hMemDC2 = m_MemDC2.GetSafeHdc();	
	if ( NULL == hDC ) { return; }	
	if ( NULL == hMemDC1 ) { return; }	
	if ( NULL == hMemDC2 ) { return; }	

	RECT WndRect={0,0,0,0};
	m_ImageWnd.GetClientRect(&WndRect);		
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC1, 0, 0, SRCCOPY );	
	DrawBlobList(hMemDC2);
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::CreateBKImage()
{
	HDC hDC = m_MemDC1.GetSafeHdc();
	if ( NULL == hDC ) { return; }

	RECT Rect={0,0,0,0};
	m_ImageWnd.GetClientRect(&Rect);		
	HBRUSH hBrush = ::CreateSolidBrush(m_BKColor);		
	::FillRect(hDC, &Rect, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;		

	DrawImage(hDC, Rect);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::DrawImage(HDC hDC, const RECT &Rect)
{
	if ( NULL == hDC ) { return; }
	if ( NULL == m_ShowPtr ) { return; }
	
	COLORREF clrBK = m_BKColor;	
	ImageAPI.DrawImageToDC(hDC, m_ShowW, m_ShowH, m_ShowStep, m_ShowBitCount, m_ShowPtr, m_WndRect, m_ImageOffset, m_ImageZoom, clrBK);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::DrawBlobList(HDC hDC)
{
	if ( NULL == hDC ) { return; }

	size_t       i=0;
	POINT        RectCp;
	TPOINT2D     WndPt1, WndPt2;
	TPOINT2D     ImagePt1, ImagePt2;
	TRECT4D      WndRect;
	TRECT4D      ImageRect;
	RECT         Rect={0,0,0,0};
	TBlobObj    *BlobPtr = NULL;	
	const size_t BlobCount = m_BlobList.size();
	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hPenSel = ::CreatePen(PS_DOT, 1, 0x00FFFF);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));

	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = &(m_BlobList[i]);
		if ( false == BlobPtr->BlobVisibled ) { continue; }
		ImageRect = BlobPtr->BlobRect;		
		ImageAPI.MapImageRectToWndRect_DBL(m_ShowW, m_ShowH, m_WndRect, m_ImageOffset, m_ImageZoom, ImageRect, WndRect);

		Rect.left = (int)(WndRect.left+0.5);
		Rect.top = (int)(WndRect.top+0.5);
		Rect.right = (int)(WndRect.right+0.5);
		Rect.bottom = (int)(WndRect.bottom+0.5);

		//if ( false == BlobPtr->BlobSelected ) 
		//{	::SelectObject(hDC, hPen);	}
		//else
		//{	::SelectObject(hDC, hPenSel);	}
		if ( false == BlobPtr->BlobMatchResult )
		{	::SelectObject(hDC, hPenNG);	}
		else
		{	::SelectObject(hDC, hPenOK);	}

		::MoveToEx(hDC, Rect.left, Rect.top, NULL);
		::LineTo(hDC, Rect.right, Rect.top);
		::LineTo(hDC, Rect.right, Rect.bottom);
		::LineTo(hDC, Rect.left, Rect.bottom);
		::LineTo(hDC, Rect.left, Rect.top);

		if ( true == BlobPtr->BlobSelected ) 
		{	
			RectCp.x = (Rect.left+Rect.right)/2;
			RectCp.y = (Rect.bottom+Rect.top)/2;

			::SelectObject(hDC, hPenSel);	
			::MoveToEx(hDC, m_WndRect.left, RectCp.y, NULL);
			::LineTo(hDC, Rect.left, RectCp.y);

			::MoveToEx(hDC, m_WndRect.right, RectCp.y, NULL);
			::LineTo(hDC, Rect.right, RectCp.y);

			::MoveToEx(hDC, RectCp.x, m_WndRect.top, NULL);
			::LineTo(hDC, RectCp.x, Rect.top);

			::MoveToEx(hDC, RectCp.x, m_WndRect.bottom, NULL);
			::LineTo(hDC, RectCp.x, Rect.bottom);
		}		
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenOK); hPenOK = NULL;
	::DeleteObject(hPenNG); hPenNG = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::DestroyShowBuffer()
{
	if ( NULL != m_ShowPtr )
	{	JetMemory.free_func(m_ShowPtr); }
	m_ShowPtr = NULL;
	m_ShowW = 1024;
	m_ShowH = 1024;
	m_ShowStep = 1024;
	m_ShowBitCount = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::DestroyMaskBuffer()
{
	if ( NULL != m_MaskPtr )
	{	JetMemory.free_func(m_MaskPtr); }
	m_MaskPtr = NULL;
	m_MaskW = 1024;
	m_MaskH = 1024;
	m_MaskStep = 1024;
	m_MaskBitCount = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::DestroyImageBuffer()
{
	if ( NULL != m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr); }
	m_ImagePtr = NULL;
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_ImageBitCount = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::CloseProject()
{
	CloseProjectKernel();
	CreateBKImage();	
	RedrawWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::CloseProjectKernel()
{	
	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	m_BlobToward = BOX_TOWARD_NULL;
	ClearListWnd();
	ClearBlobList();	
	DestroyShowBuffer();
	DestroyMaskBuffer();
	DestroyImageBuffer();		
	return;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CEditImageBlobWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::UpdateWndSelected()
{
	UpdateWndSelectedKernel();
	CreateBKImage();	
	RedrawWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::UpdateWndSelectedKernel()
{
	CloseProjectKernel();
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	if ( MODEL_ATTACHED_COMPONENT != ModelAttachedObj )
	{	return; }	

	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }

	m_ProjectPtr = ProjectPtr;
	m_BlobToward = WndPtr->GetWndToward();
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true == IsExceptionAngle )
	{	m_BlobToward = JetAPI::RotateToward(-AttachedAngle, m_BlobToward);	}

	if ( BuildImageBuffer(ModelPtr, WndPtr) == false )
	{	return ; }

	double ZoomRatio = 1.1;
	m_ImageOffset.x = m_ImageOffset.y = 0.0;
	ImageAPI.CalcImageWndFitZoom(m_ShowW, m_ShowH, m_WndRect, ZoomRatio, m_ImageZoom);//計算影
	
	m_ModelPtr = ModelPtr;	
	m_WndPtr = WndPtr;		
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::BuildImageBuffer(CAOIModel *ModelPtr, CAOIWnd *WndPtr)
{	
	const int nAlign = 4;
	const bool bClone = false;	
	const bool bExtend = false; 
	std::vector<TUNI_FRAME> WndUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
	CAlgBinaryParam &BinaryParam = AlgParam.GetAlgImageBinParam();	
	//const unsigned int FrameIndex = BinaryParam.GetBinaryFrameIndex();	
//#define DEBUG_CONNECTOR //Alan Debug
#ifdef DEBUG_CONNECTOR
	size_t FrameIndex;
	if (BinaryParam.GetBinaryFrameUniqueID() == 6) {
		FrameIndex = 6;
	}
	else {
		FrameIndex = BinaryParam.GetBinaryFrameIndex();
	}
#else
	const size_t FrameIndex = BinaryParam.GetBinaryFrameIndex();
#endif // DEBUG_CONNECTOR

	ClearBlobList();
	DestroyShowBuffer();
	DestroyMaskBuffer();
	DestroyImageBuffer();	
	//if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, WndPtr, WndUniFrameList) == false )	
	if ( AOIDataCollect.CreateWndUniFrameListByModel(bExtend, WndPtr, WndUniFrameList) == false )		
	{	return false; }
	const size_t WndFrameCount = WndUniFrameList.size();
	if ( FrameIndex<0 || FrameIndex>=WndFrameCount )
	{
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}
	RECT       WndRect={0,0,0,0};
	IMAGE_PTR  WndMaskPtr=NULL;
	TUNI_FRAME UniFrame = WndUniFrameList[FrameIndex];
	JetAPI::SizeToRect(UniFrame.ImageW, UniFrame.ImageH, WndRect);
	if ( AlgParam.ExecAlgUniFrameBinary_Rect(BinaryParam, WndRect, WndRect, UniFrame, nAlign, WndMaskPtr) == false )
	{
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}

	CString str;
	const char fnName[] = "CEditImageBlobWnd::BuildImageBuffer";		
	IMAGE_PTR  ImagePtr=NULL;	
	const IMAGE_SIZE ImageW = UniFrame.ImageW;
	const IMAGE_SIZE ImageH = UniFrame.ImageH;	
	const IMAGE_SIZE MaskBitCount = 8;	
	const IMAGE_SIZE ImageBitCount = UniFrame.BitCount;	
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, nAlign);
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ImageBitCount, nAlign);	
	const size_t ImageBufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(ImageBufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{
		JetMemory.free_func(WndMaskPtr);					
		JetMemory.free_func(ImagePtr);
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}

	if ( NULL==UniFrame.SpacePtr || NULL==UniFrame.MaskPtr )
	{
		if ( NULL == UniFrame.ImagePtr )
		{
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(ImagePtr);
			JetAPI::ClearUniFrameList(WndUniFrameList);
			return false;
		}
		if ( ImageStep == UniFrame.ImageStep )
		{	::memcpy(ImagePtr, UniFrame.ImagePtr, sizeof(IMAGE_DATA)*ImageBufferSize);	}
		else
		{
			if ( ImageAPI.AlignImageBuffer3(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.ImagePtr, ImageStep, ImagePtr, false) == false )
			{
				JetMemory.free_func(WndMaskPtr);
				JetMemory.free_func(ImagePtr);
				JetAPI::ClearUniFrameList(WndUniFrameList);
				return false;
			}
		}
	}
	else
	{
		const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
		if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, UniFrame.SpacePtr, UniFrame.MaskPtr, WndRect, ImageStep, ImagePtr, SpaceRatio, false) == false )
		{
			JetMemory.free_func(WndMaskPtr);
			JetMemory.free_func(ImagePtr);
			JetAPI::ClearUniFrameList(WndUniFrameList);
			return false;
		}
	}
	
	MASK_PTR      ShapePtr=NULL;
	IMAGE_SIZE    ShapeW=ImageW;
	IMAGE_SIZE    ShapeH=ImageH;			
	IMAGE_SIZE    ShapeBitCount=8;
	IMAGE_SIZE    ShapeStep=JetAPI::GetBMPImagePixelsPerLine(ShapeW, ShapeBitCount, 4);		
	const size_t  ShapeBufferSize = ImageAPI.CalcBufferSize(ShapeStep, ShapeH);
	const bool bWndNeesShapMask = WndPtr->CheckWndNeedShapeMask();
	if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, fnName, "ShapePtr") == true )	
	{
		RECT RoiRect={0,0,0,0};
		JetAPI::SizeToRect(ShapeW, ShapeH, RoiRect);
		::memset(ShapePtr, 0xFF, sizeof(MASK_DATA)*ShapeBufferSize);
		if ( true==bWndNeesShapMask && ShapeStep==MaskStep )
		{	
			if ( WndPtr->BuildWndShapeMask3(ShapeW, ShapeH, ShapeStep, ShapePtr, RoiRect) == true )
			{
			#ifdef _DEBUG
				str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BlobWndShapeMask.PNG"));
				ImageAPI.SaveImage(str, ShapeW, ShapeH, ShapeStep, ShapeBitCount, ShapePtr, true);
			#endif//_DEBUG				
			}			
		}
		if ( AlgParam.CheckAlgMaskBinFrameUsed() == true )
		{
			MASK_PTR LocMaskPtr=NULL;
			if ( JetMemory.alloc_func(ShapeBufferSize, LocMaskPtr, fnName, "LocMaskPtr") == true )	
			{
				CAlgBinaryParam &MaskBinParam = AlgParam.GetAlgMaskBinParam();						
				::memset(LocMaskPtr, 0xFF, sizeof(MASK_DATA)*ShapeBufferSize);
				AlgParam.ExecAlgUniFrameMaskBinary(ModelPtr, MaskBinParam, RoiRect, RoiRect, WndUniFrameList, ShapeW, ShapeH, ShapeStep, ShapeBitCount, LocMaskPtr, true);				
				MASK_FUNC_MODE  MaskFuncMode=MaskBinParam.GetMaskFuncMode();
				if ( MASK_FUNC_ERASE == MaskFuncMode )
				{	ImageAPI.InvertMaskImage3(ShapeW, ShapeH, ShapeStep, LocMaskPtr, RoiRect, ShapeStep, LocMaskPtr);	}
				ImageAPI.MergeMaskImage3(ShapeW, ShapeH, ShapeStep, ShapePtr, LocMaskPtr, RoiRect, MERGE_MASK_AND, ShapePtr);
				JetMemory.free_func(LocMaskPtr);
			}
		}
		ImageAPI.Intersection2MaskImage3(ShapeW, ShapeH, ShapeStep, WndMaskPtr, ShapePtr, RoiRect, ShapeStep, WndMaskPtr, 255, 0, true);
		JetMemory.free_func(ShapePtr);
	}
	JetAPI::ClearUniFrameList(WndUniFrameList);
#ifdef _DEBUG		
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BlobWndImage.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr, true);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("BlobWndMask.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, WndMaskPtr, true);	
#endif//_DEBUG
	
	m_ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);

	const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();
	if ( WndRoiCount > 0 )
	{		
		IMAGE_PTR  WndMskRoiPtr=NULL;		
		const IMAGE_SIZE WndRoiMaskW = ImageW;
		const IMAGE_SIZE WndRoiMaskH = ImageH;
		const IMAGE_SIZE WndRoiMaskStep = MaskStep;
		const IMAGE_SIZE WndRoiMaskBitCnt = MaskBitCount;
		const TALG_PARAM_BLOB_COUNT &blobParam = AlgParam.GetAlgParamBlobCount();
		const int  sX = JetAPI::ToInt(blobParam.bcRoiBoxExtendX/m_ImageResX);
		const int  sY = JetAPI::ToInt(blobParam.bcRoiBoxExtendY/m_ImageResY);
		const size_t WndMaskBufferSize = ImageAPI.CalcBufferSize(WndRoiMaskStep, WndRoiMaskH);
		if ( JetMemory.alloc_func(WndMaskBufferSize, WndMskRoiPtr, fnName, "WndMskRoiPtr") == true )
		{
			RECT WndRect2;
			const bool UsResPos = true;
			JetAPI::SizeToRect(WndRoiMaskW, WndRoiMaskH, WndRect2);
			if ( WndPtr->BuildWndRoiImage(WndRoiMaskW, WndRoiMaskH, WndRoiMaskStep, WndRoiMaskBitCnt, WndMskRoiPtr, WndRect2, sX, sY, UsResPos) == true )			
			{
				for ( size_t i=0; i<WndMaskBufferSize; i++ )
				{
					if ( 0 != WndMskRoiPtr[i] )
					{	continue;	}
					WndMaskPtr[i] = 0x00;
				}
			}
			JetMemory.free_func(WndMskRoiPtr);
		}
	}

	//設定遮罩資料
	m_MaskPtr = WndMaskPtr;
	m_MaskW = ImageW;
	m_MaskH = ImageH;
	m_MaskStep = MaskStep;
	m_MaskBitCount = MaskBitCount;	

	//設定影像資料
	m_ImagePtr = ImagePtr;
	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_ImageBitCount = ImageBitCount;	
	
	m_BlobParam = WndPtr->GetWndAlgParam().GetAlgParamBlobCount();
	BuildBlobList();
	BuildListWnd();
	BuildShowBuffer();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::BuildShowBuffer()
{
	DestroyShowBuffer();
	if ( NULL==m_MaskPtr || NULL==m_ImagePtr ) { return false; }
	
	BOOL bShowRaw = CWnd::IsDlgButtonChecked(BLOB_SHOW_RAW_IMAGE_CHK);
	const int nAlign = 4;
	const char fnName[] = "CEditImageBlobWnd::BuildImageBuffer";
	if ( FALSE == bShowRaw )
	{
		IMAGE_PTR Ptr=NULL;
		m_ShowW = m_MaskW;
		m_ShowH = m_MaskH;		
		m_ShowBitCount = 24;
		m_ShowStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowW, m_ShowBitCount, nAlign);
		const size_t BufferSize = ImageAPI.CalcBufferSize(m_ShowStep, m_ShowH);
		if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "m_ShowPtr") == false )
		{	return false; }
		ImageAPI.RGBImageToColorImage3(m_MaskW, m_MaskH, m_MaskStep, m_MaskPtr, m_MaskPtr, m_MaskPtr, m_ShowStep, Ptr, false);
		m_ShowPtr = Ptr;
	}
	else
	{
		IMAGE_PTR Ptr=NULL;
		m_ShowW = m_ImageW;
		m_ShowH = m_ImageH;		
		m_ShowBitCount = 24;
		m_ShowStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowW, m_ShowBitCount, nAlign);
		const size_t BufferSize = ImageAPI.CalcBufferSize(m_ShowStep, m_ShowH);
		if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "m_ShowPtr") == false )
		{	return false; }
		if ( 8 == m_ImageBitCount )
		{	ImageAPI.RGBImageToColorImage3(m_ImageW, m_ImageH, m_ImageStep, m_ImagePtr, m_ImagePtr, m_ImagePtr, m_ShowStep, Ptr, false);	}
		else
		{	::memcpy(Ptr, m_ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
		m_ShowPtr = Ptr;
		AOIDataCollect.ExecEnhanceDisplayImage(m_ShowW, m_ShowH, m_ShowStep, m_ShowBitCount, m_ShowPtr, m_ShowPtr);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::ClearBlobList()
{
	m_BlobList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::BuildBlobList()
{
	ClearListWnd();
	ClearBlobList();	
	SetActBlobInfo(NULL);
	if ( NULL == m_MaskPtr ) { return false; }	


	CJetBlob   BlobDetector;
	RECT       RoiRect={0,0,0,0};
	IMAGE_PTR  ImagePtr = m_MaskPtr;
	IMAGE_SIZE ImageW = m_MaskW;
	IMAGE_SIZE ImageH = m_MaskH;
	IMAGE_SIZE ImageStep = m_MaskStep;
	IMAGE_SIZE BitCount = m_MaskBitCount;	
	const TALG_PARAM_BLOB_COUNT &blobParam = m_BlobParam;

	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	if ( 8 == blobParam.bcConnectivity )//BLOB_CONNECTIVITY_4, BLOB_CONNECTIVITY_8
	{	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_8);	}
	else
	{	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	}
	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	if ( BlobDetector.GrayImageRoiBlobDetect(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, 128, 255) == false )
	{	return false;	}		


	size_t       i=0;
	int          idx=0;
	int          BlobW=0;
	int          BlobH=0;
	int          BlobA=0;	
	TBlobObj     BlobObj;
	TBlobResult *BlobPtr=NULL;		
	const double ScaleX=m_ImageResX;
	const double ScaleY=m_ImageResY;
	const BOX_TOWARD BlobToward = m_BlobToward;	
	const size_t BlobResCount = BlobDetector.GetBlobCount();	
	const int BlobMaxW = (int)(blobParam.bcXSizeMax);	
	const int BlobMinW = (int)(blobParam.bcXSizeMin);
	const int BlobMaxH = (int)(blobParam.bcYSizeMax);
	const int BlobMinH = (int)(blobParam.bcYSizeMin);
	const int BlobMaxD = (int)(blobParam.bcLSizeMax);
	const int BlobMinD = (int)(blobParam.bcLSizeMin);
	const int BlobMaxA = (int)(blobParam.bcAreaSizeMax);
	const int BlobMinA = (int)(blobParam.bcAreaSizeMin);
	const bool BlobMaxWEnabled = blobParam.bcXSizeMaxEnabled;
	const bool BlobMinWEnabled = blobParam.bcXSizeMinEnabled;
	const bool BlobMaxHEnabled = blobParam.bcYSizeMaxEnabled;
	const bool BlobMinHEnabled = blobParam.bcYSizeMinEnabled;
	const bool BlobMaxDEnabled = blobParam.bcLSizeMaxEnabled;
	const bool BlobMinDEnabled = blobParam.bcLSizeMinEnabled;
	const bool BlobMaxAEnabled = blobParam.bcAreaSizeMaxEnabled;
	const bool BlobMinAEnabled = blobParam.bcAreaSizeMinEnabled;
	const double BlobMaxAspectR = blobParam.bcAspectRatioMax;
	const double BlobMinAspectR = blobParam.bcAspectRatioMin;
	const bool BlobMaxAspectREnabled = blobParam.bcAspectRatioMaxEnabled;
	const bool BlobMinAspectREnabled = blobParam.bcAspectRatioMinEnabled;
	const double BlobMaxFillR = blobParam.bcFillRatioMax;
	const double BlobMinFillR = blobParam.bcFillRatioMin;
	const bool BlobMaxFillREnabled = blobParam.bcFillRatioMaxEnabled;
	const bool BlobMinFillREnabled = blobParam.bcFillRatioMinEnabled;
	const double BlobMaxLongShort = blobParam.bcLongShortRatioMax;
	const double BlobMinLongShort = blobParam.bcLongShortRatioMin;
	const bool BlobMaxLongShortEnabled = blobParam.bcLongShortRatioMaxEnabled;
	const bool BlobMinLongShortEnabled = blobParam.bcLongShortRatioMinEnabled;	

	idx = 0;
	for ( i=0; i<BlobResCount; i++ )
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if ( NULL == BlobPtr ) { continue; }

		BlobObj.BlobID = idx;
		//BlobObj.BlobRect = BlobPtr->m_BlobRectRaw;
		BlobObj.BlobRect = BlobPtr->m_BlobRect;//20230628-Blob		
		BlobObj.BlobPixels = BlobPtr->m_BlobPixels;
		BlobObj.BlobGCPosX = BlobPtr->m_BlobGCPosX;
		BlobObj.BlobGCPosY = BlobPtr->m_BlobGCPosY;

		BlobA = BlobObj.BlobPixels;
		BlobW = (BlobObj.BlobRect.right-BlobObj.BlobRect.left);//+1;
		BlobH = (BlobObj.BlobRect.bottom-BlobObj.BlobRect.top);//+1;

		BlobObj.BlobSizeW = BlobW*ScaleX;
		BlobObj.BlobSizeH = BlobH*ScaleY;
		BlobObj.BlobSizeA = BlobA*ScaleX*ScaleY;
		BlobObj.BlobAspectRatio = JetAPI::CalcBlobRatio(BlobW, BlobH, BlobToward)*100.0;
		BlobObj.BlobFillRatio = BlobObj.BlobSizeA*100.0/(BlobObj.BlobSizeW*BlobObj.BlobSizeH);
		BlobObj.BlobSizeD = sqrt((BlobObj.BlobSizeW*BlobObj.BlobSizeW)+(BlobObj.BlobSizeH*BlobObj.BlobSizeH));
		BlobObj.BlobLongShortRatio = BlobDetector.CalcBlobLongShortRatio(BlobObj.BlobSizeW, BlobObj.BlobSizeH)*100.0;

		BlobObj.BlobMatchSizeW = true;
		BlobObj.BlobMatchSizeH = true;
		BlobObj.BlobMatchSizeD = true;
		BlobObj.BlobMatchSizeA = true;
		BlobObj.BlobMatchAspectRatio = true;
		BlobObj.BlobMatchFillRatio = true;
		BlobObj.BlobMatchLongShortRatio = true;
		if ( true == BlobMaxWEnabled ) 
		{
			if ( BlobObj.BlobSizeW > BlobMaxW ) { BlobObj.BlobMatchSizeW = false; }
		}
		if ( true == BlobMinWEnabled ) 
		{
			if ( BlobObj.BlobSizeW < BlobMinW ) { BlobObj.BlobMatchSizeW = false; }
		}

		if ( true == BlobMaxHEnabled ) 
		{
			if ( BlobObj.BlobSizeH > BlobMaxH ) { BlobObj.BlobMatchSizeH = false; }
		}
		if ( true == BlobMinHEnabled ) 
		{
			if ( BlobObj.BlobSizeH < BlobMinH ) { BlobObj.BlobMatchSizeH = false; }
		}
	
		if ( true == BlobMaxDEnabled ) 
		{
			if ( BlobObj.BlobSizeD > BlobMaxD ) { BlobObj.BlobMatchSizeD = false; }
		}
		if ( true == BlobMinDEnabled ) 
		{
			if ( BlobObj.BlobSizeD < BlobMinD ) { BlobObj.BlobMatchSizeD = false; }
		}			

		if ( true == BlobMaxAEnabled ) 
		{
			if ( BlobObj.BlobSizeA > BlobMaxA ) { BlobObj.BlobMatchSizeA = false; }
		}
		if ( true == BlobMinAEnabled ) 
		{
			if ( BlobObj.BlobSizeA < BlobMinA ) { BlobObj.BlobMatchSizeA = false; }
		}
		
		if ( true == BlobMaxAspectREnabled ) 
		{
			if ( BlobObj.BlobAspectRatio > BlobMaxAspectR ) { BlobObj.BlobMatchAspectRatio = false; }
		}
		if ( true == BlobMinAspectREnabled ) 
		{
			if ( BlobObj.BlobAspectRatio < BlobMinAspectR ) { BlobObj.BlobMatchAspectRatio = false; }
		}

		if ( true == BlobMaxFillREnabled ) 
		{
			if ( BlobObj.BlobFillRatio > BlobMaxFillR ) { BlobObj.BlobMatchFillRatio = false; }
		}
		if ( true == BlobMinFillREnabled ) 
		{
			if ( BlobObj.BlobFillRatio < BlobMinFillR ) { BlobObj.BlobMatchFillRatio = false; }
		}

		if ( true==BlobMaxLongShortEnabled )
		{	
			if ( BlobObj.BlobLongShortRatio > BlobMaxLongShort ) { BlobObj.BlobMatchLongShortRatio = false; }
		}
		if ( true == BlobMinLongShortEnabled ) 
		{
			if ( BlobObj.BlobLongShortRatio < BlobMinLongShort ) { BlobObj.BlobMatchLongShortRatio = false; }
		}

		if ( false == BlobObj.BlobMatchSizeW || 
			 false == BlobObj.BlobMatchSizeH ||
			 false == BlobObj.BlobMatchSizeD ||
			 false == BlobObj.BlobMatchSizeA ||
			 false == BlobObj.BlobMatchAspectRatio ||
			 false == BlobObj.BlobMatchFillRatio ||
			 false == BlobObj.BlobMatchLongShortRatio )
		{	BlobObj.BlobMatchResult = false;	}
		else
		{	BlobObj.BlobMatchResult = true;	}
		
		m_BlobList.push_back(BlobObj);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnItemchangedListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopBlobListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	int Index = m_ListWnd.GetItemData(nItem);
	SelectBlobObj(Index);
	RedrawWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::SelectBlobObj(int Index)
{
	size_t i=0;
	const size_t BlobCount = m_BlobList.size();
	for ( i=0; i<BlobCount; i++ )
	{	m_BlobList[i].BlobSelected = false;	}
	if ( Index<0 || Index>=BlobCount ) { return true; }
	m_BlobList[Index].BlobSelected = true;
	SetActBlobInfo(&m_BlobList[Index]);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::SetActBlobInfo(const TBlobObj *BlobPtr)
{
	CString str;
	if ( NULL == BlobPtr )
	{
		str = _T("0");
		CWnd::SetDlgItemText(BLOB_XSIZE_EDIT, str);	
		CWnd::SetDlgItemText(BLOB_YSIZE_EDIT, str);	
		CWnd::SetDlgItemText(BLOB_AREA_EDIT, str);	
		CWnd::SetDlgItemText(BLOB_AREA_EDIT, str);	
		str = _T("0.00");
		CWnd::SetDlgItemText(BLOB_FILL_RATIO_EDIT, str);
		return true;
	}
	
	const double ScaleX=m_ImageResX;
	const double ScaleY=m_ImageResY;	
	const double BlobA_um=BlobPtr->BlobSizeA;
	const double BlobW_um=BlobPtr->BlobSizeW;
	const double BlobH_um=BlobPtr->BlobSizeH;
	const double BlobFillRatio = BlobPtr->BlobFillRatio;

	str.Format(_T("%.0f"), BlobW_um);
	CWnd::SetDlgItemText(BLOB_XSIZE_EDIT, str);
	str.Format(_T("%.0f"), BlobH_um);
	CWnd::SetDlgItemText(BLOB_YSIZE_EDIT, str);
	str.Format(_T("%.0f"), BlobA_um);
	CWnd::SetDlgItemText(BLOB_AREA_EDIT, str);
	str.Format(_T("%.0f"), BlobA_um);
	CWnd::SetDlgItemText(BLOB_AREA_EDIT, str);
	str.Format(_T("%.2f"), BlobFillRatio);
	CWnd::SetDlgItemText(BLOB_FILL_RATIO_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//

bool CEditImageBlobWnd::PickBlobObj(const POINT &pt)
{
	if ( NULL == m_ShowPtr ) { return true; }

	POINT    nPt;
	size_t   i=0;
	size_t   idx=-1;
	TPOINT2D WndPt = pt;
	TPOINT2D ImagePt;
	const size_t BlobCount = m_BlobList.size();

	ImageAPI.MapWndPtToImagePt_DBL(m_ShowW, m_ShowH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);

	idx = -1;
	nPt.x = (ImagePt.x);
	nPt.y = (ImagePt.y);
	for ( i=0; i<BlobCount; i++ )
	{	
		if ( ::PtInRect(&m_BlobList[i].BlobRect, nPt) == TRUE )
		{	
			idx = i;
			break;
		}
	}
	if ( -1 == idx ) { return true; }

	int   nItem=-1;	
	const size_t ItemCount = m_ListWnd.GetItemCount();

	nItem = -1;
	for ( i=0; i<ItemCount; i++ )
	{
		if ( m_ListWnd.GetItemData(i) == idx )
		{
			nItem = (int)(i);
			break; 
		}
	}

	if ( nItem >= 0 )
	{	
		m_ListWnd.SetItemState(nItem, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED);	
		m_ListWnd.SetFocus();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	POINT pt2 = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, BLOB_IMAGE_WND, &pt2) == false ) 
	{	
		CDialog::OnLButtonDown(nFlags, point);
		return; 
	}	
	PickBlobObj(pt2);	
	CDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( this != GetCapture() ) { return; }

	POINT dp;
	POINT pt = point;
	POINT pt2 = point;
	JetAPI::CheckPtInCtrlWnd(this, pt, BLOB_IMAGE_WND, &pt2);
	m_CurrentCursorPos = pt2;
	dp.x = m_CurrentCursorPos.x-m_LastCursorPos.x;
	dp.y = m_CurrentCursorPos.y-m_LastCursorPos.y;

	if (nFlags & MK_LBUTTON )//滑鼠左鍵
	{
	}

	if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		m_ImageOffset.x += dp.x;
		m_ImageOffset.y += dp.y;
		CreateBKImage();
		RedrawWnd();
	}
	m_LastCursorPos = m_CurrentCursorPos;
	CDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( WM_MOUSEWHEEL != message ) { return false; }
	CWnd *pWnd = GetFocus();
	if ( NULL == pWnd ) { return false; }
	if ( this != pWnd )
	{	pWnd = pWnd->GetParent();	}	
	if ( this != pWnd ) { return false; }

	CPoint pt, point;
	point.x = pt.x = GET_X_LPARAM(lParam); 
	point.y = pt.y = GET_Y_LPARAM(lParam); 
	this->ScreenToClient(&point);
	if ( JetAPI::CheckPtInCtrlWnd(this, point, BLOB_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBlobWnd::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;	
	CreateBKImage();	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CEditImageBlobWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	POINT pt2 = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, BLOB_IMAGE_WND, &pt2) == false ) 
	{	
		CDialog::OnRButtonDown(nFlags, point);
		return; 
	}	
	SetCapture();
	CWnd::SetFocus();
	m_CurrentCursorPos = m_LastCursorPos = pt2;	
	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	::ReleaseCapture();
	POINT pt = point;
	POINT pt2 = point;
	JetAPI::CheckPtInCtrlWnd(this, pt, BLOB_IMAGE_WND, &pt2);
	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnShowRawImageChk() 
{
	// TODO: Add your control notification handler code here
	BuildShowBuffer();
	CreateBKImage();	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnUpdateWndSelectedBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateWndSelected();
}
//-------------------------------------------------------------------------------------//
int CEditImageBlobWnd::CompareBlobListWnd(size_t index1, size_t index2)
{
	const int ColID = m_BlobListWndColID;
	const size_t BlobCount = m_BlobList.size();
	if ( index1>=BlobCount || index2>=BlobCount ) { return 0; }
	int     Res=0;
	CTime   time1;
	CTime   time2;	
	TBlobObj *Finder1=&(m_BlobList[index1]);
	TBlobObj *Finder2=&(m_BlobList[index2]);
	switch ( ColID )
	{
	case BLOB_COLUMN_ID_INDEX://filename
		if ( Finder1->BlobID > Finder2->BlobID ) 
		{	Res = 1; }
		else
		{	Res = 0; }		
		break;
	case BLOB_COLUMN_ID_WIDTH:
		if ( Finder1->BlobSizeW > Finder2->BlobSizeW ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;
	case BLOB_COLUMN_ID_HEIGHT:
		if ( Finder1->BlobSizeH > Finder2->BlobSizeH ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;
	case BLOB_COLUMN_ID_LENGTH:
		if ( Finder1->BlobSizeD > Finder2->BlobSizeD ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;
	case BLOB_COLUMN_ID_AREA:
		if ( Finder1->BlobSizeA > Finder2->BlobSizeA ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;
	case BLOB_COLUMN_ID_ASPECT_RATIO:
		if ( Finder1->BlobAspectRatio > Finder2->BlobAspectRatio ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;
	case BLOB_COLUMN_ID_FILL_RATIO:
		if ( Finder1->BlobFillRatio > Finder2->BlobFillRatio ) 
		{	Res = 1; }
		else
		{	Res = 0; }
		break;		
	default:
		Res = 0;
		break;
	}
	if ( 1 == m_BlobListSortMode ) 
	{	return Res; }
	if ( 0 == Res ) { Res = 1; }
	else { Res = 0; }
	return Res;	
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnColumnclickListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int ColumnsIdx = pNMListView->iSubItem;	
	if ( m_BlobListWndColID != ColumnsIdx )
	{	m_BlobListSortMode = 1; }
	else
	{
		if ( 0 == m_BlobListSortMode ) { m_BlobListSortMode = 1; }
		else {	m_BlobListSortMode = 0;  }
	}
	m_BlobListWndColID = ColumnsIdx;
	m_ListWnd.SortItems(BlobListCompareFn, (DWORD_PTR)this);

	const int nItem = m_ListWnd.GetNextItem(-1, LVNI_FOCUSED);
	if ( nItem >= 0 ) 
	{	m_ListWnd.EnsureVisible(nItem, FALSE); }
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	POINT pt2 = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, BLOB_IMAGE_WND, &pt2) == true ) 
	{	
		if ( NULL != m_ShowPtr )
		{
			double ZoomRatio = 1.1;
			m_ImageOffset.x = m_ImageOffset.y = 0.0;
			ImageAPI.CalcImageWndFitZoom(m_ShowW, m_ShowH, m_WndRect, ZoomRatio, m_ImageZoom);//計算影
			CreateBKImage();
			RedrawWnd();
		}
	}	
	CDialog::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageBlobWnd::OnShowAllChk() 
{
	// TODO: Add your control notification handler code here
	BuildListWnd();
}
//-------------------------------------------------------------------------------------//