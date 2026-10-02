// ProjectMarkWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "ProjectMarkWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMarkWnd dialog
//-------------------------------------------------------------------------------------//
CProjectMarkWnd::CProjectMarkWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectMarkWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectMarkWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ProjectPtr = NULL;
	
	m_Modified = false;
	m_MarkFrameIndex = 0;
	m_MarkW = 0;
	m_MarkH = 0;
	m_MarkStep = 0;
	m_BitCount = 0;	
	m_MarkPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectMarkWnd)
	DDX_Control(pDX, PRGMARK_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectMarkWnd, CDialog)
	//{{AFX_MSG_MAP(CProjectMarkWnd)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(PRGMARK_GO_TO_BTN, OnGoToBtn)
	ON_BN_CLICKED(PRGMARK_SET_POS_BTN, OnSetPosBtn)
	ON_BN_CLICKED(PRGMARK_CLEAR_BTN, OnClearBtn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(PRGMARK_CENTER_LINE_CHK, OnCenterLineChk)	
	ON_BN_CLICKED(PRGMARK_APPLY_BTN, OnApplyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMarkWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CProjectMarkWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetShowCameraRgn(false);
	m_ImageWnd.SetShowComponentName(false);	
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_NULL);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);
	m_ImageWnd.SetShowFieldRgn(false);

	SwitchMultiLanguage();	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
	}
	else
	{	
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
BOOL CProjectMarkWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CProjectMarkWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
CAOIProject* CProjectMarkWnd::GetProjectPtr()//取得專案指標	
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::SetProjectPtr(CAOIProject *ProjectPtr)//設定專案指標	
{
	m_Modified = false;	
	ClearMarkBuffer();
	m_ProjectPtr = ProjectPtr;
	HWND hWnd = m_ImageWnd.GetSafeHwnd();
	if ( NULL == hWnd ) { return; }	
	m_ImageWnd.ReleaseImageBuffer();
	m_ImageWnd.RedrawWnd(TRUE);
	if ( NULL == ProjectPtr ) 
	{	return;	}
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;
	if ( ProjectPtr->CloneProjectMarkImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return;	}
	
	unsigned int MarkFrameIndex=0;
	double PosX=0, PosY=0, PosZ=0;

	m_LaneID = ProjectPtr->GetProjectActLaneID();
	ProjectPtr->GetProjectMarkStage(PosX, PosY, PosZ);
	MarkFrameIndex = ProjectPtr->GetProjectMarkFrameIndex();	
	SetMarkStagePos(PosX, PosY, PosZ);
	SetMarkFrameIndex(MarkFrameIndex);
	if ( SetMarkImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	
		JetMemory.free_func(ImagePtr);	
		return;
	}
	m_Modified = false;	
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::SetMarkFrameIndex(unsigned int index)//設定標記引數
{
	m_Modified = true;
	m_MarkFrameIndex = index;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::SetMarkStagePos(double PosX, double PosY, double PosZ)//設定標記機台座標
{	
	m_Modified = true;
	m_StagePos.x = PosX;
	m_StagePos.y = PosY;
	m_StagePos.z = PosZ;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::SetMarkImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr)//設定標記影像
{
	const char fnName[] = "CProjectMarkWnd::SetMarkImage";
	ClearMarkBuffer();	
	if ( NULL == ImagePtr )
	{	return true; }

	IMAGE_PTR    ShowBuffer = NULL;
	const size_t szShowBuffer = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(szShowBuffer, ShowBuffer, fnName, "ShowBuffer") == false )
	{	return false; }
	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ShowBuffer);
	
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ShowBuffer, true, true, false);	
	m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(FALSE);
	JetMemory.free_func(ShowBuffer);

	m_MarkW = ImageW;
	m_MarkH = ImageH;
	m_MarkStep = ImageStep;
	m_BitCount = BitCount;
	m_MarkPtr = ImagePtr;
	m_Modified = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::ClearMarkBuffer()
{
	if ( NULL != m_MarkPtr ) 
	{	JetMemory.free_func(m_MarkPtr); }

	m_MarkW = 0;
	m_MarkH = 0;
	m_MarkStep = 0;
	m_BitCount = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_PROJECT_MARK_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_MARK_WND;
	WndKey = _T("IDD_PROJECT_MARK_WND");
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
	WndID = PRGMARK_APPLY_BTN;
	WndKey = _T("PRGMARK_APPLY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PRGMARK_GO_TO_BTN;
	WndKey = _T("PRGMARK_GO_TO_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PRGMARK_CLEAR_BTN;
	WndKey = _T("PRGMARK_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMARK_CENTER_LINE_CHK;
	WndKey = _T("PRGMARK_CENTER_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PRGMARK_SET_POS_BTN;
	WndKey = _T("PRGMARK_SET_POS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CProjectMarkWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_PROJECT_MARK_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnGoToBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }	

	double PosX=0, PosY=0, PosZ=0;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	ProjectPtr->GetProjectMarkStage(PosX, PosY, PosZ);
	AOIDataCollect.MapStagePosLaneByLaneID(PosX, PosY, PosZ, m_LaneID);
	MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	//AOIDataCollect.PostMainFrameWndMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnSetPosBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	
	CString str;
	str = _T("Do you want to set project mark position?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	double PosX=0, PosY=0, PosZ=0;	
	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	AOIDataCollect.MapStagePosToLaneA(PosX, PosY, m_LaneID);
	ProjectPtr->SetProjectMarkStage(PosX, PosY, PosZ);
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CString str;
	str = _T("Do you want to clear project mark image?");
	str = this->LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	m_Modified=true;
	ClearMarkBuffer();	
	m_ImageWnd.ReleaseImageBuffer();
	m_ImageWnd.RedrawWnd(TRUE);
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::ExecApply()
{
	CAOIProject *ProjectPtr = GetProjectPtr();
	if ( NULL == ProjectPtr ) { return true; }
	if ( false == m_Modified ) { return true; }

	if ( NULL == m_MarkPtr )
	{	ProjectPtr->ClearProjectMarkBuffer(); }
	else
	{
		TPOINT3D StagePos_LA = m_StagePos;
		AOIDataCollect.MapStagePosToLaneA(StagePos_LA.x, StagePos_LA.y, m_LaneID);
		ProjectPtr->SetProjectMarkFrameIndex(m_MarkFrameIndex);
		ProjectPtr->SetProjectMarkStage(StagePos_LA.x, StagePos_LA.y, StagePos_LA.z);
		ProjectPtr->SetProjectMarkImage(m_MarkW, m_MarkH, m_MarkStep, m_BitCount, m_MarkPtr);
		m_MarkPtr = NULL;
	}	
	m_Modified = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMarkWnd::ExecClose()
{
	MANIPULATE_MAIN_MODE MainMode = AOIDataCollect.GetManipulateMainMode();	
	if ( MANIPULATE_MAIN_MARK == MainMode )
	{	AOIDataCollect.SetManipulateMainMode(MANIPULATE_MAIN_SELECT); }

	ClearMarkBuffer();
	m_Modified = false;
	m_StagePos = TPOINT3D();
	m_ProjectPtr = NULL;	
	m_ImageWnd.ReleaseImageBuffer();
	m_ImageWnd.RedrawWnd(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//ExecClose();//在OnClose後會再呼叫OnCancel所以這裡不用執行
	CDialog::OnClose();
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnCenterLineChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PRGMARK_CENTER_LINE_CHK);	
	m_ImageWnd.SetShowWndCenterLine((bool)(bChk));
	m_ImageWnd.Invalidate();
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnOK() 
{
	// TODO: Add extra validation here
	ExecApply();
	ExecClose();
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	ExecClose();
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CProjectMarkWnd::OnApplyBtn() 
{
	// TODO: Add your control notification handler code here
	ExecApply();
}
//-------------------------------------------------------------------------------------//
