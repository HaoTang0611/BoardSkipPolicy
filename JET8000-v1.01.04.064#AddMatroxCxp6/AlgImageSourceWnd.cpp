// AlgImageSourceWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgImageSourceWnd.h"
#include "AlgImageEdgeEnhanceWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageSourceWnd dialog
//-------------------------------------------------------------------------------------//
CAlgImageSourceWnd::CAlgImageSourceWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgImageSourceWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgImageSourceWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_UniFrameIndex = 0;	
	m_ImageSourceMode = IMAGE_SRC_GRAY;	
	m_StopIconListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgImageSourceWnd)
	DDX_Control(pDX, IMGSRC_ICON_SIZE_COMBO, m_IconSizeCombox);
	DDX_Control(pDX, IMGSRC_ICON_LIST_WND, m_IconListWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgImageSourceWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgImageSourceWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IMGSRC_ICON_SIZE_COMBO, OnSelchangeIconSizeCombo)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_01, OnFrameIDRad01)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_02, OnFrameIDRad02)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_03, OnFrameIDRad03)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_04, OnFrameIDRad04)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_05, OnFrameIDRad05)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_06, OnFrameIDRad06)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_07, OnFrameIDRad07)
	ON_BN_CLICKED(IMGSRC_FRAME_ID_RAD_08, OnFrameIDRad08)
	ON_NOTIFY(NM_CLICK, IMGSRC_ICON_LIST_WND, OnClickIconListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IMGSRC_ICON_LIST_WND, OnItemchangedIconListWnd)
	ON_BN_CLICKED(IMGSRC_EDGE_ENHANCE_BTN, OnEdgeEnhanceBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageSourceWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgImageSourceWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);	
	SwitchMultiLanguage();
	BuildIconSizeCombox();
	InitFrameRadionBtn();		
	OnSelchangeIconSizeCombo();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CString strFolder=GetSaveImageFolder();
	JetAPI::RemoveFolder(strFolder);
	JetAPI::ClearUniFrameList(m_UniFrameList);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_IconListWnd.GetSafeHwnd() != NULL  )
	{
		RECT WndRect={0};
		m_IconListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_IconListWnd.MoveWindow(&WndRect);
		m_IconListWnd.Arrange(LVA_ALIGNTOP);//LVA_ALIGNTOP
	}
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
BOOL CAlgImageSourceWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_ENHANCE_IMAGE:
			AOIDataCollect.ToggleIsEnhanceDisplayImage();
			OnSelchangeIconSizeCombo();
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CAlgImageSourceWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
unsigned int CAlgImageSourceWnd::GetUniFrameIndex() const
{
	return m_UniFrameIndex;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SetUniFrameIndex(unsigned int val)
{
	m_UniFrameIndex = val;
}
//-------------------------------------------------------------------------------------//
IMAGE_SRC_MODE CAlgImageSourceWnd::GetImageSourceMode() const
{
	return m_ImageSourceMode;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SetImageSourceMode(IMAGE_SRC_MODE val)
{
	m_ImageSourceMode = val;
}
//-------------------------------------------------------------------------------------//
EDGE_ENHANCE_MODE CAlgImageSourceWnd::GetEdgeEnhanceMode() const
{
	return m_AlgBinaryParam.GetEdgeEnhanceMode();
}
//-------------------------------------------------------------------------------------//
TBINARY_FILTER CAlgImageSourceWnd::GetEdgeEnhanceFilter1() const
{
	return m_AlgBinaryParam.GetEdgeEnhanceFilter1();
}
//-------------------------------------------------------------------------------------//
TBINARY_FILTER CAlgImageSourceWnd::GetEdgeEnhanceFilter2() const
{
	return m_AlgBinaryParam.GetEdgeEnhanceFilter2();
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList)
{
	const char fnName[] = "CAlgImageSourceWnd::SetUniFrameList";
	if ( JetAPI::CloneUniFrameList(fnName, UniFrameList, m_UniFrameList) == false )
	{
		JetAPI::ClearUniFrameList(m_UniFrameList);
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SetFrameUniqueIDList(const std::vector<unsigned int> &FrameUniqueIDList)
{
	m_FrameUniqueIDList = FrameUniqueIDList;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SetAlgBinaryParam(const CAlgBinaryParam &BinaryParam)
{
	m_AlgBinaryParam = BinaryParam;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_IMAGE_SOURCE_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_IMAGE_SOURCE_WND;
	WndKey = _T("IDD_ALG_IMAGE_SOURCE_WND");
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
	WndID = IMGSRC_ICON_SIZE_LABEL;
	WndKey = _T("IMGSRC_ICON_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = IMGSRC_FRAME_ID_RAD_01;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_01");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_02;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_02");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_03;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_03");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_04;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_04");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_05;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_05");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_06;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_06");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_07;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_07");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGSRC_FRAME_ID_RAD_08;
	WndKey = _T("IMGSRC_FRAME_ID_RAD_08");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IMGSRC_EDGE_ENHANCE_BTN;
	WndKey = _T("IMGSRC_EDGE_ENHANCE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	*/
}
//-------------------------------------------------------------------------------------//
CString CAlgImageSourceWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_IMAGE_SOURCE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::BuildIconSizeCombox()
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
	Combox.SetCurSel(3);
	return;
}
//-------------------------------------------------------------------------------------//
void  CAlgImageSourceWnd::InitFrameRadionBtn()
{
	
	UINT    CtrlID=0;
	BOOL    bEnable=FALSE;
	const size_t FrameUniqueIDCount = m_FrameUniqueIDList.size();
	switch ( FrameUniqueIDCount )
	{
	case 0:		
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_01, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_02, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_03, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_04, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_05, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 1:		
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_02, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_03, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_04, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_05, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 2:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_03, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_04, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_05, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 3:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_04, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_05, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 4:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_05, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 5:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_06, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 6:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_07, bEnable);
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	case 7:
		JetAPI::ShowCtrlWnd(this, IMGSRC_FRAME_ID_RAD_08, bEnable);
		break;
	}

	const int Index = GetUniFrameIndex();
	switch ( Index )
	{
	case 0:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_01, TRUE);	break;
	case 1:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_02, TRUE);	break;
	case 2:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_03, TRUE);	break;
	case 3:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_04, TRUE);	break;
	case 4:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_05, TRUE);	break;
	case 5:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_06, TRUE);	break;
	case 6:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_07, TRUE);	break;
	case 7:	CWnd::CheckDlgButton(IMGSRC_FRAME_ID_RAD_08, TRUE);	break;
	}
	
	size_t       i=0;
	CString      str;
	unsigned int FrameUniqueID=0;
	TFrameParam *FrameParamPtr = NULL;
	for ( i=0; i<FrameUniqueIDCount; i++ )
	{
		FrameUniqueID = m_FrameUniqueIDList[i];
		if ( FRAME_UNIQUE_ID_NULL == FrameUniqueID ) { continue; }
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
		if ( NULL == FrameParamPtr ) { continue; }
		str = FrameParamPtr->FrameName;
		switch ( i )
		{
		case 0:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_01, str);	break;
		case 1:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_02, str);	break;
		case 2:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_03, str);	break;
		case 3:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_04, str);	break;
		case 4:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_05, str);	break;
		case 5:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_06, str);	break;
		case 6:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_07, str);	break;
		case 7:	CWnd::SetDlgItemText(IMGSRC_FRAME_ID_RAD_08, str);	break;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnSelchangeIconSizeCombo() 
{
	// TODO: Add your control notification handler code here	
	BuildImageSourceListCtrl(m_UniFrameIndex, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad01() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(0, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad02() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(1, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad03() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(2, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad04() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(3, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad05() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(4, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad06() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(5, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad07() 
{
	// TODO: Add your control notification handler code here
	BuildImageSourceListCtrl(6, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnFrameIDRad08() 
{
	// TODO: Add your control notification handler code here		
	BuildImageSourceListCtrl(7, m_IconListWnd);
}
//-------------------------------------------------------------------------------------//
CString CAlgImageSourceWnd::GetSaveImageFolder() const
{
	CString str;
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ImageSource"));
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageSourceWnd::ClearImageSourceListCtrl(CThisListCtrl_37 &ListCtrl)
{
	m_StopIconListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopIconListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgImageSourceWnd::BuildImageSourceListCtrl(int index, CThisListCtrl_37 &ListCtrl)
{
	const char fnName[] = "CAlgImageSourceWnd::BuildImageSourceListCtrl";
	const size_t UniFrameCount = m_UniFrameList.size();
	ClearImageSourceListCtrl(ListCtrl);	
	if ( index<0 || index>=UniFrameCount)
	{	return false; }		
	
	int          FrameW=4;
	int          FrameH=4;
	TUNI_FRAME   UniFrame=m_UniFrameList[index];
	unsigned int FrameUniqueID=m_FrameUniqueIDList[index];
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
	CImageList  &ImageList = m_IconImageList;		
	RECT         IconRect={0, 0, IconW, IconH};
	RECT         IconRectInner={FrameW, FrameH, IconW-FrameW, IconH-FrameH};	

	ImageList.DeleteImageList();	
	ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);
	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+24;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	

	size_t       i=0, j=0;	
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
	IMAGE_PTR    BufferPtr=NULL;
	IMAGE_PTR    ImagePtr=UniFrame.ImagePtr;	
	const IMAGE_SIZE ImageW=UniFrame.ImageW;
	const IMAGE_SIZE ImageH=UniFrame.ImageH;
	const IMAGE_SIZE ImageStep=UniFrame.ImageStep;
	const IMAGE_SIZE BitCount = UniFrame.BitCount;
	const IMAGE_SIZE ColorBit = 24;
	const IMAGE_SIZE ColorStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, ColorBit, 4);
	const size_t BufferSize=ImageAPI.CalcBufferSize(ColorStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == false )
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());
		return false;
	}	
	
	int          nTextX = 8;
	int          nTextY = 8;
	int          nActIconIndex=0;
	bool         bIsOK = false;
	bool         bEnhanceImage=false;
	const int    nAlign = 4;
	const int    nTextYPitch = 16;			
	CString      str;
	CString      Filename;
	CString      strItemText;	
	IMAGE_SRC_MODE ImageSrcMode;
	CAlgParam         AlgParam;
	CString           strFolder=GetSaveImageFolder();
	CAlgBinaryParam   BinaryParam = m_AlgBinaryParam;
	const double      Offset=0.0;
	const double      GainValue = BinaryParam.GetGrayGainValue();
	const bool        bGrayGainEnabled = BinaryParam.CheckGrayGainEnabed();
	BinaryParam.SetBinaryMode(BINARY_DISABLE);

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
	
	SetUniFrameIndex(index);
	if ( 8 == BitCount )
	{	SetImageSourceMode(IMAGE_SRC_GRAY); }

	TFrameParam *FrameParamPtr=AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);	
	JetAPI::CreateFolder(strFolder);
	JetAPI::ClearFolder(strFolder);
	if ( NULL == FrameParamPtr )
	{	str = _T("SourceImage");	}
	else
	{	str = FrameParamPtr->FrameName;	}
	Filename.Format(_T("%s\\00_%s.%s"), strFolder, str, _T("PNG"));
	ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);

	nActIconIndex = -1;
	ListCtrl.SetRedraw(FALSE);
	m_StopIconListBeSelected = true;
	for ( i=0; i<IMAGE_SRC_RETURN; i++ )
	{	
		ImageSrcMode = (IMAGE_SRC_MODE)(i);
		//if ( IMAGE_SRC_COLOR == ImageSrcMode ) { continue; }
		if ( ImageSrcMode == GetImageSourceMode() )
		{	nActIconIndex = nItem; }		
		BinaryParam.SetBinaryImageSourceMode(ImageSrcMode);
		strItemText = AOIDataDefine.GetAlgImageSourceModeText(ImageSrcMode);
		ListCtrl.InsertItem(nItem, strItemText, nItem);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, 0, strItemText);
		nItem ++;

		nTextX = 8;
		nTextY = 8;

		hBitMap = ::CreateDIBSection(hMemDC, &BitMapInfo, DIB_RGB_COLORS,NULL, NULL, 0);
		hOldObj = ::SelectObject(hMemDC, hBitMap);
		::FillRect(hMemDC, &IconRect, hBrush);
		if ( IMAGE_SRC_COLOR == ImageSrcMode )
		{
			if ( 24 == BitCount )
			{	bIsOK = ImageAPI.AlignColorImageBuffer3(ImageW, ImageH, ImageStep, ImagePtr, ColorStep, BufferPtr, false);	}		
			else
			{	bIsOK = ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, ColorStep, BufferPtr, false);	}
			if ( true == bIsOK )
			{
				if ( true == bGrayGainEnabled )
				{	bIsOK = ImageAPI.ImageOffsetGain3(ImageW, ImageH, ColorStep, BitCount, BufferPtr, BufferPtr, Offset, GainValue);	}
			}
		}
		else
		{
			if ( 24 == BitCount )
			{	
				MASK_PTR   MaskPtr=NULL;		
				IMAGE_PTR  GrayPtr=NULL;
				const IMAGE_SIZE GrayBitCount = 8;
				const IMAGE_SIZE GrayStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBitCount, nAlign);				
				bIsOK = AlgParam.ExecAlgImageBinary(BinaryParam, ImageW, ImageH, ImageStep, BitCount, ImagePtr, NULL, NULL, nAlign, GrayPtr, MaskPtr);
				if ( true == bIsOK)
				{	bIsOK = ImageAPI.RGBImageToColorImage3(ImageW, ImageH, GrayStep, GrayPtr, GrayPtr, GrayPtr, ColorStep, BufferPtr, false); }
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);				
			}		
			else
			{	bIsOK = ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, ColorStep, BufferPtr, false);	}		
		}		
		if ( true == bIsOK )
		{
			Filename.Format(_T("%s\\%02d_%s.%s"), strFolder, i+1, strItemText, _T("PNG"));
			ImageAPI.SaveImage(Filename, ImageW, ImageH, ColorStep, ColorBit, BufferPtr, true);
		}
		if ( true == bIsOK )
		{	bIsOK = AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ColorStep, ColorBit, BufferPtr, BufferPtr);	}
		if ( false == bIsOK)//沒有資料一定要給予一個空的, 否則圖示與列表會錯位
		{	::memset(BufferPtr, 0x00, sizeof(IMAGE_DATA)*ImageAPI.CalcBufferSize(ColorStep, ImageH));	}
		if ( dib.SetImage(BufferPtr, ImageW, ImageH, ColorStep, ColorBit, true) == false )
		{	continue;	}		

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
	m_StopIconListBeSelected = false;	
	JetMemory.free_func(BufferPtr);
	
	// clean up
	::SelectObject(hMemDC, hPenOld);
	::DeleteObject(hPenOK);
	::DeleteObject(hPenNG);	

	::DeleteObject(hMemDC);	hMemDC = NULL;
	::DeleteObject(hBrush); hBrush=NULL;
	::DeleteObject(hBrushOK); hBrushOK=NULL;
	::DeleteObject(hBrushNG); hBrushNG=NULL;

	if ( nActIconIndex >= 0 )//v1.01.04.061
	{	ListCtrl.SetItemState(nActIconIndex, LVIS_SELECTED|LVNI_FOCUSED, LVIS_SELECTED|LVNI_FOCUSED); }
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnClickIconListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnItemchangedIconListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopIconListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	

	m_ImageSourceMode = (IMAGE_SRC_MODE)(m_IconListWnd.GetItemData(nItem));
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgImageSourceWnd::OnEdgeEnhanceBtn() 
{
	// TODO: Add your control notification handler code here		
	const int index=m_UniFrameIndex;
	const size_t UniFrameCount = m_UniFrameList.size();
	if ( index<0 || index>=UniFrameCount)
	{	return ; }		
	RECT ImageRect;
	TUNI_FRAME UniFrame=m_UniFrameList[index];
	const int  nAlign = 4;
	IMAGE_PTR  GrayPtr=NULL;
	IMAGE_SIZE ImageW = UniFrame.ImageW;
	IMAGE_SIZE ImageH = UniFrame.ImageH;
	IMAGE_SIZE BitCount = UniFrame.BitCount;
	IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	IMAGE_PTR  ImagePtr = UniFrame.ImagePtr;	
	IMAGE_SRC_MODE ImageSrcMode=IMAGE_SRC_COLOR;	
	const int nItem = m_IconListWnd.GetNextItem(-1, LVNI_FOCUSED);
	IMAGE_SIZE GrayStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, (IMAGE_SIZE)8, nAlign);
	
	JetAPI::SizeToRect(ImageW, ImageH, ImageRect);
	if ( -1 == nItem )
	{	ImageSrcMode=IMAGE_SRC_COLOR;	}
	else
	{	ImageSrcMode=(IMAGE_SRC_MODE)(nItem);	}
	if ( 24 == BitCount )
	{		
		const bool bReverse = false;
		const int WR = m_AlgBinaryParam.GetBinarySynthesisWR();
		const int WG = m_AlgBinaryParam.GetBinarySynthesisWG();
		const int WB = m_AlgBinaryParam.GetBinarySynthesisWB();
		if ( ImageAPI.ColorImageToGrayImage(ImageW, ImageH, ImageStep, ImagePtr, ImageRect, GrayStep, GrayPtr, ImageSrcMode, WR, WG, WB, bReverse) == false )
		{	return; }
	}
	else
	{
		GrayStep = ImageStep;
		const bool bReverse = false;
		if ( ImageAPI.CloneImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, GrayPtr, bReverse) == false )
		{	return; }
	}	

	CAlgImageEdgeEnhanceWnd Wnd;
	Wnd.SetAlgBinaryParam(m_AlgBinaryParam);
	Wnd.SetImage(ImageW, ImageH, GrayStep, 8, GrayPtr);
	if ( Wnd.DoModal() != IDOK )
	{
		JetMemory.free_func(GrayPtr);
		return; 
	}
	JetMemory.free_func(GrayPtr);
	Wnd.CopyAlgEdgeEnhanceParam(m_AlgBinaryParam);
	BuildImageSourceListCtrl(m_UniFrameIndex, m_IconListWnd);	
	return;
}
//-------------------------------------------------------------------------------------//