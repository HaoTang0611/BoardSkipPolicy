// EditImageProcessPage.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditImageProcessPage.h"
//-------------------------------------------------------------------------------------//
#include "AlgImageSourceWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditImageProcessPage dialog
//-------------------------------------------------------------------------------------//
CEditImageProcessPage::CEditImageProcessPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImageProcessPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageProcessPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;	
	JetAPI::InitialUUID(m_uidWnd);
	m_wndImageBinary.SetBinaryParam(m_WndPtr, &m_BinaryParam, false);
	m_wndColorFilter.SetColorFilterParam(m_WndPtr, &m_BinaryParam, WND_DEFECT_NONE, false);	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageProcessPage)
	DDX_Control(pDX, IMGPRO_FILTER_MODE_COMBO2, m_FilterModeCombox2);
	DDX_Control(pDX, IMGPRO_FILTER_PARAM_COMBO2, m_FilterParamCombox2);	
	DDX_Control(pDX, IMGPRO_FILTER_MODE_COMBO, m_FilterModeCombox1);
	DDX_Control(pDX, IMGPRO_FILTER_PARAM_COMBO, m_FilterParamCombox1);	
	DDX_Control(pDX, IMGPRO_BINARY_MODE_COMBO, m_BinaryModeCombox);
	DDX_Control(pDX, IMGPRO_IMAGE_SOURCE_COMBO, m_ImageSourceCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageProcessPage, CDialog)
	//{{AFX_MSG_MAP(CEditImageProcessPage)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IMGPRO_IMAGE_SOURCE_COMBO, OnSelchangeImageSourceCombo)
	ON_CBN_SELCHANGE(IMGPRO_BINARY_MODE_COMBO, OnSelchangeBinaryModeCombo)
	ON_CBN_SELCHANGE(IMGPRO_FILTER_MODE_COMBO, OnSelchangeFilterModeCombo)
	ON_CBN_SELCHANGE(IMGPRO_FILTER_PARAM_COMBO, OnSelchangeFilterParamCombo)	
	ON_CBN_SELCHANGE(IMGPRO_FILTER_MODE_COMBO2, OnSelchangeFilterModeCombo2)
	ON_CBN_SELCHANGE(IMGPRO_FILTER_PARAM_COMBO2, OnSelchangeFilterParamCombo2)	
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IMGPRO_GRAY_INVERT_CHK, OnGrayInvertChk)
	ON_BN_CLICKED(IMGPRO_BINARY_INVERT_CHK, OnBinaryInvertChk)
	ON_BN_CLICKED(IMGPRO_SHOW_RAW_IMAGE_CHK, OnShowRawImageChk)
	ON_EN_KILLFOCUS(IMGPRO_GAIN_PARAM_EDIT, OnKillfocusGainParamEdit)
	ON_BN_CLICKED(IMGPRO_GAIN_PARAM_CHK, OnGainParamChk)
	ON_EN_CHANGE(IMGPRO_GAIN_PARAM_EDIT, OnChangeGainParamEdit)	
	ON_BN_CLICKED(IMGPRO_SHOW_ALL_BTN, OnShowAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageProcessPage message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageProcessPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_wndImageBinary.Create(IDD_EDIT_IMAGE_BINARY_WND,this) == FALSE )
	{
		TRACE(_T("Error, Create Image Binary Wnd Fault"));
		return FALSE;
	}
	if ( m_wndColorFilter.Create(IDD_EDIT_IMAGE_COLOR_FILTER_WND,this) == FALSE )
	{
		TRACE(_T("Error, Create Color Filter Wnd Fault"));
		return FALSE;
	}
	if ( m_wndImagePattern.Create(IDD_EDIT_IMAGE_PATTERN_WND, this) == FALSE )
	{
		TRACE(_T("Error, Create Image Pattern Wnd Fault"));
		return FALSE;
	}

	SwitchMultiLanguage();	
	m_StopUpdateGainValue=true;
	CWnd::SetDlgItemText(IMGPRO_GAIN_PARAM_EDIT, _T("1.0"));
	m_StopUpdateGainValue=false;
	CWnd::CheckDlgButton(IMGPRO_BINARY_INVERT_CHK, m_BinaryParam.GetBinaryInvert());
	CWnd::CheckDlgButton(IMGPRO_GRAY_INVERT_CHK, m_BinaryParam.GetGrayInvert());
	CWnd::CheckDlgButton(IMGPRO_GAIN_PARAM_CHK, m_BinaryParam.GetGrayGainEnabled());
	
	AdjustUIForWndSelected();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return; }
	const int ncy2 = cy/2;
	const int nTab = 80;//80
	const int nBot = ncy2-nTab+40;
	if ( m_wndImageBinary.GetSafeHwnd() != NULL )
	{	m_wndImageBinary.MoveWindow(0, nTab, cx, nBot);	}
	if ( m_wndColorFilter.GetSafeHwnd() != NULL )
	{	m_wndColorFilter.MoveWindow(0, nTab, cx, nBot);	}
	const int nStartY = nTab+nBot+8;
	if ( m_wndImagePattern.GetSafeHwnd() != NULL )
	{	m_wndImagePattern.MoveWindow(0, nStartY, cx, cy-nStartY);	}	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_PROCESS_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_IMAGE_PROCESS_WND;
	WndKey = _T("IDD_EDIT_IMAGE_PROCESS_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	m_WindowText = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = IMGPRO_IMAGE_SOURCE_LABEL;
	WndKey = _T("IMGPRO_IMAGE_SOURCE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IMGPRO_BINARY_MODE_LABEL;
	WndKey = _T("IMGPRO_BINARY_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_FILTER_MODE_LABEL;
	WndKey = _T("IMGPRO_FILTER_MODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_FILTER_PARAM_LABEL;
	WndKey = _T("IMGPRO_FILTER_PARAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_FILTER_MODE_LABEL2;
	WndKey = _T("IMGPRO_FILTER_MODE_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_FILTER_PARAM_LABEL2;
	WndKey = _T("IMGPRO_FILTER_PARAM_LABEL2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_BINARY_INVERT_CHK;
	WndKey = _T("IMGPRO_BINARY_INVERT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IMGPRO_SHOW_RAW_IMAGE_CHK;
	WndKey = _T("IMGPRO_SHOW_RAW_IMAGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_GAIN_PARAM_CHK;
	WndKey = _T("IMGPRO_GAIN_PARAM_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPRO_GRAY_INVERT_CHK;
	WndKey = _T("IMGPRO_GRAY_INVERT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = IMGPRO_SHOW_ALL_BTN;
	WndKey = _T("IMGPRO_SHOW_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CString CEditImageProcessPage::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_PROCESS_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::ChangeDrawModelMode()
{
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
}
//-------------------------------------------------------------------------------------//
DRAW_MODEL_MODE CEditImageProcessPage::GetDrawModelMode() const
{
	return DRAW_MODEL_EDIT;

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	
	return DrawModelMode;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{	
	HWND hWnd = NULL;
	CWnd *WndPtr = CWnd::GetOwner();
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		hWnd = WndPtr->GetSafeHwnd();
		::PostMessage(hWnd, message, wParam, lParam);
	}
	return;
	hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);		
	//AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = NULL;
	CWnd *WndPtr = CWnd::GetOwner();
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		hWnd = WndPtr->GetSafeHwnd();
		::SendMessage(hWnd, message, wParam, lParam);
	}
	return ;
	hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);		
	//AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeImageSourceCombo() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	BINARY_MODE BinaryMode = m_BinaryParam.GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = (IMAGE_SRC_MODE)(JetAPI::GetComboxCurSelData(m_ImageSourceCombox));

	CWnd::GetDlgItemText(IMGPRO_IMAGE_SOURCE_LABEL, strValueName);
	strValueNew = AOIDataDefine.GetAlgImageSourceModeText(ImageSourceMode);
	strValueOld = AOIDataDefine.GetAlgImageSourceModeText(m_BinaryParam.GetBinaryImageSourceMode());
	if ( IMAGE_SRC_COLOR == m_BinaryParam.GetBinaryImageSourceMode()  )
	{
		if ( IMAGE_SRC_COLOR!=ImageSourceMode && BinaryMode==BINARY_COLOR_FILTER )
		{	BinaryMode = BINARY_DISABLE; }
	}
	else
	{
		if ( IMAGE_SRC_COLOR == ImageSourceMode )
		{	
			BinaryMode = BINARY_DISABLE; 
			BinaryMode = BINARY_COLOR_FILTER; 
		}
	}
	m_BinaryParam.SetBinaryImageSourceMode(ImageSourceMode);
	if ( BinaryMode != m_BinaryParam.GetBinaryMode() )
	{
		m_BinaryParam.SetBinaryMode(BinaryMode);
		JetAPI::SetComboxCurSel(m_BinaryModeCombox, BinaryMode);
		CEditImageProcessPage::OnSelchangeBinaryModeCombo();
	}
	else
	{	m_wndImageBinary.UpdateBinaryWndEnable(); }
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);	

	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeBinaryModeCombo() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	CAOIProject *ProjectPtr = GetActiveProject();
	const int OldFrameIndex = m_BinaryParam.GetBinaryFrameIndex();
	BINARY_MODE BinaryMode = (BINARY_MODE)(JetAPI::GetComboxCurSelData(m_BinaryModeCombox));	

	CWnd::GetDlgItemText(IMGPRO_BINARY_MODE_LABEL, strValueName);
	strValueNew = AOIDataDefine.GetAlgBinaryModeText(BinaryMode);
	strValueOld = AOIDataDefine.GetAlgBinaryModeText(m_BinaryParam.GetBinaryMode());
	switch ( BinaryMode )
	{
	case BINARY_COLOR_FILTER:
		m_wndImageBinary.ShowWindow(SW_HIDE);
		m_wndColorFilter.ShowWindow(SW_SHOW);
		break;
	case BINARY_DISABLE:
	case BINARY_FIXED_THRESHOLD:
	case BINARY_DYNAMIC_THRESHOLD:
	case BINARY_RELATIVE_AVE_THRESHOLD:
	case BINARY_ADAPTIVE_THRESHOLD:
		m_wndColorFilter.ShowWindow(SW_HIDE);
		m_wndImageBinary.ShowWindow(SW_SHOW);
	   break;	
	}		
	m_BinaryParam.SetBinaryMode(BinaryMode);	
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	if ( BinaryMode==BINARY_COLOR_FILTER && NULL!=ProjectPtr )
	{		
		const int LinkIndex = m_BinaryParam.GetBinaryColorGroupLinkIndex();
		CColorGroup *ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(LinkIndex, true);
		if ( NULL != ColorGroupPtr )
		{	m_BinaryParam.SetBinaryColorGroup(*ColorGroupPtr);	}
	}
	m_wndImageBinary.UpdateBinaryWndEnable();
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);

	const int NewFrameIndex = m_BinaryParam.GetBinaryFrameIndex();
	if ( NewFrameIndex != OldFrameIndex )
	{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL); }	
	return;	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeFilterModeCombo() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	NOISE_FILTER_MODE NoiseFilterMode = (NOISE_FILTER_MODE)(JetAPI::GetComboxCurSelData(m_FilterModeCombox1));
	TBINARY_FILTER NoiseFilter = m_BinaryParam.GetBinaryNoiseFilter1();

	CWnd::GetDlgItemText(IMGPRO_FILTER_MODE_LABEL, strValueName);
	strValueNew = AOIDataDefine.GetAlgNoiseFilterModeText(NoiseFilterMode);
	strValueOld = AOIDataDefine.GetAlgNoiseFilterModeText(NoiseFilter.FilterMode);	
	NoiseFilter.FilterMode = NoiseFilterMode;	
	m_BinaryParam.SetBinaryNoiseFilter1(NoiseFilter);
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeFilterParamCombo()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	int Param = (int)(JetAPI::GetComboxCurSelData(m_FilterParamCombox1));
	TBINARY_FILTER NoiseFilter = m_BinaryParam.GetBinaryNoiseFilter1();

	CWnd::GetDlgItemText(IMGPRO_FILTER_PARAM_LABEL, strValueName);
	strValueNew.Format(_T("%d"), Param);
	strValueOld.Format(_T("%d"), NoiseFilter.FilterParam1);
	NoiseFilter.FilterParam1 = Param;	
	m_BinaryParam.SetBinaryNoiseFilter1(NoiseFilter);
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeFilterModeCombo2() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	NOISE_FILTER_MODE NoiseFilterMode = (NOISE_FILTER_MODE)(JetAPI::GetComboxCurSelData(m_FilterModeCombox2));
	TBINARY_FILTER NoiseFilter = m_BinaryParam.GetBinaryNoiseFilter2();

	CWnd::GetDlgItemText(IMGPRO_FILTER_MODE_LABEL2, strValueName);
	strValueNew = AOIDataDefine.GetAlgNoiseFilterModeText(NoiseFilterMode);
	strValueOld = AOIDataDefine.GetAlgNoiseFilterModeText(NoiseFilter.FilterMode);	
	NoiseFilter.FilterMode = NoiseFilterMode;	
	m_BinaryParam.SetBinaryNoiseFilter2(NoiseFilter);
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);

	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnSelchangeFilterParamCombo2()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	int Param = (int)(JetAPI::GetComboxCurSelData(m_FilterParamCombox2));
	TBINARY_FILTER NoiseFilter = m_BinaryParam.GetBinaryNoiseFilter2();

	CWnd::GetDlgItemText(IMGPRO_FILTER_PARAM_LABEL2, strValueName);
	strValueNew.Format(_T("%d"), Param);
	strValueOld.Format(_T("%d"), NoiseFilter.FilterParam1);

	NoiseFilter.FilterParam1 = Param;	
	m_BinaryParam.SetBinaryNoiseFilter2(NoiseFilter);
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);

	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageProcessPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_CLOSE:
			CloseProject();
			break;
		case WPARAM_PROJECT_SWITCH:			
			UpdateWndSelected();
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{	UpdateWndSelected(); }
			}
			break;		
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
		default:			
			break;
		}		
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_IMAGE_MODEL_SELECTED:
			if ( CWnd::IsWindowVisible() == TRUE )
			{	UpdateWndSelected(); }
			break;
		case WPARAM_UPDATE_IMAGE_WND_SELECTED:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				if ( UpdateWndSelectedKernel((CAOIWnd*)lParam) == false )
				{	ResetWndAlgParam(); }
				else
				{
					BOOL bCheck = CWnd::IsDlgButtonChecked(IMGPRO_SHOW_RAW_IMAGE_CHK);
					if ( FALSE == bCheck )
					{
						AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
						AOIDataCollect.SetDrawImageMode(DRAW_IAMGE_BY_ALG);	
						//PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
						UpdateAlgParamImage(false, true);
					}			
				}
			}
			break;
		case WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS:
		case WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS:
			break;
		}
		break;
	case MSG_EDIT_IMAGE_PROCESS_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_WND_ALG:			
			if ( UpdateWndSelectedKernel((CAOIWnd*)lParam) == false )
			{	ResetWndAlgParam(); }
			else
			{
				BOOL bCheck = CWnd::IsDlgButtonChecked(IMGPRO_SHOW_RAW_IMAGE_CHK);
				if ( FALSE == bCheck )
				{
					AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
					AOIDataCollect.SetDrawImageMode(DRAW_IAMGE_BY_ALG);	
					//PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
					UpdateAlgParamImage(false, true);
				}			
			}
			break;
		case WPARAM_UPDATE_ALG_COLOR_FILTER:
			if ( UpdateWndSelectedKernel((CAOIWnd*)lParam) == false )
			{	ResetWndAlgParam(); }
			break;
		case WPARAM_UPDATE_ALG_PARAM:
			if ( TRUE == lParam )
			{	UpdateAlgParamImage(true, true); }
			else
			{	UpdateAlgParamImage(false, false); }
			break;
		case WPARAM_UPDATE_ALG_RESULT:
			UpdateAlgResult();
			break;
		case WPARAM_UPDATE_GATHER_COLOR_CHK:
			if ( m_wndColorFilter.GetSafeHwnd()!=NULL )
			{	m_wndColorFilter.UpdateGatherColorCheckButton(); }
			break;
		case WPARAM_PATTERN_ADD:
			if ( m_wndImagePattern.GetSafeHwnd()!=NULL )
			{	m_wndImagePattern.ExecAddPattern();	}
			break;
		case WPARAM_PATTERN_TEXT:
			if ( m_wndImagePattern.GetSafeHwnd()!=NULL )
			{	m_wndImagePattern.ExecTextPattern();	}
			break;
		default:
			//UpdateWndSelected();
			break;
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::CloseProject()
{
	m_ProjectPtr = NULL;
	ClearWndSelected();
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditImageProcessPage::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::AdjustUIForWndSelected()
{
	FRAME_TYPE FrameType = FRAME_COLOR;
	const size_t FrameUniqueID = m_BinaryParam.GetBinaryFrameUniqueID();
	TFrameParam* FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( NULL != FrameParamPtr )
	{	FrameType = FrameParamPtr->FrameType; }
	TBINARY_FILTER NoiseFilter1 = m_BinaryParam.GetBinaryNoiseFilter1();
	TBINARY_FILTER NoiseFilter2 = m_BinaryParam.GetBinaryNoiseFilter2();
	AOIDataDefine.BuildImageSourceModeCombox(m_ImageSourceCombox, FrameType);
	AOIDataDefine.BuildBinaryModeCombox(m_BinaryModeCombox, FrameType);	
	AOIDataDefine.BuildBinaryFilterParamCombox(m_FilterParamCombox1);
	AOIDataDefine.BuildBinaryFilterParamCombox(m_FilterParamCombox2);
	AOIDataDefine.BuildBinaryFilterModeCombox(m_FilterModeCombox1, FrameType);
	AOIDataDefine.BuildBinaryFilterModeCombox(m_FilterModeCombox2, FrameType);	
	JetAPI::SetComboxCurSel(m_ImageSourceCombox, m_BinaryParam.GetBinaryImageSourceMode());
	JetAPI::SetComboxCurSel(m_BinaryModeCombox, m_BinaryParam.GetBinaryMode());
	JetAPI::SetComboxCurSel(m_FilterModeCombox1, NoiseFilter1.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox1, NoiseFilter1.FilterParam1);
	JetAPI::SetComboxCurSel(m_FilterModeCombox2, NoiseFilter2.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox2, NoiseFilter2.FilterParam1);
	switch ( m_BinaryParam.GetBinaryMode() )
	{
	//case BINARY_DISABLE:
	case BINARY_COLOR_FILTER:
		m_wndImageBinary.ShowWindow(SW_HIDE);
		m_wndColorFilter.ShowWindow(SW_SHOW);
		break;
	default:
		m_wndColorFilter.ShowWindow(SW_HIDE);
		m_wndImageBinary.ShowWindow(SW_SHOW);
		break;
	}

	CAOIWnd *WndPtr = m_WndPtr;
	UINT ShowMode = SW_SHOW;
	if ( NULL == WndPtr )
	{	ShowMode = SW_HIDE; }
	else
	{
		ALG_TYPE AlgType = WndPtr->GetWndAlgType();
		const bool UseImage = CAlgParam::CheckAlgPatternFileUsed(AlgType);
		if ( false == UseImage ) 
		{	ShowMode = SW_HIDE; }
		else
		{	ShowMode = SW_SHOW; }
	}
	m_wndImagePattern.ShowWindow(ShowMode);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::ClearWndSelected()
{
	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_BinaryParam = CAlgBinaryParam();
	AdjustUIForWndSelected();	

	TBINARY_FILTER NoiseFilter1 = m_BinaryParam.GetBinaryNoiseFilter1();
	TBINARY_FILTER NoiseFilter2 = m_BinaryParam.GetBinaryNoiseFilter2();
	JetAPI::SetComboxCurSel(m_ImageSourceCombox, m_BinaryParam.GetBinaryImageSourceMode());
	JetAPI::SetComboxCurSel(m_BinaryModeCombox, m_BinaryParam.GetBinaryMode());
	JetAPI::SetComboxCurSel(m_FilterModeCombox1, NoiseFilter1.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox1, NoiseFilter1.FilterParam1);
	JetAPI::SetComboxCurSel(m_FilterModeCombox2, NoiseFilter2.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox2, NoiseFilter2.FilterParam1);
	CWnd::CheckDlgButton(IMGPRO_BINARY_INVERT_CHK, m_BinaryParam.GetBinaryInvert());

	m_StopUpdateGainValue=true;
	CWnd::SetDlgItemText(IMGPRO_GAIN_PARAM_EDIT, _T("1.0"));
	m_StopUpdateGainValue=false;
	CWnd::CheckDlgButton(IMGPRO_GRAY_INVERT_CHK, m_BinaryParam.GetGrayInvert());
	CWnd::CheckDlgButton(IMGPRO_GAIN_PARAM_CHK, m_BinaryParam.GetGrayGainEnabled());

	m_wndImageBinary.SetBinaryParam(m_WndPtr, &m_BinaryParam, true);
	m_wndColorFilter.SetColorFilterParam(m_WndPtr, &m_BinaryParam, WND_DEFECT_NONE, true);		
	m_wndImagePattern.SetImagePatternWndPtr(m_WndPtr, true);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::ResetWndAlgParam()
{	
	ClearWndSelected();
	LockUIWnd(true);
	return;

	DRAW_IMAGE_MODE DrawingImageMode = AOIDataCollect.GetDrawingImageMode();
	if ( DRAW_IAMGE_BY_ALG == DrawingImageMode )//導致每次切換零件都會重新取像, 效果不好
	{	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL); }
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndSelected()
{
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		UpdateWndSelected_Fd();
		break;
	case MODEL_ATTACHED_MARK:
		UpdateWndSelected_Mark();
		break;
	case MODEL_ATTACHED_BARCODE:
		UpdateWndSelected_Barcode();
		break;
	case MODEL_ATTACHED_COMPONENT:
		UpdateWndSelected_Component();
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndSelected_Fd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true==bLockUIWnd ) { return; }

	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();	
	if ( NULL == ProjectPtr ) { ResetWndAlgParam(); return; }
	CAOIFd *FdPtr = ProjectPtr->GetProjectActiveFd();
	if ( NULL == FdPtr ) { ResetWndAlgParam(); return ; }
	CAOIModel     *ModelPtr = FdPtr->GetFdModelPtr();
	if ( NULL == ModelPtr ) { ResetWndAlgParam(); return; }
	m_ProjectPtr = ProjectPtr;
	m_ModelPtr = ModelPtr;
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	if ( UpdateWndSelectedKernel(WndPtr) == false )
	{	ResetWndAlgParam(); }	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndSelected_Mark()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true==bLockUIWnd ) { return; }

	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));		
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr ) { ResetWndAlgParam(); return; }
	CAOIMark *pMark = ProjectPtr->GetProjectActiveMark();
	if ( NULL == pMark ) { ResetWndAlgParam(); return ; }
	CAOIModel     *ModelPtr = pMark->GetMarkModelPtr();
	if ( NULL == ModelPtr ) { ResetWndAlgParam(); return; }
	m_ProjectPtr = ProjectPtr;
	m_ModelPtr = ModelPtr;
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	if ( UpdateWndSelectedKernel(WndPtr) == false )
	{	ResetWndAlgParam(); }	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndSelected_Barcode()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true==bLockUIWnd ) { return; }

	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));		
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr ) { ResetWndAlgParam(); return; }
	CAOIBarcode *pBarcode = ProjectPtr->GetProjectActiveBarcode();
	if ( NULL == pBarcode ) { ResetWndAlgParam(); return ; }
	CAOIModel     *ModelPtr = pBarcode->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { ResetWndAlgParam(); return; }
	m_ProjectPtr = ProjectPtr;
	m_ModelPtr = ModelPtr;
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	if ( UpdateWndSelectedKernel(WndPtr) == false )
	{	ResetWndAlgParam(); }	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndSelected_Component()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true==bLockUIWnd ) { return; }

	m_WndPtr = NULL;
	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveTaskProject();
	if ( NULL == ProjectPtr ) { ResetWndAlgParam(); return; }
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == pComponent ) { ResetWndAlgParam(); return ; }
	CAOIModel     *ModelPtr = pComponent->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { ResetWndAlgParam(); return; }
	m_ProjectPtr = ProjectPtr;
	m_ModelPtr = ModelPtr;
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	if ( UpdateWndSelectedKernel(WndPtr) == false )
	{	ResetWndAlgParam(); }	
}
//-------------------------------------------------------------------------------------//
bool CEditImageProcessPage::UpdateWndSelectedKernel(CAOIWnd *WndPtr)
{	
	bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return true; }

	if ( NULL == WndPtr ) {	return false;	}	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) {	return false;	}	
	CAOIModel   *ModelPtr=WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	CAOIRgn     *ModelAttachedPtr = ModelPtr->GetModelAttachedPtr();	
	if ( NULL == ModelAttachedPtr ) { return false; }

	CString         str;
	bool            NewWndObj = false;	
	const UUID      uuid = WndPtr->GetObjUuid();	
	WND_DEFECT_ID   WndDefectID=WndPtr->GetWndDefectID();
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	

	if ( uuid != m_uidWnd )	
	{	NewWndObj = true;	}

	if ( NULL == BinParamPtr )
	{	m_BinaryParam = CAlgBinaryParam();	}
	else
	{	m_BinaryParam = *BinParamPtr;	}

	m_uidWnd = uuid;
	m_WndPtr = WndPtr;
	m_ModelPtr = ModelPtr;
	m_ProjectPtr = ProjectPtr;

	CColorRGBV *rgbvPtr = m_BinaryParam.GetBinaryColorActivePtr();

	AdjustUIForWndSelected();	
	TBINARY_FILTER NoiseFilter1=m_BinaryParam.GetBinaryNoiseFilter1();
	TBINARY_FILTER NoiseFilter2=m_BinaryParam.GetBinaryNoiseFilter2();
	IMAGE_SRC_MODE ImageSrcMode = m_BinaryParam.GetBinaryImageSourceMode();
	JetAPI::SetComboxCurSel(m_ImageSourceCombox, ImageSrcMode);
	JetAPI::SetComboxCurSel(m_BinaryModeCombox, m_BinaryParam.GetBinaryMode());
	JetAPI::SetComboxCurSel(m_FilterModeCombox1, NoiseFilter1.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox1, NoiseFilter1.FilterParam1);
	JetAPI::SetComboxCurSel(m_FilterModeCombox2, NoiseFilter2.FilterMode);
	JetAPI::SetComboxCurSel(m_FilterParamCombox2, NoiseFilter2.FilterParam1);
	CWnd::CheckDlgButton(IMGPRO_BINARY_INVERT_CHK, m_BinaryParam.GetBinaryInvert());

	double GainValue = m_BinaryParam.GetGrayGainValue();
	str.Format(_T("%.1f"), GainValue);	
	m_StopUpdateGainValue=true;
	CWnd::SetDlgItemText(IMGPRO_GAIN_PARAM_EDIT, str);
	m_StopUpdateGainValue=false;
	CWnd::CheckDlgButton(IMGPRO_GRAY_INVERT_CHK, m_BinaryParam.GetGrayInvert());
	CWnd::CheckDlgButton(IMGPRO_GAIN_PARAM_CHK, m_BinaryParam.GetGrayGainEnabled());

	m_wndImageBinary.SetBinaryParam(m_WndPtr, &m_BinaryParam, true);
	m_wndColorFilter.SetColorFilterParam(m_WndPtr, &m_BinaryParam, WndDefectID, true);		
	m_wndImagePattern.SetImagePatternWndPtr(WndPtr, true);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateWndCaptionText(ModelPtr, WndPtr);
	LockUIWnd(false);
	return true;	
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	
		UpdateWndSelected();	
		AOIDataCollect.SetEditImagePageWndID(WPARAM_SHOW_IMAGE_PROCESS_PAGE);
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateAlgResult()
{	
	bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return; }

	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));
	CAOIProject *Project = CEditImageProcessPage::GetActiveProject();
	if ( NULL == Project ) { return; }
	CAOIModel *ModelPtr = m_ModelPtr;	
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtrByUUID(m_uidWnd);
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }

	m_WndPtr   = WndPtr;
	m_ModelPtr = ModelPtr;
	UpdateWndCaptionText(ModelPtr, WndPtr);
	m_wndImageBinary.UpdateBinaryParamToUI();			
	m_wndColorFilter.UpdateFilterListWnd();
	m_wndImagePattern.SetImagePatternWndPtr(WndPtr, true);
}
//-------------------------------------------------------------------------------------//
bool CEditImageProcessPage::UpdateAlgParamImage(bool Modified, bool RedrawAlgImae)
{
	//bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return true; }

	CWnd *FocusWndPtr = this->GetFocus();
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, _T(""));
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = m_ModelPtr;	
	if ( NULL == ModelPtr ) { return false; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtrByUUID(m_uidWnd);//依照唯一碼來確認是對的檢測框
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return false; }
	
	m_WndPtr        = WndPtr;
	UpdateWndCaptionText(ModelPtr, WndPtr);
	m_BinaryParam.UpdateBinaryColorUsed();
	m_BinaryParam.UpdateBinaryColorShowColor();
	BINARY_MODE BinaryMode = m_BinaryParam.GetBinaryMode();
	if ( true == Modified )
	{	ProjectPtr->UpdateProjectColorGroup(m_BinaryParam);	}

	//unsigned int PatIdx = -1;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.SetAlgActiveBinParam(m_BinaryParam);
	//CPatternParam *PatParamPtr = NULL;	
	if ( true == Modified )
	{
		WndPtr->SetWndModified(true);
		ModelPtr->ApplyModelWnd(WndPtr);
		//ChangeDrawModelMode();
	}
	//AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	m_wndImagePattern.SetImagePatternWndPtr(WndPtr, true);

	AOIDataCollect.SetDrawImageMode(DRAW_IAMGE_BY_ALG);	
	CWnd::CheckDlgButton(IMGPRO_SHOW_RAW_IMAGE_CHK, FALSE);

	if ( true == RedrawAlgImae )
	{	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_ALG_IMAGE, NULL); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnGrayInvertChk()
{
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	BOOL bCheck = CWnd::IsDlgButtonChecked(IMGPRO_GRAY_INVERT_CHK);
	CWnd::GetDlgItemText(IMGPRO_GRAY_INVERT_CHK, strValueName);
	strValueNew = AOIDataDefine.GetEnableDisableText(bCheck);
	strValueOld = AOIDataDefine.GetEnableDisableText(!bCheck);
	if ( TRUE == bCheck )
	{	m_BinaryParam.SetGrayInvert(true);	}
	else
	{	m_BinaryParam.SetGrayInvert(false);	}
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnBinaryInvertChk() 
{
	// TODO: Add your control notification handler code here
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	BOOL bCheck = CWnd::IsDlgButtonChecked(IMGPRO_BINARY_INVERT_CHK);
	CWnd::GetDlgItemText(IMGPRO_BINARY_INVERT_CHK, strValueName);
	strValueNew = AOIDataDefine.GetEnableDisableText(bCheck);
	strValueOld = AOIDataDefine.GetEnableDisableText(!bCheck);
	if ( TRUE == bCheck )
	{	m_BinaryParam.SetBinaryInvert(true);	}
	else
	{	m_BinaryParam.SetBinaryInvert(false);	}
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnShowRawImageChk() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	BOOL bCheck = CWnd::IsDlgButtonChecked(IMGPRO_SHOW_RAW_IMAGE_CHK);
	if ( TRUE == bCheck )
	{	
		AOIDataCollect.SetDrawImageMode(DRAW_IMAGE_NORMAL);	
		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
	}
	else
	{	
		AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
		AOIDataCollect.SetDrawImageMode(DRAW_IAMGE_BY_ALG);	
		PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
		UpdateAlgParamImage(false, true);
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateWndCaptionText(CAOIModel *ModelPtr, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return; }
	if ( NULL == ModelPtr ) { return; }	
	CString str, str2, str3;
	CString ModelName = ModelPtr->GetModelName();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const size_t WndIndex = WndPtr->GetWndIndex();
	if ( NULL != ComponentPtr )
	{
		CString ComName = ComponentPtr->GetComponentFullName();
		str2.Format(_T("C[%s]_Wnd[%s]"), ComName, AOIDataDefine.GetWndIndexText(WndIndex));
	}
	else if ( NULL != BarcodePtr)
	{
		CString BarName = _T("Barcode");
		unsigned int BarcodeIndex = BarcodePtr->GetBarcodeIndex_Project();
		BarName = LoadMultiLanguageString(BarName, BarName);
		str2.Format(_T("%s%d_Wnd[%s]"), BarName, BarcodeIndex+1, AOIDataDefine.GetWndIndexText(WndIndex));
	}
	else
	{	str2.Format(_T("Model[%s]_Wnd[%s]"), ModelName, AOIDataDefine.GetWndIndexText(WndIndex)); }
	
	const unsigned int Index = m_BinaryParam.GetBinaryBelongIndex();
	BIN_PARAM_BELONG_TO BelongToWho=m_BinaryParam.GetBinaryBelongToWho();	
	switch ( BelongToWho )
	{
	case BIN_PARAM_BELONG_TO_ALG_IMAGE:
		str3 = _T("-Alg");
		break;
	case BIN_PARAM_BELONG_TO_MASK_IMAGE:
		str3 = _T("-Mask");
		break;
	case BIN_PARAM_BELONG_TO_PATTERN_IMAGE:
		str3.Format(_T("-Pat [%d]"), Index+1);
		break;
	case BIN_PARAM_BELONG_TO_ROI_IMAGE:
		str3.Format(_T("-Roi [%d]"), Index+1);
		break;
	}
	
	if ( str3.GetLength() == 0 )
	{	str = str2;	}
	else
	{	str = str2 + str3;	}
	CWnd::SetDlgItemText(IMGPRO_ALG_CAPTION_EDIT, str);
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::LockUIWnd(bool bLock)
{
	UINT CtrlID = 0;
	BOOL bEnable = true;	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();	
	if ( true == bLock ) 
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	bEnable = FALSE;	}
	
	CtrlID = IMGPRO_IMAGE_SOURCE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_BINARY_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_FILTER_MODE_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_FILTER_PARAM_COMBO;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_FILTER_MODE_COMBO2;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_FILTER_PARAM_COMBO2;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_BINARY_INVERT_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	CtrlID = IMGPRO_SHOW_RAW_IMAGE_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_GRAY_INVERT_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	CtrlID = IMGPRO_GAIN_PARAM_EDIT;
	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	CtrlID = IMGPRO_GAIN_PARAM_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = IMGPRO_SHOW_ALL_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
}
//-------------------------------------------------------------------------------------//
bool CEditImageProcessPage::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	if ( true == AOIDataCollect.GetIsLockUIWnd() ) { return true; }
	return false;

	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();		
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::UpdateUIToParam_ImageGain()
{
	//bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return; }
	if ( true == m_StopUpdateGainValue )
	{	return; }

	CString str;
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	CWnd::GetDlgItemText(IMGPRO_GAIN_PARAM_EDIT, str);
	double val = ::_ttof(str);
	const bool bGainEnabled=m_BinaryParam.GetGrayGainEnabled();	
	if ( val < 0.001 )
	{	return; }
	strValueNew = str;
	CWnd::GetDlgItemText(IMGPRO_GAIN_PARAM_CHK, strValueName);
	strValueOld.Format(_T("%.1f"), m_BinaryParam.GetGrayGainValue());
	m_BinaryParam.SetGrayGainValue(val);
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);	
	if ( true==bGainEnabled )
	{	UpdateAlgParamImage(true, true); }
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImageProcessPage::ExecSaveLogModelWndOperate(LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	CString sOper = AOIDataDefine.GetSetText();
	CString sAlg = m_WindowText;//WndPtr->GetWndAlgTypeText();	
	LogOperCtrl.SaveLogModelWndOperate(WndPtr, sOper, sAlg, sKey, sOld, sNew);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnKillfocusGainParamEdit() 
{
	// TODO: Add your control notification handler code here	
	UpdateUIToParam_ImageGain();
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnGainParamChk() 
{
	// TODO: Add your control notification handler code here
	//bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return; }
	CString strValueOld;
	CString strValueNew;
	CString strValueName;
	const BOOL bChk = CWnd::IsDlgButtonChecked(IMGPRO_GAIN_PARAM_CHK);
	CWnd::GetDlgItemText(IMGPRO_GAIN_PARAM_CHK, strValueName);
	strValueNew = AOIDataDefine.GetEnableDisableText(bChk);
	strValueOld = AOIDataDefine.GetEnableDisableText(!bChk);
	if ( TRUE == bChk )
	{	m_BinaryParam.SetGrayGainEnabled(true); }
	else
	{	m_BinaryParam.SetGrayGainEnabled(false); }
	ExecSaveLogModelWndOperate(strValueName, strValueOld, strValueNew);
	AOIDataCollect.SetBinaryParamTemp(m_BinaryParam);
	UpdateAlgParamImage(true, true);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnChangeGainParamEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here	
	UpdateUIToParam_ImageGain();
}
//-------------------------------------------------------------------------------------//
void CEditImageProcessPage::OnShowAllBtn() 
{
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }

	std::vector<TUNI_FRAME> ModelUniFrameList;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd   *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return; }	
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return; }

	AOIDataCollect.CopyModelUniFrameList(ModelUniFrameList, false);
	const size_t UniFrameCount = ModelUniFrameList.size();
	if ( 0 == UniFrameCount ) { return; }

	CAlgImageSourceWnd Wnd;
	CAlgBinaryParam &BinaryParam=m_BinaryParam;
	std::vector<unsigned int> FrameUniqueIDList;
	unsigned int FrameIndexOld = BinaryParam.GetBinaryFrameIndex();
	IMAGE_SRC_MODE ImageSrcModeOld = BinaryParam.GetBinaryImageSourceMode();
	ProjectPtr->CloneProjectFrameUniqueIDList(FrameUniqueIDList);

	Wnd.SetAlgBinaryParam(BinaryParam);
	Wnd.SetUniFrameIndex(FrameIndexOld);
	Wnd.SetImageSourceMode(ImageSrcModeOld);
	Wnd.SetUniFrameList(ModelUniFrameList);	
	Wnd.SetFrameUniqueIDList(FrameUniqueIDList);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }	

	CString        str, str2, str3;
	bool           bChangeFrame=false;
	TFrameParam   *FrameParamPtr=NULL; 
	BINARY_MODE    BinaryMode = BinaryParam.GetBinaryMode();	
	const unsigned int FrameIndexNew = Wnd.GetUniFrameIndex();
	IMAGE_SRC_MODE     ImageSrcModeNew = Wnd.GetImageSourceMode();	
	BIN_PARAM_BELONG_TO BinaryBelongToWho = BinaryParam.GetBinaryBelongToWho();
	const int          ColorGroupLinkIndex = BinaryParam.GetBinaryColorGroupLinkIndex();
	unsigned int       FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(FrameIndexNew, true);
	
	BinaryParam.SetEdgeEnhanceMode(Wnd.GetEdgeEnhanceMode());
	BinaryParam.SetEdgeEnhanceFilter1(Wnd.GetEdgeEnhanceFilter1());
	BinaryParam.SetEdgeEnhanceFilter2(Wnd.GetEdgeEnhanceFilter2());

	FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
	if ( FrameIndexNew != FrameIndexOld )
	{		
		bChangeFrame = false;		
		if ( -1!= FrameIndexNew && NULL!=FrameParamPtr )
		{	
			if ( ColorGroupLinkIndex<0 || BINARY_COLOR_FILTER!=BinaryMode) 
			{	bChangeFrame = true; }
			else
			{
				str2 = _T("The frame link to Project Color Group");
				str2 = LoadMultiLanguageString(str2, str2);
				str3.Format(_T("%s [%d]"), str2, ColorGroupLinkIndex+1);
				str2 = _T("Do you want to change the Frame?");
				str2 = LoadMultiLanguageString(str2, str2);
				str.Format(_T("%s\n%s"), str3, str2);
				if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
				{	bChangeFrame = true; }
			}
		}
	}

	if ( true==bChangeFrame && NULL!=FrameParamPtr )
	{		
		unsigned int FrameIndex = FrameIndexNew;
		ProjectPtr->SetProjectMapIndex(FrameIndex);			
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);		
		if ( BIN_PARAM_BELONG_TO_ALG_IMAGE == BinaryBelongToWho )
		{
			WndPtr->SetWndAlgImageFrameIndex(FrameIndex);
			WndPtr->SetWndAlgImageFrameUniqueID(FrameUniqueID);
		}
		if ( BIN_PARAM_BELONG_TO_ROI_IMAGE == BinaryBelongToWho )
		{
			bool bWndRoiBinParam=false;
			CAOIWndRoi *WndRoiPtr = WndPtr->GetWndRoiWndActived();
			if ( NULL == WndRoiPtr )
			{	bWndRoiBinParam = false;	}
			else 
			{	bWndRoiBinParam = WndRoiPtr->GetWndRoiSelfFrameEnabled(); }
			if ( false == bWndRoiBinParam )
			{
				WndPtr->SetWndAlgImageFrameIndex(FrameIndex);
				WndPtr->SetWndAlgImageFrameUniqueID(FrameUniqueID);
			}
			else
			{
				WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameIndex(FrameIndex);
				WndRoiPtr->GetWndRoiBinaryParam().SetBinaryFrameUniqueID(FrameUniqueID);
			}
		}
		WndPtr->SetWndUIUpated_Param(false);
		CAlgParam::AdjustAlgBinaryParam(FrameParamPtr->FrameType, BinaryParam);
		//PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);		
	}
	if ( NULL != FrameParamPtr )
	{
		switch ( FrameParamPtr->FrameType )
		{
		case FRAME_GRAY:
			ImageSrcModeNew = IMAGE_SRC_GRAY;
			break;
		case FRAME_BAYER:			
		case FRAME_COLOR:
			if ( IMAGE_SRC_GRAY == ImageSrcModeNew )
			{	ImageSrcModeNew = IMAGE_SRC_COLOR; }	
			break;
		case FRAME_SPACE:
			ImageSrcModeNew = IMAGE_SRC_GRAY;
			break;
		}		
	}
	JetAPI::SetComboxCurSel(m_ImageSourceCombox, ImageSrcModeNew);	
	OnSelchangeImageSourceCombo();	
	if ( true == bChangeFrame )
	{	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL); }
}
//-------------------------------------------------------------------------------------//