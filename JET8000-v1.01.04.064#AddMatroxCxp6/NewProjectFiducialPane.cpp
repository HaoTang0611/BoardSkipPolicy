// NewProjectFiducialPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "NewProjectFiducialPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneFiducial dialog
//-------------------------------------------------------------------------------------//
CNewProjectPaneFiducial::CNewProjectPaneFiducial(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectPaneFiducial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectPaneFiducial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_FdIndex = 0;
	m_ProjectPtr = NULL;
	m_CameraID = PRIMARY_CAMERA_ID;
	m_LightMode = LIGHT_MODE_A;
	m_FrameType = FRAME_GRAY;
	m_FrameIndex = 0;
	m_FrameUniqueID = FRAME_UNIQUE_ID_FD;
	m_NewProjectMode = NEW_PROJECT_ONLINE;

	m_ResetView = true;
	m_FovStageX = 0.0;
	m_FovStageY = 0.0;

	m_ReCalcStagePos = false;
	m_GetComponentPos = false;
	m_CADFileContentMode=CAD_FILE_CONTENT_NORMAL;
	m_EnableMultiDistrictMode = false;

	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_BitCount = 0;
	this->m_ImageStep = 0;
	this->m_ImageBuffer = NULL;
	this->m_ImageBufferSize = 0;	

	this->m_ShowImageW = 0;
	this->m_ShowImageH = 0;
	this->m_ShowImageStep = 0;
	this->m_ShowBitCount = 0;
	this->m_ShowBuffer = NULL;
	this->m_ShowBufferSize = 0;			
	m_DistrictID=DISTRICT_ID_A;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectPaneFiducial)
	DDX_Control(pDX, PANEFD_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CNewProjectPaneFiducial, CDialog)
	//{{AFX_MSG_MAP(CNewProjectPaneFiducial)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(PAENFD_FD_ADD_BTN, OnFdAddBtn)
	ON_BN_CLICKED(PAENFD_FD_DEL_BTN, OnFdDelBtn)
	ON_BN_CLICKED(PAENFD_FD_RADIO_1, OnFdRadio1)
	ON_BN_CLICKED(PAENFD_FD_RADIO_2, OnFdRadio2)
	ON_BN_CLICKED(PAENFD_FD_RADIO_3, OnFdRadio3)
	ON_BN_CLICKED(PAENFD_FD_RADIO_4, OnFdRadio4)
	ON_EN_UPDATE(PAENFD_EXTEND_PATTERN_W_EDIT, OnUpdateExtendPatternWEdit)
	ON_EN_UPDATE(PAENFD_EXTEND_PATTERN_H_EDIT, OnUpdateExtendPatternHEdit)	
	ON_BN_CLICKED(PAENFD_SELECT_COMPONENT_BTN, OnSelectComponentBtn)
	ON_BN_CLICKED(PAENFD_DISTRICT_A_BTN, OnDistrictABtn)
	ON_BN_CLICKED(PAENFD_DISTRICT_B_BTN, OnDistrictBBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneFiducial message handlers
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageWnd.SetProjectPtr(this->m_ProjectPtr);
	m_ImageWnd.SetShowLBtnPos(true);
	m_ImageWnd.SetShowEditCenterLine(true);
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);;
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);	

	SwitchMultiLanguage();	
	CAMERA_ID CameraID = this->m_CameraID;
	m_BitCount = 8;
	m_ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	m_ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);
	m_ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, m_BitCount, 4);
	m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, false, true);	
	

	CString str;
	DISTRICT_ID DistrictID = GetDistrictID();
	CheckDlgButton(PAENFD_FD_RADIO_1, TRUE);

	SetDlgItemInt(PAENFD_EXTEND_PATTERN_W_EDIT, 0);
	SetDlgItemInt(PAENFD_EXTEND_PATTERN_H_EDIT, 0);

	SetDlgItemInt(PAENFD_EXTEND_ROI_W_EDIT, 4000);
	SetDlgItemInt(PAENFD_EXTEND_ROI_H_EDIT, 4000);
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(PAENFD_DISTRICT_EDIT, str);
	UpdateFdCount();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->m_ShowBuffer = NULL;
	this->m_ShowBufferSize = 0;
	this->m_ImageBuffer = NULL;
	this->m_ImageBufferSize = 0;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( this->m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		this->m_ImageWnd.MoveWindow(&WndRect);			
	}
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_NEW_PROJECT_PANE_FIDUCIAL");
	//---------------------------------------------------------------------------------//
	WndID = IDD_NEW_PROJECT_PANE_FIDUCIAL;
	WndKey = _T("IDD_NEW_PROJECT_PANE_FIDUCIAL");
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
	WndID = PAENFD_FD_RADIO_1;
	WndKey = _T("PAENFD_FD_RADIO_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_FD_RADIO_2;
	WndKey = _T("PAENFD_FD_RADIO_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_FD_RADIO_3;
	WndKey = _T("PAENFD_FD_RADIO_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_FD_RADIO_4;
	WndKey = _T("PAENFD_FD_RADIO_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PAENFD_FD_ADD_BTN;
	WndKey = _T("PAENFD_FD_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_FD_DEL_BTN;
	WndKey = _T("PAENFD_FD_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_EXTEND_PATTERN_W_LABEL;
	WndKey = _T("PAENFD_EXTEND_PATTERN_W_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_EXTEND_PATTERN_H_LABEL;
	WndKey = _T("PAENFD_EXTEND_PATTERN_H_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_EXTEND_ROI_W_LABEL;
	WndKey = _T("PAENFD_EXTEND_ROI_W_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_EXTEND_ROI_H_LABEL;
	WndKey = _T("PAENFD_EXTEND_ROI_H_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = PAENFD_USE_COMPONENT_POS_CHK;
	WndKey = _T("PAENFD_USE_COMPONENT_POS_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PAENFD_SELECT_COMPONENT_BTN;
	WndKey = _T("PAENFD_SELECT_COMPONENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
	WndID = PAENFD_DISTRICT_A_BTN;
	WndKey = _T("PAENFD_DISTRICT_A_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PAENFD_DISTRICT_B_BTN;
	WndKey = _T("PAENFD_DISTRICT_B_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CNewProjectPaneFiducial::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_NEW_PROJECT_PANE_FIDUCIAL");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LRESULT CNewProjectPaneFiducial::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class		
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:
		m_ResetView = true;
		if ( this->UpdateFovImage(wParam, lParam, true) == false )
		{	this->LockUIWnd(false); }
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		this->ExecGrabImage();
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		this->LockUIWnd(false);
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:		
		DrawCtrlWnd(wParam, lParam);
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:	
			m_ResetView = false;
			if ( this->UpdateFovImage(PRIMARY_CAMERA_ID, lParam, false) == false )
			{	this->LockUIWnd(false); }
			break;
		}
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		OnImageWndNotify(wParam, lParam);		
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetProjectPtr(CAOIProject *ProjectPtr)
{
	this->m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CNewProjectPaneFiducial::GetDistrictID() const
{
	return m_DistrictID;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetDistrictID(DISTRICT_ID Mode)
{
	m_DistrictID = Mode;
}
//-------------------------------------------------------------------------------------//
NEW_PROJECT_MODE CNewProjectPaneFiducial::GetNewProjectMode() const
{
	return m_NewProjectMode;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetNewProjectMode(NEW_PROJECT_MODE Mode)
{
	m_NewProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::GetEnableMultiDistrictMode() const
{ 
	return m_EnableMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetEnableMultiDistrictMode(bool Mode)
{ 
	m_EnableMultiDistrictMode = Mode; 
}	
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetShowBuffer(size_t BufferSize, IMAGE_PTR Ptr)
{
	this->m_ShowBuffer = Ptr;
	this->m_ShowBufferSize = BufferSize;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::SetImageBuffer(size_t BufferSize, IMAGE_PTR ImagePtr)
{
	this->m_ImageBuffer = ImagePtr;
	this->m_ImageBufferSize = BufferSize;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CNewProjectPaneFiducial::GetActiveProjectPtr()//取得目前專案指標
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CNewProjectPaneFiducial::GetActivePanelPtr()//取得目前取用的整板指標
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) 
	{ 
		this->m_ErrorString.Format(_T("Error, No Active Project"));
		return NULL; 
	}
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) 
	{
		this->m_ErrorString.Format(_T("Error, No Active Panel"));
		return NULL; 
	}
	return PanelPtr;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CNewProjectPaneFiducial::GetPanelFdPtr(size_t idx)
{
	CAOIFd    *FdPtr=NULL;
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return NULL; }
	if ( false == m_EnableMultiDistrictMode )
	{	FdPtr = PanelPtr->GetPanelFdPtr(idx, true);	}
	else
	{
		DISTRICT_ID DistrictID = GetDistrictID();
		FdPtr = PanelPtr->GetPanelFdPtr(idx, DistrictID);		
	}
	return FdPtr;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::CreateBKDC(bool ResetView)//建立背景DC	
{
	if ( NULL==m_ShowBuffer ) { return FALSE; }	
	TPOINT2D   ImageRes;	
	TREGION4D  StageRgn;
	IMAGE_SIZE ImageW=0, ImageH=0;
	CAMERA_ID  CameraID = m_CameraID;			

	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);	
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer, false, ResetView);	
	RedrawProjectImageWnd();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::ExecGrabImage()
{
	if ( NULL == this->m_ProjectPtr ) { return FALSE; }	
	const bool MultiFdLight = AOIDataCollect.GetSystemMultiFdLight();
#ifndef OFFLINE_VERSION
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( true == OfflineMode )
	{
		PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
		return TRUE;
	}
	if ( AOIDataCollect.CheckCanGrabNextUniFrameImage() == false )
	{	return TRUE;	}
	this->LockUIWnd(true);
	if ( true == MultiFdLight )
	{
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
#else
	if ( AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		this->LockUIWnd(false);
		return FALSE;
	}	
#endif//LIGHT_CTRL_DISABLE
	}
	else
	{
		m_FrameIndex = 0;
		m_FrameUniqueID = m_ProjectPtr->GetProjectFrameUniqueID_Fd();	
		if ( AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			this->LockUIWnd(false);
			return FALSE;
		}
	}
#else
	PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
#endif//OFFLINE_VERSION	
	return TRUE;	
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::LockUIWnd(bool bLock)
{
	UINT  CtrlID = 0;
	BOOL  bEnable = TRUE;	

	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }		
	
//	CtrlID = ALIGNPANEL_ORIENTATION_ROTATE_090_BTN;
//	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	//Edit Control
//	CtrlID = REGION_CORNER_WIDTH_EDIT;
//	JetAPI::EnableEditWnd(this, CtrlID, bEnable);
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::DrawCtrlWnd(WPARAM wParam, LPARAM lParam)//在控制像繪圖後重新繪圖
{
	HDC  hDC = (HDC)(lParam);
	UINT CtrlID = (UINT)(wParam);	
	switch ( CtrlID )
	{
	case ALIGNPANEL_IMAGE_WND:
		//::MoveToEx(hDC, 0, 300, NULL);
		//::LineTo(hDC, 600, 300);
		break;
	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
{	
	bool bGetImage = false;
	if ( AOIDataCollect.GetOfflineMode() == false )
	{
	#ifndef LIGHT_CTRL_DISABLE
		if ( RetrieveCameraUniFrame(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#else
		if ( RetrieveCameraImage(wParam, lParam, bCameraCallBack, bGetImage) == false )
		{	return false; }
	#endif//LIGHT_CTRL_DISABLE		
		if ( false == bGetImage ) { return true; }
	}
	else
	{
		if ( LoadProgramOfflineImage(wParam, lParam, bGetImage) == false )
		{	return false; }		
	}

	//無適合的資料
	if ( false == bGetImage )
	{	::memset(m_ShowBuffer, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	else
	{
		if ( AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ShowBuffer) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}		
	}
	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;
	this->CreateBKDC(m_ResetView); 
	this->LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return FALSE; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( this->m_CameraID != CameraID ) { return false; }		
	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return true;	
	}
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	m_FrameIndex = 0;
	m_FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID_Fd();	
	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);
	if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	bGetImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage)//取得相機影像	
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();	
	//const unsigned int MapIndex = 2;	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		//str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));		
		return true;	
	}	

	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}		

	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return true; }

	TUNI_FRAME UniFrame = UniFrameList[0];
	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);	
	
	const size_t MaxFrames = UniFrameList.size();
	if ( MapIndex>=0 && MapIndex<MaxFrames )
	{	UniFrame = UniFrameList[MapIndex]; }
	else
	{
		MapIndex = 0;
		UniFrame = UniFrameList[0]; 
	}
	m_FrameIndex = MapIndex;
	m_FrameUniqueID = UniFrame.FrameUniqueID;	
	BuffserSize = ImageAPI.CalcBufferSize(UniFrame.ImageStep, UniFrame.ImageH);
	if ( NULL!=UniFrame.ImagePtr && BuffserSize <= m_ImageBufferSize )
	{	
		m_ImageW = UniFrame.ImageW;
		m_ImageH = UniFrame.ImageH;
		m_ImageStep = UniFrame.ImageStep;
		m_BitCount = UniFrame.BitCount;		
		::memcpy(m_ImageBuffer, UniFrame.ImagePtr, sizeof(unsigned char)*BuffserSize);
	}	
	else
	{	::memset(m_ImageBuffer, 0x00, sizeof(unsigned char)*m_ImageBufferSize);	}
	JetAPI::ClearUniFrameList(UniFrameList);
	bGetImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ImageBuffer ) { return false; }
	if ( NULL == this->m_ShowBuffer ) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( this->m_CameraID != CameraID ) { return false; }
	
	double PosX=0, PosY=0, PosZ=0;
	const int  MaxFrames = FRAME_MAX_COUNT;
	TUNI_FRAME UniFrameList[FRAME_MAX_COUNT];
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	//IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);

	if ( MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	Pos.x = PosX;
	Pos.y = PosY;
	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);

	m_ResetView = JetAPI::CheckMoved(m_FovStageX, m_FovStageY, PosX, PosY);	
	m_FovStageX = PosX;
	m_FovStageY = PosY;	
	
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_PROGRAM;
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}
	else
	{
		if ( MapIndex>=0 && MapIndex<MaxFrames )
		{
			m_FrameIndex = MapIndex;
			m_FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);;

			BitCount = UniFrameList[MapIndex].BitCount;
			ImageStep = UniFrameList[MapIndex].ImageStep;	
			BuffserSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( NULL!=UniFrameList[MapIndex].ImagePtr && BuffserSize <= m_ShowBufferSize )
			{
				bGetImage = true;
				m_ImageW = ImageW;
				m_ImageH = ImageH;
				m_ImageStep = ImageStep;
				m_BitCount = BitCount;
				::memcpy(m_ImageBuffer, UniFrameList[MapIndex].ImagePtr, sizeof(unsigned char)*BuffserSize);
			}
		}
	}	
	JetAPI::ClearUniFrameList(UniFrameList, FRAME_MAX_COUNT);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		DISTRICT_ID  DistrictID = GetDistrictID();
		ChangeDistrictID(DistrictID);

		CAOIProject *ProjectPtr = GetActiveProjectPtr();		
		if ( NULL != ProjectPtr )
		{	AOIDataCollect.SetProjectLightSetting(ProjectPtr);	}
		//v1.01.01.057
		m_GetComponentPos = false;
		m_CADFileContentMode=AOIDataCollect.GetLoadCADFileContentMode();
		if ( CAD_FILE_CONTENT_FIDUCIAL == m_CADFileContentMode )
		{				
			m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_SELECT_COMPONENT);	 
			CWnd::CheckDlgButton(PAENFD_USE_COMPONENT_POS_CHK, TRUE);
			JetAPI::EnableCtrlWnd(this, PAENFD_USE_COMPONENT_POS_CHK, FALSE);
		}
		else
		{	
			m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);	 
			CWnd::CheckDlgButton(PAENFD_USE_COMPONENT_POS_CHK, FALSE);//v1.01.01.057
			JetAPI::EnableCtrlWnd(this, PAENFD_USE_COMPONENT_POS_CHK, TRUE);//v1.01.01.057
		}
		
		//this->m_ImageWnd.ShowFittedZoom();		
		AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());
		ExecGrabImage();
		PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SHOW_PROJECT_MAP_WND, TRUE);
	}
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::UpdateFdParamToUI(CAOIFd *FdPtr)//更新參數至定位點介面
{	
	if ( NULL == FdPtr ) { return FALSE; }
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::UpdateFdUIRadio()//更新定位點Radio介面 
{
	switch ( m_FdIndex)
	{
	case 0:
		this->CheckDlgButton(PAENFD_FD_RADIO_1, TRUE);
		this->CheckDlgButton(PAENFD_FD_RADIO_2, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_3, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_4, FALSE);
		break;
	case 1:
		this->CheckDlgButton(PAENFD_FD_RADIO_1, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_2, TRUE);
		this->CheckDlgButton(PAENFD_FD_RADIO_3, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_4, FALSE);
		break;
	case 2:
		this->CheckDlgButton(PAENFD_FD_RADIO_1, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_2, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_3, TRUE);
		this->CheckDlgButton(PAENFD_FD_RADIO_4, FALSE);
		break;
	case 3:
		this->CheckDlgButton(PAENFD_FD_RADIO_1, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_2, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_3, FALSE);
		this->CheckDlgButton(PAENFD_FD_RADIO_4, TRUE);
		break;
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::UpdateFdParamToUI(unsigned int FdIdx)//更新參數至定位點介面
{
	CAOIPanel *PanelPtr = CNewProjectPaneFiducial::GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return FALSE; }
	CAOIFd *FdPtr = PanelPtr->GetPanelFdPtr(FdIdx, true);
	if ( NULL == FdPtr ) { return FALSE; }
	m_FdIndex = FdIdx;
	return UpdateFdParamToUI(FdPtr);	
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::UpdateFdParamToObj(CAOIFd *FdPtr)//更新參數至定位點物件
{
	if ( NULL == FdPtr ) { return FALSE; }
	if ( NULL == m_ProjectPtr ) { return FALSE; }
	CAOIProject *ProjectPtr = m_ProjectPtr;
	CAOIPanel   *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return FALSE; }

	CString str;	
	RECT    FdImageRect={0};
	TRECT4D FdImageRect4d;	
	CAMERA_ID CameraID = this->m_CameraID;			
	const IMAGE_SIZE ImageW = this->m_ImageW;
	const IMAGE_SIZE ImageH = this->m_ImageH;
	const IMAGE_SIZE ImageW2 = ImageW/2;
	const IMAGE_SIZE ImageH2 = ImageH/2;
	const IMAGE_SIZE ImageStep = this->m_ImageStep;
	const IMAGE_SIZE BitCount = this->m_ShowBitCount;
	const LANE_ID LaneID = m_ProjectPtr->GetProjectActLaneID();
	const double ExtendPatWum = this->GetDlgItemInt(PAENFD_EXTEND_PATTERN_W_EDIT);//um
	const double ExtendPatHum = this->GetDlgItemInt(PAENFD_EXTEND_PATTERN_H_EDIT);//um
	const double ExtendRoiWum = this->GetDlgItemInt(PAENFD_EXTEND_ROI_W_EDIT);//um
	const double ExtendRoiHum = this->GetDlgItemInt(PAENFD_EXTEND_ROI_H_EDIT);//um
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const DISTRICT_ID  DistrictID = GetDistrictID();
	const unsigned int FrameIndex = m_FrameIndex;
	const unsigned int FrameUniqueID = m_FrameUniqueID;	
	const int    FdGroupID = ProjectPtr->GetProjectFdFreeGroupID();
	TFrameParam *FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);

	//if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID || NULL==FrameParamPtr )
	//{
	//	m_ErrorString = _T("Error, Can not set DLP Image be Fiducial Image");		
	//	return FALSE;
	//}

	TRECT4D  FdStageRect4d;	
	TPOINT2D StageCp;
	TPOINT2D FdCadPos;
	TSIZE2D  FdRoiExtSize;
	TSIZE2D  FdPatExtSize;
	TSIZE2D  FdStageSize;
	TPOINT2D FdStagePos2D; 
	TPOINT3D FdStagePos3D; 
	TPOINT3D FdStagePos3D_LA; 

	FdPatExtSize.cx = (ExtendPatWum)/ResX;
	FdPatExtSize.cy = (ExtendPatHum)/ResY;

	FdRoiExtSize.cx = ExtendRoiWum;
	FdRoiExtSize.cy = ExtendRoiHum;
	m_ImageWnd.GetImageEditRect(FdImageRect4d);	

	FdImageRect.left   = JetAPI::Floor(FdImageRect4d.left-FdPatExtSize.cx);
	FdImageRect.right  = JetAPI::Floor(FdImageRect4d.right+FdPatExtSize.cx);
	FdImageRect.top    = JetAPI::Floor(FdImageRect4d.top-FdPatExtSize.cy);
	FdImageRect.bottom = JetAPI::Floor(FdImageRect4d.bottom+FdPatExtSize.cy);
	const size_t ImageRectW = FdImageRect.right-FdImageRect.left;
	const size_t ImageRectH = FdImageRect.bottom-FdImageRect.top;	
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	if ( FdImageRect.left<0 || FdImageRect.right<0 || FdImageRect.top<0 || FdImageRect.bottom<0 ) 
	{
		this->m_ErrorString.Format(_T("Error, Fd Image Rect Exception [%d, %d, %d, %d]"), FdImageRect.left, FdImageRect.top, FdImageRect.right, FdImageRect.bottom);
		return FALSE; 
	}
	if ( FdImageRect.left>=nImageW || FdImageRect.right>=nImageW || FdImageRect.top>=nImageH || FdImageRect.bottom>=nImageH ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Fd Image Rect Exception [%d, %d, %d, %d]"), FdImageRect.left, FdImageRect.top, FdImageRect.right, FdImageRect.bottom);
		return FALSE; 
	}

	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(StageCp.x, StageCp.y, FdStagePos3D.z, OfflineMode);	
	AOIDataCollect.MapCameraRectToStage(CameraID, FdImageRect4d, StageCp, FdStageRect4d);

	FdStagePos2D.x = FdStagePos3D.x = (FdStageRect4d.left+FdStageRect4d.right)*0.5;
	FdStagePos2D.y = FdStagePos3D.y = (FdStageRect4d.top+FdStageRect4d.bottom)*0.5;
	FdStageSize.cx = ::fabs(FdStageRect4d.right-FdStageRect4d.left);
	FdStageSize.cy = ::fabs(FdStageRect4d.bottom-FdStageRect4d.top);
	PanelPtr->GetPanelMapSTCPtr(DistrictID)->Map2D(FdStagePos3D.x, FdStagePos3D.y, FdCadPos.x, FdCadPos.y);
	if ( true == m_GetComponentPos ) 
	{
		FdCadPos = m_FdCadPos;		
		FdStagePos3D = m_FdStagePos;		
		m_GetComponentPos = false;
		m_ReCalcStagePos = true;
		m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_SELECT_COMPONENT);
	}
	FdStagePos3D_LA = FdStagePos3D;
	AOIDataCollect.MapStagePosToLaneA(FdStagePos3D_LA.x, FdStagePos3D_LA.y, LaneID);

	FdRoiExtSize.cx = FdStageSize.cx+ExtendRoiWum+ExtendRoiWum;
	FdRoiExtSize.cy = FdStageSize.cy+ExtendRoiHum+ExtendRoiHum;

	FdPtr->SetFdLaneID(LaneID);
	FdPtr->SetFdGroupID(FdGroupID);
	FdPtr->SetFdFrameIndex(FrameIndex);
	FdPtr->SetFdFrameUniqueID(FrameUniqueID);
	FdPtr->SetFdPatExtendSize(FdPatExtSize);
	FdPtr->SetFdRoiSize(FdRoiExtSize);
	FdPtr->SetFdBodySizeW(FdStageSize.cx);
	FdPtr->SetFdBodySizeH(FdStageSize.cy);
	FdPtr->SetFdStagePos(FdStagePos3D);			
	FdPtr->SetFdCadPosX(FdCadPos.x);
	FdPtr->SetFdCadPosY(FdCadPos.y);	
	FdPtr->SetFdTeachStagePos(FdStagePos3D_LA);
	FdPtr->CalcFdCadCornerPos();
	FdPtr->LayoutFdStageCornerPos();	
	FdPtr->SetFdFovCadPosX(FdCadPos.x);
	FdPtr->SetFdFovCadPosY(FdCadPos.y);
	FdPtr->SetFdFovStagePosX(FdStagePos3D.x);
	FdPtr->SetFdFovStagePosY(FdStagePos3D.y);
	FdPtr->SetFdFieldCadPosX(FdCadPos.x);
	FdPtr->SetFdFieldCadPosY(FdCadPos.y);
	FdPtr->SetFdFieldStagePosX(FdStagePos3D.x);
	FdPtr->SetFdFieldStagePosY(FdStagePos3D.y);	

	//圖像
	IMAGE_PTR    PatPtr = NULL;	
	IMAGE_SIZE PatH = FdImageRect.bottom-FdImageRect.top;
	IMAGE_SIZE PatW = FdImageRect.right-FdImageRect.left;
	IMAGE_SIZE PatBitCount = BitCount;
	IMAGE_SIZE PatStep = JetAPI::GetBMPImagePixelsPerLine(PatW, BitCount, 4);
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, this->m_ImageBuffer, FdImageRect, PatStep, PatPtr, false) == false )
	{	return FALSE; }
	if ( NULL == PatPtr ) { return FALSE; }

	const int FdUniqueID = FdPtr->GetFdUniqueID();
	CString   FdFolder = ProjectPtr->GetProjectFdFolder();
	JetAPI::CreateFolder(FdFolder);
	CString FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);
	JetAPI::CreateFolder(FdModelFolder);
	
	TUNI_FRAME UniFrame;
	FRAME_TYPE FrameType = FrameParamPtr->FrameType;

	JetAPI::InitialUniFrame(UniFrame);
	UniFrame.ImageW = PatW;
	UniFrame.ImageH = PatH;
	UniFrame.ImageStep = PatStep;
	UniFrame.BitCount = PatBitCount;
	UniFrame.ImagePtr = PatPtr;	
	FdPtr->BuildNewFd(FdCadPos, FdStagePos2D, FdStageSize, FdRoiExtSize, FrameIndex, FrameUniqueID, FrameType, FdModelFolder, UniFrame);
	
#ifdef _DEBUG
	str.Format(_T("%s\\FdPat_%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdPtr->GetFdIndex_Panel()+1);
	ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, BitCount, PatPtr, true);
#endif
	if ( FdPtr->SetFdPattern(0, PatW, PatH, PatStep, BitCount, PatPtr, false) == false )
	{
		this->m_ErrorString = _T("Error, Fd SetFdPattern Fault");
		JetMemory.free_func(PatPtr);
		return FALSE;
	}
	JetMemory.free_func(PatPtr);
	
#ifdef _DEBUG
	FdPtr->GetFdPattern(0, PatW, PatH, PatStep, PatBitCount, PatPtr);
	str.Format(_T("%s\\FdPat2_%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdPtr->GetFdIndex_Panel()+1);
	ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, PatBitCount, PatPtr, true);
#endif

	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdAddBtn() 
{
	// TODO: Add your control notification handler code here
	CString    str;	
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	const DISTRICT_ID  DistrictID = GetDistrictID();
	if ( NULL == PanelPtr ) { return; }
	const unsigned int FdCountTotal = (unsigned int)(PanelPtr->GetPanelFdCount());
	const unsigned int FdCount = (unsigned int)(PanelPtr->GetPanelFdCount(DistrictID));
	if ( FdCount >= PANEL_MAX_FD_COUNT ) { return; }
	BOOL bUseComponentChk = CWnd::IsDlgButtonChecked(PAENFD_USE_COMPONENT_POS_CHK);	
	if ( TRUE == bUseComponentChk )
	{
		if ( false == m_GetComponentPos )
		{
			str = _T("Please select component first!");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return;
		}		
	}

	CAOIProject *ProjectPtr = m_ProjectPtr;
	CAOIFd *FdPtr = AOIObjManager.CreateFdObj();
	if ( NULL == FdPtr ) { return; }	
	int FdUniqueID = ProjectPtr->GetProjectFdMaxUniqueID();
	FdPtr->SetFdIndex_Panel(FdCountTotal);
	FdPtr->SetFdUniqueID(FdUniqueID);
	FdPtr->SetFdDistrictID(DistrictID);
	//FdPtr->SetFdUniqueID((int)(FdCount));
	if ( UpdateFdParamToObj(FdPtr) == FALSE )
	{
		JetAPI::ShowMessageBox(m_ErrorString);
		AOIObjManager.DestroyFdObj(FdPtr);
		return;
	}	
	m_FdIndex = FdCount;
	FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
	ProjectPtr->AddProjectFdPtr(FdPtr, false);
	PanelPtr->AddPanelFdPtr(FdPtr);	

	if ( TRUE == bUseComponentChk )
	{
		PanelPtr->CalcPanelMapParam(DistrictID);
		PanelPtr->CalcPanelStagePosition(DistrictID);
		PanelPtr->LayoutPanelBoardListRegion(DistrictID);
	}
	this->UpdateFdCount();
	this->UpdateFdUIRadio();
	this->RedrawWindow();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdDelBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = GetPanelFdPtr(m_FdIndex);
	if ( NULL == FdPtr ) { return; }
	
	PanelPtr->SelectPanelAllFds(false);
	FdPtr->SetFdSelected(true);	
	PanelPtr->RemovePanelFdSelected();
	ProjectPtr->DestroyProjectFdSelected();

	m_FdIndex = 0;
	UpdateFdCount();
	UpdateFdUIRadio();
	UpdateFdParamToUI(m_FdIndex);
	RedrawWindow();
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdRadio1() 
{
	// TODO: Add your control notification handler code here
	m_FdIndex = 0;
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = GetPanelFdPtr(m_FdIndex);
	if ( NULL == FdPtr ) { return; }

	const double StagePosX = FdPtr->GetFdStagePosX(); 
	const double StagePosY = FdPtr->GetFdStagePosY();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode);
	this->UpdateFdParamToUI(FdPtr);
	if ( MotionCtrlPtr->WaitForMotionStop() == false ) { return; }	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdRadio2() 
{
	// TODO: Add your control notification handler code here
	m_FdIndex = 1;
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = GetPanelFdPtr(m_FdIndex);
	if ( NULL == FdPtr ) { return; }

	const double StagePosX = FdPtr->GetFdStagePosX(); 
	const double StagePosY = FdPtr->GetFdStagePosY();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode);
	this->UpdateFdParamToUI(FdPtr);
	if ( MotionCtrlPtr->WaitForMotionStop() == false ) { return; }	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdRadio3() 
{
	// TODO: Add your control notification handler code here
	m_FdIndex = 2;
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = GetPanelFdPtr(m_FdIndex);
	if ( NULL == FdPtr ) { return; }

	const double StagePosX = FdPtr->GetFdStagePosX(); 
	const double StagePosY = FdPtr->GetFdStagePosY();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode);
	this->UpdateFdParamToUI(FdPtr);
	if ( MotionCtrlPtr->WaitForMotionStop() == false ) { return; }	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnFdRadio4() 
{
	// TODO: Add your control notification handler code here
	m_FdIndex = 3;
	CAOIPanel *PanelPtr = GetActivePanelPtr();
	if ( NULL == PanelPtr ) { return; }
	CAOIFd *FdPtr = GetPanelFdPtr(m_FdIndex);
	if ( NULL == FdPtr ) { return; }

	const double StagePosX = FdPtr->GetFdStagePosX(); 
	const double StagePosY = FdPtr->GetFdStagePosY();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY, OfflineMode);
	this->UpdateFdParamToUI(FdPtr);
	if ( MotionCtrlPtr->WaitForMotionStop() == false ) { return; }	
	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
}
//-------------------------------------------------------------------------------------//
BOOL CNewProjectPaneFiducial::RedrawProjectImageWnd()//重繪專案影像視窗
{
	PostParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, NULL);	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SendParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//發送訊息給父視窗
{
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, message, wParam, lParam);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnUpdateExtendPatternWEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnUpdateExtendPatternHEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ExecNextPane()
{	
	ExecSavePanelFd();	
	ExecCalcPanelBasePlane();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ExecPrevPane()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ExecFinishPane()
{	
	ExecSavePanelFd();
	ExecCalcPanelBasePlane();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ReInitialPane()
{
	m_ReCalcStagePos = false;
	CWnd::CheckDlgButton(PAENFD_FD_RADIO_1, TRUE);
	CWnd::CheckDlgButton(PAENFD_FD_RADIO_2, FALSE);
	CWnd::CheckDlgButton(PAENFD_FD_RADIO_3, FALSE);
	CWnd::CheckDlgButton(PAENFD_FD_RADIO_4, FALSE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CNewProjectPaneFiducial::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{	
	switch ( wParam )
	{
	case WPARAM_LBUTTON_DOWN:
		break;
	case WPARAM_LBUTTON_UP:
		if ( OnImageWndLButtonUp(lParam) == false )
		{	return false; }
		break;
	case WPARAM_LBUTTON_DBCLICK:
		break;
	case WPARAM_RBUTTON_DOWN:
		break;
	case WPARAM_RBUTTON_UP:
		break;
	case WPARAM_RBUTTON_DBCLICK:
		break;
	case WPARAM_MOUSE_MOVE:
		break;
	case WPARAM_MOUSE_WHEEL:
		break;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::OnImageWndLButtonUp(LPARAM lParam)
{
	BOOL bChk = CWnd::IsDlgButtonChecked(PAENFD_USE_COMPONENT_POS_CHK);
	if ( TRUE == bChk )
	{
		CAOIProject *ProjectPtr = GetActiveProjectPtr();
		if ( NULL == ProjectPtr ) { return false; }

		CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
		if ( NULL == ComponentPtr ) 
		{	return false;	}

		if ( false == m_GetComponentPos )
		{
			m_FdCadPos = ComponentPtr->GetComponentCadPos();
			m_FdStagePos = ComponentPtr->GetComponentStagePos();
			m_GetComponentPos = true;	
			m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);	 
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnSelectComponentBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bChk = CWnd::IsDlgButtonChecked(PAENFD_USE_COMPONENT_POS_CHK);
	if ( FALSE == bChk )	{	return; }
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return ; }	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SetProjectActiveComponentIndex(-1);
	m_GetComponentPos = false;
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_SELECT_COMPONENT);
	Invalidate();
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::UpdateFdCount()
{
	CString      str;
	CString      strFd;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return true; }
	DISTRICT_ID  DistrictID = GetDistrictID();	
	const size_t FdCount = PanelPtr->CalcPanelFdCount(DistrictID);
	strFd = _T("PAENFD_FD_LABEL");
	strFd = LoadMultiLanguageString(strFd, strFd);
	str.Format(_T("%s: %d"), strFd, FdCount);
	CWnd::SetDlgItemText(PAENFD_FD_LABEL, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ExecSavePanelFd()
{	
	CString      str;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return true; }
	DISTRICT_ID  DistrictID = GetDistrictID();	
	unsigned int PanelIndex = PanelPtr->GetPanelIndex_Project();	
	PanelPtr->AssignPanelMapParamToBoards(DistrictID);
	if ( 0 == PanelIndex )
	{
		//編程只以機台座標為主		
		double CadPosX=0, CadPosY=0;
		double StagePosX=0, StagePosY=0;
		CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad						
		ProjectPtr->MapProjectMapStageToCadPos(*STCPtr);
		ProjectPtr->MapProjectMapLocStageToCadPos(*STCPtr);
	}	
	CString OfflineFoler = ProjectPtr->GetProjectProgramOfflineFolder();
	str = AOIDataDefine.GetProjectOfflineFileName(OfflineFoler, DistrictID);	
	ProjectPtr->SaveProjectProgramOfflineFile(str);
	str = AOIDataDefine.GetProjectOfflineFdName(OfflineFoler, DistrictID);		
	ProjectPtr->SaveProjectOfflineFdFile(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ExecCalcPanelBasePlane()
{	
	CString      str;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return true; }
	DISTRICT_ID  DistrictID = GetDistrictID();	

	TSIZE2D   Res;	
	TPOINT3D  Pos;	
	SIZE      FdSize={0};
	RECT      FdRect={0};
	TSIZE2D   FdBodySize;
	size_t    i=0, j=0, k=0;	
	size_t    UniFrameCount=0;
	CAOIFd   *FdPtr = NULL;		
	CAOIWnd  *WndPtr=NULL;		
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;		
	const size_t FrameCount=FRAME_MAX_COUNT;
	const size_t FdCount = PanelPtr->GetPanelFdCount();	
	CString    Folder=AOIDataCollect.GetAOITempDirectory();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);	
	Res.cx = AOIDataCollect.GetCameraResolutionX(CameraID);
	Res.cy = AOIDataCollect.GetCameraResolutionY(CameraID);	

	TUNI_FRAME   UniFrame;
	TUNI_FRAME   UniFrameArray[FrameCount];	
	std::vector<TUNI_FRAME> UniFrameList;	
	const bool   bReverse = true;
	const bool   bEnhance = true;
	const bool   bSave3D  = false;
	const bool   bApppend = false;
	const double  SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = PanelPtr->GetPanelFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		Pos = FdPtr->GetFdStagePos();

		for ( j=0; j<FrameCount; j++ )
		{	JetAPI::InitialUniFrame(UniFrameArray[j]); }
		if ( ProjectPtr->FillCurrentProgramFrame(Pos, Res, ImageW, ImageH, UniFrameArray, FrameCount) == false ) 
		{	continue; }
		WndPtr = FdPtr->GetFdWndPtr();
		if ( NULL == WndPtr ) { continue; }

	#ifdef _DEBUG
		UniFrameList.clear();
		for ( j=0; j<FrameCount; j++ )
		{
			UniFrame = UniFrameArray[j];
			if ( NULL == UniFrame.ImagePtr) { continue; }
			UniFrameList.push_back(UniFrame);
		}
		str.Format(_T("%s\\Fd_%04d_FOV.PNG"), Folder, i+1);
		ImageAPI.SaveUniFrameImage(str, UniFrameList, bReverse, bEnhance, bSave3D, bApppend, SpaceRatio);
	#endif//_DEBUG

		UniFrameList.clear();
		for ( j=0; j<FrameCount; j++ )
		{
			UniFrame = UniFrameArray[j];
			if ( 0==UniFrame.ImageW || 0==UniFrame.ImageH || 0==UniFrame.ImageStep ) { continue; }
			if ( NULL==UniFrame.SpacePtr || NULL==UniFrame.MaskPtr ) 
			{	
				JetAPI::ClearUniFrame(UniFrameArray[j]);						
				continue; 
			}
			UniFrameList.push_back(UniFrame);			
			for ( k=j+1; k<FrameCount; k++ )
			{	JetAPI::ClearUniFrame(UniFrameArray[k]);	}
			break;
		}
		UniFrameCount = UniFrameList.size();
		if ( 0 == UniFrameCount ) { continue; }

		FdBodySize.cx = WndPtr->GetWndBox().GetBoxSizeX();
		FdBodySize.cy = WndPtr->GetWndBox().GetBoxSizeY();
		FdSize.cx = JetAPI::Floor(FdBodySize.cx/Res.cx);
		FdSize.cy = JetAPI::Floor(FdBodySize.cy/Res.cy);

		FdRect.left   = (ImageW-FdSize.cx)/2;
		FdRect.top    = (ImageH-FdSize.cy)/2;
		FdRect.right  = FdRect.left+FdSize.cx;
		FdRect.bottom = FdRect.top+FdSize.cy;
		WndPtr->SetWndImageRect(FdRect);
		FdPtr->CalcFdSpaceBasePlane(UniFrameList);
		FdPtr->SetFdResultID_AOI(RESULT_ID_OK);
		JetAPI::ClearUniFrameList(UniFrameList);
	}
	PanelPtr->CalcPanelBasePlaneParam(DistrictID);

	CString OfflineFoler = ProjectPtr->GetProjectProgramOfflineFolder();
	str = AOIDataDefine.GetProjectOfflineFdName(OfflineFoler, DistrictID);		
	ProjectPtr->SaveProjectOfflineFdFile(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CNewProjectPaneFiducial::ChangeDistrictID(DISTRICT_ID DistrictID)
{
	CString       str;
	CAOIProject  *ProjectPtr = GetActiveProjectPtr();	
	if ( NULL == ProjectPtr ) { return false; }
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const LANE_ID ActLaneID = AOIDataCollect.GetActiveLaneID();
	const DISTRICT_ID ActDistrictID = AOIDataCollect.GetActiveDistrictID();
	if ( ActDistrictID != DistrictID )
	{
		if ( AOIDataCollect.MovePCBToDistrictID(ActLaneID, DistrictID) == false )
		{
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;
		}
	}
	m_DistrictID = DistrictID;
	str = AOIDataDefine.GetDistrictIDText(DistrictID);
	CWnd::SetDlgItemText(PAENFD_DISTRICT_EDIT, str);
	m_ImageWnd.SetImageText(str, true);
	AOIDataCollect.SetActiveDistrictID(DistrictID);
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);

	CString OfflineFolder = ProjectPtr->GetProjectProgramOfflineFolder();
	CString OfflineFilename = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DistrictID);
	if ( true == OfflineMode ) 
	{
		if ( ActDistrictID != DistrictID )
		{	ProjectPtr->LoadProjectProgramOfflineFile(OfflineFilename);	 }
	}
	RedrawWnd();
	SendParentWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_PROJECT_MAP, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnDistrictABtn() 
{
	// TODO: Add your control notification handler code here
	return ;
	ChangeDistrictID(DISTRICT_ID_A);
}
//-------------------------------------------------------------------------------------//
void CNewProjectPaneFiducial::OnDistrictBBtn() 
{
	// TODO: Add your control notification handler code here
	return ;
	ChangeDistrictID(DISTRICT_ID_B);
}
//-------------------------------------------------------------------------------------//