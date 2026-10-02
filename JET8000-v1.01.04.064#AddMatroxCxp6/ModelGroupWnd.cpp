// ModelGroupWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ModelGroupWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelGroupWnd dialog
//-------------------------------------------------------------------------------------//
CModelGroupWnd::CModelGroupWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CModelGroupWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelGroupWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_ImageNameIndex = -1;
	m_DefaultModelType = MODEL_TYPE_NULL;
	m_StopGroupListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelGroupWnd)		
	DDX_Control(pDX, MGW_GROUP_LIST_WND, m_GroupListWnd);	
	DDX_Control(pDX, MGW_ICON_SIZE_COMBO, m_IconSizeCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CModelGroupWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CModelGroupWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, MGW_GROUP_LIST_WND, OnClickGroupListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, MGW_GROUP_LIST_WND, OnItemchangedGroupListWnd)
	ON_CBN_SELCHANGE(MGW_ICON_SIZE_COMBO, OnSelchangeIconSizeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelGroupWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CModelGroupWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//ShowWindow(SW_SHOWDEFAULT);
	SwitchMultiLanguage();
	BuildGroupList();
	BuildIconSizeCombox();	
	OnSelchangeIconSizeCombo();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::SetDlgItemText(MGW_GROUP_NAME_EDIT, m_DefaultGroupName);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_GroupListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_GroupListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_GroupListWnd.MoveWindow(&WndRect);
		m_GroupListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
BOOL CModelGroupWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ENHANCE_IMAGE:
			AOIDataCollect.ToggleIsEnhanceDisplayImage();
			//OnSelchangeIconSizeCombo();
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CModelGroupWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
MODEL_TYPE CModelGroupWnd::GetDefaultModelType() const
{
	return m_DefaultModelType;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::SetDefaultModelType(MODEL_TYPE Type)
{
	m_DefaultModelType = Type;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CModelGroupWnd::GetDefaultGroupName() const
{
	return m_DefaultGroupName;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::SetDefaultGroupName(LPCTSTR  Name)
{
	m_DefaultGroupName = Name;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CModelGroupWnd::GetDefaultImageName() const
{
	return m_DefaultImageName;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::SetDefaultImageName(LPCTSTR  Name)
{
	m_DefaultImageName = Name;
}
//-------------------------------------------------------------------------------------//
CString CModelGroupWnd::GetGroupNameSelected() const
{	
	if ( -1 == m_ImageNameIndex )
	{	return m_DefaultGroupName; }
	const int ImageCount = (int)(m_ImageNameList.size());
	if ( m_ImageNameIndex >= ImageCount )
	{	return m_DefaultGroupName; }

	CString GroupName;
	CString ImageName=m_ImageNameList[m_ImageNameIndex];
	if ( JetAPI::ExtractMainFileNameNoPath(ImageName, GroupName) == false )
	{	return m_DefaultGroupName; }
	return GroupName;
}
//-------------------------------------------------------------------------------------//
CString CModelGroupWnd::GetImageNameSelected() const
{
	if ( -1 == m_ImageNameIndex )
	{	return m_DefaultImageName; }
	const int ImageCount = (int)(m_ImageNameList.size());
	if ( m_ImageNameIndex >= ImageCount )
	{	return m_DefaultImageName; }
	return m_ImageNameList[m_ImageNameIndex];	
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MODEL_GROUP_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MODEL_GROUP_WND;
	WndKey = _T("IDD_MODEL_GROUP_WND");
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
	WndID = MGW_GROUP_NAME_LABEL;
	WndKey = _T("MGW_GROUP_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MGW_ICON_SIZE_LABEL;
	WndKey = _T("MGW_ICON_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	return;
}
//-------------------------------------------------------------------------------------//
CString CModelGroupWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MODEL_GROUP_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::OnClickGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::OnItemchangedGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopGroupListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	
	m_ImageNameIndex = (m_GroupListWnd.GetItemData(nItem));
	CString GroupName = GetGroupNameSelected();
	CWnd::SetDlgItemText(MGW_GROUP_NAME_EDIT, GroupName);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CModelGroupWnd::BuildIconSizeCombox()
{
	int          i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		
	CComboBox &Combox = m_IconSizeCombox;
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
bool CModelGroupWnd::BuildGroupList()
{
	m_ImageNameIndex=-1;
	std::vector<CString>  &ImageNameList = m_ImageNameList;
	ImageNameList.clear();
	
	CString ExtName = _T("MDL");
	std::vector<CString> FileList;
	MODEL_TYPE ModelType;
	MODEL_TYPE DefaultModelType = GetDefaultModelType();
	CString Folder = AOIDataCollect.GetAOIDefaultModelDirectory();	
	if ( JetAPI::ListFilesInFolder(Folder, ExtName, FileList) == false )
	{	return false; }

	size_t       i=0;
	CString      Filename;
	CString      ImageName;
	CAOIModel    ModelTmp;
	const size_t FileCount = FileList.size();
	for ( i=0; i<FileCount; i++ )
	{
		Filename.Format(_T("%s\\%s"), Folder, FileList[i]);
		if ( ModelTmp.LoadModelParamFile(Filename) == false )
		{	continue; }
		ModelType = ModelTmp.GetModelType();
		if ( ModelType != DefaultModelType ) { continue; }
		ImageName = ModelTmp.GetModelDefaultImageFilename();
		ImageNameList.push_back(ImageName);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelGroupWnd::ClearGroupListCtrl(CThisListCtrl_38 &ListCtrl)
{
	m_StopGroupListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopGroupListBeSelected = false;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelGroupWnd::BuildGroupListCtrl(CThisListCtrl_38 &ListCtrl)
{	
	const char fnName[] = "CModelGroupWnd::BuildGroupListCtrl";	
	ClearGroupListCtrl(ListCtrl);
	
	int          FrameW=4;
	int          FrameH=4;
	const int    IconW = JetAPI::GetComboxCurSelData(m_IconSizeCombox);
	const int    IconH = JetAPI::GetComboxCurSelData(m_IconSizeCombox);
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
	CImageList  &ImageList = m_GroupImageList;		
	RECT         IconRect={0, 0, IconW, IconH};
	std::vector<CString>  &ImageNameList = m_ImageNameList;
	RECT         IconRectInner={FrameW, FrameH, IconW-FrameW, IconH-FrameH};	

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//«Ø¥ßImage¦Cªí
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+24;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	

	size_t       i=0, j=0;	
	CString      ImageName;
	bool         IsOK = true;	
	int          nItem=0;	
	int          BmpAddResultID=0;
	int          nWidth=0, nHeight=0;
	int          nTmpW=0, nTmpH=0;
	int          nItem_W=0, nItem_H=0;
	int          nDW=0, nDH=0;	
	int          nRoiX=2, nRoiY=2;
	//RECT         PatternRect={0,0,0,0};
	double       dPatZoom=1.0;
	double       dTmpRatio=0.0;
	IMAGE_PTR    ImagePtr=NULL;
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount = 0;	
	const IMAGE_SIZE BlankW=64;
	const IMAGE_SIZE BlankH=64;
	const IMAGE_SIZE BlankStep=64;
	const IMAGE_SIZE BlankBitCnt=8;
	const size_t     BlankBufSize=BlankStep*BlankH;
	const size_t ImageNameCount = ImageNameList.size();	
	
	int          nTextX = 8;
	int          nTextY = 8;
	bool         bIsOK = false;
	bool         bEnhanceImage=false;
	const int    nAlign = 4;
	const int    nTextYPitch = 16;			
	CString      strItemText;		

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
	
	//hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
	//hOldObj = ::SelectObject(hMemDC, hBitMap);				
	// set stretch mode
	::SetStretchBltMode(hMemDC, COLORONCOLOR);//HALFTONE
	::SetTextColor(hMemDC, 0x0080FF);
	::SetBkMode(hMemDC, TRANSPARENT);
	
	ListCtrl.SetRedraw(FALSE);
	m_StopGroupListBeSelected = true;
	for ( i=0; i<ImageNameCount; i++ )
	{	
		ImageName = ImageNameList[i];		
		JetAPI::ExtractMainFileNameNoPath(ImageName, strItemText);
		ListCtrl.InsertItem(nItem, strItemText, nItem);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, 0, strItemText);
		nItem ++;

		nTextX = 8;
		nTextY = 8;

		hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
		hOldObj = ::SelectObject(hMemDC, hBitMap);
		::FillRect(hMemDC, &IconRect, hBrush);

		if ( ImageAPI.LoadImage(ImageName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true) == false )
		{
			ImageW = BlankW;
			ImageH = BlankH;
			ImageStep = BlankStep;
			BitCount = BlankBitCnt;
			if ( JetMemory.alloc_func(BlankBufSize, ImagePtr, fnName, "BlankPtr") == false )
			{
				JetMemory.free_func(ImagePtr);	ImagePtr = NULL;
				continue;		
			}
			::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BlankBufSize);
		}
		if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr) == false ) 
		{
			JetMemory.free_func(ImagePtr);	ImagePtr = NULL;
			continue;		
		}
		if ( dib.SetImage(ImagePtr, ImageW, ImageH, ImageStep, BitCount, true) == false )
		{
			JetMemory.free_func(ImagePtr);	ImagePtr = NULL;
			continue;	
		}		
		JetMemory.free_func(ImagePtr);	ImagePtr = NULL;

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
	m_StopGroupListBeSelected = false;		
	
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
void CModelGroupWnd::OnSelchangeIconSizeCombo() 
{
	// TODO: Add your control notification handler code here
	BuildGroupListCtrl(m_GroupListWnd);
}
//-------------------------------------------------------------------------------------//