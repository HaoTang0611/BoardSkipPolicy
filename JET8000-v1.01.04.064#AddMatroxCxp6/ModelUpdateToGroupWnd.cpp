// ModelUpdateToGroupWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ModelUpdateToGroupWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelUpdateToGroupWnd dialog
//-------------------------------------------------------------------------------------//
CModelUpdateToGroupWnd::CModelUpdateToGroupWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CModelUpdateToGroupWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelUpdateToGroupWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ModelPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelUpdateToGroupWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CModelUpdateToGroupWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CModelUpdateToGroupWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(MUTG_UPDATE_ALL_RADIO, OnUpdateAllRadio)
	ON_BN_CLICKED(MUTG_UPDATE_SEL_RADIO, OnUpdateSelRadio)
	ON_BN_CLICKED(MUTG_UPDATE_ADD_ONE_CHK, OnUpdateAddOneChk)
	ON_BN_CLICKED(MUTG_UPDATE_DELETE_CHK, OnUpdateDeleteChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelUpdateToGroupWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CModelUpdateToGroupWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	UpdateParamToUI();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::SetModelPtr(CAOIModel *ModelPtr)
{
	m_ModelPtr = ModelPtr;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CModelUpdateToGroupWnd::GetModelPtr()
{
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::SetUpdateParam(TModelUpdateToGroupParam &Param)
{
	m_UpdateParam = Param;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::GetUpdateParam(TModelUpdateToGroupParam &Param)
{
	Param = m_UpdateParam;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MODEL_UPDATE_TO_GROUP_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MODEL_UPDATE_TO_GROUP_WND;
	WndKey = _T("IDD_MODEL_UPDATE_TO_GROUP_WND");
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
	WndID = MUTG_MODEL_NAME_LABEL;
	WndKey = _T("MUTG_MODEL_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_GROUP_NAME_LABEL;
	WndKey = _T("MUTG_GROUP_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_WND_SELECTED_LABEL;
	WndKey = _T("MUTG_WND_SELECTED_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_ALL_RADIO;
	WndKey = _T("MUTG_UPDATE_ALL_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_SEL_RADIO;
	WndKey = _T("MUTG_UPDATE_SEL_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_SETTING_GROUP;
	WndKey = _T("MUTG_UPDATE_SETTING_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_PARAM_CHK;
	WndKey = _T("MUTG_UPDATE_PARAM_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_BINARY_CHK;
	WndKey = _T("MUTG_UPDATE_BINARY_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_WND_SIZE_CHK;
	WndKey = _T("MUTG_UPDATE_WND_SIZE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_ADD_ONE_CHK;
	WndKey = _T("MUTG_UPDATE_ADD_ONE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_UPDATE_DELETE_CHK;
	WndKey = _T("MUTG_UPDATE_DELETE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = MUTG_KEEP_SPEC_CHK;
	WndKey = _T("MUTG_KEEP_SPEC_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::UpdateParamToUI()
{
	BOOL       bEnabled=TRUE;	
	CString    strModelName;
	CString    strGroupName;
	CString    strWndSelected=_T("0");
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL != ModelPtr )
	{
		strModelName = ModelPtr->GetModelName();
		strGroupName = ModelPtr->GetModelGroupName();
		strWndSelected.Format(_T("%d"), ModelPtr->GetModelWndSelectedCount());
	}
	CWnd::SetDlgItemText(MUTG_MODEL_NAME_EDIT, strModelName);
	CWnd::SetDlgItemText(MUTG_GROUP_NAME_EDIT, strGroupName);
	CWnd::SetDlgItemText(MUTG_WND_SELECTED_EDIT, strWndSelected);

	if ( true==m_UpdateParam.bUpdateAll ) 
	{
		bEnabled=FALSE;
		CWnd::CheckDlgButton(MUTG_UPDATE_ALL_RADIO, TRUE);
		CWnd::CheckDlgButton(MUTG_UPDATE_SEL_RADIO, FALSE);		
	}
	else
	{
		bEnabled=TRUE;
		CWnd::CheckDlgButton(MUTG_UPDATE_ALL_RADIO, FALSE);
		CWnd::CheckDlgButton(MUTG_UPDATE_SEL_RADIO, TRUE);
	}

	JetAPI::EnableCtrlWnd(this, MUTG_UPDATE_PARAM_CHK, bEnabled);
	JetAPI::EnableCtrlWnd(this, MUTG_UPDATE_BINARY_CHK, bEnabled);
	JetAPI::EnableCtrlWnd(this, MUTG_UPDATE_WND_SIZE_CHK, bEnabled);
	JetAPI::EnableCtrlWnd(this, MUTG_UPDATE_ADD_ONE_CHK, bEnabled);	
	JetAPI::EnableCtrlWnd(this, MUTG_UPDATE_DELETE_CHK, bEnabled);		
	JetAPI::EnableCtrlWnd(this, MUTG_KEEP_SPEC_CHK, bEnabled);

	CWnd::CheckDlgButton(MUTG_UPDATE_PARAM_CHK, m_UpdateParam.bUpdateParam);
	CWnd::CheckDlgButton(MUTG_UPDATE_BINARY_CHK, m_UpdateParam.bUpdateBinary);
	CWnd::CheckDlgButton(MUTG_UPDATE_WND_SIZE_CHK, m_UpdateParam.bUpdateWndSize);
	CWnd::CheckDlgButton(MUTG_UPDATE_ADD_ONE_CHK, m_UpdateParam.bUpdateAddOne);
	CWnd::CheckDlgButton(MUTG_UPDATE_DELETE_CHK, m_UpdateParam.bUpdateDelete);		
	CWnd::CheckDlgButton(MUTG_KEEP_SPEC_CHK, m_UpdateParam.bKeepSpec);			
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::UpdateUIToParam()
{
	BOOL bCheck=TRUE;

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_ALL_RADIO);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateAll = true; }
	else
	{	m_UpdateParam.bUpdateAll = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_PARAM_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateParam = true; }
	else
	{	m_UpdateParam.bUpdateParam = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_BINARY_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateBinary = true; }
	else
	{	m_UpdateParam.bUpdateBinary = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_WND_SIZE_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateWndSize = true; }
	else
	{	m_UpdateParam.bUpdateWndSize = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_ADD_ONE_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateAddOne = true; }
	else
	{	m_UpdateParam.bUpdateAddOne = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_DELETE_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bUpdateDelete = true; }
	else
	{	m_UpdateParam.bUpdateDelete = false; }

	bCheck = CWnd::IsDlgButtonChecked(MUTG_KEEP_SPEC_CHK);
	if ( TRUE == bCheck ) 
	{	m_UpdateParam.bKeepSpec = true; }
	else
	{	m_UpdateParam.bKeepSpec = false; }	
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnUpdateAllRadio() 
{
	// TODO: Add your control notification handler code here
	m_UpdateParam.bUpdateAll = true;
	UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnUpdateSelRadio() 
{
	// TODO: Add your control notification handler code here
	m_UpdateParam.bUpdateAll = false;
	UpdateParamToUI();
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateUIToParam();
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnUpdateAddOneChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_ADD_ONE_CHK);
	if ( TRUE == bCheck )
	{	CWnd::CheckDlgButton(MUTG_UPDATE_DELETE_CHK, FALSE);	}
}
//-------------------------------------------------------------------------------------//
void CModelUpdateToGroupWnd::OnUpdateDeleteChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MUTG_UPDATE_DELETE_CHK);
	if ( TRUE == bCheck )
	{
		CWnd::CheckDlgButton(MUTG_UPDATE_PARAM_CHK, FALSE);
		CWnd::CheckDlgButton(MUTG_UPDATE_BINARY_CHK, FALSE);
		CWnd::CheckDlgButton(MUTG_UPDATE_WND_SIZE_CHK, FALSE);
		CWnd::CheckDlgButton(MUTG_UPDATE_ADD_ONE_CHK, FALSE);	
		CWnd::CheckDlgButton(MUTG_KEEP_SPEC_CHK, FALSE);			
	}
}
//-------------------------------------------------------------------------------------//