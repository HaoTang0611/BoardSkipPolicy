// ImagePhaseAllWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ImagePhaseAllWnd.h"
//-------------------------------------------------------------------------------------//
#include "JetZip.h"
#include "Light3DCtrl.h"
#include "InputBoxWnd.h"
#include "InputListWnd.h"
#include "SpaceBaseParamWnd.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseAllWnd dialog
//-------------------------------------------------------------------------------------//
CImagePhaseAllWnd::CImagePhaseAllWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImagePhaseAllWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImagePhaseAllWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	size_t i = 0;
	const size_t Count = MAX_PAHSE_COUNT;
	for ( i=0; i<Count; i++ )
	{	
		this->m_EllapseTime[i] = 0.0;
		this->m_bClonePhase[i] = TRUE;
		this->m_bCloneSpace[i] = TRUE;
		this->m_ImageW[i] = 0;
		this->m_ImageH[i] = 0;
		this->m_ShowStep[i] = 0;
		this->m_ImageStep[i] = 0;
		this->m_PhaseStep[i] = 0;
		this->m_SpaceStep[i] = 0;		
		this->m_BitCount[i] = 0;	
		this->m_MaskBuffer[i] = NULL;
		this->m_ShowBuffer[i] = NULL;
		this->m_ImageBuffer[i] = NULL;
		this->m_PhaseBuffer[i] = NULL;	
		this->m_SpaceBuffer[i] = NULL;		
		this->m_PhaseMax[i] = 0.0;
		this->m_PhaseMin[i] = 0.0;
		this->m_PhaseAve[i] = 0.0;			
	}
	m_BkColor = 0xE0E0E0;
	
	this->m_ImagePt.x = m_ImagePt.y = 0;
	this->m_ImageW_M = 0;
	this->m_ImageH_M = 0;
	this->m_BitCount_M = 0;
	this->m_ImageStep_M = 0;
	this->m_PhaseStep_M = 0;
	this->m_SpaceStep_M = 0;
	this->m_MaskBuffer_M = NULL;
	this->m_ImageBuffer_M = NULL;
	this->m_PhaseBuffer_M = NULL;
	this->m_SpaceBuffer_M = NULL;

	this->m_ImageZoom = 1.1;
	this->m_ShowModeID = IDC_SHOW_IMAGE_RADIO;
	this->m_SliceFuncMode=SLICE_FUNC_3D_4STEP_4STEP_1EXP;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImagePhaseAllWnd)
	DDX_Control(pDX, ALLPHAS_DLP_LED_COMBO, m_DLPLEDCombox);
	DDX_Control(pDX, ALLPHAS_OPEN_MP_COMBO, m_OpenMPCombox);
	DDX_Control(pDX, ALLPHAS_NOISE_DEFINE_MODE_COMBO, m_NoiseDefineModeCombox);
	DDX_Control(pDX, ALLPHAS_MULTI_CAST_MERGE_MODE_COMBO, m_SpaceMergeModeCombox);
	DDX_Control(pDX, ALLPHAS_MULTI_CAST_MERGE_BEST_MODE_COMBO, m_SpaceMergeBestModeCombox);
	DDX_Control(pDX, ALLPHAS_MULTI_INTENSITY_MERGE_MODE_COMBO, m_SpaceMergeIntensityModeCombox);	
	DDX_Control(pDX, ALLPHAS_MERGE_RECURSION_MODE_COMBO, m_MergeRecursionModeCombox);
	DDX_Control(pDX, ALLPHAS_NF_FIRST_FILTER_MODE_COMBO, m_FirstFilterModeCombox);		
	DDX_Control(pDX, ALLPHAS_NF_OVER_LOW_MODE_COMBO, m_OverLowModeCombox);		
	DDX_Control(pDX, ALLPHAS_NF_HEIGHT_UNEXPECTED_MODE_COMBO, m_HeightUnexpectedModeCombox);		
	DDX_Control(pDX, ALLPHAS_NF_FINAL_FILTER_MODE_COMBO, m_FinalFilterModeCombox);
	DDX_Control(pDX, ALLPHAS_NF_FINAL_FILTER_MODE_COMBO2, m_FinalFilterModeCombox2);
	DDX_Control(pDX, ALLPHAS_IMAGE_WND4, m_ImageWnd4);
	DDX_Control(pDX, ALLPHAS_IMAGE_WND3, m_ImageWnd3);
	DDX_Control(pDX, ALLPHAS_IMAGE_WND2, m_ImageWnd2);
	DDX_Control(pDX, ALLPHAS_IMAGE_WND1, m_ImageWnd1);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImagePhaseAllWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImagePhaseAllWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(ALLPHAS_LOAD_PHASE_BTN1, OnLoadPhaseBtn1)
	ON_BN_CLICKED(ALLPHAS_LOAD_PHASE_BTN2, OnLoadPhaseBtn2)
	ON_BN_CLICKED(ALLPHAS_LOAD_PHASE_BTN3, OnLoadPhaseBtn3)
	ON_BN_CLICKED(ALLPHAS_LOAD_PHASE_BTN4, OnLoadPhaseBtn4)
	ON_BN_CLICKED(ALLPHAS_MERGE_PHASE_BTN, OnMergePhaseBtn)
	ON_BN_CLICKED(ALLPHAS_CALCULATE_ALL_BTN, OnCalculateAllBtn)
	ON_BN_CLICKED(IDC_SHOW_IMAGE_RADIO, OnShowImageRadio)
	ON_BN_CLICKED(IDC_SHOW_PHASE_RADIO, OnShowPhaseRadio)
	ON_BN_CLICKED(IDC_SHOW_SPACE_RADIO, OnShowSpaceRadio)
	ON_BN_CLICKED(IDC_ENHANCE_IMAGE_CHK, OnEnhanceImageChk)
	ON_BN_CLICKED(ALLPHAS_COMPARE_SPACE_BTN, OnCompareSpaceBtn)
	ON_BN_CLICKED(ALLPHAS_NF_DISABLE_BTN, OnNFDisableBtn)
	ON_BN_CLICKED(ALLPHAS_CONVERT_PHASE_FACTOR_BTN, OnConvertPhaseFactorBtn)
	ON_BN_CLICKED(ALLPHAS_PHASE_COMPARE_BTN, OnPhaseCompareBtn)
	ON_BN_CLICKED(ALLPHAS_MEDIAN_DEBUG_BTN, OnMedianDebugBtn)
	ON_BN_CLICKED(ALLPHAS_USE_CUDA_CHK, OnUseCudaChk)	
	ON_BN_CLICKED(ALLPHAS_LOCK_RECT_BTN, OnLockRectBtn)
	ON_BN_CLICKED(ALLPHAS_BASE_PLANE_PARAM_BTN, OnBasePlaneParamBtn)
	ON_BN_CLICKED(ALLPHAS_SPACE_NOISE_FILTER_BTN, OnSpaceNoiseFilterBtn)
	ON_BN_CLICKED(ALLPHAS_LOAD_PARAM_BTN, OnLoadParamBtn)
	ON_BN_CLICKED(ALLPHAS_SAVE_IMAGE_BTN, OnSaveImageBtn)	
	ON_BN_CLICKED(ALLPHAS_SET_PATTERN_TYPE_BTN, OnSetPatternTypeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseAllWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseAllWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	this->m_PhaseWnd.Create(IDD_IMAGE_PHASE_WND, this);
	this->m_Draw3DWnd.Create(IDD_DRAW3D_WND, this);
	this->SwitchMultiLanguage();
	
	int     i=0;
	CString str;
	const int CompuenterCPUCnt = AOIDataCollect.GetComputerCPUCoreNumber();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int NoiseDefine = SysParam.m_PhaseNoiseDefine;
	
	JetAPI::ClearCombox(m_OpenMPCombox);
	for ( i=0; i<CompuenterCPUCnt; i++ )
	{
		str.Format(_T("%d"), i);		
		m_OpenMPCombox.InsertString(-1, str);
		m_OpenMPCombox.SetItemData(i, i);		
	}

	CWnd::CheckDlgButton(ALLPHAS_USE_CUDA_CHK, SysParam.m_CudaFnEnabled);
#ifndef CUDA_USE
	JetAPI::EnableCtrlWnd(this, ALLPHAS_USE_CUDA_CHK, FALSE);
#endif//CUDA_USE

#ifdef OPEN_MP_USE	
	JetAPI::SetComboxCurSel(m_OpenMPCombox, SysParam.m_OpenMPCount_General);
#else
	JetAPI::SetComboxCurSel(m_OpenMPCombox, 0);
	m_OpenMPCombox.EnableWindow(FALSE);
#endif//OPEN_MP_USE

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuidlSpaceMergeModeCombox(m_SpaceMergeModeCombox);
	AOIDataDefine.BuidlSpaceMergeBestModeCombox(m_SpaceMergeBestModeCombox);
	AOIDataDefine.BuidlSpaceMergeIntensityModeCombox(m_SpaceMergeIntensityModeCombox);	
	AOIDataDefine.BuidlSpaceNoiseDefineModeCombox(m_NoiseDefineModeCombox);	
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_MergeRecursionModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FirstFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_OverLowModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_HeightUnexpectedModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox2);	

	str.Format(_T("%.2f"), SysParam.m_PhasePeriod1);
	this->SetDlgItemText(ALLPHAS_PERIODE_EDIT1, str);
	str.Format(_T("%.2f"), SysParam.m_PhasePeriod2);
	this->SetDlgItemText(ALLPHAS_PERIODE_EDIT2, str);
	AOIDataDefine.BuildDLPPhaseLEDColorCombox(m_DLPLEDCombox, true);	
	JetAPI::SetComboxCurSel(m_DLPLEDCombox, AOIDataCollect.GetSystemDlpLedColor());	
	
	JetAPI::SetComboxCurSel(m_NoiseDefineModeCombox, SysParam.m_PhaseNoiseDefineMode);	
	if ( (NoiseDefine&PHASE_NOSIE_SMOOTH_FILTER) == 0 )
	{	CWnd::CheckDlgButton(ALLPHAS_SMOOTH_FILTER_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(ALLPHAS_SMOOTH_FILTER_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_LOW_CONTRAST) == 0 )
	{	CWnd::CheckDlgButton(ALLPHAS_LOW_CONTRAST_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(ALLPHAS_LOW_CONTRAST_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_LOW_POTENTIAL) == 0 )
	{	CWnd::CheckDlgButton(ALLPHAS_LOW_POTENTIAL_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(ALLPHAS_LOW_POTENTIAL_CHK, TRUE);	}
	if ( (NoiseDefine&PHASE_NOSIE_OVER_SATURATED) == 0 )
	{	CWnd::CheckDlgButton(ALLPHAS_OVER_SATURATED_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(ALLPHAS_OVER_SATURATED_CHK, TRUE);	}	
	if ( (NoiseDefine&PHASE_NOSIE_VOID_EXPAND) == 0 )
	{	CWnd::CheckDlgButton(ALLPHAS_VOID_EXPAND_CHK, FALSE);	}
	else
	{	CWnd::CheckDlgButton(ALLPHAS_VOID_EXPAND_CHK, TRUE);	}
	CWnd::SetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT, SysParam.m_PhaseNoiseLowContrastA);
	CWnd::SetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT, SysParam.m_PhaseNoiseLowPotentialA);
	CWnd::SetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT, SysParam.m_PhaseNoiseOverSaturatedA);
	CWnd::SetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT2, SysParam.m_PhaseNoiseLowContrastB);
	CWnd::SetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT2, SysParam.m_PhaseNoiseLowPotentialB);
	CWnd::SetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT2, SysParam.m_PhaseNoiseOverSaturatedB);
	CWnd::SetDlgItemInt(ALLPHAS_VOID_EXPAND_EDIT, SysParam.m_PhaseNoiseExtendVoid);
	CWnd::SetDlgItemInt(ALLPHAS_SMOOTH_FILTER_EDIT, SysParam.m_PhaseNoiseSmoothFilter);	

	CWnd::SetDlgItemInt(ALLPHAS_SINGLE_CAST_OVER_LOW_EDIT, (int)(SysParam.m_SpaceNoiseSingleCastLowLimit));	
	JetAPI::SetComboxCurSel(m_SpaceMergeModeCombox, SysParam.m_SpaceNoiseMultiCastMergeMode);
	JetAPI::SetComboxCurSel(m_SpaceMergeBestModeCombox, SysParam.m_SpaceNoiseMultiCastMergeBestMode);
	JetAPI::SetComboxCurSel(m_SpaceMergeIntensityModeCombox, SysParam.m_SpaceNoiseMultiIntensityMergeMode);	
	CWnd::SetDlgItemInt(ALLPHAS_MULTI_CAST_PATCH_SIZE_EDIT, SysParam.m_SpaceNoiseMultiCastPatchSize);	
	CWnd::SetDlgItemInt(ALLPHAS_MULTI_CAST_MIN_VALID_COUNT_EDIT, SysParam.m_SpaceNoiseMultiCastMinValidCount);	
	CWnd::SetDlgItemInt(ALLPHAS_MULTI_CAST_MAX_DIFF_EDIT, SysParam.m_SpaceNoiseMultiCastMaxDifference);
	CWnd::SetDlgItemInt(ALLPHAS_MULTI_CAST_LIMIT_DIFF_EDIT, SysParam.m_SpaceNoiseMultiCastLimitDifference);
	//m_SpaceNoiseMultiCastValidBestRatio;//高度雜訊多投光高度最好比例-um	
	CWnd::SetDlgItemInt(ALLPHAS_MULTI_CAST_VALID_DIFF_EDIT, SysParam.m_SpaceNoiseMultiCastValidDifference);	
	//m_SpaceNoiseMultiCastOppositeMaxGray;//高度雜訊多投光合併對邊灰階上限-gray

	JetAPI::SetComboxCurSel(m_MergeRecursionModeCombox, SysParam.m_SpaceMergeRecursionMode);
	CWnd::SetDlgItemInt(ALLPHAS_MERGE_RECURSION_SIZE_EDIT, SysParam.m_SpaceMergeRecursionKernelSize);
	CWnd::SetDlgItemInt(ALLPHAS_MERGE_RECURSION_COUNT_EDIT, SysParam.m_SpaceMergeRecursionMaxCount);
	CWnd::SetDlgItemInt(ALLPHAS_MERGE_RECURSION_IGNORE_SIZE_EDIT, SysParam.m_SpaceMergeRecursionMaskSize);
	
	AOIDataCollect.GetPhaseNoiseDefineParam(m_PhaseNoiseParam);	
	AOIDataCollect.GetSpaceNoiseFilterParam(m_NoiseFilterParam);	
	UpdateSpaceNoiseFilterParamToUI(m_NoiseFilterParam);
	
	CWnd::CheckDlgButton(m_ShowModeID, TRUE);
	CWnd::CheckDlgButton(IDC_ENHANCE_IMAGE_CHK, TRUE);	
	
	if ( FN_ENABLE == SysParam.m_MedianFilterShiftEnabled || FN_ENABLE == SysParam.m_LevelFilterShiftEnabled || FN_ENABLE == SysParam.m_PyramidMedianFilterShiftEnabled)
	{	CWnd::CheckDlgButton(ALLPHAS_NF_HEIGHT_UNEXPECTED_FILTER_SHIFT_CHK, TRUE);	}
	
	TSliceParam *SliceParamPtr=AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SLICE_UNIQUE_ID_DLP);//依據Slice ID來取得Slice Param指標
	if ( NULL != SliceParamPtr )
	{	m_SliceFuncMode = SliceParamPtr->SliceFuncMode; }
	m_SliceFuncMode = AOIDataCollect.CheckSliceFuncModeBasicMode(m_SliceFuncMode);

	CWnd::SetDlgItemInt(ALLPHAS_LOCK_RECT_X_EDIT1, 0);
	CWnd::SetDlgItemInt(ALLPHAS_LOCK_RECT_Y_EDIT1, 0);
	CWnd::SetDlgItemInt(ALLPHAS_LOCK_RECT_X_EDIT2, 0);
	CWnd::SetDlgItemInt(ALLPHAS_LOCK_RECT_Y_EDIT2, 0);
	LoadRectParam();

	//this->AdjustCtrlWnd();//SW_SHOWMAXIMIZED
	//const size_t ImageW = this->m_ImageW;
	//const size_t ImageH = this->m_ImageH;
	//ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, this->m_ImageWndRect, 1.1, this->m_ImageZoom);
	//CImagePhaseAllWnd::DrawImageWndMemDC();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	size_t i = 0;
	const size_t Count = MAX_PAHSE_COUNT;
	for ( i=0; i<Count; i++ )
	{	this->ReleaseBuffer(i);	}
	this->ReleaseBuffer_M();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustCtrlWnd(cx, cy);	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		size_t i=0;
		for ( i=0; i<MAX_PAHSE_COUNT; i++ )
		{	this->DrawImageWndMemDC(i); }

	}
}
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseAllWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_IMAGE_PHASE_ALL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_IMAGE_PHASE_ALL_WND;
	WndKey = _T("IDD_IMAGE_PHASE_ALL_WND");
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
	WndID = ALLPHAS_LOAD_PHASE_BTN1;
	WndKey = _T("ALLPHAS_LOAD_PHASE_BTN1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_ENABLE_PHASE_CHK1;
	WndKey = _T("ALLPHAS_ENABLE_PHASE_CHK1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_LOAD_PHASE_BTN2;
	WndKey = _T("ALLPHAS_LOAD_PHASE_BTN2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_ENABLE_PHASE_CHK2;
	WndKey = _T("ALLPHAS_ENABLE_PHASE_CHK2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_LOAD_PHASE_BTN3;
	WndKey = _T("ALLPHAS_LOAD_PHASE_BTN3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_ENABLE_PHASE_CHK3;
	WndKey = _T("ALLPHAS_ENABLE_PHASE_CHK3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_LOAD_PHASE_BTN4;
	WndKey = _T("ALLPHAS_LOAD_PHASE_BTN4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ALLPHAS_ENABLE_PHASE_CHK4;
	WndKey = _T("ALLPHAS_ENABLE_PHASE_CHK4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ALLPHAS_MERGE_PHASE_BTN;
	WndKey = _T("ALLPHAS_MERGE_PHASE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ALLPHAS_SMOOTH_FILTER_CHK;
	WndKey = _T("ALLPHAS_SMOOTH_FILTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_LOW_CONTRAST_CHK;
	WndKey = _T("ALLPHAS_LOW_CONTRAST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_LOW_POTENTIAL_CHK;
	WndKey = _T("ALLPHAS_LOW_POTENTIAL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_OVER_SATURATED_CHK;
	WndKey = _T("ALLPHAS_OVER_SATURATED_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_VOID_EXPAND_CHK;
	WndKey = _T("ALLPHAS_VOID_EXPAND_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_SINGLE_CAST_OVER_LOW_LABEL;
	WndKey = _T("ALLPHAS_SINGLE_CAST_OVER_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_MERGE_RECURSION_LABEL;
	WndKey = _T("ALLPHAS_MERGE_RECURSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = ALLPHAS_MULTI_CAST_MAX_DIFF_LABEL;
	WndKey = _T("ALLPHAS_MULTI_CAST_MAX_DIFF_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_MULTI_CAST_BEST_MODE_LABEL;
	WndKey = _T("ALLPHAS_MULTI_CAST_BEST_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_MULTI_INTENSITY_MERGE_MODE_LABEL;
	WndKey = _T("ALLPHAS_MULTI_INTENSITY_MERGE_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_PERIODE_LABEL1;
	WndKey = _T("ALLPHAS_PERIODE_LABEL1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_PERIODE_LABEL2;
	WndKey = _T("ALLPHAS_PERIODE_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_ZERO_PLANE_CHK;
	WndKey = _T("ALLPHAS_ZERO_PLANE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_PHASE_MODE_CHK;
	WndKey = _T("ALLPHAS_PHASE_MODE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_CALCULATE_ALL_BTN;
	WndKey = _T("ALLPHAS_CALCULATE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = IDC_SHOW_IMAGE_RADIO;
	WndKey = _T("IDC_SHOW_IMAGE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IDC_SHOW_PHASE_RADIO;
	WndKey = _T("IDC_SHOW_PHASE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IDC_SHOW_SPACE_RADIO;
	WndKey = _T("IDC_SHOW_SPACE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IDC_ENHANCE_IMAGE_CHK;
	WndKey = _T("IDC_ENHANCE_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDC_SPACE_MASK_IMAGE_CHK;
	WndKey = _T("IDC_SPACE_MASK_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = ALLPHAS_COMPARE_SPACE_BTN;
	WndKey = _T("ALLPHAS_COMPARE_SPACE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	//---------------------------------------------------------------------------------//		
	WndID = ALLPHAS_NOISE_FILTER_GROUP;
	WndKey = _T("ALLPHAS_NOISE_FILTER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_VOID_EXPAND_CHK;
	WndKey = _T("ALLPHAS_NF_VOID_EXPAND_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_FIRST_FILTER_LABEL;
	WndKey = _T("ALLPHAS_NF_FIRST_FILTER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_OVER_LOW_LABEL;
	WndKey = _T("ALLPHAS_NF_OVER_LOW_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_HEIGHT_UNEXPECTED_LABEL;
	WndKey = _T("ALLPHAS_NF_HEIGHT_UNEXPECTED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_VOID_RECONTRUCT_CHK;
	WndKey = _T("ALLPHAS_NF_VOID_RECONTRUCT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_FINAL_FILTER_LABEL;
	WndKey = _T("ALLPHAS_NF_FINAL_FILTER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_FINAL_FILTER_LABEL2;
	WndKey = _T("ALLPHAS_NF_FINAL_FILTER_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = ALLPHAS_BASE_PLANE_PARAM_BTN;
	WndKey = _T("ALLPHAS_BASE_PLANE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_SPACE_NOISE_FILTER_BTN;
	WndKey = _T("ALLPHAS_SPACE_NOISE_FILTER_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALLPHAS_NF_DISABLE_BTN;
	WndKey = _T("ALLPHAS_NF_DISABLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//		
	WndID = ALLPHAS_LOAD_PARAM_BTN;
	WndKey = _T("ALLPHAS_LOAD_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALLPHAS_SAVE_IMAGE_BTN;
	WndKey = _T("ALLPHAS_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = ALLPHAS_SET_PATTERN_TYPE_BTN;
	WndKey = _T("ALLPHAS_SET_PATTERN_TYPE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//			
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::AdjustCtrlWnd(int cx, int cy)
{
	RECT Rect={0};
	if ( cx<0 || cy<0 )
	{
		this->GetClientRect(&Rect);
		cx = Rect.right-Rect.left;
		cy = Rect.bottom-Rect.top;
	}

	POINT Pt={0, 0};
	CWnd *pWnd = NULL;
	const int MarginW=4;
	const int MarginH=4;
	pWnd = this->GetDlgItem(ALLPHAS_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		RECT WndRect={0};
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.right = cx-MarginW;
		pWnd->MoveWindow(&WndRect);

		Pt.x = WndRect.left;
		Pt.y = WndRect.bottom + 4;
	}	
	pWnd = this->GetDlgItem(ALLPHAS_INFO_EDIT2);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		RECT WndRect={0};
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.right = cx-MarginW;
		pWnd->MoveWindow(&WndRect);

		Pt.x = WndRect.left;
		Pt.y = WndRect.bottom + 4;
	}
	pWnd = this->GetDlgItem(ALLPHAS_INFO_EDIT3);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		RECT WndRect={0};
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.right = cx-MarginW;
		pWnd->MoveWindow(&WndRect);

		Pt.x = WndRect.left;
		Pt.y = WndRect.bottom + 4;
	}
	SIZE  Size={0, 0};	
	size_t IdxLT = 1;
	size_t IdxRT = 2;
	size_t IdxLB = 3;
	size_t IdxRB = 0;	
	CWnd *pWndLT = &m_ImageWnd2;
	CWnd *pWndRT = &m_ImageWnd3;
	CWnd *pWndLB = &m_ImageWnd4;
	CWnd *pWndRB = &m_ImageWnd1;
	RECT *pRectLT = &m_ImageWndRect2;
	RECT *pRectRT = &m_ImageWndRect3;
	RECT *pRectLB = &m_ImageWndRect4;
	RECT *pRectRB = &m_ImageWndRect1;
	CJetMemDC *pMemDcLT = &m_ImageWndMemDC2;
	CJetMemDC *pMemDcRT = &m_ImageWndMemDC3;
	CJetMemDC *pMemDcLB = &m_ImageWndMemDC4;
	CJetMemDC *pMemDcRB = &m_ImageWndMemDC1;

	//Left Top
	if ( pWndLT->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		pWndLT->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		//Pt.x = WndRect.left;
		//Pt.y = WndRect.top;
		WndRect.left = Pt.x;
		WndRect.top = Pt.y;
		Size.cx = (cx-Pt.x-MarginW-MarginW)/2;
		Size.cy = (cy-Pt.y-MarginH-MarginH)/2;
		WndRect.right = WndRect.left + Size.cx;
		WndRect.bottom = WndRect.top + Size.cy;
		pWndLT->MoveWindow(&WndRect);
		pWndLT->GetClientRect(pRectLT);
		pMemDcLT->CreateMemDC(pWndLT, m_BkColor);		
		this->DrawImageWndMemDC(IdxLT);
	}
	
	//Right Top
	if ( pWndRT->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		pWndRT->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);		
		WndRect.top  = Pt.y;
		WndRect.left = Pt.x + Size.cx+MarginW;
		WndRect.right = WndRect.left + Size.cx;
		WndRect.bottom = WndRect.top + Size.cy;
		pWndRT->MoveWindow(&WndRect);
		pWndRT->GetClientRect(pRectRT);
		pMemDcRT->CreateMemDC(pWndRT, m_BkColor);		
		this->DrawImageWndMemDC(IdxRT);
	}

	//Left Bottom
	if ( pWndLB->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		pWndLB->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);				
		WndRect.left = Pt.x;
		WndRect.top  = Pt.y + Size.cy + MarginH;
		WndRect.right = WndRect.left + Size.cx;
		WndRect.bottom = WndRect.top + Size.cy;
		pWndLB->MoveWindow(&WndRect);
		pWndLB->GetClientRect(pRectLB);
		pMemDcLB->CreateMemDC(pWndLB, m_BkColor);	
		this->DrawImageWndMemDC(IdxLB);
	}

	//Right Bottom
	if ( pWndRB->GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		pWndRB->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);				
		WndRect.left = Pt.x + Size.cx+MarginW;
		WndRect.top  = Pt.y + Size.cy + MarginH;
		WndRect.right = WndRect.left + Size.cx;
		WndRect.bottom = WndRect.top + Size.cy;
		pWndRB->MoveWindow(&WndRect);
		pWndRB->GetClientRect(pRectRB);
		pMemDcRB->CreateMemDC(pWndRB, m_BkColor);	
		this->DrawImageWndMemDC(IdxRB);
	}
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ReleaseBuffer_M()
{	
	if ( NULL != this->m_ImageBuffer_M ) 
	{	JetMemory.free_func(m_ImageBuffer_M); }
	if ( NULL != this->m_MaskBuffer_M )
	{	JetMemory.free_func(m_MaskBuffer_M); }
	if ( NULL != this->m_PhaseBuffer_M )
	{	JetMemory.free_func(m_PhaseBuffer_M); }
	if ( NULL != this->m_SpaceBuffer_M )
	{	JetMemory.free_func(m_SpaceBuffer_M); }
	
	this->m_ImageW_M = 0;
	this->m_ImageH_M = 0;
	this->m_ImageStep_M = 0;
	this->m_PhaseStep_M = 0;
	this->m_SpaceStep_M = 0;
	//this->m_BitCount_M = 0;	
	this->m_MaskBuffer_M = NULL;
	this->m_ImageBuffer_M = NULL;
	this->m_PhaseBuffer_M = NULL;
	this->m_SpaceBuffer_M = NULL;	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ReleaseBuffer(size_t idx)
{	
	const size_t Count = MAX_PAHSE_COUNT;
	if ( idx >= Count ) { return; }
	if ( NULL != this->m_ShowBuffer[idx] ) 
	{	JetMemory.free_func(m_ShowBuffer[idx]); }
	if ( NULL != this->m_ImageBuffer[idx] ) 
	{	JetMemory.free_func(m_ImageBuffer[idx]); }
	if ( NULL != this->m_MaskBuffer[idx] )
	{	JetMemory.free_func(m_MaskBuffer[idx]); }
	if ( TRUE==m_bClonePhase[idx] && NULL!=m_PhaseBuffer[idx] )
	{	JetMemory.free_func(m_PhaseBuffer[idx]);	}
	if ( TRUE==m_bCloneSpace[idx] && NULL!=m_SpaceBuffer[idx] )
	{	JetMemory.free_func(m_SpaceBuffer[idx]);	}

	this->m_ImageW[idx] = 0;
	this->m_ImageH[idx] = 0;
	this->m_ShowStep[idx] = 0;
	this->m_ImageStep[idx] = 0;
	this->m_PhaseStep[idx] = 0;
	this->m_SpaceStep[idx] = 0;
	this->m_BitCount[idx] = 0;	
	this->m_PhaseMax[idx] = 0.0;
	this->m_PhaseMin[idx] = 0.0;
	this->m_PhaseAve[idx] = 0.0;	
	this->m_MaskBuffer[idx] = NULL;
	this->m_PhaseBuffer[idx] = NULL;	
	this->m_SpaceBuffer[idx] = NULL;
	JetAPI::ClearCastParam(m_CastParam[idx]);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::DrawImageWndMemDC(size_t idx)
{	
	if ( idx >= MAX_PAHSE_COUNT ) { return; }
	HDC hDC = NULL;	
	RECT Rect={0};
	switch ( idx )
	{
	case 0:
		Rect = m_ImageWndRect1;
		hDC = m_ImageWndMemDC1.GetSafeHdc();	
		break;
	case 1:
		Rect = m_ImageWndRect2;
		hDC = m_ImageWndMemDC2.GetSafeHdc();	
		break;
	case 2:
		Rect = m_ImageWndRect3;
		hDC = m_ImageWndMemDC3.GetSafeHdc();	
		break;
	case 3:
		Rect = m_ImageWndRect4;
		hDC = m_ImageWndMemDC4.GetSafeHdc();	
		break;
	}	
	if ( NULL == hDC ) { return; }

	COLORREF clrBK = m_BkColor;
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}

	if ( NULL!=m_ShowBuffer[idx] )
	{	ImageAPI.DrawImageToDC(hDC, m_ImageW[idx], m_ImageH[idx], m_ShowStep[idx], m_BitCount[idx], m_ShowBuffer[idx], Rect, m_ImageOffset, m_ImageZoom, clrBK);	}
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::RedrawWnd()
{
	CString  str;
	int      HalfW=2;
	int      HalfH=2;
	size_t   idx = 0;
	TPOINT2D dWndPt1, dWndPt2;
	POINT    nWndPt1={0}, nWndPt2={0};	
	HPEN     hPen = NULL;
	HPEN     hOldPen = NULL;
	COLORREF clrRectLine = 0x0000FF;
	COLORREF clrCenterLine = 0x2FAFAF;
	COLORREF clrBK = 0x000000;
	COLORREF clrText = 0x00FF00;
	const BOOL bDrawRectLine = TRUE;
	if ( m_ImageWnd1.GetSafeHwnd() != NULL )
	{	
		idx = 0;
		CClientDC dc(&m_ImageWnd1);	
		RECT Rect = m_ImageWndRect1;
		HDC hDC = dc.GetSafeHdc();	
		HDC hBKDC = m_ImageWndMemDC1.GetSafeHdc();		
		::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);	

		//Copy m_MemHDC Image to MemHDC1 use in Back ground
		::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hBKDC, 0, 0, SRCCOPY );

		hPen = ::CreatePen(PS_DOT, 1, clrCenterLine);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt, dWndPt1);				
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		::MoveToEx(hDC, Rect.left, nWndPt1.y, NULL);
		::LineTo(hDC, Rect.right, nWndPt1.y);
		::MoveToEx(hDC, nWndPt1.x, Rect.top, NULL);
		::LineTo(hDC, nWndPt1.x, Rect.bottom);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;

		::SetBkColor(hDC, clrBK);
		::SetTextColor(hDC, clrText);
		str.Format(_T("Cast-1  Ave:%.0f, Max:%.0f, Min:%.0f"), m_PhaseAve[idx], m_PhaseMax[idx], m_PhaseMin[idx]);
		::TextOut(hDC, 0, 0, str, str.GetLength());				

		if ( TRUE == bDrawRectLine )
		{	
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt1, dWndPt1);
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt2, dWndPt2);
			nWndPt1.x = JetAPI::Floor(MIN(dWndPt1.x, dWndPt2.x));
			nWndPt1.y = JetAPI::Floor(MIN(dWndPt1.y, dWndPt2.y));
			nWndPt2.x = JetAPI::Floor(MAX(dWndPt1.x, dWndPt2.x));
			nWndPt2.y = JetAPI::Floor(MAX(dWndPt1.y, dWndPt2.y));

			hPen = ::CreatePen(PS_SOLID, 1, clrRectLine);
			hOldPen = (HPEN)::SelectObject(hDC, hPen);			
			::MoveToEx(hDC, nWndPt1.x, nWndPt1.y, NULL);
			::LineTo(hDC, nWndPt2.x, nWndPt1.y);
			::LineTo(hDC, nWndPt2.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt1.y);			
			::DeleteObject(hPen);	hPen=NULL;
		}

		//Mark Pos		
		hPen = ::CreatePen(PS_SOLID, 1, 0x00FF00);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark1, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		

		hPen = ::CreatePen(PS_SOLID, 1, 0xFF0000);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark2, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		
	}

	if ( m_ImageWnd2.GetSafeHwnd() != NULL )
	{			
		idx = 1;
		CClientDC dc(&m_ImageWnd2);	
		RECT Rect = m_ImageWndRect2;
		HDC hDC = dc.GetSafeHdc();	
		HDC hBKDC = m_ImageWndMemDC2.GetSafeHdc();		
		::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);	

		//Copy m_MemHDC Image to MemHDC1 use in Back ground
		::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hBKDC, 0, 0, SRCCOPY );

		hPen = ::CreatePen(PS_DOT, 1, clrCenterLine);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		::MoveToEx(hDC, Rect.left, nWndPt1.y, NULL);
		::LineTo(hDC, Rect.right, nWndPt1.y);
		::MoveToEx(hDC, nWndPt1.x, Rect.top, NULL);
		::LineTo(hDC, nWndPt1.x, Rect.bottom);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;

		::SetBkColor(hDC, clrBK);
		::SetTextColor(hDC, clrText);
		str.Format(_T("Cast-2  Ave:%.0f, Max:%.0f, Min:%.0f"), m_PhaseAve[idx], m_PhaseMax[idx], m_PhaseMin[idx]);
		::TextOut(hDC, 0, 0, str, str.GetLength());		

		if ( TRUE == bDrawRectLine )
		{	
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt1, dWndPt1);
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt2, dWndPt2);
			nWndPt1.x = JetAPI::Floor(MIN(dWndPt1.x, dWndPt2.x));
			nWndPt1.y = JetAPI::Floor(MIN(dWndPt1.y, dWndPt2.y));
			nWndPt2.x = JetAPI::Floor(MAX(dWndPt1.x, dWndPt2.x));
			nWndPt2.y = JetAPI::Floor(MAX(dWndPt1.y, dWndPt2.y));

			hPen = ::CreatePen(PS_SOLID, 1, clrRectLine);
			hOldPen = (HPEN)::SelectObject(hDC, hPen);			
			::MoveToEx(hDC, nWndPt1.x, nWndPt1.y, NULL);
			::LineTo(hDC, nWndPt2.x, nWndPt1.y);
			::LineTo(hDC, nWndPt2.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt1.y);			
			::DeleteObject(hPen);	hPen=NULL;
		}

		//Mark Pos		
		hPen = ::CreatePen(PS_SOLID, 1, 0x00FF00);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark1, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		

		hPen = ::CreatePen(PS_SOLID, 1, 0xFF0000);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark2, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		
	}

	if ( m_ImageWnd3.GetSafeHwnd() != NULL )
	{			
		idx = 2;
		CClientDC dc(&m_ImageWnd3);	
		RECT Rect = m_ImageWndRect3;
		HDC hDC = dc.GetSafeHdc();	
		HDC hBKDC = m_ImageWndMemDC3.GetSafeHdc();		
		::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);	

		//Copy m_MemHDC Image to MemHDC1 use in Back ground
		::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hBKDC, 0, 0, SRCCOPY );

		hPen = ::CreatePen(PS_DOT, 1, clrCenterLine);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt, dWndPt1);				
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		::MoveToEx(hDC, Rect.left, nWndPt1.y, NULL);
		::LineTo(hDC, Rect.right, nWndPt1.y);
		::MoveToEx(hDC, nWndPt1.x, Rect.top, NULL);
		::LineTo(hDC, nWndPt1.x, Rect.bottom);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;

		::SetBkColor(hDC, clrBK);
		::SetTextColor(hDC, clrText);
		str.Format(_T("Cast-3  Ave:%.0f, Max:%.0f, Min:%.0f"), m_PhaseAve[idx], m_PhaseMax[idx], m_PhaseMin[idx]);
		::TextOut(hDC, 0, 0, str, str.GetLength());		

		if ( TRUE == bDrawRectLine )
		{	
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt1, dWndPt1);
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt2, dWndPt2);
			nWndPt1.x = JetAPI::Floor(MIN(dWndPt1.x, dWndPt2.x));
			nWndPt1.y = JetAPI::Floor(MIN(dWndPt1.y, dWndPt2.y));
			nWndPt2.x = JetAPI::Floor(MAX(dWndPt1.x, dWndPt2.x));
			nWndPt2.y = JetAPI::Floor(MAX(dWndPt1.y, dWndPt2.y));

			hPen = ::CreatePen(PS_SOLID, 1, clrRectLine);
			hOldPen = (HPEN)::SelectObject(hDC, hPen);			
			::MoveToEx(hDC, nWndPt1.x, nWndPt1.y, NULL);
			::LineTo(hDC, nWndPt2.x, nWndPt1.y);
			::LineTo(hDC, nWndPt2.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt1.y);			
			::DeleteObject(hPen);	hPen=NULL;
		}

		//Mark Pos		
		hPen = ::CreatePen(PS_SOLID, 1, 0x00FF00);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark1, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		

		hPen = ::CreatePen(PS_SOLID, 1, 0xFF0000);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark2, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		
	}

	if ( m_ImageWnd4.GetSafeHwnd() != NULL )
	{			
		idx = 3;
		CClientDC dc(&m_ImageWnd4);	
		RECT Rect = m_ImageWndRect4;
		HDC hDC = dc.GetSafeHdc();	
		HDC hBKDC = m_ImageWndMemDC4.GetSafeHdc();		
		::IntersectClipRect(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);	

		//Copy m_MemHDC Image to MemHDC1 use in Back ground
		::BitBlt(hDC, 0, 0, Rect.right, Rect.bottom, hBKDC, 0, 0, SRCCOPY );

		hPen = ::CreatePen(PS_DOT, 1, clrCenterLine);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt, dWndPt1);				
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		::MoveToEx(hDC, Rect.left, nWndPt1.y, NULL);
		::LineTo(hDC, Rect.right, nWndPt1.y);
		::MoveToEx(hDC, nWndPt1.x, Rect.top, NULL);
		::LineTo(hDC, nWndPt1.x, Rect.bottom);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;

		::SetBkColor(hDC, clrBK);
		::SetTextColor(hDC, clrText);
		str.Format(_T("Cast-4  Ave:%.0f, Max:%.0f, Min:%.0f"), m_PhaseAve[idx], m_PhaseMax[idx], m_PhaseMin[idx]);
		::TextOut(hDC, 0, 0, str, str.GetLength());		

		if ( TRUE == bDrawRectLine )
		{	
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt1, dWndPt1);
			ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePt2, dWndPt2);
			nWndPt1.x = JetAPI::Floor(MIN(dWndPt1.x, dWndPt2.x));
			nWndPt1.y = JetAPI::Floor(MIN(dWndPt1.y, dWndPt2.y));
			nWndPt2.x = JetAPI::Floor(MAX(dWndPt1.x, dWndPt2.x));
			nWndPt2.y = JetAPI::Floor(MAX(dWndPt1.y, dWndPt2.y));

			hPen = ::CreatePen(PS_SOLID, 1, clrRectLine);
			hOldPen = (HPEN)::SelectObject(hDC, hPen);			
			::MoveToEx(hDC, nWndPt1.x, nWndPt1.y, NULL);
			::LineTo(hDC, nWndPt2.x, nWndPt1.y);
			::LineTo(hDC, nWndPt2.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt2.y);
			::LineTo(hDC, nWndPt1.x, nWndPt1.y);			
			::DeleteObject(hPen);	hPen=NULL;
		}

		//Mark Pos		
		hPen = ::CreatePen(PS_SOLID, 1, 0x00FF00);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark1, dWndPt1);		
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		

		hPen = ::CreatePen(PS_SOLID, 1, 0xFF0000);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[idx], m_ImageH[idx], Rect, m_ImageOffset, m_ImageZoom, m_ImagePtMark2, dWndPt1);				
		JetAPI::Point2DToPoint(dWndPt1, nWndPt1);
		ImageAPI.DrawRectLine(hDC, nWndPt1, HalfW, HalfH);
		::SelectObject(hDC, hPen);
		::DeleteObject(hPen);	hPen=NULL;		
	}	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	POINT pt2 = point;	
	this->m_LBtnUpPos = point;
	this->m_LBtnDownPos = this->m_MovingPos = this->m_LBtnUpPos;	
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND1, &pt2) == true )
	{			
		this->m_ImageWndPt1 = pt2;			
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND2, &pt2) == true )
	{			
		this->m_ImageWndPt1 = pt2;			
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND3, &pt2) == true )
	{			
		this->m_ImageWndPt1 = pt2;			
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND4, &pt2) == true )
	{			
		this->m_ImageWndPt1 = pt2;			
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}	
	this->SetCapture();
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( GetCapture() != this )	{	return; }
	::ReleaseCapture();	
	bool  bInImageWnd = false;	
	POINT pt = point;
	POINT pt2 = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND1, &pt2) == true )
	{	
		bInImageWnd = true;	
		this->m_ImageWndPt2 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND2, &pt2) == true )
	{			
		bInImageWnd = true;	
		this->m_ImageWndPt2 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND3, &pt2) == true )
	{			
		bInImageWnd = true;	
		this->m_ImageWndPt2 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND4, &pt2) == true )
	{			
		bInImageWnd = true;	
		this->m_ImageWndPt2 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
	}

	POINT dp;
	dp.x = point.x-m_LBtnUpPos.x;
	dp.y = point.y-m_LBtnUpPos.y;
	this->m_LBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	
	if ( true==bInImageWnd && abs(dp.x)<2 && abs(dp.y)<2 )
	{		
		m_ImagePtMark1 = m_ImagePt1;	
		UpdateInfoEdit(ALLPHAS_INFO_EDIT2, m_ImagePt1); 
	}
	this->RedrawWnd();
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	POINT pt2 = point;	
	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;	
	/*
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND1, &pt2) == true )
	{	
		this->m_ImageWndPt1 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND2, &pt2) == true )
	{			
		this->m_ImageWndPt1 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND3, &pt2) == true )
	{	
		this->m_ImageWndPt1 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND4, &pt2) == true )
	{	
		this->m_ImageWndPt1 = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, m_ImageWndPt1, m_ImagePt1);
		m_ImagePt2 = m_ImagePt1;
	}*/
	this->SetCapture();		
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( GetCapture() != this )	{	return; }
	::ReleaseCapture();
	bool  bInImageWnd = false;	
	POINT pt = point;
	POINT pt2 = point;	
	TPOINT2D WndPt;
	TPOINT2D ImagePt;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND1, &pt2) == true )
	{	
		bInImageWnd = true;	
		WndPt = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND2, &pt2) == true )
	{			
		bInImageWnd = true;	
		WndPt = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND3, &pt2) == true )
	{			
		bInImageWnd = true;	
		WndPt = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND4, &pt2) == true )
	{			
		bInImageWnd = true;	
		WndPt = pt2;	
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
	}
	
	POINT dp;
	dp.x = point.x-m_RBtnUpPos.x;
	dp.y = point.y-m_RBtnUpPos.y;
	this->m_RBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	if ( true==bInImageWnd && abs(dp.x)<2 && abs(dp.y)<2 )
	{	
		m_ImagePtMark2 = ImagePt;
		UpdateInfoEdit(ALLPHAS_INFO_EDIT3, ImagePt); 
	}
	this->RedrawWnd();
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::UpdateInfoEdit(UINT EditID, TPOINT2D ImagePt)
{
	//ImagePt.x = 1370;
	//ImagePt.y =  402;
	bool     bPhaseUsed = false;
	bool     bSpaceUsed = false;
	int      R[MAX_PAHSE_COUNT]={0}, G[MAX_PAHSE_COUNT]={0}, B[MAX_PAHSE_COUNT]={0};
	int      RawA1[MAX_PAHSE_COUNT]={0};
	int      RawA2[MAX_PAHSE_COUNT]={0};
	int      RawA3[MAX_PAHSE_COUNT]={0};
	int      RawA4[MAX_PAHSE_COUNT]={0};
	int      RawB1[MAX_PAHSE_COUNT]={0};
	int      RawB2[MAX_PAHSE_COUNT]={0};
	int      RawB3[MAX_PAHSE_COUNT]={0};
	int      RawB4[MAX_PAHSE_COUNT]={0};
	int      RawC1[MAX_PAHSE_COUNT]={0};
	int      RawC2[MAX_PAHSE_COUNT]={0};
	int      RawC3[MAX_PAHSE_COUNT]={0};
	int      RawC4[MAX_PAHSE_COUNT]={0};
	int      RawD1[MAX_PAHSE_COUNT]={0};
	int      RawD2[MAX_PAHSE_COUNT]={0};
	int      RawD3[MAX_PAHSE_COUNT]={0};
	int      RawD4[MAX_PAHSE_COUNT]={0};
	size_t   Imgidx=0, PhsIdx=0, SpcIdx=0, RawIdx=0;
	double   PhaseM=0;
	double   PhaseMax=-FLT_MAX;
	double   PhaseMin= FLT_MAX;
	double   PhaseDif=0;
	double   SpaceM=0;			
	double   SpaceMax=-FLT_MAX;	
	double   SpaceMin= FLT_MAX;	
	double   SpaceDif=0;	
	double   Phase[MAX_PAHSE_COUNT]={0};
	double   Space[MAX_PAHSE_COUNT]={0};	
	int      nImageW = 0;
	int      nImageH = 0;
	POINT    ImagePos={0};
	CString  strPixel;	
	size_t   idx = 0;

	bPhaseUsed = false;
	bSpaceUsed = false;
	JetAPI::Point2DToPoint(ImagePt, ImagePos);
	for ( idx=0; idx<MAX_PAHSE_COUNT; idx++ )
	{	
		if ( NULL == m_ShowBuffer[idx] )
		{	continue; }
		nImageW = (int)(m_ImageW[idx]);
		nImageH = (int)(m_ImageH[idx]);
		if ( ImagePos.x <0 || ImagePos.y <0 )
		{	continue; }
		if ( ImagePos.x>=nImageW || ImagePos.y>=nImageH )
		{	continue; }

		if ( NULL != m_PhaseBuffer[idx] )
		{
			PhsIdx = (ImagePos.y*m_PhaseStep[idx])+(ImagePos.x);
			Phase[idx] = m_PhaseBuffer[idx][PhsIdx];
			if ( PhaseMax < Phase[idx] ) { PhaseMax = Phase[idx]; }
			if ( PhaseMin > Phase[idx] ) { PhaseMin = Phase[idx]; }
			bPhaseUsed = true;
		}
		if ( NULL != m_SpaceBuffer[idx] )
		{
			SpcIdx = (ImagePos.y*m_SpaceStep[idx])+(ImagePos.x);
			Space[idx] = m_SpaceBuffer[idx][SpcIdx];
			if ( SpaceMax < Space[idx] ) { SpaceMax = Space[idx]; }
			if ( SpaceMin > Space[idx] ) { SpaceMin = Space[idx]; }
			bSpaceUsed = true;
		}
		if ( 8 == m_BitCount[idx] )
		{					
			Imgidx = (ImagePos.y*m_ShowStep[idx])+(ImagePos.x);
			R[idx] = G[idx] = B[idx] = m_ShowBuffer[idx][Imgidx];							
		}
		else if ( 24 == m_BitCount[idx] )
		{	
			Imgidx = (ImagePos.y*m_ShowStep[idx])+(ImagePos.x*3);
			B[idx] = m_ShowBuffer[idx][Imgidx]; 
			G[idx] = m_ShowBuffer[idx][Imgidx+1]; 
			R[idx] = m_ShowBuffer[idx][Imgidx+2]; 
		}	

		RawIdx = (ImagePos.y*m_PhaseStep[idx])+(ImagePos.x);
		if ( NULL != m_CastParam[idx].PtrA1 )
		{	RawA1[idx] = m_CastParam[idx].PtrA1[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrA2 )
		{	RawA2[idx] = m_CastParam[idx].PtrA2[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrA3 )
		{	RawA3[idx] = m_CastParam[idx].PtrA3[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrA4 )
		{	RawA4[idx] = m_CastParam[idx].PtrA4[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrB1 )
		{	RawB1[idx] = m_CastParam[idx].PtrB1[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrB2 )
		{	RawB2[idx] = m_CastParam[idx].PtrB2[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrB3 )
		{	RawB3[idx] = m_CastParam[idx].PtrB3[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrB4 )
		{	RawB4[idx] = m_CastParam[idx].PtrB4[RawIdx]; }

		if ( NULL != m_CastParam[idx].PtrC1 )
		{	RawC1[idx] = m_CastParam[idx].PtrC1[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrC2 )
		{	RawC2[idx] = m_CastParam[idx].PtrC2[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrC3 )
		{	RawC3[idx] = m_CastParam[idx].PtrC3[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrC4 )
		{	RawC4[idx] = m_CastParam[idx].PtrC4[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrD1 )
		{	RawD1[idx] = m_CastParam[idx].PtrD1[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrD2 )
		{	RawD2[idx] = m_CastParam[idx].PtrD2[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrD3 )
		{	RawD3[idx] = m_CastParam[idx].PtrD3[RawIdx]; }
		if ( NULL != m_CastParam[idx].PtrD4 )
		{	RawD4[idx] = m_CastParam[idx].PtrD4[RawIdx]; }
	}

	if ( ImagePos.x>=0 && ImagePos.y>=0 && ImagePos.x<m_ImageW_M && ImagePos.y<m_ImageH_M )
	{
		if ( NULL != m_PhaseBuffer_M )
		{
			PhsIdx = (ImagePos.y*m_PhaseStep_M)+(ImagePos.x);
			PhaseM = m_PhaseBuffer_M[PhsIdx];								
		}
		if ( NULL != m_SpaceBuffer_M )
		{
			SpcIdx = (ImagePos.y*m_SpaceStep_M)+(ImagePos.x);
			SpaceM = m_SpaceBuffer_M[SpcIdx];								
		}
	}
	if ( true == bPhaseUsed )
	{	PhaseDif = (PhaseMax-PhaseMin)*0.5;	}
	else
	{	PhaseDif = 0.0; }

	if ( true == bSpaceUsed )
	{	SpaceDif = (SpaceMax-SpaceMin)*0.5; }
	else
	{	SpaceDif = 0.0;	}

	bool bOldFormate=false;
	if ( true ==bOldFormate )
	{	strPixel.Format(_T("Image(%.2f, %.2f), Space[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Phase[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Cast01(%d,%d,%d), Cast02(%d,%d,%d), Cast03(%d,%d,%d), Cast04(%d,%d,%d)"), ImagePt.x, ImagePt.y, SpaceM, SpaceDif, Space[0], Space[1], Space[2], Space[3], PhaseM, PhaseDif, Phase[0], Phase[1], Phase[2], Phase[3], R[0], G[0], B[0], R[1], G[1], B[1], R[2], G[2], B[2], R[3], G[3], B[3]); }
	else
	{	
		if ( 2==m_CastParam[0].ExpCount || 2==m_CastParam[1].ExpCount || 2==m_CastParam[2].ExpCount || 2==m_CastParam[3].ExpCount )
		{
			strPixel.Format(_T("Image(%.2f, %.2f), Space[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Phase[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Cast01(%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d), Cast02(%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d), Cast03(%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d), Cast04(%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d-%d,%d,%d,%d)"), 
			ImagePt.x, ImagePt.y, SpaceM, SpaceDif, Space[0], Space[1], Space[2], Space[3], PhaseM, PhaseDif, Phase[0], Phase[1], Phase[2], Phase[3], 
			RawA1[0], RawA2[0], RawA3[0], RawA4[0], RawB1[0], RawB2[0], RawB3[0], RawB4[0], RawC1[0], RawC2[0], RawC3[0], RawC4[0], RawD1[0], RawD2[0], RawD3[0], RawD4[0],
			RawA1[1], RawA2[1], RawA3[1], RawA4[1], RawB1[1], RawB2[1], RawB3[1], RawB4[1], RawC1[1], RawC2[1], RawC3[1], RawC4[1], RawD1[1], RawD2[1], RawD3[1], RawD4[1],
			RawA1[2], RawA2[2], RawA3[2], RawA4[2], RawB1[2], RawB2[2], RawB3[2], RawB4[2], RawC1[2], RawC2[2], RawC3[2], RawC4[2], RawD1[2], RawD2[2], RawD3[2], RawD4[2],
			RawA1[3], RawA2[3], RawA3[3], RawA4[3], RawB1[3], RawB2[3], RawB3[3], RawB4[3], RawC1[3], RawC2[3], RawC3[3], RawC4[3], RawD1[3], RawD2[3], RawD3[3], RawD4[3] ); 
		}
		else
		{
			strPixel.Format(_T("Image(%.2f, %.2f), Space[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Phase[%.0f+-%.0f](%.0f, %.0f, %.0f, %.0f), Cast01(%d,%d,%d,%d--%d,%d,%d,%d), Cast02(%d,%d,%d,%d--%d,%d,%d,%d), Cast03(%d,%d,%d,%d--%d,%d,%d,%d), Cast04(%d,%d,%d,%d--%d,%d,%d,%d)"), ImagePt.x, ImagePt.y, SpaceM, SpaceDif, Space[0], Space[1], Space[2], Space[3], PhaseM, PhaseDif, Phase[0], Phase[1], Phase[2], Phase[3], 
			RawA1[0], RawA2[0], RawA3[0], RawA4[0], RawB1[0], RawB2[0], RawB3[0], RawB4[0],
			RawA1[1], RawA2[1], RawA3[1], RawA4[1], RawB1[1], RawB2[1], RawB3[1], RawB4[1],
			RawA1[2], RawA2[2], RawA3[2], RawA4[2], RawB1[2], RawB2[2], RawB3[2], RawB4[2],
			RawA1[3], RawA2[3], RawA3[3], RawA4[3], RawB1[3], RawB2[3], RawB3[3], RawB4[3] ); 
		}
	}
	this->SetDlgItemText(EditID, strPixel);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	bool  bInImageWnd = false;	
	POINT WndPt = point;	
	POINT WndPt2 = point;	
	TPOINT2D ImagePt=WndPt;
	TPOINT2D WndPt3 =WndPt;	
	if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, ALLPHAS_IMAGE_WND1, &WndPt2) == true )
	{
		bInImageWnd = true;		
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, ImagePt, WndPt3);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, ALLPHAS_IMAGE_WND2, &WndPt2) == true )
	{
		bInImageWnd = true;		
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, ImagePt, WndPt3);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, ALLPHAS_IMAGE_WND3, &WndPt2) == true )
	{
		bInImageWnd = true;		
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, ImagePt, WndPt3);
	}
	else if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, ALLPHAS_IMAGE_WND4, &WndPt2) == true  )
	{
		bInImageWnd = true;		
		ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, ImagePt, WndPt3);
	}

	if ( true == bInImageWnd )
	{	
		m_ImagePt = ImagePt;
		UpdateInfoEdit(ALLPHAS_INFO_EDIT, ImagePt);		
	}

	if ( this != GetCapture() ) 
	{
		this->RedrawWnd();
		CBaseDialog::OnMouseMove(nFlags, point);
		return; 
	}	

	POINT pt  = point;
	POINT pt2  = point;
	if ( nFlags&MK_LBUTTON )
	{
		if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND1, &pt2) == true )
		{	
			this->m_ImageWndPt2 = (pt2);	
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[0], m_ImageH[0], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
		}
		else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND2, &pt2) == true )
		{	
			this->m_ImageWndPt2 = (pt2);	
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[1], m_ImageH[1], m_ImageWndRect2, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
		}
		else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND3, &pt2) == true )
		{	
			this->m_ImageWndPt2 = (pt2);	
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[2], m_ImageH[2], m_ImageWndRect3, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
		}
		else if ( JetAPI::CheckPtInCtrlWnd(this, pt, ALLPHAS_IMAGE_WND4, &pt2) == true )
		{	
			this->m_ImageWndPt2 = (pt2);	
			ImageAPI.MapWndPtToImagePt_DBL(m_ImageW[3], m_ImageH[3], m_ImageWndRect4, m_ImageOffset, m_ImageZoom, m_ImageWndPt2, m_ImagePt2);
		}
	}
	else if ( nFlags&MK_RBUTTON )
	{		
		m_ImageOffset.x += point.x-m_MovingPos.x;
		m_ImageOffset.y += point.y-m_MovingPos.y;		
		m_MovingPos = point;
		this->DrawImageWndMemDC(0);
		this->DrawImageWndMemDC(1);
		this->DrawImageWndMemDC(2);
		this->DrawImageWndMemDC(3);
	}
	this->RedrawWnd();
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( WM_MOUSEWHEEL != message ) { return false; }
	CWnd *pWnd = GetFocus();
	if ( NULL == pWnd ) { return false; }
	if ( this != pWnd )
	{	pWnd = pWnd->GetParent();	}	
	if ( this != pWnd ) { return false; }

	bool bPick=false;
	CPoint pt, point;
	point.x = pt.x = GET_X_LPARAM(lParam); 
	point.y = pt.y = GET_Y_LPARAM(lParam); 
	this->ScreenToClient(&point);
	if ( JetAPI::CheckPtInCtrlWnd(this, point, ALLPHAS_IMAGE_WND1, NULL) == true || 
	     JetAPI::CheckPtInCtrlWnd(this, point, ALLPHAS_IMAGE_WND2, NULL) == true ||
		 JetAPI::CheckPtInCtrlWnd(this, point, ALLPHAS_IMAGE_WND3, NULL) == true ||
		 JetAPI::CheckPtInCtrlWnd(this, point, ALLPHAS_IMAGE_WND4, NULL) == true )
	{	bPick=true;	}
	if ( false == bPick )
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;	

	size_t i=0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{	this->DrawImageWndMemDC(i); }

	this->RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseAllWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ExecLoadPhaseBtn(size_t idx)
{
	if ( idx >= MAX_PAHSE_COUNT ) { return; }

	TCHAR szFilters[]=_T("TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("TXT"), _T("*.TXT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	CString filename = dialog.GetPathName();
	ExecLoadFileBtn(idx, filename);
}
//-------------------------------------------------------------------------------------//
SLICE_FUNC_MODE CImagePhaseAllWnd::GetDlpSliceFuncMode() const
{
	return m_SliceFuncMode;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ExecLoadFileBtn(size_t idx, LPCTSTR pfilename)
{
	CString str;
	CString filename = pfilename;
	const int stridx = filename.ReverseFind(_T('\\'));
	CString foldername =  filename.Left(stridx);

	TCHAR TMode[32] = _T("");
	_tcscpy(TMode, _T("r"));
	JetAPI::ModifyOpenFileMode_Read(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( pfile == NULL )
	{
		str.Format(_T("Error, Open File Fault (%s)"), filename);
		JetAPI::ShowMessageBox(str);
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
		if (_T('\n') == txtBuffer[len - 1])
		{	txtBuffer[len - 1] = _T('\0');	}		
		fileList.push_back(txtBuffer);
	};
	::fclose(pfile); pfile = NULL;

	LARGE_INTEGER  nStartTime;
	LARGE_INTEGER  nEndTime;	
	bool bIsOk=true;
	BOOL bSave = FALSE;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	const size_t MaxPtrCount = 32;
	double PeriodP=0;	
	unsigned char *Ptr=NULL;
	unsigned char *PtrList[MaxPtrCount]={0x00};	
	unsigned char *DstPtr=NULL;		
	PHASE_PTR BasePhasePtr=NULL;	
	SPACE_PTR FactorPtr = NULL;	
	double ElapsedTime=0;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();	
	const int DLPExposureTime_us = CaliParam.m_DLPExposureTime_us;
	const int DLPExposureTime2_us = CaliParam.m_DLPExposureTime2_us;
	int    ExpTimeA=DLPExposureTime_us;
	int    ExpTimeB=DLPExposureTime2_us;
	int    ExpTimeC=DLPExposureTime_us;
	int    ExpTimeD=DLPExposureTime2_us;
	double P1 = SysParam.m_PhasePeriod1;
	double P2 = SysParam.m_PhasePeriod2;
	double P3 = SysParam.m_PhasePeriod3;
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const BOOL PhaseMode = CWnd::IsDlgButtonChecked(ALLPHAS_PHASE_MODE_CHK);
	const BOOL UseZeroPhasePtr = CWnd::IsDlgButtonChecked(ALLPHAS_ZERO_PLANE_CHK);

	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT1, str);
	P1 = ::_ttof(str);
	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT2, str);
	P2 = ::_ttof(str);
	
	TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	SystemParam.m_PhasePeriod1 = P1;
	SystemParam.m_PhasePeriod2 = P2;
	
	int PhaseNoiseDef = 0;	
	TPhaseNoiseParam NoiseParam;
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	NoiseParam.PhaseNoiseDef = SysParam.m_PhaseNoiseDefine;
	NoiseParam.PhaseNoiseDef = 0xFF;
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_SMOOTH_FILTER_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_SMOOTH_FILTER; }	
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_LOW_CONTRAST_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_CONTRAST; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_LOW_POTENTIAL_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_POTENTIAL; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_OVER_SATURATED_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_OVER_SATURATED; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_VOID_EXPAND_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_VOID_EXPAND; }	
	NoiseParam.PhaseNoiseDef = PhaseNoiseDef;
	NoiseParam.PhaseLowContrastA = CWnd::GetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedA = CWnd::GetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT);
	NoiseParam.PhaseLowPotentialA = CWnd::GetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT);
	NoiseParam.PhaseLowContrastB = CWnd::GetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT2);
	NoiseParam.PhaseOverSaturatedB = CWnd::GetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT2);
	NoiseParam.PhaseLowPotentialB = CWnd::GetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT2);
	NoiseParam.PhaseExtendVoid = CWnd::GetDlgItemInt(ALLPHAS_VOID_EXPAND_EDIT);
	NoiseParam.PhaseSmoothFilter = CWnd::GetDlgItemInt(ALLPHAS_SMOOTH_FILTER_EDIT);

	size_t i=0, j=0;
	size_t   TempBufferSize=0;
	size_t   SmoothPeriod=0;
	unsigned char *TempBufferPtr=NULL;
	const int Align = 4;
	const size_t FileCount = fileList.size();
	switch ( FileCount )
	{
	case 3:	SmoothPeriod = 0;	break;
	case 4:	SmoothPeriod = 0;	break;
	case 5:	SmoothPeriod = 0;	break;		
	case 6:	SmoothPeriod = 4;	break;
	case 8:	SmoothPeriod = 4;	break;	
	case 10: SmoothPeriod = 5;	break;
	case 9:	SmoothPeriod = 6;	break;
	case 12: SmoothPeriod = 8;	break;
	case 15: SmoothPeriod = 10;	break;	
	}
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
				JetAPI::ShowMessageBox(str);

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

			if ( NoiseParam.PhaseSmoothFilter>0 && i>=SmoothPeriod && (PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER)!=0 ) 
			{				
				TempBufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
				const int KerSize = NoiseParam.PhaseSmoothFilter*2+1;
				if ( JetMemory.alloc_func(TempBufferSize, TempBufferPtr, "CImagePhaseAllWnd::ExecLoadFileBtn", "TempBufferPtr") == true )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, PtrList[i], KerSize, TempBufferPtr) == false )
					{	JetMemory.free_func(TempBufferPtr); }
					else
					{
						JetMemory.free_func(PtrList[i]);
						PtrList[i] = TempBufferPtr;
					}					
				}
			}
		}
	}	
	m_PhaseNoiseParam=NoiseParam;

	int       PatternID=0;
	int       DLPExpCount=1;
	int       CastPhaseMode = 0;	
	double    ImageGamma=1.0;
	DECODE_PHASE_MODE DecodeMode=DECODE_PHASE_NONE;
	SLICE_FUNC_MODE SliceFuncMode=GetDlpSliceFuncMode();
	const int DLPLEDColor = JetAPI::GetComboxCurSelData(m_DLPLEDCombox);
	LIGHT_3D_CAST_ID Light3DCastID=LIGHT_3D_CAST_00;
	switch ( FileCount )
	{
	case 4: 
		DLPExpCount = 1;
		DecodeMode = DECODE_PHASE_4STEP_1;
		CastPhaseMode = LIGHT3D_PHASE_4_4_1; 
		break;
	case 5: 
		DLPExpCount = 1;
		DecodeMode = DECODE_PHASE_2_2STEP_2;
		CastPhaseMode = LIGHT3D_PHASE_2_2_M; 
		break;
	case 6: 
		DLPExpCount = 1;
		DecodeMode = DECODE_PHASE_4_2STEP_2;
		CastPhaseMode = LIGHT3D_PHASE_4_2_M; 
		break;
	case 8: 
		DLPExpCount = 1;
		if ( SLICE_FUNC_3D_4STEP_4GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode )
		{
			DecodeMode = DECODE_PHASE_4STEP_4GC_2;
			CastPhaseMode = LIGHT3D_PHASE_4_4GC_M; 
		}
		else
		{
			DecodeMode = DECODE_PHASE_4_4STEP_2;
			CastPhaseMode = LIGHT3D_PHASE_4_4_M; 
		}
		break;
	case 9: 
		//if ( SLICE_FUNC_3D_4STEP_5GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode )
		DLPExpCount = 1;
		DecodeMode = DECODE_PHASE_4STEP_5GC_2;
		CastPhaseMode = LIGHT3D_PHASE_4_5GC_M; 
		break;
	case 10:
		//if ( SLICE_FUNC_3D_4STEP_6GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode )
		DLPExpCount = 1;
		DecodeMode = DECODE_PHASE_4STEP_6GC_2;
		CastPhaseMode = LIGHT3D_PHASE_4_6GC_M; 
		//DLPExpCount = 2;
		//DecodeMode = DECODE_PHASE_2_2STEP_2;
		//CastPhaseMode = LIGHT3D_PHASE_2_2_M_2; 		
		break;
	case 12:
		DLPExpCount = 2;
		DecodeMode = DECODE_PHASE_4_2STEP_2;
		CastPhaseMode = LIGHT3D_PHASE_4_2_M_2; 		
		break;
	case 16: 
		DLPExpCount = 2;
		if ( SLICE_FUNC_3D_4STEP_4GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode )
		{
			DecodeMode = DECODE_PHASE_4STEP_4GC_2;
			CastPhaseMode = LIGHT3D_PHASE_4_4GC_M_2; 
		}
		else
		{
			DecodeMode = DECODE_PHASE_4_4STEP_2;
			CastPhaseMode = LIGHT3D_PHASE_4_4_M_2; 
		}
		break;
	case 18: 
		DLPExpCount = 2;
		if ( SLICE_FUNC_3D_4STEP_5GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode )
		{
			DecodeMode = DECODE_PHASE_4STEP_5GC_2;
			CastPhaseMode = LIGHT3D_PHASE_4_5GC_M_2; 
		}		
		break;
	case 20: 
		DLPExpCount = 2;
		if ( SLICE_FUNC_3D_4STEP_6GC_1EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode )
		{
			DecodeMode = DECODE_PHASE_4STEP_6GC_2;
			CastPhaseMode = LIGHT3D_PHASE_4_6GC_M_2; 
		}		
		break;
	}

	Light3DCtrl.GetLight3DImageGamma(Light3DCastID, ImageGamma);

	IMAGE_SIZE ZeroW=0, ZeroH=0, ZeroStep=0;
	Light3DCastID = CLight3DCtrl::GetLight3DCastIDByIndex(idx);	
	if ( FALSE==PhaseMode || TRUE==UseZeroPhasePtr )
	{			
		Light3DCtrl.GetLight3DPhaseZero(Light3DCastID, CastPhaseMode, DLPLEDColor, ZeroW, ZeroH, ZeroStep, BasePhasePtr);
		if ( ZeroW!=ImageW || ZeroH!=ImageH || ZeroStep!=ImageStep )
		{	BasePhasePtr = NULL; }
		//BasePhasePtr = NULL;
	}
	IMAGE_SIZE FactorW=0, FactorH=0, FactorStep=0;
	double    HeightFactor0[HEIGHT_FACTOR_PARAM_COUNT];
	double    HeightFactor1[HEIGHT_FACTOR_PARAM_COUNT];
	double    HeightFactor2[HEIGHT_FACTOR_PARAM_COUNT];
	::memset(HeightFactor0, 0x00, sizeof(HeightFactor0));
	::memset(HeightFactor1, 0x00, sizeof(HeightFactor1));
	::memset(HeightFactor2, 0x00, sizeof(HeightFactor2));
	if ( FALSE == PhaseMode )
	{
		Light3DCtrl.GetLight3DHeightFactor(Light3DCastID, HeightFactor0, HeightFactor1, HeightFactor2);
		Light3DCtrl.GetLight3DPhaseFactor(Light3DCastID, CastPhaseMode, DLPLEDColor, FactorW, FactorH, FactorStep, FactorPtr);
		if ( FactorW!=ImageW || FactorH!=ImageH || FactorStep!=ImageStep )
		{	FactorPtr = NULL; }		
		//FactorPtr = NULL;
	}
	CImagePhaseAllWnd::ReleaseBuffer(idx);
	QueryPerformanceCounter(&nStartTime);
	switch ( FileCount )
	{
	case 3://1周期, 3張圖
		PatternID = PHASE_PATTERN_A;
		bIsOk = ImageAPI.GrayImage3FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PatternID, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx]);
		break;
	case 4://1周期, 4張圖
		PatternID = PHASE_PATTERN_A;
		bIsOk = ImageAPI.GrayImage4FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PatternID, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx]);
		break;
	case 5://1周期, 5張圖 or 2週期, 5張圖(2+2+1)
		if (DECODE_PHASE_2_2STEP_2 == DecodeMode)
		{
			bIsOk = ImageAPI.GrayImage221FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], P1, ExpTimeA, PtrList[2], PtrList[3], PtrList[4], P2, ExpTimeB, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);
		}
		else
		{
			PatternID = PHASE_PATTERN_A;
			bIsOk = ImageAPI.GrayImage5FrameToPhase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], PatternID, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx]);
		}
		break;		
	case 6://2周期, 3 and 3 or 4 and 2+1
		//bIsOk = ImageAPI.GrayImage3FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], P1, PtrList[3], PtrList[4], PtrList[5], P2, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);
		bIsOk = ImageAPI.GrayImage42FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, ExpTimeA, PtrList[4], PtrList[5], P2, ExpTimeB, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);
		break;
	case 8://2周期, 4 and 4
		if ( DECODE_PHASE_4STEP_4GC_2 == DecodeMode )
		{	bIsOk = ImageAPI.GrayImage44GCFrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, ExpTimeA, PtrList[4], PtrList[5], PtrList[6], PtrList[7], P2, ExpTimeB, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		else
		{	bIsOk = ImageAPI.GrayImage4FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, PtrList[4], PtrList[5], PtrList[6], PtrList[7], P2, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		break;	
	case 10://2周期, 5 and 5
		if ( DECODE_PHASE_4STEP_6GC_2 == DecodeMode )
		{	bIsOk = ImageAPI.GrayImage46GCFrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, ExpTimeA, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], P2, ExpTimeB, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		else
		{	bIsOk = ImageAPI.GrayImage5FrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], P1, PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], P2, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		break;
	case 9://3周期, 3 and 3 and 3
		if ( DECODE_PHASE_4STEP_5GC_2 == DecodeMode )
		{	bIsOk = ImageAPI.GrayImage45GCFrameTo2Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, ExpTimeA, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[8], P2, ExpTimeB, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		else
		{	bIsOk = ImageAPI.GrayImage3FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], P1, PtrList[3], PtrList[4], PtrList[5], P2, PtrList[6], PtrList[7], PtrList[8], P3, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		break;
	case 12://3周期, 4 and 4 and 4
		bIsOk = ImageAPI.GrayImage4FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], P1, PtrList[4], PtrList[5], PtrList[6], PtrList[7], P2, PtrList[8], PtrList[9], PtrList[10], PtrList[11], P3, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);		
		break;
	case 15://3周期, 5 and 5 and 5
		bIsOk = ImageAPI.GrayImage5FrameTo3Phase(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[4], P1, PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], P2, PtrList[10], PtrList[11], PtrList[12], PtrList[13], PtrList[14], P3, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);		
		break;
	case 16:
		if ( DECODE_PHASE_4STEP_4GC_2 == DecodeMode )		            
		{	bIsOk = ImageAPI.GrayImage44GCFrameTo2Phase2Exp(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[8], PtrList[9], PtrList[10], PtrList[11], P1, ExpTimeA, ExpTimeC, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[12], PtrList[13], PtrList[14], PtrList[15], P2, ExpTimeB, ExpTimeD, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}		
		else
		{	bIsOk = ImageAPI.GrayImage4FrameTo2Phase2Exp(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[8], PtrList[9], PtrList[10], PtrList[11], P1, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[12], PtrList[13], PtrList[14], PtrList[15], P2, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);		}
		break;
	case 18:
		if ( DECODE_PHASE_4STEP_5GC_2 == DecodeMode )		            
		{	bIsOk = ImageAPI.GrayImage45GCFrameTo2Phase2Exp(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[9], PtrList[10], PtrList[11], PtrList[12], P1, ExpTimeA, ExpTimeC, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[13], PtrList[14], PtrList[15], PtrList[16], PtrList[17], P2, ExpTimeB, ExpTimeD, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		break;
	case 20:
		if ( DECODE_PHASE_4STEP_6GC_2 == DecodeMode )
		{	bIsOk = ImageAPI.GrayImage46GCFrameTo2Phase2Exp(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], PtrList[10], PtrList[11], PtrList[12], PtrList[13], P1, ExpTimeA, ExpTimeC, PtrList[4], PtrList[5], PtrList[6], PtrList[7], PtrList[8], PtrList[9], PtrList[14], PtrList[15], PtrList[16], PtrList[17], PtrList[18], PtrList[19], P2, ExpTimeB, ExpTimeD, ImageGamma, BasePhasePtr, NoiseParam, m_MaskBuffer[idx], m_PhaseBuffer[idx], PeriodP);	}
		break;
	}
	QueryPerformanceCounter(&nEndTime);
	if ( false == bIsOk )		
	{	
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(m_MaskBuffer[idx]);
		JetMemory.free_func(m_PhaseBuffer[idx]);		
		for ( j=0; j<MaxPtrCount; j++ )
		{	JetMemory.free_func(PtrList[j]);	}
		return;
	}
	m_EllapseTime[idx] = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("Mask"), idx+1);
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, m_MaskBuffer[idx], true);
	}

	Light3DCastID = CLight3DCtrl::GetLight3DCastIDByIndex(idx);	
	if ( TRUE==PhaseMode )
	{	
		if ( DLP_LED_COLOR_DEBUG == DLPLEDColor )
		{
			LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(Light3DCastID);
			if ( NULL != Light3DPtr )
			{
				Light3DPtr->SetPhaseZero(CastPhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, m_PhaseBuffer[idx]);			
				Light3DPtr->SavePhaseZero();
			}		
		}
	}	

	if ( NULL != BasePhasePtr )
	{
		str.Format(_T("%s\\PhaseZero_%d.DAT"), AOIDataCollect.GetAOITempDirectory(), idx+1);		
		SavePhaseFile_JET6500(str, ZeroW, ZeroH, ZeroStep, BasePhasePtr);		
	}
	if ( NULL != FactorPtr )
	{		
		str.Format(_T("%s\\PhaseKVale_%d.DAT"), AOIDataCollect.GetAOITempDirectory(), idx+1);		
		SaveKValeFile_JET6500(str, FactorW, FactorH, FactorStep, FactorPtr);		
	}	

	str.Format(_T("%s\\PhaseMask_LowContrast_%d.PNG"), AOIDataCollect.GetAOITempDirectory(), idx+1);
	SavePhaseMask_Debug(str, ImageW, ImageH, ImageStep, m_MaskBuffer[idx], PHASE_MASK_LOW_CONTRAST);
	str.Format(_T("%s\\PhaseMask_LowPotential_%d.PNG"), AOIDataCollect.GetAOITempDirectory(), idx+1);
	SavePhaseMask_Debug(str, ImageW, ImageH, ImageStep, m_MaskBuffer[idx], PHASE_MASK_LOW_POTENTIAL);
	str.Format(_T("%s\\PhaseMask_OverSaturated_%d.PNG"), AOIDataCollect.GetAOITempDirectory(), idx+1);
	SavePhaseMask_Debug(str, ImageW, ImageH, ImageStep, m_MaskBuffer[idx], PHASE_MASK_OVER_SATURATED);
	
	//Save Phase Data
	str.Format(_T("%s\\Phase_%d.DAT"), AOIDataCollect.GetAOITempDirectory(), idx+1);
	SavePhaseImage_Debug(str, ImageW, ImageH, ImageStep, m_PhaseBuffer[idx]);

	//求得平均樣版影像
	switch ( FileCount )
	{
	case 3://1周期, 3張圖	
	case 9://3周期, 3 and 3 and 3
		if ( DECODE_PHASE_4STEP_5GC_2 == DecodeMode )
		{	bIsOk = ImageAPI.AverageGrayImage4Frame(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], DstPtr);	}
		else
		{	bIsOk = ImageAPI.AverageGrayImage3Frame(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], DstPtr);	}		
		break;
	case 5://1周期, 5張圖
		if (DECODE_PHASE_2_2STEP_2 == DecodeMode)
		{	bIsOk = ImageAPI.CloneGrayImage(ImageW, ImageH, ImageStep, PtrList[4], DstPtr, false);	}
		else
		{	bIsOk = ImageAPI.AverageGrayImage4Frame(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], DstPtr);	}
		break;
	case 6://2周期, 4+21
	case 4://1周期, 4張圖
	case 8://2周期, 4 and 4
	case 10://2周期, 5 and 5
	case 12://3周期, 4 and 4 and 4
	case 15://3周期, 5 and 5 and 5
		//if ( DECODE_PHASE_4STEP_4GC_2 == DecodeMode )
		//if ( DECODE_PHASE_4STEP_6GC_2 == DecodeMode )
		bIsOk = ImageAPI.AverageGrayImage4Frame(ImageW, ImageH, ImageStep, PtrList[0], PtrList[1], PtrList[2], PtrList[3], DstPtr);
		break;
	case 16:
		bIsOk = ImageAPI.AverageGrayImage4Frame(ImageW, ImageH, ImageStep, PtrList[8], PtrList[9], PtrList[10], PtrList[11], DstPtr);
		break;
	case 20:
		bIsOk = ImageAPI.AverageGrayImage4Frame(ImageW, ImageH, ImageStep, PtrList[10], PtrList[11], PtrList[12], PtrList[13], DstPtr);
		break;
	}
	if ( false == bIsOk )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(m_MaskBuffer[idx]);
		JetMemory.free_func(m_PhaseBuffer[idx]);		
		for ( j=0; j<MaxPtrCount; j++ )
		{	JetMemory.free_func(PtrList[j]);	}
		return;
	}
	
	str.Format(_T("%s\\ImageAverage_%d.PNG"), AOIDataCollect.GetAOITempDirectory(), idx+1);
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, DstPtr, true);

	size_t AveCount = 0;
	this->m_PhaseStep[idx] = ImageStep;
	this->m_PhaseAve[idx] = 0.0;
	this->m_PhaseMax[idx] = PHASE_MIN;
	this->m_PhaseMin[idx] = PHASE_MAX;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	for ( i=0; i<BufferSize; i++ )
	{		
		if ( ImageAPI.CheckSpaceMaskValid(m_MaskBuffer[idx][i]) == false ) { continue; }

		m_PhaseAve[idx] += m_PhaseBuffer[idx][i];
		if ( this->m_PhaseMax[idx] < m_PhaseBuffer[idx][i] ) { this->m_PhaseMax[idx] = m_PhaseBuffer[idx][i]; }
		if ( this->m_PhaseMin[idx] > m_PhaseBuffer[idx][i] ) { this->m_PhaseMin[idx] = m_PhaseBuffer[idx][i]; }
		AveCount ++;
	}
	if ( AveCount > 0 ) 
	{	this->m_PhaseAve[idx] /= AveCount; }

	const IMAGE_SIZE BitCount2 = 24;
	const IMAGE_SIZE ImageStep2 = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount2, 4);		
	if ( NULL == FactorPtr )
	{
		//bIsOk = ImageAPI.PhaseGrayImageConvertToGray(ImageW, ImageH, ImageStep, m_PhaseBuffer[idx], ImageStep, m_ShowBuffer[idx], false);
		bIsOk = ImageAPI.PhaseGrayImageConvertToColor(ImageW, ImageH, ImageStep, m_PhaseBuffer[idx], m_MaskBuffer[idx], ImageStep2, m_ShowBuffer[idx], false);
		if ( bIsOk == false )//Phase Image
		{
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(m_MaskBuffer[idx]);
			JetMemory.free_func(m_PhaseBuffer[idx]);	
			for ( j=0; j<MaxPtrCount; j++ )
			{	JetMemory.free_func(PtrList[j]);	}

			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
			return;
		}	
	}
	else
	{	
		bIsOk = ImageAPI.PhaseImageToSpaceImage(ImageW, ImageH, ImageStep, m_PhaseBuffer[idx], FactorPtr, m_MaskBuffer[idx], m_SpaceBuffer[idx]);//將相位影像轉成空間相對位置
		if ( bIsOk == false )//Space Image
		{
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(m_MaskBuffer[idx]);					
			JetMemory.free_func(m_PhaseBuffer[idx]);	
			JetMemory.free_func(m_SpaceBuffer[idx]);	
			for ( j=0; j<MaxPtrCount; j++ )
			{	JetMemory.free_func(PtrList[j]);	}
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
			return;
		}
		this->m_SpaceStep[idx] = ImageStep;
		bIsOk = ImageAPI.SpaceGrayImageConvertToColor(ImageW, ImageH, ImageStep, m_SpaceBuffer[idx], m_MaskBuffer[idx], ImageStep2, m_ShowBuffer[idx], SpaceRatio, false);//將相位影像轉成空間相對位置		
		if ( bIsOk == false )//Space Image
		{
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(m_MaskBuffer[idx]);					
			JetMemory.free_func(m_PhaseBuffer[idx]);	
			JetMemory.free_func(m_SpaceBuffer[idx]);	
			for ( j=0; j<MaxPtrCount; j++ )
			{	JetMemory.free_func(PtrList[j]);	}
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString()); 
			return;
		}
	}

	j = 0;	
	switch ( FileCount ) 
	{
	case 3:
		m_CastParam[idx].ExpCount = 1;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];		
		m_CastParam[idx].PtrA3 = PtrList[j++];
		break;
	case 4:
		m_CastParam[idx].ExpCount = 1;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];		
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		break;
	case 5:
		if(DECODE_PHASE_2_2STEP_2 == DecodeMode)
		{
			m_CastParam[idx].ExpCount = 1;
			m_CastParam[idx].PtrA1 = PtrList[j++];
			m_CastParam[idx].PtrA2 = PtrList[j++];		
			m_CastParam[idx].PtrB1 = PtrList[j++];
			m_CastParam[idx].PtrB2 = PtrList[j++];	
			m_CastParam[idx].PtrB3 = PtrList[j++];	
		}		
		else
		{
			m_CastParam[idx].ExpCount = 1;
			m_CastParam[idx].PtrA1 = PtrList[j++];
			m_CastParam[idx].PtrA2 = PtrList[j++];		
			m_CastParam[idx].PtrA3 = PtrList[j++];
			m_CastParam[idx].PtrA4 = PtrList[j++];
			m_CastParam[idx].PtrA5 = PtrList[j++];
		}
		break;
	case 6:
		m_CastParam[idx].ExpCount = 1;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];		
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];//4Step
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];		
		//m_CastParam[idx].PtrB3 = PtrList[j++];		
		break;
	case 8:
		//if ( DECODE_PHASE_4STEP_4GC_2 == DecodeMode )
		m_CastParam[idx].ExpCount = 1;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];		
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];		
		m_CastParam[idx].PtrB3 = PtrList[j++];
		m_CastParam[idx].PtrB4 = PtrList[j++];
		break;
	case 9:
		if ( DECODE_PHASE_4STEP_5GC_2 == DecodeMode )
		{
			m_CastParam[idx].ExpCount = 1;
			m_CastParam[idx].PtrA1 = PtrList[j++];
			m_CastParam[idx].PtrA2 = PtrList[j++];		
			m_CastParam[idx].PtrA3 = PtrList[j++];
			m_CastParam[idx].PtrA4 = PtrList[j++];
			m_CastParam[idx].PtrB1 = PtrList[j++];
			m_CastParam[idx].PtrB2 = PtrList[j++];		
			m_CastParam[idx].PtrB3 = PtrList[j++];
			m_CastParam[idx].PtrB4 = PtrList[j++];
			m_CastParam[idx].PtrB5 = PtrList[j++];
		}
		break;
	case 10:
		if ( DECODE_PHASE_4STEP_6GC_2 == DecodeMode )
		{
			m_CastParam[idx].ExpCount = 1;
			m_CastParam[idx].PtrA1 = PtrList[j++];
			m_CastParam[idx].PtrA2 = PtrList[j++];		
			m_CastParam[idx].PtrA3 = PtrList[j++];
			m_CastParam[idx].PtrA4 = PtrList[j++];
			m_CastParam[idx].PtrB1 = PtrList[j++];
			m_CastParam[idx].PtrB2 = PtrList[j++];		
			m_CastParam[idx].PtrB3 = PtrList[j++];
			m_CastParam[idx].PtrB4 = PtrList[j++];
			m_CastParam[idx].PtrB5 = PtrList[j++];
			m_CastParam[idx].PtrB6 = PtrList[j++];
		}
		else
		{
			if(DECODE_PHASE_2_2STEP_2 == DecodeMode)
			{
				m_CastParam[idx].ExpCount = 2;
				m_CastParam[idx].PtrA1 = PtrList[j++];
				m_CastParam[idx].PtrA2 = PtrList[j++];
				m_CastParam[idx].PtrB1 = PtrList[j++];
				m_CastParam[idx].PtrB2 = PtrList[j++];
				m_CastParam[idx].PtrB3 = PtrList[j++];	
				m_CastParam[idx].PtrC1 = PtrList[j++];
				m_CastParam[idx].PtrC2 = PtrList[j++];
				m_CastParam[idx].PtrD1 = PtrList[j++];
				m_CastParam[idx].PtrD2 = PtrList[j++];
				m_CastParam[idx].PtrD3 = PtrList[j++];
			}
			else
			{
				m_CastParam[idx].ExpCount = 1;
				m_CastParam[idx].PtrA1 = PtrList[j++];
				m_CastParam[idx].PtrA2 = PtrList[j++];		
				m_CastParam[idx].PtrA3 = PtrList[j++];
				m_CastParam[idx].PtrA4 = PtrList[j++];
				m_CastParam[idx].PtrA5 = PtrList[j++];
				m_CastParam[idx].PtrB1 = PtrList[j++];
				m_CastParam[idx].PtrB2 = PtrList[j++];		
				m_CastParam[idx].PtrB3 = PtrList[j++];
				m_CastParam[idx].PtrB4 = PtrList[j++];
				m_CastParam[idx].PtrB5 = PtrList[j++];
			}
		}
		break;
	case 12:
		m_CastParam[idx].ExpCount = 2;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];		
		m_CastParam[idx].PtrC1 = PtrList[j++];
		m_CastParam[idx].PtrC2 = PtrList[j++];
		m_CastParam[idx].PtrC3 = PtrList[j++];
		m_CastParam[idx].PtrC4 = PtrList[j++];
		m_CastParam[idx].PtrD1 = PtrList[j++];
		m_CastParam[idx].PtrD2 = PtrList[j++];		
		break;
	case 16:
		m_CastParam[idx].ExpCount = 2;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];
		m_CastParam[idx].PtrB3 = PtrList[j++];
		m_CastParam[idx].PtrB4 = PtrList[j++];
		m_CastParam[idx].PtrC1 = PtrList[j++];
		m_CastParam[idx].PtrC2 = PtrList[j++];
		m_CastParam[idx].PtrC3 = PtrList[j++];
		m_CastParam[idx].PtrC4 = PtrList[j++];
		m_CastParam[idx].PtrD1 = PtrList[j++];
		m_CastParam[idx].PtrD2 = PtrList[j++];
		m_CastParam[idx].PtrD3 = PtrList[j++];
		m_CastParam[idx].PtrD4 = PtrList[j++];
		break;
	case 18:
		m_CastParam[idx].ExpCount = 2;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];
		m_CastParam[idx].PtrB3 = PtrList[j++];
		m_CastParam[idx].PtrB4 = PtrList[j++];
		m_CastParam[idx].PtrB5 = PtrList[j++];
		m_CastParam[idx].PtrC1 = PtrList[j++];
		m_CastParam[idx].PtrC2 = PtrList[j++];
		m_CastParam[idx].PtrC3 = PtrList[j++];
		m_CastParam[idx].PtrC4 = PtrList[j++];
		m_CastParam[idx].PtrD1 = PtrList[j++];
		m_CastParam[idx].PtrD2 = PtrList[j++];
		m_CastParam[idx].PtrD3 = PtrList[j++];
		m_CastParam[idx].PtrD4 = PtrList[j++];
		m_CastParam[idx].PtrD5 = PtrList[j++];
		break;
	case 20:
		m_CastParam[idx].ExpCount = 2;
		m_CastParam[idx].PtrA1 = PtrList[j++];
		m_CastParam[idx].PtrA2 = PtrList[j++];
		m_CastParam[idx].PtrA3 = PtrList[j++];
		m_CastParam[idx].PtrA4 = PtrList[j++];
		m_CastParam[idx].PtrB1 = PtrList[j++];
		m_CastParam[idx].PtrB2 = PtrList[j++];
		m_CastParam[idx].PtrB3 = PtrList[j++];
		m_CastParam[idx].PtrB4 = PtrList[j++];
		m_CastParam[idx].PtrB5 = PtrList[j++];
		m_CastParam[idx].PtrB6 = PtrList[j++];
		m_CastParam[idx].PtrC1 = PtrList[j++];
		m_CastParam[idx].PtrC2 = PtrList[j++];
		m_CastParam[idx].PtrC3 = PtrList[j++];
		m_CastParam[idx].PtrC4 = PtrList[j++];
		m_CastParam[idx].PtrD1 = PtrList[j++];
		m_CastParam[idx].PtrD2 = PtrList[j++];
		m_CastParam[idx].PtrD3 = PtrList[j++];
		m_CastParam[idx].PtrD4 = PtrList[j++];
		m_CastParam[idx].PtrD5 = PtrList[j++];
		m_CastParam[idx].PtrD6 = PtrList[j++];
		break;
	}
	m_CastParam[idx].PerA = P1;
	m_CastParam[idx].PerB = P2;		
	m_CastParam[idx].ImageW = ImageW;
	m_CastParam[idx].ImageH = ImageH;
	m_CastParam[idx].ImageStep = ImageStep;
	m_CastParam[idx].ImageCount = FileCount;
	m_CastParam[idx].DecodeMode = DecodeMode;
	m_CastParam[idx].Gamma = ImageGamma;
	m_CastParam[idx].ExpTimeA = ExpTimeA;
	m_CastParam[idx].ExpTimeB = ExpTimeB;
	m_CastParam[idx].ExpTimeC = ExpTimeC;
	m_CastParam[idx].ExpTimeD = ExpTimeD;
	m_CastParam[idx].ZeroPhasePtr = BasePhasePtr;
	m_CastParam[idx].HeightFactorPtr = FactorPtr;
	::memcpy(m_CastParam[idx].HeightFactor0, HeightFactor0, sizeof(m_CastParam[idx].HeightFactor0));
	::memcpy(m_CastParam[idx].HeightFactor1, HeightFactor1, sizeof(m_CastParam[idx].HeightFactor1));
	::memcpy(m_CastParam[idx].HeightFactor2, HeightFactor2, sizeof(m_CastParam[idx].HeightFactor2));	
	if ( JetMemory.alloc_func(BufferSize, m_CastParam[idx].PtrMask, "CImagePhaseAllWnd::ExecLoadFileBtn", "m_CastParam[idx].PtrMask") == false ||
		 JetMemory.alloc_func(BufferSize, m_CastParam[idx].PtrSpace, "CImagePhaseAllWnd::ExecLoadFileBtn", "m_CastParam[idx].PtrSpace") == false )
	{	
		JetMemory.free_func(m_CastParam[idx].PtrMask);
		JetMemory.free_func(m_CastParam[idx].PtrSpace);
	}
	//JetMemory.free_func(DstPtr);	
	//for ( j=0; j<MaxPtrCount; j++ )
	//{	JetMemory.free_func(PtrList[j]);	}

	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseImage"), idx+1);
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep2, 24, m_ShowBuffer[idx], true);	
	}

	UINT ChkWndID = 0;
	switch ( idx )
	{
	case 0:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK1;	break;
	case 1:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK2;	break;
	case 2:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK3;	break;
	case 3:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK4;	break;
	}
	if ( 0 != ChkWndID )
	{
		JetAPI::EnableCtrlWnd(this, ChkWndID, TRUE);		
		CWnd::CheckDlgButton(ChkWndID, TRUE);
	}	

	this->m_ImageW[idx] = ImageW;
	this->m_ImageH[idx] = ImageH;
	this->m_ImageStep[idx] = ImageStep;
	this->m_BitCount[idx] = 24;	
	this->m_ShowStep[idx] = ImageStep2;	
	this->m_ImageBuffer[idx] = DstPtr;
	this->m_PhaseFileName[idx] = pfilename;
	this->DrawImageWndMemDC(idx);
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLoadPhaseBtn1() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseAllWnd::ExecLoadPhaseBtn(0);
	CImagePhaseAllWnd::OnShowImageRadio();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLoadPhaseBtn2() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseAllWnd::ExecLoadPhaseBtn(1);
	CImagePhaseAllWnd::OnShowImageRadio();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLoadPhaseBtn3() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseAllWnd::ExecLoadPhaseBtn(2);
	CImagePhaseAllWnd::OnShowImageRadio();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLoadPhaseBtn4() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseAllWnd::ExecLoadPhaseBtn(3);
	CImagePhaseAllWnd::OnShowImageRadio();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnMergePhaseBtn() 
{
	// TODO: Add your control notification handler code here	
	BOOL bPhaseMode = CWnd::IsDlgButtonChecked(ALLPHAS_PHASE_MODE_CHK);
	BOOL bZeroPhase = CWnd::IsDlgButtonChecked(ALLPHAS_ZERO_PLANE_CHK);

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);
	if ( FALSE==bPhaseMode || TRUE==bZeroPhase )
	{	ExecMergeSpace(); }
	else
	{	ExecMergePhase(); }
	::SetCursor(hOldCursor);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnCalculateAllBtn() 
{
	// TODO: Add your control notification handler code here
	size_t i = 0, Count = 0;
	CString filename;
	UINT    ChkWndID = 0;
	BOOL    bChk = TRUE;

	Count = 0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{
		switch ( i )
		{
		case 0:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK1;	break;
		case 1:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK2;	break;
		case 2:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK3;	break;
		case 3:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK4;	break;
		default: ChkWndID = 0;
		}
		if ( 0 != ChkWndID )
		{	bChk = CWnd::IsDlgButtonChecked(ChkWndID); }

		filename = this->m_PhaseFileName[i];
		if ( filename.GetLength() == 0 ) { continue; }
		CImagePhaseAllWnd::ExecLoadFileBtn(i, filename);
	
		if ( 0 != ChkWndID )
		{	CWnd::CheckDlgButton(ChkWndID, bChk);	}

		Count ++;
	}
	if ( Count > 1 ) 
	{
		CWnd::CheckDlgButton(IDC_SHOW_IMAGE_RADIO, TRUE);
		CImagePhaseAllWnd::OnShowImageRadio();
		CImagePhaseAllWnd::OnMergePhaseBtn();
	}
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ExecMergeSpace()
{
	const char fnName[] = "CImagePhaseAllWnd::ExecMergeSpace";
	this->ReleaseBuffer_M();

	CString str;
	UINT       ChkWndID=0;
	size_t     i=0, j=0, idx=0;
	LARGE_INTEGER  nStartTime;
	LARGE_INTEGER  nEndTime;
	IMAGE_SIZE ImageW=0, ImageH=0, PhaseStep=0;
	MASK_PTR  MaskPtrM=NULL;
	MASK_PTR  ImagePtrM=NULL;
	SPACE_PTR SpacePtrM=NULL;
	TCastParam CastParam[MAX_PAHSE_COUNT];
	MASK_PTR  MaskPtr[MAX_PAHSE_COUNT]={NULL};
	IMAGE_PTR ImagePtr[MAX_PAHSE_COUNT]={NULL};
	SPACE_PTR SpacePtr[MAX_PAHSE_COUNT]={NULL};
	
	idx=0;
	ChkWndID = 0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{
		if ( NULL == m_MaskBuffer[i] ) { continue; }
		if ( NULL == m_SpaceBuffer[i] ) { continue; }
		if ( NULL == m_ImageBuffer[i] ) { continue; }
		switch ( i )
		{
		case 0:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK1;	break;
		case 1:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK2;	break;
		case 2:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK3;	break;
		case 3:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK4;	break;
		}
		if ( 0 != ChkWndID )
		{
			if ( CWnd::IsDlgButtonChecked(ChkWndID) == FALSE )
			{	continue; }
		}

		if ( idx == 0 ) 
		{
			ImageW    = m_ImageW[i];
			ImageH    = m_ImageH[i];
			PhaseStep = m_SpaceStep[i];
		}
		else
		{
			if ( ImageW != m_ImageW[i] ) { continue; }
			if ( ImageH != m_ImageH[i] ) { continue; }
			if ( PhaseStep != m_SpaceStep[i] ) { continue; }
		}		
		MaskPtr[idx] = m_MaskBuffer[i];
		ImagePtr[idx] = m_ImageBuffer[i];
		SpacePtr[idx] = m_SpaceBuffer[i];
		CastParam[idx] = m_CastParam[i];
		idx ++;
	}
	if ( idx < 1 ) { return; }		

	bool   bIsOK = false;	
	const  size_t DLPUseCount = idx;
	int    nDataVoidExpandSize = 1;
	int    nDataVoidExpandIterCount = 1;	
	size_t MaskBufferSize=PhaseStep*ImageH;		
	nDataVoidExpandSize = CWnd::GetDlgItemInt(ALLPHAS_VOID_EXPAND_EDIT);
	if ( nDataVoidExpandSize < 0 ) { nDataVoidExpandSize = 0; }	
	if ( nDataVoidExpandSize > 0 ) 
	{	nDataVoidExpandSize = nDataVoidExpandSize*2+1; }	
	
	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT1, str);
	double P1 = ::_ttof(str);
	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT2, str);
	double P2 = ::_ttof(str);

	TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	SystemParam.m_PhasePeriod1 = P1;
	SystemParam.m_PhasePeriod2 = P2;
	SystemParam.m_OpenMPCount_General = JetAPI::GetComboxCurSelData(m_OpenMPCombox);	

	SystemParam.m_PhaseNoiseDefineMode = JetAPI::GetComboxCurSelData(m_NoiseDefineModeCombox);
	SystemParam.m_SpaceNoiseSingleCastLowLimit = (int)(CWnd::GetDlgItemInt(ALLPHAS_SINGLE_CAST_OVER_LOW_EDIT));			
	SystemParam.m_SpaceNoiseMultiCastMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeModeCombox);
	SystemParam.m_SpaceNoiseMultiCastMergeBestMode = JetAPI::GetComboxCurSelData(m_SpaceMergeBestModeCombox);
	SystemParam.m_SpaceNoiseMultiIntensityMergeMode = JetAPI::GetComboxCurSelData(m_SpaceMergeIntensityModeCombox);	
	SystemParam.m_SpaceNoiseMultiCastPatchSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_MULTI_CAST_PATCH_SIZE_EDIT));
	SystemParam.m_SpaceNoiseMultiCastMinValidCount = (int)(CWnd::GetDlgItemInt(ALLPHAS_MULTI_CAST_MIN_VALID_COUNT_EDIT));
	SystemParam.m_SpaceNoiseMultiCastMaxDifference = (int)(CWnd::GetDlgItemInt(ALLPHAS_MULTI_CAST_MAX_DIFF_EDIT));
	SystemParam.m_SpaceNoiseMultiCastLimitDifference = (int)(CWnd::GetDlgItemInt(ALLPHAS_MULTI_CAST_LIMIT_DIFF_EDIT));
	//m_SpaceNoiseMultiCastValidBestRatio;//高度雜訊多投光高度最好比例-um	
	SystemParam.m_SpaceNoiseMultiCastValidDifference = (int)(CWnd::GetDlgItemInt(ALLPHAS_MULTI_CAST_VALID_DIFF_EDIT));	
	//m_SpaceNoiseMultiCastOppositeMaxGray;//高度雜訊多投光合併對邊灰階上限-gray

	SystemParam.m_SpaceMergeRecursionMode = JetAPI::GetComboxCurSelData(m_MergeRecursionModeCombox);
	SystemParam.m_SpaceMergeRecursionKernelSize = CWnd::GetDlgItemInt(ALLPHAS_MERGE_RECURSION_SIZE_EDIT);
	SystemParam.m_SpaceMergeRecursionMaxCount = CWnd::GetDlgItemInt(ALLPHAS_MERGE_RECURSION_COUNT_EDIT);
	SystemParam.m_SpaceMergeRecursionMaskSize = CWnd::GetDlgItemInt(ALLPHAS_MERGE_RECURSION_IGNORE_SIZE_EDIT);	

	int PhaseNoiseDef = 0;	
	TPhaseNoiseParam NoiseParam;
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	NoiseParam.PhaseNoiseDef = SystemParam.m_PhaseNoiseDefine;
	NoiseParam.PhaseNoiseDef = 0xFF;
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_SMOOTH_FILTER_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_SMOOTH_FILTER; }	
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_LOW_CONTRAST_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_CONTRAST; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_LOW_POTENTIAL_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_LOW_POTENTIAL; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_OVER_SATURATED_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_OVER_SATURATED; }
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_VOID_EXPAND_CHK) == TRUE )
	{	PhaseNoiseDef |= PHASE_NOSIE_VOID_EXPAND; }	
	NoiseParam.PhaseNoiseDef = PhaseNoiseDef;
	NoiseParam.PhaseNoiseDefMode = JetAPI::GetComboxCurSelData(m_NoiseDefineModeCombox);
	NoiseParam.PhaseLowContrastA = CWnd::GetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT);
	NoiseParam.PhaseOverSaturatedA = CWnd::GetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT);
	NoiseParam.PhaseLowPotentialA = CWnd::GetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT);
	NoiseParam.PhaseLowContrastB = CWnd::GetDlgItemInt(ALLPHAS_LOW_CONTRAST_EDIT2);
	NoiseParam.PhaseOverSaturatedB = CWnd::GetDlgItemInt(ALLPHAS_OVER_SATURATED_EDIT2);
	NoiseParam.PhaseLowPotentialB = CWnd::GetDlgItemInt(ALLPHAS_LOW_POTENTIAL_EDIT2);
	NoiseParam.PhaseExtendVoid = CWnd::GetDlgItemInt(ALLPHAS_VOID_EXPAND_EDIT);
	NoiseParam.PhaseSmoothFilter = CWnd::GetDlgItemInt(ALLPHAS_SMOOTH_FILTER_EDIT);	

	if ( CheckNeedReloadRawFile(NoiseParam) == true )
	{
		str = _T("Error, Some Param Changed, You have to reload all raw image files");
		JetAPI::ShowMessageBox(str);
		return;
	}	

	MASK_PTR  TmpMaskPtr[MAX_PAHSE_COUNT]={NULL};
	if ( 0<nDataVoidExpandSize && (PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND)!=0 )
	{
		for ( i=0; i<DLPUseCount; i++ )
		{
			if ( JetMemory.alloc_func(MaskBufferSize, TmpMaskPtr[i], fnName, "TempMaskPtr[i]") == false )
			{
				for ( j=0; j<i; j++ )
				{	JetMemory.free_func(TmpMaskPtr[j]);	}
				JetMemory.free_func(MaskPtrM);
				JetMemory.free_func(SpacePtrM);
				return;
			}			
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, PhaseStep, MaskPtr[i], nDataVoidExpandSize, nDataVoidExpandIterCount, TmpMaskPtr[i]) == false )
			{
				for ( j=0; j<=i; j++ )
				{	JetMemory.free_func(TmpMaskPtr[j]);	}
				JetMemory.free_func(MaskPtrM);
				JetMemory.free_func(SpacePtrM);
				return ;
			}
			MaskPtr[i] = TmpMaskPtr[i];
		}
	}	
	//
	
	bool  bOpenMP = true;
	const bool bCastMode = true;
	QueryPerformanceCounter(&nStartTime);
	if ( false == bCastMode ) 
	{
		switch ( DLPUseCount )
		{
		case 1: 
			bIsOK = true;
			if ( ImageAPI.CloneGrayImage(ImageW, ImageH, PhaseStep, MaskPtr[0], MaskPtrM, false) == false ||
				 ImageAPI.CloneSpaceGrayImage(ImageW, ImageH, PhaseStep, SpacePtr[0], SpacePtrM, false) == false )
			{
				bIsOK = false;
				JetMemory.free_func(MaskPtrM);
				JetMemory.free_func(SpacePtrM);
			}		
			break;
		case 2:	bIsOK = ImageAPI.Merge2SpaceImage(ImageW, ImageH, PhaseStep, SpacePtr[0], MaskPtr[0], SpacePtr[1], MaskPtr[1], bOpenMP, SpacePtrM, MaskPtrM);	break;
		case 3: bIsOK = ImageAPI.Merge3SpaceImage(ImageW, ImageH, PhaseStep, SpacePtr[0], MaskPtr[0], SpacePtr[1], MaskPtr[1], SpacePtr[2], MaskPtr[2], bOpenMP, SpacePtrM, MaskPtrM);	break;
		case 4:	bIsOK = ImageAPI.Merge4SpaceImage(ImageW, ImageH, PhaseStep, SpacePtr[0], MaskPtr[0], SpacePtr[1], MaskPtr[1], SpacePtr[2], MaskPtr[2], SpacePtr[3], MaskPtr[3], bOpenMP, SpacePtrM, MaskPtrM);	break;
		}
	}
	else
	{
		if ( 1 == DLPUseCount ) 
		{
			if ( 2 == CastParam[0].ExpCount )
			{	bIsOK = ImageAPI.PatternCast1ToSpace2Exp(ImageW, ImageH, PhaseStep, CastParam[0], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
			else
			{	bIsOK = ImageAPI.PatternCast1ToSpace(ImageW, ImageH, PhaseStep, CastParam[0], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
		}
		else if ( 2 == DLPUseCount ) 
		{	
			if ( 2 == CastParam[0].ExpCount )
			{	bIsOK = ImageAPI.PatternCast2ToSpace2Exp(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
			else
			{	bIsOK = ImageAPI.PatternCast2ToSpace(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
		}
		else if ( 3 == DLPUseCount ) 
		{
			if ( 2 == CastParam[0].ExpCount )
			{	bIsOK = ImageAPI.PatternCast3ToSpace2Exp(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
			else
			{	bIsOK = ImageAPI.PatternCast3ToSpace(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
		}
		else if ( 4 == DLPUseCount ) 
		{	
			if ( 2 == CastParam[0].ExpCount )
			{	bIsOK = ImageAPI.PatternCast4ToSpace2Exp(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
			else
			{	bIsOK = ImageAPI.PatternCast4ToSpace(ImageW, ImageH, PhaseStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, MaskPtrM, SpacePtrM);	}
		}
	}
	m_PhaseNoiseParam=NoiseParam;

	QueryPerformanceCounter(&nEndTime);
	double Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
	str.Format(_T("Merge Time = %.4f ms"), Time);
	CWnd::SetDlgItemText(ALLPHAS_MERGE_TIME_EDIT, str);	
	
	if ( true == bIsOK )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("MergeSpace.Z3D"));
		ImageAPI.SaveSpaceGrayImage(str, ImageW, ImageH, PhaseStep, SpacePtrM, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("MergeMask.PNG"));
		ImageAPI.SavePNGGrayImage(str, ImageW, ImageH, PhaseStep, MaskPtrM, true);


		bool bDebugFImg=false;
		if ( true == bDebugFImg )
		{
			IMAGE_SIZE W2=0;
			IMAGE_SIZE H2=0;
			IMAGE_SIZE Step2=0;
			IMAGE_SIZE BitCount2=0;
			float     *FloatPtr2=NULL;
			CString aName=_T("R:\\Debug.fImg");
			if (ImageAPI.LoadGrayFloatFile(aName, W2, H2, Step2, BitCount2, FloatPtr2, false, 4) == true )
			{
				if ( W2!=ImageW || H2!=ImageH || Step2!=PhaseStep )
				{	str = _T("Error, Test Fault");	}
				else
				{
					float Err=0.0f;
					float HInput=0.0f;
					float HOutput=0.0f;
					size_t CntErr = 0;
					float MaxErr=0.0f;					
					const size_t Size=Step2*H2;
					FILE *pfile = ::_tfopen(_T("R:\\CheckList.TXT"), _T("w+"));
					if ( NULL != pfile )
					{	::_ftprintf(pfile, _T("Index, %s, JOE, Err\n"), AOI3D_APP_NAME);	}
					for ( i=0; i<Size; i++ )
					{
						HInput = FloatPtr2[i];
						HOutput = SpacePtrM[i]/10.0f;
						Err=::fabs(HOutput-HInput);
						if ( Err < 10.0f ) { continue; }
						if ( MaxErr < Err ) { MaxErr = Err; }
						CntErr ++;		
						::_ftprintf(pfile, _T("%d, %.4f, %.4f, %.4f\n"), i, HOutput, HInput, Err);
					}
					::fclose(pfile); pfile = NULL;
					str.Format(_T("Error Count=%d, Max Err=%.2f"), CntErr, MaxErr);
				}
				JetAPI::ShowMessageBox(str);
				JetMemory.free_func(FloatPtr2);
			}

			bool bDebugHeight=false;
			if ( true == bDebugHeight )
			{	
				float Err=0.0f;
				float HInput=0.0f;
				float HOutput=0.0f;				
				for ( int cc=0; cc<4; cc++ )
				{					
					aName.Format(_T("R:\\H%d.fImg"), cc+1);
					str.Format(_T("R:\\HeightCheckList_%d.TXT"), cc+1);
					if (ImageAPI.LoadGrayFloatFile(aName, W2, H2, Step2, BitCount2, FloatPtr2, false, 4) == true )
					{
						if ( W2!=ImageW || H2!=ImageH || Step2!=PhaseStep )
						{	continue; }

						bool MyDbug=false;
						FILE *pfile = NULL;
						const size_t Size=Step2*H2;
						const TCastParam &CastParamRef=CastParam[cc];						
						if ( FN_DISABLE == SystemParam.m_CudaFnEnabled )
						{	pfile = ::_tfopen(str, _T("w+"));	}
						if ( NULL != pfile )
						{	::_ftprintf(pfile, _T("Index, %s, JOE, Err\n"), AOI3D_APP_NAME);	}
						for ( i=0; i<Size; i++ )
						{
							MyDbug=false;
							if ( 1076859==i || 8763527==i || 8480904==i || 1128132== i )
							{	MyDbug = true; }

							if ( true == MyDbug )
							{	MyDbug = MyDbug; }

							HInput = FloatPtr2[i];
							HOutput = CastParamRef.PtrSpace[i]/10.0f;
							Err=::fabs(HOutput-HInput);
							if ( Err < 0.1f ) { continue; }
							if ( NULL != pfile )
							{	::_ftprintf(pfile, _T("%d, %.4f, %.4f, %.4f\n"), i, HOutput, HInput, Err); }							
						}
						if ( NULL != pfile )
						{	::fclose(pfile); pfile = NULL;	}
						JetMemory.free_func(FloatPtr2);
					}
				}
			}
		}
	}
	if ( NULL != SpacePtrM )
	{		
		/*
		CString tmpName=_T("C:\\3DData_Merge.DAT");
		FILE *tmpFile=::_tfopen(tmpName, _T("rb"));
		if ( NULL != tmpFile )
		{				
			size_t tmpDifMaxIdx=0;
			float  tmpDif=0;
			float  tmpDifAbs=0;
			float  tmpDifAbsMax=0;			
			float  tmpHeight=0.0f;
			size_t tmpRes=0;
			size_t tmpX=0, tmpY=0;
			size_t tmpSize=ImageH*ImageW;
			float *tmpSpace=NULL;
			std::vector<int> IndexList;
			if ( JetMemory.alloc_func(tmpSize, tmpSpace, "fnName", "tmpSpace") == true )
			{
				tmpRes = ::fread(tmpSpace, sizeof(float), tmpSize, tmpFile);				
				for ( i=0; i<PhaseStep*ImageH; i++ )
				{
					//tmpHeight = tmpSpace[i]*PHASE_MAX/PHASE_HEIGHT_FACTOR_SCALE;
					tmpHeight = tmpSpace[i];
					tmpDif = SpacePtrM[i]-tmpHeight;
					tmpDifAbs = ::fabs(tmpDif);
					if ( tmpDifAbsMax<tmpDifAbs )
					{	
						tmpDifAbsMax = tmpDifAbs; 
						tmpDifMaxIdx = i;
					}					

					if ( tmpDifAbs > 10 )
					{
						tmpX = i%ImageW;
						tmpY = i/ImageW;
						tmpDifAbs = tmpDifAbs;
						IndexList.push_back((int)i);
					}
				}
			}
			JetMemory.free_func(tmpSpace);
			::fclose(tmpFile);
			tmpFile = NULL;
		}//*/

		SPACE_DATA  tmpH=0;
		int tmpX=0, tmpY=0;
		for ( i=0; i<PhaseStep*ImageH; i++ )
		{
			tmpX = i%PhaseStep;
			tmpY = i/PhaseStep;
			if ( 2240==tmpX && 584==tmpY )
			{
				i = i;
				tmpH = SpacePtrM[i];
			}
		}
	}
	
	
#ifdef _DEBUG
	CString    DebugFolder;
	const bool bSave3DDebug=false;
	DebugFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("MergeSpace"));
	::CreateDirectory(DebugFolder, NULL); ::Sleep(0);	
	if ( true == bSave3DDebug )
	{
		str.Format(_T("%s\\%s"), DebugFolder, _T("RawMask.BMP"));
		ImageAPI.SaveImage(str, ImageW, ImageH, PhaseStep, 8, MaskPtrM, true);

		FILE *pfile=NULL;
		str.Format(_T("%s\\%s"), DebugFolder, _T("RawSpace.DAT"));
		pfile = ::_tfopen(str, _T("wb"));
		if ( NULL != pfile )
		{
			::fwrite(SpacePtrM, sizeof(SPACE_DATA)*PhaseStep*ImageH, 1, pfile);
			::fclose(pfile); pfile=NULL;
		}		
	}
#endif//_DEBUG
	for ( i=0; i<DLPUseCount; i++ )
	{	
		//MaskPtr[i] = NULL;
		JetMemory.free_func(TmpMaskPtr[i]); 
	}
	if ( false == bIsOK ) 
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);
		return;		
	}	
	switch ( DLPUseCount )
	{
	case 1:	bIsOK = ImageAPI.CloneGrayImage(ImageW, ImageH, PhaseStep, ImagePtr[0], ImagePtrM, false);	break;
	case 2:	bIsOK = ImageAPI.MergeGrayImage2Frame(ImageW, ImageH, PhaseStep, ImagePtr[0], ImagePtr[1], ImagePtrM);	break;
	case 3: bIsOK = ImageAPI.MergeGrayImage3Frame(ImageW, ImageH, PhaseStep, ImagePtr[0], ImagePtr[1], ImagePtr[2], ImagePtrM);	break;
	case 4:	bIsOK = ImageAPI.MergeGrayImage4Frame(ImageW, ImageH, PhaseStep, ImagePtr[0], ImagePtr[1], ImagePtr[2], ImagePtr[3], ImagePtrM);	break;
	}	
	if ( false == bIsOK ) 
	{ 		
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);
		return; 
	}
#ifdef _DEBUG
	if ( NULL != ImagePtr[0] )
	{
		str.Format(_T("%s\\%s"), DebugFolder, _T("PhaseImage1.PNG"));
		ImageAPI.SaveImage(str, ImageW, ImageH, PhaseStep, 8, ImagePtr[0], true);
	}
	if ( NULL != ImagePtr[1] )
	{
		str.Format(_T("%s\\%s"), DebugFolder, _T("PhaseImage2.PNG"));
		ImageAPI.SaveImage(str, ImageW, ImageH, PhaseStep, 8, ImagePtr[1], true);
	}
	str.Format(_T("%s\\%s"), DebugFolder, _T("PhaseImageM.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, PhaseStep, 8, ImagePtrM, true);

	str.Format(_T("%s\\%s"), DebugFolder, _T("Space3D.M3D"));
	ImageAPI.SaveSpaceGrayImage(str, ImageW, ImageH, PhaseStep, SpacePtrM, false);	

	RECT MyRoi;
	MyRoi.left = 0; MyRoi.top = 0;
	MyRoi.right = ImageW;
	MyRoi.bottom = ImageH;
	//ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, PhaseStep, SpacePtrM, MaskPtrM, MyRoi, PhaseStep, ImagePtrM, 50, false);
	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SpaceImageM.PNG"));
	//ImageAPI.SaveImage(str, ImageW, ImageH, PhaseStep, 8, ImagePtrM, true);
#endif//_DEBUG	
	RECT         RoiRect={0};	
	RECT         WndRect = m_ImageWndRect1;	
	MASK_PTR     RoiMaskPtr=NULL;
	MASK_PTR     RoiImagePtr=NULL;
	SPACE_PTR    RoiSpacePtr=NULL;	
	SPACE_PTR    RoiSpace2Ptr=NULL;	
	
	RoiRect.left   = JetAPI::Floor(MIN(m_ImagePt1.x, m_ImagePt2.x));
	RoiRect.top    = JetAPI::Floor(MIN(m_ImagePt1.y, m_ImagePt2.y));
	RoiRect.right  = JetAPI::Floor(MAX(m_ImagePt1.x, m_ImagePt2.x));
	RoiRect.bottom = JetAPI::Floor(MAX(m_ImagePt1.y, m_ImagePt2.y));
	const IMAGE_SIZE RoiPhaseStep = JetAPI::GetBMPImagePixelsPerLine(RoiRect.right-RoiRect.left, 8, 4);
	RoiRect.right = RoiRect.left + RoiPhaseStep;
	const int RoiImageW = RoiRect.right-RoiRect.left;
	const int RoiImageH = RoiRect.bottom-RoiRect.top;
	if ( RoiRect.left>ImageW || RoiRect.top>ImageH || RoiRect.right<0 || RoiRect.bottom<0 || RoiImageW<=0 || RoiImageH<=0 ) 
	{ 
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(ImagePtrM);
		JetMemory.free_func(SpacePtrM);
		return; 
	}	
	if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtrM, RoiRect, RoiPhaseStep, RoiMaskPtr, false) == false || 
		 ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, ImagePtrM, RoiRect, RoiPhaseStep, RoiImagePtr, false) == false || 
		 ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtrM, RoiRect, RoiPhaseStep, RoiSpacePtr, false) == false ||
		 ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtrM, RoiRect, RoiPhaseStep, RoiSpace2Ptr, false) == false )
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(ImagePtrM);
		JetMemory.free_func(SpacePtrM);
		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiImagePtr);
		JetMemory.free_func(RoiSpacePtr);
		JetMemory.free_func(RoiSpace2Ptr);
		return; 
	}

#ifdef _DEBUG	
	if ( true == bSave3DDebug )
	{
		//輸出ROI的高度與遮罩圖
		MASK_PTR  TmpMaskPtr=NULL;
		SPACE_PTR TmpSpacePtr=NULL;
		const size_t RoiSize=RoiPhaseStep*RoiImageH; 
		if ( 4 == DLPUseCount )
		{			
			int DlpIdx=0;			
			if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtr[DlpIdx], RoiRect, RoiPhaseStep, TmpMaskPtr, false) == true ) 
			{
				for ( i=0; i<RoiSize; i++ )
				{	
					if ( 0 == TmpMaskPtr[i] ) { TmpMaskPtr[i] = 0xFF; }
					else { TmpMaskPtr[i] = 0x00; }
				}
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-01-RoiMask.PNG"));
				ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiPhaseStep, 8, TmpMaskPtr, true);	
				JetMemory.free_func(TmpMaskPtr);
			}
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtr[DlpIdx], RoiRect, RoiPhaseStep, TmpSpacePtr, false) == true )
			{
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-01-RoiSpace.DAT"));
				SaveKValeFile_JET6500(str, RoiImageW, RoiImageH, RoiPhaseStep, TmpSpacePtr);
				JetMemory.free_func(TmpSpacePtr);
			}
			DlpIdx = 1;
			if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtr[DlpIdx], RoiRect, RoiPhaseStep, TmpMaskPtr, false) == true ) 
			{
				for ( i=0; i<RoiSize; i++ )
				{	
					if ( 0 == TmpMaskPtr[i] ) { TmpMaskPtr[i] = 0xFF; }
					else { TmpMaskPtr[i] = 0x00; }
				}
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-02-RoiMask.PNG"));
				ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiPhaseStep, 8, TmpMaskPtr, true);	
				JetMemory.free_func(TmpMaskPtr);
			}
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtr[DlpIdx], RoiRect, RoiPhaseStep, TmpSpacePtr, false) == true )
			{
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-02-RoiSpace.DAT"));
				SaveKValeFile_JET6500(str, RoiImageW, RoiImageH, RoiPhaseStep, TmpSpacePtr);
				JetMemory.free_func(TmpSpacePtr);
			}
			DlpIdx = 2;
			if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtr[DlpIdx], RoiRect, RoiPhaseStep, TmpMaskPtr, false) == true ) 
			{
				for ( i=0; i<RoiSize; i++ )
				{	
					if ( 0 == TmpMaskPtr[i] ) { TmpMaskPtr[i] = 0xFF; }
					else { TmpMaskPtr[i] = 0x00; }
				}
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-03-RoiMask.PNG"));
				ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiPhaseStep, 8, TmpMaskPtr, true);	
				JetMemory.free_func(TmpMaskPtr);
			}
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtr[DlpIdx], RoiRect, RoiPhaseStep, TmpSpacePtr, false) == true )
			{	
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-03-RoiSpace.DAT"));
				SaveKValeFile_JET6500(str, RoiImageW, RoiImageH, RoiPhaseStep, TmpSpacePtr);
				JetMemory.free_func(TmpSpacePtr);
			}
			DlpIdx = 3;
			if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtr[DlpIdx], RoiRect, RoiPhaseStep, TmpMaskPtr, false) == true ) 
			{
				for ( i=0; i<RoiSize; i++ )
				{	
					if ( 0 == TmpMaskPtr[i] ) { TmpMaskPtr[i] = 0xFF; }
					else { TmpMaskPtr[i] = 0x00; }
				}
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-04-RoiMask.PNG"));
				ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiPhaseStep, 8, TmpMaskPtr, true);	
				JetMemory.free_func(TmpMaskPtr);
			}
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtr[DlpIdx], RoiRect, RoiPhaseStep, TmpSpacePtr, false) == true )
			{
				str.Format(_T("%s\\%s"), DebugFolder, _T("DLP-04-Space.DAT"));
				SaveKValeFile_JET6500(str, RoiImageW, RoiImageH, RoiPhaseStep, TmpSpacePtr);
				JetMemory.free_func(TmpSpacePtr);
			}
		}
		if ( JetMemory.alloc_func(RoiSize, TmpMaskPtr, fnName, "TmpMaskPtr") == true )
		{
			for ( i=0; i<RoiSize; i++ )
			{	
				if ( 0 == RoiMaskPtr[i] ) { TmpMaskPtr[i] = 0xFF; }
				else { TmpMaskPtr[i] = 0x00; }
			}
			str.Format(_T("%s\\%s"), DebugFolder, _T("Merge-RoiMask.PNG"));		
			ImageAPI.SaveImage(str, RoiImageW, RoiImageH, RoiPhaseStep, 8, TmpMaskPtr, true);
			JetMemory.free_func(TmpMaskPtr);
		}
		str.Format(_T("%s\\%s"), DebugFolder, _T("Merge-RoiSpace.DAT"));
		SaveKValeFile_JET6500(str, RoiImageW, RoiImageH, RoiPhaseStep, RoiSpacePtr);		
	}
