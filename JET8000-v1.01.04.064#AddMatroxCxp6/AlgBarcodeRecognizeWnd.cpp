// AlgBarcodeRecognizeWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgBarcodeRecognizeWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
#include "WndAlgPropertyDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd dialog
//-------------------------------------------------------------------------------------//
CAlgBarcodeRecognizeWnd::CAlgBarcodeRecognizeWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgBarcodeRecognizeWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgBarcodeRecognizeWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ImageIndex = 0;

	m_ImageW = 256;
	m_ImageH = 256;
	m_ImageStep = 256;
	m_ImageBitCount = 8;
	m_ImagePtr = NULL;	
	
	m_ShowPtr = NULL;
	m_ShowW = 256;
	m_ShowH = 256;
	m_ShowStep = 256;
	m_ShowSize = 0;	
	m_ShowBitCount = 8;
	m_BitmapInfoPtr = NULL;
	
	m_ParamActPtr = NULL;
	m_ImageEnhanced = false;
	m_StopParamListBeSelected = false;

	m_LastImageExtName = _T("*.PNG");
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgBarcodeRecognizeWnd)	
	DDX_Control(pDX, ALGBAR_PARAM_EDIT, m_EditCtrl);	
	DDX_Control(pDX, ALGBAR_PARAM_BTN, m_BtnCtrl);		
	DDX_Control(pDX, ALGBAR_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, ALGBAR_PARAM_LIST_WND, m_ParamListCtrl);
	DDX_Control(pDX, ALGBAR_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgBarcodeRecognizeWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgBarcodeRecognizeWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_BN_CLICKED(ALGBAR_LOAD_IMAGE_BTN, OnLoadImageBtn)
	ON_BN_CLICKED(ALGBAR_BARCODE_RECOGNIZE_BTN, OnBarcodeRecognizeBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, ALGBAR_PARAM_LIST_WND, OnItemchangedParamListWnd)
	ON_NOTIFY(NM_DBLCLK, ALGBAR_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_EN_KILLFOCUS(ALGBAR_PARAM_EDIT, OnKillfocusParamEdit)
	ON_CBN_SELCHANGE(ALGBAR_PARAM_COMBO, OnSelchangeParamCombo)
	ON_CBN_KILLFOCUS(ALGBAR_PARAM_COMBO, OnKillfocusParamCombo)
	ON_BN_CLICKED(ALGBAR_SAVE_IMAGE_BTN, OnSaveImageBtn)
	ON_BN_CLICKED(ALGBAR_SHOW_LETTER_LIST_BTN, OnShowLetterListBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgBarcodeRecognizeWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	m_BkColor = 0x000000;
	m_ImageMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);

	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	SwitchMultiLanguage();
	BuildParamListWndHeader();
	BuildParamListWnd();
	CreateBKImageWnd();	
	ExecBarcodeRecogine();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
	ReleaseShowBuffer();
	ReleaseImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return ; }

	int BtnX = cx;
	int EditY=cy;
	CWnd *WndPtr = NULL;	
	POINT Offset={0,0};
	
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  768;
}
//-------------------------------------------------------------------------------------//
BOOL CAlgBarcodeRecognizeWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
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
LRESULT CAlgBarcodeRecognizeWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_BARCODE_RECOGNIZE_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_BARCODE_RECOGNIZE_WND;
	WndKey = _T("IDD_ALG_BARCODE_RECOGNIZE_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	m_WndTitle = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = ALGBAR_LOAD_IMAGE_BTN;
	WndKey = _T("ALGBAR_LOAD_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALGBAR_BARCODE_RECOGNIZE_BTN;
	WndKey = _T("ALGBAR_BARCODE_RECOGNIZE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALGBAR_SAVE_IMAGE_BTN;
	WndKey = _T("ALGBAR_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALGBAR_SHOW_LETTER_LIST_BTN;
	WndKey = _T("ALGBAR_SHOW_LETTER_LIST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//

	/*
	WndID = AAAAAAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	*/
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CAlgBarcodeRecognizeWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_BARCODE_RECOGNIZE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::SetBarcodeRecognizeParam(const TALG_PARAM_BARCODE_RECOGNIZE &Param)
{
	m_BarcodeParam = Param;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::GetBarcodeRecognizeParam(TALG_PARAM_BARCODE_RECOGNIZE &Param) const
{
	Param = m_BarcodeParam;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::SetBinaryParam(CAlgBinaryParam &BinParam)
{
	m_BinaryParam=BinParam;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList)
{
	const size_t Count = UniFrameList.size();
	if ( FrameIndex >= Count )
	{	m_ImageIndex = 0;	}
	else
	{	m_ImageIndex = FrameIndex; }
	m_BarcodeUniFrameList = UniFrameList;
	SwitchImageBuffer(FrameIndex, m_BarcodeUniFrameList);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::ReleaseImageBuffer()
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
bool CAlgBarcodeRecognizeWnd::LoadImageFile(LPCTSTR filename)
{
	if ( NULL == filename) { return false; }
	ReleaseImageBuffer();	

	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	SetBarcodeResultText(_T(""));
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false; }

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_ImageBitCount = BitCount;
	m_ImagePtr = ImagePtr;	

	m_ImageIndex = (unsigned int)(m_BarcodeUniFrameList.size());

	BuildShowBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::SwitchImageBuffer(unsigned int ImageIndex, std::vector<TUNI_FRAME> &UniFrameList)
{
	ReleaseImageBuffer();	
	SetBarcodeResultText(_T(""));
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
IMAGE_PTR CAlgBarcodeRecognizeWnd::GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_ImageW;
	ImageH = m_ImageH;
	ImageStep = m_ImageStep;
	BitCount = m_ImageBitCount;
	return m_ImagePtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ReleaseShowBuffer()
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
	return true;
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR CAlgBarcodeRecognizeWnd::GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount)
{
	ImageW = m_ShowW;
	ImageH = m_ShowH;
	ImageStep = m_ShowStep;
	BitCount = m_ShowBitCount;
	return m_ShowPtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::BuildShowBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	const char fnName[] = "CAlgBarcodeRecognizeWnd::BuildShowBuffer";
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
bool CAlgBarcodeRecognizeWnd::BuildBarcodeImage(CThisListCtrl_01 &ListCtrl, int nItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }	
	int        FinalStep=0;
	CParamUni *ParamPtr=&(m_ParamList[ParamIndex]);	
	CWnd      *BtnWndPtr = ParamPtr->GetBtnWndPtr();	
	const int nSubItem = ParamPtr->GetSubItemIndex();
	WND_ALG_PROPERTY_ID	ParamID = (WND_ALG_PROPERTY_ID)(ParamPtr->GetParamID());
	ExecBuildBarcodeImage(ParamID);
	CreateBKImageWnd();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ExecBuildBarcodeImage(int ParamID)
{	
	if ( ParamID<WND_ALG_PROPERTY_BARCODE_BEGIN || ParamID>WND_ALG_PROPERTY_BARCODE_END )
	{	return false; }

	int FinalStep = 0;
	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_1:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_1:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_1:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_1:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_1:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_1:
		FinalStep = 1;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_2:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_2:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_2:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_2:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_2:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_2:
		FinalStep = 2;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_3:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_3:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_3:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_3:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_3:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_3:
		FinalStep = 3;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_4:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_4:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_4:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_4:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_4:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_4:
		FinalStep = 4;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_5:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_5:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_5:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_5:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_5:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_5:
		FinalStep = 5;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_6:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_6:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_6:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_6:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_6:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_6:
		FinalStep = 6;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_7:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_7:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_7:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_7:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_7:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_7:
		FinalStep = 7;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_8:
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_8:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_8:
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_8:
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_8:
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_8:
		FinalStep = 8;
		break;
	default:
		FinalStep = 0;
		break;
	}
	
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	IMAGE_SIZE   ImageBitCount = 0;

	ImagePtr = GetImageBuffer(ImageW, ImageH, ImageStep, ImageBitCount);
	if ( NULL == ImagePtr ) { return false; }
	if ( 0 == FinalStep ) 		
	{
		BuildShowBuffer(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr);		
		return true;
	}

	bool IsOK=true;
	RECT CalcRect={0};
	RECT MaskRect={0};	
	CAlgParam AlgParam;
	CAlgBinaryParam  BinParam = m_BinaryParam;

	MASK_PTR  MaskPtr=NULL;
	IMAGE_PTR GrayPtr=NULL;
	IMAGE_SIZE MaskW=0;
	IMAGE_SIZE MaskH=0;
	IMAGE_SIZE MaskStep=0;
	IMAGE_SIZE MaskBitCount=0;
	const bool bTestWnd = true;	
	TUNI_FRAME UniFrame;
	std::vector<TUNI_FRAME> UniFrameList;

	BinParam.SetBinaryFrameIndex(0);
	JetAPI::SizeToRect(ImageW, ImageH, CalcRect);
	JetAPI::SizeToRect(ImageW, ImageH, MaskRect);
	UniFrame.SetUniFrame(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr);
	UniFrameList.push_back(UniFrame);
	if ( AlgParam.ExecAlgUniFrameBinary(BinParam, CalcRect, MaskRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{
		BuildShowBuffer(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr);
		return false; 
	}

	BINARY_MODE BinaryMode = BinParam.GetBinaryMode();
	if ( BINARY_DISABLE == BinaryMode )
	{	IsOK = BuildBarcodeImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, FinalStep);	}
	else
	{	IsOK = BuildBarcodeImage(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, FinalStep); }
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);
	if ( false == IsOK )
	{
		BuildShowBuffer(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr);		
		return false; 
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::BuildBarcodeImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, int FinalStep)
{
	if ( NULL == Ptr ) { return false; }
	if ( 8 != BitCount ) { return false; }	
	const char fnName[] = "CAlgBarcodeRecognizeWnd::BuildBarcodeImage";
	const int MaxBarcodeDecodeCount=MIN(FinalStep, MAX_BARCODE_DECODE_COUNT);
	
	IMAGE_PTR     CodePtr = NULL;
	IMAGE_SIZE    CodeW=(ImageW);
	IMAGE_SIZE    CodeH=(ImageH);	
	IMAGE_SIZE    CodeBitCount = BitCount;	
	IMAGE_SIZE    CodeStep=ImageStep;	
	size_t        CodeSize=ImageAPI.CalcBufferSize(CodeStep, CodeH);

	IMAGE_PTR     CodePtr2 = NULL;
	IMAGE_SIZE    CodeW2=0;
	IMAGE_SIZE    CodeH2=0;	
	IMAGE_SIZE    CodeStep2=0;
	IMAGE_SIZE    CodeBitCount2=BitCount;	

	int           i=0;
	bool          IsOK = true;
	bool          Decoded=false;
	double        Gain=0;
	double        Scale=0;
	double        Offset=0;
	int           nKernelSize=0;
	int           nIterCount =0;	
	TALG_BARCODE_STEP_PARAM       BarcodeStepParam;
	TALG_PARAM_BARCODE_RECOGNIZE  barParam=m_BarcodeParam;

	if ( JetMemory.alloc_func(CodeSize, CodePtr, fnName, "CodePtr") == false )
	{	return false; }
	::memcpy(CodePtr, Ptr, sizeof(unsigned char)*CodeSize);
	for ( i=0; i<MaxBarcodeDecodeCount; i++ )
	{	
		BarcodeStepParam = barParam.brDecodeStep[i];
		if ( ALG_BARCODE_STEP_NONE == BarcodeStepParam.BarcodeStep ) { continue; }

		IsOK = true;
		Decoded = false;
		CodeW2 = CodeW;
		CodeH2 = CodeH;
		CodeStep2 = CodeStep;		
		switch ( BarcodeStepParam.BarcodeStep )
		{
		case ALG_BARCODE_STEP_SCALE:
			Scale = BarcodeStepParam.Param1;
			Scale = MIN(1.0, Scale);
			IsOK = ImageAPI.ScaleImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, Scale, CodeW2, CodeH2, CodeStep2, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_GAIN_OFFSET:
			Gain = BarcodeStepParam.Param1;
			Offset = -BarcodeStepParam.Param2;
			IsOK = ImageAPI.ImageOffsetGain(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2, Offset, Gain);
			break;
		case ALG_BARCODE_STEP_SMOOTH:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.SmoothImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_OPEN:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.MorphImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, MORPH_OPEN, MORPH_SHAPE_RECT, nKernelSize, nIterCount, CodePtr2);
			break;
		case ALG_BARCODE_STEP_CLOSE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.MorphImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, MORPH_CLOSE, MORPH_SHAPE_RECT, nKernelSize, nIterCount, CodePtr2);
			break;
		case ALG_BARCODE_STEP_MEDIAN:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.MedianImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_INVERT:
			IsOK = ImageAPI.InvertImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2);
			break;
		case ALG_BARCODE_STEP_FLIP:
			IsOK = ImageAPI.ReverseImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2);
			break;
		case ALG_BARCODE_STEP_FILL:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.FillBarcodeImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_ERODE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.ErodeImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, nIterCount, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_DILATE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.DilateImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, nIterCount, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_FILL_2D:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.FillBarcode2DImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_SHARP:			
			IsOK = ImageAPI.SharpnessGausImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, 5, 5, 200, CodePtr2);
			break;
		case ALG_BARCODE_STEP_GRAY_RANGE:
			IsOK = ImageAPI.RangeGrayImage(CodeW, CodeH, CodeStep, CodePtr, CodePtr2, BarcodeStepParam.Param1, BarcodeStepParam.Param2);
			break;
		}	
		if ( false == IsOK )
		{	break;	}

		JetMemory.free_func(CodePtr);

		CodePtr = CodePtr2;
		CodeW = CodeW2;
		CodeH = CodeH2;
		CodeStep = CodeStep2;
		CodeBitCount = CodeBitCount2;
		CodeSize=ImageAPI.CalcBufferSize(CodeStep, CodeH);

		CodePtr2 = NULL;
	}
	if ( false == IsOK )
	{
		JetMemory.free_func(CodePtr);
		return false;
	}

	
	if ( NULL != m_ShowPtr )
	{
		if ( CodeSize > m_ShowSize )
		{	::memset(m_ShowPtr, 0x00, sizeof(unsigned char)*m_ShowSize);	}
		else
		{			
			m_ShowW = CodeW;
			m_ShowH = CodeH;
			m_ShowStep = CodeStep;
			m_ShowBitCount = CodeBitCount;
			m_ImageEnhanced = false;
			::memcpy(m_ShowPtr, CodePtr, sizeof(unsigned char)*CodeSize);

			if ( NULL != m_BitmapInfoPtr ) { delete[] m_BitmapInfoPtr; }
			m_BitmapInfoPtr = NULL;
			ImageAPI.CreateBMPInfo(m_BitmapInfoPtr, CodeW, CodeH, CodeBitCount);			
		}
	}	
	JetMemory.free_func(CodePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
CParamUni*  CAlgBarcodeRecognizeWnd::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::BuildParamList()
{
	CThisListCtrl_01 &ListCtrl = m_ParamListCtrl;
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopParamListBeSelected = false;

	int       i=0;
	int       intValue=0;
	CString   str;
	CString   strValue;	
	CString   strCaption;	
	CString   strDescription;
	CString   strName;
	CString   strParam;
	CString   strDecode;
	int       nItem=0;
	CParamUni    ParamUnit;	
	WND_ALG_PROPERTY_ID ParamID;	
	TALG_BARCODE_STEP_PARAM    DecodeStep;//解碼步驟
	TALG_PARAM_BARCODE_RECOGNIZE brParam = m_BarcodeParam;
	const int nSubItem = 1;		
	CParamList &ParamList = m_ParamList;

	ParamList.clear();
	SetActParamUni(NULL);

	//1D Code
	ParamUnit = CParamUni();
	str = _T("1D Code");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_1D_ENABLED;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.br1DCodeEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//QRCode
	ParamUnit = CParamUni();
	str = _T("QRCode");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_QRCODE_ENABLED;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.brQRCodeEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//DataMatrix
	ParamUnit = CParamUni();
	str = _T("Data Matrix");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_ENABLED;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.brDataMatrixEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Code Count
	ParamUnit = CParamUni();
	str = _T("Code Count");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_CODE_COUNT;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(brParam.brCodeCount);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Enable Code Content
	ParamUnit = CParamUni();
	str = _T("Enable Code Content");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_CODE_CONTENT_ENABLE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.brCodeContentEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Code Content
	ParamUnit = CParamUni();
	str = _T("Code Content");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_CODE_CONTENT;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_STR(brParam.brBarcodeContent.c_str());	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Enable Check JSON
	ParamUnit = CParamUni();
	str = _T("Enable Check JSON");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_CODE_JSON_CHECK_ENABLE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.brCodeJSONCheckEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//條碼的CheckSum啟用
	ParamUnit = CParamUni();
	str = _T("Enable Check Sum");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_CHECK_SUM_ENABLE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
	strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
	ParamUnit.SetValue_SEL(brParam.brCheckSumEnabled);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Verify USL
	ParamUnit = CParamUni();
	str = _T("Verify USL");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_VERIFY_USL;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(brParam.brVerifyUSL, 2, 0, 100);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Verify LSL
	ParamUnit = CParamUni();
	str = _T("Verify LSL");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_VERIFY_LSL;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_DBL(brParam.brVerifyLSL, 2, 0, 100);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);	

	//Timeout
	ParamUnit = CParamUni();
	str = _T("Timeout");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_DECODE_TIMEOUT;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	ParamUnit.SetValue_INT(brParam.brDecodeTimeout, 0);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Start Step	
	ParamUnit = CParamUni();
	str = _T("Start Step");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_DECODE_START_STEP;	
	ParamUnit.SetParamID((UINT)(ParamID));		
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{
		strValue.Format(_T("%d"), i+1);
		ParamUnit.AddSelItem(i, strValue);
	}
	//ParamUnit.SetValue_INT(brParam.brDecodeStartStep+1, 1, MAX_BARCODE_DECODE_COUNT);	
	ParamUnit.SetValue_SEL(brParam.brDecodeStartStep);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	//Direction Mode
	ParamUnit = CParamUni();
	str = _T("Direction");
	strCaption = LoadMultiLanguageString(str, str);
	ParamUnit.SetCaption(strCaption);
	ParamID = WND_ALG_PROPERTY_BARCODE_DIRECTION_MODE;	
	ParamUnit.SetParamID((UINT)(ParamID));	
	strValue = AOIDataDefine.GetBarcodeDirectionModeText(ALG_BARCODE_DIR_AUTO);	ParamUnit.AddSelItem(ALG_BARCODE_DIR_AUTO, strValue);
	strValue = AOIDataDefine.GetBarcodeDirectionModeText(ALG_BARCODE_DIR_HOR);	ParamUnit.AddSelItem(ALG_BARCODE_DIR_HOR, strValue);
	strValue = AOIDataDefine.GetBarcodeDirectionModeText(ALG_BARCODE_DIR_VER);	ParamUnit.AddSelItem(ALG_BARCODE_DIR_VER, strValue);
	strValue = AOIDataDefine.GetBarcodeDirectionModeText(ALG_BARCODE_DIR_ALL);	ParamUnit.AddSelItem(ALG_BARCODE_DIR_ALL, strValue);
	ParamUnit.SetValue_SEL(brParam.brCodeDirectionMode);	
	ParamUnit.SetDesction(strDescription);
	ParamList.push_back(ParamUnit);

	str = _T("Name");
	strName = LoadMultiLanguageString(str, str);
	str = _T("Param");
	strParam = LoadMultiLanguageString(str, str);	
	strDecode = AOIDataDefine.GetDecodeText();
	std::vector<ALG_BARCODE_STEP_MODE> DecodeList;
	CAlgParam::BuildBarcodeDecodeStepList(DecodeList);
	const size_t DecodeCount=DecodeList.size();
	const int ParamStep = WND_ALG_PROPERTY_BARCODE_DECODE_STEP_2-WND_ALG_PROPERTY_BARCODE_DECODE_STEP_1;
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{
		DecodeStep = brParam.brDecodeStep[i];
		
		//Step Name
		ParamUnit = CParamUni();
		strCaption.Format(_T("%d-%s"), i+1, strName);
		//strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = (WND_ALG_PROPERTY_ID)(WND_ALG_PROPERTY_BARCODE_DECODE_NAME_1+(i*ParamStep));	
		ParamUnit.SetParamID((UINT)(ParamID));	
		for ( size_t j=0; j<DecodeCount; j++ )
		{	strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(DecodeList[j]);	ParamUnit.AddSelItem(DecodeList[j], strValue);	}		
		ParamUnit.SetValue_SEL(DecodeStep.BarcodeStep);	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);		

		//Param 1
		ParamUnit = CParamUni();
		strCaption.Format(_T("  %d-%s[1]"), i+1, strParam);
		//strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = (WND_ALG_PROPERTY_ID)(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_1+(i*ParamStep));	
		ParamUnit.SetParamID((UINT)(ParamID));			
		ParamUnit.SetValue_DBL(DecodeStep.Param1);	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);	

		//Param 2
		ParamUnit = CParamUni();
		strCaption.Format(_T("  %d-%s[2]"), i+1, strParam);
		//strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = (WND_ALG_PROPERTY_ID)(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_1+(i*ParamStep));	
		ParamUnit.SetParamID((UINT)(ParamID));			
		ParamUnit.SetValue_DBL(DecodeStep.Param2);	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);	

		//Enabled		
		ParamUnit = CParamUni();
		strCaption.Format(_T("  %d-%s"), i+1, strDecode);
		//strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = (WND_ALG_PROPERTY_ID)(WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_1+(i*ParamStep));	
		ParamUnit.SetParamID((UINT)(ParamID));			
		strValue = AOIDataDefine.GetEnableDisableText(FN_ENABLE);	ParamUnit.AddSelItem(FN_ENABLE, strValue);
		strValue = AOIDataDefine.GetEnableDisableText(FN_DISABLE);	ParamUnit.AddSelItem(FN_DISABLE, strValue);
		ParamUnit.SetValue_SEL(DecodeStep.Enabled);	
		ParamUnit.SetDesction(strDescription);
		ParamList.push_back(ParamUnit);	

		//Decoded
		ParamUnit = CParamUni();
		strCaption.Format(_T("  %d-Decoded"), i+1);
		//strCaption = LoadMultiLanguageString(str, str);
		ParamUnit.SetCaption(strCaption);
		ParamID = (WND_ALG_PROPERTY_ID)(WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_1+(i*ParamStep));	
		ParamUnit.SetParamID((UINT)(ParamID));			
		ParamUnit.SetValue_DBL(DecodeStep.Param2);	
		ParamUnit.SetDesction(strDescription);
		//ParamList.push_back(ParamUnit);	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::BuildParamListWnd()
{	
	CThisListCtrl_01 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopParamListBeSelected = false;

	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;		
	
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

		strCaption = ParamPtr->GetCaption();
		strValue = ParamPtr->GetParamText();
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption);
		ListCtrl.SetItemText(nItem, nSubItem, strValue);
		nItem ++;
	}	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_01 &ListCtrl = m_ParamListCtrl;

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
void CAlgBarcodeRecognizeWnd::SetDescriptionText(const CParamUni *Ptr)
{
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::RedrawWnd()
{
	DrawImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::DrawImageWnd()
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
void CAlgBarcodeRecognizeWnd::CreateBKImageWnd()
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
void CAlgBarcodeRecognizeWnd::DrawImage(HDC hDC, const RECT &Rect)
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
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	DrawImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnLoadImageBtn() 
{
	// TODO: Add your control notification handler code here
	CString strExtName=m_LastImageExtName;
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG;PNG"), strExtName, OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	bool    IsOK=true;
	CString str;	
	CString extName=dialog.GetFileExt();
	CString filename = dialog.GetPathName();
	str.Format(_T("%s - %s"), m_WndTitle, filename);
	CWnd::SetWindowText(str);	
	m_LastImageExtName.Format(_T("*.%s"), extName);
	IsOK = LoadImageFile(filename);
	CreateBKImageWnd();
	RedrawWnd();
	if ( false == IsOK ) 
	{
		str.Format(_T("Error, Load File Fault [%s]"), filename);
		JetAPI::ShowMessageBox(str);
		return;
	}
	ExecBarcodeRecogine();
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ExecBarcodeRecogine()
{
	IMAGE_PTR    ImagePtr = NULL;
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	IMAGE_SIZE   ImageBitCount = 0;

	m_BarcodeText = _T("");
	SetBarcodeResultText(_T(""));
	ImagePtr = GetImageBuffer(ImageW, ImageH, ImageStep, ImageBitCount);
	if ( NULL == ImagePtr ) { return false; }
	
	bool       IsOK=true;
	CString    str;
	CString    strTime;
	CString    strBarcode;
	CAOIWnd    WndObj;
	CAOIModel  ModelObj;
	CAlgParam  AlgParam;
	TUNI_FRAME UniFrame;
	TREGION4D  ModelRgn;
	const bool bTestWnd=true;
	RECT       RoiRect={0,0,0,0};
	RECT       WndRect={0,0,0,0};
	std::vector<TUNI_FRAME> UniFrameList;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	CAlgBinaryParam BinParam = m_BinaryParam;
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);

	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	JetAPI::SizeToRect(ImageW, ImageH, WndRect);
	UniFrame.SetUniFrame(ImageW, ImageH, ImageStep, ImageBitCount, ImagePtr);
	UniFrameList.push_back(UniFrame);	

	BinParam.SetBinaryFrameIndex(0);
	AlgParam.SetAlgWndPtr(&WndObj);
	AlgParam.SetAlgType(ALG_BARCODE_RECOGNIZE);
	AlgParam.SetAlgImageBinParam(BinParam);
	AlgParam.SetAlgParamBarcodeRecognize(m_BarcodeParam);
	AlgParam.InitAlgInspection(false);

	LARGE_INTEGER  fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	IsOK = AlgParam.ExecAlgInspection(&ModelObj, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd);
	JetAPI::SetFuncTimeEnd(fnEnd);
	double CalcTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

	strTime.Format(_T("%.2f ms"), CalcTime);
	strBarcode = AlgParam.GetAlgParamBarcodeRecognize().brBarcodeResult.c_str();
	if ( strBarcode.GetLength() == 0 ) 
	{	strBarcode = _T("Error"); }	
	else
	{	m_BarcodeText = strBarcode; }
	str.Format(_T("%s, Time:%s"), strBarcode, strTime);
	SetBarcodeResultText(str);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnBarcodeRecognizeBtn() 
{
	// TODO: Add your control notification handler code here
	ExecBarcodeRecogine();
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
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
	DWORD Res=0;
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
	BuildBarcodeImage(m_ParamListCtrl, nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }

	ExecDblclkParamListWnd(m_ParamListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnKillfocusParamEdit() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnSelchangeParamCombo() 
{
	// TODO: Add your control notification handler code here
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnKillfocusParamCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);	
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ExecItemchangedParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem)
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
bool CAlgBarcodeRecognizeWnd::ExecDblclkParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem, int nSubItem)
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
bool CAlgBarcodeRecognizeWnd::ExecReleaseParamCtrl()
{
	m_BtnCtrl.ShowWindow(SW_HIDE);
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));
	m_ParamListCtrl.SetFocus();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ExecUpdateParamByEdit()
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
	if ( SetBarcodeParameterStringByID(SysParam, m_BarcodeParam, ItemText) == false )
	{	return false; }
	
	CThisListCtrl_01 *pListCtrl = (CThisListCtrl_01*)(ParamPtr->GetListCtrl());
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
		BuildBarcodeImage(*pListCtrl, nItem);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ExecUpdateParamByCombox()
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
	if ( SetBarcodeParameterStringByID(SysParam, m_BarcodeParam, ItemText) == false )
	{	return false; }

	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_01 *pListCtrl = (CThisListCtrl_01*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
		BuildBarcodeImage(*pListCtrl, nItem);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::SetBarcodeResultText(LPCTSTR Text)
{
	if ( NULL == Text ) { return; }
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	CWnd::SetDlgItemText(ALGBAR_BARCODE_RESULT_EDIT, Text);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::SetBarcodeParameterStringByID(UINT ParamID, TALG_PARAM_BARCODE_RECOGNIZE &BarcodeParam, LPCTSTR String)//設定條碼參數
{
	int idx=0;
	const int MaxBarcodeDecodeCount = MAX_BARCODE_DECODE_COUNT;
	switch ( ParamID ) 
	{
	case WND_ALG_PROPERTY_BARCODE_1D_ENABLED:
		BarcodeParam.br1DCodeEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_QRCODE_ENABLED:
		BarcodeParam.brQRCodeEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_ENABLED:
		BarcodeParam.brDataMatrixEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_1D_DECODER:
		BarcodeParam.br1DCodeDecoderType = (BARCODE_DECODER_TYPE)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_QRCODE_DECODER:
		BarcodeParam.brQRCodeDecoderType = (BARCODE_DECODER_TYPE)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_DECODER:
		BarcodeParam.brDataMatrixDecoderType = (BARCODE_DECODER_TYPE)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_TIMEOUT:
		BarcodeParam.brDecodeTimeout = (::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_START_STEP:
		BarcodeParam.brDecodeStartStep = (::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_DIRECTION_MODE:
		BarcodeParam.brCodeDirectionMode = (ALG_BARCODE_DIR_MODE)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_COUNT:
		BarcodeParam.brCodeCount = (::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_CONTENT:
		JetAPI::TCHAR2wstring(String, BarcodeParam.brBarcodeContent);
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_CONTENT_ENABLE:
		BarcodeParam.brCodeContentEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_JSON_CHECK_ENABLE:
		BarcodeParam.brCodeJSONCheckEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_CHECK_SUM_ENABLE:
		BarcodeParam.brCheckSumEnabled = (bool)(::_ttoi(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_VERIFY_USL:
		BarcodeParam.brVerifyUSL = (::_ttof(String));
		break;
	case WND_ALG_PROPERTY_BARCODE_VERIFY_LSL:
		BarcodeParam.brVerifyLSL = (::_ttof(String));
		break;

	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_1:
		idx = 0;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_1:
		idx = 0;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_1:
		idx = 0;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_1:
		idx = 0;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_2:
		idx = 1;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_2:
		idx = 1;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_2:
		idx = 1;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_2:
		idx = 1;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_3:
		idx = 2;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_3:
		idx = 2;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_3:
		idx = 2;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_3:
		idx = 2;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_4:
		idx = 3;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_4:
		idx = 3;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_4:
		idx = 3;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_4:
		idx = 3;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_5:
		idx = 4;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_5:
		idx = 4;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_5:
		idx = 4;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_5:
		idx = 4;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_6:
		idx = 5;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_6:
		idx = 5;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_6:
		idx = 5;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_6:
		idx = 5;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_7:
		idx = 6;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_7:
		idx = 6;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_7:
		idx = 6;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_7:
		idx = 6;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_8:
		idx = 7;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].BarcodeStep = (ALG_BARCODE_STEP_MODE)(::_ttoi(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_8:
		idx = 7;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param1 = (::_ttof(String)); }
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_8:
		idx = 7;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Param2 = (::_ttof(String)); }
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_8:
		idx = 7;
		if ( idx < MaxBarcodeDecodeCount )
		{	BarcodeParam.brDecodeStep[idx].Enabled = (::_ttoi(String)); }
		break;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnSaveImageBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString str;
	CString filename = dialog.GetPathName();	
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR ImagePtr=NULL;	
	if ( true == m_ImageEnhanced )
	{	ImagePtr = GetImageBuffer(ImageW, ImageH, ImageStep, BitCount);	}
	else
	{	ImagePtr = GetShowBuffer(ImageW, ImageH, ImageStep, BitCount); }
	if ( NULL == ImagePtr )  { return; }
	if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
	{
		str = ImageAPI.GetImageApiErrorString();
		JetAPI::ShowMessageBox(str);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBarcodeRecognizeWnd::ShowBarcodeLetterList()
{
	CString         str;
	TListNode       Node;	
	CInputListWnd   EnumWnd;
	DWORD_PTR       OldIndex=0;
	CString         strCaption, strLabel;	
	std::vector<TListNode> NodelList;
	const int Len = m_BarcodeText.GetLength();
	const std::map<int, std::string> &AsciiTable=AOIDataCollect.GetAsciiTable();
	
	for ( int i=0; i<Len; i++ )
	{
		Node.Data = i;
		int idx = m_BarcodeText.GetAt(i);
		str.Format(_T("%c"), (char)(idx));
		auto iter = AsciiTable.find(idx);
		if ( iter != AsciiTable.end() )
		{	str = iter->second.c_str();	}
		int len=str.GetLength();
		switch ( len )
		{
		case 0: str += "    "; break;
		case 1: str += "   "; break;
		case 2: str += "  "; break;
		case 3: str += " "; break;
		}		
		Node.Text.Format(_T("%02d, [0x%02X]%s"), i+1, idx, str);		
		//Node.Text.Format(_T("%s, [0x%02X] = [%03d]"), str, i, i);
		NodelList.push_back(Node);		
	}	
	
	strLabel = _T("Letter List");
	strCaption = _T("Barcode Text");		
	EnumWnd.SetParam1(strCaption, strLabel, OldIndex, NodelList);	
	EnumWnd.SetSelIndex1(0);
	EnumWnd.DoModal();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBarcodeRecognizeWnd::OnShowLetterListBtn() 
{
	// TODO: Add your control notification handler code here
	ShowBarcodeLetterList();
}
//-------------------------------------------------------------------------------------//