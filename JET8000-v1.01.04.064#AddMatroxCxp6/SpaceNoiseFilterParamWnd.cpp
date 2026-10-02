// SpaceNoiseFilterParamWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SpaceNoiseFilterParamWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceNoiseFilterParamWnd dialog
//-------------------------------------------------------------------------------------//
CSpaceNoiseFilterParamWnd::CSpaceNoiseFilterParamWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSpaceNoiseFilterParamWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpaceNoiseFilterParamWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_MapIndex = -1;
	m_StopFilterListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpaceNoiseFilterParamWnd)	
	DDX_Control(pDX, SNFP_FIRST_FILTER_MODE_COMBO, m_FirstFilterModeCombox);
	DDX_Control(pDX, SNFP_OVER_LOW_MODE_COMBO, m_OverLowModeCombox);
	DDX_Control(pDX, SNFP_HEIGHT_VARIATION_MODE_COMBO, m_HeightVarModeCombox);			
	DDX_Control(pDX, SNFP_FINAL_FILTER_MODE_COMBO, m_FinalFilterModeCombox);
	DDX_Control(pDX, SNFP_FINAL_FILTER_MODE_COMBO2, m_FinalFilterModeCombox2);
	DDX_Control(pDX, SNFP_HEIGHT_CORRECT_MODE_COMBO, m_HeightCorrectModeCombox);	
	DDX_Control(pDX, SNFP_FILTER_LIST_WND, m_FilterListCtrl);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSpaceNoiseFilterParamWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CSpaceNoiseFilterParamWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_CLICK, SNFP_FILTER_LIST_WND, OnClickFilterListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, SNFP_FILTER_LIST_WND, OnItemchangedFilterListWnd)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_01, OnSendToBtn01)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_02, OnSendToBtn02)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_03, OnSendToBtn03)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_04, OnSendToBtn04)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_05, OnSendToBtn05)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_06, OnSendToBtn06)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_07, OnSendToBtn07)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_08, OnSendToBtn08)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_09, OnSendToBtn09)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_10, OnSendToBtn10)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_11, OnSendToBtn11)
	ON_BN_CLICKED(SNFP_SEND_TO_BTN_12, OnSendToBtn12)
	ON_BN_CLICKED(SNFP_UNLINK_BTN, OnUnlinkBtn)
	ON_BN_CLICKED(SNFP_SAVE_PARAM_BTN, OnSaveParamBtn)
	ON_BN_CLICKED(SNFP_LOAD_PARAM_BTN, OnLoadParamBtn)
	ON_BN_CLICKED(SNFP_UPDATE_BTN, OnUpdateBtn)
	ON_CBN_SELCHANGE(SNFP_FIRST_FILTER_MODE_COMBO, OnCbnSelchangeFirstFilterModeCombo)
	ON_CBN_SELCHANGE(SNFP_FINAL_FILTER_MODE_COMBO, OnCbnSelchangeFinalFilterModeCombo)
	ON_CBN_SELCHANGE(SNFP_FINAL_FILTER_MODE_COMBO2, OnCbnSelchangeFinalFilterModeCombo2)
	ON_BN_CLICKED(SNFP_FIRST_FILTER_SEARCHON_CHK, OnBnClickedFirstFilterSearchonChk)
	ON_BN_CLICKED(SNFP_FINAL_FILTER_SEARCHON_CHK, OnBnClickedFinalFilterSearchonChk)
	ON_BN_CLICKED(SNFP_FINAL_FILTER_SEARCHON_CHK2, OnBnClickedFinalFilterSearchonChk2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceNoiseFilterParamWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSpaceNoiseFilterParamWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here		
	JetAPI::InitialListCtrl(m_FilterListCtrl);
	m_Draw3DWnd.Create(IDD_DRAW3D_WND, this);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FirstFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_OverLowModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_HeightVarModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox);
	AOIDataDefine.BuidlSpaceNoiseFilterModeCombox(m_FinalFilterModeCombox2);
	AOIDataDefine.BuildSpaceHeightCorrectModelCombox(m_HeightCorrectModeCombox);	

	BuildFilterListWndHeader();
	BuildFilterListWnd();

	const size_t UniFrameCount = m_UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	JetAPI::EnableCtrlWnd(this, SNFP_UPDATE_BTN, FALSE);	}
	else
	{
		CString str;
		const bool bSave3D = true;
		const bool bAppend = false;
		const bool bReverse = true;
		const bool bEnhance = true;		
		const double RatioRatio = 50.0;
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SpaceNoiseFilter.PNG"));
		ImageAPI.SaveUniFrameImage(str, m_UniFrameList, bReverse, bEnhance, bSave3D, bAppend, RatioRatio);
	}
	SwitchMultiLanguage();
	UpdateParamToUI();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::SetNoiseFilterParam(const TNoiseFilterParam &Param)
{	
	m_NoiseFilterParam = Param;
	m_NoiseFilterParamDefault = Param;
	m_DefaultIndex = Param.DataFilterIndex;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::GetNoiseFilterParam(TNoiseFilterParam &Param)
{
	Param = m_NoiseFilterParam;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::SetModelUniFrameList(unsigned int MapIndex, std::vector<TUNI_FRAME> &UniFrameList)
{
	m_MapIndex = MapIndex;
	m_UniFrameList = UniFrameList;	
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SPACE_NOISE_FILTER_PARAM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SPACE_NOISE_FILTER_PARAM_WND;
	WndKey = _T("IDD_SPACE_NOISE_FILTER_PARAM_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(IDOK, NewLabelText);	

	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(IDCANCEL, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = SNFP_SAVE_PARAM_BTN;
	WndKey = _T("SNFP_SAVE_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_LOAD_PARAM_BTN;
	WndKey = _T("SNFP_LOAD_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_UPDATE_BTN;
	WndKey = _T("SNFP_UPDATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = SNFP_UNLINK_BTN;
	WndKey = _T("SNFP_UNLINK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_01;
	WndKey = _T("SNFP_SEND_TO_BTN_01");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_02;
	WndKey = _T("SNFP_SEND_TO_BTN_02");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_03;
	WndKey = _T("SNFP_SEND_TO_BTN_03");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_04;
	WndKey = _T("SNFP_SEND_TO_BTN_04");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_05;
	WndKey = _T("SNFP_SEND_TO_BTN_05");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_06;
	WndKey = _T("SNFP_SEND_TO_BTN_06");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_07;
	WndKey = _T("SNFP_SEND_TO_BTN_07");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_08;
	WndKey = _T("SNFP_SEND_TO_BTN_08");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_09;
	WndKey = _T("SNFP_SEND_TO_BTN_09");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_10;
	WndKey = _T("SNFP_SEND_TO_BTN_10");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_11;
	WndKey = _T("SNFP_SEND_TO_BTN_11");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_SEND_TO_BTN_12;
	WndKey = _T("SNFP_SEND_TO_BTN_12");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = SNFP_VOID_EXPAND_GROUP;
	WndKey = _T("SNFP_VOID_EXPAND_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_VOID_EXPAND_ENABLE_CHK;
	WndKey = _T("SNFP_VOID_EXPAND_ENABLE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_VOID_EXPAND_SIZE_LABEL;
	WndKey = _T("SNFP_VOID_EXPAND_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FIRST_FILTER_GROUP;
	WndKey = _T("SNFP_FIRST_FILTER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FIRST_FILTER_MODE_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SNFP_FIRST_FILTER_PITCH_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FIRST_FILTER_KER_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_KER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FIRST_FILTER_USE_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_USE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

#pragma region Content Aware Param - First
	WndID = SNFP_FIRST_FILTER_ALPHAF_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_ALPHAF_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FIRST_FILTER_ALPHAS_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_ALPHAS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FIRST_FILTER_ALPHAM_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_ALPHAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FIRST_FILTER_ALPHAI_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_ALPHAI_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FIRST_FILTER_SEARCHON_CHK;
	WndKey = _T("SNFP_FIRST_FILTER_SEARCHON_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FIRST_FILTER_OUTLIER_LABEL;
	WndKey = _T("SNFP_FIRST_FILTER_OUTLIER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
#pragma endregion
	
	WndID = SNFP_HEIGHT_VARIATION_GROUP;
	WndKey = _T("SNFP_HEIGHT_VARIATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_MODE_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_RANGE_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_RANGE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_PITCH_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_CHK_SIZE_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_CHK_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_KER_SIZE_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_KER_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_USE_SIZE_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_USE_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_HEIGHT_VARIATION_REPEAT_CNT_LABEL;
	WndKey = _T("SNFP_HEIGHT_VARIATION_REPEAT_CNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_OVER_LOW_GROUP;
	WndKey = _T("SNFP_OVER_LOW_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_OVER_LOW_MODE_LABEL;
	WndKey = _T("SNFP_OVER_LOW_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_OVER_LOW_RANGE_LABEL;
	WndKey = _T("SNFP_OVER_LOW_RANGE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_OVER_LOW_LIMIT_LABEL;
	WndKey = _T("SNFP_OVER_LOW_LIMIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_OVER_LOW_KER_SIZE_LABEL;
	WndKey = _T("SNFP_OVER_LOW_KER_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_OVER_LOW_USE_SIZE_LABEL;
	WndKey = _T("SNFP_OVER_LOW_USE_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_VOID_RECONTRUCT_GROUP;
	WndKey = _T("SNFP_VOID_RECONTRUCT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_VOID_RECONTRUCT_ENABLE_CHK;
	WndKey = _T("SNFP_VOID_RECONTRUCT_ENABLE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_VOID_RECONTRUCT_EXT_SIZE_LABEL;
	WndKey = _T("SNFP_VOID_RECONTRUCT_EXT_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_GROUP;
	WndKey = _T("SNFP_FINAL_FILTER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_MODE_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SNFP_FINAL_FILTER_PITCH_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_PITCH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_KER_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_KER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_USE_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_USE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

#pragma region Content Aware Param - Final
	WndID = SNFP_FINAL_FILTER_ALPHAF_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAF_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAS_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAM_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAI_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAI_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_SEARCHON_CHK;
	WndKey = _T("SNFP_FINAL_FILTER_SEARCHON_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_OUTLIER_LABEL;
	WndKey = _T("SNFP_FINAL_FILTER_OUTLIER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
#pragma endregion

	WndID = SNFP_FINAL_FILTER_GROUP2;
	WndKey = _T("SNFP_FINAL_FILTER_GROUP2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_MODE_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_MODE_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SNFP_FINAL_FILTER_PITCH_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_PITCH_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_KER_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_KER_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SNFP_FINAL_FILTER_USE_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_USE_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

#pragma region Content Aware Param - Final2
	WndID = SNFP_FINAL_FILTER_ALPHAF_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAF_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAS_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAS_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAM_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAM_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_ALPHAI_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_ALPHAI_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_SEARCHON_CHK2;
	WndKey = _T("SNFP_FINAL_FILTER_SEARCHON_CHK2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SNFP_FINAL_FILTER_OUTLIER_LABEL2;
	WndKey = _T("SNFP_FINAL_FILTER_OUTLIER_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
#pragma endregion
	//---------------------------------------------------------------------------------//	
	WndID = SNFP_HEIGHT_CORRECT_MODE_LABEL;
	WndKey = _T("SNFP_HEIGHT_CORRECT_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CSpaceNoiseFilterParamWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SPACE_NOISE_FILTER_PARAM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CSpaceNoiseFilterParamWnd::BuildNoiseFilterList()
{
	m_NoiseFilterParamList.clear();
	const int DefaultIndex = m_DefaultIndex;//m_NoiseFilterParamDefault.DataFilterIndex;
	AOIDataCollect.CloneSystemNoiseFilterParamList(m_NoiseFilterParamList);

	const int FilterCount = (int)(m_NoiseFilterParamList.size());
	if ( DefaultIndex<0 || DefaultIndex>=FilterCount )
	{	m_NoiseFilterParamList.push_back(m_NoiseFilterParam);	}
	//m_NoiseFilterParamList.push_back(m_NoiseFilterParam);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSpaceNoiseFilterParamWnd::BuildFilterListWndHeader()
{
	CString str;
	int   nCol = 0;	
	int width  = 0;
	int widthF = 64;
	int width1 = 48;
	int width2 = 64;
	int width3 = 48;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	CThisListCtrl_28 &ListCtrl = m_FilterListCtrl;

	ListCtrl.GetClientRect(&Rect);
	widthF = (Rect.right-Rect.left-8);
	width1 = 48;
	width3 = 48;

	width = width1;
	str = AOIDataDefine.GetIndexText();	
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;
	
	width = (widthF-width1-width3);
	str = _T("Information");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width = width3;
	str = _T("Use");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CSpaceNoiseFilterParamWnd::BuildFilterListWnd()
{	
	CThisListCtrl_28 &ListCtrl = m_FilterListCtrl;	
	m_StopFilterListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopFilterListBeSelected = false;	

	int           i=0;
	CString       str;
	CString       strIndex;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;		
	int           nItemSel=-1;
	const int     nSubItem1 = 1;
	const int     nSubItem2 = 2;
	COLORREF      clrTextBkLink = 0x0000FF;
	COLORREF      clrTextBkUnLink = 0xA0A0FF;
	TNoiseFilterParam *NoiseFilterPtr=NULL;
	
	BuildNoiseFilterList();
	const int DefaultIndex = m_DefaultIndex;
	const int ParamCount = (int)(m_NoiseFilterParamList.size());
	

	nItem=0;
	nItemSel=-1;
	ListCtrl.SetRedraw(FALSE);
	m_StopFilterListBeSelected = true;
	for ( i=0; i<ParamCount; i++ )
	{
		NoiseFilterPtr = &(m_NoiseFilterParamList[i]);
		if ( NULL == NoiseFilterPtr ) { continue; }		

		strIndex.Format(_T("%d"), nItem+1);		
		strValue = NoiseFilterPtr->DataFilterInfoText;
		ListCtrl.InsertItem(nItem, strIndex);
		ListCtrl.SetItemData(nItem, i);		
		ListCtrl.SetItemText(nItem, nSubItem1, strValue);
		if ( -1 == nItemSel )
		{
			if ( DefaultIndex == NoiseFilterPtr->DataFilterIndex )
			{
				nItemSel = nItem;
				ListCtrl.SetItemText(nItem, nSubItem2, _T("*"));	
			}
		}
		if ( i >= MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT )
		{	ListCtrl.SetItemTextBkColor(nItem, clrTextBkUnLink);	}
		nItem ++;
	}	
	m_StopFilterListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);

	if ( nItemSel > 0 ) 
	{	ListCtrl.SetItemState(nItemSel, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	}

	return true;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::UpdateParamToUI()
{
	CString str;
	const TNoiseFilterParam &Param = m_NoiseFilterParam;

	//Data
	//無效點外擴
	CWnd::CheckDlgButton(SNFP_VOID_EXPAND_ENABLE_CHK, Param.DataVoidExpandEnabled);
	CWnd::SetDlgItemInt(SNFP_VOID_EXPAND_SIZE_EDIT, Param.DataVoidExpandSize);
	
	//首次濾波處理	
	JetAPI::SetComboxCurSel(m_FirstFilterModeCombox, Param.DataFirstFilterMode);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_KER_EDIT, Param.DataFirstFilterKerSize);	
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_USE_EDIT, Param.DataFirstFilterUseSize);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_PITCH_EDIT, Param.DataFirstFilterPitch);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_ALPHAF_EDIT, Param.DataFirstFilterAlphaF);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_ALPHAS_EDIT, Param.DataFirstFilterAlphaS);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_ALPHAM_EDIT, Param.DataFirstFilterAlphaM);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_ALPHAI_EDIT, Param.DataFirstFilterAlphaI);
	CWnd::SetDlgItemInt(SNFP_FIRST_FILTER_OUTLIER_EDIT, Param.DataFirstFilterThresdhold_Outlier);
	CWnd::CheckDlgButton(SNFP_FIRST_FILTER_SEARCHON_CHK, Param.DataFirstFilterSearchOn);

	//將過低的雜訊給設定為0
	JetAPI::SetComboxCurSel(m_OverLowModeCombox, Param.DataOverLowFTMode);	
	CWnd::SetDlgItemInt(SNFP_OVER_LOW_RANGE_EDIT, Param.DataOverLowFTRange);	
	CWnd::SetDlgItemInt(SNFP_OVER_LOW_LIMIT_EDIT, Param.DataOverLowFTLimit);
	CWnd::SetDlgItemInt(SNFP_OVER_LOW_KER_SIZE_EDIT, Param.DataOverLowFTKerSize);	
	CWnd::SetDlgItemInt(SNFP_OVER_LOW_USE_SIZE_EDIT, Param.DataOverLowFTUseSize);	

	//高度異常濾除	
	JetAPI::SetComboxCurSel(m_HeightVarModeCombox, Param.DataHeightFTMode);		
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_RANGE_EDIT, Param.DataHeightFTRange);	
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_PITCH_EDIT, Param.DataHeightFTPitch);	
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_CHK_SIZE_EDIT, Param.DataHeightFTChkSize);
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_KER_SIZE_EDIT, Param.DataHeightFTKerSize);
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_USE_SIZE_EDIT, Param.DataHeightFTUseSize);
	CWnd::SetDlgItemInt(SNFP_HEIGHT_VARIATION_REPEAT_CNT_EDIT, Param.DataHeightFTRepeatCnt);		
	
	//雜訊重建
	CWnd::CheckDlgButton(SNFP_VOID_RECONTRUCT_ENABLE_CHK, Param.DataVoidReContructed);
	CWnd::SetDlgItemInt(SNFP_VOID_RECONTRUCT_EXT_SIZE_EDIT, Param.DataVoidReContructedExtSize);
	
	//後製濾波處理	
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox, Param.DataFinalFilterMode);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_KER_EDIT, Param.DataFinalFilterKerSize);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_USE_EDIT, Param.DataFinalFilterUseSize);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_PITCH_EDIT, Param.DataFinalFilterPitch);	
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAF_EDIT, Param.DataFinalFilterAlphaF);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAS_EDIT, Param.DataFinalFilterAlphaS);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAM_EDIT, Param.DataFinalFilterAlphaM);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAI_EDIT, Param.DataFinalFilterAlphaI);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_OUTLIER_EDIT, Param.DataFinalFilterThresdhold_Outlier);
	CWnd::CheckDlgButton(SNFP_FINAL_FILTER_SEARCHON_CHK, Param.DataFinalFilterSearchOn);

	//後製濾波處理-2
	JetAPI::SetComboxCurSel(m_FinalFilterModeCombox2, Param.DataFinalFilterMode2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_KER_EDIT2, Param.DataFinalFilterKerSize2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_USE_EDIT2, Param.DataFinalFilterUseSize2);	
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_PITCH_EDIT2, Param.DataFinalFilterPitch2);	
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAF_EDIT2, Param.DataFinalFilterAlphaF2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAS_EDIT2, Param.DataFinalFilterAlphaS2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAM_EDIT2, Param.DataFinalFilterAlphaM2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_ALPHAI_EDIT2, Param.DataFinalFilterAlphaI2);
	CWnd::SetDlgItemInt(SNFP_FINAL_FILTER_OUTLIER_EDIT2, Param.DataFinalFilterThresdhold_Outlier2);
	CWnd::CheckDlgButton(SNFP_FINAL_FILTER_SEARCHON_CHK2, Param.DataFinalFilterSearchOn2);

	//高度校正模式
	JetAPI::SetComboxCurSel(m_HeightCorrectModeCombox, Param.DataCorrectMode);
	
	CWnd::SetDlgItemText(SNFP_INFO_TEXT_EDIT, Param.DataFilterInfoText);
	this->UpdateContentAwareUI();
	return;
}
//-------------------------------------------------------------------------------------//
bool CSpaceNoiseFilterParamWnd::UpdateUIToParam()
{
	CString str;
	TNoiseFilterParam &Param = m_NoiseFilterParam;

	//Data
	//無效點外擴
	if ( CWnd::IsDlgButtonChecked(SNFP_VOID_EXPAND_ENABLE_CHK) == TRUE )
	{	Param.DataVoidExpandEnabled = true; }
	else
	{	Param.DataVoidExpandEnabled = false; }
	Param.DataVoidExpandSize = CWnd::GetDlgItemInt(SNFP_VOID_EXPAND_SIZE_EDIT);	

	//首次濾波處理		
	Param.DataFirstFilterMode = JetAPI::GetComboxCurSelData(m_FirstFilterModeCombox);
	Param.DataFirstFilterKerSize = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_KER_EDIT);	
	Param.DataFirstFilterUseSize = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_USE_EDIT);	
	Param.DataFirstFilterPitch = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_PITCH_EDIT);		
	Param.DataFirstFilterAlphaF = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_ALPHAF_EDIT);
	Param.DataFirstFilterAlphaS = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_ALPHAS_EDIT);
	Param.DataFirstFilterAlphaM = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_ALPHAM_EDIT);
	Param.DataFirstFilterAlphaI = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_ALPHAI_EDIT);
	Param.DataFirstFilterThresdhold_Outlier = CWnd::GetDlgItemInt(SNFP_FIRST_FILTER_OUTLIER_EDIT);
	Param.DataFirstFilterSearchOn = CWnd::IsDlgButtonChecked(SNFP_FIRST_FILTER_SEARCHON_CHK) == TRUE ? true : false;

	//將過低的雜訊給設定為0	
	Param.DataOverLowFTMode = JetAPI::GetComboxCurSelData(m_OverLowModeCombox);
	Param.DataOverLowFTRange = (int)(CWnd::GetDlgItemInt(SNFP_OVER_LOW_RANGE_EDIT));//注意負數的處理
	Param.DataOverLowFTLimit = (int)(CWnd::GetDlgItemInt(SNFP_OVER_LOW_LIMIT_EDIT));//注意負數的處理	
	Param.DataOverLowFTKerSize = (int)(CWnd::GetDlgItemInt(SNFP_OVER_LOW_KER_SIZE_EDIT));//注意負數的處理	
	Param.DataOverLowFTUseSize = (int)(CWnd::GetDlgItemInt(SNFP_OVER_LOW_USE_SIZE_EDIT));//注意負數的處理

	//高度異常濾除	
	Param.DataHeightFTMode = JetAPI::GetComboxCurSelData(m_HeightVarModeCombox);	
	Param.DataHeightFTRange = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_RANGE_EDIT);
	Param.DataHeightFTPitch = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_PITCH_EDIT);
	Param.DataHeightFTChkSize = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_CHK_SIZE_EDIT);	
	Param.DataHeightFTKerSize = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_KER_SIZE_EDIT);	
	Param.DataHeightFTUseSize = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_USE_SIZE_EDIT);
	Param.DataHeightFTRepeatCnt = CWnd::GetDlgItemInt(SNFP_HEIGHT_VARIATION_REPEAT_CNT_EDIT);		
	if ( Param.DataHeightFTPitch < 1 ) { Param.DataHeightFTPitch = 1; }
	
	//雜訊重建
	if ( CWnd::IsDlgButtonChecked(SNFP_VOID_RECONTRUCT_ENABLE_CHK) == TRUE )
	{	Param.DataVoidReContructed = true; }
	else
	{	Param.DataVoidReContructed = false; }
	Param.DataVoidReContructedExtSize = CWnd::GetDlgItemInt(SNFP_VOID_RECONTRUCT_EXT_SIZE_EDIT);	
	
	//後製濾波處理		
	Param.DataFinalFilterMode = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox);
	Param.DataFinalFilterKerSize = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_KER_EDIT);	
	Param.DataFinalFilterUseSize = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_USE_EDIT);	
	Param.DataFinalFilterPitch = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_PITCH_EDIT);		
	Param.DataFinalFilterAlphaF = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAF_EDIT);
	Param.DataFinalFilterAlphaS = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAS_EDIT);
	Param.DataFinalFilterAlphaM = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAM_EDIT);
	Param.DataFinalFilterAlphaI = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAI_EDIT);
	Param.DataFinalFilterThresdhold_Outlier = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_OUTLIER_EDIT);
	Param.DataFinalFilterSearchOn = CWnd::IsDlgButtonChecked(SNFP_FINAL_FILTER_SEARCHON_CHK) == TRUE ? true : false;

	//後製濾波處理-2
	Param.DataFinalFilterMode2 = JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox2);
	Param.DataFinalFilterKerSize2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_KER_EDIT2);	
	Param.DataFinalFilterUseSize2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_USE_EDIT2);	
	Param.DataFinalFilterPitch2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_PITCH_EDIT2);
	Param.DataFinalFilterAlphaF2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAF_EDIT2);
	Param.DataFinalFilterAlphaS2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAS_EDIT2);
	Param.DataFinalFilterAlphaM2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAM_EDIT2);
	Param.DataFinalFilterAlphaI2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_ALPHAI_EDIT2);
	Param.DataFinalFilterThresdhold_Outlier2 = CWnd::GetDlgItemInt(SNFP_FINAL_FILTER_OUTLIER_EDIT2);
	Param.DataFinalFilterSearchOn2 = CWnd::IsDlgButtonChecked(SNFP_FINAL_FILTER_SEARCHON_CHK2) == TRUE ? true : false;

	//高度校正模式
	Param.DataCorrectMode = (HEIGHT_DATA_CORRECT_MODE)(JetAPI::GetComboxCurSelData(m_HeightCorrectModeCombox));	

	CWnd::GetDlgItemText(SNFP_INFO_TEXT_EDIT, Param.DataFilterInfoText);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnOK() 
{
	// TODO: Add extra validation here
	if ( UpdateUIToParam() == false )
	{	return; }

	const int Index = m_NoiseFilterParam.DataFilterIndex;
	if ( Index>=0 && Index<MAX_SYSTEM_NOISE_FILTER_PARAM_COUNT )
	{	AOIDataCollect.SetSystemNoiseFilterParam(Index, m_NoiseFilterParam);	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnClickFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnItemchangedFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopFilterListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		CWnd::SetDlgItemText(SNFP_INFO_TEXT_EDIT, _T(""));		
		return; 
	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	const size_t Index = (size_t)(m_FilterListCtrl.GetItemData(nItem));
	const size_t Count = m_NoiseFilterParamList.size();
	if ( Index >= Count ) 
	{
		CWnd::SetDlgItemText(SNFP_INFO_TEXT_EDIT, _T(""));		
		return; 
	}
	m_NoiseFilterParam = m_NoiseFilterParamList[Index];
	UpdateParamToUI();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn01() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(0);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn02() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(1);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn03() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(2);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn04() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(3);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn05() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(4);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn06() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(5);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn07() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(6);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn08() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(7);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn09() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(8);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn10() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(9);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn11() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(10);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSendToBtn12() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(11);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnUnlinkBtn() 
{
	// TODO: Add your control notification handler code here
	SendToSystemNoiseFilter(-1);
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::SendToSystemNoiseFilter(int index)
{
	UpdateUIToParam();
	m_DefaultIndex = index;
	m_NoiseFilterParam.DataFilterIndex = index;
	AOIDataCollect.SetSystemNoiseFilterParam(index, m_NoiseFilterParam);
	BuildFilterListWnd();
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnSaveParamBtn() 
{
	// TODO: Add your control notification handler code here
	bool IsLockSystemParam = AOIDataCollect.CheckIsLockSystemParameter();	
	if ( true == IsLockSystemParam )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return; 
	}

	CString str;
	str = _T("Do you want to save space noise filter param?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON1) == IDNO )
	{	return; }

	AOIDataCollect.SaveSystemNoiseFilterParam();
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnLoadParamBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str = _T("Do you want to load space noise filter param?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO|MB_DEFBUTTON2) == IDNO )
	{	return; }

	AOIDataCollect.LoadSystemNoiseFilterParam();
	BuildFilterListWnd();
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::OnUpdateBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateUIToParam();	
	CString      str;
	size_t       i=0;
	TUNI_FRAME   UniFrame;
	const bool   bOpenMP = true;
	const size_t UniFrameCount = m_UniFrameList.size();
	TNoiseFilterParam  NoiseFilter = m_NoiseFilterParam;

	for ( i=0; i<UniFrameCount; i++ )
	{
		UniFrame = m_UniFrameList[i];
		if ( NULL == UniFrame.MaskPtr ) { continue; }
		if ( NULL == UniFrame.SpacePtr ) { continue; }
		break;
	}
	if ( UniFrameCount == i ) { return; }

	const char fnName[] = "CSpaceNoiseFilterParamWnd::OnUpdateBtn";
	IMAGE_SIZE ImageW = UniFrame.ImageW;
	IMAGE_SIZE ImageH = UniFrame.ImageH;
	IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	IMAGE_PTR  ImagePtr  = NULL;
	MASK_PTR   MaskPtr   = UniFrame.MaskPtr;
	MASK_PTR   MaskPtr2D = NULL;
	SPACE_PTR  SpacePtr  = UniFrame.SpacePtr;

	MASK_PTR    MaskPtrDst = NULL;
	SPACE_PTR   SpacePtrDst = NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, MaskPtrDst, fnName, "MaskPtrDst") == false ||
		 JetMemory.alloc_func(BufferSize, SpacePtrDst, fnName, "SpacePtrDst") == false )
	{
		JetMemory.free_func(MaskPtrDst);
		JetMemory.free_func(SpacePtrDst);
		return;
	}

	RECT PadRect={0};
	RECT RoiRect={0};
	bool IsColor = false;
	LARGE_INTEGER  fnStart, fnEnd;
	if ( m_MapIndex<0 || m_MapIndex>=UniFrameCount )
	{	ImagePtr = NULL;	}
	else
	{
		ImagePtr = m_UniFrameList[m_MapIndex].ImagePtr;
		IsColor = ImageAPI.CheckIsColor(m_UniFrameList[m_MapIndex].BitCount);
	}
	const int nOpenMPCnt = AOIDataCollect.CheckOpenMPCount_SpaceFilter(bOpenMP, ImageW*ImageH);	
	JetAPI::SetFuncTimeStart(fnStart);
	ImageAPI.BuildSpaceData3(ImageW, ImageH, ImageStep, SpacePtr, MaskPtr, MaskPtr2D, nOpenMPCnt, NoiseFilter, SpacePtrDst, MaskPtrDst);
	JetAPI::SetFuncTimeEnd(fnEnd);
	double fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Exec Filter Time=%.3fms"), fnTime);
	JetAPI::ShowMessageBox(str);

	m_Draw3DWnd.Set3DData(SpacePtrDst, ImagePtr, ImageW, ImageH, ImageStep, IsColor, -1, -1, -1, -1, PadRect, RoiRect, -1);
	if ( m_Draw3DWnd.IsWindowVisible() == FALSE )
	{	m_Draw3DWnd.ShowWindow(SW_SHOW);	}
	JetMemory.free_func(MaskPtrDst);
	JetMemory.free_func(SpacePtrDst);	
}
//-------------------------------------------------------------------------------------//
void CSpaceNoiseFilterParamWnd::UpdateContentAwareUI()
{
	if (JetAPI::GetComboxCurSelData(m_FirstFilterModeCombox) == DATA_NF_CONTENTAWARE)
	{
		if(CWnd::IsDlgButtonChecked(SNFP_FIRST_FILTER_SEARCHON_CHK) == TRUE)
		{
			CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAF_EDIT)->EnableWindow(TRUE);
			CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAS_EDIT)->EnableWindow(TRUE);
		}
		else
		{
			CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAF_EDIT)->EnableWindow(FALSE);
			CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAS_EDIT)->EnableWindow(FALSE);
		}
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAM_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAI_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_OUTLIER_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_SEARCHON_CHK)->EnableWindow(TRUE);
	}
	else
	{
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAF_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAS_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAM_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_ALPHAI_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_OUTLIER_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FIRST_FILTER_SEARCHON_CHK)->EnableWindow(FALSE);
	}

	if (JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox) == DATA_NF_CONTENTAWARE)
	{
		if (CWnd::IsDlgButtonChecked(SNFP_FINAL_FILTER_SEARCHON_CHK) == TRUE)
		{
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT)->EnableWindow(TRUE);
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT)->EnableWindow(TRUE);
		}
		else
		{
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT)->EnableWindow(FALSE);
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT)->EnableWindow(FALSE);
		}
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAM_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAI_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_OUTLIER_EDIT)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_SEARCHON_CHK)->EnableWindow(TRUE);
	}
	else
	{
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAM_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAI_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_OUTLIER_EDIT)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_SEARCHON_CHK)->EnableWindow(FALSE);
	}
	
	if (JetAPI::GetComboxCurSelData(m_FinalFilterModeCombox2) == DATA_NF_CONTENTAWARE)
	{
		if (CWnd::IsDlgButtonChecked(SNFP_FINAL_FILTER_SEARCHON_CHK2) == TRUE)
		{
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT2)->EnableWindow(TRUE);
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT2)->EnableWindow(TRUE);
		}
		else
		{
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT2)->EnableWindow(FALSE);
			CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT2)->EnableWindow(FALSE);
		}
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAM_EDIT2)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAI_EDIT2)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_OUTLIER_EDIT2)->EnableWindow(TRUE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_SEARCHON_CHK2)->EnableWindow(TRUE);
	}
	else
	{
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAF_EDIT2)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAS_EDIT2)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAM_EDIT2)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_ALPHAI_EDIT2)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_OUTLIER_EDIT2)->EnableWindow(FALSE);
		CWnd::GetDlgItem(SNFP_FINAL_FILTER_SEARCHON_CHK2)->EnableWindow(FALSE);
	}
}
void CSpaceNoiseFilterParamWnd::OnCbnSelchangeFirstFilterModeCombo()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}
void CSpaceNoiseFilterParamWnd::OnCbnSelchangeFinalFilterModeCombo()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}
void CSpaceNoiseFilterParamWnd::OnCbnSelchangeFinalFilterModeCombo2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}

void CSpaceNoiseFilterParamWnd::OnBnClickedFirstFilterSearchonChk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}
void CSpaceNoiseFilterParamWnd::OnBnClickedFinalFilterSearchonChk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}
void CSpaceNoiseFilterParamWnd::OnBnClickedFinalFilterSearchonChk2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData();
	this->UpdateContentAwareUI();
}
//-------------------------------------------------------------------------------------//