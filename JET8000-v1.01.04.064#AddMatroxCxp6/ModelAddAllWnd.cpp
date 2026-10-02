// ModelAddAllWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ModelAddAllWnd.h"
//-------------------------------------------------------------------------------------//
#include "ModelGroupWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelAddAllWnd dialog
//-------------------------------------------------------------------------------------//
CModelAddAllWnd::CModelAddAllWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CModelAddAllWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelAddAllWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	BuildCtrlIdMapWndDefectId(m_CtrlIdMapDefectId);
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelAddAllWnd)	
	DDX_Control(pDX, MAAW_MODEL_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, MAAW_CHIP_SIZE_COMBO, m_ChipSizeModeCombox);
	DDX_Control(pDX, MAAW_PART_ALIGN_MODE_COMBO, m_PartAlignModeCombox);
	DDX_Control(pDX, MAAW_BODY_MISSING_MODE_COMBO, m_BodyMissingModeCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CModelAddAllWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CModelAddAllWnd)
	ON_BN_CLICKED(MAAW_DISABLE_ALL_BTN, OnDisableAllBtn)
	ON_BN_CLICKED(MAAW_DEFAULT_PARAM_BTN, OnDefaultParamBtn)
	ON_BN_CLICKED(MAAW_DEFAULT_PARAM_SAVE_BTN, OnDefaultParamSaveBtn)
	ON_BN_CLICKED(MAAW_LEVEL_BASIC_BTN, OnLevelBasicBtn)
	ON_BN_CLICKED(MAAW_LEVEL_ADVANCE_BTN, OnLevelAdvanceBtn)
	ON_CBN_SELCHANGE(MAAW_CHIP_SIZE_COMBO, OnSelchangeChipSizeCombo)
	ON_BN_CLICKED(MAAW_USE_DEFAULT_MODEL_CHK, OnUseDefaultModelChk)
	ON_BN_CLICKED(MAAW_SHOW_ALL_GROUP_BTN, OnShowAllGroupBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelAddAllWnd message handlers
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnOK() 
{
	// TODO: Add extra validation here
	UIToParam();
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::SetAddAllParam(const TMODEL_DEFAULT_WND_PARAM &Param, const CWndDefectItem & ExistDefectItems, const CWndDefectItem & BasicDefectItems, int nLevel)
{	
	m_ModelAddWndParam = Param;
	m_ModelDefaultParam = Param;
	m_ModelDefaultModel.SetAll(true);
	m_ModelExistDefectItems = ExistDefectItems;	
	m_ModelBasicDefectItems = BasicDefectItems;		
	if ( 1 == nLevel )
	{	m_ModelDefectItems = BasicDefectItems;	}
	else
	{	m_ModelDefectItems.SetAll(1);	}
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::SetModelImageFilename(LPCTSTR name)
{
	m_ModelImageFilename = name;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::GetAddAllParam(TMODEL_DEFAULT_WND_PARAM &Param)
{
	Param = m_ModelAddWndParam;
}
//-------------------------------------------------------------------------------------//
BOOL CModelAddAllWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	SwitchMultiLanguage();	
	BuildChipSizeMode(m_ChipSizeModeCombox);
	BuildPartAlignMode(m_PartAlignModeCombox);
	BuildBodyMissingMode(m_BodyMissingModeCombox);
	BuildModleImageWnd();
	UpdateUseDefaultModel();
	ParamToUI(true);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	if ( true == m_ModelAddWndParam.bUseDefaultModel )
	{	CWnd::PostMessage(WM_COMMAND, MAAW_USE_DEFAULT_MODEL_CHK, NULL);	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MODEL_ADD_ALL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MODEL_ADD_ALL_WND;
	WndKey = _T("IDD_MODEL_ADD_ALL_WND");
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
	WndID = MAAW_GROUP_NAME_LABEL;
	WndKey = _T("MAAW_GROUP_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_CHIP_SIZE_LABEL;
	WndKey = _T("MAAW_CHIP_SIZE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_USE_DEFAULT_MODEL_CHK;
	WndKey = _T("MAAW_USE_DEFAULT_MODEL_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_USE_DEFAULT_MODEL_ALL_DEFECT_CHK;
	WndKey = _T("MAAW_USE_DEFAULT_MODEL_ALL_DEFECT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MAAW_DISABLE_ALL_BTN;
	WndKey = _T("MAAW_DISABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_DEFAULT_PARAM_BTN;
	WndKey = _T("MAAW_DEFAULT_PARAM_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_DEFAULT_PARAM_SAVE_BTN;
	WndKey = _T("MAAW_DEFAULT_PARAM_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_LEVEL_BASIC_BTN;
	WndKey = _T("MAAW_LEVEL_BASIC_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_LEVEL_ADVANCE_BTN;
	WndKey = _T("MAAW_LEVEL_ADVANCE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = MAAW_SHOW_ALL_GROUP_BTN;
	WndKey = _T("MAAW_SHOW_ALL_GROUP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WND_DEFECT_ID WndDefectID;
	const std::map<UINT, WND_DEFECT_ID> &Map=m_CtrlIdMapDefectId;
	for ( auto iter=Map.begin(); iter!=Map.end(); ++iter )
	{
		WndID=iter->first;
		WndDefectID=iter->second;
		NewLabelText = AOIDataDefine.GetWndDefectIDText(WndDefectID);		
		this->SetDlgItemText(WndID, NewLabelText);	
	}
	//---------------------------------------------------------------------------------//
	WndID = MAAW_PAD_ALIGN_EXTEND_LABEL;	
	WndKey = _T("MAAW_PAD_ALIGN_EXTEND_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = MAAW_PART_ALIGN_MODE_LABEL;	
	WndKey = _T("MAAW_PART_ALIGN_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_PART_ALIGN_EXTEND_LABEL;	
	WndKey = _T("MAAW_PART_ALIGN_EXTEND_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_PART_ALIGN_HEIGHT_TOL_LABEL;	
	WndKey = _T("MAAW_PART_ALIGN_HEIGHT_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_PART_ALIGN_OFFSET_LIMIT_LABEL;	
	WndKey = _T("MAAW_PART_ALIGN_OFFSET_LIMIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = MAAW_BODY_MISSING_MODE_LABEL;	
	WndKey = _T("MAAW_BODY_MISSING_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MAAW_BODY_MISSING_HEIGHT_TOL_LABEL;	
	WndKey = _T("MAAW_BODY_MISSING_HEIGHT_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = MAAW_BODY_TILT_HEIGHT_TOL_LABEL;	
	WndKey = _T("MAAW_BODY_TILT_HEIGHT_TOL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = MAAW_BRIDGE_USE_2D_CHK;	
	WndKey = _T("MAAW_BRIDGE_USE_2D_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MAAW_BRIDGE_USE_3D_CHK;	
	WndKey = _T("MAAW_BRIDGE_USE_3D_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MAAW_BRIDGE_END_WND_CHK;	
	WndKey = _T("MAAW_BRIDGE_END_WND_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MAAW_BRIDGE_EXTEND_LABEL;	
	WndKey = _T("MAAW_BRIDGE_EXTEND_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::UIToParam()
{	
	CString str;
	m_ModelAddWndParam.eChipSizeMode = (CHIP_SIZE_MODE)(JetAPI::GetComboxCurSelData(m_ChipSizeModeCombox));

	if ( CWnd::IsDlgButtonChecked(MAAW_USE_DEFAULT_MODEL_CHK) == BST_CHECKED )
	{	m_ModelAddWndParam.bUseDefaultModel = true;	}
	else
	{	m_ModelAddWndParam.bUseDefaultModel = false; }

	if ( CWnd::IsDlgButtonChecked(MAAW_USE_DEFAULT_MODEL_ALL_DEFECT_CHK) == BST_CHECKED )
	{	m_ModelAddWndParam.bDefaultModelAllDefect = true;	}
	else
	{	m_ModelAddWndParam.bDefaultModelAllDefect = false;	}	
	//---------------------------------------------------------------------------------//	
	const std::map<UINT, WND_DEFECT_ID> &Map=m_CtrlIdMapDefectId;
	for ( auto iter=Map.begin(); iter!=Map.end(); ++iter )
	{
		bool bEnable=false;
		UINT WndID=iter->first;
		WND_DEFECT_ID WndDefectID=iter->second;
		if ( CWnd::IsDlgButtonChecked(WndID) == BST_CHECKED )
		{	bEnable = true;	}
		else
		{	bEnable = false; }
		m_ModelAddWndParam.SetEnable(WndDefectID, bEnable);
	}
	//---------------------------------------------------------------------------------//	
	CWnd::GetDlgItemText(MAAW_PAD_ALIGN_EXTEND_EDIT, str);
	m_ModelAddWndParam.dPadAlignExtendRange = ::_ttof(str);
	//---------------------------------------------------------------------------------//	
	m_ModelAddWndParam.nPartAlignMode = JetAPI::GetComboxCurSelData(m_PartAlignModeCombox);
	CWnd::GetDlgItemText(MAAW_PART_ALIGN_EXTEND_EDIT, str);
	m_ModelAddWndParam.dPartAlignExtendRange = ::_ttof(str);
	CWnd::GetDlgItemText(MAAW_PART_ALIGN_HEIGHT_TOL_EDIT, str);
	m_ModelAddWndParam.dPartAlignHeightTolerance = ::_ttof(str);
	CWnd::GetDlgItemText(MAAW_PART_ALIGN_OFFSET_LIMIT_EDIT, str);
	m_ModelAddWndParam.dPartAlignOffsetLimit = ::_ttof(str);
	//---------------------------------------------------------------------------------//	
	m_ModelAddWndParam.nBodyMissingMode = JetAPI::GetComboxCurSelData(m_BodyMissingModeCombox);
	CWnd::GetDlgItemText(MAAW_BODY_MISSING_HEIGHT_TOL_EDIT, str);
	m_ModelAddWndParam.dBodyMissingHeightTolerance = ::_ttof(str);
	//---------------------------------------------------------------------------------//	
	CWnd::GetDlgItemText(MAAW_BODY_TILT_HEIGHT_TOL_EDIT, str);
	m_ModelAddWndParam.dBodyTiltHeightTolerance = ::_ttof(str);
	//---------------------------------------------------------------------------------//	
	if ( CWnd::IsDlgButtonChecked(MAAW_BRIDGE_USE_2D_CHK) == BST_CHECKED )
	{	m_ModelAddWndParam.bBridgeUse2D = true;		}
	else
	{	m_ModelAddWndParam.bBridgeUse2D = false;	}

	if ( CWnd::IsDlgButtonChecked(MAAW_BRIDGE_USE_3D_CHK) == BST_CHECKED )
	{	m_ModelAddWndParam.bBridgeUse3D = true;		}
	else
	{	m_ModelAddWndParam.bBridgeUse3D = false;	}
#ifdef DISABLE_3D
	m_ModelAddWndParam.bBridgeUse3D = false;
#endif//DISABLE_3D

	if ( CWnd::IsDlgButtonChecked(MAAW_BRIDGE_END_WND_CHK) == BST_CHECKED )
	{	m_ModelAddWndParam.bBridgeTwoSide = true;	}
	else
	{	m_ModelAddWndParam.bBridgeTwoSide = false; }

	CWnd::GetDlgItemText(MAAW_BRIDGE_EXTEND_EDIT, str);
	m_ModelAddWndParam.dBridgeExtendRange = ::_ttof(str);
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::ParamToUI(bool Filter)
{
	CString  str;
	UINT CtrlID=0;
	BOOL  bCheck = TRUE;		
	WND_DEFECT_ID WndDefectID;	
	CWndDefectItem DefectItems = m_ModelDefectItems;	
	CWndDefectItem ExistDefectItems = m_ModelExistDefectItems;//現在已經有的瑕疵項目	
	std::map<UINT, WND_DEFECT_ID> &Map=m_CtrlIdMapDefectId;

	CWnd::SetDlgItemText(MAAW_GROUP_NAME_EDIT, m_ModelAddWndParam.sGroupName);

	JetAPI::SetComboxCurSel(m_ChipSizeModeCombox, m_ModelAddWndParam.eChipSizeMode);
	const bool bUseChipSizeLevel = CAOIModel::CheckModelTypUseChipSizeMode(m_ModelAddWndParam.eModelType);
	JetAPI::EnableCtrlWnd(this, MAAW_CHIP_SIZE_COMBO, bUseChipSizeLevel);	

	if ( true == m_ModelAddWndParam.bUseDefaultModel )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	CWnd::CheckDlgButton(MAAW_USE_DEFAULT_MODEL_CHK, bCheck);

	if ( true == m_ModelAddWndParam.bDefaultModelAllDefect )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	CWnd::CheckDlgButton(MAAW_USE_DEFAULT_MODEL_ALL_DEFECT_CHK, bCheck);

	CtrlID = MAAW_PAD_ALIGN_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);	
	str.Format(_T("%.2f"), m_ModelAddWndParam.dPadAlignExtendRange);
	CWnd::SetDlgItemText(MAAW_PAD_ALIGN_EXTEND_EDIT, str);	

	CtrlID = MAAW_PART_ALIGN_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	JetAPI::SetComboxCurSel(m_PartAlignModeCombox, m_ModelAddWndParam.nPartAlignMode);	
	str.Format(_T("%.2f"), m_ModelAddWndParam.dPartAlignExtendRange);
	CWnd::SetDlgItemText(MAAW_PART_ALIGN_EXTEND_EDIT, str);	
	str.Format(_T("%.2f"), m_ModelAddWndParam.dPartAlignHeightTolerance);
	CWnd::SetDlgItemText(MAAW_PART_ALIGN_HEIGHT_TOL_EDIT, str);	
	str.Format(_T("%.2f"), m_ModelAddWndParam.dPartAlignOffsetLimit);
	CWnd::SetDlgItemText(MAAW_PART_ALIGN_OFFSET_LIMIT_EDIT, str);	

	CtrlID = MAAW_PAD_ADJUST_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	
	CtrlID = MAAW_LEAD_ADJUST_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	
	CtrlID = MAAW_BODY_MISSING_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	JetAPI::SetComboxCurSel(m_BodyMissingModeCombox, m_ModelAddWndParam.nBodyMissingMode);
	str.Format(_T("%.2f"), m_ModelAddWndParam.dBodyMissingHeightTolerance);
	CWnd::SetDlgItemText(MAAW_BODY_MISSING_HEIGHT_TOL_EDIT, str);	

	CtrlID = MAAW_BODY_TILT_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}	
	CWnd::CheckDlgButton(CtrlID, bCheck);	
	str.Format(_T("%.2f"), m_ModelAddWndParam.dBodyTiltHeightTolerance);
	CWnd::SetDlgItemText(MAAW_BODY_TILT_HEIGHT_TOL_EDIT, str);	

	CtrlID = MAAW_BODY_MOUNT_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_POLARITY_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	
	CtrlID = MAAW_TEXT_WRONG_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_TEXT_WRONG_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_BODY_DAMAGED_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_LEAD_BENDED_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);	

	CtrlID = MAAW_SOLDER_OPEN_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_SOLDER_POOR_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);
	
	CtrlID = MAAW_SOLDER_PAD_EXPOSED_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_PAD_SCRATCH_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	CtrlID = MAAW_BRIDGE_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	if ( true == m_ModelAddWndParam.bBridgeUse2D )
	{	bCheck = TRUE;	}
	else
	{	bCheck = FALSE;	}
	CWnd::CheckDlgButton(MAAW_BRIDGE_USE_2D_CHK, bCheck);

	if ( true == m_ModelAddWndParam.bBridgeUse3D )
	{	bCheck = TRUE;	}
	else
	{	bCheck = FALSE;	}
	CWnd::CheckDlgButton(MAAW_BRIDGE_USE_3D_CHK, bCheck);

	str.Format(_T("%.2f"), m_ModelAddWndParam.dBridgeExtendRange);
	CWnd::SetDlgItemText(MAAW_BRIDGE_EXTEND_EDIT, str);	

	CtrlID = MAAW_FOREIGN_BODY_CHK;
	WndDefectID=Map[CtrlID];
	if ( true == m_ModelAddWndParam.GetEnable(WndDefectID) )
	{	bCheck = TRUE; }
	else
	{	bCheck = FALSE; }
	if ( true == Filter )
	{
		if ( 0 == DefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	bCheck = FALSE;	}
	}
	CWnd::CheckDlgButton(CtrlID, bCheck);

	//3D未使用的變更
	UpdateUse3DLight();	
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::BuildModleImageWnd()
{
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	const int len = m_ModelImageFilename.GetLength();
	if ( 0 == len ) { return; }
	
	CString   filename = m_ModelImageFilename;
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
	{	return; }

	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, true, true, false);	
	m_ImageWnd.ShowFittedZoom();
	JetMemory.free_func(ImagePtr);
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::BuildCtrlIdMapWndDefectId(std::map<UINT, WND_DEFECT_ID> &Map)
{
	Map.clear();
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_PAD_ALIGN_CHK, WND_DEFECT_PAD_ALIGN));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_PART_ALIGN_CHK, WND_DEFECT_PART_ALIGN));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_PAD_ADJUST_CHK, WND_DEFECT_PAD_ADJUST));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_LEAD_ADJUST_CHK, WND_DEFECT_LEAD_ADJUST));

	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_BODY_MISSING_CHK, WND_DEFECT_BODY_MISSING));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_BODY_TILT_CHK, WND_DEFECT_BODY_TILT));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_BODY_MOUNT_CHK, WND_DEFECT_BODY_MOUNT));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_POLARITY_CHK, WND_DEFECT_BODY_POLARITY));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_TEXT_WRONG_CHK, WND_DEFECT_BODY_WRONG_TEXT));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_BODY_DAMAGED_CHK, WND_DEFECT_BODY_DAMAGED));

	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_LEAD_LIFTED_CHK, WND_DEFECT_LEAD_LIFTED));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_LEAD_BENDED_CHK, WND_DEFECT_LEAD_BENDED));

	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_SOLDER_OPEN_CHK, WND_DEFECT_SOLDER_OPEN));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_SOLDER_POOR_CHK, WND_DEFECT_SOLDER_POOR));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_PAD_SCRATCH_CHK, WND_DEFECT_PAD_SCRATCH));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_SOLDER_PAD_EXPOSED_CHK, WND_DEFECT_SOLDER_PAD_EXPOSED));

	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_BRIDGE_CHK, WND_DEFECT_SOLDER_BRIDGE));
	Map.insert(std::pair<UINT, WND_DEFECT_ID>(MAAW_FOREIGN_BODY_CHK, WND_DEFECT_FOREIGN_BODY));

	//Map.insert(std::pair<UINT, WND_DEFECT_ID>(AAAAAAAAAAA, BBBBBBBBBBBBB));
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::BuildChipSizeMode(CComboBox &Combox)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return; }
	
	size_t       i=0;
	int          idx=0;
	CString      str;
	CString      str1;	
	CHIP_SIZE_MODE ChipSizeMode;
	idx = 0;
	JetAPI::ClearCombox(Combox);	

	for ( i=0; i<=CHIP_SIZE_OTHERS; i++ )
	{
		ChipSizeMode = (CHIP_SIZE_MODE)(CHIP_SIZE_OTHERS-i);
		if ( CHIP_SIZE_NONE == ChipSizeMode )
		{	continue; }
		str = CAOIModel::GetModelChipSizeModeText(ChipSizeMode);
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, ChipSizeMode);
		idx ++;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::BuildPartAlignMode(CComboBox &Combox)
{
	if ( Combox.GetSafeHwnd() == NULL ) { return; }
	
	size_t       i=0;
	int          idx=0;
	CString      str;
	CString      str1;
	DWORD        Param=0;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str1 = AOIDataDefine.GetAlgTypeText(ALG_MODEL_MATCH);
	str.Format(_T("%s-2D"), str1);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, PART_ALIGN_MODE_BY_MODEL_MATCH_2D);
	idx ++;

	str1 = AOIDataDefine.GetAlgTypeText(ALG_MODEL_MATCH);
	str.Format(_T("%s-3D"), str1);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, PART_ALIGN_MODE_BY_MODEL_MATCH_3D);
	idx ++;

	str1 = AOIDataDefine.GetAlgTypeText(ALG_OBJECT_MEASURE);
	str.Format(_T("%s-3D"), str1);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, PART_ALIGN_MODE_BY_OBJECT_MEASURE);
	idx ++;

	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::BuildBodyMissingMode(CComboBox &Combox)
{	
	if ( Combox.GetSafeHwnd() == NULL ) { return; }
	
	size_t       i=0;
	int          idx=0;
	CString      str;
	DWORD        Param=0;		

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	str = AOIDataDefine.GetThicknessText();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, BODY_MISSING_MODE_BY_HEIGHT);
	idx ++;

	str = AOIDataDefine.GetVolumeText();
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, BODY_MISSING_MODE_BY_VOLUME);
	idx ++;
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = FALSE;
	const std::map<UINT, WND_DEFECT_ID> &Map=m_CtrlIdMapDefectId;
	for ( auto iter=Map.begin(); iter!=Map.end(); ++iter )
	{
		UINT CtrlID=iter->first;
		CWnd::CheckDlgButton(CtrlID, bCheck);
	}	
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnDefaultParamBtn() 
{
	// TODO: Add your control notification handler code here
	m_ModelAddWndParam = m_ModelDefaultParam;
	UpdateUseDefaultModel();
	ParamToUI(true);
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnDefaultParamSaveBtn() 
{
	// TODO: Add your control notification handler code here
	UIToParam();
	TMODEL_DEFAULT_WND_PARAM ModelAddWndParam;
	CWndDefectItem ExistDefectItems = m_ModelExistDefectItems;//現在已經有的瑕疵項目

	ModelAddWndParam = m_ModelAddWndParam;
	//填回現有檢測項目
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		WND_DEFECT_ID WndDefectID=List[i];
		if ( 0 < ExistDefectItems.GetItemCount(WndDefectID) )
		{	ModelAddWndParam.SetEnable(WndDefectID, true);	}
	}
	AOIDataCollect.SaveModelDefaultWndParam(ModelAddWndParam, MDW_VERSION_1);
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnLevelBasicBtn() 
{
	// TODO: Add your control notification handler code here
	m_ModelDefectItems=m_ModelBasicDefectItems;//僅用部分
	ParamToUI(true);
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnLevelAdvanceBtn() 
{
	// TODO: Add your control notification handler code here
	m_ModelDefectItems.SetAll(1);//全用
	ParamToUI(true);
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnSelchangeChipSizeCombo() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnUseDefaultModelChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MAAW_USE_DEFAULT_MODEL_CHK);	
	if ( TRUE == bCheck )
	{	m_ModelAddWndParam.bUseDefaultModel = true; }
	else
	{	m_ModelAddWndParam.bUseDefaultModel = false; }
	m_ModelDefaultModel.SetAll(true);
	if ( TRUE == bCheck )
	{
		CAOIModel DefaultModel;
		CString   GroupName=m_ModelAddWndParam.sGroupName;
		CString   Filename = CAOIModel::GetModelDefaultFilename(GroupName);		
		if ( DefaultModel.LoadModelParamFile(Filename) == true )		
		{	m_ModelDefaultModel = DefaultModel.CheckModelTestDefectItems();	}		
	}
	UpdateUseDefaultModel();
	UpdateUse3DLight();
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::UpdateUse3DLight()
{
	const bool bUse3DLight = m_ModelAddWndParam.bUse3DLight;
	if ( true == bUse3DLight ) { return ; }
	
	BOOL bCheck = FALSE;
	//零件對齊
	JetAPI::SetComboxCurSel(m_PartAlignModeCombox, PART_ALIGN_MODE_BY_MODEL_MATCH_2D);
	JetAPI::EnableCtrlWnd(this, MAAW_PART_ALIGN_MODE_COMBO, bCheck);

	//本體缺件
	CWnd::CheckDlgButton(MAAW_BODY_MISSING_CHK, bCheck);

	//本體傾斜
	CWnd::CheckDlgButton(MAAW_BODY_TILT_CHK, bCheck);
	JetAPI::EnableCtrlWnd(this, MAAW_BODY_TILT_CHK, bCheck);

	//翹腳
	CWnd::CheckDlgButton(MAAW_LEAD_LIFTED_CHK, bCheck);
	JetAPI::EnableCtrlWnd(this, MAAW_LEAD_LIFTED_CHK, bCheck);		

	//使用3D部分		
	CWnd::CheckDlgButton(MAAW_BRIDGE_USE_3D_CHK, bCheck);
	JetAPI::EnableCtrlWnd(this, MAAW_BRIDGE_USE_3D_CHK, bCheck);
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::UpdateUseDefaultModel()
{
	bool bEnable = true;
	const bool bUsed = m_ModelAddWndParam.bUseDefaultModel;
	if ( true == bUsed )
	{	bEnable = false; }
	else
	{	bEnable = true; }
	JetAPI::EnableCtrlWnd(this, MAAW_PAD_ALIGN_EXTEND_EDIT, bEnable);

	JetAPI::EnableCtrlWnd(this, MAAW_PART_ALIGN_MODE_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_PART_ALIGN_EXTEND_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_PART_ALIGN_HEIGHT_TOL_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_PART_ALIGN_OFFSET_LIMIT_EDIT, bEnable);

	JetAPI::EnableCtrlWnd(this, MAAW_BODY_MISSING_MODE_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_BODY_MISSING_HEIGHT_TOL_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_BODY_TILT_HEIGHT_TOL_EDIT, bEnable);

	JetAPI::EnableCtrlWnd(this, MAAW_BRIDGE_USE_2D_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_BRIDGE_USE_3D_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_BRIDGE_END_WND_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, MAAW_BRIDGE_EXTEND_EDIT, bEnable);

	JetAPI::EnableCtrlWnd(this, MAAW_USE_DEFAULT_MODEL_ALL_DEFECT_CHK, bUsed);

	//勾選部分	
	const bool bSetChk = bUsed;
	const std::map<UINT, WND_DEFECT_ID> &Map=m_CtrlIdMapDefectId;
	for ( auto iter=Map.begin(); iter!=Map.end(); ++iter )
	{
		UINT CtrlID=iter->first;
		WND_DEFECT_ID WndDefectID=iter->second;
		const int nValue=m_ModelDefaultModel.GetItemCount(WndDefectID);
		EnableCheckWnd(CtrlID, nValue, bSetChk);
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::EnableCheckWnd(UINT CtrlID, int nWnds, bool SetChk)
{	
	bool bEnable =  (nWnds>0) ? true:false;
	//JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);	
	if ( true == SetChk )
	{	CWnd::CheckDlgButton(CtrlID, bEnable); }
	else
	{	//只取消確認方框
		if ( false == bEnable ) 
		{	CWnd::CheckDlgButton(CtrlID, bEnable); }
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CModelAddAllWnd::OnShowAllGroupBtn() 
{
	// TODO: Add your control notification handler code here
	CModelGroupWnd Wnd;	
	Wnd.SetDefaultImageName(m_ModelImageFilename);
	Wnd.SetDefaultModelType(m_ModelAddWndParam.eModelType);
	Wnd.SetDefaultGroupName(m_ModelAddWndParam.sGroupName);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return; }

	CWnd::CheckDlgButton(MAAW_USE_DEFAULT_MODEL_CHK, TRUE);

	CString ImageName = Wnd.GetImageNameSelected();
	CString GroupName = Wnd.GetGroupNameSelected();
	if ( m_ModelAddWndParam.sGroupName.CompareNoCase(GroupName) != 0 )
	{			
		m_ModelAddWndParam.sGroupName = GroupName;	
		CWnd::SetDlgItemText(MAAW_GROUP_NAME_EDIT, GroupName);
	}
	if ( m_ModelImageFilename.CompareNoCase(ImageName) != 0 )
	{
		m_ModelImageFilename = ImageName;
		BuildModleImageWnd();
	}
	OnUseDefaultModelChk();	
}
//-------------------------------------------------------------------------------------//