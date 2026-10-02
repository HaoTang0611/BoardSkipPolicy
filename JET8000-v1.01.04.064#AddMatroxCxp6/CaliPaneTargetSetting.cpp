// CaliPaneTargetSetting.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CaliPaneTargetSetting.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneTargetSetting dialog
//-------------------------------------------------------------------------------------//
CCaliPaneTargetSetting::CCaliPaneTargetSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliPaneTargetSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaliPaneTargetSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaliPaneTargetSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaliPaneTargetSetting, CDialog)
	//{{AFX_MSG_MAP(CCaliPaneTargetSetting)
	ON_BN_CLICKED(CALITARGET_GRID_POS_GO_BTN, OnGridPosGoBtn)
	ON_BN_CLICKED(CALITARGET_RECT_POS_GO_BTN, OnRectPosGoBtn)
	ON_BN_CLICKED(CALITARGET_WHITE_POS_GO_BTN, OnWhitePosGoBtn)
	ON_BN_CLICKED(CALITARGET_HEIGHT_POS_GO_BTN, OnHeightPosGoBtn)
	ON_BN_CLICKED(CALITARGET_GRID_POS_SET_BTN, OnGridPosSetBtn)
	ON_BN_CLICKED(CALITARGET_RECT_POS_SET_BTN, OnRectPosSetBtn)
	ON_BN_CLICKED(CALITARGET_WHITE_POS_SET_BTN, OnWhitePosSetBtn)
	ON_BN_CLICKED(CALITARGET_HEIGHT_POS_SET_BTN, OnHeightPosSetBtn)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneTargetSetting message handlers
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneTargetSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->UpdateTargetPosToUI();	      
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::SwitchMultiLanguage()
{		
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_PANE_TARGET_SETTING");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_PANE_TARGET_SETTING;
	WndKey = _T("IDD_CALIBRATION_PANE_TARGET_SETTING");
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
	WndID = CALITARGET_GRID_POS_GO_BTN;
	WndKey = _T("CALITARGET_GRID_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_GRID_POS_SET_BTN;
	WndKey = _T("CALITARGET_GRID_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_RECT_POS_GO_BTN;
	WndKey = _T("CALITARGET_RECT_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_RECT_POS_SET_BTN;
	WndKey = _T("CALITARGET_RECT_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_WHITE_POS_GO_BTN;
	WndKey = _T("CALITARGET_WHITE_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_WHITE_POS_SET_BTN;
	WndKey = _T("CALITARGET_WHITE_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_HEIGHT_POS_GO_BTN;
	WndKey = _T("CALITARGET_HEIGHT_POS_GO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALITARGET_HEIGHT_POS_SET_BTN;
	WndKey = _T("CALITARGET_HEIGHT_POS_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::UpdateTargetPosToUI()
{
	CString str;
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosX);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosY);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosZ);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_Z, str);

	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosX);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosY);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosZ);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_Z, str);

	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosX);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosY);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosZ);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_Z, str);

	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosX);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosY);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosZ);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_Z, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnGridPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetGridPosX, CaliParam.m_TargetGridPosY, CaliParam.m_TargetGridPosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnRectPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetRectPosX, CaliParam.m_TargetRectPosY, CaliParam.m_TargetRectPosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnWhitePosGoBtn() 
{
	// TODO: Add your control notification handler code here
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetWhitePosX, CaliParam.m_TargetWhitePosY, CaliParam.m_TargetWhitePosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnHeightPosGoBtn() 
{
	// TODO: Add your control notification handler code here
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	if ( MotionCtrlPtr->XYZMoveTo(CaliParam.m_TargetHeightPosX, CaliParam.m_TargetHeightPosY, CaliParam.m_TargetHeightPosZ) == false )
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnGridPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetGridPosX = PosX;
	CaliParam.m_TargetGridPosY = PosY;
	CaliParam.m_TargetGridPosZ = PosZ;
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosX);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosY);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetGridPosZ);
	this->SetDlgItemText(CALITARGET_GRID_POS_EDIT_Z, str);	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnRectPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetRectPosX = PosX;
	CaliParam.m_TargetRectPosY = PosY;
	CaliParam.m_TargetRectPosZ = PosZ;
	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosX);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosY);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetRectPosZ);
	this->SetDlgItemText(CALITARGET_RECT_POS_EDIT_Z, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnWhitePosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetWhitePosX = PosX;
	CaliParam.m_TargetWhitePosY = PosY;
	CaliParam.m_TargetWhitePosZ = PosZ;		
	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosX);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosY);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetWhitePosZ);
	this->SetDlgItemText(CALITARGET_WHITE_POS_EDIT_Z, str);	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnHeightPosSetBtn() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double PosX=0, PosY=0, PosZ=0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	CaliParam.m_TargetHeightPosX = PosX;
	CaliParam.m_TargetHeightPosY = PosY;
	CaliParam.m_TargetHeightPosZ = PosZ;	
	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosX);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_X, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosY);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_Y, str);
	str.Format(_T("%.0f"), CaliParam.m_TargetHeightPosZ);
	this->SetDlgItemText(CALITARGET_HEIGHT_POS_EDIT_Z, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	this->UpdateTargetPosToUI(); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnOK() 
{
	// TODO: Add extra validation here
	this->ShowWindow(SW_HIDE);
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneTargetSetting::OnCancel() 
{
	// TODO: Add extra cleanup here
	this->ShowWindow(SW_HIDE);
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//