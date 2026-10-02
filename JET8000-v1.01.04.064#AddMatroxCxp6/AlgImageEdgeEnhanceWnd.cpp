// AlgImageEdgeEnhanceWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgImageEdgeEnhanceWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const bool bShowParam2=false;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageEdgeEnhanceWnd dialog
//-------------------------------------------------------------------------------------//
CAlgImageEdgeEnhanceWnd::CAlgImageEdgeEnhanceWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgImageEdgeEnhanceWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgImageEdgeEnhanceWnd)
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_BitCount = 8;
	m_ImagePtr = NULL;
	
	m_EdgePtr = NULL;	
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgImageEdgeEnhanceWnd)
	DDX_Control(pDX, IMGEDG_IMAGE_WND, m_ImageWnd);	
	DDX_Control(pDX, IMGEDG_EDGE_MODE_COMBO, m_EdgeModeCombox);
	DDX_Control(pDX, IMGEDG_FILTER1_MODE_COMBO, m_Filter1ModeCombox);
	DDX_Control(pDX, IMGEDG_FILTER2_MODE_COMBO, m_Filter2ModeCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgImageEdgeEnhanceWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgImageEdgeEnhanceWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IMGEDG_REDRAW_BTN, OnRedrawBtn)
	ON_CBN_SELCHANGE(IMGEDG_EDGE_MODE_COMBO, OnSelchangeEdgeModeCombo)
	ON_CBN_SELCHANGE(IMGEDG_FILTER1_MODE_COMBO, OnSelchangeFilter1ModeCombo)
	ON_EN_KILLFOCUS(IMGEDG_FILTER1_PARAM_EDIT1, OnKillfocusFilter1ParamEdit1)
	ON_EN_KILLFOCUS(IMGEDG_FILTER1_PARAM_EDIT2, OnKillfocusFilter1ParamEdit2)
	ON_CBN_SELCHANGE(IMGEDG_FILTER2_MODE_COMBO, OnSelchangeFilter2ModeCombo)
	ON_EN_KILLFOCUS(IMGEDG_FILTER2_PARAM_EDIT1, OnKillfocusFilter2ParamEdit1)
	ON_EN_KILLFOCUS(IMGEDG_FILTER2_PARAM_EDIT2, OnKillfocusFilter2ParamEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageEdgeEnhanceWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgImageEdgeEnhanceWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	AOIDataDefine.BuildEdgeEnhanceModeCombox(m_EdgeModeCombox);
	AOIDataDefine.BuildEdgeEnhanceFilterModeCombox(m_Filter1ModeCombox);	
	AOIDataDefine.BuildEdgeEnhanceFilterModeCombox(m_Filter2ModeCombox);
	if ( false == bShowParam2 )
	{
		const BOOL bShow=FALSE;
		JetAPI::ShowCtrlWnd(this, IMGEDG_FILTER1_PARAM_EDIT2, bShow);
		JetAPI::ShowCtrlWnd(this, IMGEDG_FILTER1_PARAM_LABEL2, bShow);
		JetAPI::ShowCtrlWnd(this, IMGEDG_FILTER2_PARAM_EDIT2, bShow);
		JetAPI::ShowCtrlWnd(this, IMGEDG_FILTER2_PARAM_LABEL2, bShow);
	}
	UpdateKernelToUI();
	UpdateEdgeImageWnd(true);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateUIToKernel();
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::SetAlgBinaryParam(const CAlgBinaryParam &BinaryParam)
{
	m_AlgBinaryParam = BinaryParam;
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::CopyAlgEdgeEnhanceParam(CAlgBinaryParam &BinaryParam) const
{
	BinaryParam.CopyEdgeEnhanceParam(m_AlgBinaryParam);
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::SetImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr)
{	
	IMAGE_PTR Ptr=NULL;
	IMAGE_PTR EdgePtr=NULL;	
	const int nAlign = 4;
	IMAGE_SIZE GrayStep=0;
	std::vector<IMAGE_PTR> PtrList;
	const IMAGE_SIZE GrayBitCnt=8;
	if ( 8 == BitCount )
	{	GrayStep = ImageStep;	}
	else
	{	GrayStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBitCnt, nAlign); }
	
	ReleaseImageBuffer();
	const size_t BufferSize=ImageAPI.CalcBufferSize(GrayStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, "CAlgImageEdgeEnhanceWnd::SetImage", "Ptr") == false ||
		 JetMemory.alloc_func(BufferSize, EdgePtr, "CAlgImageEdgeEnhanceWnd::SetImage", "EdgePtr") == false )
	{
		JetMemory.free_func(Ptr);
		JetMemory.free_func(EdgePtr);
		return;	
	}
	PtrList.push_back(Ptr);
	PtrList.push_back(EdgePtr);

	if ( 24 == BitCount )
	{
		if ( ImageAPI.ColorImageToGrayImage3(ImageW, ImageH, ImageStep, ImagePtr, GrayStep, Ptr) == false )
		{
			JetMemory.free_list(PtrList);
			return ; 
		}
	}
	else
	{
		if ( ImageAPI.CloneImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Ptr, false) == false )
		{
			JetMemory.free_list(PtrList);
			return ; 
		}
	}
	::memset(EdgePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	
	m_ImagePtr = Ptr;
	m_EdgePtr = EdgePtr;	
	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = GrayStep;
	m_BitCount = GrayBitCnt;	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_ALG_IMAGE_EDGE_ENHANCE_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IMGEDG_EDGE_MODE_LABEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER1_GROUP));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER1_MODE_LABEL));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER1_PARAM_LABEL1));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER1_PARAM_LABEL2));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER2_GROUP));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER2_MODE_LABEL));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER2_PARAM_LABEL1));
	SetMultiLanauage(LoadIDAndName(IMGEDG_FILTER2_PARAM_LABEL2));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IMGEDG_REDRAW_BTN));
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
bool CAlgImageEdgeEnhanceWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_ALG_IMAGE_EDGE_ENHANCE_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAlgImageEdgeEnhanceWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_IMAGE_EDGE_ENHANCE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::ReleaseImageBuffer()
{
	if ( NULL != m_EdgePtr )
	{	JetMemory.free_func(m_EdgePtr); }

	if ( NULL != m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr); }

	IMAGE_SIZE BitCnt=8;
	IMAGE_SIZE Size=1024;

	m_ImageW = Size;
	m_ImageH = Size;
	m_ImageStep = Size;
	m_BitCount = BitCnt;
	m_EdgePtr = NULL;
	m_ImagePtr = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::ExecRedrawBtn()
{
	UpdateUIToKernel();
	UpdateEdgeImageWnd();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::UpdateKernelToUI()
{
	const CAlgBinaryParam  &BinaryParam=m_AlgBinaryParam;
	JetAPI::SetComboxCurSel(m_EdgeModeCombox, BinaryParam.GetEdgeEnhanceMode());

	{
		const TBINARY_FILTER Filter=BinaryParam.GetEdgeEnhanceFilter1();
		JetAPI::SetComboxCurSel(m_Filter1ModeCombox, Filter.FilterMode);
		CWnd::SetDlgItemInt(IMGEDG_FILTER1_PARAM_EDIT1, Filter.FilterParam1);
		CWnd::SetDlgItemInt(IMGEDG_FILTER1_PARAM_EDIT2, Filter.FilterParam2);
	}

	{
		const TBINARY_FILTER Filter=BinaryParam.GetEdgeEnhanceFilter2();
		JetAPI::SetComboxCurSel(m_Filter2ModeCombox, Filter.FilterMode);
		CWnd::SetDlgItemInt(IMGEDG_FILTER2_PARAM_EDIT1, Filter.FilterParam1);
		CWnd::SetDlgItemInt(IMGEDG_FILTER2_PARAM_EDIT2, Filter.FilterParam2);
	}
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::UpdateUIToKernel()
{
	DWORD Data=0;
	CAlgBinaryParam  &BinaryParam=m_AlgBinaryParam;
	BinaryParam.SetEdgeEnhanceMode((EDGE_ENHANCE_MODE)JetAPI::GetComboxCurSelData(m_EdgeModeCombox));

	{
		TBINARY_FILTER Filter;
		Filter.FilterParam1 = (int)(CWnd::GetDlgItemInt(IMGEDG_FILTER1_PARAM_EDIT1));
		Filter.FilterParam2 = (int)(CWnd::GetDlgItemInt(IMGEDG_FILTER1_PARAM_EDIT2));
		Filter.FilterMode = (NOISE_FILTER_MODE)(JetAPI::GetComboxCurSelData(m_Filter1ModeCombox));
		BinaryParam.SetEdgeEnhanceFilter1(Filter);
	}

	{
		TBINARY_FILTER Filter;
		Filter.FilterParam1 = (int)(CWnd::GetDlgItemInt(IMGEDG_FILTER2_PARAM_EDIT1));
		Filter.FilterParam2 = (int)(CWnd::GetDlgItemInt(IMGEDG_FILTER2_PARAM_EDIT2));
		Filter.FilterMode = (NOISE_FILTER_MODE)(JetAPI::GetComboxCurSelData(m_Filter2ModeCombox));
		BinaryParam.SetEdgeEnhanceFilter2(Filter);
	}
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::UpdateEdgeImageWnd(bool bFirst)
{
	if ( NULL==m_ImagePtr || NULL==m_EdgePtr )
	{	return; }	

	RECT ImageRect;
	CAlgParam AlgParam;	
	CAlgBinaryParam  BinaryParam=m_AlgBinaryParam;
	IMAGE_SIZE ImageW = m_ImageW;
	IMAGE_SIZE ImageH = m_ImageH;
	IMAGE_SIZE BitCount = m_BitCount;
	IMAGE_SIZE ImageStep = m_ImageStep;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	JetAPI::SizeToRect(ImageW, ImageH, ImageRect);	
	::memcpy(m_EdgePtr, m_ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	AlgParam.ExecAlgGrayEdge_Rect(BinaryParam, ImageRect, ImageRect, ImageW, ImageH, ImageStep, m_EdgePtr);
	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, m_EdgePtr, m_EdgePtr);

	bool bRedraw=true;
	if ( true == bFirst )
	{	bRedraw=false;	}	
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, m_EdgePtr, true, false, bRedraw);
	if ( true == bFirst )
	{	m_ImageWnd.ShowFittedZoom();	}
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnRedrawBtn() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();	
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnSelchangeEdgeModeCombo() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnSelchangeFilter1ModeCombo() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnKillfocusFilter1ParamEdit1() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnKillfocusFilter1ParamEdit2() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnSelchangeFilter2ModeCombo() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnKillfocusFilter2ParamEdit1() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
void CAlgImageEdgeEnhanceWnd::OnKillfocusFilter2ParamEdit2() 
{
	// TODO: Add your control notification handler code here
	ExecRedrawBtn();
}
//-------------------------------------------------------------------------------------//
BOOL CAlgImageEdgeEnhanceWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		if ( VK_RETURN == pMsg->wParam )
		{
			ExecRedrawBtn();
			return TRUE;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//