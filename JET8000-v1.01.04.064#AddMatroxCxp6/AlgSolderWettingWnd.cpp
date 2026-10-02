// AlgSolderWettingWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgSolderWettingWnd.h"
//-------------------------------------------------------------------------------------//
#include "WndAlgPropertyDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgSolderWettingWnd dialog
//-------------------------------------------------------------------------------------//
CAlgSolderWettingWnd::CAlgSolderWettingWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgSolderWettingWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgSolderWettingWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;
	m_ImageIndex = 0;

	m_ImagePtr = NULL;
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_ImageBitCount = 8;

	m_ImageEnhanced = false;	
	m_ShowPtr = NULL;
	m_ShowW = 1024;
	m_ShowH = 1024;
	m_ShowStep = 1024;
	m_ShowSize = 0;   
	m_ShowBitCount = 8;
	m_BitmapInfoPtr = NULL;
	m_ViewZoom = 1.0;
	
	m_BkColor = 0x000000;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgSolderWettingWnd)
	DDX_Control(pDX, ALGSDWT_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, ALGSDWT_PARAM_BTN, m_BtnCtrl);
	DDX_Control(pDX, ALGSDWT_PARAM_EDIT, m_EditCtrl);
	DDX_Control(pDX, ALGSDWT_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, ALGSDWT_PARAM_LIST_WND, m_ParamListCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgSolderWettingWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgSolderWettingWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, ALGSDWT_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, ALGSDWT_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(ALGSDWT_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(ALGSDWT_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(ALGSDWT_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(ALGCHAR_TEST_BTN, OnTestBtn)
	ON_BN_CLICKED(ALGCHAR_SHOW_CIRCLE_ANGLE_CHK, OnShowCircleAngleChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgSolderWettingWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgSolderWettingWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BkColor = 0x000000;
	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	m_ImageMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
	m_ImageMemDC2.CreateMemDC(&m_ImageWnd, m_BkColor);
	
	SwitchMultiLanguage();
	BuildParamListWndHeader();		
	BuildParamListWnd();		
	CreateBKImageWnd();			
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::CheckDlgButton(ALGCHAR_SHOW_CIRCLE_ANGLE_CHK, TRUE);
	TestWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseShowBuffer();
	ReleaseImageBuffer();
	AOIObjManager.DestroyWndObj(m_WndPtr);
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
BOOL CAlgSolderWettingWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			//TestWnd();
			if ( m_EditCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByEdit();
				m_EditCtrl.ShowWindow(SW_HIDE);	
				m_EditCtrl.SetWindowText(_T(""));
				return TRUE;				
			}
			if ( m_ComboxCtrl.IsWindowVisible() == TRUE ) 
			{
				ExecUpdateParamByCombox();
				m_ComboxCtrl.ShowWindow(SW_HIDE);
				JetAPI::ClearCombox(m_ComboxCtrl);
				return TRUE;				
			}
			return TRUE;
			break;		
		case VK_ESCAPE:
			ExecReleaseParamCtrl();	
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);	
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SetWndPtr(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	m_WndPtr = WndPtr->CloneWndObj();
	if ( NULL == m_WndPtr ) { return false; }	
	m_SolderWettingParam = WndPtr->GetWndAlgParam().GetAlgParamSolderWetting();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::GetSolderWettingParam(TALG_PARAM_SOLDER_WETTING &Param) const
{
	Param = m_SolderWettingParam;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SetBinaryParam(CAlgBinaryParam &BinParam)
{
	m_BinaryParam=BinParam;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList)
{
	const size_t Count = UniFrameList.size();
	if ( FrameIndex >= Count )
	{	m_ImageIndex = 0;	}
	else
	{	m_ImageIndex = FrameIndex; }
	m_WndUniFrameList = UniFrameList;
	SwitchImageBuffer(FrameIndex, m_WndUniFrameList);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_ALG_SOLDER_WETTING_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//
	SetMultiLanauage(LoadIDAndName(ALGCHAR_TEST_BTN));	
	SetMultiLanauage(LoadIDAndName(ALGCHAR_SHOW_CIRCLE_ANGLE_CHK));
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_ALG_SOLDER_WETTING_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAlgSolderWettingWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_SOLDER_WETTING_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::UpdateResultToUI()
{	
	BuildParamListWnd();
	//UpdateParamListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::ReleaseImageBuffer()
{
	if ( NULL != m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr); }
	
	m_ImageW = 256;
	m_ImageH = 256;
	m_ImageStep = 256;
	m_ImageBitCount = 8;
	m_ImagePtr = NULL;	
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::LoadImageFile(LPCTSTR filename)
{
	if ( NULL == filename) { return false; }
	ReleaseImageBuffer();	

	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;	
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false; }

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_ImageBitCount = BitCount;
	m_ImagePtr = ImagePtr;	

	m_ImageIndex = (unsigned int)(m_WndUniFrameList.size());

	BuildShowBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SwitchImageBuffer(unsigned int ImageIndex, std::vector<TUNI_FRAME> &UniFrameList)
{
	ReleaseImageBuffer();		
	const size_t FrameCount = UniFrameList.size();	
	if ( ImageIndex >= FrameCount ) { return false; }

	TUNI_FRAME UniFrame = UniFrameList[ImageIndex];
	if ( NULL == UniFrame.ImagePtr ) { return false; }

	IMAGE_PTR  DstPtr = NULL;
	IMAGE_PTR  StcPtr = UniFrame.ImagePtr;
	IMAGE_SIZE ImageW = UniFrame.ImageW;
	IMAGE_SIZE ImageH = UniFrame.ImageH;
	IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	IMAGE_SIZE BitCount = UniFrame.BitCount;

	if ( ImageAPI.CloneImage(ImageW, ImageH, ImageStep, BitCount, StcPtr, DstPtr, false) == false ) 
	{	return false; }

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_ImageBitCount = BitCount;
	m_ImagePtr = DstPtr;	
	m_ImageIndex = ImageIndex;

	BuildShowBuffer(ImageW, ImageH, ImageStep, BitCount, DstPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR CAlgSolderWettingWnd::GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_ImageW;
	ImageH = m_ImageH;
	ImageStep = m_ImageStep;
	BitCount = m_ImageBitCount;
	return m_ImagePtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ReleaseShowBuffer()
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
	m_ShowSize = 0;
	m_ShowPtr = NULL;
	m_ImageEnhanced = false;
	m_ViewZoom = 1.0;
	return true;
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR CAlgSolderWettingWnd::GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_ShowW;
	ImageH = m_ShowH;
	ImageStep = m_ShowStep;
	BitCount = m_ShowBitCount;
	return m_ShowPtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::BuildShowBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	const char fnName[] = "CAlgSolderWettingWnd::BuildShowBuffer";
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
	m_ShowSize= ShowSize;	
	m_ShowBitCount= ShowBitCount;		
	m_ImageEnhanced = true;
	return true;
}
//-------------------------------------------------------------------------------------//
CParamUni* CAlgSolderWettingWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::BuildParamList()
{	

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	CString   strName;
	CString   strParam;
	CString   strDecode;
	COLORREF  clrText;
	int       nItem=0;
	bool      bPass=false;
	CParamUni    ParamUnit;	
	WND_ALG_PROPERTY_ID ParamID;		
	const TALG_PARAM_SOLDER_WETTING  &swParam=m_SolderWettingParam;
	const int nSubItem = 1;	
	const int FloatPrecision=2;
	const COLORREF  clrOK = 0x008000;
	const COLORREF  clrNG = 0x000080;
	const COLORREF  clrUnTest = 0x808080;	
	CParamList &ParamList = m_ParamList;
	CThisListCtrl_68 &ListCtrl = m_ParamListCtrl;		
	
	ParamList.clear();	
	ClearParamListWnd();
	SetActParamUni(NULL);

	//焊接環繞角啟用
	ParamUnit = CParamUni();
	str = _T("Circle Angle Enable");	
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_ENABLED;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(swParam.swCircleAngleEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//焊接環繞角上限
	ParamUnit = CParamUni();
	str = _T("Circle Angle USL");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_USL;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(swParam.swCircleAngleUSL, FloatPrecision, 0, 360);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);
	
	//焊接環繞角下限	
	ParamUnit = CParamUni();
	str = _T("Circle Angle LSL");	
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LSL;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(swParam.swCircleAngleLSL, FloatPrecision, 0, 360);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	
	
	//焊接環繞角半徑始比例
	ParamUnit = CParamUni();
	str = _T("Circle Angle Start Ratio");	
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START_RATIO;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(swParam.swCircleAngleLineStartRatio, FloatPrecision, 0, 100);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//焊接環繞角半徑通過比例
	ParamUnit = CParamUni();
	str = _T("Circle Angle Pass Ratio");	
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_PASS_RATIO;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(swParam.swCircleAngleLinePassRatio, FloatPrecision, 0, 100);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//焊接環繞角讀值
	ParamUnit = CParamUni();
	str = _T("Circle Angle Reading");	
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_READING;	
	ParamUnit.SetParamID((UINT)(ParamID));			
	ParamUnit.SetValue_DBL(swParam.swReadingCircleAngle);	
	ParamUnit.SetDesction(strDescription);	
	ParamUnit.SetReadOnly(true);
	if ( false == swParam.swCircleAngleEnabled )
	{	clrText = clrUnTest;	}
	else
	{
		bPass = CAlgParam::CheckOK_SolderWettingSectorAngle(swParam);
		if ( true == bPass )
		{	clrText = clrOK;	}
		else
		{	clrText = clrNG;	}
	}
	ParamUnit.SetTextColor(clrText);
	ParamList.push_back(ParamUnit);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::ClearParamListWnd()
{	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(m_ParamListCtrl, FALSE);	
	m_StopParamListBeSelected = false;
	return;	
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::BuildParamListWnd()
{
	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	COLORREF      clrText;
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;		
	CThisListCtrl_68 &ListCtrl = m_ParamListCtrl;		

	BuildParamList();

	const int ParamCount = (int)(m_ParamList.size());

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem);		

		clrText = ParamPtr->GetTextColor();
		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);
		if ( CLR_DEFAULT != clrText )
		{	ListCtrl.SetItemTextColor(nItem, clrText);	}
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::UpdateParamListWnd()
{
	int           i=0;	
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItemData=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;		
	CThisListCtrl_68 &ListCtrl = m_ParamListCtrl;		
	const int ItemCount=ListCtrl.GetItemCount();
	const int ParamCount = (int)(m_ParamList.size());
	
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i=0; i<ItemCount; i++ )
	{
		const int nItem=i;
		nItemData = ListCtrl.GetItemData(i);
		if ( nItemData<0 || nItemData>=ParamCount )
		{	continue; }

		ParamPtr = &(m_ParamList[nItemData]);
		if ( NULL == ParamPtr ) { continue; }

		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);		
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_68 &ListCtrl = m_ParamListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	width2 = width*5;
	str = _T("Item");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::RedrawWnd()
{
	DrawImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::DrawImageWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageMemDC2.GetSafeHdc();	
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }
	if ( NULL == hMemDC2 ) { return; }

	RECT WndRect={0,0,0,0};
	m_ImageWnd.GetClientRect(&WndRect);
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	

	DrawCircleAngle(hMemDC2, WndRect);
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::CreateBKImageWnd()
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
void CAlgSolderWettingWnd::DrawImage(HDC hDC, const RECT &Rect)
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
	ImageAPI.DrawImageToDC(hDC, pInfo, ImagePtr, Rect, OffsetPt2D, dZoom, BkColor);
	m_ViewZoom = dZoom;
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::DrawCircleAngle(HDC hDC, const RECT &Rect)
{
	if ( NULL == hDC ) { return; }
	BOOL bShow=CWnd::IsDlgButtonChecked(ALGCHAR_SHOW_CIRCLE_ANGLE_CHK);
	if ( FALSE == bShow ) { return; }

	int RadiusX=0, RadiusY=0;	
	const double ViewZoom = m_ViewZoom;
	const double fWndCpX=(Rect.left+Rect.right)*0.5;
	const double fWndCpY=(Rect.top+Rect.bottom)*0.5;
	const TALG_PARAM_SOLDER_WETTING &wsParam=m_SolderWettingParam;
	const double fCircleCpX =fWndCpX;
	const double fCircleCpY =fWndCpY;
	const double Radius=wsParam.swCircleAngleRadius/ViewZoom;	
	const size_t RadiusCount=wsParam.swResultCircleAngleRadius.size();	
	const int CircleCpX=(int)(JetAPI::ToInt(fCircleCpX));
	const int CircleCpY=(int)(JetAPI::ToInt(fCircleCpY));
	const int PenSize=JetAPI::Ceil(ViewZoom)+1;
	HPEN hOkPen = ::CreatePen(PS_SOLID, PenSize, 0x00A000);
	HPEN hNgPen = ::CreatePen(PS_SOLID, PenSize, 0x0000A0);	
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hOkPen));
	
	for ( size_t i=0; i<RadiusCount; i++ )
	{
		const TRESULT_CIRCLE_ANGLE &caLine=wsParam.swResultCircleAngleRadius[i];
		const double AngleDeg = caLine.caAngle;
		const double AngleImg = JetAPI::MapCadAngleToImageAngle(AngleDeg);
		const double AngleRad=AngleImg*DEG_TO_RAD_DBL;		
		const double COSA=::cos(AngleRad);
		const double SINA=::sin(AngleRad);
		const double RadiusRatio=caLine.caRadiusPassResult/100.0;
		const double fRadiusX=Radius*RadiusRatio*COSA+fCircleCpX;
		const double fRadiusY=Radius*RadiusRatio*SINA+fCircleCpY;	
		RadiusX = JetAPI::ToInt(fRadiusX);
		RadiusY = JetAPI::ToInt(fRadiusY);

		if ( caLine.caRadiusPassResult < wsParam.swCircleAngleLinePassRatio )
		{	::SelectObject(hDC, hNgPen);	}
		else
		{	::SelectObject(hDC, hOkPen);	}

		::MoveToEx(hDC, CircleCpX, CircleCpY, NULL);
		::LineTo(hDC, RadiusX, RadiusY);
	}	

	RECT StartRect;
	HPEN hStartPen = ::CreatePen(PS_SOLID, 1, 0xFFFFFF);
	const double RadiusStart=Radius*wsParam.swCircleAngleLineStartRatio/100;
	StartRect.left  = JetAPI::ToInt(fCircleCpX-RadiusStart);
	StartRect.top   = JetAPI::ToInt(fCircleCpY-RadiusStart);
	StartRect.right = JetAPI::ToInt(fCircleCpX+RadiusStart);
	StartRect.bottom= JetAPI::ToInt(fCircleCpY+RadiusStart);
	::SelectObject(hDC, hStartPen);
	ImageAPI.DrawEllipseLine(hDC, StartRect);		

	RECT RadiusPassRect;
	HPEN hRadiusPassPen = ::CreatePen(PS_SOLID, 1, 0x0000A0);
	const double RadiusPass=Radius*wsParam.swCircleAngleLinePassRatio/100;
	RadiusPassRect.left  = JetAPI::ToInt(fCircleCpX-RadiusPass);
	RadiusPassRect.top   = JetAPI::ToInt(fCircleCpY-RadiusPass);
	RadiusPassRect.right = JetAPI::ToInt(fCircleCpX+RadiusPass);
	RadiusPassRect.bottom= JetAPI::ToInt(fCircleCpY+RadiusPass);
	::SelectObject(hDC, hRadiusPassPen);
	ImageAPI.DrawEllipseLine(hDC, RadiusPassRect);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hNgPen); hNgPen=NULL;
	::DeleteObject(hOkPen); hOkPen=NULL;
	::DeleteObject(hStartPen); hStartPen=NULL;	
	::DeleteObject(hRadiusPassPen); hRadiusPassPen=NULL;	
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	DrawImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::TestWnd()
{
	CWnd::PostMessage(WM_COMMAND, ALGCHAR_TEST_BTN, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecTestBtn(CAOIWnd *WndPtr)
{	
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr=WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	TALG_PARAM_SOLDER_WETTING &wsParam=m_SolderWettingParam;
	WndPtr->GetWndAlgParam().SetAlgParamSolderWetting(wsParam);


	TREGION4D Region;
	TPOINT2D  RgnCp, Scale, ImageCp;	
	const bool bTestWnd = true;
	const int ImageW=(int)(m_ImageW);
	const int ImageH=(int)(m_ImageH);	

	WndPtr->GetWndRegion(Region);
	const double RegionW=Region.GetWidth();
	const double RegionH=Region.GetHeight();

	Scale.x = ImageW;
	Scale.y = ImageH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;	
	RgnCp.x = Region.GetCpX();
	RgnCp.y = Region.GetCpY();
	ImageCp.x = ImageW;
	ImageCp.y = ImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;				
	WndPtr->InitWndInspection(false);
	WndPtr->ExecWndInspection(ModelPtr, Region, RgnCp, Scale, ImageCp, m_WndUniFrameList, bTestWnd);
	m_SolderWettingParam = WndPtr->GetWndAlgParam().GetAlgParamSolderWetting();
	UpdateResultToUI();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecItemchangedParamListWnd(CThisListCtrl_68 &ListCtrl, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();	
	const int nSubItem = ParamPtr->GetSubItemIndex();
	
	SetDescriptionText(ParamPtr);
	if ( NULL!=BtnWndPtr && BtnWndPtr->GetSafeHwnd()!=NULL) 	
	{
		CRect ItemRect;
		if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == TRUE )
		{	
			SIZE BtnSize={0};
			RECT BtnRect={0};
			RECT CtrlRect = ItemRect;
			ListCtrl.ClientToScreen(&CtrlRect);
			this->ScreenToClient(&CtrlRect);
			BtnWndPtr->GetWindowRect(&BtnRect);
			JetAPI::GetRectSize(BtnRect, BtnSize);
			BtnRect = CtrlRect;			
			BtnRect.left = BtnRect.right-BtnSize.cx;
			BtnWndPtr->MoveWindow(&BtnRect, FALSE);
			BtnWndPtr->ShowWindow(SW_SHOW);			
			BtnWndPtr->BringWindowToTop();
			ListCtrl.UpdateWindow();
			BtnWndPtr->Invalidate();
			SetActParamUni(ParamPtr);			
		}		
	}
	else
	{	m_BtnCtrl.ShowWindow(SW_HIDE);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecDblclkParamListWnd(CThisListCtrl_68 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < 1 ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);
	::OffsetRect(&CtrlRect, 0, -2);
	m_BtnCtrl.ShowWindow(SW_HIDE);
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(m_ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				m_ComboxCtrl.InsertString(nSelIdx, ItemText);
				m_ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(m_ComboxCtrl, ParamPtr->GetSelParam());
			m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			m_ComboxCtrl.SetFocus();
			m_ComboxCtrl.ShowDropDown();
			m_ComboxCtrl.ShowWindow(SW_SHOW);
			m_ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			m_ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		if ( m_EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			m_EditCtrl.SetWindowText(ItemText);
			m_EditCtrl.MoveWindow(&CtrlRect, FALSE);
			m_EditCtrl.SetFocus();
			m_EditCtrl.SetSel(0,-1);			
			m_EditCtrl.ShowWindow(SW_SHOW);	
			m_EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			m_EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecUpdateParamByEdit()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);	

	CString ItemText;	
	WND_ALG_PROPERTY_ID SysParam = (WND_ALG_PROPERTY_ID)(ParamPtr->GetParamID());	
	m_EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		m_EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( SetSolderWettingParamStringByID(SysParam, m_SolderWettingParam, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_68 *pListCtrl = (CThisListCtrl_68*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
		TestWnd();
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	WND_ALG_PROPERTY_ID SysParam = (WND_ALG_PROPERTY_ID)(ParamPtr->GetParamID());
	const int nCurSel = m_ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);	
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( SetSolderWettingParamStringByID(SysParam, m_SolderWettingParam, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_68 *pListCtrl = (CThisListCtrl_68*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
		TestWnd();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::SetDescriptionText(const CParamUni *Ptr)
{
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgSolderWettingWnd::SetSolderWettingParamStringByID(UINT ParamID, TALG_PARAM_SOLDER_WETTING &swParam, LPCTSTR String)//設定焊接參數
{
	bool bUpdate=true;
	switch ( ParamID ) 
	{
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_ENABLED:
		swParam.swCircleAngleEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_USL:
		swParam.swCircleAngleUSL = (float)(::_ttof(String));
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LSL:
		swParam.swCircleAngleLSL = (float)(::_ttof(String));
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START_RATIO:
		swParam.swCircleAngleLineStartRatio = (float)(::_ttof(String));
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_PASS_RATIO:
		swParam.swCircleAngleLinePassRatio = (float)(::_ttof(String));
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_READING:
		break;
	default:
		bUpdate = false;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		SetDescriptionText(NULL);
		return; 
	}
	DWORD Res = 0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;	
	if ( 0!=ResOld && 0==ResNew )
	{
		ExecReleaseParamCtrl();
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	//BuildBarcodeImage(m_ParamListCtrl, nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnKillfocusParamEdit()
{
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnSelchangeParamCombo()
{
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnKillfocusParamCombo()
{
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnTestBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecTestBtn(m_WndPtr);	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgSolderWettingWnd::OnShowCircleAngleChk()
{
	// TODO: Add your control notification handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
