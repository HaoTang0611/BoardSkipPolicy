// EditImageBinaryWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "EditImageBinaryWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const bool bUseCtrlDynamicPos = true;//使用控制項動態位置
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageBinaryWnd dialog
//-------------------------------------------------------------------------------------//
CEditImageBinaryWnd::CEditImageBinaryWnd(CWnd* pParent /*=NULL*/)
	: CBasicDialog(CEditImageBinaryWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageBinaryWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;
	m_BinaryParamPtr = NULL;
	m_FixedThresholdMax = FIXED_THRESHOLD_MAX_2D;
	m_FixedThresholdMin = FIXED_THRESHOLD_MIN_2D;
	m_DynamicThresholdMax = DYNAMIC_THRESHOLD_MAX;
	m_DynamicThresholdMin = DYNAMIC_THRESHOLD_MIN;
	m_RelativeBiasMax = RELATIVE_BIAS_MAX_2D;
	m_RelativeBiasMin = RELATIVE_BIAS_MIN_2D;
	m_RelativeThresholdMax = RELATIVE_THRESHOLD_MAX_2D;
	m_RelativeThresholdMin = RELATIVE_THRESHOLD_MIN_2D;
	m_AdaptiveThresholdGapMax = ADAPTIVE_THRESHOLD_GAP_MAX_2D;
	m_AdaptiveThresholdGapMin = ADAPTIVE_THRESHOLD_GAP_MIN_2D;	
	m_AdaptiveThresholdCalcSizeMin = ADAPTIVE_THRESHOLD_CALC_SIZE_MIN;
	m_AdaptiveThresholdCalcSizeMax = ADAPTIVE_THRESHOLD_CALC_SIZE_MAX;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::DoDataExchange(CDataExchange* pDX)
{
	CBasicDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageBinaryWnd)		
	DDX_Control(pDX, BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_SPIN, m_AdaThresholdCalcSizeSpin);
	DDX_Control(pDX, BINARY_ADAPTIVE_THRESHOLD_GAP_SPIN, m_AdaThresholdGapSpin);
	DDX_Control(pDX, BINARY_ADAPTIVE_THRESHOLD_GAP_SLIDER, m_AdaThresholdGapSlider);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_BIAS_SPIN, m_AveThresholdBiasSpin);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_BIAS_SLIDER, m_AveThresholdBiasSlider);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_BELOW_SPIN, m_AveThresholdBelowSpin);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_BELOW_SLIDER, m_AveThresholdBelowSlider);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_ABOVE_SPIN, m_AveThresholdAboveSpin);
	DDX_Control(pDX, BINARY_AVE_THRESHOLD_ABOVE_SLIDER, m_AveThresholdAboveSlider);
	DDX_Control(pDX, BINARY_DYNAMIC_THRESHOLD_SPIN, m_DynamicThresholdSpin);
	DDX_Control(pDX, BINARY_DYNAMIC_THRESHOLD_SLIDER, m_DynamicThresholdSlider);
	DDX_Control(pDX, BINARY_FIX_THRESHOLD_LOW_SPIN, m_FixThresholdLowSpin);
	DDX_Control(pDX, BINARY_FIX_THRESHOLD_LOW_SLIDER, m_FixThresholdLowSlider);
	DDX_Control(pDX, BINARY_FIX_THRESHOLD_HIGH_SPIN, m_FixThresholdHighSpin);
	DDX_Control(pDX, BINARY_FIX_THRESHOLD_HIGH_SLIDER, m_FixThresholdHighSlider);	
	DDX_Control(pDX, BINARY_WEIGHTING_BLUE_SPIN, m_WeightingBlueSpin);
	DDX_Control(pDX, BINARY_WEIGHTING_BLUE_SLIDER, m_WeightingBlueSlider);
	DDX_Control(pDX, BINARY_WEIGHTING_GREEN_SPIN, m_WeightingGreenSpin);
	DDX_Control(pDX, BINARY_WEIGHTING_GREEN_SLIDER, m_WeightingGreenSlider);
	DDX_Control(pDX, BINARY_WEIGHTING_RED_SPIN, m_WeightingRedSpin);
	DDX_Control(pDX, BINARY_WEIGHTING_RED_SLIDER, m_WeightingRedSlider);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageBinaryWnd, CBasicDialog)
	//{{AFX_MSG_MAP(CEditImageBinaryWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(BINARY_RATIO_THRESHOLD_LOW_BTN, OnRatioThresholdLowBtn)	
	ON_BN_CLICKED(BINARY_RATIO_THRESHOLD_HIGH_BTN, OnRatioThresholdHighBtn)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_WEIGHTING_RED_SPIN, OnDeltaposWeightingRedSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_WEIGHTING_GREEN_SPIN, OnDeltaposWeightingGreenSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_WEIGHTING_BLUE_SPIN, OnDeltaposWeightingBlueSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_FIX_THRESHOLD_HIGH_SPIN, OnDeltaposFixThresholdHighSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_FIX_THRESHOLD_LOW_SPIN, OnDeltaposFixThresholdLowSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_DYNAMIC_THRESHOLD_SPIN, OnDeltaposDynamicThresholdSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_AVE_THRESHOLD_ABOVE_SPIN, OnDeltaposAveThresholdAboveSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_AVE_THRESHOLD_BELOW_SPIN, OnDeltaposAveThresholdBelowSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_AVE_THRESHOLD_BIAS_SPIN, OnDeltaposAveThresholdBiasSpin)	
	ON_NOTIFY(UDN_DELTAPOS, BINARY_ADAPTIVE_THRESHOLD_GAP_SPIN, OnDeltaposAdaptiveThresholdGapSpin)
	ON_NOTIFY(UDN_DELTAPOS, BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_SPIN, OnDeltaposAdaptiveThresholdCalcSizeSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageBinaryWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageBinaryWnd::OnInitDialog() 
{
	CBasicDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	const int WeightingMax = SYNTHESIS_WEIGHTING_MAX;
	const int WeightingMin = SYNTHESIS_WEIGHTING_MIN;
	m_WeightingRedSpin.SetRange32(WeightingMin, WeightingMax);
	m_WeightingRedSlider.SetRange(WeightingMin, WeightingMax);
	m_WeightingGreenSpin.SetRange32(WeightingMin, WeightingMax);
	m_WeightingGreenSlider.SetRange(WeightingMin, WeightingMax);
	m_WeightingBlueSpin.SetRange32(WeightingMin, WeightingMax);
	m_WeightingBlueSlider.SetRange(WeightingMin, WeightingMax);
	
	const int FixThresholdMax = m_FixedThresholdMax;
	const int FixThresholdMin = m_FixedThresholdMin;
	m_FixThresholdHighSpin.SetRange32(FixThresholdMin, FixThresholdMax);
	m_FixThresholdHighSlider.SetRange(FixThresholdMin, FixThresholdMax);	
	m_FixThresholdLowSpin.SetRange32(FixThresholdMin, FixThresholdMax);
	m_FixThresholdLowSlider.SetRange(FixThresholdMin, FixThresholdMax);

	const int DynamicThresholdMax = m_DynamicThresholdMax;
	const int DynamicThresholdMin = m_DynamicThresholdMin;
	m_DynamicThresholdSpin.SetRange32(DynamicThresholdMin, DynamicThresholdMax);
	m_DynamicThresholdSlider.SetRange(DynamicThresholdMin, DynamicThresholdMax);

	const int RelativeThresholdMax = m_RelativeThresholdMax;
	const int RelativeThresholdMin = m_RelativeThresholdMin;
	m_AveThresholdAboveSpin.SetRange32(RelativeThresholdMin, RelativeThresholdMax);
	m_AveThresholdAboveSlider.SetRange(RelativeThresholdMin, RelativeThresholdMax);
	m_AveThresholdBelowSpin.SetRange32(RelativeThresholdMin, RelativeThresholdMax);
	m_AveThresholdBelowSlider.SetRange(RelativeThresholdMin, RelativeThresholdMax);

	const int RelativeBiasMax = m_RelativeBiasMax;
	const int RelativeBiasMin = m_RelativeBiasMin;
	m_AveThresholdBiasSpin.SetRange32(RelativeBiasMin, RelativeBiasMax);
	m_AveThresholdBiasSlider.SetRange(RelativeBiasMin, RelativeBiasMax);	

	const int AdaptiveThresholdGapMax = m_AdaptiveThresholdGapMax;
	const int AdaptiveThresholdGapMin = m_AdaptiveThresholdGapMin;
	m_AdaThresholdGapSpin.SetRange32(AdaptiveThresholdGapMin, AdaptiveThresholdGapMax);
	m_AdaThresholdGapSlider.SetRange(AdaptiveThresholdGapMin, AdaptiveThresholdGapMax);

	const int AdaptiveThresholdCalcSizeMax = m_AdaptiveThresholdCalcSizeMax;
	const int AdaptiveThresholdCalcSizeMin = m_AdaptiveThresholdCalcSizeMin;
	m_AdaThresholdCalcSizeSpin.SetRange32(AdaptiveThresholdCalcSizeMin, AdaptiveThresholdCalcSizeMax);
	
	SwitchMultiLanguage();
	AdjustUIForBinaryParam();	
	UpdateBinaryParamToUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDestroy() 
{
	CBasicDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBasicDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( false == bUseCtrlDynamicPos ) { return; }
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_WeightingRedSlider.GetSafeHwnd() == NULL ) { return; }	
	RECT  WndRect;
	UINT  CtrlID = 0;	
	CWnd *WndPtr = NULL;
	POINT Point, Offset;

	//Fix Threshold
	CtrlID = BINARY_FIX_THRESHOLD_HIGH_LABEL;
	WndPtr = CWnd::GetDlgItem(CtrlID);
	if ( NULL==WndPtr || NULL==WndPtr->GetSafeHwnd() ) { return; }
	WndPtr->GetWindowRect(&WndRect);
	Point.x = WndRect.left;
	Point.y = WndRect.top;

	//Dynamic Threshold
	CtrlID = BINARY_DYNAMIC_THRESHOLD_LABEL;
	WndPtr = CWnd::GetDlgItem(CtrlID);
	if ( NULL==WndPtr || NULL==WndPtr->GetSafeHwnd() ) { return; }
	WndPtr->GetWindowRect(&WndRect);
	Offset.x = Point.x-WndRect.left;
	Offset.y = Point.y-WndRect.top;
	JetAPI::MoveCtrlWnd(this, BINARY_DYNAMIC_THRESHOLD_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_DYNAMIC_THRESHOLD_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_DYNAMIC_THRESHOLD_SLIDER, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_DYNAMIC_THRESHOLD_SPIN, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_DYNAMIC_THRESHOLD_EDIT2, Offset);

	//Above/Below Ave
	CtrlID = BINARY_AVE_THRESHOLD_ABOVE_LABEL;
	WndPtr = CWnd::GetDlgItem(CtrlID);
	if ( NULL==WndPtr || NULL==WndPtr->GetSafeHwnd() ) { return; }
	WndPtr->GetWindowRect(&WndRect);
	Offset.x = Point.x-WndRect.left;
	Offset.y = Point.y-WndRect.top;
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_ABOVE_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_ABOVE_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_ABOVE_SLIDER, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_ABOVE_SPIN, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BELOW_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BELOW_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BELOW_SLIDER, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BELOW_SPIN, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BIAS_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BIAS_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BIAS_SLIDER, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_AVE_THRESHOLD_BIAS_SPIN, Offset);

	//Adaptive Threshold 
	CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_LABE;
	WndPtr = CWnd::GetDlgItem(CtrlID);
	if ( NULL==WndPtr || NULL==WndPtr->GetSafeHwnd() ) { return; }
	WndPtr->GetWindowRect(&WndRect);
	Offset.x = Point.x-WndRect.left;
	Offset.y = Point.y-WndRect.top;
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_GAP_LABE, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_GAP_SLIDER, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_GAP_SPIN, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_LABEL, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_SPIN, Offset);	
	
	/*
	const int GapH=52;
	CtrlID = BINARY_IMAGE_HISTOGRAM_WND;
	WndPtr = CWnd::GetDlgItem(CtrlID);
	if ( NULL==WndPtr || NULL==WndPtr->GetSafeHwnd() ) { return; }
	WndPtr->GetWindowRect(&WndRect);
	Offset.x = Point.x-WndRect.left;
	Offset.y = (Point.y+GapH)-WndRect.top;
	JetAPI::MoveCtrlWnd(this, BINARY_IMAGE_HISTOGRAM_WND, Offset);	
	*/
	//JetAPI::MoveCtrlWnd(this, AAAAAAAAAAAAAAAAAAA, Offset);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBasicDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_BINARY_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_IMAGE_BINARY_WND;
	WndKey = _T("IDD_EDIT_IMAGE_BINARY_WND");
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
	WndID = BINARY_WEIGHTING_GROUP;
	WndKey = _T("BINARY_WEIGHTING_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_WEIGHTING_RED_LABEL;
	WndKey = _T("BINARY_WEIGHTING_RED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_WEIGHTING_GREEN_LABEL;
	WndKey = _T("BINARY_WEIGHTING_GREEN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_WEIGHTING_BLUE_LABEL;
	WndKey = _T("BINARY_WEIGHTING_BLUE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = BINARY_PARAMETER_GROUP;
	WndKey = _T("BINARY_PARAMETER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = BINARY_FIX_THRESHOLD_HIGH_LABEL;
	WndKey = _T("BINARY_FIX_THRESHOLD_HIGH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_FIX_THRESHOLD_LOW_LABEL;
	WndKey = _T("BINARY_FIX_THRESHOLD_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_RATIO_THRESHOLD_HIGH_BTN;
	WndKey = _T("BINARY_RATIO_THRESHOLD_HIGH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_RATIO_THRESHOLD_LOW_BTN;
	WndKey = _T("BINARY_RATIO_THRESHOLD_LOW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_DYNAMIC_THRESHOLD_LABEL;
	WndKey = _T("BINARY_DYNAMIC_THRESHOLD_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_AVE_THRESHOLD_ABOVE_LABEL;
	WndKey = _T("BINARY_AVE_THRESHOLD_ABOVE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_AVE_THRESHOLD_BELOW_LABEL;
	WndKey = _T("BINARY_AVE_THRESHOLD_BELOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_AVE_THRESHOLD_BIAS_LABEL;
	WndKey = _T("BINARY_AVE_THRESHOLD_BIAS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = BINARY_ADAPTIVE_THRESHOLD_GAP_LABE;
	WndKey = _T("BINARY_ADAPTIVE_THRESHOLD_GAP_LABE");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_LABEL;
	WndKey = _T("BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CEditImageBinaryWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_BINARY_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::ChangeDrawModelMode()
{
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
}
//-------------------------------------------------------------------------------------//
bool CEditImageBinaryWnd::CheckBinaryParamPtr()
{
	if ( NULL == CEditImageBinaryWnd::m_BinaryParamPtr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::SetBinaryParam(CAOIWnd *WndPtr, CAlgBinaryParam *ParamPtr, bool UpdateToUI)
{
	m_WndPtr = WndPtr;
	m_BinaryParamPtr = ParamPtr;
	if ( this->GetSafeHwnd()!=NULL && true==UpdateToUI )
	{	
		AdjustUIForBinaryParam();
		UpdateBinaryParamToUI();	
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::AdjustUIForBinaryParam()
{
	if ( CheckBinaryParamPtr() == false ) { return ; }

	int FixThresholdMax = 0;
	int FixThresholdMin = 0;
	int RelativeBiasdMax = 0;
	int RelativeBiasdMin = 0;
	int RelativeThresholdMax = 0;
	int RelativeThresholdMin = 0;
	int AdaptiveThresholdGapMax = 0;
	int AdaptiveThresholdGapMin = 0;
	int AdaptiveThresholdCalcSizeMin = 0;
	int AdaptiveThresholdCalcSizeMax = 0;
	const BOOL bRedraw = TRUE;//重繪控制項介面
	const int FixThresholdHigh = m_BinaryParamPtr->GetFixedThresholdHigh();
	const int FixThresholdLow = m_BinaryParamPtr->GetFixedThresholdLow();
	const int RelativeThresholdBias = m_BinaryParamPtr->GetRelativeAveThresholdBias();
	const int RelativeThresholdAbove = m_BinaryParamPtr->GetRelativeAveThresholdAbove();
	const int RelativeThresholdBelow = m_BinaryParamPtr->GetRelativeAveThresholdBelow();
	const int AdaptiveThresholdGap = m_BinaryParamPtr->GetAdaptiveThresholdGap();
	const int AdaptiveThresholdCalcSize = m_BinaryParamPtr->GetAdaptiveThresholdCalcSize();
	const unsigned int FrameUniqueID = m_BinaryParamPtr->GetBinaryFrameUniqueID();
	TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL==FrameParamPtr || FRAME_SPACE!=FrameParamPtr->FrameType )
	{
		RelativeBiasdMax = RELATIVE_BIAS_MAX_2D;
		RelativeBiasdMin = RELATIVE_BIAS_MIN_2D;
		FixThresholdMax = FIXED_THRESHOLD_MAX_2D;
		FixThresholdMin = FIXED_THRESHOLD_MIN_2D;
		RelativeThresholdMax = RELATIVE_THRESHOLD_MAX_2D;
		RelativeThresholdMin = RELATIVE_THRESHOLD_MIN_2D;
		AdaptiveThresholdGapMax = ADAPTIVE_THRESHOLD_GAP_MAX_2D;
		AdaptiveThresholdGapMin = ADAPTIVE_THRESHOLD_GAP_MIN_2D;
	}
	else
	{
		RelativeBiasdMax = RELATIVE_BIAS_MAX_3D;
		RelativeBiasdMin = RELATIVE_BIAS_MIN_3D;
		FixThresholdMax = FIXED_THRESHOLD_MAX_3D;
		FixThresholdMin = FIXED_THRESHOLD_MIN_3D;
		RelativeThresholdMax = RELATIVE_THRESHOLD_MAX_3D;
		RelativeThresholdMin = RELATIVE_THRESHOLD_MIN_3D;
		AdaptiveThresholdGapMax = ADAPTIVE_THRESHOLD_GAP_MAX_3D;
		AdaptiveThresholdGapMin = ADAPTIVE_THRESHOLD_GAP_MIN_3D;
	}
	AdaptiveThresholdCalcSizeMin = ADAPTIVE_THRESHOLD_CALC_SIZE_MIN;
	AdaptiveThresholdCalcSizeMax = ADAPTIVE_THRESHOLD_CALC_SIZE_MAX;	
	
	m_FixedThresholdMax = FixThresholdMax;
	m_FixedThresholdMin = FixThresholdMin;
	m_DynamicThresholdMax = DYNAMIC_THRESHOLD_MAX;
	m_DynamicThresholdMin = DYNAMIC_THRESHOLD_MIN;
	m_RelativeBiasMax = RelativeBiasdMax;
	m_RelativeBiasMin = RelativeBiasdMin;
	m_RelativeThresholdMax = RelativeThresholdMax;
	m_RelativeThresholdMin = RelativeThresholdMin;
	m_AdaptiveThresholdGapMax = AdaptiveThresholdGapMax;
	m_AdaptiveThresholdGapMin = AdaptiveThresholdGapMin;
	m_AdaptiveThresholdCalcSizeMin = AdaptiveThresholdCalcSizeMin;
	m_AdaptiveThresholdCalcSizeMax = AdaptiveThresholdCalcSizeMax;
	
	m_FixThresholdHighSpin.SetRange32(FixThresholdMin, FixThresholdMax);
	m_FixThresholdHighSlider.SetRange(FixThresholdMin, FixThresholdMax, bRedraw);	
	m_FixThresholdLowSpin.SetRange32(FixThresholdMin, FixThresholdMax);
	m_FixThresholdLowSlider.SetRange(FixThresholdMin, FixThresholdMax, bRedraw);
	m_FixThresholdHighSpin.SetPos32(FixThresholdHigh);
	m_FixThresholdHighSlider.SetPos(FixThresholdHigh);
	m_FixThresholdLowSpin.SetPos32(FixThresholdLow);
	m_FixThresholdLowSlider.SetPos(FixThresholdLow);	
	
	m_AveThresholdAboveSpin.SetRange32(RelativeThresholdMin, RelativeThresholdMax);
	m_AveThresholdAboveSlider.SetRange(RelativeThresholdMin, RelativeThresholdMax, bRedraw);	
	m_AveThresholdBelowSpin.SetRange32(RelativeThresholdMin, RelativeThresholdMax);
	m_AveThresholdBelowSlider.SetRange(RelativeThresholdMin, RelativeThresholdMax, bRedraw);
	m_AveThresholdAboveSpin.SetPos32(RelativeThresholdAbove);
	m_AveThresholdAboveSlider.SetPos(RelativeThresholdAbove);
	m_AveThresholdBelowSpin.SetPos32(RelativeThresholdBelow);
	m_AveThresholdBelowSlider.SetPos(RelativeThresholdBelow);
	
	m_AveThresholdBiasSpin.SetRange32(RelativeBiasdMin, RelativeBiasdMax);
	m_AveThresholdBiasSlider.SetRange(RelativeBiasdMin, RelativeBiasdMax, bRedraw);	
	m_AveThresholdBiasSpin.SetPos32(RelativeThresholdBias);
	m_AveThresholdBiasSlider.SetPos(RelativeThresholdBias);

	m_AdaThresholdGapSpin.SetRange32(AdaptiveThresholdGapMin, AdaptiveThresholdGapMax);
	m_AdaThresholdGapSlider.SetRange(AdaptiveThresholdGapMin, AdaptiveThresholdGapMax, bRedraw);
	m_AdaThresholdGapSpin.SetPos32(AdaptiveThresholdGap);
	m_AdaThresholdGapSlider.SetPos(AdaptiveThresholdGap);
	
	m_AdaThresholdCalcSizeSpin.SetRange32(AdaptiveThresholdCalcSizeMin, AdaptiveThresholdCalcSizeMax);	
	m_AdaThresholdCalcSizeSpin.SetPos32(AdaptiveThresholdCalcSize);		

	UpdateBinaryWndEnable();
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::UpdateBinaryWndEnable()
{
	if ( CheckBinaryParamPtr() == false ) { return; }

	BINARY_MODE      BinaryMode = m_BinaryParamPtr->GetBinaryMode();
	IMAGE_SRC_MODE   ImageSourceMode = m_BinaryParamPtr->GetBinaryImageSourceMode();

	UINT  CtrlID=0;
	BOOL  bEnable = TRUE;
	const bool bSwitchShow = bUseCtrlDynamicPos;
	if ( IMAGE_SRC_SYNTHESIS == ImageSourceMode )
	{	bEnable = TRUE;	}
	else
	{	bEnable = FALSE;	}
	//CWnd::SetRedraw(FALSE);//不要使用，會導致Z排序未更新

	CtrlID = BINARY_WEIGHTING_RED_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_RED_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_RED_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_WEIGHTING_GREEN_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_GREEN_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_GREEN_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_WEIGHTING_BLUE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_BLUE_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_WEIGHTING_BLUE_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	if ( BINARY_FIXED_THRESHOLD == BinaryMode )
	{	bEnable = TRUE;	}
	else
	{	bEnable = FALSE;	}
	CtrlID = BINARY_FIX_THRESHOLD_HIGH_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_FIX_THRESHOLD_HIGH_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_FIX_THRESHOLD_HIGH_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_FIX_THRESHOLD_LOW_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_FIX_THRESHOLD_LOW_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_FIX_THRESHOLD_LOW_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	if ( true == bSwitchShow )
	{
		CtrlID = BINARY_FIX_THRESHOLD_HIGH_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_HIGH_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_HIGH_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_HIGH_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_RATIO_THRESHOLD_HIGH_BTN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);

		CtrlID = BINARY_FIX_THRESHOLD_LOW_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_LOW_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_LOW_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_FIX_THRESHOLD_LOW_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_RATIO_THRESHOLD_LOW_BTN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
	}

	if ( BINARY_DYNAMIC_THRESHOLD == BinaryMode )
	{	bEnable = TRUE;	}
	else
	{	bEnable = FALSE;	}
	CtrlID = BINARY_DYNAMIC_THRESHOLD_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_DYNAMIC_THRESHOLD_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_DYNAMIC_THRESHOLD_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	if ( true == bSwitchShow )
	{
		CtrlID = BINARY_DYNAMIC_THRESHOLD_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_DYNAMIC_THRESHOLD_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_DYNAMIC_THRESHOLD_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_DYNAMIC_THRESHOLD_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_DYNAMIC_THRESHOLD_EDIT2;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
	}


	if ( BINARY_RELATIVE_AVE_THRESHOLD == BinaryMode )
	{	bEnable = TRUE;	}
	else
	{	bEnable = FALSE;	}
	CtrlID = BINARY_AVE_THRESHOLD_ABOVE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_ABOVE_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_ABOVE_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_AVE_THRESHOLD_BELOW_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_BELOW_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_BELOW_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_AVE_THRESHOLD_BIAS_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_BIAS_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_AVE_THRESHOLD_BIAS_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	if ( true == bSwitchShow )
	{
		CtrlID = BINARY_AVE_THRESHOLD_ABOVE_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_ABOVE_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_ABOVE_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_ABOVE_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);

		CtrlID = BINARY_AVE_THRESHOLD_BELOW_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BELOW_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BELOW_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BELOW_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);

		CtrlID = BINARY_AVE_THRESHOLD_BIAS_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BIAS_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BIAS_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_AVE_THRESHOLD_BIAS_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
	}

	if ( BINARY_ADAPTIVE_THRESHOLD == BinaryMode )
	{	bEnable = TRUE;	}
	else
	{	bEnable = FALSE;	}
	CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_SLIDER;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	if ( true == bSwitchShow )
	{
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_LABE;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);		
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_SLIDER;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_GAP_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);

		CtrlID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_LABEL;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
		CtrlID = BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_SPIN;
		JetAPI::ShowCtrlWnd(this, CtrlID, bEnable);
	}

	//CWnd::SetRedraw(TRUE);
	//CWnd::Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::UpdateBinaryParamToUI()
{
	if ( CheckBinaryParamPtr() == false ) { return; }

	CString  str;
	int      nValue=0;	

	//Weighting Red
	nValue = m_BinaryParamPtr->GetBinarySynthesisWR();
	str.Format(_T("%d"), nValue);
	m_WeightingRedSpin.SetPos32(nValue);
	m_WeightingRedSlider.SetPos(nValue);
	SetDlgItemText(BINARY_WEIGHTING_RED_EDIT, str);

	//Weighting Green
	nValue = m_BinaryParamPtr->GetBinarySynthesisWG();
	str.Format(_T("%d"), nValue);
	m_WeightingGreenSpin.SetPos32(nValue);
	m_WeightingGreenSlider.SetPos(nValue);
	SetDlgItemText(BINARY_WEIGHTING_GREEN_EDIT, str);

	//Weighting Blue
	nValue = m_BinaryParamPtr->GetBinarySynthesisWB();
	str.Format(_T("%d"), nValue);
	m_WeightingBlueSpin.SetPos32(nValue);
	m_WeightingBlueSlider.SetPos(nValue);
	SetDlgItemText(BINARY_WEIGHTING_BLUE_EDIT, str);

	//Fixed Threshold High
	nValue = m_BinaryParamPtr->GetFixedThresholdHigh();
	str.Format(_T("%d"), nValue);
	m_FixThresholdHighSpin.SetPos32(nValue);
	m_FixThresholdHighSlider.SetPos(nValue);
	SetDlgItemText(BINARY_FIX_THRESHOLD_HIGH_EDIT, str);

	//Fixed Threshold Low
	nValue = m_BinaryParamPtr->GetFixedThresholdLow();
	str.Format(_T("%d"), nValue);
	m_FixThresholdLowSpin.SetPos32(nValue);
	m_FixThresholdLowSlider.SetPos(nValue);
	SetDlgItemText(BINARY_FIX_THRESHOLD_LOW_EDIT, str);

	//Dynamic Threshold Ratio
	nValue = JetAPI::Floor(m_BinaryParamPtr->GetDynamicThresholdRatio());
	str.Format(_T("%d"), nValue);
	m_DynamicThresholdSpin.SetPos32(nValue);
	m_DynamicThresholdSlider.SetPos(nValue);
	SetDlgItemText(BINARY_DYNAMIC_THRESHOLD_EDIT, str);
	nValue = m_BinaryParamPtr->GetDynamicThresholdValue();
	str.Format(_T("%d"), nValue);
	SetDlgItemText(BINARY_DYNAMIC_THRESHOLD_EDIT2, str);	

	//Above Average Threshold
	nValue = m_BinaryParamPtr->GetRelativeAveThresholdAbove();
	str.Format(_T("%d"), nValue);
	m_AveThresholdAboveSpin.SetPos32(nValue);
	m_AveThresholdAboveSlider.SetPos(nValue);
	SetDlgItemText(BINARY_AVE_THRESHOLD_ABOVE_EDIT, str);

	//Below Average Threshold
	nValue = m_BinaryParamPtr->GetRelativeAveThresholdBelow();
	str.Format(_T("%d"), nValue);
	m_AveThresholdBelowSpin.SetPos32(nValue);
	m_AveThresholdBelowSlider.SetPos(nValue);
	SetDlgItemText(BINARY_AVE_THRESHOLD_BELOW_EDIT, str);

	//Bias Average Threshold
	nValue = m_BinaryParamPtr->GetRelativeAveThresholdBias();
	str.Format(_T("%d"), nValue);
	m_AveThresholdBiasSpin.SetPos32(nValue);
	m_AveThresholdBiasSlider.SetPos(nValue);
	SetDlgItemText(BINARY_AVE_THRESHOLD_BIAS_EDIT, str);

	//Adaptive Threshold
	nValue = m_BinaryParamPtr->GetAdaptiveThresholdGap();
	str.Format(_T("%d"), nValue);
	m_AdaThresholdGapSpin.SetPos32(nValue);
	m_AdaThresholdGapSlider.SetPos(nValue);
	SetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT, str);

	nValue = m_BinaryParamPtr->GetAdaptiveThresholdCalcSize();
	str.Format(_T("%d"), nValue);
	m_AdaThresholdCalcSizeSpin.SetPos32(nValue);
	SetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT, str);		
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::UpdateBinaryUIToParam()
{
	if ( CheckBinaryParamPtr() == false ) { return; }

	CString  str;
	int      nValue=0;	
	int      nValue1=0;	
	int      nValue2=0;	
	CAlgBinaryParam BinaryOld = *m_BinaryParamPtr;

	//Weighting Red
	this->GetDlgItemText(BINARY_WEIGHTING_RED_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetBinarySynthesisWR(nValue);	

	//Weighting Green
	this->GetDlgItemText(BINARY_WEIGHTING_GREEN_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetBinarySynthesisWG(nValue);

	//Weighting Blue
	this->GetDlgItemText(BINARY_WEIGHTING_BLUE_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetBinarySynthesisWB(nValue);
	
	//Fixed Threshold High, Low
	this->GetDlgItemText(BINARY_FIX_THRESHOLD_HIGH_EDIT, str);
	nValue1 = ::_ttoi(str);
	this->GetDlgItemText(BINARY_FIX_THRESHOLD_LOW_EDIT, str);
	nValue2 = ::_ttoi(str);

	m_BinaryParamPtr->SetFixedThresholdHigh(MAX(nValue1, nValue2));	
	m_BinaryParamPtr->SetFixedThresholdLow(MIN(nValue1, nValue2));	

	//Dynamic Threshold Ratio
	this->GetDlgItemText(BINARY_DYNAMIC_THRESHOLD_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetDynamicThresholdRatio((double)(nValue));

	//Above Average Threshold
	this->GetDlgItemText(BINARY_AVE_THRESHOLD_ABOVE_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetRelativeAveThresholdAbove(nValue);	

	//Below Average Threshold
	this->GetDlgItemText(BINARY_AVE_THRESHOLD_BELOW_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetRelativeAveThresholdBelow(nValue);

	//Bias Average Threshold
	this->GetDlgItemText(BINARY_AVE_THRESHOLD_BIAS_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetRelativeAveThresholdBias(nValue);	

	//Adaptive Threshold
	CWnd::GetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT, str);
	nValue = ::_ttoi(str);
	m_BinaryParamPtr->SetAdaptiveThresholdGap(nValue);

	CWnd::GetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT, str);
	nValue = ::_ttoi(str);
	if ( nValue < m_AdaptiveThresholdCalcSizeMin )
	{
		nValue = m_AdaptiveThresholdCalcSizeMin;
		CWnd::SetDlgItemInt(BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT, nValue);
	}
	m_BinaryParamPtr->SetAdaptiveThresholdCalcSize(nValue);

	CAOIWnd *WndPtr = m_WndPtr;
	LogOperCtrl.SaveLogModelWndAlgBinaryCompare(WndPtr, &BinaryOld, m_BinaryParamPtr);

	ChangeDrawModelMode();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default	
	if ( nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION )
	{
		if ( CheckBinaryParamPtr() != false )
		{	
			CString    str;			
			int        PosUsed=0;
			int        PosOther=0;
			bool       bChanged=false;			
			int        Pos = (int)(nPos);//超過65535會失效						
			const UINT CtrlID = pScrollBar->GetDlgCtrlID();		
			CAlgBinaryParam BinaryOld = *m_BinaryParamPtr;

			switch ( CtrlID )
			{
			case BINARY_WEIGHTING_RED_SLIDER:				
				Pos = m_WeightingRedSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_WeightingRedSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_WEIGHTING_RED_EDIT, str);
				m_BinaryParamPtr->SetBinarySynthesisWR(Pos);
				bChanged = true;
				break;
			case BINARY_WEIGHTING_GREEN_SLIDER:
				Pos = m_WeightingGreenSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_WeightingGreenSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_WEIGHTING_GREEN_EDIT, str);
				m_BinaryParamPtr->SetBinarySynthesisWG(Pos);
				bChanged = true;
				break;
			case BINARY_WEIGHTING_BLUE_SLIDER:
				Pos = m_WeightingBlueSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_WeightingBlueSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_WEIGHTING_BLUE_EDIT, str);
				m_BinaryParamPtr->SetBinarySynthesisWB(Pos);
				bChanged = true;
				break;
			case BINARY_FIX_THRESHOLD_HIGH_SLIDER:
				Pos = m_FixThresholdHighSlider.GetPos();
				PosOther = m_BinaryParamPtr->GetFixedThresholdLow();
				PosUsed = MAX(Pos, PosOther+1);				
				str.Format(_T("%d"), PosUsed);
				m_FixThresholdHighSpin.SetPos32(PosUsed);
				CWnd::SetDlgItemText(BINARY_FIX_THRESHOLD_HIGH_EDIT, str);
				m_BinaryParamPtr->SetFixedThresholdHigh(PosUsed);
				if ( PosUsed != Pos )
				{	m_FixThresholdHighSlider.SetPos(PosUsed); }
				bChanged = true;
				break;
			case BINARY_FIX_THRESHOLD_LOW_SLIDER:
				Pos = m_FixThresholdLowSlider.GetPos();
				PosOther = m_BinaryParamPtr->GetFixedThresholdHigh();
				PosUsed = MIN(Pos, PosOther-1);
				str.Format(_T("%d"), PosUsed);
				m_FixThresholdLowSpin.SetPos32(PosUsed);
				CWnd::SetDlgItemText(BINARY_FIX_THRESHOLD_LOW_EDIT, str);
				m_BinaryParamPtr->SetFixedThresholdLow(PosUsed);
				if ( PosUsed != Pos )
				{	m_FixThresholdLowSlider.SetPos(PosUsed); }
				bChanged = true;
				break;
			case BINARY_DYNAMIC_THRESHOLD_SLIDER:
				Pos = m_DynamicThresholdSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_DynamicThresholdSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_DYNAMIC_THRESHOLD_EDIT, str);
				m_BinaryParamPtr->SetDynamicThresholdRatio(Pos);
				bChanged = true;
				break;
			case BINARY_AVE_THRESHOLD_ABOVE_SLIDER:
				Pos = m_AveThresholdAboveSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_AveThresholdAboveSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_ABOVE_EDIT, str);
				m_BinaryParamPtr->SetRelativeAveThresholdAbove(Pos);
				bChanged = true;
				break;
			case BINARY_AVE_THRESHOLD_BELOW_SLIDER:
				Pos = m_AveThresholdBelowSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_AveThresholdBelowSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_BELOW_EDIT, str);
				m_BinaryParamPtr->SetRelativeAveThresholdBelow(Pos);
				bChanged = true;
				break;
			case BINARY_AVE_THRESHOLD_BIAS_SLIDER:
				Pos = m_AveThresholdBiasSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_AveThresholdBiasSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_BIAS_EDIT, str);
				m_BinaryParamPtr->SetRelativeAveThresholdBias(Pos);
				bChanged = true;
				break;
			case BINARY_ADAPTIVE_THRESHOLD_GAP_SLIDER:
				Pos = m_AdaThresholdGapSlider.GetPos();
				str.Format(_T("%d"), Pos);
				m_AdaThresholdGapSpin.SetPos32(Pos);
				CWnd::SetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT, str);
				m_BinaryParamPtr->SetAdaptiveThresholdGap(Pos);
				bChanged = true;
				break;
			default:
				break;
			}			

			if ( true == bChanged )
			{	
				CAOIWnd *WndPtr = m_WndPtr;
				LogOperCtrl.SaveLogModelWndAlgBinaryCompare(WndPtr, &BinaryOld, m_BinaryParamPtr);	
				UpdateBinaryUIToParam();	
				UpdateBinaryParamToParentWnd();
			}
		}
	}
	CBasicDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposWeightingRedSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here	
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > SYNTHESIS_WEIGHTING_MAX ) { nNextPos = SYNTHESIS_WEIGHTING_MAX; }
	else if ( nNextPos < SYNTHESIS_WEIGHTING_MIN ) { nNextPos = SYNTHESIS_WEIGHTING_MIN; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_WeightingRedSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_WEIGHTING_RED_EDIT, str);
	m_BinaryParamPtr->SetBinarySynthesisWR(nNextPos);	
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_WEIGHTING_RED_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_WEIGHTING_GROUP, strValueName, strValueOld, strValueNew);

	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposWeightingGreenSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > SYNTHESIS_WEIGHTING_MAX ) { nNextPos = SYNTHESIS_WEIGHTING_MAX; }
	else if ( nNextPos < SYNTHESIS_WEIGHTING_MIN ) { nNextPos = SYNTHESIS_WEIGHTING_MIN; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_WeightingGreenSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_WEIGHTING_GREEN_EDIT, str);
	m_BinaryParamPtr->SetBinarySynthesisWG(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_WEIGHTING_GREEN_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_WEIGHTING_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposWeightingBlueSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > SYNTHESIS_WEIGHTING_MAX ) { nNextPos = SYNTHESIS_WEIGHTING_MAX; }
	else if ( nNextPos < SYNTHESIS_WEIGHTING_MIN ) { nNextPos = SYNTHESIS_WEIGHTING_MIN; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_WeightingBlueSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_WEIGHTING_BLUE_EDIT, str);
	m_BinaryParamPtr->SetBinarySynthesisWB(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_WEIGHTING_BLUE_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_WEIGHTING_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposFixThresholdHighSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	const int MinValue = m_BinaryParamPtr->GetFixedThresholdLow()+1;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > m_FixedThresholdMax ) { nNextPos = m_FixedThresholdMax; }
	else if ( nNextPos < MinValue ) { nNextPos = MinValue; }
	else if ( nNextPos < m_FixedThresholdMin ) { nNextPos = m_FixedThresholdMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_FixThresholdHighSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_FIX_THRESHOLD_HIGH_EDIT, str);
	m_BinaryParamPtr->SetFixedThresholdHigh(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_FIX_THRESHOLD_HIGH_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposFixThresholdLowSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	const int MaxValue = m_BinaryParamPtr->GetFixedThresholdHigh()+1;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > MaxValue ) { nNextPos = MaxValue; }
	else if ( nNextPos > m_FixedThresholdMax ) { nNextPos = m_FixedThresholdMax; }
	else if ( nNextPos < m_FixedThresholdMin ) { nNextPos = m_FixedThresholdMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_FixThresholdLowSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_FIX_THRESHOLD_LOW_EDIT, str);
	m_BinaryParamPtr->SetFixedThresholdLow(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_FIX_THRESHOLD_LOW_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposDynamicThresholdSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > (int)(m_DynamicThresholdMax) ) { nNextPos = (int)(m_DynamicThresholdMax); }
	else if ( nNextPos < (int)(m_DynamicThresholdMin) ) { nNextPos = (int)(m_DynamicThresholdMin); }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_DynamicThresholdSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_DYNAMIC_THRESHOLD_EDIT, str);
	m_BinaryParamPtr->SetDynamicThresholdRatio((double)(nNextPos));
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_DYNAMIC_THRESHOLD_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposAveThresholdAboveSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > m_RelativeThresholdMax ) { nNextPos = m_RelativeThresholdMax; }
	else if ( nNextPos < m_RelativeThresholdMin ) { nNextPos = m_RelativeThresholdMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_AveThresholdAboveSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_ABOVE_EDIT, str);
	m_BinaryParamPtr->SetRelativeAveThresholdAbove(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_AVE_THRESHOLD_ABOVE_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposAveThresholdBelowSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > m_RelativeThresholdMax ) { nNextPos = m_RelativeThresholdMax; }
	else if ( nNextPos < m_RelativeThresholdMin ) { nNextPos = m_RelativeThresholdMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_AveThresholdBelowSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_BELOW_EDIT, str);
	m_BinaryParamPtr->SetRelativeAveThresholdBelow(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);
	CWnd::GetDlgItemText(BINARY_AVE_THRESHOLD_BELOW_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposAveThresholdBiasSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here	
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }		
	if ( nNextPos > m_RelativeBiasMax ) { nNextPos = m_RelativeBiasMax; }
	else if ( nNextPos < m_RelativeBiasMin ) { nNextPos = m_RelativeBiasMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_AveThresholdBiasSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_AVE_THRESHOLD_BIAS_EDIT, str);
	m_BinaryParamPtr->SetRelativeAveThresholdBias(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);
	CWnd::GetDlgItemText(BINARY_AVE_THRESHOLD_BIAS_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateBinaryUIToParam();
	UpdateBinaryParamToUI();
	UpdateBinaryParamToParentWnd();
	return;
	CBasicDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CBasicDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::UpdateBinaryParamToParentWnd()
{
	if ( CheckBinaryParamPtr() == false ) { return; }

	CAlgBinaryParam BinaryParam;
	AOIDataCollect.GetBinaryParamTemp(BinaryParam);
	BinaryParam = *m_BinaryParamPtr;
	AOIDataCollect.SetBinaryParamTemp(BinaryParam);

	UINT message = MSG_EDIT_IMAGE_PROCESS_WND;
	WPARAM wParam = WPARAM_UPDATE_ALG_PARAM;
	LPARAM lParam = TRUE;
	SendParentWndMessage(message, wParam, lParam);
	//PostParentWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);		
}
//-------------------------------------------------------------------------------------//
bool CEditImageBinaryWnd::ExecSaveLogModelWndOperate_Binary(UINT  nGroup, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)
{	
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == m_BinaryParamPtr ) { return false; }	
	CString sGroup;
	CString sOper = AOIDataDefine.GetSetText();	
	if ( BINARY_WEIGHTING_GROUP == nGroup )
	{	CWnd::GetDlgItemText(nGroup, sGroup);	}
	else
	{	sGroup = AOIDataDefine.GetAlgBinaryModeText(m_BinaryParamPtr->GetBinaryMode());	}
	LogOperCtrl.SaveLogModelWndOperate(WndPtr, sOper, sGroup, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposAdaptiveThresholdGapSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > m_AdaptiveThresholdGapMax ) { nNextPos = m_AdaptiveThresholdGapMax; }
	else if ( nNextPos < m_AdaptiveThresholdGapMin ) { nNextPos = m_AdaptiveThresholdGapMin; }
	pNMUpDown->iDelta = nNextPos-nPos;	

	CString    str;
	str.Format(_T("%d"), nNextPos);
	m_AdaThresholdGapSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_GAP_EDIT, str);
	m_BinaryParamPtr->SetAdaptiveThresholdGap(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_GAP_LABE, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnDeltaposAdaptiveThresholdCalcSizeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	if ( nNextPos > m_AdaptiveThresholdCalcSizeMax ) { nNextPos = m_AdaptiveThresholdCalcSizeMax; }
	else if ( nNextPos < m_AdaptiveThresholdCalcSizeMin ) { nNextPos = m_AdaptiveThresholdCalcSizeMin; }
	pNMUpDown->iDelta = nNextPos-nPos;	

	CString    str;
	str.Format(_T("%d"), nNextPos);
	//m_AdaThresholdGapSlider.SetPos(nNextPos);
	CWnd::SetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_EDIT, str);
	m_BinaryParamPtr->SetAdaptiveThresholdCalcSize(nNextPos);
	strValueOld.Format(_T("%d"), nPos);
	strValueNew.Format(_T("%d"), nNextPos);	
	CWnd::GetDlgItemText(BINARY_ADAPTIVE_THRESHOLD_CALC_SIZE_LABEL, strValueName);
	ExecSaveLogModelWndOperate_Binary(BINARY_PARAMETER_GROUP, strValueName, strValueOld, strValueNew);
	UpdateBinaryUIToParam();
	UpdateBinaryParamToParentWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CEditImageBinaryWnd::CheckFixThresholdValue(int Th)//確認固定閥值
{
	CString str, str2;
	if ( Th < m_FixedThresholdMin )
	{
		str = _T("Error, Threshold out of range");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s (%d / %d)"), str, Th, m_FixedThresholdMin);
		JetAPI::ShowMessageBox(str2);
		return false;
	}
	if ( Th > m_FixedThresholdMax )
	{
		str = _T("Error, Threshold out of range");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("%s (%d / %d)"), str, Th, m_FixedThresholdMax);
		JetAPI::ShowMessageBox(str2);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnRatioThresholdHighBtn()
{
	// TODO: Add your control notification handler code here
	CString str;
	CInputBoxWnd Wnd;	
	CString sCaption;
	CString sKey1, sKey2;
	CString sValue1, sValue2;	
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	sCaption = _T("Set High Ratio Threshold");
	sCaption = LoadMultiLanguageString(sCaption, sCaption);
	sKey1 = _T("Target");
	sKey1 = LoadMultiLanguageString(sKey1, sKey1);
	sKey2 = AOIDataDefine.GetRatioText();	
	sValue1.Format(_T("%.2f"), m_BinaryParamPtr->GetRatioThresholdTarget());
	sValue2.Format(_T("%.2f"), m_BinaryParamPtr->GetRatioThresholdRatioHigh());
	Wnd.SetParam2(sCaption, sKey1, sValue1, sKey2, sValue2);
	if ( Wnd.DoModal() != IDOK )	{	return; }	
	const double Ratio=::_ttof(Wnd.m_DataEdit2);
	const double Target=::_ttof(Wnd.m_DataEdit1);
	const int Threshold=(int)(((Ratio*Target)/100.0)+0.5);
	if ( CheckFixThresholdValue(Threshold) == false )
	{	return; }
	CAlgBinaryParam BinaryOld = *m_BinaryParamPtr;
	m_BinaryParamPtr->SetRatioThresholdTarget(Target);
	m_BinaryParamPtr->SetRatioThresholdRatioHigh(Ratio);
	m_BinaryParamPtr->SetFixedThresholdHigh(Threshold);
	if ( m_BinaryParamPtr->GetFixedThresholdLow() > Threshold )
	{	
		m_BinaryParamPtr->SetFixedThresholdLow(Threshold); 
		m_BinaryParamPtr->SetRatioThresholdRatioLow(Ratio);
	}
	LogOperCtrl.SaveLogModelWndAlgBinaryCompare(m_WndPtr, &BinaryOld, m_BinaryParamPtr);	
	UpdateBinaryParamToUI();
	UpdateBinaryParamToParentWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageBinaryWnd::OnRatioThresholdLowBtn()
{
	// TODO: Add your control notification handler code here	
	CInputBoxWnd Wnd;
	CString sCaption;
	CString sKey1, sKey2;
	CString sValue1, sValue2;	
	if ( CheckBinaryParamPtr() == false ) { return ; }	
	sCaption = _T("Set Low Ratio Threshold");
	sCaption = LoadMultiLanguageString(sCaption, sCaption);
	sKey1 = _T("Target");
	sKey1 = LoadMultiLanguageString(sKey1, sKey1);
	sKey2 = AOIDataDefine.GetRatioText();	
	sValue1.Format(_T("%.2f"), m_BinaryParamPtr->GetRatioThresholdTarget());
	sValue2.Format(_T("%.2f"), m_BinaryParamPtr->GetRatioThresholdRatioLow());
	Wnd.SetParam2(sCaption, sKey1, sValue1, sKey2, sValue2);
	if ( Wnd.DoModal() != IDOK )	{	return; }	
	const double Ratio=::_ttof(Wnd.m_DataEdit2);
	const double Target=::_ttof(Wnd.m_DataEdit1);
	const int Threshold=(int)(((Ratio*Target)/100.0)+0.5);
	if ( CheckFixThresholdValue(Threshold) == false )
	{	return; }
	CAlgBinaryParam BinaryOld = *m_BinaryParamPtr;
	m_BinaryParamPtr->SetRatioThresholdTarget(Target);
	m_BinaryParamPtr->SetRatioThresholdRatioLow(Ratio);
	m_BinaryParamPtr->SetFixedThresholdLow(Threshold);
	if ( m_BinaryParamPtr->GetFixedThresholdHigh() < Threshold )
	{	
		m_BinaryParamPtr->SetFixedThresholdHigh(Threshold); 
		m_BinaryParamPtr->SetRatioThresholdRatioHigh(Ratio);
	}	
	LogOperCtrl.SaveLogModelWndAlgBinaryCompare(m_WndPtr, &BinaryOld, m_BinaryParamPtr);
	UpdateBinaryParamToUI();
	UpdateBinaryParamToParentWnd();
	return;
}
//-------------------------------------------------------------------------------------//