#endif//_DEBUG		

	RECT              TagRect={0};
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const BOOL bEnhance = CWnd::IsDlgButtonChecked(IDC_ENHANCE_IMAGE_CHK);
	const BOOL bMaskImage = CWnd::IsDlgButtonChecked(IDC_SPACE_MASK_IMAGE_CHK);
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const double Gain = SysParam.m_ImageDisplayGain;	
	if ( TRUE == bEnhance )
	{	ImageAPI.GrayImageOffsetGain3(RoiImageW, RoiImageH, RoiPhaseStep, RoiImagePtr, RoiImagePtr, 0, Gain); }
	else
	{	ImageAPI.GrayImageOffsetGain3(RoiImageW, RoiImageH, RoiPhaseStep, RoiImagePtr, RoiImagePtr, 0, 1.0); }
	
	BOOL bShift = CWnd::IsDlgButtonChecked(ALLPHAS_NF_HEIGHT_UNEXPECTED_FILTER_SHIFT_CHK);
	if ( TRUE == bShift ) 
	{ 
		AOIDataCollect.GetSystemParameter().m_LevelFilterShiftEnabled = FN_ENABLE;
		AOIDataCollect.GetSystemParameter().m_MedianFilterShiftEnabled = FN_ENABLE;
		AOIDataCollect.GetSystemParameter().m_SmoothFilterShiftEnabled = FN_ENABLE;
		AOIDataCollect.GetSystemParameter().m_PyramidMedianFilterShiftEnabled = FN_ENABLE;
	}
	else 
	{ 
		AOIDataCollect.GetSystemParameter().m_LevelFilterShiftEnabled = FN_DISABLE;
		AOIDataCollect.GetSystemParameter().m_MedianFilterShiftEnabled = FN_DISABLE;
		AOIDataCollect.GetSystemParameter().m_SmoothFilterShiftEnabled = FN_DISABLE;		
		AOIDataCollect.GetSystemParameter().m_PyramidMedianFilterShiftEnabled = FN_DISABLE;
	}
	TNoiseFilterParam NoiseFilterParam = m_NoiseFilterParam;
	UpdateSpaceNoiseFilterParamFromUI(NoiseFilterParam);	
	m_NoiseFilterParam = NoiseFilterParam;

	MASK_PTR Mask2DPtr = NULL;
	QueryPerformanceCounter(&nStartTime);	
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, RoiImageW*RoiImageH);	
	if ( ImageAPI.BuildSpaceData3(RoiImageW, RoiImageH, RoiPhaseStep, RoiSpacePtr, RoiMaskPtr, Mask2DPtr, nOpenMPCnt, NoiseFilterParam, RoiSpace2Ptr, RoiMaskPtr, RoiImagePtr) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());

		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiImagePtr);
		JetMemory.free_func(RoiSpacePtr);

		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(ImagePtrM);
		JetMemory.free_func(SpacePtrM);
		JetMemory.free_func(RoiSpace2Ptr);
		return;
	}

	/*
	CString tmpName2=_T("C:\\3D Filter.DAT");
	FILE *tmpFile2=::_tfopen(tmpName2, _T("rb"));
	if ( NULL != tmpFile2 )
	{				
		size_t tmpDifMaxIdx=0;
		float  tmpDif=0;
		float  tmpDifAbs=0;
		float  tmpDifAbsMax=0;			
		float  tmpHeight=0.0f;		
		size_t tmpRes=0;
		size_t tmpX=0, tmpY=0;
		size_t tmpSize=RoiImageH*RoiImageW;
		size_t tmpSize2=RoiImageH*RoiPhaseStep;
		float *tmpSpace=NULL;
		std::vector<int> IndexList;
		if ( JetMemory.alloc_func(tmpSize, tmpSpace, "fnName", "tmpSpace") == true )
		{
			tmpRes = ::fread(tmpSpace, sizeof(float), tmpSize, tmpFile2);
			if ( tmpRes==tmpSize && tmpSize==tmpSize2 )
			{
				for ( i=0; i<tmpSize; i++ )
				{	
					tmpHeight = tmpSpace[i];
					tmpDif = RoiSpace2Ptr[i]-tmpHeight;
					tmpDifAbs = ::fabs(tmpDif);
					if ( tmpDifAbsMax<tmpDifAbs )
					{	
						tmpDifAbsMax = tmpDifAbs; 
						tmpDifMaxIdx = i;
					}					
					if ( tmpDifAbs > 1000 )
					{
						tmpX = i%RoiPhaseStep;
						tmpY = i/RoiPhaseStep;
						tmpDifAbs = tmpDifAbs;
						IndexList.push_back((int)i);
					}
				}								
			}
		}
		JetMemory.free_func(tmpSpace);
		::fclose(tmpFile2);
		tmpFile2 = NULL;
	}//*/

	QueryPerformanceCounter(&nEndTime);
	str.Format(_T("%.3f"), NoiseFilterParam.DataVoidExpandSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_VOID_EXPAND_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataFirstFilterSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_FIRST_FILTER_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataOverLowSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_OVER_LOW_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataHeightFTSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_HEIGHT_UNEXPECTED_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataVoidRecontructedSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_VOID_RECONTRUCT_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataFinalFilterSpentTime);
	CWnd::SetDlgItemText(ALLPHAS_NF_FINAL_FILTER_TIME_EDIT, str);

	str.Format(_T("%.3f"), NoiseFilterParam.DataFinalFilterSpentTime2);
	CWnd::SetDlgItemText(ALLPHAS_NF_FINAL_FILTER_TIME_EDIT2, str);

	Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;//ms
	str.Format(_T("Total Spent Time: %.3fms"), Time);
	CWnd::SetDlgItemText(ALLPHAS_NF_INFO_EDIT, str);

	//m_PhaseWnd.SetSpaceBuffer(ImageW, ImageH, PhaseStep, MaskPtrM, SpacePtrM, PHASE_TO_IMAGE_FIXED_SCALE, TRUE);	//PHASE_TO_IMAGE_FIXED_SCALE, PHASE_TO_IMAGE_DYNAMIC_SCALE
	m_PhaseWnd.SetStartPos(RoiRect.left, RoiRect.top);
	m_PhaseWnd.SetSpaceBuffer(RoiImageW, RoiImageH, RoiPhaseStep, RoiMaskPtr, RoiSpacePtr, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	
	//m_PhaseWnd.SetSpaceBuffer(RoiImageW, RoiImageH, RoiPhaseStep, RoiMaskPtr, SpacePtrM, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);	
	//m_Draw3DWnd.Set3DData(SpacePtrM, ImagePtrM, ImageW, ImageH, PhaseStep, false, -1, -1, -1, -1, TagRect, TagRect, 0);	

	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;
	if ( TRUE == bMaskImage )
	{
		IMAGE_PTR  MaskClrPtr = NULL;
		IMAGE_SIZE MaskClrStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, 24, 4);
		if ( ImageAPI.SpaceGrayImageConvertToColor(RoiImageW, RoiImageH, RoiPhaseStep, RoiSpace2Ptr, RoiMaskPtr, MaskClrStep, MaskClrPtr, SpaceRatio, false) == true )
		{
			m_Draw3DWnd.Set3DData(RoiSpace2Ptr, MaskClrPtr, RoiImageW, RoiImageH, RoiPhaseStep, true, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, TagRect, 0);
			JetMemory.free_func(MaskClrPtr); MaskClrPtr = NULL;
		}
	}
	else
	{	m_Draw3DWnd.Set3DData(RoiSpace2Ptr, RoiImagePtr, RoiImageW, RoiImageH, RoiPhaseStep, false, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, TagRect, TagRect, 0); }
	//m_OpenGLWnd.SetModalCenter(false);
	m_Draw3DWnd.ShowWindow(SW_SHOW);	
	if ( m_PhaseWnd.IsWindowVisible() == TRUE )
	{	m_PhaseWnd.RedrawWnd();	}
	else
	{	m_PhaseWnd.ShowWindow(SW_SHOW); }
	
	const bool KeepRoi=false;
	if ( true == KeepRoi )
	{
		this->m_ImageW_M = RoiImageW;
		this->m_ImageH_M = RoiImageH;	
		this->m_SpaceStep_M = RoiPhaseStep;
		this->m_MaskBuffer_M = RoiMaskPtr;
		this->m_ImageBuffer_M = RoiImagePtr;	
		this->m_SpaceBuffer_M = RoiSpacePtr;	

		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(ImagePtrM);
		JetMemory.free_func(SpacePtrM);
		JetMemory.free_func(RoiSpace2Ptr);
	}
	else
	{
		this->m_ImageW_M = ImageW;
		this->m_ImageH_M = ImageH;	
		this->m_SpaceStep_M = PhaseStep;
		this->m_MaskBuffer_M = MaskPtrM;
		this->m_ImageBuffer_M = ImagePtrM;	
		this->m_SpaceBuffer_M = SpacePtrM;	

		JetMemory.free_func(RoiMaskPtr);
		JetMemory.free_func(RoiImagePtr);
		JetMemory.free_func(RoiSpacePtr);
		JetMemory.free_func(RoiSpace2Ptr);
	}

	//ALLPHAS_NF_INFO_EDIT
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::UpdateSpaceNoiseFilterParamToUI(const TNoiseFilterParam &NoiseFilterParam)
{
	//無效點外擴//pixel 
	CWnd::CheckDlgButton(ALLPHAS_NF_VOID_EXPAND_CHK, NoiseFilterParam.DataVoidExpandEnabled);	
	CWnd::SetDlgItemInt(ALLPHAS_NF_VOID_EXPAND_SIZE_EDIT, NoiseFilterParam.DataVoidExpandSize);

	//首次處理//pixel	
	JetAPI::SetComboxCurSel(m_FirstFilterModeCombox, NoiseFilterParam.DataFirstFilterMode);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_SIZE_EDIT, NoiseFilterParam.DataFirstFilterKerSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_PITCH_EDIT, NoiseFilterParam.DataFirstFilterPitch);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_USE_SIZE_EDIT, NoiseFilterParam.DataFirstFilterUseSize);	

	//高度過低判定//um
	JetAPI::SetComboxCurSel(m_OverLowModeCombox, NoiseFilterParam.DataOverLowFTMode);
	CWnd::SetDlgItemInt(ALLPHAS_NF_OVER_LOW_RANGE_EDIT, NoiseFilterParam.DataOverLowFTRange);
	CWnd::SetDlgItemInt(ALLPHAS_NF_OVER_LOW_LIMIT_EDIT, NoiseFilterParam.DataOverLowFTLimit);	
	CWnd::SetDlgItemInt(ALLPHAS_NF_OVER_LOW_KER_SIZE_EDIT, NoiseFilterParam.DataOverLowFTKerSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_OVER_LOW_USE_SIZE_EDIT, NoiseFilterParam.DataOverLowFTUseSize);	

	//高度異常判定//um		
	JetAPI::SetComboxCurSel(m_HeightUnexpectedModeCombox, NoiseFilterParam.DataHeightFTMode);	
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_PITCH_EDIT, NoiseFilterParam.DataHeightFTPitch);
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_KER_SIZE_EDIT, NoiseFilterParam.DataHeightFTKerSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_USE_SIZE_EDIT, NoiseFilterParam.DataHeightFTUseSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT, NoiseFilterParam.DataHeightFTChkSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_RANGED_EDIT, NoiseFilterParam.DataHeightFTRange);
	CWnd::SetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT, NoiseFilterParam.DataHeightFTRepeatCnt);		
	
	//雜訊重建//um
	CWnd::CheckDlgButton(ALLPHAS_NF_VOID_RECONTRUCT_CHK, NoiseFilterParam.DataVoidReContructed);
	CWnd::SetDlgItemInt(ALLPHAS_NF_VOID_RECONTRUCT_SIZE_EDIT, NoiseFilterParam.DataVoidReContructedExtSize);
	//最末處理//pixel	
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox, NoiseFilterParam.DataFinalFilterMode);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_SIZE_EDIT, NoiseFilterParam.DataFinalFilterKerSize);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_PITCH_EDIT, NoiseFilterParam.DataFinalFilterPitch);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_USE_SIZE_EDIT, NoiseFilterParam.DataFinalFilterUseSize);	

	//最末處理-2//pixel	
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox2, NoiseFilterParam.DataFinalFilterMode2);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_SIZE_EDIT2, NoiseFilterParam.DataFinalFilterKerSize2);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_PITCH_EDIT2, NoiseFilterParam.DataFinalFilterPitch2);
	CWnd::SetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_USE_SIZE_EDIT2, NoiseFilterParam.DataFinalFilterUseSize2);	
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::UpdateSpaceNoiseFilterParamFromUI(TNoiseFilterParam &NoiseFilterParam) const
{
	//無效點外擴
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_NF_VOID_EXPAND_CHK) == TRUE )
	{	NoiseFilterParam.DataVoidExpandEnabled = true; }
	else
	{	NoiseFilterParam.DataVoidExpandEnabled = false; }		
	//NoiseFilterParam.DataVoidExpandEnabled = false;
	NoiseFilterParam.DataVoidExpandSize = CWnd::GetDlgItemInt(ALLPHAS_NF_VOID_EXPAND_SIZE_EDIT);
	NoiseFilterParam.DataVoidExpandIterCount = 1;

	//首次處理	
	NoiseFilterParam.DataFirstFilterMode = JetAPI::GetComboxCurSelData(m_FirstFilterModeCombox);
	NoiseFilterParam.DataFirstFilterPitch = CWnd::GetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_PITCH_EDIT);	
	NoiseFilterParam.DataFirstFilterKerSize = CWnd::GetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_SIZE_EDIT);	
	NoiseFilterParam.DataFirstFilterUseSize = CWnd::GetDlgItemInt(ALLPHAS_NF_FIRST_FILTER_USE_SIZE_EDIT);

	//高度過低判定	
	NoiseFilterParam.DataOverLowFTMode = JetAPI::GetComboxCurSelData(m_OverLowModeCombox);		
	NoiseFilterParam.DataOverLowFTRange = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_OVER_LOW_RANGE_EDIT));
	NoiseFilterParam.DataOverLowFTLimit = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_OVER_LOW_LIMIT_EDIT));
	NoiseFilterParam.DataOverLowFTKerSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_OVER_LOW_KER_SIZE_EDIT));
	NoiseFilterParam.DataOverLowFTUseSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_OVER_LOW_USE_SIZE_EDIT));		

	//高度異常判定			
	NoiseFilterParam.DataHeightFTMode = JetAPI::GetComboxCurSelData(m_HeightUnexpectedModeCombox);
	NoiseFilterParam.DataHeightFTPitch = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_PITCH_EDIT));
	NoiseFilterParam.DataHeightFTKerSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_KER_SIZE_EDIT));
	NoiseFilterParam.DataHeightFTUseSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_USE_SIZE_EDIT));
	NoiseFilterParam.DataHeightFTChkSize = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_CHK_SIZE_EDIT));
	NoiseFilterParam.DataHeightFTRange = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_RANGED_EDIT));	
	NoiseFilterParam.DataHeightFTRepeatCnt = (int)(CWnd::GetDlgItemInt(ALLPHAS_NF_HEIGHT_UNEXPECTED_REPEAT_CNT_EDIT));

	//雜訊重建
	if ( CWnd::IsDlgButtonChecked(ALLPHAS_NF_VOID_RECONTRUCT_CHK) == TRUE )
	{	NoiseFilterParam.DataVoidReContructed = true; }
	else
	{	NoiseFilterParam.DataVoidReContructed = false; }	
	NoiseFilterParam.DataVoidReContructedExtSize = CWnd::GetDlgItemInt(ALLPHAS_NF_VOID_RECONTRUCT_SIZE_EDIT);

	//最末處理	
	NoiseFilterParam.DataFinalFilterMode = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox);
	NoiseFilterParam.DataFinalFilterPitch = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_PITCH_EDIT);
	NoiseFilterParam.DataFinalFilterKerSize = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_SIZE_EDIT);	
	NoiseFilterParam.DataFinalFilterUseSize = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_USE_SIZE_EDIT);	

	//最末處理-2
	NoiseFilterParam.DataFinalFilterMode2 = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox2);
	NoiseFilterParam.DataFinalFilterPitch2 = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_PITCH_EDIT2);
	NoiseFilterParam.DataFinalFilterKerSize2 = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_SIZE_EDIT2);
	NoiseFilterParam.DataFinalFilterUseSize2 = CWnd::GetDlgItemInt(ALLPHAS_NF_FINAL_FILTER_USE_SIZE_EDIT2);	
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::ExecMergePhase()//合併相同投光的相位資料
{
	CString str;
	size_t i=0, idx=0;
	double P1=16.0, P2=20.0, P=0.0;
	IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;		
	MASK_PTR  MaskPtrM1=NULL;
	PHASE_PTR PhasePtrM1=NULL;
	MASK_PTR  MaskPtrM2=NULL;
	PHASE_PTR PhasePtrM2=NULL;
	MASK_PTR  MaskPtr1=NULL;
	MASK_PTR  MaskPtr2=NULL;
	PHASE_PTR PhasePtr1=NULL;
	PHASE_PTR PhasePtr2=NULL;
	IMAGE_SIZE  PhaseW1=0, PhaseH1=0, PhaseStep1=0;
	IMAGE_SIZE  PhaseW2=0, PhaseH2=0, PhaseStep2=0;
	const int CombinePeriodMode = AOIDataCollect.GetPhaseCombinePeriodMode();
	idx=0;
	MaskPtr1 = m_MaskBuffer[0];
	PhasePtr1 = m_PhaseBuffer[0];
	MaskPtr2 = m_MaskBuffer[1];
	PhasePtr2 = m_PhaseBuffer[1];
	PhaseW1   = m_ImageW[0];
	PhaseH1   = m_ImageH[0];
	PhaseStep1= m_PhaseStep[0]; 
	PhaseW2   = m_ImageW[1];
	PhaseH2   = m_ImageH[1];
	PhaseStep2= m_PhaseStep[1]; 
	
	if ( NULL==MaskPtr1 || NULL==MaskPtr2 || NULL==PhasePtr1 || NULL==PhasePtr2 ) { return; }
	if ( PhaseW1!=PhaseW2 || PhaseH1!=PhaseH2 || PhaseStep1!=PhaseStep2 ) { return; }

	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT1, str);
	P1 = ::_tcstod(str, NULL);
	this->GetDlgItemText(ALLPHAS_PERIODE_EDIT2, str);
	P2 = ::_tcstod(str, NULL);

	PhaseW = PhaseW1;
	PhaseH = PhaseH1;
	PhaseStep = PhaseStep1;
	const size_t PhaseBufferSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	if ( ImageAPI.PhaseImage2PeriodeToPhase(PhaseW, PhaseH, PhaseStep, PhasePtr1, MaskPtr1, P1, PhasePtr2, MaskPtr2, P2, NULL, PhasePtrM1, MaskPtrM1, P, CombinePeriodMode) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}

	//清除3, 4的資料
	CImagePhaseAllWnd::ReleaseBuffer(2);
	CImagePhaseAllWnd::ReleaseBuffer(3);
	m_PhaseFileName[2] = _T("");
	m_PhaseFileName[3] = _T("");

	idx = 2;
	m_ImageW[idx] = PhaseW;
	m_ImageH[idx] = PhaseH;
	m_PhaseStep[idx] = PhaseStep;
	m_MaskBuffer[idx] = MaskPtrM1;
	m_PhaseBuffer[idx] = PhasePtrM1;
	const IMAGE_SIZE BitCount2 = 24;
	const IMAGE_SIZE ImageStep2 = JetAPI::GetBMPImagePixelsPerLine(PhaseW, BitCount2, 4);
	m_BitCount[idx] = 24;
	m_ShowStep[idx] = ImageStep2;
	ImageAPI.PhaseGrayImageConvertToColor(PhaseW, PhaseH, PhaseStep, m_PhaseBuffer[idx], m_MaskBuffer[idx], ImageStep2, m_ShowBuffer[idx], false);
	CImagePhaseAllWnd::DrawImageWndMemDC(idx);
	
	//修改成以短周期為主
	int    nPeriod=0, nPeriodT=0;
	double dPeriod=0.0;
	double dPeriod1=0.0, dPeriod2=0.0;
	int    nPeriod1=0, nPeriod2=0;
	double dRatio1=0, dRatio2=0;
	int    Phase1T=0, Phase2T=0;
	int    Phase1M=0, Phase2M=0;
	int    Phase1=0, Phase2=0, PhaseM=0, PhaseMT=0;	
	int    PhaseM12=0, PhaseM13=0, PhaseM14=0, PhaseM15=0;
	int    PhaseM22=0, PhaseM23=0, PhaseM24=0, PhaseM25=0;		
	const int PhaseShift = 0;//PHASE_PERIOD*3/4;//(int)(PHASE_PERIOD*0.13);	
	const double PP1 = __min(P1, P2);
	const double PP2 = __max(P1, P2);
	if ( P2 > P1 )//P2 為主
	{	
		dRatio2 = PP1/(PP2-PP1);	
		dRatio1 = PP2/(PP2-PP1);	
	}
	else //P1為主 
	{	
		dRatio2 = PP2/(PP2-PP1);	
		dRatio1 = PP1/(PP2-PP1);	
	}
	dRatio1 = dRatio1*1;
	dRatio2 = dRatio2*1;
	const int PhaseDummy = PHASE_PERIOD_HALF;//增加特定周期
	const int FullPeriodM1 = (int)(dRatio1*PHASE_PERIOD);//以第1週期為主的大週期
	const int FullPeriodM2 = (int)(dRatio2*PHASE_PERIOD);//以第2週期為主的大週期

	if ( ImageAPI.PhaseImage2PeriodeToPhase(PhaseW, PhaseH, PhaseStep, PhasePtr1, MaskPtr1, P1, PhasePtr2, MaskPtr2, P2, NULL, PhasePtrM2, MaskPtrM2, P, CombinePeriodMode) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}
	
	size_t x = 1856;
	size_t y = 1865;
	size_t xyIdx = y*PhaseStep+x;
	for ( i=0; i<PhaseBufferSize; i++ )
	{
		if ( i == xyIdx )
		{
			i = i;
		}
		Phase1 = PhasePtr1[i];
		Phase2 = PhasePtr2[i];
		PhaseM = PhasePtrM2[i];
		if ( Phase2 > Phase1 )
		{	PhaseM = static_cast<int>(Phase1-Phase2+PHASE_PERIOD);	}
		else
		{	PhaseM = Phase1-Phase2;	}

		PhaseMT = PhaseM;		
		Phase1T=(int)(PhaseM*dRatio1);
		Phase2T=(int)(PhaseM*dRatio2);

		Phase1M = Phase1T;// - PHASE_PERIOD_HALF;
		if ( Phase1M < 0 ) { Phase1M += PHASE_PERIOD; }
		Phase2M = Phase2T;// - PHASE_PERIOD_HALF;
		if ( Phase2M < 0 ) { Phase2M += PHASE_PERIOD; }
		//if ( Phase1M < Phase1 )
		//if ( Phase2M < Phase2 ) { Phase2M += PHASE_PERIOD; }
		dPeriod1 = ((Phase1M-Phase1+PhaseDummy+0.0)/PHASE_PERIOD);
		dPeriod2 = ((Phase2M-Phase2+PhaseDummy+0.0)/PHASE_PERIOD);
		nPeriod1 = JetAPI::Floor(dPeriod1);
		nPeriod2 = JetAPI::Floor(dPeriod2);
		if ( nPeriod1 < 0 ) 
		{	nPeriod1 = nPeriod1; }
		PhaseM13 = Phase1+(nPeriod1*PHASE_PERIOD);
		PhaseM23 = Phase2+(nPeriod2*PHASE_PERIOD);
		if ( PhaseM13 < 0 ) 
		{	PhaseM13 += FullPeriodM1; }
		if ( PhaseM13 > FullPeriodM1 ) 
		{	PhaseM13 -= FullPeriodM1; } 
		if ( PhaseM23 < 0 ) 
		{	PhaseM23 += FullPeriodM2; }
		if ( PhaseM23 > FullPeriodM2 ) 
		{	PhaseM23 -= FullPeriodM2; } 
		PhaseM14 = (int)(PhaseM13/dRatio1);
		PhaseM24 = (int)(PhaseM23/dRatio2);

		/*
		PhaseMT = PhaseM;
		PhaseM12 = (int)(PhaseMT*dRatio1);

		nPeriodT  = (PhaseM12-Phase1T+PhaseDummy+0.0)/PHASE_PERIOD;
		dPeriod = (PhaseM12-Phase1T+PhaseDummy+0.0)/PHASE_PERIOD;
		dPeriod = (PhaseM12)/PHASE_PERIOD;
		nPeriod = (int)(dPeriod+1000)-1000;
		
		PhaseM13 = Phase1T+(nPeriod*PHASE_PERIOD);				
		PhaseM14  = (int)(PhaseM13/dRatio1);		
		PhaseM15 = PhaseM12-PhaseM13;
		if ( nPeriod != nPeriodT )
		{
			nPeriod = nPeriod;
		}
		//if ( PhaseM15 < 0 ) 
		if ( PhaseM12 < Phase2T )
		{
		//	MaskPtrM2[i] = PHASE_MASK_OVER_SATURATED;
		//	PhaseM14 = PHASE_PERIOD_HALF;//PHASE_PERIOD/2;
		}
		*/

		//PhaseM22 = (int)(PhaseMT*dRatio2);		
		//PhaseM23 = Phase1+(nPeriod2*PHASE_PERIOD);
		//PhaseM24  = (int)(PhaseM23/dRatio2);
		PhaseM  = PhaseM14;
		PhasePtrM2[i] = static_cast<PHASE_DATA>(PhaseM);
	}
	
	idx = 3;
	m_ImageW[idx] = PhaseW;
	m_ImageH[idx] = PhaseH;
	m_PhaseStep[idx] = PhaseStep;
	m_MaskBuffer[idx] = MaskPtrM2;
	m_PhaseBuffer[idx] = PhasePtrM2;	
	m_BitCount[idx] = 24;
	m_ShowStep[idx] = ImageStep2;
	ImageAPI.PhaseGrayImageConvertToColor(PhaseW, PhaseH, PhaseStep, m_PhaseBuffer[idx], m_MaskBuffer[idx], ImageStep2, m_ShowBuffer[idx], false);
	CImagePhaseAllWnd::DrawImageWndMemDC(idx);

	str.Format(_T("%s\\%s.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseImageM2"));
	ImageAPI.SaveBMPImage(str, PhaseW, PhaseH, ImageStep2, 24, m_ShowBuffer[idx], true);	

	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnShowImageRadio() 
{
	// TODO: Add your control notification handler code here	
	size_t i=0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{	CImagePhaseAllWnd::UpdateShowImage(IDC_SHOW_IMAGE_RADIO, i); }	
	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnShowPhaseRadio() 
{
	// TODO: Add your control notification handler code here	
	size_t i=0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{	CImagePhaseAllWnd::UpdateShowImage(IDC_SHOW_PHASE_RADIO, i); }		
	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnShowSpaceRadio() 
{
	// TODO: Add your control notification handler code here
	size_t i=0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{	CImagePhaseAllWnd::UpdateShowImage(IDC_SHOW_SPACE_RADIO, i); }
	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::UpdateShowImage(UINT ID, size_t idx)
{
	m_ShowModeID = ID;
	if ( idx >= MAX_PAHSE_COUNT ) { return; }
	size_t DataStep = 0;
	const IMAGE_SIZE ImageW = m_ImageW[idx];
	const IMAGE_SIZE ImageH = m_ImageH[idx];
	const IMAGE_SIZE ShowStep = m_ShowStep[idx];	
	const double SpaceRatio = -1;//AOIDataCollect.GetSpaceToGrayRatio();
	IMAGE_PTR ShowBuffer = m_ShowBuffer[idx];
	if ( NULL == ShowBuffer ) { return; }

	BOOL bEnhance = CWnd::IsDlgButtonChecked(IDC_ENHANCE_IMAGE_CHK);
	switch ( m_ShowModeID )
	{
	case IDC_SHOW_IMAGE_RADIO:
		DataStep = m_ImageStep[idx];
		ImageAPI.RGBImageToColorImage3(ImageW, ImageH, m_ImageStep[idx], m_ImageBuffer[idx], m_ImageBuffer[idx], m_ImageBuffer[idx], ShowStep, ShowBuffer, false);
		if ( TRUE == bEnhance )
		{	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, 24, ShowBuffer, ShowBuffer); }
		break;
	case IDC_SHOW_PHASE_RADIO:
		DataStep = m_PhaseStep[idx];
		ImageAPI.PhaseGrayImageConvertToColor3(ImageW, ImageH, m_PhaseStep[idx], m_PhaseBuffer[idx], m_MaskBuffer[idx], ShowStep, m_ShowBuffer[idx], false);		
		break;
	case IDC_SHOW_SPACE_RADIO:
		DataStep = m_SpaceStep[idx];
		ImageAPI.SpaceGrayImageConvertToColor3(ImageW, ImageH, m_SpaceStep[idx], m_SpaceBuffer[idx], m_MaskBuffer[idx], ShowStep, m_ShowBuffer[idx], SpaceRatio, false);//將相位影像轉成空間相對位置		
		break;
	}
	CImagePhaseAllWnd::DrawImageWndMemDC(idx);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnEnhanceImageChk() 
{
	// TODO: Add your control notification handler code here
	size_t i=0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{	CImagePhaseAllWnd::UpdateShowImage(IDC_SHOW_IMAGE_RADIO, i); }	
	CImagePhaseAllWnd::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnCompareSpaceBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[] = "CImagePhaseAllWnd::OnCompareSpaceBtn()";	

	CString    str;
	UINT       ChkWndID=0;
	size_t     i=0, j=0, idx=0;	
	IMAGE_SIZE ImageW=0, ImageH=0, PhaseStep=0;
	MASK_PTR  MaskPtr[MAX_PAHSE_COUNT]={NULL};
	IMAGE_PTR ImagePtr[MAX_PAHSE_COUNT]={NULL};
	SPACE_PTR SpacePtr[MAX_PAHSE_COUNT]={NULL};
	
	idx=0;
	ChkWndID = 0;
	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{
		if ( NULL == m_MaskBuffer[i] ) { continue; }
		if ( NULL == m_SpaceBuffer[i] ) { continue; }
		if ( NULL == m_ImageBuffer[i] ) { continue; }
		switch ( i )
		{
		case 0:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK1;	break;
		case 1:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK2;	break;
		case 2:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK3;	break;
		case 3:	ChkWndID = ALLPHAS_ENABLE_PHASE_CHK4;	break;
		}
		if ( 0 != ChkWndID )
		{
			if ( CWnd::IsDlgButtonChecked(ChkWndID) == FALSE )
			{	continue; }
		}

		if ( idx == 0 ) 
		{
			ImageW    = m_ImageW[i];
			ImageH    = m_ImageH[i];
			PhaseStep = m_SpaceStep[i];
		}
		else
		{
			if ( ImageW != m_ImageW[i] ) { continue; }
			if ( ImageH != m_ImageH[i] ) { continue; }
			if ( PhaseStep != m_SpaceStep[i] ) { continue; }
		}
		MaskPtr[idx] = m_MaskBuffer[i];
		ImagePtr[idx] = m_ImageBuffer[i];
		SpacePtr[idx] = m_SpaceBuffer[i];
		idx ++;
	}
	if ( idx < 2 ) { return; }	
	
	RECT         RoiRect={0};	
	RECT         WndRect = m_ImageWndRect1;	
	MASK_PTR     MaskPtrM=NULL;
	SPACE_PTR    SpacePtrM=NULL;
	MASK_PTR     RoiMaskPtr[MAX_PAHSE_COUNT]={NULL};	
	SPACE_PTR    RoiSpacePtr[MAX_PAHSE_COUNT]={NULL};			
	RoiRect.left   = JetAPI::Floor(MIN(m_ImagePt1.x, m_ImagePt2.x));
	RoiRect.top    = JetAPI::Floor(MIN(m_ImagePt1.y, m_ImagePt2.y));
	RoiRect.right  = JetAPI::Floor(MAX(m_ImagePt1.x, m_ImagePt2.x));
	RoiRect.bottom = JetAPI::Floor(MAX(m_ImagePt1.y, m_ImagePt2.y));

//	RoiRect.left = 0;
//	RoiRect.top  = 0;
//	RoiRect.right = ImageW;
//	RoiRect.bottom = ImageH;

	const IMAGE_SIZE RoiPhaseStep = JetAPI::GetBMPImagePixelsPerLine(RoiRect.right-RoiRect.left, 8, 4);
	RoiRect.right = RoiRect.left + RoiPhaseStep;
	const int RoiImageW = RoiRect.right-RoiRect.left;
	const int RoiImageH = RoiRect.bottom-RoiRect.top;
	if ( RoiRect.left>ImageW || RoiRect.top>ImageH || RoiRect.right<0 || RoiRect.bottom<0 || RoiImageW<=0 || RoiImageH<=0 ) 
	{	return;	}	

	for ( i=0; i<idx; i++ )
	{
		if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, PhaseStep, MaskPtr[i], RoiRect, RoiPhaseStep, RoiMaskPtr[i], false) == false || 			 			 
			 ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, PhaseStep, SpacePtr[i], RoiRect, RoiPhaseStep, RoiSpacePtr[i], false) == false )
		{
			for ( j=0; j<idx; j++ )
			{
				JetMemory.free_func(RoiMaskPtr[j]);
				JetMemory.free_func(RoiSpacePtr[j]);			
			}
			return; 
		}
	}

	RECT              TagRect={0};		
	const size_t RoiBufferSize = RoiPhaseStep*RoiImageH;
	if ( JetMemory.alloc_func(RoiBufferSize, MaskPtrM, fnName, "MaskPtrM") == false ||
		 JetMemory.alloc_func(RoiBufferSize, SpacePtrM, fnName, "SpacePtrM") == false )
	{
		JetMemory.free_func(MaskPtrM);
		JetMemory.free_func(SpacePtrM);
		for ( j=0; j<idx; j++ )
		{
			JetMemory.free_func(RoiMaskPtr[j]);
			JetMemory.free_func(RoiSpacePtr[j]);			
		}
		return; 
	}
	
	unsigned int SpaceCount = 0;
	double MaxSpace=0, MinSpace=0, DifSpace=0;
	double TotalMax=0, TotalAve=0, TotalSum=0;
	for ( j=0; j<RoiBufferSize; j++ )
	{
		MaxSpace = -FLT_MAX;
		MinSpace =  FLT_MAX;		
		for ( i=0; i<idx; i++ )
		{
			if ( (RoiMaskPtr[i][j]&PHASE_MASK_LOW_CONTRAST) != NULL ) 
			{ 
				SpacePtrM[j] = 0;
				MaskPtrM[j] = PHASE_MASK_LOW_CONTRAST;
				break;
			}
			if ( (RoiMaskPtr[i][j]&PHASE_MASK_LOW_POTENTIAL) != NULL ) 
			{ 
				SpacePtrM[j] = 0;
				MaskPtrM[j] = PHASE_MASK_LOW_POTENTIAL;
				break;
			}
			if ( (RoiMaskPtr[i][j]&PHASE_MASK_OVER_SATURATED) != NULL ) 
			{
				SpacePtrM[j] = 0;
				MaskPtrM[j] = PHASE_MASK_OVER_SATURATED;
				break;
			}

			if ( RoiSpacePtr[i][j] > MaxSpace ) { MaxSpace = RoiSpacePtr[i][j]; }
			if ( RoiSpacePtr[i][j] < MinSpace ) { MinSpace = RoiSpacePtr[i][j]; }
		}
		if ( i < idx ) { continue; }
		MaskPtrM[j] = PHASE_MASK_VALID;
		DifSpace = MaxSpace-MinSpace;		
		SpacePtrM[j]  = (SPACE_DATA)(DifSpace);	

		if ( DifSpace > TotalMax ) { TotalMax = DifSpace; }
		TotalSum += DifSpace;
		SpaceCount ++;
	}
	m_PhaseWnd.SetStartPos(RoiRect.left, RoiRect.top);
	m_PhaseWnd.SetSpaceBuffer(RoiImageW, RoiImageH, RoiPhaseStep, MaskPtrM, SpacePtrM, PHASE_TO_IMAGE_DYNAMIC_SCALE, TRUE);			
	if ( m_PhaseWnd.IsWindowVisible() == TRUE )
	{	m_PhaseWnd.RedrawWnd();	}
	else
	{	m_PhaseWnd.ShowWindow(SW_SHOW); }

	
	if ( SpaceCount > 0 ) 
	{
		TotalAve = TotalSum/SpaceCount;
		str.Format(_T("Space difference range (Max:%.2f, Ave:%.2f)"), TotalMax, TotalAve);
		JetAPI::ShowMessageBox(str);
	}

	JetMemory.free_func(MaskPtrM);
	JetMemory.free_func(SpacePtrM);
	for ( j=0; j<idx; j++ )
	{
		JetMemory.free_func(RoiMaskPtr[j]);
		JetMemory.free_func(RoiSpacePtr[j]);			
	}
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnNFDisableBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = FALSE;
	
	JetAPI::SetComboxCurSel(m_FirstFilterModeCombox, DATA_NF_DISABLE);
	JetAPI::SetComboxCurSel(m_OverLowModeCombox, DATA_NF_DISABLE);	
	JetAPI::SetComboxCurSel(m_HeightUnexpectedModeCombox, DATA_NF_DISABLE);
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox, DATA_NF_DISABLE);	
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox2, DATA_NF_DISABLE);		
	CWnd::CheckDlgButton(ALLPHAS_NF_VOID_EXPAND_CHK, bCheck);		
	CWnd::CheckDlgButton(ALLPHAS_NF_VOID_RECONTRUCT_CHK, bCheck);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::ExecPhaseCompare_JET6500()
{
	const char fnName[] = "CImagePhaseAllWnd::ExecPhaseCompare_JET6500";
	TCHAR szFilters[]=_T("DAT Files (*.DAT)|*.DAT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("DAT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	size_t           i=0;
	CString          str;	
	CString          strLabelW;
	CString          strLabelH;
	CString          strValueW;
	CString          strValueH;
	CString          strCaption;
	CInputBoxWnd     InputBox; 
	CString          filename=dialog.GetPathName();
	IMAGE_PTR        ImagePtr=NULL;
	float           *SpacePtr1 = NULL;
	float           *SpacePtr2 = NULL;
	CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	strLabelW = _T("Image W");
	strLabelH = _T("Image H");
	strValueW.Format(_T("%d"), ImageW);
	strValueH.Format(_T("%d"), ImageH);
	InputBox.SetParam2(strCaption, strLabelW, strValueW, strLabelH, strValueH);	
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	strValueW = InputBox.m_DataEdit1;
	strValueH = InputBox.m_DataEdit2;
	ImageW = ::_ttoi(strValueW);
	ImageH = ::_ttoi(strValueH);

	const size_t BufferSize=ImageW*ImageH;
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false ||
		 JetMemory.alloc_func(BufferSize, SpacePtr1, fnName, "SpacePtr1") == false ||
		 JetMemory.alloc_func(BufferSize, SpacePtr2, fnName, "SpacePtr2") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);
		JetMemory.free_func(ImagePtr);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}
	::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	::memset(SpacePtr1, 0x00, sizeof(float)*BufferSize);
	::memset(SpacePtr2, 0x00, sizeof(float)*BufferSize);

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("rb"));
	if ( NULL == pfile )
	{
		JetMemory.free_func(ImagePtr);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}
	::fread(SpacePtr1, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile);	pfile = NULL;
	
	CFileDialog dialog2 (TRUE, _T("DAT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog2.DoModal() == IDCANCEL )
	{
		JetMemory.free_func(ImagePtr);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false; 
	}

	filename=dialog2.GetPathName();
	pfile = ::_tfopen(filename, _T("rb"));
	if ( NULL == pfile )
	{
		JetMemory.free_func(ImagePtr);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}
	::fread(SpacePtr2, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile);	pfile = NULL;


	float v1=0;
	float v2=0;
	float val=0;
	int   nval=0;
	IMAGE_SIZE ImageX=0;
	IMAGE_SIZE ImageY=0;
	for ( i=0; i<BufferSize; i++ )
	{
	#ifdef _DEBUG
		ImageX = i%ImageW;
		ImageY = i/ImageW;
		if ( 37==ImageX && 165==ImageY )
		{	i = i; }
	#endif//_DEBUG
		v1 = SpacePtr1[i];
		v2 = SpacePtr2[i];
		val = fabs(v1-v2);

		nval = JetAPI::Ceil(val);		
		if ( nval > 255 ) { nval = 255; }

	#ifdef _DEBUG
		if ( nval > 1 ) 
		{	i = i; }
	#endif//_DEBUG
		ImagePtr[i]=(IMAGE_DATA)(nval);
	}

	filename.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("PhaseCmp"));
	ImageAPI.SaveImage(filename, ImageW, ImageH, ImageW, 8, ImagePtr, true);

	JetMemory.free_func(ImagePtr);
	JetMemory.free_func(SpacePtr1);
	JetMemory.free_func(SpacePtr2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::ExecConvertPhaseFactor_JET6500()
{
	const char fnName[] = "CImagePhaseAllWnd::ExecConvertPhaseFactor_JET6500";
	TCHAR szFilters[]=_T("DAT Files (*.DAT)|*.DAT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("DAT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	size_t           i=0;
	CString          str;	
	CString          strLabelW;
	CString          strLabelH;
	CString          strValueW;
	CString          strValueH;
	CString          strCaption;
	CInputBoxWnd     InputBox; 
	CString          filename=dialog.GetPathName();
	float           *SpacePtr = NULL;
	CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	strLabelW = _T("Image W");
	strLabelH = _T("Image H");
	strValueW.Format(_T("%d"), ImageW);
	strValueH.Format(_T("%d"), ImageH);
	InputBox.SetParam2(strCaption, strLabelW, strValueW, strLabelH, strValueH);	
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	strValueW = InputBox.m_DataEdit1;
	strValueH = InputBox.m_DataEdit2;
	ImageW = ::_ttoi(strValueW);
	ImageH = ::_ttoi(strValueH);

	const size_t BufferSize=ImageW*ImageH;
	if ( JetMemory.alloc_func(BufferSize, SpacePtr, fnName, "SpacePtr") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	::memset(SpacePtr, 0x00, sizeof(float)*BufferSize);

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("rb"));
	if ( NULL == pfile )
	{
		JetMemory.free_func(SpacePtr);
		return false;
	}
	::fread(SpacePtr, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile);	pfile = NULL;

	//轉單位
	float val=0;
	//const float fScale=32767.0f/PHASE_HEIGHT_FACTOR_SCALE;
	const float fScale=PHASE_HEIGHT_FACTOR_SCALE/32767.0f;
	for ( i=0; i<BufferSize; i++ )
	{
		val = SpacePtr[i];
		val = val*fScale;
		SpacePtr[i] = val;
	}
	

	IMAGE_SIZE ImageWSys = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageHSys = AOIDataCollect.GetCameraImageH(CameraID);
	LIGHT_3D_CLS_PTR Light3DPtr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_01);
	if ( NULL != Light3DPtr && ImageW==ImageWSys && ImageH==ImageHSys )
	{
		int CastPhaseMode = LIGHT3D_PHASE_4_4_M;
		int DLPLEDColorTmp = DLP_LED_COLOR_DEBUG;
		Light3DPtr->SetPhaseFactor(CastPhaseMode, DLPLEDColorTmp, ImageW, ImageH, ImageW, SpacePtr);
		Light3DPtr->SavePhaseFactor();
	}

	JetMemory.free_func(SpacePtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnConvertPhaseFactorBtn() 
{
	// TODO: Add your control notification handler code here
	ExecConvertPhaseFactor_JET6500();
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::SaveKValeFile_JET6500(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, float *Ptr)
{
	if ( NULL == Ptr ) { return false; }

	FILE *pfile = NULL;
	pfile = _tfopen(filename, _T("wb"));
	if ( NULL == pfile ) { return false; }

	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	::fwrite(Ptr, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::SavePhaseFile_JET6500(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, short *Ptr)
{
	if ( NULL == Ptr ) { return false; }

	FILE *pfile = NULL;
	pfile = _tfopen(filename, _T("wb"));
	if ( NULL == pfile ) { return false; }

	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	::fwrite(Ptr, sizeof(short)*BufferSize, 1, pfile);
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::SavePhaseImage_Debug(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, PHASE_PTR Ptr)
{
	if ( NULL == Ptr ) { return false; }

	size_t i = 0;
	double  val=0;
	float  *ValPtr=NULL;	
	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	if ( JetMemory.alloc_func(BufferSize, ValPtr, "CImagePhaseAllWnd::SavePhaseImage_Debug", "ValPtr") == false )
	{	return false; }

	::memset(ValPtr, 0x00, sizeof(float)*BufferSize);
	for ( i=0; i<BufferSize; i++ )
	{
		val = Ptr[i];
		val = 100.0*val/PHASE_PERIOD;
		//val = 100.0*val/PHASE_MAX;		
		ValPtr[i] = (float)(val);
	}


	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("wb"));
	if ( NULL == pfile )
	{
		JetMemory.free_func(ValPtr);
		return false;
	}
	::fwrite(ValPtr, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile);	pfile = NULL;

	JetMemory.free_func(ValPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::SavePhaseMask_Debug(LPCTSTR filename, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR Ptr, int Mask)
{
	if ( NULL == Ptr ) { return false; }

	size_t i = 0;
	IMAGE_PTR ImagePtr = NULL;
	const size_t BufferSize = MaskStep*MaskH;
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CImagePhaseAllWnd::SavePhaseMask_Debug", "ImagePtr") == false )
	{	return false; }
	
	for ( i=0; i<BufferSize; i++ )
	{
		if ( JetAPI::BitMask_Check(Ptr[i], Mask) == false )
		{	ImagePtr[i] = 0;	}
		else
		{	ImagePtr[i] = 0xFF;	}
	}
	ImageAPI.SaveImage(filename, MaskW, MaskH, MaskStep, 8, ImagePtr, true);
	JetMemory.free_func(ImagePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::CreateMedianFilterSample_Debug()
{
	const char fnName[] = "CImagePhaseAllWnd::CreateMedianFilterSample_Debug";

	int              val=0;	
	CString          str;	
	CString          strLabelW;
	CString          strLabelH;
	CString          strValueW;
	CString          strValueH;
	CString          strCaption;
	CInputBoxWnd     InputBox; 
	CString          filename;
	size_t           i=0;	
	IMAGE_PTR        MaskPtr1=NULL;	
	float           *SpacePtr1 = NULL;	
	IMAGE_SIZE ImageW = 32;
	IMAGE_SIZE ImageH = 32;

	strLabelW = _T("Image W");
	strLabelH = _T("Image H");
	strValueW.Format(_T("%d"), ImageW);
	strValueH.Format(_T("%d"), ImageH);
	InputBox.SetParam2(strCaption, strLabelW, strValueW, strLabelH, strValueH);	
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	strValueW = InputBox.m_DataEdit1;
	strValueH = InputBox.m_DataEdit2;
	ImageW = ::_ttoi(strValueW);
	ImageH = ::_ttoi(strValueH);

	const size_t BufferSize=ImageW*ImageH;
	if ( JetMemory.alloc_func(BufferSize, MaskPtr1, fnName, "MaskPtr1") == false ||		 		 
		 JetMemory.alloc_func(BufferSize, SpacePtr1, fnName, "SpacePtr1") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);		
		JetMemory.free_func(MaskPtr1);		
		JetMemory.free_func(SpacePtr1);		
		return false;
	}
	val=0;
	for ( i=0; i<BufferSize; i++ )
	{
		MaskPtr1[i] = val;
		SpacePtr1[i] = val;
		val ++;
		if ( val > 255 ) 
		{	val -= 255; }
	}

	filename.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("MedianSample"));
	ImageAPI.SaveImage(filename, ImageW, ImageH, ImageW, 8, MaskPtr1, true);

	filename.Format(_T("%s\\%s.DAT"), AOIDataCollect.GetAOITempDirectory(), _T("MedianSample"));
	FILE *pfile = ::_tfopen(filename, _T("wb"));
	if ( NULL != pfile )
	{
		::fwrite(SpacePtr1, sizeof(float)*BufferSize, 1, pfile);
		::fclose(pfile);	pfile = NULL;
	}
	JetMemory.free_func(MaskPtr1);		
	JetMemory.free_func(SpacePtr1);
	return true;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnPhaseCompareBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecPhaseCompare_JET6500();
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::ExecMedianFilter_Debug()
{
	const char fnName[] = "CImagePhaseAllWnd::ExecMedianFilter_Debug";
	TCHAR szFilters[]=_T("DAT Files (*.DAT)|*.DAT|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("DAT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return true; }

	size_t           i=0;
	CString          str;	
	CString          strLabelW;
	CString          strLabelH;
	CString          strValueW;
	CString          strValueH;
	CString          strCaption;
	CInputBoxWnd     InputBox; 
	CString          filename=dialog.GetPathName();	
	IMAGE_PTR        MaskPtr1=NULL;
	IMAGE_PTR        MaskPtr2=NULL;
	IMAGE_PTR        GuidedImagePtr=NULL;	
	float           *SpacePtr1 = NULL;
	float           *SpacePtr2 = NULL;
	CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	strLabelW = _T("Image W");
	strLabelH = _T("Image H");
	strValueW.Format(_T("%d"), ImageW);
	strValueH.Format(_T("%d"), ImageH);
	InputBox.SetParam2(strCaption, strLabelW, strValueW, strLabelH, strValueH);	
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	strValueW = InputBox.m_DataEdit1;
	strValueH = InputBox.m_DataEdit2;
	ImageW = ::_ttoi(strValueW);
	ImageH = ::_ttoi(strValueH);

	const size_t BufferSize=ImageW*ImageH;
	if ( JetMemory.alloc_func(BufferSize, MaskPtr1, fnName, "MaskPtr1") == false ||
		 JetMemory.alloc_func(BufferSize, MaskPtr2, fnName, "MaskPtr2") == false ||
		 JetMemory.alloc_func(BufferSize, SpacePtr1, fnName, "SpacePtr1") == false ||
		 JetMemory.alloc_func(BufferSize, SpacePtr2, fnName, "SpacePtr2") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);		
		JetMemory.free_func(MaskPtr1);
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}
	
	::memset(SpacePtr1, 0x00, sizeof(float)*BufferSize);
	::memset(SpacePtr2, 0x00, sizeof(float)*BufferSize);

	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("rb"));
	if ( NULL == pfile )
	{		
		JetMemory.free_func(MaskPtr1);
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}
	::fread(SpacePtr1, sizeof(float)*BufferSize, 1, pfile);
	::fclose(pfile);	pfile = NULL;
	
	bool               bOpenMP=true;
	TImageFilterParam  FilterParam;
	FilterParam.FilterSize = 13;
	FilterParam.FilterUseSize = 1;
	FilterParam.FilterIterCount = 1;
	FilterParam.FilterMode = DATA_NF_MEDIAN;
	AOIDataCollect.GetSystemParameter().m_OpenMPCount_General = JetAPI::GetComboxCurSelData(m_OpenMPCombox);

	::memset(MaskPtr1, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(MaskPtr2, 0xFF, sizeof(MASK_DATA)*BufferSize);
	if ( ImageAPI.RefineSpaceImage3(ImageW, ImageH, ImageW, SpacePtr1, MaskPtr1, MaskPtr2, bOpenMP, FilterParam, SpacePtr2, GuidedImagePtr) == false )
	{
		JetMemory.free_func(MaskPtr1);
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(SpacePtr1);
		JetMemory.free_func(SpacePtr2);
		return false;
	}

	float val = 0.0f;
	for ( i=0; i<BufferSize; i++ )
	{
		val = SpacePtr1[i];
		if ( val > 255.0f ) { val = 255.0f; }
		MaskPtr1[i] = (unsigned char)(val);

		val = SpacePtr2[i];
		if ( val > 255.0f ) { val = 255.0f; }
		MaskPtr2[i] = (unsigned char)(val);
	}

	filename.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("MedianSrc"));
	ImageAPI.SaveImage(filename, ImageW, ImageH, ImageW, 8, MaskPtr1, true);

	filename.Format(_T("%s\\%s.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("MedianDst"));
	ImageAPI.SaveImage(filename, ImageW, ImageH, ImageW, 8, MaskPtr2, true);

	filename.Format(_T("%s\\%s.DAT"), AOIDataCollect.GetAOITempDirectory(), _T("MedianDst"));
	pfile = ::_tfopen(filename, _T("wb"));
	if ( NULL != pfile )
	{
		::fwrite(SpacePtr2, sizeof(float)*BufferSize, 1, pfile);
		::fclose(pfile);	pfile = NULL;
	}	
	
	JetMemory.free_func(MaskPtr1);
	JetMemory.free_func(MaskPtr2);
	JetMemory.free_func(SpacePtr1);
	JetMemory.free_func(SpacePtr2);
	return true;
	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnMedianDebugBtn() 
{
	// TODO: Add your control notification handler code here
	ExecMedianFilter_Debug();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnUseCudaChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(ALLPHAS_USE_CUDA_CHK);
	AOIDataCollect.GetSystemParameter().m_CudaFnEnabled = bChk;	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLockRectBtn() 
{
	// TODO: Add your control notification handler code here	
	const int Index=0;
	if ( NULL == m_ImageBuffer[Index] ) { return; }
	const int ImagePtX1 = (int)(CWnd::GetDlgItemInt(ALLPHAS_LOCK_RECT_X_EDIT1));
	const int ImagePtY1 = (int)(CWnd::GetDlgItemInt(ALLPHAS_LOCK_RECT_Y_EDIT1));
	const int ImagePtX2 = (int)(CWnd::GetDlgItemInt(ALLPHAS_LOCK_RECT_X_EDIT2));
	const int ImagePtY2 = (int)(CWnd::GetDlgItemInt(ALLPHAS_LOCK_RECT_Y_EDIT2));

	SaveRectParam();
	m_ImagePt1.x = ImagePtX1;
	m_ImagePt1.y = ImagePtY1;
	m_ImagePt2.x = ImagePtX2;
	m_ImagePt2.y = ImagePtY2;	
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[Index], m_ImageH[Index], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImagePt1, m_ImageWndPt1);
	ImageAPI.MapImagePtToWndPt_DBL(m_ImageW[Index], m_ImageH[Index], m_ImageWndRect1, m_ImageOffset, m_ImageZoom, m_ImagePt2, m_ImageWndPt2);
	UpdateInfoEdit(ALLPHAS_INFO_EDIT2, m_ImagePt1); 
	UpdateInfoEdit(ALLPHAS_INFO_EDIT3, m_ImagePt2); 
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::SaveRectParam()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString Default;	
	CString Section=_T("IDD_IMAGE_PHASE_ALL_WND");	
	
	WndID = ALLPHAS_LOCK_RECT_X_EDIT1;
	WndKey = _T("ALLPHAS_LOCK_RECT_X_EDIT1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = ALLPHAS_LOCK_RECT_Y_EDIT1;
	WndKey = _T("ALLPHAS_LOCK_RECT_Y_EDIT1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = ALLPHAS_LOCK_RECT_X_EDIT2;
	WndKey = _T("ALLPHAS_LOCK_RECT_X_EDIT2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	

	WndID = ALLPHAS_LOCK_RECT_Y_EDIT2;
	WndKey = _T("ALLPHAS_LOCK_RECT_Y_EDIT2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.SaveWndUIParam(Section, WndKey, Default);	
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::LoadRectParam()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CString String;
	CString Default;
	CString Section=_T("IDD_IMAGE_PHASE_ALL_WND");	
	
	WndID = ALLPHAS_LOCK_RECT_X_EDIT1;
	WndKey = _T("ALLPHAS_LOCK_RECT_X_EDIT1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = ALLPHAS_LOCK_RECT_Y_EDIT1;
	WndKey = _T("ALLPHAS_LOCK_RECT_Y_EDIT1");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = ALLPHAS_LOCK_RECT_X_EDIT2;
	WndKey = _T("ALLPHAS_LOCK_RECT_X_EDIT2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);

	WndID = ALLPHAS_LOCK_RECT_Y_EDIT2;
	WndKey = _T("ALLPHAS_LOCK_RECT_Y_EDIT2");
	CWnd::GetDlgItemText(WndID, Default);
	AOIDataCollect.LoadWndUIParam(Section, WndKey, Default, String);	
	CWnd::SetDlgItemText(WndID, String);
	return true;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnBasePlaneParamBtn() 
{
	// TODO: Add your control notification handler code here
	int  nBaseColoeIndex=0;	
	bool bBaseColorEnabled=false;	
	TBasePlaneParam BasePlaneParam;
	CSpaceBaseParamWnd ParamWnd;
	
	bBaseColorEnabled = false;
	nBaseColoeIndex   = -1;
	BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;
	if ( nBaseColoeIndex >= 0 ) { nBaseColoeIndex -= PROJECT_COLOR_ID_BOARD_BEGIN; }

	//ParamWnd.SetProjectPtr(ProjectPtr);
	ParamWnd.SetBasePlaneParam(BasePlaneParam);
	ParamWnd.SetBaseColorIndex(nBaseColoeIndex);
	ParamWnd.SetBaseColorEnabled(bBaseColorEnabled);	
	if ( ParamWnd.DoModal() == IDCANCEL )
	{	return ; }	

	ParamWnd.GetBasePlaneParam(BasePlaneParam);
	nBaseColoeIndex = ParamWnd.GetBaseColorIndex();
	bBaseColorEnabled = ParamWnd.GetBaseColorEnabled();
	const bool bParamSetting = ParamWnd.GetBasePlaneParamSetting();
	const bool bColorSetting = ParamWnd.GetBasePlaneColorSetting();

	if ( true == bParamSetting )
	{	m_NoiseFilterParam.BasePlaneParam = BasePlaneParam; }

	if ( true == bColorSetting ) 
	{
		//if ( false==bBaseColorEnabled || -1==nBaseColoeIndex )
		//{	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(false);	}
		//else
		//{
		//	nBaseColoeIndex += PROJECT_COLOR_ID_BOARD_BEGIN;
		//	ProjectPtr->EnableProjectComponentSelectedMaskFunc_Base(true);
		//	ProjectPtr->SetProjectComponentSelectedMaskColorIndex_Base(nBaseColoeIndex); 
		//}
	}
	AOIDataCollect.UpdateSystemBasePlaneParamToProject();
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnSpaceNoiseFilterBtn() 
{
	// TODO: Add your control notification handler code here	
	CSpaceNoiseFilterParamWnd Wnd;	
	TNoiseFilterParam NoiseFilterParam;
	
	NoiseFilterParam = m_NoiseFilterParam;
	Wnd.SetNoiseFilterParam(NoiseFilterParam);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }
	Wnd.GetNoiseFilterParam(NoiseFilterParam);

	NoiseFilterParam.BasePlaneParam = m_NoiseFilterParam.BasePlaneParam;	
	m_NoiseFilterParam = NoiseFilterParam;	
	UpdateSpaceNoiseFilterParamToUI(NoiseFilterParam);
	AOIDataCollect.UpdateSystemNoiseFilterParamToProject();
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnLoadParamBtn() 
{
	// TODO: Add your control notification handler code here
	TestDLPZeroBinFile();	
	//TestDLPFactorBinFile();
	const bool bCreateTempFolder = false;
	AOIDataCollect.LoadSystemParameter(bCreateTempFolder);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnSaveImageBtn() 
{
	// TODO: Add your control notification handler code here
	const char fnName[]="CImagePhaseAllWnd::OnSaveImageBtn";
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	size_t  i=0;
	CString str;
	CString Filename;
	CString filenameBase;	
	const bool bReverse=true;
	CString extname = dialog.GetFileExt();	
	CString filename = dialog.GetPathName();		
	JetAPI::ExtractMainFileName(filename, filenameBase);

	for ( i=0; i<MAX_PAHSE_COUNT; i++ )
	{
		if ( NULL == m_ShowBuffer[i] ) { continue; }
		Filename.Format(_T("%s#%d.%s"), filenameBase, i+1, extname);
		if ( ImageAPI.SaveImage(Filename, m_ImageW[i], m_ImageH[i], m_ShowStep[i], m_BitCount[i], m_ShowBuffer[i], bReverse) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
		}
	}

	const bool bSavePhaseImage=true;
	const int PhaseMode=LIGHT3D_PHASE_4_4_M;	
	const int LEDColor=JetAPI::GetComboxCurSelData(m_DLPLEDCombox);
	if ( true == bSavePhaseImage )
	{
		for ( i=0; i<DLP_CAST_COUNT; i++ )
		{
			LIGHT_3D_CAST_ID CastID = Light3DCtrl.GetLight3DCastIDByIndex(i);
			LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
			if ( NULL == CastPtr ) { continue; }

			PHASE_PTR  Ptr = NULL;
			IMAGE_SIZE W=0, H=0, Step=0;
			if ( CastPtr->GetPhaseZero(PhaseMode, LEDColor, W, H, Step, Ptr ) == false ) { continue; }
			if ( NULL == Ptr ) { continue; }			

			IMAGE_PTR ImgPtr=NULL;
			const size_t BufferSize=ImageAPI.CalcBufferSize(Step, H);
			if ( JetMemory.alloc_func(BufferSize, ImgPtr, fnName, "ImgPtr") == false )
			{	continue; }

			ImageAPI.PhaseGrayImageConvertToGray3(W, H, Step, Ptr, Step, ImgPtr, false);

			Filename.Format(_T("%s_Zero#%d.%s"), filenameBase, i+1, extname);
			if ( ImageAPI.SaveImage(Filename, W, H, Step, 8, ImgPtr, bReverse) == false )
			{
				str = ImageAPI.GetImageApiErrorString();
				JetAPI::ShowMessageBox(str);
			}			
			JetMemory.free_func(ImgPtr);
		}
	}

	const bool bSaveHeightFactorImage=true;
	if ( true == bSavePhaseImage )
	{	
		for ( i=0; i<DLP_CAST_COUNT; i++ )
		{
			LIGHT_3D_CAST_ID CastID = Light3DCtrl.GetLight3DCastIDByIndex(i);
			LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
			if ( NULL == CastPtr ) { continue; }

			SPACE_PTR  Ptr = NULL;
			IMAGE_SIZE W=0, H=0, Step=0;
			if ( CastPtr->GetPhaseFactor(PhaseMode, LEDColor, W, H, Step, Ptr ) == false ) { continue; }
			if ( NULL == Ptr ) { continue; }

			MASK_PTR MskPtr=NULL;
			IMAGE_PTR ImgPtr=NULL;
			const size_t BufferSize=ImageAPI.CalcBufferSize(Step, H);
			if ( JetMemory.alloc_func(BufferSize, MskPtr, fnName, "MskPtr") == false || 
				 JetMemory.alloc_func(BufferSize, ImgPtr, fnName, "ImgPtr") == false )
			{
				JetMemory.free_func(MskPtr);
				JetMemory.free_func(ImgPtr);
				continue; 
			}

			RECT RoiRect={0,0,0,0};
			const double Ratio = -1;
			JetAPI::SizeToRect(W, H, RoiRect);
			ImageAPI.SpaceGrayImageConvertToGray3(W, H, Step, Ptr, MskPtr, RoiRect, Step, ImgPtr, Ratio, false);

			Filename.Format(_T("%s_Factor#%d.%s"), filenameBase, i+1, extname);
			if ( ImageAPI.SaveImage(Filename, W, H, Step, 8, ImgPtr, bReverse) == false )
			{
				str = ImageAPI.GetImageApiErrorString();
				JetAPI::ShowMessageBox(str);
			}
			JetMemory.free_func(MskPtr);
			JetMemory.free_func(ImgPtr);
		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseAllWnd::OnSetPatternTypeBtn()
{
	// TODO: Add your control notification handler code here	
	CString         str;
	CString         strLabel;
	CString         strCaption;	
	TListNode       Node;
	CInputListWnd   EnumWnd;
	CString         Name=_T("3D Pattern");
	SLICE_FUNC_MODE SliceFuncMode=m_SliceFuncMode;
	DWORD_PTR       dwDefault=(DWORD_PTR)(SliceFuncMode);
	std::vector<TListNode> NodelList;		
	
	strLabel = _T("Pattern Type");
	//strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("DLP Pattern Type");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);			

	SliceFuncMode = SLICE_FUNC_3D_4STEP_4STEP_1EXP;			Node.Data = SliceFuncMode;
	Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);

	SliceFuncMode = SLICE_FUNC_3D_4STEP_2STEP_1EXP;			Node.Data = SliceFuncMode;
	//Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);

	SliceFuncMode = SLICE_FUNC_3D_2STEP_2STEP_1EXP;			Node.Data = SliceFuncMode;
	//Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);

	SliceFuncMode = SLICE_FUNC_3D_4STEP_5GC_1EXP;			Node.Data = SliceFuncMode;
	Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);

	SliceFuncMode = SLICE_FUNC_3D_4STEP_6GC_1EXP;			Node.Data = SliceFuncMode;
	Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);

	SliceFuncMode = SLICE_FUNC_3D_4STEP_4GC_1EXP;			Node.Data = SliceFuncMode;
	Node.Text.Format(_T("%s [%03d]"), Name, SliceFuncMode); NodelList.push_back(Node);
	
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }		

	SliceFuncMode = (SLICE_FUNC_MODE)(EnumWnd.GetSelData());	
	if ( SliceFuncMode != m_SliceFuncMode )
	{
		str = _T("Error, 3D Pattern Type Changed, You have to reload all raw image files");
		JetAPI::ShowMessageBox(str);		
	}
	m_SliceFuncMode = SliceFuncMode;
	//m_SliceFuncMode = AOIDataCollect.CheckSliceFuncModeBasicMode(m_SliceFuncMode);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::TestDLPZeroBinFile()
{
	size_t     i=0;
	size_t     ImageSize=0;
	const int  VerCnt=2;
	bool       bDiff=false;
	CString    Folder;
	CString    filename[VerCnt];
	IMAGE_SIZE ImageW[VerCnt]={0};
	IMAGE_SIZE ImageH[VerCnt]={0};
	IMAGE_SIZE ImageStep[VerCnt]={0};
	IMAGE_SIZE BitCount[VerCnt]={0};
	PHASE_PTR  PhasePtr[VerCnt]={NULL};

	for ( i=0; i<VerCnt; i++ )
	{
		PhasePtr[i] = NULL;
		ImageW[i] = ImageH[i] = ImageStep[i] = BitCount[i] = 0;
	}
	Folder = _T("D:\\20200107_foxconn raw 3D\\bin");

	bDiff=false;
	filename[0].Format(_T("%s\\%s"), Folder, _T("TiDLPZero4S2PM_ID-04_Blue.BIN"));
	filename[1].Format(_T("%s\\%s"), Folder, _T("TiDLPZero4S2PM_ID-04_White.BIN"));	

	//filename[1].Format(_T("%s\\%s"), Folder, _T("TiDLPFactor4S2PM_ID-04_Debug.BIN"));
	if ( LoadDLPPhaseZeroFile(filename[0], ImageW[0], ImageH[0], ImageStep[0], PhasePtr[0])==false || 
		 LoadDLPPhaseZeroFile(filename[1], ImageW[1], ImageH[1], ImageStep[1], PhasePtr[1])==false )
	{
		JetMemory.free_func(PhasePtr[0]);
		JetMemory.free_func(PhasePtr[1]);
		return false;
	}
	
	if ( ImageW[0]!=ImageW[1] || ImageH[0]!=ImageH[1] || ImageStep[0]!=ImageStep[1] )
	{
		JetMemory.free_func(PhasePtr[0]);
		JetMemory.free_func(PhasePtr[1]);
		return false;
	}

	ImageSize = ImageStep[0]*ImageH[0];
	for ( i=0; i<ImageSize; i++ )
	{
		if ( PhasePtr[0][i] != PhasePtr[1][i] )
		{	break; }
	}
	if ( i != ImageSize )
	{	bDiff = true;	}	
	JetMemory.free_func(PhasePtr[0]);
	JetMemory.free_func(PhasePtr[1]);	
	return bDiff;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::TestDLPFactorBinFile()
{
	size_t     i=0;
	size_t     ImageSize=0;
	const int  VerCnt=2;
	bool       bDiff=false;
	CString    Folder;
	CString    filename[VerCnt];
	IMAGE_SIZE ImageW[VerCnt]={0};
	IMAGE_SIZE ImageH[VerCnt]={0};
	IMAGE_SIZE ImageStep[VerCnt]={0};
	IMAGE_SIZE BitCount[VerCnt]={0};
	SPACE_PTR  SpacePtr[VerCnt]={NULL};

	for ( i=0; i<VerCnt; i++ )
	{
		SpacePtr[i] = NULL;
		ImageW[i] = ImageH[i] = ImageStep[i] = BitCount[i] = 0;
	}
	Folder = _T("D:\\20200107_foxconn raw 3D\\bin");

	bDiff=false;
	filename[0].Format(_T("%s\\%s"), Folder, _T("TiDLPFactor4S2PM_ID-04_Blue.BIN"));
	filename[1].Format(_T("%s\\%s"), Folder, _T("TiDLPFactor4S2PM_ID-04_White.BIN"));
	if ( LoadDLPPhaseFactorFile(filename[0], ImageW[0], ImageH[0], ImageStep[0], SpacePtr[0])==false || 
		 LoadDLPPhaseFactorFile(filename[1], ImageW[1], ImageH[1], ImageStep[1], SpacePtr[1])==false )
	{
		JetMemory.free_func(SpacePtr[0]);
		JetMemory.free_func(SpacePtr[1]);
		return false;
	}
	
	if ( ImageW[0]!=ImageW[1] || ImageH[0]!=ImageH[1] || ImageStep[0]!=ImageStep[1] )
	{
		JetMemory.free_func(SpacePtr[0]);
		JetMemory.free_func(SpacePtr[1]);
		return false;
	}

	ImageSize = ImageStep[0]*ImageH[0];
	for ( i=0; i<ImageSize; i++ )
	{
		if ( SpacePtr[0][i] != SpacePtr[1][i] )
		{	break; }
	}
	if ( i != ImageSize )
	{	bDiff = true;	}	
	JetMemory.free_func(SpacePtr[0]);
	JetMemory.free_func(SpacePtr[1]);	
	return bDiff;	
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CImagePhaseAllWnd::LoadDLPPhaseZeroFile";
	bool         IsOK = true;	
	CString      ErrorString;
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	PHASE_PTR    TempPhase = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BuferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	
	if ( JetMemory.alloc_func(BufferSize, TempPhase, fnName, "m_PhaseZeroPtr") == false )
	{
		ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempPhase, sizeof(PHASE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempPhase);
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;

	PhaseW = TempW;
	PhaseH = TempH;
	PhaseStep = TempStep;	
	PhasePtr = TempPhase;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CImagePhaseAllWnd::LoadDLPPhaseFactorFile";	
	bool         IsOK = true;		
	CString      ErrorString;
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	SPACE_PTR    TempSpace = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BufferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	if ( JetMemory.alloc_func(BufferSize, TempSpace, fnName, "m_PhaseFactorPtr") == false )
	{
		ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempSpace, sizeof(SPACE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempSpace);
		ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;

	SpaceW = TempW;
	SpaceH = TempH;
	SpaceStep = TempStep;	
	SpacePtr = TempSpace;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseAllWnd::CheckNeedReloadRawFile(const TPhaseNoiseParam &NewParam)
{
	const int NewPhaseNoiseDef=NewParam.PhaseNoiseDef;
	const int OldPhaseNoiseDef=m_PhaseNoiseParam.PhaseNoiseDef;

	if ( (OldPhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER) != (NewPhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER) )
	{	return true;	}
	
	if ( m_PhaseNoiseParam.PhaseSmoothFilter != NewParam.PhaseSmoothFilter )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//